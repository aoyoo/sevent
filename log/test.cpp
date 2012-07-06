#include <iostream>
#include <fstream>

using namespace std;


int main()
{
	ofstream of;
	of.open("1.txt", ios::app);
	streambuf *buf = clog.rdbuf(of.rdbuf());
	clog << "hello, strembuf " << endl;
	of.flush();
	clog.rdbuf(buf);
	of.close();
	return 0;
}
