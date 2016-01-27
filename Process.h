//Ho Choi 53734155

#ifndef Process_H
#define Process_H

#include "Resource.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;
//this is PCB

class Process
{
public:
	Process(string pid, int p);
	Process(string pid, int p, Process* parent);
	~Process();

	void setPID(string s);
	string getPID();
	void setMemory();
	void addOther_Resources(Resource* r,int i);
	void delOther_Resources(Resource* r);
	void setStatusType(string s);
	void setStatusList(vector<Process*>*);

	void setCreation_Tree();
	void setPriority(int p);
	void setParent(Process* p);
	void addChildren(Process* p);
	Process* getParent();
	int getPriority();
	string getStatusType();
	
	int findResourceIndex(Resource* r);
	vector<pair<Resource*,int> >* getOtherResources();
	vector<Process*>* getChildren();

private:
	string PID;
	
	vector<pair<Resource*,int> > Other_Resources;
	
	string StatusType;//Type&List

	
	Process* Parent;
	vector<Process*> Children;

	//0,1,2(init,user,system) don't change
	string Property;

	vector<Process*>* listed;
	int Priority;

};

#endif
