// Reference: https://ja.wikipedia.org/wiki/Brainfuck

#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_SIZE 8192

unsigned int get_code_size(FILE*);
void bfi(FILE*);


int main(int argc, char** argv){
    FILE* fp;

    if(argc<2){
        fprintf(stderr, "No input file. usage: %s <InputFile>\n", argv[0]);
        return -1;
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL){
        fprintf(stderr, "Can't open %s\n", argv[1]);
        return -1;
    }

    bfi(fp);
    fclose(fp);

    return 0;
}


void bfi(FILE* fp){
    char* buf = (char*)calloc(MAX_BUF_SIZE, sizeof(char));
    char* ptr = buf + (MAX_BUF_SIZE) / 2;
    char c;
    unsigned long i = 0;
    int loop = 0;

    while((c = fgetc(fp)) != EOF){
        i++;
        switch(c){
            case '>':
                ptr++;
                break;
            
            case '<':
                ptr--;
                break;

            case '+':
                (*ptr)++;
                break;

            case '-':
                (*ptr)--;
                break;

            case '.':
                putchar(*ptr);
                break;

            case ',':
                *ptr = getchar();
                break;

            case '[':
                if(*ptr == 0){
                    int cnt = 0;
                    while(1){
                        c = fgetc(fp);
                        i++;
                        if(c == EOF){
                            fprintf(stderr, "ERROR: Loop is not closed. No.%ld char '['\n", i);
                            return;
                        }
                        else if(c == '['){
                            cnt++;
                        }
                        else if(c == ']'){
                            if(cnt == 0)
                                break;
                            else
                                cnt--;
                        }
                    }       
                }
                else
                    loop++;
                break;

            case ']':
                if(*ptr != 0){
                    int cnt = 0;
                    if(loop == 0){
                        fprintf(stderr, "ERROR: Loop did not start. No.%ld char ']'\n", i);
                        return;
                    }
                    fseek(fp, -1L, SEEK_CUR);
                    i--;
                    while(1){
                        fseek(fp, -1L, SEEK_CUR);
                        i--;
                        c = fgetc(fp);
                        if(c == ']'){
                            cnt++;
                        }else if(c == '['){
                            if(cnt == 0)
                                break;
                            else
                                cnt--;
                        }
                        fseek(fp, -1L, SEEK_CUR);
                    }
                }
                else{
                    loop--;
                }
                break;

            default:
                // Other chars are considered as comments.
                break;
        }
    }
    free(buf);
    return;
}


