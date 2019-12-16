#ifndef SQLLANG
#define SQLLANG

#include "language.h"
class SQL : public Language
{
	private:
		string query;
	 	vector<string> tokens;
	public:
		SQL(string);
		void tokenize();
		vector<vector<string>> evaluate_query(Context &);
		~SQL();
};

#endif