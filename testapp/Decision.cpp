#include "Decision.h"
#include "StateInterface.h"


namespace testapp {
namespace states {

const Decision Decision::Back = { true, false, nullptr };
const Decision Decision::Stay = { false, false, nullptr };


Decision::Decision() noexcept
    : leave_it{ Decision::Stay.leave_it }, keep_it{ Decision::Stay.keep_it }, next_state{ Decision::Stay.next_state.get() } {}


Decision::Decision(bool _leave_it, bool _keep_it, std::unique_ptr<StateInterface> _next_state) noexcept
    : leave_it{ _leave_it }, keep_it{ _keep_it }, next_state{ std::move(_next_state) } {}


Decision::Decision(Decision&& other) noexcept
    : Decision() {
    if ( this == &other )
        return;
    std::swap(leave_it, other.leave_it);
    std::swap(keep_it, other.keep_it);
    std::swap(next_state, other.next_state);
}


Decision& Decision::operator=(Decision&& other) noexcept {
    if ( this == &other )
        return *this;
    std::swap(leave_it, other.leave_it);
    std::swap(keep_it, other.keep_it);
    std::swap(next_state, other.next_state);
    return *this;
}


bool Decision::operator==(const Decision& rhs) const {
    return leave_it == rhs.leave_it && keep_it == rhs.keep_it && next_state == rhs.next_state;
}


bool Decision::operator!=(const Decision& rhs) const {
    return !operator==(rhs);
}

}
}
