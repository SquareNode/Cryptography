//galois field implementation

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BYTE 8
// #define DEBUG


//custom types

typedef enum {
	PRIME,
	EXTENDED
}type;

typedef struct{
	int n;
	int *el;
	type t;
}galois;

//error

void err(char *msg){
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


//checking whether numbers are prime or prime ^ x

int is_prime(int n) {
	int i, sq = (int)sqrt(n);
	for(i = 2; i <= sq; i++) 
		if(n%i == 0)
			return 0;
	return 1;
}

int is_prime_to_x(int n) {
	
	int primes_less_than_n[n/2], i, k = 0;
	
	for(i = 2; i < n; i++)
		if(is_prime(i))
			primes_less_than_n[k++] = i;
	for(i = 0; i < k; i++){
		int n_copy = n, count = 0;
		while(n_copy != 1 && n_copy%primes_less_than_n[i] == 0) {
			count++;
			n_copy/=primes_less_than_n[i];
		}
		if(pow(primes_less_than_n[i], count) == n) {
			#ifdef DEBUG
			printf("%d ^ %d = %d\n", primes_less_than_n[i], count, n);
			#endif
			return 1;
		}
	}	
	return 0;
}


int check_gal(int n) {
	
	if(is_prime(n) || is_prime_to_x(n))
		return 1;
	return 0;
}

//making galois field

galois mk_gal(int n) {
	galois res;
	int i;
	
	if(!check_gal(n))
		err("mkgal: can't make GF with n");
	
	res.n = n;
	res.el = malloc(n * sizeof(int));
	if(res.el == NULL)
		err("mkgal: malloc");
	for(i = 0; i < n; i++)
		res.el[i] = i;
	
	if(is_prime(n))
		res.t = PRIME;
	else
		res.t = EXTENDED;
	
	return res;
}

//printing GF(p^x), printing GF(p) is trivial

void print_pol(unsigned n) {
	// printf("%d\n\n", n);
	//n -> binary
	//print pol, coef are 0 1, binaries;
	unsigned mask = 1 << BYTE - 1; //sizeof(unsigned) * BYTE - 1;
	// while(n & mask != 1) 
		// mask >>= 1;
	printf("%d = ", n);
	int count = 7;
	while(mask) {
		if(mask & n) 
			if(count == 0)
				printf("1");
			else
				printf("x ^ %d + ", count);
		mask >>= 1;
		count--;
		
	}
	putchar('\n');
	
}

//addition

int gf_add(galois g, int a, int b) {
	
	if(g.t == PRIME)
		return (g.el[a] + g.el[b])%g.n;
	// else
		// print_pol((g.el[a] + g.el[b])%g.n);
	
}
	
//main

int main () {
	int i, g, el1, el2;
	galois m;
	
	scanf("%d", &g);
	m = mk_gal(g);
	printf("GF(%d)\nelements:\n", m.n);
	if(m.t == PRIME)
		for(i = 0; i < g; i++)
			printf("%d ", m.el[i]);	
	else
		for(i = 0; i < g; i++)
			print_pol((unsigned)m.el[i]);
	
	printf(m.t == PRIME ? "\nthe field is prime\n" : "\nthe field is extended\n");
	
	scanf("%d %d", &el1, &el2);
	
	if(el1 > m.n || el2 > m.n) 
		err("main: out of range");
	if(m.t == PRIME)
		printf("%d + %d = %d\n", m.el[el1], m.el[el2], gf_add(m,el1,el2));
	else {
		print_pol(m.el[el1]);
		print_pol(m.el[el2]);
		print_pol((m.el[el1] + m.el[el2])%m.n);
	}
	
	free(m.el);
	return 0;
}