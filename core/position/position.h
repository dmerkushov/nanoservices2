//
// Created by dmerkushov on 09.07.2021.
//

#ifndef NANOSERVICES2_POSITION_H
#define NANOSERVICES2_POSITION_H

// Sonar advises using std::source_location of C++20, but GCC 10.3.0 doesn't support it
#define NS_POSITION \
    ((std::string("") + __FILE__ + ":" + std::to_string(__LINE__) + ", in " + __PRETTY_FUNCTION__).c_str())

#endif // NANOSERVICES2_POSITION_H
