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

#include "Excepcion.h"
#include <sstream>

using namespace Inpaws;
using namespace std;

Excepcion::Excepcion(const string& texto)
: _texto(texto)
{
    //ctor
}


Excepcion::~Excepcion()
{
    //dtor
}

ExcFichero::ExcFichero(const string& texto): Excepcion(texto)
{
    //ctor
}

ExcFichero::~ExcFichero()
{
    //dtor
}

ExcTokeniser::ExcTokeniser(const string& texto): Excepcion(texto)
{
    //ctor
}

ExcTokeniser::ExcTokeniser(unsigned long linea, const std::string& archivo, const std::string& texto)
{
    ostringstream ostr;
    ostr << linea << ":" << archivo << ":" << texto;
    setDesc(ostr.str());
}

ExcTokeniser::~ExcTokeniser()
{
    //dtor
}

