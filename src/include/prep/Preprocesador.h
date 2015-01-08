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

//-------------------------------------------------------------------------------------------------
//
// RESUMEN DEL ALGORITMO QUE SIGUE EL PREPROCESADOR
// 1) Lee el codigo sin preprocesar de los elementos de la aventura
// 2) Añade, a medida que los lee:
//      2.1) Los nombres de las nuevas entidades definidas con identificador al diccionario
//      2.2) Los códigos de las nuevas entidades definidas con número al diccionario
//          2.2.1) Para estas últimas, añade al pool de asignados para que luego no se asignen
// 3) Una vez finalizada TODA la lectura del codigo sin preprocesar,se asignan valores a los identificadores
//    no definidos, tomando del pool los no asignados.
// 4) Se genera el código preprocesado de cada entidad, sustituyendo las referencias a los elementos
//    de la aventura por los valores almacenados en el diccionario.
//-----------------------------------------------------------------------------------------------------------

#ifndef _PREPROCESADOR_H_
#define _PREPROCESADOR_H_

#include <iostream>
#include "PrepTokeniser.h"
#include "Aventura.h"
#include "Diccionario.h"
#include "PrepLocalidades.h"
#include "PrepVocabulario.h"
#include "PrepObjetos.h"
#include "PrepListaMensajes.h"
#include "PrepListaDatos.h"
#include "PrepDefaults.h"
#include "PrepProcesos.h"
#include "PrepFlags.h"
#include "PrepAbreviaturas.h"
#include "PrepSubChar.h"
#include "PrepConstantes.h"

namespace Inpaws
{
    class Preprocesador
    {
        public:
            Preprocesador(std::ifstream* is, const std::string& nombreFichero = "", const uint16_t formatoSalida = VERSPECTRUM);
            virtual ~Preprocesador();

            // Funcion principal
            void preprocesar(std::ostream& os);

            // Generar codigo "amistoso" (reinterpretable por el preprocesador ~> Desensamblar aventuras)
            void generarCodigoPrep (std::ostream& os, const Aventura& aventura) const;
            void generarCodigoGraficos (std::ostream& os, const Aventura& aventura) const;
            void generarCodigoCaracteres (std::ostream& os, const Aventura& aventura) const;

            SpWord ahorroCompresion() const { return _ahorroCompresion; }
            void mostrarSimbolos(std::ostream& os) const { return _diccionario.mostrarSimbolos(os); }
            const Diccionario& diccionario() const { return _diccionario; }

        protected:
            // Funciones del algoritmo principal
            void leerDeCodigo();                        // Lee la aventura y crea los simbolos
            void asignarValoresSimbolos();              // Asigna los valores a los simbolos
            void generarCodigo(std::ostream& os) const; // Genera el codigo preprocesado

        private:
            PrepTokeniser _tokeniser;
            Diccionario _diccionario;
            PrepSubChar _subchar;
            PrepLocalidades _localidades;
            PrepVocabulario _vocabulario;
            PrepObjetos _objetos;
            PrepListaMensajes _mensajes;
            PrepListaMensajes _sismensajes;
            PrepListaDatos _graficos;
            PrepListaDatos _caracteres;
            PrepDefaults _defaults;
            PrepProcesos _procesos;
            PrepFlags _flags;
            PrepConstantes _constantes;
            PrepAbreviaturas _abreviaturas;
            SpWord _ahorroCompresion;
            uint16_t _formatoSalida; // Controlan lo que hay que generar (comprimido si/no, version cp/m, etc)
    };
}

#endif
