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

//---------------------------------------------------------------------
// Estructura en memoria de las tablas de procesos
// (la tabla de respuestas es la tabla 0)
//
// Proceso 0
// {
//    Entrada 0
//    ...
//    Entrada n
//    Codigo 255 (?)
//    Tabla entradas proceso 0
// }
// ...
// Proceso n
// {
//    Entrada 0
//    ...
//    Entrada n
//    Codigo 255 (?)
//    Tabla entradas proceso n
// }
// Indices tablas de procesos (apuntados por ptrDatos)
//----------------------------------------------------------------------

#include "ListaProcesos.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include "ListaEntradasVolcadas.h"
#include <vector>
#include <list>

using namespace Inpaws;
using namespace std;

ListaProcesos::ListaProcesos(PtrBuffer ptrIndice, SpWord offNumPro)
: PawPart(ptrIndice), _offNumPro(offNumPro)
{
}

ListaProcesos::~ListaProcesos()
{
    for (TListaProcesos::const_iterator it = _listaProcesos.begin();
         it != _listaProcesos.end();
         it++)
    {
        delete (*it).second;
    }
}


PtrBuffer
ListaProcesos::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    vector<PtrBuffer> _indiceProcesos;
    list<ListaEntradasVolcadas::EntradaLista > _indiceProcesoActual;
    ListaEntradasVolcadas listaVolcadas; // Para reutilizar entradas iguales
    PtrBuffer ptrActual = ptrInicioDatos;


    // Fijamos el numero de procesos
    SPBUFFER->bytePoke(SPBUFFER->wordPeek(PTR_MAINTOP) + _offNumPro, _listaProcesos.size());
    for (TListaProcesos::const_iterator itProceso = _listaProcesos.begin();
         itProceso != _listaProcesos.end(); itProceso++)
    {
         // Escribimos las entradas del proceso actual
         const EntradaProcesos* entradaActual = (*itProceso).second->leerEntradaInit();
         while (entradaActual != NULL)
         {
             ListaEntradasVolcadas::EntradaLista entradaIndice;
             entradaIndice.entrada = entradaActual;
             //pair<SpByte, SpByte> verboNombre = pair<SpByte, SpByte>(entradaActual->verbo(), entradaActual->nombre());
             PtrBuffer dirVolcada = listaVolcadas.buscarEntrada(entradaActual);
             if (dirVolcada)
             {
                 entradaIndice.dirVolcada = dirVolcada;
                 _indiceProcesoActual.push_back(entradaIndice);
                 //cout << "Insertado " << (int)entradaActual->verbo() << " " << (int)entradaActual->nombre()
                 // << ": " << (unsigned int)dirVolcada << endl;
             }
             else
             {
                 entradaIndice.dirVolcada = ptrActual;
                 _indiceProcesoActual.push_back(entradaIndice);
                 listaVolcadas.anadirEntrada(entradaActual, ptrActual);
                 ptrActual = entradaActual->volcarAMemoria(ptrActual) + 1;
             }
             entradaActual = (*itProceso).second->leerEntradaNext();
         }
         PtrBuffer ultimaEntrada = ptrActual++;
         SPBUFFER->bytePoke (ultimaEntrada, 255); // ????
         // Ahora volcamos la tabla indice de verbo-nombre
         // a condactos para el proceso actual
         _indiceProcesos.push_back(ptrActual);
         for (list<ListaEntradasVolcadas::EntradaLista >::const_iterator it = _indiceProcesoActual.begin();
              it != _indiceProcesoActual.end();
              it++)
         {
             SPBUFFER->bytePoke(ptrActual, (*it).entrada->verbo());
             SPBUFFER->bytePoke(ptrActual+1, (*it).entrada->nombre());
             SPBUFFER->wordPoke(ptrActual+2, (*it).dirVolcada);

             //cout << (int)SPBUFFER->bytePeek(ptrActual) << " " << (int)SPBUFFER->bytePeek(ptrActual+1)
             //     << ": " << (unsigned int)SPBUFFER->wordPeek(ptrActual+2) << endl;

             ptrActual += 4;

         }

         // Y terminamos con un cero y borrando la estructura para el siguiente
         SPBUFFER->bytePoke(ptrActual, 0);
         SPBUFFER->bytePoke(ptrActual+1, 0);
         SPBUFFER->wordPoke(ptrActual+2, ultimaEntrada);
         ptrActual += 4;
         _indiceProcesoActual.clear();
    }
    // Por último volcamos los indices a procesos y fijamos el
    // valor de PTR_PROCESOS
    SPBUFFER->wordPoke(PTR_PROCESOS, ptrActual);

    for (vector<PtrBuffer>::const_iterator it2 = _indiceProcesos.begin();
         it2 != _indiceProcesos.end();
         it2++)
    {
        SPBUFFER->wordPoke(ptrActual, *it2);
        ptrActual +=2 ;
    }

    // También hay que pokear cierta direccion de memoria con el
    // numero de procesos, si no no funciona el editor (empírico)
    SPBUFFER->bytePoke(65532, _listaProcesos.size());

    return ptrActual-1;
}

PtrBuffer
ListaProcesos::volcarDeMemoria ()
{
    PtrBuffer ptrProc, ptrEntradas;
    SpByte verbo, nombre;
    Proceso* nuevoProceso;

    getInicioDatos();              // Fijar ptrDatos al sitio desde donde hay que leer

    SpByte numProcs = SPBUFFER->bytePeek(SPBUFFER->wordPeek(PTR_MAINTOP) + _offNumPro);

    for (unsigned int i = 0; i < numProcs; i++)
    {
        nuevoProceso = new Proceso();
        ptrProc = SPBUFFER->wordPeek(ptrDatos() + 2*i);
        verbo = SPBUFFER->bytePeek(ptrProc);
        while (verbo != 0)
        {
            nombre = SPBUFFER->bytePeek(ptrProc + 1);
            ptrEntradas = SPBUFFER->wordPeek (ptrProc + 2);

            EntradaProcesos* nuevaEntrada =
              new EntradaProcesos(verbo, nombre);
            ptrEntradas = nuevaEntrada->volcarDeMemoria(ptrEntradas);

            nuevoProceso->anadirEntrada(nuevaEntrada);
            ptrProc += 4;
            verbo = SPBUFFER->bytePeek(ptrProc);
        }

        anadirProceso(i, nuevoProceso);
    }

    return ptrDatos()+2*numProcs-1;
}

void
ListaProcesos::generarCodigo (std::ostream& os)
{
    os << RSV_PROCESS_TABLE << endl << "{" << endl;
    for (unsigned int i=0; i < _listaProcesos.size(); i++)
    {
        os << '\t' << RSV_PROCESS << " " << i << endl << '\t' << "{" << endl;
        const Proceso* proceso = obtenerProceso(i);
        if (proceso != NULL)
        {
            EntradaProcesos* entrada = proceso->leerEntradaInit();
            while (entrada)
            {
                os << "\t\t" << (unsigned int)entrada->verbo() << " "
                             << (unsigned int)entrada->nombre() << " : ";
                entrada->generarCodigo(os);
                os << endl;
                entrada = proceso->leerEntradaNext();
            }
        }

        os << '\t' << "}" << endl;
    }
    os << "}" << endl;
}

void
ListaProcesos::generarCodigoDisco (std::ostream& os, uint16_t flags, const Vocabulario* vocabulario,
                                   const Diccionario::TDiccionarioInverso* diccionario)
{
    os << "; " << RSV_PROCESS_TABLE << endl;
    for (unsigned int i=0; i < _listaProcesos.size(); i++)
    {
        os << "/PRO ";
        if (diccionario)
        {
            Diccionario::TDiccionarioInverso::const_iterator it = diccionario->find(pair<int, int>(i, PROCNO));
            if (it == diccionario->end())
                os << i;
            else
                os << (*it).second;
        }
        else
        {
            os << i;
        }

        os << endl;


        const Proceso* proceso = obtenerProceso(i);
        if (proceso != NULL)
        {
            EntradaProcesos* entrada = proceso->leerEntradaInit();
            while (entrada)
            {
                os << endl;
                const Vocabulario::Palabra* verbo =
                    (vocabulario->leerPalabraInit(entrada->verbo(), Vocabulario::VERB) != NULL?
                    vocabulario->leerPalabraInit(entrada->verbo(), Vocabulario::VERB) :
                    vocabulario->leerPalabraInit(entrada->verbo(), Vocabulario::NOUN));
                const Vocabulario::Palabra* nombre =
                    vocabulario->leerPalabraInit(entrada->nombre(), Vocabulario::NOUN);

                string verboStr = (verbo?verbo->palabra(flags):"_");
                string nombreStr = (nombre?nombre->palabra(flags):"_");


                if (flags&CONVERTSPE && i>0)
                    os << "_\t_\t";
                else {
                    os << verboStr << "\t"
                       << nombreStr << "\t";
                }

                entrada->generarCodigo(os, vocabulario, flags, diccionario);

                entrada = proceso->leerEntradaNext();
            }
        }
        os << endl;
    }
}

void
ListaProcesos::generarCodigoSuperglus (std::ostream& os, uint16_t flags, const Vocabulario* vocabulario,
                                       const Diccionario::TDiccionarioInverso& diccionario)
{
    generarCodigoDisco(os, flags, vocabulario, &diccionario);
}


void
ListaProcesos::leerDeCodigo (Tokeniser& tokeniser)
{
    Tokeniser::TTokenType token;
    SpByte numProcTmp;

    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, RSV_PROCESS_TABLE);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");

    token = tokeniser.leeToken();
    while (!(token == Tokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
        // El literal "PROCESS"
        if (token == Tokeniser::TK_IDENTIFICADOR && tokeniser.lvalue().literal == RSV_PROCESS)
        {
            // El numero de proceso
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_NUMERO)
            {
                numProcTmp = tokeniser.lvalue().valor;
                saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");
                _leerDeCodigoProceso(tokeniser, numProcTmp);
                saltarToken (tokeniser, Tokeniser::TK_CONTROL, "}");
            }
        } else throw ExcTokeniser(tokeniser.errorLeidoEsperado(RSV_PROCESS));
        token = tokeniser.leeToken();
    }
}

void
ListaProcesos::anadirProceso (unsigned int numProc, Proceso *proceso)
{
    if (_listaProcesos.count(numProc))
    {
        delete (*(_listaProcesos.find(numProc))).second;
        _listaProcesos.erase(numProc); // Si existe, lo machacamos
    }

    _listaProcesos.insert(pair<unsigned int, Proceso*>(numProc, proceso));
}

const Proceso*
ListaProcesos::obtenerProceso(unsigned int numProc) const
{
    TListaProcesos::const_iterator it = _listaProcesos.find(numProc);
    if (it != _listaProcesos.end())
        return (*it).second;
    else
        return NULL;
}

void
ListaProcesos::_leerDeCodigoProceso (Tokeniser& tokeniser, SpByte numProc)
{
    Tokeniser::TTokenType token = tokeniser.leeToken();
    Proceso* procActual = new Proceso;
    EntradaProcesos* entradaActual(NULL);
    SpByte verboActual, nombreActual;

    while (token == Tokeniser::TK_NUMERO)
    {
        verboActual = tokeniser.lvalue().valor;
        token = tokeniser.leeToken();
        if (token == Tokeniser::TK_NUMERO)
            nombreActual = tokeniser.lvalue().valor;
        else
        {
            delete procActual;
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NOMBRE_ENTRADA_PROCESO));
        }

        try
        {
          saltarToken (tokeniser, Tokeniser::TK_CONTROL, ":");
        } catch (...)
        {
            delete procActual;
            throw;
        }

        entradaActual = new EntradaProcesos(verboActual, nombreActual);
        try {
            entradaActual->leerDeCodigo(tokeniser);
            saltarToken (tokeniser, Tokeniser::TK_CONTROL, ";");
        } catch (...)
        {
            delete entradaActual;
            delete procActual;
            throw;
        }

        procActual->anadirEntrada(entradaActual);

        token = tokeniser.leeToken();
    }
    anadirProceso(numProc, procActual);

    tokeniser.putback();
}

PtrBuffer
ListaProcesos::inicioReal() const
{
    // El inicio es la primera entrada del primer proceso
    PtrBuffer primerProceso = SPBUFFER->wordPeek (ptrDatos());

    // Los dos primeros octetos corresponden con el verbo y nombre de la entrada
    // el tercero y cuarto son la dirección de la entrada
    return SPBUFFER->wordPeek(primerProceso+2);
}
