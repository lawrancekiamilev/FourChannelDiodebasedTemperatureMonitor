from PyQt6.QtCore import QThread, pyqtSignal
import serial
import time

class TempMonitorCommunication(QThread):
    error_signal = pyqtSignal(str, str)  # Signal to emit any error messages
    names_signal = pyqtSignal(list)
    temps_signal = pyqtSignal(list, float)

    def __init__(self, port, baudrate=9600, parent=None):
        super().__init__(parent)
        self.port = port
        self.baudrate = baudrate
        self.running = True
        self.serial_connection = None

    def run(self):
        try:
            # Initialize the serial connection with a 1-second timeout
            self.serial_connection = serial.Serial(self.port, self.baudrate, timeout=1)
            if self.serial_connection.is_open:
                print(f"Connected to {self.port} at {self.baudrate} baud.")
            else:
                self.error_signal.emit("Connection Error", "Failed to open serial port.")
                self.stop()
                return
            
            temps = [0.0, 0.0, 0.0, 0.0]
            names = ['', '', '', '']

            # Send initial command to check device response
            if self.serial_connection.is_open:
                self.serial_connection.reset_input_buffer()
                self.serial_connection.reset_output_buffer()
                self.serial_connection.write(b'c')
            
            if not self.wait_for_response():
                self.error_signal.emit("Communication Error", "No response received, try connecting again.")
                self.stop()
                return
            else:
                print("Connected to CDS TempMonitor")
                StartTime = time.time()
            while self.running:
                # Only proceed if the serial connection is still open
                
                if self.serial_connection and self.serial_connection.is_open:
                    
                    self.serial_connection.reset_input_buffer()
                    self.serial_connection.reset_output_buffer()
                    self.serial_connection.write(b't')

                    # Wait for temperature data with timeout
                    temperature_data = self.get_response()
                    if not temperature_data:
                        self.error_signal.emit("No Response From TempMonitor", "Try connecting again.")
                        break
                    SampleTime = time.time()
                    # Parse temperature data
                    time.sleep(.8)
                    try:
                        channels = temperature_data.split(';')
                        for i, channel in enumerate(channels):
                            data = channel.split(':')
                            temps[i] = float(data[1])
                            names[i] = data[0]
                    except (IndexError, ValueError) as e:
                        self.error_signal.emit("Parsing Error", "Error parsing temperature data.")
                        break

                    # Emit signals with updated data
                    SampleTimeInterval = SampleTime - StartTime
                    self.names_signal.emit(names)
                    self.temps_signal.emit(temps, SampleTimeInterval)
                else:
                    break  # Exit if the serial connection is no longer open

        except Exception as e:
            self.error_signal.emit("Communication Error", f"Error in TempMonitorCommunication: {e}")
            self.reconnect()  # Attempt to reconnect if there's a failure

    def wait_for_response(self, timeout=1):
        start_time = time.time()
        while (time.time() - start_time) < timeout:
            if self.serial_connection and self.serial_connection.is_open and self.serial_connection.in_waiting > 0:
                response = self.serial_connection.readline().decode().strip()
                print(response)  # For debugging
                return response.startswith('Chan1')
        return False

    def get_response(self, timeout=5):
        start_time = time.time()
        while (time.time() - start_time) < timeout:
            if self.serial_connection and self.serial_connection.is_open and self.serial_connection.in_waiting > 0:
                return self.serial_connection.readline().decode().strip()
        return None

    def stop(self):
        self.running = False
        if self.serial_connection and self.serial_connection.is_open:
            try:
                self.serial_connection.close()
            except OSError as e:
                if e.errno == 9:  # Handle 'The handle is invalid' error
                    print("Handle invalid, closing connection.")
            except Exception as e:
                self.error_signal.emit("Communication Error", f"Failed to close port: {e}")
            finally:
                self.serial_connection = None  # Ensure serial connection is fully released
        self.wait()

    def reconnect(self):
        try:
            # Ensure previous connection is closed and released
            if self.serial_connection and self.serial_connection.is_open:
                self.serial_connection.close()
                self.serial_connection = None  # Release the handle
            time.sleep(0.5)  # Short delay to ensure the port is fully released by the OS
            
            # Reinitialize the serial connection
            self.serial_connection = serial.Serial(self.port, self.baudrate, timeout=1)
            if self.serial_connection.is_open:
                print("Reconnected successfully.")
                return True
        except Exception as e:
            self.error_signal.emit("Reconnection Failed", str(e))
            return False