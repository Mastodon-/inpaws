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

#ifndef EXCEPCION_H
#define EXCEPCION_H

#include <string>

namespace Inpaws
{
    class Excepcion
    {
        public:
            Excepcion(const std::string& texto);
            Excepcion() { }
            virtual ~Excepcion();
            const std::string& desc() const { return _texto; }
            void setDesc (const std::string texto) { _texto = texto; }
            operator std::string() { return desc(); }

        protected:
        private:
            std::string _texto;
    };

    class ExcFichero: public Excepcion
    {
        public:
            ExcFichero(const std::string& texto);
            operator std::string() { return desc(); }
            virtual ~ExcFichero();
    };

    class ExcTokeniser: public Excepcion
    {
        public:
            ExcTokeniser(const std::string& texto);
            ExcTokeniser(unsigned long linea, const std::string& archivo, const std::string& texto);
            operator std::string() { return desc(); }
            virtual ~ExcTokeniser();
    };

}

#endif // EXCEPCION_H
