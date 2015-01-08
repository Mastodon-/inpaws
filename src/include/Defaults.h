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

#ifndef DEFAULTS_H_
#define DEFAULTS_H_

#include "PawPart.h"
#include <vector>

namespace Inpaws
{
    class Defaults: public PawPart
    {
      public:
        Defaults();
        virtual ~Defaults();

        // Metodos heredados de PawPart
        virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
        virtual PtrBuffer volcarDeMemoria ();
        virtual void generarCodigo (std::ostream& os);
        virtual void leerDeCodigo (Tokeniser& tokeniser);

      private:
        void _asignarItem(const std::string& item, SpByte valor);

        SpByte _charSet, _ink, _paper, _flash, _bright,
               _inverse, _over, _border;
    };
}

#endif

