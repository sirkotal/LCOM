#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) {
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++) {
        sprintf(binstr + k, "%d", binary[j]);
    }
        
	return binstr;
}

void print_usage(char *name) {
	printf("Usage: %s <action> <byte> <bit no>\n", name);
	printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
		"\t\t <byte> 8-bit integer in hexadecimal\n"
		"\t\t <bit no> of bit to operate on\n");
}

bool checkHex(char* hex_num) {
    size_t len = strlen(hex_num);
    uint8_t start_index = 0;

    if (hex_num[0] == '0' && hex_num[1] == 'x') {
        len -= 2;
        start_index = 2;
    }

    if (len > 2) {
        return false;
    }

    for (uint8_t i = start_index; i < len; i++) {
        if ((hex_num[i] < '0' || hex_num[i] > '9') && (hex_num[i] < 'A' || hex_num[i] > 'F') && (hex_num[i] < 'a' || hex_num[i] > 'f')) {
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
	char a;	// action: 'h', 'l', 'r', 's', 't'
	unsigned long n; // value to convert must be smaller than 256
	int bit; // bit to operate on: must be between 0 an 7
    char binary[9]; // array for binary representation of n,
					//  remember that in C strings are terminated with a 0
 
	// Validate command line arguments

    if (argc != 4) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    a = argv[1][0];

    if (a != 'h' && a != 'l' && a != 'r' && a != 's' && a != 't') {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[1]) != 1) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }


    if (!checkHex(argv[2])) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (isdigit(argv[3][0]) && strlen(argv[3]) != 1) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!(argv[2][0] == '0' && argv[2][1] == 'x')) {
        char* hex = (char*) malloc((strlen(argv[2]) + 3) * sizeof(char));
        strcat(hex, "0x");
        strcat(hex, argv[2]);

        n = strtoul(hex, NULL, 16);
    } 
    else {
        n = strtoul(argv[2], NULL, 16);
    }


    bit = strtod(argv[3], NULL);

    if (bit > 7 || bit < 0) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

	// Print to stdout the binary representation of n

    byte2bin(n, binary);

    printf("- Action: %c\n", a);
    printf("- Byte: %s\n", binary);
    printf("- Byte Number: %d\n", bit);
    printf("\n");
	
	// Do what the user asked and print the result

    switch (a) {
        case 'h':
            if(~((n >> bit) & 1) & 1) {
                printf("bit %d is not set to 1\n", bit);
            } 
            else {
                printf("bit %d is set to 1\n", bit);
            }
            break;
        case 'l':
            if(!(n & BIT(bit))) {
                printf("bit %d is set to 0\n", bit);
            } 
            else {
                printf("bit %d is not set to 0\n", bit);
            }
            break;
        case 's':
            n = n | BIT(bit);
            byte2bin(n, binary);
            printf("Set bit %d to 1\n", bit);
            printf("Byte: %s\n", binary);
            break;
        case 'r':
            n = n & ~BIT(bit);
            byte2bin(n, binary);
            printf("Cleared bit %d to 0\n", bit);
            printf("Byte: %s\n", binary);
            break;
        case 't':
            n = n ^ BIT(bit);
            byte2bin(n, binary);
            if(n & BIT(bit)) {
                printf("Toggled bit %d to 1\n", bit);
            } 
            else {
                printf("Toggled bit %d to 0\n", bit);
            }
            printf("Byte: %s\n", binary);
            break;
    }
	
    return 0;
}
