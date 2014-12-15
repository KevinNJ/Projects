__author__ = 'knelson'

import numpy as np
import matplotlib.pyplot as plt


def half_cosine(window, total_segments, hop_size):
    win_length = window.shape
    total_length = win_length + hop_size*(total_segments-1)
    result = np.zeros(total_length)

    for i in xrange(total_segments):
        current_loc = i*hop_size
        result[current_loc:current_loc+win_length] += window

    plt.figure()
    plt.plot(result)
    plt.ylim([0, 1.2])
    plt.show()


if __name__ == '__main__':
    win_length = 500
    overlap = 1 - 0.5
    segments = 10
    total_length = np.int32(win_length + (win_length * overlap)*(segments-1))
    hop_length = np.int32(win_length * overlap)

    print 'win length:', win_length
    print 'overlap_fac:', overlap
    print 'segments:', segments
    print 'total_length:', total_length
    print 'hop_length:', hop_length
    print ''

    win = np.hanning(win_length) * overlap * 2
    result = np.zeros(total_length)

    for i in xrange(segments):
        current_loc = i*hop_length
        print 'current location:', current_loc
        result[current_loc:current_loc+win_length] += win

    print ' '.join(map(lambda x: '%.4f'%x, result))

    plt.plot(result)
    plt.ylim([0, 1.2])
    plt.show()

