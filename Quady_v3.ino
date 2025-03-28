#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x38, 16, 2); // Инициализация LCD дисплея с адресом 0x27 и размером 16x2
const int a = 4; // Размерность массивов

// Определение пинов для шаговых моторов
#define MOTOR1_IN1 10
#define MOTOR1_IN2 11
#define MOTOR1_IN3 12
#define MOTOR1_IN4 13

#define MOTOR2_IN1 A0
#define MOTOR2_IN2 A1
#define MOTOR2_IN3 A2
#define MOTOR2_IN4 A3

int dl = 5; // время задержки между импульсами

int Ver[a][a] = {}; // Двумерный массив для хранения случайных чисел
int NeVer[a] = {}; // Массив для хранения введенных пользователем чисел
int test[a] = {0, 0, 0, 0}; // Массив для хранения всегда работающего кода
int i = 0; // Счетчик для массива NeVer
int n = 0; // Счетчик для проверки совпадений
int wa = 0; // Переменная для подсчета неверных попыток
int test1 = 3; // Переменная для проверки всегда работающего кода

// Настройка клавиатуры
const byte ROWS = 4; // Количество строк на клавиатуре
const byte COLS = 4; // Количество столбцов на клавиатуре
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; // Пины для подключения строк клавиатуры
byte colPins[COLS] = {6, 7, 8, 9}; // Пины для подключения столбцов клавиатуры
Keypad Klava = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); // Инициализация клавиатуры

void setup() {
  Serial.begin(9600); // Запуск последовательного соединения для отладки

  // Настройка пинов для шаговых моторов
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR1_IN3, OUTPUT);
  pinMode(MOTOR1_IN4, OUTPUT);
  
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(MOTOR2_IN3, OUTPUT);
  pinMode(MOTOR2_IN4, OUTPUT);

  // Заполнение массива Ver случайными числами
  randomSeed(analogRead(A3)); // Инициализация генератора случайных чисел
  for (int r = 0; r < a; r++) {
    for (int i = 0; i < a; i++) {
      Ver[r][i] = random(0, 10); // Заполнение массива случайными числами от 0 до 9
    }
  }

  // Вывод содержимого массива Ver в монитор порта для отладки
  for (int t = 0; t < a; t++) {
    Serial.print(t + 1);
    Serial.print(" ячейка - ");
    for (int i = 0; i < a; i++) {
      Serial.print(Ver[t][i]);
    }
    Serial.println();
  }

  LCD.init(); // Инициализация LCD дисплея
  LCD.backlight(); // Включение подсветки дисплея
  LCD.clear();
  vvediteParol();
}

// Функция для вращения шагового мотора 1 (90 градусов по часовой стрелке)
void rotateMotor1CW() {
  for(int j = 0; j < 128; j++) { // 128 шагов примерно соответствуют 90 градусам
    digitalWrite(MOTOR1_IN1, HIGH); 
    digitalWrite(MOTOR1_IN2, LOW); 
    digitalWrite(MOTOR1_IN3, LOW); 
    digitalWrite(MOTOR1_IN4, HIGH);
    delay(dl);

    digitalWrite(MOTOR1_IN1, HIGH); 
    digitalWrite(MOTOR1_IN2, HIGH); 
    digitalWrite(MOTOR1_IN3, LOW); 
    digitalWrite(MOTOR1_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR1_IN1, LOW); 
    digitalWrite(MOTOR1_IN2, HIGH); 
    digitalWrite(MOTOR1_IN3, HIGH); 
    digitalWrite(MOTOR1_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR1_IN1, LOW); 
    digitalWrite(MOTOR1_IN2, LOW); 
    digitalWrite(MOTOR1_IN3, HIGH); 
    digitalWrite(MOTOR1_IN4, HIGH);
    delay(dl);
  }
}

// Функция для вращения шагового мотора 1 (90 градусов против часовой стрелки)
void rotateMotor1CCW() {
  for(int j = 0; j < 128; j++) { // 128 шагов примерно соответствуют 90 градусам
    digitalWrite(MOTOR1_IN1, LOW); 
    digitalWrite(MOTOR1_IN2, LOW); 
    digitalWrite(MOTOR1_IN3, HIGH); 
    digitalWrite(MOTOR1_IN4, HIGH);
    delay(dl);

    digitalWrite(MOTOR1_IN1, LOW); 
    digitalWrite(MOTOR1_IN2, HIGH); 
    digitalWrite(MOTOR1_IN3, HIGH); 
    digitalWrite(MOTOR1_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR1_IN1, HIGH); 
    digitalWrite(MOTOR1_IN2, HIGH); 
    digitalWrite(MOTOR1_IN3, LOW); 
    digitalWrite(MOTOR1_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR1_IN1, HIGH); 
    digitalWrite(MOTOR1_IN2, LOW); 
    digitalWrite(MOTOR1_IN3, LOW); 
    digitalWrite(MOTOR1_IN4, HIGH);
    delay(dl);
  }
}

// Функции для управления вторым мотором (аналогично)
void rotateMotor2CW() {
  for(int j = 0; j < 128; j++) {
    digitalWrite(MOTOR2_IN1, HIGH); 
    digitalWrite(MOTOR2_IN2, LOW); 
    digitalWrite(MOTOR2_IN3, LOW); 
    digitalWrite(MOTOR2_IN4, HIGH);
    delay(dl);

    digitalWrite(MOTOR2_IN1, HIGH); 
    digitalWrite(MOTOR2_IN2, HIGH); 
    digitalWrite(MOTOR2_IN3, LOW); 
    digitalWrite(MOTOR2_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR2_IN1, LOW); 
    digitalWrite(MOTOR2_IN2, HIGH); 
    digitalWrite(MOTOR2_IN3, HIGH); 
    digitalWrite(MOTOR2_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR2_IN1, LOW); 
    digitalWrite(MOTOR2_IN2, LOW); 
    digitalWrite(MOTOR2_IN3, HIGH); 
    digitalWrite(MOTOR2_IN4, HIGH);
    delay(dl);
  }
}

void rotateMotor2CCW() {
  for(int j = 0; j < 128; j++) {
    digitalWrite(MOTOR2_IN1, LOW); 
    digitalWrite(MOTOR2_IN2, LOW); 
    digitalWrite(MOTOR2_IN3, HIGH); 
    digitalWrite(MOTOR2_IN4, HIGH);
    delay(dl);

    digitalWrite(MOTOR2_IN1, LOW); 
    digitalWrite(MOTOR2_IN2, HIGH); 
    digitalWrite(MOTOR2_IN3, HIGH); 
    digitalWrite(MOTOR2_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR2_IN1, HIGH); 
    digitalWrite(MOTOR2_IN2, HIGH); 
    digitalWrite(MOTOR2_IN3, LOW); 
    digitalWrite(MOTOR2_IN4, LOW);
    delay(dl);

    digitalWrite(MOTOR2_IN1, HIGH); 
    digitalWrite(MOTOR2_IN2, LOW); 
    digitalWrite(MOTOR2_IN3, LOW); 
    digitalWrite(MOTOR2_IN4, HIGH);
    delay(dl);
  }
}

void loop() {
  char simvol = Klava.getKey(); // Считывание нажатой клавиши

  if (simvol) { // Если клавиша нажата
    if (simvol == '*' && i > 0) { // Если нажата '*' и есть введенные символы
      i--; // Уменьшаем счетчик
      NeVer[i] = 0; // Сбрасываем значение
      LCD.setCursor(0, 0);
      LCD.print("Vvedite kod:");
      LCD.setCursor(0, 1);
      for (int j = 0; j < a; j++) {
        if (j < i) {
          LCD.print(NeVer[j]); // Выводим введенные символы
        } else {
          LCD.print(" "); // Очищаем оставшиеся позиции
        }
      }
    }
    else if (i < a && isdigit(simvol)) { // Если введена цифра и есть место в массиве
      NeVer[i] = simvol - '0'; // Преобразование символа в число
      LCD.setCursor(0, 0);
      LCD.print("Vvedite kod:");
      LCD.setCursor(i, 1);
      LCD.print(NeVer[i]); // Отображение введенной цифры
      i++; // Увеличение счетчика введенных символов
    }

    if (i == a) { // Если введено 4 символа
      // Проверка совпадения введенного кода с одним из кодов в массиве Ver
      for (int i = 0; i < 3; i++){
        for (int b = 0; b < a; b++) {
          if (test[b] == NeVer[b]) {
            test1++; // Увеличение счетчика совпадений
          }
        }
      }
      if (test1 == 3){
         rotateMotor1CW(); // Вращение первого мотора по часовой стрелке
      }
      LCD.clear();
          LCD.setCursor(3, 0);
          LCD.print("nazhmite #");
          LCD.setCursor(1, 1);
          LCD.print("dlya zakritiya");
      if (simvol == '#') {
              // Возвращаем мотор в исходное положение
              if (q == 0) {
                rotateMotor1CCW(); // Возврат первого мотора
              }
              break;
      }
      for (int q = 0; q < a; q++) {
        n = 0; // Сброс счетчика совпадений перед каждой проверкой
        for (int e = 0; e < a; e++) {
          if (Ver[q][e] == NeVer[e]) {
            n++; // Увеличение счетчика совпадений
          }
        }

        if (n == a) { // Если все символы совпали
          LCD.clear();
          LCD.setCursor(3, 0);
          LCD.print("nazhmite #");
          LCD.setCursor(1, 1);
          LCD.print("dlya zakritiya");
          
          // Выполняем соответствующее действие для каждого кода
          if (q == 0) {
            rotateMotor1CW(); // Вращение первого мотора по часовой стрелке
          } else if (q == 1) {
            rotateMotor1CCW(); // Вращение первого мотора против часовой стрелки
          } else if (q == 2) {
            rotateMotor2CW(); // Вращение второго мотора по часовой стрелке
          } else if (q == 3) {
            rotateMotor2CCW(); // Вращение второго мотора против часовой стрелки
          }
          
          // Ожидание нажатия клавиши '#' для возврата
          while (true) {
            char simvol = Klava.getKey();
            if (simvol == '#') {
              // Возвращаем мотор в исходное положение
              if (q == 0) {
                rotateMotor1CCW(); // Возврат первого мотора
              } else if (q == 1) {
                rotateMotor1CW(); // Возврат первого мотора
              } else if (q == 2) {
                rotateMotor2CCW(); // Возврат второго мотора
              } else if (q == 3) {
                rotateMotor2CW(); // Возврат второго мотора
              }
              break;
            }
          }
          
          LCD.clear();
          wa = 0; // Сброс счетчика неверных попыток при успешном вводе
          break; // Выход из цикла проверки
        }
      }

      if (n != a) { // Если код не совпал
        wa++;
        if (wa >= 3) { // Блокировка после 3 неудачных попыток
          LCD.clear();
          LCD.setCursor(3, 0);
          LCD.print("Blokirovka");
          LCD.setCursor(4, 1);
          LCD.print("3 minuty");
          delay(180000); // Блокировка на 3 минуты (180000 миллисекунд)
          wa = 0; // Сброс счетчика неверных попыток
        } else {
          LCD.clear();
          LCD.setCursor(1, 0);
          LCD.print("Podozhdite 15s");
          for (int t = 15; t > 0; t--) { // Обратный отсчет от 15 до 1
            LCD.setCursor(2, 1);
            LCD.print("Ostalos: ");
            LCD.print(t);
            LCD.print("s  ");
            delay(1000); // Задержка на 1 секунду
          }
        }
      }

      // Сброс и подготовка к новому вводу
      i = 0;
      sbrosNeVer();
      vvediteParol();
    }
  }
}

void vvediteParol() {
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Vvedite kod:");
  LCD.setCursor(0, 1);
  for (int j = 0; j < a; j++) {
    LCD.print(" ");
  }
}

void sbrosNeVer() {
  for (int i = 0; i < a; i++) {
    NeVer[i] = 0;
  }
}
