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

#include "UtilTexto.h"
#include "prep/MsgPrep.h"
#include "MsgAplicacion.h"
#include "Excepcion.h"
#include <sstream>
#include <cstdlib>

using namespace Inpaws;
using namespace std;

UtilTexto::UtilTexto()
 : _posactual(0), _colorCodes(0)
{
}

UtilTexto::~UtilTexto()
{
}

bool
UtilTexto::textoCorrecto (const string& texto)
{
    resetearLectura();
    _errorStr = "";

    Item item = siguienteItem(texto);
    while (item.tipo != ERROR && item.tipo != FIN)
    {
        item = siguienteItem(texto);
        if (item.tipo == ERROR)
        {
            ostringstream ostr;
            ostr << MSGPREP_LITERALINCORRECTO << " (pos. " << _posactual << ")";
            _errorStr = ostr.str();
        }
    }

    if (item.tipo == ERROR)
        return false;
    else
        return true;
}

int
UtilTexto::expandirAbreviaturas (string& texto, const PrepAbreviable::TDiccionario& abreviaturas)
{
    if (abreviaturas.empty())
    {
        return 0;
    }
    int cuenta = 0;
    resetearLectura();

    ostringstream textoFinal;

    Item item = siguienteItem(texto);
    while (item.tipo != FIN)
    {
        if (item.tipo == CARACTER)
        {
            if (item.caracter == '^' || item.caracter == '\\' || item.caracter == '{')
                textoFinal << '\\';
            textoFinal << item.caracter;
        }
        else if (item.tipo == CODIGO)
        {
            if (item.codigo == 7 && !item.colorCode) // Retorno de carro
            {
                textoFinal << '^';
            }
            else if (abreviaturas.find(item.codigo) != abreviaturas.end())
            {
                textoFinal << _expandirAbreviatura(item.codigo, abreviaturas);
                cuenta++;
            }
            else
            {
                textoFinal << "{" << item.codigo << "}";
            }
        }
        else if (item.tipo == ERROR)
        {
            textoFinal << "ERROR"; // Esto no debería ocurrir
        }

        item = siguienteItem(texto);
    }
    texto = textoFinal.str();

    return cuenta;
}

int
UtilTexto::sustituirCaracteres (string& texto, const map<char, string>& diccionario)
{
    if (diccionario.empty())
    {
        return 0;
    }
    int cuenta = 0;
    resetearLectura();

    ostringstream textoFinal;

    Item item = siguienteItem(texto);
    while (item.tipo != FIN)
    {
        if (item.tipo == CARACTER && diccionario.count(item.caracter))
        {
            textoFinal << (*diccionario.find(item.caracter)).second;
            cuenta++;
        }
        else if (item.tipo == CODIGO)
        {
            if (item.codigo == 7 && !item.colorCode)
                textoFinal << '^';
            else
                textoFinal << "{" << item.codigo << "}";
        }
        else if (item.tipo == ERROR)
        {
            textoFinal << "ERROR"; // Esto no debería ocurrir
        }
        else  // CARACTER
        {
            if (item.caracter == '^' || item.caracter == '\\' || item.caracter == '{')
                textoFinal << '\\';
            textoFinal << item.caracter;
        }

        item = siguienteItem(texto);
    }
    texto = textoFinal.str();

    return cuenta;
}

UtilTexto::Item
UtilTexto::siguienteItem(const string& texto)
{
    Item itemRet;

    if (_posactual >= texto.size())
    {
        itemRet.tipo = FIN;
        itemRet.codigo = 0;
    }
    else
    {
        if (_colorCodes)
        {
            if (_posactual + 1 < texto.size())
            {
                _posactual++;
                _leerCodigo ( itemRet, texto);
                itemRet.colorCode = true;
                _colorCodes--;
            }
            else // El texto no puede terminar en un solo '{'
            {
                itemRet.tipo = ERROR;
                itemRet.codigo = 0;
            }
        }
        else
        {
            itemRet.colorCode = false;

            switch (texto[_posactual])
            {
                case '{':
                    if (_posactual + 1 < texto.size())
                    {
                        _posactual++;
                        _leerCodigo ( itemRet, texto);
                        if (_colorCodes == 0) // Aqui siempre debe ser cero, pero por si acaso...
                        {
                            if (itemRet.codigo > 15 && itemRet.codigo < 22) _colorCodes = 1;
                            else if (itemRet.codigo == 22 || itemRet.codigo== 23) _colorCodes = 2;
                            else _colorCodes = 0;
                        }
                    }
                    else // El texto no puede terminar en un solo '{'
                    {
                        itemRet.tipo = ERROR;
                        itemRet.codigo = 0;
                    }
                    break;
                case '\\':
                    if (_posactual + 1 < texto.size())
                    {
                        _posactual++;
                        if (texto[_posactual] == '\\' || texto[_posactual] == '{' || texto[_posactual] == '^')
                        {
                            itemRet.tipo = CARACTER;
                            itemRet.caracter = texto[_posactual];
                        }
                        else // Tiene que ir uno de los tres anteriores tras la barra
                        {
                            itemRet.tipo = ERROR;
                            itemRet.codigo = 0;
                        }
                    }
                    else // El texto no puede terminar en un solo '\'
                    {
                        itemRet.tipo = ERROR;
                        itemRet.codigo = 0;
                    }
                    break;
                case '^':
                    itemRet.tipo = CODIGO;
                    itemRet.codigo = 7;
                    break;
                default:
                    itemRet.tipo = CARACTER;
                    itemRet.caracter = texto[_posactual];
                    break;
            }
        }
    }

    _posactual++;
    return itemRet;
}

string
UtilTexto::toUpr(const string& texto)
{
    string textoRet(texto);
    for (size_t i = 0; i < textoRet.length(); i++)
    {
        if (textoRet[i] == 'ñ')
            textoRet[i] = 'Ñ';
        else
            textoRet[i] = toupper(textoRet[i]);
    }
    return textoRet;
}

string
UtilTexto::compactarTexto (const string& texto)
{
    string retString;

    resetearLectura();
    Item item = siguienteItem(texto);
    while (item.tipo != FIN)
    {
        if (item.tipo == CODIGO)
        {
            retString.append(1, (unsigned char)item.codigo);
        }
        else  // CARACTER
        {
            retString.append(1, item.caracter);
        }

        item = siguienteItem(texto);
    }

    return retString;
}

string
UtilTexto::expandirTexto (const string& texto, bool codificarComillas)
{
    string cadenaRet;
    int colorCodes = 0;
    for (size_t i = 0; i < texto.length(); i++)
    {
        if (colorCodes)
        {
            ostringstream ostr;
            ostr << "{" << (int)(unsigned char)texto[i] << "}";
            cadenaRet += ostr.str();
            colorCodes--;
        }
        else
        {
            if ((int)(unsigned char)texto[i] > 15 && (int)(unsigned char)texto[i] < 22) colorCodes = 1;
            else if ((int)(unsigned char)texto[i] ==22 && (int)(unsigned char)texto[i] == 23) colorCodes = 2;
            else colorCodes = 0;

            switch (texto[i])
            {
                case 7:
                    cadenaRet.append(1, '^');
                    break;
                case '{':
                case '\\':
                case '^':
                    cadenaRet.append(1, '\\');
                    cadenaRet.append(1, texto[i]);
                    break;
                case '\"':
                    if (codificarComillas)
                    {
                        ostringstream ostr;
                        ostr << "{" << (int)(unsigned char)texto[i] << "}";
                        cadenaRet += ostr.str();
                    } else cadenaRet += "\"";
                    break;
                default:
                    if (isprint(texto[i]) && (int)(unsigned char)texto[i] < 128)
                    {
                        cadenaRet.append(1, texto[i]);
                    }
                    else
                    {
                        ostringstream ostr;
                        ostr << "{" << (int)(unsigned char)(texto[i]) << "}";
                        cadenaRet += ostr.str();
                    }
                    break;
            }
        }
    }

    return cadenaRet;
}

string
UtilTexto::filtrarParaSCE(const string& texto)
{
    string textoCompacto = compactarTexto(texto);
    string cadenaRet = "";
    int tamlineaactual(0);
    for (size_t i = 0; i < textoCompacto.size(); i++)
    {
        if (textoCompacto[i] == 0x07) // Retorno de carro
        {
            if (!(i>0 && textoCompacto[i-1]>15 && textoCompacto[i-1]<22) &&
                !(i>1 && textoCompacto[i-2] >=22 && textoCompacto[i-2]<=23))
            {
                    if (i>0 && textoCompacto[i-1]!=0x07)
                        cadenaRet += "\n\n";
                    else
                        cadenaRet += "\n";

                    tamlineaactual = 0;
            }
        }

        else if ((unsigned int)textoCompacto[i] >= ' ' /*&& (unsigned int)textoCompacto[i] <= 0x80*/)
        {
            if (textoCompacto[i] == ' ')
            {
                if (tamlineaactual > 70) // Intentamos ajustar las lineas a 80 caracteres
                {
                    cadenaRet += '\n';
                    tamlineaactual = -1;
                }
                else
                    cadenaRet += textoCompacto[i];
            }
            else cadenaRet += textoCompacto[i];

            tamlineaactual++;

         }
    }

    //Un pequeño ajuste necesario
    if (*cadenaRet.rbegin() == '\n')
        cadenaRet.resize(cadenaRet.size()-1);

    return cadenaRet;
}

string
UtilTexto::filtrarParaSuperglus(const string& texto, bool cortarLineas)
{
    string textoCompacto = compactarTexto(texto);
    string cadenaRet = "";
    int tamlineaactual(0);
    for (size_t i = 0; i < textoCompacto.size(); i++)
    {
        if (textoCompacto[i] == 0x07) // Retorno de carro
        {
            if (!(i>0 && textoCompacto[i-1]>15 && textoCompacto[i-1]<22) &&
                !(i>1 && textoCompacto[i-2] >=22 && textoCompacto[i-2]<=23))
            {
                    cadenaRet +="\\n";
                    if (cortarLineas) cadenaRet += "\n";

                    tamlineaactual = 0;
            }
        }
        else if (textoCompacto[i] == '\"' &&           // Las comillas dobles no las escapa superglus
                 i && (i<textoCompacto.length()-1))
        {
            cadenaRet += '\''; // Las convertimos en simples
        }
        else if (textoCompacto[i] == '*' || textoCompacto[i]=='|' ||
                  textoCompacto[i] == '^' || textoCompacto[i]=='@' ||
                  textoCompacto[i] == '#')
        {
            cadenaRet += '\\';
            cadenaRet += textoCompacto[i];
        }

        else if ((unsigned int)textoCompacto[i] >= ' ')
        {
            if (textoCompacto[i] == ' ')
            {
                if (tamlineaactual > 70 && cortarLineas) // Intentamos ajustar las lineas a 80 caracteres
                {
                    cadenaRet += '\n';
                    tamlineaactual = -1;
                }
                else
                    cadenaRet += textoCompacto[i];
            }
            else cadenaRet += textoCompacto[i];

            tamlineaactual++;

         }
    }

    //Un pequeño ajuste necesario
    if (*cadenaRet.rbegin() == '\n')
        cadenaRet.resize(cadenaRet.size()-1);

    // Cambiamos los espacios al final por caracteres '¬'
    for (int i = cadenaRet.length()-1; i >= 0 && cadenaRet[i]==' '; i--)
    {
        cadenaRet[i] = '¬';
    }

    return cadenaRet;
}


void
UtilTexto::_leerCodigo (Item& item, const string& texto)
{
    // Empezamos en la posición tras el "{"
    size_t posCierre = texto.find("}", _posactual);
    if (posCierre == string::npos || posCierre == _posactual)
    {
        item.tipo = ERROR;
        item.codigo = 0;
    }
    else
    {
        string codigoStr = texto.substr(_posactual, posCierre - _posactual);
        if (codigoStr.length() > 3)
        {
            item.tipo = ERROR;
            item.codigo = 0;
        }
        else
        {
            // Comprobamos que todo sean cifras
            for (size_t i = 0; i < codigoStr.length(); i++)
            {
                if (codigoStr[i] < '0' || codigoStr[i] > '9')
                {
                    item.tipo = ERROR;
                    item.codigo = 0;
                    return;
                }
            }

            int codigo = atoi(codigoStr.c_str());
            if (codigo < 0 || codigo > 255)
            {
                item.tipo = ERROR;
                item.codigo = 0;
            }
            else
            {
                item.tipo = CODIGO;
                item.codigo = codigo;
                _posactual = posCierre;
            }
        }
    }
}

string
UtilTexto::_expandirAbreviatura(int codigo, const PrepAbreviable::TDiccionario& abreviaturas)
{
    string abrev = (*abreviaturas.find(codigo)).second;
    string abrevRet;
    for (size_t i = 0; i < abrev.size(); i++)
    {
        if (isprint(abrev[i]))
            abrevRet.append(1, abrev[i]);
        else
        {
            ostringstream ostr;
            ostr << "{" << (unsigned int)abrev[i] << "}";
            abrevRet += ostr.str();
        }
    }
    return abrevRet;
}

TextoExt::TextoExt (const std::string& texto)
{
    if (!texto.length()) return;

    UtilTexto utilTexto;
    if (!utilTexto.textoCorrecto(texto))
        throw Excepcion("[\"" + texto + "\"]:" + MSG_CADENA_MAL_FORMADA);

    utilTexto.resetearLectura();
    UtilTexto::Item item = utilTexto.siguienteItem(texto);
    while (item.tipo != UtilTexto::FIN)
    {
        switch (item.tipo)
        {
            case UtilTexto::CARACTER:
                _texto.push_back(string(1, item.caracter));
                break;
            case UtilTexto::CODIGO:
            {
                ostringstream ostr;
                ostr << "{" << item.codigo << "}";
                _texto.push_back(ostr.str());
                break;
            }
            default:
                throw Excepcion("TextoExt: error inesperado"); // No debería ocurrir*/
                break;
        }
        item = utilTexto.siguienteItem(texto);
    }

}

string
TextoExt::substr(size_t pos, size_t n) const
{
    string retStr;
    size_t final;
    if (n == string::npos)
        final = length();
    else
        final = (pos+n > length() ? length() : pos+n);

    for (size_t posActual = pos; posActual < final; posActual++)
    {
        retStr += _texto[posActual];
    }
    return retStr;
}

