#ifndef _TOOLS_ENDIAN_HPP
#define _TOOLS_ENDIAN_HPP

#include "tools/keywords_override_protection.hpp"

#include <stdint.h>
#include <stddef.h>

#include <type_traits>

#include "tools/c++11.hpp"


namespace tools {


    typedef uintptr_t memsize_t;


    namespace _hidden {


        enum class Endian {

            little,
            big,
            unknown,
        };


        template <bool _IsLittle, bool _IsBig>
        struct Switcher {

            static Endian const value = Endian::unknown;
        };


        template <>
        struct Switcher<true, false> {

            static Endian const value = Endian::little;
        };


        template <>
        struct Switcher<false, true> {

            static Endian const value = Endian::big;
        };


        class Detector {

            static memsize_t const _mask =
                    memsize_t(0x1) |
                    memsize_t(0x2) << (sizeof(memsize_t) - 1) * 8;
            static uint8_t const _first = static_cast<uint8_t const &>(_mask);

        public:
            static Endian const current =
                    Switcher<_first == 0x1, _first == 0x2>::value;
        };


        template <
            typename _Integer,
            size_t _Size = sizeof(_Integer),
            size_t _Current = _Size / 2 + 1
        >
        class SwapBytesImplementation {

            static_assert(std::is_integral<_Integer>::value,
                          "Not integral type!");

            static size_t const _offset1 = (_Size / 2 + 1 - _Current) * 8;
            static size_t const _offset2 = (_Size - 1) * 8 - _offset1;

        public:
            _CONSTEXPR static _Integer call(_Integer value) {

                return ((value >> _offset1) & _Integer(0xff)) << _offset2 |
                       ((value >> _offset2) & _Integer(0xff)) << _offset1 |
                       SwapBytesImplementation<_Integer, _Size, _Current - 1>
                           ::call(value);
            }
        };


        template <typename _Integer, size_t _Size>
        struct SwapBytesImplementation<_Integer, _Size, 0> {

            static_assert(std::is_integral<_Integer>::value,
                          "Not integral type!");

            _CONSTEXPR static _Integer call(_Integer) {

                return _Integer(0);
            }
        };


        template <Endian _Current>
        struct Converter { };


        template <>
        struct Converter<Endian::little> {

            template <typename _Integer>
            _CONSTEXPR static _Integer toLittle(_Integer value) {

                return value;
            }

            template <typename _Integer>
            _CONSTEXPR static _Integer toBig(_Integer value) {

                return SwapBytesImplementation<_Integer>::call(value);
            }

            template <typename _Integer>
            _CONSTEXPR static _Integer fromLittle(_Integer value) {

                return value;
            }

            template <typename _Integer>
            _CONSTEXPR static _Integer fromBig(_Integer value) {

                return SwapBytesImplementation<_Integer>::call(value);
            }
        };


        template <>
        struct Converter<Endian::big> {

            template <typename _Integer>
            _CONSTEXPR static _Integer toLittle(_Integer value) {

                return SwapBytesImplementation<_Integer>::call(value);
            }

            template <typename _Integer>
            _CONSTEXPR static _Integer toBig(_Integer value) {

                return value;
            }

            template <typename _Integer>
            _CONSTEXPR static _Integer fromLittle(_Integer value) {

                return SwapBytesImplementation<_Integer>::call(value);
            }

            template <typename _Integer>
            _CONSTEXPR static _Integer fromBig(_Integer value) {

                return value;
            }
        };

    } // namespace _hidden


    enum class Endian {

        little  = int(_hidden::Endian::little),
        big     = int(_hidden::Endian::big),
        unknown = int(_hidden::Endian::unknown),
        current = int(_hidden::Detector::current),
    };


    template <typename _Integer>
    _CONSTEXPR _Integer swapBytes(_Integer value) {

        return _hidden::SwapBytesImplementation<_Integer>::call(value);
    }


#if !_PP_IS_EMPTY(_CONSTEXPR)
#if _CONSTEXPR == constexpr

    static_assert(swapBytes(uint8_t(0x11)) == uint8_t(0x11),
                  "swapBytes<uint8_t> failed");
    static_assert(swapBytes(uint16_t(0x1122)) == uint16_t(0x2211),
                  "swapBytes<uint16_t> failed");
    static_assert(swapBytes(uint32_t(0x11223344)) == uint32_t(0x44332211),
                  "swapBytes<uint32_t> failed");
    static_assert(swapBytes(uint64_t(0x1122334455667788)) ==
                  uint64_t(0x8877665544332211),
                  "swapBytes<uint64_t> failed");

#endif
#endif


    template <typename _Integer>
    _CONSTEXPR _Integer toLittleEndian(_Integer value) {

        return _hidden
                ::Converter<_hidden::Detector::current>::toLittle(value);
    }


    template <typename _Integer>
    _CONSTEXPR _Integer toBigEndian(_Integer value) {

        return _hidden
                ::Converter<_hidden::Detector::current>::toBig(value);
    }

    template <typename _Integer>
    _CONSTEXPR _Integer fromLittleEndian(_Integer value) {

        return _hidden
                ::Converter<_hidden::Detector::current>::fromLittle(value);
    }


    template <typename _Integer>
    _CONSTEXPR _Integer fromBigEndian(_Integer value) {

        return _hidden
                ::Converter<_hidden::Detector::current>::fromBig(value);
    }

} // namespace tools

#endif // _TOOLS_ENDIAN_HPP
