// file - foo.cpp

#include <stdio.h>


int bar(int param) {

    
    ::printf("-> bar [executable]\n");
    ::printf("   param =  %d\n", param);
    return param + 1;
}


int main() {

    ::printf("-> main [executable]\n");
    ::printf("   &bar = %p\n", reinterpret_cast<void*>(bar));
    
    int result = bar(100500);
    ::printf("   result = %d\n", result);
    
    return 0;
}
