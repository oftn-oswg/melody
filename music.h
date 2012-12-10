#ifndef MUSIC_H
#define MUSIC_H

#include <stdbool.h>

#define PARSE_ERROR(...) { fprintf (stderr, __VA_ARGS__); exit(1); }

typedef enum music_item_type {
	MUSIC_ITEM_NOTE,
	MUSIC_ITEM_REST,
	MUSIC_ITEM_TEMPO,
	MUSIC_ITEM_SUB
} music_item_type_t;

typedef unsigned int music_item_tempo_t;

typedef struct music_item_note {
	unsigned int id;
	unsigned int frac;
	unsigned int beats;
} music_item_note_t;

typedef struct music_item_sub {
	unsigned int beats;
	struct music_item *first;
} music_item_sub_t;

typedef struct music_item {
	music_item_type_t type;
	union {
		music_item_note_t note;
		music_item_tempo_t tempo;
		music_item_sub_t sub;
	} value;
	struct music_item *next;
} music_item_t;


music_item_t * music_parse (char *input);
void music_item_destroy (music_item_t *item);

bool music_generate (const char *file, music_item_tempo_t tempo, music_item_t *sequence);


#endif
