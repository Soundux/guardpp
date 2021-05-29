## guardpp
A cross-platform C++17 library that can restrict your application to a single instance.

## Compatibility
| Platform | Mechanism           | Dependencies |
| -------- | ------------------- | ------------ |
| Windows  | Shared Memory Mutex | pthread      |
| Linux    | Shared Memory Mutex | WinAPI       |

## Usage

- Add the library to your project
  - ```cmake
    add_subdirectory(/path/to/guardpp)
    link_libraries(guard)
    ```
## Features
- Tiny & Easy to use
- Prevents issues when previous process crashed

## Usage
```cpp
#include <guard.hpp>
#include <iostream>

int main()
{
    Instance::Guard instanceGuard("test");

    if (instanceGuard.isAnotherRunning())
    {
        std::cerr << "Only one instance allowed" << std::endl;
        return 1;
    }

    //...

    // Shared Memory Mutex is free'd upon destruction of the Guard instance.
    return 0;
}
```