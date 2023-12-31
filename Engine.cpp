#include "Engine.hpp"
#include "PhysicalObject.hpp"
#include "UIObject.hpp"
#include <iostream>
#include <queue>
#include "MainClass.hpp"

Engine* Engine::_instance = nullptr;

Engine::Engine()
    :_window(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Game", sf::Style::Fullscreen)), _renderer(),
     _screenSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), _fullscreenMode(true), _timeRate(1), _mouseCursorVisible(true)
{
    _window->setFramerateLimit(240);
    GameInfo::_resolution = Vector2i(1280, 720);
    GameInfo::_keyStatusMask = 0;
    GameInfo::_mousePosition = Vector2i(0, 0);
    std::srand(std::time(nullptr));
}

using std::cout;
using std::endl;

Engine& Engine::Instance()
{
    if (_instance == nullptr)
        _instance = new Engine();
    return *_instance;
}

SoundPlayer& Engine::soundPlayer()
{
    return Engine::Instance()._player;
}

void Engine::loadTextures(const std::vector<std::string>& textures)
{
    _renderer.loadTextures(textures);
}

void Engine::loadFont(const std::string& filename)
{
    _font.loadFromFile(filename);
}

size_t Engine::generateTextureFromText(const std::string& text, size_t textSize)
{
    sf::Text t(text, _font, textSize);
    sf::RenderTexture tex;
    tex.create(t.getLocalBounds().left + t.getLocalBounds().width, t.getLocalBounds().top + t.getLocalBounds().height, 1);
    tex.draw(t);
    tex.display();
    return _renderer.addTextureOnFly(new sf::Texture(tex.getTexture()));
}

void Engine::removeTextureOnFly(size_t textureID)
{
    _renderer.removeTextureOnFly(textureID);
}

Vector2i Engine::getTextureSize(size_t textureID)
{
    return _renderer.getTextureSize(textureID);
}

void Engine::loadSounds(const std::vector<std::string>& sounds)
{
    _player.loadSounds(sounds);
}

std::weak_ptr<GameObject> Engine::findObjectById(const std::string& id)
{
    const auto& objects = GameObject::getObjects();
    for (auto& obj : objects)
    {
        if (!obj.expired() && obj.lock()->id() == id)
            return obj;
    }
    return std::weak_ptr<GameObject>();
}

std::vector<std::weak_ptr<GameObject>> Engine::getObjects(std::function<bool (GameObject*)> predicate)
{
    std::vector<std::weak_ptr<GameObject>> objects;
    for (auto& obj: GameObject::getObjects())
        if (!obj.expired() && predicate(obj.lock().get()))
            objects.push_back(obj);

    return objects;
}

bool Engine::nextFrame()
{
    _window->clear();
    updateGameInfo();
    checkEvents();
    checkCollisions();
    updateUI();

    std::priority_queue<Renderer::RenderSettings, std::vector<Renderer::RenderSettings>, Renderer::RendererSettingsPriorityCompare> texturesToRender;

    const auto& objects = GameObject::getObjects();
    for (size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i].expired())
            continue;

        {
            auto currObj = objects[i].lock();
            if (!currObj->isActive())
                continue;
            currObj->onFrame();
        }

        if (!objects[i].expired())
        {
            auto currObj = objects[i].lock();
            for (const auto& texture : currObj->_textures)
                if (texture.visible == true)
                {
                    sf::Color c = sf::Color(std::get<0>(texture.color), std::get<1>(texture.color), std::get<2>(texture.color), std::get<3>(texture.color));
                    texturesToRender.push(Renderer::RenderSettings(_window, currObj->position(), texture.offsetPosition, texture.textureID,
                                                                        currObj->angle(), texture.angle, texture.textureOffset, texture.textureScale, sf::Color(c), texture.priority));
                }
        }
    }

    while(!texturesToRender.empty())
    {
        _renderer.render(texturesToRender.top());
        texturesToRender.pop();
    }

    GameObject::clearVector();

    if (MainClass::_instance != nullptr)
        MainClass::_instance->onFrame();

    _window->display();
    return _window->isOpen();
}

void Engine::setTimeRate(double timeRate)
{
    _timeRate = timeRate;
}

double Engine::timeRate()
{
    return _timeRate;
}

void Engine::switchFullscreenMode(bool fullscreenMode)
{
    delete _window;
    _fullscreenMode = fullscreenMode;
    if (fullscreenMode)
        _window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Game", sf::Style::Fullscreen);
    else
        _window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Game");

    _window->setFramerateLimit(240);
    _window->setMouseCursorVisible(_mouseCursorVisible);
}

bool Engine::fullscreenMode() const
{
    return _fullscreenMode;
}

void Engine::setCursorVisible(bool visible)
{
    _window->setMouseCursorVisible(visible);
    _mouseCursorVisible = visible;
}

void Engine::exit()
{
    if (MainClass::_instance != nullptr)
        MainClass::_instance->onExit();
    _window->close();
}

void Engine::checkEvents()
{
    sf::Event event;
    while (_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            exit();
        }
    }
}

// simple check throws out cases where it is easy to say that the collision is not happening
static inline bool isCollisionPossible(Vector2f obj1Pos, Vector2f obj2Pos, const Collider& col1, const Collider& col2)
{
    Vector2f o(obj1Pos - obj2Pos);
    return (col1.longestSquare() + col2.longestSquare() >= o.squareLength());
}

void Engine::checkCollisionsWithType(size_t objIndex, Collider& col, Collider::CollisionType type) const
{
    const auto& objects = PhysicalObject::getObjects();
    auto& obj1 = objects[objIndex];
    const auto& colliders = PhysicalObject::getColliders(static_cast<Collider::CollisionType>(type));
    for (const auto& collInfo: colliders)
    {
        if (!(collInfo.isAlive && obj1.second && collInfo.object->isActive()) || obj1.first == collInfo.object)
            continue;

        auto& collider = collInfo.object->_colliders[collInfo.collider];
        if (!isCollisionPossible(obj1.first->position(), collInfo.object->position(), col, collider))
            continue;

        collider.fixPosition(collInfo.object->angle(), collInfo.object->position());
        col.fixPosition(obj1.first->angle(), obj1.first->position());
        if (col.isCollideWith(collider))
        {
            obj1.first->onCollide(collInfo.object);
            if (collInfo.isAlive)
                collInfo.object->onCollide(obj1.first);
        }
    }
}

void Engine::checkCollisions()
{
    const auto& objects = PhysicalObject::getObjects();
    for (size_t i = 0; i < objects.size(); i++)
    {
        auto& obj1 = objects[i];
        if (!obj1.second || !obj1.first->isActive()) // continue if object is deleted or not active
            continue;

        for (auto& myCollider: obj1.first->_colliders)
            for (size_t k = 0; k < Collider::CollisionTypesNumber; k++)
                if (myCollider.collWith(static_cast<Collider::CollisionType>(k)))
                    checkCollisionsWithType(i, myCollider, static_cast<Collider::CollisionType>(k));
    }
    PhysicalObject::clearVector();
}

void Engine::updateUI()
{
    const auto& objects = UIObject::getObjects();
    const auto& mousePos = GameInfo::mousePosition();
    for (size_t i = 0; i < objects.size(); i++)
    {
        auto& obj = objects[i].first;
        if (objects[i].second == false || !obj->isActive()) continue;
        if (objects[i].second == true &&
            mousePos.x > obj->position().x && mousePos.x < obj->position().x + obj->size().x &&
            mousePos.y > obj->position().y && mousePos.y < obj->position().y + obj->size().y)
        {
            if (!obj->_dragged)
            {
                obj->_dragged = true;
                obj->onStartDrag();
            }
            obj->onDrag();
            if (_mouse.isButtonPressed(sf::Mouse::Left))
            {
                if (!obj->_clicked)
                {
                    obj->_clicked = true;
                    obj->onMouseDown();
                }
                obj->onMouseClick();
            }
            else
            {
                if (obj->_clicked)
                {
                    obj->_clicked = false;
                    obj->onMouseUp();
                }
            }
        }
        else
        {
            if (obj->_dragged)
            {
                obj->_dragged = false;
                obj->onEndDrag();
            }
            obj->_clicked = false;
        }
    }
    UIObject::clearVector();
}

void Engine::updateGameInfo()
{
    int mask = 0;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::Right) << 0;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::Left) << 1;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::Up) << 2;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::Down) << 3;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::D) << 4;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::A) << 5;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::W) << 6;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::S) << 7;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::Space) << 8;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) << 9;
    mask += sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) << 10;

    GameInfo::_keyStatusMask = mask;
    const auto& pos = _mouse.getPosition(*_window);
    GameInfo::_absoluteDeltaTime = _clock.getElapsedTime().asSeconds();
    _clock.restart();

    GameInfo::_deltaTime = GameInfo::_absoluteDeltaTime * _timeRate;
    if (fullscreenMode())
        GameInfo::_mousePosition = Vector2i(pos.x * 1280 / _screenSize.x, pos.y * 720 / _screenSize.y);
    else
        GameInfo::_mousePosition = Vector2i(pos.x, pos.y);
    static int licznik = 0;
    licznik++;
    if (licznik%100 == 0)
        cout<<1.f/GameInfo::absoluteDeltaTime()<<" - "<<GameObject::getObjects().size()<<" "<<UIObject::getObjects().size()<<endl;

}
