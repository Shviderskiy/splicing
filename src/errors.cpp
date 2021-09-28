#include "errors.hpp"


namespace splicing {


    class ErrorCategory final : public std::error_category {


        ErrorCategory() _NOEXCEPT = default;
        ErrorCategory(ErrorCategory const &) = delete;
        ErrorCategory& operator =(ErrorCategory const &) = delete;
        virtual ~ErrorCategory() override = default;

    public:

        virtual char const* name() const _NOEXCEPT override;
        virtual std::string message(int errorCode) const override;

        static std::error_category const& get() _NOEXCEPT;
    };


    char const* ErrorCategory::name() const _NOEXCEPT {

        return "splicing";
    }


    std::string ErrorCategory::message(int errorCode) const {

        switch (errorCode) {

            case _ERR_SUCCESS:
                return "Operation completed successfully";
            case _ERR_ALREADY_HOOKED:
                return "Hook is already setted";
            case _ERR_NOT_HOOKED_YET:
                return "Hook is not setted yet";
            case _ERR_INVALID_ADDRESS:
                return "Invalid address for hook setting";
            default:
                return "Unknown errror";
        }
    }


    std::error_category const& ErrorCategory::get() _NOEXCEPT {

        static ErrorCategory errorCategory;
        return errorCategory;
    }


    std::error_code makeErrorCode(Error error) _NOEXCEPT {

        return std::error_code(int(error), ErrorCategory::get());
    }


    std::error_code make_error_code(Error error) _NOEXCEPT {

        return makeErrorCode(error);
    }

} // namespace splicing
