#!/usr/bin/env python
from machine import UART #Biblioteca para el manejo del puerto UART0:: PINES SDA: SCL:

#Función principal
while True:

	if uart.any() > 0: #Revisando conexion y envio exitoso de datos

		dato = uart.readline()
		print(dato)
		

