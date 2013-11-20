/*******************************************************************************
 * Copyright (c) 2013 Wojciech Migda
 * All rights reserved
 * Distributed under the terms of the MIT license
 *******************************************************************************
 *
 * Filename: main.cpp
 *
 * Description:
 *      description
 *
 * Authors:
 *          Wojciech Migda (wm)
 *
 *******************************************************************************
 * History:
 * --------
 * Date         Who  Ticket     Description
 * ----------   ---  ---------  ------------------------------------------------
 * 2013-11-20   wm              Initial version
 *
 ******************************************************************************/

#ifdef __GNUC__
    #if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 7)
        #error Your GCC compiler version does not support operator"" yet.
    #endif
#endif

#include "StaticStringHash.hpp"
#include <iostream>

int main()
{
    std::cout << std::hex;
    std::cout << "[empty string] - " << ""_hash << std::endl;
    std::cout << "[space] - " << " "_hash << std::endl;
    std::cout << "foo - " << "foo"_hash << std::endl;
    std::cout << "Hello world! - " << "Hello world!"_hash << std::endl;
    std::cout << "Hello World! - " << "Hello World!"_hash << std::endl;
    std::cout << "HELLO WORLD! - " << "HELLO WORLD!"_hash << std::endl;
    std::cout << "The quick brown fox jumps over the lazy dog 1234567890 times - " << "The quick brown fox jumps over the lazy dog 1234567890 times"_hash << std::endl;
    return 0;
}
