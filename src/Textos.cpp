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

#include "Textos.h"
#include "defines.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include "UtilTexto.h"
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace Inpaws;
using namespace std;

Textos::Textos(PtrBuffer ptrIndice, SpWord offNumText,
               const std::string& titulo)
 : PawPart(ptrIndice), _offNumText (offNumText), _titulo (titulo)
{
}

Textos::~Textos()
{

}

PtrBuffer
Textos::volcarDeMemoria ()
{
    getInicioDatos();                 // Fijar desde donde hay que leer

    SpByte numTextos = SPBUFFER->bytePeek(SPBUFFER->wordPeek(PTR_MAINTOP) + _offNumText);
    SpWord ultimaPos(0); // Última posicion leida

    _textos.clear();               // Limpiamos lo anterior

    for (unsigned char i = 0; i < numTextos; i++)
    {

        // DEBUG ***
        /*    PtrBuffer ptr = SPBUFFER->wordPeek(ptrIndice + 2*i);
            cout << (unsigned int)i << ": ";
            while (SPBUFFER->charPeek(ptr) != 31)
                cout << (int)(unsigned char)SPBUFFER->charPeek(ptr++) << " ";
            cout << endl;*/
        // <- DEBUG

        string texto;
        ultimaPos =
          _leerTextoMemoria (texto,
                             SPBUFFER->wordPeek(ptrDatos() + 2*i));

        anadirTexto (i, texto);



    }
    return ultimaPos + numTextos*2; // Ultima posicion detras de los indices
}

PtrBuffer
Textos::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    // El formato de los mesajes es, primero volcamos todos los mensajes
    // a partir de ptrInicioDatos. A continuación van todos los indices
    // La direccion del primer indice va a _ptrIndice
    vector<SpWord> punterosAMensajes;
    PtrBuffer ptrTextos = ptrInicioDatos;

    // Escribimos el numero de mensajes en PTR_MAINTOP+desplazamiento
    SPBUFFER->bytePoke(SPBUFFER->wordPeek(PTR_MAINTOP) + _offNumText,
                       _textos.size());

    // Y tambien en cierta direccion de memoria, para que funcione
    // el editor (comprobación empírica)

    switch (ptrIndice())
    {
        case PTR_MENSAJES:
            SPBUFFER->bytePoke(65528, _textos.size());
            break;
        case PTR_SYSMENSAJES:
            SPBUFFER->bytePoke(65529, _textos.size());
            break;
        case PTR_OBJETOS:
            SPBUFFER->bytePoke(65530, _textos.size());
            break;
        case PTR_LOCALIDADES:
            SPBUFFER->bytePoke(65531, _textos.size());
            break;
    }

    // Primero escribimos los mensajes
    for (TTextos::const_iterator it = _textos.begin();
         it != _textos.end();
         it++)
    {
        // Primero el puntero al mensaje en el indice
        punterosAMensajes.push_back(ptrTextos);
        // Recorremos el mensaje y empezamos a escribir
        for (string::const_iterator itTexto = (*it).second.begin();
             itTexto != (*it).second.end();
             itTexto++)
        {
            if (*itTexto == '^')
            {
                SPBUFFER->charPoke(ptrTextos, 7);
            }
            else if (*itTexto == '{')
            {
                itTexto++;
                if (itTexto == (*it).second.end())
                    break;

                SPBUFFER->charPoke(ptrTextos, _valueOf (itTexto));
            }
            else if (*itTexto == '\\')
            {
                itTexto++;
                if (itTexto == (*it).second.end())
                    break;
                if (*itTexto == '{')
                    SPBUFFER->charPoke(ptrTextos, 0x7B); // literal de llave abierta en PAW
                else if (*itTexto == '^')
                    SPBUFFER->charPoke(ptrTextos, '^'); // literal de ^ en PAW
                else if (*itTexto == '\\')
                    SPBUFFER->charPoke(ptrTextos, '\\'); // literal de barra invertida en PAW
                else throw Excepcion("\""+ (*it).second + "\": "+MSG_CADENA_MAL_FORMADA);

            }
            else
            {
                SPBUFFER->charPoke(ptrTextos, *itTexto);
            }
            ptrTextos++;
        }
        SPBUFFER->charPoke(ptrTextos++, 31);
    }

    // Y a continuación los indices

    // Primero almacenamos el inicio de los indices en ptrIndice
    SPBUFFER->wordPoke (_ptrIndice, ptrTextos);
    // Y luego la lista de indices uno tras otro
    for (vector<SpWord>::const_iterator itIndices = punterosAMensajes.begin();
         itIndices != punterosAMensajes.end();
         itIndices++)
    {
        SPBUFFER->wordPoke(ptrTextos, *itIndices);
        ptrTextos += 2;
    }

    return ptrTextos-1;
}


void
Textos::anadirTexto (SpByte codigo,const std::string& texto)
{
    if (_textos.find(codigo) != _textos.end())
        _textos.erase(codigo);

    _textos.insert (pair<SpByte, std::string>(codigo, texto));
}

const std::string&
Textos::obtenerTexto (SpByte codigo) const
{
    TTextos::const_iterator it = _textos.find (codigo);
    if (it != _textos.end())
        return (*it).second;
    else
        throw (Excepcion ("Textos: " + string(MSG_FUERA_DE_LIMITE)));
}

SpWord
Textos::_leerTextoMemoria (std::string& texto, SpWord posicion) const
{
    // Sigue la estructura de unpaws
    int colorCodes = 0;
    SpByte c = SPBUFFER->charPeek(posicion);

    // El valor de retorno es la última posición leida
    texto.clear();
    while (c != 31)
    {
        if (colorCodes)
        {
            texto.append ("{");
            texto.append (_numeroAString(c));
            texto.append ("}");
            colorCodes--;
        }
        else
        {
            if (c > 15 && c < 22) colorCodes = 1;
                else if (c == 22 || c== 23) colorCodes = 2;
                else colorCodes = 0;

            if (c == 0X7B)
            {
                texto.append("\\{"); // Si hay realmente un caracter '{' en el mensaje
            }
            else if (c == 0x5E) // El caracter ^ en PAW
            {
                texto.append("\\^");
            }
            else if (c == '\\') // El caracter de la barra invertida en PAW (que por cierto es la Ñ en el español)
            {
                texto.append("\\\\");
            }
            else if (c==22 || c== 23)
            {
                texto.append(1, 32);

            }
            else if (c > 31 && c != '^' && c < 127 && c != 96)
            {
                texto.append(1, c);
            }
            else if (c > 164) // Abreviaturas
            {
                texto.append ("{");
                texto.append (_numeroAString(c));
                texto.append ("}");
            }
            else if (c == 7)
            {
                texto.append(1, '^');
            }
            else
            {
                texto.append ("{");
                texto.append (_numeroAString(c));
                texto.append ("}");

            }
        }
        posicion++;
        c = SPBUFFER->charPeek (posicion);
    }

    return posicion;
}

void
Textos::generarCodigo (std::ostream& os)
{
    os << _titulo << endl << "{" << endl;
    for (SpByte i=0; i < _textos.size(); i++)
    {
        os << '\t' << (unsigned int) i << ": \"";

        _generarTexto(os, _textos[i]);

        os << "\";" << endl;
    }
    os << "}" << endl;
}

void
Textos::generarCodigoDisco (std::ostream& os, uint16_t flags)
{
    UtilTexto utilTexto;

    if (_titulo == RSV_SYSMESSAGES)
        os << "/STX";
    else if (_titulo == RSV_MESSAGES)
        os << "/MTX";
    else if (_titulo == RSV_LOCATIONS)
        os << "/LTX";
    else if (_titulo == RSV_OBJECTS)
        os << "/OTX";

    os << "\t; " << _titulo << endl;

    for (SpByte i=0; i < _textos.size(); i++)
    {
        os << "/" << (unsigned int) i << endl;
        if (flags & VERGLULX)
            os << utilTexto.filtrarParaSuperglus(_textos[i]) << endl;
        else
            os << utilTexto.filtrarParaSCE(_textos[i]) << endl;
    }
}

void
Textos::generarCodigoSuperglus (std::ostream& os, uint16_t flags,
                             Diccionario::TDiccionarioInverso diccionario)
{
    UtilTexto utilTexto;
    int tipo = -1;

    if (_titulo == RSV_SYSMESSAGES)
    {
        os << "/STX";
        tipo = SYSNO;
    }
    else if (_titulo == RSV_MESSAGES)
    {
        os << "/MTX";
        tipo = MESNO;
    }
    else if (_titulo == RSV_LOCATIONS)
    {
        os << "/LTX";
        tipo = LOCNO;
    }
    else if (_titulo == RSV_OBJECTS)
    {
        os << "/OTX";
        tipo = OBJNO;
    }

    os << endl;

    for (SpByte i=0; i < _textos.size(); i++)
    {
        os << "/" << (unsigned int) i;

        pair<int, int> indice((unsigned int) i, tipo);
        Diccionario::TDiccionarioInverso::const_iterator it = diccionario.find(indice);
        if (it != diccionario.end() && (*it).second[0] != '\"')
        {
            os << " = " << (*it).second;
        }
        os << endl;

        if (it == diccionario.end() || (*it).second[0] != '\"')
        {
            os << utilTexto.filtrarParaSuperglus(_textos[i]) << endl;
        }
        else
        {
            os << "; Este mensaje se muestra con WRITE, está definido por compatibilidad con Inpaws" << endl
               << "; y puedes reutilizarlo para tus propios fines" << endl
               << ";" << (*it).second << endl;
        }
    }
}

void
Textos::leerDeCodigo (Tokeniser& tokeniser)
{
    // Al tratarse de un código ya preprocesado y supuestamente sin
    // errores, todo lo que no esté bien generará excepción

    Tokeniser::TTokenType token;

    // Primero aparece el literal LOCATIONS/MESSAGES/SYSMESSAGES/OBJECTS y el "{"
    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, _titulo);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");

    // Ahora leemos la lista de textos
    // en formato: Nº: "Cadena de texto" (ej: 131: "Te encuentras en un oscuro paraje.")
    SpByte numTexto;
    string textoLeido;
    token = tokeniser.leeToken();
    while (!(token == Tokeniser::TK_CONTROL &&
             tokeniser.lvalue().literal == "}"))
    {
        // Codigo

        if (token == Tokeniser::TK_NUMERO)
            numTexto = tokeniser.lvalue().valor;
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NUMERO_TEXTO_ESPERADO));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ":");

        // Literal de texto
        token = tokeniser.leeToken();
        if (token == Tokeniser::TK_LITERAL)
            textoLeido = tokeniser.lvalue().literal;
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_LITERAL_ENTRECOMILLADO));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ";");

        // Añadimos el texto
        anadirTexto (numTexto, textoLeido);

        token = tokeniser.leeToken();
    }
}

std::string
Textos::_numeroAString(SpByte spbyte) const
{
    ostringstream osstr;

    osstr << (unsigned int)spbyte;

    return osstr.str();
}

SpByte
Textos::_valueOf (string::const_iterator& it) const
{
    string tmp;

    // Lo primero es un {, lo saltamos

    while (*it != '}')
    {
        if (*it >= '0' && *it <= '9' && tmp.size() < 3)
            tmp.append(1,*it);
        else
            throw Excepcion(tmp+":"+MSG_CODIGO_NO_NUMERICO);
        it++;
    }
    return atoi(tmp.c_str());
}

void
Textos::_generarTexto (std::ostream& os, const string& texto) const
{
    Tokeniser::imprimirLiteral(texto, os);
}
