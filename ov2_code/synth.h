#ifndef _SYNTH_H
#define _SYNTH_H 
#include <stdint.h>
#include <stdlib.h>

/* Half-periods for each semitone in 1st octave */
/* C1 approximately 32.7 Hz */
#define NOTE_C  674
#define NOTE_Db 633
#define NOTE_D  601
#define NOTE_Eb 567
#define NOTE_E  535
#define NOTE_F  505
#define NOTE_Gb 477
#define NOTE_G  450
#define NOTE_Ab 425
#define NOTE_A  401
#define NOTE_Bb 378
#define NOTE_B  357

/* Some commonly seen amplitudes */
#ifndef TEST
#define MAX_AMPLITUDE 0xFFF
#define MAX_AMPLITUDE_PER_CHANNEL 0x3FF
#define MAX_BIAS 0x7FF
#else
#define MAX_AMPLITUDE 0xFF
#define MAX_AMPLITUDE_PER_CHANNEL 0x3F
#define MAX_BIAS 0x7F
#endif

/* Period is given in the first octave. 
   Amplitudes greater than 0x3FF will be clamped. 
   Duty cycle is given in percent. */
typedef struct 
{
    uint16_t period_begin;
    uint16_t octave;
    uint16_t amp_begin;

    uint16_t period_end;
    uint16_t amp_end;
    uint16_t duty_cycle;
    int32_t duration;
} square_note_t;

typedef struct
{
    uint16_t period_begin;
    uint16_t octave;
    uint16_t amp_begin;

    uint16_t period_end;
    uint16_t amp_end;
    int32_t duration;
} triangle_note_t;

typedef struct
{
    uint16_t amp_begin;
    uint16_t amp_end;
    uint16_t multiplier;
} noise_note_t;

// TODO: Legacy
typedef struct 
{
    uint16_t half_period;
    uint16_t octave;
    uint16_t amplitude;
} note_t;

void square1_play_note(square_note_t note);
void square2_play_note(square_note_t note);
void triangle_play_note(note_t note);
void noise_play(note_t note);

uint16_t get_sample();
void put_samples(uint16_t *buf, uint16_t count);

/* "private" functions */
uint16_t square1_get_sample();
uint16_t square2_get_sample();
uint16_t triangle_get_sample();
uint16_t noise_get_sample();


#endif