#pragma once

// Vector.
struct Vec3
{
    float x, y, z; // Position stuff.

    Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3 operator+(Vec3 b) { return Vec3(x + b.x, y + b.y, z + b.z); };
    Vec3 operator-(Vec3 b) { return Vec3(x - b.x, y - b.y, z - b.z); };
};