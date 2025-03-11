#include <iostream>
#include <string>
#include <vector>

enum class Params{error, success, move};

class Logs
{
public:
	Logs()
	{
	}
	~Logs() = default;
	void add(std::string msg, int param)
	{
		std::string addParam {""};
		switch (param){
			case static_cast<int>(Params::error):
				addParam = "\x1b[31;1m";
				break;
			case static_cast<int>(Params::success):
				addParam = "\x1b[32;1m";
				break;
			case static_cast<int>(Params::move):
				addParam = "\x1b[33;1m";
				break;
			default:
				addParam = "";
			}
		logList.push_back(addParam + msg + addParam + "\n");
	}

	void check()
	{
		for(auto i : logList)
		{
			std::cout << i;
		}
	}
private:
	std::vector<std::string> logList {""};
};

class VM
{
public:
	VM(int inp_cpu_v, int inp_ram_v, std::string inp_name_v)
	{
		RAM_V = inp_ram_v;
		CPU_V = inp_cpu_v;
		name = inp_name_v;

	}
	~VM() = default;
	int RAM_V {};
	int CPU_V {};
	std::string name {""};
};

class Hosts
{
public:
	Hosts(int inp_cpu_h, int inp_ram_h)
	{
		RAM_H = inp_ram_h;
		CPU_H = inp_cpu_h;
		RAM_curr = inp_ram_h;
		CPU_curr = inp_cpu_h;
	}
	~Hosts() = default;
	bool add(VM vrM)
	{
		if(checkFree())
		{
			log1.add("Here's some space! Check if it's enough...", 1);
			if(checkEnough(vrM.RAM_V, vrM.CPU_V))
			{
				log1.add("Enough space! Adding...", 1);
				vMachines.push_back(vrM);
				RAM_curr -= vrM.RAM_V;
				CPU_curr -= vrM.CPU_V;
				return true;
			}
			else 
			{
				log1.add("Not enough space! Check if some VM can be replaced", 0);
				return false;
			}
		}
		else
		{
			log1.add("Here isn't any space!", 0);
			return false;
		}
	}

	bool checkFree()
	{
		if(RAM_curr > 0 && CPU_curr > 0) return true;
		else return false;
	}

	bool checkEnough(int ram, int cpu)
	{
		if(RAM_curr >= ram && CPU_curr >= cpu) return true;
		else return false;
	}

	void log_check()
	{
		log1.check();
	}


// private:
	int RAM_H {};
	int CPU_H {};
	int RAM_curr {};
	int CPU_curr {};
	std::vector<VM> vMachines {};
	Logs log1;
};


int main()
{
	std::vector <Hosts> hostList {};
	std::vector <VM> vmList {};
	Hosts host1(4, 16);
	Hosts host2(8, 32);
	hostList.push_back(host1);
	hostList.push_back(host2);
	VM vm1(8, 32, "vm1");
	VM vm2(4, 16, "vm2");
	vmList.push_back(vm1);
	vmList.push_back(vm2);

	for(int i = 0; i < vmList.size(); i++)
	{
		std::cout << "\x1b[37;0mTry to set " << vmList[i].name << " to... ";
		for(int j= 0; j < hostList.size(); j++)
		{
			std::cout << "\x1b[37;0mhost #\x1b[37;0m" << j + 1 << "...\n";
			if(hostList[j].add(vmList[i])) 
			{
				hostList[j].log_check();
				break;
			}
			else 
			{
				hostList[j].log_check();
				continue;
			}
		}
	}

	std::cout << "\nHost resources: \n";

	for(int k = 0; k < hostList.size(); k++)
	{
		std::cout << "Host #" << k + 1 << "\nCPU: " << hostList[k].CPU_curr << "\nRAM: " << hostList[k].RAM_curr;
		std::cout << "\n\nvMachines: ";
		for(int e = 0; e < hostList[k].vMachines.size(); e++)
		{
			std::cout << e + 1 << ") " << hostList[k].vMachines[e].name << "\n";
		}
	}


	return 0;
}
