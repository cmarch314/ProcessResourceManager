//Ho Choi 53734155
#include "Resource.h"
using namespace std;
Resource::Resource(string rid)
{
	RID = rid;
	Cap = 1;
	freeR = 1;
};

Resource::Resource(string rid, int cap)
{
	RID = rid;
	Cap = cap;
	freeR = cap;
};
Resource::~Resource()
{

};
string Resource::getRID()
{
	return RID;
}
int Resource::find(Process* p)
{
	for(int i = 0; i < Wating_List.size();i++)
	{
		if(Wating_List.at(i).first == p)
		{
			return i;
		}
	}
	return -1;
}

vector<pair<Process*,int> >* Resource::getWatingList()
{
	return &Wating_List;
};

bool Resource::available(int i)
{
	if(i <= freeR && Wating_List.size() == 0)
	{
		return true;
	}
	return false;
};
void Resource::allocate(int i)
{
	freeR = freeR-i;
};
void Resource::release(int i)
{
	freeR = freeR+i;
}

void Resource::delProcessWL(Process* p)
{
	for(int i = 0 ; i < Wating_List.size();i++)
	{
		if(Wating_List.at(i).first == p)
		{
			Wating_List.erase(Wating_List.begin()+i);
		}
	}
};
int Resource::getFree()
{
	return freeR;
}