#include "hashtable.h"
// typedef hashmap hashmap;

hashmap *create_hm(unsigned int num_elements){
    //initializing the hashmap
    hashmap *hm = calloc(1, sizeof(hashmap));
    hm->arr = calloc(num_elements,sizeof(linked_list));
    hm->size = num_elements;
    return hm;
}

u_int64_t Hash(int pref, unsigned int c,unsigned int size){
    return (((unsigned long)(pref) << c) | (unsigned)(c)) % size;
}


void destroy_hm(hashmap *hm){
    // printf("h");
    //freeing the chains
    if(!hm){
        return;
    }
    if(hm->arr){
    for(int i = 0; i < hm->size; i++){
        linked_list *l = hm->arr[i];
        while(l){
            linked_list *t = l->rest;
            free(l);
            l = t;
        }
    }
    
    }
    //& then the hashmap array
    free(hm->arr);
    
}

void* get_val(hashmap *hm, int pref, unsigned int c){
    unsigned int h = Hash(pref, c, hm->size);
    linked_list *l = hm->arr[h];
    while(l){
        if(l->pref == pref && l->c == c){
            return &(l->val);
        }
        l = l->rest;
    }
    return NULL;
}



void* set_val(hashmap *hm,int pref, unsigned int c, unsigned int val){
    //fetching the hash value
    u_int64_t h = Hash(pref, c, hm->size);
    // void* old_v = get_val(hm, pref, c);
    //only calling this when setting new codes
    //setting it in the respective chain linked list
    if(!hm->arr[h]){
        hm->arr[h] = l_cons(val, pref, c, NULL);
    }else{
        hm->arr[h] = l_cons(val, pref, c, hm->arr[h]);
    }
    return NULL;
}


void dump_hm(FILE *fptr, hashmap *hm){
    unsigned int *arrp = calloc(hm->size, sizeof(unsigned int));
    unsigned int *arrc = calloc(hm->size, sizeof(unsigned int));

    for(int i = 0; i < hm->size; i++){
        if(hm->arr[i] != NULL){
            linked_list *l = hm->arr[i];
            while(l ){
                arrc[l->val] = l->c;
                arrp[l->val] = l->pref;
                
                l = l->rest;
            }
        }
    }
    for(int i = 0; i < hm->size; i++){
        fprintf(fptr, " Code %d, prefix %d char %d\n", i, arrp[i], arrc[i]);
    }
    free(arrc);
    free(arrp);

}

key_pair* hm_table(hashmap *hm, unsigned int size){
    //returns a sorted table of hashmap values
    //we know will be filled up sequentially, so running through hm and
    //dumping the pref and c values at the array location
    linked_list *s = hm->arr[0];
    //array of linked lists containing
    key_pair* arr = malloc(sizeof(key_pair)* size);
    for(int i = 0; i < size; i++){
        if(hm->arr[i]){
            linked_list *l = hm->arr[i];
            while(l){
                //dumping the value in our array
                key_pair v;
                v.c = l->c;
                v.pref = l->pref;
                arr[l->val] = v;
                l = l->rest;
            }
        }
    }
    return arr;
}