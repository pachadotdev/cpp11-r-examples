
# cpp11gaussjordan

<!-- badges: start -->
<!-- badges: end -->

The goal of cpp11gaussjordan is to show an implementation of the Gauss-Jordan algorithm that uses cpp11 vendoring capabilities.

I wrote about cpp11 vendoring in my [blog](https://pacha.dev/blog/2023/05/23/cpp11-vendoring/).

## Installation

You can install the development version of cpp11gaussjordan from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("pachadotdev/cpp11gaussjordan")
```

## Example

This is a basic example which shows you how to invert a matrix:

``` r
library(cpp11gaussjordan)

A <- matrix(c(2,1,3,-1), nrow = 2, ncol = 2)
invert_matrix(A)

> invert_matrix(A)
     [,1] [,2]
[1,]  0.2  0.6
[2,]  0.2 -0.4
```
