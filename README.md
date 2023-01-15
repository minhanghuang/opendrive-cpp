# opendrive-cpp

## #1 Dependency

[tinyxml2](https://github.com/leethomason/tinyxml2/tree/master)

## #2 Build

```
make build 
cd build
cmake ..
make -j4
```

## #3 Instructions

- find package

```
pkg_check_modules(OpenDriveCpp REQUIRED opendrive-cpp)
include_directories(
  ${OpenDriveCpp_INCLUDE_DIRS}
)

link_directories(
  ${OpenDriveCpp_LIBRARY_DIRS}
)
target_link_libraries(${TARGET_NAME}
  ${OpenDriveCpp_LIBRARIES}
)
```

- parse xodr file

```cpp
#include <opendrive-cpp/opendrive.h>
const std::string file_path = "town.xodr";
opendrive::Parser parser;
auto ele_map = std::make_shared<opendrive::element::Map>();
parser.ParseMap(file_path, ele_map);
```

