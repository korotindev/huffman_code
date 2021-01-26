#pragma once

#include <iostream>
#include <type_traits>

template <typename T>
void WritePrimitive(std::ostream& out, T t, size_t n = sizeof(T)) {
  static_assert(std::is_integral<T>::value, "Integral required.");
  out.write(reinterpret_cast<const char*>(&t), n);
}

template <typename T>
void WritePrimitive(std::ostream& out, const T* t, size_t n = sizeof(T)) {
  static_assert(std::is_integral<T>::value, "Integral required.");
  out.write(reinterpret_cast<const char*>(t), n);
}

template <typename T>
void ReadPrimitive(std::istream& in, T& t, size_t n = sizeof(T)) {
  static_assert(std::is_integral<T>::value, "Integral required.");
  in.read(reinterpret_cast<char*>(&t), n);
}

template <typename T>
void ReadPrimitive(std::istream& in, T* t, size_t n = sizeof(T)) {
  static_assert(std::is_integral<T>::value, "Integral required.");
  in.read(reinterpret_cast<char*>(t), n);
}