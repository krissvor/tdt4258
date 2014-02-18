#ifndef _SYNTH_H
#define _SYNTH_H 
#include <stdint.h>

/* Half-periods for each semitone in 1st octave */
/* C1 approximately 32.7 Hz */
#define NOTE_C  673
#define NOTE_Db 635
#define NOTE_D  599
#define NOTE_Eb 566
#define NOTE_E  534
#define NOTE_F  504
#define NOTE_Gb 476
#define NOTE_G  449
#define NOTE_Ab 424
#define NOTE_A  400
#define NOTE_Bb 378
#define NOTE_B  336

/* Half-period is given in the first octave. 
   Amplitudes greater than 0x3FF will be clamped. */
typedef struct 
{
    uint16_t half_period;
    uint16_t octave;
    uint16_t amplitude;
} note_t;

extern const note_t silent_note;

void square1_play_note(note_t note);
void square2_play_note(note_t note);
void triangle_play_note(note_t note);
void noise_play(note_t note); /* Disregards period and octave */

uint16_t get_sample();
void put_samples(uint16_t *buf);

/* "private" functions */
uint16_t square1_get_sample();
uint16_t square2_get_sample();
uint16_t triangle_get_sample();
uint16_t noise_get_sample();

void fix_note(note_t *note);

#endif