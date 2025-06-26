module;

#include <version>

#if __cpp_lib_modules < 202207L
#include <cstdint>
#include <array>
#include <bit>
#include <format>
#include <type_traits>
#include <utility>
#endif

export module flag_enum;

#if __cpp_lib_modules >= 202207L
import std;
#endif

export template <typename BitType>
class Flags;

export template <typename>
struct FlagTraits {
    static constexpr bool isBitmask = false;
};

export template <typename BitType>
[[nodiscard]] constexpr Flags<BitType> operator&(BitType bit, const Flags<BitType> &flags) noexcept {
    return flags.operator&(bit);
}

export template <typename BitType>
[[nodiscard]] constexpr Flags<BitType> operator^(BitType bit, const Flags<BitType> &flags) noexcept {
    return flags.operator^(bit);
}

export template <typename BitType> requires (FlagTraits<BitType>::isBitmask)
[[nodiscard]] constexpr Flags<BitType> operator&(BitType lhs, BitType rhs) noexcept {
    return Flags<BitType>(lhs) & rhs;
}

export template <typename BitType> requires (FlagTraits<BitType>::isBitmask)
[[nodiscard]] constexpr Flags<BitType> operator|(BitType lhs, BitType rhs) noexcept {
    return Flags<BitType>(lhs) | rhs;
}

export template <typename BitType> requires (FlagTraits<BitType>::isBitmask)
[[nodiscard]] constexpr Flags<BitType> operator^(BitType lhs, BitType rhs) noexcept {
    return Flags<BitType>(lhs) ^ rhs;
}

export template <typename BitType> requires (FlagTraits<BitType>::isBitmask)
[[nodiscard]] constexpr Flags<BitType> operator~(BitType bit) noexcept {
    return ~Flags<BitType>(bit);
}

template <typename BitType>
class Flags {
public:
    using MaskType = typename std::underlying_type_t<BitType>;

    constexpr Flags() noexcept = default;
    constexpr Flags(BitType bit) noexcept : mask { std::to_underlying(bit) } { }
    explicit constexpr Flags(MaskType flags) noexcept : mask { flags } { }

    [[nodiscard]] auto operator<=>(const Flags&) const = default;

    [[nodiscard]] constexpr bool operator!() const noexcept { return !mask; }
    [[nodiscard]] constexpr Flags operator&(const Flags &rhs) const noexcept { return Flags { static_cast<MaskType>(mask & rhs.mask) }; }
    [[nodiscard]] constexpr Flags operator|(const Flags &rhs) const noexcept { return Flags { static_cast<MaskType>(mask | rhs.mask) }; }
    [[nodiscard]] constexpr Flags operator^(const Flags &rhs) const noexcept { return Flags { static_cast<MaskType>(mask ^ rhs.mask) }; }
    [[nodiscard]] constexpr Flags operator~() const noexcept { return Flags { static_cast<MaskType>(mask ^ FlagTraits<BitType>::allFlags.mask) }; }

    constexpr Flags &operator|=(const Flags &rhs) noexcept { mask |= rhs.mask; return *this; }
    constexpr Flags &operator&=(const Flags &rhs) noexcept { mask &= rhs.mask; return *this; }
    constexpr Flags &operator^=(const Flags &rhs) noexcept { mask ^= rhs.mask; return *this; }

    [[nodiscard]] constexpr operator bool() const noexcept { return !!mask; }
    [[nodiscard]] explicit constexpr operator MaskType() const noexcept { return mask; }

private:
    MaskType mask;
};

/**
 * @brief Check if \p bit is presented in \p flags.
 * @param flags Flags to check.
 * @param bit Bit to check.
 * @return <tt>true</tt> if \p bit is presented in \p flags, <tt>false</tt> otherwise.
 */
export template <typename BitType> requires (FlagTraits<BitType>::isBitmask)
[[nodiscard]] constexpr bool contains(Flags<BitType> flags, BitType bit) noexcept {
    return flags & bit;
}

/**
 * @brief Check if all bits in \sub are presented in \p sup.
 * @param sup Flags to check if contains \p sub.
 * @param sub Flags to check if \p sup contains.
 * @return <tt>true</tt> if \p sup contains all bits in \p sub, <tt>false</tt> otherwise.
 */
export template <typename BitType> requires (FlagTraits<BitType>::isBitmask)
[[nodiscard]] constexpr bool contains(Flags<BitType> sup, Flags<BitType> sub) noexcept {
    return (sup & sub) == sub;
}

export template <typename BitType, typename CharT>
struct std::formatter<Flags<BitType>, CharT> : formatter<BitType, CharT> {
    [[nodiscard]] auto format(const Flags<BitType> &flags, auto &ctx) const {
        const typename Flags<BitType>::MaskType flagMask { flags };

        constexpr typename Flags<BitType>::MaskType allFlagMask { FlagTraits<BitType>::allFlags };
        constexpr std::size_t flagCount = std::popcount(allFlagMask);

        std::array<BitType, flagCount> flagBits;
        std::size_t flagBitCount = 0;
        typename Flags<BitType>::MaskType bitmask { 1 };
        for (std::size_t i = 0; i < flagCount; bitmask <<= 1) {
            if (allFlagMask & bitmask) {
                if (flagMask & bitmask) {
                    flagBits[flagBitCount++] = static_cast<BitType>(bitmask);
                }
                ++i;
            }
        }

        if (flagBitCount > 0) {
            std::size_t i = 0;
            for (; i < flagBitCount - 1; ++i) {
                formatter<BitType, CharT>::format(flagBits[i], ctx);
                format_to(ctx.out(), " | ");
            }
            formatter<BitType, CharT>::format(flagBits[i], ctx);
        }

        return ctx.out();
    }
};