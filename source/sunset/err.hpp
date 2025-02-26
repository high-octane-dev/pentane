#pragma once

#include <iostream>
#include <format>

// Use custom Result/Option implementations if STX is not present.
#ifndef STX_VERSION
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
#else
template <typename T, typename Error>
using Result = stx::Result<T, Error>;
using stx::Option, stx::None, stx::Some;
#endif