#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void err(char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

char stream_enc(char x, unsigned key) {
	
	unsigned res = 0, curr_key, curr_x, i;
	for(i = 0; i < 32; i++){
		
		curr_key = key & (1 << i);
		curr_x = x & (1 << i) ;
		res += curr_x ^ curr_key;
		
		// printf("step: %d, curr_key: %d, curr_x: %d, res: %d\n", i, curr_key, curr_x, res);
		
	}
	
	return res;
}

char stream_dec(char x, unsigned key) {
	
	unsigned res = 0, curr_key, curr_x, i;
	for(i = 0; i < 32; i++){
		
		curr_key = key & (1 << i);
		curr_x = x & (1 << i) ;
		res += curr_x ^ curr_key;
		
		// printf("step: %d, curr_key: %d, curr_x: %d, res: %d\n", i, curr_key, curr_x, res);
		
	}
	
	return res;
}

void stream_on_letters(char *msg, char *key) {
	
	//map chars to 0 - 25, xor, find in map
	char map[26], *enc, *dec;
	int i, len = strlen(msg);
	for(i = 0; i < 26; i++)
		map[i] = 'a' + i;
	
	enc = malloc((len + 1) * sizeof(char));
	dec = malloc((len + 1) * sizeof(char));
	
	if(enc == NULL || dec == NULL) 
		err("letters: malloc");
	
	printf("enc: ");

	for(i = 0; i < len; i++) 
		//enc[i] = msg[i] ^ key[i]
		if(isspace(msg[i]))
			enc[i] = ' ';
		else 
			enc[i] = map[(msg[i] - 'a' + key[i] - 'a')%26];
						//      0 - 25   		 ^      0-25
	enc[i] = '\0';
	
	printf("%s\n", enc);
	
	
	printf("dec: ");
	
	for(i = 0; i < len; i++) 
		//enc[i] = msg[i] ^ key[i]
		if(isspace(enc[i]))
			dec[i] = ' ';
		else 
			dec[i] = map[(enc[i] - 'a' - key[i] - 'a')%26 > 0 ? (enc[i] - 'a' - key[i] - 'a')%26: (enc[i] - 'a' - key[i] - 'a')%26+26];
	
	dec[i] = '\0';
	
	printf("%s\n", dec);
	
}

void dec(char *ciphertext, char *key) {
	
	while(*ciphertext) {
		int c = *ciphertext - 'a';
		int k = *key - 'a';
		int x = (c - k)%26;
		if(x < 0)
			x+=26;
		x+='a';
		printf("%c", x);
		ciphertext++, key++;
		// printf("%d %d %d\n", c,k,x - 'a');
	}
	
}



int main () {
	
	printf("-5%%26 = %d\n", -5%26);
	
	/*
	char msg = 'a';
	unsigned key = rand()%128;
	
	
	printf("plaintext: %d\nciphertext: %d\ndecrypted: %d\n", msg, stream_enc(msg, key), stream_dec(stream_enc(msg, key), key));
	*/
	// printf("%c\n", tolower('a'));
	stream_on_letters("bsaspp kkuosp", "rsidpy dkawoa");
	
	dec("bsaspp kkuosp", "rsidpy dkawoa");

	return 0;
}