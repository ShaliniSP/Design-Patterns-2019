#include "interpreter.h"

int main(int argc, char const *argv[])
{
	vector<string> col_names{"A", "B", "C"};
	table t = table(col_names);
	map<string, string> m = {{"A", "a"}, {"B", "b"}, {"C", "c"}};
	map<string, string> n = {{"A", "x"}, {"B", "y"}, {"C", "z"}};
	t.display();
	t.add_row(m);
	t.display();
	t.add_row(n);
	t.display();
	t.del_row(1);
	t.display();
	return 0;
}