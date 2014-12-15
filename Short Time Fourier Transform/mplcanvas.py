__author__ = 'kevin.nelson'


from PySide import QtGui, QtCore
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


class MplCanvas(FigureCanvas):
    onResize = QtCore.Signal(QtCore.QSize)
    def __init__(self, parent=None):
        self.fig = Figure()
        super(MplCanvas, self).__init__(self.fig)
        self.setParent(parent)

    def resizeEvent(self, event):
        super(MplCanvas, self).resizeEvent(event)
        self.onResize.emit(event.size())

