[![Build status](https://ci.appveyor.com/api/projects/status/ik2dgnluc7qe89mw/branch/main?svg=true)](https://ci.appveyor.com/project/mgorshkov/pd/branch/main)

# About
Methods from pandas library on top of NP library.

# Requirements
Any C++20-compatible compiler:
* gcc 8 or higher
* clang 10 or higher
* Visual Studio 2019 or higher

# Repo
```
git clone https://github.com/mgorshkov/pd.git
```

# Build unit tests and sample
## Linux/MacOS
```
mkdir build && cd build
cmake ..
cmake --build .
```
## Windows
```
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

# Build docs
```
cmake --build . --target doc
```

Open scipy/build/doc/html/index.html in your browser.

# Install
```
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=~/pd_install
cmake --build . --target install
```

# Usage example (samples/read_csv)
```
#include <iostream>

#include <pd/read_csv.hpp>

int main(int, char **) {
    using namespace pd;

    auto data = read_csv("https://github.com/adityakumar529/Coursera_Capstone/blob/master/diabetes.csv");
    std::cout << "data.shape=" << data.shape() << std::endl;
    return 0;
}
```
# How to build the sample

1. Clone the repo
```
git clone https://github.com/mgorshkov/pd.git
```
2. cd samples/read_csv
```
cd samples/read_csv
```
3. Make build dir
```
mkdir -p build-release && cd build-release
```
4. Configure cmake
```
cmake -DCMAKE_BUILD_TYPE=Release ..
```
5. Build
## Linux/MacOS
```
cmake --build .
```
## Windows
```
cmake --build . --config Release
```
6. Run the app
```
$./read_csv

```

# Links
* C++ numpy-like template-based array implementation: https://github.com/mgorshkov/np
* Scientific methods on top of NP library: https://github.com/mgorshkov/scipy
* ML Methods from scikit-learn library: https://github.com/mgorshkov/sklearn
