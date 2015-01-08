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

#ifndef LISTAPROCESOS_H_
#define LISTAPROCESOS_H_

#include "PawPart.h"
#include "Proceso.h"
#include <map>

namespace Inpaws
{
    class ListaProcesos: public PawPart
    {
        public:
            ListaProcesos(PtrBuffer ptrIndice, SpWord offNumPro);
            virtual ~ListaProcesos();

            // Metodos heredados de PawPart
            virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
            virtual PtrBuffer volcarDeMemoria ();
            virtual void generarCodigo (std::ostream& os);
            void generarCodigoDisco (std::ostream& os, uint16_t flags, const Vocabulario* vocabulario,
                                     const Diccionario::TDiccionarioInverso* diccionario = NULL);
            void generarCodigoSuperglus (std::ostream& os, uint16_t flags, const Vocabulario* vocabulario,
                                         const Diccionario::TDiccionarioInverso& diccionario);
            virtual void leerDeCodigo (Tokeniser& tokeniser);
            virtual PtrBuffer inicioReal() const;
            virtual int numElems() const { return _listaProcesos.size()-1; }

            // Funciones para obtener y añadir procesos
            void anadirProceso (unsigned int numProc, Proceso *proceso);
            const Proceso* obtenerProceso(unsigned int numProc) const;
            size_t numPro() const { return _listaProcesos.size(); }

        private:
            typedef std::map<unsigned int, Proceso*> TListaProcesos;
            TListaProcesos _listaProcesos;
            SpWord _offNumPro;

            void _leerDeCodigoProceso (Tokeniser& tokeniser, SpByte numProc);
    };
}

#endif // LISTAPROCESOS_H_
