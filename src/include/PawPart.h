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
// Clase: PawPart
//
// Clase base del resto de entidades de PAW (localidades, mensajes, ...)
// Toda entidad PAW permite:
//    - Fijar la posición donde se almacena el puntero a los datos (constructor)
//    - Fijar la dirección de inicio de los datos (setInicioDatos)
//    - Leer los datos de un buffer de memoria (0-65535) Tipo SpBuffer
//    - Escribir los datos a un buffer de memoria de tipo SpBuffer
//    - Generar código que luego se pueda compilar
//-----------------------------------------------------------------------

#ifndef PAWPART_H
#define PAWPART_H

#include "SpBuffer.h"
#include "Tokeniser.h"
#include "prep/Diccionario.h"
#include <iostream>

namespace Inpaws
{
    class PawPart
    {
        public:
            PawPart(PtrBuffer ptrIndice);
            virtual ~PawPart();
            PtrBuffer ptrIndice() const { return _ptrIndice; }
            PtrBuffer ptrDatos() const { return _ptrDatos; }
            //void setInicioDatos (PtrBuffer ptrDatos) { _ptrDatos = ptrDatos; }
            virtual void getInicioDatos() { _ptrDatos = SPBUFFER->wordPeek(_ptrIndice); }
            virtual PtrBuffer inicioReal() const { return ptrDatos(); }

            // Volcado a la memoria, devuelve el puntero al ultimo dato escrito
            // (importante ya que es a partir de donde hay que seguir escribiendo)
            virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const = 0;
            // Volcado de la memoria, devuelve el puntero al ultimo dato leido
            virtual PtrBuffer volcarDeMemoria () = 0;

            // Funciones para escribir y leer codigo fuente
            // OJO: Fuente puro PAW sin preprocesar, no debería
            // tener errores
            virtual void generarCodigo (std::ostream& os) = 0;

            // Funcion para generar código compatible con las versiones CP/M, PAW-PC y Superglus
            virtual void generarCodigoDisco (std::ostream& os, uint16_t flags) { };
            virtual void generarCodigoSuperglus (std::ostream& os, uint16_t flags,
                                                 Diccionario::TDiccionarioInverso diccionario) { };

            virtual void leerDeCodigo (Tokeniser& tokeniser) = 0;
            // Funcion para saltar las palabras reservadas,lanza excepciones
            static void saltarToken (Tokeniser& tokeniser,
                                      Tokeniser::TTokenType token,
                                      const std::string& literal);

            // Funcion que devuelve el número de Localidades, Objetos, etc...
            virtual int numElems() const { return 0; }

        protected:
            // Dirección (absoluta) donde está el puntero a los datos
            PtrBuffer _ptrIndice;
            // Dirección donde comienzan los datos (apuntada por ptrIndice)
            PtrBuffer _ptrDatos;
        private:

    };
}

#endif // PAWPART_H
