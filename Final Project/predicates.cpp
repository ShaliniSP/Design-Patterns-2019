#include "predicates.h"

isEqual::isEqual(string s) : filter_string(s){}

bool isEqual::operator()(string check)
{
	return filter_string==check;
}

isNotEqual::isNotEqual(string s) : filter_string(s){}

bool isNotEqual::operator()(string check)
{
	return filter_string!=check;
}

startsWith::startsWith(string s) : filter_string(s){}

bool startsWith::operator()(string check)
{
	return check.find(filter_string)==0;
}