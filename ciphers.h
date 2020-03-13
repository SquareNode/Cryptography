#ifndef __CIPHERS__
#define __CIPHERS__

//struct 
typedef struct { 
	int first;
	int second;	
}pair;

//ciphers
void caesar_cipher_case_sensitive(char *msg, int shift);

void caesar_cipher_lower_case_only(char *msg, int shift);

void substitution_cipher_inefficient(char *msg);

void substitution_cipher_efficient(char *msg);

void playfare_cipher(char * msg);

void vigenere_cipher(char *msg);

void transposition_cipher(char *msg);

//ultility

pair find(char c, char table[5][5]);

void playfare_cipher_mk_table(char *secret, char table[5][5]);

void permutation_smart(char alphabet[26], int i, int *swap_calls);

void permutation(char alphabet[26], int i, int *swap_calls);

void swap(char *arr, int i, int j);

#endif