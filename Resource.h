//Ho Choi 53734155

#ifndef Resource_H
#define Resource_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;
//this is RCB

class Process;

class Resource
{
public:

	Resource(string rid);
	Resource(string rid, int cap);
	~Resource();
	string getRID();
	int getFree();
	void allocate(int i);
	void release(int i);
	bool available(int i);
	int find(Process* p);
	vector<pair<Process*,int> >* getWatingList();
	void delProcessWL(Process* p);
private:
	string RID;

	int Cap;//# of free unit limit
	int freeR;// current # of free unit

	vector<pair<Process*,int> > Wating_List;
};

#endif