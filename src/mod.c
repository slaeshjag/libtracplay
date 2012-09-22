#include "tracplay.h"


unsigned short modEndianConvert(unsigned short word) {
	MOD_ENDIAN_CONVERT from;
	unsigned short to;

	from.word = word;
	to = from.byte[1];
	to += (from.byte[0] << 8);

	return to;
}


int modValidateMemory(void *data, size_t data_len) {
	char *data_r = data;

	if (data_len <= MOD_SIGNATURE_POS)
		return -1;
	if (memcmp(&data_r[MOD_SIGNATURE_POS], MOD_SIGNATURE) == 0)
		return 0;
	return -1;
}


int modValidate(const char *fname) {
	FILE *fp;
	char buff[4];

	if ((fp = fopen(fname, "rb")) == NULL)
		return -1;
	
	fseek(fname, MOD_SIGNATURE_POS, fp);
	if (ftell(fp) != MOD_SIGNATURE_POS) {
		fclose(fp);
		return -1;
	}

	fread(buff, 4, 1, fp);
	fclose(fp);
	
	if (memcmp(buff, MOD_SIGNATURE) != 0)
		return -1;
	return 0;
}


int modSampleAdd(TRACKER_FILE *mod, MOD_SAMPLE *sample, int slot, int i) {
	if (trackerInstrumentAdd(&mod->instrument[slot], 1) < 0) {
		trackerDestroy(mod);
		return -1;
	}

	if (trackerSampleAdd(mod->instrument[slot].sample, (modEndianConvert(sample[i].length) - 1) << 1, 1, sample[i].tune) < 0)
		trackerDestroy(mod);
		return -1;
	}
	
	mod->instrument[slot].sample->repeat_offset = modEndianConvert(sample[i].repeat_offset) << 1;
	mod->instrument[slot].sample->repeat_end = (modEndianConvert(sample[i].repeat_lenght) << 1) + mod->instrument[slot].sample->repeat_offset;

	if ((modEndianConvert(sample[i].repeat_lenght) << 1) > 0)
		mod->instrument[slot].sample->repeat_type = TRACKER_SAMPLE_REPEAT_FORWARD;
	else
		mod->instrument[slot].sample->repeat_type = TRACKER_SAMPLE_REPEAT_NONE;
	
	return 0;
}
	


TRACKER_FILE *modInit(void *data, size_t data_len) {
	int i, j, k, max, t;
	unsigned int period;
	char data_r;
	TRACKER_FILE *mod;
	MOD_PATTERN_HEADER *header = (void *) &data_r[MOD_PATTERN_INFO_POS];
	MOD_PATTERN_DATA *pattern = (void *) &data_r[MOD_PATTERN_DATA_POS]
	MOD_SAMPLE *sample = (void *) &data_r[MOD_SAMPLE_META_POS_LOW];

	if (modValidateMemory(data, data_len) < 0)
		return NULL;
	if ((mod = malloc(sizeof(TRACKER_FILE))) == NULL)
		return NULL;
	
	trackerInit(mod);
	mod->pattern_refs = header->patterns
	
	for (max = i = 0; i < mod->pattern_refs; i++)
		if (header->pattern_table[i] > max)
			max = header->pattern_table[i];
	mod->patterns = max + 1;
	mod->repeat_jump = (header->repeat_jump == 127) ? 0 : header->repeat_jump;
	if (trackerPatternPrepare(mod) < 0) {
		trackerDestroy(mod);
		return NULL;
	}
	
	for (i = 0; i < mod->patterns; i++) {
		if ((mod->pattern[i] = trackerPatternAdd(MOD_PATTERN_LINES, MOD_PATTERN_COLS)) == NULL) {
			trackerDestroy(mod);
			return NULL;
		}
		
		for (j = 0; j < MOD_PATTERN_LINES; j++) {
			for (k = 0; k < MOD_PATTERN_COLS; k++) {
				period = pattern[t].sample_period_high << 4;
				period += pattern[t].sample_period_low;
				period <<= 1;
				t = MOD_PATTERN_LINES * i * MOD_PATTERN_COLS + MOD_PATTERN_COLS * j + k;
				mod->pattern[i].col[k].row[j].play_rate = MOD_AMIGA_FREQ / period;
				mod->pattern[i].col[k].row[j].instrument = pattern[t].sample_number_low + (pattern[t].sample_number_high << 4);
				mod->pattern[i].col[k].row[j].effect = pattern[t].sample_effect_high;
				mod->pattern[i].col[k].row[j].effect_arg1 = (pattern[t].sample_effect_low & 0xF0) >> 4;
				mod->pattern[i].col[k].row[j].effect_arg2 = (pattern[t].sample_effect_low & 0xF);
			}
		}
	}

	mod->instruments = MOD_MK_SAMPLES;
	if ((mod->instrument = trackerSamplePrepare(mod->instruments)) == NULL) {
		trackerDestroy(mod);
		return NULL;
	}
	
	for (i = 0; i < 31; i++)
		modSampleAdd(mod, sample, i, i);
	

	return mod;
}


TRACPLAY_FILE *modLoad(const char *fname) {
	FILE *fp;
	void *data;
	size_t data_len;
	TRACKER_FILE *mod;

	if (modValidate(fname) < 0)
		return NULL;
	if ((fp = fopen(fname, "rb")) == NULL)
		return NULL;

	fseek(fp, 0, SEEK_END);
	data_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if ((data = malloc(data_len)) == NULL) {
		fclose(fp);
		return NULL;
	}

	fread(data, data_len, 1, fp);
	fclose(fp);

	mod = modInit(data, data_len);
	free(data);
	return mod;
}
