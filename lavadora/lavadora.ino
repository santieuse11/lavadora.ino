// version 1.1 de la lavadora con arduino
// la valvula de entrada de agua o val1 estara conectada al pin3
// el giro del motor 1 o giro1 en el sentido contrario a la manecillas el reloj, estara conectado al pin 4
// el giro del motor 2 o giro2 en el sentido de las manecillas el reloj, estara conectado al pin 5
// la bomba de agua estara conectada junto del el bloqueo del tanque ya que solo dispongo de 4 reles, en el pin 6
// el modulo de reles se activa con os pines en bajo, o LOW, o 0 logico.
// este codigo fue creado por Santiago Eusse Toro.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ESTE ARREGLO DETERMINA EN QUE ESTADO SE ENCUENTRA NUESTRA LAVADORA
String ciclos[6 ] = {"LLENANDO",
                     "LAVANDO",
                     "VACIANDO",
                     "ACELERAR",
                     "CENTRIFUGANDO",
                     "ESPERA"
                    };

bool led = true;
unsigned long hora = 0;
const int intervalo = 1000;

int contador = 0;
int segundos = 0;
int minuto = 0;

String ciclo;    // VARIABLE PARA LA ACTUALIZACION DEL CICLO ENLA PANTALL
int paso = 0;    // REGISTRO DE PASO PARA EL LAVADO Y EL CICLO DE ACELERACION DEL TANQUE

int val1 =  3;    // VALVULA DE ENTRADA DE AGUA
int giro1 = 4;    // GIRO DEL MOTOR
int giro2 = 5;    // GIRO DEL MOTOR
int bomba = 6;    // BOMBA DE AGUA
int bloqueo = 7;  // BLOQUEO DE TANQUE O FRENO DEL TANQUE (AUN NO UTILIZADO)
int alarma = 8;   // ALARMA BUZZER PARA FIN DE LAVADO (AUN NO UTILIZADO)


// si la direccion 0x27 no funciona, prueba con 0x28 ó con 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

//CONFIGURACION DE PINES
void setup() {

  //CONFIGURAMOS LOS PINES DE SALIDA NECESARIOS PARA NUESTRA LAVADORA
  pinMode(val1, OUTPUT);   // VALVULA UNO O VAL1
  pinMode(giro1, OUTPUT);
  pinMode(giro2, OUTPUT);
  pinMode(bomba, OUTPUT);
  pinMode(bloqueo, OUTPUT);
  pinMode(alarma, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);   // LED INDICATIVO DE TRANCURSO DEL TIEMPO

  // CONFIGURACION INICIAL DE LOS PINES EN ALTO, YA QUE LOS RELES ENCIENDEN CUANDO PONEMOS EN BAJO EL PIN
  // CONFIGURAMOIS EN ALTO LOS PINES PARA QUE LOS RELES ESTEN APAGADOS AL INICIO DEL LOOP
  digitalWrite(val1, HIGH);
  digitalWrite(giro1, HIGH);
  digitalWrite(giro2, HIGH);
  digitalWrite(bomba, HIGH);

  //INICIALIZACION DE LA PANTALLA LCD 16X2
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("LAVADORA");
  lcd.setCursor(3, 1);
  lcd.print("VER  1.1");
  delay(2000);

  // PRESENTAMOS NUESTRA PANTALLA INICIAL QUE MOSTRARA EL TIEMPO Y EL ESTADO SE NUESTRA LAVADORA
  PantallaLavado();
}

//FUNCIONES PARA OPTIMIZAR UN POCO EL CODIGO

//FUNCION PARA MOSTRAR EN PANTALLA EL TIEMPO Y EL ESTADO DE LA LAVADORA
void PantallaLavado() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ciclo);
  lcd.setCursor(0, 1);
  lcd.print("TIEMPO");
  lcd.setCursor(8, 1);
  lcd.print(minuto);
  lcd.setCursor(11, 1);
  lcd.print(segundos);

}

//FUNCION DE LLENADO
void llenado() {

  digitalWrite(giro1, HIGH); // APAGAMOS EL RELE DE GIRO
  digitalWrite(giro2, HIGH); // APAGAMOS EL RELE DE GIRO
  digitalWrite(bomba, HIGH); // APAGAMOS EL RELE DE BOMBA

  ciclo = ciclos[0];         // ACTUALIZAMOS EL ESTADO EN PANTALLA "LLENANDO"
  digitalWrite(val1, LOW);   // ENCENDEMOS LA VALVULA PARA QUE PUEDA ENTRAR AGUA

}

//FUNCION DE LAVADO
void lavado() {

  ciclo = ciclos[1];         // SE ACTUALIZA EL ESTADO EN PANTALLA "LAVANDO"
  digitalWrite(val1, HIGH);      // SE APAGA LA VALVULA DE ENTRADA DE AGUA

  if (minuto == 3 && segundos <= 1 ) {    // COMPROBACION NECESARIA PARA RESETEAR LOS PASOS DE LAVADO
    paso = 1;
  }

  if (minuto == 28 && segundos <= 1 ) {   // COMPROBACION PARA EL SEGUNDO CICLO DE LAVADO
    paso = 1;
  }

  if (minuto == 43 && segundos <= 1 ) {   // COMPROBACION PARA EL TERCER CICLO DE LAVADO
    paso = 1;
  }

  if (paso == 1) {               //PASO DE LAVADO 1  CICLO DE MOTOR APAGADO
    digitalWrite(giro1, HIGH);
    digitalWrite(giro2, HIGH);
  }
  else if (paso == 2) {        //PASO DE LAVADO 2  CICLO DE GIRO EN EL SENTIDO CONTRARIO A LAS MANECILLAS DEL RELOJ
    digitalWrite(giro1, LOW);
    digitalWrite(giro2, HIGH);
  }
  else if (paso == 3) {        //PASO DE LAVADO 3  CICLO DE MOTOR APAGADO
    digitalWrite(giro1, HIGH);
    digitalWrite(giro2, HIGH);
  }
  else if (paso == 4) {        //PASO DE LAVADO 4  CICLO DE GIRO EN EL SENTIDO DE LAS MANECILLAS DEL RELOJ
    digitalWrite(giro1, HIGH);
    digitalWrite(giro2, LOW);
  }

  if (paso == 4) {              // RESETEAR LOS PASOS PARA REPETIR EL CICLO DE LAVADO DURANTE EL TIEMPO ESTIMADO
    paso = 0;
  }

}

//FUNCION DE VACIADO DE TANQUE
void vaciado() {

  ciclo = ciclos[2];         //ACTUALIZAMOS EL ESTADO EN PANTALLA A "VACIANDO"
  digitalWrite(val1, HIGH);   // APAGAMOS FUNCIONES QUE NO NECESITAMOS
  digitalWrite(giro1, HIGH);
  digitalWrite(giro2, HIGH);

  digitalWrite(bomba, LOW);     // ENCENDIDO DE LA BOMBA PARA VACIAR EL TANQUE
}

void acelerar() {          //FUNCION PARA ACELERAR EL TANQUE PARA CENTRIFUGAR


  if (minuto == 32 && segundos <= 1 ) {
    paso = 1;
  }

  if (minuto == 55 && segundos <= 1 ) {
    paso = 1;
  }

  ciclo = ciclos[3];
  digitalWrite(val1, HIGH);    // APAGAMOS FUNCIONES QUE NO NECESITAMOS
  digitalWrite(giro1, HIGH);

  digitalWrite(bomba, LOW);  //MANTENEMOS ENCENDIDO


  if (paso == 1) {
    digitalWrite(giro2, HIGH);
    digitalWrite(giro1, HIGH);
  } else if (paso == 2) {
    digitalWrite(giro2, LOW);
    digitalWrite(giro1, HIGH);
  }

  if (paso == 2) {
    paso = 0;
  }

}

void centrifugar() {       //FUNCION DE CENTRIFUGADO

  ciclo = ciclos[4];
  digitalWrite(val1, HIGH);
  digitalWrite(giro1, HIGH);

  digitalWrite(bomba, LOW);
  digitalWrite(giro2, LOW);

}

void apagar() {

  ciclo = ciclos[5];

  digitalWrite(val1, HIGH);
  digitalWrite(giro1, HIGH);
  digitalWrite(bomba, HIGH);
  digitalWrite(giro2, HIGH);


}

void loop() {

  /////////////////////////////////////////// control tiempos
  if (millis() - hora >= intervalo) {
    hora = millis();
    digitalWrite(LED_BUILTIN, led);
    led = !led;
    contador = contador + 1;
    segundos = segundos + 1;

    if (segundos == 60) {
      minuto = minuto + 1;
      segundos = 0;
    }

    if (contador == 2) {
      contador = 0;
      paso = paso + 1;
    }

    PantallaLavado();

  }

  ///////////////////////////////////////// LLENADO INICIAL DEL TANQUE

  if ( minuto < 3 ) {
    llenado();
  }

  ///////////////////////////////////////////// PRIMER CICLO DE LAVADO

  if (minuto >= 3 && minuto < 23 ) {
    lavado();
  }

  //////////////////////////////////////////// CILO DE VACIADO

  if (minuto >= 23 && minuto < 25 ) {
    vaciado();
  }

  ///////////////////////////////////////// LLENADO PARA EL CICLO DE ENGUAGUE

  if (minuto >= 25 && minuto < 28 ) {
    llenado();
  }

  ///////////////////////////////////////////// SEGUNDO CICLO DE LAVADO, ENGUAGUE DE ROPA

  if (minuto >= 28 && minuto < 38 ) {
    lavado();
  }

  //////////////////////////////////////////// CILO DE VACIADO

  if (minuto >= 30 && minuto < 32 ) {
    vaciado();
  }
  ///////////////////////////////////////////// ACELERACION DE TANQUE

  if (minuto >= 32 && minuto < 34 ) {
    acelerar();
  }

  ////////////////////////////////////////////// CENTRIFUGADO

  if (minuto >= 34 && minuto < 40 ) {
    centrifugar();
  }

  //////////////////////////////////////////////////TERCER CILO DE LLENADO

  if (minuto >= 40 && minuto < 43 ) {
    llenado();
  }

  ///////////////////////////////////////////////////////////////// TERCER CICLO DE LAVADO

  if (minuto >= 43 && minuto < 53 ) {
    lavado();
  }

  ////////////////////////////////////////////////////////////////// CICLO DE VACIADO PARA SECADO FINAL

  if (minuto >= 53 && minuto < 55 ) {
    vaciado();
  }

  ////////////////////////////////////////////////////////// CICLO DE ACELERACION DEL TANQUE

  if (minuto >= 55 && minuto < 56 ) {
    acelerar();
  }

  ////////////////////////////////////////////////////////// SEGUNDO CICLO DE CENTRIFUGADO

  if (minuto >= 56 && minuto < 66) {
    centrifugar();
  }

  ////////////////////////////////////////////////////////// CICLO DE ESPERA AQUI SE APAGN TODAS LAS FUNCIONES

  if (minuto >= 66 && minuto < 70) {
    apagar();
  }

}
