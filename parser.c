#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"



typedef enum {
    AST_VAR_DECL,
    AST_IF_STMT,
    AST_WHILE_STMT,
    AST_FOR_STMT,
    AST_EXPR_STMT,
    AST_FUNCTION_DEF,
    AST_FUNCTION_CALL,
    AST_PRINT_STMT,
    AST_BINARY_EXPR,
    AST_LITERAL,
    AST_IDENTIFIER,
    AST_ASSIGNMENT,
    AST_RETURN,
    AST_BLOCK
} ASTNodeType;

typedef struct ASTNode ASTNode;

typedef struct {
    char* name;
    ASTNode* value;
} ASTVarDecl;


typedef struct {
    ASTNode* value; // Может быть NULL, если просто "return;"
} ASTReturn;

typedef struct {
    ASTNode* condition;
    ASTNode* thenBranch;
    ASTNode* elseBranch;
} ASTIf;

typedef struct {
    ASTNode* condition;
    ASTNode* body;
} ASTWhile;

typedef struct {
    char* var_name;
    ASTNode* iterable;
    ASTNode* body;
} ASTFor;

typedef struct {
    ASTNode* expression;
} ASTExprStmt;

typedef struct {
    ASTNode* value;
} ASTPrint;

typedef struct {
    char* op; // example: "+"
    ASTNode* left;
    ASTNode* right;
} ASTBinaryExpr;

typedef struct {
    data_t data;
} ASTLiteral;


typedef struct {
    char* name;
    char** params;
    int param_count;
    ASTNode* body; // Обычно блок
} ASTFunctionDef;

typedef struct {
    char* name;
    ASTNode** args;
    int arg_count;
} ASTFunctionCall;


typedef struct {
    char* name;
} ASTIdentifier;

typedef struct {
    ASTNode** statements;
    int count;
} ASTBlock;


typedef struct {
    TokenArray_t * token_arr;     // массив токенов
    int position;        // текущая позиция в массиве токенов
    int token_count;     // общее число токенов
} Parser_t;


typedef struct ASTNode {
    ASTNodeType type;
    union {
        ASTVarDecl varDecl;
        ASTIf ifStmt;
        ASTWhile whileStmt;
        ASTFor forStmt;
        ASTExprStmt exprStmt;
        ASTPrint printStmt;
        ASTBinaryExpr binary;
        ASTLiteral literal;
        ASTIdentifier identifier;
        ASTBlock block;
        ASTFunctionDef funcDef;
        ASTFunctionCall funcCall;
        ASTReturn returnStmt;
    } nodeType;
}ASTNode;


Token_t * peak (Parser_t * parser){
    if (parser->position>= parser->token_arr->count){
        return NULL;
    }
    return parser->token_arr->tokens[parser->position];
}


Token_t * advance(Parser_t * parser){
    if (parser->position<parser->token_arr->count){
        parser->position++;
    }
    return peak(parser);
}


int match (Parser_t * parser , TypeOfToken expected){
    Token_t * token = peek(parser);
    if (token && token->type == expected){
        advance(parser);
        return 1;
    }
    return 0;
}


Token_t * consume (Parser_t * parser , TypeOfToken expected,char * message){
    Token_t * token = peak(parser);
    if (token->type == expected){
        return advance(parser);
    }
    printf("ERROR [%s]\n",message);
    abort();
}


ASTNode * parseIdentifier(Parser_t * parser){
    Token_t * token = peak(parser);
    if (token && token->type == IDENTIFIER){
        ASTNode * node = malloc(sizeof(ASTNode));
        node->type = AST_IDENTIFIER;
        node->nodeType.identifier.name = strdup((char * )token->typeOf.identifier->name);
        advance(parser);
        return node;
    }
    printf("SOMETHING GOES WRONG WItH UR IDEntIFIER\n");
    abort();
}



ASTNode * parseBinaryExpr(Parser_t * parser){
    ASTNode * left = parseLiterral(parser);

    while (1){
        Token_t * token = peak(parser);
        if (!token || token->type!=OPERATOR)break;

        if (token->typeOf.operator->typeOfOperator == PLUS || token->typeOf.operator->typeOfOperator == MINUS){
            char * op = (token->typeOf.operator->typeOfOperator == PLUS) ? "+" : "-";
            advance(parser);

            ASTNode * right = parseLiterral(parser);
            ASTNode * expr = malloc(sizeof(ASTNode));
            expr->type = AST_BINARY_EXPR;
            expr->nodeType.binary.left = left;
            expr->nodeType.binary.right = right;
            expr->nodeType.binary.op = strdup((char *)op);

            left = expr;
        }
        else {
            break;
        }
    }
    return left;
}



ASTNode * parseWhile (Parser_t * parser){
    consume(parser,KEYWORD,"Expected while(");

    
}


ASTNode * parseLiterral (Parser_t * parser){
    Token_t * token = peak(parser);
    if (token->type == DATA){
        ASTNode * node = malloc(sizeof(ASTNode));
        node->type = AST_LITERAL;
        if (token->typeOf.data->typeOfData == INT){
            node->nodeType.literal.data.typeOfData = INT;
            int * val = malloc(sizeof(int));
            *val = *(int *)token->typeOf.data->value;
            node->nodeType.literal.data.value = val;
        }
        else if (token->typeOf.data->typeOfData == STRING){
            node->nodeType.literal.data.typeOfData = STRING;
            node->nodeType.literal.data.value = strdup((char *)token->typeOf.data->value);
        }
        else if (token->typeOf.data->typeOfData == FLOAT){
            node->nodeType.literal.data.typeOfData = FLOAT;
            float * val = malloc(sizeof(float));
            *val = *(float *)token->typeOf.data->value;
            node->nodeType.literal.data.value = (float * )val;
        }
        // NEED TO work with bool sstill , i remember about this shit(
        else {
            printf("Undefined literal type\n");
            abort();
        }
        advance(parser);
        return node;
    }
    printf("EXPECTED LITERRAL\n");
    abort();
}



int main(){

}