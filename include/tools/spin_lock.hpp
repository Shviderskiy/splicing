#ifndef _TOOLS_SPIN_LOCK_HPP
#define _TOOLS_SPIN_LOCK_HPP

#include "tools/keywords_override_protection.hpp"

#include <string.h>
#include <atomic>

#include "tools/c++11.hpp"


namespace tools {


    struct SpinLock final {


        _CONSTEXPR SpinLock() : _state(ATOMIC_FLAG_INIT) { }
        SpinLock(SpinLock const &) = delete;
        SpinLock& operator =(SpinLock const &) = delete;
        ~SpinLock() = default;


        void lock() {

            while (this->_state.test_and_set(std::memory_order_acquire));
        }


        bool try_lock() {

            return !this->_state.test_and_set(std::memory_order_acquire);
        }


        void unlock() {

            this->_state.clear(std::memory_order_release);
        }

    private:

        std::atomic_flag _state;
    };

} // namespace tools

#endif // _TOOLS_SPIN_LOCK_HPP
