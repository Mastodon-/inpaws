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

#ifndef GRAFICOS_H_
#define GRAFICOS_H_

#include "PawPart.h"
#include <vector>

namespace Inpaws
{
    class Graficos: public PawPart
    {
      public:
        Graficos(PtrBuffer ptrIndice, PtrBuffer ptrAttr);
        virtual ~Graficos();

        // Metodos heredados de PawPart
        virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
        virtual PtrBuffer volcarDeMemoria ();
        virtual void generarCodigo (std::ostream& os);
        virtual void leerDeCodigo (Tokeniser& tokeniser);
        virtual PtrBuffer inicioReal() const
          { return SPBUFFER->wordPeek(ptrDatos()); }

        // Completar hasta numLocs-1 con vacias para no tener
        // que tocar el fuente cada vez que se añade una localidad
        void completarLocalidades (SpByte numLocs, SpByte attr = 7);

        // Tamaño de todo el volcado (para luego meter en memoria en
        // la posición correcta)
        SpWord size() const
          { return _datosGraficos.size() + 3*_tamanoGraficos.size() + 4; }

      private:
        PtrBuffer _ptrAttr;
        std::vector<SpByte> _attr;
        std::vector<unsigned int> _tamanoGraficos;
        std::vector<SpByte> _datosGraficos;
    };
}

#endif

