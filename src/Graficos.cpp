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

//--------------------------------------------------------------------
//
// Formato de los graficos en PAW
// 1) Datos del grafico de la localidad 0
// 2) ...
// 3) Datos del grafico de la localidad n
// 4) Indices a los datos de graficos de la localidad (ptrDatos)
// 5) 4 bytes que no tengo ni idea de por qué están ahí, pero
//    que contienen:
//      5.1) Los dos primeros un puntero al inicio de los indices (ptrDatos)
//      5.2) los dos segundos tienen el valor 255
// 6) Datos de los atributos, un byte por localidad
//---------------------------------------------------------------------

#include "Graficos.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"

using namespace Inpaws;
using namespace std;

Graficos::Graficos(PtrBuffer ptrIndice, PtrBuffer ptrAttr)
  : PawPart(ptrIndice), _ptrAttr(ptrAttr)
{
}

Graficos::~Graficos()
{
}


PtrBuffer
Graficos::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    PtrBuffer tempPtr = ptrInicioDatos;
    PtrBuffer tempIndice = ptrInicioDatos;
    // Primero van los datos de los graficos
    for (unsigned int i = 0; i < _datosGraficos.size(); i++)
        SPBUFFER->bytePoke(tempPtr++, _datosGraficos[i]);

    SPBUFFER->wordPoke(_ptrIndice, tempPtr);


    // A continuacion los indices a lo anterior

    for (unsigned int j = 0; j < _tamanoGraficos.size(); j++)
    {
        SPBUFFER->wordPoke(tempPtr, tempIndice);
        tempPtr += 2;
        tempIndice += _tamanoGraficos[j];
    }

    // Ahora volcamos el extraño puntero al comienzo de los indices
    // y los dos octetos con 255
    SPBUFFER->wordPoke(tempPtr, SPBUFFER->wordPeek(_ptrIndice));
    SPBUFFER->bytePoke(tempPtr+2, 255);
    SPBUFFER->bytePoke(tempPtr+3, 255);
    tempPtr += 4;

    // Y por ultimo la lista de atributos

    SPBUFFER->wordPoke(_ptrAttr, tempPtr);

    for (unsigned int k = 0; k < _attr.size(); k++)
    {
        SPBUFFER->bytePoke(tempPtr++, _attr[k]);
    }

    return tempPtr-1;
}

PtrBuffer
Graficos::volcarDeMemoria ()
{
    getInicioDatos();

    _attr.clear();
    _tamanoGraficos.clear();
    _datosGraficos.clear();

    SpByte numLocs = SPBUFFER->bytePeek(SPBUFFER->wordPeek(PTR_MAINTOP) + OFF_NUMLOC);

    // Volcamos los datos de las localidades
    for (PtrBuffer ptr = SPBUFFER->wordPeek(ptrDatos());
         ptr < ptrDatos();
         ptr ++)
    {
        _datosGraficos.push_back(SPBUFFER->bytePeek(ptr));
    }

    // Ahora los tamaños de los graficos, para luego elaborar los indices
    for (PtrBuffer ptrIndices = ptrDatos();
         ptrIndices < ptrDatos()+2*numLocs;
         ptrIndices +=2)
    {
        PtrBuffer ptrActual = SPBUFFER->wordPeek(ptrIndices);
        PtrBuffer ptrSiguiente;
        if (ptrIndices == ptrDatos()+2*(numLocs-1))
        {
            ptrSiguiente = ptrDatos();
        }
        else
        {
            ptrSiguiente = SPBUFFER->wordPeek(ptrIndices+2);
        }

        _tamanoGraficos.push_back(ptrSiguiente - ptrActual);
    }

    // Por ultimo los atributos
    for (PtrBuffer ptrAttrs = SPBUFFER->wordPeek(_ptrAttr);
         ptrAttrs < SPBUFFER->wordPeek(_ptrAttr)+numLocs;
         ptrAttrs ++)
    {
        _attr.push_back(SPBUFFER->bytePeek(ptrAttrs));
    }

    /*PtrBuffer ptrGraficoActual;
    SpByte numArgsActual, byteActual;
    PtrBuffer datosAttr = SPBUFFER->wordPeek(_ptrAttr);

    for (SpByte i = 0; i < numLocs; i++)
    {
        _attr.push_back(SPBUFFER->bytePeek(datosAttr+i));
        ptrGraficoActual = SPBUFFER->wordPeek(ptrDatos()+2*i);
        _ptrGraficos.push_back(ptrGraficoActual);
        byteActual = SPBUFFER->bytePeek(ptrGraficoActual);
        while (byteActual & 7 != 7)
        {
            switch (byteActual & 7)
            {
                case 0:
                case 1:
                    numArgsActual = 2;
                    break;
                case 2:
                    if (byteActual&0x10 && byteActual&0x20) numArgsActual = 3;
                    else if (byteActual&0x10) numArgsActual = 4;
                    else if (byteActual&0x20) numArgsActual = 3;
                    else numArgsActual = 2;
                    break;
                case 3:
                    numArgsActual = 1;
                    break;
                case 4:
                    numArgsActual = 3;
                    break;
                case 5:
                case 6:
                case 7:
                    numArgsActual = 0;
                    break;

            }

            _datosGraficos.push_back(byteActual);
            for (int j=0; j<numArgsActual; j++)
                _datosGraficos.push_back(SPBUFFER->bytePeek(ptrGraficoActual+j+1));
            ptrGraficoActual += numArgsActual+1;

            byteActual = SPBUFFER->bytePeek(ptrGraficoActual);
        }
        _datosGraficos.push_back(byteActual);
    }*/

    // DEBUG ***
//    cout << "Inicio Indices:" << ptrDatos() << endl
//         << "Fin Indices:" << ptrDatos() + 2*numLocs - 1 << endl
//         << "Inicio Attr:" << SPBUFFER->wordPeek(_ptrAttr) << endl
//         << "Fin Attr:" << SPBUFFER->wordPeek(_ptrAttr) + numLocs -1 << endl
//         << "Inicio Datos:" << SPBUFFER->wordPeek(ptrDatos()) << endl
//         << "Fin Datos:" << ptrDatos()-1 << endl;
//    for (PtrBuffer ptr=ptrDatos() + 2*numLocs; ptr < SPBUFFER->wordPeek(_ptrAttr); ptr++)
//        cout << "Dato en " << ptr << ": " << (SpWord)SPBUFFER->bytePeek(ptr) << endl;
    // DEBUG FIN ***

    return SPBUFFER->wordPeek(_ptrAttr) + numLocs - 1;
}

void
Graficos::generarCodigo (std::ostream& os)
{
    os << RSV_GRAPHICS << endl << "{" << endl;

    // Volcamos el numero de graficos
    os << '\t' << _tamanoGraficos.size() << "," << endl << '\t';

    // Luego todos los tamaños de todos los graficos
    for (unsigned int i = 0; i < _tamanoGraficos.size(); i++)
    {
        os << _tamanoGraficos[i] << ",";
        if (i % 10 == 9) os << endl << '\t';
    }
    os << endl << '\t';

    // Despues los atributos
    for (unsigned int i = 0; i < _attr.size(); i++)
    {
        os << (unsigned int)_attr[i] << ",";
        if (i % 10 == 9) os << endl << '\t';
    }
    os << endl << '\t';

    // A continuación finalizamos con el volcado de los datos
    for (unsigned int i = 0; i < _datosGraficos.size(); i++)
    {
        os << (unsigned int)_datosGraficos[i];
        if (i !=  _datosGraficos.size()-1) os << ",";
        if (i % 10 == 9) os << endl << '\t';
    }
    os << endl;

    os << "}" << endl;
}

void
Graficos::leerDeCodigo (Tokeniser& tokeniser)
{
    Tokeniser::TTokenType token;
    SpByte numLocs;

    _attr.clear();
    _tamanoGraficos.clear();
    _datosGraficos.clear();

    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, RSV_GRAPHICS);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");

    token = tokeniser.leeToken();
    if (!(token == Tokeniser::TK_CONTROL && tokeniser.lvalue().literal=="}"))
    {
        if (token == Tokeniser::TK_NUMERO)
            numLocs = tokeniser.lvalue().valor;
        else throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DECIMAL));
        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ",");
        // Lectura de los tamaños
        for (SpByte i = 0; i < numLocs; i++)
        {
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_NUMERO || token == Tokeniser::TK_NUMEROGRANDE)
                _tamanoGraficos.push_back(tokeniser.lvalue().valorGrande);
            else
                throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DECIMAL));
            saltarToken (tokeniser, Tokeniser::TK_CONTROL, ",");
        }
        // Lectura de los atributos
        for (SpByte i = 0; i < numLocs; i++)
        {
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_NUMERO)
                _attr.push_back(tokeniser.lvalue().valor);
            else
                throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DECIMAL));
            saltarToken (tokeniser, Tokeniser::TK_CONTROL, ",");
        }
        // Lectura de los datos
        token = tokeniser.leeToken();
        do
        {
            if (token == Tokeniser::TK_NUMERO)
                _datosGraficos.push_back(tokeniser.lvalue().valor);
            else
                throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DECIMAL));

            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_CONTROL && tokeniser.lvalue().literal == ",")
                token = tokeniser.leeToken();
        } while (!(token == Tokeniser::TK_CONTROL && tokeniser.lvalue().literal=="}"));

        //saltarToken (tokeniser, Tokeniser::TK_CONTROL, "}");
    }
}

void
Graficos::completarLocalidades (SpByte numLocs, SpByte attr)
{
    if (_tamanoGraficos.size() >= numLocs) return;
    SpByte numIter = numLocs - _tamanoGraficos.size();
    for (SpByte i = 0; i < numIter; i++)
    {
        _tamanoGraficos.push_back(1);
        _attr.push_back(attr);
        _datosGraficos.push_back(7);
    }
}
