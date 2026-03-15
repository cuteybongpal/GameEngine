#include "Matrix4x4.h"
#include <cmath>

Matrix4x4 position2Matrix4x4(Vector3& position)
{
    Matrix4x4 positionMatrix{};
    positionMatrix.m[0][0] = 1;
    positionMatrix.m[1][1] = 1;
    positionMatrix.m[2][2] = 1;
    positionMatrix.m[3][3] = 1;

    positionMatrix.m[0][3] = position.x;
    positionMatrix.m[1][3] = position.y;
    positionMatrix.m[2][3] = position.z;

    return positionMatrix;
}

Matrix4x4 rotation2Matrix4x4(Vector3& rotation)
{
    float cx = cosf(rotation.x);
    float sx = sinf(rotation.x);

    float cy = cosf(rotation.y);
    float sy = sinf(rotation.y);

    float cz = cosf(rotation.z);
    float sz = sinf(rotation.z);

    Matrix4x4 rot{};

    rot.m[0][0] = cy * cz;
    rot.m[0][1] = cz * sx * sy - cx * sz;
    rot.m[0][2] = cx * cz * sy + sx * sz;
    rot.m[0][3] = 0;

    rot.m[1][0] = cy * sz;
    rot.m[1][1] = sx * sy * sz + cx * cz;
    rot.m[1][2] = cx * sy * sz - cz * sx;
    rot.m[1][3] = 0;

    rot.m[2][0] = -sy;
    rot.m[2][1] = cy * sx;
    rot.m[2][2] = cx * cy;
    rot.m[2][3] = 0;

    rot.m[3][0] = 0;
    rot.m[3][1] = 0;
    rot.m[3][2] = 0;
    rot.m[3][3] = 1;

    return rot;
}

Matrix4x4 scale2Matrix4x4(Vector3& scale)
{
    Matrix4x4 scaleMatrix{};
    scaleMatrix.m[0][0] = scale.x;
    scaleMatrix.m[1][1] = scale.y;
    scaleMatrix.m[2][2] = scale.z;
    scaleMatrix.m[3][3] = 1;

    return scaleMatrix;
}

Matrix4x4 getIdentityMatrix4x4()
{
    Matrix4x4 identityMatrix{};
    identityMatrix.m[0][0] = 1;
    identityMatrix.m[1][1] = 1;
    identityMatrix.m[2][2] = 1;
    identityMatrix.m[3][3] = 1;
    return identityMatrix;
}
