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

#include "Abreviaturas.h"
#include "SpBuffer.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include "UtilTexto.h"

using namespace Inpaws;
using namespace std;

Abreviaturas::Abreviaturas(SpWord offAbrev)
: PawPart(0), _offAbrev(offAbrev), _mainTop(0)
{
}

Abreviaturas::~Abreviaturas()
{
}

void
Abreviaturas::getInicioDatos()
{
    _mainTop = SPBUFFER->wordPeek(PTR_MAINTOP);
    _ptrIndice = SPBUFFER->wordPeek(PTR_MAINTOP)+OFF_ABBREV;
    _ptrDatos = SPBUFFER->wordPeek(_ptrIndice);

    // Base de datos comprimida: no lo está si el puntero de los
    // caracteres y el de las abreviaturas es el mismo
    PtrBuffer inicioAbrev = SPBUFFER->wordPeek(_ptrIndice);

    _comprimido = (SPBUFFER->bytePeek(inicioAbrev) == 255?false:true);
}

PtrBuffer
Abreviaturas::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    // En el caso de una aventura generada desde código fuente
    // no sabemos a priori donde estará el maintop (el contenido
    // de PTR_MAINTOP) por lo que hay que pasarselo desde fuera

    if (!_mainTop)
        throw Excepcion(MSG_MAINTOP_NO_DEF);

    SPBUFFER->wordPoke(_mainTop+OFF_ABBREV, ptrInicioDatos);

    if (_comprimido)
    {
        for (SpByte i = 164; i <= 254; i++)
        {
            for (unsigned int j = 0; j < (*_abreviaturas.find(i)).second.size(); j++)
            {
                if (j < (*_abreviaturas.find(i)).second.size()-1)
                {
                    SPBUFFER->bytePoke(ptrInicioDatos, (*_abreviaturas.find(i)).second[j]);
                }
                else
                {
                    SPBUFFER->bytePoke(ptrInicioDatos, (*_abreviaturas.find(i)).second[j] | 128);
                }

                ptrInicioDatos++;
            }
        }
    }
    else SPBUFFER->bytePoke(ptrInicioDatos++, 255);

    return ptrInicioDatos-1;

}

PtrBuffer
Abreviaturas::volcarDeMemoria ()
{
    getInicioDatos();
    PtrBuffer ptrTmp = ptrDatos();
    if (_comprimido)
    {
        string abrevActual;
        for (SpByte i = 164; i <= 254; i++)
        {
            SpByte debug;
            while(!(SPBUFFER->bytePeek(ptrTmp) & 128))
            {
                debug = SPBUFFER->bytePeek(ptrTmp);
                abrevActual.append(1, SPBUFFER->bytePeek(ptrTmp++));
            }
            debug = SPBUFFER->bytePeek(ptrTmp);
            abrevActual.append(1, SPBUFFER->bytePeek(ptrTmp++) & 127);

            _abreviaturas[i] = abrevActual;
            abrevActual.clear();
        }
        return ptrTmp-1;
    }
    else return ptrTmp;

}

void
Abreviaturas::generarCodigo (std::ostream& os)
{
    UtilTexto utilTexto;

    os << RSV_ABBREVIATIONS << endl << "{" << endl;
    if (_comprimido)
    {
        for (SpByte i = 164; i <= 254; i++)
        {
            os << '\t' << (unsigned int)i << " : \"";
            os << utilTexto.expandirTexto(_abreviaturas[i], true);
            os << "\";" << endl;
        }
    }
    os << "}" << endl;
}

void
Abreviaturas::leerDeCodigo (Tokeniser& tokeniser)
{
    UtilTexto utilTexto;
    Tokeniser::TTokenType token;
    SpByte codTokenActual;

    // Primero aparece el literal ABREVIATIONS y el "{"
    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, RSV_ABBREVIATIONS);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");
    token = tokeniser.leeToken();

    while (!(token==Tokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
        if (token == Tokeniser::TK_NUMERO &&
            tokeniser.lvalue().valor >= 164 &&
            tokeniser.lvalue().valor <= 254)
        {
            codTokenActual = tokeniser.lvalue().valor;
            saltarToken (tokeniser, Tokeniser::TK_CONTROL, ":");
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_LITERAL)
            {
                string nuevaAbrev = utilTexto.compactarTexto(tokeniser.lvalue().literal);
                _abreviaturas[codTokenActual] = nuevaAbrev;
            }
            else
                throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_LITERAL_ENTRECOMILLADO));

            saltarToken (tokeniser, Tokeniser::TK_CONTROL, ";");
        } else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_ABREVIATURA));

        token = tokeniser.leeToken();
    }
    _comprimido = (_abreviaturas.size()?true:false);
}

void
Abreviaturas::setMainTop(PtrBuffer dirMaintop)
{
    _mainTop = dirMaintop;
}
