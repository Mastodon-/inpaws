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

#include "Conexiones.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include <vector>

using namespace Inpaws;
using namespace std;

Conexiones::TConexiones::const_iterator Conexiones::_itTmp;

Conexiones::Conexiones (PtrBuffer ptrIndice,SpByte numLocs)
  : PawPart(ptrIndice), _numLocs(numLocs)
{
}

Conexiones::~Conexiones()
{
}


PtrBuffer
Conexiones::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    if (!_numLocs)
        throw Excepcion(MSG_NUMLOCS_NO_DEF); // Llamar a setNumLocs antes de volcar

    vector<SpWord> punterosAConexiones;
    pair<TConexiones::const_iterator, TConexiones::const_iterator> range;

    // Volcado de las conexiones, para cada localidad (tenga o no conexiones)
    // Tuplas verbo-destino, codigo 255 al final de las conexiones de cada localidad
    for (SpByte i = 0; i < _numLocs; i++)
    {
        punterosAConexiones.push_back(ptrInicioDatos);
        range = _conexiones.equal_range(i);
        for (TConexiones::const_iterator it = range.first;
             it != range.second;
             it++)
        {
            SPBUFFER->bytePoke(ptrInicioDatos++, (*it).second.palabra);
            SPBUFFER->bytePoke(ptrInicioDatos++, (*it).second.destino);
        }
        SPBUFFER->bytePoke(ptrInicioDatos++, 255);
    }

    // Volcado de los indices a continuación de lo anterior
    // pero antes fijamos el comienzo de los indices en PTR_CONEXIONES
    SPBUFFER->wordPoke (_ptrIndice, ptrInicioDatos);
    for (vector<SpWord>::const_iterator it2 = punterosAConexiones.begin();
         it2 != punterosAConexiones.end();
         it2++)
    {
        SPBUFFER->wordPoke (ptrInicioDatos, *it2);
        ptrInicioDatos += 2;
    }

    return ptrInicioDatos - 1;
}

PtrBuffer
Conexiones::volcarDeMemoria ()
{
    PtrBuffer conexionLoc;

    getInicioDatos();
    setNumLocs(SPBUFFER->bytePeek(SPBUFFER->wordPeek(PTR_MAINTOP) + OFF_NUMLOC));

    for (SpByte i = 0; i < _numLocs; i++)
    {
        conexionLoc = SPBUFFER->wordPeek (ptrDatos() + 2*i);
        while (SPBUFFER->bytePeek(conexionLoc) != 255)
        {
            anadirConexion (i, SPBUFFER->bytePeek(conexionLoc), SPBUFFER->bytePeek(conexionLoc+1));
            conexionLoc += 2;
        }
    }

    return ptrDatos() + 2*_numLocs - 1;
}

void
Conexiones::generarCodigo (std::ostream& os)
{
    os << RSV_CONNECTIONS << endl << "{" << endl;
    for (TConexiones::const_iterator it = _conexiones.begin();
         it != _conexiones.end();
         it++)
    {
        os << '\t';
        os << (unsigned int)(*it).second.localidad << ": ";
        os << (unsigned int)(*it).second.palabra << " TO ";
        os << (unsigned int)(*it).second.destino << ";" << endl;
    }
    os << "}" << endl;
}

void
Conexiones::generarCodigoDisco (std::ostream& os, uint16_t flags, const Vocabulario* vocabulario)
{
    os << "/CON";
    if (!(flags & VERGLULX)) os << "\t; " << RSV_CONNECTIONS; // En superglus SCE no comentamos
    os << endl;
    for (int i=0; i < _numLocs; i++)
    {
        os << "/" << i;
        pair<TConexiones::const_iterator, TConexiones::const_iterator> rango = _conexiones.equal_range(i);
        for (TConexiones::const_iterator it = rango.first;
             it != rango.second;
             it++)
        {
            const Vocabulario::Palabra* palabra =
                (vocabulario->leerPalabraInit((*it).second.palabra, Vocabulario::VERB) != NULL?
                vocabulario->leerPalabraInit((*it).second.palabra, Vocabulario::VERB) :
                vocabulario->leerPalabraInit((*it).second.palabra, Vocabulario::NOUN));

            os << endl << '\t'
               << palabra->palabra(flags)
               << '\t' << (unsigned int)(*it).second.destino;
        }
        os << endl;
    }
}

void
Conexiones::generarCodigoSuperglus (std::ostream& os, uint16_t flags,
                                    const Vocabulario* vocabulario,
                                    Diccionario::TDiccionarioInverso diccionario)
{
    os << "/CON" << endl;
    for (int i=0; i < _numLocs; i++)
    {
        os << "/";
        if (diccionario.count(pair<int,int>(i, LOCNO)))
            os << (*diccionario.find(pair<int,int>(i, LOCNO))).second;
        else
            os << i;
        pair<TConexiones::const_iterator, TConexiones::const_iterator> rango = _conexiones.equal_range(i);
        for (TConexiones::const_iterator it = rango.first;
             it != rango.second;
             it++)
        {
            const Vocabulario::Palabra* palabra =
                (vocabulario->leerPalabraInit((*it).second.palabra, Vocabulario::VERB) != NULL?
                vocabulario->leerPalabraInit((*it).second.palabra, Vocabulario::VERB) :
                vocabulario->leerPalabraInit((*it).second.palabra, Vocabulario::NOUN));

            os << endl << '\t'
               << palabra->palabra(flags)
               << '\t';

            if (diccionario.count(pair<int,int>((unsigned int)(*it).second.destino, LOCNO)))
                os << (*diccionario.find(pair<int,int>((unsigned int)(*it).second.destino, LOCNO))).second;
            else
                os << (unsigned int)(*it).second.destino;
        }
        os << endl;
    }
}

void
Conexiones::leerDeCodigo (Tokeniser& tokeniser)
{
    Conexion nuevaConexion;
    Tokeniser::TTokenType token;

    // OJO: La lectura de codigo borra todas las conexiones
    _conexiones.clear();

    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, RSV_CONNECTIONS);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");
    token = tokeniser.leeToken();
    while (!(token == Tokeniser::TK_CONTROL &&
             tokeniser.lvalue().literal == "}"))
    {
        // Codigo de localidad
        if (token == Tokeniser::TK_NUMERO)
            nuevaConexion.localidad = tokeniser.lvalue().valor;
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NUMERO_LOCALIDAD));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ":");

        // Verbo de direccion
        token = tokeniser.leeToken();
        if (token == Tokeniser::TK_NUMERO)
            nuevaConexion.palabra = tokeniser.lvalue().valor;
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DE_PALABRA));

        // Literal "TO"
        saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, "TO");

        // Destino de la conexion
        token = tokeniser.leeToken();
        if (token == Tokeniser::TK_NUMERO)
            nuevaConexion.destino = tokeniser.lvalue().valor;
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NUMERO_LOCALIDAD));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ";");

        anadirConexion (nuevaConexion.localidad,
                        nuevaConexion.palabra,
                        nuevaConexion.destino);

        token = tokeniser.leeToken();
    }
}

void
Conexiones::anadirConexion (SpByte localidad, SpByte palabra, SpByte destino)
{
    Conexion nuevaConexion;
    nuevaConexion.localidad = localidad;
    nuevaConexion.palabra = palabra;
    nuevaConexion.destino = destino;
    _conexiones.insert(pair<SpByte, Conexion>(localidad, nuevaConexion));
}

const Conexiones::Conexion*
Conexiones::leerConexionInit (SpByte localidad) const
{
    _itTmp = _conexiones.lower_bound(localidad);
    if (_itTmp == _conexiones.upper_bound(localidad))
        return NULL;
    else
        return &((*_itTmp).second);
}

const Conexiones::Conexion*
Conexiones::leerConexionNext (SpByte localidad) const
{
    _itTmp++;
    if (_itTmp == _conexiones.upper_bound(localidad))
        return NULL;
    else
        return &((*_itTmp).second);
}

PtrBuffer
Conexiones::inicioReal() const
{
    return SPBUFFER->wordPeek(ptrDatos());
}
