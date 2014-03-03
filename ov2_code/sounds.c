#include "sounds.h"

static square_note_t sq_silence[] = 
  {
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0}
  };

static triangle_note_t tri_silence[] = 
  {
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0}
  };
static noise_note_t noise_silence[] = 
  {
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0},
    {.amp_begin = 0, .amp_end = 0}
  };

static square_note_t coin_notes[] = 
  {
    {.period_begin = 2*NOTE_Bb, .period_end = 2*NOTE_Bb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/1.4, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/1.4, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/1.96, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/1.96, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/2.74, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/2.74, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/3.84, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/3.84, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/5.38, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/5.38, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/7.53, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/7.53, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/10.54, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/10.54, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/14.76, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/14.76, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/20.66, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/20.66, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/28.93, .duty_cycle = 50, .duration = 2205},
    {.period_begin = NOTE_Eb, .period_end = NOTE_Eb, .octave = 6, .amp_begin = MAX_AMPLITUDE_PER_CHANNEL/28.93, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/40.5, .duty_cycle = 50, .duration = 2205}
  };

sound_t coin = 
  {
    .note_count = 11,
    .note_duration = 2205,
    .sq1_notes = coin_notes,
    .sq2_notes = sq_silence,
    .tri_notes = tri_silence,
    .noise_notes = noise_silence
  };

static noise_note_t cannon_notes[] =
  {
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL, .amp_end = MAX_AMPLITUDE_PER_CHANNEL, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/1.4, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/1.4, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/1.96, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/1.96, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/2.74, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/2.74, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/3.84, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/3.84, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/5.38, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/5.38, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/7.53, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/7.53, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/10.54, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/10.54, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/14.76, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/14.76, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/20.66, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/20.66, .multiplier = 1, .duration = 2205},
    {.amp_begin = MAX_AMPLITUDE_PER_CHANNEL/28.93, .amp_end = MAX_AMPLITUDE_PER_CHANNEL/28.93, .multiplier = 1, .duration = 2205},
  };

sound_t cannon =
  {
    .note_count = 11,
    .note_duration = 2205,
    .sq1_notes = sq_silence,
    .sq2_notes = sq_silence,
    .tri_notes = tri_silence,
    .noise_notes = cannon_notes
  };
