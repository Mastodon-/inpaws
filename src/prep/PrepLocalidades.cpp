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

#include "prep/PrepLocalidades.h"
#include "prep/MsgPrep.h"
#include "UtilTexto.h"
#include "prep/Error.h"
#include "Textos.h"
#include "Conexiones.h"
#include "Vocabulario.h"
#include "MsgAplicacion.h"
#include "defines.h"

using namespace Inpaws;
using namespace std;

PrepLocalidades::PrepLocalidades(const PrepSubChar& prepSubChar)
  : PrepPart(RSV_LOCATIONS, 251), _prepSubChar(prepSubChar)
{
}

PrepLocalidades::~PrepLocalidades()
{
}

void
PrepLocalidades::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    // No llamar, llamar en su lugar a la que expande las abreviaturas
}

void
PrepLocalidades::generarCodigoPrep (std::ostream& os, const Aventura& aventura,
                                const PrepAbreviable::TDiccionario& abreviaturas) const
{
    UtilTexto utilTexto;
    // Obtenemos punteros a Localidades, conexiones y vocabulario
    const Textos* localidades = dynamic_cast<const Textos*>(aventura.getModulo(RSV_LOCATIONS));
    const Conexiones* conexiones = dynamic_cast<const Conexiones*>(aventura.getModulo(RSV_CONNECTIONS));
    const Vocabulario* vocabulario = dynamic_cast<const Vocabulario*>(aventura.getModulo(RSV_VOCABULARY));

    for (unsigned int i=0; i < localidades->size(); i++)
    {
        os << RSV_LOCATION << " " << i << endl << "{" << endl;
        os << "\t\"";
        string desc = localidades->obtenerTexto(i);
        utilTexto.expandirAbreviaturas(desc, abreviaturas);
        PrepTokeniser::imprimirLiteral(desc, os);
        os << "\";" << endl;
        const Conexiones::Conexion* conex = conexiones->leerConexionInit(i);
        if (conex) os << "\t" << RSV_CONNECTIONS << " { ";
        while (conex)
        {
            const Vocabulario::Palabra* palTmp = vocabulario->leerPalabraInit(conex->palabra, Vocabulario::VERB);
            if (!palTmp && conex->palabra < 14)
                palTmp = vocabulario->leerPalabraInit(conex->palabra, Vocabulario::NOUN);

            os << palTmp->palabraCompleta
               << " TO " << (unsigned int)conex->destino << " ";
            conex = conexiones->leerConexionNext(i);
            if (!conex) os << "};" << endl;
        }

        os << "}" << endl << endl;
    }
}

void
PrepLocalidades::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    // Formatos permitidos
    //    LOCATION nombre {... }
    //    LOCATION número { ... }
    //    LOCATION nombre número {...}


    Localidad nuevaLocalidad;
    PrepTokeniser::TTokenType token;
    UtilTexto utilTexto;

    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, RSV_LOCATION);

    // Código / nombre de la localidad
    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_IDENTIFICADOR) // Se define un nombre para la loc.
    {
        if (diccionario.buscarEntrada(tokeniser.lvalue().literal, LOCNO) != -2 ||
            diccionario.buscarEntrada(tokeniser.lvalue().literal, VALOR) != -2)
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), (string)RSV_LOCATION+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
        }

        nuevaLocalidad.nombre = tokeniser.lvalue().literal;

        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_NUMERO)
        {
            if (disponibleValor(tokeniser.lvalue().valor))
            {
               diccionario.anadirEntrada(nuevaLocalidad.nombre, LOCNO, tokeniser.lvalue().valor);
                // Y añadimos al pool de asignadas para que no se vuelva a utilizar
                anadirPool (tokeniser.lvalue().valor);
            } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), (string)RSV_LOCATION+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
        }
        else
        {
            tokeniser.putback();
            // Añadimos al diccionario la nueva entrada sin asignar
            diccionario.anadirEntrada(nuevaLocalidad.nombre, LOCNO);
        }
    }
    else if (token == PrepTokeniser::TK_NUMERO)
    {
        // Si ya existe, error
        if (diccionario.buscarEntrada(tokeniser.lvalue().literal, LOCNO) != -2)
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), (string)RSV_LOCATION+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
        }
        // Si es mayor de 251 (máximo para localidades), error
        if (tokeniser.lvalue().valor > 251)
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSGPREP_LOCRESERV, Error::FATAL);
        }
        // Añadimos al diccionario el número de localidad, ya asignado
        diccionario.anadirEntrada(tokeniser.lvalue().literal, LOCNO, tokeniser.lvalue().valor);

        // Y añadimos al pool de asignadas para que no se vuelva a utilizar
        anadirPool (tokeniser.lvalue().valor);

        nuevaLocalidad.nombre = tokeniser.lvalue().literal;
    }
    else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGONOMBRELOC), Error::FATAL);

    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");

    // Descripcion de la localidad
    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_LITERAL)
    {
        nuevaLocalidad.desc = tokeniser.lvalue().literal;
    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_DESCLOCALIDAD), Error::FATAL);

    if (!utilTexto.textoCorrecto(nuevaLocalidad.desc))
    {
        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[" + nombre() + "] " + utilTexto.errorStr(), Error::NORMAL);
    }

    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");

    // CONEXIONES
    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_IDENTIFICADOR && UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_CONNECTIONS)
    {
        saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");
        _leerConexiones(tokeniser, nuevaLocalidad);
        saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "}");
        saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");
    } else tokeniser.putback();
    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "}");
    _localidades.push_back(nuevaLocalidad);
}

void
PrepLocalidades::asignarValoresSimbolos(Diccionario& diccionario)
{
    // Comprobar que haya al menos una localidad
    if (!_localidades.size())
    {
        ERRORPAW("", 0, MSGPREP_ALMENOSUNALOC, Error::NORMAL);
    }

    for (std::vector<Localidad>::const_iterator it = _localidades.begin();
         it != _localidades.end();
         it++)
    {
        if (diccionario.buscarEntrada((*it).nombre, LOCNO) == -1)
        {
            diccionario.fijarValor((*it).nombre, LOCNO, dameValorPool());
        }
    }
    if (!poolDefinicionesOk()) // Comprueba que todas las definiciones son correlativas
    {
        ERRORPAW("", 0, nombre() + ": "+MSGPREP_DEFCORR, Error::FATAL);
    }
}

void
PrepLocalidades::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{

    UtilTexto utilTexto;
    // Generamos localidades y conexiones

    // Localidades
    os << RSV_LOCATIONS << endl << "{" << endl;
    for (std::vector<Localidad>::const_iterator it = _localidades.begin();
         it != _localidades.end();
         it++)
    {
        os << '\t';
        os << diccionario.buscarEntrada((*it).nombre, LOCNO) << ": ";
        os << "\"";
        PrepTokeniser::imprimirLiteral((*it).desc, os);
        os << "\";" << endl;
    }
    os << "}" << endl << endl;

    // Conexiones
    os << RSV_CONNECTIONS << endl << "{" << endl;
    for (std::vector<Localidad>::const_iterator it = _localidades.begin();
         it != _localidades.end();
         it++)
    {
        for (std::vector<Conexion>::const_iterator itCon = (*it).conexiones.begin();
             itCon != (*it).conexiones.end();
             itCon++)
        {
            int direccion = diccionario.buscarEntrada((*itCon).direccion, WORD);
            int destino = diccionario.buscarEntrada((*itCon).destino, LOCNO);

            if (direccion == -2)
              ERRORPAW((*itCon).archivolinea, (*itCon).linea, "["+(*itCon).direccion + "] " + MSGPREP_PALABRANODEF, Error::NORMAL);
            if (destino == -2)
              ERRORPAW((*itCon).archivolinea, (*itCon).linea, "["+(*itCon).destino + "] " + MSGPREP_LOCNODEF, Error::NORMAL);
            os << '\t';
            os << diccionario.buscarEntrada((*it).nombre, LOCNO) << ": ";
            os << direccion << " TO " << destino << ";" << endl;
        }
    }
    os << "}" << endl << endl;
}

PrepAbreviable::TListaCadenas
PrepLocalidades::leerCadenas() const
{
    PrepAbreviable::TListaCadenas localidades;
    for (std::vector<Localidad>::const_iterator it = _localidades.begin();
         it != _localidades.end();
         it++)
    {
        PrepAbreviable::anadirCadena(localidades, (*it).desc);
    }
    return localidades;
}

SpWord
PrepLocalidades::sustituirCadenas (const PrepAbreviable::TDiccionario& diccionario)
{
    SpWord ahorro = 0;
    UtilTexto utilTexto;
    for (std::vector<Localidad>::iterator it = _localidades.begin();
         it != _localidades.end();
         it++)
    {
        size_t longAntes = utilTexto.compactarTexto((*it).desc).length();
        (*it).desc = PrepAbreviable::sustituirCadena(diccionario, (*it).desc);
        size_t longDespues = utilTexto.compactarTexto((*it).desc).length();
        ahorro += longAntes - longDespues;
    }
    return ahorro;
}

void
PrepLocalidades::_leerConexiones (PrepTokeniser& tokeniser, Localidad& localidad)
{
    PrepTokeniser::TTokenType token;
    token = tokeniser.leeToken();
    while (!(token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
        Conexion nuevaConexion;
        // literal de palabra (puede ser un número, el PAW lo permite)
        if (token == PrepTokeniser::TK_IDENTIFICADOR || token == PrepTokeniser::TK_NUMERO
            || token == PrepTokeniser::TK_NUMEROGRANDE || token == PrepTokeniser::TK_NUMEROVOCABULARIO)
        {
           nuevaConexion.linea = tokeniser.lineaActual();
           nuevaConexion.archivolinea = tokeniser.archivoActual();
           nuevaConexion.direccion = tokeniser.lvalue().literal;
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGOPALABRA), Error::FATAL);

        // Literal "TO"
        saltarToken(tokeniser, PrepTokeniser::TK_IDENTIFICADOR, "TO");

        // Localidad de destino (codigo o nombre)
        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_IDENTIFICADOR || token == PrepTokeniser::TK_NUMERO)
        {
           nuevaConexion.destino = tokeniser.lvalue().literal;
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGONOMBRELOC), Error::FATAL);
        localidad.conexiones.push_back(nuevaConexion);
        token = tokeniser.leeToken();
    }
    tokeniser.putback(); // devolvemos el "}"
}

void
PrepLocalidades::sustituirCaracteresEspeciales()
{
    UtilTexto utilTexto;
    for (std::vector<Localidad>::iterator it = _localidades.begin();
         it != _localidades.end();
         it++)
    {
        string descripcionSub = (*it).desc;
        utilTexto.sustituirCaracteres(descripcionSub, _prepSubChar.sustituciones());
        (*it).desc = descripcionSub;
    }
}
