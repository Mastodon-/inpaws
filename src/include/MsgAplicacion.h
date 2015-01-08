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

//-----------------------------------------------------------------
// Códigos para las letras acentuadas y la eñe:
// ñ -> \244
// Ñ -> \245
// á -> \240
// é -> \202
// í -> \241
// ó -> \242
// ú -> \243
//------------------------------------------------------------------

#ifndef MSGAPLICACION_H_
#define MSGAPLICACION_H_

#ifdef MSG_ESPANOL
// Errores de ejecucion
#define MSG_FALTAN_PARAMETROS "Faltan par\240metros"
#define MSG_ARCHIVO_NO_EXISTE "El archivo especificado no existe o est\240 corrupto"
#define MSG_ARCHIVO_NO_64K "El archivo especificado no es un volcado de memoria de 64K"
#define MSG_TIPONOREC "Tipo de archivo no reconocido"
#define MSG_ARCHIVO_NO_SNA "El archivo no es un volcado .SNA de 48k"
#define MSG_ARCHIVO_NO_Z80_48k "El archivo no es un volcado .Z80 de 48k"
#define MSG_NOBDPAW "No se encontr\242 la firma de una base de datos PAW en el fichero"

// Tipos de vocabulario
#define MSG_VERB "VERB"
#define MSG_ADVERB "ADVERB"
#define MSG_NOUN "NOUN"
#define MSG_ADJECTIVE "ADJECTIVE"
#define MSG_PREPOSITION "PREPOSITION"
#define MSG_CONJUNCTION "CONJUNCTION"
#define MSG_PRONOUN "PRONOUN"
#define MSG_RESERVED "RESERVED"

// Palabras reservadas
#define RSV_VOCABULARY "VOCABULARY"
#define RSV_LOCATIONS "LOCATIONS"
#define RSV_MESSAGES "MESSAGES"
#define RSV_SYSMESSAGES "SYSMESSAGES"
#define RSV_OBJECTS "OBJECTS"
#define RSV_CONNECTIONS "CONNECTIONS"
#define RSV_OBJWORDS "OBJWORDS"
#define RSV_INITIALLYAT "INITIALLYAT"
#define RSV_OBJWEIGHT "OBJWEIGHT"
#define RSV_PROCESS_TABLE "PROCESS_TABLE"
#define RSV_PROCESS "PROCESS"
#define RSV_RESPONSE "RESPONSE"
#define RSV_ABBREVIATIONS "ABBREVIATIONS"
#define RSV_CHARACTERS "CHARACTERS"
#define RSV_GRAPHICS "GRAPHICS"
#define RSV_DEFAULTS "DEFAULTS"
#define RSV_CHARSET "CHARSET"
#define RSV_INK "INK"
#define RSV_PAPER "PAPER"
#define RSV_FLASH "FLASH"
#define RSV_BRIGHT "BRIGHT"
#define RSV_INVERSE "INVERSE"
#define RSV_OVER "OVER"
#define RSV_BORDER "BORDER"
// Palabras reservadas del preprocesador
#define RSV_LOCATION "LOCATION"
#define RSV_OBJECT "OBJECT"
#define RSV_NOTCREATED "NOTCREATED"
#define RSV_WORN "WORN"
#define RSV_CARRIED "CARRIED"
#define RSV_WORDS "WORDS"
#define RSV_WEIGHT "WEIGHT"
#define RSV_PROPERTY "PROPERTY"
#define RSV_CONTAINER "CONTAINER"
#define RSV_CLOTHING "CLOTHING"
#define RSV_FLAG "FLAG"
#define RSV_SUBCHAR "SUBCHAR"
#define RSV_CONSTANT "CONSTANT"
#define RSV_INCLUDE "INCLUDE"
#define RSV_IFDEF "IFDEF"
#define RSV_IFNDEF "IFNDEF"
#define RSV_ENDIF "ENDIF"
#define RSV_DEFINE "DEFINE"

// Errores de sintaxis
#define MSG_PALAB_RESERV_NO_ENCONTRADA "esperado, pero encontrado "
#define MSG_FIN_DE_ARCHIVO "fin de archivo"
#define MSG_CODIGO_DE_PALABRA "c\242digo de palabra de diccionario"
#define MSG_LITERAL_DE_TIPO_DE_PALABRA "literal de tipo de palabra"
#define MSG_LITERAL_ENTRECOMILLADO "literal entrecomillado"
#define MSG_NUMERO_TEXTO_ESPERADO "n\243mero de mensaje de texto"
#define MSG_NUMERO_LOCALIDAD "n\243mero de localidad"
#define MSG_NUMERO_OBJETO "n\243mero de objeto"
#define MSG_DATO_ATRIBUTO_OBJETO "dato de atributo de objeto"
#define MSG_NOMBRE_CONDACTO "nombre de condacto"
#define MSG_PARAMETRO_CONDACTO "par\240metro de condacto"
#define MSG_CODIGO_ABREVIATURA "c\242digo de abreviatura (164-254)"
#define MSG_NOMBRE_ENTRADA_PROCESO "c\242digo de nombre en la entrada de procesos"
#define MSG_CODIGO_DECIMAL "c\242digo decimal"
#define MSG_ATRIBUTO_CHARSET "literal de atributo o juego de caracteres"
#define MSG_NOMBRE_MODULO_PAW "literal de modulo de PAW"

// Otros errores
#define MSG_FUERA_DE_LIMITE "Acceso a estructura de datos fuera de l\241mite"
#define MSG_CODIGO_NO_NUMERICO "C\242digo de control no num\202rico"
#define MSG_ATRIBUTOS_OBJ_INSUFICIENTES "Faltan objetos por asignar atributo"
#define MSG_COD_CONDACTO_MUY_GRANDE "C\242digo de condacto fuera de l\241mite (0-107)"
#define MSG_ARG_CONDACTO_MUY_GRANDE "Argumento de Condacto demasiado grande"
#define MSG_CONDACTO_INEXISTENTE "No existe un condacto con ese nombre"
#define MSG_NO_VERBO_UNO "Los verbos con c\242odigo 1 generan inconsistencias en las tablas de procesos y respuestas de PAW"
#define MSG_MAINTOP_NO_DEF "Maintop no definido, fija su valor con la funcion setMaintop()"
#define MSG_ATRIBUTO_MUY_GRANDE "c\242digo de atributo demasiado grande"
#define MSG_NUMLOCS_NO_DEF "debes definir el n\243mero de localidades con la funcion setNumLocs antes de volcar"
#define MSG_CADENA_MAL_FORMADA "cadena de caracteres mal formada"
#define MSG_SINTAXIS "Sintaxis: inpaws <comando> <fichero de entrada> [opciones]\n\n\
  Comandos:\n\
    (Generaci\242n para ZX Spectrum 48k):\n\
      c:  Compilar a fichero .tap\n\n\
    (Generaci\242n para PAW CP/M y PC):\n\
      cd: Compilar a fichero fuente .SCE de PAW-PC\n\
      cm: Compilar a fichero fuente .SCE de PAW-CPM (Amstrad CPC)\n\n\
    (Generaci\242n para Superglus[Glulx]):\n\
      cs: Compilar a fichero fuente .SCE de Superglus sin s\241mbolos\n\
      ct: Compilar a fichero fuente .TXP de Superglus/txtPaws con s\241mbolos\n\n\
    (Otros comandos):\n\
      e:  Extraer fuente de .SNA o .Z80 con una aventura de PAW 48k\n\
      cp: S\242lo generar fichero preprocesado, sin compilar (DEPURACION)\n\
      eg: Extraer graficos de .SNA o .Z80 con una aventura de PAW 48k\n\
      ec: Extraer juegos de caracteres de .SNA o .Z80\n\n\
  Opciones:\n\
      -o <fichero de salida>: Nombre dado al fichero generado por inpaws\n\
      -s : Listado detallado de s\241mbolos tras la compilaci\242n\n"
#define MSG_SINTAXISINC "Sintaxis incorrecta"
#define MSG_ERRORFICHERO "Error al abrir el fichero"
#define MSG_MEMLIBRE "Memoria libre tras compilaci\242n: "
#define MSG_SINMEMORIA "Compilaci\242n fallida: l\241mite de memoria de PAW superado"

// Resumen de una compilacion
#define MSG_RESUMEN_LOCALIDADES "localidades"
#define MSG_RESUMEN_OBJETOS "objetos"
#define MSG_RESUMEN_MENSAJES "mensajes"
#define MSG_RESUMEN_SISMENSAJES "mensajes del sistema"
#define MSG_RESUMEN_PROCESOS "procesos"
#define MSG_RESUMEN_CARACTERES "juegos de caracteres"
#define MSG_RESUMEN_VERSION "Versi\242n de la base de datos:"
#define MSG_RESUMEN_ABREVIATURAS "Ahorro por compresi\242n:"
#define MSG_RESUMEN_COMPILOK "Proceso de compilaci\242n finalizado correctamente\nArchivo generado: "
#define MSG_RESUMEN_EXTRAEOK "Base de datos PAW correctamente extraida al archivo: "
#define MSG_SIMBOLOSUSADOS "S\241mbolos usados en la compilaci\242n:"
#define MSG_SCEPORINPAWS "Archivo generado automáticamente por Inpaws, a partir de una sintaxis Inpaws"

#else
// MENSAJES EN INGLES

// Errores de ejecucion
#define MSG_FALTAN_PARAMETROS "Too few parameters"
#define MSG_ARCHIVO_NO_EXISTE "The specified file doesn't exists, or is corrupt"
#define MSG_ARCHIVO_NO_64K "The specified file is not a 64K memory dump"
#define MSG_ARCHIVO_NO_SNA "The specified file is not a .SNA snapshot"
#define MSG_ARCHIVO_NO_Z80_48k "The specified file is not a 48k .Z80 snapshot"
#define MSG_TIPONOREC "Unrecognized file type"
#define MSG_NOBDPAW "PAW DataBase signature not found in file"

// Tipos de vocabulario
#define MSG_VERB "VERB"
#define MSG_ADVERB "ADVERB"
#define MSG_NOUN "NOUN"
#define MSG_ADJECTIVE "ADJECTIVE"
#define MSG_PREPOSITION "PREPOSITION"
#define MSG_CONJUNCTION "CONJUNCTION"
#define MSG_PRONOUN "PRONOUN"
#define MSG_RESERVED "RESERVED"

// Palabras reservadas
#define RSV_VOCABULARY "VOCABULARY"
#define RSV_LOCATIONS "LOCATIONS"
#define RSV_MESSAGES "MESSAGES"
#define RSV_SYSMESSAGES "SYSMESSAGES"
#define RSV_OBJECTS "OBJECTS"
#define RSV_CONNECTIONS "CONNECTIONS"
#define RSV_OBJWORDS "OBJWORDS"
#define RSV_INITIALLYAT "INITIALLYAT"
#define RSV_OBJWEIGHT "OBJWEIGHT"
#define RSV_PROCESS_TABLE "PROCESS_TABLE"
#define RSV_PROCESS "PROCESS"
#define RSV_RESPONSE "RESPONSE"
#define RSV_ABBREVIATIONS "ABBREVIATIONS"
#define RSV_CHARACTERS "CHARACTERS"
#define RSV_GRAPHICS "GRAPHICS"
#define RSV_DEFAULTS "DEFAULTS"
#define RSV_CHARSET "CHARSET"
#define RSV_INK "INK"
#define RSV_PAPER "PAPER"
#define RSV_FLASH "FLASH"
#define RSV_BRIGHT "BRIGHT"
#define RSV_INVERSE "INVERSE"
#define RSV_OVER "OVER"
#define RSV_BORDER "BORDER"
// Palabras reservadas del preprocesador
#define RSV_LOCATION "LOCATION"
#define RSV_OBJECT "OBJECT"
#define RSV_NOTCREATED "NOTCREATED"
#define RSV_WORN "WORN"
#define RSV_CARRIED "CARRIED"
#define RSV_WORDS "WORDS"
#define RSV_WEIGHT "WEIGHT"
#define RSV_PROPERTY "PROPERTY"
#define RSV_CONTAINER "CONTAINER"
#define RSV_CLOTHING "CLOTHING"
#define RSV_FLAG "FLAG"
#define RSV_SUBCHAR "SUBCHAR"
#define RSV_CONSTANT "CONSTANT"
#define RSV_INCLUDE "INCLUDE"
#define RSV_IFDEF "IFDEF"
#define RSV_IFNDEF "IFNDEF"
#define RSV_ENDIF "ENDIF"
#define RSV_DEFINE "DEFINE"

// Errores de sintaxis
#define MSG_PALAB_RESERV_NO_ENCONTRADA "expected, but found "
#define MSG_FIN_DE_ARCHIVO "end of file"
#define MSG_CODIGO_DE_PALABRA "dictionary word code"
#define MSG_LITERAL_DE_TIPO_DE_PALABRA "word type"
#define MSG_LITERAL_ENTRECOMILLADO "quotation marked literal"
#define MSG_NUMERO_TEXTO_ESPERADO "text message number"
#define MSG_NUMERO_LOCALIDAD "location number"
#define MSG_NUMERO_OBJETO "object number"
#define MSG_DATO_ATRIBUTO_OBJETO "object attribute data"
#define MSG_NOMBRE_CONDACTO "condact name"
#define MSG_PARAMETRO_CONDACTO "condact parameter"
#define MSG_CODIGO_ABREVIATURA "abbreviature code (164-254)"
#define MSG_NOMBRE_ENTRADA_PROCESO "noun code in process entry"
#define MSG_CODIGO_DECIMAL "decimal code"
#define MSG_ATRIBUTO_CHARSET "attribute/charset literal"
#define MSG_NOMBRE_MODULO_PAW "PAW module literal"

// Otros errores
#define MSG_FUERA_DE_LIMITE "Out of bounds data access"
#define MSG_CODIGO_NO_NUMERICO "Not numeric control code"
#define MSG_ATRIBUTOS_OBJ_INSUFICIENTES "Not all objects have attribute"
#define MSG_COD_CONDACTO_MUY_GRANDE "Condact code out of range (0-107)"
#define MSG_ARG_CONDACTO_MUY_GRANDE "Condact argument too big"
#define MSG_CONDACTO_INEXISTENTE "The named condact does no exists"
#define MSG_NO_VERBO_UNO "Verbs with code 1 produce inconsistencies with Response/Process tables in PAW"
#define MSG_MAINTOP_NO_DEF "Maintop not defined, set it with setMaintop() function"
#define MSG_ATRIBUTO_MUY_GRANDE "c\242digo de atributo demasiado grande"
#define MSG_NUMLOCS_NO_DEF "location number not defined, set it with setNumLocs"
#define MSG_CADENA_MAL_FORMADA "character string bad formed"

#define MSG_SINTAXIS "Syntax: inpaws <command> <input file> [options]\n\n\
  Commands:\n\
    (Target ZX Spectrum 48k):\n\
      c:  Compile to .tap file\n\n\
    (Target PAW CP/M and PC):\n\
      cd: Compile to PAW-PC .SCE source file\n\
      cm: Compile to PAW CPM(Amstrad CPC) .SCE source file\n\n\
    (Target Superglus[Glulx]):\n\
      cs: Compile to Superglus .SCE source file (without symbols)\n\
      ct: Compile to Superglus/txtPaws .TXP source file (with symbols)\n\n\
    (Other commands):\n\
      e:  Extract source from a 48k PAW .SNA or .Z80\n\
      cp: Only generate preprocesed file, without compiling (DEBUGGING)\n\
      eg: Extract graphics code from a 48k PAW .SNA or .Z80\n\
      ec: Extract character info from a 48k PAW .SNA or .Z80\n\n\
  Options:\n\
      -o <output file>: Output file name\n\
      -s : detailed symbol list after compilation\n"
#define MSG_SINTAXISINC "Incorrect syntax"
#define MSG_ERRORFICHERO "Error opening file"
#define MSG_MEMLIBRE "Free memory after compile: "
#define MSG_SINMEMORIA "Compile failed: PAW memory limit reached"

// Resumen de una compilacion
#define MSG_RESUMEN_LOCALIDADES "locations"
#define MSG_RESUMEN_OBJETOS "objects"
#define MSG_RESUMEN_MENSAJES "messages"
#define MSG_RESUMEN_SISMENSAJES "system messages"
#define MSG_RESUMEN_PROCESOS "processes"
#define MSG_RESUMEN_CARACTERES "character sets"
#define MSG_RESUMEN_VERSION "Database version:"
#define MSG_RESUMEN_ABREVIATURAS "Saved by compression:"
#define MSG_RESUMEN_COMPILOK "Compilation successfully finished\nResulting file: "
#define MSG_RESUMEN_EXTRAEOK "PAW Database successfully extracted to file: "
#define MSG_SIMBOLOSUSADOS "Used symbols in compilation process:"
#define MSG_SCEPORINPAWS "File automatically generated by Inpaws, from an Inpaws syntax"

#endif

#endif
