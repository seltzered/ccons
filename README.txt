ccons README
============

The goal of ccons is to provide an interactive console for the C programming
language, similar to what irb is for Ruby.


How to build:
=============

1. Follow the steps on the "Clang - Getting Started" page:

   http://clang.llvm.org/get_started.html

2. Move the ccons directory to llvm/tools/ccons.

3. Install editline 3.0 from:

   http://www.thrysoee.dk/editline/

4. Install CMake if necessary.

5. Run "cmake CMakeLists.txt" from the ccons directory.

6. Build ccons using the build files generated by CMake (ie: 'make' on Unix).


License
=======

ccons is distributed under the terms of the MIT license. Please see the LICENSE
for details.

