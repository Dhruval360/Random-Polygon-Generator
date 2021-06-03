#include <stack>
#include "Helpers.hpp"
using namespace std;

pair<double,double> justBelowTop(stack <pair<double,double>>& stk){
	pair<double,double> theTop = stk.top();
	stk.pop();
	pair<double,double> retVal = stk.top();
	stk.push(theTop);
	return retVal;
}