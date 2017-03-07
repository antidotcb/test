#pragma once

#include "Decision.h"


namespace testapp {
namespace states {

class StateInterface
{
public:


    virtual void Init() = 0;
    virtual void Enter() = 0;
    virtual void Leave() = 0;
    virtual Decision Update() = 0;

    virtual ~StateInterface() = default;
};

}
}
