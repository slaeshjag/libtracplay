#ifndef __TRACKER_H__
#define	__TRACKER_H__


#define	TRACKER_SAMPLE_REPEAT_NONE	0
#define	TRACKER_SAMPLE_REPEAT_FORWARD	1
#define	TRACKER_SAMPLE_REPEAT_PINGPONG	2


typedef struct {
	unsigned int			play_rate;
	unsigned int			instrument;
	unsigned int			effect;
	unsigned int			effect_arg1;
	unsigned int			effect_arg2;
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
	unsigned int			octave__tune;
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
} TRACKER_FILE;


TRACKER_FILE *trackerInit();
signed short trackerExpand8Bit(signed char sample);


#endif
