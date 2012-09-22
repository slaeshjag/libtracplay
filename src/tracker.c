#include "tracplay.h"


signed short trackerExpand8Bit(signed char sample) {
	signed short target;
	int t;

	target = sample << 8;
	t = abs(sample);
	t <<= 1;
	if (t > 0) t--;
	target += t;

	return target;
}


TRACKER_FILE *trackerInit() {
	TRACKER_FILE *file;
	
	if ((file = malloc(sizeof(TRACKER_FILE))) == NULL)
		return NULL;
	
	file->instrument = NULL;
	file->instruments = 0;
	file->pattern = NULL;
	file->patterns = 0;
	file->pattern_ref = NULL;
	file->pattern_refs = 0;
	file->state.sample_ref = 0;
	file->state.row = 0;
	file->state.row_time = 0;
	file->state.row_time_total = 0;

	return file;
}


