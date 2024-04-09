#pragma once
#include <iostream>

namespace LestaTest
{
	namespace Math
	{
		template<typename T>
		struct Vector2
		{
            static constexpr Vector2 zero() { return Vector2(0, 0); }
            static constexpr Vector2 down() { return Vector2(0, -1); }
            static constexpr Vector2 up() { return Vector2(0, 1); }
        
            inline constexpr Vector2() noexcept
            { 
                x = 0;
                y = 0;
            };

            explicit constexpr Vector2(T x, T y) noexcept
            { 
                this->x = x;  
                this->y = y;
            }

            explicit constexpr Vector2(T x) noexcept
            { 
                this->x = y = x;
            }

            friend std::ostream& operator<<(std::ostream& os, const Vector2& vec2)
            {
                os << "x: " << vec2.x << " y: " << vec2.y;
                return os;
            };

            bool operator==(const Vector2& rhs) const
            {
                return x == rhs.x && y == rhs.y;
            };

            bool operator!=(const Vector2& rhs) const
            {
                return x != rhs.x || y != rhs.y;
            };

            bool operator<(const Vector2& rhs) const
            {
                if (x < rhs.x && y < rhs.y)
                    return true;
                else
                    return false;
            };

            bool operator>(const Vector2& rhs) const
            {
                return rhs < *this;
            };

            bool operator<=(const Vector2& rhs) const
            {
                return rhs >= *this;
            };

            bool operator>=(const Vector2& rhs) const
            {
                return *this >= rhs;
            };

            void operator+=(const Vector2& rhs)
            {
                x += rhs.x;
                y += rhs.y;
            };
            void operator/=(const Vector2& rhs)
            {
                x /= rhs.x;
                y /= rhs.y;
            };
            void operator-=(const Vector2& rhs)
            {
                x -= rhs.x;
                y -= rhs.y;
            };
            void operator*=(const Vector2& rhs)
            {
                x *= rhs.x;
                y *= rhs.y;
            };

            void operator+=(T elem)
            {
                x += elem;
                y += elem;
            };
            void operator-=(T elem)
            {
                x -= elem;
                y -= elem;
            };
            void operator*=(T elem)
            {
                x *= elem;
                y *= elem;
            };
            void operator/=(T elem)
            {
                x /= elem;
                y /= elem;
            };

            const Vector2 operator+(const Vector2& rhs) const
            {
                return Vector2<T>(x + rhs.x, y + rhs.y);
            };

            const Vector2 operator-(const Vector2& rhs) const
            {
                return Vector2<T>(x - rhs.x, y - rhs.y);
            };

            const Vector2 operator*(const Vector2& rhs) const
            {
                return Vector2<T>(x * rhs.x, y * rhs.y);
            };

            const Vector2 operator/(const Vector2& rhs) const
            {
                return Vector2<T>(x / rhs.x, y / rhs.y);
            };

            const Vector2 operator+(T elem) const
            {
                return Vector2<T>(x + elem, y + elem);
            };

            const Vector2 operator-(T elem) const
            {
                return Vector2<T>(x - elem, y - elem);
            };

            const Vector2 operator*(T elem) const
            {
                return Vector2<T>(x * elem, y * elem);
            };

            const Vector2 operator/(T elem) const
            {
                return Vector2<T>(x / elem, y / elem);
            };

            T x;
            T y;   
		};

		typedef Vector2<float> Vec2;
		typedef Vector2<int> Vec2i;
	}
}
