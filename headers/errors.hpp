#ifndef _SPLICING_ERRORS_HPP
#define _SPLICING_ERRORS_HPP

#include "tools/keywords_override_protection.hpp"

#include <system_error>

#include "tools/c++11.hpp"


#define _ERR_SUCCESS         0
#define _ERR_ALREADY_HOOKED  1
#define _ERR_NOT_HOOKED_YET  2
#define _ERR_INVALID_ADDRESS 3


namespace splicing {


    enum class Error {

        success        = _ERR_SUCCESS,
        alreadyHooked  = _ERR_ALREADY_HOOKED,
        notHookedYet   = _ERR_NOT_HOOKED_YET,
        invalidAddress = _ERR_INVALID_ADDRESS,
    };


    std::error_code makeErrorCode(Error error) _NOEXCEPT;

    // for implicit error_code instance construction
    std::error_code make_error_code(Error error) _NOEXCEPT;

} // namespace splicing


namespace std {


    template <>
    struct is_error_code_enum<splicing::Error> {

        static typename true_type::value_type const value = true_type::value;
    };

} // namespace std

#endif // _SPLICING_ERRORS_HPP
