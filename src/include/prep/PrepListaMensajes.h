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

#ifndef PREPLISTAMENSAJES_H_
#define PREPLISTAMENSAJES_H_

#include "prep/PrepPart.h"
#include "prep/PrepAbreviable.h"
#include "PrepSubChar.h"

namespace Inpaws
{
    class PrepListaMensajes: public PrepPart, public PrepAbreviable
    {
        public:
            struct Mensaje
            {
                std::string nombre;
                std::string desc;
            };
            PrepListaMensajes(const std::string& nombre, const PrepSubChar& prepSubChar);
            virtual ~PrepListaMensajes();

            size_t size() const { return _mensajes.size(); }

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;

            // Añadir mensajes desde fuera de la clase (usado por los procesos)
            void anadirMensaje(const std::string& nombre, const std::string& desc, Diccionario& diccionario);

            // sustituir caracteres especiales (acentuados, eñes..)
            void sustituirCaracteresEspeciales();

            // Generar codigo expandiendo abreviaturas
            void generarCodigoPrep (std::ostream& os, const Aventura& aventura,
                                    const PrepAbreviable::TDiccionario& abreviaturas) const;

            // Metodos heredados de PrepAbreviable
            virtual PrepAbreviable::TListaCadenas leerCadenas() const;
            virtual SpWord sustituirCadenas (const PrepAbreviable::TDiccionario& diccionario);

        private:
            std::vector<Mensaje> _mensajes;
            const PrepSubChar& _prepSubChar;
    };
}

#endif
