/***************************************************
 *  SIGNOS INGENIERÍA YOUNGSTERS WORKSHOP
 *  
 *  PROYECTO CYBORG YOTILC ROS VERSION
 * 
 * Programa correspondiente a la integración de YOLTIIC
 * a una arquitectura más compleja utilizando un nodo 
 * de ROS para la comunicación alámbrica mendiante USB 
 * con una tarjeta Raspberry Pi.
 * 
 *  Fecha: Junio 2021
 ***************************************************/

//  <__DaGaMa13__>
//****************************************************************
/***********************
 *  >>>CONEXIONES
 *  
 * < Declaración pines de entrada >
 * A0 :: Entrada analógica del Fotoresistor Derecho
 * A1 :: Entrada analógica del Fotoresistor Izquierdo
 * A2 :: Entrada analógica del Sensor de temperatura
 * A3 :: Entrada analógica del Divisor de voltaje
 * A4 :: Entrada digital del PushButton de <modo>
 *  
 *  <Declaración pines de salida >
 * D2 :: RGB RED -- [TEMPORAL]
 * D4 :: RGB GREEN -- [TEMPORAL]
 * -D6 :: BUZZER
 * D7 :: RGB BLUE -- [TEMPORAL]
 * D13 :: LED STATUS
 */

//DECLARACIÓN DE LAS BIBLIOTECAS
#include <ros.h>
#include <std_msgs/Float32MultiArray.h> 
//________________________________________________________________________________________
//****************************************************************************************


//ASIGNACIÓN DE LOS PINES DEL ARDUINO 

// << ASIGNACIÓN DE LOS PINES DE ENTRADA >>
  
int sen_L[2] = {A0,A1}; //Sensores Fotosennsibles/Fotoresistencias :: [Der,Izq]
int sen_T= A2; //Sensor de temperatura
int sen_V = A3; //Sensor de voltaje

//int ultraSDer_T = 2; //Sensor Ultrasónico Derecho Tx
//int ultraSDer_R = 3; //Sensor Ultrasónico Derecho Rx
//int ultraSIzq_T = 4; //Sensor Ultrasónico Izquierdo Tx
//int ultraSIzq_R = 5; //Sensor Ultrasónico Izquierdo Rx

//------------------------------------------------------------

// << ASIGNACIÓN DE LOS PINES DE SALIDA >>

int ledRED = 2; //LED RGB RED
int ledGREEN = 4; //LED RGB GREEN
int ledBLUE = 7; //LED RGB BLUE

int bzzr = 6; // Salida digital Buzzer de alerta
//______________________________________________________________________

// << VARIABLES GLOBALES >>

//VARIABLES DE CONTROL Y STATUS              
int temptMax = 40; //Temperatura máxima de operación del robot en °C :: 30°C
int voltjMin = 3; //Voltaje mínimo nomical de fuente de alimentación :: 3V 
//----------------------------------------------------------------------------------------

  //Variables Topicos
float lumen[2] = {0,0};  //Almacenamiento de valores sensados de intensidad luminosa [FotoR_Der, FotoR,Iqz]
int lumen_max = 0; //Almacenamiento de la posición aproximada de la fuente luminosa
float valor_Temp = 0; //Almacenamiento del valor sensado de temperatura
float valor_Volt= 0; //Almacenamiento del valor de voltaje de la fuente de alimentacion(motores)
float dist[2] = {0,0}; //Almaceanmiento de los valores de la distancia obtenida [UltraS Der, UltraS Izq]
 
  //Almacenamiento de los datos que se enviarán a la Raspberry
float data_arduino[5]; //Temporalemente 5
  // [2]>Luminicencia + [1]>Posición lumen + [1]>Temperatura + [1]>Voltaje || + [2]>Distancias

  //Almacenamiento del topico de los LEDS
int data_RGB[3]={0,0,0};
//****************************************************************************************

 << CREACIÓN DE LOS OBJETOS TIPO ROS >>
 
std_msgs::Float32MultiArray d_robot;//Crea un arreglo de tipo multif32 de la libreria std_msgs
ros::NodeHandle n;//Crea un nodo en ros llamado n

  //Se subscribe al topico publicado en la raspbery para el uso del led RGB
ros::Subscriber<std_msgs::Float32MultiArray> ledDATA("/hardware/arduino/rgb",&dataRGB);
  
 //Publica los valores registrados por el arduino para ser leidos po un nodo el la Raspberry
ros::Publisher data_robot("/hardware/arduino/data", &d_robot);

//________________________________________________________________________________________
//****************************************************************************************

//FUNCIONES CONTROL DE HARDWARE

//Función de toma de valores de Status del robot
bool tomaStatus(){

  //Variables
  bool precaucion = false; // FALSE::OK, TRUE::ALERTA
  float temptTemp=0;

  //toma de datos
  valor_Volt = (analogRead(A3)/1023)*5.0; //Voltaje de la batería de alimentación
  temptTemp = analogRead(A2); //Tempertatura ambiental del robot
  valor_Temp = ((temptTemp / 1023.0) * 5000)/10; //Obteniedo temperatura del ambiente °C

  if(valor_Volt > voltjMin){ //Voltaje nominal adecuado

    if(valor_Temp <= temptMax){ //Temperatura ambiental adecuada
      precaucion = false; 
      digitalWrite(bzzr, LOW);
      digitalWrite(ledS,LOW);   } //Voltaje nominal y temperatura adecuada

    else{ //Temperatura ambiental peligrosa
    precaucion = true; 
    Serial.println("[ALERTA]:: TEMPERATURA ALTA!!!!!");
    ledRGB(1);
    analogWrite(bzzr, 120);
    digitalWrite(ledS,HIGH);
    delay(10);}
    
    }//Voltaje Nominal OK

  else { //ALERTA VOLTAJE NOMINAL BAJO
    precaucion = true;
    Serial.println("[ALERTA]:: NIVEL DE BATERÍA BAJO!!!!!");
    ledRGB(1);
    analogWrite(bzzr, 120);
    digitalWrite(ledS,HIGH);
    delay(10);} 

  return precaucion;  
}//Fin de la función tomaStatus
//________________________________________________________________________________________

void ledRGB(int led){

  switch(led){

              case 0://OFF
                    digitalWrite(ledRED,LOW);
                    digitalWrite(ledGREEN,LOW);
                    digitalWrite(ledBLUE,LOW);
                    break;

              case 1: //RED
                    digitalWrite(ledRED,HIGH);
                    digitalWrite(ledGREEN,LOW);
                    digitalWrite(ledBLUE,LOW);
                    break;

              case 2: //GREEN
                    digitalWrite(ledRED,LOW);
                    digitalWrite(ledGREEN,HIGH);
                    digitalWrite(ledBLUE,LOW);
                    break;

              case 3: //BLUE
                    digitalWrite(ledRED,LOW);
                    digitalWrite(ledGREEN,LOW);
                    digitalWrite(ledBLUE,HIGH);
                    break;

              case 4:  //ALL
                    digitalWrite(ledRED,HIGH);
                    digitalWrite(ledGREEN,HIGH);
                    digitalWrite(ledBLUE,HIGH);
                    break;

              default: //OFF
                    digitalWrite(ledRED,LOW);
                    digitalWrite(ledGREEN,LOW);
                    digitalWrite(ledBLUE,LOW);
                    break;
    
  }//fin del switch
  
}//FIN LED RGB
//_______________________________________________________________________

//Función de toma de intensidad luminosa
int tomaLumen(){

  //Variables
  int valorMaximo = 0; // 0::NONE, 1::FRENTE, 2::DERECHA, 3::IZQUIERDA
  int diferencia = 10; // Porcentaje de diferencia entre ambos sensores

  //Toma de datos
  lumen[0] = analogRead(A0); //Valor Fotoresistor Derecho
  lumen[1] = analogRead(A1); //Valor Fotoresistor Izquierdo

   //Comparación datos 

   if( lumen[0] == lumen[1]){ //Fuente de Luz FRENTE
    valorMaximo = 1;   }

   else if ( lumen[0] > lumen[1] ){ //Fuente de Luz DERECHA
    valorMaximo = 2;   }

   else if (lumen[0] < lumen[1] ){ //Fuente de Luz IZQUIERDA
    valorMaximo = 3;   }


   else if ( lumen[0] < 200 and lumen[1] < 200){ //Sin Fuente de Luz
    valorMaximo = 0;   }

    else{
      valorMaximo = 0;    }

   //lumen_max=valorMaximo;

 return valorMaximo;   
}// Fin de la función tomaLumen
//****************************************************************************************

  <<< FUNCIONES ROS >>>
  
 //FUNCION DEL CALLBACK
void dataRGB(const std_msgs::Float32MultiArray& msg){
   
  for(int i=0;i<3;i++){
    data_RGB[i]= msg.data[i];
  }//Fin de la toma de datos 
  
}//Fin del la funcion callback data_RGB
//________________________________________________________________________________________
//****************************************************************************************

void setup() {

  //Pines de estado 
  pinMode(bzzr,OUTPUT);
  pinMode(ledS,OUTPUT);

  //>> LED RGB
  pinMode(ledRED,OUTPUT);
  pinMode(ledGREEN,OUTPUT); 
  pinMode(ledBLUE, OUTPUT);

  //.............................................
 //>>Configuración Nodos ROS
  n.getHardware()->setBaud(500000);
  
  //Iniciar nodo en ros
  n.initNode();  
  
  n.subscribe(ledDATA); // Subscribe al topico de rgb
  
  n.advertise(data_robot); 
  d_robot.data_length = 5; //Declara tamaño del arreglo a publicar
  
}//FIN DEL SETUP/CONFIGURACIÓN DE LOS PARAMETROS INICIALES
//________________________________________________________________________________________
//****************************************************************************************

void loop() {

  // >> Obtención de los datos >>

  if( tomaStatus()){ //Revisión de los STATUS de temperatura y voltaje

   lumen_max = tomaLumen(); //Obtención de los valores de los fotoresistores
    
  }//Fin tomaStatus

  // >> Asignación al arreglo del tópico a pubicar

  data_arduino[0] = lumen[0];
  data_arduino[1] = lumen[1];
  data_arduino[2] = lumen_max;
  data_arduino[3] = valor_Temp;
  data_arduino[4] = valor_Volt;

  // >> PUBLICACIÓN DEL TÓPICO
  d_robot.data=data_arduino;//Se le asignan los datos recolectados al topico que se va a publicar

  // >> Publicación de la DATA general del arduino

    data_robot.publish(&d_robot);
    n.spinOnce();  //Publica los datos del tópico
}
