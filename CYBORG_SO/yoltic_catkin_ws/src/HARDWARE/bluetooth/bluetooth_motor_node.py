#!/usr/bin/env python
import serial, time, sys, math #Biblioteca estándar de operaciones matemáticas
from machine import UART #Biblioteca para el manejo del puerto UART0:: PINES SDA: SCL:

#Bibliotecas para el uso de ROS
import rospy
from std_msgs.msg import Empty
from std_msgs.msg import Float32
from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import TransformStamped
from geometry_msgs.msg import Twist
#// ダ・ガ・マ・jû-san
#********************************************************************************************************

# << DECLARACIÓN FUNCIONES >>

#Despligue de las opciones de ayuda necesarios en caso de que exista algun problema
def printHelp():
    print "Ayuda/Help:"
    print "\t --GPIO \t Los pines por defecto del modulo HC 05/06 son \"SDA:13 SCL:15\""
    print ">> /hardware/bluetooth/data\t Los valores enviados por la app movil son alfabeticos"
    '''	  a         on::f  ser:g  13V:h 10V:i 9V:j 6V:k 5V:l
 	*  	b c d                       R:m  n     o   p    q
 	*     e 
 	* '''
    print "<!> En caso de que se haya quemado algún componente: ¡¿Sabes lo que cuesta ese equipo hijo?!<!>"
#--------------------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------
#*****************************************************************************************************************

# FUNCION PRINCIPAL 
def main(portName):
    print "[BLUETOOTH_TEST]:: Inicializando nodo bluetooth PRUEBA"

    ###Connection with ROS
    rospy.init_node("bluetooth_test")

    #Estableciendo parametros de ROS
    rate = rospy.Rate(1)

    #Publica los datos de la posición actual del robot
    pubBluetooth = rospy.Publisher("/hardware/bluetooth/data", String, queue_size = 1) #Publica la información obtenida del bluetooth
    pubVel = rospy.Publisher("/hardware/motors/speeds", Float32MultiArray,queue_size = 2) #Publica la información de velocidad de motores

    #Varibles de control de la velocidad
    global databluetooth
    global motor_vel[2] = {0,0}

    #Ciclo ROS
    print "[VEGA]:: Probando recepción bluetooth de ROTOMBOTTO"
    while not rospy.is_shutdown():

		if uart.any() > 0: #Revisando conexion y envio exitoso de datos

			databluetooth = uart.readline()
			print(databluetooth)

			pubBluetooth.publish(databluetooth) #Publicando el valor enviado por el bluetooth



        rate.sleep()

    #Fin del WHILE ROS
    #------------------------------------------------------------------------------------------------------

    #ALTO PREVENTIVO DEL CICLO ROS 

#FIN DEL MAIN()


#******************************************************************************************************************
#______________________________________________________________________________________________________________________

#Definicion del programa principal con el puerto ttyACM a utilzar
if __name__ == '__main__':
    try:
        if "--help" in sys.argv:
            printHelp()
        elif "-h" in sys.argv:
            printHelp()
        else:
            main()

    except rospy.ROSInterruptException:
        pass
		

'''******************************************************************************
*   Garces Marin Daniel         
*   [SIGNOS INGENIERÍA] [YOUNGSTERS WORKSHOP] ::
*
*   <HARDWARE> Nodo BLUETOTH_TEST 
*   El objetivo del nodo es el de gestionar la interacción con el módulo bluetoth HC 05/06 para control remoto
*       --  NODO ESPECFICAMENTE PARA PRUEBAS DE HARDWARE 
*       
*   Ultima version: 28 de Junio del 2021
*****************************************************************************************