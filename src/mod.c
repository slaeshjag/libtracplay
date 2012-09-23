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
	if (memcmp(&data_r[MOD_SIGNATURE_POS], MOD_SIGNATURE, 4) == 0 || memcmp(&data_r[MOD_SIGNATURE_POS], MOD_SIGNATURE2, 4) == 0)
		return 0;
	return -1;
}


int modValidate(const char *fname) {
	FILE *fp;
	char buff[4];

	if ((fp = fopen(fname, "rb")) == NULL)
		return -1;
	
	fseek(fp, MOD_SIGNATURE_POS, SEEK_SET);
	if (ftell(fp) != MOD_SIGNATURE_POS) {
		fprintf(stderr, "Mod-file is too short!\n");
		fclose(fp);
		return -1;
	}

	fread(buff, 4, 1, fp);
	fclose(fp);
	
	if ((memcmp(buff, MOD_SIGNATURE, 4) != 0) && (memcmp(buff, MOD_SIGNATURE2, 4) != 0))
		return -1;
	
	return 0;
}


void modCompileExtendedEffect(TRACKER_FILE *mod, TRACKER_PATTERN_ROW *row) {
	/* TODO: Fix */
	row->effect = TRACKER_EFFECT_NONE;

	return;
}


void modCompileEffect(TRACKER_FILE *mod, TRACKER_PATTERN_ROW *row) {

	if (row->effect == MOD_EFFECT_EXTENDED) {
		modCompileExtendedEffect(mod, row);
		return;
	}

	if (row->effect == MOD_EFFECT_ARPEGGIO) {
		row->effect = (row->effect_arg1 == 0 && row->effect_arg2 == 0) ? TRACKER_EFFECT_NONE : TRACKER_EFFECT_ARPEGGIO;
	} else if (row->effect == MOD_EFFECT_SLIDE_UP) {
		row->effect = TRACKER_EFFECT_SLIDE;
		row->effect_arg1 = ((row->effect_arg1 << 4) + row->effect_arg2) * -1;
	} else if (row->effect == MOD_EFFECT_SLIDE_DOWN) {
		row->effect = TRACKER_EFFECT_SLIDE;
		row->effect_arg1 = (row->effect_arg1 << 4) + row->effect_arg2;
	} else if (row->effect == MOD_EFFECT_SLIDE_TO_NOTE) {
		row->effect = TRACKER_EFFECT_SLIDE_TO_NOTE;
		row->effect_arg1 = (row->effect_arg1 << 4) + row->effect_arg2;
	} else if (row->effect == MOD_EFFECT_VIBRATO) {
		row->effect = TRACKER_EFFECT_VIBRATO;
	} else if (row->effect == MOD_EFFECT_SLIDE_TO_NOTE_WITH_VOL) {
		row->effect = TRACKER_EFFECT_SLIDE_NOTE_WITH_VOLUME;
		row->effect_arg1 = (row->effect_arg1 > 0) ? row->effect_arg1 : row->effect_arg2;
		row->effect_arg2 <<= 2;		/* Internally, volume is 0..256 */
	} else if (row->effect == MOD_EFFECT_VIBRATO_WITH_VOL_SLIDE) {
		row->effect = TRACKER_EFFECT_VIBRATO_WITH_VOLUME_SLIDE;
		row->effect_arg1 = (row->effect_arg1 > 0) ? row->effect_arg1 : row->effect_arg2;
		row->effect_arg2 <<= 2;		/* Internally, volume is 0..256 */
	} else if (row->effect == MOD_EFFECT_TREMOLO) {
		row->effect = TRACKER_EFFECT_TREMOLO;
	} else if (row->effect == MOD_EFFECT_SET_SAMPLE_OFFSET) {
		row->effect = TRACKER_EFFECT_CHANGE_SAMPLE_POS;
		row->effect_arg1 = ((row->effect_arg1 << 12) + (row->effect_arg2 << 8)) << 1;
	} else if (row->effect == MOD_EFFECT_VOLUME_SLIDE) {
		row->effect = TRACKER_EFFECT_VOLUME_SLIDE;
		row->effect_arg1 = (row->effect_arg1 > 0) ? row->effect_arg1 : row->effect_arg2;
		row->effect_arg1 <<= 2;		/* Internally, volume is 0..256 */
	} else if (row->effect == MOD_EFFECT_POSITION_JUMP) {
		row->effect = TRACKER_EFFECT_POSITION_JUMP;
		row->effect_arg1 = (row->effect_arg1 << 4) + row->effect_arg2;
		row->effect_arg2 = 0;
	} else if (row->effect == MOD_EFFECT_SET_VOLUME) {
		row->effect = TRACKER_EFFECT_NONE;
		row->volume = ((row->effect_arg1 << 4) + (row->effect_arg2)) << 2;	/* Volume is 0..256 */
	} else if (row->effect == MOD_EFFECT_PATTERN_BREAK) {
		row->effect = TRACKER_EFFECT_PATTERN_BREAK;
		row->effect_arg1 = (row->effect_arg1 * 10 + row->effect_arg2);
	} else if (row->effect == MOD_EFFECT_SET_SPEED) {
		row->effect = TRACKER_EFFECT_SET_SPEED;
		/* TODO: Fix argument */
	} else
		row->effect = TRACKER_EFFECT_NONE;
	
	return;
}


int modSampleAdd(TRACKER_FILE *mod, MOD_SAMPLE *sample, int slot, int i) {
	if (trackerInstrumentAdd(&mod->instrument[slot], 1) < 0) {
		trackerDestroy(mod);
		return -1;
	}
	
	if (sample[i].length == 0)
		return 0;
	if (trackerSampleAdd(mod->instrument[slot].sample, (modEndianConvert(sample[i].length) - 1) << 1, 1, sample[i].tune) < 0) {
		trackerDestroy(mod);
		return -1;
	}
	
	mod->instrument[slot].sample->repeat_start = modEndianConvert(sample[i].repeat_offset) << 1;
	mod->instrument[slot].sample->repeat_end = (modEndianConvert(sample[i].repeat_lenght) << 1) + mod->instrument[slot].sample->repeat_start;

	if ((modEndianConvert(sample[i].repeat_lenght) << 1) > 0)
		mod->instrument[slot].sample->repeat_type = TRACKER_SAMPLE_REPEAT_FORWARD;
	else
		mod->instrument[slot].sample->repeat_type = TRACKER_SAMPLE_REPEAT_NONE;
	
	return 0;
}


void modEffectSlide(void *file) {
	TRACKER_FILE *mod = file;

	/* TODO: Implement */

	return;
}


void modEffectSlideToNote(void *file) {
	TRACKER_FILE *mod = file;

	/* TODO: Implement */

	return;
}


void modEffectSlideToNoteVol(void *file) {
	TRACKER_FILE *mod = file;

	/* TODO: Implement */

	return;
}


TRACKER_FILE *modInit(void *data, size_t data_len) {
	int i, j, k, max, t;
	unsigned int period;
	char *data_r = data;
	signed char *sound_data;
	TRACKER_FILE *mod;
	MOD_PATTERN_HEADER *header = (void *) &data_r[MOD_PATTERN_INFO_POS];
	MOD_PATTERN_DATA *pattern = (void *) &data_r[MOD_PATTERN_DATA_POS];
	MOD_SAMPLE *sample = (void *) &data_r[MOD_SAMPLE_META_POS_LOW];

	if (modValidateMemory(data, data_len) < 0)
		return NULL;
	if ((mod = trackerInit()) == NULL)
		return NULL;
	
	if (trackerPatternRefs(mod, header->patterns) < 0) {
		trackerDestroy(mod);
		return NULL;
	}
	
	for (max = i = 0; i < mod->pattern_refs; i++) {
		if (header->pattern_table[i] > max)
			max = header->pattern_table[i];
		mod->pattern_ref[i] = header->pattern_table[i];
	}

	mod->repeat_jump = (header->repeat_jump == 127) ? 0 : header->repeat_jump;
	if (trackerPatternPrepare(mod, max + 1) < 0) {
		trackerDestroy(mod);
		return NULL;
	}
	
	for (i = 0; i < mod->patterns; i++) {
		if (trackerPatternAdd(&mod->pattern[i], MOD_PATTERN_LINES, MOD_PATTERN_COLS) < 0) {
			trackerDestroy(mod);
			return NULL;
		}
		
		for (j = 0; j < MOD_PATTERN_LINES; j++) {
			for (k = 0; k < MOD_PATTERN_COLS; k++) {
				period = pattern[t].sample_period_high << 4;
				period += pattern[t].sample_period_low;
				period <<= 1;
				t = MOD_PATTERN_LINES * i * MOD_PATTERN_COLS + MOD_PATTERN_COLS * j + k;
				if (period)
					mod->pattern[i].col[k].row[j].play_rate = MOD_AMIGA_FREQ / period;
				mod->pattern[i].col[k].row[j].instrument = pattern[t].sample_number_low + (pattern[t].sample_number_high << 4);
				mod->pattern[i].col[k].row[j].extra_data = period;
				mod->pattern[i].col[k].row[j].effect = pattern[t].sample_effect_high;
				mod->pattern[i].col[k].row[j].effect_arg1 = (pattern[t].sample_effect_low & 0xF0) >> 4;
				mod->pattern[i].col[k].row[j].effect_arg2 = (pattern[t].sample_effect_low & 0xF);
				mod->pattern[i].col[k].row[j].volume = -1;
			}
		}
	}

	if (trackerInstrumentPrepare(mod, MOD_MK_SAMPLES) < 0) {
		trackerDestroy(mod);
		return NULL;
	}
	
	sound_data = (void *) &pattern[t];		/* t now makes it point right after pattern data. How convinient is that? :D */
	
	for (i = 0; i < 31; i++) {
		if (modSampleAdd(mod, sample, i, i) < 0)
			return NULL;
		for (j = 0; j < mod->instrument[i].sample->samples; j++)
			mod->instrument[i].sample->sample[j] = trackerExpand8Bit(sound_data[j]);
		sound_data = &sound_data[j];
	}

	/* We need some custom effect funcs that'll differ between formats */
	mod->effect[TRACKER_EFFECT_SLIDE] = modEffectSlide;
	mod->effect[TRACKER_EFFECT_SLIDE_TO_NOTE] = modEffectSlideToNote;
	mod->effect[TRACKER_EFFECT_SLIDE_NOTE_WITH_VOLUME] = modEffectSlideToNoteVol;
	

	return mod;
}


TRACKER_FILE *modLoad(const char *fname) {
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
