#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "music.h"

static music_item_t * music_item_new (music_item_type_t type, ...);
static bool music_parse_unsigned_int (unsigned int *out, char **input);
static bool music_parse_signed_int (int *out, char **input);
static bool music_create_note (music_item_note_t *note, int octave, char letter, char modifier, unsigned int beats);
static void music_item_push (music_item_t **head, music_item_t **tail, music_item_t **item);
static music_item_t *music_item_parse (char **input);

music_item_t * music_parse (char *input) {
	music_item_t *head = NULL;
	music_item_t *item = NULL;
	music_item_t *tail = NULL;

	if (!input)
		return NULL;

	do {
		item = music_item_parse (&input);
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
	*out = 0;

	if (!isdigit (**input))
		return false;
	
	do {
		*out *= 10;
		*out += (**input - '0');
		(*input)++;
	} while (isdigit (**input));

	return true;
}

static bool music_parse_signed_int (int *out, char **input) {
	bool success = false;
	unsigned int part = 0;

	if (**input == '-') {
		*out = -1;
		(*input)++;
	} else {
		*out = 1;
	}

	success = music_parse_unsigned_int (&part, input);
	*out = *out * part;

	return success;
}

static bool music_create_note (music_item_note_t *note, int octave, char letter, char modifier, unsigned int beats) {
	static unsigned short int lookup[] = { 0, 2, 4, 5, 7, 9, 11 };

	if (octave < -5 || octave > 5 || letter < 'A' || letter > 'G')
		return false;

	note->id =
		/* Octave shift */   (octave + 5) * 12 +
		/* Note shift */     lookup[(letter >= 'C') ? (letter - 'C') : (letter + 5 - 'A')] +
		/* Mofifier shift */ ((modifier == '+') ? 1 : (modifier == '-' ? -1 : 0));
	note->beats = beats;
	
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

static music_item_t *music_item_parse (char **input) {
	music_item_t *item = NULL;
	music_item_t *subtail = NULL;
	music_item_t *subitem = NULL;

	/* For parsing MUSIC_ITEM_TEMPOs */
	unsigned int tempo;

	/* For parsing MUSIC_ITEM_NOTEs */
	int octave = 0;
	char letter = '\0', modifier = '\0';
	unsigned int beats = 0;
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
			subitem = music_item_parse (input);
			if (subitem) {
				switch (subitem->type) {
				case MUSIC_ITEM_TEMPO: break;
				case MUSIC_ITEM_NOTE: beats += subitem->value.note.beats; break;
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
		if (music_parse_signed_int (&octave, input)) {
			letter = toupper (**input);
			(*input)++;
			modifier = '=';
			beats = 1;

			if (**input == '-' || **input == '+') {
				modifier = **input;
				(*input)++;
			}

			if (**input == ':') {
				(*input)++;
				if (!music_parse_unsigned_int (&beats, input))
					PARSE_ERROR ("Expected number of beats after ':' marker.");

				if (beats < 1)
					PARSE_ERROR ("Can not have a beat length of 0.");
			}

			if (!music_create_note (&note, octave, letter, modifier, beats))
				PARSE_ERROR ("Could not parse music note item.");

			return music_item_new (MUSIC_ITEM_NOTE, note);
		} else {
			PARSE_ERROR ("Unexpected character in stream: '%c'", **input);
		}
	}
}
