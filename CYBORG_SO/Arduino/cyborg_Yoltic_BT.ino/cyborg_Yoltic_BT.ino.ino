/***************************************************
 *  SIGNOS INGENIERÍA YOUNGSTERS WORKSHOP
 *  
 *  PROYECTO CYBORG YOTILC BLUETOOTH VERSION
 * 
 * Programa correspondiente al uso de un módulo bluetooth 
 * para el control de movimientos de YOLTIC por medio de
 * una aplicación móvil.
 * 
 *  Fecha: Marzo 2021
 ***************************************************/

//  <__DaGaMa13__>
//****************************************************************

//Declaración de las bibliotecas
#include <SoftwareSerial.h> //Librería Software seria, incluye modulo bluetooth

//Declaración del objeto Bluetooth
SoftwareSerial BT(0,1); //Pines Rx y Tx respectivamente del módulo bluetooth
/*    a         on::f  ser:g  13V:h 10V:i 9V:j 6V:k 5V:l
 *  b c d                       R:m  n     o   p    q
 *    e 
 */

//Declaración de pines de salida del Arduino

int ledRED = 2; //Salida digital LED RGB RED
int ledGREEN = 4; //Salida digital LED RGB GREEN
int ledBLUE =7; //Salida digital LED RGB BLUE

int bzzr = 6; // Salida digital Buzzer de alerta

int motorDer_0 = 3; //Salida analógica 0 de dirección del Motor Derecho
int motorDer_1 = 5; //Salida analógica 1 de dirección del Motor Derecho
int motorIzq_0 = 10; //Salida analógica 0 de dirección del Motor Izquierdo
int motorIzq_1 = 11; //Salida analógica 1 de dirección del Motor Izquierdo

int ledS = 13; //Salida digital para el LED de estatus del robot (compartir con el RGB)

// << Variables >>

int i,j,k; //contadores generales

//VARIABLES DE SENSORES
float tempt = 0; // Temperatura del robot registrada
float voltj = 0; //Voltaje de la batería de aimentación del robot
float lumen[2] = {0,0}; //Arreglo del la cantidad luminosa captada [FotoR Der, FotoR Izq]
float dist[2] = {0,0}; //Arreglo de la distancia obtenida [UltraS Der, UltraS Izq]

//VARIABLES DE CONTROL Y STATUS              
int motorVel = 70; //Velocidad de los motores, {fijada por temas de funcionalidad}             
int motorVelMax = 255; //Velocidad de los motores máxima
int temptMax = 40; //Temperatura máxima de operación del robot en °C :: 30°C
int voltjMin = 3; //Voltaje mínimo nomical de fuente de alimentación :: 3V 

//VARIABLE BLUETOOTH
char bt;

//****************************************************************************

//Función de toma de valores de Status del robot
bool tomaStatus(){

  //Variables
  bool precaucion = false; // FALSE::OK, TRUE::ALERTA
  float temptTemp=0;

  //toma de datos
  voltj = (analogRead(A3)/1023)*5.0; //Voltaje de la batería de alimentación
  temptTemp = analogRead(A2); //Tempertatura ambiental del robot
  tempt = ((temptTemp / 1023.0) * 5000)/10; //Obteniedo temperatura del ambiente °C

  if(voltj > voltjMin){ //Voltaje nominal adecuado

    if(tempt <= temptMax){ //Temperatura ambiental adecuada
      precaucion = false; 
      digitalWrite(bzzr, LOW);
      digitalWrite(ledS,LOW);   } //Voltaje nominal y temperatura adecuada

    else{ //Temperatura ambiental peligrosa
    precaucion = true; 
    digitalWrite(bzzr, HIGH);
    digitalWrite(ledS,HIGH);}
    
    }//Voltaje Nominal OK

  else { //ALERTA VOLTAJE NOMINAL BAJO
    precaucion = true;
    digitalWrite(bzzr, HIGH);
    digitalWrite(ledS,HIGH);} 

  return precaucion;  
}//Fin de la función tomaStatus
//----------------------------------------------------------

//Función de control de dirección de los motores
void controlMotor(int direccion, int velocidad){

  switch(direccion){

                    case 0: //FRENO
                           digitalWrite(motorDer_0,HIGH);
                           digitalWrite(motorDer_1,HIGH);
                           digitalWrite(motorIzq_0,HIGH);
                           digitalWrite(motorIzq_1,HIGH);
                           break;

                    case 1: //FRENTE
                           analogWrite(motorDer_0,velocidad);
                           digitalWrite(motorDer_1,LOW);
                           analogWrite(motorIzq_0,velocidad);
                           digitalWrite(motorIzq_1,LOW);
                           break;

                    case 2: //DERECHA
                           digitalWrite(motorDer_0,LOW);
                           analogWrite(motorDer_1,velocidad);
                           analogWrite(motorIzq_0,velocidad);
                           digitalWrite(motorIzq_1,LOW);
                           break;

                    case 3: //ATRAS
                           digitalWrite(motorDer_0,LOW);
                           analogWrite(motorDer_1,velocidad);
                           digitalWrite(motorIzq_0,LOW);
                           analogWrite(motorIzq_1,velocidad);
                           break;

                    case 4: //IZQUIERDA
                           analogWrite(motorDer_0,velocidad);
                           digitalWrite(motorDer_1,LOW);
                           digitalWrite(motorIzq_0,LOW);
                           analogWrite(motorIzq_1,velocidad);
                           break;

                    default: //ALTO
                           digitalWrite(motorDer_0,LOW);
                           digitalWrite(motorDer_1,LOW);
                           digitalWrite(motorIzq_0,LOW);
                           digitalWrite(motorIzq_1,LOW);
                           break;
                           
    
  }//fin del switch de direccion
}//Fin de la función {controlMotor}
//______________________________________________________________________________

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

              case 4: //OFF
                    digitalWrite(ledRED,LOW);
                    digitalWrite(ledGREEN,LOW);
                    digitalWrite(ledBLUE,LOW);
                    break;

              case 5: //ALL
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
  float error_diferencia = 0.3; // Porcentaje de diferencia entre ambos sensores 100% ~ 1023
  int diferencia = 0; //Dato que colectara la diferencia en porcentaje

  //Toma de datos
  lumen[0] = analogRead(A0); //Valor Fotoresistor Derecho
  lumen[1] = analogRead(A1); //Valor Fotoresistor Izquierdo

  diferencia = abs( (lumen[0] - lumen[1]));

   //Comparación datos 

   if( lumen[0] == lumen[1]){ //Fuente de Luz FRENTE
    valorMaximo = 1;   }

   if(diferencia <= (1023*error_diferencia)){
    valorMaximo = 1;   }

   else if ( lumen[0] > lumen[1] ){ //Fuente de Luz DERECHA
    valorMaximo = 2;   }

   else if (lumen[0] < lumen[1] ){ //Fuente de Luz IZQUIERDA
    valorMaximo = 4;   }


   else if ( lumen[0] < 200 and lumen[1] < 200){ //Sin Fuente de Luz
    valorMaximo = 0;   }

    else{
      valorMaximo = 0;    }

 return valorMaximo;   
}// Fin de la función tomaLumen

//*****************************************************************************
void setup() {
  
  BT.begin(9600); //Inicializando comunicación serial con el bluetooth
  Serial.begin(9600); //Inicializando en puerto serial

  //Pines de estado 
  pinMode(bzzr,OUTPUT);
  pinMode(ledS,OUTPUT);

  //Pines de salida LED RGB
  pinMode(ledRED,OUTPUT);
  pinMode(ledGREEN,OUTPUT);
  pinMode(ledBLUE,OUTPUT);

  //Pines de salida MOTORES
  pinMode(motorDer_0,OUTPUT);//Salida analógica 0 de dirección del Motor Derecho
  pinMode(motorDer_1,OUTPUT);//Salida analógica 1 de dirección del Motor Derecho
  pinMode(motorIzq_0,OUTPUT); //Salida analógica 0 de dirección del Motor Izquierdo
  pinMode(motorIzq_1,OUTPUT); //Salida analógica 1 de dirección del Motor Izquierdo

}
//******************************************************************************

void loop() {

  if(BT.available()){ //Obtención de data con BT disponible
    bt =BT.read();
    Serial.write(bt);
  }//Fin de obtención de data

  switch(bt){ //Movimientos del robot

              case 'g': //STOP
                       ledRGB(1);
                       controlMotor(0,motorVel);
                       break;

              case 'a': //Adelante
                       ledRGB(2);
                       controlMotor(1,motorVel);
                       break;

              case 'd': //Derecha
                       ledRGB(2);
                       controlMotor(2,motorVel);
                       break;

              case 'e': //Atras
                       ledRGB(1);
                       controlMotor(3,motorVel);
                       break;

              case 'b': //Izquierda
                       ledRGB(2);
                       controlMotor(4,motorVel);
                       break;

              case 'f': //Seguidor de luz
                      ledRGB(3);
                      controlMotor(tomaLumen(),motorVel);
                      break;

              default: //STOP
                       ledRGB(1);
                       controlMotor(0,motorVel);
                       break;
    
  }//FIN SWITCH MOVIMIENTOS

}//FIN MAIN LOOP
