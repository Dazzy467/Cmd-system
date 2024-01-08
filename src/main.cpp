#include <iostream>
#include "tokenizer.h"
#include "event.h"
#include "cmdsys.h"

bool isRun = true;
float speed = 5;
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
    // Check if the event type is CMD_EVT_PROCESSED
    if (e.getEvtType() == "CMD_EVT_PROCESSED")
    {
        // Print the event ID, command, event type, and arguments
        // std::cout << "Id event of: " << e.getId() << std::endl;
        // std::cout << "Command: " << e.getCmd().command << std::endl;
        // std::cout << "EVT TYPE: " << e.getEvtType() << std::endl;
        // std::cout << "Arguments: ";
        // for (const auto& arg : e.getCmd().arguments)
        // {
        //     std::cout << arg << " ";
        // }

        if (e.getCmd().command == "say")
        {
            std::cout << "You said \"" << e.getCmd().arguments[0] << "\"\n\n";  
        }
        else if(e.getCmd().command == "set_speed")
        {
            if (e.getCmd().arguments.empty())
                std::cout << "Your speed is " << speed << std::endl << std::endl;
            else{
                speed = std::stof(e.getCmd().arguments[0]);
                std::cout << "Set speed to " << speed << std::endl << std::endl;

            } 
            
        }
        else if (e.getCmd().command == "exit")
        {
            std::cout << "Exiting!\n";
            isRun = false;  

        }

    }
    // Check if the event type is CMD_EVT_INVALID_ARG
    else
    {
        // Print the error message associated with the event
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    std::vector<Command> commands {
        {"set_speed",{ARG_TYPE::ARG_FLOAT},true},
        {"say",{ARG_TYPE::ARG_STRING},false,true},
        {"exit"}
        };
    
    
    eventSystem evtsys;
    cmdsys commandSys(commands,&evtsys,cmdHandler);

    std::string cmd;

    do{
        std::cout << "Input command: \n";
        std::getline(std::cin,cmd);
        commandSys.proccessCommand(cmd);
    }while(isRun);

    return 0;
}