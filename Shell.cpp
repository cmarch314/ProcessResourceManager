//Ho Choi 53734155

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "Manager.cpp"
#include <cstdlib>

using namespace std;

void parse(string input, string* result,Manager* m)
{
	string parsed[3];
	while(input.at(0) == ' ')
		input.erase(0);
	string token;
	size_t index;

	index = input.find(' ');
	if(index==std::string::npos)
	{
		token = (string)input;
	}
	else
	{
		token = input.substr(0,index);
		input.erase(0,index+1);
		parsed[0] = token;		
	}
//	cout<<"token:"<<token<<"\n";
	if (token.find("init") == 0)
	{
	/*
		init
		Restore the system to its initial state.
	*/
		*result +="\n";
		m->init();

	}
	else if(token.compare("cr") == 0)
	{
	/*
		cr <name> <priority>
		Invoke function Create(), which creates a new process <name> at the priority level <priority>; 
		<name> is a single character; 
		<priority> can be 1 or 2 (0 is reserved for Init process).
	*/

		string name;
		int priority;
		name = input.at(0);
		priority = (int)input.at(2);
		if(priority > '2' || priority < '0')
		{
			exception e;
			throw e;
		}
		priority = priority-48;
		m->Create(name,priority);

	}
	else if(token.compare("de")==0)
	{
	/*
	de <name>
	Invoke the function Destroy(), which destroy the process <name> and all of its descendants.
	*/
		parsed[1] = input.at(0);
		parsed[2] = "";

		m->Destroy(parsed[1]);
	}
	else if(token.compare("req")==0)
	{
	/*
	req <name>
	Invoke the function  Request(), which requests the resource <name>; <name> can be R1, R2, R3, or R4. 
	When implementing the additional task 5.1 (page 490), the command is extended to: 
	req <name> <# of units> 
	where <# of units> gives the number of units of  resource <name> to be requested.
	The numbers of units constituting each resource are as follows: 1 for R1, 2 for R2, 3 for R3, and 4 for R4.
	*/
		string resource;
		resource = input.substr(0,2);
		int cap = input.at(1);
		int unit = input.at(3);
		if(!(resource.compare("R1")==0||resource.compare("R2")==0||resource.compare("R3")==0||resource.compare("R4")==0))
		{
			exception e;
			throw e;	
		}
		parsed[1] = resource;			
		parsed[2] = unit;
		unit = unit-48;
		cap = cap-48;
		if(unit > cap || unit < 0)
		{
			exception e;
			throw e;
		}
		m->Request(resource,m->getRP(),unit);
		
		


	}
	else if(token.compare("rel")==0)
	{
	/*
	rel <name>
	Invoke the function Release(), which release the resource <name>. 
	When implementing the additional task 5.1 (page 490), the command is extended to: 
	rel <name> <# of units> 
	where <# of units> gives the number of units of  resource <name> to be released.
	*/
		string resource;
		resource = input.substr(0,2);
		int unit = input.at(3);
		int cap = input.at(2);

		if(resource.compare("R1")==0||resource.compare("R2")==0||resource.compare("R3")==0||resource.compare("R4")==0)
		{
			parsed[1] = resource;			
		}
		parsed[2] = unit;
		unit = unit-48;
		cap = cap-48;
		if(unit > cap || unit < 0)
		{
			exception e;
			throw e;
		}
		m->Release(resource,m->getRP(),unit);
		

	}
	else if(token.find("to")==0)
	{
	/*
	to
	Invoke the function Timeout().
	*/	
		m->timeOut();
	}
	else if(token.find("exit")==0)
	{
		cout<<"good bye"<<"\n";
	}
	else if(token.find("report")==0)
	{
		m->Report();
	}
	else
	{
		exception e;
		throw e;
	}
		
}


int main()
{
	string filename;

	ofstream outputtxt ("53734155.txt");

		cout<<"Enter txt name(or type \"exit\"):";
		getline(cin,filename);
		ifstream inputtxt (filename.c_str());
		if (!inputtxt)
		{
			cerr << "File could not be opened." << endl;
			return 0;
		}
		string input;
		string parsed[3]  = {"","",""};
		Manager *m = new Manager();
		string result = "";
		
		result += m->getRunning();
		result+= " ";
		while(getline(inputtxt,input))
		{
			try
			{
				//cout <<"input:"<<input<<"\n";
				parse(input,&result,m);
				result += m->getRunning();
				result+=" ";
			}
			catch(exception e)
			{
				result += "error ";
			}


		}
		result+="\n";
		cout<<result;
		outputtxt<<result;
	
	return 0;
}


