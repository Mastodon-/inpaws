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

#include "prep/PrepAbreviable.h"
#include "UtilTexto.h"
#include <sstream>

using namespace Inpaws;
using namespace std;

void
PrepAbreviable::anadirCadena(TListaCadenas& lista, const std::string& cadena)
{
    // Añade a la lista todas las cadenas de una cadena entre codigos de control no abrevianbles
    if (!cadena.length()) return;

    UtilTexto utilTexto;
    UtilTexto::Item item = utilTexto.siguienteItem(cadena);
    string strTmp;

    while (item.tipo != UtilTexto::FIN)
    {
        switch (item.tipo)
        {
            case UtilTexto::CARACTER:
                if (item.caracter != '_')
                {
                    strTmp.append(1,item.caracter);
                }
                else if (strTmp.length())
                {
                    lista.push_back(strTmp);

                    strTmp.clear();
                }
                break;
            case UtilTexto::CODIGO:
                if (item.codigo >= 128 && item.codigo <=254) // Cuidado con los bucles
                {
                    if (strTmp.length())
                    {
                        lista.push_back(strTmp);
                        strTmp.clear();
                    }
                }
                else strTmp.append(1, (char)item.codigo);
                break;
        }
        item = utilTexto.siguienteItem(cadena);
    }
    if (strTmp.length())
        lista.push_back(strTmp);
}

string
PrepAbreviable::sustituirCadena (const TDiccionario& diccionario,
                                 const string& cadenaIn,
                                 TFrecuencia* calcularFrecuencias)
{
    UtilTexto utilTexto;
    TFrecuencia freqAlg1, freqAlg2;

    string cadenaPar = utilTexto.compactarTexto(cadenaIn);
    string susCad1 = sustituirCadenaAlg1(diccionario, cadenaPar, &freqAlg1);
    string susCad2 = sustituirCadenaAlg2(diccionario, cadenaPar, &freqAlg2);

    if (susCad1.length() < susCad2.length())
    {
        if (calcularFrecuencias)
        {
            incluirFrecuencias(freqAlg1, calcularFrecuencias);
        }
        return utilTexto.expandirTexto(susCad1);
    }
    else
    {
        if (calcularFrecuencias)
        {
            incluirFrecuencias(freqAlg2, calcularFrecuencias);
        }
        return utilTexto.expandirTexto(susCad2);
    }
}

string
PrepAbreviable::sustituirCadenaAlg1 (const TDiccionario& diccionario,
                                     const string& cadenaIn,
                                     TFrecuencia* calcularFrecuencias)
{
    // Esta versión sustituye cadenas a medida que las encuentra
    UtilTexto utilTexto;
    if (cadenaIn == "") return cadenaIn;
    string cadenaRet;
    bool encontrado;
    size_t pos = 0;

    while (pos < cadenaIn.length())
    {
        // Comprobamos que no rompa un código de color
        if ((pos > 0 && (cadenaIn[pos-1] > 15 && cadenaIn[pos-1] <= 23)) ||
            (pos > 1 && (cadenaIn[pos-2] == 22 || cadenaIn[pos-2] == 23)))
        {
            cadenaRet += cadenaIn[pos];
            pos++;
        }
        else
        {
            encontrado = false;
            for (TDiccionario::const_iterator it = diccionario.begin();
                  it != diccionario.end();
                  it++)
            {
                if ((*it).second.length() <= cadenaIn.length() - pos)
                {
                    string tmp = cadenaIn.substr(pos, (*it).second.length());
                    if (tmp == (*it).second)
                    {
                        cadenaRet.append(1, (*it).first);
                        pos += (*it).second.length();
                        encontrado = true;
                        if (calcularFrecuencias)
                            sumarFrecuencia(tmp, *calcularFrecuencias);
                        break;
                    }
                }
            }
            if (!encontrado)
            {
                cadenaRet += cadenaIn[pos];
                pos++;
            }
        }
    }

    return cadenaRet;
}

string
PrepAbreviable::sustituirCadenaAlg2 (const TDiccionario& diccionario,
                                     const string& cadenaIn,
                                     TFrecuencia* calcularFrecuencias)
{
    // Esta versión de la funcion intenta sustituir por las cadenas más largas posibles...

    UtilTexto utilTexto;
    if (cadenaIn == "") return cadenaIn;

    string cadenaRet = cadenaIn;

    for (TDiccionario::const_iterator it = diccionario.begin();
          it != diccionario.end();
          it++)
    {
        size_t pos = cadenaRet.find((*it).second);
        while (pos != string::npos)
        {
            // Comprobar que no rompemos un código de color
            if ((pos > 0 && (cadenaRet[pos-1] > 15 && cadenaRet[pos-1] <= 23)) ||
                (pos > 1 && (cadenaRet[pos-2] == 22 || cadenaRet[pos-2] == 23)))
            {
                pos = cadenaRet.find((*it).second, pos+1);
            }
            else
            {
                string cadenaTmp = cadenaRet.substr(0, pos);
                cadenaTmp.append(1, (*it).first);
                if (pos+(*it).second.length() < cadenaRet.length())
                    cadenaTmp += cadenaRet.substr(pos+(*it).second.length());
                cadenaRet = cadenaTmp;

                if (pos+(*it).second.length() < cadenaRet.length())
                    pos = cadenaRet.find((*it).second, pos+(*it).second.length());
                else
                    pos = string::npos;

                if (calcularFrecuencias)
                    sumarFrecuencia((*it).second, *calcularFrecuencias);
            }
        }
    }

    return cadenaRet;
}

string
PrepAbreviable::sustituirCadenaAlg3 (const TDiccionario& diccionario,
                                     const string& cadenaIn,
                                     TFrecuencia* calcularFrecuencias)
{
    // Esta versión de la funcion intenta sustituir por las cadenas más cortas posibles...

    UtilTexto utilTexto;
    if (cadenaIn == "") return cadenaIn;

    string cadenaRet = cadenaIn;

    for (TDiccionario::const_reverse_iterator it = diccionario.rbegin();
          it != diccionario.rend();
          it++)
    {
        size_t pos = cadenaRet.find((*it).second);
        while (pos != string::npos)
        {
            // Comprobar que no rompemos un código de color
            if ((pos > 0 && (cadenaRet[pos-1] > 15 && cadenaRet[pos-1] <= 23)) ||
                (pos > 1 && (cadenaRet[pos-2] == 22 || cadenaRet[pos-2] == 23)))
            {
                pos = cadenaRet.find((*it).second, pos+1);
            }
            else
            {
                string cadenaTmp = cadenaRet.substr(0, pos);
                cadenaTmp.append(1, (*it).first);
                if (pos+(*it).second.length() < cadenaRet.length())
                    cadenaTmp += cadenaRet.substr(pos+(*it).second.length());
                cadenaRet = cadenaTmp;

                if (pos+(*it).second.length() < cadenaRet.length())
                    pos = cadenaRet.find((*it).second, pos+(*it).second.length());
                else
                    pos = string::npos;

                if (calcularFrecuencias)
                    sumarFrecuencia((*it).second, *calcularFrecuencias);
            }
        }
    }

    return cadenaRet;
}

void
PrepAbreviable::incluirFrecuencias (const TFrecuencia& calculada, TFrecuencia* frecuencias)
{
    if (!frecuencias) return;

    for (TFrecuencia::const_iterator it = calculada.begin();
         it != calculada.end();
         it++)
    {
        sumarFrecuencia ((*it).first, *frecuencias);
    }
}

void
PrepAbreviable::sumarFrecuencia (const std::string& cadena, TFrecuencia& frecuencias)
{
    TFrecuencia::iterator ptrFreq = frecuencias.find(cadena);
    if (ptrFreq == frecuencias.end())
    {
        ptrFreq = (frecuencias.insert(pair<string, unsigned long>(cadena, 0))).first;
    }
    (*ptrFreq).second++;
}
