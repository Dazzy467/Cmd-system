#include <iostream>
#include "tokenizer.h"
#include "event.h"
#include "cmdsys.h"

class eventHello : public event
{
private:
    std::string msg;
public:
    eventHello(int _id = -1,const std::string& _msg = "hellow")
    {
        this->id = _id;
        this->msg = _msg;
    }

    const char* getMsg() const
    {
        return this->msg.c_str();
    }
};

// handle event
void helloEvent(const eventHello& e)
{
    std::cout << "Id event of: " << e.getId() << std::endl;
    std::cout << "With evtType of: " << e.getEvtType() << std::endl;
    std::cout << "Has a message of: " << e.getMsg();
}

void cmdHandler(const CommandEvent& e)
{
    std::cout << "Id event of: " << e.getId() << std::endl;
    std::cout << "Command: " << e.getCmd().command << std::endl;
    std::cout << "EVT TYPE: " << e.getEvtType() << std::endl;
    std::cout << "Arguments: ";
    for (auto i : e.getCmd().arguments)
        std::cout << i << " "; 
}

int main()
{
    std::vector<Command> commands {
        {"tell",{ARG_TYPE::ARG_STRING}},
        {"say",{ARG_TYPE::ARG_STRING}},
        };
    
    
    eventSystem evtsys;
    cmdsys commandSys(commands,&evtsys,cmdHandler);
    commandSys.proccessCommand("say hello");
    return 0;
}