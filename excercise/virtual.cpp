#include "iostream"
using namespace std;

class interface 
{
	public:
		virtual void abc () = 0;
};
class base :public interface
{
	int a;
	public:
		void abc()
		{
			cout << "AATGC\n";
		}
};

class other 
{
public:
	void print ()
	{
		cout << "print\n";
		interface *in;
		in->abc();
	}
};

int main ()
{
	base *b = new base();
	b->abc();
		
	other oth;
	oth.print ();
	return 0;
}
