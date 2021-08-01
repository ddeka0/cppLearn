##### Struct Reflection Library

##### Run:
	1. g++ -std=c++11 main.cpp -Wall -Wextra -Wwrite-strings
	2. ./a.out

##### Example:

```cpp
/// message_headers.hpp
enum En : uint8_t {
	TYPE_1 = 0,
	TYPE_2 = 1,
	TYPE_3 = 2
};

struct messageC {
	int x[5];
};

struct messageE {
	int k;
	float m;
};

struct messageD {
	double t;
	int v;
	messageE y[2];
};


struct messageB {
	int p;
	En q;
	messageC r;
	messageD s[3];
};


struct messageA {
	int a;
	double b;
	messageB c;
};

```


```cpp
/// extra_header.hpp -- User needs to add this

#include "reflection.hpp"
#include "message_headers.hpp"

REFLECTION(messageA,a,b,c);
REFLECTION(messageB,p,q,r,s);
REFLECTION(messageC,x);
REFLECTION(messageD,t,v,y);
REFLECTION(messageE,k,m);

```



```cpp
/// Driver code

int main() {

	json j;
	parseFile("input_a.json",j);

	messageA ma{};

	Fill(j,ma);

	Print(ma);

}

```


```json
/// For example input_a.json
{
	"a" : 12,
	"b" : 14.35,
	"c" : {
		"p" : 123,
		"q" : 1,
		"r" : {
			"x" : [12,10,5,2,3]
		},
		"s" : [
			{
				"t" : 23.45,
				"v" : 12,
				"y" : [
					{
						"k":100,
						"m": 3.4
					},
					{
						"k":200,
						"m": 6.4
					}
				]
			},
			{
				"t" : 3.4,
				"v" : 10,
				"y" : [
					{
						"k":10,
						"m": 5.4
					},
					{
						"k": 0,
						"m": 1.4
					}
				]
			},
			{
				"t" : 345.4,
				"v" : 1078,
				"y" : [
					{
						"k":1000,
						"m": 3.434
					},
					{
						"k":300,
						"m": 7.4
					}
				]
			}
		]
	}
}
```


Then,

```cpp
/// output from the main function

a : 12
b : 14.35
  p : 123
  q : 1
    x : [12,10,5,2,3]
  s : [
      t : 23.45
      v : 12
      y : [
          k : 100
          m : 3.4
         ,
          k : 200
          m : 6.4
      ]
     ,
      t : 3.4
      v : 10
      y : [
          k : 10
          m : 5.4
         ,
          k : 0
          m : 1.4
      ]
     ,
      t : 345.4
      v : 1078
      y : [
          k : 1000
          m : 3.434
         ,
          k : 300
          m : 7.4
      ]
  ]
  
```