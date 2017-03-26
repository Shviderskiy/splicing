#ifndef _SPLICING_API_HPP
#define _SPLICING_API_HPP

#include "tools/keywords_override_protection.hpp"

#include <system_error>

#include "tools/c++11.hpp"


namespace splicing {


    struct IApi {


        virtual ~IApi() = default;

        virtual std::error_code
        trySetHook(void *target, void *hook) _NOEXCEPT = 0;
        virtual std::error_code
        tryRestore(void *target) _NOEXCEPT = 0;

        virtual void setHook(void *target, void *hook) = 0;
        virtual void restore(void *target) = 0;
    };


    IApi& api();

} // namespace splicing

#endif // _SPLICING_API_HPP
