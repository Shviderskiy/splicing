#ifndef _SPLICING_JUMP_HPP
#define _SPLICING_JUMP_HPP

#include "tools/keywords_override_protection.hpp"

#include <stdint.h>

#include "tools/c++11.hpp"
#include "tools/endian.hpp"


namespace splicing {


    namespace intel_86_64 {


        #pragma pack(push, 1)
        union Jump
        {
            Jump(void const *destinationAddress)
                : Jump(this, destinationAddress)
            { }

            Jump(void const *currentAddress, void const *destinationAddress)
                : Jump(uintptr_t(currentAddress),
                       uintptr_t(destinationAddress))
            { }

        private:

            Jump(uintptr_t currentAddress, uintptr_t destinationAddress)
            {
                using JmpNearDistance = decltype(JmpNear::distance);
                int64_t maxDistance = int64_t(~JmpNearDistance(0));

                uintptr_t targetAddress = currentAddress + sizeof(JmpNear);
                int64_t distance = destinationAddress - targetAddress;
                if (std::abs(distance) < maxDistance)
                {
                    jmpNear.operationCode = 0xe9;
                    jmpNear.distance =
                            tools::toLittleEndian(JmpNearDistance(distance));
                }
                else
                {
#if defined(__x86_64__)
                    jmpX64.pushRax = 0x50;
                    jmpX64.movRaxValue.x64Prefix = 0x48;
                    jmpX64.movRaxValue.operationCode = 0xb8;
                    jmpX64.movRaxValue.value =
                            tools::toLittleEndian(destinationAddress);
                    jmpX64.xchgDerefRspRax.x64Prefix = 0x48;
                    jmpX64.xchgDerefRspRax.operationCode = 0x87;
                    jmpX64.xchgDerefRspRax.code[0] = 0x04;
                    jmpX64.xchgDerefRspRax.code[1] = 0x24;
                    jmpX64.ret = 0xc3;
#endif
                }
            }

            struct JmpNear
            {
                uint8_t  operationCode;
                uint32_t distance;
            };

            struct MovRegImm
            {
                uint8_t  x64Prefix;
                uint8_t  operationCode;
                uint64_t value;
            };

            struct XchgDerefRegReg
            {
                uint8_t x64Prefix;
                uint8_t operationCode;
                uint8_t code[2];
            };

            // 50                             push   rax
            // 48 b8 ef cd ab 90 78 56 34 12  movabs rax,0x1234567890abcdef
            // 48 87 04 24                    xchg   QWORD PTR [rsp],rax
            // c3                             ret
            struct JmpX64
            {
                uint8_t         pushRax;
                MovRegImm       movRaxValue;
                XchgDerefRegReg xchgDerefRspRax;
                uint8_t         ret;
            };

            JmpNear jmpNear;
#if defined(__x86_64__)
            JmpX64  jmpX64;
#endif
        };
        #pragma pack(pop)

    } // namespace intel_86_64


#if defined(__i386__) || defined(__x86_64__)
    typedef intel_86_64::Jump Jump;
#else
#error "Processor architecture is not supported Mr. Hacker!"
#endif

} // namespace splicing

#endif // _SPLICING_JUMP_HPP
