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
                         printf("So big name of variable");
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
          token->typeOf.data->value = value;
          
     }
     else{
          token->type = DATA;
          int * value = malloc(sizeof(int));
          *value = atoi(buffer);
          token->typeOf.data = malloc(sizeof(data_t));
          token->typeOf.data->typeOfData = FLOAT;
          token->typeOf.data->value = value;
          
     }
     return token;
}


void lexer(FILE * file){
     if (file==NULL)return;
     char current = fgetc(file);
     int index = 0;
     int semicolon = 0;
     int count_of_correct_str = 0;
     while(current!=EOF){
          semicolon = 0;
	  while(current!='\n'){
	  if (is_specific_symbol(current)){
          Token_t * token = generate_symbol(current,file);
          if (token->typeOf.symbol->symbol == lPAREN){
               printf("Found ( this shit\n");
               free(token->typeOf.symbol);
               free(token);
          }
          else if (token->typeOf.symbol->symbol == rPAREN){
               printf("Found ) this shit\n");
               free(token->typeOf.symbol);
               free(token);
          }
          else if (token->typeOf.symbol->symbol == lBRACKET){
               printf("Found { this shit\n");
               free(token->typeOf.symbol);
               free(token);
          }
          else if (token->typeOf.symbol->symbol == rBRACKET){
               printf("Found } this shit\n");
               free(token->typeOf.symbol);
               free(token);
          }
          else if (token->typeOf.symbol->symbol == lKwSkob){
               printf("Found [ this shit\n");
               free(token->typeOf.symbol);
               free(token);
          }
          else if (token->typeOf.symbol->symbol == rKwSkob){
               printf("Found ] this shit\n");
               free(token->typeOf.symbol);
               free(token);
          }
          else if (token->typeOf.symbol->symbol == SEMICOLON){
               printf("Found ; this shit\n");
               free(token->typeOf.symbol);
               free(token);
          }
       }
     else if (current=='#'){

     }
	  else if (isdigit(current)){
	       Token_t * test_token = generate_number(current,file);
            if (test_token->typeOf.data->typeOfData==INT){
               printf("TOKEN TYPE IS [INT] ");
               printf(" AND VALUE IS %d\n",*(int * )test_token->typeOf.data->value);
            }
            if (test_token->typeOf.data->typeOfData==FLOAT){
               printf("TOKEN TYPE IS [FLOAT] ");
               printf(" AND VALUE IS %f\n",*(float * )test_token->typeOf.data->value);
            }
	       printf("\n");
	       free(test_token->typeOf.data->value);
	       free(test_token);
	       
	  }
       else if (isoperator(current)){
          char tmp = current;
          Token_t * operator_token = generate_operator(current,file);
          if (operator_token!=NULL){
          if (operator_token->typeOf.operator->typeOfOperator==ASSIGN){
               printf("EQUALS\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==COMPARE){
               printf("COMPARING\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==MINUS){
               printf("MINUS\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==PLUS){
               printf("PLUS\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==DIVIDE){
               printf("DIVIDE\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==MULTIPLY){
               printf("MULLTIPLY\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==LESS){
               printf("LESS\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==LESSorEQUALS){
               printf("LESS OR EQUALS\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==MORE){
               printf("MORE\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==MOREorEQUALS){
               printf("MORE OR EQUALSE\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==NotEQUALS){
               printf("NOT EQUALS\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==AND){
               printf("AND\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==OR){
               printf("OR\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
          }
          else if (operator_token->typeOf.operator->typeOfOperator==MODULE){
               printf("MODULE\n");
               free(operator_token->typeOf.operator);
               free(operator_token);
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
               printf("TYPE of TOKEN[STR] ");
               printf(" AND VALUE [%s]\n",(char*)str_token->typeOf.data->value);
               free(str_token->typeOf.data->value);
               free(str_token);
          }
          else{
               error_t * error = malloc(sizeof(error_t));
               error->typeOfError = SyntaxError;
               error->line = index+1;
               error_call(error);
               return;
          }
       }
	  else if (isalpha(current)){
	       Token_t * test_tok = generate_keyword(current,file);
            if (test_tok!=NULL){
               if (test_tok->type == KEYWORD){
                    if (test_tok->typeOf.keyword->cmd==VAR){
                         printf("Found variable with name [%s]\n",(char *)test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword);
                         free(test_tok);
                    }
                    else if (test_tok->typeOf.keyword->cmd==IF || test_tok->typeOf.keyword->cmd == ELIF || test_tok->typeOf.keyword->cmd==ELSE){
                         printf("Found ветвление [%s]\n",test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword);
                         free(test_tok);
                    }
                    else if (test_tok->typeOf.keyword->cmd == RETURN){
                         printf("Found return statemant[%s]\n",test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword);
                         free(test_tok);
                    }
                    else if (test_tok->typeOf.keyword->cmd == FOR || test_tok->typeOf.keyword->cmd == WHILE){
                         printf("Found  loop [%s]\n",test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword->value);
                         free(test_tok->typeOf.keyword);
                         free(test_tok);
                    }
               }
               
               else if (test_tok->type == IDENTIFIER){
                    printf("Found identifier [%s] \n",test_tok->typeOf.identifier->name);
                    free(test_tok->typeOf.identifier->name);
                    free(test_tok->typeOf.identifier);
                    free(test_tok);
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
}
#endif

int main(int argc , char **argv){
    FILE * file;
    file = fopen("test.unn","r");
    lexer(file);
    fclose(file);
}
