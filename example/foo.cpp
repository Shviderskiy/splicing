// file - foo.cpp

#include <stdio.h>
#include <stdlib.h>


int bar(int param) {

    ::printf("-> bar [executable]\n");
    ::printf("   param =  %d\n", param);
    return param + 1;
}


int main() {

    ::printf("-> main [executable]\n");

    // check LD_PRELOAD
    char *ldPreload = ::getenv("LD_PRELOAD");
    if (ldPreload != nullptr)
        ::printf("   LD_PRELOAD=%s\n", ::getenv("LD_PRELOAD"));
    else
        ::printf("   LD_PRELOAD not found\n");

    ::printf("   &bar = %p\n", reinterpret_cast<void*>(bar));

    int result = bar(100500);
    ::printf("   result = %d\n", result);

    return 0;
}
