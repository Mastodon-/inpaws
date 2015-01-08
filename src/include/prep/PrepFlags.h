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

#ifndef PREPFLAGS_H_
#define PREPFLAGS_H_

#include "prep/PrepPart.h"
#include <set>
#include <string>

namespace Inpaws
{
    class PrepFlags: public PrepPart
    {
        public:
            PrepFlags();
            virtual ~PrepFlags();

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;

        private:
            std::set<std::string> _flags;
    };
}

#endif
