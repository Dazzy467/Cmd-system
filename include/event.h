#pragma once
#include <unordered_map>
#include <functional>
#include <string>
class event
{
protected:
    int id;
    std::string evtType;
public:
    virtual ~event(){}
    friend class eventSystem;

    int getId() const
    {
        return this->id;
    }
    
    const std::string getEvtType () const
    {
        return this->evtType;
    }
};

class eventSystem
{
private:
    std::unordered_map<std::string,std::vector<std::function<void(const event&)>>> m_evtHandler;
public:
    template<typename T>
    void Bind(const std::string& evtType,const std::function<void(const T&)>& handler)
    {
        // Convert any event to standard event

        auto processedHandler = [handler](const event& e)
        {
            if (const T* evtObj = dynamic_cast<const T*>(&e))
                return handler(*evtObj);
        };

        this->m_evtHandler[evtType].push_back(processedHandler);
    }

    void Publish(const std::string& evtType,const event& evt)
    {
        const_cast<event&>(evt).evtType = evtType;
        auto it = this->m_evtHandler.find(evtType);
        if (it != this->m_evtHandler.end())
        {
            for (auto& handler : it->second)
                return handler(evt);
        }
    }
};
