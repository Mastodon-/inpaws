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
// Esta clase almacena la lista de (punteros a) entradas global de la aventura
// para todos los procesos. Se utiliza para optimizar el volcado de entradas
// a memoria de forma que dos o más entradas iguales sólo almacenarán
// en memoria el código de una de ellas.
//------------------------------------------------------------------------------

#ifndef _LISTA_ENTRADAS_VOLCADAS_H
#define _LISTA_ENTRADAS_VOLCADAS_H

#include "EntradaProcesos.h"
#include <set>

namespace Inpaws
{
    class ListaEntradasVolcadas
    {
        public:
            struct EntradaLista
            {
                const EntradaProcesos* entrada;
                PtrBuffer dirVolcada;
                bool operator< (const EntradaLista& otra) const
                {

                    return (*entrada < *otra.entrada);
                }

            };

            ListaEntradasVolcadas();
            //~ListaEntradasVolcadas();
            void anadirEntrada (const EntradaProcesos* entrada, PtrBuffer dirVolcada);
            PtrBuffer buscarEntrada (const EntradaProcesos* entrada) const; // 0 si no la encuentra

        private:
            std::set<EntradaLista> _entradas;
    };
}

#endif
