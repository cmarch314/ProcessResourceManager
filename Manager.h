//Ho Choi 53734155

#ifndef Manager_H
#define Manager_H
#include <iostream>
#include <string>
#include <vector>
#include "Process.cpp"
#include "Resource.cpp"
#include <exception>


using namespace std;

class Manager 
{

public:
	Manager();
	~Manager();

	void init();
	void Create(string pid,int priority);
	void Destroy(string pid);
	void Kill_Tree(Process* p);
	void Suspend();
	void Activate();

	void Request(string RID,Process* p,int i);

	void insert();
	
	void Release(string RID,Process* p,int i);
	void Scheduler(Process* p);
	void timeOut();
	string getRunning();
	Process* getRP();
	Resource* getRCB(string RID);
	Process* getPCB(string PID);
	Process* Find_Highest_Process();

	void Report();
private:
	vector< vector< Process* > > Ready_List;
	vector<Process*> Process_List;
	vector<Resource*> Resource_List;
	Process* Running;

};

#endif