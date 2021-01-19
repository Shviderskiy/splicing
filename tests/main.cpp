#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mutex>

#include "tools/static_initialization.hpp"
#include "tools/endian.hpp"
#include "tools/spin_lock.hpp"

#include "api.hpp"
#include "errors.hpp"
#include "jump.hpp"


namespace tools {


    namespace tests {


        static bool isStaticInitilizationBlockExecuted = false;


        _STATIC_INITIALIZATION_BLOCK {

            isStaticInitilizationBlockExecuted = true;
        };


        void staticInitialization() {

            assert(isStaticInitilizationBlockExecuted);
        }


        void swapBytes() {

            assert(tools::swapBytes(uint8_t(0x11)) ==
                   uint8_t(0x11));
            assert(tools::swapBytes(uint16_t(0x1122)) ==
                   uint16_t(0x2211));
            assert(tools::swapBytes(uint32_t(0x11223344)) ==
                   uint32_t(0x44332211));
            assert(tools::swapBytes(uint64_t(0x1122334455667788)) ==
                   uint64_t(0x8877665544332211));
        }


        void spinLock() {

            {
                tools::SpinLock spinlock;
                assert(spinlock.try_lock() == true);
                assert(spinlock.try_lock() == false);
            }
            {
                tools::SpinLock spinlock;
                spinlock.lock();
                assert(spinlock.try_lock() == false);
                spinlock.unlock();
                assert(spinlock.try_lock() == true);
            }
            {
                tools::SpinLock spinlock;
                std::lock_guard<tools::SpinLock> lock_guard(spinlock);
                assert(spinlock.try_lock() == false);
            }
            {
                tools::SpinLock spinlock;
                {
                    std::lock_guard<tools::SpinLock> lock_guard(spinlock);
                    assert(spinlock.try_lock() == false);
                }
                assert(spinlock.try_lock() == true);
            }

        }

    } // namespace tests

} // namespace tools


namespace splicing {


    namespace tests {


        template <typename _Ret, typename... _Args>
        void* voidPtr(_Ret(*func)(_Args...));

        int foo(int param);
        int bar(int param);

        void* fakeMalloc(size_t size);


        void api() {

            {
                assert(splicing::makeErrorCode(Error::success) ==
                       splicing::Error::success);
                assert(splicing::makeErrorCode(Error::success) !=
                       std::errc(int(splicing::Error::success)));
            }
            {
                assert(splicing::api().trySetHook(nullptr, nullptr) ==
                       splicing::Error::invalidAddress);
                assert(splicing::api().tryRestore(nullptr) ==
                       splicing::Error::notHookedYet);
            }
            {
                uint8_t buffer[sizeof(splicing::Jump)];
                memset(buffer, 0x00, sizeof(buffer));

                void *hook = reinterpret_cast<void*>(0xCAFEBABE);
                assert(splicing::api().trySetHook(buffer, hook) ==
                       splicing::Error::success);

                splicing::Jump jump(buffer, hook);
                assert(memcmp(buffer, &jump, sizeof(splicing::Jump)) == 0);
                assert(splicing::api().tryRestore(buffer) ==
                       splicing::Error::success);
                assert(memcmp(buffer, &jump, sizeof(splicing::Jump)) != 0);

            }
            {
                void *mallocAddr = voidPtr(malloc);
                void *fakeMallocAddr = voidPtr(fakeMalloc);

                uint8_t backup[sizeof(splicing::Jump)];
                assert(splicing::api().trySetHookUnsafe(
                           mallocAddr, fakeMallocAddr, backup) ==
                       splicing::Error::success);
                assert(malloc(10) == reinterpret_cast<void*>(10));
                assert(splicing::api().tryRestoreUnsafe(mallocAddr, backup) ==
                       splicing::Error::success);
            }
            {
                int(*volatile fooPtr)(int) = foo;
                int(*volatile barPtr)(int) = bar;

                assert(fooPtr(10) == 11);
                assert(barPtr(10) == 9);

                void *fooAddr = voidPtr(foo);
                void *barAddr = voidPtr(bar);

                assert(splicing::api().trySetHook(fooAddr, barAddr) ==
                       splicing::Error::success);
                assert(fooPtr(10) == 9);
                assert(splicing::api().trySetHook(fooAddr, barAddr) ==
                       splicing::Error::alreadyHooked);

                assert(splicing::api().tryRestore(fooAddr) ==
                       splicing::Error::success);
                assert(fooPtr(10) == 11);
                assert(splicing::api().tryRestore(fooAddr) ==
                       splicing::Error::notHookedYet);

                {
                    uint8_t buffer[sizeof(splicing::Jump)];
                    std::error_code errorCode;
                    errorCode =
                            splicing::api().trySetHookUnsafe(
                                fooAddr, barAddr, buffer);
                    assert(errorCode == splicing::Error::success);
                    assert(fooPtr(10) == 9);
                    errorCode =
                            splicing::api().tryRestoreUnsafe(
                                fooAddr, buffer);
                    assert(errorCode == splicing::Error::success);

                }
            }
        }


        template <typename _Ret, typename... _Args>
        void* voidPtr(_Ret(*func)(_Args...)) {

            typedef _Ret(*FuncPtr)(_Args...);

            static_assert(sizeof(FuncPtr) == sizeof(void*), "Wrong size!");

            union {
                FuncPtr _func;
                void *result;
            };

            _func = func;
            return result;
        }


        int foo(int param) {

            return param + 1;
        }


        int bar(int param) {

            return param - 1;
        }

        void* fakeMalloc(size_t size)
        {
            return reinterpret_cast<void*>(size);
        }

    } // namespace tests

} // namespace splicing


int main() {

    tools::tests::staticInitialization();
    tools::tests::swapBytes();
    tools::tests::spinLock();

    splicing::tests::api();

    printf("All tests passed.\n");

    return 0;
}
