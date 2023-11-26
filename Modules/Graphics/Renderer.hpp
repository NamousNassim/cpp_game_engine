#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Vector2f.hpp"
#include "TextureContainer.hpp"

class Renderer
{
public:
    struct RenderSettings
    {
        RenderSettings(sf::RenderWindow* window,
                       Vector2f objPosition,
                       Vector2f position,
                       size_t textureId,
                       double angle,
                       double textureAngle,
                       Vector2i textureOffset,
                       Vector2f scale,
                       sf::Color color,
                       size_t priority)
            :window(window), objPosition(objPosition), position(position), textureId(textureId), angle(angle), textureAngle(textureAngle),
             textureOffset(textureOffset), scale(scale), color(color), priority(priority) {}

        sf::RenderWindow* window;
        Vector2f objPosition;
        Vector2f position;
        size_t textureId;
        double angle;
        double textureAngle;
        Vector2i textureOffset;
        Vector2f scale;
        sf::Color color;
        size_t priority;
    };
    struct RendererSettingsPriorityCompare
    {
        bool operator()(const RenderSettings& r1, const RenderSettings& r2)
        {
            return r1.priority > r2.priority;
        }
    };

public:
    Renderer();
    void render(RenderSettings settings);
    size_t addTextureOnFly(sf::Texture* texture);
    void removeTextureOnFly(size_t textureID);
    void loadTextures(const std::vector<std::string>& filenames);
    Vector2i getTextureSize(size_t textureID);

private:
    TextureContainer _tcon;

};

#endif // RENDERER_HPP
