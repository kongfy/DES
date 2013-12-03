#include <stdio.h>
#include <stdlib.h>

#include "tables.h"

#define LB32_MASK 0x00000001
#define LB64_MASK 0x0000000000000001
#define L64_MASK 0x00000000ffffffff
#define H64_MASK 0xffffffff00000000

uint64_t des(uint64_t input, uint64_t key, char mode)
{
    int i, j;

    /* 8 bits */
    char row, column;

    /* 28 bits */
    uint32_t C = 0;
    uint32_t D = 0;

    /* 32 bits */
    uint32_t L = 0;
    uint32_t R = 0;
    uint32_t s_output = 0;
    uint32_t f_function_res = 0;
    uint32_t temp = 0;

    /* 48 bits */
    uint64_t sub_key[16] = {0};
    uint64_t s_input = 0;

    /* 56 bits */
    uint64_t permuted_choice_1 = 0;
    uint64_t permuted_choice_2 = 0;

    /* 64 bits */
    uint64_t init_perm_res = 0;
    uint64_t des_result = 0;
    uint64_t pre_output = 0;

    /* IP permutation */
    for (i = 0; i < 64; i++) {
        init_perm_res <<= 1;
        init_perm_res |= (input >> (64-IP[i])) & LB64_MASK;
    }

    L = (uint32_t) (init_perm_res >> 32) & L64_MASK;
    R = (uint32_t) init_perm_res & L64_MASK;

    /* Calculation of the 16 keys */
    for (i = 0; i < 56; i++) {
        permuted_choice_1 <<= 1;
        permuted_choice_1 |= (key >> (64-PC1[i])) & LB64_MASK;
    }

    C = (uint32_t) ((permuted_choice_1 >> 28) & 0x000000000fffffff);
    D = (uint32_t) (permuted_choice_1 & 0x000000000fffffff);

    for (i = 0; i< 16; i++) {
        for (j = 0; j < iteration_shift[i]; j++) {
            C = (0x0fffffff & (C << 1)) | (0x00000001 & (C >> 27));
            D = (0x0fffffff & (D << 1)) | (0x00000001 & (D >> 27));
        }

        permuted_choice_2 = 0;
        permuted_choice_2 = (((uint64_t) C) << 28) | (uint64_t) D ;

        sub_key[i] = 0;

        for (j = 0; j < 48; j++) {
            sub_key[i] <<= 1;
            sub_key[i] |= (permuted_choice_2 >> (56-PC2[j])) & LB64_MASK;
        }
    }

    for (i = 0; i < 16; i++) {
        /* f(R,k) function */
        s_input = 0;

        for (j = 0; j< 48; j++) {
            s_input <<= 1;
            s_input |= (uint64_t) ((R >> (32-E[j])) & LB32_MASK);
        }
        
        /* XORing E(Ri) with Ki */
        if (mode == 'd') {
            // decryption
            s_input = s_input ^ sub_key[15-i];
        } else {
            // encryption
            s_input = s_input ^ sub_key[i];
        }

        /* S-Box Tables */
        for (j = 0; j < 8; j++) {
            row = (char) ((s_input & (0x0000840000000000 >> (6*j))) >> (42-6*j));
            row = (row >> 4) | (row & 0x01);

            column = (char) ((s_input & (0x0000780000000000 >> (6*j))) >> (43-6*j));

            s_output <<= 4;
            s_output |= (uint32_t) (S[j][16*row + column] & 0x0f);
        }

        f_function_res = 0;

        for (j = 0; j < 32; j++) {
            f_function_res <<= 1;
            f_function_res |= (s_output >> (32 - P[j])) & LB32_MASK;
        }

        temp = R;
        R = L ^ f_function_res;
        L = temp;
    }

    pre_output = (((uint64_t) R) << 32) | (uint64_t) L;

    /* PI permutation */
    for (i = 0; i < 64; i++) {
        des_result <<= 1;
        des_result |= (pre_output >> (64-PI[i])) & LB64_MASK;
    }

    return des_result;

}

void validate_des()
{
    int i;
    uint64_t result = 0x9474B8E8C73BCA7D;

    for (i = 0; i < 16; i++) {
        if (i%2 == 0) {
            result = des(result, result, 'e');
        } else {
            result = des(result, result, 'd');
        }
    }
    
    printf("%016llX\n", result);
    return;
}
