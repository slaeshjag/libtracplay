#ifndef __MOD_H__
#define	__MOD_H__

#define				MOD_AMIGA_FREQ		7159090		/* Video clock base on Amiga */
#define				MOD_SIGNATURE		"M.K."
#define				MOD_SAMPLE_META_POS_LOW	0x14	
#define				MOD_SIGNATURE_POS	0x438
#define				MOD_PATTERN_INFO_POS	0x3B6
#define				MOD_PATTERN_LINES	64
#define				MOD_MK_SAMPLES		31


typedef struct {
	union {
		unsigned short	word;
		unsigned char	byte[2];
	}
} MOD_ENDIAN_CONVERT;


typedef struct {
	const char		name[20];
} MOD_HEADER;


typedef struct {
	const char		name[22];
	unsigned short		length;
	unsigned short		tune 			: 4;	/* Remember that > 7 is actually negative, so the
									number needs to be two-complemented to be human readable */
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
