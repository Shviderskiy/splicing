#ifndef _TOOLS_C_PLUS_PLUS_11_HPP
#define _TOOLS_C_PLUS_PLUS_11_HPP


#define __DO_JOIN(_X, _Y) _X ## _Y

#define __EXPAND(_X) __DO_JOIN(_X, 1)
#define _PP_IS_EMPTY(_MACRO) (__EXPAND(_MACRO) == 1)

#define __JOIN(_X, _Y) __DO_JOIN(_X, _Y)
#define _UNIQUE_NAME __JOIN(__, __COUNTER__)


#ifdef __GNUC__

#include <bits/c++config.h>
#define _CONSTEXPR _GLIBCXX_CONSTEXPR
#define _NOEXCEPT  _GLIBCXX_NOEXCEPT

#else

#define _CONSTEXPR
#define _NOEXCEPT

#endif

#endif // _TOOLS_C_PLUS_PLUS_11_HPP
