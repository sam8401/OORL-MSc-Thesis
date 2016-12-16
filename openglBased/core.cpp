#include "core.h"

float magnitude(Vec2f x) 
{
	return sqrt(pow(x[0],2) + pow(x[1],2)) ; 
}

// check similarity of vectors
bool similar_vectors(Vec2f x, Vec2f y)
{
	return ((x[0] == y[0]) && (x[1] == y[1])) ;
}

void readData(vector <Vec2f> *p , const char * filename)
{ 
	string line ;
	int temp ;
	Vec2f v ;
	ifstream infile; 
	infile.open(filename, ifstream::in) ;
	if(infile!= NULL)
	{
		while(infile.good())
		{ 
			getline(infile,line);
			temp = line.find(' ');											
			v[0]= atoi(line.substr(0,temp).c_str());
			v[1] = atoi(line.substr(temp+1,line.length()-1).c_str()); 
			(*p).push_back(v);	
		}
		infile.close();
	}
}

vector<string> split_string(string s, char c)
{
	vector<string> result ;
	int len,   ind ;
	len = s.length() ; 
	while(1)
	{
		ind = s.find(c);
		if(ind == string::npos)   // word after the last occurrence of the fucking character.
		{
			result.push_back(s);
			break ;
		}
		else
		{
			result.push_back(s.substr(0,ind)) ;
			s = s.substr(ind+1,string::npos) ;
		}
	}
	return result ;
}

// useless function at the top
int getIndex(Vec2f val, vector<Vec2f> list) 
{
	for(int i=0;i<list.size();i++)
	{
		if(list[i][0] == val[0] && list[i][1] == val[1])
			return i ;
	}
}