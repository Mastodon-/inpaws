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

#include "prep/PrepTokeniser.h"
#include "MsgAplicacion.h"
#include "prep/MsgPrep.h"
#include "Excepcion.h"
#include "UtilTexto.h"
#include <sstream>

using namespace Inpaws;
using namespace std;

PrepTokeniser::PrepTokeniser(std::ifstream* is, const string& nombre)
: Tokeniser(is)
{
    Archivo nuevoArchivo (nombre, is);
    _archivosAbiertos.push_front(nuevoArchivo);
}

PrepTokeniser::~PrepTokeniser()
{
    while (_archivosAbiertos.size() > 1)
    {
        delete topFile().stream;
        _archivosAbiertos.erase(_archivosAbiertos.begin());
    }
}

PrepTokeniser::TTokenType
PrepTokeniser::leeToken()
{
    if (_putback)
    {
        _putback = false;
        return _lastToken;
    }

    char c;
    _saltarEspacios();

    while/*if*/ (_is->eof())
    {
        cerrarArchivoActual();
        _saltarEspacios();
        if (_lastToken == TK_NULL && _archivosAbiertos.empty())
            return _lastToken;
    }

    _lvalue.literal.erase();
    _is->get(c);
    if (c == '#')
    {
        procesarDirectivaPreprocesador();
        if (_lastToken != TK_ERROR)
            return leeToken(); // Llamada recursiva, por si nos encontramos un include al principio
        else return _lastToken;
    }
    else
    {
        _is->putback(c);
        Tokeniser::leeToken();
        if (_lastToken == TK_NULL)
        {
            cerrarArchivoActual();
            if (_archivosAbiertos.empty())
                return _lastToken;
            else
                return leeToken(); // Llamada recursiva, por si nos encontramos un include al principio
        }
        else return _lastToken;
    }
}

std::string
PrepTokeniser::errorLeidoEsperadoPrep (const std::string& esperado) const
{
    ostringstream ostr;
    ostr << archivoActual() << ":" << lineaActual() << ": " << esperado << " " << MSG_PALAB_RESERV_NO_ENCONTRADA
       << lvalue().literal;
    return ostr.str();
}

void
PrepTokeniser::cerrarArchivoActual()
{
    // Si hay algun #ifdef abierto, soltamos excepcion
    if (topFile().bloqueIfActivo)
    {
        throw ExcTokeniser(topFile().bloqueIfLinea, archivoActual(), MSGPREP_IFSINEND);
    }

    ifstream* archivoACerrar = topFile().stream;
    archivoACerrar->close();
    _archivosAbiertos.erase(_archivosAbiertos.begin());

    if (_archivosAbiertos.empty())
        _lastToken = TK_NULL;
    else
    {
        _is = topFile().stream; // apuntamos al nuevo archivo
        _lineaActual = topFile().lineaActual;
        delete archivoACerrar; // Y borramos la memoria asignada (del primero no, por que no es nuestro)
    }
}

void
PrepTokeniser::procesarDirectivaPreprocesador()
{
    TTokenType token = Tokeniser::leeToken();
    if (token == TK_IDENTIFICADOR &&
        UtilTexto::toUpr(lvalue().literal) == RSV_INCLUDE)
    {
        abrirNuevoArchivo();
    }
    else if (token == TK_IDENTIFICADOR &&
             (UtilTexto::toUpr(lvalue().literal) == RSV_IFDEF ||
              UtilTexto::toUpr(lvalue().literal) == RSV_IFNDEF))
    {
        leerSi(UtilTexto::toUpr(lvalue().literal));
    }
    else if (token == TK_IDENTIFICADOR &&
             UtilTexto::toUpr(lvalue().literal) == RSV_ENDIF)
    {
        if (topFile().bloqueIfActivo)
            topFile().bloqueIfActivo = false;
        else
        {
            throw ExcTokeniser(lineaActual(), archivoActual(), string("[#")+RSV_ENDIF+"] "+MSGPREP_ENDSINIF);
        }
    }
    else if (token == TK_IDENTIFICADOR &&
             UtilTexto::toUpr(lvalue().literal) == RSV_DEFINE)
    {
        define();
    }
    else
    {
        _lastToken = TK_ERROR;
        _lvalue.literal = "#" + lvalue().literal;
    }
}

void
PrepTokeniser::abrirNuevoArchivo()
{
    // Lanzará una excepción si no puede encontrar el nuevo archivo, o si hay anidamiento de includes
    //_saltarEspacios();
    if (_is->eof())
    {
        throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_FINARCHIVO);
    }

    // Leemos lo que debería ser un literal de cadena
    Tokeniser::leeToken();
    if (_lastToken != TK_LITERAL)
    {
        if (_lastToken == TK_NULL)
        {
            throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_FINARCHIVO);
        }
        else throw ExcTokeniser(errorLeidoEsperadoPrep(MSGPREP_NOMBREINCLUDE));
    }
    if (!comprobarNombre(lvalue().literal))
    {
        throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_NOPATH);
    }

    string nombreNuevoArchivo = obtenerRuta((*_archivosAbiertos.begin()).nombre) + lvalue().literal;

    // Comprobamos que el archivo no está ya abierto (sería anidamiento de includes)
    if (abiertoArchivo(nombreNuevoArchivo))
    {
        throw ExcTokeniser (lineaActual(), archivoActual(), string("\"")+lvalue().literal + "\" " + MSGPREP_INCLUDEANIDADO);
    }
    // Por fin, podemos intentar abrir el archivo (con la misma ruta del archivo padre
    ifstream* nuevoArchivo = new ifstream(nombreNuevoArchivo.c_str());
    if (!nuevoArchivo->good())
    {
        throw ExcTokeniser(lineaActual(), archivoActual(), MSG_ERRORFICHERO);
    }

    // Creación del registro con los datos del archivo
    topFile().lineaActual = _lineaActual; // Guardamos la linea del antiguo
    Archivo nuevoRegistroArchivo (nombreNuevoArchivo, nuevoArchivo);
    _archivosAbiertos.push_front(nuevoRegistroArchivo);
    _lineaActual = nuevoRegistroArchivo.lineaActual;
    _is = nuevoRegistroArchivo.stream;
}

std::string
PrepTokeniser::obtenerRuta(const std::string& nombreArchivo)
{
    size_t pos = nombreArchivo.find_last_of("/\\");
    if (pos != string::npos)
        return nombreArchivo.substr(0,pos+1); // Lo devolvemos con el / incluido
    else return "";
}

string
PrepTokeniser::baseName(const std::string& nombreArchivo)
{
    size_t pos = nombreArchivo.find_last_of("/\\");
    if (pos != string::npos)
        return nombreArchivo.substr(pos+1);
    else
        return nombreArchivo;
}

bool
PrepTokeniser::comprobarNombre(const std::string& nombreArchivo)
{
    if (nombreArchivo.find_last_of("/\\") != string::npos)
        return false;
    else
        return true;
}

bool
PrepTokeniser::abiertoArchivo(const std::string& nombreArchivo) const
{
    for (TListaArchivos::const_iterator it=_archivosAbiertos.begin();
         it != _archivosAbiertos.end();
         it++)
    {
        if (UtilTexto::toUpr(nombreArchivo) == UtilTexto::toUpr((*it).nombre))
            return true;
    }
    return false;
}

bool
PrepTokeniser::insertarSimbolo(const std::string& simbolo)
{
    if (_simbolos.count(UtilTexto::toUpr(simbolo)))
        return false;
    else
    {
        _simbolos.insert(UtilTexto::toUpr(simbolo));
        return true;
    }
}

bool
PrepTokeniser::estaDefSimbolo(const std::string& simbolo) const
{
    return (_simbolos.count(UtilTexto::toUpr(simbolo)) != 0);
}

void
PrepTokeniser::leerSi (const std::string directiva)
{
    if (topFile().bloqueIfActivo)
    {
        throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_NODEFANID);
    }

    if (_is->eof())
    {
        throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_FINARCHIVO);
    }

    // Leemos lo que debería ser un identificador
    Tokeniser::leeToken();
    if (_lastToken != TK_IDENTIFICADOR)
    {
        if (_lastToken == TK_NULL)
        {
            throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_FINARCHIVO);
        }
        else throw ExcTokeniser(errorLeidoEsperadoPrep(MSGPREP_DEFINENAME));
    }

    topFile().bloqueIfActivo = true;
    topFile().bloqueIfLinea = lineaActual();

    if ((estaDefSimbolo(lvalue().literal) && directiva == RSV_IFNDEF) ||
        (!estaDefSimbolo(lvalue().literal) && directiva == RSV_IFDEF))
    {
        ignorarBloqueDirectiva();
    }
}

void
PrepTokeniser::ignorarBloqueDirectiva()
{
    Tokeniser::leeToken();
    while (_lastToken != TK_NULL)
    {
        if (_lastToken == TK_CONTROL && lvalue().literal == "#")
        {
            Tokeniser::leeToken();
            if (_lastToken == TK_IDENTIFICADOR && UtilTexto::toUpr(lvalue().literal) == RSV_ENDIF)
            {
                topFile().bloqueIfActivo = false;
                return;
            }
            else if (UtilTexto::toUpr(lvalue().literal) == RSV_IFDEF || UtilTexto::toUpr(lvalue().literal) == RSV_IFNDEF)
            {
                throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_NODEFANID);
            }
            else putback();
        }
        Tokeniser::leeToken();
    }

    throw ExcTokeniser(topFile().bloqueIfLinea, archivoActual(), MSGPREP_IFSINEND);
}

void
PrepTokeniser::define()
{
    if (_is->eof())
    {
        throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_FINARCHIVO);
    }

    // Leemos lo que debería ser un identificador
    Tokeniser::leeToken();
    if (_lastToken != TK_IDENTIFICADOR)
    {
        if (_lastToken == TK_NULL)
        {
            throw ExcTokeniser(lineaActual(), archivoActual(), MSGPREP_FINARCHIVO);
        }
        else throw ExcTokeniser(errorLeidoEsperadoPrep(MSGPREP_DEFINENAME));
    }

    if (estaDefSimbolo(lvalue().literal))
    {
        throw ExcTokeniser(lineaActual(), archivoActual(), string("\"")+lvalue().literal+"\", "+MSGPREP_DEFINEREPE);
    }

    insertarSimbolo(lvalue().literal);
}
