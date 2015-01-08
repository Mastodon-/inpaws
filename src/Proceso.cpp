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

#include "Proceso.h"

using namespace Inpaws;
using namespace std;

Proceso::TEntradas::const_iterator Proceso::_itRecorrido;

Proceso::Proceso()
{
}

Proceso::~Proceso()
{
    for (TEntradas::const_iterator it = _entradas.begin();
         it != _entradas.end();
         it++)
    {
        delete (*it).second;
    }
}

void
Proceso::anadirEntrada(EntradaProcesos* entrada)
{

    Clave nuevaClave;
    nuevaClave.nombre = entrada->nombre();
    nuevaClave.verbo = entrada->verbo();
    _entradas.insert(pair<Clave, EntradaProcesos*>(nuevaClave, entrada));
}

EntradaProcesos*
Proceso::leerEntradaInit () const
{
    _itRecorrido = _entradas.begin();
    if (_itRecorrido != _entradas.end())
        return (*_itRecorrido).second;
    else
        return NULL;
}

EntradaProcesos*
Proceso::leerEntradaNext () const
{
    _itRecorrido++;

    if (_itRecorrido != _entradas.end())
        return (*_itRecorrido).second;
    else
        return NULL;
}

void
Proceso::_borrarDatos()
{
    for (TEntradas::const_iterator it = _entradas.begin();
         it != _entradas.end();
         it++)
    {
        delete (*it).second;
    }
    _entradas.clear();
}

void
Proceso::depuraEntradas(std::ostream& os) const
{
    for (TEntradas::const_iterator it = _entradas.begin();
         it != _entradas.end();
         it++)
    {
        cout << (int)(*it).second->verbo() << "," << (int)(*it).second->nombre() << endl;
    }
}
