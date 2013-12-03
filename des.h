#ifndef _DES_H_
#define _DES_H_

#include <stdint.h>

/*
 * The DES function
 * input: 64 bit message
 * key: 64 bit key for encryption/decryption
 * mode: 'e' = encryption; 'd' = decryption
 */
uint64_t des(uint64_t input, uint64_t key, char mode);

/*
 * TESTING IMPLEMENTATION OF DES
 * Ronald L. Rivest
 * Detail: "Riv85.txt"
 *
 * X0: 9474B8E8C73BCA7D
 * X16: 1B1A2DDB4C642438
 */
void validate_des();


#endif /* _DES_H_ */
