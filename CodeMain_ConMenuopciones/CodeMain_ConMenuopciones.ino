/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <LowPower.h>
#include <DHT11.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte arrowDown[8]  = {B00000, B01110, B01110, B01110, B11111, B01110, B00100}; // Caracteres personalizados para el LCD
byte cursorLCD[8] = {B00000, B00000, B00110, B01111, B01111, B00110, B00000};
const int pinhumsub=A0;//Para humedad en el piso.
const int pinrain=10;
const int pinAmbtemp=8;
float humAmb=0;
float humSub=0;
float tempe=0;
int presenciaRain=LOW;
int err;
const int boton1=9;//Derecha.
const int boton2=7;//Abajo.
const int boton3=6;//Izquierda.
const int boton4=0;//Arriba.

int read1;//Lectura derecha.
int read2;//Lectura abajo.
int read3;//Lectura izquierda
int read4;//Lectura arriba.

const float umbralHumSub=800;
const float umbralHumAmb=50;
const float tempMin=24.0;
const float tempMax=32.9;
const int hourRain=1;
const int minRain=60;
const int hourHumSub=1;
const int minHumSub=60;
const int hourHumAmb=1;
const int minHumAmb=60;
const int hourTempLuz=1;
const int minTempLuz=60;
int riega=0;
int presenciaLuz=1;
//Constantes y variables para fotoresistencia LDR.
const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ
const int LDRPin = A3;   //Pin del LDR
const int umbralDarkness=5;
int V;
int ilum;
DHT11 dht11(pinAmbtemp);
void setup() {
  pinMode(10, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Se añaden los caracteres especiales al objeto LCD:
  lcd.createChar(0, cursorLCD);
  lcd.createChar(1, arrowDown);
  // Print a message to the LCD.
 // lcd.print("hello, world!");
 pinMode(boton1, INPUT_PULLUP);
 pinMode(boton2,INPUT_PULLUP);
 pinMode(boton3,INPUT_PULLUP);
 pinMode(boton4,INPUT_PULLUP);
 pinMode(1,OUTPUT);
 digitalWrite(1,HIGH);
}
void loop() {
  
  // Guarda el resultado leído de una pulsación por teclado:
  char resultButton[7] = "null";

  // Se imprime la pantalla de inicio:
  lcd.setCursor(0, 0); lcd.print("*Sistema Irrigafree*");
  lcd.setCursor(2, 1); lcd.print("By: Jorge Coral");
  lcd.setCursor(5, 3); lcd.print("Presione "); lcd.write(byte(1));

  // Se comprueba si se ha pulsado un botón:
  getButtonPush( resultButton,200);

  // Si se pulsa la tecla abajo, se entra en el menú:
  if( !strcmp(resultButton,"bottom"))
    LCDmenu(); 

}

/*----------------------------------------------------------------------------------------------------------*/
void muestreaParametros(){
   //Apartado de captura de datos:
  presenciaRain=digitalRead(pinrain);
  V = analogRead(LDRPin);
  ilum = ((long)V*A*10)/((long)B*Rc*(1024-V));    //usar si LDR entre A0 y Vcc (como en el esquema anterior);//Luz del sol es mayor a 2000
  humSub = analogRead(pinhumsub)*100/1024;
  if(analogRead(pinhumsub))
    humSub=0;
if((err=dht11.read(humAmb, tempe))==0)
  {
  humAmb=humAmb;
  tempe=tempe;
  }
  delay(DHT11_RETRY_DELAY); //delay for reread
}
/*----------------------------------------------------------------------------------------------------------*/

void despliegueParametros(){
   //Apartado despliegue LCD:
  lcd.clear();
  lcd.noCursor();
  lcd.setCursor(1,0);
  lcd.print("Humedad del aire:");
  lcd.setCursor(8,1);
  lcd.print(humAmb);
  lcd.print("%");
  lcd.setCursor(1,2);
  lcd.print("Humedad en suelo:");
  lcd.setCursor(8,3);
  lcd.print(humSub);
  lcd.print("%");
  delay(5000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Temperatura:");
  lcd.setCursor(8,1);
  lcd.print(tempe);
  lcd.print((char)223);
  lcd.print ("C");
  lcd.setCursor(1,2);
  lcd.print("Senseo de lluvia?");
  lcd.setCursor(6,3);
  if (presenciaRain==1){
    lcd.print("Negativo");
  }else{
    lcd.print("Positivo");
  }
  delay(5000);
  lcd.clear();
 //Termina apartado de LCD:
}
/*----------------------------------------------------------------------------------------------------------*/

// MENU PRINCIPAL, DA OPCION PARA ABRIR EL MENU 1 Y MENU 2, TAMBIEN PARA SALIR A LA PANTALLA PRINCIPAL
void LCDmenu()
{
  char resultButton[7];
  int posCursor=0, opcMenu=-2;


  // Menu
  menuSecun:
  lcd.clear();
  lcd.setCursor(1, 0); 
  lcd.print("-Activar Sistema-");
  lcd.setCursor(1, 1); 
  lcd.print("-Desactivar Sistema-");
  lcd.setCursor(1, 2); 
  lcd.print("-Salir-");
  lcd.setCursor(0,3);
  lcd.print("[Seleccione alguna]");


  // Mientras no se pulse sobre salir, no se saldrá a la pantalla principal:
  do{
    strcpy(resultButton,"null");
    getButtonPush( resultButton, 200);

    // Cursor que indica la opción seleccionada:
    lcd.setCursor(0, posCursor); lcd.write(byte(0));

    // Desplaza el cursor arriba o abajo:
    if(!strcmp(resultButton,"up"))
      LCDMueveCursor(&posCursor, 2, 'r'); // Se resta una posición al cursor

    else if(!strcmp(resultButton,"bottom")) // Se suma una posición al cursor
      LCDMueveCursor(&posCursor, 2, 's');

    else if(!strcmp(resultButton,"right"))
      opcMenu = posCursor;        // Al pulsar a la derecha (para acceder a esa opcion) se actualiza la opción de menú elegida según donde esta el cursor ahora.


    // Según la opción elegida del menú, se llama a otro menú o se cierra el menú actual:
    switch( opcMenu )
    {
      case 0:       // Activar modo de automático riego.
        riegoAutomatico();
        opcMenu = -2;
        goto menuSecun;
        break;
      case 1:       // Dormir Sistema.
        modoSleep();
        opcMenu = -2;
        break;
      case 2:       // Salir a la pantalla principal.
        opcMenu = -1;
        lcd.clear();
        break;
    };
  }
  while(opcMenu != -1);
}
/*----------------------------------------------------------------------------------------------------------*/

// DESPLAZA EL CURSOR UNA POSICION EN LA DIRECCION INDICADA
void LCDMueveCursor(int *pos, int max, char ope)
{
  // Precondición:  El parámetro ope debería de ser el carácter 's' o 'r' para sumar o restar una posición.
  //          El parámetro max indica el numero de opciones a elegir.
  // Poscondición:  Se devuelve por referencia el nuevo valor del cursor.

  if(ope == 's')
    if(*pos < max)  *pos += 1;
    else      *pos = 0;
  else
    if(*pos > 0)  *pos -= 1;
    else      *pos = max;

  // Se borran los cursores:
  lcd.setCursor(0, 0); lcd.print(" ");
  lcd.setCursor(0, 1); lcd.print(" ");
  lcd.setCursor(0, 2); lcd.print(" ");
  lcd.setCursor(0, 3); lcd.print(" ");
}

/*----------------------------------------------------------------------------------------------------------*/

// PROCEDIMIENTO QUE COMPRUEBA SI HA HABIDO NUEVAS PULSACIONES CON LOS BOTONES:
void getButtonPush(char *resultButton, int pause)
{
  // Precondición:  El parámetro pin ha de ser un pin Analógico del Arduino.
  // Poscondición:  El parámetro resultButton es una cadena por referencia que devolverá el botón pulsado.
  //          El parámetro pause ha de ser un numero entero igual o mayor que 0.
  // Info:      El teclado usado es este: http://giltesa.com/?p=11458 , según el tipo de resistencias habrá que cambiar las condicionales para adaptarse al valor leído desde el pin Analógico.
  read1=digitalRead(boton1);
  read2=digitalRead(boton2);
  read3=digitalRead(boton3);
  read4=digitalRead(boton4);

    if (read4==LOW)
      strcpy(resultButton,"up");
    if (read3==LOW)
      strcpy(resultButton,"left");
     if (read2==LOW)
      strcpy(resultButton,"bottom");
     if (read1==LOW)
      strcpy(resultButton,"right");

    delay(pause);
  
}

void riegoAutomatico(){
  int contador=0;
  riego:
  if(digitalRead(boton3)==LOW){
     lcd.clear();
     return;
  }
  muestreaParametros();
  despliegueParametros();
  delay(1000);
  lcd.clear();
  lcd.home();
  lcd.print("Verificando...");
  if(presenciaRain==HIGH){
    if(humSub>umbralHumSub){//Un suelo ligeramente húmero daría valores típicos de 600-700. Un suelo seco tendrá valores de 800-1023.
        if(humAmb<umbralHumAmb){//100% muy humedo, 0 % falta de humedad.
          if((tempe>tempMin && tempe<tempMax)||ilum<=umbralDarkness){
                //Se manda un pulsto para activar el relevador durante un tiempo.
                riega=1;
          }else{
            //Esperar.
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("No apto para riego.");
            lcd.setCursor(0,2);
            lcd.print("Prorroga de riego...");
            for (int k=0;k<hourTempLuz;k++){//Cantidad de horas.
              for(int j=0;j<minTempLuz;j++){//Una hora.
                 for (int i=0;i<8;i++){//Un minuto.
                     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);//Función dormir arduino por un cierto tiempo. 
                                         }
                                   }
                            }
            riega=0;
            goto riego;
          }
        }else{
          //Esperar.
          riega=0;
          //Función dormir arduino por un cierto tiempo.
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("No apto para riego");
            lcd.setCursor(0,2);
            lcd.print("Prorroga de riego...");
            for (int k=0;k<hourHumAmb;k++){//Cantidad de horas.
              for(int j=0;j<minHumAmb;j++){//Una hora.
                 for (int i=0;i<8;i++){//Un minuto.
                     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);//Función dormir arduino por un cierto tiempo. 
                                         }
                                    }
                            }
          //Volver a intentar riego.
          goto riego;
        }
    }else{
      //Esperad.
      riega=0;
      //Función dormir arduino por un cierto tiempo.
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("No apto para riego");
            lcd.setCursor(0,2);
            lcd.print("Prorroga de riego...");
            for (int k=0;k<hourHumSub;k++){//Cantidad de horas.
              for(int j=0;j<minHumSub;j++){//Una hora.
                 for (int i=0;i<8;i++){//Un minuto.
                     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);//Función dormir arduino por un cierto tiempo. 
                                         }
                                    }
                            }
      //Volver a intentar riego.
      goto riego;
    }
  }else{
    //Esperar.
    riega=0;
    //Función dormir arduino por un cierto tiempo.
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("No apto para riego");
            lcd.setCursor(0,2);
            lcd.print("Prorroga de riego...");
            for (int k=0;k<hourRain;k++){//Cantidad de horas.
              for(int j=0;j<minRain;j++){//Una hora.
                 for (int i=0;i<8;i++){//Un minuto.
                     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);//Función dormir arduino por un cierto tiempo. 
                                         }
                                    }
                            }
    //Volver a intentar riego.
    goto riego;
    
  }
lcd.clear();
lcd.home();
lcd.print("Sistema listo!");
lcd.setCursor(0,1);
lcd.print("Regando... ");
for (int i=0;i<30;i++){
  digitalWrite(13,HIGH);
  if(i>20){
  lcd.print(".");
  }
  delay(1000);
}
digitalWrite(13,LOW);
messegeWait();
  for(int k=0;k<8;k++){
      for(int j=0;j<60;j++){
        for(int i=0;i<8;i++){
            LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
        }
     }
  }
}


void modoSleep(){  
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    messegeWait();
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    //LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
}

void messegeWait()
{   
    lcd.clear();
    lcd.setCursor(3,1);
    lcd.print("--Modo Sleep--");
}

