#include "State.h"
#include "Controls.h"


namespace testapp {
namespace states {


void State::GoToNext(std::unique_ptr<StateInterface>&& state, bool keep_this) {
    decision.leave_it = true;
    decision.keep_it = keep_this;
    decision.next_state = std::move(state);
}


void State::GoToPrevious() {
    decision.leave_it = Decision::Back.leave_it;
    decision.keep_it = Decision::Back.keep_it;
    decision.next_state = nullptr;
}


void State::SetupKeys(Input::KeyBindings&& bindings) {
    controls.Setup(std::move(bindings));
}


void State::Print() {
    Controls::ClearScreen();
    controls.Print();
}


void State::Process() {
    controls.Process();
}


Decision State::Update() {
    Print();

    Process();

    auto result = std::move(decision);
    return result;
}

} // namespace states
} // namespace testapp
