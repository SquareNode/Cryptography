#include <stdio.h>
#include <stdlib.h>

void err(char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

typedef struct { 
	int num;
	int *memb;
} Ring;

int euclid(int a, int b) {
	if(b == 0)
		return a;
	return euclid(b, a%b);
}

int extended_euclid(int a, int b, int *x, int *y, Ring r) {
	//ax + by = gcd(a,b)
	//I need only a, and positive a
	// -7*2 + 1*15 = 1 = gcd(2,15)
	// 8*2 - 1*15 = 1 = gcd(2,15)
	// x*2 = gcd(2, 15) + 1*15
	
    if(b == 0) {
       *x = 1;
       *y = 0;
       return a;
    }

    int x1, y1, gcd = extended_euclid(b, a % b, &x1, &y1, r);
    *x = y1;
	//*y = x1 - (a/b) * y1;
    *y = x1 - mul((a / b), y1, x);
	
    return gcd;
}


int find_inverse(int el, int n) {
	//el * res ≡ 1 mod n
	//el * res - 1 ≡ 0 mod n
	//el * res - 1 = q * n , q = {1,2,3...}
	//el * res - q * n = 1 , q = {1,2,3...}
	int i;
	for(i = 0; i < n; i++)
		if(el * i % n == 1)
			return i;
	
}

Ring mkring(int n) {
	int i;
	
	if(n <= 0)
		err("cannot make ring out of negative num or zero");
	
	Ring res;
	res.num = n;
	res.memb = malloc(n * sizeof(int));
	if(res.memb == NULL)
		err("mkring: malloc");
	for(i = 0; i < n; i++)
		res.memb[i] = i;
	return res;
	
}

void print(Ring ring) {
	int i;
	
	printf("n: %d\n", ring.num);
	printf("Ring members: ");
	for(i = 0; i < ring.num; i++)
		printf("%d ", ring.memb[i]);
	putchar('\n');
	
}

int add(int a, int b, Ring r) {
	if(a >= r.num || b >= r.num)
		err("number not in ring");
	return (a+b)%r.num;
}

int mul(int a, int b, Ring r) {
	if(a >= r.num || b >= r.num)
		err("number not in ring");
	return a*b%r.num;
}


int main () {
	
	int n,a,b,i;
	scanf("%d", &n);
	
	Ring ring = mkring(n);

	print(ring);
	scanf("%d %d", &a, &b);
	
	printf("ring %d:\n%d + %d = %d\n%d * %d = %d\n", ring.num, a, b, add(a,b, ring), a,b, mul(a,b, ring));
	
	for(i = 0; i < n; i++) {
		int x, y;
		if(extended_euclid(ring.memb[i], n, &x, &y, ring) == 1)
			printf("%d inverse: %d\n", ring.memb[i], x);
	}
	
	/* 
	for(i = 0; i < n; i++) 
		if(euclid(ring.memb[i], n) == 1)
			printf("%d inverse: %d\n", ring.memb[i], find_inverse(ring.memb[i], n));
	
	free(ring.memb);
	 */
	/*
	int x,y;
	int g = extended_euclid(a,b,&x,&y);
	printf("gcd(%d, %d) = %d, %d*%d + %d*%d = %d\n", a,b, g, a, x, b, y, g);
	*/
	return 0;
}