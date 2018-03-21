#ifndef _service_handler_
#define _service_handler_

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "../objects/service.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class process_handler
{
    private:
        vector<service> service_list;

    public:
		void generate_services(ifstream &ifile, profile prf)
		{
		}
        void print_services()
        {
            cout<<setw(16)<<"Address"<<setw(16)<<"PID"<<setw(16)<<"PPID"<<setw(16)<<"Name"<<endl;
            services srv;
            for(int i = 0; i < process_list.size(); i++)
            {
                proc = process_list[i];
                cout<<setw(16)<<hex<<proc.physical_offset<<setw(16)<<dec<<proc.pid<<setw(16)<<proc.ppid<<setw(16)<<proc.name<<endl;
            }
        }
        
};

int main(void)
{
    service_handler sh;
    ifstream ifile;
	profile prf(7);
    char fname[] = "../data/samples/win764.vmem";
    
    ifile.open(fname, ios::in | ios::binary);
    if(!ifile)
	{
		cout<<"Error in opening file..!!";
	}	
	cout<<"File opened..";
	cout<<"\n";
    
    sh.generate_services(ifile, prf);
    sh.print_services();
}

#endif