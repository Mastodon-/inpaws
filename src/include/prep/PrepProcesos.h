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

#ifndef PREPPROCESOS_H_
#define PREPPROCESOS_H_

#include "prep/PrepPart.h"
#include "prep/PrepListaMensajes.h"
#include "prep/PrepVocabulario.h"
#include "prep/PrepEntradaMultiple.h"
#include "Condacto.h"
#include <vector>
#include <map>

namespace Inpaws
{
    class PrepProcesos: public PrepPart
    {
        public:

            typedef std::vector<PrepEntradaMultiple::EntradaProceso> Proceso;

            PrepProcesos(PrepListaMensajes& listaMensajes,
                         PrepListaMensajes& listaSisMensajes,
                         PrepVocabulario& vocabulario);
            virtual ~PrepProcesos();

            // Metodos heredados de PrepPart
            virtual void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            virtual void leerDeCodigo(PrepTokeniser& tokeniser, Diccionario& diccionario);
            virtual void asignarValoresSimbolos(Diccionario& diccionario);
            virtual void generarCodigo(std::ostream& os, const Diccionario& diccionario) const;

        private:
            typedef std::map<std::string, Proceso> TProcesos;
            TProcesos _procesos;
            PrepListaMensajes& _listaMensajes;
            PrepListaMensajes& _listaSisMensajes;
            PrepVocabulario& _vocabulario;

            // Funciones de apoyo a leerDeCodigo
            void _leerVerboNombre (PrepEntradaMultiple& entrada, PrepTokeniser& tokeniser) const;
            void _leerCondactos (PrepEntradaMultiple& entrada, PrepTokeniser& tokeniser, Diccionario& diccionario);
            void _leerCondacto (PrepEntradaMultiple::Condacto& condacto, PrepTokeniser& tokeniser, Diccionario& diccionario);
            void _leerParametroCondacto(SpByte param, PrepEntradaMultiple::Condacto& condacto,
                                        const ::Inpaws::Condacto* infoCondacto,
                                        PrepTokeniser& tokeniser, Diccionario& diccionario);

            // Funciones de apoyo a generarCodigo
            void _volcarProceso (std::ostream& os, const Diccionario& diccionario,
                                 const TProcesos::const_iterator& procItr) const;
            void _volcarEntradaProceso (std::ostream& os, const Diccionario& diccionario,
                                 const std::vector<PrepEntradaMultiple::EntradaProceso>::const_iterator& entradaItr) const;
            void _volcarCondacto (std::ostream& os, const Diccionario& diccionario,
                                 const std::vector<PrepEntradaMultiple::Condacto>::const_iterator& itCond) const;
            int _obtenerValorPrm(const std::string& valorStr, const std::string& archivo, unsigned long linea,
                                 int tipo, const Diccionario& diccionario) const;
            bool _esTipoPalabraCorrecta (const std::string& condacto, const std::string& palabra) const;
            bool _esValorNumerico (const std::string& cadena) const;
    };
}

#endif
