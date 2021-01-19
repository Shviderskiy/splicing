#include "memory_permissions.hpp"

#include <stdint.h>
#include <assert.h>


namespace splicing {


    bitfield_t _unused;


#if defined(__linux__)


    // TODO: clarify the value of argument "size"
    std::error_code setMemoryPermissions(
            void *address, size_t size, bitfield_t mp, bitfield_t &old) {

        old = _MP_UNKNOWN;

        void *page = reinterpret_cast<void*>(
            uintptr_t(address) & ~(sysconf(_SC_PAGESIZE) - 1));
        size += static_cast<uint8_t *>(address) -
                static_cast<uint8_t *>(page);
        return mprotect(page, size, mp) == 0
            ? std::error_code(0, std::system_category())
            : std::error_code(errno, std::system_category());
    }

#elif defined(_WIN32) || defined(_WIN64)


    std::error_code setMemoryPermissions(
            void *address, size_t size, bitfield_t mp, bitfield_t &old) {

        return VirtualProtect(address, size, mp, &old) == TRUE
            ? std::error_code(0, std::system_category())
            : std::error_code(GetLastError(), std::system_category());
    }

#endif

} // namespace splicing
