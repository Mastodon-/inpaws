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

#include "prep/Pool.h"

using namespace Inpaws;
using namespace std;

Pool::Pool (SpByte inicial, SpByte final)
: _inicial(inicial), _final(final), _proximoValorPool(inicial)
{
}

int
Pool::dameValorPool ()
{
    SpByte retValor;

    if (_proximoValorPool > _final) return -1;

    while (_poolAsignados.count(_proximoValorPool))
    {
        if (_proximoValorPool++ == _final)
            return -1;
    }
    retValor = (SpByte)_proximoValorPool;
    _poolAsignados.insert(retValor);
    _proximoValorPool++;

    return retValor;
}

bool
Pool::poolOk() const
{
    if (_poolAsignados.empty())
      return true;
    else
    {
        SpByte ultimo = *_poolAsignados.rbegin();
        SpByte primero = *_poolAsignados.begin();
        if ((unsigned)(ultimo - primero + 1) == _poolAsignados.size())
            return true;
    }
    return false;
}
