//
// Created by dmerkushov on 09.07.2021.
//

#ifndef NANOSERVICES2_POSITION_H
#define NANOSERVICES2_POSITION_H

#include <memory>
#include <string>

// Sonar advises using std::source_location of C++20, but GCC 10.3.0 doesn't support it
#define NS_POSITION ((std::string("") + __FILE__ + ":" + std::to_string(__LINE__) + ", in " + __PRETTY_FUNCTION__).c_str())

#define NS_POSITION_SHAREDPTR (std::make_shared<std::string>(NS_POSITION))

#endif // NANOSERVICES2_POSITION_H
