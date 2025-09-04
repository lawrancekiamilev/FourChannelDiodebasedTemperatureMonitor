from PyQt6.QtWidgets import QWidget, QVBoxLayout
import pyqtgraph as pg

class Chan3graph(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Channel 3: Temperature Vs Time")
        self.resize(800, 500)
        
        self.x_data = []
        self.y_data = []

        layout = QVBoxLayout(self)
        self.graphWidget = pg.PlotWidget()
        self.graphWidget.setLabel('left', 'Temperature (Kelvin)')
        self.graphWidget.setLabel('bottom', 'Time (Seconds)')
        self.data_line = self.graphWidget.plot([], [], pen=pg.mkPen(color="b", width=2))

        layout.addWidget(self.graphWidget)

    def update_plot(self, temps, sampleTime):
        self.x_data.append(sampleTime)
        self.y_data.append(temps[0])
        if len(self.x_data) > 100:
            self.x_data = self.x_data[-100:]
            self.y_data = self.y_data[-100:]
        self.data_line.setData(self.x_data, self.y_data)

    def stop(self):
        self.running = False
        self.wait() 
        self.graphWidget.close() 