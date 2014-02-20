#include <stdint.h>
#include <stdlib.h>
#include "synth.h"

#include <stdio.h>

const note_t silent_note = 
{
    .half_period = 0,
    .octave = 1,
    .amplitude = 0
};

/* 
 * Common functions 
 */

 static uint16_t bias;

 uint16_t get_sample()
 {
    uint16_t sample = 0;
    bias = MAX_BIAS;
    sample += square1_get_sample();
    sample += square2_get_sample();
    sample += triangle_get_sample();
    sample += noise_get_sample();

    sample += bias;

    return sample;
}

uint16_t clamp(uint16_t amplitude)
{
    if (amplitude > MAX_AMPLITUDE_PER_CHANNEL)
    {
        amplitude = MAX_AMPLITUDE_PER_CHANNEL;
    }
    return amplitude;
}

/* 
 * Square wave generator 1 
 */

static uint16_t square1_period_begin;
static uint16_t square1_period_end;
static uint16_t square1_duty_cycle;
static int32_t square1_duration;
static uint16_t square1_amp_begin;
static uint16_t square1_amp_end;

static uint16_t square1_half_period;
static uint16_t square1_amplitude;
static uint16_t square1_direction;
static uint16_t square1_sample_idx;
static int32_t square1_progress;


void square1_play_note(square_note_t note)
{
    uint16_t old_period_end = square1_period_end;

    square1_period_begin = note.period_begin / (1 << note.octave);
    square1_period_end = note.period_end / (1 << note.octave);
    square1_duty_cycle = note.duty_cycle;
    square1_duration = (note.duration ? note.duration : 1);
    square1_amp_begin = clamp(note.amp_begin);
    square1_amp_end = clamp(note.amp_end);

    if (square1_period_begin != old_period_end)
    {
        square1_half_period = (square1_period_begin * square1_duty_cycle) / 100;
        square1_amplitude = square1_amp_begin;
        square1_direction = !square1_direction;
        square1_sample_idx = 0;
    }
    square1_progress = 0;
}


uint16_t square1_get_sample()
{
    uint16_t sample;
    sample = (square1_direction ? square1_amplitude : 0);
    bias -= square1_amplitude / 2;

    square1_sample_idx++;
    square1_progress++;
    if (square1_sample_idx >= square1_half_period)
    {
        square1_sample_idx = 0;
        square1_direction = !square1_direction;

        int16_t period_diff = square1_period_end - square1_period_begin;
        uint16_t new_period = square1_period_begin + (period_diff * square1_progress) / square1_duration;
        square1_half_period = (square1_direction ? 
            (new_period * square1_duty_cycle) / 100 : 
            new_period - (new_period * square1_duty_cycle) / 100);

        int16_t amp_diff = square1_amp_end - square1_amp_begin;
        square1_amplitude = square1_amp_begin + (amp_diff * square1_progress) / square1_duration;
        #ifdef DEBUG
        fprintf(stderr, "%d %d %d %d\n", 
            amp_diff, 
            amp_diff * square1_progress, 
            (amp_diff * square1_progress) / square1_duration, 
            square1_amplitude);
        #endif
    }

    return sample;
}

/* 
 * Square wave generator 2
 */

static uint16_t square2_period_begin;
static uint16_t square2_period_end;
static uint16_t square2_duty_cycle;
static int32_t square2_duration;
static uint16_t square2_amp_begin;
static uint16_t square2_amp_end;

static uint16_t square2_half_period;
static uint16_t square2_amplitude;
static uint16_t square2_direction;
static uint16_t square2_sample_idx;
static int32_t square2_progress;


void square2_play_note(square_note_t note)
{
    uint16_t old_period_end = square2_period_end;

    square2_period_begin = note.period_begin / (1 << note.octave);
    square2_period_end = note.period_end / (1 << note.octave);
    square2_duty_cycle = note.duty_cycle;
    square2_duration = (note.duration ? note.duration : 1);
    square2_amp_begin = clamp(note.amp_begin);
    square2_amp_end = clamp(note.amp_end);

    if (square2_period_begin != old_period_end)
    {
        square2_half_period = (square2_period_begin * square2_duty_cycle) / 100;
        square2_amplitude = square2_amp_begin;
        square2_direction = !square2_direction;
        square2_sample_idx = 0;
    }
    square2_progress = 0;
}

uint16_t square2_get_sample()
{
    uint16_t sample;
    sample = (square2_direction ? square2_amplitude : 0);
    bias -= square2_amplitude / 2;

    square2_sample_idx++;
    square2_progress++;
    if (square2_sample_idx >= square2_half_period)
    {
        square2_sample_idx = 0;
        square2_direction = !square2_direction;

        int16_t period_diff = square2_period_end - square2_period_begin;
        uint16_t new_period = square2_period_begin + (period_diff * square2_progress) / square2_duration;
        square2_half_period = (square2_direction ?            
            (new_period * square2_duty_cycle) / 100 : 
            new_period - (new_period * square2_duty_cycle) / 100);

        int16_t amp_diff = square2_amp_end - square2_amp_begin;
        square2_amplitude = square2_amp_begin + (amp_diff * square2_progress) / square2_duration;
    }

    return sample;
}

/*
 * Triangle wave generator
 */

 static note_t triangle_note;

 static uint16_t triangle_direction;
 static uint16_t triangle_sample_idx;
 static uint16_t triangle_half_period;

 void triangle_play_note(note_t note)
 {
    if (note.half_period != triangle_note.half_period || note.octave != triangle_note.octave)
    {

        triangle_direction = 0;
        triangle_sample_idx = 0;
        triangle_note = note;
        triangle_half_period = note.half_period / (1 << note.octave);
    }
    else
    {
        triangle_note.amplitude = note.amplitude;
    }
}

uint16_t triangle_get_sample()
{
    uint16_t sample;
    if (triangle_half_period != 0)
    {
        if (triangle_direction) /* Rising edge */
        {
            sample = (triangle_note.amplitude * (triangle_half_period - triangle_sample_idx)) / triangle_half_period;
        }
        else /* Falling edge */
        {
            sample = (triangle_note.amplitude * triangle_sample_idx) / triangle_half_period;
        }
        bias -= triangle_note.amplitude / 2;
    }
    else
    {
        sample = 0;
    }

    triangle_sample_idx++;
    if (triangle_sample_idx >= triangle_half_period)
    {
        triangle_sample_idx = 0;
        triangle_direction = !triangle_direction;
    }

    return sample;
}

/*
 * Noise Generator
 */

static uint16_t noise_amplitude;

void noise_play(note_t note)
{
    noise_amplitude = note.amplitude;
}

uint16_t noise_get_sample()
{
    uint16_t sample;

    sample = rand() & MAX_AMPLITUDE_PER_CHANNEL;
    sample = (sample * noise_amplitude) / MAX_AMPLITUDE_PER_CHANNEL;

    bias -= noise_amplitude / 2;

    return sample;
}

#ifdef TEST
#include <stdio.h>
int main(int argc, char *argv[])
{
    int i, j;
    srand(0);

    uint16_t coin[] = {2*NOTE_Bb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb};
    uint16_t amp = MAX_AMPLITUDE_PER_CHANNEL;
    for (i = 0; i < 10; i++)
    {
        square1_play_note((square_note_t){.period_begin = coin[i], .period_end = coin[i], .octave = 5, .amp_begin = amp, .amp_end = amp/1.2, .duty_cycle = 50, .duration = 4410});
        square2_play_note((square_note_t){.period_begin = coin[i], .period_end = coin[i], .octave = 5, .amp_begin = 0, .amp_end = 0, .duty_cycle = 50, .duration = 4410});
        triangle_play_note(silent_note);
        noise_play(silent_note);


        for (j = 0; j < 4410; ++j)
        {
            uint16_t sample = get_sample();
            //printf("%c%c", (sample >> 8), (sample & 0xff));
            printf("%c", sample);
        }

        amp /= 1.2;
    }

    return 0;
}

#endif