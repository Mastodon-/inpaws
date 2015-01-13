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

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

src/Abreviaturas.o: src/include/Abreviaturas.h src/include/PawPart.h
src/Abreviaturas.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Abreviaturas.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Abreviaturas.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Abreviaturas.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Abreviaturas.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Abreviaturas.o: src/include/defines.h src/include/MsgAplicacion.h
src/Abreviaturas.o: src/include/Excepcion.h src/include/UtilTexto.h
src/Abreviaturas.o: src/include/prep/PrepAbreviaturas.h
src/Abreviaturas.o: src/include/prep/PrepAbreviable.h src/include/Aventura.h
src/Aventura.o: src/include/Aventura.h src/include/PawPart.h
src/Aventura.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Aventura.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Aventura.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Aventura.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Aventura.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Aventura.o: src/include/defines.h src/include/MsgAplicacion.h
src/Aventura.o: src/include/Defaults.h src/include/Vocabulario.h
src/Aventura.o: src/include/Textos.h src/include/Conexiones.h
src/Aventura.o: src/include/ListaSimple.h src/include/Excepcion.h
src/Aventura.o: src/include/ListaSimpleImpl.h src/include/ListaProcesos.h
src/Aventura.o: src/include/Proceso.h src/include/EntradaProcesos.h
src/Aventura.o: src/include/Condacto.h src/include/Abreviaturas.h
src/Aventura.o: src/include/Graficos.h src/include/Caracteres.h
src/Caracteres.o: src/include/Caracteres.h src/include/PawPart.h
src/Caracteres.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Caracteres.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Caracteres.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Caracteres.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Caracteres.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Caracteres.o: src/include/defines.h src/include/MsgAplicacion.h
src/Caracteres.o: src/include/Excepcion.h
src/Condacto.o: src/include/Condacto.h src/include/SpBuffer.h
src/Condacto.o: /usr/include/stdint.h /usr/include/features.h
src/Condacto.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/Condacto.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/Condacto.o: /usr/include/bits/wchar.h src/include/Tokeniser.h
src/Condacto.o: src/include/Vocabulario.h src/include/PawPart.h
src/Condacto.o: src/include/prep/Diccionario.h src/include/defines.h
src/Condacto.o: src/include/MsgAplicacion.h src/include/Excepcion.h
src/Condacto.o: src/include/PawCondacts.h src/include/UtilTexto.h
src/Condacto.o: src/include/prep/PrepAbreviaturas.h
src/Condacto.o: src/include/prep/PrepAbreviable.h src/include/Aventura.h
src/Conexiones.o: src/include/Conexiones.h src/include/PawPart.h
src/Conexiones.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Conexiones.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Conexiones.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Conexiones.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Conexiones.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Conexiones.o: src/include/Vocabulario.h src/include/defines.h
src/Conexiones.o: src/include/MsgAplicacion.h src/include/Excepcion.h
src/Defaults.o: src/include/Defaults.h src/include/PawPart.h
src/Defaults.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Defaults.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Defaults.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Defaults.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Defaults.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Defaults.o: src/include/defines.h src/include/MsgAplicacion.h
src/Defaults.o: src/include/Excepcion.h
src/EntradaProcesos.o: src/include/EntradaProcesos.h src/include/SpBuffer.h
src/EntradaProcesos.o: /usr/include/stdint.h /usr/include/features.h
src/EntradaProcesos.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/EntradaProcesos.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/EntradaProcesos.o: /usr/include/bits/wchar.h src/include/Condacto.h
src/EntradaProcesos.o: src/include/Tokeniser.h src/include/Vocabulario.h
src/EntradaProcesos.o: src/include/PawPart.h src/include/prep/Diccionario.h
src/EntradaProcesos.o: src/include/defines.h src/include/PawCondacts.h
src/Excepcion.o: src/include/Excepcion.h
src/Fichero.o: src/include/Fichero.h src/include/MsgAplicacion.h
src/Fichero.o: src/include/Excepcion.h
src/Graficos.o: src/include/Graficos.h src/include/PawPart.h
src/Graficos.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Graficos.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Graficos.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Graficos.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Graficos.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Graficos.o: src/include/defines.h src/include/MsgAplicacion.h
src/Graficos.o: src/include/Excepcion.h
src/ListaEntradasVolcadas.o: src/include/ListaEntradasVolcadas.h
src/ListaEntradasVolcadas.o: src/include/EntradaProcesos.h
src/ListaEntradasVolcadas.o: src/include/SpBuffer.h /usr/include/stdint.h
src/ListaEntradasVolcadas.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/ListaEntradasVolcadas.o: /usr/include/bits/wordsize.h
src/ListaEntradasVolcadas.o: /usr/include/gnu/stubs.h
src/ListaEntradasVolcadas.o: /usr/include/gnu/stubs-32.h
src/ListaEntradasVolcadas.o: /usr/include/bits/wchar.h src/include/Condacto.h
src/ListaEntradasVolcadas.o: src/include/Tokeniser.h
src/ListaEntradasVolcadas.o: src/include/Vocabulario.h src/include/PawPart.h
src/ListaEntradasVolcadas.o: src/include/prep/Diccionario.h
src/ListaEntradasVolcadas.o: src/include/defines.h
src/ListaProcesos.o: src/include/ListaProcesos.h src/include/PawPart.h
src/ListaProcesos.o: src/include/SpBuffer.h /usr/include/stdint.h
src/ListaProcesos.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/ListaProcesos.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/ListaProcesos.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/ListaProcesos.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/ListaProcesos.o: src/include/Proceso.h src/include/EntradaProcesos.h
src/ListaProcesos.o: src/include/Condacto.h src/include/Vocabulario.h
src/ListaProcesos.o: src/include/defines.h src/include/MsgAplicacion.h
src/ListaProcesos.o: src/include/Excepcion.h
src/ListaProcesos.o: src/include/ListaEntradasVolcadas.h
src/ListaSimple.o: src/include/MsgAplicacion.h
src/PawCondacts.o: src/include/PawCondacts.h src/include/Condacto.h
src/PawCondacts.o: src/include/SpBuffer.h /usr/include/stdint.h
src/PawCondacts.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/PawCondacts.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/PawCondacts.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/PawCondacts.o: src/include/Tokeniser.h src/include/Vocabulario.h
src/PawCondacts.o: src/include/PawPart.h src/include/prep/Diccionario.h
src/PawCondacts.o: src/include/defines.h src/include/MsgAplicacion.h
src/PawCondacts.o: src/include/Excepcion.h
src/PawPart.o: src/include/PawPart.h src/include/SpBuffer.h
src/PawPart.o: /usr/include/stdint.h /usr/include/features.h
src/PawPart.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/PawPart.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/PawPart.o: /usr/include/bits/wchar.h src/include/Tokeniser.h
src/PawPart.o: src/include/prep/Diccionario.h src/include/Excepcion.h
src/Proceso.o: src/include/Proceso.h src/include/EntradaProcesos.h
src/Proceso.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Proceso.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Proceso.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Proceso.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Proceso.o: src/include/Condacto.h src/include/Tokeniser.h
src/Proceso.o: src/include/Vocabulario.h src/include/PawPart.h
src/Proceso.o: src/include/prep/Diccionario.h src/include/defines.h
src/SpBuffer.o: src/include/SpBuffer.h /usr/include/stdint.h
src/SpBuffer.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/SpBuffer.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/SpBuffer.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/SpBuffer.o: src/include/Excepcion.h src/include/MsgAplicacion.h
src/SpBuffer.o: src/include/UtilTexto.h src/include/prep/PrepAbreviaturas.h
src/SpBuffer.o: src/include/prep/PrepAbreviable.h src/include/Aventura.h
src/SpBuffer.o: src/include/PawPart.h src/include/Tokeniser.h
src/SpBuffer.o: src/include/prep/Diccionario.h src/include/defines.h
src/Textos.o: src/include/Textos.h src/include/PawPart.h
src/Textos.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Textos.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Textos.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Textos.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Textos.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Textos.o: src/include/defines.h src/include/MsgAplicacion.h
src/Textos.o: src/include/Excepcion.h src/include/UtilTexto.h
src/Textos.o: src/include/prep/PrepAbreviaturas.h
src/Textos.o: src/include/prep/PrepAbreviable.h src/include/Aventura.h
src/Tokeniser.o: src/include/Tokeniser.h src/include/SpBuffer.h
src/Tokeniser.o: /usr/include/stdint.h /usr/include/features.h
src/Tokeniser.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/Tokeniser.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/Tokeniser.o: /usr/include/bits/wchar.h src/include/MsgAplicacion.h
src/UtilTexto.o: src/include/UtilTexto.h src/include/prep/PrepAbreviaturas.h
src/UtilTexto.o: src/include/SpBuffer.h /usr/include/stdint.h
src/UtilTexto.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/UtilTexto.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/UtilTexto.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/UtilTexto.o: src/include/prep/PrepAbreviable.h src/include/Aventura.h
src/UtilTexto.o: src/include/PawPart.h src/include/Tokeniser.h
src/UtilTexto.o: src/include/prep/Diccionario.h src/include/defines.h
src/UtilTexto.o: src/include/prep/MsgPrep.h src/include/MsgAplicacion.h
src/UtilTexto.o: src/include/Excepcion.h
src/Vocabulario.o: src/include/Vocabulario.h src/include/PawPart.h
src/Vocabulario.o: src/include/SpBuffer.h /usr/include/stdint.h
src/Vocabulario.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/Vocabulario.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/Vocabulario.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/Vocabulario.o: src/include/Tokeniser.h src/include/prep/Diccionario.h
src/Vocabulario.o: src/include/defines.h src/include/MsgAplicacion.h
src/Vocabulario.o: src/include/Excepcion.h
src/main.o: src/include/Excepcion.h src/include/prep/Preprocesador.h
src/main.o: src/include/prep/PrepTokeniser.h src/include/Tokeniser.h
src/main.o: src/include/SpBuffer.h /usr/include/stdint.h
src/main.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/main.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/main.o: src/include/Aventura.h src/include/PawPart.h
src/main.o: src/include/prep/Diccionario.h src/include/defines.h
src/main.o: src/include/prep/Diccionario.h src/include/prep/PrepLocalidades.h
src/main.o: src/include/prep/PrepPart.h src/include/prep/PrepTokeniser.h
src/main.o: src/include/prep/Pool.h src/include/prep/PrepSubChar.h
src/main.o: src/include/prep/PrepPart.h src/include/prep/PrepAbreviable.h
src/main.o: src/include/prep/PrepVocabulario.h src/include/prep/Pool.h
src/main.o: src/include/prep/PrepObjetos.h src/include/MsgAplicacion.h
src/main.o: src/include/prep/PrepListaMensajes.h
src/main.o: src/include/prep/PrepAbreviable.h
src/main.o: src/include/prep/PrepListaDatos.h src/include/prep/PrepDefaults.h
src/main.o: src/include/prep/PrepProcesos.h
src/main.o: src/include/prep/PrepListaMensajes.h
src/main.o: src/include/prep/PrepVocabulario.h
src/main.o: src/include/prep/PrepEntradaMultiple.h src/include/Condacto.h
src/main.o: src/include/Vocabulario.h src/include/prep/PrepFlags.h
src/main.o: src/include/prep/PrepAbreviaturas.h
src/main.o: src/include/prep/PrepConstantes.h src/include/UtilTexto.h
src/main.o: src/include/prep/PrepAbreviaturas.h src/include/Fichero.h
src/prep/Diccionario.o: src/include/prep/Diccionario.h
src/prep/Diccionario.o: src/include/UtilTexto.h
src/prep/Diccionario.o: src/include/prep/PrepAbreviaturas.h
src/prep/Diccionario.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/Diccionario.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/Diccionario.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/prep/Diccionario.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/prep/Diccionario.o: src/include/prep/PrepAbreviable.h
src/prep/Diccionario.o: src/include/Aventura.h src/include/PawPart.h
src/prep/Diccionario.o: src/include/Tokeniser.h src/include/defines.h
src/prep/Diccionario.o: src/include/MsgAplicacion.h
src/prep/Error.o: src/include/prep/Error.h
src/prep/Pool.o: src/include/prep/Pool.h src/include/SpBuffer.h
src/prep/Pool.o: /usr/include/stdint.h /usr/include/features.h
src/prep/Pool.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/prep/Pool.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/prep/Pool.o: /usr/include/bits/wchar.h
src/prep/PrepAbreviable.o: src/include/prep/PrepAbreviable.h
src/prep/PrepAbreviable.o: src/include/UtilTexto.h
src/prep/PrepAbreviable.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepAbreviable.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepAbreviable.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepAbreviable.o: /usr/include/bits/wordsize.h
src/prep/PrepAbreviable.o: /usr/include/gnu/stubs.h
src/prep/PrepAbreviable.o: /usr/include/gnu/stubs-32.h
src/prep/PrepAbreviable.o: /usr/include/bits/wchar.h
src/prep/PrepAbreviable.o: src/include/prep/PrepAbreviable.h
src/prep/PrepAbreviable.o: src/include/Aventura.h src/include/PawPart.h
src/prep/PrepAbreviable.o: src/include/Tokeniser.h
src/prep/PrepAbreviable.o: src/include/prep/Diccionario.h
src/prep/PrepAbreviable.o: src/include/defines.h
src/prep/PrepAbreviaturas.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepAbreviaturas.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepAbreviaturas.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepAbreviaturas.o: /usr/include/bits/wordsize.h
src/prep/PrepAbreviaturas.o: /usr/include/gnu/stubs.h
src/prep/PrepAbreviaturas.o: /usr/include/gnu/stubs-32.h
src/prep/PrepAbreviaturas.o: /usr/include/bits/wchar.h
src/prep/PrepAbreviaturas.o: src/include/prep/PrepAbreviable.h
src/prep/PrepAbreviaturas.o: src/include/Aventura.h src/include/PawPart.h
src/prep/PrepAbreviaturas.o: src/include/Tokeniser.h
src/prep/PrepAbreviaturas.o: src/include/prep/Diccionario.h
src/prep/PrepAbreviaturas.o: src/include/defines.h src/include/Abreviaturas.h
src/prep/PrepAbreviaturas.o: src/include/UtilTexto.h
src/prep/PrepAbreviaturas.o: src/include/MsgAplicacion.h
src/prep/PrepConstantes.o: src/include/prep/PrepConstantes.h
src/prep/PrepConstantes.o: src/include/prep/PrepPart.h
src/prep/PrepConstantes.o: src/include/prep/MsgPrep.h
src/prep/PrepConstantes.o: src/include/prep/Error.h
src/prep/PrepConstantes.o: src/include/MsgAplicacion.h src/include/defines.h
src/prep/PrepConstantes.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepConstantes.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepConstantes.o: /usr/include/bits/wordsize.h
src/prep/PrepConstantes.o: /usr/include/gnu/stubs.h
src/prep/PrepConstantes.o: /usr/include/gnu/stubs-32.h
src/prep/PrepConstantes.o: /usr/include/bits/wchar.h
src/prep/PrepDefaults.o: src/include/prep/PrepDefaults.h
src/prep/PrepDefaults.o: src/include/prep/PrepPart.h src/include/prep/Error.h
src/prep/PrepDefaults.o: src/include/MsgAplicacion.h src/include/UtilTexto.h
src/prep/PrepDefaults.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepDefaults.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepDefaults.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepDefaults.o: /usr/include/bits/wordsize.h
src/prep/PrepDefaults.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/prep/PrepDefaults.o: /usr/include/bits/wchar.h
src/prep/PrepDefaults.o: src/include/prep/PrepAbreviable.h
src/prep/PrepDefaults.o: src/include/Aventura.h src/include/PawPart.h
src/prep/PrepDefaults.o: src/include/Tokeniser.h
src/prep/PrepDefaults.o: src/include/prep/Diccionario.h src/include/defines.h
src/prep/PrepEntradaMultiple.o: src/include/prep/PrepEntradaMultiple.h
src/prep/PrepEntradaMultiple.o: src/include/prep/PrepProcesos.h
src/prep/PrepEntradaMultiple.o: src/include/prep/PrepPart.h
src/prep/PrepEntradaMultiple.o: src/include/prep/PrepListaMensajes.h
src/prep/PrepEntradaMultiple.o: src/include/prep/PrepVocabulario.h
src/prep/PrepEntradaMultiple.o: src/include/Condacto.h src/include/SpBuffer.h
src/prep/PrepEntradaMultiple.o: /usr/include/stdint.h /usr/include/features.h
src/prep/PrepEntradaMultiple.o: /usr/include/sys/cdefs.h
src/prep/PrepEntradaMultiple.o: /usr/include/bits/wordsize.h
src/prep/PrepEntradaMultiple.o: /usr/include/gnu/stubs.h
src/prep/PrepEntradaMultiple.o: /usr/include/gnu/stubs-32.h
src/prep/PrepEntradaMultiple.o: /usr/include/bits/wchar.h
src/prep/PrepEntradaMultiple.o: src/include/Tokeniser.h
src/prep/PrepEntradaMultiple.o: src/include/Vocabulario.h
src/prep/PrepEntradaMultiple.o: src/include/PawPart.h
src/prep/PrepEntradaMultiple.o: src/include/prep/Diccionario.h
src/prep/PrepEntradaMultiple.o: src/include/defines.h
src/prep/PrepFlags.o: src/include/prep/PrepFlags.h
src/prep/PrepFlags.o: src/include/prep/PrepPart.h src/include/prep/MsgPrep.h
src/prep/PrepFlags.o: src/include/prep/Error.h src/include/MsgAplicacion.h
src/prep/PrepFlags.o: src/include/defines.h src/include/SpBuffer.h
src/prep/PrepFlags.o: /usr/include/stdint.h /usr/include/features.h
src/prep/PrepFlags.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/prep/PrepFlags.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/prep/PrepFlags.o: /usr/include/bits/wchar.h
src/prep/PrepListaDatos.o: src/include/prep/PrepListaDatos.h
src/prep/PrepListaDatos.o: src/include/prep/PrepPart.h
src/prep/PrepListaDatos.o: src/include/prep/MsgPrep.h
src/prep/PrepListaDatos.o: src/include/prep/Error.h
src/prep/PrepListaMensajes.o: src/include/prep/PrepListaMensajes.h
src/prep/PrepListaMensajes.o: src/include/prep/MsgPrep.h
src/prep/PrepListaMensajes.o: src/include/MsgAplicacion.h
src/prep/PrepListaMensajes.o: src/include/Textos.h src/include/PawPart.h
src/prep/PrepListaMensajes.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepListaMensajes.o: /usr/include/features.h
src/prep/PrepListaMensajes.o: /usr/include/sys/cdefs.h
src/prep/PrepListaMensajes.o: /usr/include/bits/wordsize.h
src/prep/PrepListaMensajes.o: /usr/include/gnu/stubs.h
src/prep/PrepListaMensajes.o: /usr/include/gnu/stubs-32.h
src/prep/PrepListaMensajes.o: /usr/include/bits/wchar.h
src/prep/PrepListaMensajes.o: src/include/Tokeniser.h
src/prep/PrepListaMensajes.o: src/include/prep/Diccionario.h
src/prep/PrepListaMensajes.o: src/include/defines.h src/include/prep/Error.h
src/prep/PrepListaMensajes.o: src/include/UtilTexto.h
src/prep/PrepListaMensajes.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepListaMensajes.o: src/include/prep/PrepAbreviable.h
src/prep/PrepListaMensajes.o: src/include/Aventura.h
src/prep/PrepLocalidades.o: src/include/prep/PrepLocalidades.h
src/prep/PrepLocalidades.o: src/include/prep/PrepPart.h
src/prep/PrepLocalidades.o: src/include/Aventura.h src/include/PawPart.h
src/prep/PrepLocalidades.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepLocalidades.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepLocalidades.o: /usr/include/bits/wordsize.h
src/prep/PrepLocalidades.o: /usr/include/gnu/stubs.h
src/prep/PrepLocalidades.o: /usr/include/gnu/stubs-32.h
src/prep/PrepLocalidades.o: /usr/include/bits/wchar.h src/include/Tokeniser.h
src/prep/PrepLocalidades.o: src/include/prep/Diccionario.h
src/prep/PrepLocalidades.o: src/include/defines.h
src/prep/PrepLocalidades.o: src/include/prep/Diccionario.h
src/prep/PrepLocalidades.o: src/include/prep/PrepTokeniser.h
src/prep/PrepLocalidades.o: src/include/prep/Pool.h
src/prep/PrepLocalidades.o: src/include/prep/PrepSubChar.h
src/prep/PrepLocalidades.o: src/include/prep/PrepPart.h
src/prep/PrepLocalidades.o: src/include/prep/PrepAbreviable.h
src/prep/PrepLocalidades.o: src/include/prep/MsgPrep.h
src/prep/PrepLocalidades.o: src/include/UtilTexto.h
src/prep/PrepLocalidades.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepLocalidades.o: src/include/prep/Error.h src/include/Textos.h
src/prep/PrepLocalidades.o: src/include/Conexiones.h
src/prep/PrepLocalidades.o: src/include/Vocabulario.h
src/prep/PrepLocalidades.o: src/include/MsgAplicacion.h
src/prep/PrepObjetos.o: src/include/prep/PrepObjetos.h
src/prep/PrepObjetos.o: src/include/prep/PrepPart.h src/include/Aventura.h
src/prep/PrepObjetos.o: src/include/PawPart.h src/include/SpBuffer.h
src/prep/PrepObjetos.o: /usr/include/stdint.h /usr/include/features.h
src/prep/PrepObjetos.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/prep/PrepObjetos.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/prep/PrepObjetos.o: /usr/include/bits/wchar.h src/include/Tokeniser.h
src/prep/PrepObjetos.o: src/include/prep/Diccionario.h src/include/defines.h
src/prep/PrepObjetos.o: src/include/prep/Diccionario.h
src/prep/PrepObjetos.o: src/include/prep/PrepTokeniser.h
src/prep/PrepObjetos.o: src/include/prep/Pool.h
src/prep/PrepObjetos.o: src/include/prep/PrepSubChar.h
src/prep/PrepObjetos.o: src/include/prep/PrepPart.h
src/prep/PrepObjetos.o: src/include/prep/PrepVocabulario.h
src/prep/PrepObjetos.o: src/include/prep/Pool.h src/include/MsgAplicacion.h
src/prep/PrepObjetos.o: src/include/prep/PrepAbreviable.h
src/prep/PrepObjetos.o: src/include/prep/MsgPrep.h src/include/prep/Error.h
src/prep/PrepObjetos.o: src/include/UtilTexto.h
src/prep/PrepObjetos.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepObjetos.o: src/include/Textos.h src/include/Vocabulario.h
src/prep/PrepObjetos.o: src/include/ListaSimple.h src/include/Excepcion.h
src/prep/PrepObjetos.o: src/include/ListaSimpleImpl.h
src/prep/PrepPart.o: src/include/prep/PrepPart.h src/include/prep/Error.h
src/prep/PrepPart.o: src/include/prep/MsgPrep.h src/include/UtilTexto.h
src/prep/PrepPart.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepPart.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepPart.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepPart.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/prep/PrepPart.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/prep/PrepPart.o: src/include/prep/PrepAbreviable.h src/include/Aventura.h
src/prep/PrepPart.o: src/include/PawPart.h src/include/Tokeniser.h
src/prep/PrepPart.o: src/include/prep/Diccionario.h src/include/defines.h
src/prep/PrepProcesos.o: src/include/prep/PrepProcesos.h
src/prep/PrepProcesos.o: src/include/prep/PrepPart.h
src/prep/PrepProcesos.o: src/include/prep/PrepListaMensajes.h
src/prep/PrepProcesos.o: src/include/prep/PrepVocabulario.h
src/prep/PrepProcesos.o: src/include/prep/PrepEntradaMultiple.h
src/prep/PrepProcesos.o: src/include/Condacto.h src/include/SpBuffer.h
src/prep/PrepProcesos.o: /usr/include/stdint.h /usr/include/features.h
src/prep/PrepProcesos.o: /usr/include/sys/cdefs.h
src/prep/PrepProcesos.o: /usr/include/bits/wordsize.h
src/prep/PrepProcesos.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/prep/PrepProcesos.o: /usr/include/bits/wchar.h src/include/Tokeniser.h
src/prep/PrepProcesos.o: src/include/Vocabulario.h src/include/PawPart.h
src/prep/PrepProcesos.o: src/include/prep/Diccionario.h src/include/defines.h
src/prep/PrepProcesos.o: src/include/prep/MsgPrep.h src/include/prep/Error.h
src/prep/PrepProcesos.o: src/include/MsgAplicacion.h
src/prep/PrepProcesos.o: src/include/ListaProcesos.h src/include/Proceso.h
src/prep/PrepProcesos.o: src/include/EntradaProcesos.h
src/prep/PrepProcesos.o: src/include/PawCondacts.h src/include/UtilTexto.h
src/prep/PrepProcesos.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepProcesos.o: src/include/prep/PrepAbreviable.h
src/prep/PrepProcesos.o: src/include/Aventura.h
src/prep/PrepSubChar.o: src/include/prep/PrepSubChar.h
src/prep/PrepSubChar.o: src/include/prep/PrepPart.h
src/prep/PrepSubChar.o: src/include/prep/MsgPrep.h src/include/prep/Error.h
src/prep/PrepSubChar.o: src/include/UtilTexto.h
src/prep/PrepSubChar.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepSubChar.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepSubChar.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepSubChar.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/prep/PrepSubChar.o: /usr/include/gnu/stubs-32.h /usr/include/bits/wchar.h
src/prep/PrepSubChar.o: src/include/prep/PrepAbreviable.h
src/prep/PrepSubChar.o: src/include/Aventura.h src/include/PawPart.h
src/prep/PrepSubChar.o: src/include/Tokeniser.h
src/prep/PrepSubChar.o: src/include/prep/Diccionario.h src/include/defines.h
src/prep/PrepSubChar.o: src/include/MsgAplicacion.h
src/prep/PrepTokeniser.o: src/include/prep/PrepTokeniser.h
src/prep/PrepTokeniser.o: src/include/MsgAplicacion.h
src/prep/PrepTokeniser.o: src/include/prep/MsgPrep.h src/include/Excepcion.h
src/prep/PrepTokeniser.o: src/include/UtilTexto.h
src/prep/PrepTokeniser.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepTokeniser.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepTokeniser.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepTokeniser.o: /usr/include/bits/wordsize.h
src/prep/PrepTokeniser.o: /usr/include/gnu/stubs.h
src/prep/PrepTokeniser.o: /usr/include/gnu/stubs-32.h
src/prep/PrepTokeniser.o: /usr/include/bits/wchar.h
src/prep/PrepTokeniser.o: src/include/prep/PrepAbreviable.h
src/prep/PrepTokeniser.o: src/include/Aventura.h src/include/PawPart.h
src/prep/PrepTokeniser.o: src/include/Tokeniser.h
src/prep/PrepTokeniser.o: src/include/prep/Diccionario.h
src/prep/PrepTokeniser.o: src/include/defines.h
src/prep/PrepUtilTexto.o: src/include/prep/PrepUtilTexto.h
src/prep/PrepUtilTexto.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepUtilTexto.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepUtilTexto.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepUtilTexto.o: /usr/include/bits/wordsize.h
src/prep/PrepUtilTexto.o: /usr/include/gnu/stubs.h
src/prep/PrepUtilTexto.o: /usr/include/gnu/stubs-32.h
src/prep/PrepUtilTexto.o: /usr/include/bits/wchar.h
src/prep/PrepUtilTexto.o: src/include/prep/PrepAbreviable.h
src/prep/PrepUtilTexto.o: src/include/Aventura.h src/include/PawPart.h
src/prep/PrepUtilTexto.o: src/include/Tokeniser.h
src/prep/PrepUtilTexto.o: src/include/prep/Diccionario.h
src/prep/PrepUtilTexto.o: src/include/defines.h src/include/prep/MsgPrep.h
src/prep/PrepVocabulario.o: src/include/prep/PrepVocabulario.h
src/prep/PrepVocabulario.o: src/include/prep/Error.h
src/prep/PrepVocabulario.o: src/include/prep/MsgPrep.h
src/prep/PrepVocabulario.o: src/include/Vocabulario.h src/include/PawPart.h
src/prep/PrepVocabulario.o: src/include/SpBuffer.h /usr/include/stdint.h
src/prep/PrepVocabulario.o: /usr/include/features.h /usr/include/sys/cdefs.h
src/prep/PrepVocabulario.o: /usr/include/bits/wordsize.h
src/prep/PrepVocabulario.o: /usr/include/gnu/stubs.h
src/prep/PrepVocabulario.o: /usr/include/gnu/stubs-32.h
src/prep/PrepVocabulario.o: /usr/include/bits/wchar.h src/include/Tokeniser.h
src/prep/PrepVocabulario.o: src/include/prep/Diccionario.h
src/prep/PrepVocabulario.o: src/include/defines.h src/include/MsgAplicacion.h
src/prep/PrepVocabulario.o: src/include/UtilTexto.h
src/prep/PrepVocabulario.o: src/include/prep/PrepAbreviaturas.h
src/prep/PrepVocabulario.o: src/include/prep/PrepAbreviable.h
src/prep/PrepVocabulario.o: src/include/Aventura.h
src/prep/Preprocesador.o: src/include/prep/Preprocesador.h
src/prep/Preprocesador.o: src/include/prep/PrepTokeniser.h
src/prep/Preprocesador.o: src/include/Tokeniser.h src/include/SpBuffer.h
src/prep/Preprocesador.o: /usr/include/stdint.h /usr/include/features.h
src/prep/Preprocesador.o: /usr/include/sys/cdefs.h
src/prep/Preprocesador.o: /usr/include/bits/wordsize.h
src/prep/Preprocesador.o: /usr/include/gnu/stubs.h
src/prep/Preprocesador.o: /usr/include/gnu/stubs-32.h
src/prep/Preprocesador.o: /usr/include/bits/wchar.h src/include/Aventura.h
src/prep/Preprocesador.o: src/include/PawPart.h
src/prep/Preprocesador.o: src/include/prep/Diccionario.h
src/prep/Preprocesador.o: src/include/defines.h
src/prep/Preprocesador.o: src/include/prep/Diccionario.h
src/prep/Preprocesador.o: src/include/prep/PrepLocalidades.h
src/prep/Preprocesador.o: src/include/prep/PrepPart.h
src/prep/Preprocesador.o: src/include/prep/PrepTokeniser.h
src/prep/Preprocesador.o: src/include/prep/Pool.h
src/prep/Preprocesador.o: src/include/prep/PrepSubChar.h
src/prep/Preprocesador.o: src/include/prep/PrepPart.h
src/prep/Preprocesador.o: src/include/prep/PrepAbreviable.h
src/prep/Preprocesador.o: src/include/prep/PrepVocabulario.h
src/prep/Preprocesador.o: src/include/prep/Pool.h
src/prep/Preprocesador.o: src/include/prep/PrepObjetos.h
src/prep/Preprocesador.o: src/include/MsgAplicacion.h
src/prep/Preprocesador.o: src/include/prep/PrepListaMensajes.h
src/prep/Preprocesador.o: src/include/prep/PrepAbreviable.h
src/prep/Preprocesador.o: src/include/prep/PrepListaDatos.h
src/prep/Preprocesador.o: src/include/prep/PrepDefaults.h
src/prep/Preprocesador.o: src/include/prep/PrepProcesos.h
src/prep/Preprocesador.o: src/include/prep/PrepListaMensajes.h
src/prep/Preprocesador.o: src/include/prep/PrepVocabulario.h
src/prep/Preprocesador.o: src/include/prep/PrepEntradaMultiple.h
src/prep/Preprocesador.o: src/include/Condacto.h src/include/Vocabulario.h
src/prep/Preprocesador.o: src/include/prep/PrepFlags.h
src/prep/Preprocesador.o: src/include/prep/PrepAbreviaturas.h
src/prep/Preprocesador.o: src/include/prep/PrepConstantes.h
src/prep/Preprocesador.o: src/include/prep/MsgPrep.h src/include/prep/Error.h
src/prep/Preprocesador.o: src/include/UtilTexto.h
src/prep/Preprocesador.o: src/include/prep/PrepAbreviaturas.h
