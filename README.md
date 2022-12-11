# LittleProfiler

This is a small header-only profiler that measures execution time of code.

## How to use it

Just copy and paste the [`profiler.h`](profiler.h) header file in your project.  
Then you only need to include the header in the source files (`.c`) where you
want to use it.  
As simple as that.

### Profiling a function
```c
#include "profiler.h"

void func(void)
{
    PROFILE_FUNC();
    // ...
    PROFILE_END();
}
```

### Profiling a section
```c
#include "profiler.h"

void func(void)
{
    // ...
    PROFILE_BEGIN("my_section");
    // ...
    PROFILE_END();
    // ...
}
```

## Example

An example of the header is available [here](example.c).

To compile an run the example, open a terminal and type :
```
gcc -oexample example.c -O0 -std=c99 -W -Wall -Wextra -Wpedantic
./example
```

## Output

![](output.png)
