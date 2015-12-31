#include <stdio.h>
#include <stdlib.h>
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
int main(int argc, char* argv[] ){
        if(argc==1){
                printf("Usage: none yet\n");
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
        char source[size];
        
        /*****          Load characters from source into char array             ****/
        int fileInd;
        for(fileInd=0; fileInd<size; fileInd++){
                source[fileInd]=fgetc(fp);
        }
        
        /**********      Define TokenStruct used to gather tokens       **********/
        struct _token{
                int size_of_image;
                int tokenType;
                char image[];
        };
        typedef struct _token TokenStruct;
        
        const int size_of_tokenList = 1024;
        TokenStruct tokenList[size_of_tokenList];
        int tokenListMax=0;
        
        
        /**********     loop to tokenize source         *********/
        const char reservedKeywords = {"int","char","float","double","struct","union",
                                        "long","short","unsigned","auto","extern",
                                        "register","typedef","static","goto","return",
                                        "sizeof","break","continue","if","else","for",
                                        "do","while","switch","case","default","entry" };
        int tokenizeIndex;
        int floor = 0;
        int biggest, tokenType, biggestTokenType;
        biggest = -1; tokenType = INT; biggestTokenType = -1;
        while(floor!=size){
                int reservedIndex = 0;
                for(tokenizeIndex=floor; tokenizeIndex<size; tokenizeIndex++){
                        if(tokenType<=ENTRY){
                                if(source[tokenizeIndex]==reservedKeywords[tokenType][reservedIndex]){
                                        reservedIndex++;
                                        
                                }else if(reserved_keywords[tokenType][reservedIndex]=='\0'){
                                        /****     Check if this token is biggest     ****/
                                        if(biggest<reservedIndex){
                                                biggest = reservedIndex;
                                                biggestTokenType = tokenType;
                                        }
                                        break;
                                }else{
                                        break;
                                }
                        }
                }
                tokenType++;
        }
        
        
        fclose(fp);
return 0;
}