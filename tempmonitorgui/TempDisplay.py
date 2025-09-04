from PyQt6.QtCore import QThread

def get_temperature_color(temperature):
    min_temp = 77.0   # Minimum temperature for blue
    max_temp = 300.0  # Maximum temperature for red
    clamped_temp = max(min(temperature, max_temp), min_temp)
    factor = (clamped_temp - min_temp) / (max_temp - min_temp)
    red = int(factor * 255)
    blue = int((1 - factor) * 255)
    green = 0  # No green component for this gradient
    return f"rgb({red}, {green}, {blue})"

class TemperatureWorker(QThread):
    def __init__(self, ui, parent=None):
        super().__init__(parent)
        self.ui = ui
        self.running = True

    def run(self):
        print("Worker started reading temperature data.")
        while self.running:
            # Worker runs continuously but waits for data to process
            self.sleep(1)

    def receive_data(self, temps, SampleTime):
        status_labels = [
            self.ui.arrayStatus0, 
            self.ui.array1Status, 
            self.ui.array2Status, 
            self.ui.array3Status
        ]
        temp_channels = [
            self.ui.TempChan0, 
            self.ui.TempChan1, 
            self.ui.TempChan2, 
            self.ui.TempChan3
        ]
        # Update UI based on received data
        i=0
        for temp in temps:
            if temp == 0:
                self.reset_ui(i)
            else:
                self.update_ui(status_labels[i], "Connected", "green")
                color = get_temperature_color(temp)
                self.update_ui(temp_channels[i], temp, None, float)
                self.update_ui(temp_channels[i], None, color)
            i+=1

    def stop(self):
        self.running = False
        self.wait()

    def update_ui(self, widget, text=None, color=None, value_type=None):
        if color is not None:
            widget.setStyleSheet(f"color: {color};")
        if value_type is float and text is not None:
            widget.display(float(text))

    def reset_ui(self, i):
        if i == 0:
            self.update_ui(self.ui.arrayStatus0, "UnConnected", "red")
            self.update_ui(self.ui.TempChan0, "0", "black", float)
            self.update_ui(self.ui.Chan0Name, " ")
        elif i == 1:
            self.update_ui(self.ui.array1Status, "UnConnected", "red")
            self.update_ui(self.ui.TempChan1, "0", "black", float)
            self.update_ui(self.ui.Chan1Name, " ")
        elif i == 2:
            self.update_ui(self.ui.array2Status, "UnConnected", "red")
            self.update_ui(self.ui.TempChan2, "0", "black", float)
            self.update_ui(self.ui.Chan2Name, " ")
        elif i == 3:
            self.update_ui(self.ui.array3Status, "UnConnected", "red")
            self.update_ui(self.ui.TempChan3, "0", "black", float)
            self.update_ui(self.ui.Chan3Name, " ")