/*
    MIT License

    Copyright (c) 2020-2022 Basit Ayantunde

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once
#include <iostream>
#include <format>

template<class E>
class Err {
private:
    E inner;
public:
    explicit constexpr Err(E&& value) : inner(std::move(value)) {}
    constexpr E&& consume() {
        return std::move(inner);
    }
};

template<class T>
class Ok {
private:
    T inner;
public:
    explicit constexpr Ok(T&& value) : inner(std::move(value)) {}
    constexpr T&& consume() {
        return std::move(inner);
    }
};

template<class T>
class Some {
private:
    T inner;
public:
    explicit constexpr Some(T&& value) : inner(std::move(value)) {}
    constexpr T&& consume() {
        return std::move(inner);
    }
};

template<class T, class E>
class Result {
private:
    T inner;
    E error_value;
    bool is_ok;
public:
    constexpr Result(Err<E> error) : error_value(error.consume()), is_ok(false), inner() {};
    constexpr Result(Ok<T>&& object) : inner(std::move(object.consume())), is_ok(true), error_value() {};

    constexpr T unwrap() {
        if (is_ok) {
            return std::move(inner);
        }
        else {
            if constexpr (std::is_default_constructible_v<std::formatter<E>>) {
                throw std::exception(std::format("\'called `Result::unwrap()` on an `Err` value: {}\'", error_value).c_str());
            } else 
            {
                throw std::exception("\'called `Result::unwrap()` on an `Err` value: ()\'");
            }
        }
    }
    constexpr T unwrap_or(T&& or_value) {
        if (is_ok) {
            return std::move(inner);
        }
        else {
            return std::move(or_value);
        }
    }
};

class NoneType {};

constexpr NoneType const None{};

template<class T>
class Option {
private:
    T inner;
    bool is_some;
public:
    constexpr Option(NoneType none) : is_some(false), inner() {};
    constexpr Option(Some<T>&& object) : inner(std::move(object.consume())), is_some(true) {};

    constexpr T unwrap() {
        if (is_some) {
            return std::move(inner);
        }
        else {
            throw std::exception("\'called `Option::unwrap()` on an `None` value: ()\'");
        }
    }

    constexpr T unwrap_or(T&& or_value) {
        if (is_some) {
            return std::move(inner);
        }
        else {
            return std::move(or_value);
        }
    }
};