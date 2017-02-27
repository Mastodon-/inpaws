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

#include <fstream>
#include <sstream>
#include "Excepcion.h"
#include "prep/Preprocesador.h"
#include "UtilTexto.h"
#include "Fichero.h"
#include "prep/PrepTokeniser.h"
#include <cstdlib>
#include <cstring>

#define INPAWS_VERSION "1.0.2"
#define INPAWS_BUILD "170227"
#define INPAWS_COPYRIGHT "Copyright (c) 2009-2016 Francisco Javier Lopez"

using namespace std;
using namespace Inpaws;

void leerParametros(int argc, char** argv);
void sintaxis();
void compilarFuente();
void extraerFuente();
void compilarAPrep();
void extraerGraficosCaracteres(const string& comando);
void compilarDePrep();
void extraeAPrep();
void compilarADisco(uint16_t flags); // Generar un .SCE para PAW CPM/PC o para Superglus sin símbolos

string comando;
string ficheroEntrada;
string ficheroSalida;
bool detalleSimbolos = false; // Si se desea el detalle de la tabla de simbolos tras la compilación
bool conversionDesdeSpectrum = false; // Utilizar si se generan .SCE desde aventuras extraidas de spectrum

int main(int argc, char** argv)
{
    leerParametros(argc, argv);

    try
    {
        if (comando == "c")
        {
            compilarFuente();
            exit(0);
        }
        else if (comando == "e")
        {
            extraerFuente();
            exit(0);
        }
        else if (comando == "cp")
        {
            compilarAPrep();
            exit(0);
        }
        else if (comando == "eg" || comando == "ec")
        {
            extraerGraficosCaracteres(comando);
            exit(0);
        }
        else if (comando == "compprep")
        {
            compilarDePrep();
            exit(0);
        }
        else if (comando == "extraeprep")
        {
            extraeAPrep();
            exit(0);
        }
        else if (comando == "cm") // Compilar a version disco CPM (Amstrad CPC)
        {
            int opciones = VERCPM+NOCOMPRIMIR;
            if (conversionDesdeSpectrum) opciones += CONVERTSPE;
            compilarADisco(opciones);
            exit(0);
        }
        else if (comando == "cd") // Compilar a version disco para PC
        {
            int opciones = VERPC+NOCOMPRIMIR;
            if (conversionDesdeSpectrum) opciones += CONVERTSPE;
            compilarADisco(opciones);
            exit(0);
        }
        else if (comando == "cs") // Compilar a Superglus sin símbolos (.SCE)
        {
            int opciones = VERGLULX+NOCOMPRIMIR;
            if (conversionDesdeSpectrum) opciones += CONVERTSPE;
            compilarADisco(opciones);
            exit(0);
        }
        else if (comando == "ct") // Compilar a Superglus con símbolos (.TXP)
        {
            int opciones = VERGLULX+VERTXP+NOCOMPRIMIR;
            if (conversionDesdeSpectrum) opciones += CONVERTSPE;
            compilarADisco(opciones);
            exit(0);
        }
    }
    catch (Excepcion& ex)
    {
        cerr << ex.desc() << endl;
        exit(1);
    }
    catch(...)
    {
        cerr << "ooops!!" << endl;
    }
}

void leerParametros(int argc, char** argv)
{
    if (argc < 3)
    {
        cerr << MSG_FALTAN_PARAMETROS << endl;
        sintaxis();
        exit(1);
    }

    // Comando
    if (!strcmp(argv[1],"c") || !strcmp(argv[1],"e") || !strcmp(argv[1],"cp") || !strcmp(argv[1],"cs") ||
        !strcmp(argv[1],"eg") || !strcmp(argv[1],"ec") || !strcmp(argv[1],"cm") || !strcmp(argv[1],"cd") ||
        !strcmp(argv[1],"ct") || !strcmp(argv[1], "compprep") || !strcmp(argv[1], "extraeprep"))
    {
        comando = argv[1];
        ficheroEntrada = argv[2];
    }
    else
    {
        cerr << MSG_SINTAXISINC << endl;
        sintaxis();
        exit(1);
    }

    // Opciones
    if (argc > 3)
    {
        for (int i = 3; i < argc; i++)
        {
            if (!strcmp(argv[i], "-o") && argc > i+1)
            {
                ficheroSalida = argv[i+1];
                i++;
            }
            else if (!strcmp(argv[i], "-s"))
            {
                detalleSimbolos = true;
            }
//            else if (!strcmp(argv[i], "-z"))
//            {
//                conversionDesdeSpectrum = true;
//            }
            else
            {
                cerr << MSG_SINTAXISINC << endl;
                sintaxis();
                exit(1);
            }
        }
    }
}

void sintaxis()
{
    cerr << MSG_SINTAXIS << endl;
    cerr << "InPAWS " << INPAWS_VERSION << " Build " << INPAWS_BUILD << endl;
    cerr << INPAWS_COPYRIGHT << endl;
}

void compilarFuente()
{
    IFichero fuentestr(ficheroEntrada);

    static OFichero ficheroIntermedio(ficheroEntrada + ".prep", true);

    const Tokeniser* tokeniserPtr(NULL);

    try
    {
        Preprocesador prepro(&fuentestr, ficheroEntrada);
        prepro.preprocesar(ficheroIntermedio);

        IFichero ifstr(ficheroEntrada + ".prep", true);
        Tokeniser tokeniser(&ifstr);
        tokeniserPtr = &tokeniser;
        Aventura aventura;

        aventura.leerDeCodigo(tokeniser);
        SpWord libre = aventura.volcarAMemoria();
        if (libre == 65535)
            cerr << MSG_SINMEMORIA << endl;
        else
        {
            if (ficheroSalida == "")
                ficheroSalida = ficheroEntrada + ".tap";
            SPBUFFER->writeBufferTAP(ficheroSalida);
            cout << MSG_RESUMEN_COMPILOK << ficheroSalida << endl;
            cout << MSG_MEMLIBRE << libre << endl;
            cout << "--------------------------------" << endl;
            aventura.mostrarResumen(cout);
            if (prepro.ahorroCompresion())
                cout << MSG_RESUMEN_ABREVIATURAS << " " << prepro.ahorroCompresion() << endl;
            cout << "--------------------------------" << endl;
            if (detalleSimbolos)
            {
                cout << MSG_SIMBOLOSUSADOS << endl
                     << "--------------------------------" << endl;
                prepro.mostrarSimbolos(cout);
            }
        }
    }
    catch (ExcTokeniser& ex)
    {
        throw;
    }
    catch (Excepcion& ex)
    {
        ostringstream ostr;
        ostr << tokeniserPtr->lineaActual() << ": [EXCEPCION]" << ex.desc();
        throw Excepcion(ostr.str());
    }

}

void extraerFuente()
{
    if (ficheroEntrada.find(".") != string::npos)
    {
        string nombreUpr = UtilTexto::toUpr(ficheroEntrada);
        if (nombreUpr.find(".Z80") != string::npos)
        {
            SPBUFFER->readBufferZ80(ficheroEntrada);
        }
        else if (nombreUpr.find(".SNA") != string::npos)
        {
            SPBUFFER->readBufferSNA(ficheroEntrada);
        }
        else throw Excepcion("\""+ficheroEntrada+"\": " + MSG_TIPONOREC);
    } else throw Excepcion("\""+ficheroEntrada+"\": " + MSG_TIPONOREC);

    ifstream nullStr;
    Aventura aventura;
    if (!aventura.pawSignature())
        throw Excepcion("\""+ficheroEntrada+"\": " + MSG_NOBDPAW);
    aventura.volcarDeMemoria();

    Preprocesador prepro(&nullStr);
    if (ficheroSalida == "")
        prepro.generarCodigoPrep(cout, aventura);
    else
    {
        OFichero strSalida(ficheroSalida);
        prepro.generarCodigoPrep(strSalida, aventura);
    }
    cout << MSG_RESUMEN_EXTRAEOK << (ficheroSalida == ""? "<standard>":ficheroSalida) << endl;
    cout << "--------------------------------" << endl;
    aventura.mostrarResumen(cout);
    cout << "--------------------------------" << endl;
}

void compilarAPrep()
{
    IFichero fuentestr(ficheroEntrada);

    Preprocesador prepro(&fuentestr, ficheroEntrada);
    if (ficheroSalida == "")
        prepro.preprocesar(cout);
    else
    {
        OFichero ostr(ficheroSalida);
        prepro.preprocesar(ostr);
        if (detalleSimbolos)
        {
            cout << MSG_SIMBOLOSUSADOS << endl
                 << "--------------------------------" << endl;
            prepro.mostrarSimbolos(cout);
        }
    }
}

void extraerGraficosCaracteres(const string& comando)
{
    if (ficheroEntrada.find(".") != string::npos)
    {
        string nombreUpr = UtilTexto::toUpr(ficheroEntrada);
        if (nombreUpr.find(".Z80") != string::npos)
        {
            SPBUFFER->readBufferZ80(ficheroEntrada);
        }
        else if (nombreUpr.find(".SNA") != string::npos)
        {
            SPBUFFER->readBufferSNA(ficheroEntrada);
        }
        else throw Excepcion("\""+ficheroEntrada+"\": " + MSG_TIPONOREC);
    } else throw Excepcion("\""+ficheroEntrada+"\": " + MSG_TIPONOREC);

    ifstream nullStr;

    Aventura aventura;
    if (!aventura.pawSignature())
        throw Excepcion("\""+ficheroEntrada+"\": " + MSG_NOBDPAW);

    aventura.volcarDeMemoria();
    Preprocesador prepro(&nullStr);
    if (ficheroSalida == "")
    {
        if (comando == "eg")
            prepro.generarCodigoGraficos(cout, aventura);
        else
            prepro.generarCodigoCaracteres(cout, aventura);
    }
    else
    {
        OFichero strSalida(ficheroSalida);
        if (comando == "eg")
            prepro.generarCodigoGraficos(strSalida, aventura);
        else
            prepro.generarCodigoCaracteres(strSalida, aventura);
    }
}

void compilarDePrep()
{
    IFichero fuentestr(ficheroEntrada);

    Aventura aventura;
    Tokeniser tokeniser(&fuentestr);
    try
    {
        aventura.leerDeCodigo(tokeniser);
        SpWord libre = aventura.volcarAMemoria();
        if (libre == 65535)
            cout << MSG_SINMEMORIA << endl;
        else
        {
            if (ficheroSalida == "")
                ficheroSalida = ficheroEntrada + ".tap";
            cout << MSG_MEMLIBRE << libre << endl;
            SPBUFFER->writeBufferTAP(ficheroSalida);
        }
    }
    catch (Excepcion& ex)
    {
        ostringstream ostr;
        ostr << tokeniser.lineaActual() << ": [EXCEPCION]" << ex.desc();
        throw Excepcion(ostr.str());
    }
}

void extraeAPrep()
{
    if (ficheroEntrada.find(".") != string::npos)
    {
        string nombreUpr = UtilTexto::toUpr(ficheroEntrada);
        if (nombreUpr.find(".Z80") != string::npos)
        {
            SPBUFFER->readBufferZ80(ficheroEntrada);
        }
        else if (nombreUpr.find(".SNA") != string::npos)
        {
            SPBUFFER->readBufferSNA(ficheroEntrada);
        }
        else throw Excepcion("\""+ficheroEntrada+"\": " + MSG_TIPONOREC);
    } else throw Excepcion("\""+ficheroEntrada+"\": " + MSG_TIPONOREC);

    Aventura aventura;
    if (!aventura.pawSignature())
        throw Excepcion("\""+ficheroEntrada+"\": " + MSG_NOBDPAW);
    aventura.volcarDeMemoria();
    if (ficheroSalida == "")
        aventura.generarCodigo(cout);
    else
    {
        OFichero ostr(ficheroSalida);
        aventura.generarCodigo(ostr);
    }
}

void compilarADisco(uint16_t flags)
{
    IFichero fuentestr(ficheroEntrada);

    static OFichero ficheroIntermedio(ficheroEntrada + ".prep", true);

    const Tokeniser* tokeniserPtr(NULL);

    try
    {
        Preprocesador prepro(&fuentestr, ficheroEntrada, flags);
        prepro.preprocesar(ficheroIntermedio);

        IFichero ifstr(ficheroEntrada + ".prep", true);
        Tokeniser tokeniser(&ifstr);
        tokeniserPtr = &tokeniser;
        Aventura aventura;

        aventura.leerDeCodigo(tokeniser);

        if (ficheroSalida == "")
            ficheroSalida = ficheroEntrada + (flags&VERTXP?".TXP":".SCE");

        OFichero ofstr (ficheroSalida);
        if (! (flags&VERTXP))
            aventura.generarCodigoDisco(ofstr, flags);
        else
            aventura.generarCodigoSuperglus(ofstr, flags, prepro.diccionario());

        cout << MSG_RESUMEN_COMPILOK << ficheroSalida << endl;
        cout << "--------------------------------" << endl;
        aventura.mostrarResumen(cout, false);
        cout << "--------------------------------" << endl;
        if (detalleSimbolos)
        {
            cout << MSG_SIMBOLOSUSADOS << endl
                 << "--------------------------------" << endl;
            prepro.mostrarSimbolos(cout);
        }

    }
    catch (ExcTokeniser& ex)
    {
        throw;
    }
    catch (Excepcion& ex)
    {
        ostringstream ostr;
        ostr << tokeniserPtr->lineaActual() << ": [EXCEPCION]" << ex.desc();
        throw Excepcion(ostr.str());
    }
}
