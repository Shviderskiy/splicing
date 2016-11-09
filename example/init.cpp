// file - init.cpp

#include "../sources/lib.hpp"

#include <stdio.h>
#include <stdlib.h>


static void* bar_address = reinterpret_cast<void*>(0x80484fd);


// hook for bar
int my_bar(int param) {

    ::printf("-> my_bar [library]\n");
    ::printf("   param = %d\n", param);

    int(*origin_bar)(int) =
            splicing::restore<int(int)>(::bar_address);

    if (origin_bar == nullptr) {

        ::fprintf(stderr, "splicing: can't restore %p\n", bar_address);
        ::exit(-1);
    }

    int fictive_param = 400;
    return origin_bar(fictive_param);
}


// need implement as main
void splicing::init() {

    ::printf("-> splicing::init [library]\n");
    ::printf("   &my_bar = %p\n", reinterpret_cast<void*>(my_bar));

    // covering its tracks
    ::unsetenv("LD_PRELOAD");

    splicing::Status status =
            splicing::set_hook(::bar_address, my_bar);

    if (status == splicing::Status::error) {

        ::fprintf(stderr, "splicing: can't set_hook to %p\n", bar_address);
        ::exit(-1);
    }
}
