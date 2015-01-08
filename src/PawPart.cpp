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

#include "PawPart.h"
#include "Excepcion.h"

using namespace Inpaws;

PawPart::PawPart(PtrBuffer ptrIndice)
    : _ptrIndice(ptrIndice)
{
    //ctor
}

PawPart::~PawPart()
{
    //dtor
}

void
PawPart::saltarToken (Tokeniser& tokeniser,
                      Tokeniser::TTokenType token,
                      const std::string& literal)
{
    Tokeniser::TTokenType tokenRead;

    tokenRead = tokeniser.leeToken();
    if (token != tokenRead ||
        tokeniser.lvalue().literal != literal)
    {
        throw ExcTokeniser (tokeniser.errorLeidoEsperado(literal));
    }
}
