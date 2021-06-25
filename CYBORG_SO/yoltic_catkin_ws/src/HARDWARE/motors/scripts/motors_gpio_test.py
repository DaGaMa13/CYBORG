#!/usr/bin/env python
import serial, time, sys, math #Biblioteca estándar de operaciones matemáticas

import RPi.GPIO as GPIO #Biblioteca para el uso de los pines GPIO

#Bibliotecas para el uso de ROS
import rospy
from std_msgs.msg import Empty
from std_msgs.msg import Float32
from std_msgs.msg import Float32MultiArray
from nav_msgs.msg import Odometry
from geometry_msgs.msg import TransformStamped
#// ダ・ガ・マ・jû-san
from geometry_msgs.msg import Twist

#*****************************************************************************

#Declaración de los pines de salida y control de motores

GPIO.setmode(GPIO.BCM)

GPIO.setup(13, GPIO.OUT) #Motor Derecho 1
GPIO.setup(15, GPIO.OUT) #Motor Derecho 2
GPIO.setup(16, GPIO.OUT) #Motor Izquierdo 1
GPIO.setup(18, GPIO.OUT) #Motor Izquiero 2

motorDer1 = GPIO.PWM(13,100) #Motor Derecho 1
motorDer2 = GPIO.PWM(15,100) #Motor Derecho 2
motorIzq1 = GPIO.PWM(16,100) #Motor Izquierdo 1
motorIzq1 = GPIO.PWM(18,100) #Motor Izquierdo 2
#********************************************************************************************************

# << DECLARACIÓN FUNCIONES >>

#Despligue de las opciones de ayuda necesarios en caso de que exista algun problema
def printHelp():
    print "Ayuda/Help:"
    print "\t --GPIO \t Los pines por defecto del puente H para el uso de motores son \"MD1:13 MD2:15 MI1:16 MI2:18\""
    print ">> /hardware/motors/speeds\t Los valores usados en el topico deben encontrarse en el rango [-1, 1], 1 max velocidad del motor"
    print "<!> En caso de que se haya quemado algún componente: ¡¿Sabes lo que cuesta ese equipo hijo?!<!>"
#--------------------------------------------------------------------------------------------------------------

#Obtencion de las velociaddes por medio del topico /hardware/motors/speeds :: [Vel_Der , Vel Izq] >>> [M1, M2]
def callbackSpeeds(msg):
    
    #Las velocidades recibidas deben ser float en el rango [-1,1], respectivamente los valores de [0:127] traducidos a [0:1]
    #Siendo el valor 1 la maxima potencia del motor en la direccion indicada por medio de los signos
    # (+)>Adelante, (-)>Retroceso

    global leftSpeed
    global rightSpeed
    global newSpeedData

    #Obteniedo datos del topico 
    rightSpeed = float(msg.data[0])
    leftSpeed= float(msg.data[1])
    
    print "[MOTOR_TEST|>>>Velocidades::  velDer:_"+str(float(rightSpeed))" ; VelIzq:_"+str(float(leftSpeed))+" <<"

    #Revision de los limites de veocidad obtenidas
    if leftSpeed > 1:
        leftSpeed = 1
    elif leftSpeed < -1:
        leftSpeed = -1
    if rightSpeed > 1:
        rightSpeed = 1
    elif rightSpeed < -1:
        rightSpeed = -1

    newSpeedData = True #Se activa bandera de que nuevos datos fueron obtenidos
#-----------------------------------------------------------------------------------------------------------------
#*****************************************************************************************************************

# FUNCION PRINCIPAL 
def main(portName):
    print "[MOTOR_TEST]:: Inicializando motores en modo de PRUEBA"

    ###Connection with ROS
    rospy.init_node("motor_test")

    #Suscripcion a Topicos
    subSpeeds = rospy.Subscriber("/hardware/motors/speeds", Float32MultiArray, callbackSpeeds)  #Topico para obtener vel y dir de los motores

    #Estableciendo parametros de ROS
    rate = rospy.Rate(1)

    #Varibles de control de la velocidad
    global leftSpeed
    global rightSpeed
    global newSpeedData

    leftSpeed = 0 #[-1:0:1]
    rightSpeed = 0 #[-1:0:1]
    newSpeedData = False #Al inciar no existe nuevos datos de movmiento
    speedCounter = 5

    #Ciclo ROS
    print "[VEGA]:: Probando los motores de ROTOMBOTTO"
    while not rospy.is_shutdown():

        if newSpeedData: #Se obtuvieron nuevos datos del topico /hardware/motors/speeds

            newSpeedData = False
            speedCounter = 5

            #Indicando la informacion de velocidades a la Roboclaw

            #Realizando trasnformacion de la informacion
            leftSpeed = int(leftSpeed*100)
            rightSpeed = int(rightSpeed*100)

            #Asignando las direcciones del motor derecho

            if rightSpeed >= 0: #AVANZAR MOTOR DERECHO
            	motorDer1.ChangeDutyCicle(rightSpeed) #Motor Derecho 1
				motorDer2.ChangeDutyCicle(0) #Motor Derecho 2

            else: #RETROCEDER MOTOR DERECHO
            	motorDer1.ChangeDutyCicle(0) #Motor Derecho 1
				motorDer2.ChangeDutyCicle(-rightSpeed) #Motor Derecho 2

            #Asignando las direcciones del motor izquierdo
            if leftSpeed >= 0:
				motorIzq1.ChangeDutyCicle(rightSpeed) #Motor Izquierdo 1
				motorIzq1.ChangeDutyCicle(0) #Motor Izquierdo 2
            else:
                RC.BackwardM2(address, -leftSpeed)

        else: #NO se obtuvieron nuevos datos del topico, los motores se detienen

  			  print "[VEGA]:: <!!!!> [PRECAUCIÓN] NO SE RECIBEN DATOS DE VELOCIDAD"

            speedCounter -= 1 

            if speedCounter == 0:
            	motorDer1.ChangeDutyCicle(0) #Motor Derecho 1
				motorDer2.ChangeDutyCicle(0) #Motor Derecho 2
				motorIzq1.ChangeDutyCicle(0) #Motor Izquierdo 1
				motorIzq1.ChangeDutyCicle(0) #Motor Izquierdo 2
 
            if speedCounter < -1:
                speedCounter = -1

        rate.sleep()

    #Fin del WHILE ROS
    #------------------------------------------------------------------------------------------------------

    #ALTO PREVENTIVO DEL CICLO ROS 
	motorDer1.ChangeDutyCicle(0) #Motor Derecho 1
	motorDer2.ChangeDutyCicle(0) #Motor Derecho 2
	motorIzq1.ChangeDutyCicle(0) #Motor Izquierdo 1
	motorIzq1.ChangeDutyCicle(0) #Motor Izquierdo 2

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
*   <HARDWARE> Nodo MOTOR_TEST motor
*   El objetivo del nodo es el de gestionar el movimiento  de los motores por medio de los pines GPIO 
*       --  NODO ESPECFICAMENTE PARA PRUEBAS DE HARDWARE 
*       
*   Ultima version: 24 de Junio del 2021
*