#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//y^2 = x^3 + a * x + b mod m

#define GEN_X 5
#define GEN_Y 1
#define EC_A 2
#define EC_B 2
#define EC_MOD 17 
#define INF -1

typedef struct {
	int x,y;
}Point;

typedef struct {
	int a,b,mod;
	Point gen, inf;
}Elliptic_curve;

typedef struct {
	int kpr;
	Point kpub;
	Elliptic_curve ec;
}user;

int eea(int a, int b, int *x, int *y) {
	
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

int inverse(int a, int m) {
	
	if(a < 0)
		a = (a + m) % m;
	if(a > m)
		a %= m;
		
	int x1, x2;
	eea(a,m,&x1,&x2);
	if(x1 < 0)
		x1 = (x1 + m) % m;
	return x1;
	
}

Point add_double(Point p, Elliptic_curve ec) {
	
	Point res;
	int s;
	
	s = (3*p.x*p.x + ec.a)*inverse(2*p.y, ec.mod);
	res.x = (s*s - p.x - p.x) % ec.mod;
	if(res.x < 0)
		res.x = (res.x + ec.mod) % ec.mod;
	res.y = (s*(p.x - res.x) - p.y) % ec.mod;
	if(res.y < 0)
		res.y = (res.y + ec.mod) % ec.mod;
	
	return res;	
}

Point add(Point p, Point q, Elliptic_curve ec) {
	
	Point res;
	int s;
	
	if(p.x == ec.inf.x && p.y == ec.inf.y)
		return q;
	if(q.x == ec.inf.x && q.y == ec.inf.y)
		return p;
	
	if(p.x == q.x && p.y == q.y)
		return add_double(p, ec);
	
	if(p.x == q.x && (p.y + q.y) % ec.mod == 0) {
		res.x = INF;
		res.y = INF;
		return res;
	}
		
	s = (q.y - p.y) * inverse(q.x - p.x, ec.mod) % ec.mod;
	if(s < 0)
		s = (s + ec.mod) % ec.mod;
	
	res.x = (s * s - p.x - q.x) % ec.mod;
	if(res.x < 0)
		res.x = (res.x + ec.mod) % ec.mod;
	res.y = (s * (p.x - res.x) - p.y) % ec.mod;
	if(res.y < 0)
		res.y = (res.y + ec.mod) % ec.mod;
	
	return res;
}

void init(Elliptic_curve *ec) {
	ec->a = EC_A;
	ec->b = EC_B;
	ec->mod = EC_MOD;
	
	ec->gen.x = GEN_X;
	ec->gen.y = GEN_Y;
	
	ec->inf.x = INF;
	ec->inf.y = INF;
}

void print_points(Elliptic_curve ec) {
	
	Point q, res;
	int count = 2;
	q = ec.gen;
	
	while(!(res.x == ec.inf.x && res.y == ec.inf.y)) {
		res = add(ec.gen, q, ec);
	
		printf("%dP : (%d, %d) + (%d, %d) = (%d, %d)\n", count++, ec.gen.x, ec.gen.y, q.x, q.y, res.x, res.y);
		
		q = res;
	}
}

Point n_times(Elliptic_curve ec, int n) {
	
	Point q, res;
	int count = 1;
	q = ec.gen;
	
	while(count < n) {
		res = add(ec.gen, q, ec);
		q = res;
		count++;
	}
	return res;
	
}

void init_user(user *u, Elliptic_curve ec) {
	u->ec = ec;
	u->kpr = rand() % (ec.mod - 2) + 2;
	u->kpub = n_times(ec, u->kpr);
}

Point user_n_times(Point kpub, int kpr, Elliptic_curve ec) {
	
	Point q, res;
	int count = 1;
	
	q = kpub;
	while(count < kpr) {
		res = add(kpub, q, ec);
		count++;
		q = res;
	}
	return res;
}

Point calculate_session_key(user u, Point kpub) {
	return user_n_times(kpub, u.kpr, u.ec);
}

int main () {
	
	srand(time(NULL));
	
	Elliptic_curve ec;
	init(&ec);
	
	/* 
	calc nP, where P is generator
	
	Point p, res, another_res, p_inv;
	
	int n;
	
	printf("Calc nP\n----\nEnter n: ");
	scanf("%d", &n);
	
	res = n_times(ec, n);
	
	printf("%dP = (%d, %d)\n", n, res.x, res.y);
	
	 */
	 
	 
	/*
	all points on the curve
	
	print_points(ec);
	*/
	
	
	//ECDH
	
	user Alice, Bob;
	
	init_user(&Alice, ec);
	init_user(&Bob, ec);
	
	Point Alice_calcs, Bob_calcs;
	
	printf("Alice: kpr %d, kpub (%d, %d)\n", Alice.kpr, Alice.kpub.x, Alice.kpub.y);
	printf("Bob: kpr %d, kpub (%d, %d)\n", Bob.kpr, Bob.kpub.x, Bob.kpub.y);
	
	Alice_calcs = calculate_session_key(Alice, Bob.kpub);
	Bob_calcs = calculate_session_key(Bob, Alice.kpub);
	
	printf("Alice calcs: (%d, %d)\nBob calcs: (%d, %d)\nKEY EXCHANGE SUCCESS == %s\n",
	Alice_calcs.x, Alice_calcs.y, Bob_calcs.x, Bob_calcs.y, (Alice_calcs.x == Bob_calcs.x && 
	Alice_calcs.y == Bob_calcs.y ? "TRUE" : "FALSE"));
	
	return 0;
}