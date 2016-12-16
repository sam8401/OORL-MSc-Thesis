#include "relation.h"

relation :: relation(string p1, string p2, string p3) : name(p1),obj1(p2), obj2(p3)
{
}

void relation :: print()
{
	cout << name << " " << obj1 << " " << obj2 << endl ;
	cout << "n" << name << " " << obj1 << " " << obj2 << endl ;
}