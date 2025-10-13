## The Default Guidelines

This document is based on [Jan Wilmans' C++ Guidelines](https://github.com/janwilmans/guidelines) (MIT Licensed),
which themselves are inspired by and reference the official [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines) (CC-BY 4.0).

Original copyright © Jan Wilmans. \
Some sections have been modified or extended to suit this project.

-   [H.1] Turn on warnings! and use sanitizers [[P.12]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p12-use-supporting-tools-as-appropriate) (see [details](warnings.md) here)
-   [H.2] No raw loops [(Sean Parent, C++ Seasoning)](https://www.youtube.com/watch?v=qH6sSOr-yk8)
-   [H.3] Avoid global mutable state [[I.2]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-global) [[I.3]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-singleton)
-   [H.4] Keep the scope of variables and type declarations as limited as possible. [examples](examples.md#keep-scope-as-limited-as-possible)
-   [H.5] Initialize all variables at declaration. [[meme]](https://github.com/janwilmans/guidelines/assets/5933444/4592cf74-7957-46e8-8133-0d065bab56d8)
-   [H.6] Use `constexpr` or `const` whenever you can [[P.10]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p10-prefer-immutable-data-to-mutable-data) (but no const for member variables and return types [[C.12]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c12-dont-make-data-members-const-or-references-in-a-copyable-or-movable-type)). [[meme]](https://github.com/janwilmans/guidelines/assets/5933444/e1f32720-76e9-41d2-a2cd-c7167a6fe881)
-   [H.7] Use `[[nodiscard]]` for all `const` member functions returning a value.
-   [H.8] Avoid returning values using arguments. [[F.20]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#f20-for-out-output-values-prefer-return-values-to-output-parameters) [[F.21]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#f21-to-return-multiple-out-values-prefer-returning-a-struct)
-   [H.9] Use automatic resource management (RAII). [[P.8]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p8-dont-leak-any-resources) [[C.31]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c31-all-resources-acquired-by-a-class-must-be-released-by-the-classs-destructor)
-   [H.10] Follow the [rule of 0](https://en.cppreference.com/w/cpp/language/rule_of_three) or the rule of 5 in that order.
-   [H.11] Avoid owning raw pointers. [[I.11]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-raw) [[F.26]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#f26-use-a-unique_ptrt-to-transfer-ownership-where-a-pointer-is-needed) [[doc]](https://en.cppreference.com/w/cpp/memory)
-   [H.12] Avoid manual memory management using `new`, `delete`, `malloc`, `free`, etc.
    -   When working with Qt the use of the new keyword is explicitly allowed.
-   [H.13] Do not use [C-style casts](https://en.cppreference.com/w/cpp/language/explicit_cast). [[meme]](https://github.com/janwilmans/guidelines/assets/5933444/27784daa-1ed8-4d75-9482-0e3e2be1aae7)
-   [H.14] Do not add member variables to classes used as interfaces. (Interfaces are defined as pure virtual classes that have a virtual = default destructor)
-   [H.15] Do not use `protected` member variables.
-   [H.16] Avoid the use of `volatile`, `const_cast`, `reinterpret_cast`, `typedef`, `register`, `extern`, `protected` or `va_arg`
-   [H.17] Make all destructors of classes used in runtime polymorphism virtual. [[C.35]](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c35-a-base-class-destructor-should-be-either-public-and-virtual-or-protected-and-non-virtual)
-   [H.18] Avoid references as data members of a class

---

The following MIT license applies **to the portions of this document derived from that work**:

MIT License

Copyright (c) 2023 Jan Wilmans

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
