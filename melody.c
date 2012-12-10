#include <stdio.h>

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
	char *filename;

	if (argc < 2) {
		fprintf (stderr, "Usage: %s filename.mid\n\n", argv[0]);
		return 1;
	}

	filename = argv[1];

	if (fgets (buffer, BUFFER_SIZE, stdin)) {
		music_item_t *music = music_parse (buffer);
		//music_item_print (music);
		//putchar ('\n');
		if (!music_generate (filename, 120, music))
			fprintf (stderr, "Could not do music generation\n");

		music_item_destroy (music);
	}

	return 0;
}
