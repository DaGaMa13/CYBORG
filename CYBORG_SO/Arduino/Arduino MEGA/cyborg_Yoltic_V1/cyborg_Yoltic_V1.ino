/***************************************************
 *  SIGNOS INGENIERÍA YOUNGSTERS WORKSHOP
 *  
 *  PROYECTO CYBORG YOTILC VERSION 1.0
 * 
 * Programa correspondiente al proposito de que el arduino
 * funcione como unidad principal de procesamiento y control
 * de hardware del robot.
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

int numModos = 5; //Numero de modos por el cual el robot se encuentra programado
int modo = 0; //Modo en el que se encuentra el robot habilidado (PushButton)
              // 0::TestMotors
              // 1::Seguidor de Luz, 2::Evasor de Luz, 3::Seguidor de Objetos
              // 4::Evasor de objetos, 5::Seguidor de línea

int motorDir = 0; //Dirección de los motores 
                  // 0::ALTO, 1::FRENTE, 2::ATRAS, 3::DERECHA, 4::IZQUIERDA

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
  voltj = analogRead(A3); //Voltaje de la batería de alimentación
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
//---------------------------------------------------------

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

                    case 2: //ATRAS
                           /*digitalWrite(motorDer_0,LOW);
                           digitalWrite(motorDer_1,HIGH);
                           digitalWrite(motorIzq_0,LOW);
                           digitalWrite(motorIzq_1,HIGH);*/
                           digitalWrite(motorDer_0,LOW);
                           analogWrite(motorDer_1,velocidad);
                           digitalWrite(motorIzq_0,LOW);
                           analogWrite(motorIzq_1,velocidad);
                           break;

                    case 3: //DERECHA
                           /*digitalWrite(motorDer_0,LOW);
                           digitalWrite(motorDer_1,HIGH);
                           digitalWrite(motorIzq_0,HIGH);
                           digitalWrite(motorIzq_1,LOW);*/
                           digitalWrite(motorDer_0,LOW);
                           analogWrite(motorDer_1,velocidad);
                           analogWrite(motorIzq_0,velocidad);
                           digitalWrite(motorIzq_1,LOW);
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

//FUNCIONES TEST
void motorsTest(){
  
        controlMotor(0,motorVel);
        delay(1500);
        controlMotor(1,motorVel);
        delay(1500);
        controlMotor(2,motorVel);
        delay(1500);
        controlMotor(3,motorVel);
        delay(1500);
        controlMotor(4,motorVel);
        delay(1500);
        controlMotor(5,motorVel);
        delay(1500);
  
}//Fin Motor Test
//______________________________________________________________________________________


//FUNCIONES DE COMPORTAMIENTO

//Función Comportamiento Seguidor de Luz
void seguidorLuz( int sensorMayor){

  if(sensorMayor == 1){ //Fuente LUZ FRENTE
    controlMotor(1,motorVel);}

  else if (sensorMayor == 2){ //FUENTE LUZ DERECHA
    controlMotor(3,motorVel);}

  else if (sensorMayor == 3){ //FUENTE LUZ IZQUIERDA
    controlMotor(4,motorVel); }

  else{ //SIN FUENTE DE LUZ
    controlMotor(0,0); }
    //[PENDIENTE]::: Programar algoritmo para la busqueda de una fuente de luz
  
}//Fin Seguidor de Luz
//--------------------------------------------------------------------

//Función Comportamiento Evasor de Luz
void evasorLuz( int sensorMayor){

  if(sensorMayor == 1){ //Fuente LUZ FRENTE
    controlMotor(0,0);}

  else if (sensorMayor == 2){ //FUENTE LUZ DERECHA
    controlMotor(4,motorVel);}

  else if (sensorMayor == 3){ //FUENTE LUZ IZQUIERDA
    controlMotor(2,motorVel); }

  else{ //SIN FUENTE DE LUZ
    controlMotor(1,motorVel); }
  
}//Fin Evasor de Luz
//---------------------------------------------------------------------

//Función Comportamiento Seguidor Obstáculos
void seguidorObj(int sensorMayor){
  
}//Fin Seguidor de Objetos
//---------------------------------------------------------------------

//Función Comportamiento Evasor Obstáculos
void evasorObj( int sensorMayor){
  
}//Fin Evasor de Objetos
//---------------------------------------------------------------------
//______________________________________________________________________________________
//********************************************************************************

//CONFIGURACIÓN DE LOS COMPONENTES Y LOS PINES DEL ARDUINO
void setup() {

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

    //if( !tomaStatus() ){ //Robot en Optimas Condiciones
    if( true ){ //Robot en Optimas Condiciones

    /*  //if(digitalRead(A4) == HIGH){
      while(digitalRead(A4) == HIGH){

        
        modo++;
        digitalWrite(ledRed,HIGH);
        
        if (modo > numModos){
          modo = 0;}
          
        delay(1000); 
        digitalWrite(ledRed,LOW); 
        delay(1500);
        } //Fin While para la toma del modo
*/
        modo=0;

        switch(modo){

                      case 0:
                             digitalWrite(ledBlue,HIGH);
                             motorsTest(); //Prueba del avance de los motores
                             break;

                      case 1:
                             digitalWrite(ledBlue,HIGH);
                            seguidorLuz(tomaLumen());
                            break;
                            
                      case 2:      
                             digitalWrite(ledBlue,HIGH);
                             evasorLuz(tomaLumen());
                             break;
                                        
                      default:  
                              digitalWrite(ledRed,HIGH);
                              delay(500);
                              digitalWrite(ledRed,LOW);
                              delay(500);
                             digitalWrite(ledBlue,HIGH);
                             delay(500);
                             digitalWrite(ledBlue,LOW);
                              break;

          
        }//FIN EJECUCIÓN DE COMPORTAMIENTO

        digitalWrite(ledBlue, LOW);
      
    }//Robot en optimas condiciones
  
}//FIN FUNCION MAIN VOID
