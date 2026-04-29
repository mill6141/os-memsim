#include <algorithm>
#include "pagetable.h"
#include <iomanip>
#include <mmu.h>

PageTable::PageTable(int page_size, int physical_memory)
{
    _page_size = page_size;
    num_pages = physical_memory / page_size;
    used_pages = std::vector<bool>(num_pages);
}

PageTable::~PageTable()
{
}

std::vector<std::string> PageTable::sortedKeys()
{
    std::vector<std::string> keys;

    std::map<std::string, int>::iterator it;
    for (it = _table.begin(); it != _table.end(); it++)
    {
        keys.push_back(it->first);
    }

    std::sort(keys.begin(), keys.end(), PageTableKeyComparator());

    return keys;
}

void PageTable::addEntry(uint32_t pid, int page_number)
{
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    if(_table.count(entry) > 0){
        return; // already exists
    }

    int frame = -1; 
    // Find free frame
    for(int i=0; i<used_pages.size(); i++){
        if(used_pages.at(i) == false){
            used_pages.at(i) = true;
            frame = i;
            break;
        }
    }

    if(frame == -1){
        // No space left, throw error probably
        std::cout<< "Error: no memeory left \n";
        return;
    }

    _table[entry] = frame;
}

int PageTable::getPhysicalAddress(uint32_t pid, uint32_t virtual_address)
{
    // Convert virtual address to page_number and page_offset
    int page_number = virtual_address / _page_size;
    int page_offset = virtual_address % _page_size;

    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0)
    {
        int frame = _table[entry];
        address = (frame * _page_size) + page_offset;
    }

    return address;
}

void PageTable::print()
{
    int i;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    std::vector<std::string> keys = sortedKeys();

    for (i = 0; i < keys.size(); i++)
    {
        // Split "1024|3" into pid and page_number
        std::string key = keys[i];
        int delimiter = key.find('|');
        std::string pid_str = key.substr(0, delimiter);
        std::string page_str = key.substr(delimiter + 1);
        int frame = _table[key];

        std::cout << " " << std::setw(4) << pid_str
                  << " | " << std::setw(11) << page_str
                  << " | " << std::setw(12) << frame
                  << std::endl;
    }
}


int PageTable::getPageSize(){
    return _page_size;
}


void PageTable::removeEntry(uint32_t pid, int page_number) {
    std::string key = std::to_string(pid) + "|" + std::to_string(page_number);
    
    if (_table.count(key) > 0) {
        int frame = _table[key];
        used_pages.at(frame) = false; // Mark physical memory as free
        _table.erase(key);           // Remove from virtual lookup
    }
}

/*
void PageTable::freeAllPagesFromProcess(uint32_t pid){

    std::vector<std::string> keys = sortedKeys();

    for (int i = 0; i < keys.size(); i++)
    {
        // Split "1024|3" into pid and page_number
        std::string key = keys[i];
        int delimiter = key.find('|');
        std::string pid_str = key.substr(0, delimiter);
        std::string page_str = key.substr(delimiter + 1);
        int frame = _table[key];

        if(stoi(pid_str) == pid){
            // release the frame
            _table.erase(key);
            used_pages.at(frame) = false;
        } else{
            // Not the right process, ignore
            continue;
        }
    }

}

void PageTable::freePageIfRemovingOnlyVariable(uint32_t pid, std::string var_name, Mmu* mmu){
    Process* p = mmu->getProcessFromPid(pid);
    Variable* v = mmu->getVariable(pid, var_name);

    if(p == nullptr) return;
    if(v == nullptr) return;

    int first_page = v->virtual_address / _page_size;
    int last_page  = (v->virtual_address + v->size - 1) / _page_size;

    for(int i=first_page; i<=last_page; i++){
        bool dupe_page = false;
        // Loop through every other variable in the process, if none of them are between these pages, free them
        for( Variable* var: p->variables){
            if(var->name == var_name){
                continue;
            }

            int var_first_page = var->virtual_address / _page_size;
            int var_last_page  = (var->virtual_address + var->size - 1) / _page_size;
            if(var_first_page <= i && i <= var_last_page){
                dupe_page = true;
                break;
            }
        }

        if(!dupe_page){
            // Should be fine to delete page

            std::string key = std::to_string(pid) + "|" + std::to_string(i);
            int frame = _table[key];


            _table.erase(key);
            used_pages.at(frame) = false;
        }
    }


}
>>>>>>> origin/Jon

*/
