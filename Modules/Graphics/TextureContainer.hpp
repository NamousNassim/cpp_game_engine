#ifndef TEXTURECONTAINER_HPP
#define TEXTURECONTAINER_HPP

#include <SFML/Graphics.hpp>
#include "Vector2f.hpp"
#include "Vector2i.hpp"

class TextureContainer
{
public:
    constexpr static size_t MAX_TEXTURES = 1000000000;

public:
    void loadTexture(const std::string& filename);
    void loadTextures(const std::vector<std::string>& filenames);
    size_t addTextureOnFly(sf::Texture* texture);
    void removeTextureOnFly(size_t textureID);
    const sf::Sprite& sprite(Vector2f position, size_t textureID, Vector2i leftUp, Vector2i rightDown);
    Vector2i getSpriteSize(size_t textureID) const;

private:
    std::vector<sf::Texture*> _tex;
    std::vector<sf::Sprite> _textures;

    std::map<size_t, sf::Texture*> _texturesAddedOnFly;
    std::map<size_t, sf::Sprite> _spritesAddedOnFly;
    size_t _nextID = MAX_TEXTURES;
};

#endif // TEXTURECONTAINER_HPP
