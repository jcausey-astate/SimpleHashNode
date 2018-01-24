# `SimpleHashNode`
`SimpleHashNode` is a C++ implementation of the "node", the basic building block for a cryptographic hash chain (i.e. "Blockchain").  

## Information
This repository contains the header `SimpleHashNode.h` and implementation `SimpleHashNode.h` as well as a demo/driver program to demonstrate their use (`demo.cpp`).  It was created for fun, but I haven't had time to work on it further.

To create a simple hash chain, you can use an STL container like `std::list` where the values are `SimpleHashNode` objects (see demo program).  Ideally, you would create a class to more specifically represent the hash chain and automatically check the integrity of the chain as nodes are added.  This work is left as an exercise for th reader for now.

This implementation does not contain any distributed consensus algorithm (i.e. "Proof of Work" or "Proof of Stake", etc.), so it cannot be used as-is for a secure distributed ledger; you could add the necessary components if you desire.

## Dependencies
The code relies on the [PicoSHA2](https://github.com/okdshin/PicoSHA2) library for hashing; it was chosen for its simplicity, not necessarily because it was the best choice for constructing a secure ledger.  If you extend this work, you should review your options.  The library is included as a Git submodule, you need to run the following command from within the _SimpleHashNode_ directory after you clone this repository:

```bash
git submodule update --init --recursive
```

## Building the Demo
The demo can be built simply by running:

```bash
clang++ -std=c++14 -o SHC_demo demo.cpp SimpleHashChain.cpp
```
for the Clang compiler package, or
```bash
clang++ -std=c++14 -o SHC_demo demo.cpp SimpleHashChain.cpp
```
for the GNU compiler suite.  

Run with `./SHC_demo` on a Linux-like system or MacOS.


## License: MIT

Copyright 2018 Jason L Causey, Arkansas State University

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
