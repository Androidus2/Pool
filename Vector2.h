#pragma once

struct Vector2 {
    float x, y;

    float GetSqrMagnitude() const;
    float GetMagnitude() const;

    void Normalize();
    const Vector2& GetNormalized() const;

    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    float Dot(const Vector2& other) const;
    float DistanceToSegmentSqr(const Vector2& A, const Vector2& B) const;
};

Vector2 operator*(float scalar, const Vector2& v);