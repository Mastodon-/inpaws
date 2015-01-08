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

#include "Tokeniser.h"
#include "MsgAplicacion.h"
#include <sstream>
#include <algorithm>

using namespace Inpaws;
using namespace std;

Tokeniser::Tokeniser(istream* is)
: _putback(false), _lastToken(TK_NULL), _is(is),
  _lineaActual(1)
{
    //ctor
}

Tokeniser::~Tokeniser()
{
    //dtor
}

Tokeniser::TTokenType
Tokeniser::leeToken()
{
    if (_putback)
    {
        _putback = false;
        return _lastToken;
    }

    char c;
    _saltarEspacios();
    if (_is->eof()) _lastToken = TK_NULL;

    _lvalue.literal.erase();
    _is->get(c);

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c<='Z'))
    {
        // Identificador
        do
        {
            _lvalue.literal.append(1,c);
            _is->get(c);
        } while (!_is->eof() &&
                 ((c >= 'a' && c<='z') || (c >= 'A' && c<='Z') ||
                 (c >= '0' && c<='9') || c == '_' || c=='Ñ' || c=='ñ'));
        if (!_is->eof())
        {
            _is->putback(c);
        }
        _lastToken = TK_IDENTIFICADOR;
    }
    else if (c >= '0' && c <= '9')
    {
        // Numero
        do
        {
            _lvalue.literal.append(1,c);
            _is->get(c);
        } while (!_is->eof() && (c >= '0' && c <= '9'));

        if (!_is->eof() && (
            (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            /*|| _lvalue.literal.size() > 5*/))
        {
            _is->putback(c);
            // No se permite letras tras los números
            _lastToken = TK_ERROR;
        } else
        {
            unsigned long val = strtoul(_lvalue.literal.c_str(), NULL, 10);
            if (_lvalue.literal.length() > 5 || val > 65535 )
            {
                _is->putback(c);
                // Numero de vocabulario, número mayor de 65535 usado como palabra de vocabulario
                _lastToken = TK_NUMEROVOCABULARIO;
            }
            else if (val > 255)
            {
                _is->putback(c);
                // Numero grande
                _lvalue.valorGrande = val;
                _lastToken = TK_NUMEROGRANDE;
            }
            else
            {
                _is->putback(c);
                _lvalue.valor = (SpByte)val;
                _lvalue.valorGrande = val;
                _lastToken = TK_NUMERO;
            }
        }
    }
    else if (c == '\"')
    {
        // Literal de cadena
        do
        {
            _is->get(c);
            if (!_is->eof())
            {
                if (c=='\\')
                {
                    _is->get(c);
                    if (!_is->eof())
                    {
                        if (c == '\"')
                        {
                            _lvalue.literal.append(1, c);
                            _is->get(c);
                        }
                        else _lvalue.literal.append(1,'\\');
                    } else break;
                } else if (c == '\n')
                {
                    c = ' '; // Nueva linea se considera espacio
                    _lineaActual++;
                }

                if (c != '\"') _lvalue.literal.append(1, c);
            }
        } while (!_is->eof() && c != '\"');

        if (!_is->eof())
            _lastToken = TK_LITERAL;
        else
            _lastToken = TK_ERROR;
    }
    else if (isprint(c))
    {
        _lvalue.literal.append(1,c);
        _lastToken = TK_CONTROL;
    }

    return _lastToken;
}

string
Tokeniser::errorLeidoEsperado (const string& esperado) const
{
    ostringstream strTmp;

    strTmp << /*lineaActual() << ": " <<*/ esperado << " "
           << MSG_PALAB_RESERV_NO_ENCONTRADA;
    if (eof() && _lastToken == TK_NULL)
        strTmp << MSG_FIN_DE_ARCHIVO;
    else
        strTmp << "<" << lvalue().literal << ">";

    return strTmp.str();
}

void
Tokeniser::_saltarEspacios()
{
    if (_putback)
    {
        return;
    }

    char c;
    bool fin = false;


    while (!_is->eof() && !fin)
    {
        _is->get(c);
        if (!_is->eof())
        {
            if (!(c ==' ' || c == '\t' || c == '\r' || c == '\n' || c == '/'))
                fin = true;
            else if (c == '\n') _lineaActual++;
            else if (c == '/')
            {
                _is->get(c);
                if (!_is->eof() && c == '/')
                {
                    while (!_is->eof() && c != '\n') _is->get(c);
                    _lineaActual++;
                } else
                {
                    _is->unget();
                    fin = true;
                }
            }
        }
    }
    if (!_is->eof()) _is->putback(c);
}

void
Tokeniser::imprimirLiteral (const std::string& literal, ostream& os)
{
    // El tokeniser no guarda los caracteres de escape para las comillas, asi
    // que hay que restaurarlos al pintar (usado por el preprocesador)
    for (size_t i = 0; i < literal.length(); i++)
    {
        if (literal[i] == '\"')
            os << "\\\"";
        else
            os << literal[i];
    }
}

void
Tokeniser::_saltarLinea()
{
    char c;
    _is->get(c);
    while (!_is->eof() && c != '\n')
    {
        _is->get(c);
    }
    _lineaActual++;
}
