#include "interpreter.h"
#include "language.h"
#include "expression.h"
#include "sql.h"
#include "rest.h"
#include "xml.h"

int main(int argc, char const *argv[])
{
	Database db= Database();

	cout<<"DATABASE SYSTEM\n\n";
	//Table 1
	vector<string> col_names{"A", "B", "C"};
	string table_name = "t";
	Table t = Table(table_name,col_names);
	map<string, string> m = {{"A", "a"}, {"B", "b"}, {"C", "c"}};
	map<string, string> o = {{"A", "x"}, {"B", "y"}, {"C", "z"}};
	map<string, string> n = {{"A", "p"}, {"B", "q"}, {"C", "r"}};
	map<string, string> p = {{"A", "h"}, {"B", "b"}, {"C", "j"}};
	t.add_row(m);
	t.add_row(n);
	t.add_row(o);
	t.add_row(p);
	db.add_table(table_name,t);



	//Table 2

	vector<string> col_names2{"srn", "name", "grade"};
	string table_name2 = "school";
	Table school = Table(table_name2,col_names2);
	map<string, string> x = {{"srn", "1"}, {"name", "John"}, {"grade", "B"}};
	map<string, string> y = {{"srn", "2"}, {"name", "Jane"}, {"grade", "A"}};
	school.add_row(x);
	school.add_row(y);
	db.add_table(table_name2,school);
	db.display_tables();


	//Evaluating queries:
	string choice = "1";
	string lang_name;
	string query;
	vector<vector<string>> result;

	Context ctx = Context(db);
	while(choice!= "4")
	{
		cout<<"\n\nMENU\n-------------\n(1)SQL-like\n(2)REST-like\n(3)XML-like\n(4)Exit\nChoose language :";
		getline(cin,choice);


		if(choice== "1")
		{
				cout<<"Enter query : ";
				//query ="SELECT * from t where B = b";
				getline(cin,query);
				//cin>>query;
				cout<<query;
				//query ="SELECT * FROM t";
				lang_name= "SQL";

				Language *l = new SQL(query);
				l->tokenize();
				result= l->evaluate_query(ctx);
				l->display_result(ctx, query, result);
		}
		else if(choice== "2")
		{
			cout<<"Enter query : ";
			getline(cin,query);
			cout << query;
			lang_name= "REST_methods";
			Language *l = new REST_methods(query);
			l->tokenize();
			result= l->evaluate_query(ctx);
			l->display_result (ctx,  query, result );
		}
		else if(choice== "3")
		{
			cout<<"Enter query : ";
			getline(cin,query);
			//cout << query;
			lang_name= "XML";
			Language *l = new XML(query);
			//l->tokenize();
			//result= l->evaluate_query(ctx);
			vector<vector<string>> temp = {{"a", "b", "c"}, {"d", "e", "f"}};
			l->display_result (ctx,  query, temp );
		}
		else if(choice=="4")
		{
			exit(1);
		}
		else
		{
			cout<<"\nWrong entry";
			continue;
		}
		display_result ( query, result );

		//ctx.db.display_tables();

	}



// 	string lang_name= "SQL";
// 	string query1 = "INSERT INTO t values A:d,B:i,C:y";
// //	string query2 = "SELECT * FROM t";
// 	string query2 = "DELETE FROM t WHERE B = b";
// 	//string query2 = "SELECT srn FROM school WHERE grade = A";
// 	//string query2 = "INSERT INTO school values srn:3,name:Please,grade:A";
// 	vector<vector<string>> result;
//
// 	if(lang_name=="SQL")
// 	{
// 		Language *l = new SQL(query1);
// 		l->tokenize();
// 		result= l->evaluate_query(ctx);
// 	}
//
// 	display_result ( query1, result );
// 	ctx.db.display_tables();
// 	ctx.clear();
//
// 	if(lang_name=="SQL")
// 	{
// 		Language *l = new SQL(query2);
// 		l->tokenize();
// 		result= l->evaluate_query(ctx);
// 	}
//
// 	display_result ( query2, result );
// 	ctx.db.display_tables();
// 	ctx.clear();
//
// 	lang_name= "REST_methods";
// 	//string query3 = "IN school POST DATA srn:5,name:Jack,grade:C";
// 	string query3 = "IN school GET srn SUCH THAT grade != B";
// 	//string query3 = "FROM t DELETE IF A = p";
//
// 	if(lang_name=="REST_methods")
// 	{
// 		Language *l = new REST_methods(query3);
// 		l->tokenize();
// 		result= l->evaluate_query(ctx);
// 	}
//
// 	display_result ( query3, result );
// 	ctx.db.display_tables();
// 	ctx.clear();

	return 0;
}
