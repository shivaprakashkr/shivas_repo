#include "iostream"
using namespace std;

class interface 
{
	public:
		virtual void abc () = 0;
};

class test : public interface
{
  public:
  void abc()
  {
    cout << "Abc test interface\n";
  }
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
	base ba;
	interface *in;
	in = &ba;
	in->abc();

	test t1;
	in = &t1;
	in->abc ();
#if 0
	base *b = new base();
	b->abc();

	test t;

	t.abc();
		
	other oth;
	oth.print ();
#endif
	return 0;
}
