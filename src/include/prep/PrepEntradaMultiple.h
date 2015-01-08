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

//------------------------------------------------------------------------
//
// Tratamiento de entradas en las tablas de procesos con elementos OR
// Ejemplo: COGE HACHA|COGE CUCHILLO: AT Sitio0|AT Sitio1 ... etc
// será transformado en tantas entradas al volcar a PAW
// como sea necesario para cubir la funcionalidad
//---------------------------------------------------------------------


#ifndef PREPENTRADAMULTIPLE_H_
#define PREPENTRADAMULTIPLE_H_

#include <vector>
#include <string>

namespace Inpaws
{
    class PrepEntradaMultiple
    {
        public:
            struct Opcion
            {
                std::string literal;
                unsigned long linea;
                std::string archivo;
            };
            struct Condacto
            {
                std::string nombre;
                std::string param1;
                std::string param2;

                unsigned long lineaParam1, lineaParam2;
                std::string archivoParam1, archivoParam2;
            };
            struct EntradaProceso
            {
                std::string verbo;
                std::string nombre;
                std::vector<Condacto> listaCondactos;

                unsigned long lineaVerbo, lineaNombre;
                std::string archivoVerbo, archivoNombre;
            };

            struct CondactoMultiple: public std::vector<Condacto>
            {
                const_iterator itCond;
            };


            PrepEntradaMultiple();
            //~PrepEntradaMultiple();
            void anadirVerboNombre (const std::string& verbo, unsigned long lineaverbo, const std::string& archivoverbo,
                                    const std::string& nombre, unsigned long lineanombre, const std::string& archivonombre);
            void anadirCondacto(const Condacto& condacto, bool nuevo = true);

            // Funciones para leer las entradas generadas una vez finalizado el
            // añadido de opciones
            const EntradaProceso* leerEntradaInit();
            const EntradaProceso* leerEntradaNext();

        private:
            typedef std::vector<std::pair<Opcion, Opcion> > TEntradaMultiple;
            TEntradaMultiple _verboNombre;

            std::vector<CondactoMultiple> _condactos;

            void _avanzarPuntero();
            TEntradaMultiple::const_iterator _itVerboNombre;

            std::vector<EntradaProceso> _entradas;
    };
}

#endif
