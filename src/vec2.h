#pragma once

// Vector.
struct Vec2
{
    float x, y; // Position stuff.

    Vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Vec2 operator+(Vec2 b) { return Vec2(x + b.x, y + b.y); };
    Vec2 operator-(Vec2 b) { return Vec2(x - b.x, y - b.y); };
};