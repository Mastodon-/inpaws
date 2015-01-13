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
// Este es un SIGLETON que contiene la memoria del spectrum
// y las funciones para acceder y modificarla
//
//----------------------------------------------------------------------------

#ifndef SPBUFFER_H
#define SPBUFFER_H

#include <iostream>
#include <string>
#include <stdint.h>

#define SPBUFFER ::Inpaws::SpBuffer::spBuffer()

namespace Inpaws
{
    typedef uint16_t PtrBuffer;
    typedef uint16_t SpWord;
    typedef uint8_t SpByte;

    class SpBuffer
    {
        public:
            SpBuffer();                           // Bloque de memoria vacío
            virtual ~SpBuffer();
            static SpBuffer* spBuffer()
            {
                if (_spbuffer == NULL)
                {
                    _spbuffer = new SpBuffer();
                }
                return _spbuffer;
            }

            // Leer y escribir el bloque completo
            void readBuffer(std::istream& is);
            void writeBuffer (std::ostream& os) const;
            void writeBufferTAP (const std::string& nombre); // Aventura a TAP de PAW
            void readBufferDmp (const std::string& fichero); // Leer de un volcado de 64k
            void readBufferSNA (const std::string& fichero); // Leer un .SNA
            void readBufferZ80 (const std::string& fichero);


            // Leer y escribir palabras de memoria
            SpWord wordPeek(PtrBuffer ptr) const;
            void wordPoke(PtrBuffer ptr, SpWord word);

            // Leer y escribir Bytes en memoria
            SpByte bytePeek(PtrBuffer ptr) const;
            void bytePoke (PtrBuffer ptr, SpByte byte);

            // Leer y escribir caracteres (conversión pc<->paw
            char charPeek (PtrBuffer ptr) const;
            void charPoke (PtrBuffer ptr, char caracter);

            // DEBUG
            void compararMemoria(const std::string& otroFicheroSNA) const;

        protected:

        private:
            SpByte _memoria[65536];
            static SpBuffer* _spbuffer;
            void _saveTapeBlock (std::ostream& os, const std::string& nombre,
                                 PtrBuffer inicio, PtrBuffer final);
            size_t _leerBloqueZ80(const SpByte *buffer, PtrBuffer dir, SpWord len, size_t indiceBuffer);
    };
}

#endif // SPBUFFER_H
