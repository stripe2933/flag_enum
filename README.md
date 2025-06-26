# flag_enum

Make your enum flags.

```c++
import std;
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

// ----- contains() -----

static_assert(contains(Color::Red | Color::Green, Color::Red));
static_assert(!contains(Color::Red | Color::Green, Color::Blue));
static_assert(contains(Color::Red | Color::Green | Color::Blue, Color::Red | Color::Blue));

// ----- formatting -----

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

std::print("{}", Color::Red | Color::Green); // Red | Green
std::print("{}", ~Color::Green); // Red | Blue

// Work with standard format specification
std::print("{:?}", Color::Red | Color::Green); // "Red" | "Green"
```

## Usage

### Manual Installation

```shell
git clone --depth 1 https://github.com/stripe2933/flag_enum.git
cd flag_enum
mkdir build
cmake -S . -B build -G Ninja
cmake --build build --target install
```

Then in your CMake project, you can use:

`CMakeLists.txt`
```cmake
find_package(flag_enum CONFIG REQUIRED)
target_link_libraries(<target> PRIVATE flag_enum::flag_enum)
```

### vcpkg

```shell
vcpkg add port flag-enum
```

Then in your CMake project, you can use:

`CMakeLists.txt`
```cmake
find_package(flag_enum CONFIG REQUIRED)
target_link_libraries(<target> PRIVATE flag_enum::flag_enum)
```

### CPM

`CMakeLists.txt`
```cmake
CPMAddPackage("gh:stripe2933/flag_enum@0.1.0")
target_link_libraries(<target> PRIVATE flag_enum::flag_enum)
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.txt) file for details.