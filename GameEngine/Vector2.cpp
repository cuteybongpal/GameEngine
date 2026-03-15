#include "Vector2.h"

bool Vector2::operator==(Vector2& other)
{
    return this->x == other.x && this->y == other.y;
}
