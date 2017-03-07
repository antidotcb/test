#pragma once

#include <map>


namespace testapp {

template <typename K, typename V>
class Selector
{
public:
    using Container = std::map<K, V>;

    explicit Selector(const Container& _container);

    K Id() const;
    void Prev();
    void Next();
    void Select(const K& id);

private:
    bool IsValid() const;

    const Container& container;
    typename Container::const_iterator selected;
};


template <typename K, typename V>
Selector<K, V>::Selector(const Container& _container)
    : container{ _container }, selected{ std::end(container) } {}


template <typename K, typename V>
K Selector<K, V>::Id() const {
    return IsValid() ?
               selected->first :
               K{};
}


template <typename K, typename V>
void Selector<K, V>::Prev() {
    if ( !IsValid() ) {
        return;
    }
    auto backup = selected--;
    if ( !IsValid() ) {
        selected = backup;
    }
}


template <typename K, typename V>
void Selector<K, V>::Next() {
    if ( !IsValid() ) {
        return;
    }
    auto backup = selected++;
    if ( !IsValid() ) {
        selected = backup;
    }
}


template <typename K, typename V>
void Selector<K, V>::Select(const K& id) {
    auto result = std::end(container);
    if ( id.IsValid() ) {
        result = container.find(id);
    }
    if ( result == std::end(container) && !container.empty() ) {
        result = std::begin(container);
    }
    selected = result;
}


template <typename K, typename V>
bool Selector<K, V>::IsValid() const {
    return selected != std::end(container);
}

}
