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


int trackerInstrumentPrepare(TRACKER_FILE *file, unsigned int instruments) {
	int i;

	if ((file->instrument = malloc(sizeof(TRACKER_INSTRUMENT) * instruments)) == NULL)
		return -1;
	
	file->instruments = instruments;
	for (i = 0; i < instruments; i++) {
		file->instrument[i].sample = NULL;
		file->instrument[i].samples = 0;
	}

	return 0;
}


int trackerInstrumentAdd(TRACKER_INSTRUMENT *instrument, int samples) {
	int i;

	if ((instrument->sample = malloc(sizeof(TRACKER_SAMPLE) * samples)) == NULL)
		return -1;
	
	instrument->samples = samples;
	for (i = 0; i < samples; i++) {
		instrument->sample[i].sample = NULL;
		instrument->sample[i].samples = 0;
		instrument->sample[i].sample_channels = 0;
		instrument->sample[i].octave = 0;
		instrument->sample[i].octave_tune = 0;
		instrument->sample[i].repeat_type = TRACKER_SAMPLE_REPEAT_NONE;
		instrument->sample[i].repeat_start = 0;
		instrument->sample[i].repeat_end = 0;
	}

	return 0;
}


int trackerSampleAdd(TRACKER_SAMPLE *sample, const unsigned int lenght, const unsigned int channels, signed int tune) {
	if ((sample->sample = malloc(sizeof(unsigned short) * lenght * channels)) == NULL)
		return -1;
	
	memset(sample->sample, 0, lenght * channels);
	return 0;
}


int trackerPatternRefs(TRACKER_FILE *file, int pattern_refs) {
	int i;

	if ((file->pattern_ref = malloc(sizeof(unsigned int) * pattern_refs)) == NULL)
		return -1;
	
	file->pattern_refs = pattern_refs;
	for (i = 0; i < pattern_refs; i++)
		file->pattern_ref[i] = 0;
	
	return 0;
}


int trackerPatternPrepare(TRACKER_FILE *file, int patterns) {
	int i;

	if ((file->pattern = malloc(sizeof(TRACKER_PATTERN) * patterns)) == NULL)
		return -1;
	file->patterns = patterns;

	for (i = 0; i < patterns; i++) {
		file->pattern[i].col = NULL;
		file->pattern[i].cols = 0;
	}

	return 0;
}


int trackerPatternAdd(TRACKER_PATTERN *pattern, unsigned int rows, unsigned int cols) {
	int i, j;

	if ((pattern->col = malloc(sizeof(TRACKER_PATTERN_COL) * cols)) == NULL)
		return -1;
	for (i = 0; i < cols; i++) {
		pattern->col[i].row = NULL;
		pattern->col[i].rows = 0;
	}

	for (i = 0; i < cols; i++) {
		if ((pattern->col[i].row = malloc(sizeof(TRACKER_PATTERN_ROW) * rows)) == NULL)
			return -1;
		pattern->col[i].rows = rows;
		for (j = 0; j < rows; j++) {
			pattern->col[i].row[j].play_rate = 0;
			pattern->col[i].row[j].instrument = 0;
			pattern->col[i].row[j].volume = 0;
			pattern->col[i].row[j].extra_data = 0;
			pattern->col[i].row[j].effect = 0;
			pattern->col[i].row[j].effect_arg1 = 0;
			pattern->col[i].row[j].effect_arg2 = 0;
		}
	}

	return 0;
}


TRACKER_FILE *trackerInit() {
	TRACKER_FILE *file;
	int i;
	
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

	for (i = 0; i < TRACKER_EFFECTS; i++)
		file->effect[i] = NULL;

	return file;
}


void *trackerDestroy(TRACKER_FILE *file) {
	int i, j;

	for (i = 0; i < file->instruments; i++) {
		for (j = 0; j < file->instrument[i].samples; j++)
			free(file->instrument[i].sample[j].sample);
		free(file->instrument[i].sample);
	}

	free(file->instrument);

	for (i = 0; i < file->patterns; i++) {
		for (j = 0; j < file->pattern[i].cols; j++)
			free(file->pattern[i].col[j].row);
		free(file->pattern[i].col);
	}

	free(file->pattern);
	free(file->pattern_ref);
	
	return NULL;
}
