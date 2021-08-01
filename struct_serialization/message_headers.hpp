#pragma once

enum En : uint8_t {
	TYPE_1 = 0,
	TYPE_2 = 1,
	TYPE_3 = 2
};

struct messageC {
	int x[5];
};

struct messageD {
	double t;
	int v;
};


struct messageB {
	int p;
	En q;
	messageC r;
	messageD s[2];
};


struct messageA {
	int a;
	double b;
	messageB c;
};
