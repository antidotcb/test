#pragma once

#include <galaxy/GalaxyApi.h>
#include "State.h"


namespace testapp {
namespace states {

class SignIn : public State,
               galaxy::api::SelfRegisteringListener<galaxy::api::IAuthListener>,
               galaxy::api::SelfRegisteringListener<galaxy::api::IOperationalStateChangeListener>
{
public:
    SignIn(const std::string& login, const std::string& password);


protected:
    void Init() override;
    void Print() override;

    void OnAuthSuccess() override;
    void OnAuthFailure(FailureReason failureReason) override;
    void OnAuthLost() override;
    void OnOperationalStateChanged(uint32_t operationalState) override;

private:
    const std::string& username;
    const std::string& password;
};

} // namespace states
} // namespace testapp
