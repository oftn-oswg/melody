#include <limits.h>
#include <smf.h>
#include <stdio.h>

#include "music.h"

#define SMF_NOTE_ON (0x90)
#define SMF_NOTE_OFF (0x80)
#define SMF_PROG_CHG (0xC0)
#define SMF_VELOCITY (100)

static bool music_generate_note (smf_track_t *track, music_item_tempo_t tempo, music_item_t *item, double *seconds) {
	music_item_note_t note = item->value.note;
	smf_event_t *noteon = smf_event_new_from_bytes (SMF_NOTE_ON, note.id, SMF_VELOCITY);
	smf_event_t *noteoff = smf_event_new_from_bytes (SMF_NOTE_OFF, note.id, SMF_VELOCITY);

	if (!noteon || !noteoff) {
		if (noteon) smf_event_delete (noteon);
		if (noteoff) smf_event_delete (noteoff);
		return false;
	}

	smf_track_add_event_seconds (track, noteon, *seconds);
	*seconds += (note.beats * 60.0) / (note.frac * tempo);
	smf_track_add_event_seconds (track, noteoff, *seconds);
	return true;
}

static void music_generate_rest (music_item_tempo_t tempo, double *seconds) {
	*seconds += 60.0 / tempo;
}

static bool music_generate_sub (smf_track_t *track, music_item_tempo_t *base, music_item_tempo_t tempo, music_item_t *item, double *seconds) {
	unsigned int beats = item->value.sub.beats;
	music_item_t *sequence = item->value.sub.first;
	music_item_tempo_t tempo_sub = tempo * beats;
	music_item_tempo_t tempo_new;

	while (sequence) {
		switch (sequence->type) {
		case MUSIC_ITEM_NOTE:
			if (!music_generate_note (track, tempo_sub, sequence, seconds))
				return false;
			break;

		case MUSIC_ITEM_REST:
			music_generate_rest (tempo_sub, seconds);
			break;

		case MUSIC_ITEM_TEMPO:
			tempo_new = sequence->value.tempo;
			tempo_sub = tempo_new * tempo_sub / *base;
			*base = tempo_new;
			break;

		case MUSIC_ITEM_SUB:
			if (!music_generate_sub (track, base, tempo_sub, sequence, seconds))
				return false;
			break;
		}
		sequence = sequence->next;
	}
	return true;
}

bool music_generate (const char *file, music_item_tempo_t tempo, music_item_t *sequence, int instr_id) {
	double seconds = 0.0;

	int ret;
	smf_t *smf = NULL;
	smf_track_t *track = NULL;
	smf_event_t *instrument = NULL;

	smf = smf_new ();
	if (!smf)
		return false;

	track = smf_track_new ();
	if (!track) {
		smf_track_delete (track);
		goto cleanup;
	}

	smf_add_track (smf, track);

	instrument = smf_event_new_from_bytes (SMF_PROG_CHG, instr_id, -1);
	if (instrument)
		smf_track_add_event_seconds (track, instrument, seconds);

	while (sequence) {
		switch (sequence->type) {
		case MUSIC_ITEM_NOTE:
			if (!music_generate_note (track, tempo, sequence, &seconds))
				goto cleanup;

			break;

		case MUSIC_ITEM_REST:
			music_generate_rest (tempo, &seconds);
			break;

		case MUSIC_ITEM_TEMPO:
			tempo = sequence->value.tempo;
			break;

		case MUSIC_ITEM_SUB:
			if (!music_generate_sub (track, &tempo, tempo, sequence, &seconds))
				goto cleanup;
			break;
		}

		sequence = sequence->next;
	}

	ret = smf_save (smf, file);
	if (ret)
		goto cleanup;
	
	smf_delete (smf);
	return true;

cleanup:
	smf_delete (smf);

	return false;
}
