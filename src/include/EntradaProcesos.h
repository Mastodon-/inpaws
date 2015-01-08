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

//-----------------------------------------------------------------------
//
// EntradaProcesos:
//    Clase para almacenar cada una de las entradas en las tablas de
//    procesos y respuestas. Cada entrada se identifica por una pareja
//    verbo-nombre  y tiene asociada una lista de condactos.
//    El verbo y el nombre valdr�n 1 cuando queramos representar
//    el car�cter '*', y 255 para el caracter '_'.
//    Hay un bug del PAW que hace que si definimos un verbo de la
//    aventura con c�digo 1, en las tablas de procesos se confunde con
//    el '*' que tambi�n tiene asociado el 1 (aunque no es un verbo).
//    Por esa raz�n, no se permitir� en Inpaws definir verbos 1 (en PAW
//    original s� se permite).
//----------------------------------------------------------------------

#ifndef ENTRADA_PROCESOS_H_
#define ENTRADA_PROCESOS_H_

#include "SpBuffer.h"
#include "Condacto.h"
#include "Vocabulario.h"
#include <vector>

namespace Inpaws
{
    class EntradaProcesos
    {
        public:
            EntradaProcesos (SpByte verbo, SpByte nombre);
            ~EntradaProcesos();

            SpByte verbo() const { return _verbo; }
            SpByte nombre() const { return _nombre; }

            void anadirCondacto(Condacto::CondactData* const condacto);
            PtrBuffer volcarAMemoria (PtrBuffer ptrDireccion) const;
            PtrBuffer volcarDeMemoria (PtrBuffer ptrDireccion);
            void generarCodigo (std::ostream& os) const;
            void leerDeCodigo (Tokeniser& tokeniser);

            // Funcion utilizada por el preprocesador que muestra nombres de las palabras
            void generarCodigo (std::ostream& os, const Vocabulario* vocabulario,
                                 int flagDeFormato = VERSPECTRUM,
                                 const Diccionario::TDiccionarioInverso* diccionario = NULL) const;

            bool operator<(const EntradaProcesos& otra) const
            {
                size_t max = (_condactos.size() < otra._condactos.size()?_condactos.size():
                                                  otra._condactos.size());
                for (size_t i = 0; i < max; i++)
                {
                    if (*_condactos[i] < *otra._condactos[i])
                        return true;
                    else if (*otra._condactos[i] < *_condactos[i])
                        return false;
                }
                // Son iguales hasta el tama�o comprobado, la menor es la m�s corta
                if (_condactos.size() < otra._condactos.size())
                    return true;
                else
                    return false;
            }
        private:
            SpByte _verbo, _nombre;
            typedef std::vector<Condacto::CondactData*> TCondactos;
            TCondactos _condactos;
            void _borrarDatos();
    };
}

#endif //ENTRADA_PROCESOS_H_
