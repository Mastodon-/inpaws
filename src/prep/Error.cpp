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

#include "prep/Error.h"
#include <iostream>
#include <cstdlib>

using namespace Inpaws;
using namespace std;

Error* Error::_error = NULL;

Error::Error()
{
    //ctor
}

Error::~Error()
{
    //dtor
}

void
Error::nuevoError (const string& fichero, unsigned long linea, const string& texto, int tipo)
{
    // Buscamos el error por si ya lo hemos añadido
    // (ocurre al utilizar sinónimos en entradas de procesos)
    for (TListaErrores::const_iterator it = _listaErrores.begin();
        it != _listaErrores.end();
        it++)
    {
        if ((*it).fichero == fichero &&
            (*it).linea == linea &&
            (*it).texto == texto &&
            (*it).tipo == tipo)
        {
            return;
        }
    }

    Item nuevoError;

    nuevoError.fichero = fichero;
    nuevoError.linea = linea;
    nuevoError.texto = texto;
    nuevoError.tipo = tipo;

    _listaErrores.push_back(nuevoError);
    if (tipo == FATAL)
    {
        volcarErrores(cerr);
        exit(1);
    }
}

void
Error::volcarErrores (ostream& os) const
{
    for (TListaErrores::const_iterator it = _listaErrores.begin();
         it != _listaErrores.end();
         it++)
    {
        os << (*it).fichero << ":";
        if ((*it).linea !=0) os << (*it).linea << ":";
        if ((*it).tipo == WARNING)
        {
            os << "(WARNING) ";
        }
        os << (*it).texto << endl;
    }
}
