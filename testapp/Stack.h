#pragma once

#include <memory>
#include <stack>


namespace testapp {
namespace states {

class StateInterface;

class Stack
{
public:
    explicit Stack(std::unique_ptr<StateInterface>&& active);

    bool Update();

protected:
    void Push(std::unique_ptr<StateInterface> state);

    void Pop();
    void FatalError(const char* title, const char* msg);

private:
    using States = std::stack<std::unique_ptr<StateInterface>>;
    States states;
};

} // namespace states
} // namespace testapp
