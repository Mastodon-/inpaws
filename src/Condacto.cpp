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

#include "Condacto.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include "PawCondacts.h"
#include "defines.h"
#include "UtilTexto.h"

using namespace Inpaws;
using namespace std;

Condacto::Condacto(SpByte codigo , const string& nombre,
                   SpByte numParams,
                   int tipo_prm1, SpByte max_valor_prm1,
                   int tipo_prm2, SpByte max_valor_prm2)
: _codigo(codigo), _nombre (nombre), _numParams (numParams),
  _tipo_prm1(tipo_prm1), _max_valor_prm1(max_valor_prm1),
  _tipo_prm2(tipo_prm2), _max_valor_prm2(max_valor_prm2)
{
    //ctor
}

Condacto::~Condacto()
{
    //dtor
}

PtrBuffer
Condacto::volcarAMemoria (PtrBuffer ptrDireccion, const CondactData& dato) const
{
    SPBUFFER->bytePoke(ptrDireccion, _codigo);
    if (_numParams>0)
    {
        if (_tipo_prm1 == VALOR && dato.prm1 > _max_valor_prm1)
          throw Excepcion(_nombre + ":" + MSG_ARG_CONDACTO_MUY_GRANDE);

        ptrDireccion++;
        SPBUFFER->bytePoke(ptrDireccion, dato.prm1);
    }
    if (_numParams>1)
    {
        if (_tipo_prm2 == VALOR && dato.prm2 > _max_valor_prm2)
          throw Excepcion(_nombre + ":" + MSG_ARG_CONDACTO_MUY_GRANDE);

        ptrDireccion++;
        SPBUFFER->bytePoke(ptrDireccion, dato.prm2);
    }

    return ptrDireccion;
}

PtrBuffer
Condacto::volcarDeMemoria (PtrBuffer ptrDireccion, CondactData& dato) const
{
    dato.codigo = SPBUFFER->bytePeek(ptrDireccion);
    if (_numParams>0)
    {
        ptrDireccion++;
        dato.prm1 = SPBUFFER->bytePeek(ptrDireccion);

        if (_tipo_prm1 == VALOR && dato.prm1 > _max_valor_prm1)
          throw Excepcion(_nombre + ":" + MSG_ARG_CONDACTO_MUY_GRANDE);
    }
    if (_numParams>1)
    {
        ptrDireccion++;
        dato.prm2 = SPBUFFER->bytePeek(ptrDireccion);

        if (_tipo_prm2 == VALOR && dato.prm2 > _max_valor_prm2)
          throw Excepcion(_nombre + ":" + MSG_ARG_CONDACTO_MUY_GRANDE);
    }

    return ptrDireccion;
}

void
Condacto::generarCodigo (std::ostream& os, const CondactData& dato) const
{
    const Condacto* condacto = PAWCONDACT(dato.codigo);

    os << condacto->nombre();
    if (condacto->numParams() > 0)
    {
        os << " " << (unsigned int)dato.prm1;
        if (condacto->numParams() > 1)
        {
            os << " " << (unsigned int)dato.prm2;
        }
    }
}

void
Condacto::generarCodigo (std::ostream& os, const CondactData& dato,
                         const Vocabulario* vocabulario, int flagsDeFormato,
                         const Diccionario::TDiccionarioInverso* diccionario) const
{
    const Condacto* condacto = PAWCONDACT(dato.codigo);


    if (diccionario && _tratarCondactoSuperglus(os, dato, diccionario))
        return;

    os << condacto->nombre();

    if (condacto->numParams() > 0)
    {
        if (condacto->numParams()) os << " ";

        if (condacto->nombre() == "ADJECT1" || condacto->nombre() == "ADJECT2" ||
            condacto->nombre() == "ADVERB" || condacto->nombre() == "PREP" ||
            condacto->nombre() == "NOUN2")
        {
            if (dato.prm1 == 255)
                os << "_";
            else if (dato.prm1 == 1)
                os << "*";
            else
            {
                const Vocabulario::Palabra* palabra = NULL;
                if (condacto->nombre() == "ADJECT1" || condacto->nombre() == "ADJECT2")
                    palabra = vocabulario->leerPalabraInit(dato.prm1, Vocabulario::ADJECTIVE);
                else if (condacto->nombre() == "ADVERB")
                    palabra = vocabulario->leerPalabraInit(dato.prm1, Vocabulario::ADVERB);
                else if (condacto->nombre() == "PREP")
                    palabra = vocabulario->leerPalabraInit(dato.prm1, Vocabulario::PREPOSITION);
                else if (condacto->nombre() == "NOUN2")
                    palabra = vocabulario->leerPalabraInit(dato.prm1, Vocabulario::NOUN);

                os << palabra->palabra(flagsDeFormato);
            }
        }
        else _printNombreOValor(os, dato.prm1, tipo_prm1(), diccionario);

        if (condacto->numParams() > 1)
        {
            if (condacto->nombre() != "MODE" || !(flagsDeFormato&(VERPC+VERCPM+VERGLULX)))
            {
                os << " ";
                _printNombreOValor(os, dato.prm2, tipo_prm2(), diccionario);
            }
        }
    }
}

void
Condacto::leerDeCodigo (Tokeniser& tokeniser, CondactData& dato) const
{
    // Codigo del condacto
    Tokeniser::TTokenType token = tokeniser.leeToken();
    if (token != Tokeniser::TK_IDENTIFICADOR)
        throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NOMBRE_CONDACTO));
    else
        dato.codigo = PAWCONDACT(tokeniser.lvalue().literal)->codigo();

    // Primer parametro
    if (PAWCONDACT(dato.codigo)->numParams() > 0)
    {
        token = tokeniser.leeToken();
        if (token != Tokeniser::TK_NUMERO)
        {
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_PARAMETRO_CONDACTO));
        }
        else if (PAWCONDACT(dato.codigo)->tipo_prm1() == VALOR &&
                 tokeniser.lvalue().valor > PAWCONDACT(dato.codigo)->max_valor_prm1())
        {
            throw ExcTokeniser(PAWCONDACT(dato.codigo)->nombre() + ":" + MSG_ARG_CONDACTO_MUY_GRANDE);
        }
        else
            dato.prm1 = tokeniser.lvalue().valor;
    }

    // Segundo parametro
    if (PAWCONDACT(dato.codigo)->numParams() > 1)
    {
        token = tokeniser.leeToken();
        if (token != Tokeniser::TK_NUMERO)
        {
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_PARAMETRO_CONDACTO));
        }
        else if (PAWCONDACT(dato.codigo)->tipo_prm2() == VALOR &&
                 tokeniser.lvalue().valor > PAWCONDACT(dato.codigo)->max_valor_prm2())
        {
            throw ExcTokeniser(PAWCONDACT(dato.codigo)->nombre() + ":" + MSG_ARG_CONDACTO_MUY_GRANDE);
        }
        else
            dato.prm2 = tokeniser.lvalue().valor;
    }
}

void
Condacto::_printNombreOValor(ostream& os, SpByte prm, int tipo, const Diccionario::TDiccionarioInverso* diccionario) const
{
    if (!diccionario)
        os << (unsigned int)prm;
    else
    {
        Diccionario::TDiccionarioInverso::const_iterator it = diccionario->find(pair<int, int>(prm, tipo));
        if (it == diccionario->end() || tipo == VALOR)
            os << (unsigned int)prm;
        else
            os << (*it).second;
    }
}

bool
Condacto::_tratarCondactoSuperglus (std::ostream& os, const CondactData& dato,
                                    const Diccionario::TDiccionarioInverso* diccionario) const
{
    UtilTexto utilTexto;
    if (nombre() == "MES" || nombre() == "MESSAGE")
    {
        Diccionario::TDiccionarioInverso::const_iterator it = diccionario->find(pair<int, int>(dato.prm1, MESNO));
        if (it != diccionario->end())
        {
            if ((*it).second[0] == '\"')
            {
                os << "WRITE " << utilTexto.filtrarParaSuperglus((*it).second, false);
                if (nombre() == "MESSAGE") os << endl << "\t\tNEWLINE";
            }
            else
            {
                os << nombre() << " " << (*it).second;
            }
        }
        else
        {
            os << nombre() << " " << (unsigned int)dato.prm1;
        }
        return true;
    }
    else if (nombre() == "SYSMESS")
    {
        Diccionario::TDiccionarioInverso::const_iterator it = diccionario->find(pair<int, int>(dato.prm1, SYSNO));
        if (it != diccionario->end())
        {
            if ((*it).second[0] == '\"')
            {
                os << "WRITE " << utilTexto.filtrarParaSuperglus((*it).second, false);
            }
            else
            {
                //os << nombre() << " " << (*it).second;
                os << nombre() << " " << (unsigned int)dato.prm1; // Los Sysmess no admiten nombres en superglus
            }
        }
        else
        {
            os << nombre() << " " << (unsigned int)dato.prm1;
        }
        return true;
    }
    else if (nombre() == "EQ" && (dato.prm1==56 || dato.prm1==57)) // Contenedores/prendas
    {
        // En superglus los flags 56 y 57 tienen otro significado, hay que traducir
        if (dato.prm2)
            os << "BNOTZERO ";
        else
            os << "BZERO ";

        os << "56 ";

        if (dato.prm1 == 56) // Contenedor
            os << "2";
        else                 // Prenda
            os << "1";

        return true;
    }
    else if ((nombre() == "ZERO" || nombre() == "NOTZERO") && (dato.prm1==56 || dato.prm1==57))
    {
        // Igual que el anterior
        if (nombre()=="ZERO")
            os << "BZERO ";
        else
            os << "BNOTZERO ";

        os << "56 ";

        if (dato.prm1 == 56) // Contenedor
            os << "2";
        else                 // Prenda
            os << "1";

        return true;
    }

    return false;
}
