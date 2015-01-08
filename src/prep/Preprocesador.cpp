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

#include "prep/Preprocesador.h"
#include "prep/MsgPrep.h"
#include "prep/Error.h"
#include "MsgAplicacion.h"
#include "UtilTexto.h"
#include <cstdlib>


using namespace Inpaws;
using namespace std;

Preprocesador::Preprocesador(ifstream* is, const std::string& nombreFichero, const uint16_t formatoSalida)
  : _tokeniser(is, nombreFichero), _localidades(_subchar),
    _vocabulario(((formatoSalida&VERGLULX)?Vocabulario::superglus_word_size:Vocabulario::paw_word_size)),
    _objetos(_subchar, _vocabulario), _mensajes(RSV_MESSAGES, _subchar), _sismensajes(RSV_SYSMESSAGES, _subchar),
    _graficos(RSV_GRAPHICS), _caracteres(RSV_CHARACTERS),
    _procesos(_mensajes, _sismensajes, _vocabulario),
    _ahorroCompresion(0), _formatoSalida(formatoSalida)
{
    _abreviaturas.anadirAbreviable(&_localidades);
    _abreviaturas.anadirAbreviable(&_mensajes);
    _abreviaturas.anadirAbreviable(&_sismensajes);
    _abreviaturas.anadirAbreviable(&_objetos);

    // Incluimos los símbolos del hardware para el que vamos a compilar
    if (_formatoSalida & VERPC)
        _tokeniser.insertarSimbolo("PAWPC");
    else if (_formatoSalida & VERCPM)
        _tokeniser.insertarSimbolo("PAWCPM");
    else if (_formatoSalida & VERGLULX)
        _tokeniser.insertarSimbolo("PAWGLULX");
    else
        _tokeniser.insertarSimbolo("PAWSPECTRUM");
}

Preprocesador::~Preprocesador()
{
}

void
Preprocesador::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    PrepAbreviaturas abrev;
    abrev.leerDeAventura(aventura);

    _defaults.generarCodigoPrep (os, aventura);
    _localidades.generarCodigoPrep(os, aventura, abrev.abreviaturas());
    _vocabulario.generarCodigoPrep(os, aventura);
    _objetos.generarCodigoPrep(os, aventura);
    _mensajes.generarCodigoPrep(os, aventura, abrev.abreviaturas());
    _sismensajes.generarCodigoPrep(os, aventura, abrev.abreviaturas());
    _graficos.generarCodigoPrep(os, aventura);
    _caracteres.generarCodigoPrep(os, aventura);
    _procesos.generarCodigoPrep(os, aventura);
    _flags.generarCodigoPrep(os, aventura);
    _constantes.generarCodigoPrep(os, aventura);
}

void
Preprocesador::generarCodigoGraficos (std::ostream& os, const Aventura& aventura) const
{
    _graficos.generarCodigoPrep(os, aventura);
}

void
Preprocesador::generarCodigoCaracteres (std::ostream& os, const Aventura& aventura) const
{
    _caracteres.generarCodigoPrep(os, aventura);
}

void
Preprocesador::preprocesar(ostream& os)
{
    leerDeCodigo();
    asignarValoresSimbolos();

    if (!Error::error()->empty())
    {
        Error::error()->volcarErrores(cerr);
        exit(1);
    }

    generarCodigo(os);

    if (!Error::error()->empty())
    {
        Error::error()->volcarErrores(cerr);
        exit(1);
    }
}

void
Preprocesador::leerDeCodigo()
{
    PrepTokeniser::TTokenType token;

    token = _tokeniser.leeToken();
    while (token != PrepTokeniser::TK_NULL)
    {
        if (token == PrepTokeniser::TK_IDENTIFICADOR)
        {
            if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_LOCATION)
            {
                _tokeniser.putback();
                _localidades.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_VOCABULARY)
            {
                _tokeniser.putback();
                _vocabulario.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_OBJECT)
            {
                _tokeniser.putback();
                _objetos.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_MESSAGES)
            {
                _tokeniser.putback();
                _mensajes.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_SYSMESSAGES)
            {
                _tokeniser.putback();
                _sismensajes.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_GRAPHICS)
            {
                _tokeniser.putback();
                _graficos.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_CHARACTERS)
            {
                _tokeniser.putback();
                _caracteres.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_DEFAULTS)
            {
                _tokeniser.putback();
                _defaults.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_RESPONSE ||
                     UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_PROCESS)
            {
                _tokeniser.putback();
                _procesos.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_FLAG)
            {
                _tokeniser.putback();
                _flags.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_SUBCHAR)
            {
                _tokeniser.putback();
                _subchar.leerDeCodigo(_tokeniser, _diccionario);
            }
            else if (UtilTexto::toUpr(_tokeniser.lvalue().literal) == RSV_CONSTANT)
            {
                _tokeniser.putback();
                _constantes.leerDeCodigo(_tokeniser, _diccionario);
            }
            else
                ERRORPAW(_tokeniser.archivoActual(), _tokeniser.lineaActual(), _tokeniser.errorLeidoEsperado(MSGPREP_IDEFPAW), Error::FATAL);
        }
        else if (!(token == Tokeniser::TK_CONTROL && _tokeniser.lvalue().literal == ";")) // Se ignoran los ; aislados
        {
            ERRORPAW(_tokeniser.archivoActual(), _tokeniser.lineaActual(), _tokeniser.errorLeidoEsperado(MSGPREP_IDEFPAW), Error::FATAL);
        }
        token = _tokeniser.leeToken();
    }
}

void
Preprocesador::asignarValoresSimbolos()
{

    // Ahora asignamos valor a todos los símbolos usados en el código
    _localidades.asignarValoresSimbolos(_diccionario);
    _vocabulario.asignarValoresSimbolos(_diccionario);
    _objetos.asignarValoresSimbolos(_diccionario);
    _mensajes.asignarValoresSimbolos(_diccionario);
    _sismensajes.asignarValoresSimbolos(_diccionario);
    _graficos.asignarValoresSimbolos(_diccionario);
    _caracteres.asignarValoresSimbolos(_diccionario);
    _defaults.asignarValoresSimbolos(_diccionario);
    _procesos.asignarValoresSimbolos(_diccionario);
    _flags.asignarValoresSimbolos(_diccionario);
    _subchar.asignarValoresSimbolos(_diccionario);
    _constantes.asignarValoresSimbolos(_diccionario);

    // Sustitución de caracteres acentuados, eñes, etc por sus códigos
    _localidades.sustituirCaracteresEspeciales();
    _mensajes.sustituirCaracteresEspeciales();
    _sismensajes.sustituirCaracteresEspeciales();
    _objetos.sustituirCaracteresEspeciales();

    // Calculamos las mejores abreviaturas (si no hemos anulado la compresión)
    if (!(_formatoSalida & NOCOMPRIMIR))
        _ahorroCompresion = _abreviaturas.calcularAbreviaturas();
}

void
Preprocesador::generarCodigo(std::ostream& os) const
{
    // Debe estar definido el bloque CHARACTERES en la versión de Spectrum, para poder continuar
    if (_caracteres.size() == 0 && !(_formatoSalida&(VERPC|VERCPM|VERGLULX)))
    {
        ERRORPAW("",0, MSGPREP_NOCHARACTERS, Error::FATAL);
    }

    // Para la versión de disco (CPC/PC) debe haber un mínimo de 61 mensajes del sistema (0-60)
    if ((_formatoSalida & (VERCPM|VERPC|VERGLULX)) && _sismensajes.size() < 61)
    {
        ERRORPAW("",0, MSGPREP_SYSMESCPC, Error::FATAL);
    }

    _defaults.generarCodigo (os, _diccionario);
    _localidades.generarCodigo(os, _diccionario);
    _vocabulario.generarCodigo(os, _diccionario);
    _objetos.generarCodigo(os, _diccionario);
    _mensajes.generarCodigo(os, _diccionario);
    _sismensajes.generarCodigo(os, _diccionario);
    _graficos.generarCodigo(os, _diccionario);
    _caracteres.generarCodigo(os, _diccionario);
    _procesos.generarCodigo(os, _diccionario);
    _flags.generarCodigo(os, _diccionario);
    _subchar.generarCodigo(os, _diccionario);
    _constantes.generarCodigo(os, _diccionario);
    _abreviaturas.generarCodigo(os);
}
