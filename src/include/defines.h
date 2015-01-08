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

//-----------------------------------------------------------
// Algunas constantes de importancia
//-----------------------------------------------------------

#ifndef DEFINES_H_
#define DEFINES_H_

#include "SpBuffer.h"

namespace Inpaws
{
    // Punteros a los datos
    const SpWord PTR_MAINTOP = 65533;
    const SpWord PTR_VOCABULARIO = 65509;
    const SpWord PTR_LOCALIDADES = 65501;
    const SpWord PTR_MENSAJES = 65503;
    const SpWord PTR_SYSMENSAJES = 65505;
    const SpWord PTR_OBJETOS = 65499;
    const SpWord PTR_PROCESOS = 65497;
    const SpWord PTR_GRAPH = 65521;
    const SpWord PTR_GRAPHATTR = 65523;
    const SpWord PTR_CONEXIONES = 65507;
    const SpWord PTR_INITIALLYAT = 65511;
    const SpWord PTR_OBJWEIGHT = 65515;
    const SpWord PTR_OBJWORDS = 65513;

    // Punteros a los datos (estos son respecto al MAINTOP)
    const SpWord OFF_ABBREV = 332;
    const SpWord OFF_FONTS = 330;


    // Desplazamientos respecto al MainTop de los contadores de datos (mensajes,objetos,...)
    const SpWord OFF_MAINATTR = 311;
    const SpWord OFF_MENSAJES = 326;
    const SpWord OFF_SYSMENS = 327;
    const SpWord OFF_NUMLOC = 325;
    const SpWord OFF_NUMOBJ = 324;
    const SpWord OFF_NUMPRO = 328;
    const SpWord OFF_NUMFONTS = 329;

    // Desplazamientos resp. MAINATTR de los "defaults"
    const SpWord OFF_DEF_CHARSET = 281; // Este es respecto al maintop
    const SpWord OFF_DEF_INK = 1;
    const SpWord OFF_DEF_PAPER = 3;
    const SpWord OFF_DEF_FLASH = 5;
    const SpWord OFF_DEF_BRIGHT = 7;
    const SpWord OFF_DEF_INVERSE = 9;
    const SpWord OFF_DEF_OVER = 11;
    const SpWord OFF_DEF_BORDER = 12;

    // OTROS VALORES RELEVANTES
    const SpWord PTR_FINDATOS = 65517;
    const SpWord PTR_INICIOGRAFICOS = 65519;
    const SpWord DIR_FINGRAFICOS = 65491;

    // Comienzo de indicadores de paginas del 128k (255 para no definidas)
    const SpWord OFF_128K = 298; // Desplazamiento resp maintop.
    const SpWord OFF_128KOTRO = 284; // Resp mainTop

    // Tipos de variable
    enum { FLAGNO, LOCNO, LOCNO_PLUS, MESNO,
           OBJNO, PROCNO, SYSNO, WORD, VALOR };

    // Versión a generar
    enum { VERSPECTRUM = 1,   // Versión .TAP para Spectrum
           VERCPM = 2,        // Versión .SCE para CPM (Amstrad CPC)
           VERPC = 4,         // Versión .SCE para PC
           NOCOMPRIMIR = 8,   // No generar abreviaturas
           CONVERTSPE = 16,   // Si se está intentando una conversión a SCE desde una extracción de Spectrum
           VERGLULX = 32,   // Version para Superglus(SCE)
           VERTXP = 64 };   // Version para Superglus(TXP)
}

#endif
