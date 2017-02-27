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

#include "Vocabulario.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include "defines.h"
#include <cstring>

using namespace Inpaws;
using namespace std;

const size_t Vocabulario::superglus_word_size(10);
const size_t Vocabulario::paw_word_size(5);

Vocabulario::TVocabulario::const_iterator Vocabulario::_itVoc;
Vocabulario::TVocabulario::const_iterator Vocabulario::_itVocEnd;

Vocabulario::Vocabulario(PtrBuffer ptrIndice)
: PawPart(ptrIndice)
{
    //ctor
}

Vocabulario::~Vocabulario()
{
    //dtor
}

PtrBuffer
Vocabulario::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    TVocabulario::const_iterator it = _vocabulario.begin();
    PtrBuffer pos = ptrInicioDatos;
    while (it != _vocabulario.end())
    {
        char palabraTmp[6];
        strncpy(palabraTmp, (*it).second.palabraCorta,5);
        transformarEnyeEnBarra(palabraTmp);
        SPBUFFER->charPoke(pos, palabraTmp[0]);
        SPBUFFER->charPoke(pos+1, palabraTmp[1]);
        SPBUFFER->charPoke(pos+2, palabraTmp[2]);
        SPBUFFER->charPoke(pos+3, palabraTmp[3]);
        SPBUFFER->charPoke(pos+4, palabraTmp[4]);

        SPBUFFER->bytePoke(pos+5, (*it).second.codigo);
        SPBUFFER->bytePoke(pos+6, (*it).second.tipo);
        pos += 7;
        it++;
    }
    // Fin de vocabulario (una palabra con todo a cero)
    for (SpByte i=0; i<7; i++) SPBUFFER->bytePoke(pos++, 0);

    // por último fijamos el puntero al inicio de los datos
    SPBUFFER->wordPoke(_ptrIndice, ptrInicioDatos);

    return pos-1;
}

PtrBuffer
Vocabulario::volcarDeMemoria ()
{
    getInicioDatos();              // Fijar desde donde hay que leer

    PtrBuffer ptrtmp = ptrDatos(); // Posicionamos al principio de los datos
    _vocabulario.clear();          // Limpiamos lo anterior

    while (SPBUFFER->bytePeek(ptrtmp) != 0)
    {
        Palabra palabraActual; // Aqui almacenamos la palabra leida
        // Los primeros 5 caracteres son la palabra
        palabraActual.palabraCorta[0] = SPBUFFER->charPeek(ptrtmp);
        palabraActual.palabraCorta[1] = SPBUFFER->charPeek(ptrtmp+1);
        palabraActual.palabraCorta[2] = SPBUFFER->charPeek(ptrtmp+2);
        palabraActual.palabraCorta[3] = SPBUFFER->charPeek(ptrtmp+3);
        palabraActual.palabraCorta[4] = SPBUFFER->charPeek(ptrtmp+4);
        palabraActual.palabraCorta[5] = 0; // fin de cadena

        transformarBarraEnEnye(palabraActual.palabraCorta);
        palabraActual.palabraCompleta = quitaEspacios(palabraActual.palabraCorta);

        // Despues el código
        palabraActual.codigo = SPBUFFER->bytePeek(ptrtmp+5);

        // Y por último el tipo
        palabraActual.tipo = SPBUFFER->bytePeek(ptrtmp+6);

        // Pal multimapa
        _vocabulario.insert(pair<SpByte,Palabra>(palabraActual.codigo, palabraActual));

        ptrtmp += 7; // Pasamos a la siguiente palabra
    }

    // Los siguientes 7 octetos están a cero, pero están
    return ptrtmp+6;
}

void
Vocabulario::generarCodigo (std::ostream& os)
{
    const Palabra* actual = leerPalabraInit();

    os << "VOCABULARY" << endl << "{" << endl;
    while (actual != NULL)
    {
        os << "\t";
        os << (unsigned int)actual->codigo << ", " << tipoAStr(actual->tipo) << ": ";
        os << "\"" << quitaEspacios(actual->palabraCorta) << "\";" << endl;
        actual = leerPalabraNext();
    }
    os << "}" << endl;
}

void
Vocabulario::generarCodigoDisco (std::ostream& os, uint16_t flags)
{
    const Palabra* actual = leerPalabraInit();

    os << "/VOC";
    if (!(flags & VERGLULX)) os << "\t\t; " << RSV_VOCABULARY;
    os << endl;
    while (actual != NULL)
    {
        if ((unsigned int)actual->codigo != 1 && (unsigned int)actual->codigo != 255)
        {

            os << actual->palabra(flags)
               << '\t'
               << (unsigned int)actual->codigo << '\t'
               << ((flags&VERCPM)&&tipoAStr(actual->tipo)=="CONJUNCTION"?"CONJUGATION":tipoAStr(actual->tipo))
               << endl;
        }

        actual = leerPalabraNext();
    }
}

void
Vocabulario::leerDeCodigo (Tokeniser& tokeniser)
{
    // Al tratarse de un código ya preprocesado y supuestamente sin
    // errores, todo lo que no esté bien generará excepción

    Tokeniser::TTokenType token;

    // Primero aparece el literal VOCABULARIO y el "{"
    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, RSV_VOCABULARY);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");

    // Ahora leemos la lista de palabras
    // en formato: Nº, Tipo: "TEXTO" (ej: 131, NOUN: "COCHE")
    Palabra palabra;
    token = tokeniser.leeToken();
    do
    {
        // Codigo

        if (token == Tokeniser::TK_NUMERO)
            palabra.codigo = tokeniser.lvalue().valor;
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_CODIGO_DE_PALABRA));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ",");

        // Tipo
        token = tokeniser.leeToken();
        if (token == Tokeniser::TK_IDENTIFICADOR)
        {
            if (tokeniser.lvalue().literal == MSG_VERB)
                palabra.tipo = VERB;
            else if (tokeniser.lvalue().literal == MSG_ADVERB)
                palabra.tipo = ADVERB;
            else if (tokeniser.lvalue().literal == MSG_NOUN)
                palabra.tipo = NOUN;
            else if (tokeniser.lvalue().literal == MSG_ADJECTIVE)
                palabra.tipo = ADJECTIVE;
            else if (tokeniser.lvalue().literal == MSG_PREPOSITION)
                palabra.tipo = PREPOSITION;
            else if (tokeniser.lvalue().literal == MSG_CONJUNCTION)
                palabra.tipo = CONJUNCTION;
            else if (tokeniser.lvalue().literal == MSG_PRONOUN)
                palabra.tipo = PRONOUN;
            else if (tokeniser.lvalue().literal == MSG_RESERVED)
                palabra.tipo = RESERVED;
            else throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_LITERAL_DE_TIPO_DE_PALABRA));
        }
        else throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_LITERAL_DE_TIPO_DE_PALABRA));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ":");

        // Palabra
        token = tokeniser.leeToken();
        if (token == Tokeniser::TK_LITERAL)
        {
            strncpy(palabra.palabraCorta, llenaespacios5(tokeniser.lvalue().literal).c_str(), 5);
            palabra.palabraCorta[5] = 0;
            palabra.palabraCompleta = tokeniser.lvalue().literal;
        }
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_LITERAL_ENTRECOMILLADO));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ";");

        // Añadimos la palabra a la lista del vocabulario
        _vocabulario.insert (pair<SpByte, Palabra>(palabra.codigo, palabra));

        token = tokeniser.leeToken();
    } while (!(token == Tokeniser::TK_CONTROL &&
               tokeniser.lvalue().literal == "}"));
}

void
Vocabulario::anadirPalabra (SpByte codigo, const std::string& palabra, SpByte tipo)
{
    // No vamos a permitir añadir el verbo 1, ya que en las tablas
    // de procesos y respuestas se hacen indistiguibles de la
    // palabra reservada '*' (creo que es una cagada de PAW)

    if (codigo == 1 && tipo == Vocabulario::VERB)
        throw Excepcion(palabra + ": " + MSG_NO_VERBO_UNO);

    Palabra nuevaPalabra;

    nuevaPalabra.codigo = codigo;
    nuevaPalabra.tipo = tipo;

    // Hay que rellenar con espacios a la derecha
    for (unsigned int i=0; i<5; i++)
    {
        nuevaPalabra.palabraCorta[i] = (i<palabra.size()?palabra[i]:' ');
    }
    nuevaPalabra.palabraCorta[5] = 0;
    nuevaPalabra.palabraCompleta = palabra;
    _vocabulario.insert (pair<SpByte, Palabra>(codigo, nuevaPalabra));
}

const Vocabulario::Palabra*
Vocabulario::leerPalabraInit () const
{
    _itVoc = _vocabulario.begin();
    if (_itVoc != _vocabulario.end())
        return &((*_itVoc).second);
    else
        return NULL;
}

const Vocabulario::Palabra*
Vocabulario::leerPalabraNext () const
{
    _itVoc++;
    if (_itVoc != _vocabulario.end())
        return &((*_itVoc).second);
    else return NULL;
}

const Vocabulario::Palabra*
Vocabulario::leerPalabraInit (SpByte codigo, SpByte tipo) const
{
    pair<TVocabulario::const_iterator, TVocabulario::const_iterator> range = _vocabulario.equal_range(codigo);
    if (range.first == range.second)
        return NULL;

    _itVoc = range.first;
    _itVocEnd = range.second;

    while ((*_itVoc).second.tipo != tipo)
    {
        _itVoc++;
        if (_itVoc == _itVocEnd) return NULL;
    }
    return &((*_itVoc).second);
}

const Vocabulario::Palabra*
Vocabulario::leerPalabraNext (SpByte codigo, SpByte tipo) const
{
    _itVoc++;
    if (_itVoc == _itVocEnd) return NULL;

    while ((*_itVoc).second.tipo != tipo)
    {
        _itVoc++;
        if (_itVoc == _itVocEnd) return NULL;
    }
    return &((*_itVoc).second);
}

string
Vocabulario::tipoAStr(SpByte tipo)
{
    switch (tipo)
    {
        case 0:
            return MSG_VERB;
        case 1:
            return MSG_ADVERB;
        case 2:
            return MSG_NOUN;
        case 3:
            return MSG_ADJECTIVE;
        case 4:
            return MSG_PREPOSITION;
        case 5:
            return MSG_CONJUNCTION;
        case 6:
            return MSG_PRONOUN;
        default:
            return MSG_RESERVED;
    }
}

const std::string
Vocabulario::quitaEspacios(const char* palabra)
{
    const char* ptr = strchr(palabra, ' ');
    if (ptr)
        return string(palabra).substr(0, ptr-palabra);
    else
        return string(palabra).substr(0,5);
}

const std::string
Vocabulario::llenaespacios5(const string& palabra)
{
    string palabraNueva;
    palabraNueva.append(palabra);
    if (palabraNueva.length() > 5)
        palabraNueva.resize(5);

    palabraNueva.append(5-palabraNueva.length(), ' ');

    return palabraNueva;
}

void
Vocabulario::transformarEnyeEnBarra(char* palabra)
{
    while (*palabra)
    {
        if (*palabra == 'Ñ')
            *palabra = '\\';
        palabra++;
    }
}

void
Vocabulario::transformarBarraEnEnye(char* palabra)
{
    while (*palabra)
    {
        if (*palabra == '\\')
            *palabra = 'Ñ';
        palabra++;
    }
}
