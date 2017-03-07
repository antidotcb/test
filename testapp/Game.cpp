#include "Game.h"
#include <galaxy/GalaxyApi.h>
#include "Stack.h"
#include "LoginSelector.h"
#include "BackgroundTask.h"


namespace testapp {


Game::Game()
    : background_polling{ std::make_unique<BackgroundTask>([]() {
            galaxy::api::ProcessData();
        }) } {
    const auto client_id = "";
    const auto client_secret = "";
    galaxy::api::Init(client_id, client_secret, true);
}


Game::~Game() {
    galaxy::api::Shutdown();
}


void Game::Run() const {
    background_polling->Start();
    states::Stack states{ std::make_unique<states::LoginSelector>() };

    bool keep_updating;
    do {
        keep_updating = states.Update();
    } while ( keep_updating );

    background_polling->Stop();
}

} // namespace testapp
