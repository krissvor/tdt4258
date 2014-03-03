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

#ifndef NO_ALIAS

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

#else


static uint16_t square1_period_begin;
static uint16_t square1_period_end;
static uint16_t square1_duty_cycle;
static int32_t square1_duration;
static uint16_t square1_amp_begin;
static uint16_t square1_amp_end;

static uint16_t square1_amplitude;
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
        square1_amplitude = square1_amp_begin;
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
    }

    return sample;
}

#endif

/*
 * Triangle wave generator
 */

static uint16_t triangle_period_begin;
static uint16_t triangle_period_end;
static uint16_t triangle_octave;
static uint16_t triangle_amp_begin;
static uint16_t triangle_amp_end;
static int32_t triangle_duration;

static uint16_t triangle_half_period;
static uint16_t triangle_amplitude;
static uint16_t triangle_direction;
static uint16_t triangle_sample_idx;
static int32_t triangle_progress;

void triangle_play_note(triangle_note_t note)
{
    uint16_t old_period_end = triangle_period_end;

    triangle_octave = note.octave;
    triangle_period_begin = note.period_begin / (1 << triangle_octave);
    triangle_period_end = note.period_end / (1 << triangle_octave);
    triangle_duration = (note.duration ? note.duration : 1);
    triangle_amp_begin = clamp(note.amp_begin);
    triangle_amp_end = clamp(note.amp_end);

    if (triangle_period_begin != old_period_end)
    {
        triangle_half_period = triangle_period_begin; // HMMMMMMMMMMMM!!!!!!!!
        triangle_amplitude = triangle_amp_begin;
        triangle_direction = !triangle_direction;
        triangle_sample_idx = 0;
    }
    triangle_progress = 0;
}

uint16_t triangle_get_sample()
{
    uint16_t sample;
    if (triangle_half_period != 0)
    {
        if (triangle_direction) /* Rising edge */
        {
            sample = (triangle_amplitude * (triangle_half_period - triangle_sample_idx)) / triangle_half_period;
        }
        else /* Falling edge */
        {
            sample = (triangle_amplitude * triangle_sample_idx) / triangle_half_period;
        }
        bias -= triangle_amplitude / 2;
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

static uint32_t rand_state = 0;

uint32_t rand()
{
    rand_state = (1103515245 * rand_state + 12345) & 0xFFFFFFFF;
    return rand_state;
}

static uint16_t noise_amp_begin;
static uint16_t noise_amp_end;
static uint16_t noise_multiplier;
static uint16_t noise_sample_idx;
static uint16_t noise_sample;
static int32_t noise_duration;
static int32_t noise_progress;

void noise_play(noise_note_t note)
{
    noise_amp_begin = note.amp_begin;
    noise_amp_end = note.amp_end;
    noise_multiplier = note.multiplier;
    noise_duration = note.duration;

    noise_sample_idx = 0;
    noise_progress = 0;
}

uint16_t noise_get_sample()
{
    uint16_t noise_amplitude;

    noise_sample_idx++; 
    noise_progress++;

    if (noise_sample_idx >= noise_multiplier)
      {
	noise_amplitude = ((noise_amp_begin + noise_amp_end) * noise_progress) / noise_duration;
        noise_sample = rand() & MAX_AMPLITUDE_PER_CHANNEL;
        noise_sample = (noise_sample * noise_amplitude) / MAX_AMPLITUDE_PER_CHANNEL;
        noise_sample_idx = 0;
      }

    bias -= noise_amplitude / 2;

    return noise_sample;
}

/*
 * Write samples for DMA
 */

void put_samples(uint16_t *buf, uint16_t count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        buf[i] = get_sample();
    }
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

#ifdef TEST2
#include <stdio.h>
int main(int argc, char *argv[])
{
    int i, j;
    srand(0);

    uint16_t amp = MAX_AMPLITUDE_PER_CHANNEL;

    square1_play_note((square_note_t){.period_begin = NOTE_G, .period_end = NOTE_B, .octave = 2, .amp_begin = amp, .amp_end = amp/1.2, .duty_cycle = 50, .duration = 44100});
    square2_play_note((square_note_t){.period_begin = NOTE_G, .period_end = NOTE_B, .octave = 5, .amp_begin = 0, .amp_end = 0, .duty_cycle = 50, .duration = 44100});
    triangle_play_note(silent_note);
    noise_play(silent_note);


    for (j = 0; j < 44100; ++j)
    {
        uint16_t sample = get_sample();
        //printf("%c%c", (sample >> 8), (sample & 0xff));
        printf("%c", sample);
    }

    return 0;
}

#endif
