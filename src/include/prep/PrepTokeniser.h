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

#ifndef PREPTOKENISER_H_
#define PREPTOKENISER_H_

#include "Tokeniser.h"
#include <list>
#include <fstream>
#include <set>

namespace Inpaws
{
    class PrepTokeniser: public Tokeniser
    {
        public:
            struct Archivo
            {
                Archivo (const std::string& anombre, std::ifstream* astream)
                    : nombre(anombre), stream(astream), lineaActual(1), bloqueIfActivo(false) { }
                std::string nombre;
                std::ifstream* stream;
                unsigned long lineaActual;
                bool bloqueIfActivo; // Si estamos dentro de un #IFDEF o #IFNDEF
                unsigned long bloqueIfLinea; // Linea donde se abre
            };

            PrepTokeniser(std::ifstream* is, const std::string& nombre);
            virtual ~PrepTokeniser();
            std::string archivoActual() const { return topFile().nombre; }
            virtual TTokenType leeToken();
            std::string errorLeidoEsperadoPrep (const std::string& esperado) const;
            bool insertarSimbolo(const std::string& simbolo);

        protected:
            Archivo& topFile() { return *_archivosAbiertos.begin(); }
            const Archivo& topFile() const { return *_archivosAbiertos.begin(); }
            void cerrarArchivoActual();
            void procesarDirectivaPreprocesador();
            void abrirNuevoArchivo();
            static std::string obtenerRuta(const std::string& nombreArchivo);
            static std::string baseName(const std::string& nombreArchivo);
            static bool comprobarNombre(const std::string& nombreArchivo);
            bool abiertoArchivo(const std::string& nombreArchivo) const;
            bool estaDefSimbolo(const std::string& simbolo) const;
            void leerSi (const std::string directiva);
            void ignorarBloqueDirectiva();
            void define();

        private:
            typedef std::list<Archivo> TListaArchivos;
            typedef std::set<std::string> TSimbolos;
            TListaArchivos _archivosAbiertos;
            TSimbolos _simbolos;
    };
}

#endif
