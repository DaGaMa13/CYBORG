********************************************************************
*	Garcés Marín Daniel 		
*	[SIGNOS INGENIERÍA] [YOUNGSTERS WORKSHOP] < CYBORG YOLTIC>
*
*	<HARDWARE> Nodo Sensor
*		OBJETIVO: El nodo obtendrá los valores de los sensores trasnmitidos por el arduino, y los clasificará
*					en diferentes tópicos: Luminosidad[2] / Lum_MAX[1] / Temperatura[1] / Voltaje[1]
*		-- >>> NODO ESPECFICAMENTE PARA PRUEBAS DE HARDWARE <<<<<
*		
*   Ultima versión: 24 de Junio el 2021
*********************************************************************/

// ダ・ガ・マ・jû-san
//>>>>>>>> LIBRERÍAS <<<<<<<<<<<<<
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <termio.h>
#include <sys/ioctl.h>
//----------------------------------------

	//>>>VARIABLES GLOBALES
int i=0, j=0; //varaibles para contadores
float val_Foto[2]={0,0}; //Valores de los sensores de luz
int val_lumenMax = 0; //Valor de la posición del lumen máximo
float val_Tempt=0; //Valor del sensor de temperatura
float val_Volt = 0; //Valor del voltaje de la batería de actuadores
//_________________________________________________________________
____
	//>>>FUNCIONES

void callbackArduino(const std_msgs::Float32MultiArray::ConstPtr& dataArduino){

	for(i=0;i<2;i++){ //Vaciando los datos de los sensores de luz
		val_Foto[i]=dataArduino->data[i]; 
		std::cout<<"Fotoresitor-"<<i<<":_ "<<val_Foto[i]<< std::endl;	}  //Fin del vaciado sensor luz

		val_lumenMax = dataArduino->data[2]; //Asignando valor de posición lumen
		std::cout<<"Posición Lumen = "<<val_lumenMax<< std::endl;

		val_Tempt = dataArduino->data[3]; //Asignando valor de temperatura
		std::cout<<"Temperatura Grados °C:_ "<<val_Tempt<< std::endl;

		val_Volt = dataArduino->data[4]; //Asignando valor de temperatura
		std::cout<<"Voltaje bateria :_ "<<val_Volt<< std::endl;

}//fin del callbackArduino

//_________________________________________________________________________

	//>>>FUNCIÓN MAIN PRINCIPAL
int main(int  argc, char** argv){

	//std::cout<<"     >>>>>LABORATORIO DE BIOROBÓTICA<<<<<<"<<std::endl;
	std::cout<<">_ROTOMBOTTO (SENSOR TEST NODE) en línea"<<std::endl;
	std::cout<<">_Recolectando datos...."<<std::endl;

	//_>Inicialiación del nodo de ROS); //Publicar datos enconders

	ros::init(argc,argv,"RotomBotto");
	ros:: NodeHandle n;

	//_>Obtención de los datos proporcionados por el arduino
 	ros::Subscriber subArd = n.subscribe("/hardware/arduino/data", 100, callbackArduino); //Obtener los datos de lso sensores

	//_>Variables a publicar
	std_msgs::Float32MultiArray data_luz; //Datos de los fotoresistores
	data_luz.data.resize(3);	

	std_msgs::Float32MultiArray data_tempt; //Datos de los encoders
	data_tempt.data.resize(1);

	std_msgs::Float32MultiArray data_tempt; //Datos de los encoders
	data_voltj.data.resize(1);

	//_>Tópicos que se van a publicar con la información por separado
	ros::Publisher pubFoto=n.advertise <std_msgs::Float32MultiArray> ("/hardware/sensors/luz",10); //Publicar datos fotoresistores
	ros::Publisher pubTempt=n.advertise <std_msgs::Float32MultiArray> ("/hardware/sensors/tempt",10); //Publicar datos temperatura
	ros::Publisher pubVoltj=n.advertise <std_msgs::Float32MultiArray> ("/hardware/sensors/voltj",10); //Publicar datos temperatura

	ros::Rate loop(10);
    ros::Rate r(1000);
	
	//Ciclo ROS
	while(ros::ok()){

		//Asignación de los valores para publicar
		for(i=0;i<2;i++){
		data_luz.data[i] = val_Foto[i]; } //Sensores de luz
		data_luz.data[2] = val_lumenMax;

		data_tempt.data[0]=val_Tempt; //Valor temperatura

		data_voltj.data[0]=val_Volt; //Valor temperatura

		//Publicación de los tópicos
		pubFoto.publish(data_luz);
		pubTempt.publish(data_tempt);
		pubVoltj.publish(data_voltj);
        std::cout<<"Datos de sensores publicados correctamete"<<std::endl;

		ros::spinOnce();
		loop.sleep();
        std::cout<<""<<std::endl;

	} //Fin del while(ROS)

}//Fin del MAIN 