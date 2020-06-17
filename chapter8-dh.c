#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MOD 10000000
#define BYTE 8

typedef struct{
	int p;
	int alfa;
}parameters;

typedef struct{
	parameters* params;
	int private;
	int public;
}user;

bool prime(int cand) {
	
	for(int i = 2; i * i < cand; i++)
		if(cand % i == 0)
			return false;
	
	return true;
}

int get_a_large_prime() {
	
	int candidate = rand() % MOD;
	
	while(!prime(candidate))
		candidate = rand() % MOD;
	
	return candidate;
}

int sq_and_mul(int a, int b, int mod){
	
	int res = 1;
	unsigned mask = 1 << (sizeof(int) * BYTE - 1);
	while(!(mask & b))
		mask >>= 1;
	while(mask){
		res = (res * res) % mod;
		if(mask & b)
			res = (res * a) % mod;
		mask >>= 1;
	}
	
	// int another_res = 1;
	// for(int i = 0; i < b; i++)
		// another_res = (another_res * a) % mod;
	
	// printf("%d\n", res==another_res);
	
	return res;
}

void init_user(user *u, parameters* p) {
	u->params = p;
	u->private = (rand() % (u->params->p - 4)) + 2;
	u->public = sq_and_mul(p->alfa, u->private, p->p);
}

void init(parameters* para) {
	para->p = get_a_large_prime();
	para->alfa = (rand() % (para->p - 4)) + 2;
}


int calculate_session_key(user u1, int public) {
	return sq_and_mul(public, u1.private, u1.params->p);
}

int main () {
	srand(time(NULL));
	
	parameters my_params;
	user Alice, Bob;

	init(&my_params);
	init_user(&Alice, &my_params), init_user(&Bob, &my_params);

	printf("p = %d, alfa = %d\n", my_params.p, my_params.alfa);
	printf("Alice: kpr = %d, kpub = %d\n", Alice.private, Alice.public);
	printf("Bob: kpr = %d, kpub = %d\n", Bob.private, Bob.public);
	
	int k1 = calculate_session_key(Alice, Bob.public);
	int k2 = calculate_session_key(Bob, Alice.public);
	
	printf("Alice calcs: %d,\nBob calcs: %d\nkey xchange success == %s", k1, k2, (k1==k2 ? "true" : "false"));
	
	return 0;
}