# METIS 

METIS is a set of serial programs for partitioning graphs, partitioning finite element meshes, 
and producing fill reducing orderings for sparse matrices. The algorithms implemented in 
METIS are based on the multilevel recursive-bisection, multilevel k-way, and multi-constraint 
partitioning schemes developed in our lab.

##  Downloading METIS

You can download METIS by simply cloning it using the command:
```
git clone https://github.com/KarypisLab/METIS.git
```

## Building standalone METIS binaries and library

To build METIS you can follow the instructions below:

### Dependencies

General dependencies for building METIS are: gcc, cmake, build-essential. 
In Ubuntu systems these can be obtained from the apt package manager (e.g., apt-get install cmake, etc) 

```
sudo apt-get install build-essential
sudo apt-get install cmake
```

In addition, you need to download and install
[GKlib](https://github.com/KarypisLab/GKlib) by following the instructions there. 


### Building and installing METIS  

METIS is primarily configured by passing options to make config. For example:

```
make config shared=1 cc=gcc prefix=~/local
make install
```

will configure METIS to be built as a shared library using GCC and then install the binaries, header files, and libraries at 

```
~/local/bin
~/local/include
~/local/lib
```

directories, respectively.

### Common configuration options are:

    cc=[compiler]     - The C compiler to use [default is determined by CMake]
    shared=1          - Build a shared library instead of a static one [off by default]
    prefix=[PATH]     - Set the installation prefix [~/local by default]
    gklib_path=[PATH] - Set the prefix path where GKlib has been installed. You can skip
                        this if GKlib's installation prefix is the same as that of METIS.
    i64=1             - Sets to 64 bits the width of the datatype that will store information
                        about the vertices and their adjacency lists. 
    r64=1             - Sets to 64 bits the width of the datatype that will store information 
                        about floating point numbers.

### Advanced debugging related options:

    gdb=1           - Build with support for GDB [off by default]
    debug=1         - Enable debugging support [off by default]
    assert=1        - Enable asserts [off by default]
    assert2=1       - Enable very expensive asserts [off by default]

### Other make commands

    make uninstall
         Removes all files installed by 'make install'.

    make clean
         Removes all object files but retains the configuration options.

    make distclean
         Performs clean and completely removes the build directory.


## Copyright & License Notice
Copyright 1998-2020, Regents of the University of Minnesota

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

