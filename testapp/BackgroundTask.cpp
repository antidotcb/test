#include "BackgroundTask.h"
#include <chrono>
#include <iostream>
#include <galaxy/Errors.h>


namespace testapp {

BackgroundTask::BackgroundTask(std::function<void()>&& func)
    : background_func{ std::move(func) }, should_stop{ true } { }


BackgroundTask::~BackgroundTask() {
    Stop();
}


void BackgroundTask::Start(const Duration& polling) {
    if ( work_thread.joinable() ) {
        return;
    }

    should_stop = false;
    auto thread_proc = [&, polling]() {
                Process(polling);
            };
    work_thread = std::thread(thread_proc);
}


void BackgroundTask::Stop() {
    should_stop = true;

    if ( work_thread.joinable() ) {
        work_thread.join();
    }
}


void BackgroundTask::Process(const Duration& polling) const {
    while ( !should_stop ) {
        try {
            background_func();
        }
        catch ( galaxy::api::IError& error ) {
            std::cerr << "THREAD: " << error.GetName() << ": " << error.GetMsg() << std::endl;
            break;
        }
        catch ( std::exception& exception ) {
            std::cerr << "THREAD: Exception: " << exception.what() << std::endl;
            break;
        }
        catch ( ... ) {
            std::cerr << "THREAD: Unknown exception" << std::endl;
            break;
        }

    }
}


} // namespace testapp
