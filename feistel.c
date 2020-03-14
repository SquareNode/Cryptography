#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void err(char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

char* generate_key(int round) {
	
	int i, num, key_len = 8;
	char *res = malloc((key_len + 1) * sizeof(char));
	
	srand(round);
	num = rand();
	
	if(res == NULL)
		err("generate_key: malloc");
	
	for(i = 0; i < key_len; i++){
		if(num % 100 >= 50) 
			res[i] = num % 10 + '0';
		else 
			res[i] = num % 26 + 'a';
		num /= 10;
		if(num == 0)
			num = rand();
	}
	res[i] = '\0';
	return res;
}

void f(char *key, char *right) {
	int count = 0;
	while(*right) {
		*right = (*right + *key)/2;
		
		/*
		
		*right & *key are (0 - 9 | a - z | A - Z), adding? 
					       48      97     65   
		[48-57]U[65-90]U[97-122]
		
		[58-64]U[91-96] shouldn't be happening
		-10
		[48-54]U[81-86] -> alright, favors digits and uppercase
		solution: alternating between adding and subtracting 10
		
		*/
		
		if(!isdigit(*right) && !isalpha(*right)){
			if(count%2)
				*right-=10;
			else 
				*right+=10;
			count++;
		}
		
		key++;					
		right++;
	}
}

void swap(char *a, char *b) {
	
	char *tmp = malloc(strlen(a) * sizeof(char));
	if(tmp == NULL)
		err("malloc");
	
	strcpy(tmp, a);
	strcpy(a,b);
	strcpy(b, tmp);
	
	free(tmp);
	
}

void init(char **left, char ** right, char **right_copy, char *msg) {
	//not working...
	int i, msg_len = strlen(msg), k = 0;
	
	*left = malloc((msg_len/2 + 1) * sizeof(char));
	*right = malloc((msg_len/2 + 1) * sizeof(char));
	*right_copy = malloc((msg_len/2 + 1) * sizeof(char));
	
	if(left == NULL || right == NULL)
		err("malloc");
	
	for(i = 0; i < msg_len/2; i++)
		*left[i] = msg[i];
	
	*left[i] = '\0';
	
	for( ; i < msg_len; i++)
		*right[k++] = msg[i];
		
	*right[k] = '\0';
	
	strcpy(*right_copy, *right);
	
	printf("left: %s right: %s right_copy: %s\n", *left, *right, *right_copy);
	
}

void xor(char *a, char *b) {
	
	int len = strlen(a), i, k;
	char map[62];
	for(i = 0; i < 10; i++) 
		map[i] = i + '0';
	for(k = 0; i < 36; i++, k++)
		map[i] = 'a' + k;
	for(k = 0; i < 62; i++, k++)
		map[i] = 'A' + k;
	
	//	[48-57]U[65-90]U[97-122]
	
	for(i = 0; i < len; i++) {
		a[i] ^= b[i];
		
		/*
		hacking it back to digits and alphas
		
		1) bunch of stuff
			
		if(a[i] < 10)
			a[i]+='0';
		else if(a[i] < 36)
			a[i]+='A'-10;
		else if((a[i] >= 36 && a[i] <'0') || (a[i] > 57 && a[i] < 65) || 
		(a[i] > 90 && a[i] < 97))
			a[i]= a[i]%26 +'a';
		else if(a[i] > 122) 
			a[i]-=5;
		
		2) using map 
		*/
		
		if(!isdigit(a[i]) && !isalpha(a[i]))
			a[i] = map[a[i]%62];
		
	}
	// printf("xor result: %s\n", a);
		
}

void feistel_cipher(char *msg) {
	
	int rounds = 8;
	char *left, *right, *key, *right_copy;
	int i, msg_len = strlen(msg), k = 0;
	
	left = malloc((msg_len/2 + 1) * sizeof(char));
	right = malloc((msg_len/2 + 1) * sizeof(char));
	right_copy = malloc((msg_len/2 + 1) * sizeof(char));
	
	if(left == NULL || right == NULL)
		err("feistel_cipher: malloc");
	
	for(i = 0; i < msg_len/2; i++)
		left[i] = msg[i];
	
	left[i] = '\0';
	
	for( ; i < msg_len; i++)
		right[k++] = msg[i];
		
	right[k] = '\0';
	
	// init(&left,&right,&right_copy,msg);

	for(i = 0; i < rounds; i++) {
		key = generate_key(i);
		// printf("%d: %s %s %s\n", i, left, right, key);
		strcpy(right_copy, right);
		f(key, right_copy);
		xor(left, right_copy);
		swap(left, right);
	}
	
	printf("result: %s%s", left, right);
	free(key);
	free(left);
	free(right);
	free(right_copy);
}


int main () {
	
	feistel_cipher("Crypthography");

	return 0;
}