#pragma once
#include "event.h"
#include "tokenizer.h"
#include <vector>
#include <deque>
#include <iostream>
#include "helper.h"

enum class ARG_TYPE
{
    ARG_STRING,
    ARG_INT,
    ARG_FLOAT,
    ARG_DOUBLE,
    ARG_BOOL
};

bool check_type(const std::string &str, const ARG_TYPE &type)
{
    if (type == ARG_TYPE::ARG_INT)
        return hp::isNumber(str);
    if (type == ARG_TYPE::ARG_FLOAT)
        return hp::isFloat(str);
    if (type == ARG_TYPE::ARG_DOUBLE)
        return hp::isDouble(str);
    if (type == ARG_TYPE::ARG_BOOL)
        return (str == "true" || str == "false");
    if (type == ARG_TYPE::ARG_STRING)
        return true;

    return false;
}

struct Command
{
    std::string command;
    std::vector<ARG_TYPE> arg_types;
    bool allow_no_argument = false;
    bool single_string_arg = false;
    std::vector<std::string> arguments;
};

class CommandEvent : public event
{
private:
    Command cmd;
    std::string message;
public:
    CommandEvent(const Command& _cmd,const std::string& msg = "Ok",int _id = -1)
    {
        this->id = _id;
        this->cmd = _cmd;
        this->message = msg;
    }
    

    const Command getCmd() const 
    {
        return this->cmd;
    }

    const char* what() const
    {
        return this->message.c_str();
    }
};


class cmdsys
{
private:
    std::vector<Command> Commands;
    eventSystem* evtSystem;
public:
    cmdsys(const std::vector<Command>& _Commands,eventSystem* _evts,const std::function<void(const CommandEvent&)>& handler)
    {
        this->Commands = _Commands;
        this->evtSystem = _evts;
        this->evtSystem->Bind<CommandEvent>("CMD_EVT_PROCESSED",handler);
        this->evtSystem->Bind<CommandEvent>("CMD_EVT_INVALID_NOTFOUND",handler);
        this->evtSystem->Bind<CommandEvent>("CMD_EVT_INVALID_ARG",handler);
        this->evtSystem->Bind<CommandEvent>("CMD_EVT_INVALID_ARGTYPE",handler);
    }
    ~cmdsys(){}

    int proccessCommand(const std::string& str_cmd)
    {
        // Tokenkan string dulu, abis itu cek command dan parameter nya
        std::deque<std::string> tokens;
        std::string cmd = str_cmd;
        cmd = tokenizer(str_cmd);
        while (!cmd.empty())
        {
            tokens.push_back(cmd);
            cmd = tokenizer();
        }
        
        if (tokens.empty())
        {
            this->evtSystem->Publish("CMD_EVT_INVALID_NOTFOUND",CommandEvent(Command({}),"No command entered!"));
            return -1;
        }


        cmd = tokens.at(0); 
        tokens.pop_front();

        Command publishedCommand;
        publishedCommand.command = cmd;
        for (auto& arg : tokens)
            publishedCommand.arguments.push_back(arg);

        // Proses command
        for (size_t i = 0; i < this->Commands.size(); i++)
        {
            if (cmd == this->Commands[i].command)
            {
                if(!this->Commands[i].allow_no_argument && !this->Commands[i].single_string_arg)
                {
                    // Cek ada berapa parameter
                    if (tokens.size() != this->Commands[i].arg_types.size())
                    {
                        this->evtSystem->Publish("CMD_EVT_INVALID_ARG",CommandEvent(this->Commands[i],"Invalid parameter, expecting " + std::to_string(this->Commands[i].arg_types.size()) + ", " + std::to_string(tokens.size()) + " given."));
                        return -1;
                    }
                    // Cek apakah tipe data parameter benar? (Error here)
                    for (size_t param_i = 0; param_i < tokens.size();param_i++)
                    {
                        if (!(check_type(tokens[param_i],this->Commands[i].arg_types[param_i])))
                        {
                            this->evtSystem->Publish("CMD_EVT_INVALID_ARGTYPE",CommandEvent(this->Commands[i]));
                            return -1;
                        }
                    }
                }

                else if (this->Commands[i].single_string_arg)
                {
                    if (!tokens.empty())
                    {
                        std::string arg_result = str_cmd;
                        size_t pos = arg_result.find(cmd);
                        arg_result = arg_result.substr(pos + cmd.length() + 1);
                        publishedCommand.arguments.clear();
                        // for(const auto& i : tokens)
                        //     arg_result += i;
                        publishedCommand.arguments.push_back(arg_result); 
                    }
                    else {
                        this->evtSystem->Publish("CMD_EVT_INVALID_ARG",CommandEvent(this->Commands[i],"Invalid parameter, expecting " + std::to_string(this->Commands[i].arg_types.size()) + ", 0 given."));
                        return -1;
                    }
                }
                this->evtSystem->Publish("CMD_EVT_PROCESSED",CommandEvent(publishedCommand));
                return 1;
            }
        }
        this->evtSystem->Publish("CMD_EVT_INVALID_NOTFOUND",CommandEvent(Command({cmd}),"Command " + cmd + " not found!"));
        return -1;
    }
};

