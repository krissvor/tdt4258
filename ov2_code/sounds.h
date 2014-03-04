#ifndef _SOUNDS_H
#define _SOUNDS_H
#include <stdint.h>
#include "synth.h"

typedef struct 
{
  uint16_t note_count;
  uint16_t note_duration;
  square_note_t *sq1_notes;
  square_note_t *sq2_notes;
  triangle_note_t *tri_notes;
  noise_note_t *noise_notes;
} sound_t;

extern sound_t cannon;
extern sound_t coin;
extern sound_t k_chip_tune;
extern sound_t test;

#endif
