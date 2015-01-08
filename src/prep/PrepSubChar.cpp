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

#include "prep/PrepSubChar.h"
#include "prep/MsgPrep.h"
#include "prep/Error.h"
#include "UtilTexto.h"
#include "MsgAplicacion.h"
#include "defines.h"

using namespace Inpaws;
using namespace std;

PrepSubChar::PrepSubChar()
    : PrepPart (RSV_SUBCHAR)
{

}

PrepSubChar::~PrepSubChar()
{
}

void
PrepSubChar::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    // No hace nada, lógicamente
}

void
PrepSubChar::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    // El formato es:
    //      SUBCHAR carácter <literal entrecomillado>;
    //      ej: SUBCHAR "á" "{144}";
    UtilTexto utilTexto;
    PrepTokeniser::TTokenType token;
    char nuevoSub(0);
    unsigned long lineaSub(0);
    string archivoSub;
    string defSub;

    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, RSV_SUBCHAR);

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_LITERAL) // carácter a sustituir
    {
        if (utilTexto.compactarTexto(tokeniser.lvalue().literal).length() == 1 &&
            utilTexto.textoCorrecto(tokeniser.lvalue().literal))
        {
            nuevoSub = utilTexto.compactarTexto(tokeniser.lvalue().literal)[0];
            lineaSub = tokeniser.lineaActual();
            archivoSub = tokeniser.archivoActual();
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[" + tokeniser.lvalue().literal + "] "+MSGPREP_SUBLARGO, Error::FATAL);
    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CARACTERSUB), Error::FATAL);

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_LITERAL)
    {
        defSub = tokeniser.lvalue().literal;
        if (defSub.length() == 0 || !utilTexto.textoCorrecto(defSub))
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[\"" + defSub + "\"] "+MSGPREP_CADENASUBINCORRECTA, Error::FATAL);
        }
    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CADENASUB), Error::FATAL);

    saltarToken(tokeniser, PrepTokeniser::TK_CONTROL, ";");

    if (_sustituciones.find(nuevoSub) != _sustituciones.end())
    {
        ERRORPAW(archivoSub, lineaSub, nombre()+" [\""+nuevoSub+"\"] "+MSGPREP_SUBYADEF, Error::FATAL);
    }
    _sustituciones.insert(pair<char, string>(nuevoSub, defSub));
}

void
PrepSubChar::asignarValoresSimbolos(Diccionario& diccionario)
{
    // Esta funcion no hace nada
}

void
PrepSubChar::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    // Esta funcion tampoco hace nada.
}


