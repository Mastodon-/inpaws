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

#include "EntradaProcesos.h"
#include "PawCondacts.h"

using namespace Inpaws;
using namespace std;

EntradaProcesos::EntradaProcesos (SpByte verbo, SpByte nombre)
: _verbo (verbo), _nombre (nombre)
{
}

EntradaProcesos::~EntradaProcesos()
{
    for (TCondactos::const_iterator it = _condactos.begin();
         it != _condactos.end();
         it++)
    {
        delete *it;
    }
}

void
EntradaProcesos::anadirCondacto(Condacto::CondactData* const condacto)
{
    _condactos.push_back(condacto);
}

PtrBuffer
EntradaProcesos::volcarAMemoria (PtrBuffer ptrDireccion) const
{
    // Escribimos a partir de los condactos (los codigos del verbo y
    // el nombre los habrá escrito la clase "ListaProcesos")
    for (TCondactos::const_iterator it = _condactos.begin();
         it != _condactos.end();
         it++)
    {
        ptrDireccion =
          PAWCONDACT((*it)->codigo)->volcarAMemoria(ptrDireccion, *(*it)) + 1;
    }
    SPBUFFER->bytePoke(ptrDireccion, 255);

    return ptrDireccion;
}

PtrBuffer
EntradaProcesos::volcarDeMemoria (PtrBuffer ptrDireccion)
{
    // Leemos a partir de los condactos (los codigos del verbo y
    // el nombre los habrá leido la clase "ListaProcesos")
    while (SPBUFFER->bytePeek(ptrDireccion) != 255)
    {
        Condacto::CondactData* condacto = new Condacto::CondactData;
        ptrDireccion = PAWCONDACT(SPBUFFER->bytePeek(ptrDireccion))->volcarDeMemoria(ptrDireccion,*condacto);
        anadirCondacto(condacto);
        ptrDireccion++;
    }

    return ptrDireccion;
}

void
EntradaProcesos::generarCodigo (std::ostream& os) const
{
    // Aplica lo mismo que en la escritura, no se pinta el verbo y
    // el nombre, lo hará la clase "ListaProcesos"
    for (unsigned int i=0; i<_condactos.size(); i++)
    {
        PAWCONDACT(_condactos[i]->codigo)->generarCodigo(os, *_condactos[i]);
        if (i < _condactos.size()-1) os << " ";
    }
    os << ";";
}

void
EntradaProcesos::generarCodigo (std::ostream& os, const Vocabulario* vocabulario,
                                int flagDeFormato, const Diccionario::TDiccionarioInverso* diccionario) const
{
    for (unsigned int i=0; i<_condactos.size(); i++)
    {
        if (flagDeFormato & (VERCPM|VERPC|VERGLULX))
        {
            if (_condactos[i]->codigo == 65 || // PAPER
                _condactos[i]->codigo == 66 || // INK
                _condactos[i]->codigo == 67 || // BORDER
                _condactos[i]->codigo == 78 || // CHARSET
                _condactos[i]->codigo == 66 || // INK
                _condactos[i]->codigo == 82 || // LINE
                _condactos[i]->codigo == 84 || // PICTURE
                _condactos[i]->codigo == 87 || // GRAPHIC
                _condactos[i]->codigo == 96 || // INPUT
                _condactos[i]->codigo == 97 || // SAVEAT
                _condactos[i]->codigo == 98 || // BACKAT
                _condactos[i]->codigo == 99 || // PRINTAT
                _condactos[i]->codigo == 107 ||// PROTECT
                _condactos[i]->codigo == 64 || // BEEP
                _condactos[i]->codigo == 61)   // EXTERN
                continue;
        }
        if ((flagDeFormato &(VERCPM|VERPC|VERGLULX)) && i > 0) os << "\t\t";
        PAWCONDACT(_condactos[i]->codigo)->generarCodigo(os, *_condactos[i], vocabulario, flagDeFormato, diccionario);
        if (i < _condactos.size()-1) os << " ";
        if (flagDeFormato &(VERCPM|VERPC|VERGLULX)) os << endl;
    }
    if (!(flagDeFormato &(VERCPM|VERPC|VERGLULX))) os << ";";
}

void
EntradaProcesos::leerDeCodigo (Tokeniser& tokeniser)
{
    // Aplica lo mismo que en la lectura de memoria, no se lee el verbo y
    // el nombre, lo hará la clase "ListaProcesos"
    Condacto::CondactData* nuevoCondacto(NULL);
    string literalCondactoActual;
    Tokeniser::TTokenType token = tokeniser.leeToken();
    literalCondactoActual = tokeniser.lvalue().literal;
    tokeniser.putback(); // Devolvemos el token para que lo vuelva a leer el condacto
    do
    {
        nuevoCondacto = new Condacto::CondactData;
        try {
          PAWCONDACT(literalCondactoActual)->leerDeCodigo(tokeniser, *nuevoCondacto);
        }
        catch (...)
        {
            delete nuevoCondacto;
            throw;
        }
        anadirCondacto (nuevoCondacto);
        token = tokeniser.leeToken();
        if (!(token == Tokeniser::TK_CONTROL && tokeniser.lvalue().literal == ";"))
        {
            literalCondactoActual = tokeniser.lvalue().literal;
            tokeniser.putback(); // Devolvemos el token para que lo vuelva a leer el condacto
        }
    } while (!(token == Tokeniser::TK_CONTROL &&
             tokeniser.lvalue().literal == ";"));

    tokeniser.putback(); // Devolvemos el ";" final por mantener el orden
}

void
EntradaProcesos::_borrarDatos()
{
    for (TCondactos::const_iterator it = _condactos.begin();
         it != _condactos.end();
         it++)
    {
        delete *it;
    }
    _condactos.clear();
}
