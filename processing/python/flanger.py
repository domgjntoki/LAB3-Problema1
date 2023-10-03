import signal

import numpy as np
import scipy.signal
from scipy.io import wavfile
import wave

input_sound = r'../resources/music.wav'
output_sound = r'../resources/flanger.wav'

# Read input sound
fs, x = wavfile.read(input_sound)


def flanger3(input, fs, mix=0.75, delay=5, width=5, rate=0.7):
    length = len(input)
    # interpolate in each channel
    # input = np.interp(np.arange(length), input, np.arange(0.25, length))
    input[:, 0] = np.interp(np.arange(length), input[:, 0], np.arange(0.25, length))
    input[:, 1] = np.interp(np.arange(length), input[:, 1], np.arange(0.25, length))

    fsn = fs * 4

    min_delay_sample = int(np.ceil(delay * fsn / 1000))
    max_delay_sample = int(np.ceil((delay + width) * fsn / 1000))
    n = np.arange(length) + max_delay_sample
    lfo = scipy.signal.sawtooth(2 * np.pi * rate * n / fsn, 0.5)
    delay_time_samples = ((delay + width / 2) + lfo * width / 2) * fsn / 1000
    out = np.zeros((length + min_delay_sample, 2))
    #out[:max_delay_sample] = input[:max_delay_sample]
    out[:max_delay_sample, 0] = input[:max_delay_sample, 0]
    out[:max_delay_sample, 1] = input[:max_delay_sample, 1]

    for i in range(max_delay_sample, length):
        delay_samples = int(np.ceil(delay_time_samples[i]))
        out[i] = input[i] + mix * out[i - delay_samples]

    # remover estouro no audio
    out = np.clip(out, -32768, 32767)

    return out


def flanger2(x, fs, lfo_freq=0.5, lfo_amp=0.008):
    length = len(x)
    nsample = np.array(range(length))
    lfo = 2 + scipy.signal.sawtooth(2 * np.pi * lfo_freq * nsample / fs, 0.5)

    index = np.round(nsample - fs * lfo_amp * lfo)  # Read-out index
    index[index < 0] = 0  # Clip delay
    index[index > length - 1] = length - 1  # Clip delay

    out = np.zeros((length, 2))

    out[:, 0] = x[:, 0] + np.interp(np.arange(length), index[:], x[:, 0])  # Add delayed signal
    out[:, 1] = x[:, 1] + np.interp(np.arange(length), index[:], x[:, 1])  # Add delayed signal

    # for i in range(length):
    #     out[i][0] = float(x[i][0]) + float(x[int(index[i])][0])  # Add delayed signal
    #     out[i][1] = float(x[i][1]) + float(x[int(index[i])][1])  # Add delayed signal

    # remover estouro no audio
    out = np.clip(out, -32768, 32767)

    return out


def flanger(x, fs, g=1.0, average_delay=6.0, maximum_delay_swing=1.0, rate=0.1):
    """
    Flanger effect
    :param x: input signal
    :param g: depth of the flanging effect (0 to 1)

    :return:
    """
    # Initialize output signal
    L = average_delay * (1 + maximum_delay_swing * scipy.signal.sawtooth(2 * np.pi * rate * np.arange(len(x)) / fs, 0.5))

    # do for each channel
    channels = len(x[0])
    y = np.zeros((len(x), channels))
    # y[n] = x[n] + g * x[n - L[n]]
    for i in range(channels):
        y[:, i] = x[:, i] + g * np.interp(np.arange(len(x)), np.arange(len(x)) - L, x[:, i])



    # remover estouro no audio
    y = np.clip(y, -32768, 32767)

    return y


out = flanger2(x, fs)

# saves the output sound
wavfile.write(output_sound, fs, out.astype(np.int16))

