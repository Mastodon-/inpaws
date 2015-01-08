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

#include "prep/PrepDefaults.h"
#include "prep/Error.h"
#include "MsgAplicacion.h"
#include "UtilTexto.h"

using namespace Inpaws;
using namespace std;

PrepDefaults::PrepDefaults()
: PrepPart(RSV_DEFAULTS), _charSet(0), _ink(7), _paper(0), _flash(0),
  _bright(0), _inverse(0), _over(0), _border(0)
{
}

PrepDefaults::~PrepDefaults()
{
}

void
PrepDefaults::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    PawPart* defaults = (PawPart*)aventura.getModulo(RSV_DEFAULTS);
    defaults->generarCodigo(os);
    os << endl;
}

void
PrepDefaults::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    // Copiado de la clase Defaults, cambiando excepciones por errores
    PrepTokeniser::TTokenType token;
    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, RSV_DEFAULTS);
    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");

    do
    {
        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_IDENTIFICADOR)
        {
            string item = tokeniser.lvalue().literal;
            saltarToken(tokeniser, PrepTokeniser::TK_CONTROL, ":");
            token = tokeniser.leeToken();
            if (token == PrepTokeniser::TK_NUMERO)
            {
                _asignarItem (item, tokeniser);
            }
            saltarToken(tokeniser, PrepTokeniser::TK_CONTROL, ";");
        }
        else if (!(token==PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal=="}"))
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(),tokeniser.errorLeidoEsperado(MSG_ATRIBUTO_CHARSET), Error::FATAL);
        }
    } while (!(token==PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal=="}"));
}

void
PrepDefaults::asignarValoresSimbolos(Diccionario& diccionario)
{
    // No hay símbolos
}

void
PrepDefaults::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    os << RSV_DEFAULTS << endl << "{" << endl;
    os << '\t' << RSV_CHARSET << ": " << (unsigned int)_charSet << ";" << endl;
    os << '\t' << RSV_INK << ": " << (unsigned int)_ink << ";" << endl;
    os << '\t' << RSV_PAPER << ": " << (unsigned int)_paper << ";" << endl;
    os << '\t' << RSV_FLASH << ": " << (unsigned int)_flash << ";" << endl;
    os << '\t' << RSV_BRIGHT << ": " << (unsigned int)_bright << ";" << endl;
    os << '\t' << RSV_INVERSE << ": " << (unsigned int)_inverse << ";" << endl;
    os << '\t' << RSV_OVER << ": " << (unsigned int)_over << ";" << endl;
    os << '\t' << RSV_BORDER << ": " << (unsigned int)_border << ";" << endl;

    os << "}" << endl;
}

void
PrepDefaults::_asignarItem(const std::string& item, const PrepTokeniser& tokeniser)
{
    SpByte valor = tokeniser.lvalue().valor;

    if (UtilTexto::toUpr(item) == RSV_CHARSET)
    {
        _charSet = valor;
    }
    else if (UtilTexto::toUpr(item) == RSV_INK)
    {
        if (valor <= 9)
            _ink = valor;
        else
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-9)", Error::FATAL);
    }
    else if (UtilTexto::toUpr(item) == RSV_PAPER)
    {
        if (valor <= 9)
            _paper = valor;
        else
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-9)", Error::FATAL);
    }
    else if (UtilTexto::toUpr(item) == RSV_FLASH)
    {
        if (valor <= 1)
            _flash = valor;
        else
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)", Error::FATAL);
    }
    else if (UtilTexto::toUpr(item) == RSV_BRIGHT)
    {
        if (valor <= 1)
            _bright = valor;
        else
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)", Error::FATAL);
    }
    else if (UtilTexto::toUpr(item) == RSV_INVERSE)
    {
        if (valor <= 1)
            _inverse = valor;
        else
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)", Error::FATAL);
    }
    else if (UtilTexto::toUpr(item) == RSV_OVER)
    {
        if (valor <= 1)
            _over = valor;
        else
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)", Error::FATAL);
    }
    else if (UtilTexto::toUpr(item) == RSV_BORDER)
    {
        if (valor <= 7)
            _border = valor;
        else
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-7)", Error::FATAL);
    }
    else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSG_ATRIBUTO_CHARSET + (string)" " +
                             MSG_PALAB_RESERV_NO_ENCONTRADA +
                             item,Error::FATAL);
}
