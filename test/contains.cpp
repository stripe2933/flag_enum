#include <version>
#if __cpp_lib_modules < 202207L
#include <cstdint>
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
    static_assert(contains(Color::Red | Color::Green, Color::Green));
    static_assert(!contains(Color::Red | Color::Green, Color::Blue));
    static_assert(contains(Color::Red | Color::Green | Color::Blue, Color::Blue));
    static_assert(contains(Color::Red | Color::Green | Color::Blue, Color::Red | Color::Blue));

    // Edge cases
    static_assert(contains(Color::Red | Color::Green | Color::Blue, Flags<Color>{}));
    static_assert(contains(Flags<Color>{}, Flags<Color>{}));
    static_assert(!contains(Flags<Color>{}, Color::Red));
}