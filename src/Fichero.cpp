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

#include "Fichero.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"

using namespace Inpaws;
using namespace std;

OFichero::OFichero(const std::string& nombre, bool borrar)
    : _nombre (nombre), _borrar(borrar)
{
    open(_nombre.c_str(), ifstream::out);
    if (!good())
    {
        throw ExcFichero("\""+nombre+"\": " + MSG_ERRORFICHERO);
    }
}

OFichero::~OFichero()
{
    close();
    if (_borrar)
        remove(_nombre.c_str());
}

IFichero::IFichero(const std::string& nombre, bool borrar)
    : _nombre (nombre), _borrar(borrar)
{
    open(_nombre.c_str(), ifstream::in);
    if (!good())
    {
        throw ExcFichero("\""+nombre+"\": " + MSG_ERRORFICHERO);
    }
}

IFichero::~IFichero()
{
    close();
    if (_borrar)
        remove(_nombre.c_str());
}
