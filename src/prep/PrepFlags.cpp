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

#include "prep/PrepFlags.h"
#include "prep/MsgPrep.h"
#include "prep/Error.h"
#include "MsgAplicacion.h"
#include "defines.h"
#include <sstream>

using namespace Inpaws;
using namespace std;

PrepFlags::PrepFlags()
    : PrepPart (RSV_FLAG)
{

}

PrepFlags::~PrepFlags()
{
}

void
PrepFlags::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    // No hace nada, lógicamente
}

void
PrepFlags::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    // El formato es:
    //      FLAG <nombreFlag>; , o bien
    //      FLAG <nombreFlag> <numeroFlag>;

    PrepTokeniser::TTokenType token;
    string nuevoFlag;
    int numeroFlag = -1, lineaFlag(0);
    string ficheroFlag;

    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, RSV_FLAG);

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_IDENTIFICADOR)
    {
        nuevoFlag = tokeniser.lvalue().literal;
        lineaFlag = tokeniser.lineaActual();
        ficheroFlag = tokeniser.archivoActual();
    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_NOMFLAG), Error::FATAL);

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_NUMERO)
    {
        numeroFlag = tokeniser.lvalue().valor;
    } else tokeniser.putback();
    saltarToken(tokeniser, PrepTokeniser::TK_CONTROL, ";");

    if (diccionario.buscarEntrada(nuevoFlag, FLAGNO) != -2)
    {
        ERRORPAW(ficheroFlag, lineaFlag, nombre()+" ["+nuevoFlag+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
    }
    _flags.insert(nuevoFlag);

    if (numeroFlag == -1)
    {
        diccionario.anadirEntrada(nuevoFlag, FLAGNO);
    }
    else
    {
        diccionario.anadirEntrada(nuevoFlag, FLAGNO, numeroFlag);
        anadirPool (numeroFlag);
    }
}

void
PrepFlags::asignarValoresSimbolos(Diccionario& diccionario)
{
    // Definimos flags del 0 al 255
    for (int i = 0; i < 256; i++)
    {
        ostringstream ostr;
        ostr << i;
        diccionario.anadirEntrada(ostr.str(), FLAGNO, (SpByte)i);

        // Los flags 0-10 y 29-59 están reservados y no se asignarán por pool
        if ((i>=0 && i <=10) || (i>=29 && i<=59)) anadirPool(i);
    }

    for (set<string>::const_iterator it = _flags.begin();
         it != _flags.end();
         it++)
    {
        if (diccionario.buscarEntrada(*it, FLAGNO) == -1)
        {
            diccionario.fijarValor(*it, FLAGNO, dameValorPool());
        }
    }

    // No necesita tener definiciones correlativas, se puede definir cualquier flag
}

void
PrepFlags::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    // Esta funcion tampoco hace nada.
}

