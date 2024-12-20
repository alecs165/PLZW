
#include "encode.h"


unsigned int prune(hashmap **hm, unsigned int max_bits){
    //since we are only calling this to prune a full table
    unsigned int size = 0b1<<max_bits;
    key_pair* sorted_table = hm_table(*hm, size);
    hashmap *pruned_hm  = create_hm(size);
    //an old_code ->new code array
    unsigned int* oc_nc = malloc(sizeof(unsigned int)* size);
    key_pair* pruned_arr = malloc(size*sizeof(key_pair));
    //the first 255 codes definitely
    for(int i = 0; i < 256; i++){
        set_val(pruned_hm,-1,i, i);
        oc_nc[i] = i;
        pruned_arr[i].c = i;
        pruned_arr[i].pref = -1;
    }
    //initializing to -2 just in case we encounter a prefix of zero
    for(int i = 256; i < size; i++){
        pruned_arr[i].pref = -2;
    }
    //first going to insert into an array
    //then collapse that array
    //then reinsert into the hashmap with the new code values
    for(int i = 256; i < size; i++){
        //we know it will exist
        //accessing the hashmap array directly, because we do know the prefix values (without
        //exceeding linear time complexity) in advance
        if(sorted_table[i].pref > 255){
            //making sure the prefix isn't just a char
            //then inserting the pref code into the table
            pruned_arr[sorted_table[i].pref].c = sorted_table[sorted_table[i].pref].c;
            pruned_arr[sorted_table[i].pref].pref = sorted_table[sorted_table[i].pref].pref;
        }
    }
    //how many codes we have ignored, thus 'offsetting' the
    //original codes by this amount to account for the
    //discrepancy
    unsigned int offset = 0;
    //now need to go through the arr, keep track of the offset caused by empty indices
    //and insert into the hashmap
   for(int i = 256; i < size; i++){
       if(pruned_arr[i].pref != -2){
           oc_nc[i] = i - offset;
       }else{
           oc_nc[i] = i - offset;
           offset++;
       }
   }
   for(int i = 256; i < size; i++){
        //only adding if a valid code
        if(pruned_arr[i].pref != -2){
            set_val(pruned_hm,oc_nc[pruned_arr[i].pref],pruned_arr[i].c, oc_nc[i]);
        }
   }
    //no need to reassign the codes on the stack since LZW runs sequentially
    //and set the hashmap equal to the pruned one
    //don't need the old table anymore
    destroy_hm(*hm);
    free(*hm);
    //need to fix this part in all probability
    *hm = pruned_hm;
    //and free our temp variables
    free(sorted_table);
    free(pruned_arr);
    free(oc_nc);
    //returning the new max code
    return size - offset;
}


void encode(unsigned int p, unsigned int max_bit_length) {
    //prints out in ascii form, but we don't care
    printf("%c", (char)max_bit_length);

    //and second whether it has been pruned
    printf("%c", p);
    //go a byte at a time
    //2^ max_bit_length
    hashmap *hm  = create_hm(0b1<<max_bit_length);
    stack *s = init_stack(s);
    unsigned int max_size = 0b1<<max_bit_length;
    //Initialize the string table to contain the pair (EMPTY,K) for each char K
    for(int i = 0; i < 256; i++){
        set_val(hm,-1,i, i);
    }
    unsigned int K;
    unsigned int C = -1;
    //to keep track what number we are on for the codes
    unsigned int val_ctr = 256;
    unsigned char packed_val = 0;
    char packed_index = 0;
    char max_b_to_pack = 9;
    char left_over = 0;
    
    char pack = 0;
    if(getenv("STAGE") == NULL){
        pack = 1;
    }else if (getenv("STAGE") != NULL && !strcmp(getenv("STAGE"), "3")){
        pack = 1;
    }



    while ((K = getchar()) != EOF){
        //decode
        
        if(get_val(hm,C, K)){
            C = *(unsigned int*)get_val(hm,C, K);
        }else{
            //stage 3 is the default
            if(pack){
                if(val_ctr >= 1<<max_b_to_pack && max_b_to_pack < max_bit_length){
            max_b_to_pack++;
        }
                //the min
            if (left_over + max_b_to_pack == 8){
                packed_val = C;
                printf("%c", packed_val);
                packed_val = 0;
                packed_index = 0;
                left_over = 0;
            }else if(left_over){
                unsigned int tmp = max_b_to_pack + left_over;
                unsigned int ctmp = C;
                //preserving the amt we need for the new code
                ctmp = (ctmp << (32 - max_b_to_pack));
                //packing the bits
                packed_val = packed_val | ctmp>>(24 + left_over);
                printf("%c", packed_val);
                ctmp = ctmp << (8 - left_over);
                tmp -= 8;
                while(tmp >= 8){
                    packed_val = ctmp >> 24;
                    printf("%c", packed_val);
                    ctmp = ctmp << 8;
                    tmp -= 8;
                }
                left_over = tmp;
                if(left_over){
                    packed_val = ctmp >> 24;
                }else{
                    packed_val = 0;
                }
            }else{
                // printf("hi how are you?");
                //base case
                unsigned int tmp = max_b_to_pack;
                unsigned int ctmp = C;
                //preserving the amt we need for the new code
                ctmp = ctmp << (32 - max_b_to_pack);
                while(tmp >= 8){
                    packed_val = ctmp >> 24;
                    printf("%c", packed_val);
                    ctmp = ctmp << 8;
                    tmp -= 8;
                }
                left_over = tmp;
                if(left_over){
                    packed_val = ctmp >> 24;
                }else{
                    packed_val = 0;
                }
                }
            }else{
                printf("%d\n", C);
            }
            //otherwise we would just continue to add to our hashmap
        if(val_ctr < max_size){
            set_val(hm, C, K, val_ctr);
            val_ctr++;
        }else{
            if(p){
                val_ctr = prune(&hm, max_bit_length);
                int tmp1 = val_ctr;
                max_b_to_pack = 0;
                while(tmp1){
                    tmp1 = tmp1>>1;
                    max_b_to_pack++;
                }
                if(val_ctr < max_size){
                    set_val(hm, C, K, val_ctr);
                    //but also pop and push the new value on the stack
                    val_ctr++;
                }
                
                }
        }
            //set C to equal just the character again
            C = *(unsigned int*)get_val(hm,-1,K);
        }
    }
    if(C != -1){
        if(!pack){
            printf("%d\n", C);
        }else{
            if(left_over){
                unsigned int tmp = max_b_to_pack + left_over;
                unsigned int ctmp = C;
                //preserving the amt we need for the new code
                ctmp = ctmp << (32 - max_b_to_pack);
                //packing the bits
                packed_val = packed_val | (ctmp >> (24 + left_over));
                printf("%c", packed_val);
                ctmp = ctmp << (8 - left_over);
                tmp -= 8;
                while(tmp >= 8){
                    packed_val = ctmp >> 24;
                    printf("%c", packed_val);
                    ctmp = ctmp << 8;
                    tmp -= 8;
                }
                left_over = tmp;
                if(left_over){
                    packed_val = ctmp >> 24;
                printf("%c", packed_val);
                }else{
                    packed_val = 0;
                }
            }else{
                //base case
                unsigned int tmp = max_b_to_pack;
                unsigned int ctmp = C;
                //preserving the amt we need for the new code
                ctmp = ctmp << (32 - max_b_to_pack);
                while(tmp >= 8){
                    packed_val = ctmp >> 24;
                    printf("%c", packed_val);
                    ctmp = ctmp << 8;
                    tmp -= 8;
                }
                left_over = tmp;
                if(left_over){
                    packed_val = ctmp >> 24;
                printf("%c", packed_val);
                }else{
                    packed_val = 0;
                }
                }
        }
    }
    //if DBG env variable set, dumping the table in human
    //readable format
    if(getenv("DBG")){
        FILE *fptr;
        fptr = fopen("./DBG.encode", "w");
        dump_hm(fptr, hm);
    }
    stack_free(s);
    destroy_hm(hm);
    free(hm);
}

