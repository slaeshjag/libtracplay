#ifndef __MOD_H__
#define	__MOD_H__

#define				MOD_AMIGA_FREQ			7159090		/* Video clock base on Amiga */
#define				MOD_SIGNATURE			"M.K."
#define				MOD_SIGNATURE2			"4CHN"
#define				MOD_SAMPLE_META_POS_LOW		0x14
#define				MOD_SIGNATURE_POS		0x438
#define				MOD_PATTERN_INFO_POS		0x3B6
#define				MOD_PATTERN_DATA_POS		0x43C
#define				MOD_PATTERN_LINES		64
#define				MOD_PATTERN_COLS		4
#define				MOD_MK_SAMPLES			31


#define				MOD_EFFECT_ARPEGGIO			0x0
#define				MOD_EFFECT_SLIDE_UP			0x1
#define				MOD_EFFECT_SLIDE_DOWN			0x2
#define				MOD_EFFECT_SLIDE_TO_NOTE		0x3
#define				MOD_EFFECT_VIBRATO			0x4
#define				MOD_EFFECT_SLIDE_TO_NOTE_WITH_VOL	0x5
#define				MOD_EFFECT_VIBRATO_WITH_VOL_SLIDE	0x6
#define				MOD_EFFECT_TREMOLO			0x7
#define				MOD_EFFECT_SET_SAMPLE_OFFSET		0x9
#define				MOD_EFFECT_VOLUME_SLIDE			0xA
#define				MOD_EFFECT_POSITION_JUMP		0xB
#define				MOD_EFFECT_SET_VOLUME			0xC
#define				MOD_EFFECT_PATTERN_BREAK		0xD
#define				MOD_EFFECT_EXTENDED			0xE
#define				MOD_EFFECT_SET_SPEED			0xF


#define				MOD_EFFECT_EXTENDED_FINESLIDE_UP	0x1
#define				MOD_EFFECT_EXTENDED_FINESLIDE_DOWN	0x2
#define				MOD_EFFECT_EXTENDED_GLISSANDO		0x3
#define				MOD_EFFECT_EXTENDED_VIBRATO_WAVEFORM	0x4
#define				MOD_EFFECT_EXTENDED_FINETUNE_VALUE	0x5
#define				MOD_EFFECT_EXTENDED_LOOP_PATTERN	0x6
#define				MOD_EFFECT_EXTENDED_TREMOLO_WAVEFORM	0x7
#define				MOD_EFFECT_EXTENDED_RETRIGGER_SAMPLE	0x9
#define				MOD_EFFECT_EXTENDED_FINE_VOLUME_UP	0xA
#define				MOD_EFFECT_EXTENDED_FINE_VOLUME_DOWN	0xB
#define				MOD_EFFECT_EXTENDED_CUT_SAMPLE		0xC
#define				MOD_EFFECT_EXTENDED_DELAY_SAMPLE	0xD
#define				MOD_EFFECT_EXTENDED_DELAY_PATTERN	0xE
#define				MOD_EFFECT_EXTENDED_INVERT_LOOP		0xF


typedef struct {
	union {
		unsigned short	word;
		unsigned char	byte[2];
	};
} MOD_ENDIAN_CONVERT;


typedef struct {
	const char		name[20];
} MOD_HEADER;


typedef struct {
	const char		name[22];
	unsigned short		length;
	signed short		tune 			: 4;
	signed short		dummy 			: 4;
	unsigned char		volume;
	unsigned short		repeat_offset;
	unsigned short		repeat_lenght;
} __attribute__((packed)) MOD_SAMPLE;


typedef struct {
	unsigned char		patterns;
	unsigned char		repeat_jump;
	const unsigned char	pattern_table[128];
	const unsigned char	format_signature[4];
} __attribute__((packed)) MOD_PATTERN_HEADER;


typedef struct {
	unsigned int		sample_period_low	: 4;
	unsigned int		sample_number_high	: 4;
	unsigned int		sample_period_high	: 8;
	unsigned int		sample_effect_high	: 4;
	unsigned int		sample_number_low	: 4;
	unsigned int		sample_effect_low	: 8;
} __attribute__((packed)) MOD_PATTERN_DATA;



int modValidate(const char *fname);
int modValidateMemory(void *data, size_t data_len);


#endif
