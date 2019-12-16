#ifndef REST
#define REST

#include "language.h"
class REST_methods : public Language
{
	private:
		string query;
	 	vector<string> tokens;
	public:
		REST_methods(string);
		void tokenize();
		vector<vector<string>> evaluate_query(Context &);
		void display_result(Context ctx, string query, vector<vector<string>> result);
		~REST_methods();
};
#endif