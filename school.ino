// библиотеки
#include <OneButton.h>
#include <TimeLib.h>

//переменные
#define rele 5 // пин реле
OneButton button(3, true); 

int w[2][16] = {{8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 14},
                 {0, 40, 50, 30, 50, 30, 50, 30, 40, 20, 30, 10, 20, 0, 10, 50}};
int s[2][8] = {{8, 8, 8, 9, 9, 10, 10, 11},
             {0, 40, 50, 30, 40, 20, 30, 10}};
boolean f = 0;
volatile boolean flag = 1, d = 0;
int i = 0;

//Настройка
void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, 0); //выключаем светодиод
  pinMode(rele, OUTPUT);
  digitalWrite(rele, 0);
  setTime(7, 30, 00, 5, 2, 18);
  button.attachClick(oneclick);
  button.attachDoubleClick(doubleclick);
  button.attachLongPressStart(longPressStart);
  button.attachLongPressStop(longPressStop);
  Serial.begin(9600);
  Serial.println("starting...");
}

//Основной цикл
void loop() {
  button.tick();
  sync();
  if (d) debug();
  if (flag) { //проверка режима
    if (2 <= weekday() <= 6 && hour() == w[0][i] && minute() == w[1][i] && second() == 0) {
      digitalWrite(rele, 1);
      Serial.println("dzin");
      f = 1;
    } else if (weekday() == 7 && hour() == s[0][i] && minute() == s[1][i] && second() == 0) {
        digitalWrite(rele, 1);
        Serial.println("dzin");
        f = 1;
    } else if (f) {
       digitalWrite(rele, 0);
       i++;
       f = 0;
       Serial.println(i);
     } else if (i == 16 && 2 <= weekday() <= 6) {
        i = 0;
     } else if (i == 8 && weekday() == 7) {
      i = 0;
     }
  }
}

//Синхронизация
void sync() {
  if (Serial.available()) {
    d = Serial.parseInt();
    int a[5];
    a[0] = Serial.parseInt();
    a[1] = Serial.parseInt();
    a[2] = Serial.parseInt();
    a[3] = Serial.parseInt();
    a[4] = Serial.parseInt();
    a[5] = Serial.parseInt();
    setTime(a[0], a[1], a[2], a[3], a[4], a[5]);
    Serial.println(second());
  }
}

//Переключение режима
void oneclick() {
  flag = !flag;
  Serial.print("oneclick= ");
  Serial.println(flag);
}

//Три звонка
void doubleclick() {
  for (int i = 0; i < 3; i++) {
    Serial.println("drele = 1");
    digitalWrite(rele, 1);
    delay(1000);
    Serial.println("drele = 0");
    digitalWrite(rele, 0);
    delay(500);
  }
}

//Звонок вкл
void longPressStart() {
  Serial.println("Button longPress start");
  digitalWrite(rele, 1);
}

//Звонок выкл
void longPressStop() {
  Serial.println("Button longPress stop");
  digitalWrite(rele, 0);
}

//
void debug() {
  Serial.print(hour());
  Serial.print(" ");
  Serial.print(minute());
  Serial.print(" ");
  Serial.print(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(weekday());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.println(year());
}

