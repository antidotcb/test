# Documentation

# Build

You cannot build it without gogsdk. Write to GOG.com team to receive it.

# Usage

Simply run executable. Galaxy*.dll files should be copied by build-system into exe-folder.
Each screen provides hint about how to use app (use arrows for “Up & Down”). I didn’t finalize asynchronous key state polling, and as result you must eventually update Major states pressing any key not bind to controls (I prefer Space).
I hope it's not too complex. In case of questions you can reach me via email: antidotcb@gmail.com. 


# Architecture

I started with simple straight forward implementation in one main.cpp file. But soon I understood it would be much overweighed, and I had decided to split a bit of logic into state machine. You see what’s happened after. The project grows bigger each day.
I started implementation of state machine (you will see classes - State, State Interface), and realized there is no need in full state machine, as I only required some simple Stack of states, one over another with possibility to store & return to previous state.
Each state have following methods, as described in State Interface:
 * Init - executed once, when added to Stack
 * Enter - each time state is activated 
 * Leave - each time state is leaved
 * Update - repeatedly called by Stack, and this method returns Decision, about what to do next (stay at current state, return to previous or pointer to next state). Decision have one important flag, named "keep it", which gives hints to Stack to keep it in stack or not.

## Major states:
 * Login selector - provides possibility to select which login to use (and one incorrect one, just for testing purposes)
 * Lobby list - updates list of lobbies by request, and provides possibility to select whatever to create new lobby or join existing one
 * Lobby main - a simple chat interface with possibility to "whisper" messages to members of lobbies (using P2P messages), or send message to entire lobby (using Matchmaking module of GOG SDK)

## Transition states:
 * Sign In - displays "progress bar" while user is signed in with selected credentials
 * Lobby load - this one joins lobby. If no lobby was provided at construction time - creates one.

## helper state:
 * Error - simply displays error

Also there is a Background Task (implemented using simple std::thread) which provides polling of GOG SDK for events via call to galaxy::api::ProcessData().

# Improvements
 * Protect all containers access with std::mutex & std::lock_guard. Right now due to sync key state it’s very unfortunate to happen.
 * Add more error handling
 * Implement async key state read, and make update states after GOG SDK callback & key presses only
 * Create ClearScreen Linux version, and make solution totally cross platform via *pImpl trick.
