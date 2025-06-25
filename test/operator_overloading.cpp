#include <version>
#if __cpp_lib_modules < 202207L
#include <cstdint>
#include <type_traits>
#endif

#if __cpp_lib_modules >= 202207L
import std;
#endif
import flag_enum;

enum class Color : std::uint8_t {
    Red = 1,
    Green = 2,
    Blue = 4,
};

template <>
struct FlagTraits<Color> {
    static constexpr bool isBitmask = true;
    static constexpr Flags<Color> allFlags = Color::Red | Color::Green | Color::Blue;
};

int main() {
    static_assert(static_cast<std::underlying_type_t<Color>>(Color::Red | Color::Green) == 3);
    static_assert(static_cast<std::underlying_type_t<Color>>(Color::Red & Color::Green) == 0);
    static_assert(~Color::Red == (Color::Green | Color::Blue));
}