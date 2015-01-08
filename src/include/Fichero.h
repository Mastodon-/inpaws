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

#ifndef _FICHERO_H_
#define _FICHERO_H_

#include <fstream>
#include <string>

namespace Inpaws
{
    class OFichero: public std::ofstream
    {
        public:
            OFichero(const std::string& nombre, bool borrar = false);
            ~OFichero();
        private:
            std::string _nombre;
            bool _borrar;
    };

    class IFichero: public std::ifstream
    {
        public:
            IFichero(const std::string& nombre, bool borrar = false);
            ~IFichero();
        private:
            std::string _nombre;
            bool _borrar;
    };
}

#endif
