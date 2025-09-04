# CDS Temperature Monitor GUI V1.1

This application provides a PyQt6-based graphical interface for monitoring temperatures from four sensor channels connected via a serial interface. It allows real-time temperature graphing, device connection management, and configurable data logging.

---

## 🔧 Components

### Main GUI Files
- **Main.py** – Launches the application and initializes the GUI.
- **TempGUI.py** – Contains auto-generated GUI layout code (via `pyuic6`) for the main window.
- **TempDisplay.py** – Handles UI updates for temperature channels and status labels.
- **TempMonitorCommunication.py** – Manages serial communication with the microcontroller that sends temperature and channel name data.
- **Logging.py** – Implements a worker thread to periodically log temperature data into CSV files.
- **Chan1Graph.py**, **Chan2Grpah.py**, **Chan3Graph.py**, **Chan4Graph.py** – Handle real-time plotting of temperature vs. time for individual channels using PyQtGraph.

---

## ⚙️ Features

- **Serial Communication:**
  - Auto-detect and connect to a serial device.
  - Polls temperature and channel name data.
  - Reconnection handling and error recovery.

- **Temperature Display:**
  - Displays live readings for 4 temperature channels in Kelvin.
  - Color-coded temperature values using a red-blue gradient.
  - Shows connection status and channel labels.

- **Graphing:**
  - Open a live updating graph window for any of the four channels.
  - Shows temperature vs time with a sliding window of 100 samples.

- **Logging:**
  - Periodic CSV logging based on user-selected interval and unit.
  - Supports manual and auto logging modes.
  - Progress bar indicates time to next auto log.

---


### Requirements

```bash
pip install pyqt6 pyqtgraph pyserial
```

---

## 🧪 Example Serial Format Expected

Microcontroller should send data in this format when `b't'` is sent:

```
Chan1:298.5;Chan2:300.2;Chan3:295.6;Chan4:301.1 
```

---

## 📁 Log Format

- Logs are saved in CSV format:
```
chan1Time(sec),chan1,chan2Time(sec),chan2,chan3Time(sec),chan3,chan4Time(sec),chan4
0.0,298.5,0.0,300.2,0.0,295.6,0.0,301.1
...
```

---

## ❗ Notes

- Modify the serial port in `TempMonitorCommunication.py` when deploying to different systems.
- Graph buttons open PyQtGraph windows for individual channels.
- Ensure the microcontroller is programmed to respond to `b'c'` and `b't'` commands.

