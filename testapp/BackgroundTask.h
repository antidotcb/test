#pragma once

#include <chrono>
#include <functional>
#include <thread>


namespace testapp {

using namespace std::chrono_literals;

class BackgroundTask
{
public:
    using Duration = std::chrono::duration<int64_t, std::milli>;

    explicit BackgroundTask(std::function<void()>&& func);
    ~BackgroundTask();

    void Start(const Duration& = 50ms);
    void Stop();

private:
    void Process(const Duration& polling) const;

    std::function<void()> background_func;
    std::thread work_thread;
    bool should_stop;
};

}
