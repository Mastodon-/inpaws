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

//----------------------------------------------------------------
//
// ListaSimple: <- PawPart
//
// Template que vale para una lista simple de una serie de
// valores (utilizada por Objects Words, Initially At y Object Weight
// El valor del template es el numero de datos SpByte que tiene cada
// Lectura
//----------------------------------------------------------------

#ifndef LISTASIMPLE_H
#define LISTASIMPLE_H

#include "PawPart.h"
#include "Excepcion.h"
#include "defines.h"
#include <map>
#include <string>
#include <cstdlib>
#include <cstring>

namespace Inpaws
{
    template <int ELEMENTOS, const char* TITULO>
    class ListaSimple: public PawPart
    {


        public:
            typedef SpByte* TDato;

            ListaSimple (PtrBuffer ptrIndice, SpByte numDatos = 0);
            virtual ~ListaSimple();

            // Metodos heredados de PawPart
            virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
            virtual PtrBuffer volcarDeMemoria ();
            virtual void generarCodigo (std::ostream& os);
            virtual void leerDeCodigo (Tokeniser& tokeniser);

            // Para fijar el numero de datos (objetos)
            // OJO: hacerlo siempre antes de volcarDeMemoria
            void setNumDatos(SpByte numDatos) { _numDatos = numDatos; }

            // Funciones para obtener y añadir datos
            void anadirDato (SpByte numObj, const TDato dato);
            const TDato leerDato (SpByte numDato) const;

            size_t size() const { return _datos.size(); }

        protected:
            typedef std::map<SpByte,TDato> TDatos;
            TDatos _datos;
            std::string _titulo;

        private:
            SpByte _numDatos; // Tantos como objetos
            void _borrarDatos();


    };

    #include "ListaSimpleImpl.h"
    extern char rsv_objwords[];
    extern char rsv_initiallyat[];
    extern char rsv_objweight[];
    typedef ListaSimple<2, rsv_objwords> ObjWords;
    typedef ListaSimple<1, rsv_initiallyat> InitiallyAt;
    typedef ListaSimple<1, rsv_objweight> ObjWeightType;
}

#endif // LISTASIMPLE_H

