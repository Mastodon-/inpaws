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

#include "prep/PrepPart.h"
#include "prep/Error.h"
#include "prep/MsgPrep.h"
#include "UtilTexto.h"

using namespace Inpaws;
using namespace std;

PrepPart::PrepPart(const std::string& nombre, SpByte maxValorPool)
  : _nombre(nombre), _pool(0, maxValorPool)
{
}

PrepPart::~PrepPart()
{
}

void
PrepPart::saltarToken (PrepTokeniser& tokeniser,
                      PrepTokeniser::TTokenType token,
                      const std::string& literal)
{
    PrepTokeniser::TTokenType tokenRead;

    tokenRead = tokeniser.leeToken();
    if (token != tokenRead ||
        UtilTexto::toUpr(tokeniser.lvalue().literal) != UtilTexto::toUpr(literal))
    {
        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(literal), Error::FATAL);
    }
}

SpByte
PrepPart::dameValorPool ()
{
    int retValor = _pool.dameValorPool();

    if (retValor == -1)
        ERRORPAW("", 0, _nombre + ": " + MSGPREP_NUMELEMSUP, Error::FATAL);

    return (SpByte) retValor;

    /*if (_proximoValorPool > _maxValorPool) ERRORPAW(0, _nombre + ": " + MSGPREP_NUMELEMSUP, Error::FATAL);

    while (_poolAsignados.count(_proximoValorPool))
    {
        if (_proximoValorPool == _maxValorPool)
            ERRORPAW(0, _nombre + ": " + MSGPREP_NUMELEMSUP, Error::FATAL);
        _proximoValorPool++;
    }
    retValor = (SpByte)_proximoValorPool;
    _poolAsignados.insert(retValor);
    _proximoValorPool++;*/
}
