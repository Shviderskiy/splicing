#include "lib.hpp"

#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <unordered_map>


#define _X_JOIN(_X, _Y) _X ## _Y
#define _JOIN(_X, _Y) _X_JOIN(_X, _Y)
#define _UNIQUE_NAME _JOIN(__, __COUNTER__)

#if defined(__x86_64__) // x64
#define _BITNESS_DEP_PREFIX \
    splicing::Byte _UNIQUE_NAME[1] { 0x48 };
#elif defined(__i386__) // x86
#define _BITNESS_DEP_PREFIX
#else // other
#error "System bitness is not supported Mr. Hacker!"
#endif


namespace splicing {


    struct Jump final {

        Jump(Address address) : code() {

            code.address = address;
        }

        // In x64 (16 bytes):
        // 50                             push   rax
        // 48 b8 78 56 34 12 78 56 34 12  movabs rax, 0x1234567812345678
        // 48 87 04 24                    xchg   QWORD PTR [rsp], rax
        // c3                             ret

        // In x86 (10 bytes):
        // 50              push   eax
        // b8 78 56 34 12  mov    eax, 0x12345678
        // 87 04 24        xchg   DWORD PTR [esp], eax
        // c3              ret 

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Weffc++"

        #pragma pack(push, 1)
        struct { // is POD

            Byte push[1] { 0x50 };
            _BITNESS_DEP_PREFIX Byte movabs[1] { 0xb8 };
            Address address;
            _BITNESS_DEP_PREFIX Byte xchg[3] { 0x87, 0x04, 0x24 };
            Byte ret[1] { 0xc3 };
        } code;
        #pragma pack(pop)

        #pragma GCC diagnostic pop
    };


    struct Backup final {

        Backup() = default;

        Backup(Address address) {

            ::memcpy(this->data, address, sizeof(this->data));
        }

        Backup(Backup const &other) {

            ::memcpy(this->data, other.data, sizeof(this->data));
        }
        
        Backup& operator =(Backup const &other) {

            ::memcpy(this->data, other.data, sizeof(this->data));
            return *this;
        }

        Byte data[sizeof(Jump::code)];
    };


    static struct Core final {

        Core() : backups() { init(); }

        std::unordered_map<Address, Backup> backups;
    } core; // create instance


    static Status enableRwx(Address address) {

        size_t pageSize = sysconf(_SC_PAGESIZE);
        void *page = reinterpret_cast<void*>(
            uintptr_t(address) & ~(pageSize - 1));
        int prot = PROT_READ | PROT_WRITE | PROT_EXEC;

        if (mprotect(page, pageSize, prot) != 0)
            return Status::error;

        return Status::success;
    }


    Status set_hook(Address target, Address hook) {

        if (core.backups.find(target) != core.backups.cend())
            return Status::error;

        if (enableRwx(target) != Status::success)
            return Status::error;

        core.backups[target] = Backup(target);

        Jump jump(hook);
        ::memcpy(target, &jump.code, sizeof(Jump::code));

        return Status::success;
    }


    Address restore(Address target) {

        if (core.backups.find(target) == core.backups.cend())
            return nullptr;

        if (enableRwx(target) != Status::success)
            return nullptr;

        Backup backup(core.backups[target]);
        ::memcpy(target, backup.data, sizeof(Backup::data));
        core.backups.erase(target);

        return target;
    }

} // namespace splicing
