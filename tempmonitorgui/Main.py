import sys
from PyQt6 import QtWidgets, QtCore
from TempGUI import Ui_MainWindow
import serial.tools.list_ports
import serial
from TempMonitorCommunication import TempMonitorCommunication
from TempDisplay import TemperatureWorker
from Logging import LoggingWorker
from Chan1Graph import Chan1graph
from Chan2Grpah import Chan2graph
from Chan3Graph import Chan3graph
from Chan4Graph import Chan4graph


class MainApp(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.temperature_worker = None
        self.logworker_worker = None
        self.comm_thread = None
        self.Graph1Worker = None
        self.Graph2Worker = None
        self.Graph3Worker = None
        self.Graph4Worker = None
        self.start_reading = QtCore.QEventLoop()
        self.setup_connections()
        self.populate_com_ports()
        self.populate_log_export_period()
        self.disable_logging()
        self.disableGraphs()
        self.ui.ExportSheet.setEnabled(False)

    def setup_connections(self):
        self.ui.Connect.clicked.connect(self.connect_button_clicked)
        self.ui.RefreshComm.clicked.connect(self.refresh_button_clicked)
        self.ui.LogPathSelect.clicked.connect(self.LogPathSelect_clicked)
        self.ui.StartLog.clicked.connect(self.logButton_clicked)
        self.ui.ExportSheet.clicked.connect(self.ForceExptortSheetClicked)
        self.ui.GraphChan0.clicked.connect(self.showChan1Grpah_clicked)
        self.ui.GraphChan1.clicked.connect(self.showChan2Grpah_clicked)
        self.ui.GraphChan2.clicked.connect(self.showChan3Grpah_clicked)
        self.ui.GraphChan3.clicked.connect(self.showChan4Grpah_clicked)
        
        
    def connect_button_clicked(self):
        self.Connect_com_port()


    def refresh_button_clicked(self):
        self.populate_com_ports()
        
    def ForceExptortSheetClicked(self):
        self.enable_logging()
        self.ui.ExportSheet.setEnabled(False)
        self.logworker_worker.stop()
        self.logworker_worker.wait()
        
    def showChan1Grpah_clicked(self):
        if self.Graph1Worker is None or not self.Graph1Worker.isVisible():
            self.Graph1Worker = Chan1graph()
            self.comm_thread.temps_signal.connect(self.Graph1Worker.update_plot)
            self.Graph1Worker.show()
        else:
            self.Graph1Worker.raise_()
            self.Graph1Worker.activateWindow()
        
    def showChan2Grpah_clicked(self):
        if self.Graph2Worker is None or not self.Graph2Worker.isVisible():
            self.Graph2Worker = Chan2graph()
            self.comm_thread.temps_signal.connect(self.Graph2Worker.update_plot)
            self.Graph2Worker.show()
        else:
            self.Graph2Worker.raise_()
            self.Graph2Worker.activateWindow()
        
    def showChan3Grpah_clicked(self):
        if self.Graph3Worker is None or not self.Graph3Worker.isVisible():
            self.Graph3Worker = Chan3graph()
            self.comm_thread.temps_signal.connect(self.Graph3Worker.update_plot)
            self.Graph3Worker.show()
        else:
            self.Graph3Worker.raise_()
            self.Graph3Worker.activateWindow()
        
    def showChan4Grpah_clicked(self):
        if self.Graph4Worker is None or not self.Graph4Worker.isVisible():
            self.Graph4Worker = Chan4graph()
            self.comm_thread.temps_signal.connect(self.Graph4Worker.update_plot)
            self.Graph4Worker.show()
        else:
            self.Graph4Worker.raise_()
            self.Graph4Worker.activateWindow()
        
    def disableGraphs(self):
        self.ui.GraphChan0.setEnabled(False)
        self.ui.GraphChan1.setEnabled(False)
        self.ui.GraphChan2.setEnabled(False)
        self.ui.GraphChan3.setEnabled(False)
    def enableGraphs(self):
        self.ui.GraphChan0.setEnabled(True)
        self.ui.GraphChan1.setEnabled(True)
        self.ui.GraphChan2.setEnabled(True)
        self.ui.GraphChan3.setEnabled(True)
    
    def logButton_clicked(self):
        self.ui.StartLog.setEnabled(False)
        folder_path = self.ui.FilePath.text()
        savePeriodUnit = self.ui.TimeUnitSelect_2.currentText()
        fileName = self.ui.logFileNameInput.text()
        savePeriod = self.ui.ExportPeriod.text()
        autoLog = not self.ui.DisableAuto.isChecked()
        self.disable_logging()
        self.ui.ExportSheet.setEnabled(True)
        
        # Initialize LoggingWorker and connect progress update signal
        self.logworker_worker = LoggingWorker(folder_path, fileName, savePeriod, savePeriodUnit, autoLog)
        self.logworker_worker.progress_update.connect(self.ui.progressBar.setValue)  # Connect signal to progress bar
        self.comm_thread.temps_signal.connect(self.logworker_worker.update_data)
        self.logworker_worker.start()

    def LogPathSelect_clicked(self):
        folder_path = QtWidgets.QFileDialog.getExistingDirectory(self, "Select Folder")
        if folder_path:
            self.ui.FilePath.setText(folder_path)

    def disable_logging(self):
        self.ui.DisableAuto.setEnabled(False)
        self.ui.LogPathSelect.setEnabled(False)
        self.ui.logFileNameInput.setEnabled(False)
        self.ui.ExportPeriod.setEnabled(False)
        self.ui.TimeUnitSelect_2.setEnabled(False)
        self.ui.StartLog.setEnabled(False)

    def enable_logging(self):
        self.ui.DisableAuto.setEnabled(True)
        self.ui.LogPathSelect.setEnabled(True)
        self.ui.logFileNameInput.setEnabled(True)
        self.ui.ExportPeriod.setEnabled(True)
        self.ui.TimeUnitSelect_2.setEnabled(True)
        self.ui.StartLog.setEnabled(True)

    def disable_commSelect(self):
        self.ui.Connect.setEnabled(False)
        self.ui.RefreshComm.setEnabled(False)
        self.ui.CommPortSelect.setEnabled(False)
        
        
    def enable_commSelect(self):
        self.ui.Connect.setEnabled(True)
        self.ui.RefreshComm.setEnabled(True)
        self.ui.CommPortSelect.setEnabled(True)
        
    def populate_com_ports(self):
        ports = list(serial.tools.list_ports.comports())
        self.ui.CommPortSelect.clear()
        for port in ports:
            self.ui.CommPortSelect.addItem(f"{port.device} - {port.description}")

    def populate_log_export_period(self):
        unit = ['Minute', 'Hour', 'Day']
        self.ui.TimeUnitSelect_2.clear()
        for units in unit:
            self.ui.TimeUnitSelect_2.addItem(units)

    def show_error_message(self, title, message):
        error_dialog = QtWidgets.QMessageBox(self)
        error_dialog.setIcon(QtWidgets.QMessageBox.Icon.Critical)
        error_dialog.setWindowTitle(title)
        error_dialog.setText(message)
        error_dialog.setStandardButtons(QtWidgets.QMessageBox.StandardButton.Ok)
        error_dialog.exec()
        self.enable_commSelect()

        
    def Connect_com_port(self):
        selected_port = self.ui.CommPortSelect.currentText().split(" - ")[0]
        if selected_port:
            try:
                # Initialize the communication thread
                self.comm_thread = TempMonitorCommunication(selected_port)
                self.comm_thread.error_signal.connect(self.show_error_message)
            
                # Start the communication thread
                self.comm_thread.start()
            
                # Enable logging and disable COM selection
                self.enable_logging()
                self.disable_commSelect()
                self.enableGraphs()
            
                # Initialize TemperatureWorker after the communication thread starts
                self.temperature_worker = TemperatureWorker(
                    ui=self.ui
                )
                self.comm_thread.temps_signal.connect(self.temperature_worker.receive_data)
                self.comm_thread.names_signal.connect(self.UpdateChanName)
                self.temperature_worker.start()
            
            except Exception as e:
                self.show_error_message('Cannot Connect', f'Cannot connect to port {selected_port}: {e}')
                self.enable_commSelect()
    def UpdateChanName(self,names):
        i = 0 
        for name in names:
            match i:
                case 0:
                    if not name:
                        self.ui.arrayStatus0.setStyleSheet('red')
                        self.ui.arrayStatus0.setText('Disconnected')
                        self.ui.Chan0Name.setText('')
                    else:
                        self.ui.arrayStatus0.setStyleSheet('green')
                        self.ui.arrayStatus0.setText('Connected')
                        self.ui.Chan0Name.setText(name)
                case 1:
                    if not name:
                        self.ui.array1Status.setStyleSheet('red')
                        self.ui.array1Status.setText('Disconnected')
                        self.ui.Chan1Name.setText('')
                    else:
                        self.ui.array1Status.setStyleSheet('green')
                        self.ui.array1Status.setText('Connected')
                        self.ui.Chan1Name.setText(name)
                case 2:
                    if not name:
                        self.ui.array2Status.setStyleSheet('red')
                        self.ui.array2Status.setText('Disconnected')
                        self.ui.Chan2Name.setText('')
                    else:
                        self.ui.array2Status.setStyleSheet('green')
                        self.ui.array2Status.setText('Connected')
                        self.ui.Chan2Name.setText(name)
                case 3:
                    if not name:
                        self.ui.array3Status.setStyleSheet('red')
                        self.ui.array3Status.setText('Disconnected')
                        self.ui.Chan3Name.setText('')
                    else:
                        self.ui.array3Status.setStyleSheet('green')
                        self.ui.array3Status.setText('Connected')
                        self.ui.Chan3Name.setText(name)
            i+=1
                        
                        
    def closeEvent(self, event):
        if self.comm_thread is not None:
            self.comm_thread.stop()
            self.comm_thread.wait() 
        if self.temperature_worker is not None:
            self.temperature_worker.stop()
            self.temperature_worker.wait() 
        if self.logworker_worker is not None:
            self.logworker_worker.stop()
            self.logworker_worker.wait()
        if self.Graph1Worker is not None:
            self.Graph1Worker.stop()
        if self.Graph2Worker is not None:
            self.Graph2Worker.stop()
        if self.Graph3Worker is not None:
            self.Graph3Worker.stop()
        if self.Graph4Worker is not None:
            self.Graph4Worker.stop()
        event.accept()

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    main_app = MainApp()
    main_app.show()
    sys.exit(app.exec())