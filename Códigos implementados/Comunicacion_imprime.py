import serial
import time

SerialEsp = serial.Serial("COM8", 9600) #conectar la esp8266 que va en el cohete.
SerialARD = serial.Serial("COM5", 9600) #conectar el arduino uno que va en el cohete.
archivo_txt = 'datos.txt' #nombre del archivo el cual se va a guardar la información recolectada de los sensores.

with open(archivo_txt, 'w') as archivo:
    while True:
        value = SerialEsp.readline()
        valDecode = value.decode('utf-8')
        print(valDecode)
        archivo.write(valDecode)  # Guardar en el archivo.
        
        SerialARD.write(valDecode.encode('utf-8'))

        if SerialARD.in_waiting > 0:
            data = SerialARD.readline().decode('utf-8')
            print(data)
            archivo.write(data)  # Guardar en el archivo