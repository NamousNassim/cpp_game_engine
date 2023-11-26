#include "TextureContainer.hpp"

#include <iostream>

constexpr size_t TextureContainer::MAX_TEXTURES;

void TextureContainer::loadTexture(const std::string& filename)
{
    _tex.push_back(new sf::Texture);
    _textures.push_back({});
    _tex.back()->loadFromFile(filename);
    _textures.back().setTexture(*_tex.back());
    _textures.back().setOrigin(_tex.back()->getSize().x/2, _tex.back()->getSize().y/2);
}
void TextureContainer::loadTextures(const std::vector<std::string>& filenames)
{
    for (const auto& filename: filenames)
        loadTexture(filename);
}

size_t TextureContainer::addTextureOnFly(sf::Texture* texture)
{
    _texturesAddedOnFly[_nextID] = texture;
    _spritesAddedOnFly[_nextID] = {};
    auto& sprite = _spritesAddedOnFly[_nextID];
    sprite.setTexture(*texture);
    sprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2);

    return _nextID++;
}

void TextureContainer::removeTextureOnFly(size_t textureID)
{
    auto texIt = _texturesAddedOnFly.find(textureID);
    auto sprIt = _spritesAddedOnFly.find(textureID);

    if (texIt == _texturesAddedOnFly.end() || sprIt == _spritesAddedOnFly.end())
        throw "there is no such texture added in fly";

    delete texIt->second;
    _texturesAddedOnFly.erase(texIt);
    _spritesAddedOnFly.erase(sprIt);
}

const sf::Sprite& TextureContainer::sprite(Vector2f position, size_t textureID, Vector2i rectPos, Vector2i rectSize)
{
    if (textureID >= MAX_TEXTURES)
    {
        auto sprIt = _spritesAddedOnFly.find(textureID);
        if (sprIt == _spritesAddedOnFly.end())
            throw "there is no such texture added in fly";

        auto& sprite = sprIt->second;
        sprite.setPosition(static_cast<float>(position.x),
                                         static_cast<float>(position.y));
        sprite.setTextureRect(sf::IntRect(rectPos.x, rectPos.y, rectSize.x, rectSize.y));

        return sprIt->second;
    }

    size_t texturesSize = _tex.size();
    if (textureID >= texturesSize)
        throw "there is no such texture";

    _textures[textureID].setPosition(static_cast<float>(position.x),
                                     static_cast<float>(position.y));
    _textures[textureID].setTextureRect(sf::IntRect(rectPos.x, rectPos.y, rectSize.x, rectSize.y));
    return _textures[textureID];
}

Vector2i TextureContainer::getSpriteSize(size_t textureID) const
{
    if (textureID >= MAX_TEXTURES)
    {
        auto texIt = _texturesAddedOnFly.find(textureID);
        if (texIt == _texturesAddedOnFly.end())
            throw "there is no such texture added in fly";

        return Vector2i(static_cast<int>(texIt->second->getSize().x), static_cast<int>(texIt->second->getSize().y));
    }

    size_t texturesSize = _tex.size();
    if (textureID >= texturesSize)
        throw "there is no such texture";

    return Vector2i(static_cast<int>(_tex[textureID]->getSize().x), static_cast<int>(_tex[textureID]->getSize().y));
}
