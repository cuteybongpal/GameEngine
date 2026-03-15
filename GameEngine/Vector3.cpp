#include "Vector3.h"

bool Vector3::operator==(Vector3& other)
{
    return this->x == other.x && this->y == other.y && this->z == other.z;
}
