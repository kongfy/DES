#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "des.h"

static const char *opt_string = "dc:k:h";

void display_usage()
{
    // display usage...
}

int main(int argc, char *argv[])
{
    /* validate_des(); */
    /* return 0; */

    char mode = 'e';
    
    uint64_t input = 0x0000000000000000;
    uint64_t key = 0x0000000000000000;
    char *input_str = NULL;
    char *input_key = NULL;

    int opt;
    while ((opt = getopt(argc, argv, opt_string)) != -1) {
        switch(opt) {
        case 'd':
            mode = 'd';
            break;
        case 'h':
            display_usage();
            return 0;
        case 'c':
            input_str = optarg;
            break;
        case 'k':
            input_key = optarg;
            break;
        default:
            return 0;
        }
    }

    if (!input_str || !input_key) {
        return 0;
    }

    if (strlen(input_str) != 16 || strlen(input_key) != 16) {
        return 0;
    }
    
    input = strtoul(input_str, NULL, 16);
    key  = strtoul(input_key, NULL, 16);

    printf("%016llX\n", des(input, key, mode));
    
    return 0;
}
