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

#ifndef _PREPVOCABULARIO_H_
#define _PREPVOCABULARIO_H_

#include "PrepPart.h"
#include "Pool.h"
#include <vector>
#include <string>

namespace Inpaws
{
    class PrepVocabulario: public PrepPart
    {
        public:
            struct Palabra
            {
                unsigned long linea;
                int tipo;
                std::vector<std::string> listasinonimos;
            };

            PrepVocabulario(size_t tamPalabra);
            virtual ~PrepVocabulario();

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;

            // Funcion de apoyo a procesos y conexiones
            bool existePalabra (const std::string& palabra, int tipo) const
            { if (!_vocabularioNombres.count(palabra) ||
                  (*_vocabularioNombres.find(palabra)).second != tipo) return false;
                  else return true; }

        private:
            static bool wordOk(const std::string& palabra);

            std::vector<Palabra> _vocabulario;
            std::map<std::string, std::string> _palabrasGenera; // Set para comprobar sinónimos ocultos (ALICATE/ALICANTE)
            std::map<std::string, int> _vocabularioNombres; // necesario para los procesos y conexiones
            Pool* _pooles[7]; // Un pool por cada tipo de palabra
            size_t _tamPalabra; // Tamaño de la palabra (5 en paw, 10 en superglus)
    };
}

#endif

