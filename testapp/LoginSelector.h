#pragma once

#include "State.h"


namespace testapp {
namespace states {

class LoginSelector : public State
{
public:
    struct Login
    {
        std::string user;
        std::string pwd;
    };

    LoginSelector();

    const Login& SelectedLogin() const;

protected:
    virtual void LoadUserCredentials();

    void Init() override;
    void Print() override;

private:
    std::vector<Login> logins;
    std::size_t selected;
};

} // namespace states
} // namespace testapp
