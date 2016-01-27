//Ho Choi 53734155

#include "Manager.h"
using namespace std;


/*States
Running, Ready, None, Blocked, 
PCB process control block
*/
string Ready = "ready";
string Running = "running";
string Blocked = "blocked";

Manager::Manager()
{
	init();
}
void Manager::init()
{
//	cout<<"Manager->init\n";
	Resource_List.clear();
	Ready_List.clear();
	Running = NULL;
	
	Resource* R1 = new Resource("R1",1);
	Resource* R2 = new Resource("R2",2);
	Resource* R3 = new Resource("R3",3);
	Resource* R4 = new Resource("R4",4);
	Resource_List.reserve(4);
	Resource_List.push_back(R1);
	Resource_List.push_back(R2);
	Resource_List.push_back(R3);
	Resource_List.push_back(R4);

	Process* init = new Process("init",0);

	Process_List.push_back(init);
	Ready_List.reserve(3);
	Ready_List[0].push_back(init);

	Scheduler(init);
};
//None -> Ready, put it end of the queue
void Manager::Create(string pid,int priority)
{

//	cout<<"Manager->create\n";
	//checking for duplicate
	Process* p = getPCB(pid);
	if(p != NULL) // if it already exists
	{
		exception e;
		throw e;
	}
	p = new Process(pid,priority);
	
	if(Running != NULL)
	{
		Running->addChildren(p);
		p->setParent(Running);
	}
	else
	{
		Running = p;
	}
	Process_List.push_back(p);
	Ready_List[priority].push_back(p);
	if(Running->getPriority() < p->getPriority())
	{
		Running->setStatusType("Ready");
	}
	Scheduler(p);
};

//all -> None
void Manager::Destroy(string pid)
{

//	cout<<"Manager->Destroy\n";
	Process* p = getPCB(pid);
	if(p == NULL) // if it does not exists
	{
		exception e;
		throw e;
	}
	Process* parent = p->getParent();
	for(int i = 0 ; i < parent->getChildren()->size();i++)
	{
		if(parent->getChildren()->at(i)==p)
		{
			parent->getChildren()->erase(parent->getChildren()->begin()+i);	
		}
	}
	Kill_Tree(p);

	Scheduler(p);
};

void Manager::Kill_Tree(Process* p)
{
//	cout<<"Manager->Kill_Tree\n";
	p->setStatusType("Destroyed");
	for(int i = 0 ; i < Process_List.size();i++)
	{
		if (Process_List[i] == p)
		{
			Process_List.erase(Process_List.begin()+i);
		}
	}
	for(int i = 0 ; i < p->getChildren()->size();i++)
	{
		Kill_Tree(p->getChildren()->at(i));
	}
	int priority = p->getPriority();
	for(int i = 0 ; i < Ready_List[priority].size();i++)
	{
		if(Ready_List[priority][i] == p)
		{
			Ready_List[priority].erase(Ready_List[priority].begin()+i);
			break;
		}
	}
	Resource* tempR = NULL;
	//check the resource
	for(int i = 0 ; i < p->getOtherResources()->size();i++)//for each resource that p holds
	{
		tempR = p->getOtherResources()->at(i).first;
		int amount = p->getOtherResources()->at(i).second;
		Release(tempR->getRID(), p, amount);
	}
	p = NULL;
};

//running -> ready?
void Manager::Suspend()
{

};
//ready -> running?
void Manager::Activate()
{

};

//running -> blocked
void Manager::Request(string RID,Process* p,int i)
{

//	cout<<"Manager->Request\n";
	Resource* r = getRCB(RID);
	if(p->getPID().compare("init")==0)//init cannot hold resource;
	{
		exception e;
		throw e;
	}
	int index = p->findResourceIndex(r);
	if(r->available(i))//if possible
	{

//		cout<<"available\n";
		
		if(index != -1)
		{
			int pre_amount = p->getOtherResources()->at(index).second;
			r->release(pre_amount); //freeR-pre_amount
			i = i+pre_amount;
			p->delOther_Resources(r);

		}
		r->allocate(i); // u = u - n
		p->addOther_Resources(r,i); // add r into p's resource list
	}
	else// if not, block it, erase from ready list,put it in waiting list
	{
//		cout<<"not available\n";
		if(index != -1)
		{
			int pre_amount = p->getOtherResources()->at(index).second;
			r->release(pre_amount); //freeR-pre_amount
			i = i+pre_amount;
			p->delOther_Resources(r);
		}
		else
		{
			r->getWatingList()->push_back(make_pair(p,i));
		}
		//put
		//block
		p->setStatusType("Blocked");
		int priority = p->getPriority();
		//erase
		for(int j=0;j < Ready_List[priority].size();j++)
		{
			if(Ready_List[priority][j]==p)
			{
				Ready_List[priority].erase(Ready_List[priority].begin()+j);
			}

		}
	}
	Scheduler(p);
};


//blocked -> ready, put it end of the queue
void Manager::Release(string RID,Process*p,int i)
{

//	cout<<"Manager->Release\n";
	Resource* r = getRCB(RID);
	int index = p->findResourceIndex(r);
	if(index!=-1)
	{
		vector<pair<Resource*,int> >* tempL = p->getOtherResources();
		int temp_amount = (tempL->at(index).second);
		if(temp_amount == i)// if releasing all of resource
		{
			tempL->erase(tempL->begin()+index);//remove it from p.OtherResources
			r->release(i); // freeR = freeR+i
		}
		else if(temp_amount > i)
		{
			r->release(i);//freeR = freeR+i
		}
		else//temp_amound < i
		{
			//could be error?
		}

		if(r->getWatingList()->size() > 0)//check anything to release
		{
			int i = 0;
			while(true)//while loop for checking all of it.
			{
				Process* tempP = r->getWatingList()->at(0).first;
				temp_amount = r->getWatingList()->at(0).second;

				if(r->getFree() >= temp_amount) //if it is available
				{

					Ready_List[tempP->getPriority()].push_back(tempP);//put it on to ready list
					tempP->setStatusType("Ready"); //change the status to "Ready"
					tempP->addOther_Resources(r,temp_amount);
					r->allocate(temp_amount);
					r->getWatingList()->erase(r->getWatingList()->begin());
					if(r->getWatingList()->size() == 0)
					{
						break;
					}
				}
				else//if not available break the loop, this is FIFO
				{
					break;
				}
			}
		}
	}else
	{

	}
	Scheduler(p);
};


void Manager::Scheduler(Process* p)
{

//	cout<<"Manager->Scheduler\n";
	Process* h;
	h = Find_Highest_Process();//this will be at least "init"

	if(h->getPriority() >= p->getPriority()
		|| p->getStatusType().compare("Running")!=0
		|| p == NULL)
	{
		h->setStatusType("Running");
		Running = h;
	}
	

	//find highest priority.
	

};
Process* Manager::Find_Highest_Process()
{

//	cout<<"Manager->FHR\n";
	Process* h = NULL;
	for(int i = 2; i >= 0;i--)
	{	
		if(Ready_List[i].size() > 0)
		{
			h = Ready_List[i].at(0);
			return h;
		}
	}
	return h;
}
Resource* Manager::getRCB(string RID)
{

//	cout<<"Manager->getRCB\n";
	for(int i = 0 ; i < Resource_List.size();i++)
	{
		if (Resource_List[i]->getRID().compare(RID)==0)
		{
			return Resource_List[i];
		}
	}
	return NULL;
}
Process* Manager::getPCB(string PID)
{

//	cout<<"Manager->getPCB\n";
	for(int i = 0 ; i < Process_List.size();i++)
	{
		if (Process_List[i]->getPID().compare(PID) == 0)
		{
			return Process_List[i];
		}
	}
//	cout<<"null\n";
	return NULL;
};
string Manager::getRunning()
{

//	cout<<"Manager->getRunning\n";
	if(Running != NULL)
	{
		return Running->getPID();
	}
	return "NULL";
}
void Manager::Report()
{
	cout<<"\nManager->report \n -Process_List-\n";
	for(int i = 0 ; i < Process_List.size();i++)
	{
		cout<<i+1<<":"<<Process_List.at(i)->getPID()<<"("<<Process_List.at(i)->getPriority()<<")";
		if(Process_List.at(i)->getParent()!=NULL)
		{cout<<",Parent("<<Process_List.at(i)->getParent()->getPID()<<"),";}
		for (int j = 0; j < Process_List.at(i)->getChildren()->size();j++)
		{
			cout<<"Children ("<<Process_List.at(i)->getChildren()->at(j)->getPID()<<")";
		}
		cout<<",Status("<<Process_List.at(i)->getStatusType()<<") ";
		for (int j = 0; j < Process_List.at(i)->getOtherResources()->size();j++)
		{
			cout<<"Resource("<<Process_List.at(i)->getOtherResources()->at(j).first->getRID()<<"";
			cout<<","<<Process_List.at(i)->getOtherResources()->at(j).second<<") ";
		}

		cout<<"\n";
	}
	cout<<"-Resource_List-\n";
	
	for(int i = 0 ; i < Resource_List.size();i++)
	{
		cout<<i+1<<":"<<Resource_List.at(i)->getRID()<<"("<<Resource_List.at(i)->getFree()<<") - WL:";
		for(int j = 0; j < Resource_List.at(i)->getWatingList()->size() ; j++)
		{
			cout<<" "<<Resource_List.at(i)->getWatingList()->at(j).first->getPID();
			cout<<","<<Resource_List.at(i)->getWatingList()->at(j).second;
		}
		cout<<"\n";
	}
	cout<<" -Tree\n";
	for(int i = 0; i < 3 ; i++)
	{	
		cout<<"#"<<i<<": ";
		for(int j = 0 ; j < Ready_List[i].size(); j++)
		{
			cout<<Ready_List[i][j]->getPID()<<" ";
		}
		cout<<" end\n";
	}
	
	cout<<" -Running ("<<Running->getPID()<<")\n";
	
};
void Manager::timeOut()
{
	Running->setStatusType("Ready");
	int priority = Running->getPriority();
	Ready_List[priority].erase(Ready_List[priority].begin());
	Ready_List[priority].push_back(Running);
	Scheduler(Running);
}
Process* Manager::getRP()
{
	return Running;
};