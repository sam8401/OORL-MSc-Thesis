// this class may not be necessary !
// try to get rid of this fucking class later
#include <string>
#include "core.h"
using namespace std ;

class relation {
	// in case it is single object, object 2 is it's attribute and relation name is 
	// it's value s
	public :  // think about making few of these private later
	string name ;    // name of the relation
	string obj1 ; // name of object 1
	string obj2 ;
	relation(string, string, string);
	void print();
};
