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

#ifndef CARACTERES_H_
#define CARACTERES_H_

#include "PawPart.h"
#include <vector>

namespace Inpaws
{
    class Caracteres: public PawPart
    {
      public:
        Caracteres();
        virtual ~Caracteres();

        // Metodos heredados de PawPart
        virtual void getInicioDatos();
        virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
        virtual PtrBuffer volcarDeMemoria ();
        virtual void generarCodigo (std::ostream& os);
        virtual void leerDeCodigo (Tokeniser& tokeniser);
        void setMainTop(PtrBuffer dirMaintop);
        virtual int numElems() const { return _numFonts; }

      private:
        SpByte _numFonts;
        PtrBuffer _mainTop;
        std::vector<SpByte> _udg_shades;
        std::vector<SpByte> _fonts;
    };
}

#endif
