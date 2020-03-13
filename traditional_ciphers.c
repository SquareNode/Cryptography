#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <string.h>


#include "ciphers.h"


void caesar_cipher_case_sensitive(char *msg, int shift) {
	
	char chararr[52];
	int i;
	for(i = 0; i < 26; i++) 
		chararr[i] = i + 65;
	for( ; i < 52; i++) 
		chararr[i] = i + 71;

	while(*msg) {
		if(isupper(*msg)) 
			printf("%c", chararr[(*msg - 65 + shift)%52]);
		else if(islower(*msg))
			printf("%c", chararr[(*msg - 71 + shift)%52]);
		else 
			printf("%c", *msg);
		msg++;
	}
	
}


void caesar_cipher_lower_case_only(char *msg, int shift) {
	
	char chararr[26];
	int i;
	for(i = 0; i < 26; i++) 
		chararr[i] = i + 97;
	
	while(*msg) {
		if(isupper(*msg)) 
			
			printf("%c", chararr[(tolower(*msg) - 97 + shift)%26]);
		
		else if(islower(*msg))
			printf("%c", chararr[(*msg - 97 + shift)%26]);
		else 
			printf("%c", *msg);
		msg++;
	}
	
}

void swap(char *arr, int i, int j) {
	
	char tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}
	

void permutation(char alphabet[26], int i, int *swap_calls) {
	
	if(i == 26)
		return;
	int k = rand()%(26-i);
	
	//printf("i: %d k: %d\n", i, k);
	
	if(k == 0)
		permutation(alphabet, i+1, swap_calls);
	else {
		swap(alphabet, i, k + i);
		*swap_calls+=1;
		permutation(alphabet, i+1, swap_calls);
	}

}

void permutation_smart(char alphabet[26], int i, int *swap_calls) {
	
	if(i == 13)
		return;
	int k = rand()%(26-i);
	
	printf("i: %d k: %d\n", i, k);
	
	if(k == 0)
		permutation_smart(alphabet, i+1, swap_calls);
	else {
		swap(alphabet, i, k + i);
		*swap_calls+=1;
		permutation_smart(alphabet, i+1, swap_calls);
	}

}

void substitution_cipher_inefficient(char *msg) {
	
	srand(time(NULL));
	//a -> x, b -> r ...
	char map[26],last = 'z';
	int i, taken[26], rand_calls=0;
	for(i = 0; i < 26; i++) 
		taken[i] = 0;
	
	//map['a'] = 'x';
	
	for(i = 0; i < 26; i++) {
		
		int candidate = rand()%26+97;
		rand_calls++;
		while(taken[candidate - 97] && rand_calls++)
			candidate = rand()%26+97;
		map[i] = candidate;
		taken[candidate - 97] = 1;
		
	}
	
	for(i = 0; i < 26; i++) 
		printf("%c ", map[i]);
	
	while(*msg) {
		
		if(isalpha(*msg)) 
			printf("%c", map[*msg - 'a']);
		else
			printf("%c", *msg);
		msg++;
	}
	
	printf("\nrand() calls: %d", rand_calls);
	
}

void substitution_cipher_efficient(char *msg) {
	
	srand(time(NULL));
	char map[26];
	int i, swap_calls = 0;
	for(i = 0; i < 26; i++)
		map[i] = i + 'a';

	permutation_smart(map, 0, &swap_calls);
	for(i = 0; i < 26; i++)
		printf("%c ", map[i]);
	
	putchar('\n');
	
	while(*msg) {
		
		if(isalpha(*msg)) 
			printf("%c", map[*msg - 'a']);
		else
			printf("%c", *msg);
		msg++;
	}
	
	printf("\nswap calls: %d", swap_calls);
	
}

void playfare_cipher_mk_table(char *secret, char table[5][5]) {
	
	int used[26];
	int i = 0, j = 0;
	
	for(i = 0; i < 26; i++) 
		used[i] = 0;
	
	i = 0;
	
	while(*secret) {
		if(!used[*secret - 'a']) {
			table[i][j] = *secret;
			i = j + 1 == 5 ? i + 1: i;
			j = j + 1 == 5 ? 0 : j + 1;
			
			used[*secret - 'a'] = 1;
		}
		
		secret++;
	}
	
	int curr_letter = 0;
	
	while(i < 5) {
		
		if(!used[curr_letter])
			table[i][j] = 'a' + curr_letter++;
		else {
			curr_letter++;
			while(used[curr_letter])
				curr_letter++;
			table[i][j] = 'a' + curr_letter++;
		}
		i = j + 1 == 5 ? i + 1: i;
		j = j + 1 == 5 ? 0 : j + 1;
		
	}
	
}

pair find(char c, char table[5][5]) {

	int i, j;
	pair res;
	
	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			if(c == table[i][j]) {
				res.first = i;
				res.second = j;
				return res;
			}
	
}

void playfare_cipher(char * msg) {
	
	char *secret = "secret", table[5][5];
	int i, j;
	playfare_cipher_mk_table(secret, table);
	
	//26 letters but 25 chars in table ? 
	
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 5; j++) 
			printf("%c ", table[i][j]);
		putchar('\n');
	} 
	
	while(*msg) {
		char first = *msg;
		char second = *(msg + 1); //what if *(msg + 1) == '\0' ? 
		
		
		pair fst = find(first, table);
		pair scnd = find(second, table);
		
		if(fst.first == scnd.first)
			printf("%c%c", table[fst.first][(fst.second+1)%5], 
			table[scnd.first][(scnd.second+1)%5]);
		else if(fst.second == scnd.second) 
			printf("%c%c", table[(fst.first+1)%5][fst.second],
			table[(scnd.first+1)%5][scnd.second]);
		else
			printf("%c%c", table[fst.first][scnd.second],
			table[scnd.first][fst.second]);
		msg+=2;
	}
	
}

void vigenere_cipher(char *msg) {

	char *secret = "aaa";
	
	int i = 0;
	while(*msg) {
		//char doesn't work here-> overflow because only 8 bits fit into char
		int curr = *msg + secret[i] - 'a' + 1;
		printf("%c", curr > 'z' ? curr - 26 : curr);
		msg++;
		i = !secret[i+1] ? 0 : i+1;
	}
	
}

void transposition_cipher(char *msg) {
	
	int msg_size = strlen(msg), dim = sqrt(msg_size) + 1, i, j, k = 0;
	char table[dim][dim];
	for(i = 0; i < dim; i++) 
		for(j = 0; j < dim; j++) {
			if(k < msg_size) {
				table[i][j] = *msg;
				msg++;
				k++;
			}
			else 
				table[i][j] = '-';
		}
	
	for(i = 0; i < dim; i++) {
		for(j = 0; j < dim; j++)
			printf("%c ", table[i][j]);
		putchar('\n');
	}
	
	for(i = 0; i < dim; i++) 
		for(j = 0; j < dim; j++) 
			if(table[j][i] != '-')
				printf("%c", table[j][i]);
	
}		


int main () {
	
	playfare_cipher("amon");
	
	return 0;
}