#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "GameInfo.hpp"
#include "SoundPlayer.hpp"
#include "Collider.hpp"
#include <functional>
#include <memory>

class PhysicalObject;

class Engine
{
public:
    using ColorType = std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>;
    Engine(const Engine& e) = delete;
    static Engine& Instance();
    static SoundPlayer& soundPlayer();
    void loadTextures(const std::vector<std::string>& textures);
    void loadFont(const std::string& filename);
    size_t generateTextureFromText(const std::string& text, size_t textSize);
    void removeTextureOnFly(size_t textureID);
    Vector2i getTextureSize(size_t textureID);
    void loadSounds(const std::vector<std::string>& sounds);
    std::weak_ptr<GameObject> findObjectById(const std::string& id);
    std::vector<std::weak_ptr<GameObject>> getObjects(std::function<bool(GameObject*)> predicate);
    bool nextFrame();
    void setTimeRate(double timeRate);
    double timeRate();
    void switchFullscreenMode(bool fullscreenMode);
    bool fullscreenMode() const;
    void setCursorVisible(bool visible);
    void exit();

private:
    Engine();
    void checkEvents();
    void checkCollisionsWithType(size_t objIndex, Collider& col, Collider::CollisionType type) const;
    void checkCollisions();
    void updateUI();
    void updateGameInfo();

    static Engine* _instance;
    sf::RenderWindow* _window;
    sf::Mouse _mouse;
    sf::Clock _clock;
    sf::Font _font;
    Renderer _renderer;
    SoundPlayer _player;
    Vector2i _screenSize; // usend in fullscreen mode
    bool _fullscreenMode;
    double _timeRate;
    bool _mouseCursorVisible;
};

#endif // WINDOW_HPP
