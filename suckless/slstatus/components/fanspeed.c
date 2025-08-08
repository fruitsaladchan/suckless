#include <stdio.h>
#include <stdint.h>

#include "../slstatus.h"
#include "../util.h"

const char *
fanspeed(const char *path)
{
    int fan_rpm;

    if (pscanf(path, "%d", &fan_rpm) != 1)
        return NULL;

    return bprintf("%d RPM", fan_rpm);
}
