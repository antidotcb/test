#pragma once

#include <vector>
#include <map>
#include <functional>


namespace testapp {

class Input
{
public:
    class Action
    {
    public:
        using Callback = std::function<void()>;

        Action(const Callback& callback, const std::string& desc);

        void Execute() const;
        const std::string& Desc() const;

    private:
        Callback callback;
        std::string desc;
        bool valid = false;
    };

    enum
    {
        MODIFIER = 224,
        ENTER = 13,
        ESCAPE = 27,
        UP = 72 << sizeof(uint8_t) * 8 | MODIFIER,
        DOWN = 80 << sizeof(uint8_t) * 8 | MODIFIER,
    };

    using ExtendedKey = uint16_t;
    using Keys = std::vector<ExtendedKey>;
    using KeyBindings = std::map<Keys, Action>;

    void Setup(KeyBindings&& bindings);
    bool bindingExists(uint16_t key) const;
    void Process();
    void Print();
private:
    static std::string KeyToString(const ExtendedKey key);

    using Bindings = std::map<ExtendedKey, Action>;
    Bindings bindings;
};
}
