//
// Created by dmerkushov on 04.08.2021.
//

#ifndef NANOSERVICES2_LAMBDATOFUNCTION_H
#define NANOSERVICES2_LAMBDATOFUNCTION_H

#include <functional>

namespace nanoservices {

namespace detail {

template<typename F>
struct function_traits : public function_traits<decltype(&F::operator())> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const> {
    using function_type = std::function<R(Args...)>;
};

} // namespace detail

template<typename F>
using function_type_t = typename detail::function_traits<F>::function_type;

template<typename F>
function_type_t<F> toFunction(F lambda) {
    return static_cast<function_type_t<F>>(lambda);
}

} // namespace nanoservices

#endif // NANOSERVICES2_LAMBDATOFUNCTION_H
