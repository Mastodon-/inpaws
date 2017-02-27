CC = g++
LANG= #-DMSG_ESPANOL # Uncomment for spanish error messages
CFLAGS = $(LANG) --pedantic -std=c++98 -Wall -s -O2
INCLUDES = -Isrc/include

ifdef COMSPEC
	# Static linking for M$
	LF_SPEC_FLAGS=-static-libgcc -static-libstdc++
else
	LF_SPEC_FLAGS=
endif
LFLAGS =$(LF_SPEC_FLAGS)

LIBS = 

SRCS = src/Abreviaturas.cpp src/Aventura.cpp src/Caracteres.cpp src/Condacto.cpp src/Conexiones.cpp src/Defaults.cpp src/EntradaProcesos.cpp src/Excepcion.cpp src/Fichero.cpp src/Graficos.cpp src/ListaEntradasVolcadas.cpp src/ListaProcesos.cpp src/ListaSimple.cpp src/PawCondacts.cpp src/PawPart.cpp src/Proceso.cpp src/SpBuffer.cpp src/Textos.cpp src/Tokeniser.cpp src/UtilTexto.cpp src/Vocabulario.cpp src/main.cpp src/prep/Diccionario.cpp src/prep/Error.cpp src/prep/Pool.cpp src/prep/PrepAbreviable.cpp src/prep/PrepAbreviaturas.cpp src/prep/PrepConstantes.cpp src/prep/PrepDefaults.cpp src/prep/PrepEntradaMultiple.cpp src/prep/PrepFlags.cpp src/prep/PrepListaDatos.cpp src/prep/PrepListaMensajes.cpp src/prep/PrepLocalidades.cpp src/prep/PrepObjetos.cpp src/prep/PrepPart.cpp src/prep/PrepProcesos.cpp src/prep/PrepSubChar.cpp src/prep/PrepTokeniser.cpp src/prep/PrepUtilTexto.cpp src/prep/PrepVocabulario.cpp src/prep/Preprocesador.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = inpaws

.PHONY: depend clean

all:    $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) $(OBJS) *~ $(MAIN)

