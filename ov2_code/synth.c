#include <stdint.h>
#include "synth.h"

const note_t silent_note = 
{
    .half_period = 0,
    .octave = 1,
    .amplitude = 0
};

static uint16_t bias;

uint16_t get_sample()
{
    uint16_t sample = 0;
    bias = 0;

    sample += square1_get_sample();
/*    sample += square2_get_sample();
    sample += triangle_get_sample();
    sample += noise_get_sample();
*/
    sample += bias;

    return sample;
}

static note_t square1_note;

static uint16_t square1_direction;
static uint16_t square1_sample_idx;
static uint16_t square1_half_period;

void fix_note(note_t *note)
{
    if (note->octave == 0)
    {
        note->octave = 1;
    }
    if (note->amplitude > 0x3FF)
    {
        note->amplitude = 0x3ff;
    }
}

void square1_play_note(note_t note)
{
    fix_note(&note);
    if (note.half_period != square1_note.half_period || note.octave != square1_note.octave)
    {
        
        square1_direction = 0;
        square1_sample_idx = 0;
        square1_note = note;
        square1_half_period = note.half_period / note.octave;
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

    return sample;
}

#ifdef TEST
#include <stdio.h>
int main(int argc, char *argv[])
{
    int i;
    square1_play_note((note_t){.half_period = 3, .octave = 1, .amplitude = 1});
    for (i = 0; i < 100; ++i)
    {
        printf("%d", square1_get_sample());
    }

    puts("");

    square1_play_note((note_t){.half_period = 3, .octave = 1, .amplitude = 5});
    for (i = 0; i < 100; ++i)
    {
        printf("%d", square1_get_sample());
    }

    puts("");
    return 0;
}

#endif