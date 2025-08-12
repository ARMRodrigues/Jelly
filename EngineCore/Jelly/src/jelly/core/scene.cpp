#include "jelly/core/scene.hpp"

#include <iostream>

namespace jelly::core {

Scene::Scene(std::string name)
    : name_(std::move(name)) {}

const std::string& Scene::getName() const {
    return name_;
}

entt::registry& Scene::getEntityManager() {
    return registry_;
}

void Scene::addGameSystem(std::shared_ptr<GameSystemInterface> system) {
    for (const auto& gs : gameSystems_) {
        if (gs == system) return;
    }
    gameSystems_.push_back(std::move(system));
}

void Scene::initialize() {
    for (const auto& system : gameSystems_) {
        system->initialize();
    }
}

void Scene::update() {
    for (const auto& system : gameSystems_) {
        system->update();
    }
}

void Scene::fixedUpdate() {
    for (const auto& system : gameSystems_) {
        system->fixedUpdate();
    }
}

void Scene::render() {
    for (const auto& system : gameSystems_) {
        system->render();
    }
}

void Scene::shutdown() {
    for (const auto& system : gameSystems_) {
        system->shutdown();
    }
}

}
