#pragma once
#include "Entt.hpp"
#include <iostream>

struct TComp {

    int i = 10;
    int j = 15;

};

struct TComp2 {

    int i = 10;
    int j = 15;

};

void update(TComp& i, const TComp2& j) {

    std::cout << "UPDATE" << std::endl;
}

class ISystem {
public:
    virtual void Update() = 0;

protected:
    entt::registry registry;
};

template<class ...T>
class System : public ISystem {
public:
    void Update() {

        //auto view = registry.view<T...>();
        //
        //for (const auto entt : view) {
        //    update(view.get<T...>(entt)...);
        //}

        //registry.view<TComp, const TComp2>().each(update);
        //registry.view<TComp, const TComp2>().each(update);

        //registry.view<T...>().each(update);
        //T t; // View Components -> pass to Update
        //update(&args...);
        //update();
    }

    //virtual void update(T&... args) = 0;
};

class SystemManager {
public:

    void Create() {
        auto entity = registry.create();
        registry.emplace<TComp>(entity);
        registry.emplace<TComp2>(entity);
    }

    template<class ...T>
    void Register(std::function<void(T...)> f) {

        funcs.push_back([&]() {
            std::cout << "In" << std::endl;
            registry.view<T...>().each(f);
            std::cout << "In2" << std::endl;
        });
    }

    void Update() {
        std::cout << "Here" << std::endl;
        for (auto func : funcs)
            func();
        std::cout << "Here2" << std::endl;
        TComp t1{};
        TComp2 t2{};
        update(t1, t2);
        std::cout << "Here3" << std::endl;
        registry.view<TComp, TComp2>().each(update);
        std::cout << "Here4" << std::endl;
    }

private:
    std::vector<std::function<void()>> funcs;
    entt::registry registry;
};

void main() {
    SystemManager* manager = new SystemManager();

    manager->Create();
    manager->Register<TComp, const TComp2>(std::bind(update));
    manager->Update();

    std::cin.get();
}

