#include <iostream>
#include <algorithm>
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
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            // TODO: print all variables (excluding those of type DataType::FreeSpace)
        }
    }
}

Process* Mmu::getProcessFromPid(uint32_t pid){

    for( Process* p : _processes){
        if(p->pid == pid){
            return p;
        }
    }

    return nullptr;
}

void Mmu::removeProcess(uint32_t pid){

    int index = -1;

    for(int i=0; i<_processes.size(); i++){
        if(_processes[i]->pid == pid){
            index = i;
            break;
        }
    }

    if(index == -1){
        // Couldn't find pid 
        return;
    }

    _processes.erase(_processes.begin() + index);
}

void Mmu::removeVariable(uint32_t pid, std::string var_name){
    Process* p = getProcessFromPid(pid);

    for( int i=0; i<p->variables.size(); i++){
        if(p->variables.at(i)->name == var_name){
            // remove this variable from list, do I have to delete the pointer too?? idk
            p->variables.erase(p->variables.begin() + i);
        }
    }
}

Variable* Mmu::getVariable(uint32_t pid, std::string var_name){
    Process* p = getProcessFromPid(pid);

    for(Variable* v: p->variables){
        if(v->name == var_name){
            return v;
        }
    }

    return nullptr;
}