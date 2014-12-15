__author__ = 'kevin.nelson'

import numpy as np
import scipy as sp
import scipy.signal as sig
from math import floor

def stft(data, win_size, fft_size, overlap_fac):
    win = np.hanning(win_size)
    hop_size = int(floor(win_size * overlap_fac))

    data = np.array(data)
    pad_start = np.zeros(win_size // 2, dtype=data.dtype)
    pad_end = np.zeros(win_size, dtype=data.dtype)

    proc = np.concatenate((pad_start, data, pad_end))

    total_segments = (pad_start.shape[0]+data.shape[0]) / hop_size  # int/int returns floor of result
    result = np.empty((total_segments, fft_size), dtype=np.float32)

    inner_pad = np.zeros(fft_size*2-win_size)

    for i in range(total_segments):
        current_loc = hop_size*i
        segment = proc[current_loc:current_loc+win_size]
        windowed = segment * win
        padded = np.append(windowed, inner_pad)
        freqs = np.fft.fft(padded) / fft_size
        result[i, :] = np.abs(freqs * np.conj(freqs))[:fft_size]

    return total_segments, result


def stft_freqs(fft_size, fs):
    return np.arange(fft_size) / np.float32(fft_size*2) * fs


def stft_time(frames, t):
    return np.arange(frames) / np.float32(frames) * t


def create_ticks_optimum(axis, num_ticks, resolution):
    max_freq = axis[-1]
    hop_size = max_freq / np.float32(num_ticks)
    print 'hop size:', hop_size

    current_hop = 0

    indicies = []
    ideal_vals = []

    while current_hop < max_freq:
        next_index = np.abs(axis-current_hop).argmin()

        indicies.append(next_index)
        ideal_vals.append(current_hop)

        print 'error: ', np.abs(axis[next_index] - current_hop)

        current_hop += hop_size
        current_hop = resolution * round(float(current_hop)/resolution)



    return indicies, ideal_vals





if __name__ == '__main__':
    from scipy.io.wavfile import read as wavread
    import matplotlib.pyplot as plt

    #'''
    fs, y = wavread('New Seal and New Spring_conv.wav')
    #fs, y = wavread('equation9sec.wav')
    y = y[...,0]
    t = y.shape[0] / np.float32(fs)

    #'''

    '''
    f0 = 440
    fs = 48000
    t = 5
    n = np.arange(fs*t)
    y = 0.5*np.cos(2*np.pi*f0*n/float(fs))
    '''


    win_size = 2**11
    fft_size = win_size

    downsample = 3
    y = sig.decimate(y, downsample, ftype='fir')
    fs /= downsample

    print 'fs:', fs, 't:', t

    print 'y shape:', y.shape

    frames, result = stft(y, win_size=win_size, fft_size=fft_size, overlap_fac=0.1)
    result = 20*np.log10(result)
    result = np.clip(result, -60, 200)

    print 'result shape:', result.shape

    print 'frames:', frames

    img = plt.imshow(result, origin='lower', cmap='jet', interpolation='none', aspect='auto')
    cbar=plt.colorbar(img)
    tick_res_x = result.shape[1] / 10
    tick_res_y = result.shape[0] / 10

    freqs = stft_freqs(fft_size, fs)
    print 'num of freqs:', freqs.shape[0]
    print 'max freq:', freqs[-1]
    x_tick_locations, x_tick_vals = create_ticks_optimum(freqs, num_ticks=15, resolution=50)

    print 'tick_locations:', x_tick_locations
    print 'tick_values', x_tick_vals

    plt.xticks(x_tick_locations, x_tick_vals)

    time = stft_time(frames, t)
    print time.shape, time[-1]
    y_tick_locations, y_tick_vals = create_ticks_optimum(time, num_ticks=10, resolution=1)

    plt.yticks(y_tick_locations, y_tick_vals)

    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Time [s]')
    plt.title('Autopower spectrum of "New Seal and New Spring"')

    plt.show()



    '''
    next_pow_2 = np.power(2, np.int32(np.ceil(np.log2(y.shape[0]))))
    pad = np.zeros(next_pow_2 - y.shape[0])
    y = np.append(y, pad)

    spectrum = np.fft.fft(y)
    autopower = np.empty(spectrum.shape[0]/2, dtype=np.float32)
    autopower[:] = np.abs(spectrum * np.conj(spectrum))[:autopower.shape[0]]

    plt.subplot(1,2,1)
    spectrum_freqs = np.arange(spectrum.shape[0]) / np.float32(next_pow_2) * fs
    plt.plot(spectrum_freqs, spectrum)

    plt.subplot(1,2,2)
    autopower_freqs = np.arange(autopower.shape[0]) / np.float32(next_pow_2) * fs
    plt.plot(autopower_freqs, autopower)
    print "Highest frequency is:", np.argmax(autopower) * fs / np.float32(next_pow_2), "Hz"

    plt.show()
    '''


