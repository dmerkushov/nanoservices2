//
// Created by dmerkushov on 09.07.2021.
//

#ifndef NANOSERVICES2_POSITION_H
#define NANOSERVICES2_POSITION_H

#include <memory>
#include <source_location>
#include <string>

#if __cplusplus > 201703L && __has_builtin(__builtin_source_location)
#    define __NANOSERVICES2_LOC (std::source_location::current())
#    define NS_POSITION ((std::string("") + __NANOSERVICES2_LOC.file_name() + ":" + std::to_string(__NANOSERVICES2_LOC.line()) + ", in " + __NANOSERVICES2_LOC.function_name()).c_str())
#else
#    ifdef __GNUC__
#        define NS_POSITION ((std::string("") + __FILE__ + ":" + std::to_string(__LINE__) + ", in " + __PRETTY_FUNCTION__).c_str())
#    else
#        define NS_POSITION ((std::string("") + __FILE__ + ":" + std::to_string(__LINE__)).c_str())
#    endif
#endif

#define NS_POSITION_SHAREDPTR (std::make_shared<std::string>(NS_POSITION))

#endif // NANOSERVICES2_POSITION_H
