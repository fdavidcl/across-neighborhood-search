# across-neighborhood-search
An implementation of Across Neighborhood Search and variants

## Usage

Just compile the code and run either `bin/ans` for a single run, or `bin/run` for a complete set of 25 runs. You can later process the results with `stats/stats.R`. Use `make doc` to compile the documentation (only available in Spanish).

## CEC 2014 functions

The code implementing the test functions has not been included in this repository due to the lack of a free license. However, it can be found at [P. N. Suganthan's public site](http://web.mysites.ntu.edu.sg/epnsugan/PublicSite/Shared%20Documents/CEC-2014/cec14-c-code.zip). Only the `cec14_test_func.cpp` file and the `input_data` directory have been used, in the following structure:

* `include/`
    * `cec14_test_func.hxx`
* `input_data/`
* `src/`


## Algorithms

The following algorithms are implemented in their own files within the `src/` directory.

### Across Neighborhood Search (ANS)

The original ANS algorithm designed by Guohua Wu:

* Wu, G. (2016). [Across neighborhood search for numerical optimization](https://arxiv.org/pdf/1401.3376.pdf). Information Sciences, 329, 597-618.

### ANS-Memory

This variant adds a memory to each individual that allows it to continue searching within its own neighborhood when improvements are found.

### Hybrid-ANS (HANS)

This variant of ANS attempts to optimize the best global solution found so that individuals can infer characteristics from it later.

#### Local search algorithms

The Simplex, Solis-Wets and CMAES algorithms are implemented inside the `lib/localsearch` directory, from the *realea* library by [@dmolina](https://github.com/dmolina).

## License

This code is licensed under [GPLv3](https://github.com/fdavidcl/across-neighborhood-search/blob/master/LICENSE). [What does this mean?](https://tldrlegal.com/license/gnu-general-public-license-v3-%28gpl-3%29)
