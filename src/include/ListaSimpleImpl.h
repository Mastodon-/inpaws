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

//-------------------------------------------------------------------------------
// Implementacion de la ListaSimple
//
// Formato de los ObjWords, InitiallyAt y ObjWeight:
//
// ObjWords:
//     PTR_OBJWORDS contiene un puntero a la lista de palabras de los objetos.
//     Por cada objeto se almacenan dos bytes: el codigo del nombre y el codigo
//     del adjetivo (codigos de vocabulario). Se usará el 255 como código si
//     el objeto no tiene esa palabra asociada (nombre o adjetivo)
//
// InitiallyAt:
//     PTR_INIITALLYAT contiene un puntero a la lista de localidades iniciales de
//     los objetos, un byte por objeto indicando la localidad donde comienza,
//     con los siguientes codigos especiales:
//         252: No creado
//         253: Llevado puesto
//         254: En posesión del jugador
//
// ObjWeightType:
//     PTR_OBJWEIGHT contiene un puntero a la lista de pesos y tipos de cada uno de los
//     objetos del juego. Para cada objeto se almacena un byte, que se descompone
//     de la siguiente manera:
//         1er bit (AND 0x80): indica si el objeto se puede llevar puesto
//         2º bit (AND 0x40): indica si el objeto es un contenedor
//         6 ultimos bits (AND 0x3f): peso del objeto
//-------------------------------------------------------------------------------

template <int ELEMENTOS,const char* TITULO>
ListaSimple<ELEMENTOS,TITULO>::ListaSimple (PtrBuffer ptrIndice, SpByte numDatos):
 PawPart(ptrIndice), _numDatos(numDatos)
{

}

template <int ELEMENTOS,const char* TITULO>
ListaSimple<ELEMENTOS,TITULO>::~ListaSimple()
{
    for (TDatos::iterator it = _datos.begin();
         it != _datos.end();
         it++)
    {
        delete[] (*it).second;
    }
}

template <int ELEMENTOS,const char* TITULO>
PtrBuffer
ListaSimple<ELEMENTOS,TITULO>::volcarAMemoria (PtrBuffer ptrInicioDatos) const
{
    // Si no está completo, soltamos una excepción
    if (_numDatos != _datos.size())
    {
        throw Excepcion(std::string(TITULO) + MSG_ATRIBUTOS_OBJ_INSUFICIENTES);
    }
    // Primero almacenamos el inicio de los datos en el puntero maestro
    SPBUFFER->wordPoke(_ptrIndice, ptrInicioDatos);

    SpByte* dato;
    // Y ahora a copiar
    for (SpByte i = 0; i < _numDatos; i++)
    {
        dato = leerDato(i);
        if (dato != NULL)
        {
            for (int elem = 0; elem < ELEMENTOS; elem++)
            {
                SPBUFFER->bytePoke(ptrInicioDatos + (SpWord)ELEMENTOS*i + elem, dato[elem]);
            }
        }
        else
            throw Excepcion(MSG_ATRIBUTOS_OBJ_INSUFICIENTES);
    }

    // Por ultimo hay que volcar un dato a cero (255 si es localidad)
    for (SpWord i = 0; i < ELEMENTOS; i++)
    {
        SpByte dato;
        if (!strcmp(TITULO, RSV_INITIALLYAT))
            dato = 255;
        else
            dato = 0;
        SPBUFFER->bytePoke(ptrInicioDatos + _numDatos*ELEMENTOS + i, dato);
    }

    return ptrInicioDatos + (_numDatos+1)*ELEMENTOS - 1;
}

template <int ELEMENTOS,const char* TITULO>
PtrBuffer
ListaSimple<ELEMENTOS,TITULO>::volcarDeMemoria ()
{
    getInicioDatos(); // Apuntamos al comienzo de los datos con _ptrDatos

    // Fijamos el total de datos en el número total de objetos de la aventura
    setNumDatos (SPBUFFER->bytePeek(SPBUFFER->wordPeek(PTR_MAINTOP) + OFF_NUMOBJ));

    // Y escribimos tantas veces como objetos haya a partir de _ptrDatos
    for (SpByte i = 0; i < _numDatos; i++)
    {
        SpByte* dato = new SpByte[ELEMENTOS];
        for (int elem = 0; elem < ELEMENTOS; elem++)
        {
            dato[elem] = SPBUFFER->bytePeek(ptrDatos()+ (SpWord)ELEMENTOS*i + elem);
        }
        anadirDato(i,dato);
    }

    // Al final hay ELEMENTOS bytes a cero (255 en el caso de localidades)

    return ptrDatos() + (_numDatos+1)*ELEMENTOS - 1;
}

template <int ELEMENTOS,const char* TITULO>
void
ListaSimple<ELEMENTOS,TITULO>::generarCodigo (std::ostream& os)
{
    os << TITULO << std::endl << "{" << std::endl;
    for (TDatos::const_iterator i=_datos.begin(); i !=_datos.end(); i++)
    {
        os << '\t' << (unsigned int)(*i).first << ": ";
        for (int j = 0; j < ELEMENTOS; j++)
        {
            os << (unsigned int)(*i).second[j];
            if (j < ELEMENTOS-1) os << ", ";
        }
        os << ";" << std::endl;
    }
    os << "}" << std::endl;
}

template <int ELEMENTOS,const char* TITULO>
void
ListaSimple<ELEMENTOS,TITULO>::leerDeCodigo (Tokeniser& tokeniser)
{

    TDato dato;
    Tokeniser::TTokenType token;
    SpByte numObjActual;

    // Actualmente esta funcion borra todo antes de leer
    _borrarDatos();

    saltarToken (tokeniser, Tokeniser::TK_IDENTIFICADOR, TITULO);
    saltarToken (tokeniser, Tokeniser::TK_CONTROL, "{");

    token = tokeniser.leeToken();
    while (!(token == Tokeniser::TK_CONTROL &&
             tokeniser.lvalue().literal == "}"))
    {
        // Codigo del objeto al que se asocia el atributo
        if (token == Tokeniser::TK_NUMERO)
            numObjActual = tokeniser.lvalue().valor;
        else
            throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_NUMERO_OBJETO));

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ":");

        // Los datos
        dato = new SpByte[ELEMENTOS];
        for (int i=0; i < ELEMENTOS; i++)
        {
            token = tokeniser.leeToken();
            if (token == Tokeniser::TK_NUMERO)
                dato[i] = tokeniser.lvalue().valor;
            else
            {
                delete[] dato;
                throw ExcTokeniser(tokeniser.errorLeidoEsperado(MSG_DATO_ATRIBUTO_OBJETO));
            }

            if (i < ELEMENTOS-1)
            {
                token = tokeniser.leeToken();
                if (!(token == Tokeniser::TK_CONTROL && tokeniser.lvalue().literal == ","))
                {
                    delete[] dato;
                    throw ExcTokeniser(tokeniser.errorLeidoEsperado(","));
                }
            }
        }
        anadirDato(numObjActual,dato);

        saltarToken (tokeniser, Tokeniser::TK_CONTROL, ";");
        token = tokeniser.leeToken();
    }
}

template <int ELEMENTOS,const char* TITULO>
void
ListaSimple<ELEMENTOS,TITULO>::anadirDato (SpByte numObj, const TDato dato)
{
    _datos.insert(std::pair<SpByte,TDato>(numObj,dato));
}

template <int ELEMENTOS,const char* TITULO>
const typename ListaSimple<ELEMENTOS,TITULO>::TDato
ListaSimple<ELEMENTOS,TITULO>::leerDato (SpByte numDato) const
{
    TDatos::const_iterator it = _datos.find(numDato);
    if (it != _datos.end())
        return (*it).second;
    else
        return NULL;
}

template <int ELEMENTOS,const char* TITULO>
void
ListaSimple<ELEMENTOS,TITULO>::_borrarDatos()
{
    for (TDatos::iterator it = _datos.begin();
         it != _datos.end();
         it++)
    {
        delete[] (*it).second;
    }
    _datos.clear();
}
