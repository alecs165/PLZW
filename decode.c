#include "decode.h"
#include "stack.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>




unsigned int prune_decode(data_entry** arr, unsigned int max_storage){
    data_entry *pruned_arr = malloc(max_storage*sizeof(data_entry));

    for(int i = 0; i < 256; i++){
        pruned_arr[i].pref = -1;
        pruned_arr[i].ch = i;
    }
    //initializing all 'empty' values to -2 just in case they use the null
    //ascii character very unlikely but not impossible
    for(int i = 256; i < max_storage; i++){
        pruned_arr[i].pref = -2;
    }
    for(int i = 256; i < max_storage; i++){
        //only if the pref is contained will it be
        //entered into the pruned array
        pruned_arr[(*arr)[i].pref].pref = (*arr)[(*arr)[i].pref].pref;
        pruned_arr[(*arr)[i].pref].ch = (*arr)[(*arr)[i].pref].ch;
    }
    //overriding all old arr values (no longer up to date)
    free(*arr);
    *arr = (malloc(max_storage*sizeof(data_entry)));
    for(int i = 256; i < max_storage; i++){
        (*arr)[i].pref = -2;
    }
    //use this to help create a new collapsed array (counts the gaps)
    unsigned int offset = 0;
    //old code -> new code array
    unsigned int *oc_nc = malloc(max_storage*sizeof(unsigned int));

    for(int i = 0; i < max_storage; i++){
        if(pruned_arr[i].pref != -2){
            oc_nc[i] = i - offset;
        }else{                        
            offset++;
            oc_nc[i] = i - offset;
        }
    }
    for(int i = 0; i < 256; i++){
        (*arr)[i].pref = -1;
        (*arr)[i].ch = i;
    }
    for(int i = 256; i < max_storage; i++){
        if(pruned_arr[i].pref != -2){
            //need to convert them all to the new codes
            (*arr)[oc_nc[i]].ch = pruned_arr[i].ch;
            (*arr)[oc_nc[i]].pref = oc_nc[pruned_arr[i].pref];
        }
    }

    free(pruned_arr);
    free(oc_nc);
    return max_storage - offset;
}

void decode() {

    // assuming max_bits = 12 for the first stage
    char MAX_BITS = 12;
    char pruned = 0;
    //only if stage 2 reading them as pruned and such 
    scanf("%c%c", &MAX_BITS, &pruned);
    char pack = 0;
    if(getenv("STAGE") == NULL){
        pack = 1;
    }else if (getenv("STAGE") != NULL && !strcmp(getenv("STAGE"), "3")){
        pack = 1;
    }

    //else default values
    if(!(getenv("STAGE"), "1")){
        MAX_BITS = 12;
        pruned = 0;
    }
    unsigned int max_storage = 0b1<<MAX_BITS;
    //so allocating that much for the array
    data_entry *arr = malloc(max_storage*sizeof(data_entry));
    
    stack *s = init_stack(s);

    for(unsigned int i = 0; i < 256; i++){
        arr[i].pref = -1;
        arr[i].ch = i;
    }
    //empty is -1
    int oldC = -1;
    unsigned int count = 256;
    unsigned char newC;
    unsigned int newCtmp = 0;
    unsigned int C = 0;
    // unsigned int left_over_packed = 0;
    char left_over_b = 0;
    char max_b_to_pack = 9;
    unsigned int left_over = 0;
    int index = 0;
    //using this when pruning and have to change max_b
    //in between operations
    if(!pack){
        unsigned int diffnewC;
        while (scanf("%d", &diffnewC) != EOF){
            
            C = diffnewC;
                //prune at beginning to sync w encode
            if (pruned && count >= max_storage){
                count = prune_decode(&arr, max_storage);
                int tmp1 = count;
                max_b_to_pack = 0;
                while(tmp1){
                    tmp1 = tmp1>>1;
                    max_b_to_pack++;
                }
            }

        //unknown code
        if(C >= count){
            int t = oldC;
            while(arr[t].pref != -1){
                t = arr[t].pref;
            }
            s = push(arr[t].ch,s);
            C = oldC;
        }
        while(arr[C].pref != -1){
            s =push(arr[C].ch,s);
            C = arr[C].pref;
        }
        
        int finalK = arr[C].ch;
        printf("%c", (char)finalK);
        while(s->size > 0){
            unsigned int ch = pop(&s);
            printf("%c", ch);
        }

        //adding new value to the new code
        if(oldC != -1){
            if(count < max_storage){
                arr[count].ch = finalK;
                arr[count].pref = oldC;
                count++;
            }
        }
        oldC = diffnewC;
        }
    }else{
        while (scanf("%c", &newC) != EOF){
        // printf("%d NEW CODE TEEHEE\n", newC);
        
        if (pruned && count >= max_storage){
            count = prune_decode(&arr, max_storage);
            int tmp1 = count;
                max_b_to_pack = 0;
                while(tmp1){
                    tmp1 = tmp1>>1;
                    max_b_to_pack++;
                }
        }
        C = 0;
        if(left_over_b){
            C = 0;
            //index is equal to num_bits in left_over
            while(index + 8 < max_b_to_pack){
               C = (C<<8)|newC;
               if(max_b_to_pack - index != 0){
                    scanf("%c", &newC);
                }
               index+=8;
            }
            if(index + 8 == max_b_to_pack){
                C = (C<<8)|newC;
                index += 8;
            }
            //add at the end
            if(max_b_to_pack > 16){
                C = (C << ((max_b_to_pack) - index)) | (left_over <<(max_b_to_pack - 8));
            }else{
            C = (C << ((max_b_to_pack - 8) - index%8)) | (left_over <<(max_b_to_pack - 8));
            }
        } else{
            C = 0;
            while(index + 8 < max_b_to_pack){
               C = (C<<8)|newC;
               if(max_b_to_pack - index != 0){
                    scanf("%c", &newC);
                }
               index+=8;
            }
            if(index + 8 == max_b_to_pack){
                C = (C<<8)|newC;
                index += 8;
            }
            //shifting to the top
            C = C<<(max_b_to_pack - index);
        }
        //now acct for remaining bits in newC by saving them in left_over
            if(max_b_to_pack - index != 0){
                unsigned char tmpc = (newC >> (8 - (max_b_to_pack - index)));
                C = (C)|tmpc;
            // printf("\n,%d char\n", C);
                //erasing the bits we just gave to C
                left_over = 0;
                tmpc = (newC << (max_b_to_pack - index));
                left_over = tmpc;
                index = 8 - (max_b_to_pack - index);
                left_over_b = 1;
            }else{
                left_over = 0;
                left_over_b = 0;
                index = 0;
            }
        newCtmp = C;
        //unknown code
        if(C >= count){
            int t = oldC;
            while(arr[t].pref != -1){
                t = arr[t].pref;

            }

            s = push(arr[t].ch,s);
            C = oldC;
        }
        while(arr[C].pref != -1){
            s =push(arr[C].ch,s);
            C = arr[C].pref;
        }
        
        int finalK = arr[C].ch;
        printf("%c", (char)finalK);
        while(s->size > 0){
            unsigned int ch = pop(&s);
            printf("%c", ch);
        }

        //adding new value to the new code
        if(oldC != -1){
            if(count < max_storage){
                arr[count].ch = finalK;
                arr[count].pref = oldC;
                count++;
            }
        }
        oldC = newCtmp;
        if(count + 1 >= 1<<max_b_to_pack && max_b_to_pack < MAX_BITS){
            max_b_to_pack++;
        }
        }

    }

   //if DBG env variable set, dumping the table in human
    //readable format
    if(getenv("DBG")){
        FILE *fptr;
        fptr = fopen("./DBG.decode", "w");
        for(int code = 0; code < max_storage; code++){
            //if one of the arrays is valid
            if(arr[code].pref != -2){
                fprintf(fptr," Code %d, prefix %d char %d\n", code, arr[code].pref, arr[code].ch);
            }
        }
    }
    free(arr);
    //this should be redundant, but just in case
    stack_free(s);
}

