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

void fix_note(note_t *note)
{
    if (note->octave == 0)
    {
        note->octave = 1;
    }
    if (note->amplitude > MAX_AMPLITUDE_PER_CHANNEL)
    {
        note->amplitude = MAX_AMPLITUDE_PER_CHANNEL;
    }
}

/* 
 * Square wave generator 1 
 */

 static note_t square1_note;

 static uint16_t square1_direction;
 static uint16_t square1_sample_idx;
 static uint16_t square1_half_period;

 void square1_play_note(note_t note)
 {
    fix_note(&note);
    if (note.half_period != square1_note.half_period || note.octave != square1_note.octave)
    {

        square1_direction = 0;
        square1_sample_idx = 0;
        square1_note = note;
        square1_half_period = note.half_period / (1 << note.octave);
    }
    else
    {
        square1_note.amplitude = note.amplitude;
    }

}

uint16_t square1_get_sample()
{
    uint16_t sample;
    sample = (square1_direction ? square1_note.amplitude : 0);

    square1_sample_idx++;
    if (square1_sample_idx >= square1_half_period)
    {
        square1_sample_idx = 0;
        square1_direction = !square1_direction;
    }

    bias -= square1_note.amplitude / 2;

    return sample;
}

/* 
 * Square wave generator 2 
 */

 static note_t square2_note;

 static uint16_t square2_direction;
 static uint16_t square2_sample_idx;
 static uint16_t square2_half_period;

 void square2_play_note(note_t note)
 {
    fix_note(&note);
    if (note.half_period != square2_note.half_period || note.octave != square2_note.octave)
    {

        square2_direction = 0;
        square2_sample_idx = 0;
        square2_note = note;
        square2_half_period = note.half_period / (1 << note.octave);
    }
    else
    {
        square2_note.amplitude = note.amplitude;
    }
}

uint16_t square2_get_sample()
{
    uint16_t sample;
    sample = (square2_direction ? square2_note.amplitude : 0);

    square2_sample_idx++;
    if (square2_sample_idx >= square2_half_period)
    {
        square2_sample_idx = 0;
        square2_direction = !square2_direction;
    }

    bias -= square2_note.amplitude / 2;

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
    fix_note(&note);
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
    fix_note(&note);
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
        square1_play_note((note_t){.half_period = coin[i], .octave = 5, .amplitude = amp});
        square2_play_note(silent_note);
        triangle_play_note(silent_note);
        noise_play(silent_note);


        for (j = 0; j < 4410; ++j)
        {
            uint16_t sample = get_sample();
            //printf("%c%c", (sample >> 8), (sample & 0xff));
            printf("%c", sample);
        }
        fprintf(stderr, "%x\n", bias);

        amp /= 1.2;
    }

    return 0;
}

#endif