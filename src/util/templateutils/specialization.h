//
// Created by dmerkushov on 20.07.2021.
//

#ifndef NANOSERVICES2_SPECIALIZATION_H
#define NANOSERVICES2_SPECIALIZATION_H

namespace nanoservices {

// Found at https://stackoverflow.com/questions/16337610/how-to-know-if-a-type-is-a-specialization-of-stdvector

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

}; // namespace nanoservices

#endif // NANOSERVICES2_SPECIALIZATION_H
