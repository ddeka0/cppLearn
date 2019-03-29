#include <bits/stdc++.h>
#include <variant>
using namespace std;

class TaskRPCB {
	private:
		int x;
		std::function<void(int)> func;
	public:
		TaskRPCB(std::function<void(int)>&f , int x) {
			this->func = f;
			this->x = x;
		}
		void operator()() {
			printf("TaskRPCB function is executing...\n");
			func(x);
		}
};

class TaskECB {
	private:
		std::function<void(void)> func;
	public:
		TaskECB(std::function<void(void)>&f) : func(f) {}
		void operator()() {
			printf("TaskECB function is executing...\n");
			func();
		}
};

void F1() {
	cout <<"no x"<<endl;
}
void F2(int x) {
	cout <<"x : "<<x<<endl;
}

using myVariantType = std::variant<TaskECB, TaskRPCB>;

struct VisitPackage {
	void operator()(TaskECB & task) { 
		task();
	}
	void operator()(TaskRPCB& task) {
		task();
	}
};

int main() {
  queue<myVariantType> Q;

	function<void()>    func1   = F1;
	function<void(int)> func2   = F2;

	TaskECB task1(func1);
	TaskRPCB task2(func2,4);

	Q.emplace(task1);
	Q.emplace(task2);

	std::visit(VisitPackage(), Q.front());
	Q.pop();
	std::visit(VisitPackage(), Q.front());
	Q.pop();
}
