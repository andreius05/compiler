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
      VAR
}TokenKeywordCmd;


typedef enum types{
      INT ,STRING,BOOL,FLOAT,SEPARATOR
}TokenType;

typedef struct {
      TokenType type;
      void * value;
}TokenLiteral_t;


typedef struct {
      TokenKeywordCmd command;
      char * value;
}TokenKeyword_t;



typedef enum syntax_sugar{SEMICOLON=';',OPENBRACKET='(',CLOSEBRACKET=')'};



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
TokenKeyword_t * generate_keyword(char current,FILE * file){
     TokenKeyword_t * token = malloc(sizeof(TokenKeyword_t));
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
     if ((strcmp(keyword,"exit")==0)){
         token->command = EXIT;
	 token->value = keyword;
	 return token;
     }
     free(token);
     free(keyword);
     return NULL;
}


TokenLiteral_t * generate_string(char current,FILE * file){
     TokenLiteral_t * str_token = malloc(sizeof(TokenLiteral_t));
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
     str_token->type = STRING;
     printf("FOUND STR [%s]",buffer);
     str_token->value = (char *)buffer;
     return str_token;
}


TokenLiteral_t * generate_number(char current,FILE * file){
     TokenLiteral_t * token = malloc(sizeof(TokenLiteral_t));
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
          token->type = INT;
          int * value = malloc(sizeof(int));
          *value = atoi(buffer);
          token->value = value ;
          printf("This number (%d) is INT\n" , *value);
          
     }
     else{
          token->type = FLOAT;
          float * value = malloc(sizeof(float));
          * value = atof(buffer);
          token->value = value;
          printf("This number (%f) is FLOAT\n" , *value);
          
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
	  if (current==';'){
	      printf("Found SEMICOLON\n");
	      semicolon+=1;
	  }
	  else if (current=='('){
	       printf("Открыл скобку\n");

	  }
	  else if (current==')'){
	       printf("Закрыл скобку\n");
	  }
	  else if (isdigit(current)){
	       TokenLiteral_t * test_token = generate_number(current,file);
            if (test_token->type==INT){
               printf("TOKEN TYPE IS [INT]\n");
               printf("AND VALUE IS %d",*(int * )test_token->value);
            }
            if (test_token->type==FLOAT){
               printf("TOKEN TYPE IS [FLOAT]\n");
               printf("AND VALUE IS %f\n",*(float * )test_token->value);
            }
	       printf("\n");
	       free(test_token->value);
	       free(test_token);
	       
	  }
       else if (current =='"'){
          count_of_correct_str+=1;
          TokenLiteral_t * str_token = generate_string(current,file);
          if (str_token!=NULL){
               printf("TYPE of ELEMENT[STR]\n");
               printf("AND VALUE [%s]",(char*)str_token->value);
               free(str_token->value);
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
	       TokenKeyword_t * test_tok = generate_keyword(current,file);
            if (test_tok!=NULL){
               printf("HERE IS COMMAND THAT I GOT [%s]\n",(char *)test_tok->value);
               free(test_tok->value);
               free(test_tok);
            }
            else{
               printf("Undefinded command \n");
            }
	       
	  }
	  //else{printf("%c",current);}
	  current = fgetc(file);
	  }
       if (semicolon==0){
          error_t * error = malloc(sizeof(error_t));
          error->line = index	+1;
          error->typeOfError = SemiColonError;
          error_call(error);      
      }
      if (semicolon>1){
          error_t * error = malloc(sizeof(error_t));
          error->line = index	+1;
          error->typeOfError = SemiColonError;
          error_call(error); 
      }
       index+=1;
       printf("%d line\n",index);
	  
	  
       if (current=='\n'){
          current = fgetc(file);
       }
     }
     printf("\n");
}
#endif

int main(){
    FILE * file;
    file = fopen("test.unn","r");
    char current = '"';
    if (isalpha(current)){
        printf("%c is alpha\n",current);
    }
    else{
        printf("%c is not current\n",current);
    }
    lexer(file);
    fclose(file);
}
