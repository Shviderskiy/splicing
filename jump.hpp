#ifndef _SPLICING_JUMP_HPP
#define _SPLICING_JUMP_HPP

#include "tools/keywords_override_protection.hpp"

#include <stdint.h>

#include "tools/c++11.hpp"
#include "tools/endian.hpp"


namespace splicing {


    namespace intel_86_64 {


        #pragma pack(push, 1)
        template <typename _Integer>
        struct JmpNear final {

            _CONSTEXPR JmpNear(_Integer currentAddress,
                               _Integer absoluteAddress)
                : _operationCode(0xe9)
                , _relativeAddress(
                      tools::toLittleEndian<uint32_t>(
                          absoluteAddress - currentAddress - sizeof(JmpNear)))
            { }

            _CONSTEXPR JmpNear(void *currentAddress, void *absoluteAddress)
                : JmpNear(_Integer(currentAddress), _Integer(absoluteAddress))
            { static_assert(sizeof(void*) == sizeof(_Integer), "Wrong size"); }

            _CONSTEXPR JmpNear(void *absoluteAddress)
                : JmpNear(this, absoluteAddress)
            { }

            ~JmpNear() = default;

        private:

            uint8_t  _operationCode;
            uint32_t _relativeAddress;
        };
        #pragma pack(pop)

    } // namespace intel_86_64


#if defined(__i386__)
    typedef intel_86_64::JmpNear<uint32_t> Jump;
#elif defined(__x86_64__)
    typedef intel_86_64::JmpNear<uint64_t> Jump;
#else
#error "Processor architecture is not supported Mr. Hacker!"
#endif

} // namespace splicing

#endif // _SPLICING_JUMP_HPP
