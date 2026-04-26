#include <algorithm>
#include "pagetable.h"
#include <iomanip>

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
