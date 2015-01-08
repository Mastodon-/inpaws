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

//-----------------------------------------------------------
// Un sencillo analizador léxico que interpreta los siguientes
// tokens:
// - Identificador: nombres de variables, palabras reservadas,...
// - Numero: un número decimal entero
// - Literal: cualquier cosa encerrada entre comillas
// - Control: caracteres individuales para el control de
//            flujo de programa ("{", "}", ";")
//-----------------------------------------------------------

#ifndef TOKENISER_H
#define TOKENISER_H

#include <iostream>
#include <string>
#include "SpBuffer.h"

namespace Inpaws
{
    class Tokeniser
    {
        public:
            enum TTokenType {TK_NULL, TK_ERROR, TK_IDENTIFICADOR,
                             TK_NUMERO, TK_LITERAL,TK_CONTROL,
                             TK_NUMEROGRANDE, TK_NUMEROVOCABULARIO};
            struct Tlvalue
            {
                SpByte valor;
                SpWord valorGrande;
                std::string literal;
            };

            Tokeniser(std::istream* is);
            virtual ~Tokeniser();

            // Funciones para leer y comprobar lo leido
            bool eof() const { return _is->eof() && _lastToken == TK_NULL; }
            virtual TTokenType leeToken();
            void putback() { _putback = true; }
            Tlvalue lvalue() const { return _lvalue; }
            unsigned long lineaActual() const { return _lineaActual; }

            // Funciones de ayuda en los posibles mensajes de error
            virtual std::string errorLeidoEsperado (const std::string& esperado) const;

            // Imprimir literales estrecomillados teniendo en cuenta comillas interiores
            static void imprimirLiteral (const std::string& literal, std::ostream& os);

        protected:
            bool _putback;
            TTokenType _lastToken;
            void _saltarEspacios();
            void _saltarLinea();
            std::istream* _is;
            Tlvalue _lvalue;
            unsigned long _lineaActual;

        private:

    };
}

#endif // TOKENISER_H
