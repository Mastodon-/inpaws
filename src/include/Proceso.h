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

//---------------------------------------------------------------------------
//
// Proceso
//   Esta clase representa tanto la tabla de respuestas (Proceso 0) como
//   cualquiera de los procesos de una aventura. Consiste en un conjunto de
//   parejas nombre-verbo, asociadas a una serie de condactos
//
//---------------------------------------------------------------------------


#ifndef PROCESO_H_
#define PROCESO_H_

#include <map>
#include "EntradaProcesos.h"

namespace Inpaws
{
    class Proceso
    {
        public:
            struct Clave
            {
                SpByte verbo;
                SpByte nombre;
                bool operator<(const Clave& clave) const
                { return (verbo < clave.verbo ||
                          (verbo == clave.verbo && nombre < clave.nombre)); }
            };
            Proceso ();
            ~Proceso ();

            void anadirEntrada(EntradaProcesos* entrada);
            EntradaProcesos* leerEntradaInit () const;
            EntradaProcesos* leerEntradaNext () const;
            void depuraEntradas(std::ostream& os) const;
            size_t size() const { return _entradas.size(); }

        private:

            typedef std::multimap<Clave, EntradaProcesos*> TEntradas;
            static TEntradas::const_iterator _itRecorrido;
            TEntradas _entradas;

            void _borrarDatos();
    };
}

#endif // PROCESO_H_
