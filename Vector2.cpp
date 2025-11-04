#include "Vector2.h"
#include <cmath>
#include <iostream>

float Vector2::GetSqrMagnitude() const {
    return x * x + y * y;
}
float Vector2::GetMagnitude() const {
    return std::sqrt(GetSqrMagnitude());
}

void Vector2::Normalize() {
    float length = GetMagnitude();
    if (length == 0.0f)
        return;
    *this /= length;
}
const Vector2& Vector2::GetNormalized() const {
    Vector2 copy(*this);
    copy.Normalize();
    return copy;
}

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
Vector2& Vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(*this) += other;
}
Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(*this) -= other;
}
Vector2 Vector2::operator*(float scalar) const {
    return Vector2(*this) *= scalar;
}
Vector2 Vector2::operator/(float scalar) const {
    return Vector2(*this) /= scalar;
}
Vector2 operator*(float scalar, const Vector2& v) {
    return v * scalar;
}


float Vector2::Dot(const Vector2& other) const {
    return this->x * other.x + this->y * other.y;
}

float Vector2::DistanceToSegmentSqr(const Vector2& A, const Vector2& B) const
{
    Vector2 ab = B - A;
    Vector2 ap = *this - A;

    float ab2 = ab.x * ab.x + ab.y * ab.y;
    if (ab2 == 0.0f)
        return ap.x * ap.x + ap.y * ap.y;

    float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;

    Vector2 closest = { A.x + ab.x * t, A.y + ab.y * t };
    float dx = x - closest.x;
    float dy = y - closest.y;
    return dx * dx + dy * dy;
}