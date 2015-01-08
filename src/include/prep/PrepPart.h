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

#ifndef _PREP_PART_H
#define _PREP_PART_H

#include <iostream>
#include <set>
#include "Aventura.h"
#include "Diccionario.h"
#include "prep/PrepTokeniser.h"
#include "prep/Pool.h"

namespace Inpaws
{
    class PrepPart
    {
        public:
            PrepPart(const std::string& nombre, SpByte maxValorPool = 255);
            virtual ~PrepPart();

            // Genera codigo que luego se puede volver a compilar
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const = 0;

            // Funciones llamadas por el preprocesador
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario) = 0;
            virtual void asignarValoresSimbolos(Diccionario& diccionario) = 0;
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const = 0;

            static void saltarToken (PrepTokeniser& tokeniser,
                                      PrepTokeniser::TTokenType token,
                                      const std::string& literal);
        protected:

            // Funciones para manipular los pooles de valores asignados a los identificadores

            // Añade un valor al pool, que ya no es podrá utilizar en otro símbolo
            void anadirPool (SpByte valor) { _pool.anadirPool(valor); }

            // Devuelve un nuevo valor que no esté asignado o comprueba si esta disponible
            SpByte dameValorPool ();
            bool disponibleValor(SpByte valor) const { return _pool.disponibleValor(valor); }

            // Devuelve true si las definiciones del pool son consecutivas
            bool poolDefinicionesOk() { return _pool.poolOk(); }

            const std::string& nombre() const { return _nombre; }

        private:
            std::string _nombre;
            Pool _pool;
    };
}

#endif // _PREP_PART_H
