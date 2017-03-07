#include "SignIn.h"
#include "Error.h"
#include "LobbyList.h"
#include "Controls.h"


namespace testapp {
namespace states {

using namespace galaxy::api;


SignIn::SignIn(const std::string& _username, const std::string& _password)
    : username{ _username }, password{ _password } {}


void SignIn::Print() {
    State::Print();
    Controls::Username(username);
    Controls::Progressbar("Signing in");
}


void SignIn::Init() {
    try {
        User()->SignIn(username.c_str(), password.c_str());
    }
    catch ( galaxy::api::IError& error ) {
        throw Error(error);
    }
}


void SignIn::OnAuthSuccess() {
    GoToNext(std::make_unique<LobbyList>(username), false);
}


void SignIn::OnAuthFailure(FailureReason failureReason) {
    const std::map<SignIn::FailureReason, const char*> FAILURE_REASONS =
    {
        { FAILURE_REASON_UNDEFINED, "Undefined error." },
        { FAILURE_REASON_GALAXY_SERVICE_NOT_AVAILABLE, "Galaxy Service is not installed properly or fails to start." },
        { FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN, "Galaxy Service is not signed in properly." },
        { FAILURE_REASON_CONNECTION_FAILURE, "Unable to communicate with backend services." },
        { FAILURE_REASON_NO_LICENSE, "User that is being signed in has no license for this application." },
        { FAILURE_REASON_INVALID_CREDENTIALS, "Unable to match client credentials (ID, secret) or user credentials (username, password)." },
        { FAILURE_REASON_GALAXY_NOT_INITIALIZED, "Galaxy has not been initialized." },
    };

    GoToNext(std::make_unique<Error>("Authentification failed.", FAILURE_REASONS.at(failureReason)), false);
}


void SignIn::OnAuthLost() {
    GoToNext(std::make_unique<Error>("Authentification have been lost."), false);
}


void SignIn::OnOperationalStateChanged(uint32_t operationalState) {
    auto logged_on = operationalState & OPERATIONAL_STATE_LOGGED_ON;
    auto signed_in = operationalState & OPERATIONAL_STATE_SIGNED_IN;
    if ( !logged_on || !signed_in ) {
        GoToNext(std::make_unique<Error>("Operational state", "Signed out / Logged out"), false);
    }
}

} // namespace states
} // namespace testapp
