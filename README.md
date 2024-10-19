# Learning C++ as an R user

Here I show how to achieve some common tasks by integrating R and C++.

All the examples here use the cpp11 package, which is a header-only library that
provides a C++11 interface to R's C API.

## Contents

The examples here are meant to be *instructional*, and the goal is to be
clear and simple, not to be efficient except when it is clearly stated. For
example, the OLS example uses a naive approach to solve the problem, which is
**not** the way R's `lm()` function does it. See
[this blog post](https://pacha.dev/blog/2023/06/05/cpp11-omp/).

Please see the respective README files for more information.

### [cpp11dummyols](#cpp11dummyols)

- [x] Integers
- [x] Doubles
- [x] Doubles matrices
- [x] Conditionals

### [cpp11omp](#cpp11omp)

- [x] Integers
- [x] Doubles
- [x] Lists
- [x] Loops (for)
- [x] OpenMP parallelization

### [cpp11gaussjordan](#cpp11gaussjordan)

- [x] Integers
- [x] Doubles
- [x] Doubles matrices
- [x] Conditionals
- [x] Loops (for)
- [x] Vendoring

### [cpp11simplexphase2](#cpp11simplexphase2)

- [x] Integers
- [x] Doubles
- [x] Doubles matrices
- [x] Conditionals
- [x] Loops (for and while)
- [x] Messages
