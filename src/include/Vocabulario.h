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

#ifndef VOCABULARIO_H
#define VOCABULARIO_H

#include "PawPart.h"
#include "SpBuffer.h"
#include "Tokeniser.h"
#include "defines.h"

#include <map>

namespace Inpaws
{
    class Vocabulario: public PawPart
    {
        public:
            static const size_t superglus_word_size;
            static const size_t paw_word_size;
            struct Palabra
            {
                SpByte codigo;
                char palabraCorta[6];              // Palabra corta formato ZX
                std::string palabraCompleta;        // Misma palabra larga para Superglulx
                SpByte tipo;
                std::string palabra(uint16_t flags) const
                {
                    if (flags&VERGLULX)
                        return palabraCompleta.substr(0, superglus_word_size);
                    else
                        return quitaEspacios(palabraCorta);
                };
            };
            enum {VERB,ADVERB,NOUN,ADJECTIVE,PREPOSITION,
                  CONJUNCTION,PRONOUN, RESERVED=255};

            Vocabulario(PtrBuffer ptrIndice);
            virtual ~Vocabulario();

            // Metodos heredados de PawPart
            virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
            virtual PtrBuffer volcarDeMemoria ();
            virtual void generarCodigo (std::ostream& os);
            virtual void generarCodigoDisco (std::ostream& os, uint16_t flags);
            virtual void leerDeCodigo (Tokeniser& tokeniser);

            // Funciones para obtener y añadir palabras
            void anadirPalabra (SpByte codigo,const std::string& palabra, SpByte tipo);
            // Lee el primer sinónimo y los siguientes. Devuelve codigo cero en caso
            // de no existir o fin de sinonimos
            const Palabra* leerPalabraInit (SpByte codigo, SpByte tipo) const;
            const Palabra* leerPalabraNext (SpByte codigo, SpByte tipo) const;
            // Lo mismo para todo el vocabulario
            const Palabra* leerPalabraInit () const;
            const Palabra* leerPalabraNext () const;
            size_t size() const { return _vocabulario.size(); }

            // Mostrar el literal del tipo de palabra
            static std::string tipoAStr(SpByte tipo);

            static const std::string quitaEspacios(const char* palabra);
            static const std::string llenaespacios5(const std::string& palabra);
            static void transformarEnyeEnBarra(char* palabra); // Sustituir las Ñ por las barras invertidas
            static void transformarBarraEnEnye(char* palabra); // Sustituir las barras por las Ñ

        protected:
            typedef std::multimap<SpByte,Palabra> TVocabulario;
            TVocabulario _vocabulario;
        private:
            static TVocabulario::const_iterator _itVoc, _itVocEnd; // Puntero para recorrerlo

    };
}

#endif // VOCABULARIO_H
