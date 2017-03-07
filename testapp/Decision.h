#pragma once

#include <memory>


namespace testapp {
namespace states {

class StateInterface;

struct Decision
{
    static const Decision Back;
    static const Decision Stay;

    bool leave_it;
    bool keep_it;
    std::unique_ptr<StateInterface> next_state;

    Decision() noexcept;
    Decision(bool leave_it, bool keep_it, std::unique_ptr<StateInterface> next_state) noexcept;
    Decision(Decision&& other) noexcept;

    Decision& operator=(Decision&& other) noexcept;

    bool operator ==(const Decision& rhs) const;
    bool operator !=(const Decision& rhs) const;
};

}
}
