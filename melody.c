#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "music.h"

#define BUFFER_SIZE 4096

#if 0

static void music_item_print (music_item_t *item) {
	while (item) {
		switch (item->type) {
		case MUSIC_ITEM_NOTE:
			printf ("=%u:%u ", item->value.note.id, item->value.note.beats);
			break;
		case MUSIC_ITEM_REST:
			printf (". ");
			break;
		case MUSIC_ITEM_TEMPO:
			printf ("tempo%u ", (unsigned int) item->value.tempo);
			break;
		case MUSIC_ITEM_SUB:
			printf ("[ (%u) ", item->value.sub.beats);
			music_item_print (item->value.sub.first);
			printf ("] ");
		}
		item = item->next;
	}
}

#endif

int main(int argc, char *argv[]) {
	char buffer[BUFFER_SIZE];
	char *filename = NULL;
	int instr_id = 0;

	/* Don't feel like using getopt or whatever */
	for (int i = 1; i < argc; i++) {
		if (strcmp (argv[i], "-i") == 0) {
			i++;
			instr_id = atoi (argv[i]);
		} else {
			filename = argv[i];
		}
	}

	if (!filename) {
		fprintf (stderr, "Usage: %s [-i <instrument_number>] filename.mid\n\n", argv[0]);
		return 1;
	}

	if (fgets (buffer, BUFFER_SIZE, stdin)) {
		music_item_t *music = music_parse (buffer);
		//music_item_print (music);
		//putchar ('\n');
		if (!music_generate (filename, 120, music, instr_id))
			fprintf (stderr, "Could not do music generation\n");

		music_item_destroy (music);
	}

	return 0;
}
