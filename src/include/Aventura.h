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

#ifndef AVENTURA_H_
#define AVENTURA_H_

#include <iostream>
#include <map>
#include <vector>
#include "PawPart.h"
#include "defines.h"
#include "prep/Diccionario.h"

namespace Inpaws
{
    class Aventura
    {
        public:
            Aventura();
            ~Aventura();

            SpWord volcarAMemoria ();
            PtrBuffer volcarDeMemoria ();

            // Funciones para escribir y leer codigo fuente
            // OJO: Fuente puro PAW sin preprocesar, no debería
            // tener errores
            void generarCodigo (std::ostream& os);
            void generarCodigoDisco(std::ostream& os, uint16_t flags); // Código .SCE para versiones CPM, PC y Superglus
            void generarCodigoSuperglus(std::ostream& os,
                 uint16_t flags,
                 const Diccionario& diccionario); // Código .TXP para Superglus
            void leerDeCodigo (Tokeniser& tokeniser);
            const PawPart* getModulo(const std::string& nombre) const;
            PawPart* getModulo(const std::string& nombre);
            bool pawSignature() const;
            std::string version() const;
            void mostrarResumen(std::ostream& os, bool mostrarCaracs = true) const;

        private:
            void _parchearDatosVolcado() const;
            void _generarCodigoDiscoOBJ(std::ostream& os, uint16_t flags);
            void _generarCodigoDiscoOBJSuperglus(std::ostream& os, uint16_t flags, const Diccionario::TDiccionarioInverso& diccionario);
            void _generarCodigoSuperglusDefines(std::ostream& os, int tipo, const Diccionario::TDiccionarioInverso& diccionario) const;
            void _ajustarLineralesDictSuperglus(Diccionario::TDiccionarioInverso& diccionario) const;
            typedef std::map<std::string, PawPart*> TModulos;
            typedef std::vector<std::string> TListaVolcado;
            TModulos _modulos;
            TListaVolcado _listaVolcado; // Guarda el orden de volcado
    };
}

#endif // AVENTURA_H_
