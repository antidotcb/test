#include "Input.h"
#include <conio.h>
#include <unordered_map>
#include <sstream>
#include <thread>
#include <iostream>
#include <set>


namespace testapp {

Input::Action::Action(const Callback& _callback, const std::string& _desc)
    : callback(_callback), desc(_desc), valid(true) {}


void Input::Action::Execute() const {
    if ( valid && callback ) {
        callback();
    }
}


const std::string& Input::Action::Desc() const {
    return desc;
}


void Input::Setup(KeyBindings&& _bindings) {
    Bindings empty;
    bindings.swap(empty);

    for ( const auto& keybinding : _bindings ) {
        for ( const auto& key : keybinding.first ) {
            bindings.emplace(key, std::move(keybinding.second));
        }
    }
}


bool Input::bindingExists(uint16_t key) const {
    return bindings.find(key) != end(bindings);
}


void Input::Process() {
    if ( bindings.empty() ) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(200ms);
        return;
    }

    using shifttype = uint8_t;
    uint16_t key = static_cast<shifttype>(_getch());

    if ( key == MODIFIER ) {
        key |= static_cast<shifttype>(_getch()) << sizeof(shifttype) * 8;
    }

    if ( bindingExists(key) ) {
        bindings.at(key).Execute();
    }
}


std::string Input::KeyToString(const ExtendedKey key) {
    if ( key > ' ' && key < 255 ) {
        return std::string(1, static_cast<char>(key));
    }

    switch ( key ) {
        case DOWN: return "Down";
        case UP: return "Up";
        case ESCAPE: return "Esc";
        case ENTER: return "Enter";
        default: return "?";
    }
}


void Input::Print() {
    if ( bindings.empty() )
        return;

    std::unordered_map<std::string, std::set<uint16_t>> controls;
    for ( auto& keybinding: bindings ) {
        auto key = keybinding.first;
        auto& desc = keybinding.second.Desc();
        controls[desc].insert(key);
    }

    std::stringstream output;

    output << "[";
    auto first_ctrl = 0u;
    for ( auto& ctrl : controls ) {
        output << ( first_ctrl++ ? ", " : "" );

        auto first_key = 0u;
        for ( auto& key : ctrl.second ) {
            output << ( first_key++ ? " & " : "" );
            output << KeyToString(key);
        }
        if ( !ctrl.first.empty() ) {
            output << " - " << ctrl.first.c_str();
        }
    }
    output << "]" << std::endl;

    std::cout << output.str();
}

} // namespace testapp
