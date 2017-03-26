#ifndef _SPLICING_MEMORY_PERMISSIONS_HPP
#define _SPLICING_MEMORY_PERMISSIONS_HPP

#include "tools/keywords_override_protection.hpp"

#if defined(__linux__)

#include <unistd.h>
#include <sys/mman.h>

typedef int bitfield_t;

#define _MP_U   (bitfield_t(-1))
#define _MP_N   (PROT_NONE)
#define _MP_R   (PROT_READ)
#define _MP_W   (PROT_WRITE)
#define _MP_X   (PROT_EXEC)
#define _MP_RW  (PROT_READ | PROT_WRITE)
#define _MP_RX  (PROT_READ | PROT_EXEC)
#define _MP_WX  (PROT_EXEC | PROT_WRITE)
#define _MP_RWX (PROT_READ | PROT_WRITE | PROT_EXEC)

#elif defined(_WIN32) || defined(_WIN64)

#include <windows.h>

typedef DWORD bitfield_t;

#define _MP_U   (bitfield_t(-1))
#define _MP_N   (PAGE_NOACCESS)
#define _MP_R   (PAGE_READONLY)
#define _MP_W   (PAGE_WRITECOPY)
#define _MP_X   (PAGE_EXECUTE)
#define _MP_RW  (PAGE_READWRITE)
#define _MP_RX  (PAGE_EXECUTE_READ)
#define _MP_WX  (PAGE_EXECUTE_WRITECOPY)
#define _MP_RWX (PAGE_EXECUTE_READWRITE)

#else
#error "OS is not supported Mr. Hacker!"
#endif

#define _MP_UNKNOWN _MP_U
#define _MP_NONE    _MP_N


#include <system_error>


namespace splicing {


    extern bitfield_t _unused;


    std::error_code setMemoryPermissions(
            void *address, size_t size,
            bitfield_t mp, bitfield_t &old = _unused);


} // namespace splicing

#endif // _SPLICING_MEMORY_PERMISSIONS_HPP
