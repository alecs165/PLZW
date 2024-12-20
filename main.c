#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h> // Include for basename
#include <string.h>
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    static char bin[64], bout[64];
    setvbuf(stdin, bin, _IOFBF, 64);
    setvbuf(stdout, bout, _IOFBF, 64);

    char *exec_name = basename(argv[0]); // Get the executable name
    
    if (strcmp(exec_name, "encode") == 0) {
        int maxBits = 12;
        int pruning = 0;
        //looping through the arguments to detect either
        //maxbits or pruning sets
        //only care about env variables p and m
        //anything but stage 1 will want both prune and max_bits activated
        // if()
        //default
        if(getenv("STAGE") == NULL || !strcmp(getenv("STAGE"), "2") || !strcmp(getenv("STAGE"), "3")){
            // printf("HELLO\n\n\n\n\n\n\n\n");
            for(int i = 0; i <  argc; i++){
                if(!strcmp(argv[i],"-p")){
                    pruning = 1;
                }
                if(!strcmp(argv[i], "-m")){
                    //latest maxbits is the most relevat
                    //we assume no invalid inputs
                    maxBits = atoi(argv[i + 1]);
                }
            }
        }
        if(maxBits <= 8 || maxBits > 20){
            maxBits = 12;
        }

        encode(pruning, maxBits);
    } else if (strcmp(exec_name, "decode") == 0) {
        if (argc > 1) {
            fprintf(stderr, "decode: invalid option '%s'\n", argv[1]);
            exit(1);
        }
        decode();
    } else {
        fprintf(stderr, "Usage: %s [-m MAXBITS] [-p] < input > output\n", argv[0]);
        fprintf(stderr, "       %s < input > output\n", argv[0]);
        exit(1);
    }
    exit(0);
}