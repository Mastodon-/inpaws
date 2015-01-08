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

#include "Defaults.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"

using namespace Inpaws;
using namespace std;

Defaults::Defaults()
  : PawPart(0), _charSet(0), _ink(7), _paper(0), _flash(0),
    _bright(0), _inverse(0), _over(0), _border(0)
{
}

Defaults::~Defaults()
{
}


PtrBuffer
Defaults::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    // el parámetro ptrInicioDatos se ignora...

    PtrBuffer mainTop = SPBUFFER->wordPeek(PTR_MAINTOP);
    PtrBuffer mainAttr = mainTop + OFF_MAINATTR;

    SPBUFFER->bytePoke(mainTop + OFF_DEF_CHARSET, _charSet);
    SPBUFFER->bytePoke(mainAttr + OFF_DEF_INK, _ink);
    SPBUFFER->bytePoke(mainAttr + OFF_DEF_PAPER, _paper);
    SPBUFFER->bytePoke(mainAttr + OFF_DEF_FLASH, _flash);
    SPBUFFER->bytePoke(mainAttr + OFF_DEF_BRIGHT, _bright);
    SPBUFFER->bytePoke(mainAttr + OFF_DEF_INVERSE, _inverse);
    SPBUFFER->bytePoke(mainAttr + OFF_DEF_OVER, _over);
    SPBUFFER->bytePoke(mainAttr + OFF_DEF_BORDER, _border);

    // Estas posiciones de memoria contienen siempre estos valores
    // Desconozco la razón, pero lo haré igual
    SPBUFFER->bytePoke(mainAttr + 2, 17);
    SPBUFFER->bytePoke(mainAttr + 4, 18);
    SPBUFFER->bytePoke(mainAttr + 6, 19);
    SPBUFFER->bytePoke(mainAttr + 8, 20);
    SPBUFFER->bytePoke(mainAttr + 10, 21);


    return mainAttr + OFF_DEF_BORDER;
}

PtrBuffer
Defaults::volcarDeMemoria ()
{
    PtrBuffer mainTop = SPBUFFER->wordPeek(PTR_MAINTOP);
    PtrBuffer mainAttr = mainTop + OFF_MAINATTR;

    _charSet = SPBUFFER->bytePeek(mainTop + OFF_DEF_CHARSET);
    _ink = SPBUFFER->bytePeek(mainAttr + OFF_DEF_INK);
    _paper = SPBUFFER->bytePeek(mainAttr + OFF_DEF_PAPER);
    _flash = SPBUFFER->bytePeek(mainAttr + OFF_DEF_FLASH);
    _bright = SPBUFFER->bytePeek(mainAttr + OFF_DEF_BRIGHT);
    _inverse = SPBUFFER->bytePeek(mainAttr + OFF_DEF_INVERSE);
    _over = SPBUFFER->bytePeek(mainAttr + OFF_DEF_OVER);
    _border = SPBUFFER->bytePeek(mainAttr + OFF_DEF_BORDER);

    return mainAttr + OFF_DEF_BORDER;
}

void
Defaults::generarCodigo (std::ostream& os)
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
Defaults::leerDeCodigo (Tokeniser& tokeniser)
{
    Tokeniser::TTokenType token;
    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, RSV_DEFAULTS);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");

    do
    {
        token = tokeniser.leeToken();
        if (token == Tokeniser::TK_IDENTIFICADOR)
        {
            string item = tokeniser.lvalue().literal;
            saltarToken(tokeniser, Tokeniser::TK_CONTROL, ":");
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_NUMERO)
            {
                _asignarItem (item, tokeniser.lvalue().valor);
            }
            saltarToken(tokeniser, Tokeniser::TK_CONTROL, ";");
        }
        else if (!(token==Tokeniser::TK_CONTROL && tokeniser.lvalue().literal=="}"))
        {
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_ATRIBUTO_CHARSET));
        }
    } while (!(token==Tokeniser::TK_CONTROL && tokeniser.lvalue().literal=="}"));
}

void
Defaults::_asignarItem(const std::string& item, SpByte valor)
{
    if (item == RSV_CHARSET)
    {
        _charSet = valor;
    }
    else if (item == RSV_INK)
    {
        if (valor <= 9)
            _ink = valor;
        else
            throw ExcTokeniser(MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-9)");
    }
    else if (item == RSV_PAPER)
    {
        if (valor <= 9)
            _paper = valor;
        else
            throw ExcTokeniser(MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-9)");
    }
    else if (item == RSV_FLASH)
    {
        if (valor <= 1)
            _flash = valor;
        else
            throw ExcTokeniser(MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)");
    }
    else if (item == RSV_BRIGHT)
    {
        if (valor <= 1)
            _bright = valor;
        else
            throw ExcTokeniser(MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)");
    }
    else if (item == RSV_INVERSE)
    {
        if (valor <= 1)
            _inverse = valor;
        else
            throw ExcTokeniser(MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)");
    }
    else if (item == RSV_OVER)
    {
        if (valor <= 1)
            _over = valor;
        else
            throw ExcTokeniser(MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-1)");
    }
    else if (item == RSV_BORDER)
    {
        if (valor <= 7)
            _border = valor;
        else
            throw ExcTokeniser(MSG_ATRIBUTO_MUY_GRANDE + (string)" (0-7)");
    }
    else throw ExcTokeniser(MSG_ATRIBUTO_CHARSET + (string)" " +
                             MSG_PALAB_RESERV_NO_ENCONTRADA +
                             item);
}
