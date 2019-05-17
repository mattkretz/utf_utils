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

## Results

### `-march=skylake` on an Intel Core i7-6700 @ 3.40GHz

file\algo | iconv | llvm | av | std::codecvt | Boost.Text | Hoehrmann | kewb-basic | kewb-fast | kewb-sse | kewb-vir-simd
--------- | ----- | ---- | -- | ------------ | ---------- | --------- | ---------- | --------- | -------- | -------------
english_wiki.txt | 605 | 722 | 365 | 849 | 276 | 642 | 271 | 166 | 88 | 38
chinese_wiki.txt | 614 | 726 | 419 | 788 | 421 | 704 | 328 | 247 | 151 | 150
hindi_wiki.txt | 691 | 754 | 466 | 784 | 492 | 752 | 381 | 302 | 215 | 230
japanese_wiki.txt | 577 | 679 | 418 | 752 | 493 | 706 | 321 | 270 | 201 | 201
korean_wiki.txt | 671 | 821 | 479 | 827 | 476 | 762 | 374 | 297 | 193 | 205
portuguese_wiki.txt | 630 | 758 | 398 | 835 | 297 | 675 | 322 | 193 | 91 | 73
russian_wiki.txt | 649 | 851 | 475 | 881 | 504 | 746 | 373 | 277 | 197 | 206
swedish_wiki.txt | 636 | 770 | 404 | 820 | 298 | 681 | 336 | 198 | 94 | 74
stress_test_0.txt | 601 | 721 | 359 | 761 | 277 | 634 | 239 | 159 | 35 | 33
stress_test_1.txt | 440 | 472 | 396 | 517 | 558 | 633 | 238 | 240 | 239 | 232
stress_test_2.txt | 480 | 538 | 359 | 544 | 468 | 634 | 237 | 230 | 405 | 246
hindi_wiki_in_english.txt | 605 | 726 | 368 | 801 | 283 | 644 | 287 | 174 | 85 | 39
hindi_wiki_in_russian.txt | 674 | 858 | 486 | 811 | 481 | 757 | 407 | 289 | 202 | 214
kermit.txt | 639 | 781 | 431 | 786 | 380 | 708 | 374 | 226 | 136 | 94
z1_kosme.txt | 742 | 807 | 376 | 1078 | 308 | 523 | 306 | 174 | 84 | 94
z1_ascii.txt | 820 | 789 | 423 | 969 | 320 | 512 | 312 | 211 | 56 | 45
