#include <iostream>
#include "bind_func.hpp"
#include "callback.h"
#include "functional"
#include <vector>

using namespace std;
using namespace std::placeholders;
//test gtest;
function <void (int)> templateCallbackVar;
vector <int> array;
test::test()
{
	cout<< "test constructore ...\n";
}
test::~test()
{
	cout << "Test distructore ... \n";
}
void abc (int a)
{
	cout << "abc a = " << a << endl;
}

test ts_obj;
void callback()
{
	cout << "CALLBACK TRIGGERED\n";
	function <void (int)> memVar = std::bind(&test::a, ts_obj);
	function <int (test	&)> var = &test::a;
//	var(ts_obj, 15);
	memVar(10);
	cout << __func__ << "   ===> a = " << ts_obj.a << endl;	
	function <void (int)> cb = std::bind(&test::class_calback, ts_obj, _1);
	cb(9); 
}
void test::class_calback(int ab)
{
	cout << __func__ << endl;
	cout << __func__ << " member variable a = " << a << endl;	
}
void test::cfuncInit()
{
	int err=-1;
	test ts;
	a = 123;		
	//templateCallbackVar = bind(&test::class_calback,ts, _1);
	cout << __func__ << " member variable a = " << a << endl;
	err = cInit(callback);
	cout << "cinit = "; err? cout << "Failed\n" : cout << "success\n";
}
int main()
{
	cout << "AATGC" << endl;
	test tst;
	tst.cfuncInit(); 
}
