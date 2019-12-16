#ifndef LANGUAGE
#define LANGUAGE

#include "interpreter.h"
#include "expression.h"
#include "predicates.h"
class Language
{
	public:
		virtual ~Language() = 0;
		virtual void tokenize() =0 ;
		virtual vector<vector<string>> evaluate_query(Context &) = 0;
		virtual void display_result(Context ctx, string query, vector<vector<string>> result) = 0;
};

#endif