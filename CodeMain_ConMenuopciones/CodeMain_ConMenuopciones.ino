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
#include <DHT11.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte arrowDown[8]  = {B00000, B01110, B01110, B01110, B11111, B01110, B00100}; // Caracteres personalizados para el LCD
byte cursorLCD[8] = {B00000, B00000, B00110, B01111, B01111, B00110, B00000};
const int pinhumsub=A0;//Para humedad en el piso.
const int pinrain=10;
const int pinAmbtemp=8;
float humAmb=0;
int humSub=0;
float tempe=0;
byte presenciaRain=LOW;
int err;
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
  
}

void loop() {
  
  // Guarda el resultado leído de una pulsación por teclado:
  char resultButton[7] = "null";

  // Se imprime la pantalla de inicio:
  lcd.setCursor(5, 0); lcd.print("***Sistema Irrigafree***");
  lcd.setCursor(0, 1); lcd.print("Hecho por Jorge Coral");
  lcd.setCursor(7, 3); lcd.print("Presione "); lcd.write(byte(1));

  // Se comprueba si se ha pulsado un botón:
  getButtonPush( resultButton, 0, 200);

  // Si se pulsa la tecla abajo, se entra en el menú:
  if( !strcmp(resultButton,"bottom"))
    LCDmenu(); 

}

/*----------------------------------------------------------------------------------------------------------*/
void muestreaParametros(){
   //Apartado de captura de datos:
  presenciaRain=digitalRead(pinrain);
  humSub = analogRead(pinhumsub)*100/1024;
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
  delay(1500);
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
  delay(1500);
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
  lcd.clear();
  lcd.setCursor(2, 0); lcd.print("Activar Sistema");
  lcd.setCursor(2, 1); lcd.print("Desactivar Sistema");
  lcd.setCursor(2, 2); lcd.print("Salir");


  // Mientras no se pulse sobre salir, no se saldrá a la pantalla principal:
  do{
    strcpy(resultButton,"null");
    getButtonPush( resultButton, 0, 200);

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
      case 0:       // Se accede al menú de encendido y apagado de los leds.
       
        opcMenu = -2;
        break;
      case 1:       // Se accede al menú de ajuste de brillo de los leds.
      
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
void getButtonPush(char *resultButton, int pin, int pause)
{
  // Precondición:  El parámetro pin ha de ser un pin Analógico del Arduino.
  // Poscondición:  El parámetro resultButton es una cadena por referencia que devolverá el botón pulsado.
  //          El parámetro pause ha de ser un numero entero igual o mayor que 0.
  // Info:      El teclado usado es este: http://giltesa.com/?p=11458 , según el tipo de resistencias habrá que cambiar las condicionales para adaptarse al valor leído desde el pin Analógico.

  int data = analogRead(1);

  if (data > 10)
  {
    if (data > 500 && data < 540)
      strcpy(resultButton,"up");
    else if (data > 330 && data < 370)
      strcpy(resultButton,"left");
    else if (data > 240 && data < 280)
      strcpy(resultButton,"bottom");
    else if (data > 190 && data < 230)
      strcpy(resultButton,"right");

    delay(pause);
  }
}
/*----------------------------------------------------------------------------------------------------------*/
