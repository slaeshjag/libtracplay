#ifndef __TRACKER_H__
#define	__TRACKER_H__


#define	TRACKER_SAMPLE_REPEAT_NONE			0
#define	TRACKER_SAMPLE_REPEAT_FORWARD			1
#define	TRACKER_SAMPLE_REPEAT_PINGPONG			2


/* NOTE: "Adjust" means, in this case, a relative change to the current value */

#define	TRACKER_EFFECTS					0x19		/* One bigger than the last value */

#define	TRACKER_EFFECT_NONE				0x0
#define	TRACKER_EFFECT_ARPEGGIO				0x1
#define	TRACKER_EFFECT_SLIDE				0x2
#define	TRACKER_EFFECT_SLIDE_TO_NOTE			0x3
#define	TRACKER_EFFECT_VIBRATO				0x4
#define	TRACKER_EFFECT_SLIDE_NOTE_WITH_VOLUME		0x5
#define	TRACKER_EFFECT_VIBRATO_WITH_VOLUME_SLIDE	0x6
#define	TRACKER_EFFECT_TREMOLO				0x7
#define	TRACKER_EFFECT_CHANGE_SAMPLE_POS		0x8
#define	TRACKER_EFFECT_VOLUME_SLIDE			0x9
#define	TRACKER_EFFECT_POSITION_JUMP			0xA
#define	TRACKER_EFFECT_PATTERN_BREAK			0xB
#define	TRACKER_EFFECT_SAMPLE_FINE_ADJUST		0xC
#define	TRACKER_EFFECT_SET_GLISSANDO			0xD
#define	TRACKER_EFFECT_SET_VIBRATO_WAVEFORM		0xE
#define	TRACKER_EFFECT_FINE_SET				0xF
#define	TRACKER_EFFECT_PATTERN_LOOP			0x10
#define	TRACKER_EFFECT_SET_TREMOLO_WAVEFORM		0x11
#define	TRACKER_EFFECT_SAMPLE_RETRIGGER			0x12
#define	TRACKER_EFFECT_VOLUME_ADJUST			0x13
#define	TRACKER_EFFECT_SAMPLE_CUT			0x14
#define	TRACKER_EFFECT_DELAY_SAMPLE			0x15
#define	TRACKER_EFFECT_DELAY_PATTERN			0x16
#define	TRACKER_EFFECT_INVERT_LOOP			0x17
#define	TRACKER_EFFECT_SET_SPEED			0x18


typedef struct {
	unsigned int			play_rate;
	unsigned int			instrument;
	int				volume;
	unsigned int			extra_data;
	unsigned int			effect;
	signed int			effect_arg1;
	signed int			effect_arg2;
} TRACKER_PATTERN_ROW;

typedef struct {
	TRACKER_PATTERN_ROW		*row;
	unsigned int			rows;
} TRACKER_PATTERN_COL;

typedef struct {
	TRACKER_PATTERN_COL		*col;
	unsigned int			cols;
} TRACKER_PATTERN;


typedef struct {
	signed short			*sample;
	unsigned int			samples;
	unsigned int			sample_channels;
	unsigned int			octave;
	unsigned int			octave_tune;
	unsigned int			repeat_type;
	unsigned int			repeat_start;
	unsigned int			repeat_end;
} TRACKER_SAMPLE;


typedef struct {
	TRACKER_SAMPLE			*sample;
	unsigned int			samples;
} TRACKER_INSTRUMENT;


typedef struct {
	unsigned int			sample_ref;
	unsigned int			row;
	unsigned int			row_time;
	unsigned int			row_time_total;
} TRACKER_STATE;


typedef struct {
	TRACKER_INSTRUMENT		*instrument;
	unsigned int			instruments;
	TRACKER_PATTERN			*pattern;
	unsigned int			patterns;
	unsigned int			*pattern_ref;
	unsigned int			pattern_refs;
	TRACKER_STATE			state;
	unsigned int			repeat_jump;
	void				(*effect[TRACKER_EFFECTS])(void *file);
} TRACKER_FILE;


TRACKER_FILE *trackerInit();
signed short trackerExpand8Bit(signed char sample);


#endif
