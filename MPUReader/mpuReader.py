import serial
import os

def setup_fifo(fifo_path):
    """Create a FIFO if it doesn't exist and open it for writing."""
    if not os.path.exists(fifo_path):
        os.mkfifo(fifo_path)
    return open(fifo_path, 'w')

def main():
    rfcomm_port = '/dev/rfcomm0'  # Update this to your RFCOMM device path
    baud_rate = 9600  # Update as per your device's configuration
    fifo_path = '/tmp/mpu_fifo'  # Path to the FIFO

    ser = None  # Initialize serial port variable
    fifo = None  # Initialize FIFO variable

    try:
        # Set up RFCOMM connection
        ser = serial.Serial(rfcomm_port, baud_rate)
        print(f"Connected to {rfcomm_port} at {baud_rate} baud.")

        # Set up FIFO
        fifo = setup_fifo(fifo_path)
        print(f"Writing to FIFO at {fifo_path}.")

        # Read from RFCOMM and write to FIFO
        while True:
            if ser.in_waiting > 0:
                data = ser.readline().decode().strip()  # Read a line and decode it
                # print(data)
                fifo.write(data + '\n')  # Write to FIFO
                fifo.flush()  # Ensure data is written to FIFO immediately

    except serial.SerialException as e:
        print(f"Serial connection error: {e}")
        main()
    except Exception as e:
        print(f"Unhandled exception: {e}")
        main()
    finally:
        if ser:
            ser.close()
        if fifo:
            fifo.close()

if __name__ == "__main__":
    main()
