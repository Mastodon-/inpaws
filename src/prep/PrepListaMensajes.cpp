/*
 *   Copyright 2009 Francisco Javier Lopez
 *   e-mail: lane.mastodon@gmail.com
 *
 *   This file is part of InPAWS.
 *
 *   InPAWS is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   InPAWS is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with InPAWS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "prep/PrepListaMensajes.h"
#include "prep/MsgPrep.h"
#include "MsgAplicacion.h"
#include "Textos.h"
#include "defines.h"
#include "prep/Error.h"
#include "UtilTexto.h"

using namespace Inpaws;
using namespace std;

PrepListaMensajes::PrepListaMensajes(const std::string& nombre, const PrepSubChar& prepSubChar)
  : PrepPart(nombre, 254), _prepSubChar(prepSubChar)
{
}

PrepListaMensajes::~PrepListaMensajes()
{
}

void
PrepListaMensajes::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    // No utilizar, utilizar en su lugar la que expande abreviaturas
}

void
PrepListaMensajes::generarCodigoPrep (std::ostream& os, const Aventura& aventura,
                        const PrepAbreviable::TDiccionario& abreviaturas) const
{
    UtilTexto utilTexto;
    const Textos* msg = dynamic_cast<const Textos*>(aventura.getModulo(nombre()));

    os << nombre() << endl << "{" << endl;
    for (unsigned int i=0; i < msg->size(); i++)
    {
        os << '\t' << i << ": \"";
        string desc = msg->obtenerTexto(i);
        utilTexto.expandirAbreviaturas(desc, abreviaturas);
        PrepTokeniser::imprimirLiteral(desc, os);
        os << "\";" << endl;
    }
    os << "}" << endl << endl;
}

void
PrepListaMensajes::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    PrepTokeniser::TTokenType token;
    Mensaje nuevoMensaje;
    UtilTexto utilTexto;

    int tipo = (UtilTexto::toUpr(nombre()) == RSV_MESSAGES ? MESNO : SYSNO);

    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, nombre());
    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");
    token = tokeniser.leeToken();
    while (!(token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
        if (token == PrepTokeniser::TK_IDENTIFICADOR) // Nombre del mensaje
        {
            if (diccionario.buscarEntrada(tokeniser.lvalue().literal, tipo) != -2)
            {
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), nombre()+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
            }
            nuevoMensaje.nombre = tokeniser.lvalue().literal;
            diccionario.anadirEntrada(tokeniser.lvalue().literal, tipo);
            nuevoMensaje.nombre = tokeniser.lvalue().literal;
        }
        else if (token == PrepTokeniser::TK_NUMERO) // Mensaje por número
        {
            if (diccionario.buscarEntrada(tokeniser.lvalue().literal, tipo) != -2)
            {
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), nombre()+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
            }
            nuevoMensaje.nombre = tokeniser.lvalue().literal;
            diccionario.anadirEntrada(tokeniser.lvalue().literal, tipo, tokeniser.lvalue().valor);
            anadirPool (tokeniser.lvalue().valor);
        }
        else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGOMES), Error::FATAL);

        saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ":");
        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_LITERAL)
        {
            nuevoMensaje.desc = tokeniser.lvalue().literal;
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_TEXTOMENSAJE), Error::FATAL);

        if (!utilTexto.textoCorrecto(nuevoMensaje.desc))
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[" + nombre() + "] " + utilTexto.errorStr(), Error::NORMAL);
        }

        saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");

        _mensajes.push_back(nuevoMensaje);

        token = tokeniser.leeToken();
    }
}

void
PrepListaMensajes::asignarValoresSimbolos(Diccionario& diccionario)
{
    int tipo = (UtilTexto::toUpr(nombre()) == RSV_MESSAGES ? MESNO : SYSNO);

    if (tipo == MESNO && _mensajes.size() < 1)
    {
        ERRORPAW("", 0, MSGPREP_ALMENOSUNMSG, Error::NORMAL);
    }
    else if (tipo == SYSNO && _mensajes.size() < 54)
    {
        ERRORPAW("", 0, MSGPREP_SISMESRESERV, Error::NORMAL);
    }
    for (std::vector<Mensaje>::const_iterator it = _mensajes.begin();
         it != _mensajes.end();
         it++)
    {
        if (diccionario.buscarEntrada((*it).nombre, tipo) == -1)
        {
            diccionario.fijarValor((*it).nombre, tipo, dameValorPool());
        }
    }
    if (!poolDefinicionesOk()) // Comprueba que todas las definiciones son correlativas
    {
        ERRORPAW("", 0, nombre() + ": "+MSGPREP_DEFCORR, Error::FATAL);
    }
}

void
PrepListaMensajes::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    UtilTexto utilTexto;
    int tipo = (UtilTexto::toUpr(nombre()) == RSV_MESSAGES ? MESNO : SYSNO);

    os << nombre() << endl << "{" << endl;
    for (std::vector<Mensaje>::const_iterator it = _mensajes.begin();
         it != _mensajes.end();
         it++)
    {
        os << '\t' << diccionario.buscarEntrada((*it).nombre, tipo) << ": "
           << "\"";
        PrepTokeniser::imprimirLiteral((*it).desc, os);
        os << "\";" << endl;
    }
    os << "}" << endl << endl;
}

void
PrepListaMensajes::anadirMensaje(const std::string& nombre, const std::string& desc, Diccionario& diccionario)
{
    int tipo = (UtilTexto::toUpr(this->nombre()) == RSV_MESSAGES ? MESNO : SYSNO);
    if (diccionario.buscarEntrada(nombre, tipo) != -2)
    {
        ERRORPAW("",0, this->nombre()+" ["+nombre+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
    }
    Mensaje nuevoMensaje;
    nuevoMensaje.nombre = nombre;
    nuevoMensaje.desc = desc;
    diccionario.anadirEntrada(nombre, tipo);
    _mensajes.push_back(nuevoMensaje);
}

void
PrepListaMensajes::sustituirCaracteresEspeciales()
{
    UtilTexto utilTexto;
    for (std::vector<Mensaje>::iterator it = _mensajes.begin();
         it != _mensajes.end();
         it++)
    {
        string descripcionSub = (*it).desc;
        utilTexto.sustituirCaracteres(descripcionSub, _prepSubChar.sustituciones());
        (*it).desc = descripcionSub;
    }
}

PrepAbreviable::TListaCadenas
PrepListaMensajes::leerCadenas() const
{
    PrepAbreviable::TListaCadenas mensajes;
    for (std::vector<Mensaje>::const_iterator it = _mensajes.begin();
         it != _mensajes.end();
         it++)
    {
        PrepAbreviable::anadirCadena(mensajes, (*it).desc);
    }
    return mensajes;
}

SpWord
PrepListaMensajes::sustituirCadenas (const PrepAbreviable::TDiccionario& diccionario)
{
    UtilTexto utilTexto;
    SpWord ahorro = 0;
    string tmp;
    for (std::vector<Mensaje>::iterator it = _mensajes.begin();
         it != _mensajes.end();
         it++)
    {
        size_t longAntes = utilTexto.compactarTexto((*it).desc).length();
        (*it).desc = PrepAbreviable::sustituirCadena(diccionario, (*it).desc);
        size_t longDespues = utilTexto.compactarTexto((*it).desc).length();
        ahorro += longAntes - longDespues;
    }
    return ahorro;
}

