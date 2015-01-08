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

#include "PawCondacts.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include "defines.h"

using namespace Inpaws;
using namespace std;

PawCondacts* PawCondacts::_pawCondacts = NULL;

PawCondacts::PawCondacts()
{
    _anadirCondactos();
}

PawCondacts::~PawCondacts()
{
    for (TCondactosCodigo::const_iterator it = _condactosCodigo.begin();
         it != _condactosCodigo.end();
         it++)
    {
        delete (*it).second;
    }
}

const Condacto*
PawCondacts::leeCondacto(SpByte codigo) const
{
    // Buscar un condacto que no existe es un error y lanza excepcion
    TCondactosCodigo::const_iterator condacto = _condactosCodigo.find(codigo);
    if (condacto == _condactosCodigo.end())
        throw Excepcion(MSG_COD_CONDACTO_MUY_GRANDE);
    else
        return (*condacto).second;
}

const Condacto*
PawCondacts::leeCondacto(const std::string& nombre) const
{
    // Buscar un condacto que no existe es un error y lanza excepcion
    TCondactosString::const_iterator condacto = _condactosString.find(nombre);
    if (condacto == _condactosString.end())
        throw Excepcion(nombre + ": " + MSG_CONDACTO_INEXISTENTE);
    else
        return (*condacto).second;
}

bool
PawCondacts::existeCondacto (const std::string& nombre) const
{
    TCondactosString::const_iterator condacto = _condactosString.find(nombre);
    if (condacto == _condactosString.end())
        return false;
    else
        return true;
}

void
PawCondacts::anadirCondacto(SpByte codigo ,
                            const std::string& nombre,
                            SpByte numParams,
                            SpByte tipo_prm1, SpByte max_valor_prm1,
                            SpByte tipo_prm2, SpByte max_valor_prm2)
{
    Condacto* nuevoCondacto =
      new Condacto (codigo, nombre, numParams, tipo_prm1,
                    max_valor_prm1, tipo_prm2, max_valor_prm2);

    _condactosString.insert(pair<string,Condacto*>(nuevoCondacto->nombre(), nuevoCondacto));
    _condactosCodigo.insert(pair<SpByte,Condacto*>(nuevoCondacto->codigo(), nuevoCondacto));
}

void
PawCondacts::_anadirCondactos()
{
    anadirCondacto(0, "AT", 1, LOCNO, 251);
    anadirCondacto(1, "NOTAT", 1, LOCNO, 251);
    anadirCondacto(2, "ATGT", 1, LOCNO, 251);
    anadirCondacto(3, "ATLT", 1, LOCNO, 251);
    anadirCondacto(4, "PRESENT", 1, OBJNO, 255);
    anadirCondacto(5, "ABSENT", 1, OBJNO, 255);
    anadirCondacto(6, "WORN", 1, OBJNO, 255);
    anadirCondacto(7, "NOTWORN", 1, OBJNO, 255);
    anadirCondacto(8, "CARRIED", 1, OBJNO, 255);
    anadirCondacto(9, "NOTCARR", 1, OBJNO, 255);
    anadirCondacto(10, "CHANCE", 1, VALOR, 99);
    anadirCondacto(11, "ZERO", 1, FLAGNO, 255);
    anadirCondacto(12, "NOTZERO", 1, FLAGNO, 255);
    anadirCondacto(13, "EQ", 2, FLAGNO, 255, VALOR, 255);
    anadirCondacto(14, "GT", 2, FLAGNO, 255, VALOR, 255);
    anadirCondacto(15, "LT", 2, FLAGNO, 255, VALOR, 255);
    anadirCondacto(16, "ADJECT1", 1, WORD, 255);
    anadirCondacto(17, "ADVERB", 1, WORD, 255);
    anadirCondacto(18, "INVEN", 0);
    anadirCondacto(19, "DESC", 0);
    anadirCondacto(20, "QUIT", 0);
    anadirCondacto(21, "END", 0);
    anadirCondacto(22, "DONE", 0);
    anadirCondacto(23, "OK", 0);
    anadirCondacto(24, "ANYKEY", 0);
    anadirCondacto(25, "SAVE", 0);
    anadirCondacto(26, "LOAD", 0);
    anadirCondacto(27, "TURNS", 0);
    anadirCondacto(28, "SCORE", 0);
    anadirCondacto(29, "CLS", 0);
    anadirCondacto(30, "DROPALL", 0);
    anadirCondacto(31, "AUTOG", 0);
    anadirCondacto(32, "AUTOD", 0);
    anadirCondacto(33, "AUTOW", 0);
    anadirCondacto(34, "AUTOR", 0);
    anadirCondacto(35, "PAUSE", 1, VALOR, 255);
    anadirCondacto(36, "TIMEOUT", 0);
    anadirCondacto(37, "GOTO", 1, LOCNO, 251);
    anadirCondacto(38, "MESSAGE", 1, MESNO, 255);
    anadirCondacto(39, "REMOVE", 1, OBJNO, 255);
    anadirCondacto(40, "GET", 1, OBJNO, 255);
    anadirCondacto(41, "DROP", 1, OBJNO, 255);
    anadirCondacto(42, "WEAR", 1, OBJNO, 255);
    anadirCondacto(43, "DESTROY", 1, OBJNO, 255);
    anadirCondacto(44, "CREATE", 1, OBJNO, 255);
    anadirCondacto(45, "SWAP", 2, OBJNO, 255, OBJNO, 255);
    anadirCondacto(46, "PLACE", 2, OBJNO, 255, LOCNO_PLUS, 255);
    anadirCondacto(47, "SET", 1, FLAGNO, 255);
    anadirCondacto(48, "CLEAR", 1, FLAGNO, 255);
    anadirCondacto(49, "PLUS", 2, FLAGNO, 255, VALOR, 255);
    anadirCondacto(50, "MINUS", 2, FLAGNO, 255, VALOR, 255);
    anadirCondacto(51, "LET", 2, FLAGNO, 255, VALOR, 255);
    anadirCondacto(52, "NEWLINE", 0);
    anadirCondacto(53, "PRINT", 1, FLAGNO, 255);
    anadirCondacto(54, "SYSMESS", 1, SYSNO, 255);
    anadirCondacto(55, "ISAT", 2, OBJNO, 255, LOCNO_PLUS, 255);
    anadirCondacto(56, "COPYOF", 2, OBJNO, 255, FLAGNO, 255);
    anadirCondacto(57, "COPYOO", 2, OBJNO, 255, OBJNO, 255);
    anadirCondacto(58, "COPYFO", 2, FLAGNO, 255, OBJNO, 255);
    anadirCondacto(59, "COPYFF", 2, FLAGNO, 255, FLAGNO, 255);
    anadirCondacto(60, "LISTOBJ", 0);
    anadirCondacto(61, "EXTERN", 1, VALOR, 255);
    anadirCondacto(62, "RAMSAVE", 0);
    anadirCondacto(63, "RAMLOAD", 1, FLAGNO, 255);
    anadirCondacto(64, "BEEP", 2, VALOR, 255, VALOR, 255);
    anadirCondacto(65, "PAPER", 1, VALOR, 9);
    anadirCondacto(66, "INK", 1, VALOR, 9);
    anadirCondacto(67, "BORDER", 1, VALOR, 7);
    anadirCondacto(68, "PREP", 1, WORD, 255);
    anadirCondacto(69, "NOUN2", 1, WORD, 255);
    anadirCondacto(70, "ADJECT2", 1, WORD, 255);
    anadirCondacto(71, "ADD", 2, FLAGNO, 255, FLAGNO, 255);
    anadirCondacto(72, "SUB", 2, FLAGNO, 255, FLAGNO, 255);
    anadirCondacto(73, "PARSE", 0);
    anadirCondacto(74, "LISTAT", 1, LOCNO_PLUS, 255);
    anadirCondacto(75, "PROCESS", 1, PROCNO, 255);
    anadirCondacto(76, "SAME", 2, FLAGNO, 255, FLAGNO, 255);
    anadirCondacto(77, "MES", 1, MESNO, 255);
    anadirCondacto(78, "CHARSET", 1, VALOR, 255);
    anadirCondacto(79, "NOTEQ", 2, FLAGNO, 255, VALOR, 255);
    anadirCondacto(80, "NOTSAME", 2, FLAGNO, 255, FLAGNO, 255);
    anadirCondacto(81, "MODE", 2, VALOR, 4, VALOR, 3);
    anadirCondacto(82, "LINE", 1, VALOR, 20);
    anadirCondacto(83, "TIME", 2, VALOR, 255, VALOR, 7);
    anadirCondacto(84, "PICTURE", 1, LOCNO, 251);
    anadirCondacto(85, "DOALL", 1, LOCNO_PLUS, 255);
    anadirCondacto(86, "PROMPT", 1, SYSNO, 255);
    anadirCondacto(87, "GRAPHIC", 1, VALOR, 3);
    anadirCondacto(88, "ISNOTAT", 2, OBJNO, 255, LOCNO_PLUS, 255);
    anadirCondacto(89, "WEIGH", 2, FLAGNO, 255, OBJNO, 255);
    anadirCondacto(90, "PUTIN", 2, OBJNO, 255, LOCNO, 251);
    anadirCondacto(91, "TAKEOUT", 2, OBJNO, 255, LOCNO, 251);
    anadirCondacto(92, "NEWTEXT", 0);
    anadirCondacto(93, "ABILITY", 2, VALOR, 255, VALOR, 255);
    anadirCondacto(94, "WEIGHT", 1, FLAGNO, 255);
    anadirCondacto(95, "RANDOM", 1, FLAGNO, 255);
    anadirCondacto(96, "INPUT", 1, VALOR, 7);
    anadirCondacto(97, "SAVEAT", 0);
    anadirCondacto(98, "BACKAT", 0);
    anadirCondacto(99, "PRINTAT", 2, VALOR, 20, VALOR, 31);
    anadirCondacto(100, "WHATO", 0);
    anadirCondacto(101, "RESET", 1, LOCNO, 251);
    anadirCondacto(102, "PUTO", 1, LOCNO_PLUS, 255);
    anadirCondacto(103, "NOTDONE", 0);
    anadirCondacto(104, "AUTOP", 1, LOCNO, 251);
    anadirCondacto(105, "AUTOT", 1, LOCNO, 251);
    anadirCondacto(106, "MOVE", 1, FLAGNO, 255);
    anadirCondacto(107, "PROTECT", 0);
}

bool
PawCondacts::esCondicion (const std::string& nombre) const
{
    TCondactosString::const_iterator condacto = _condactosString.find(nombre);
    if (condacto == _condactosString.end())
        return false;

    SpByte codigo = (*condacto).second->codigo();
    if (codigo <= 17 ||
        codigo == 55 ||
        (codigo >=68 && codigo <=70) ||
        codigo == 76 ||
        codigo == 79 ||
        codigo == 80 ||
        codigo == 88)
        return true;

    return false;
}
