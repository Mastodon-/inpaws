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

#include "prep/Diccionario.h"
#include "UtilTexto.h"
#include "defines.h"
#include "MsgAplicacion.h"

using namespace Inpaws;
using namespace std;

Diccionario::Diccionario()
{
}

Diccionario::~Diccionario()
{

}

void
Diccionario::anadirEntrada (const std::string& nombre, int tipo, int valor)
{
    Entrada nuevaEntradaUpr;
    if (nombre[0] != '\"')
        nuevaEntradaUpr.nombre = UtilTexto::toUpr(nombre);
    else
        nuevaEntradaUpr.nombre = nombre;

    nuevaEntradaUpr.tipo = tipo;

    if (_diccionario.find(nuevaEntradaUpr) == _diccionario.end())
    {
        _diccionario.insert(pair<Entrada, int>(nuevaEntradaUpr, valor));
    }
}

int
Diccionario::buscarEntrada(const std::string& nombre, int tipo) const
{
    Entrada entradaBuscarUpr;
    if (nombre[0] != '\"') // No es un literal de mensaje
        entradaBuscarUpr.nombre = UtilTexto::toUpr(nombre);
    else
        entradaBuscarUpr.nombre = nombre;
    entradaBuscarUpr.tipo = tipo;

    TDiccionario::const_iterator it = _diccionario.find(entradaBuscarUpr);
    if (it == _diccionario.end())
    {
        return -2;
    }
    else
        return (*it).second;
}

void
Diccionario::fijarValor (const std::string& nombre, int tipo, int valor)
{
    Entrada entradaBuscarUpr;
    if (nombre[0] != '\"') // No es un literal de mensaje
        entradaBuscarUpr.nombre = UtilTexto::toUpr(nombre);
    else
        entradaBuscarUpr.nombre = nombre;

    entradaBuscarUpr.tipo = tipo;

    TDiccionario::iterator it = _diccionario.find(entradaBuscarUpr);
    if (it != _diccionario.end())
    {
        (*it).second = valor;
    }
}

void
Diccionario::mostrarSimbolos(std::ostream& os) const
{
    for (TDiccionario::const_iterator it = _diccionario.begin();
         it != _diccionario.end();
         it++)
    {
        if (((*it).first.nombre[0] < '0' || (*it).first.nombre[0] > '9') &&
            (*it).first.nombre[0] != '\"' &&
            ((*it).first.tipo == LOCNO || (*it).first.tipo == SYSNO ||
            (*it).first.tipo == MESNO || (*it).first.tipo == OBJNO ||
            (*it).first.tipo == FLAGNO))
        {
            os << (*it).first.nombre << " [";
            switch ((*it).first.tipo)
            {
                case LOCNO: os << RSV_LOCATION; break;
                case SYSNO: os << RSV_SYSMESSAGES; break;
                case MESNO: os << RSV_MESSAGES; break;
                case OBJNO: os << RSV_OBJECT; break;
                case FLAGNO: os << RSV_FLAG; break;
            }
            os << "] = " << (*it).second << endl;
        }
    }
}

Diccionario::TDiccionarioInverso
Diccionario::crearDiccionarioInverso() const
{
    // Devuelve una estructura (por copia) de la relación (código+tipo) a nombre.
    // Sólo los símbolos con nombre, no los valores
    TDiccionarioInverso inverso;
    pair<int, int> indice;
    for (TDiccionario::const_iterator it = _diccionario.begin();
         it != _diccionario.end();
         it++)
    {
        if (!((*it).first.nombre[0] >= '0' && (*it).first.nombre[0] <= '9'))
        {
            indice.first = (*it).second; // Código
            indice.second = (*it).first.tipo; // Tipo: LOCNO, MESNO, etc
            string prefNombre;

            // Añadimos un prefijo a los id para que no haya conflictos en superglus
            switch ((*it).first.tipo)
            {
                case LOCNO:
                    prefNombre = "loc_";
                    break;
                case SYSNO:
                    if ((*it).first.nombre[0] != '\"') prefNombre = "sys_";
                    break;
                case MESNO:
                    if ((*it).first.nombre[0] != '\"') prefNombre = "mes_";
                    break;
                case OBJNO:
                    prefNombre = "obj_";
                    break;
                case FLAGNO:
                    prefNombre = "flag_";
                    break;
                case PROCNO:
                    prefNombre = "proc_";
                    break;
                case VALOR:
                    prefNombre = "const_";
                    break;
            }
            prefNombre.append((*it).first.nombre);
            TDiccionarioInverso::value_type entradaDictInverso (indice, prefNombre);
            inverso.insert(entradaDictInverso);
        }
    }
    return inverso;
}
