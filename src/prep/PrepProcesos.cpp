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

#include "prep/PrepProcesos.h"
#include "prep/MsgPrep.h"
#include "prep/Error.h"
#include "MsgAplicacion.h"
#include "ListaProcesos.h"
#include "Vocabulario.h"
#include "defines.h"
#include "PawCondacts.h"
#include "UtilTexto.h"
#include <cstdlib>

using namespace Inpaws;
using namespace std;

PrepProcesos::PrepProcesos(PrepListaMensajes& listaMensajes,
                           PrepListaMensajes& listaSisMensajes,
                           PrepVocabulario& vocabulario)
 : PrepPart(RSV_PROCESS_TABLE), _listaMensajes(listaMensajes),
   _listaSisMensajes(listaSisMensajes), _vocabulario(vocabulario)
{
    Proceso procesoNuevo;
    _procesos.insert(pair<string, Proceso>("0", procesoNuevo)); // Response
    _procesos.insert(pair<string, Proceso>("1", procesoNuevo));
    _procesos.insert(pair<string, Proceso>("2", procesoNuevo));
}

PrepProcesos::~PrepProcesos()
{
}

void
PrepProcesos::generarCodigoPrep (std::ostream& os, const Aventura& aventura) const
{
    const ListaProcesos* procesos = dynamic_cast<const ListaProcesos*>(aventura.getModulo(RSV_PROCESS_TABLE));
    const Vocabulario* vocabulario = dynamic_cast<const Vocabulario*>(aventura.getModulo(RSV_VOCABULARY));

    // Declaraciones
    for (size_t i = 3; i < procesos->numPro(); i++)
    {
        os << RSV_PROCESS << " " << i << ";" << endl;
    }
    os << endl;

    // Definiciones
    for (size_t i = 0; i < procesos->numPro(); i++)
    {
        if (i == 0)
          os << RSV_RESPONSE;
        else
          os << RSV_PROCESS << " " << i;

        os << endl << "{" << endl;

        for (const EntradaProcesos* entrada = procesos->obtenerProceso(i)->leerEntradaInit();
             entrada != NULL;
             entrada = procesos->obtenerProceso(i)->leerEntradaNext())
        {
            os << '\t';
            const Vocabulario::Palabra* verbo = vocabulario->leerPalabraInit(entrada->verbo(), Vocabulario::VERB);
            if (!verbo)
                verbo = vocabulario->leerPalabraInit(entrada->verbo(), Vocabulario::NOUN);

            const Vocabulario::Palabra* nombre = vocabulario->leerPalabraInit(entrada->nombre(), Vocabulario::NOUN);

            if (entrada->verbo() == 255)
              os << '_';
            else if (entrada->verbo() == 1)
              os << '*';
            else os << verbo->palabraCompleta;
            os << " ";
            if (entrada->nombre() == 255)
              os << '_';
            else if (entrada->nombre() == 1)
              os << '*';
            else os << nombre->palabraCompleta;
            os << ": ";
            entrada->generarCodigo(os, vocabulario);
            os << endl;
        }

        os << "}" << endl << endl;
    }
}

void
PrepProcesos::leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    PrepTokeniser::TTokenType token;
    string nomProceso;
    token = tokeniser.leeToken();

    if (token == PrepTokeniser::TK_IDENTIFICADOR && UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_PROCESS)
    {
        token = tokeniser.leeToken();
        if (tokeniser.lvalue().literal == "0") // No se puede, está reservado para la response
        {
            ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_NOMPROCESO), Error::FATAL);
        }
        if (token == PrepTokeniser::TK_IDENTIFICADOR || token == PrepTokeniser::TK_NUMERO)
        {
            nomProceso = tokeniser.lvalue().literal;
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_NOMPROCESO), Error::FATAL);

        // Si el siguiente es un "{", se trata de una definición, y si es un ";" es una declaracion
        PrepTokeniser::TTokenType tmpToken = token; // Guardamos para comprobar luego
        SpByte tmpValor = tokeniser.lvalue().valor;
        unsigned long tmpLinea = tokeniser.lineaActual();
        string tmpArchivo = tokeniser.archivoActual();
        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == ";")
        {
            if (diccionario.buscarEntrada(nomProceso, PROCNO) != -2)
            {
                ERRORPAW(tmpArchivo, tmpLinea, nombre()+" ["+nomProceso+"] "+MSGPREP_YADEFINIDO, Error::FATAL);
            }
            // Añadimos la nueva entrada
            if (tmpToken == PrepTokeniser::TK_IDENTIFICADOR)
                diccionario.anadirEntrada(nomProceso, PROCNO);
            else
            {
                diccionario.anadirEntrada(nomProceso, PROCNO, tmpValor);
                anadirPool (tmpValor);
            }

            // Creamos un proceso nuevo vacío
            Proceso procesoNuevo;
            _procesos.insert(pair<string, Proceso>(UtilTexto::toUpr(nomProceso), procesoNuevo));

            return;
        } else if (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "{")
        {
            tokeniser.putback();
            // Si el proceso no ha sido previamente declarado, dará error
            if (nomProceso != "0" && nomProceso != "1" && nomProceso != "2" &&
                diccionario.buscarEntrada(nomProceso, PROCNO) == -2)
            {
                ERRORPAW(tmpArchivo, tmpLinea, "[" + nomProceso + "] " + MSGPREP_PROCNODEC, Error::FATAL);
            }
        }
        else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_DEFPROC), Error::FATAL);

    }
    else if (token == PrepTokeniser::TK_IDENTIFICADOR && UtilTexto::toUpr(tokeniser.lvalue().literal) == RSV_RESPONSE)
    {
        nomProceso = "0";
    }

    // Empezamos a leer el código del proceso o tabla de respuestas
    Proceso& procesoActual = (*(_procesos.find(UtilTexto::toUpr(nomProceso)))).second;
    saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, "{");
    token = tokeniser.leeToken();
    while (!(token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "}"))
    {
        tokeniser.putback(); // Devolvemos el verbo
        PrepEntradaMultiple nuevaEntrada;
        _leerVerboNombre (nuevaEntrada, tokeniser);
        saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ":");
        _leerCondactos (nuevaEntrada, tokeniser, diccionario);
        saltarToken (tokeniser, PrepTokeniser::TK_CONTROL, ";");

        for (const PrepEntradaMultiple::EntradaProceso* entradaActual = nuevaEntrada.leerEntradaInit();
             entradaActual;
             entradaActual = nuevaEntrada.leerEntradaNext())
        {
            procesoActual.push_back(*entradaActual);
        }
        token = tokeniser.leeToken();
    }
}

void
PrepProcesos::asignarValoresSimbolos(Diccionario& diccionario)
{
    if (diccionario.buscarEntrada("0", PROCNO) == -2)
      diccionario.anadirEntrada("0", PROCNO, 0);
    anadirPool (0);

    if (diccionario.buscarEntrada("1", PROCNO) == -2)
      diccionario.anadirEntrada("1", PROCNO, 1);
    anadirPool (1);

    if (diccionario.buscarEntrada("2", PROCNO) == -2)
      diccionario.anadirEntrada("2", PROCNO, 2);
    anadirPool (2);

    for (TProcesos::const_iterator it = _procesos.begin();
         it != _procesos.end();
         it++)
    {
        if (diccionario.buscarEntrada((*it).first, PROCNO) == -1)
        {
            diccionario.fijarValor((*it).first, PROCNO, dameValorPool());
        }
    }
    if (!poolDefinicionesOk()) // Comprueba que todas las definiciones son correlativas
    {
        ERRORPAW("", 0, nombre() + ": "+MSGPREP_DEFCORR, Error::FATAL);
    }
}

void
PrepProcesos::generarCodigo(std::ostream& os, const Diccionario& diccionario) const
{
    os << RSV_PROCESS_TABLE << endl << "{" << endl;
    for (TProcesos::const_iterator it = _procesos.begin();
         it != _procesos.end();
         it++)
    {
        _volcarProceso (os, diccionario, it);
    }

    os << "}" << endl << endl;
}

void
PrepProcesos::_leerVerboNombre (PrepEntradaMultiple& entrada, PrepTokeniser& tokeniser) const
{
    // Lee la pareja verbo-nombre de cada entrada de un proceso
    // Contempla sinónimos en la forma "MONTA COCHE|ENTRA COCHE: xxxx"
    string verbo, nombre, archivoVerbo, archivoNombre;
    unsigned long lineaVerbo = 0, lineaNombre = 0;
    PrepTokeniser::TTokenType token;
    do
    {
        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_IDENTIFICADOR ||
            token == PrepTokeniser::TK_NUMEROGRANDE || token == PrepTokeniser::TK_NUMERO || token == PrepTokeniser::TK_NUMEROVOCABULARIO
            || (token == PrepTokeniser::TK_CONTROL &&
                   (tokeniser.lvalue().literal == "_" || tokeniser.lvalue().literal == "*")))
        {
            verbo = tokeniser.lvalue().literal;
            lineaVerbo = tokeniser.lineaActual();
            archivoVerbo = tokeniser.archivoActual();
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_PALENTRADAPROC), Error::FATAL);

        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_IDENTIFICADOR ||
            token == PrepTokeniser::TK_NUMEROGRANDE || token == PrepTokeniser::TK_NUMERO || token == PrepTokeniser::TK_NUMEROVOCABULARIO
            || (token == PrepTokeniser::TK_CONTROL &&
                   (tokeniser.lvalue().literal == "_" || tokeniser.lvalue().literal == "*")))
        {
            nombre = tokeniser.lvalue().literal;
            lineaNombre = tokeniser.lineaActual();
            archivoNombre = tokeniser.archivoActual();
        } else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_PALENTRADAPROC), Error::FATAL);


        // Añadimos a la entrada múltiple
        entrada.anadirVerboNombre (verbo, lineaVerbo, archivoVerbo,
                                   nombre, lineaNombre, archivoNombre);
        token = tokeniser.leeToken();

    } while (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "|");
    tokeniser.putback();
}

void
PrepProcesos::_leerCondactos (PrepEntradaMultiple& entrada, PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    bool nuevo = true; // nuevo=true cuando no se trata de una opcion en un OR (simbolo '|')
    unsigned long lineaCond;
    string archivoCond;
    PrepTokeniser::TTokenType token = tokeniser.leeToken();
    do
    {
        tokeniser.putback(); // Devolvemos el nombre del condacto, lo leera la otra funcion
        lineaCond = tokeniser.lineaActual();
        archivoCond = tokeniser.archivoActual();
        PrepEntradaMultiple::Condacto nuevoCondacto;

        _leerCondacto (nuevoCondacto, tokeniser, diccionario);

        entrada.anadirCondacto(nuevoCondacto, nuevo);

        token = tokeniser.leeToken();
        if (token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == "|")
        {
            nuevo = false;
            if (!PawCondacts::pawCondacts()->esCondicion(nuevoCondacto.nombre))
                ERRORPAW(archivoCond, lineaCond, "["+nuevoCondacto.nombre+"] " + MSGPREP_NOCOND, Error::FATAL);

            token = tokeniser.leeToken();
        }
        else
        {
            if (nuevo == false && !PawCondacts::pawCondacts()->esCondicion(nuevoCondacto.nombre))
                ERRORPAW(archivoCond, lineaCond, "["+nuevoCondacto.nombre+"] " + MSGPREP_NOCOND, Error::FATAL);
            nuevo = true;
        }
    }
    while (!(token == PrepTokeniser::TK_CONTROL && tokeniser.lvalue().literal == ";"));
    tokeniser.putback(); // Devolvemos el ;
}

void
PrepProcesos::_leerCondacto (PrepEntradaMultiple::Condacto& condacto, PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    PrepTokeniser::TTokenType token = tokeniser.leeToken();

    if (token == PrepTokeniser::TK_IDENTIFICADOR &&
        PawCondacts::pawCondacts()->existeCondacto(UtilTexto::toUpr(tokeniser.lvalue().literal)))
    {
        condacto.nombre = UtilTexto::toUpr(tokeniser.lvalue().literal);
        const ::Inpaws::Condacto* infoCondacto = PAWCONDACT(UtilTexto::toUpr(tokeniser.lvalue().literal));
        for (SpByte i = 1; i <= infoCondacto->numParams(); i++)
        {
            _leerParametroCondacto(i, condacto, infoCondacto, tokeniser, diccionario);
        }
    }
    else ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSGPREP_NOMCONDACTO), Error::FATAL);
}

void
PrepProcesos::_leerParametroCondacto(SpByte param, PrepEntradaMultiple::Condacto& condacto,
                                     const ::Inpaws::Condacto* infoCondacto,
                                     PrepTokeniser& tokeniser, Diccionario& diccionario)
{
    UtilTexto utilTexto;
    PrepTokeniser::TTokenType token = tokeniser.leeToken();
    bool prmLiteralMensaje = false;

    if (token == PrepTokeniser::TK_ERROR)
    {
        ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), tokeniser.errorLeidoEsperado(MSG_PARAMETRO_CONDACTO), Error::FATAL);
    }
    else if (token == PrepTokeniser::TK_LITERAL)
    {
        if ((param == 1 && infoCondacto->tipo_prm1() == MESNO) ||
            (param == 2 && infoCondacto->tipo_prm2() == MESNO))
        {
            if (!utilTexto.textoCorrecto(tokeniser.lvalue().literal))
            {
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[" + nombre() + "] " + utilTexto.errorStr(), Error::FATAL);
            }
            if (diccionario.buscarEntrada("\""+tokeniser.lvalue().literal+"\"", MESNO) == -2)
            {
                _listaMensajes.anadirMensaje("\""+tokeniser.lvalue().literal+"\"", tokeniser.lvalue().literal, diccionario);
            }
            prmLiteralMensaje = true;
        }
        else if ((param == 1 && infoCondacto->tipo_prm1() == SYSNO) ||
                 (param == 2 && infoCondacto->tipo_prm2() == SYSNO))
        {
            if (diccionario.buscarEntrada("\""+tokeniser.lvalue().literal+"\"", SYSNO) == -2)
            {
                _listaSisMensajes.anadirMensaje("\""+tokeniser.lvalue().literal+"\"", tokeniser.lvalue().literal,
                                                diccionario);
            }
            prmLiteralMensaje = true;
        }
    }
    else if ((param == 1 && infoCondacto->tipo_prm1() == VALOR) ||
            (param == 2 && infoCondacto->tipo_prm2() == VALOR))
    {
        if (param == 1)
        {
            if (token == PrepTokeniser::TK_NUMERO &&
                tokeniser.lvalue().valor > infoCondacto->max_valor_prm1())
            {
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[" + tokeniser.lvalue().literal + "] " + MSGPREP_VALORALTO, Error::FATAL);
            }
        }
        else if (param == 2)
        {
            if (token == PrepTokeniser::TK_NUMERO &&
                tokeniser.lvalue().valor > infoCondacto->max_valor_prm2())
            {
                ERRORPAW(tokeniser.archivoActual(), tokeniser.lineaActual(), "[" + tokeniser.lvalue().literal + "] " + MSGPREP_VALORALTO, Error::FATAL);
            }
        }
    }

    // Finalmente asignamos los datos del parametro
    if (param == 1)
    {
        if (prmLiteralMensaje)
            condacto.param1  = "\"" + tokeniser.lvalue().literal + "\"";
        else
            condacto.param1 = tokeniser.lvalue().literal;
        condacto.lineaParam1 = tokeniser.lineaActual();
        condacto.archivoParam1 = tokeniser.archivoActual();
    }
    else
    {
        if (prmLiteralMensaje)
            condacto.param2 = "\"" + tokeniser.lvalue().literal + "\"";
        else
            condacto.param2 = tokeniser.lvalue().literal;
        condacto.lineaParam2 = tokeniser.lineaActual();
        condacto.archivoParam2 = tokeniser.archivoActual();
    }
}

void
PrepProcesos::_volcarProceso (std::ostream& os, const Diccionario& diccionario,
                              const TProcesos::const_iterator& procItr) const
{
    const string& nomproc = (*procItr).first;
    const Proceso& proceso = (*procItr).second;

    os << '\t' << RSV_PROCESS << " " << diccionario.buscarEntrada(nomproc, PROCNO);

    os << endl << "\t{" << endl;
    for (std::vector<PrepEntradaMultiple::EntradaProceso>::const_iterator it = proceso.begin();
         it != proceso.end();
         it++)
    {
        _volcarEntradaProceso (os, diccionario, it);
    }
    os << "\t}" << endl;
}

void
PrepProcesos::_volcarEntradaProceso (std::ostream& os, const Diccionario& diccionario,
                              const std::vector<PrepEntradaMultiple::EntradaProceso>::const_iterator& entradaItr) const
{
    int verbo, nombre;

    verbo = diccionario.buscarEntrada((*entradaItr).verbo, WORD);
    if (verbo != -2)
    {
        if ((*entradaItr).verbo != "_" && (*entradaItr).verbo != "*" &&
            ! (_vocabulario.existePalabra(UtilTexto::toUpr((*entradaItr).verbo), Vocabulario::VERB) ||
               (_vocabulario.existePalabra(UtilTexto::toUpr((*entradaItr).verbo), Vocabulario::NOUN) && verbo < 20)))
        {
            ERRORPAW((*entradaItr).archivoVerbo, (*entradaItr).lineaVerbo, "[" + (*entradaItr).verbo + "] " + MSGPREP_PALNOVAL, Error::NORMAL);
        }
    } else ERRORPAW((*entradaItr).archivoVerbo, (*entradaItr).lineaVerbo, "[" + (*entradaItr).verbo + "] " + MSGPREP_PALABRANODEF, Error::NORMAL);

    nombre = diccionario.buscarEntrada((*entradaItr).nombre, WORD);
    if (nombre != -2)
    {
        if ((*entradaItr).nombre != "_" && (*entradaItr).nombre != "*" &&
            !_vocabulario.existePalabra(UtilTexto::toUpr((*entradaItr).nombre), Vocabulario::NOUN))
        {
            ERRORPAW((*entradaItr).archivoNombre, (*entradaItr).lineaNombre, "[" + (*entradaItr).nombre + "] " + MSGPREP_PALNOVAL, Error::NORMAL);
        }
    } else ERRORPAW((*entradaItr).archivoNombre, (*entradaItr).lineaNombre, "[" + (*entradaItr).nombre + "] " + MSGPREP_PALABRANODEF, Error::NORMAL);

    os << "\t\t" << verbo << " " << nombre << ": ";
    for (std::vector<PrepEntradaMultiple::Condacto>::const_iterator itCond = (*entradaItr).listaCondactos.begin();
         itCond != (*entradaItr).listaCondactos.end();
         itCond++)
    {
        _volcarCondacto (os, diccionario, itCond);
        os << " ";
    }
    os << ";" << endl;
}

void
PrepProcesos::_volcarCondacto (std::ostream& os, const Diccionario& diccionario,
                      const std::vector<PrepEntradaMultiple::Condacto>::const_iterator& itCond) const
{
    int prm1 = -1, prm2 = -1;
    const ::Inpaws::Condacto* infoCondacto = PAWCONDACT((*itCond).nombre);

    if (infoCondacto->numParams() > 0)
    {
        prm1 = _obtenerValorPrm((*itCond).param1, (*itCond).archivoParam1, (*itCond).lineaParam1, infoCondacto->tipo_prm1(), diccionario);
        if (infoCondacto->numParams() > 1)
            prm2 = _obtenerValorPrm((*itCond).param2, (*itCond).archivoParam2, (*itCond).lineaParam2, infoCondacto->tipo_prm2(), diccionario);

        if (infoCondacto->tipo_prm1() == WORD &&
            diccionario.buscarEntrada((*itCond).param1, WORD) != -2 &&
            !_esTipoPalabraCorrecta((*itCond).nombre,
            UtilTexto::toUpr((*itCond).param1)))
        {
            ERRORPAW((*itCond).archivoParam1, (*itCond).lineaParam1, "[" +(*itCond).param1+"] "+ MSGPREP_PALNOVAL, Error::FATAL);
        }
        if (infoCondacto->numParams() > 1 &&
            infoCondacto->tipo_prm2() == WORD &&
            diccionario.buscarEntrada((*itCond).param2, WORD) != -2 &&
            !_esTipoPalabraCorrecta((*itCond).nombre, UtilTexto::toUpr((*itCond).param2)))
        {
            ERRORPAW((*itCond).archivoParam2, (*itCond).lineaParam2, "[" +(*itCond).param2+"] "+ MSGPREP_PALNOVAL, Error::FATAL);
        }
    }

    os << (*itCond).nombre;
    if (prm1 != -1)
      os << " " << prm1;
    if (prm2 != -1)
      os << " " << prm2;
}

int
PrepProcesos::_obtenerValorPrm(const std::string& valorStr, const string& archivo, unsigned long linea,
                               int tipo, const Diccionario& diccionario) const
{
    int valorDevuelto;

    if (tipo == LOCNO_PLUS)
    {
        valorDevuelto = diccionario.buscarEntrada(valorStr, LOCNO);
    }
    else if (tipo == LOCNO && (valorStr == RSV_CARRIED || valorStr == "WORN"))
    {
        valorDevuelto = -2;
    }
    else if (tipo == VALOR)
    {
        if (_esValorNumerico(valorStr)) // Si es un valor numérico, no se busca en el diccionario
            valorDevuelto = atoi (valorStr.c_str());
        else
        {
            valorDevuelto = diccionario.buscarEntrada(valorStr, WORD);
            if (valorDevuelto == -2)
                valorDevuelto = diccionario.buscarEntrada(valorStr, LOCNO);
            if (valorDevuelto == -2)
                valorDevuelto = diccionario.buscarEntrada(valorStr, OBJNO);
            if (valorDevuelto == -2)
                valorDevuelto = diccionario.buscarEntrada(valorStr, VALOR);
        }
    }
    else
        valorDevuelto = diccionario.buscarEntrada(valorStr, tipo);

    if (valorDevuelto == -2)
    {
        if (tipo == LOCNO_PLUS &&
            (valorStr == "252" || valorStr == "253" ||valorStr == "254" ||valorStr == "255"))
        {
            valorDevuelto = atoi (valorStr.c_str());
        }
        else if (tipo == FLAGNO)
        {
            if (_esValorNumerico(valorStr))
            {
                valorDevuelto = atoi (valorStr.c_str());
                if (valorDevuelto > 255)
                    ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_VALORALTO, Error::NORMAL);
            }
        }
    }

    if (valorDevuelto == -2)
    {
        switch (tipo)
        {
            case LOCNO:
                if (valorStr == "252" || valorStr == "253" ||valorStr == "254" ||valorStr == "255" ||
                    valorStr == RSV_CARRIED || valorStr == RSV_WORN)
                    ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_NOLOCNOPLUS, Error::NORMAL);
                else
                    ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_LOCNODEF, Error::NORMAL);
                break;
            case LOCNO_PLUS:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_LOCNODEF, Error::NORMAL);
                break;
            case WORD:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_PALABRANODEF, Error::NORMAL);
                break;
            case PROCNO:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_PROCNODEC, Error::NORMAL);
                break;
            case MESNO:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_MESNODEF, Error::NORMAL);
                break;
            case SYSNO:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_SISMESNODEF, Error::NORMAL);
                break;
            case OBJNO:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_OBJNODEF, Error::NORMAL);
                break;
            case FLAGNO:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_FLAGNODEF, Error::NORMAL);
                break;
            default:
                ERRORPAW(archivo, linea, "[" + valorStr + "] " + MSGPREP_IDNODEF, Error::NORMAL);
                break;
        }

    }

    return valorDevuelto;
}

bool
PrepProcesos::_esTipoPalabraCorrecta (const std::string& condacto, const std::string& palabra) const
{
    if (palabra == "_" || palabra == "*")
        return true;

    if (condacto == "NOUN2" && _vocabulario.existePalabra (palabra, Vocabulario::NOUN))
        return true;
    else if ((condacto == "ADJECT1"||condacto=="ADJECT2") &&
             _vocabulario.existePalabra (palabra, Vocabulario::ADJECTIVE))
        return true;
    else if (condacto == "PREP" && _vocabulario.existePalabra (palabra, Vocabulario::PREPOSITION))
        return true;
    else if (condacto == "ADVERB" && _vocabulario.existePalabra (palabra, Vocabulario::ADVERB))
        return true;

    return false;
}

bool
PrepProcesos::_esValorNumerico (const std::string& cadena) const
{
    for (string::const_iterator it = cadena.begin();
         it != cadena.end();
         it++)
    {
        if (*it < '0' || *it > '9')
            return false;
    }
    return true;
}
