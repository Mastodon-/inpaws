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

#ifndef CONDACTO_H
#define CONDACTO_H

#include "SpBuffer.h"
#include "Tokeniser.h"
#include "Vocabulario.h"
#include "defines.h"

namespace Inpaws
{
    class Condacto
    {
        public:
            struct CondactData
            {
                CondactData(): codigo(0), prm1(0), prm2(0) { }
                SpByte codigo;
                SpByte prm1;
                SpByte prm2;

                bool operator<(const CondactData& otro) const
                {
                   return (codigo < otro.codigo ||
                          (codigo == otro.codigo && prm1 < otro.prm1) ||
                          (codigo == otro.codigo && prm1 == otro.prm1 && prm2 < otro.prm2));
                }
            };


            Condacto(SpByte codigo , const std::string& nombre,
                     SpByte numParams,
                     int tipo_prm1 = 0, SpByte max_valor_prm1 = 0,
                     int tipo_prm2 = 0, SpByte max_valor_prm2 = 0);

            ~Condacto();

            PtrBuffer volcarAMemoria (PtrBuffer ptrDireccion, const CondactData& dato) const;
            PtrBuffer volcarDeMemoria (PtrBuffer ptrDireccion, CondactData& dato) const;
            void generarCodigo (std::ostream& os, const CondactData& dato) const;
            void leerDeCodigo (Tokeniser& tokeniser, CondactData& dato) const;

            // Funcion utilizada por el preprocesador
            void generarCodigo (std::ostream& os, const CondactData& dato,
                                const Vocabulario* vocabulario, int flagsDeFormato = VERSPECTRUM,
                                const Diccionario::TDiccionarioInverso* diccionario = NULL) const;

            // Selectores
            SpByte codigo() const { return _codigo; }
            std::string nombre() const { return _nombre; }
            SpByte numParams() const { return _numParams; }
            int tipo_prm1() const { return _tipo_prm1; }
            SpByte max_valor_prm1() const { return _max_valor_prm1; }
            int tipo_prm2() const { return _tipo_prm2; }
            SpByte max_valor_prm2() const { return _max_valor_prm2; }

        protected:
        private:
            SpByte _codigo;
            std::string _nombre;
            SpByte _numParams;
            int _tipo_prm1;
            SpByte _max_valor_prm1;
            int _tipo_prm2;
            SpByte _max_valor_prm2;
            void _printNombreOValor(std::ostream& os, SpByte prm, int tipo, const Diccionario::TDiccionarioInverso* diccionario) const;
            bool _tratarCondactoSuperglus (std::ostream& os, const CondactData& dato, const Diccionario::TDiccionarioInverso* diccionario) const;
    };
}

#endif // CONDACTO_H
