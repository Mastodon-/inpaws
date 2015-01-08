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

#include "Caracteres.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include <iomanip>

using namespace Inpaws;
using namespace std;

Caracteres::Caracteres()
: PawPart(0), _numFonts(0), _mainTop(0)
{
}

Caracteres::~Caracteres()
{

}

void
Caracteres::getInicioDatos()
{
    _mainTop = SPBUFFER->wordPeek(PTR_MAINTOP);
    _ptrIndice = _mainTop+OFF_FONTS;
    _ptrDatos = SPBUFFER->wordPeek(_ptrIndice);
    _numFonts = SPBUFFER->bytePeek(_mainTop + OFF_NUMFONTS);
}

PtrBuffer
Caracteres::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    // En el caso de una aventura generada desde código fuente
    // no sabemos a priori donde estará el maintop (el contenido
    // de PTR_MAINTOP) por lo que hay que pasarselo desde fuera

    if (!_mainTop)
        throw Excepcion(MSG_MAINTOP_NO_DEF);

    SPBUFFER->wordPoke(_mainTop+OFF_FONTS, ptrInicioDatos);
    SPBUFFER->bytePoke(_mainTop + OFF_NUMFONTS, _fonts.size()/768);
    for (unsigned int i=0; i < 280u; i++)
    {
        SPBUFFER->bytePoke(_mainTop+i, _udg_shades[i]);
    }

    if (_numFonts)
    {
        for (unsigned int j=0; j < 768u*_numFonts; j++)
        {
            SPBUFFER->bytePoke(ptrInicioDatos+j, _fonts[j]);
        }
        return ptrInicioDatos+768u*_numFonts-1;
    }
    else
    {
        // No ha escrito nada (lo incrementará luego la clase Aventura)
        return ptrInicioDatos-1;
    }
}

PtrBuffer
Caracteres::volcarDeMemoria ()
{
    getInicioDatos();

    _udg_shades.clear();
    _fonts.clear();

    for (unsigned int i=0; i < 280u; i++)
    {
        _udg_shades.push_back(SPBUFFER->bytePeek(_mainTop+i));
    }

    if (_numFonts)
    {
        for (unsigned int j=0; j < 768u*_numFonts; j++)
        {
            _fonts.push_back(SPBUFFER->bytePeek(_ptrDatos+j));
        }
        return _ptrDatos+768u*_numFonts-1;
    } else
    {
        // No ha escrito nada (lo incrementará luego la clase Aventura)
        return _ptrDatos-1;
    }
}

void
Caracteres::generarCodigo (std::ostream& os)
{
    os << RSV_CHARACTERS << endl << "{" << endl;
    for (unsigned int i=0; i < 280u; i++)
    {
        if (!(i % 8)) os << '\t';
        os << (unsigned int)_udg_shades[i] << ", ";
        if (i % 8 == 7) os << endl;
    }

    os << '\t' << (unsigned int)_numFonts;
    if (_numFonts) os << ',';
    os << endl;

    for (unsigned int j=0; j < 768u*_numFonts; j++)
    {
        if (!(j % 8)) os << '\t';
        os << (unsigned int)_fonts[j];
        if (j <  768u*_numFonts-1) os << ", ";
        if (j % 8 == 7) os << endl;
        if (j % 768u == 767u) os << endl;
    }

    os << "}" << endl;
}

void
Caracteres::leerDeCodigo (Tokeniser& tokeniser)
{
    Tokeniser::TTokenType token;

    _udg_shades.clear();
    _fonts.clear();

    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, RSV_CHARACTERS);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");
    token = tokeniser.leeToken();

    if (!(token == Tokeniser::TK_CONTROL && tokeniser.lvalue().literal=="}"))
    {
        tokeniser.putback();
        for (unsigned int i=0; i < 280u; i++)
        {
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_NUMERO)
            {
                _udg_shades.push_back(tokeniser.lvalue().valor);
            } else throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DECIMAL));
            saltarToken (tokeniser, Tokeniser::TK_CONTROL, ",");
        }
        token = tokeniser.leeToken(); // el número de fonts
        if (token == Tokeniser::TK_NUMERO)
        {
            _numFonts = tokeniser.lvalue().valor;
        } else throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DECIMAL));

        if (_numFonts)
         saltarToken (tokeniser, Tokeniser::TK_CONTROL, ",");

        for (unsigned int j=0; j < 768u*_numFonts; j++)
        {
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_NUMERO)
            {
                _fonts.push_back(tokeniser.lvalue().valor);
            } else throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DECIMAL));
            if (j < 768u*_numFonts-1)
                saltarToken (tokeniser, Tokeniser::TK_CONTROL, ",");
        }

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, "}");
    }
}

void
Caracteres::setMainTop(PtrBuffer dirMaintop)
{
    _mainTop = dirMaintop;
}
