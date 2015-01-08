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

//----------------------------------------------------------------
//
// Conexiones: <- PawPart
//
// Conexiones entre localidades
//----------------------------------------------------------------

#ifndef CONEXIONES_H
#define CONEXIONES_H

#include "PawPart.h"
#include "Vocabulario.h"
#include <map>

namespace Inpaws
{
    class Conexiones: public PawPart
    {
        public:
            struct Conexion
            {
                SpByte localidad;
                SpByte palabra;
                SpByte destino;
            };

            Conexiones (PtrBuffer ptrIndice,SpByte numLocs = 0);
            virtual ~Conexiones();

            // Metodos heredados de PawPart
            virtual PtrBuffer inicioReal() const;
            virtual PtrBuffer volcarAMemoria (PtrBuffer ptrInicioDatos) const;
            virtual PtrBuffer volcarDeMemoria ();
            virtual void generarCodigo (std::ostream& os);
            void generarCodigoDisco (std::ostream& os, uint16_t flags, const Vocabulario* vocabulario);
            virtual void generarCodigoSuperglus (std::ostream& os, uint16_t flags,
                                                 const Vocabulario* vocabulario,
                                                 Diccionario::TDiccionarioInverso diccionario);
            virtual void leerDeCodigo (Tokeniser& tokeniser);

            // Para fijar el numero de localidades
            // OJO: hacerlo siempre antes de volcarAMemoria
            void setNumLocs(SpByte numLocs) { _numLocs = numLocs; }

            // Funciones para obtener y añadir conexiones
            void anadirConexion (SpByte localidad, SpByte palabra, SpByte destino);
            const Conexion* leerConexionInit (SpByte localidad) const;
            const Conexion* leerConexionNext (SpByte localidad) const;
            size_t size() const { return _conexiones.size(); }

        protected:
            typedef std::multimap<SpByte,Conexion> TConexiones;
            TConexiones _conexiones;

        private:
            SpByte _numLocs; // Numero de localidades (tantas conexiones como localidades)
            static TConexiones::const_iterator _itTmp; // Para el recorrido por leerConexionInit y ...Next


    };
}

#endif // CONEXIONES_H
