/* A simple base converter that helps converting numbers
 * from base 10 to hex, binary or octal representation.
 * 
 *
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_binary(unsigned int value);
void usage(char *prog_name);
void print_hex(unsigned int value);
void print_octal(unsigned int value);
void print_decimal(unsigned int value);
int which_base(char *num);
int is_hex(char *num);
int is_oct(char*);
int is_dec(char*);
int is_bin(char*);
void fatal(char *prog_name, char *msg);
void unknown(char *num);
unsigned int binToDec(char *value);
unsigned int hexToDec(char *value);
unsigned int octToDec(char *value);
unsigned int decToDec(char *value);
void convert(char *value, char *from, char *to,  unsigned int (*converter)(char*), void (*printer)(unsigned int));
void help();

static char* help_txt = "###                                                  \n"
						"# ###                  #########                     \n"
						"#   #####         ######       #                     \n"
						"#       ###########            #                     \n"
						"#                              #                     \n"
						"#           BACON              #                     \n"
						"#      - base converter -      #                     \n"
						" ##                        ####                      \n"
						"   #####         ##########                          \n"
						"        ##########                                   \n"
						"\n\n"
						"Convert between number bases\n"
						"\n\n"
						"        Author:   David Sugar\n"
						"        Version:  1.0\n"
						"        Date:     07.02.2019\n"
						"\n\n"
						"The program supports the following number types:     \n"
						"\n"
						"             NAME   |    BASE    |  EXAMPLE          \n"
						"        ------------------------------------         \n"
						"            BINARY  |     2      |  011001  \n"
						"        ------------------------------------\n"
						"            OCTAL   |     8      |  18q     \n"
						"        ------------------------------------\n"
						"           DECIMAL  |     10     |  128     \n"
						"        ------------------------------------\n"
						"        HEXADECIMAL |     16     |  0xff00  \n"
						"\n\n"
						"Options:\n\n"
						"        >  -b number   (convert number into binary number)\n"
						"        >  -q number   (convert number into octal number)\n"
						"        >  -d number   (convert number into decimal number)\n"
						"        >  -x number   (convert number into hexadecimal number)\n"
						"        >  -h          (help)\n"
						"\n\n"
						"Examples:               -b 25 (decimal to binary conversion of the number 25)\n"
						"                        -x 011001  ( convert 25 from binary into hexadecimal)\n";

int main(int argc, char *argv[]) {
	int i;
	int opt;
	enum base { BIN, OCT, DEC, HEX };
	char *sbase[] = { "BIN", "OCT", "DEC", "HEX"};
	unsigned int (* conv[4])(char*) = { binToDec, octToDec, decToDec, hexToDec  };	// converter functions
	void (* b_prints[4])(unsigned int) = { print_binary, print_octal, print_decimal, print_hex };  // print functions
	if(argc < 2)
		usage(argv[0]);

	while((opt = getopt(argc, argv, "b:q:x:d:h")) != -1) {
		switch(opt) {
			case 'h':
				help();
				break;
			case 'd':
				i = which_base(optarg);
				if(i == -1)	// optarg isn't a supported number
					break;
				convert(optarg, sbase[i], sbase[DEC], conv[i], b_prints[DEC]);
				break;
			case 'b':
				i = which_base(optarg);
				if(i == -1)	// optarg isn't a supported number
					break;
				convert(optarg, sbase[i], sbase[BIN], conv[i], b_prints[BIN]);
				break;
			case 'q':
				i = which_base(optarg);
				if(i == -1)	// optarg isn't a supported number
					break;
				convert(optarg, sbase[i], sbase[OCT], conv[i], b_prints[OCT]);
				break;
			case 'x':
				i = which_base(optarg);
				if(i == -1)	// optarg isn't a supported number
					break;
				convert(optarg, sbase[i], sbase[HEX], conv[i], b_prints[HEX]);
				break;
			case '?':
				  printf("[OPERR]: %c\n", optopt);
				  usage(argv[0]);
		}
	}

}

/*			Function that prints a number to the console.
 * Args:
 * 			value = string representing a number.
 * 			from, to = string representing the base (BIN, OCT, DEC, HEX)
 * 			converter = a converter function ( ...ToDec() see prototypes)
 * 			printer = function ( print_...() see prototypes)
 *
 */
void convert(char *value, char *from, char *to,  unsigned int (*converter)(char*), void (*printer)(unsigned int)) {
	unsigned int number = converter(value);		// number as string -> number
	
	printf("%s (%s) = ", value, from);
	printer(number);					// print number as bin, oct, dec, hex number (depends on passed function)
	printf(" (%s)\n", to);
}

/*##########################################################################
 * #######################   OUTPUT ########################################
 * #######################################################################*/

void help() {
	printf("%s", help_txt);
}

/* print usage message to stdout */
void usage(char *prog_name) {
	printf("[USAGE]: %s <opt> <unsigned_number_to_convert>\n", prog_name);
	printf("For more information use: %s -h\n", prog_name);
	exit(0);
}

/* print error message and the exit */
void fatal(char *prog_name, char *msg) {
	printf("[ERROR] in %s: %s\n", prog_name, msg);
	exit(-1);
}

void unknown(char *num) {
	printf("[ERROR]: can't determine type of %s!\n", num);
}
	

/* prints the given value in binary.
 *
 * Format:	00000000 00000000 00000000 00000000
 */
void print_binary(unsigned int value) {
	unsigned int byte_iterator, bit_iterator, k = 0x80000000;

	for(byte_iterator=0; byte_iterator < 4; byte_iterator++) {
		for(bit_iterator=0; bit_iterator < 8; bit_iterator++) {
			if( value & k )
				printf("1");
			else
				printf("0");
			value = value<<1;
		}
		printf(" ");
	}
}

void print_hex(unsigned int value) {
	char *mapping = "0123456789ABCDEF";
	unsigned int four_bit_iterator, val, k = 0x80000000, index;

	for(four_bit_iterator=0; four_bit_iterator < 8; four_bit_iterator++) {
		index = 0;

		for(val=8; val >= 1; val /= 2) {
			if( value & k )
				index += val;	
			value = value<<1;
		}
		printf("%c", mapping[index]);
	}
}

void print_octal(unsigned int value) {
	unsigned int iterator, val, k = 0x80000000, sum = 0;
	
	// calculate the most significant digit first to
	// make life a little bit easier
	if( value & k )
		sum += 2;
	value = value<<1;
	if( value & k )
		sum += 1;
	value = value<<1;
	printf("%u", sum);

	for(iterator=0; iterator < 10; iterator++) {
		sum = 0;
		for(val=4; val >= 1; val /= 2) {
			if( value & k )
				sum += val;
			value = value<<1;
		}
		printf("%u", sum);
	}
}

void print_decimal(unsigned int value) {
	printf("%d", value);
}

/*##########################################################################
 * #######################   PARSER ########################################
 * #######################################################################*/

/* determine the base of the given number passed as string
 * return:
 * 0 = BIN
 * 1 = OCT
 * 2 = DEC
 * 3 = HEX
 * -1 = No match
 */
int which_base(char *num) {
	int ret = -1;

	if(is_hex(num))
		ret = 3;
	else if(is_oct(num))
		ret = 1;
	else if(is_bin(num))
		ret = 0;
	else if(is_dec(num))
		ret = 2;
	
	return ret;
}

int is_hex(char *num) {
	int len = strlen(num);
	int ret = 0;
	
	if( len >= 3 ) {
		if( (*num == '0' || *num == 'O') && (*(num+1) == 'x' || *(num+1) == 'X') ) {
			for(int i = 2; *(num+i) != '\0'; i++) {
				if( !(*(num+i) >=48 && *(num+i) <= 57) && !(*(num+i) >= 65 && *(num+i) <= 70) &&
						!(*(num+i) >= 97 && *(num+i) <= 102))
					return 0;
			}
			ret = 1;
		}
	}
	return ret;
}

int is_oct(char *num) {
	int len = strlen(num);
	int ret = 0;
	
	if( len >= 2 ) {
		if( *(num+len-1) == 'q' ) {
			for(int i = 1; *(num+i) != '\0' && *(num+i) != 'q'; i++) {
				if(*(num+i) < 48 || *(num+i) > 55)
					return 0;
			}
			ret = 1;
		}
	}
	return ret;
}

int is_bin(char *num) {
	int ret = 0;

	if( *num == '0' ) {
		for(int i = 0; *(num + i) != '\0'; i++) {
			if( *(num+i) != '0' && *(num+i) != '1' )
				return 0;
		}
		ret = 1;
	}
	return ret;
}

int is_dec(char *num) {
	for(int i = 0; *(num+i) != '\0'; i++) {
		if( *(num+i) < 48 || *(num+i) > 57)
			return 0;
	}
	return 1;
}


/*##########################################################################
 * #######################   CONVERTER #####################################
 * #######################################################################*/

/* convert binary representation of a number into decimal
*/
unsigned int binToDec(char *value) {
	int sig = 1;
	int res = 0;

	for(int i = strlen(value)-1; i >= 0; i--, sig*=2)
		if(*(value+i) == '1')
			res += sig;
	return res;
}

/* convert a number from hexadecimal representation into decimal */
unsigned int hexToDec(char *value) {
	int sig = 1;
	int res = 0;
	char c;

	for(int i = strlen(value)-1; i > 1; i--, sig*=16) {
		c = *(value+i);

		if(c >= 48 && c <= 57)
			c -= 48;
		else if(c >= 65 && c <= 70)
			c -= 55;
		else if(c >= 97 && c <= 102)
			c -= 87;
		else
			c = 0;	// should never happen but you know how it is

		res += sig*c;

	}
	return res;
}

/* octal to decimal */
unsigned int octToDec(char *value) {
	int sig = 1;
	int res = 0;
	char c;

	for(int i = strlen(value)-2; i >= 0; i--, sig*=8) {	// -2 because of the 'q' at (val+strlen-1)
		c = *(value+i);
		
		if(c >= 48 && c <= 55)
			c -= 48;
		else			// if digit is not e[0,7]
			c = 0;		// just to prevent something
						// bad to happen
		res += sig*c;
	}
	return res;
}

unsigned int decToDec(char *value) {
	return atoi(value);
}






















