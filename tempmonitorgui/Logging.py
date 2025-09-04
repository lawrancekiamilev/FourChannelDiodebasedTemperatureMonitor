from PyQt6 import QtCore
import csv
import time
from datetime import datetime

class LoggingWorker(QtCore.QThread):
    progress_update = QtCore.pyqtSignal(int)  # Signal to emit progress updates

    def __init__(self, path, fileName, savePeriod, savePeriodUnit, autoLog, parent=None):
        super().__init__(parent)
        self.running = True
        self.path = path
        self.fileName = fileName
        self.savePeriod = savePeriod
        self.savePeriodUnit = savePeriodUnit
        self.autoLog = autoLog
        self.TempData = {
            'chan1Time(sec)': [],
            'chan1': [],
            'chan2Time(sec)': [],
            'chan2': [],
            'chan3Time(sec)': [],
            'chan3': [],
            'chan4Time(sec)': [],
            'chan4': []
        }

    def run(self):
        SaveTime = int(self.savePeriod) * (60 if self.savePeriodUnit.startswith('M') else (3600 if self.savePeriodUnit.startswith('H') else 216000))
        start_time = time.time()

        while self.running:
            if self.autoLog:
                elapsed_time = time.time() - start_time
                progress = min((elapsed_time / SaveTime) * 100, 100)
                self.progress_update.emit(int(progress))
                if elapsed_time >= SaveTime:
                    self.logData()
                    start_time = time.time()
            else:
                continue
                
                
    def update_data(self, temps, timestamp):
        # Store temps and names in TempData based on received temps array
        self.TempData['chan1Time(sec)'].append(timestamp)
        self.TempData['chan1'].append(float(temps[0]))
        self.TempData['chan2Time(sec)'].append(timestamp)
        self.TempData['chan2'].append(float(temps[1]))
        self.TempData['chan3Time(sec)'].append(timestamp)
        self.TempData['chan3'].append(float(temps[2]))
        self.TempData['chan4Time(sec)'].append(timestamp)
        self.TempData['chan4'].append(float(temps[3]))

    def logData(self):
        current_datetime = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        file_path = f"{self.path}/{self.fileName}_{current_datetime}.csv"
        print(f"Saving file to: {file_path}")

        # Write to CSV
        with open(file_path, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(self.TempData.keys())  # Write header
            max_length = max(len(lst) for lst in self.TempData.values())
            for i in range(max_length):
                row = [self.TempData[key][i] if i < len(self.TempData[key]) else None for key in self.TempData]
                writer.writerow(row)
        print("File saved successfully.")

    def stop(self):
        self.running = False  
        self.logData() 
        self.wait()