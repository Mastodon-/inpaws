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

//----------------------------------------------------------------
//
// Textos: <- PawPart
//
// Esta clase representa a cualquier entidad que consista en un
// conjunto de textos de la aventura, lo que incluye a
// los mensajes, los mensajes del sistema, los nombres de los
// objetos y las localidades.
// En el constructor hay que pasarle el desplazamiento con respecto
// al MainTop(PTR_MAINTOP) en el que viene almacenado el nº de mensajes
//----------------------------------------------------------------

#ifndef TEXTOS_H
#define TEXTOS_H

#include "PawPart.h"
#include "SpBuffer.h"
#include "Tokeniser.h"
#include <map>

namespace Inpaws
{
    class Textos: public PawPart
    {
        public:
            Textos(PtrBuffer ptrIndice, SpWord offNumText,
                   const std::string& titulo); // Tot.msg = *(MAINTOP+offNumText)
            virtual ~Textos();

            // Metodos heredados de PawPart
            virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
            virtual PtrBuffer volcarDeMemoria ();
            virtual void generarCodigo (std::ostream& os);
            virtual void generarCodigoDisco (std::ostream& os, uint16_t flags);
            virtual void generarCodigoSuperglus (std::ostream& os, uint16_t flags,
                                                 Diccionario::TDiccionarioInverso diccionario);
            virtual void leerDeCodigo (Tokeniser& tokeniser);
            virtual PtrBuffer inicioReal() const
             { return SPBUFFER->wordPeek(ptrDatos()); }
            virtual int numElems() const { return _textos.size(); }

            // Funciones para obtener y añadir textos
            void anadirTexto (SpByte codigo,const std::string& texto);
            const std::string& obtenerTexto (SpByte codigo) const;
            size_t size() const { return _textos.size(); }

        protected:
            typedef std::map<SpByte,std::string> TTextos;

        private:
            SpWord _offNumText;   // desplazamiento respecto al Maintop que almacena el nº
            TTextos _textos;
            std::string _titulo; // "LOCATIONS", "MESSAGES", etc
            SpWord _leerTextoMemoria (std::string& texto, SpWord posicion) const;
            std::string _numeroAString(SpByte spbyte) const;
            SpByte _valueOf (std::string::const_iterator& it) const;
            void _generarTexto (std::ostream& os, const std::string& texto) const; // Genera texto con "

    };
}

#endif // TEXTOS_H
