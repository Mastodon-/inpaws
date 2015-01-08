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

#include "prep/PrepConstantes.h"
#include "prep/MsgPrep.h"
#include "prep/Error.h"
#include "MsgAplicacion.h"
#include "defines.h"

using namespace Inpaws;
using namespace std;

PrepConstantes::PrepConstantes()
    : PrepPart (RSV_CONSTANT)
{

}

PrepConstantes::~PrepConstantes()
{
}

void
PrepConstantes::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    // No hace nada, lógicamente
}

void
PrepConstantes::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    // El formato es:
    //      CONSTANT nombre <valor 0-255>;
    //      Ej. CONSTANT mivalor 17;

    PrepTokeniser::TTokenType token;
    string nuevaConstante;
    SpByte valorConstante(0);

    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, RSV_CONSTANT);

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_IDENTIFICADOR) // nombre de la constante
    {
        if (diccionario.buscarEntrada(tokeniser.lvalue().literal, VALOR) == -2 &&
            diccionario.buscarEntrada(tokeniser.lvalue().literal, WORD) == -2 &&
            diccionario.buscarEntrada(tokeniser.lvalue().literal, LOCNO) == -2 &&
            diccionario.buscarEntrada(tokeniser.lvalue().literal, OBJNO) == -2)
        {
            nuevaConstante = tokeniser.lvalue().literal;
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), nombre()+" [\""+tokeniser.lvalue().literal+"\"] "+MSGPREP_YADEFINIDO, Error::FATAL);
    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_NOMBRECONST), Error::FATAL);

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_NUMERO) // Valor para la constante
    {
        valorConstante = tokeniser.lvalue().valor;
    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_VALORCONST), Error::FATAL);

    saltarToken(tokeniser, PrepTokeniser::TK_CONTROL, ";");

    diccionario.anadirEntrada(nuevaConstante, VALOR, valorConstante);
}

void
PrepConstantes::asignarValoresSimbolos(Diccionario& diccionario)
{
    // Esta funcion no hace nada
}

void
PrepConstantes::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    // Esta funcion tampoco hace nada.
}



