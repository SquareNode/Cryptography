#include <stdio.h>
#include <stdlib.h>

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

int main () {
	
	char msg = 'a';
	unsigned key = rand()%128;
	
	
	printf("plaintext: %d\nciphertext: %d\ndecrypted: %d\n", msg, stream_enc(msg, key), stream_dec(stream_enc(msg, key), key));
	

	return 0;
}