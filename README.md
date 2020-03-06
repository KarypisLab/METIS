# METIS 

METIS is a set of serial programs for partitioning graphs, partitioning finite element meshes, 
and producing fill reducing orderings for sparse matrices. The algorithms implemented in 
METIS are based on the multilevel recursive-bisection, multilevel k-way, and multi-constraint 
partitioning schemes developed in our lab.

##  Downloading METIS

METIS uses Git submodules to manage external dependencies. Hence, please specify the `--recursive` option while cloning the repo as follow:
```
git clone --recursive https://github.com/KarypisLab/METIS.git
```

## Building standalone METIS binaries and library

To build METIS you can follow the instructions below:

### Dependencies

General dependencies for building slim are: gcc, cmake, build-essential.
In Ubuntu systems these can be obtained from the apt package manager (e.g., apt-get install cmake, etc) 

```
sudo apt-get install build-essential
sudo apt-get install cmake
```

### Building and installing METIS  

METIS is primarily configured by passing options to make config. For example:

```
make config shared=1 cc=gcc prefix=~/local
make install
```

will configure metis to be built as a shared library using GCC and then install the binaries, header files, and libraries at 

```
~/local/bin
~/local/include
~/local/lib
```

directories, respectively.

### Common configuration options are:

    cc=[compiler]   - The C compiler to use [default is determined by CMake]
    shared=1        - Build a shared library instead of a static one [off by default]
    prefix=[PATH]   - Set the installation prefix [~/local by default]
    i64=1           - Sets to 64 bits the width of the datatype that will store information
                      about the vertices and their adjacency lists. 
    r64=1           - Sets to 64 bits the width of the datatype that will store information 
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


<!---
## Getting started

Here are some examples to quickly try out SLIM on the sample datasets that are provided with SLIM.

### Python interface

```python
import pandas as pd
from SLIM import SLIM, SLIMatrix

#read training data stored as triplets <user> <item> <rating>
traindata = pd.read_csv('../test/AutomotiveTrain.ijv', delimiter = ' ', header=None)
trainmat = SLIMatrix(traindata)

#set up parameters to learn model, e.g., use Coordinate Descent with L1 and L2
#regularization
params = {'algo':'cd', 'nthreads':2, 'l1r':1.0, 'l2r':1.0}

#learn the model using training data and desired parameters
model = SLIM()
model.train(params, trainmat)

#read test data having candidate items for users
testdata = pd.read_csv('../test/AutomotiveTest.ijv', delimiter = ' ', header=None)
#NOTE: model object is passed as an argument while generating test matrix
testmat = SLIMatrix(testdata, model)

#generate top-10 recommendations
prediction_res = model.predict(testmat, nrcmds=10, outfile = 'output.txt')

#dump the model to files on disk
model.save_model(modelfname='model.csr', # filename to save the model as a csr matrix
                 mapfname='map.csr' # filename to save the item map
                )

#load the model from from disk
model_new = SLIM()
model_new.load_model(modelfname='model.csr', # filename of the model
                 mapfname='map.csr' # filename of the item map
                )
```

The users can also refer to the python notebook [UserGuide.ipynb](./python-package/UserGuide.ipynb) located at
`./python-package/UserGuide.ipynb` for more examples on using the python api.

###  Command-line programs
SLIM can be used by running the command-line programs that are located under `./build` directory. Specifically, SLIM provides the following three command-line programs:
- `slim_learn`: for estimating a model
- `slim_predict`: for applying a previously estimated model, and
- `slim_mselect`: for exploring a set of hyper-parameters in order to select the best performing model.

Additional information about how to use these command-line programs is located in
SLIM's reference manual that is available at
[./doxygen/html/index.html](http://glaros.dtc.umn.edu/gkhome/files/fs/sw/slim/doc/html/index.html)
or
[./doxygen/latex/refman.pdf](http://glaros.dtc.umn.edu/gkhome/files/fs/sw/slim/doc/refman.pdf).

###  Library interface

You can also use SLIM by direclty linking into your C/C++ program via its library interface. SLIM's API is described 
in SLIM's reference manual (see links above).

## Citing
If you use any part of this library in your research, please cite it using the
following BibTex entry:

```
@online{slim,
  title = {{SLIM Library for Recommender Systems}},
  author = {Ning, Xia and Nikolakopoulos, Athanasios N. and Shui, Zeren and Sharma, Mohit and Karypis, George},
  url = {https://github.com/KarypisLab/SLIM},
  year = {2019},
}
```

## References
1. [Slim: Sparse linear methods for top-n recommender systems](http://glaros.dtc.umn.edu/gkhome/node/774)
## Credits & Contact Information

This implementation of SLIM was written by George Karypis with contributions by Xia Ning, Athanasios N. Nikolakopoulos, Zeren Shui and Mohit Sharma.

If you encounter any problems or have any suggestions, please contact George Karypis at <a href="mailto:karypis@umn.edu">karypis@umn.edu</a>.

-->

## Copyright & License Notice
Copyright 1998-2020, Regents of the University of Minnesota

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

