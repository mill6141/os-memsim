#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>
#include "mmu.h"
#include "pagetable.h"
#include <string_view>
#include <ranges>
#include <sstream>
#include <vector>
// 64 MB (64 * 1024 * 1024)
#define PHYSICAL_MEMORY 67108864

void printStartMessage(int page_size);
void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table);
void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table);
void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, uint8_t *memory);
void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table);
void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table);


int findFreeSpace(uint32_t pid, int bytesNeeded, Mmu *mmu, PageTable *page_table);
void moveFreeSpace(uint32_t pid, int bytes, Mmu *mmu, PageTable *page_table);
DataType findDtype(std::string s);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        std::cerr << "Error: you must specify the page size" << std::endl;
        return 1;
    }

    // Print opening instuction message
    int page_size = std::stoi(argv[1]);
    printStartMessage(page_size);

    // Create physical 'memory' (raw array of bytes)
    uint8_t *memory = new uint8_t[PHYSICAL_MEMORY];

    // Create MMU and Page Table
    Mmu *mmu = new Mmu(PHYSICAL_MEMORY);
    PageTable *page_table = new PageTable(page_size, PHYSICAL_MEMORY);

    // Prompt loop
    std::string command;
    std::cout << "> ";
    std::getline(std::cin, command);
    while (command != "exit")
    {
        // parts will have the seperate parts of the input
        // parts_count is the length of the parts array

        std::stringstream ss(command);
        std::string token;
        std::vector<std::string> parts;

        while (std::getline(ss, token, ' ')) {
            parts.push_back(token);
        }

        int parts_count = parts.size();
        // Handle command
        // TODO: implement this
        if(parts[0] == "create"){
            if(parts_count == 1){
                std::cout << "Invalid [Create] arguments\n";
            } else{
                createProcess(stoi(parts[1]),  stoi(parts[2]), mmu, page_table);
            }
        } else if(parts[0] == "allocate"){
            if(parts_count == 1){
                std::cout << "Invalid [allocate] arguments\n";
            } else{
                DataType dtype = findDtype(parts[3]);
                allocateVariable(stoi(parts[1]),  parts[2], dtype, stoi(parts[4]), mmu, page_table);
            }

        } else if(parts[0] == "set"){
            
        } else if(parts[0] == "free"){
            
        } else if(parts[0] == "terminate"){
            
        } else if(parts[0] == "print"){
            if(parts_count == 1){
                std::cout << "Invalid [print] arguments\n";
            } else{
                if(parts[1] == "page"){
                    page_table->print();
                }
            }
        }

        // Get next command
        std::cout << "> ";
        std::getline(std::cin, command);
    }

    // Cean up
    delete[] memory;
    delete mmu;
    delete page_table;

    return 0;
}

DataType findDtype(std::string s){
    if(s == "int"){
        return DataType::Int;
    } else if( s == "freespace"){
        return DataType::FreeSpace;
    } else if( s == "char"){
        return DataType::Char;
    } else if( s == "short"){
        return DataType::Short;
    } else if( s == "float"){
        return DataType::Float;
    } else if( s == "long"){
        return DataType::Long;
    } else if( s == "double"){
        return DataType::Double;
    } else{
        throw std::invalid_argument("Invalid datatype");
    }
}

void printStartMessage(int page_size)
{
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes." << std:: endl;
    std::cout << "Commands:" << std:: endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std:: endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)" << std:: endl;
    std::cout << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)" << std:: endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)" << std:: endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std:: endl;
    std::cout << "  * print <object> (prints data)" << std:: endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std:: endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std:: endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running" << std:: endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process" << std:: endl;
    std::cout << std::endl;
}

void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - create new process in the MMU
    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    //   - print pid

    //  - create new process in the MMU
    uint32_t new_pid = mmu->createProcess();

    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    allocateVariable(new_pid, "<TEXT>", DataType::Char, text_size, mmu, page_table);
    allocateVariable(new_pid, "<GLOBALS>", DataType::Char, data_size, mmu, page_table);
    allocateVariable(new_pid, "<STACK>", DataType::Char, 65536, mmu, page_table);

    //   - print pid
    std::cout << new_pid << "\n";
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    //   - if no hole is large enough, allocate new page(s)
    //   - insert variable into MMU
    //   - print virtual memory address

    
    // Find how much space is needed depending on datatype
    int bytesNeeded = num_elements;
    if(type == DataType::Char){
        bytesNeeded *= 1;
    } else if(type == DataType::Int || type == DataType::Float){
        bytesNeeded *= 4;
    } else if(type == DataType::Short){
        bytesNeeded *= 2;
    } else if(type == DataType::Long || type == DataType::Double){
        bytesNeeded *= 8;
    }

    // Now we need to find the first free space?
    int freeSpaceID = findFreeSpace(pid, bytesNeeded, mmu, page_table);

    if(freeSpaceID != -1){
        // Free space was found! Just insert here :D
        mmu->addVariableToProcess(pid, var_name, type, bytesNeeded, freeSpaceID);

        // I think something needs to be done here to move the FREESPACE variables starting address and size;
        moveFreeSpace(pid, bytesNeeded, mmu, page_table);
    } else{
        // No Free space big enough.... what to do here....
    }


}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, uint8_t *memory)
{
    // TODO: implement this!
    //   - look up physical address for variable based on its virtual address / offset
    //   - insert `value` into `memory` at physical address
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array)
}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove entry from MMU
    //   - free page if this variable was the only one on a given page
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    //   - free all pages associated with given process
}


int findFreeSpace(uint32_t pid, int bytesNeeded, Mmu *mmu, PageTable *page_table){
    Process* p = mmu->getProcessFromPid(pid);

    int found_space = -1;
    for (Variable* v : p->variables){
        if(v->type == FreeSpace && v->size >= bytesNeeded){
            found_space = v->virtual_address;
            break;
        }
    }

    printf("Found free space @ virtual address: %x\n", found_space); //Optional print, confirms this is working as intended (at least for [create])
    return found_space; // Will either return virtual address of free space, or -1 if there isn't any suitable
}

void moveFreeSpace(uint32_t pid, int bytes, Mmu *mmu, PageTable *page_table){
    Process* p = mmu->getProcessFromPid(pid);

    p->variables[0]->virtual_address += bytes; // variables[0] Should be free space
    p->variables[0]->size -= bytes;
}