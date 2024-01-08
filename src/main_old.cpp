#include <iostream>
#include "tokenizer.h"
#include "event.h"
#include "cmdsys.h"

bool isRun = true;
float speed = 5;

void cmdHandler(const CommandEvent& e)
{
    // Check if the event type is CMD_EVT_PROCESSED
    if (e.getEvtType() == "CMD_EVT_PROCESSED")
    {
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
    // Check if the event type is CMD_EVT_INVALID
    else if (e.getEvtType() == "CMD_EVT_INVALID_ARGTYPE")
    {
        if (e.getCmd().command == "set_speed")
            std::cout << e.what() << " expecting type float!" << std::endl;
    }
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