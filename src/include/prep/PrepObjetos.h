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

#ifndef _PREPOBJETOS_H_
#define _PREPOBJETOS_H_

#include "PrepPart.h"
#include "PrepSubChar.h"
#include "PrepVocabulario.h"
#include "MsgAplicacion.h"
#include "PrepAbreviable.h"

namespace Inpaws
{
    class PrepObjetos: public PrepPart, public PrepAbreviable
    {
        public:
            struct Objeto
            {
                Objeto() : initiallyat(RSV_NOTCREATED), weight(1),
                           wordnoun("_"), wordadj("_"), container(false), clothing(false)
                { }
                unsigned long initiallyatlinea;
                unsigned long wordnounlinea;
                unsigned long wordadjlinea;
                std::string initiallyatarchivo;
                std::string wordnounarchivo;
                std::string wordadjarchivo;

                std::string nombre;
                std::string desc;
                std::string initiallyat;
                int weight;
                std::string wordnoun;
                std::string wordadj;
                bool container, clothing; // Propiedades
            };

            PrepObjetos(const PrepSubChar& prepSubChar, const PrepVocabulario& prepVocabulario);
            virtual ~PrepObjetos();

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;

            // sustituir caracteres especiales (acentuados, eñes..)
            void sustituirCaracteresEspeciales();

            // Metodos heredados de PrepAbreviable
            virtual PrepAbreviable::TListaCadenas leerCadenas() const;
            virtual SpWord sustituirCadenas (const PrepAbreviable::TDiccionario& diccionario);

        private:
            std::vector<Objeto> _objetos;
            const PrepSubChar& _prepSubChar;
            const PrepVocabulario& _prepVocabulario;
            void _fijarPropiedad (PrepTokeniser& tokeniser, Objeto& objeto) const;
    };
}

#endif


