#include "jelly/core/scene_manager.hpp"

namespace jelly::core {

int SceneManager::addScene(std::unique_ptr<Scene> scene) {
    const std::string& name = scene->getName();
    if (nameToId_.contains(name)) {
        throw std::runtime_error("Scene already exists");
    }

    int id = nextId_++;
    nameToId_[name] = id;
    scenes_[id] = std::move(scene);
    return id;
}

std::shared_ptr<Scene> SceneManager::getSceneById(int id) {
    if (!scenes_.contains(id)) return nullptr;
    return scenes_[id];
}

bool SceneManager::setActiveScene(int sceneId) {
    auto it = scenes_.find(sceneId);
    if (it == scenes_.end()) {
        throw std::runtime_error("Scene does not exist");
        return false;
    }
    currentScene_ = it->second;
    return true;
}

bool SceneManager::destroyScene(int id) {
    return scenes_.erase(id) > 0;
}

void SceneManager::initializeActiveScene() {
    if (currentScene_) currentScene_->initialize();
}

void SceneManager::updateActiveScene() {
    if (currentScene_) currentScene_->update();
}

void SceneManager::fixedUpdate() {
    if (currentScene_) currentScene_->fixedUpdate();
}

void SceneManager::renderActiveScene() {
    if (currentScene_) currentScene_->render();
}

void SceneManager::shutdownActiveScene() {
    if (currentScene_) currentScene_->shutdown();
}

}