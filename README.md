## Overview

This is a fork of BobSteagall/utf_utils, to add an implementation using 
`std::experimental::simd` instead of SSE intrinsics. This makes it portable and 
scalable.

## Building and Testing

Just run `make bench-native` to create a build dir, run cmake, make, and 
execute the benchmark.

Note that you need GCC 9 and 
[VcDevel/std-simd](https://github.com/VcDevel/std-simd) to compile the code.  
Older GCC may also work, but does not optimize as good.
