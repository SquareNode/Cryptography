#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BYTE 8
#define RAND rand()%300

//TODO: something is wrong when starting primes are larger
//rand()%400 overflows somewhere

typedef struct {
	unsigned int n, e;
}pair;

typedef struct {
	unsigned int private;
	pair public;
}user;

unsigned int pow_mod(unsigned int a, unsigned int b, unsigned int mod) {
	
	unsigned int res = 1;
	for(int i = 0; i < b; i++)
		res = res * a % mod;
	return res;
}

unsigned int sq_and_mul(unsigned int a, unsigned int b, unsigned int mod) {
	
	unsigned int mask, res;
	
	mask = 1 << (sizeof(unsigned int) * BYTE - 1);
	res = 1;
	
	while(mask & b == 0)
		mask >>=1;
	
	while(mask) {
		res = (res * res) % mod;
		if(mask & b)
			res = (res * a) % mod;
		
		mask >>= 1;
	}
	
	return res;
}

int gcd(unsigned int a, unsigned int b) {
	if(a == 0)
		return b;
	return gcd(b%a, a);
}

int eea(unsigned int a, unsigned int b, int *x, int *y) {
	
	if(b == 0){
		*x = 1;
		*y = 0;
		return a;
	}
	
	int x1,y1;
	int gcd = eea(b, a%b, &x1, &y1);
	
	*x = y1;
	*y = x1 - a/b * y1;
	return gcd;
	
}

unsigned int inverse(unsigned int a, unsigned int m) {
	
	int x1, x2;
	eea(a,m,&x1,&x2);
	if(x1 < 0)
		x1 = (x1 + m) % m;
	
	printf("Checking : a * x1 mod m ------ %u * %u mod %u = %u\n", a, x1, m, a * x1 % m);
	
	return x1;
	
}

int isPrime(unsigned int p) {
	if(p % 2 == 0)
		return 0;
	
	for(int i = 3; i * i <= p; i+=2) 
		if(p%i == 0)
			return 0;
	
	return 1;
}

void getKeys(user *u) {
	
	unsigned int p, q, e, phi, d;
	
	p = RAND;
	while(!isPrime(p))
		p = RAND;
	
	q = RAND;
	while(!isPrime(q))
		q = RAND;
	
	printf("p = %u, q = %u\n", p,q);
	
	phi = (p-1) * (q-1);
	
	e = RAND % (phi-2) + 1;
	while(gcd(e, phi) != 1)
		e = RAND % (phi-2) + 1;
	
	d = inverse(e, phi);
	// printf("d * e + phi * d1 = 1 == %u\n", d * e + phi * d1 == 1);
	// d %= phi;
	// if(d < 0)
		// d = (d + phi) % phi;
	// printf("d * e + phi * d1 = 1 == %u\n", d * e + phi * d1 == 1);
	
	u->public.n = p*q;
	u->public.e = e;
	u->private = d;
	
	// printf("checking (phi = %u): d * e mod phi = %u\n", phi, (d * e) % phi);
	
}

unsigned int encrypt(unsigned int cleartext, user u) {
	return sq_and_mul(cleartext, u.public.e, u.public.n);	
}

unsigned int decrypt(unsigned int ciphertext, user u) {
	return sq_and_mul(ciphertext, u.private, u.public.n);
}

int main () {
	srand(time(NULL));
	
	user Alice;
	
	getKeys(&Alice);
	
	unsigned int msg = 13, cipher, clear;
	cipher= encrypt(msg, Alice);
	clear = decrypt(cipher, Alice);
	
	printf("message: %u\n---\nencrypt: %u\n---\ndecrypt: %u\n", msg, cipher, clear); 
	
	printf("n = %u, e = %u, d = %u\n", Alice.public.n, Alice.public.e, Alice.private);
	
	// printf("2 ^ 30 %% 137 = %u\n", sq_and_mul(2,30,137));
	// unsigned int a = 13, b = 120;
	
	// unsigned int x, y, g = eea(a,b,&x, &y);
	
	// printf("%u %u %u\n", x, y, g);
	
	// printf("sizeof(unsigned int) = %d\n", sizeof(unsigned int));
	
	return 0;
}