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

//---------------------------------------------------------------------------------------
//
// Permite realizar diversas acciones sobre los textos: validar códigos de control,
// sustituir abreviaturas, sustituir caracteres regionales, etc..
//---------------------------------------------------------------------------------------

#ifndef UTILTEXTO_H_
#define UTILTEXTO_H_

#include "prep/PrepAbreviaturas.h"
#include <string>
#include <map>
#include <vector>

namespace Inpaws
{
    class UtilTexto
    {
        public:
            enum { CODIGO, CARACTER, FIN, ERROR };
            struct Item
            {
                union { int codigo; char caracter; };
                int tipo;
                bool colorCode;
            };

            UtilTexto();
            ~UtilTexto();

            bool textoCorrecto (const std::string& texto);
            const std::string& errorStr() const { return _errorStr; }; // Descripción del último error encontrado
            int expandirAbreviaturas (std::string& texto, const PrepAbreviable::TDiccionario& abreviaturas);
            int sustituirCaracteres (std::string& texto, const std::map<char, std::string>& diccionario);
            void resetearLectura(size_t posactual = 0) { _posactual = posactual; _colorCodes = 0;}
            Item siguienteItem(const std::string& texto);
            static std::string toUpr(const std::string& texto);
            int colorCodes() const { return _colorCodes; }

            // Las dos funciones siguientes realizan el cambio entre los códigos de control no imprimibles
            // "visibles", por ejemplo la cadena "{17}" a su código real (el dato 17) y viceversa
            // Utilizado en la compresión de cadenas por abreviaturas
            std::string compactarTexto (const std::string& texto); // Compactar texto con códigos de control
            std::string expandirTexto (const std::string& texto,
                                        bool codificarComillas = false);  // Lo Inverso a lo anterior

            // Utilizada en la conversión a formato PAW-PC / CP/M
            std::string filtrarParaSCE(const std::string& texto);
            std::string filtrarParaSuperglus(const std::string& texto, bool cortarLineas = true); // Ajuste 80 cols

        private:
            std::string _errorStr;
            size_t _posactual;
            int _colorCodes;
            void _leerCodigo (Item& item, const std::string& texto);
            std::string _expandirAbreviatura(int codigo, const PrepAbreviable::TDiccionario& abreviaturas);
    };

    class TextoExt
    {
        public:
            TextoExt (const std::string& texto);
            size_t length() const { return _texto.size(); }
            const std::string& operator[](size_t pos) { return _texto[pos]; }
            std::string substr(size_t pos = 0, size_t n = std::string::npos) const;

        private:
            std::vector<std::string> _texto;
    };


}

#endif
