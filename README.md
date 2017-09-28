# LaBGen-OF

[![OpenCV Version](https://img.shields.io/badge/opencv-≥3.1-blue.svg)](http://opencv.org) [![Build Status](https://travis-ci.org/benlaug/labgen-of.svg?branch=master)](https://travis-ci.org/benlaug/labgen-of)

LaBGen-OF is a patch-based stationary background generation method introduced in a paper submitted to ACIVS 2017, and based on [LaBGen](https://github.com/benlaug/labgen). The purpose of this repository is twofold:

1. To share the source code of the method.
2. To embed the method in a ready-to-use program.

![Graphical Abstract](.readme/graphical-abstract.png)

Here is a video showing some backgrounds estimated by LaBGen-OF (click on the image below to play it):

[![Demonstration of LaBGen-OF](https://img.youtube.com/vi/6tzzY65sCzc/0.jpg)](https://www.youtube.com/watch?v=6tzzY65sCzc "Click to play")

## Compiling the program

The program implementing the method has been developed in C++11 and is distributed under the [GPLv3](LICENSE) license. In order to compile it, you need a modern C++ compiler, a copy of the [Boost](http://www.boost.org) library, a copy of the [OpenCV 3](http://opencv.org) (at least 3.1) library **with contrib modules**, and the [CMake](https://cmake.org) build automation tool. On UNIX-like environments, the program can be compiled as follows, considering that your terminal is in the source code directory:

```
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

## Running the program

Once the program has been compiled, the following command gives the complete list of available options:

```
$ ./LaBGen-OF-cli --help
```

As an example, the IBMtest2 sequence of the [SBI dataset](http://sbmi2015.na.icar.cnr.it/SBIdataset.html) [[4](#references)] can be processed with the default set of parameters as follows:

```
$ ./LaBGen-OF-cli -i path_to_IBMtest2/IBMtest2_%6d.png -o my_output_path -d -v
```

![Screenshot](.readme/screenshot.png)

A full documentation of the options of the program is [available on the wiki](https://github.com/benlaug/labgen-of/wiki/Arguments-of-the-program).

## Citation

If you use LaBGen-OF in your work, please cite paper [[1](#references)] as below:

```
@inproceedings{Laugraud2017IsAMemoryless,
  title = {Is a Memoryless Motion Detection Truly Relevant for Background Generation with {LaBGen}?},
  author = {B. Laugraud and M. {Van Droogenbroeck}},
  booktitle = {Advanced Concepts for Intelligent Vision Systems (ACIVS)},
  publisher = {Springer},
  series = {Lecture Notes in Computer Science},
  year = {2017},
  month = Sep,
  address = {Antwerp, Belgium}
}
```

## Alternatives

* The original patch-based version of [LaBGen](https://github.com/benlaug/labgen).
* A pixel-level variant of LaBGen called [LaBGen-P](https://github.com/benlaug/labgen-p).

## Testing

Each commited revision is automatically tested using [Travis CI](https://travis-ci.org/benlaug/labgen-of) on:

* Ubuntu 14.04 with the `g++` compiler and OpenCV 3.3 (with contrib modules) compiled from the sources.
* OS X El Capitan with the `clang++` compiler and OpenCV 3.3 (with contrib modules) installed with [Homebrew](https://brew.sh).

## References

[1] [B. Laugraud, M. Van Droogenbroeck. Is a Memoryless Motion Detection Truly Relevant for Background Generation with LaBGen?. *Advanced Concepts for Intelligent Vision Systems (ACIVS)*, 2017.](http://hdl.handle.net/2268/213147)

[2] L. Maddalena, A. Petrosino. Towards Benchmarking Scene Background Initialization. *International Conference on Image Analysis and Processing Workshops (ICIAP Workshops)*, 9281:469-476, 2015.
