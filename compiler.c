/***
*       This C Compiler is implemented so that it can compile itself.
*       To do C language features used must be the most basic functionality.
*       So do not use features that would make this compiler source not compile.
*       This may mean not using string.h, malloc functions, etc.
***/
#include <stdio.h>
#include <stdlib.h>
//take away string.h before releasing
#define STRUCT_TOKEN_WIDTH 64

#define DOLLAR 100
#define INT 0
#define CHAR 1
#define FLOAT 2
#define DOUBLE 3
#define STRUCT 4
#define UNION 5
#define LONG 6
#define SHORT 7
#define UNSIGNED 8
#define AUTO 9
#define EXTERN 10
#define REGISTER 11
#define TYPEDEF 12
#define STATIC 13
#define GOTO 14
#define RETURN 15
#define SIZEOF 16
#define BREAK 17
#define CONTINUE 18
#define IF 19
#define ELSE 20
#define FOR 21
#define DO 22
#define WHILE 23
#define SWITCH 24
#define CASE 25
#define DEFAULT 26
#define ENTRY 27
#define ADD_OP 28
#define SUBTRACT_OP 29
#define MULT_OP 31

//UPDATE THIS so that it is 1 greater than the max of the tokenTypes
#define RESERVED_SYMBOL_LIMIT 32
struct _token{
        int sizeOfImage;
        int tokenType;
        int row;
        int column;
        char image[STRUCT_TOKEN_WIDTH];
};

typedef struct _token TokenStruct;

/**********     loop to tokenize source         *********/
char * reservedSymbols[] = {"int","char","float","double","struct","union","long","short",
                "unsigned","auto","extern","register","typedef","static","goto","return",
                "sizeof","break","continue","if","else","for","do","while","switch","case","default","entry",
                "+", "-", "*"};

TokenStruct next();
int hasNext();
char * sourcePointer;
int newLines, column;

int main(int argc, char* argv[] ){
        if(argc==1){
                printf("Usage: ./compiler source.c\n");
                return 0;
        }
        FILE *fp;
        fp = fopen(argv[1], "r");
        if(fp==NULL){
                fprintf(stderr, "No such file: %s\n", argv[1]);
                return 0;
        }
        fseek(fp,0L,SEEK_END);
        int size = ftell(fp);
        fseek(fp,0L,SEEK_SET);
        printf("size_t size=%d\n",size);
        
        /***      INIT        ***/
        //reservedSymbols[EOF-1][0]='\0';
        char source[size];
        sourcePointer = &(source[0]);
        newLines=0; column=0;
        
        /*****          Load characters from source into char array             ****/
        int fileInd;
        for(fileInd=0; fileInd<size; fileInd++){
                source[fileInd]=fgetc(fp);
        }
        TokenStruct ret;
        while(hasNext() ){
                ret = next();
                printf("tokenType=%d sizeOfImage=%d row=%d column=%d image=%s\n",ret.tokenType, ret.sizeOfImage, ret.row, ret.column, ret.image);
                if(strcmp(ret.image, "ERROR_SYMBOL_NOT_RECOGNIZED")==0)return 0;
        }
        
return 0;
}

TokenStruct next(){

        /*****     Remove whitespaces and increment newline and column counts      *****/
        while((*sourcePointer==' ')| (*sourcePointer=='\n') ){
                if(*sourcePointer==' '){
                        column++;
                }else if(*sourcePointer=='\n'){
                        column=0;
                        newLines++;
                }
                sourcePointer++;
        }
        TokenStruct result = {-9999, -9999, newLines+1, column, "ERROR_SYMBOL_NOT_RECOGNIZED"};

        /*********    set sourcePointer past the last character of a comment    **********/
        
        
        /*******      initialize function         ********/
        
        char * tryFloor;
        int symbolIndex;
        int longestWidth = -1;
        int longestToken = -99;
        
        
        /****   Tokenize for reserved symbols including operators   *****/
        for(symbolIndex=0; symbolIndex<RESERVED_SYMBOL_LIMIT; symbolIndex++){
                tryFloor = sourcePointer;
                int isMatched = 0;
                int charIndex = 0;
                while( (reservedSymbols[symbolIndex][charIndex]!='\0') & (*tryFloor==reservedSymbols[symbolIndex][charIndex]) ){
                        tryFloor++;
                        charIndex++;
                        if(reservedSymbols[symbolIndex][charIndex]=='\0'){
                                isMatched = 1;
                        }
                }
                if((isMatched==1) & (longestWidth<(tryFloor-sourcePointer) ) ){
                        longestWidth = tryFloor - sourcePointer;
                        longestToken = symbolIndex;
                }
        }
        
        /**  Tokenize for identifiers and literals     **/
        
        
        /*******      Generate token of longest match       ********/
        if(longestWidth>0){int copyIndex;
                for(copyIndex=0; copyIndex<longestWidth; copyIndex++){
                        result.image[copyIndex]=*sourcePointer;
                        sourcePointer++;
                }
                result.sizeOfImage = longestWidth;
                result.tokenType  = longestToken;
                result.row = newLines + 1;
                result.column = column;
                column+=longestWidth;
        }else{
                if(*sourcePointer=='\0'){
                        result.image[0]='D';
                        result.image[1]='O';
                        result.image[2]='L';
                        result.image[3]='L';
                        result.image[4]='A';
                        result.image[5]='R';
                        result.image[6]='\0';
                        result.sizeOfImage = 1;
                        result.tokenType = DOLLAR;
                        result.row = newLines+1;
                        result.column = column;
                        column+=1;
                }
        }
        result.image[longestWidth]='\0';
        
        
        return result;
}

int hasNext(){
        if(*sourcePointer=='\0'){
                return 0;
        }else{
                return 1;
        }
}
