/***************************************************
 *  SIGNOS INGENIERÍA YOUNGSTERS WORKSHOP
 *  
 *  PROYECTO CYBORG YOTILC TEST VERSION
 * 
 * Programa correspondiente a la prueba de los componentes
 * integrados en el robot Cyborg Yoltic siendo administradso
 * únicamente por la unidad de procesamiento Arduino.
 * 
 *  Fecha: Marzo 2021
 ***************************************************/

//  <__DaGaMa13__>
//****************************************************************

// << Bibliotecas >>
//---------------------------------------------------------------

// << Declaración de los pines de entrada >>
//A0 :: Entrada analógica del Fotoresistor Derecho
//A1 :: Entrada analógica del Fotoresistor Izquierdo
//A2 :: Entrada analógica del Sensor de temperatura
//A3 :: Entrada analógica del Divisor de voltaje
//A4 :: Entrada digital del PushButton de <modo>

// << Declaración de lo spines de salida >>

int ledRed = 0; //LED RGB RED
int ledBlue = 1; //LED RGB BLUE


int ultraSDer_T = 2; //Sensor Ultrasónico Derecho Tx
int ultraSDer_R = 3; //Sensor Ultrasónico Derecho Rx
int ultraSIzq_T = 4; //Sensor Ultrasónico Izquierdo Tx
int ultraSIzq_R = 5; //Sensor Ultrasónico Izquierdo Rx

int bzzr = 6; // Salida digital Buzzer de alerta

int motorDer_0 = 7; //Salida digital 0 de dirección del Motor Derecho
int motorDer_1 = 8; //Salida digital 1 de dirección del Motor Derecho
int motorIzq_0 = 9; //Salida digital 0 de dirección del Motor Izquierdo
int motorIzq_1 = 10; //Salida digital 1 de dirección del Motor Izquierdo

int ledS = 13; //Salida digital para el LED de estatus del robot (compartir con el RGB)
//-----------------------------------------------------------------------------------------

// << Variables >>

int i,j,k; //contadores generales

float tempt = 0; // Temperatura del robot registrada
float voltj = 0; //Voltaje de la batería de aimentación del robot
float lumen[2] = {0,0}; //Arreglo del la cantidad luminosa captada [FotoR Der, FotoR Izq]
float dist[2] = {0,0}; //Arreglo de la distancia obtenida [UltraS Der, UltraS Izq]

int modo = 0; //Modo en el que se encuentra el robot habilidado (PushButton)
              // 0::ALLTO
              // 1::FRENTE, 2::DERECHA, 3::ATRÁS  4::IZQUIERDA

//VARIABLES DE CONTROL Y STATUS              
int motorVel = 80; //Velocidad de los motores, {fijada por temas de funcionalidad}             
int motorVelMax = 255; //Velocidad de los motores máxima
int temptMax = 40; //Temperatura máxima de operación del robot en °C :: 30°C
int voltjMin = 3; //Voltaje mínimo nomical de fuente de alimentación :: 3V 

//________________________________________________________________________________________
//****************************************************************************************

//FUNCIONES CONTROL DE HARDWARE

//Función de toma de valores de Status del robot
bool tomaStatus(){

  //Variables
  bool precaucion = false; // FALSE::OK, TRUE::ALERTA
  float temptTemp=0;

  //toma de datos
  voltj = (analogRead(A3)/1023)*5.0; //Voltaje de la batería de alimentación
  temptTemp = analogRead(A2); //Tempertatura ambiental del robot
  tempt = ((temptTemp / 1023.0) * 5000)/10;

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
                           /*digitalWrite(motorDer_0,HIGH);
                           digitalWrite(motorDer_1,LOW);
                           digitalWrite(motorIzq_0,HIGH);
                           digitalWrite(motorIzq_1,LOW);*/
                           analogWrite(motorDer_0,velocidad);
                           digitalWrite(motorDer_1,LOW);
                           analogWrite(motorIzq_0,velocidad);
                           digitalWrite(motorIzq_1,LOW);
                           break;

                    case 2: //DERECHA
                           /*digitalWrite(motorDer_0,LOW);
                           digitalWrite(motorDer_1,HIGH);
                           digitalWrite(motorIzq_0,HIGH);
                           digitalWrite(motorIzq_1,LOW);*/
                           digitalWrite(motorDer_0,LOW);
                           analogWrite(motorDer_1,velocidad);
                           analogWrite(motorIzq_0,velocidad);
                           digitalWrite(motorIzq_1,LOW);
                           break;
                           break;

                    case 3: //ATRAS
                           /*digitalWrite(motorDer_0,LOW);
                           digitalWrite(motorDer_1,HIGH);
                           digitalWrite(motorIzq_0,LOW);
                           digitalWrite(motorIzq_1,HIGH);*/
                           digitalWrite(motorDer_0,LOW);
                           analogWrite(motorDer_1,velocidad);
                           digitalWrite(motorIzq_0,LOW);
                           analogWrite(motorIzq_1,velocidad);
                           break;

                    case 4: //IZQUIERDA
                           /*digitalWrite(motorDer_0,HIGH);
                           digitalWrite(motorDer_1,LOW);
                           digitalWrite(motorIzq_0,LOW);
                           digitalWrite(motorIzq_1,HIGH);*/
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
//______________________________________________________________________________________
//********************************************************************************

//CONFIGURACIÓN DE LOS COMPONENTES Y LOS PINES DEL ARDUINO
void setup() {

  Serial.begin(9600);

  //Pines de estado 
  pinMode(bzzr,OUTPUT);
  pinMode(ledS,OUTPUT);

  //>> LED RGB
  pinMode(ledRed,OUTPUT); 
  //LEDGREEN :: ledS
  pinMode(ledBlue, OUTPUT);

  //>> Control Motores
  pinMode(motorDer_0,OUTPUT);
  pinMode(motorDer_1,OUTPUT);
  pinMode(motorIzq_0,OUTPUT);
  pinMode(motorIzq_1,OUTPUT);

}//FIN DE LA CONFIGURACIÓN
//********************************************************************************

//FUNCION MAIN PRINCIPAL
void loop() {
  
    digitalWrite(bzzr, LOW);
    digitalWrite(ledS,LOW);

    if( !tomaStatus() ){ //Robot en Optimas Condiciones

      //Obteniedo la información de los sensores y publicandola
      Serial.print("Lumen Der: ");
      Serial.println(analogRead(A0));
      Serial.print("Lumen Izq: ");
      Serial.println(analogRead(A1));

      Serial.print("Temperatura: ");
      Serial.println(tempt);

      Serial.print("Voltaje: ");
      Serial.println(voltj);
      Serial.println("-----------------");

      delay(1500);

      if(digitalRead(A4)== HIGH){ //Control de la dirección del motor
        
        modo++;
        if(modo >= 5){
          modo=0;
                      }          }

       delay(1500);

       switch(modo){//Control de la dirección del motor

                    case 0:
                            controlMotor(0,motorVel);
                            break;

                    case 1:
                            controlMotor(1,motorVel);
                            break;

                    case 2:
                            controlMotor(2,motorVel);
                            break;

                    case 3:
                            controlMotor(3,motorVel);
                            break;

                    case 4:
                            controlMotor(4,motorVel);
                            break;

                    default:
                            controlMotor(5,motorVel);
                            break;
        
       }//Fin del switch de direcciónes del motor
      delay(1500);
    }//Robot en optimas condiciones
}//FIN FUNCION MAIN VOID
