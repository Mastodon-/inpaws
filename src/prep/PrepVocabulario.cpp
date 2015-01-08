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

#include "prep/PrepVocabulario.h"
#include "prep/Error.h"
#include "prep/MsgPrep.h"
#include "Vocabulario.h"
#include "MsgAplicacion.h"
#include "defines.h"
#include "UtilTexto.h"

using namespace Inpaws;
using namespace std;

PrepVocabulario::PrepVocabulario(size_t tamPalabra)
  : PrepPart(RSV_VOCABULARY), _tamPalabra(tamPalabra)
{
    // El pool de verbos comienza en el 20
    _pooles[Vocabulario::VERB] = new Pool (20, 254);
    // El de nombres en el 50
    _pooles[Vocabulario::NOUN] = new Pool (50, 254);

    // El resto en el 2 (el uno da problemas, al confundirlos el editor con el comodin *)
    _pooles[Vocabulario::ADVERB] = new Pool (2, 254);
    _pooles[Vocabulario::ADJECTIVE] = new Pool (2, 254);
    _pooles[Vocabulario::PREPOSITION] = new Pool (2, 254);
    _pooles[Vocabulario::CONJUNCTION] = new Pool (2, 254);
    _pooles[Vocabulario::PRONOUN] = new Pool (2, 254);
}

PrepVocabulario::~PrepVocabulario()
{
    for (int i=0; i<7; i++)
        delete _pooles[i];
}


void
PrepVocabulario::generarCodigoPrep (ostream& os, const Aventura& aventura) const
{
    // Obtenemos puntero al vocabulario
    const Vocabulario* vocabulario = dynamic_cast<const Vocabulario*>(aventura.getModulo(RSV_VOCABULARY));

    os << RSV_VOCABULARY << endl << "{" << endl;
    for (SpByte codigo = 1; codigo < 255; codigo++)
    {
        for (SpByte tipo = Vocabulario::VERB; tipo <= Vocabulario::PRONOUN; tipo++)
        {
            const Vocabulario::Palabra* palabraActual = vocabulario->leerPalabraInit(codigo, tipo);
            if (palabraActual != NULL)
            {
                os << '\t';
                os << vocabulario->tipoAStr(tipo) << " "
                   << (SpWord)palabraActual->codigo << ": ";
                do
                {
                    os << "\"" << palabraActual->palabraCompleta << "\"";
                    palabraActual = vocabulario->leerPalabraNext(codigo, tipo);
                    if (palabraActual != NULL) os << ", ";
                } while (palabraActual != NULL);
                os << ";" << endl;
            }
        }
    }
    os << "}" << endl << endl;
}

void
PrepVocabulario::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    int codigoActual;
    PrepTokeniser::TTokenType token;
    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, RSV_VOCABULARY);
    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");

    token = tokeniser.leeToken();
    while (!(token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
        Palabra nuevaPalabra;
        codigoActual = -1;
        // Tipo de palabra
        if (token == PrepTokeniser::TK_IDENTIFICADOR)
        {
            if (UtilTexto::toUpr(tokeniser.lvalue().literal) == MSG_VERB)
                nuevaPalabra.tipo = Vocabulario::VERB;
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == MSG_ADVERB)
                nuevaPalabra.tipo = Vocabulario::ADVERB;
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == MSG_NOUN)
                nuevaPalabra.tipo = Vocabulario::NOUN;
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == MSG_ADJECTIVE)
                nuevaPalabra.tipo = Vocabulario::ADJECTIVE;
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == MSG_PREPOSITION)
                nuevaPalabra.tipo = Vocabulario::PREPOSITION;
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == MSG_CONJUNCTION)
                nuevaPalabra.tipo = Vocabulario::CONJUNCTION;
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == MSG_PRONOUN)
                nuevaPalabra.tipo = Vocabulario::PRONOUN;
            else
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSG_LITERAL_DE_TIPO_DE_PALABRA), Error::FATAL);
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSG_LITERAL_DE_TIPO_DE_PALABRA), Error::FATAL);

        // Código de palabra (opcional)
        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_NUMERO)
        {
            // Fijamos el valor del código para el diccionario
            codigoActual = tokeniser.lvalue().valor;
            if (codigoActual < 1 || codigoActual > 254)
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), MSGPREP_WORDRESERV, Error::FATAL);

        } else tokeniser.putback();

        saltarToken(tokeniser, PrepTokeniser::TK_CONTROL, ":");

        do
        {
            token = tokeniser.leeToken();
            if (token == PrepTokeniser::TK_LITERAL)
            {

                if (wordOk(tokeniser.lvalue().literal))
                {
                    string sinonimo = UtilTexto::toUpr(tokeniser.lvalue().literal);
                    if (_palabrasGenera.count(sinonimo.substr(0,_tamPalabra)) &&
                        (*_palabrasGenera.find(sinonimo.substr(0,_tamPalabra))).second!=sinonimo)
                    {
                        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(),
                                 "["+tokeniser.lvalue().literal+"] "+MSGPREP_PALABRASINONIMO+" ["+
                                 (*_palabrasGenera.find(sinonimo.substr(0,_tamPalabra))).second+"]", Error::NORMAL);
                    }
                    else _palabrasGenera.insert(pair<string,string>(sinonimo.substr(0,_tamPalabra), sinonimo));

                    nuevaPalabra.listasinonimos.push_back(sinonimo);
                }
                else
                    ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(),"["+tokeniser.lvalue().literal+"] "+MSGPREP_FORMATOPALABRA, Error::FATAL);
            }
            else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_LITERALPALABRA), Error::FATAL);

            token = tokeniser.leeToken();
        } while (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == ",");
        tokeniser.putback();
        saltarToken(tokeniser, PrepTokeniser::TK_CONTROL, ";");

        // Añadir palabra al vocabulario y al diccionario

        for (vector<string>::const_iterator it = nuevaPalabra.listasinonimos.begin();
             it != nuevaPalabra.listasinonimos.end();
             it++)
        {
            if (diccionario.buscarEntrada(*it, WORD) != -2 ||
                diccionario.buscarEntrada(*it, VALOR) != -2)
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), (string)RSV_VOCABULARY+" ["+*it+"] "+MSGPREP_YADEFINIDO, Error::FATAL);

            diccionario.anadirEntrada(*it, WORD, codigoActual);

            if (codigoActual != -1)
                _pooles[nuevaPalabra.tipo]->anadirPool(codigoActual);
        }
        _vocabulario.push_back(nuevaPalabra);
        for (vector<string>::const_iterator it = nuevaPalabra.listasinonimos.begin();
             it != nuevaPalabra.listasinonimos.end();
             it++)
        {
            _vocabularioNombres.insert(pair<std::string, int>(*it, nuevaPalabra.tipo));
        }

        token = tokeniser.leeToken();
    }
}

void
PrepVocabulario::asignarValoresSimbolos(Diccionario& diccionario)
{
    // Añadir las palabras especiales * y _
    diccionario.anadirEntrada("*", WORD, 1);
    diccionario.anadirEntrada("_", WORD, 255);

    for (vector<Palabra>::const_iterator it = _vocabulario.begin();
         it != _vocabulario.end();
         it++)
    {
        if (diccionario.buscarEntrada((*(*it).listasinonimos.begin()), WORD) == -1)
        {
            int nuevoValor = _pooles[(*it).tipo]->dameValorPool();
            if (nuevoValor == -1)
                ERRORPAW("", 0, nombre() + ": [" + Vocabulario::tipoAStr((*it).tipo) + "] " + MSGPREP_NUMELEMSUP, Error::FATAL);
            for (vector<string>::const_iterator itPal = (*it).listasinonimos.begin();
                 itPal != (*it).listasinonimos.end();
                 itPal++)
            {
                diccionario.fijarValor(*itPal, WORD, nuevoValor);
            }
        }
    }
}

void
PrepVocabulario::generarCodigo(ostream& os, const Diccionario& diccionario) const
{
    os << RSV_VOCABULARY << endl << "{" << endl;
    os << "\t1, RESERVED: \"*\";" << endl;

    for (vector<Palabra>::const_iterator it = _vocabulario.begin();
         it != _vocabulario.end();
         it++)
    {
        for (vector<string>::const_iterator itPal = (*it).listasinonimos.begin();
             itPal != (*it).listasinonimos.end();
             itPal++)
        {
            os << "\t";
            os << diccionario.buscarEntrada(*itPal, WORD) << ", ";
            os << Vocabulario::tipoAStr((unsigned int)(*it).tipo) << ": ";
            os << "\"" << *itPal << "\";" << endl;
        }
    }
    os << "\t255, RESERVED: \"_\";" << endl;
    os << "}" << endl << endl;
}

bool
PrepVocabulario::wordOk(const string& palabra)
{
    //if (palabra.length() > 5 || palabra.length() < 1)
    //    return false;
    for (size_t i = 0; i < palabra.length(); i++)
    {
        if (!(
              (palabra[i] >= 'a' && palabra[i] <= 'z') ||
              (palabra[i] >= 'A' && palabra[i] <= 'Z') ||
              (palabra[i] >= '0' && palabra[i] <= '9') ||
              palabra[i] == 'Ñ' ||// El PAW Español permite usar Ñ (inpaws lo cambia luego por barra invertida)
              palabra[i] == 'ñ' ||
              palabra[i] == '_' // para crear palabras intecleables (o mejor dicho tecleables pero no reconocibles)
             )
            )
            return false;
    }
    return true;
}
