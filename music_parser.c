#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "music.h"

static music_item_t * music_item_new (music_item_type_t type, ...);
static bool music_parse_unsigned_int (unsigned int *out, char **input);
static bool music_create_note (music_item_note_t *note, unsigned int octave, char letter, char modifier);
static void music_item_push (music_item_t **head, music_item_t **tail, music_item_t **item);
static music_item_t *music_item_parse (char **input, unsigned int *octave);

music_item_t * music_parse (char *input) {
	music_item_t *head = NULL;
	music_item_t *item = NULL;
	music_item_t *tail = NULL;

	unsigned int default_octave = 4;

	if (!input)
		return NULL;

	do {
		item = music_item_parse (&input, &default_octave);
		music_item_push (&head, &tail, &item);
	} while (item);

	return head;
}

void music_item_destroy (music_item_t *item) {
	while (item) {
		music_item_t *next = item->next;
		if (item->type == MUSIC_ITEM_SUB) {
			music_item_destroy (item->value.sub.first);
		}
		free (item);
		item = next;
	}
}



static music_item_t * music_item_new (music_item_type_t type, ...) {
	va_list list;
	music_item_t *item = NULL;

	va_start (list, type);

	item = malloc (sizeof *item);
	if (item) {
		item->type = type;
		item->next = NULL;

		switch (type) {
		case MUSIC_ITEM_NOTE:
			item->value.note = va_arg (list, music_item_note_t);
			break;
		case MUSIC_ITEM_REST:
			break;
		case MUSIC_ITEM_TEMPO:
			item->value.tempo = va_arg (list, music_item_tempo_t);
			break;
		case MUSIC_ITEM_SUB:
			item->value.sub.beats = 0;
			item->value.sub.first = NULL;
			break;
		}
	}

	va_end (list);

	return item;
}

static bool music_parse_unsigned_int (unsigned int *out, char **input) {
	if (!isdigit (**input))
		return false;

	*out = 0;

	do {
		*out *= 10;
		*out += (**input - '0');
		(*input)++;
	} while (isdigit (**input));

	return true;
}

static bool music_create_note (music_item_note_t *note, unsigned int octave, char letter, char modifier) {
	static unsigned short int lookup[] = { 0, 2, 4, 5, 7, 9, 11 };

	if (octave > 9 || letter < 'A' || letter > 'G')
		return false;

	note->id =
		/* Octave shift */   (octave + 1) * 12 +
		/* Note shift */     lookup[(letter >= 'C') ? (letter - 'C') : (letter + 5 - 'A')] +
		/* Mofifier shift */ ((modifier == '+') ? 1 : (modifier == '-' ? -1 : 0));

	if (note->id > 127)
		return false;
	
	return true;
}

static void music_item_push (music_item_t **head, music_item_t **tail, music_item_t **item) {
	if (!head || !tail || !item)
		return;
	
	if (*tail)
		(*tail)->next = *item;
	
	if (!(*head))
		*head = *item;
	
	*tail = *item;
}

static music_item_t *music_item_parse (char **input, unsigned int *octave) {
	music_item_t *item = NULL;
	music_item_t *subtail = NULL;
	music_item_t *subitem = NULL;

	/* For parsing MUSIC_ITEM_TEMPOs */
	unsigned int tempo;

	/* For parsing MUSIC_ITEM_NOTEs */
	char letter = '\0', modifier = '\0';
	unsigned int beats = 0, frac = 1;
	music_item_note_t note;

	/* Skip whitespace */
	while (**input && isspace (**input))
		(*input)++;

	switch (**input) {

	/* MUSIC_ITEM_TEMPO */
	case 'T': case 't':
		(*input)++;
		
		if (!music_parse_unsigned_int (&tempo, input))
			PARSE_ERROR ("Expected valid beats per minute in tempo item.");

		if (tempo == 0)
			PARSE_ERROR ("Can not have tempo of 0.");

		return music_item_new (MUSIC_ITEM_TEMPO, (music_item_tempo_t) tempo);

	/* MUSIC_ITEM_REST */
	case '.':
		(*input)++;
		return music_item_new (MUSIC_ITEM_REST);

	/* MUSIC_ITEM_SUB */
	case '[':
		(*input)++;
		item = music_item_new (MUSIC_ITEM_SUB);
		beats = 0;

		do {
			subitem = music_item_parse (input, octave);
			if (subitem) {
				switch (subitem->type) {
				case MUSIC_ITEM_TEMPO: break;
				case MUSIC_ITEM_NOTE:
					// TODO: Take fractional part of beat length into account.
					beats += subitem->value.note.beats; break;
				default:
					beats++;
				}
			}

			music_item_push (&item->value.sub.first, &subtail, &subitem);
		} while (subitem);

		item->value.sub.beats = beats;

		(*input)++;

		return item;

	/* EOF */
	case '\0': case ']': return NULL;

	/* MUSIC_ITEM_NOTE */
	default:
		music_parse_unsigned_int (octave, input);
		
		letter = toupper (**input);
		(*input)++;
		modifier = '=';
		beats = 1;
		frac = 1;

		if (**input == '-' || **input == '+') {
			modifier = **input;
			(*input)++;
		}

		if (**input == ':') {
			(*input)++;

			if (**input == '/') {
				(*input)++;
				if (!music_parse_unsigned_int (&frac, input))
					PARSE_ERROR ("Expected beat length after / marker.");
			} else if (music_parse_unsigned_int (&beats, input)) {
				if (**input == '/') {
					(*input)++;
					if (!music_parse_unsigned_int (&frac, input))
						PARSE_ERROR ("Expected beat length after / marker.");
				}
			} else {
				PARSE_ERROR ("Expected beat length after ':' marker.");
			}

			if (beats < 1 || frac < 1)
				PARSE_ERROR ("Can not have a beat length of 0.");
		}

		note.beats = beats;
		note.frac = frac;

		if (!music_create_note (&note, *octave, letter, modifier))
			PARSE_ERROR ("Could not parse music note item.");

		return music_item_new (MUSIC_ITEM_NOTE, note);
	}
}
