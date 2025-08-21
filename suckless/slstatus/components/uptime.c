/* See LICENSE file for copyright and license details. */
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "../slstatus.h"
#include "../util.h"

#if defined(CLOCK_BOOTTIME)
	#define UPTIME_FLAG CLOCK_BOOTTIME
#elif defined(CLOCK_UPTIME)
	#define UPTIME_FLAG CLOCK_UPTIME
#else
	#define UPTIME_FLAG CLOCK_MONOTONIC
#endif

const char *
uptime(const char *unused)
{
	char warn_buf[256];
	uintmax_t d, h, m;
	struct timespec uptime;

	if (clock_gettime(UPTIME_FLAG, &uptime) < 0) {
		snprintf(warn_buf, sizeof(warn_buf), "clock_gettime %d", UPTIME_FLAG);
		warn(warn_buf);
		return NULL;
	}

	// Calculate days, hours, and minutes
	d = uptime.tv_sec / (3600 * 24);  // Days
	h = (uptime.tv_sec % (3600 * 24)) / 3600;  // Hours
	m = (uptime.tv_sec % 3600) / 60;  // Minutes

	// If uptime is more than 24 hours, return the days along with hours and minutes
	if (d > 0) {
		return bprintf("%jud %juh %jum", d, h, m);
	} else {
		// Otherwise, just show hours and minutes
		return bprintf("%juh %jum", h, m);
	}
}
