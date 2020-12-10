#ifndef _TOOLS_INITIALIZATION_HPP
#define _TOOLS_INITIALIZATION_HPP

#include "tools/keywords_override_protection.hpp"

#include <stddef.h>

#include "tools/c++11.hpp"


namespace tools {


    template <size_t _Id>
    struct StaticInitializer final {


        template <typename _Function>
        StaticInitializer(_Function &&function) { function(); }
    };

} // namespace tools


#define _STATIC_INITIALIZATION_BLOCK \
    static tools::StaticInitializer<__COUNTER__> _UNIQUE_NAME = []


#endif // _TOOLS_INITIALIZATION_HPP
