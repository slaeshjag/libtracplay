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


TRACPLAY_FILE *modInit(void *data, size_t data_len) {
	int i, max;
	char data_r;
	TRACPLAY_FILE *mod;
	MOD_PATTERN_HEADER *header = (void *) &data_r[MOD_PATTERN_INFO_POS];
	MOD_SAMPLE *sample = (void *) &data_r[MOD_SAMPLE_META_POS_LOW];

	if (modValidateMemory(data, data_len) < 0)
		return NULL;
	if ((mod = malloc(sizeof(TRACPLAY_FILE))) == NULL)
		return NULL;
	
	trackerInit(mod);
	mod->pattern_refs = header->patterns
	
	for (max = i = 0; i < mod->pattern_refs; i++)
		if (header->pattern_table[i] > max)
			max = header->pattern_table[i];
	mod->patterns = max + 1;
	mod->repeat_jump = (header->repeat_jump == 127) ? 0 : header->repeat_jump;
	if ((mod->pattern_ref = trackerPatternPrepare(mod->patterns)) == NULL) {
		trackerDestroy(mod);
		return NULL;
	}
	
	for (i = 0; i < mod->patterns; i++) {
		if ((mod->pattern_ref[i] = trackerPatternAdd(MOD_PATTERN_LINES)) == NULL) {
			trackerDestroy(mod);
			return NULL;
		}

		/* TODO: Iterate the pattern data */
	}

	mod->samples = MOD_MK_SAMPLES;
	if ((mod->sample = trackerSamplePrepare(mod->samples)) == NULL) {
		trackerDestroy(mod);
		return NULL;
	}
	
	/* Lower 15 samples */
	for (i = 0; i < 15; i++) {
		if ((mod->sample[i] = trackerSampleAddMeta(modEndianConvert(sample[i].length << 1, sample[i].tune))) == NULL) {
			trackerDestroy(mod);
			return NULL;
		}
		/* TODO: Add repeat stuff */
	}
	

	return mod;
}


TRACPLAY_FILE *modLoad(const char *fname) {
	FILE *fp;
	void *data;
	size_t data_len;
	TRACPLAY_FILE *mod;

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
