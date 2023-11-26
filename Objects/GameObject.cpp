#include "GameObject.hpp"
#include <iostream>

std::vector<std::weak_ptr<GameObject>> GameObject::_allObjects;

GameObject::Texture::Texture(Vector2f offsetPosition, double angle, size_t textureID, Vector2f textureScale, Vector2i textureOffset, bool visible, std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color) :
    offsetPosition(offsetPosition),
    angle(angle),
    textureID(textureID),
    textureScale(textureScale),
    textureOffset(textureOffset),
    visible(visible),
    color(color)
{ }

GameObject::GameObject()
    :_position(), _id(""), _positionInVector(_allObjects.size()), _angle(0), _active(false)
{ }

GameObject::GameObject(const Position2D& position)
    : _position(position), _id(""), _positionInVector(_allObjects.size()), _angle(0), _active(false)
{ }

GameObject::~GameObject()
{ }

void GameObject::addTexture(size_t textureID)
{
    _textures.push_back(Texture(Vector2f(0, 0), 0, textureID, Vector2f(1, 1), Vector2i(0, 0), true, std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>(255, 255, 255, 255)));
    if (_textures.size() > 1)
        _textures.back().priority = _textures[_textures.size() - 2].priority + 1;;
}

void GameObject::addTexture(const Texture& texture)
{
    _textures.push_back(texture);
}

void GameObject::setTexture(size_t textureId, size_t textureNum)
{
    if (textureNum >= _textures.size())
        throw "there is no such texture";

    _textures[textureNum].textureID = textureId;
}

void GameObject::setSize(const Vector2i& siz)
{
    _position.setSize(siz);
}

void GameObject::setPosition(const Vector2f& pos)
{
    _position.setPosition(pos);
}

void GameObject::setId(const std::string &id)
{
    _id = id;
}

void GameObject::setAngle(double angle)
{
    _angle = angle;
}

void GameObject::setActive(bool active)
{
    _active = active;
}

const Vector2i& GameObject::size() const
{
    return _position.size();
}

const Vector2f& GameObject::position() const
{
    return _position.position();
}

const std::string& GameObject::id() const
{
    return _id;
}

double GameObject::textureID(size_t num) const
{
    if (num >= _textures.size())
        throw "there is no such texture";

    return _textures[num].textureID;
}

double GameObject::angle() const
{
    return _angle;
}

bool GameObject::isActive() const
{
    return _active;
}

const std::vector<std::weak_ptr<GameObject>>& GameObject::getObjects()
{
    return _allObjects;
}

void GameObject::clearVector()
{
    for (size_t i = 0; i < _allObjects.size(); i++)
    {
        if (_allObjects[i].expired())
        {
            std::swap(_allObjects[i], _allObjects.back());
            if (!_allObjects[i].expired())
                _allObjects[i].lock()->_positionInVector = i;
            _allObjects.pop_back();
            i--;
        }
    }
}

GameObject::Texture& GameObject::texture(size_t num)
{
    if (num >= _textures.size())
        throw "there is no such texture";

    return _textures[num];
}
