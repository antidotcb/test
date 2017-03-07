#pragma once

#include <memory>
#include "Input.h"
#include "StateInterface.h"


namespace testapp {
namespace states {

class State : public StateInterface
{
protected:
    virtual void Print();
    virtual void Process();

    void Enter() override {}
    void Leave() override {}

    void GoToNext(std::unique_ptr<StateInterface>&& state, bool keep_this = true);
    void GoToPrevious();

    void SetupKeys(Input::KeyBindings&& bindings);

private:
    Decision Update() override;

    Decision decision;
    Input controls;
};

} // namespace states
} // namespace testapp
