#include <iostream>
#include <algorithm>
#include <iomanip>
#include "mmu.h"

Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess()
{
    Process *proc = new Process();
    proc->pid = _next_pid;

    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->type = DataType::FreeSpace;
    var->virtual_address = 0;
    var->size = _max_size;
    proc->variables.push_back(var);

    _processes.push_back(proc);

    _next_pid++;
    
    return proc->pid;
}

void Mmu::addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address)
{
    int i;
    Process *proc = NULL;
    std::vector<Process*>::iterator it = std::find_if(_processes.begin(), _processes.end(), [pid](Process* p)
    { 
        return p != nullptr && p->pid == pid; 
    });

    if (proc != NULL)
    {
        Variable *var = new Variable();
        var->name = var_name;
        var->type = type;
        var->virtual_address = address;
        var->size = size;
        proc->variables.push_back(var);
    }
}

void Mmu::print()
{
    std::cout << " PID  | Variable Name | Virtual Addr | Size     " << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;

    for (int i = 0; i < _processes.size(); i++)
    {
        for (int j = 0; j < _processes[i]->variables.size(); j++)
        {
            Variable* var = _processes[i]->variables[j];

            // Assignment requirement: exclude FreeSpace from this table
            if (var->type != DataType::FreeSpace)
            {
                // PID (Width 4)
                std::cout << " " << std::setw(4) << _processes[i]->pid << " | ";

                // Variable Name (Width 13)
                std::cout << std::left << std::setw(13) << var->name << " | ";

                // Virtual Address (Hex format, 0x prefix, 8 chars, Width 12)
                std::cout << "0x" << std::setfill('0') << std::hex << std::uppercase 
                          << std::setw(8) << var->virtual_address << std::dec << std::setfill(' ') << " | ";

                // Size (Width 10, right aligned by default)
                std::cout << std::right << std::setw(10) << var->size << std::endl;
            }
        }
    }
}
Process* Mmu::getProcessFromPid(uint32_t pid){
    return _processes.at(pid-1024);
}
