#include "Vector2f.hpp"

std::ostream& operator<<(std::ostream& a, Vector2f b)
{
    a<<"("<<b.x<<", "<<b.y<<") ";
    return a;
}

double Vector2f::angleBetween(const Vector2f& a, const Vector2f& b)
{
    return atan2(b.y - a.y, b.x - a.x);
}

double Vector2f::length() const
{
    return std::sqrt(x*x+y*y);
}
