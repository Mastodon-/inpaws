//---------------------------------------------------------------------------------------
//
// Permite realizar diversas acciones sobre los textos: validar códigos de control,
// sustituir abreviaturas, sustituir caracteres regionales, etc..
//---------------------------------------------------------------------------------------

#ifndef PREPUTILTEXTO_H_
#define PREPUTILTEXTO_H_

#include "prep/PrepAbreviaturas.h"
#include <string>
#include <map>

namespace Inpaws
{
    class PrepUtilTexto
    {
        public:
            enum { CODIGO, CARACTER, FIN, ERROR };
            struct Item
            {
                union { int codigo; char caracter; };
                int tipo;
            };

            PrepUtilTexto();
            ~PrepUtilTexto();

            bool textoCorrecto (const std::string& texto);
            const std::string& errorStr() const { return _errorStr; }; // Descripción del último error encontrado
            int expandirAbreviaturas (std::string& texto, const PrepAbreviaturas& abreviaturas);
            int sustituirCaracteres (std::string& texto, const std::map<char, std::string>& diccionario);

        private:
            std::string _errorStr;
            size_t _posactual;
            Item _siguienteItem(const std::string& texto);
            void _leerCodigo (Item& item, const std::string& texto);
    };
}

#endif
