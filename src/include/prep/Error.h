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

#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <vector>

#define ERRORPAW ::Inpaws::Error::error()->nuevoError

namespace Inpaws
{
    class Error
    {
        public:

            enum { WARNING, NORMAL, FATAL };
            struct Item
            {
                std::string fichero;
                unsigned long linea;
                std::string texto;
                int tipo;
            };

            Error();
            ~Error();

            static Error* error() {
                if (!_error)
                    _error = new Error;
                return _error;
            }
            void nuevoError (const std::string& fichero, unsigned long linea, const std::string& texto, int tipo = NORMAL);
            void volcarErrores (std::ostream& os) const;
            bool empty() const { return _listaErrores.size() == 0; }

        protected:
        private:
            typedef std::vector<Item> TListaErrores;
            TListaErrores _listaErrores;
            static Error* _error;
    };
}

#endif // ERROR_H
