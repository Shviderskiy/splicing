#include "api.hpp"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <mutex>
#include <unordered_map>

#include "tools/c++11.hpp"
#include "tools/spin_lock.hpp"

#include "errors.hpp"
#include "jump.hpp"
#include "memory_permissions.hpp"


namespace splicing {


    struct Api final : public IApi {

        Api();
        Api(Api const &) = delete;
        Api& operator =(Api const &) = delete;
        virtual ~Api() override = default;

        virtual std::error_code
        trySetHook(void *target, void *hook) _NOEXCEPT override;
        virtual std::error_code
        tryRestore(void *target) _NOEXCEPT override;

        virtual void setHook(void *target, void *hook) override;
        virtual void restore(void *target) override;

        typedef std::unordered_map<void*, uint8_t[sizeof(Jump)]> SavedRegions;

    private:

        tools::SpinLock _spinLock;
        SavedRegions _savedRegions;
    };


    Api::Api() : _spinLock(), _savedRegions() { }


    std::error_code Api::trySetHook(void *target, void *hook) _NOEXCEPT {

        std::lock_guard<tools::SpinLock> _(this->_spinLock);

        if (this->_savedRegions.find(target) != this->_savedRegions.cend())
            return makeErrorCode(Error::alreadyHooked);

        bitfield_t oldMp;

        if (setMemoryPermissions(target, sizeof(Jump), _MP_RWX, oldMp))
            return makeErrorCode(Error::invalidAddress);

        memcpy(this->_savedRegions[target], target, sizeof(Jump));
        new (target) Jump(hook);

        if (oldMp != _MP_UNKNOWN)
            if (setMemoryPermissions(target, sizeof(Jump), oldMp))
                return makeErrorCode(Error::invalidAddress);

        return makeErrorCode(Error::success);
    }


    std::error_code Api::tryRestore(void *target) _NOEXCEPT {

        std::lock_guard<tools::SpinLock> _(this->_spinLock);

        SavedRegions::iterator iterator = this->_savedRegions.find(target);

        if (iterator == this->_savedRegions.cend())
            return makeErrorCode(Error::notHookedYet);

        bitfield_t oldMp;

        if (setMemoryPermissions(target, sizeof(Jump), _MP_RWX, oldMp))
            return makeErrorCode(Error::invalidAddress);

        memcpy(target, this->_savedRegions[target], sizeof(Jump));
        this->_savedRegions.erase(iterator);

        if (oldMp != _MP_UNKNOWN)
            if (setMemoryPermissions(target, sizeof(Jump), oldMp))
                return makeErrorCode(Error::invalidAddress);

        return makeErrorCode(Error::success);
    }


    void Api::setHook(void *target, void *hook) {

        if (std::error_code errorCode = this->trySetHook(target, hook))
            throw std::system_error(errorCode);
    }


    void Api::restore(void *target) {


        if (std::error_code errorCode = this->tryRestore(target))
            throw std::system_error(errorCode);
    }


    IApi& api() {

        static Api api;
        return api;
    }

} // namespace splicing
