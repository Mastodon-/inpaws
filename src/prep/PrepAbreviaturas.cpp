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

#include "prep/PrepAbreviaturas.h"
#include "Abreviaturas.h"
#include "UtilTexto.h"
#include "MsgAplicacion.h"
#include <set>

using namespace Inpaws;
using namespace std;

PrepAbreviaturas::PrepAbreviaturas()
{
}

PrepAbreviaturas::~PrepAbreviaturas()
{
}

void
PrepAbreviaturas::leerDeAventura (const Aventura& aventura)
{
    _abreviaturas.clear();

    const Abreviaturas* abrev = dynamic_cast<const Abreviaturas*>(aventura.getModulo(RSV_ABBREVIATIONS));
    if (abrev->comprimido())
    {
        for (Abreviaturas::TAbreviaturas::const_iterator it = abrev->abreviaturas().begin();
             it != abrev->abreviaturas().end();
             it++)
        {
            _abreviaturas.insert(PrepAbreviable::TDiccionario::value_type((*it).first, (*it).second));
        }
    }
}

void
PrepAbreviaturas::generarCodigo(std::ostream& os) const
{
    UtilTexto utilTexto;
    os << RSV_ABBREVIATIONS << endl << "{" << endl;
    for (PrepAbreviable::TDiccionario::const_iterator it = _abreviaturas.begin();
         it != _abreviaturas.end();
         it++)
    {
        os << '\t' << (unsigned int)(*it).first << ": \"";
        os << utilTexto.expandirTexto((*it).second, true);
        os << "\";" << endl;
    }
    os << "}" << endl << endl;
}

SpWord
PrepAbreviaturas::calcularAbreviaturas()
{
    UtilTexto utilTexto;
    int numAbrev[] = { -1, -1, 70, 6, 6, 8 }; // Esta es la que trae PAW por defecto
    int numAbrevTanteo[] = { -1, -1, 110, 35, 60, 50 };

    PrepAbreviable::TFrecuencia tmpFrecuencias;
    PrepAbreviable::TListaCadenas listaCadenas;
    PrepAbreviable::TDiccionario diccionario;

    _abreviaturas.clear();

    // Extraer todas las cadenas de localidades, mensajes, sismes y objetos en la lista general
    for (TListaAbreviables::const_iterator it = _abreviables.begin();
         it != _abreviables.end();
         it++)
    {
        PrepAbreviable::TListaCadenas listaTmp = (*it)->leerCadenas();
        copy (listaTmp.begin(),listaTmp.end(),back_inserter(listaCadenas));
    }

    // Almacenar en una estructura todas las subcadenas de 5,4,3 y 2 caracteres
    PrepAbreviable::TFrecuencia dicCad;
    for (PrepAbreviable::TListaCadenas::const_iterator it = listaCadenas.begin();
         it != listaCadenas.end();
         it++)
    {
        const string& texto = *it;
        for (size_t size = 5; size > 1; size--)
        {
            if (texto.length() >= size)
            {
                for (size_t pos = 0; pos < texto.length()-1; pos++)
                {
                    string cadTmp = texto.substr(pos, size);
                    if (cadTmp != "")
                    {
                        // No insertamos si queda cortado un código de color

                        if (!((cadTmp[cadTmp.length()-1] > 15 && cadTmp[cadTmp.length()-1] <=23) ||
                            ((cadTmp[cadTmp.length()-2] == 22 || (cadTmp[cadTmp.length()-2] ==23)))))
                        {
                            if (dicCad.find(cadTmp) == dicCad.end())
                                dicCad.insert(pair<string, unsigned long>(cadTmp, 0));

                            dicCad[cadTmp]++;
                        }
                    }
                }
            }
        }
    }

    // volcamos a una estructura, ordenados por frecuencia
    multimap<unsigned long, string> mapOrd;
    for (PrepAbreviable::TFrecuencia::const_iterator it = dicCad.begin();
         it != dicCad.end();
         it++)
    {
        if ((*it).second > 10)
            mapOrd.insert(pair<unsigned long, string>((*it).second, (*it).first));
    }

    // Creamos un diccionario ficticio, ordenado por frecuencia
    SpByte fakeCode = 0;
    for (size_t size = 5; size > 1; size--)
    {
        int numElem = 0;
        for (multimap<unsigned long, string>::reverse_iterator it = mapOrd.rbegin();
             it != mapOrd.rend();
             it++)
        {
            if ((*it).second.length() == size && numElem++ < numAbrevTanteo[size])
            {
                diccionario.insert(PrepAbreviable::TDiccionario::value_type(fakeCode++, (*it).second));
            }
            if (numElem == numAbrevTanteo[size])
                break;
        }
    }

    // Hacemos la sustitución de tanteo
    for (PrepAbreviable::TListaCadenas::iterator it = listaCadenas.begin();
        it != listaCadenas.end();
        it++)
    {
        PrepAbreviable::sustituirCadena(diccionario, *it, &tmpFrecuencias);
    }

    // Ahora tenemos en tmpFrecuencias las más sustituidas sobre el texto real
    // volvemos a ordenar por frecuencia y nos quedamos con las mejores
    mapOrd.clear();
    for (PrepAbreviable::TFrecuencia::const_iterator it = tmpFrecuencias.begin();
         it != tmpFrecuencias.end();
         it++)
    {
        mapOrd.insert(pair<unsigned long, string>((*it).second, (*it).first));
    }

    string abrev164 ("  ");
    abrev164[0] = 0;
    abrev164[1] = 128 & 127;
    _abreviaturas.insert(PrepAbreviable::TDiccionario::value_type(164, abrev164));
    int abrevActual = 165;

    for (size_t size = 5; size > 1; size--)
    {
        int numElem = 0;
        for (multimap<unsigned long, string>::reverse_iterator it = mapOrd.rbegin();
             it != mapOrd.rend();
             it++)
        {
            if ((*it).second.length() == size && numElem++ < numAbrev[size])
            {
               _abreviaturas.insert(PrepAbreviable::TDiccionario::value_type(abrevActual++, (*it).second));
               //cout << "\"" << utilTexto.expandirTexto((*it).second) << "\"" << endl;
            }
            if (numElem == numAbrev[size]) break;
        }
    }

    // Y sustituimos todos los textos por los abreviados
    if (_abreviaturas.size() < 91) _abreviaturas.clear();
    SpWord ahorro = 0;
    for (TListaAbreviables::iterator it = _abreviables.begin();
        it != _abreviables.end();
        it++)
    {
        ahorro += (*it)->sustituirCadenas(_abreviaturas);
    }
    return ahorro;
}
