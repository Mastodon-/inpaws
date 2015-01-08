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

#include "Aventura.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Defaults.h"
#include "Vocabulario.h"
#include "Textos.h"
#include "Conexiones.h"
#include "ListaSimple.h"
#include "ListaProcesos.h"
#include "Abreviaturas.h"
#include "Graficos.h"
#include "Caracteres.h"
#include <sstream>

using namespace Inpaws;
using namespace std;

Aventura::Aventura()
{
    PawPart* defaults = new Defaults();
    PawPart* vocabulario = new Vocabulario(PTR_VOCABULARIO);
    PawPart* mensajes = new Textos(PTR_MENSAJES, OFF_MENSAJES, RSV_MESSAGES);
    PawPart* sismens = new Textos(PTR_SYSMENSAJES, OFF_SYSMENS, RSV_SYSMESSAGES);
    PawPart* locs = new Textos(PTR_LOCALIDADES, OFF_NUMLOC, RSV_LOCATIONS);
    PawPart* conex = new Conexiones(PTR_CONEXIONES); // Llamar a setNumLocs
    PawPart* objetos = new Textos(PTR_OBJETOS, OFF_NUMOBJ, RSV_OBJECTS);
    PawPart* objwords = new ObjWords(PTR_OBJWORDS); // Llamar a setNumDatos
    PawPart* initiallyat = new InitiallyAt(PTR_INITIALLYAT); // Llamar a setNumDatos
    PawPart* objweight = new ObjWeightType(PTR_OBJWEIGHT); // Llamar a setNumDatos
    PawPart* procesos = new ListaProcesos(PTR_PROCESOS, OFF_NUMPRO);
    PawPart* abrev = new Abreviaturas(OFF_ABBREV); // Llamar a setMainTop
    PawPart* graficos = new Graficos(PTR_GRAPH, PTR_GRAPHATTR);
    PawPart* caracteres = new Caracteres();

    _modulos[RSV_DEFAULTS] = defaults;
    _modulos[RSV_VOCABULARY] = vocabulario;
    _modulos[RSV_MESSAGES] = mensajes;
    _modulos[RSV_SYSMESSAGES] = sismens;
    _modulos[RSV_LOCATIONS] = locs;
    _modulos[RSV_CONNECTIONS] = conex;
    _modulos[RSV_OBJECTS] = objetos;
    _modulos[RSV_OBJWORDS] = objwords;
    _modulos[RSV_INITIALLYAT] = initiallyat;
    _modulos[RSV_OBJWEIGHT] = objweight;
    _modulos[RSV_PROCESS_TABLE] = procesos;
    _modulos[RSV_ABBREVIATIONS] = abrev;
    _modulos[RSV_GRAPHICS] = graficos;
    _modulos[RSV_CHARACTERS] = caracteres;

    _listaVolcado.push_back(RSV_CHARACTERS);
    _listaVolcado.push_back(RSV_ABBREVIATIONS);
    _listaVolcado.push_back(RSV_PROCESS_TABLE);
    _listaVolcado.push_back(RSV_OBJECTS);
    _listaVolcado.push_back(RSV_LOCATIONS);
    _listaVolcado.push_back(RSV_MESSAGES);
    _listaVolcado.push_back(RSV_SYSMESSAGES);
    _listaVolcado.push_back(RSV_CONNECTIONS);
    _listaVolcado.push_back(RSV_VOCABULARY);
    _listaVolcado.push_back(RSV_INITIALLYAT);
    _listaVolcado.push_back(RSV_OBJWORDS);
    _listaVolcado.push_back(RSV_OBJWEIGHT);
    _listaVolcado.push_back(RSV_GRAPHICS);
    _listaVolcado.push_back(RSV_DEFAULTS);
}

Aventura::~Aventura()
{
    for (TModulos::const_iterator it = _modulos.begin();
          it != _modulos.end();
          it++)
    {
        delete (*it).second;
    }
}


SpWord
Aventura::volcarAMemoria ()
{
    // La funcion devuelve la memoria libre tras el volcado, o 65535 si no cabe

    // Primero fijamos el maintop, que por lo que he visto
    // siempre está en la 37632
    PtrBuffer mainTop = 37632;
    SPBUFFER->wordPoke(PTR_MAINTOP, mainTop);

    // Luego empezamos a volcar a partir de mainTop+334
    PtrBuffer ptrActual = mainTop + 334;
    PtrBuffer finDatos(0), inicioGraficos(0);

    for (TListaVolcado::const_iterator it = _listaVolcado.begin();
         it != _listaVolcado.end();
         it++)
    {
        PawPart* actual = (*_modulos.find(*it)).second;
        string parteActual = *it;
        if (*it != RSV_GRAPHICS)
        {
            ptrActual = actual->volcarAMemoria(ptrActual);
            if (*it == RSV_OBJWEIGHT)
                finDatos = ptrActual;
            if (ptrActual < mainTop || ptrActual >= 65491)
            {
                // Ha superado el 65535, no cabe
                return 65535;
            }
        } else
        {
            // Los graficos siempre terminan en la 65491
            Graficos* graficos = dynamic_cast<Graficos*>((*_modulos.find(*it)).second);

            // Extendemos los graficos al numero de localidades
            graficos->completarLocalidades(dynamic_cast<const Textos*>((*_modulos.find(RSV_LOCATIONS)).second)->size());

            inicioGraficos = DIR_FINGRAFICOS - graficos->size() + 1;

            ptrActual = graficos->volcarAMemoria(inicioGraficos);
        }
        ptrActual++;
    }

    // Por ultimo almacenamos el fin de los datos y el inicio de los
    // graficos, necesarios para grabar la aventura
    SPBUFFER->wordPoke (PTR_FINDATOS, finDatos);
    SPBUFFER->wordPoke (PTR_INICIOGRAFICOS, inicioGraficos);

    _parchearDatosVolcado(); // Varios pokes necesarios

    if (finDatos >= inicioGraficos)
        return 65535; // No cabe
    else
    {
        return inicioGraficos - finDatos - 1;
    }
}

PtrBuffer
Aventura::volcarDeMemoria ()
{
    PtrBuffer mainTop = SPBUFFER->wordPeek(PTR_MAINTOP);
    SpByte numLocs = SPBUFFER->bytePeek(mainTop + OFF_NUMLOC);
    SpByte numObjs = SPBUFFER->bytePeek(mainTop + OFF_NUMOBJ);

    dynamic_cast<Conexiones*>(_modulos[RSV_CONNECTIONS])->setNumLocs(numLocs);
    dynamic_cast<ObjWords*>(_modulos[RSV_OBJWORDS])->setNumDatos(numObjs);
    dynamic_cast<InitiallyAt*>(_modulos[RSV_INITIALLYAT])->setNumDatos(numObjs);
    dynamic_cast<ObjWeightType*>(_modulos[RSV_OBJWEIGHT])->setNumDatos(numObjs);
    dynamic_cast<Abreviaturas*>(_modulos[RSV_ABBREVIATIONS])->setMainTop(mainTop);

    PtrBuffer finDatos(0);

    for (TModulos::iterator it = _modulos.begin();
         it != _modulos.end();
         it++)
    {
        finDatos = (*it).second->volcarDeMemoria();
        // DEBUG***
        //PtrBuffer inicioDatos = (*it).second->inicioReal();
        //cout << inicioDatos << "\t Inicio " << (*it).first << endl
        //     << finDatos << "\t Fin " << (*it).first << endl;
        // END DEBUG
    }

    return finDatos;
}

void
Aventura::generarCodigo (std::ostream& os)
{
    for (TModulos::iterator it = _modulos.begin();
         it != _modulos.end();
         it++)
    {
        (*it).second->generarCodigo(os);
        os << endl;
    }
}

void
Aventura::generarCodigoDisco(std::ostream& os, uint16_t flags)
{
    const char* separador = "- - - - - - - - - - - - - - - - - - - - - -";
    os << "; " << MSG_SCEPORINPAWS << endl
       << "/CTL\n";
    if (flags & VERCPM)
        os << "A\n";
    os << "_\n; " << separador << endl;

    getModulo(RSV_VOCABULARY)->generarCodigoDisco(os, flags);
    os << "; " << separador << endl;
    getModulo(RSV_SYSMESSAGES)->generarCodigoDisco(os, flags);
    os << "; " << separador << endl;
    getModulo(RSV_MESSAGES)->generarCodigoDisco(os, flags);
    os << "; " << separador << endl;
    getModulo(RSV_OBJECTS)->generarCodigoDisco(os, flags);
    os << "; " << separador << endl;
    getModulo(RSV_LOCATIONS)->generarCodigoDisco(os, flags);
    os << "; " << separador << endl;
    dynamic_cast<Conexiones*>(getModulo(RSV_CONNECTIONS))->generarCodigoDisco(os, flags, dynamic_cast<const Vocabulario*>(getModulo(RSV_VOCABULARY)));
    os << "; " << separador << endl;

    // El /OBJ lo generamos directamente aquí, que tira de muchas tablas...
    _generarCodigoDiscoOBJ(os, flags);
    os << "; " << separador << endl;

    dynamic_cast<ListaProcesos*>(getModulo(RSV_PROCESS_TABLE))->generarCodigoDisco(os, flags, dynamic_cast<const Vocabulario*>(getModulo(RSV_VOCABULARY)));
    os << "; " << separador << endl;

}

void
Aventura::_generarCodigoDiscoOBJ(std::ostream& os, uint16_t flags)
{
    Vocabulario* vocabulario = dynamic_cast<Vocabulario*>(getModulo(RSV_VOCABULARY));
    InitiallyAt* initiallyat = dynamic_cast<InitiallyAt*>(getModulo(RSV_INITIALLYAT));
    ObjWeightType* weight = dynamic_cast<ObjWeightType*> (getModulo(RSV_OBJWEIGHT));
    ObjWords* objwords = dynamic_cast<ObjWords*>(getModulo(RSV_OBJWORDS));

    os << "/OBJ\t; Object definition section";

    if (flags&VERGLULX)
        os << "; obj\tInit\tWeight\tNoun\tAdj\tlo_atr\thi_atr" << endl;
    else
        os << endl << "; obj\tInit\tWeight\tCont\tWR\tNoun\tAdj" << endl;
    for (unsigned int i=0; i < initiallyat->size(); i++)
    {
        const Vocabulario::Palabra* nombre =
                (vocabulario->leerPalabraInit(objwords->leerDato(i)[0], Vocabulario::NOUN) != NULL?
                vocabulario->leerPalabraInit(objwords->leerDato(i)[0], Vocabulario::NOUN) :
                vocabulario->leerPalabraInit(objwords->leerDato(i)[0], Vocabulario::VERB));
        const Vocabulario::Palabra* adjetivo =
            vocabulario->leerPalabraInit(objwords->leerDato(i)[1], Vocabulario::ADJECTIVE);

        if (flags&VERGLULX)
        {
            os << "/" << i << '\t'
               << (unsigned int)initiallyat->leerDato(i)[0] << '\t'
               << (unsigned int)(weight->leerDato(i)[0] & 0x3f) << '\t'
               << (objwords->leerDato(i)[0] != 255?nombre->palabra(flags):"_") << '\t'
               << (objwords->leerDato(i)[1] != 255?adjetivo->palabra(flags): "_") << '\t'
               << (i?'0':'1')
               << ((unsigned int)(weight->leerDato(i)[0] & 0x80)?'1':'0')
               << ((unsigned int)(weight->leerDato(i)[0] & 0x40)?'1':'0')
               << "00000000000000000000000000000 00000000000000000000000000000000";
        } else
        {
            os << "/" << i << '\t'
               << (unsigned int)initiallyat->leerDato(i)[0] << '\t'
               << (unsigned int)(weight->leerDato(i)[0] & 0x3f) << '\t'
               << ((unsigned int)(weight->leerDato(i)[0] & 0x40)?'Y':'_') << '\t'
               << ((unsigned int)(weight->leerDato(i)[0] & 0x80)?'Y':'_') << '\t'
               << (objwords->leerDato(i)[0] != 255?nombre->palabra(flags):"_") << '\t'
               << (objwords->leerDato(i)[1] != 255?adjetivo->palabra(flags): "_") << '\t';
        }
        os << endl;
    }
}

void
Aventura::generarCodigoSuperglus(std::ostream& os,
                                 uint16_t flags,
                                 const Diccionario& diccionario)
{
    Diccionario::TDiccionarioInverso inverso =
          diccionario.crearDiccionarioInverso(); // Diccionario valor -> nombre
    _ajustarLineralesDictSuperglus(inverso); // Ajusta los mensajes en literal (sust. SUBCHAR)

    _generarCodigoSuperglusDefines(os, FLAGNO, inverso); // Identificadores de los flags
    //_generarCodigoSuperglusDefines(os, VALOR, inverso); // Identificadores de las constantes
    _generarCodigoSuperglusDefines(os, PROCNO, inverso); // Identificadores de los procesos

    os << "/CTL\n\n";

    getModulo(RSV_VOCABULARY)->generarCodigoDisco(os, flags);

    //getModulo(RSV_SYSMESSAGES)->generarCodigoSuperglus(os, flags, inverso);
    getModulo(RSV_SYSMESSAGES)->generarCodigoDisco(os, flags);

    getModulo(RSV_MESSAGES)->generarCodigoSuperglus(os, flags, inverso);

    getModulo(RSV_OBJECTS)->generarCodigoSuperglus(os, flags, inverso);

    getModulo(RSV_LOCATIONS)->generarCodigoSuperglus(os, flags, inverso);

    dynamic_cast<Conexiones*>(getModulo(RSV_CONNECTIONS))->generarCodigoSuperglus(os, flags, dynamic_cast<const Vocabulario*>(getModulo(RSV_VOCABULARY)), inverso);


    // El /OBJ lo generamos directamente aquí, que tira de muchas tablas...
    _generarCodigoDiscoOBJSuperglus(os, flags, inverso);

    dynamic_cast<ListaProcesos*>(getModulo(RSV_PROCESS_TABLE))->generarCodigoSuperglus(os, flags, dynamic_cast<const Vocabulario*>(getModulo(RSV_VOCABULARY)), inverso);
}

void
Aventura::_generarCodigoSuperglusDefines(std::ostream& os, int tipo,
                                         const Diccionario::TDiccionarioInverso& diccionario) const
{
    switch(tipo)
    {
        case FLAGNO:
            os << "; Definición de flags\n";
            break;

        case PROCNO:
            os << "; Procesos utilizados\n";
            break;
    }
    string tipoDef = (tipo==FLAGNO?"flg":"const");

    for (Diccionario::TDiccionarioInverso::const_iterator it = diccionario.begin();
         it != diccionario.end();
         it++)
    {
        if ((*it).first.second == tipo)
            os << "#define " << tipoDef << " " << (*it).second << " " << (*it).first.first << endl;
    }
    os << endl;
}

void
Aventura::_generarCodigoDiscoOBJSuperglus(std::ostream& os, uint16_t flags, const Diccionario::TDiccionarioInverso& diccionario)
{
    Vocabulario* vocabulario = dynamic_cast<Vocabulario*>(getModulo(RSV_VOCABULARY));
    InitiallyAt* initiallyat = dynamic_cast<InitiallyAt*>(getModulo(RSV_INITIALLYAT));
    ObjWeightType* weight = dynamic_cast<ObjWeightType*> (getModulo(RSV_OBJWEIGHT));
    ObjWords* objwords = dynamic_cast<ObjWords*>(getModulo(RSV_OBJWORDS));

    os << "/OBJ" << endl;

    for (unsigned int i=0; i < initiallyat->size(); i++)
    {
        const Vocabulario::Palabra* nombre =
                (vocabulario->leerPalabraInit(objwords->leerDato(i)[0], Vocabulario::NOUN) != NULL?
                vocabulario->leerPalabraInit(objwords->leerDato(i)[0], Vocabulario::NOUN) :
                vocabulario->leerPalabraInit(objwords->leerDato(i)[0], Vocabulario::VERB));
        const Vocabulario::Palabra* adjetivo =
            vocabulario->leerPalabraInit(objwords->leerDato(i)[1], Vocabulario::ADJECTIVE);
        string localidad =
          (diccionario.count(pair<int,int>((unsigned int)initiallyat->leerDato(i)[0], LOCNO))?
           (*diccionario.find(pair<int,int>((unsigned int)initiallyat->leerDato(i)[0], LOCNO))).second:
           "");

        os << "/";
        if (diccionario.count(pair<int,int>(i, OBJNO)))
            os << (*diccionario.find(pair<int,int>(i, OBJNO))).second;
        else
            os << i;
        os << '\t';
        if (localidad.length() && (unsigned int)initiallyat->leerDato(i)[0] < 252)
            os << localidad;
        else
            os << (unsigned int)initiallyat->leerDato(i)[0];

           os << '\t' << (unsigned int)(weight->leerDato(i)[0] & 0x3f) << '\t'
           << (objwords->leerDato(i)[0] != 255?nombre->palabra(flags):"_") << '\t'
           << (objwords->leerDato(i)[1] != 255?adjetivo->palabra(flags): "_") << '\t'
           << (i?'0':'1')
           << ((unsigned int)(weight->leerDato(i)[0] & 0x80)?'1':'0')
           << ((unsigned int)(weight->leerDato(i)[0] & 0x40)?'1':'0')
           << "00000000000000000000000000000 00000000000000000000000000000000"
        << endl;
    }
}

void
Aventura::_ajustarLineralesDictSuperglus(Diccionario::TDiccionarioInverso& diccionario) const
{
    Diccionario::TDiccionarioInverso nuevoDict;
    for (Diccionario::TDiccionarioInverso::iterator it = diccionario.begin();
         it != diccionario.end();
         it++)
    {
        if ((*it).second[0] == '\"') // Si es un literal de MESSAGE, MES o SYSMESS
        {
            string tmp ="\"";
            if ((*it).first.second == MESNO)
            {
                tmp +=
                    dynamic_cast<Textos*>((*_modulos.find(RSV_MESSAGES)).second)->obtenerTexto((*it).first.first);
            }
            else // SYSNO
            {
                tmp +=
                    dynamic_cast<Textos*>((*_modulos.find(RSV_SYSMESSAGES)).second)->obtenerTexto((*it).first.first);
            }
            tmp.append(1, '\"');
            nuevoDict.insert(pair<Diccionario::TDiccionarioInverso::key_type, const string>((*it).first, tmp));
        }
        else
            nuevoDict.insert(pair<Diccionario::TDiccionarioInverso::key_type, const string>((*it).first, (*it).second));
    }
    diccionario = nuevoDict;
}

void
Aventura::leerDeCodigo (Tokeniser& tokeniser)
{
    Tokeniser::TTokenType token;
    TModulos::iterator moduloActual;

    token = tokeniser.leeToken();

    while (token != Tokeniser::TK_NULL)
    {
        if (token == Tokeniser::TK_IDENTIFICADOR)
        {
            moduloActual = _modulos.find(tokeniser.lvalue().literal);
            if (moduloActual == _modulos.end())
                throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NOMBRE_MODULO_PAW));

            tokeniser.putback();
            (*moduloActual).second->leerDeCodigo(tokeniser);
        }
        else throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NOMBRE_MODULO_PAW));
        token = tokeniser.leeToken();
    }

    // Fijamos los valores varios
    PtrBuffer mainTop = 37632;
    SpByte numLocs = dynamic_cast<Textos*>(_modulos[RSV_LOCATIONS])->size();
    SpByte numObjs = dynamic_cast<Textos*>(_modulos[RSV_OBJECTS])->size();

    dynamic_cast<Conexiones*>(_modulos[RSV_CONNECTIONS])->setNumLocs(numLocs);
    dynamic_cast<ObjWords*>(_modulos[RSV_OBJWORDS])->setNumDatos(numObjs);
    dynamic_cast<InitiallyAt*>(_modulos[RSV_INITIALLYAT])->setNumDatos(numObjs);
    dynamic_cast<ObjWeightType*>(_modulos[RSV_OBJWEIGHT])->setNumDatos(numObjs);
    dynamic_cast<Abreviaturas*>(_modulos[RSV_ABBREVIATIONS])->setMainTop(mainTop);
    dynamic_cast<Caracteres*>(_modulos[RSV_CHARACTERS])->setMainTop(mainTop);
}

void
Aventura::_parchearDatosVolcado() const
{
    // Se fijan varios datos en memoria para que funcione el volcado
    PtrBuffer mainTop = 37632;

    // Paginas del 128k
    SPBUFFER->bytePoke (mainTop + OFF_128K, 255); // Aventura de 48k
    SPBUFFER->bytePoke (mainTop + OFF_128K+1, 1);
    SPBUFFER->bytePoke (mainTop + OFF_128K+2, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128K+3, 3);
    SPBUFFER->bytePoke (mainTop + OFF_128K+4, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128K+5, 4);
    SPBUFFER->bytePoke (mainTop + OFF_128K+6, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128K+7, 6);
    SPBUFFER->bytePoke (mainTop + OFF_128K+8, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128K+9, 7);
    SPBUFFER->bytePoke (mainTop + OFF_128K+10, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128K+11, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO, 255); // Aventura de 48k
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+1, 1);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+2, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+3, 3);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+4, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+5, 4);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+6, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+7, 6);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+8, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+9, 7);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+10, 255);
    SPBUFFER->bytePoke (mainTop + OFF_128KOTRO+11, 255);

    // Otros datos que no tengo ni idea para que valen
    // pero aparecen igual en todas las aventuras que he visto
    SPBUFFER->bytePoke (37912, 20);
    SPBUFFER->bytePoke (37943, 16);
    SPBUFFER->bytePoke (65492, 255);
    SPBUFFER->bytePoke (65525, 212);
    SPBUFFER->bytePoke (65526, 255);
}

const PawPart*
Aventura::getModulo(const string& nombre) const
{
    return (*(_modulos.find(nombre))).second;
}

PawPart*
Aventura::getModulo(const string& nombre)
{
    return (*(_modulos.find(nombre))).second;
}

bool
Aventura::pawSignature() const
{
    PtrBuffer mainTop = SPBUFFER->wordPeek(PTR_MAINTOP);
    PtrBuffer mainAttr = mainTop + 311;

    if (mainTop <= 65535-321 && mainTop >= 16384-311 &&
        SPBUFFER->bytePeek(mainAttr) == 16 &&
        SPBUFFER->bytePeek(mainAttr+2) == 17 &&
        SPBUFFER->bytePeek(mainAttr+4) == 18 &&
        SPBUFFER->bytePeek(mainAttr+6) == 19 &&
        SPBUFFER->bytePeek(mainAttr+8) == 20 &&
        SPBUFFER->bytePeek(mainAttr+10) == 21)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string
Aventura::version() const
{
    char ver;
    if (pawSignature())
    {
        ver = SPBUFFER->bytePeek(65527);
        ostringstream verStr;
        if (ver <=31)
            verStr << (int)ver;
        else
            verStr << ver;

        return (ver != 0? verStr.str() : ""); // Version 0 no existe, son las compiladas con Inpaws
    }
    else return "";
}

void
Aventura::mostrarResumen(std::ostream& os, bool mostrarCaracs) const
{
    os << getModulo(RSV_LOCATIONS)->numElems() << "\t" << MSG_RESUMEN_LOCALIDADES << endl
       << getModulo(RSV_OBJECTS)->numElems() << "\t" << MSG_RESUMEN_OBJETOS << endl
       << getModulo(RSV_MESSAGES)->numElems() << "\t" << MSG_RESUMEN_MENSAJES << endl
       << getModulo(RSV_SYSMESSAGES)->numElems() << "\t" << MSG_RESUMEN_SISMENSAJES << endl
       << getModulo(RSV_PROCESS_TABLE)->numElems() << "\t" << MSG_RESUMEN_PROCESOS << endl;
    if (mostrarCaracs)
       os << getModulo(RSV_CHARACTERS)->numElems() << "\t" << MSG_RESUMEN_CARACTERES << endl;

    if (version() != "")
       os << MSG_RESUMEN_VERSION << " " << version() << endl;
}
