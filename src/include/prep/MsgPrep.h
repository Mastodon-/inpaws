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

#ifndef MSGPREP_H_
#define MSGPREP_H_

#ifdef MSG_ESPANOL

#define MSGPREP_IDEFPAW "apertura de bloque de InPAWS o directiva"
#define MSGPREP_DEFCORR "no se pudo establecer una definici\242n correlativa de los elementos"
#define MSGPREP_CODIGONOMBRELOC "c\242digo o nombre de localidad"
#define MSGPREP_DESCLOCALIDAD "descripci\242n de la localidad"
#define MSGPREP_CODIGOPALABRA "palabra de vocabulario"
#define MSGPREP_PALABRASINONIMO "el tama\244o recortado de la palabra generará repetición con una definici\242n anterior"
#define MSGPREP_NUMELEMSUP "n\243mero de elementos definidos superado"
#define MSGPREP_YADEFINIDO "identificador/c\242digo ya definido anteriormente"
#define MSGPREP_ALMENOSUNALOC "Debe definirse al menos una localidad"
#define MSGPREP_ALMENOSUNOBJ "Debe definirse al menos un objeto"
#define MSGPREP_PALABRANODEF "palabra de vocabulario no definida"
#define MSGPREP_LOCNODEF "localidad no definida"
#define MSGPREP_LOCRESERV "los c\242digos de localidad mayores de 251 est\240n reservados por el sistema"
#define MSGPREP_FORMATOPALABRA "formato incorrecto de literal de palabra en vocabulario"
#define MSGPREP_LITERALPALABRA "literal de palabra de vocabulario entrecomillada"
#define MSGPREP_WORDRESERV "los c\242digos de palabra menores que 2 y mayores que 254 est\240n reservados"
#define MSGPREP_CODIGONOMBREOBJ "c\242digo o nombre del objeto"
#define MSGPREP_DESCOBJETO "descripci\242n del objeto"
#define MSGPREP_PROPOBJETO "palabra reservada de propiedad de objeto"
#define MSGPREP_PESOERROR "un peso de objeto entre 0 y 63"
#define MSGPREP_CODIGOMES "c\242digo o nombre de mensaje"
#define MSGPREP_TEXTOMENSAJE "descripci\242n de texto del mensaje"
#define MSGPREP_ALMENOSUNMSG "debe definirse al menos un mensaje"
#define MSGPREP_SISMESRESERV "los mensajes de sistema 0 al 53 se utilizan por el sistema y deben estar definidos"
#define MSGPREP_CODIGONUM "c\242digo num\202rico 0-255"
#define MSGPREP_NOMPROCESO "c\242digo o n\243mero de proceso"
#define MSGPREP_DEFPROC "fin de declaraci\242n o apertura de definici\242n de proceso"
#define MSGPREP_PALENTRADAPROC "palabra de entrada de proceso"
#define MSGPREP_NOMCONDACTO "nombre de condacto"
#define MSGPREP_VALORALTO "valor demasiado grande para el par\240metro del condacto"
#define MSGPREP_PROCNODEC "proceso no declarado"
#define MSGPREP_PALNOVAL "tipo de palabra no v\240lido"
#define MSGPREP_MESNODEF "mensaje no definido"
#define MSGPREP_SISMESNODEF "mensaje del sistema no definido"
#define MSGPREP_OBJNODEF "objeto no definido"
#define MSGPREP_FLAGNODEF "flag no definido"
#define MSGPREP_NOMFLAG "nombre de flag"
#define MSGPREP_LITERALINCORRECTO "literal mal formado"
#define MSGPREP_CADENASUBINCORRECTA "cadena de sustituci\242n incorrecta"
#define MSGPREP_SUBLARGO "cadena de sustituci\242n demasiado larga o mal formada"
#define MSGPREP_CARACTERSUB "car\240cter de sustituci\242n"
#define MSGPREP_CADENASUB "cadena de caracteres que sustituye al car\240cter"
#define MSGPREP_SUBYADEF "car\240cter de sustituci\242n ya definido"
#define MSGPREP_NOMBRECONST "nombre de constante"
#define MSGPREP_VALORCONST "valor de constante (0-255)"
#define MSGPREP_IDNODEF "identificador no definido"
#define MSGPREP_NOPATH "no se permite especificar ruta en los INCLUDE"
#define MSGPREP_FINARCHIVO "fin de archivo inesperado"
#define MSGPREP_NOMBREINCLUDE "nombre del archivo a incluir"
#define MSGPREP_INCLUDEANIDADO "include anidado"
#define MSGPREP_NOCHARACTERS "es necesario incluir un bloque CHARACTERS no vac\241o en el fichero fuente"
#define MSGPREP_NOREDEFDATOS "no se permite incluir m\240s de un bloque CHARACTERS o GRAPHICS en el fichero fuente"
#define MSGPREP_NODATOSVACIO "no se permite un bloque CHARACTERS o GRAPHICS sin datos"
#define MSGPREP_SYSMESCPC "las versiones para SCE o para Superglus requieren un m\241nimo de 61 mensajes del sistema"
#define MSGPREP_ENDSINIF "no se encontr\242 directiva de apertura #IFDEF o #IFNDEF"
#define MSGPREP_DEFINENAME "nombre de variable de preprocesador"
#define MSGPREP_NODEFANID "no se permite anidamiento de #IFDEF/#IFNDEF"
#define MSGPREP_IFSINEND "no se encontr\242 la directiva #ENDIF"
#define MSGPREP_DEFINEREPE "variable de preprocesador redefinida"
#define MSGPREP_NOLOCNOPLUS "el condacto no admite localidades especiales > 251"
#define MSGPREP_NOCOND "el condacto no es una condici\242n y no se admite en opciones m\243ltiples"

#else
// MENSAJES EN INGLES

#define MSGPREP_IDEFPAW "block opening or directive"
#define MSGPREP_DEFCORR "could'nt establish a correlative definition for the elements"
#define MSGPREP_CODIGONOMBRELOC "location code or name"
#define MSGPREP_DESCLOCALIDAD "location description"
#define MSGPREP_CODIGOPALABRA "vocabulary word"
#define MSGPREP_PALABRASINONIMO "reduced word size will produce the same word than a former definition"
#define MSGPREP_NUMELEMSUP "limit of defined elements reached"
#define MSGPREP_YADEFINIDO "identifier/code already defined"
#define MSGPREP_ALMENOSUNALOC "At least one location must be defined"
#define MSGPREP_ALMENOSUNOBJ "At least one object must be defined"
#define MSGPREP_PALABRANODEF "undefined vocabulary word"
#define MSGPREP_LOCNODEF "undefined location"
#define MSGPREP_LOCRESERV "location codes greater than 251 are reserved by the system"
#define MSGPREP_FORMATOPALABRA "incorrect vocabulary word format"
#define MSGPREP_LITERALPALABRA "quotation marked vocabulary word literal"
#define MSGPREP_WORDRESERV "word codes less than 2 and greater than 254 are reserved"
#define MSGPREP_CODIGONOMBREOBJ "object name or code"
#define MSGPREP_DESCOBJETO "object description"
#define MSGPREP_PROPOBJETO "object property reserved word"
#define MSGPREP_PESOERROR "object weight between 0 and 63"
#define MSGPREP_CODIGOMES "c\242digo o nombre de mensaje"
#define MSGPREP_TEXTOMENSAJE "message text description"
#define MSGPREP_ALMENOSUNMSG "at least one message must be defined"
#define MSGPREP_SISMESRESERV "sysmessages between 0 and 53 are used by the system and must be defined"
#define MSGPREP_CODIGONUM "numeric code 0-255"
#define MSGPREP_NOMPROCESO "process code or number"
#define MSGPREP_DEFPROC "process declaration end or definition opening"
#define MSGPREP_PALENTRADAPROC "process entry word"
#define MSGPREP_NOMCONDACTO "condact name"
#define MSGPREP_VALORALTO "Value too big for condact parameter"
#define MSGPREP_PROCNODEC "process not declared"
#define MSGPREP_PALNOVAL "not valid word type"
#define MSGPREP_MESNODEF "undefined message"
#define MSGPREP_SISMESNODEF "undefined system message"
#define MSGPREP_OBJNODEF "undefined object"
#define MSGPREP_FLAGNODEF "undefined flag"
#define MSGPREP_NOMFLAG "flag name"
#define MSGPREP_LITERALINCORRECTO "bad formed literal"
#define MSGPREP_CADENASUBINCORRECTA "bad formed substitution string"
#define MSGPREP_SUBLARGO "substituion string too long or bad formed"
#define MSGPREP_CARACTERSUB "substitution character"
#define MSGPREP_CADENASUB "character string that substitutes character"
#define MSGPREP_SUBYADEF "already defined substitution character"
#define MSGPREP_NOMBRECONST "constant name"
#define MSGPREP_VALORCONST "constant value (0-255)"
#define MSGPREP_IDNODEF "undefined identifier"
#define MSGPREP_NOPATH "specifying path in INCLUDE is not allowed"
#define MSGPREP_FINARCHIVO "unexpected end of file found"
#define MSGPREP_NOMBREINCLUDE "filename to include"
#define MSGPREP_INCLUDEANIDADO "nested file include"
#define MSGPREP_NOCHARACTERS "one not empty CHARACTERS block is mandatory in the source file"
#define MSGPREP_NOREDEFDATOS "it's not allowed to include more than one CHARACTERS or GRAPHICS block in the source file"
#define MSGPREP_NODATOSVACIO "empty GRAPHICS or CHARACTERS block not allowed"
#define MSGPREP_SYSMESCPC "SCE and Superglus versions require a minium of 61 system messages defined"
#define MSGPREP_ENDSINIF "#IFDEF o #IFNDEF opening directive not found"
#define MSGPREP_DEFINENAME "preprocessor variable name"
#define MSGPREP_NODEFANID "nested #IFDEF/#IFNDEF are not allowed"
#define MSGPREP_IFSINEND "#ENDIF directive not found"
#define MSGPREP_DEFINEREPE "redefined preprocessor variable"
#define MSGPREP_NOLOCNOPLUS "condact does not accept special locations > 251"
#define MSGPREP_NOCOND "condact is not a condition and is not allowed in multiple options"

#endif

#endif

