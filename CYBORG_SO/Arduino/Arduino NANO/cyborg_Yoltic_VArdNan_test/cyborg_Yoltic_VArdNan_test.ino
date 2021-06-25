/***************************************************
 *  SIGNOS INGENIERÍA YOUNGSTERS WORKSHOP
 *  
 *  PROYECTO CYBORG YOTILC TEST VERSION
 * 
 * Programa correspondiente a la prueba de los componentes
 * integrados en el robot Cyborg Yoltic siendo administradso
 * únicamente por la unidad de procesamiento Arduino.
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
 * D2 :: RGB RED
 * ~D3 :: MOTOR_DER0
 * D4 :: RGB GREEN
 * ~D5 :: MOTOR_DER1
 * -D6 :: BUZZER
 * D7 :: RGB BLUE
 * 
 * ~D9 :: MOTOR IZQ 0
 * ~D10 :: MOTOR IZQ 1
 * D13 :: LED STATUS
 */

// << Bibliotecas >>
//---------------------------------------------------------------

// << Declaración de lo spines de salida >>

int ledRED = 2; //LED RGB RED
int ledGREEN = 4; //LED RGB GREEN
int ledBLUE = 7; //LED RGB BLUE


//int ultraSDer_T = 2; //Sensor Ultrasónico Derecho Tx
//int ultraSDer_R = 3; //Sensor Ultrasónico Derecho Rx
//int ultraSIzq_T = 4; //Sensor Ultrasónico Izquierdo Tx
//int ultraSIzq_R = 5; //Sensor Ultrasónico Izquierdo Rx

int bzzr = 6; // Salida digital Buzzer de alerta

int motorDer_0 = 3; //Salida digital 0 de dirección del Motor Derecho
int motorDer_1 = 5; //Salida digital 1 de dirección del Motor Derecho
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
  tempt = ((temptTemp / 1023.0) * 5000)/10; //Obteniedo temperatura del ambiente °C

  if(voltj > voltjMin){ //Voltaje nominal adecuado

    if(tempt <= temptMax){ //Temperatura ambiental adecuada
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

 return valorMaximo;   
}// Fin de la función tomaLumen
//______________________________________________________________________________________
//********************************************************************************

//CONFIGURACIÓN DE LOS COMPONENTES Y LOS PINES DEL ARDUINO
void setup() {

  Serial.begin(9600);

  //Pines de estado 
  pinMode(bzzr,OUTPUT);
  pinMode(ledS,OUTPUT);

  //>> LED RGB
  pinMode(ledRED,OUTPUT);
  pinMode(ledGREEN,OUTPUT); 
  pinMode(ledBLUE, OUTPUT);

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

      delay(500);

      //if(digitalRead(A4)== HIGH){ //Control de la dirección del motor
        
        modo++;
        if(modo >= 5){
          modo=0;
                      }//          }

       delay(500);

       switch(modo){//Control de la dirección del motor

                    case 0:
                            controlMotor(0,motorVel);
                            ledRGB(1);
                            break;

                    case 1:
                            controlMotor(1,motorVel);
                            ledRGB(2);
                            break;

                    case 2:
                            controlMotor(2,motorVel);
                            ledRGB(3);
                            break;

                    case 3:
                            controlMotor(3,motorVel);
                            ledRGB(2);
                            break;

                    case 4:
                            controlMotor(4,motorVel);
                            ledRGB(3);
                            break;

                    default:
                            controlMotor(5,motorVel);
                            ledRGB(0);
                            break;
        
       }//Fin del switch de direcciónes del motor
      delay(1500);
    }//Robot en optimas condiciones

    controlMotor(0,motorVel);
    ledRGB(0);
}//FIN FUNCION MAIN VOID
