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
// Una clase Abreviable debe proporcionar una función, llamada por el compresor
// que devuelva todas las cadenas que contiene, así como otra función que
// sustituya en todas los cadenas, los códigos ya abreviados
//
//-------------------------------------------------------------------

#ifndef ABREVIABLE_H_
#define ABREVIABLE_H_

#include <vector>
#include <string>
#include <map>
#include "SpBuffer.h"

namespace Inpaws
{
    class PrepAbreviable
    {
        public:
            typedef std::vector<std::string> TListaCadenas;
            typedef std::map<SpByte, std::string> TDiccionario;
            typedef std::map<std::string, unsigned long> TFrecuencia;

            virtual TListaCadenas leerCadenas() const = 0;

            // Devuelve el número de bytes ahorrados
            virtual SpWord sustituirCadenas (const TDiccionario& diccionario) = 0;

            // Funciones de apoyo a las clases heredadas
            static void anadirCadena(TListaCadenas& lista, const std::string& cadena);
            static std::string sustituirCadena (const TDiccionario& diccionario,
                                                const std::string& cadenaIn,
                                                TFrecuencia* calcularFrecuencias = NULL);
            static std::string sustituirCadenaAlg1 (const TDiccionario& diccionario,
                                                    const std::string& cadenaIn,
                                                    TFrecuencia* calcularFrecuencias = NULL);
            static std::string sustituirCadenaAlg2 (const TDiccionario& diccionario,
                                                    const std::string& cadenaIn,
                                                    TFrecuencia* calcularFrecuencias = NULL);
            static std::string sustituirCadenaAlg3 (const TDiccionario& diccionario,
                                                    const std::string& cadenaIn,
                                                    TFrecuencia* calcularFrecuencias = NULL);
            static void incluirFrecuencias (const TFrecuencia& calculada, TFrecuencia* frecuencias);
            static void sumarFrecuencia (const std::string& cadena, TFrecuencia& frecuencias);
            virtual ~PrepAbreviable() {}
    };
}

#endif
