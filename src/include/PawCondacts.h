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

//----------------------------------------------------------------------------
//
// Este es un SIGLETON que contiene la lista de condactos
// y sus codigos y parametros
//
//----------------------------------------------------------------------------

#ifndef PAWCONDACTS_H
#define PAWCONDACTS_H

#include "Condacto.h"
#include "SpBuffer.h"
#include <map>
#include <string>

#define PAWCONDACT(clave) ::Inpaws::PawCondacts::pawCondacts()->leeCondacto((clave))

namespace Inpaws
{
    class PawCondacts
    {
        public:
            PawCondacts();
            ~PawCondacts();
            static PawCondacts* pawCondacts()
            {
                if (_pawCondacts == NULL)
                {
                    _pawCondacts = new PawCondacts();
                }
                return _pawCondacts;
            }

            // Funciones añadir condactos y buscarlos por nombre y por codigo
            const Condacto* leeCondacto(SpByte codigo) const;
            const Condacto* leeCondacto(const std::string& nombre) const;
            bool existeCondacto (const std::string& nombre) const;
            bool esCondicion (const std::string& nombre) const;
            void anadirCondacto(SpByte codigo ,
                                const std::string& nombre,
                                SpByte numParams,
                                SpByte tipo_prm1 = 0, SpByte max_valor_prm1 = 0,
                                SpByte tipo_prm2 = 0, SpByte max_valor_prm2 = 0);

        private:
            typedef std::map<std::string, Condacto*> TCondactosString;
            typedef std::map<SpByte, Condacto*> TCondactosCodigo;
            TCondactosString _condactosString;
            TCondactosCodigo _condactosCodigo;
            static PawCondacts* _pawCondacts;
            void _anadirCondactos();
    };
}

#endif // PAWCONDACTS_H

