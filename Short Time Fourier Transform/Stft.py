__author__ = 'kevin.nelson'

import sys
import numpy as np
import scipy.signal as sig
import scipy.io.wavfile as wav
import matplotlib
matplotlib.use('Qt4Agg')
matplotlib.rcParams['backend.qt4']='PySide'

from PySide.QtGui import QApplication, QMainWindow

from ParseMathExp import eval_expr
from BuildQtGui import make_gui

class Stft(object):
    """Computes the short time fourier transform of a signal

    How to use:
    1.) Pass the signal into the class,
    2.) Call stft() to get the transformed data
    3.) Call freq_axis() and time_axis() to get the freq and time values for each index in the array

    """
    def __init__(self, data, fs, win_size, fft_size, overlap_fac=0.5):
        """Computes a bunch of information that will be used in all of the STFT functions"""
        self.data = np.array(data, dtype=np.float32)
        self.fs = np.int32(fs)
        self.win_size = np.int32(win_size)
        self.fft_size = np.int32(fft_size)
        self.overlap_fac = np.float32(1 - overlap_fac)

        self.hop_size = np.int32(np.floor(self.win_size * self.overlap_fac))
        self.pad_end_size = self.fft_size
        self.total_segments = np.int32(np.ceil(len(self.data) / np.float32(self.hop_size)))
        self.t_max = len(self.data) / np.float32(self.fs)

    def stft(self, scale='log', ref=1, clip=None):
        """Perform the STFT and return the result"""

        # Todo: changing the overlap factor doens't seem to preserve energy, need to fix this
        window = np.hanning(self.win_size) * self.overlap_fac * 2
        inner_pad = np.zeros((self.fft_size * 2) - self.win_size)

        proc = np.concatenate((self.data, np.zeros(self.pad_end_size)))
        result = np.empty((self.total_segments, self.fft_size), dtype=np.float32)

        for i in xrange(self.total_segments):
            current_hop = self.hop_size * i
            segment = proc[current_hop:current_hop+self.win_size]
            windowed = segment * window
            padded = np.append(windowed, inner_pad)
            spectrum = np.fft.fft(padded) / self.fft_size
            autopower = np.abs(spectrum * np.conj(spectrum))
            result[i, :] = autopower[:self.fft_size]

        if scale == 'log':
            result = self.dB(result, ref)

        if clip is not None:
            np.clip(result, clip[0], clip[1], out=result)

        return result

    def dB(self, data, ref):
        """Return the dB equivelant of the input data"""
        return 20*np.log10(data / ref)

    def freq_axis(self):
        """Returns a list of frequencies which correspond to the bins in the returned data from stft()"""
        return np.arange(self.fft_size) / np.float32(self.fft_size * 2) * self.fs

    def time_axis(self):
        """Returns a list of times which correspond to the bins in the returned data from stft()"""
        return np.arange(self.total_segments) / np.float32(self.total_segments) * self.t_max


def create_ticks_optimum(axis, num_ticks, resolution, return_errors=False):
    """ Try to divide <num_ticks> ticks evenly across the axis, keeping ticks to the nearest <resolution>"""
    max_val = axis[-1]
    hop_size = max_val / np.float32(num_ticks)

    indicies = []
    ideal_vals = []
    errors = []

    for i in range(num_ticks):
        current_hop = resolution * round(float(i*hop_size)/resolution)
        index = np.abs(axis-current_hop).argmin()

        indicies.append(index)
        ideal_vals.append(current_hop)
        errors.append(np.abs(current_hop - axis[index]))

    if return_errors:
        return indicies, ideal_vals, errors
    else:
        return indicies, ideal_vals


class StftGui(QMainWindow):
    """The gui for the STFT application"""
    def __init__(self, filename, parent=None):
        super(StftGui, self).__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.render.clicked.connect(self.on_render)
        self.ui.mpl.onResize.connect(self.redraw)
        self.init(filename)

    def init(self, filename):
        self.fs, self.data = wav.read(filename)
        if len(self.data.shape) > 1:
            # if there are multiple channels, pick the first one.
            self.data = self.data[...,0]

        self.on_render()

    def redraw(self, *args, **kwargs):
        fig = self.ui.mpl.fig
        fig.tight_layout()
        self.ui.mpl.draw()

    def on_render(self):
        # get data from GUI
        downsample_fac = int(eval_expr(self.ui.downsample_fac.text()))
        win_size = int(eval_expr(self.ui.win_size.text()))
        fft_size = int(eval_expr(self.ui.fft_size.text()))
        overlap_fac = float(eval_expr(self.ui.overlap_fac.text()))
        clip_min, clip_max = float(eval_expr(self.ui.clip_min.text())), float(eval_expr(self.ui.clip_max.text()))
        x_tick_num, x_res = int(eval_expr(self.ui.x_num_ticks.text())), float(eval_expr(self.ui.x_resolution.text()))
        x_tick_rotation = int(eval_expr(self.ui.x_tick_rotation.text()))
        y_ticks_num, y_res = int(eval_expr(self.ui.y_num_ticks.text())), float(eval_expr(self.ui.y_resolution.text()))


        if downsample_fac > 1:
            downsampled_data = sig.decimate(self.data, downsample_fac, ftype='fir')
            downsampled_fs = self.fs / downsample_fac
        else:
            downsampled_data = self.data
            downsampled_fs = self.fs

        ft = Stft(downsampled_data, downsampled_fs, win_size=win_size, fft_size=fft_size, overlap_fac=overlap_fac)
        result = ft.stft(clip=(clip_min, clip_max))

        x_ticks, x_tick_labels = create_ticks_optimum(ft.freq_axis(), num_ticks=x_tick_num, resolution=x_res)
        y_ticks, y_tick_labels = create_ticks_optimum(ft.time_axis(), num_ticks=y_ticks_num, resolution=y_res)

        fig = self.ui.mpl.fig
        fig.clear()
        ax = fig.add_subplot(111)

        img = ax.imshow(result, origin='lower', cmap='jet', interpolation='none', aspect='auto')

        ax.set_xticks(x_ticks)
        ax.set_xticklabels(x_tick_labels, rotation=x_tick_rotation)
        ax.set_yticks(y_ticks)
        ax.set_yticklabels(y_tick_labels)

        if self.ui.x_grid.isChecked():
            ax.xaxis.grid(True, linestyle='-', linewidth=1)

        if self.ui.y_grid.isChecked():
            ax.yaxis.grid(True, linestyle='-', linewidth=1)

        ax.set_xlabel('Frequency [Hz]')
        ax.set_ylabel('Time [s]')

        fig.colorbar(img)
        fig.tight_layout()

        self.ui.mpl.draw()

        self.ui.sampling_freq.setText('%d' % downsampled_fs)
        self.ui.data_length.setText('%.2f' % ft.t_max)
        self.ui.freq_res.setText('%s' % (downsampled_fs * 0.5 / np.float32(ft.fft_size)))


if __name__ == '__main__':
    import os.path as path
    make_gui('sfft_gui')
    from sfft_gui import Ui_MainWindow

    
    filename = path.join('media','mike_chirp.wav')
    #filename = 'mike_annoying.wav'
    #filename = 'New Seal and New Spring_conv.wav'

    app = QApplication(sys.argv)
    win = StftGui(filename)
    win.show()
    app.exec_()