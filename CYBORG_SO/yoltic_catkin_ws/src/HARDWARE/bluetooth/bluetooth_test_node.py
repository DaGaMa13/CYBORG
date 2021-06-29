#!/usr/bin/env python
from machine import UART #Biblioteca para el manejo del puerto UART0:: PINES SDA: SCL:

#Función principal
while True:

	if uart.any() > 0: #Revisando conexion y envio exitoso de datos

		dato = uart.readline()
		print(dato)
		

'''******************************************************************************
*   Garces Marin Daniel         
*   [SIGNOS INGENIERÍA] [YOUNGSTERS WORKSHOP] ::
*
*   <HARDWARE> Nodo BLUETOTH_TEST 
*   El objetivo del nodo es el de gestionar la interacción con el módulo bluetoth HC 05/06
*       --  NODO ESPECFICAMENTE PARA PRUEBAS DE HARDWARE 
*       
*   Ultima version: 28 de Junio del 2021
*****************************************************************************************