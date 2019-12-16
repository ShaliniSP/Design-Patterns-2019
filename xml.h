#ifndef XMLLANG
#define XMLLANG
#include "language.h"
class XML : public Language
{
	private:
		string query;
	 	vector<string> tokens;
	public:
		XML(string);
		void tokenize();
		vector<vector<string>> evaluate_query(Context &);
		void display_result(Context ctx, string query, vector<vector<string>> result);
		~XML();
};
#endif