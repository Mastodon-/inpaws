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

// Diccionario de datos o tabla de simbolos del preprocesador

#ifndef _DICCIONARIO_H_
#define _DICCIONARIO_H_

#include <string>
#include <map>


namespace Inpaws
{
    class Diccionario
    {
        public:

            struct Entrada // Una entrada del diccionario de datos
            {
                std::string nombre;     // Nombre de la entrada
                int tipo;               // Tipo de la entrada (LOCNO, FLAGNO, etc...)
                bool operator<(const Entrada& otro) const
                {
                    if (nombre < otro.nombre ||
                        (nombre == otro.nombre && tipo < otro.tipo))
                        return true;
                    else
                        return false;
                }
                //int valor;              // Valor a sustituir cuando se encuentre el nombre, -1 si no definido
            };
            typedef std::map<std::pair<int, int>, const std::string > TDiccionarioInverso;
            typedef std::map<Entrada, int> TDiccionario;

            Diccionario();
            ~Diccionario();

            // Insercion, busqueda y modificacion

            void anadirEntrada (const std::string& nombre, int tipo, int valor = -1);

            // devuelve el valor asignado al símbolo, o bien
            //    -1 si existe el símbolo pero no tiene valor asignado
            //    -2 si no existe el símbolo
            int buscarEntrada(const std::string& nombre, int tipo) const;

            void fijarValor (const std::string& nombre, int tipo, int valor);
            void mostrarSimbolos(std::ostream& os) const; // Lista las localidades, objetos, mensajes y flags
            TDiccionarioInverso crearDiccionarioInverso() const;

        private:

            TDiccionario _diccionario;
    };
}

#endif
