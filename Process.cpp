//Ho Choi 53734155

#include "Process.h"
using namespace std;

Process::Process(string pid, int p)
{
	Parent = NULL;
	setPID(pid);
	setStatusType("Ready");
	setPriority(p);
	Other_Resources.reserve(1);
};

void Process::setPID(string s){
	PID = s;
};
string Process::getPID(){
	return PID;
};


void Process::delOther_Resources(Resource* r)
{
	for(int i = 0 ; i < Other_Resources.size();i++)
	{
		if(Other_Resources.at(i).first == r)
		{
			Other_Resources.erase(Other_Resources.begin()+i);
		}
	}
};
void Process::addOther_Resources(Resource* r,int i)
{
	Other_Resources.push_back( make_pair(r,i) );
};
void Process::setStatusType(string s)
{
	StatusType = s;
};
void Process::setCreation_Tree(){};
void Process::setPriority(int p){
	Priority = p;
};
void Process::setParent(Process* p)
{
	Parent = p;
};
void Process::addChildren(Process* p)
{
	Children.push_back(p);
};

vector<Process*>* Process::getChildren()
{
	return &Children;
};

int Process::getPriority()
{
	return Priority;
};

string Process::getStatusType()
{
	return StatusType;
};

vector<pair<Resource*,int> >* Process::getOtherResources()
{
	return &Other_Resources;
};


int Process::findResourceIndex(Resource* r)
{
	for(int i = 0; i < Other_Resources.size();i++)
	{
		if(Other_Resources.at(i).first == r)
		{
			return i;
		}
	}
	return -1;
};

Process* Process::getParent()
{
	return Parent;
};