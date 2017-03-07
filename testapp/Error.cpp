#include "Error.h"
#include <iostream>
#include <string>


namespace testapp {
namespace states {

Error::Error(const galaxy::api::IError& error)
    : Error{ error.GetName(), error.GetMsg() } {}


Error::Error(const std::string& _title, const std::string& _message)
    : title{ _title }, message{ _message } {}


void Error::Init() {
    SetupKeys({
        { { Input::ENTER, Input::ESCAPE }, { [this]() {
                GoToPrevious();
            }, "Return" } },
    });
}


void Error::Print() {
    State::Print();

    if ( !title.empty() ) {
        std::cerr << "Error: " << title << std::endl;
    } else {
        std::cerr << ( message.empty() ? "UNKNOWN " : "" ) << "ERROR" << std::endl;
    }

    if ( !message.empty() ) {
        std::cerr << "Message: " << message << std::endl;
    }
}


} // namespace states
} // namespace testapp
