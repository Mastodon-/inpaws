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

#include "prep/PrepListaDatos.h"
#include "prep/MsgPrep.h"
#include "prep/Error.h"

using namespace Inpaws;
using namespace std;

PrepListaDatos::PrepListaDatos(const std::string& nombre)
: PrepPart(nombre)
{
}

PrepListaDatos::~PrepListaDatos()
{
}

void
PrepListaDatos::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    // Superchapuza de const a no-const, pero era lo más corto
    PawPart* modulo = (PawPart*)aventura.getModulo(nombre());

    modulo->generarCodigo(os);
    os << endl;
}

void
PrepListaDatos::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    // Solo se permite un bloque GRAPHICS/CHARACTERES
    if (size())
        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSGPREP_NOREDEFDATOS, Error::FATAL);

    PrepTokeniser::TTokenType token;
    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, nombre());
    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}")
    {
        // No se permiten bloques vacíos
        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSGPREP_NODATOSVACIO, Error::FATAL);
    }
    while(!(token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
        if (token == PrepTokeniser::TK_NUMERO)
        {
            _datos.push_back(tokeniser.lvalue().valor);
        }
        else if (token == PrepTokeniser::TK_NUMEROGRANDE)
        {
            _datos.push_back(tokeniser.lvalue().valorGrande);
        }
        else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGONUM), Error::FATAL);

        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == ",")
            token = tokeniser.leeToken();
    }
}

void
PrepListaDatos::asignarValoresSimbolos(Diccionario& diccionario)
{
    // Esta funcion no hace nada, los graficos y los caracteres no permiten definir simbolos
}

void PrepListaDatos::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    const int datosPorLinea = 10;
    os << nombre() << endl << "{" << endl;
    for (size_t i = 0; i < _datos.size(); i++)
    {
        if (i % datosPorLinea == 0)
            os << '\t';

        os << _datos[i];
        if (i != _datos.size()-1) os << ", ";

        if (i % datosPorLinea == (datosPorLinea-1))
            os << endl;
    }
    os << endl << "}" << endl << endl;
}
