#ifndef SIMPLEBUTTON_HPP
#define SIMPLEBUTTON_HPP

#include <functional>
#include "UIObject.hpp"

class SimpleButton : public UIObject
{
    friend class Window;

public:
    SimpleButton(Vector2f position, size_t texture, Vector2i size,
                 std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> normalColor = std::make_tuple(255, 255, 255, 255),
                 std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> draggedColor = std::make_tuple(200, 200, 200, 255),
                 std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> clickedColor = std::make_tuple(150, 150, 150, 255));
    void setCallFunction(std::function<void()> call);

    void setPosition(Vector2f position);
    Vector2f position() const;

    void setNormalColor(std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color);
    void setDraggedColor(std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color);
    void setClickedColor(std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color);

private:
    void onStartDrag() final;
    void onDrag() final;
    void onEndDrag() final;
    void onMouseDown() final;
    void onMouseClick() final;
    void onMouseUp() final;
    void onFrame() override;

    std::function <void()> _call;
    size_t _texture;
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> _normalColor, _draggedColor, _clickedColor;
};

#endif // SIMPLEBUTTON_HPP
