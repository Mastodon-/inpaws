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

//--------------------------------------------------------------
//
// Cadenas de sustitución para caracteres internacionales
//    (ej: SUBCHAR "á" "{144}"; )
//--------------------------------------------------------------

#ifndef PREPSUBCHAR_H_
#define PREPSUBCHAR_H_

#include "prep/PrepPart.h"
#include <map>
#include <string>

namespace Inpaws
{
    class PrepSubChar: public PrepPart
    {
        public:
            PrepSubChar();
            virtual ~PrepSubChar();

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;

            typedef std::map<char, std::string> TSubs;

            const TSubs& sustituciones() const { return _sustituciones; }

        private:
            TSubs _sustituciones;
    };
}

#endif
