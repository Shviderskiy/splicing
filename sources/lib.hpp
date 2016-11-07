#ifndef _SPLICING_LIB_HPP
#define _SPLICING_LIB_HPP

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE

#include <stdint.h>


namespace splicing {


    // need implemented as main
    void init();


    enum class Status {

        success = 0,
        error   = 1,
    };


    typedef uint8_t Byte;
    typedef void* Address;


    Status set_hook(Address target, Address hook);

    Address restore(Address target);


    template <typename _Address, typename _Function>
    Status set_hook(_Address target, _Function hook) {

        return set_hook(
            *reinterpret_cast<Address*>(&target),
            *reinterpret_cast<Address*>(&hook));
    }


    template <typename _Function, typename _Address>
    _Function* restore(_Address target) {

        _Function *result = nullptr;
        *reinterpret_cast<Address*>(&result) = 
            restore(reinterpret_cast<Address>(target));
        return result;
    }

} // namespace splicing


#endif // _SPLICING_LIB_HPP
