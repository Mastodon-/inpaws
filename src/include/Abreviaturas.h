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

#ifndef ABREVIATURAS_H_
#define ABREVIATURAS_H_

#include "PawPart.h"
#include <map>
#include <string>

namespace Inpaws
  {
    class Abreviaturas: public PawPart
      {
      public:
        typedef std::map<SpByte, std::string> TAbreviaturas;
        Abreviaturas(SpWord offAbrev); // Desplazamiento respecto al maintop
        virtual ~Abreviaturas();

        // Metodos heredados de PawPart
        virtual void getInicioDatos();
        virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
        virtual PtrBuffer volcarDeMemoria ();
        virtual void generarCodigo (std::ostream& os);
        virtual void leerDeCodigo (Tokeniser& tokeniser);
        void setMainTop(PtrBuffer dirMaintop);

        bool comprimido() const { return _comprimido; }
        const TAbreviaturas& abreviaturas() const { return _abreviaturas; }

      private:
        bool _comprimido;

        TAbreviaturas _abreviaturas;
        SpWord _offAbrev;
        PtrBuffer _mainTop;
      };
  }

#endif
