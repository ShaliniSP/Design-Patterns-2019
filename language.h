#ifndef LANGUAGE
#define LANGUAGE
#include "interpreter.h"
class Language
{
	public:
		virtual ~Language() = 0;
		virtual void tokenize() =0 ;
		virtual vector<vector<string>> evaluate_query(Context &) = 0;
};

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

class REST_methods : public Language
{
	private:
		string query;
	 	vector<string> tokens;
	public:
		REST_methods(string);
		void tokenize();
		vector<vector<string>> evaluate_query(Context &);
		~REST_methods();
};
#endif