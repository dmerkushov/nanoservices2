//
// Created by dmerkushov on 19.07.2021.
//

#ifndef NANOSERVICES2_CLASSNAME_H
#define NANOSERVICES2_CLASSNAME_H

/**
 * @brief Get the compile-time name of the class.
 * @details Usable only in non-static member functions
 * @details Found at https://stackoverflow.com/questions/1666802/is-there-a-class-macro-in-c
 */
#define NANOSERVICES2_CLASSNAME (typeid(*this).name())

#endif // NANOSERVICES2_CLASSNAME_H
