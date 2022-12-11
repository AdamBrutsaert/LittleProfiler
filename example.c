#include "profiler.h"

int main(void)
{
    PROFILE_FUNC();

        PROFILE_BEGIN("FirstPart");
            for (uint64_t i = 0; i < 320000000; i++)
                i++;
        PROFILE_END();

        PROFILE_BEGIN("SecondPart");
            for (uint64_t i = 0; i < 32000000; i++)
                i += 2;
        PROFILE_END();

    PROFILE_END();

    return 0;
}
