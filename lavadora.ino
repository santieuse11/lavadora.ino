#include <Wire.h>
#include <LiquidCrystal_I2C.h>

String ciclos[4 ] = {"LLENANDO",
                     "LAVANDO",
                     "CENTRIFUGANDO",
                     "ESPERA"
                    };

bool estado = 0;

bool led = true;                //Estado del LED
unsigned long hora = 0;         //Hora de cambio de estado
const int intervalo = 1000;      //Intervalo de parpadeo (millisegundos)

int contador = 0;
int segundos = 0;
int minuto = 0;
int pausa = 0;

String ciclo;

int amarillo = 9;
int paso = 0;

int val1 =  3;    // valvula de entrada de agua
int giro1 = 4;    // giro del motor 1
int giro2 = 5;    // giro del motor 2
int bomba = 6;    // bomba de agua
int bloqueo = 7;  // bloqueode tanque
int alarma = 8;   // alarma fimal de lavado



LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup() {

  pinMode(val1, OUTPUT);
  pinMode(giro1, OUTPUT);
  pinMode(giro2, OUTPUT);
  pinMode(bomba, OUTPUT);
  pinMode(bloqueo, OUTPUT);
  pinMode(alarma, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(22, OUTPUT);

  digitalWrite(22, LOW);
  digitalWrite(val1, HIGH);
  digitalWrite(giro1, HIGH);
  digitalWrite(giro2, HIGH);
  digitalWrite(bomba, HIGH);
  digitalWrite(amarillo, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(6, 1);
  lcd.print("LAVADORA");
  lcd.setCursor(6, 2);
  lcd.print("VER  1.0");
  delay(2000);

  PantallaLavado();

}
/////////////////////////////////////////////
void prender() {
  for (long i = 0; i <= 50000; i++) {
    digitalWrite(amarillo, 1);
  }
  digitalWrite(amarillo, 0);
}

void PantallaLavado() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ciclo);
  lcd.setCursor(0, 3);
  lcd.print("TIEMPO");
  lcd.setCursor(8, 3);
  lcd.print(minuto);
  lcd.setCursor(11, 3);
  lcd.print(segundos);

  lcd.setCursor(0, 2);
  lcd.print("estado");
  lcd.setCursor(7, 2);
  lcd.print(estado);
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
      estado = !estado;
      contador = 0;
      paso = paso + 1;
    }

    if (minuto == 3 && segundos <= 2) {
      estado = 1;
      paso = 1;
    }
    if (minuto == 33 && segundos <= 2) {
      estado = 1;
      paso = 1;
    }
    if (minuto == 22 && segundos <= 2) {
      paso = 1;
    }

    if (minuto == 52 && segundos <= 2) {
      paso = 1;
    }


    PantallaLavado();



  }
  ///////////////////////////////////////// llenado

  if (minuto < 3) {

    ciclo = ciclos[0];
    digitalWrite(val1, LOW);

  }



  ///////////////////////////////////////////// lavado

  if (minuto >= 3 && minuto < 20) {

    digitalWrite(val1, HIGH);

    if (paso == 1) {
      digitalWrite(giro1, HIGH);
      digitalWrite(giro2, HIGH);
    } else if (paso == 2) {
      digitalWrite(giro1, LOW);
      digitalWrite(giro2, HIGH);
    } else if (paso == 3) {
      digitalWrite(giro1, HIGH);
      digitalWrite(giro2, HIGH);
    } else if (paso == 4) {
      digitalWrite(giro1, HIGH);
      digitalWrite(giro2, LOW);
    }


    if (paso == 4) {
      paso = 0;
    }

    ciclo = ciclos[1];

  }


  ////////////////////////////////////////////

  if (minuto >= 20 && minuto < 22) {
    digitalWrite(val1, HIGH);
    digitalWrite(giro1, HIGH);
    digitalWrite(giro2, HIGH);

    digitalWrite(bomba, LOW);

    ciclo = ciclos[2];

  }
  if (minuto >= 22 && minuto < 23) {
    digitalWrite(val1, HIGH);
    digitalWrite(giro1, HIGH);

    digitalWrite(bomba, LOW);
    if (paso == 1) {
      digitalWrite(giro2, HIGH);
    } else if (paso == 2) {
      digitalWrite(giro2, LOW);
    }
    if (paso == 2) {
      paso = 0;
    }

    ciclo = ciclos[2];

  }

  if (minuto >= 23 && minuto < 30) {
    digitalWrite(val1, HIGH);
    digitalWrite(giro1, HIGH);

    digitalWrite(bomba, LOW);
    digitalWrite(giro2, LOW);

    ciclo = ciclos[2];

  }

  if (minuto >= 30 && minuto < 33) {
    digitalWrite(giro1, HIGH);
    digitalWrite(bomba, HIGH);
    digitalWrite(giro2, HIGH);

    ciclo = ciclos[0];
    digitalWrite(val1, LOW);

  }
  if (minuto >= 33 && minuto < 50) {

    digitalWrite(val1, HIGH);

    if (paso == 1) {
      digitalWrite(giro1, HIGH);
      digitalWrite(giro2, HIGH);
    } else if (paso == 2) {
      digitalWrite(giro1, LOW);
      digitalWrite(giro2, HIGH);
    } else if (paso == 3) {
      digitalWrite(giro1, HIGH);
      digitalWrite(giro2, HIGH);
    } else if (paso == 4) {
      digitalWrite(giro1, HIGH);
      digitalWrite(giro2, LOW);
    }


    if (paso == 4) {
      paso = 0;
    }

    ciclo = ciclos[1];

  }

  if (minuto >= 50 && minuto < 52) {
    digitalWrite(val1, HIGH);
    digitalWrite(giro1, HIGH);
    digitalWrite(giro2, HIGH);

    digitalWrite(bomba, LOW);

    ciclo = ciclos[2];

  }

  if (minuto >= 52 && minuto < 53) {
    digitalWrite(val1, HIGH);
    digitalWrite(giro1, HIGH);

    digitalWrite(bomba, LOW);
    if (paso == 1) {
      digitalWrite(giro2, HIGH);
    } else if (paso == 2) {
      digitalWrite(giro2, LOW);
    }
    if (paso == 2) {
      paso = 0;
    }

    ciclo = ciclos[2];

  }

  if (minuto >= 53 && minuto < 63) {
    digitalWrite(val1, HIGH);
    digitalWrite(giro1, HIGH);

    digitalWrite(bomba, LOW);
    digitalWrite(giro2, LOW);

    ciclo = ciclos[2];

  }

  if (minuto >= 65 && minuto < 70) {
    digitalWrite(val1, HIGH);
    digitalWrite(giro1, HIGH);
    digitalWrite(bomba, HIGH);
    digitalWrite(giro2, HIGH);

    ciclo = ciclos[3];

  }
}
