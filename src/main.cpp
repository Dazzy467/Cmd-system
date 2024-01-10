#include <iostream>
#include "tokenizer.h"
#include "event.h"
#include "cmdsys.h"

class app
{
public:
    bool isRun;
    float speed;
    std::string cmd;
    std::vector<Command> commands {
        {"sum",{ARG_TYPE::ARG_FLOAT,ARG_TYPE::ARG_FLOAT}},
        {"set_speed",{ARG_TYPE::ARG_FLOAT},true},
        {"say",{ARG_TYPE::ARG_STRING},false,true},
        {"exit"}
    };
    eventSystem* evtsys;
    cmdsys* commandSys;
public: 
    app(){
        isRun = true;
    }
    ~app(){
        delete evtsys;
        delete commandSys;
    }
public: // Init & Loop

    void Init()
    {
        evtsys = new eventSystem;
        commandSys = new cmdsys(commands,evtsys,[this](const CommandEvent& e){
            this->commandHandler(e);
        });
    }
    // Put this on main loop
    void Loop()
    {
        std::cout << "Input command: \n";
        std::getline(std::cin,cmd);
        commandSys->proccessCommand(cmd);
    }


public: // Handler
    void commandHandler(const CommandEvent& e)
    {
        // Check if the event type is CMD_EVT_PROCESSED
        if (e.getEvtType() == "CMD_EVT_PROCESSED")
        {
            if (e.getCmd().command == "say")
            {
                std::cout << "You said \"" << e.getCmd().arguments[0] << "\"\n\n";  
            }
            else if(e.getCmd().command == "sum")
            {
                float sum = std::stof(e.getCmd().arguments[0]) + std::stof(e.getCmd().arguments[1]);
                std::cout << "The result is " << sum << "\n\n";
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
            else std::cout << e.what() << std::endl;
        }
        else
        {
            // Print the error message associated with the event
            std::cout << e.what() << std::endl;
        }
    }

};

int main()
{
    app App;
    App.Init();
    do{
        App.Loop();
    }while(App.isRun);
    return 0;
}