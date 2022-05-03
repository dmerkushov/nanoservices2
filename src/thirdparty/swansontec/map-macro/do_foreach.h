/*
 * Copyright (C) 2012 William Swanson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or
 * their institutions shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization from the authors.
 */

#ifndef __MAP_H_INCLUDED
#define __MAP_H_INCLUDED

#define __MAP_H_EVAL0(...) __VA_ARGS__
#define __MAP_H_EVAL1(...) __MAP_H_EVAL0(__MAP_H_EVAL0(__MAP_H_EVAL0(__VA_ARGS__)))
#define __MAP_H_EVAL2(...) __MAP_H_EVAL1(__MAP_H_EVAL1(__MAP_H_EVAL1(__VA_ARGS__)))
#define __MAP_H_EVAL3(...) __MAP_H_EVAL2(__MAP_H_EVAL2(__MAP_H_EVAL2(__VA_ARGS__)))
#define __MAP_H_EVAL4(...) __MAP_H_EVAL3(__MAP_H_EVAL3(__MAP_H_EVAL3(__VA_ARGS__)))
#define __MAP_H_EVAL(...) __MAP_H_EVAL4(__MAP_H_EVAL4(__MAP_H_EVAL4(__VA_ARGS__)))

#define __MAP_H_MAP_END(...)
#define __MAP_H_MAP_OUT
#define __MAP_H_MAP_COMMA ,

#define __MAP_H_MAP_GET_END2() 0, __MAP_H_MAP_END
#define __MAP_H_MAP_GET_END1(...) __MAP_H_MAP_GET_END2
#define __MAP_H_MAP_GET_END(...) __MAP_H_MAP_GET_END1
#define __MAP_H_MAP_NEXT0(test, next, ...) next __MAP_H_MAP_OUT
#define __MAP_H_MAP_NEXT1(test, next) __MAP_H_MAP_NEXT0(test, next, 0)
#define __MAP_H_MAP_NEXT(test, next) __MAP_H_MAP_NEXT1(__MAP_H_MAP_GET_END test, next)

#define __MAP_H_MAP0(f, x, peek, ...) f(x) __MAP_H_MAP_NEXT(peek, __MAP_H_MAP1)(f, peek, __VA_ARGS__)
#define __MAP_H_MAP1(f, x, peek, ...) f(x) __MAP_H_MAP_NEXT(peek, __MAP_H_MAP0)(f, peek, __VA_ARGS__)

#define __MAP_H_MAP_LIST_NEXT1(test, next) __MAP_H_MAP_NEXT0(test, __MAP_H_MAP_COMMA next, 0)
#define __MAP_H_MAP_LIST_NEXT(test, next) __MAP_H_MAP_LIST_NEXT1(__MAP_H_MAP_GET_END test, next)

#define __MAP_H_MAP_LIST0(f, x, peek, ...) f(x) __MAP_H_MAP_LIST_NEXT(peek, __MAP_H_MAP_LIST1)(f, peek, __VA_ARGS__)
#define __MAP_H_MAP_LIST1(f, x, peek, ...) f(x) __MAP_H_MAP_LIST_NEXT(peek, __MAP_H_MAP_LIST0)(f, peek, __VA_ARGS__)

/**
 * Applies the function macro `f` to each of the remaining parameters.
 */
#define DO_FOREACH(f, ...) __MAP_H_EVAL(__MAP_H_MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters and
 * inserts commas between the results.
 */
#define DO_FOREACH_LIST(f, ...) __MAP_H_EVAL(__MAP_H_MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#endif // __MAP_H_INCLUDED