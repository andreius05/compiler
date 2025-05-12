#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>






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













int isoperator(char c) {
     if (c=='='){
          return 1;
     }
     else if (c=='+'){
          return 1;
     }
     else if (c=='-'){
          return 1;
     }
     else if (c=='*'){
          return 1;
     }
     else if (c=='/'){
          return 1;
     }
     else if (c=='!'){
          return 1;
     }
     else if (c == '>'){
          return 1;
     }
     else if (c=='<'){
          return 1;
     }
     else if (c =='%'){
          return 1;
     }
     else if (c == '|')
          return 1;
     else if (c=='&')
          return 1;
     return 0;
 }




 





 int is_specific_symbol(char c) {
     return (c == '(' || c == ')' ||
             c == '{' || c == '}' ||
             c == '[' || c == ']' ||
             c == ';');
 }
 



 
 int isdoubleoperator(const char *s) {
     return strcmp(s, "==") == 0 || strcmp(s, "!=") == 0 ||
            strcmp(s, "<=") == 0 || strcmp(s, ">=") == 0;
 }


void error_call(error_t * error){
     if (error==NULL)return;
     if (error->typeOfError==SyntaxError){
          printf("PROBLEM was in [%d] line this problem conected with SyntaxError pls check youtself)\n",error->line);
          free(error);
          return;
     }
     if (error->typeOfError==SemiColonError){
          printf("PROBLEM was in [%d] line this problem conected with SEMICOLONS pls check youtself)\n",error->line);
          free(error);
          return;
     }
     //Here will some other cases
}


#if 1




Token_t * generate_symbol(char current,FILE * file){
     Token_t * token = malloc(sizeof(Token_t));
     token->typeOf.symbol = malloc(sizeof(symbol_t));
     if (current=='('){
          token->type = SYMBOL;
          token->typeOf.symbol->symbol = lPAREN;
          return token;
     }
     else if (current==')'){
          token->type = SYMBOL;
          token->typeOf.symbol->symbol = rPAREN;
          return token;
     }
     else if (current=='{'){
          token->type = SYMBOL;
          token->typeOf.symbol->symbol = lBRACKET;
          return token;
     }
     else if (current=='}'){
          token->type = SYMBOL;
          token->typeOf.symbol->symbol = rBRACKET;
          return token;
     }
     else if (current=='['){
          token->type = SYMBOL;
          token->typeOf.symbol->symbol = lKwSkob;
          return token;
     }
     else if (current==']'){
          token->type = SYMBOL;
          token->typeOf.symbol->symbol = rKwSkob;
          return token;
     }
     else if (current==';'){
          token->type = SYMBOL;
          token->typeOf.symbol->symbol = SEMICOLON;
          return token;
     }
     else {
          free(token);
          return NULL;
     }
}


Token_t * generate_operator(char current,FILE * file){
     Token_t * token = malloc(sizeof(Token_t));
     token->type = OPERATOR;
     token->typeOf.operator = malloc(sizeof(operator_t));
     char tmp = current;
     //Сначала обработка обычных операторов
     if (current =='+'){
          // mb poptom
          token->typeOf.operator->typeOfOperator = PLUS;
          return token;
     }
     else if (current =='='){
          current = fgetc(file);
          if (current == '='){
               token->typeOf.operator->typeOfOperator = COMPARE;
               return token;
          }
          ungetc(current,file);
          token->typeOf.operator->typeOfOperator = ASSIGN;
          return token;
     }
     else if (current =='-'){
          token->typeOf.operator->typeOfOperator = MINUS;
          return token;
     }
     else if (current =='/'){
          token->typeOf.operator->typeOfOperator = DIVIDE;
          return token;
     }
     else if (current =='*'){
          token->typeOf.operator->typeOfOperator = MULTIPLY;
          return token;
     }
     else if (current =='<'){
          current = fgetc(file);
          if (current == '='){
               token->typeOf.operator->typeOfOperator = LESSorEQUALS;
               return token;
          }
          ungetc(current,file);
          token->typeOf.operator->typeOfOperator = LESS;
          return token;
     }
     else if (current =='>'){
          current = fgetc(file);
          if (current == '='){
               token->typeOf.operator->typeOfOperator = MOREorEQUALS;
               return token;
          }
          ungetc(current,file);
          token->typeOf.operator->typeOfOperator = MORE;
          return token;
     }
     else if (current =='%'){
          token->typeOf.operator->typeOfOperator = MODULE;
          return token;
     }
     else if (current =='!'){
          current = fgetc(file);
          if (current == '='){
               token->typeOf.operator->typeOfOperator = NotEQUALS;
               return token;
          }
          ungetc(current,file);
          free(token->typeOf.operator);
          free(token);
          return NULL;
     }
     else if (current=='&'){
          current = fgetc(file);
          if (current=='&'){
               token->typeOf.operator->typeOfOperator=AND;
               return token;
          }
          if (current != EOF)
               ungetc(current, file);
          printf( "Unexpected '&' without pair '&'\n");
          free(token->typeOf.operator);
          free(token);
          return NULL;

     }
     else if (current=='|'){
          current = fgetc(file);
          if (current=='|'){
               token->typeOf.operator->typeOfOperator=OR;
               return token;
          }
          if (current != EOF)
               ungetc(current, file);
          printf( "Unexpected '|' without pair '|'\n");
          free(token->typeOf.operator);
          free(token);
          return NULL;
     }
     free(token->typeOf.operator);
     free(token);
     return NULL;
}
void free_token(Token_t *tok) {
     if (!tok) return;
 
     switch (tok->type) {
         case DATA:
             if (tok->typeOf.data) {
                 free(tok->typeOf.data->value);
                 free(tok->typeOf.data);
             }
             break;
 
         case KEYWORD:
             if (tok->typeOf.keyword) {
                 free(tok->typeOf.keyword->value);
                 free(tok->typeOf.keyword);
             }
             break;
 
         case OPERATOR:
             if (tok->typeOf.operator) {
                 free(tok->typeOf.operator);
             }
             break;
 
         case SYMBOL:
             if (tok->typeOf.symbol) {
                 free(tok->typeOf.symbol);
             }
             break;
 
         case IDENTIFIER:
             if (tok->typeOf.identifier) {
                 free(tok->typeOf.identifier->name);
                 free(tok->typeOf.identifier);
             }
             break;
 
         default:
             printf("Unknown token type\n");
             break;
     }
 
     free(tok);
 }
 

Token_t * generate_keyword(char current,FILE * file){
     Token_t * token = malloc(sizeof(Token_t));
     char * keyword = malloc(64);//MAX KEYWORD LEN NAMEW;
     int index = 0;
     while (isalpha(current)&&current!=EOF){
          if (index>=63){
               printf("TOO BIG KEYWORD\n");
               free(token);
               free(keyword);
               return NULL;
          }
	     if (!isalpha(current)){
	       break;
	     }
           keyword[index] = current;
	     index+=1;
	     current = fgetc(file);
     }
     keyword[index] = '\0';

     
     //HERE are CASES

      if ((strcmp(keyword,"var")==0)){
          token->type = KEYWORD;
          token->typeOf.keyword = malloc(sizeof(keyword_t));
          token->typeOf.keyword->cmd = VAR;
          free(keyword);
          char * nameOFvar = malloc(64);
          int index = 0;
          current = fgetc(file);
          while (current==' '){
               current = fgetc(file);
          }
          if (current=='_' || isalpha(current)){
               while (isalpha(current) || isdigit(current )|| current=='_'){
                    if (index>63){
                         printf("So big name of keyword");
                         abort();
                    }
                    nameOFvar[index] = current;
                    index+=1;
                    current = fgetc(file);
               }
               nameOFvar[index] = '\0';

               token->typeOf.keyword->value = (char * )nameOFvar;
          }
          else{
               printf("Something goes wrong with your variable\n");
               ungetc(current, file);
               return NULL;
          }
          ungetc(current, file);
          return token;
     }
     //IF STATEMANT
     else if ((strcmp(keyword,"if")==0)){
          token->type = KEYWORD;
          token->typeOf.keyword = malloc(sizeof(keyword_t));
          token->typeOf.keyword->cmd = IF;
          token->typeOf.keyword->value = strdup("Just if statemant");
          free(keyword);
          ungetc(current, file);
          return token;
     }
     //ELIF STATEMANT
     else if ((strcmp(keyword,"elif")==0)){
          token->type = KEYWORD;
          token->typeOf.keyword = malloc(sizeof(keyword_t));
          token->typeOf.keyword->cmd = ELIF;
          token->typeOf.keyword->value = strdup("ELIF STATEMNET");
          free(keyword);
          ungetc(current, file);
          return token;
     }
     //ELSE STATEMANT
     else if ((strcmp(keyword,"else")==0)){
          token->type = KEYWORD;
          token->typeOf.keyword = malloc(sizeof(keyword_t));
          token->typeOf.keyword->cmd = ELSE;
          token->typeOf.keyword->value = strdup("ELSE STATEMNET");
          free(keyword);
          ungetc(current, file);
          return token;
     }
     else if ((strcmp(keyword,"return")==0)){
          token->type = KEYWORD;
          token->typeOf.keyword = malloc(sizeof(keyword_t));
          token->typeOf.keyword->cmd = RETURN;
          token->typeOf.keyword->value = strdup("return STATEMNET");
          free(keyword);
          ungetc(current, file);
          return token;
     }
     else if ((strcmp(keyword,"for")==0)){
          token->type = KEYWORD;
          token->typeOf.keyword = malloc(sizeof(keyword_t));
          token->typeOf.keyword->cmd = FOR;
          token->typeOf.keyword->value = strdup("FOR LOOP");
          free(keyword);
          ungetc(current, file);
          return token;
     }
     else if ((strcmp(keyword,"while")==0)){
          token->type = KEYWORD;
          token->typeOf.keyword = malloc(sizeof(keyword_t));
          token->typeOf.keyword->cmd = WHILE;
          token->typeOf.keyword->value = strdup("WHILE LOOp");
          free(keyword);
          ungetc(current, file);
          return token;
     }
     else {
          token->type = IDENTIFIER;
          token->typeOf.identifier = malloc(sizeof(ident_t));
          token->typeOf.identifier->type =UNKNOWNnow;
          token->typeOf.identifier->name = strdup(keyword);
          free(keyword);
          ungetc(current, file); 
          return token;
     }
     
     free(token);
     free(keyword);
     ungetc(current, file);
     return NULL;
}


Token_t * generate_string(char current,FILE * file){
     Token_t * str_token = malloc(sizeof(Token_t));
     int index = 0;
     char * buffer = malloc(256);
     current = fgetc(file);
     while(current!='"' && current!=EOF){
          if (index>254){
               printf("STR is too big man\n");
               free(str_token);
               free(buffer);
               return NULL;
          }
          buffer[index] = current;
          index+=1;
          current = fgetc(file);
     }
     if (current!='"'){
          free(str_token);
          free(buffer);
          return NULL;
     }
     buffer[index]='\0';
     str_token->type = DATA;
     str_token->typeOf.data = malloc(sizeof(data_t));
     str_token->typeOf.data->typeOfData = STRING;
     str_token->typeOf.data->value = (char * )buffer;
     return str_token;
}







Token_t * generate_number(char current,FILE * file){
     Token_t * token = malloc(sizeof(Token_t));
     int is_float = 0;
     int index = 0;
     char buffer [64] = {0};
     
     while (current!=EOF && (isdigit(current)||current=='.')){
          if (current=='.'){
               if (is_float){
                    break;
               }
               is_float = 1;
          }
           buffer[index++] = current;
	      current = fgetc(file);
     }
     if (current != EOF) {
          ungetc(current, file);
     }
     if (!is_float){
          token->type = DATA;
          int * value = malloc(sizeof(int));
          *value = atoi(buffer);
          token->typeOf.data = malloc(sizeof(data_t));
          token->typeOf.data->typeOfData = INT;
          token->typeOf.data->value = (int * )value;
          
     }
     else{
          token->type = DATA;
          int * value = malloc(sizeof(int));
          *value = atoi(buffer);
          token->typeOf.data = malloc(sizeof(data_t));
          token->typeOf.data->typeOfData = FLOAT;
          token->typeOf.data->value = (float * )value;
          
     }
     return token;
}

void print_token(Token_t * tok){
     

     if (tok->type == DATA){
          if (tok->typeOf.data->typeOfData == INT){
               printf("Found an INT [%d]\n",*(int * )tok->typeOf.data->value);
               return;
          }
          else if (tok->typeOf.data->typeOfData == STRING){
               printf("Found an a STR[%s]\n",(char *)tok->typeOf.data->value);
               return;
          }
          else if (tok->typeOf.data->typeOfData == FLOAT){
               printf("Found an FLOAT [%f]\n",*(float*)tok->typeOf.data->value);
               return;
          }
          else if (tok->typeOf.data->typeOfData==BOOL){
               printf("Found an BOOL []\n");
               return;
          }
          else {
               printf("DONT NOW THIS TYPE DROP\n");
               return;
          }
     }
     else if (tok->type==KEYWORD){
          if (tok->typeOf.keyword->cmd == IF){
               printf("Found if statemant\n");
          }
          else if (tok->typeOf.keyword->cmd == ELIF){
               printf("Found elif statemant\n");
          }
          else if (tok->typeOf.keyword->cmd == ELSE){
               printf("Found else statemant\n");
          }
          else if (tok->typeOf.keyword->cmd == WHILE){
               printf("Found while statemant\n");
          }
          else if (tok->typeOf.keyword->cmd == FOR){
               printf("Found for statemant\n");
          }
          else if (tok->typeOf.keyword->cmd == RETURN){
               printf("Found return statemant\n");
          }
          else if (tok->typeOf.keyword->cmd == VAR){
               printf("Found var name of this vatr is[%s]\n",(char *)tok->typeOf.keyword->value);
          }
     }
     else if (tok->type == OPERATOR){
          if (tok->typeOf.operator->typeOfOperator== COMPARE){
               printf("Found comparing \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== ASSIGN){
               printf("Found assiging \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== MINUS){
               printf("Found minus \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== DIVIDE){
               printf("Found dividing \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== MULTIPLY){
               printf("Found MULTIPLY \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== LESSorEQUALS){
               printf("Found LESSorEQ \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== LESS){
               printf("Found just less \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== MOREorEQUALS){
               printf("Found MOReorEQ \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== MORE){
               printf("Found more \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== MODULE){
               printf("Found module \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== NotEQUALS){
               printf("Found notEQ \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== AND){
               printf("AND \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== OR){
               printf("OR \n");
          }
     }
     else if (tok->type == SYMBOL){
          if (tok->typeOf.operator->typeOfOperator== lPAREN){
               printf("lPAREN  \n");
          }
           else if (tok->typeOf.operator->typeOfOperator== rPAREN){
               printf("Found rPAren \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== lBRACKET){
               printf("Found lbracket \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== rBRACKET){
               printf("Found rbracket \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== lKwSkob){
               printf("Found lKwskob \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== rKwSkob){
               printf("Found rKwskob \n");
          }
          else if (tok->typeOf.operator->typeOfOperator== SEMICOLON){
               printf("Found SEMICOLON \n");
          }
     }
     else if (tok->type == IDENTIFIER){
          printf("Found identifier  [%s]\n",(char *)tok->typeOf.identifier->name);
     }
     else {
          printf("IDK\n");
     }
}



TokenArray_t * lexer(FILE * file){
     if (file==NULL)return NULL;
     char current = fgetc(file);
     Token_t **tokens = malloc(sizeof(Token_t*) * 1000); // Условно 1000
     int token_count = 0;
     int index = 0;
     int semicolon = 0;
     int count_of_correct_str = 0;
     while(current!=EOF){
          semicolon = 0;
	  while(current!='\n'){
	  if (is_specific_symbol(current)){
          Token_t * token = generate_symbol(current,file);
          if (token->typeOf.symbol->symbol == lPAREN){
               tokens[token_count++] = token;
          }
          else if (token->typeOf.symbol->symbol == rPAREN){
               tokens[token_count++] = token;
          }
          else if (token->typeOf.symbol->symbol == lBRACKET){
               tokens[token_count++] = token;
          }
          else if (token->typeOf.symbol->symbol == rBRACKET){
               tokens[token_count++] = token;
          }
          else if (token->typeOf.symbol->symbol == lKwSkob){
               tokens[token_count++] = token;

          }
          else if (token->typeOf.symbol->symbol == rKwSkob){
               tokens[token_count++] = token;

          }
          else if (token->typeOf.symbol->symbol == SEMICOLON){
               tokens[token_count++] = token;

          }
       }
     else if (current=='#'){

     }
	  else if (isdigit(current)){
	       Token_t * test_token = generate_number(current,file);
            if (test_token->typeOf.data->typeOfData==INT){
               tokens[token_count++] = test_token;

            }
            if (test_token->typeOf.data->typeOfData==FLOAT){
               tokens[token_count++] = test_token;

            }
	       printf("\n");
	       
	       
	  }
       else if (isoperator(current)){
          char tmp = current;
          Token_t * operator_token = generate_operator(current,file);
          if (operator_token!=NULL){
          if (operator_token->typeOf.operator->typeOfOperator==ASSIGN){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==COMPARE){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==MINUS){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==PLUS){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==DIVIDE){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==MULTIPLY){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==LESS){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==LESSorEQUALS){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==MORE){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==MOREorEQUALS){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==NotEQUALS){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==AND){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==OR){
               tokens[token_count++] = operator_token;

          }
          else if (operator_token->typeOf.operator->typeOfOperator==MODULE){
               tokens[token_count++] = operator_token;

          }
     }
     else {
          printf("Someting goes wrong with operator\n");
     }
       }
       else if (current =='"'){
          count_of_correct_str+=1;
          Token_t * str_token = generate_string(current,file);
          if (str_token!=NULL){
               tokens[token_count++] = str_token;

          }
          else{
               error_t * error = malloc(sizeof(error_t));
               error->typeOfError = SyntaxError;
               error->line = index+1;
               error_call(error);
               free(tokens);
               free(error);
               return NULL;
          }
       }
	  else if (isalpha(current)){
	       Token_t * test_tok = generate_keyword(current,file);
            if (test_tok!=NULL){
               if (test_tok->type == KEYWORD){
                    if (test_tok->typeOf.keyword->cmd==VAR){
                         tokens[token_count++] = test_tok;

                    }
                    else if (test_tok->typeOf.keyword->cmd==IF || test_tok->typeOf.keyword->cmd == ELIF || test_tok->typeOf.keyword->cmd==ELSE){
                         tokens[token_count++] = test_tok;

                    }
                    else if (test_tok->typeOf.keyword->cmd == RETURN){
                         tokens[token_count++] = test_tok;

                    }
                    else if (test_tok->typeOf.keyword->cmd == FOR || test_tok->typeOf.keyword->cmd == WHILE){
                         tokens[token_count++] = test_tok;

                    }
               }
               
               else if (test_tok->type == IDENTIFIER){
                    tokens[token_count++] = test_tok;

               }
            }
            else{
               printf("Undefinded command \n");
            }
	       
	  }
	  //else{printf("%c",current);}
	  current = fgetc(file);
	  }
	  
	  
       if (current=='\n'){
          current = fgetc(file);
       }
     }
     printf("\n");
     TokenArray_t * token_arr = malloc(sizeof(TokenArray_t));
     token_arr->tokens = tokens;
     token_arr->count = token_count;
     return token_arr;
}
#endif


void free_token_array(TokenArray_t* token_arr) {
    if (!token_arr) return;
    for (int i = 0; i < token_arr->count; ++i) {
        free_token(token_arr->tokens[i]);
    }
    free(token_arr->tokens);
    free(token_arr);
}



