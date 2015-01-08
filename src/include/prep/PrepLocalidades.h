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

#ifndef _PREPLOCALIDADES_H_
#define _PREPLOCALIDADES_H_

#include "PrepPart.h"
#include "PrepSubChar.h"
#include "PrepAbreviable.h"
#include <vector>

namespace Inpaws
{
    class PrepLocalidades: public PrepPart, public PrepAbreviable
    {
        public:
            struct Conexion
            {
                unsigned long linea;
                std::string archivolinea;
                std::string direccion;
                std::string destino;
            };
            struct Localidad
            {
                unsigned long linea;
                std::string nombre;
                std::string desc;
                std::vector<Conexion> conexiones;
            };
            PrepLocalidades(const PrepSubChar& prepSubChar);
            virtual ~PrepLocalidades();

            // sustituir caracteres especiales (acentuados, eñes..)
            void sustituirCaracteresEspeciales();

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;

            // Generar codigo expandiendo abreviaturas, devuelve bytes ahorrados
            void generarCodigoPrep (std::ostream& os, const Aventura& aventura,
                                    const PrepAbreviable::TDiccionario& abreviaturas) const;

            // Metodos heredados de PrepAbreviable
            virtual PrepAbreviable::TListaCadenas leerCadenas() const;
            virtual SpWord sustituirCadenas (const PrepAbreviable::TDiccionario& diccionario);

        private:
            std::vector<Localidad> _localidades;
            const PrepSubChar& _prepSubChar;
            void _leerConexiones (PrepTokeniser& tokeniser, Localidad& localidad);
    };
}

#endif
