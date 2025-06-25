#include <cassert>
#include <version>
#if __cpp_lib_modules < 202207L
#include <cstdint>
#include <format>
#include <string_view>
#include <utility>
#endif

#if __cpp_lib_modules >= 202207L
import std;
#endif
import flag_enum;

enum class Color : std::uint8_t {
    Red = 1,
    Green = 4,
    Blue = 16,
};

template <>
struct FlagTraits<Color> {
    static constexpr bool isBitmask = true;
    static constexpr Flags<Color> allFlags = Color::Red | Color::Green | Color::Blue;
};

template <>
struct std::formatter<Color> : formatter<std::string_view> {
    auto format(Color color, auto &ctx) const {
        switch (color) {
            case Color::Red: return formatter<std::string_view>::format("Red", ctx);
            case Color::Green: return formatter<std::string_view>::format("Green", ctx);
            case Color::Blue: return formatter<std::string_view>::format("Blue", ctx);
        }
        std::unreachable();
    }
};

int main() {
    assert(std::format("{}", Color::Red) == "Red");
    assert(std::format("{}", Color::Red | Color::Green) == "Red | Green");
    assert(std::format("{}", Color::Red | Color::Green | Color::Blue) == "Red | Green | Blue");
    assert(std::format("{}", Flags<Color>{}) == "");
}