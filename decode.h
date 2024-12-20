#ifndef B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65
#define B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65

typedef struct{
    unsigned int pref;
    unsigned int ch;
} data_entry;

/// @brief prunes the array
/// @param max_storage the maximum storage of the array
/// @return index aof where the array ends
unsigned int prune_decode(data_entry** arr, unsigned int max_storage);

/// @brief decodes using the LZW algorithm
/// @return the decoded text
void decode();

#endif /* B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65 */