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

//------------------------------------------------------------------------------------------
//
// Lista de datos en bruto, se utiliza para los gráficos y los juegos de caracteres
//
//------------------------------------------------------------------------------------------

#ifndef PREPLISTADATOS_H
#define PREPLISTADATOS_H

#include "prep/PrepPart.h"
#include <vector>

namespace Inpaws
{
    class PrepListaDatos: public PrepPart
    {
        public:
            PrepListaDatos(const std::string& nombre);
            virtual ~PrepListaDatos();

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;
            size_t size() const { return _datos.size(); }
        private:
            std::vector<SpWord> _datos;
    };
}

#endif
