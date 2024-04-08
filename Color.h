#pragma once
#pragma once

namespace LestaTest
{
    struct Color
    {
        static constexpr Color black() { return Color(0, 0, 0); }
        static constexpr Color white() { return Color(1, 1, 1); }
        static constexpr Color red() { return Color(1, 0, 0); }
        static constexpr Color green() { return Color(0, 1, 0); }
        static constexpr Color blue() { return Color(0, 0, 1); }

        constexpr Color()
            : r(0), g(0), b(0), a(0)
        {}

        constexpr Color(float r, float g, float b, float a = 1)
            : r(r), g(g), b(b), a(a)
        {}

        float r;
        float g;
        float b;
        float a;
    };
}
