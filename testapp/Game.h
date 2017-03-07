#pragma once

#include <memory>


namespace testapp {

class BackgroundTask;

class Game
{
public:
    Game();
    ~Game();

    void Run() const;
private:
    std::unique_ptr<BackgroundTask> background_polling;
};

} // namespace testapp
