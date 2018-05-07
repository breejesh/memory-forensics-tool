#ifndef _registry_key_handler_
#define _registry_key_handler_

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <iomanip>
#include <algorithm>

#include "../objects/registry_key.cpp"
#include "../data/profiles.cpp"

using namespace std;

class registry_key_handler
{
  private:
    vector<registry_key> registry_key_list;

  public:
    vector<uint64_t> pool_scan_tag(ifstream &ifile, profile prf)
    {
        vector<uint64_t> phy_offsets;
        char nk_signature[] = {'n', 'k'};
        char check_pattern[2];
        uint64_t addr_val = 0;
        char current_pattern[8];
        while (ifile.eof() == 0)
        {
            ifile.read(current_pattern, 8);
            check_pattern[0] = current_pattern[4];
            check_pattern[1] = current_pattern[5];
            addr_val += 8;
            if (utility_functions::compare_array(check_pattern, nk_signature, 2))
            {
                phy_offsets.push_back(addr_val - 8);
                ifile.ignore(8);
                addr_val += 8;
            }
        }
        return phy_offsets;
    }

    registry_key collect_info_module(ifstream &ifile, profile &prf, uint64_t phy_offset)
    {
        registry_key rk;
        uint16_t type, name_len;
        ifile.clear();
        ifile.seekg(0, ios::beg);
        uint64_t addr_val = phy_offset, vir_file_addr, phy_file_addr;
        uint32_t size;

        ifile.seekg(phy_offset);
        ifile.read(reinterpret_cast<char *>(&size), 4);
        // cout << size << endl;
        if (size - 4294967040 > 0)
        {
            //cout << hex << size - 4294967040 << phy_offset << endl;
            ifile.seekg(phy_offset + 0x06);
            ifile.read(reinterpret_cast<char *>(&type), 1);
            if (type == 0x2c)
            {
                ifile.seekg(phy_offset + 0x4c);
                ifile.read(reinterpret_cast<char *>(&name_len), 2);
                char *name = new char[name_len];
                ifile.seekg(phy_offset + 0x50);
                ifile.read(name, name_len);
                //rk.name = name;
                //rk.name.erase(remove_if(rk.name.begin(), rk.name.end(), utility_functions ::invalidChar), rk.name.end());
                rk.physical_offset = phy_offset;
                cout << hex << setw(16) << phy_offset << setw(60) << name << endl;
            }
        }
        return rk;
    }

    void generate_registry_key_modules(ifstream &ifile, profile &prf)
    {
        vector<uint64_t> phy_offsets;
        ifile.clear();
        ifile.seekg(0, ios::beg);
        phy_offsets = pool_scan_tag(ifile, prf);
        cout << "Generating Key List " << endl;
        for (int i = 0; i < phy_offsets.size(); i++)
        {
            ifile.clear();
            ifile.seekg(0, ios::beg);
            registry_key_list.push_back(collect_info_module(ifile, prf, phy_offsets[i]));
        }
    }

    vector<registry_key> get_registry_key_list(ifstream &ifile, profile &prf)
    {
        if (registry_key_list.empty())
        {
            generate_registry_key_modules(ifile, prf);
        }
        return registry_key_list;
    }

    string get_info()
    {
        string json;
        json += "{ ";
        json += "\"registry_list\" : ";
        json += "[ ";
        for (int i = 0; i < registry_key_list.size(); ++i)
        {
            json += registry_key_list[i].get_info();
            if (i != registry_key_list.size() - 1)
                json += ",";
            json += "\n";
        }

        json += "] ";

        json += "} ";

        return json;
    }
};

#ifndef mainfunc
int main(void)
{
    registry_key_handler rkh;
    ifstream ifile;
    profile prf(7);
    char fname[] = "../data/samples/win764.vmem";
    vector<uint64_t> phy_offsets;
    ifile.open(fname, ios::in | ios::binary);
    if (!ifile)
    {
        cout << "Error in opening file..!!";
        exit(0);
    }
    cout << "File opened..";
    cout << "\n";

    //vector<uint64_t> phy_off = rkh.pool_scan_tag(ifile, prf);
    vector<registry_key> list = rkh.get_registry_key_list(ifile, prf);
}
#endif
#endif