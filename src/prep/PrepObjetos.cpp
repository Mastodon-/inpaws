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

#include "prep/PrepObjetos.h"
#include "prep/MsgPrep.h"
#include "prep/Error.h"
#include "UtilTexto.h"
#include "Textos.h"
#include "Vocabulario.h"
#include "ListaSimple.h"

using namespace Inpaws;
using namespace std;

PrepObjetos::PrepObjetos(const PrepSubChar& prepSubChar, const PrepVocabulario& prepVocabulario)
    : PrepPart(RSV_OBJECT, 254), _prepSubChar(prepSubChar), _prepVocabulario(prepVocabulario)
{
}

PrepObjetos::~PrepObjetos()
{
}

void PrepObjetos::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    const Textos* objetos = dynamic_cast<const Textos*>(aventura.getModulo(RSV_OBJECTS));
    const InitiallyAt* initiallyAt = dynamic_cast<const InitiallyAt*>(aventura.getModulo(RSV_INITIALLYAT));
    const Vocabulario* vocabulario = dynamic_cast<const Vocabulario*>(aventura.getModulo(RSV_VOCABULARY));
    const ObjWords* objWords = dynamic_cast<const ObjWords*>(aventura.getModulo(RSV_OBJWORDS));
    const ObjWeightType* objWeightType = dynamic_cast<const ObjWeightType*>(aventura.getModulo(RSV_OBJWEIGHT));

    for (unsigned int i = 0; i < objetos->size(); i++)
    {
        os << RSV_OBJECT << " " << i << endl << "{" << endl;
        // Descripción
        os << "\t\"";
        PrepTokeniser::imprimirLiteral(objetos->obtenerTexto(i), os);
        os << "\";" << endl;
        // Localidad inicial
        os << '\t' << RSV_INITIALLYAT << " ";
        switch (initiallyAt->leerDato(i)[0])
        {
            case 252:
                os << RSV_NOTCREATED;
                break;
            case 253:
                os << RSV_WORN;
                break;
            case 254:
                os << RSV_CARRIED;
                break;
            default:
                os << (unsigned int)initiallyAt->leerDato(i)[0];
        }
        os << ";" << endl;

        // Palabras
        string palabraNoun = (objWords->leerDato(i)[0] == 255 ? "_" :
          vocabulario->leerPalabraInit(objWords->leerDato(i)[0], Vocabulario::NOUN)->palabraCompleta);
        string palabraAdj =  (objWords->leerDato(i)[1] == 255 ? "_" :
          vocabulario->leerPalabraInit(objWords->leerDato(i)[1], Vocabulario::ADJECTIVE)->palabraCompleta);
        os << '\t' << RSV_WORDS << " "
           << palabraNoun << " "
           << palabraAdj << ";" << endl;

        // Peso
        os << '\t' << RSV_WEIGHT << " " << (unsigned int)(objWeightType->leerDato(i)[0] & 0x3F)
           << ";" << endl;

        // Propiedades
        if (objWeightType->leerDato(i)[0] & 0xC0)
        {
          os << '\t' << RSV_PROPERTY << " ";
          if (objWeightType->leerDato(i)[0] & 0x80)
            os << RSV_CLOTHING;
          if ((objWeightType->leerDato(i)[0] & 0xC0) == 0xC0)
            os << ", ";
          if (objWeightType->leerDato(i)[0] & 0x40)
            os << RSV_CONTAINER;

          os << ";" << endl;
        }
        os << "}" << endl << endl;
    }
}

void PrepObjetos::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    PrepTokeniser::TTokenType token;
    Objeto nuevoObjeto;
    UtilTexto utilTexto;

    saltarToken (tokeniser, PrepTokeniser::TK_IDENTIFICADOR, RSV_OBJECT);

    // número o nombre del objeto
    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_IDENTIFICADOR)
    {
        if (diccionario.buscarEntrada(tokeniser.lvalue().literal, OBJNO) != -2 ||
            diccionario.buscarEntrada(tokeniser.lvalue().literal, VALOR) != -2)
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), (string)RSV_OBJECT+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
        }
        nuevoObjeto.nombre = tokeniser.lvalue().literal;

        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_NUMERO)
        {
            if (disponibleValor(tokeniser.lvalue().valor))
            {
               diccionario.anadirEntrada(nuevoObjeto.nombre, OBJNO, tokeniser.lvalue().valor);
                // Y añadimos al pool de asignadas para que no se vuelva a utilizar
                anadirPool (tokeniser.lvalue().valor);
            } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), (string)RSV_OBJECT+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
        }
        else
        {
            tokeniser.putback();
            diccionario.anadirEntrada(nuevoObjeto.nombre, OBJNO);
        }

    }
    else if (token == PrepTokeniser::TK_NUMERO)
    {
        // Si ya existe, error
        if (diccionario.buscarEntrada(tokeniser.lvalue().literal, OBJNO) != -2)
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), (string)RSV_OBJECT+" ["+tokeniser.lvalue().literal+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
        }

        // Añadimos al diccionario el número de localidad, ya asignado
        diccionario.anadirEntrada(tokeniser.lvalue().literal, OBJNO, tokeniser.lvalue().valor);

        // Y añadimos al pool de asignadas para que no se vuelva a utilizar
        anadirPool (tokeniser.lvalue().valor);

        nuevoObjeto.nombre = tokeniser.lvalue().literal;
    }
    else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGONOMBREOBJ), Error::FATAL);

    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");

    // Descripcion del objeto
    token = tokeniser.leeToken();
    if (!(token == PrepTokeniser::TK_LITERAL))
        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_DESCOBJETO), Error::FATAL);
    else
    {
        nuevoObjeto.desc = tokeniser.lvalue().literal;
    }
    if (!utilTexto.textoCorrecto(nuevoObjeto.desc))
    {
        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[" + nombre() + "] " + utilTexto.errorStr(), Error::NORMAL);
    }
    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");

    // Resto de propiedades
    token = tokeniser.leeToken();
    while (!(token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
       if (token == PrepTokeniser::TK_IDENTIFICADOR)
       {
            if (UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_INITIALLYAT)
            {
                token = tokeniser.leeToken();
                if (token == PrepTokeniser::TK_NUMERO || token == PrepTokeniser::TK_IDENTIFICADOR)
                {
                    nuevoObjeto.initiallyatlinea = tokeniser.lineaActual();
                    nuevoObjeto.initiallyatarchivo = tokeniser.archivoActual();
                    nuevoObjeto.initiallyat = tokeniser.lvalue().literal;
                } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGONOMBRELOC), Error::FATAL);
                saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");
            }
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_WORDS)
            {
                token = tokeniser.leeToken();
                if (token == PrepTokeniser::TK_NUMERO || token == PrepTokeniser::TK_IDENTIFICADOR ||
                    (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "_"))
                {
                    nuevoObjeto.wordnounlinea = tokeniser.lineaActual();
                    nuevoObjeto.wordnounarchivo = tokeniser.archivoActual();
                    nuevoObjeto.wordnoun = tokeniser.lvalue().literal;
                }
                else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGOPALABRA), Error::FATAL);

                token = tokeniser.leeToken();
                if (token == PrepTokeniser::TK_NUMERO || token == PrepTokeniser::TK_IDENTIFICADOR ||
                    (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "_"))
                {
                    nuevoObjeto.wordadjlinea = tokeniser.lineaActual();
                    nuevoObjeto.wordadjarchivo = tokeniser.archivoActual();
                    nuevoObjeto.wordadj = tokeniser.lvalue().literal;
                }
                else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_CODIGOPALABRA), Error::FATAL);
                saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");
            }
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_WEIGHT)
            {
                token = tokeniser.leeToken();
                if (token == PrepTokeniser::TK_NUMERO)
                {
                    if (tokeniser.lvalue().valor < 64)
                    {
                        nuevoObjeto.weight = tokeniser.lvalue().valor;
                    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_PESOERROR), Error::FATAL);
                } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_PESOERROR), Error::FATAL);
                saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");
            }
            else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_PROPERTY)
            {
                _fijarPropiedad (tokeniser, nuevoObjeto);
                token = tokeniser.leeToken();
                if (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == ",")
                    _fijarPropiedad (tokeniser, nuevoObjeto);
                else
                    tokeniser.putback();
                saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");
            }
       } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_PROPOBJETO), Error::FATAL);
       token = tokeniser.leeToken();
    }
    _objetos.push_back(nuevoObjeto);
}

void PrepObjetos::asignarValoresSimbolos(Diccionario& diccionario)
{
    if (!_objetos.size())
    {
        ERRORPAW("", 0, MSGPREP_ALMENOSUNOBJ, Error::NORMAL);
    }
    // Primero añadimos al diccionario las 3 localidades especiales donde puede estar un objeto
    diccionario.anadirEntrada(RSV_NOTCREATED, LOCNO, 252);
    diccionario.anadirEntrada(RSV_WORN, LOCNO, 253);
    diccionario.anadirEntrada(RSV_CARRIED, LOCNO, 254);

    // El objeto 0 también se añade por defecto, siempre habrá
    // uno y queremos poder identificarlo por su número además de
    // por su nombre. No lo añadimos al pool para no evitar
    // que se asigne al primer nombre que se declare
    diccionario.anadirEntrada("0", OBJNO, 0);

    // Ahora asignamos el resto de simbolos
    for (std::vector<Objeto>::const_iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {
        if (diccionario.buscarEntrada((*it).nombre, OBJNO) == -1)
        {
            diccionario.fijarValor((*it).nombre, OBJNO, dameValorPool());
        }
    }
    if (!poolDefinicionesOk()) // Comprueba que todas las definiciones son correlativas
    {
        ERRORPAW("", 0, nombre() + ": "+MSGPREP_DEFCORR, Error::FATAL);
    }
}

void PrepObjetos::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    UtilTexto utilTexto;
    // Objetos
    os << RSV_OBJECTS << endl << "{" << endl;
    for (std::vector<Objeto>::const_iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {
        os << '\t' << diccionario.buscarEntrada((*it).nombre, OBJNO) << ": ";
        os << "\"";
        PrepTokeniser::imprimirLiteral((*it).desc, os);
        os << "\";" << endl;
    }
    os << "}" << endl << endl;

    // InitiallyAt
    os << RSV_INITIALLYAT << endl << "{" << endl;
    for (std::vector<Objeto>::const_iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {
        int initiallyat = diccionario.buscarEntrada((*it).initiallyat, LOCNO);
        if (initiallyat < 0)
            ERRORPAW((*it).initiallyatarchivo, (*it).initiallyatlinea, "["+(*it).initiallyat + "] " + MSGPREP_LOCNODEF, Error::NORMAL);

        os << '\t' << diccionario.buscarEntrada((*it).nombre, OBJNO) << ": ";
        os << initiallyat << ";" << endl;
    }
    os << "}" << endl << endl;

    // Words
    os << RSV_OBJWORDS << endl << "{" << endl;
    for (std::vector<Objeto>::const_iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {
        int wordnoun = diccionario.buscarEntrada((*it).wordnoun, WORD);
        int wordadj = diccionario.buscarEntrada((*it).wordadj, WORD);

        // Si no existe la palabra: error
        if (wordnoun < 0)
            ERRORPAW((*it).wordnounarchivo, (*it).wordnounlinea, "["+(*it).wordnoun + "] " + MSGPREP_PALABRANODEF, Error::NORMAL);
        if (wordadj < 0)
            ERRORPAW((*it).wordadjarchivo, (*it).wordadjlinea, "["+(*it).wordadj + "] " + MSGPREP_PALABRANODEF, Error::NORMAL);
        // O si no es del tipo correcto, también error
        if ((*it).wordnoun != "_" && !_prepVocabulario.existePalabra(UtilTexto::toUpr((*it).wordnoun), Vocabulario::NOUN))
            ERRORPAW ((*it).wordnounarchivo, (*it).wordnounlinea, "["+(*it).wordnoun + "] " + MSGPREP_PALNOVAL, Error::NORMAL);
        if ((*it).wordadj != "_" && !_prepVocabulario.existePalabra(UtilTexto::toUpr((*it).wordadj), Vocabulario::ADJECTIVE))
            ERRORPAW ((*it).wordadjarchivo, (*it).wordadjlinea, "["+(*it).wordadj + "] " + MSGPREP_PALNOVAL, Error::NORMAL);

        os << '\t' << diccionario.buscarEntrada((*it).nombre, OBJNO) << ": "
           << wordnoun << ", " << wordadj << ";" << endl;
    }
    os << "}" << endl;

    // Weight/Property
    os << RSV_OBJWEIGHT << endl << "{" << endl;
    for (std::vector<Objeto>::const_iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {
        int pesoFinal = (*it).weight +
                        ((*it).container ? 0x40 : 0) +
                        ((*it).clothing ? 0x80 : 0);
        os << '\t' << diccionario.buscarEntrada((*it).nombre, OBJNO) << ": " << pesoFinal << ";" << endl;
    }
    os << "}" << endl;
}

void
PrepObjetos::_fijarPropiedad (PrepTokeniser& tokeniser, Objeto& objeto) const
{
    PrepTokeniser::TTokenType token;

    token = tokeniser.leeToken();
    if (token == PrepTokeniser::TK_IDENTIFICADOR)
    {
        if (UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_CONTAINER)
            objeto.container = true;
        else if (UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_CLOTHING)
            objeto.clothing = true;
        else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_PROPOBJETO), Error::FATAL);
    } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_PROPOBJETO), Error::FATAL);
}

void
PrepObjetos::sustituirCaracteresEspeciales()
{
    UtilTexto utilTexto;
    for (std::vector<Objeto>::iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {
        string descripcionSub = (*it).desc;
        utilTexto.sustituirCaracteres(descripcionSub, _prepSubChar.sustituciones());
        (*it).desc = descripcionSub;
    }
}

// Para objetos solo abreviamos a partir de la segunda palabra, ya que la primera
// tiene un significado especial en PAW y no debe abreviarse
PrepAbreviable::TListaCadenas
PrepObjetos::leerCadenas() const
{
    PrepAbreviable::TListaCadenas objetos;
    for (std::vector<Objeto>::const_iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {
        size_t objPos = (*it).desc.find(" ");
        if (objPos != string::npos && objPos < (*it).desc.length()-1)
            PrepAbreviable::anadirCadena(objetos, (*it).desc.substr(objPos+1));
    }
    return objetos;
}

SpWord
PrepObjetos::sustituirCadenas (const PrepAbreviable::TDiccionario& diccionario)
{
    UtilTexto utilTexto;
    SpWord ahorro = 0;
    string tmp;
    for (std::vector<Objeto>::iterator it = _objetos.begin();
         it != _objetos.end();
         it++)
    {

        size_t objPos = (*it).desc.find(" ");
        if (objPos != string::npos && objPos < (*it).desc.length()-1)
        {
            size_t longAntes = utilTexto.compactarTexto((*it).desc).length();
            string cabecera = (*it).desc.substr(0, objPos);
            string cuerpo = (*it).desc.substr(objPos+1);
            (*it).desc = cabecera + " " + PrepAbreviable::sustituirCadena(diccionario, cuerpo);
            size_t longDespues = utilTexto.compactarTexto((*it).desc).length();
            ahorro += longAntes - longDespues;
        }
    }
    return ahorro;
}
