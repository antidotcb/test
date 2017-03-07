#include "LoginSelector.h"
#include <iostream>
#include <string>
#include "Error.h"
#include "SignIn.h"


namespace testapp {
namespace states {

LoginSelector::LoginSelector()
    : selected{ 0 } {}


const LoginSelector::Login& LoginSelector::SelectedLogin() const {
    return logins[selected];
}


void LoginSelector::Init() {
    selected = 0;
    LoadUserCredentials();
    SetupKeys({
        { { Input::UP }, { [this]() {
                if ( selected > 0 ) {
                    selected--;
                }
            }, "Select" } },

        { { Input::DOWN }, { [this]() {
                if ( selected < logins.size() - 1 ) {
                    selected++;
                }
            }, "Select" } },

        { { Input::ENTER }, { [this]() {
                GoToNext(std::make_unique<SignIn>(SelectedLogin().user, SelectedLogin().pwd));
            }, "Login" } },

        { { Input::ESCAPE }, { [this]() {
                GoToPrevious();
            }, "Exit" } }
    });
}


void LoginSelector::LoadUserCredentials() {
    logins = {
        Login{ "TestGalaxyUserA@gog.com", "" },
        Login{ "TestGalaxyUserB@gog.com", "" },
        Login{ "!!!Incorrect_Login@test.com", "InvalidPassword" },
    };
}


void LoginSelector::Print() {
    State::Print();
    std::cout << "Select username to sign in: " << std::endl;

    auto login_no = 0;
    for ( auto& login : logins ) {
        std::cout << ( login_no++ == selected ? ">" : " " );
        std::cout << login.user << std::endl;
    }
}


} // namespace states
} // namespace testapp
