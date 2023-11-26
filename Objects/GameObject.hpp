#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Position2D.hpp"
#include "GameInfo.hpp"
#include <vector>
#include <string>
#include <tuple>
#include <memory>

class GameObject
{
    friend class Engine;

public:
    struct Texture
    {
        Texture(Vector2f offsetPosition, double angle, size_t textureID, Vector2f textureScale, Vector2i textureOffset, bool visible, std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color);
        Vector2f offsetPosition;
        double angle;
        size_t textureID;
        Vector2f textureScale;
        Vector2i textureOffset;
        bool visible;
        std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color;
        size_t priority = 10;
    };

    template<typename T, typename... Args>
    static std::shared_ptr<T> New(Args&&... args)
    {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        GameObject::_allObjects.push_back(std::weak_ptr<GameObject>(std::static_pointer_cast<GameObject>(ptr)));
        return ptr;
    }

    template<typename T, typename Alloc, typename... Args>
    static std::shared_ptr<T> NewAlloc(const Alloc& alloc, Args&&... args)
    {
        auto ptr = std::allocate_shared<T, Alloc>(alloc, std::forward<Args>(args)...);
        GameObject::_allObjects.push_back(std::weak_ptr<GameObject>(std::static_pointer_cast<GameObject>(ptr)));
        return ptr;
    }

    GameObject();
    GameObject(const Position2D& position);
    GameObject(const GameObject& obj) = delete;
    GameObject(GameObject&& obj) = delete;
    virtual ~GameObject();
    virtual void onFrame() = 0;
    virtual void setActive(bool active);

public:
    void addTexture(size_t textureID);
    void addTexture(const Texture& texture);
    void setTexture(size_t textureId, size_t textureNum = 0);
    void setSize(const Vector2i& siz);
    void setPosition(const Vector2f& pos);
    void setId(const std::string& id);
    void setAngle(double angle);

    const Vector2i& size() const;
    const Vector2f& position() const;
    const std::string& id() const;
    double textureID(size_t num) const;
    double angle() const;
    bool isActive() const;

protected:
    Texture& texture(size_t num);

private:
    static const std::vector<std::weak_ptr<GameObject>>& getObjects();
    static void clearVector();

    static std::vector<std::weak_ptr<GameObject>> _allObjects;

private:
    std::vector <Texture> _textures;
    Position2D _position;
    std::string _id;
    size_t _positionInVector;
    double _angle;
    bool _active;
};


#endif // GAMEOBJECT_HPP
