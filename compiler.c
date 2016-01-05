/***
*       This C Compiler is implemented so that it can compile itself.
*       Only the most basic C language features must be used.
*       So do not use features that would make this compiler source not compile.
*       This may mean not using string.h, malloc functions, etc.
***/
#include <stdio.h>
#include <stdlib.h>
#define STRUCT_TOKEN_WIDTH 64
#define LEXER_ERROR -9999

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

#define INCREMENT 28
#define DECREMENT 29
#define L_PAREN 30
#define R_PAREN 31
#define L_SQ_BRACK 32
#define R_SQ_BRACK 33
#define PERIOD 34
#define RIGHT_ARROW 35
#define ADD_OP  36
#define SUBTRACT_OP 37 
#define LOGICAL_NOT 38
#define BITW_NOT 39
#define AMPERSAND 40
#define ASTR_MULT_OP 41
#define DIV_OP 42
#define MODULO 43
#define BITW_L_SHIFT 44
#define BITW_R_SHIFT 45

#define LESS 46
#define LESS_EQUAL 47
#define GREATER 48
#define GREATER_EQUAL 49
#define EQUAL 50
#define NOT_EQUAL 51
#define BITW_XOR 52
#define BITW_OR 53
#define LOGICAL_AND 54
#define LOGICAL_OR 55
#define QUESTION_MARK 56
#define COLON 57
#define ASSIGN_DIRECT 58
#define ASSIGN_BYSUM 59
#define ASSIGN_BYDIF 60
#define ASSIGN_BYPROD 61
#define ASSIGN_BYDIV 62
#define ASSIGN_BYMOD 63
#define ASSIGN_BY_BITW_LSHIFT 64
#define ASSIGN_BY_BITW_RSHIFT 65
#define ASSIGN_BY_BITW_AND 66
#define ASSIGN_BY_BITW_OR 67
#define SEMICOLON 68
#define COMMA 69
#define DOUBLE_QUOTE 70
#define SINGLE_QUOTE 71
//UPDATE THIS so that it is 1 greater than the max of the tokenTypes
#define RESERVED_SYMBOL_LIMIT 72

// The following tokenTypes are not in reserverSymbol
// and hence should reasonably unreachable by adjusting RESERVED_SYMBOL_LIMIT
#define LIT_DECIMAL 300

struct _token{
        int sizeOfImage;
        int tokenType;
        int row;
        int column;
        char image[STRUCT_TOKEN_WIDTH];
};

typedef struct _token TokenStruct;

/**********     string array for all reserved symbols including keywords and operators         *********/
const char * reservedSymbols[] = {"int","char","float","double","struct","union","long","short",
                "unsigned","auto","extern","register","typedef","static","goto","return",
                "sizeof","break","continue","if","else","for","do","while","switch","case","default","entry",
                "++", "--","(",")","[","]",".","->","+","-","!","~","&","*","/","%","<<",">>",
                "<","<=",">",">=","==","!=","^","|","&&","||","?",":","=","+=","-=","*=","/=","%=",
                "<<=",">>=","&=","|=",";",",","\"","'"};

TokenStruct next();
void tokenizeReservedSymbols(int *, int *);
void tokenizeDecimal(int *, int *);

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
                if(ret.tokenType==LEXER_ERROR)return 0;
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
        TokenStruct result = {LEXER_ERROR, LEXER_ERROR, newLines+1, column, "ERROR_SYMBOL_NOT_RECOGNIZED"};

        /*********    set sourcePointer past the last character of a comment    **********/
        
        
        /*******      initialize function         ********/
        int longestWidth = -1;
        int longestToken = -99;
        
        
        /**** Call tokenizing routines for each type of tokens *****/
        tokenizeReservedSymbols(&longestWidth, &longestToken);
        
        
        /*******      Generate token of longest match       ********/
        if(longestWidth>0){int copyIndex;
                for(copyIndex=0; copyIndex<longestWidth; copyIndex++){
                        result.image[copyIndex]=*sourcePointer;
                        sourcePointer++;
                }
                result.sizeOfImage = longestWidth;
                result.tokenType  = longestToken;
                result.row = newLines + 1;
                result.column = column + 1;
                column+=longestWidth;
        }
        result.image[longestWidth]='\0';
        
        
        return result;
}

void tokenizeDecimal(int * longestWidth, int * longestToken){

                /****   Tokenize for reserved symbols including operators   *****/
        char * tryFloor;
        tryFloor = sourcePointer;
        int isMatched = 0;
        while( ('0' <= *tryFloor) & (*tryFloor <= '9') ){
                tryFloor++;
                if( !(('0' <= *tryFloor) & (*tryFloor <= '9') ) ){
                        isMatched = 1;
                }
        }
        if((isMatched==1) & (*longestWidth<(tryFloor-sourcePointer) ) ){
                *longestWidth = tryFloor - sourcePointer;
                *longestToken = LIT_DECIMAL;
        }

}

void tokenizeReservedSymbols(int * longestWidth, int * longestToken){

                /****   Tokenize for reserved symbols including operators   *****/
        char * tryFloor;
        int symbolIndex;
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
                if((isMatched==1) & (*longestWidth<(tryFloor-sourcePointer) ) ){
                        *longestWidth = tryFloor - sourcePointer;
                        *longestToken = symbolIndex;
                }
        }

}

int hasNext(){
        if(*(sourcePointer+1)=='\0'){
                return 0;
        }else{
                return 1;
        }
}
