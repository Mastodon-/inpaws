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

#ifndef PREPABREVIATURAS_H_
#define PREPABREVIATURAS_H_

#include <map>
#include <string>
#include "SpBuffer.h"
#include "PrepAbreviable.h"
#include "Aventura.h"

namespace Inpaws
{
    class PrepAbreviaturas
    {
        public:
            PrepAbreviaturas();
            ~PrepAbreviaturas();

            void leerDeAventura (const Aventura& aventura);
            void anadirAbreviable(PrepAbreviable* objeto) { _abreviables.push_back(objeto); }
            SpWord calcularAbreviaturas(); // devuelve el número de bytes ahorrados
            void generarCodigo(std::ostream& os) const;
            const PrepAbreviable::TDiccionario& abreviaturas() const { return _abreviaturas; }

        private:
            typedef std::vector<PrepAbreviable*> TListaAbreviables;
            PrepAbreviable::TDiccionario _abreviaturas;
            TListaAbreviables _abreviables;
    };
}

#endif
