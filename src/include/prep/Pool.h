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

//-------------------------------------------------------------------------------------
//
// Pool de valores para asignar a los símbolos
//
//-------------------------------------------------------------------------------------

#ifndef POOL_H_
#define POOL_H_

#include "SpBuffer.h"
#include <set>

namespace Inpaws
{
    class Pool
    {
        public:
            Pool (SpByte inicial = 0, SpByte final = 255);
            void anadirPool (SpByte valor) { _poolAsignados.insert(valor); }

            // Devuelve un nuevo valor que no esté asignado, o -1 si no hay más
            int dameValorPool ();
            bool disponibleValor(SpByte valor) const
              { return (_inicial <= valor && _final >= valor && !_poolAsignados.count(valor)); }

            SpByte inicial() const { return _inicial; }
            SpByte final() const { return _final; }
            int actual() const { return (_proximoValorPool > _final? -1 : _proximoValorPool); }
            bool poolOk() const; // Indica si todos los elementos son correlativos

        private:
            SpByte _inicial, _final;
            int _proximoValorPool;
            std::set<SpByte> _poolAsignados;
    };
}

#endif
