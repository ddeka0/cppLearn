#include <bits/stdc++.h>
using namespace std;

class decrementReducer {
protected:
  void reduce(int &value) { value--; }
};

class halfReducer {
protected:
  void reduce(int &value) { value / 2; }
};

class flushReducer {
protected:
  void reduce(int &value) { value = 0; }
};

template <typename ReductionPolicy>
class Foo : private ReductionPolicy {
public:
	using ReductionPolicy::reduce;
  	void doReduct(int &reduce_me) {
    	reduce(reduce_me);
  	}
};

int main() {
  typedef Foo<flushReducer> FooFlusher;
  typedef Foo<decrementReducer> FooDecrementer;
  typedef Foo<halfReducer> FooHalver;
  FooFlusher foo_flusher;
  int value = 42;
  foo_flusher.doReduct(value);  /* value is now zero */

  FooDecrementer foo_decrementer;
  int value2 = 42;
  foo_decrementer.doReduct(value2);  /* value2 is now 41*/

  FooHalver foo_halver;
  int value3 = 42;
  foo_halver.doReduct(value3);  /* value3 is now 21*/
}