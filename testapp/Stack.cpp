#include "Stack.h"
#include "State.h"
#include "Error.h"


namespace testapp {
namespace states {

Stack::Stack(std::unique_ptr<StateInterface>&& active) {
    Push(std::move(active));
}


void Stack::Push(std::unique_ptr<StateInterface> state) {
    if ( !state )
        return;

    if ( !states.empty() ) {
        states.top()->Leave();
    }

    states.push(std::move(state));
    states.top()->Init();
    states.top()->Enter();
}


void Stack::Pop() {
    if ( !states.empty() ) {
        auto active = std::move(states.top());
        active->Leave();
        states.pop();
        if ( !states.empty() ) {
            states.top()->Enter();
        }
    }
}


void Stack::FatalError(const char* title, const char* msg) {
    while ( !states.empty() ) {
        Pop();
    }

    Push(std::make_unique<Error>(title, msg));
}


bool Stack::Update() {
    if ( states.empty() )
        return false;

    try {
        const auto& state = states.top();
        auto& decision = state->Update();

        if ( decision == Decision::Back ) {
            Pop();
        } else if ( decision != Decision::Stay ) {
            if ( !decision.keep_it ) {
                Pop();
            }
            Push(std::move(decision.next_state));
        }
    }
    catch ( Error& err ) {
        Pop();
        Push(std::make_unique<Error>(err.title, err.message));
    }
    catch ( std::exception& exception ) {
        FatalError("Exception", exception.what());
    }
    catch ( ... ) {
        FatalError("", "");
    }

    return !states.empty();
}

} // namespace states
} // namespace testapp
