#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>



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
      INT ,STRING,BOOL,FLOAT
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



TokenKeyword_t * generate_keyword(char current,FILE * file){
     TokenKeyword_t * token = malloc(sizeof(TokenKeyword_t));
     char * keyword = malloc(64);//MAX KEYWORD LEN NAMEW;
     int index = 0;
     while (isalpha(current)&&current!=EOF){
	   if (!isalpha(current)){
	       break;
	   }
           keyword[index] = current;
	   index+=1;
	   current = fgetc(file);
     }
     printf("THIS IS COMMAND [%s] \n",keyword);
     //HERE are CASES
     if ((strcmp(keyword,"exit")==0)){
         token->command = EXIT;
	 token->value = keyword;
	 return token;
     }
     return NULL;

}


TokenLiteral_t * generate_number(char current,FILE * file){
     TokenLiteral_t * token = malloc(sizeof(TokenLiteral_t));
     token->type = INT;
     int  * value = malloc(sizeof(int));
     *value = 0;
     while (current!=EOF && isdigit(current)){
           if (!isdigit(current))break;
	   *value = *value * 10+(current-'0'); 
	   current = fgetc(file);
     }


     if (current != EOF) {
        ungetc(current, file);
    }

     //printf("AYBE THERE\n");
     printf("value is [%d]",*value);
     token->value = value;
     //printf("TEST\n");
     return token;
}


void lexer(FILE * file){
     if (file==NULL)return;
     char current = fgetc(file);
     int semicolon = 0;
     while(current!=EOF){
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
	       printf("TOKEN TYPE IS [INT]\n");
	       printf("AND VALUE IS %d",*(int * )test_token->value);
	       printf("\n");
	       free(test_token->value);
	       free(test_token);
	       
	  }
	  else if (isalpha(current)){
	       TokenKeyword_t * test_tok = generate_keyword(current,file);
	       printf("HERE IS COMMAND THAT I GOT [%s]\n",(char *)test_tok->value);
	  }
	  else{printf("%c",current);}
	  current = fgetc(file);
	  }
	  if (semicolon==0){
	      printf("Каждую линю закрывай ;\n");
	      return;
	  }
	  if (semicolon>1){
	      printf("Много слишком таких знаков [;]\n");
	      return;
	  }
	  current = fgetc(file);
     }
     printf("\n");
}


int main(){
    FILE * file;
    file = fopen("test.unn","r");
    lexer(file);
    fclose(file);
}
