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

#include "ListaEntradasVolcadas.h"

using namespace std;
using namespace Inpaws;

ListaEntradasVolcadas::ListaEntradasVolcadas()
{
}

void ListaEntradasVolcadas::anadirEntrada (const EntradaProcesos* entrada, PtrBuffer dirVolcada)
{
    EntradaLista nuevaEntrada;
    nuevaEntrada.entrada = entrada;
    nuevaEntrada.dirVolcada = dirVolcada;
    _entradas.insert(nuevaEntrada);
}

PtrBuffer ListaEntradasVolcadas::buscarEntrada (const EntradaProcesos* entrada) const
{
    EntradaLista entradaBuscar;
    entradaBuscar.entrada = entrada;
    set<EntradaLista>::const_iterator it = _entradas.find(entradaBuscar);
    if (it != _entradas.end())
        return (*it).dirVolcada;
    else
        return 0;
}
