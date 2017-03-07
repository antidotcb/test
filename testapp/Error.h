#pragma once

#include <galaxy/GalaxyApi.h>
#include "State.h"


namespace testapp {
namespace states {

class Error : public State
{
public:
    explicit Error(const galaxy::api::IError& error);
    explicit Error(const std::string& title, const std::string& message = nullptr);

    const std::string title;
    const std::string message;

protected:
    void Init() override;
    void Print() override;
};

} // namespace states
} // namespace testapp
