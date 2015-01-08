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

#include "prep/PrepEntradaMultiple.h"
#include "prep/PrepProcesos.h"

using namespace Inpaws;
using namespace std;

PrepEntradaMultiple::PrepEntradaMultiple()
{
}


void
PrepEntradaMultiple::anadirVerboNombre (const std::string& verbo, unsigned long lineaverbo, const std::string& archivoverbo,
                        const std::string& nombre, unsigned long lineanombre, const std::string& archivonombre)
{
    Opcion nuevoVerbo, nuevoNombre;
    nuevoVerbo.literal = verbo;
    nuevoVerbo.linea = lineaverbo;
    nuevoVerbo.archivo = archivoverbo;

    nuevoNombre.literal = nombre;
    nuevoNombre.linea = lineanombre;
    nuevoNombre.archivo = archivonombre;

    _verboNombre.push_back(pair<Opcion, Opcion>(nuevoVerbo, nuevoNombre));
}
void
PrepEntradaMultiple::anadirCondacto(const Condacto& condacto, bool nuevo)
{
    if (nuevo)
    {
        CondactoMultiple nuevoCondactoMultiple;
        _condactos.push_back(nuevoCondactoMultiple);
    }

    _condactos.back().push_back(condacto);
}


const PrepEntradaMultiple::EntradaProceso*
PrepEntradaMultiple::leerEntradaInit()
{
    _entradas.clear();
    _itVerboNombre = _verboNombre.begin();
    for (std::vector<CondactoMultiple>::iterator it = _condactos.begin();
         it != _condactos.end();
         it++)
    {
        (*it).itCond = (*it).begin();
    }

    return leerEntradaNext();
}

const PrepEntradaMultiple::EntradaProceso*
PrepEntradaMultiple::leerEntradaNext()
{
    if (_itVerboNombre == _verboNombre.end())
        return NULL;

    EntradaProceso nuevaEntrada;
    nuevaEntrada.verbo = (*_itVerboNombre).first.literal;
    nuevaEntrada.lineaVerbo = (*_itVerboNombre).first.linea;
    nuevaEntrada.archivoVerbo = (*_itVerboNombre).first.archivo;
    nuevaEntrada.nombre = (*_itVerboNombre).second.literal;
    nuevaEntrada.lineaNombre = (*_itVerboNombre).second.linea;
    nuevaEntrada.archivoNombre = (*_itVerboNombre).second.archivo;
    for (std::vector<CondactoMultiple>::const_iterator it = _condactos.begin();
         it != _condactos.end();
         it++)
    {
        Condacto nuevoCondacto = *((*it).itCond);
        nuevaEntrada.listaCondactos.push_back(nuevoCondacto);
    }
    _avanzarPuntero();
    _entradas.push_back(nuevaEntrada);
    return &(*_entradas.rbegin());
}

void
PrepEntradaMultiple::_avanzarPuntero()
{
    // Empezamos por los condactos desde el final y vamos retrocediendo
    // hasta finalizar en el nombre-verbo
    // retorna cuando al avanzar un iterador, el resultado no es el
    // final de la estructura (end() )

    for (std::vector<CondactoMultiple>::reverse_iterator it = _condactos.rbegin();
         it != _condactos.rend();
         it++)
    {
        (*it).itCond++;

        if ((*it).itCond == (*it).end())
        {
            (*it).itCond = (*it).begin();
        }
        else
        {
            return;
        }

    }

    _itVerboNombre++;
    // _itVerboNombre será end cuando finalice el proceso

}
