#ifndef LEXER_H   /* Include guard */
#define LEXER_H

#include <stdio.h>  // обязательно для FILE*


typedef enum{
    SyntaxError,LogicalError,SemanticError,SemiColonError
}ErrorTypes;

typedef struct error{
    ErrorTypes typeOfError;
    int line;
}error_t;


typedef enum commands{
     EXIT,
     DEF,
     FOR,
     WHILE,
     IF,
     ELSE,
     ELIF,
     VAR,
     RETURN,
}TokenKeywordCmd;


typedef enum operators{
    MINUS,PLUS,DIVIDE,MULTIPLY,ASSIGN ,COMPARE,MORE,
    LESS,MOREorEQUALS,LESSorEQUALS,MODULE,NotEQUALS, AND,OR
}TokenOperatorsTypes;


typedef enum types{
     INT ,STRING,BOOL,FLOAT
}TokenType;

typedef enum {DATA,KEYWORD,OPERATOR,SYMBOL,IDENTIFIER,COMMENT,UNKNOWN}TypeOfToken;
typedef enum{VARIABLE,FUNCTION,UNKNOWNnow}TypeOfIdentifier;
typedef enum symbols{SEMICOLON=';',lPAREN='(',rPAREN = ')',lBRACKET='{',rBRACKET = '}',lKwSkob = '[',rKwSkob = ']'}TypeOfSymbol;

typedef struct keyword{
    TokenKeywordCmd cmd;
    char * value;
    //Тут ещё много работы в будующем
}keyword_t;


typedef struct data_t{
    TokenType typeOfData;
    void * value;
}data_t;

typedef struct ident_t{
    TypeOfIdentifier type;
     char * name;
    // void * value;
    // тут тоже мно8го работы
}ident_t;

typedef struct operator_t{
    TokenOperatorsTypes typeOfOperator;
    // Still working on it
}operator_t;


typedef struct symbol_t{
    TypeOfSymbol symbol;
}symbol_t;

typedef struct {
    TypeOfToken type;
    union typeOf
    {
         operator_t *operator;
         symbol_t * symbol;
         ident_t * identifier;
         data_t * data;
         keyword_t * keyword;
    }typeOf;   
}Token_t;



typedef struct TokenArray
{
    Token_t ** tokens;
    int count;
}TokenArray_t;


TokenArray_t * lexer(FILE * file);
void free_token_array(TokenArray_t *tok);
void print_token(Token_t * tok);


#endif 