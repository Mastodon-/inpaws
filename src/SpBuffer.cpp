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

#include "SpBuffer.h"
#include "Excepcion.h"
#include "MsgAplicacion.h"
#include "UtilTexto.h"
#include <fstream>
#include <cstring>

using namespace Inpaws;
using namespace std;

SpBuffer* SpBuffer::_spbuffer = NULL;

SpBuffer::SpBuffer()
{
    memset(_memoria, 0, 65536);
}

void
SpBuffer::readBuffer(std::istream& is)
{
    is.read ((char*)_memoria, 65536);
}

void
SpBuffer::writeBuffer (std::ostream& os) const
{
    os.write ((char*)_memoria, 65536);
}

void
SpBuffer::writeBufferTAP (const std::string& nombre)
{
    SpWord mainTop, mainAttr;
    string basename = nombre.substr(0, nombre.find("."));
    // Lo primero, justificamos el nombre a 9 caracteres rellenando con espacios
    string nombreReal = UtilTexto::toUpr(basename.substr(0,9));
    int nombreSz = (9-(int)nombreReal.size());
    for (int i=0; i<nombreSz; i++) nombreReal.append(" ");

    string ficheroStr = basename+".TAP";
    ofstream ofstr(ficheroStr.c_str(), ofstream::out | ofstream::binary);
    if (!ofstr.good())
        throw ExcFichero (ficheroStr+": "+MSG_ARCHIVO_NO_EXISTE);

    mainTop = wordPeek(65533);
    mainAttr = mainTop + 311;
    _saveTapeBlock (ofstr, nombreReal+"A", wordPeek(65533), wordPeek(65517)-1);
    _saveTapeBlock (ofstr, nombreReal+"B", wordPeek(65519), 65535);

}

void
SpBuffer::readBufferDmp(const string& fichero)
{
    ifstream ifstr(fichero.c_str(), ifstream::in | ifstream::binary);
    if (!ifstr.good())
        throw ExcFichero (fichero+": "+MSG_ARCHIVO_NO_EXISTE);

    readBuffer (ifstr);

    if (!ifstr.good())
        throw ExcFichero (fichero+": "+MSG_ARCHIVO_NO_64K);
}

void
SpBuffer::readBufferSNA (const std::string& fichero)
{
    ifstream ifstr(fichero.c_str(), ifstream::in | ifstream::binary);
    if (!ifstr.good())
        throw ExcFichero (fichero+": "+MSG_ARCHIVO_NO_EXISTE);
    ifstr.read ((char*)_memoria, 27); // Guardamos la cabecera en 0 por si acaso
    ifstr.read ((char*)_memoria+16384, 49152);

    if (!ifstr.good())
        throw ExcFichero (fichero+": "+MSG_ARCHIVO_NO_SNA);
}

void
SpBuffer::readBufferZ80 (const std::string& fichero)
{
    // De acuerdo con la especificación sita en http://www.worldofspectrum.org/faq/reference/z80format.htm

    int version;
    SpByte cabecera[87];

    ifstream ifstr(fichero.c_str(), ifstream::in | ifstream::binary);
    if (!ifstr.good())
        throw ExcFichero (fichero+": "+MSG_ARCHIVO_NO_EXISTE);

    // Leemos la cabecera y establecemos la versión
    ifstr.read ((char*)cabecera, 30);
    if (!cabecera[6] && !cabecera[7])
    {
        ifstr.read((char*) cabecera+30, 2); // Tamaño del resto del bloque de cabecera
        int len = cabecera[30] + 0x100*cabecera[31];
        ifstr.read((char*) cabecera+32, len);
        if (len == 23)
            version = 2;
        else if (len == 54 || len == 55)
            version = 3;
        else throw (fichero+": "+MSG_ARCHIVO_NO_Z80_48k);
    } else version = 1;

    // Si no es un volcado de 48k, salimos
    if (((version == 2  && cabecera[34] > 1) ||
        (version == 3 && cabecera[34] > 1 && cabecera[34] != 3)) ||
        (version > 1 && cabecera[37] & 0x80)) // <-- 16k !
    {

        throw ExcFichero(fichero+": "+MSG_ARCHIVO_NO_Z80_48k);
    }

    // Almacenamos el resto del fichero en un buffer, para acceso más facil
    SpByte buffer[49152];
    ifstr.read((char*)buffer, 49152);

    // Por fin, la lectura de los bloques
    if (version == 1)
        _leerBloqueZ80(buffer, 0x4000, 0, 0);
    else
    {
        uint16_t len;
        uint8_t page;
        PtrBuffer dir;
        size_t indiceBuffer = 0;
        for (int j = 0; j < 3; j++) // En 48k guarda 3 páginas
        {
            len = buffer[indiceBuffer] + 0x100 * buffer[indiceBuffer+1];
            page = buffer[indiceBuffer+2];
            indiceBuffer += 3;
            if (page == 4) dir = 0x8000;
            else if (page == 5) dir = 0xC000;
            else if (page == 8) dir = 0x4000;
            else throw (fichero+": "+MSG_ARCHIVO_NO_Z80_48k);
            indiceBuffer = _leerBloqueZ80(buffer, dir, len, indiceBuffer);
        }
    }
}

SpWord
SpBuffer::wordPeek(PtrBuffer ptr) const
{
    return _memoria[ptr] + 0x100*_memoria[ptr+1];
}

void
SpBuffer::wordPoke(PtrBuffer ptr, SpWord word)
{
    _memoria[ptr] = (SpByte)(word & 0x00ff);
    _memoria[ptr+1] = (SpByte)(word >> 8);
}

SpByte
SpBuffer::bytePeek(PtrBuffer ptr) const
{
    return _memoria[ptr];
}

void
SpBuffer::bytePoke(PtrBuffer ptr, SpByte byte)
{
    _memoria[ptr] = byte;
}

char
SpBuffer::charPeek (PtrBuffer ptr) const
{
    return _memoria[ptr]^0xff;
}

void
SpBuffer::charPoke (PtrBuffer ptr, char caracter)
{
    _memoria[ptr] = caracter^0xff;
}

SpBuffer::~SpBuffer()
{
    //dtor
}

void
SpBuffer::_saveTapeBlock (ostream& os, const std::string& nombre,
                          PtrBuffer inicio, PtrBuffer final)
{
    SpByte header[17];
    SpWord length, tmpw;
    SpByte tmpb;

    header[0] = 3;
    strncpy ((char*)header+1, nombre.c_str(), 10);
    length = final-inicio+1;
    // Almacenar la longitud (menos significativo primero)
    header[11] = (SpByte)(length & 0x00ff);
    header[12] = (SpByte)(length >> 8);
    // Almacenar la direccion de inicio (menos sign. primero)
    header[13] = (SpByte)(inicio & 0x00ff);
    header[14] = (SpByte)(inicio >> 8);

    header[15] = 0;
    header[16] = 128;
    tmpw = 19; tmpb = 0;
    os.put(19);
    os.put(0);
    os.put(tmpb);
    os.write ((char*)header, 17);

    for (int i=0; i<17;i++)
        tmpb = tmpb ^ header[i];
    os.put(tmpb);
    tmpw = length+2; tmpb = 255;
    os.put((SpByte(tmpw & 0x00ff)));
    os.put((SpByte(tmpw >> 8)));
    os.put(tmpb);

    os.write((char*)_memoria+inicio, length);
    for (int i=inicio; i<=final; i++) tmpb = tmpb^_memoria[i];
    os.put(tmpb);
}

void
SpBuffer::compararMemoria(const std::string& otroFicheroSNA) const
{
    // Esta funcion compara con otra aventura en SNA
    // Solo las direcciones ocupadas por la aventura actual
    ifstream ifstr(otroFicheroSNA.c_str(), ifstream::in | ifstream::binary);
    if (!ifstr.good())
        throw ExcFichero (otroFicheroSNA+": "+MSG_ARCHIVO_NO_EXISTE);

    SpByte memoriaOtro[65536];

    ifstr.read ((char*)memoriaOtro, 27); // Guardamos la cabecera en 0 por si acaso
    ifstr.read ((char*)memoriaOtro+16384, 49152);

    if (!ifstr.good())
        throw ExcFichero (otroFicheroSNA+": "+MSG_ARCHIVO_NO_SNA);

    for (PtrBuffer ptr = wordPeek(65533); ptr <= wordPeek(65517)-1; ptr++)
    {
        if (_memoria[ptr] != memoriaOtro[ptr])
            cout << ptr << ": [" << (SpWord)_memoria[ptr] << "]Actual "
                 << "[" << (SpWord)memoriaOtro[ptr] << "]Otro" << endl;
    }
    for (PtrBuffer ptr = wordPeek(65519); ptr != 0; ptr++)
    {
        if (_memoria[ptr] != memoriaOtro[ptr])
            cout << ptr << ": [" << (SpWord)_memoria[ptr] << "]Actual "
                 << "[" << (SpWord)memoriaOtro[ptr] << "]Otro" << endl;
    }
}

size_t
SpBuffer::_leerBloqueZ80(const SpByte *buffer, PtrBuffer dir, SpWord len, size_t indiceBuffer)
{
    // Si len es cero, se trata de un version 1 y terminará con la secuencia 00 ED ED 00
    SpByte byteActual;
    bool fin = false;

    while (!fin)
    {
        byteActual = buffer[indiceBuffer];
        switch (byteActual)
        {
            case 0x00:
                if (!len && buffer[indiceBuffer+1] == 0xED &&
                    buffer[indiceBuffer+2] == 0xED &&
                    buffer[indiceBuffer+3] == 0x00) // version 1
                {
                    fin = 1;
                } else _memoria[dir++] = byteActual;
                break;
            case 0xED:
                if (buffer[indiceBuffer+1] == 0xED)
                {
                    for (int i = 0; i < buffer[indiceBuffer+2]; i++)
                    {
                        _memoria[dir++] = buffer[indiceBuffer+3];
                    }
                    indiceBuffer += 3;
                    if (len) len -= 3;
                }
                else _memoria[dir++] = byteActual;
                break;
            default:
                _memoria[dir++] = byteActual;
        };
        indiceBuffer++;
        if (len)
        {
            len--;
            if (len == 0) fin = 1;
        }
    }
    return indiceBuffer;
}
