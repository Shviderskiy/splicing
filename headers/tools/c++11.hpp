#ifndef _TOOLS_C_PLUS_PLUS_11_HPP
#define _TOOLS_C_PLUS_PLUS_11_HPP


#define __DO_EXPAND(_X) _X ## 1
#define __EXPAND(_X)    __DO_EXPAND(_X)
#define _PP_IS_EMPTY(_MACRO) (__EXPAND(_MACRO) == 1)


#ifdef __GNUC__

#include <bits/c++config.h>
#define _CONSTEXPR _GLIBCXX_CONSTEXPR
#define _NOEXCEPT  _GLIBCXX_NOEXCEPT

#else

#define _CONSTEXPR
#define _NOEXCEPT

#endif

#endif // _TOOLS_C_PLUS_PLUS_11_HPP
