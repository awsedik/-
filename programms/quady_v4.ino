#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C LCD(0x27, 16, 2);
const int a = 4;

// Определение пинов для VEX моторов
#define VEX_MOTOR1_PWM 10
#define VEX_MOTOR2_PWM 11

Servo vexMotor1;
Servo vexMotor2;

int Ver[a][a];
int NeVer[a];
int i = 0;
int n = 0;
int wa = 0; // Счётчик неверных попыток
unsigned long blockTime = 300000; // Начальное время блокировки (5 минут)
const unsigned long baseBlockTime = 300000; // Базовая блокировка 5 минут
const unsigned long additionalBlockTime = 300000; // Дополнительные 5 минут

// Настройка клавиатуры
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'}, 
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad Klava = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

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

void generiruyKody() {
  for (int r = 0; r < a; r++) {
    for (int i = 0; i < a; i++) {
      Ver[r][i] = random(0, 10);
    }
  }
  for (int t = 0; t < a; t++) {
    Serial.print(t + 1);
    Serial.print(" ячейка - ");
    for (int i = 0; i < a; i++) {
      Serial.print(Ver[t][i]);
    }
    Serial.println();
  }
}

void setVexMotorSpeed(Servo &motor, int speed) {
  speed = constrain(speed, -100, 100);
  int pulseWidth;
  
  if (speed == 0) {
    pulseWidth = 1500;
  } else if (speed > 0) {
    pulseWidth = map(speed, 0, 100, 1500, 2000);
  } else {
    pulseWidth = map(speed, -100, 0, 1000, 1500);
  }
  
  motor.writeMicroseconds(pulseWidth);
}

void rotateMotor1Forward() { setVexMotorSpeed(vexMotor1, 100); }
void rotateMotor1Backward() { setVexMotorSpeed(vexMotor1, -100); }
void stopMotor1() { setVexMotorSpeed(vexMotor1, 0); }
void rotateMotor2Forward() { setVexMotorSpeed(vexMotor2, 100); }
void rotateMotor2Backward() { setVexMotorSpeed(vexMotor2, -100); }
void stopMotor2() { setVexMotorSpeed(vexMotor2, 0); }

void setup() {
  Serial.begin(9600);
  vexMotor1.attach(VEX_MOTOR1_PWM);
  vexMotor2.attach(VEX_MOTOR2_PWM);
  stopMotor1();
  stopMotor2();
  randomSeed(analogRead(0));
  generiruyKody();
  LCD.init();
  LCD.backlight();
  LCD.clear();
  vvediteParol();
}

void showBlockScreen(int minutes) {
  LCD.clear();
  LCD.setCursor(3, 0);
  LCD.print("Blokirovka");
  LCD.setCursor(2, 1);
  LCD.print(minutes);
  // Правильное склонение слова "минут"
  if (minutes % 10 == 1 && minutes % 100 != 11) {
    LCD.print(" minuta");
  } else if ((minutes % 10 >= 2 && minutes % 10 <= 4) && !(minutes % 100 >= 12 && minutes % 100 <= 14)) {
    LCD.print(" minuty");
  } else {
    LCD.print(" minut");
  }
}

void handleBlock() {
  // Вычисляем количество полных 3-попыточных циклов
  int blockCycles = (wa - 1) / 3;
  blockTime = baseBlockTime + blockCycles * additionalBlockTime;
  
  int totalMinutes = blockTime / 60000;
  showBlockScreen(totalMinutes);
  
  unsigned long startTime = millis();
  while (millis() - startTime < blockTime) {
    unsigned long remainingTime = blockTime - (millis() - startTime);
    int remainingMinutes = remainingTime / 60000;
    int remainingSeconds = (remainingTime % 60000) / 1000;
    
    LCD.setCursor(0, 0);
    LCD.print("Blokirovka      ");
    LCD.setCursor(0, 1);
    LCD.print("Ostalos: ");
    if (remainingMinutes < 10) LCD.print(" ");
    LCD.print(remainingMinutes);
    LCD.print("m ");
    if (remainingSeconds < 10) LCD.print("0");
    LCD.print(remainingSeconds);
    LCD.print("s  ");
    
    delay(200);
  }
  
  LCD.clear();
  vvediteParol();
}

void loop() {
  char simvol = Klava.getKey();

  if (simvol) {
    if (simvol == '*' && i > 0) {
      i--;
      NeVer[i] = 0;
      LCD.setCursor(0, 0);
      LCD.print("Vvedite kod:");
      LCD.setCursor(0, 1);
      for (int j = 0; j < a; j++) {
        if (j < i) LCD.print(NeVer[j]);
        else LCD.print(" ");
      }
    }
    else if (i < a && isdigit(simvol)) {
      NeVer[i] = simvol - '0';
      LCD.setCursor(0, 0);
      LCD.print("Vvedite kod:");
      LCD.setCursor(i, 1);
      LCD.print(NeVer[i]);
      i++;
    }

    if (i == a) {
      for (int q = 0; q < a; q++) {
        n = 0;
        for (int e = 0; e < a; e++) {
          if (Ver[q][e] == NeVer[e]) n++;
        }

        if (n == a) {
          LCD.clear();
          LCD.setCursor(3, 0);
          LCD.print("nazhmite #");
          LCD.setCursor(1, 1);
          LCD.print("dlya zakritiya");
          
          if (q == 0) {
            rotateMotor1Forward();
            delay(2000);
            stopMotor1();
          } else if (q == 1) {
            rotateMotor1Backward();
            delay(2000);
            stopMotor1();
          } else if (q == 2) {
            rotateMotor2Forward();
            delay(2000);
            stopMotor2();
          } else if (q == 3) {
            rotateMotor2Backward();
            delay(2000);
            stopMotor2();
          }
                    
          LCD.clear();
          wa = 0;
          blockTime = baseBlockTime;
          vvediteParol();
          i = 0;
          sbrosNeVer();
          return;
        }
      }

      // Неверный пароль
      wa++;
      
      if (wa >= 3) {
        handleBlock();
      } else {
        LCD.clear();
        LCD.setCursor(1, 0);
        LCD.print("Podozhdite 15s");
        for (int t = 15; t > 0; t--) {
          LCD.setCursor(2, 1);
          LCD.print("Ostalos: ");
          LCD.print(t);
          LCD.print("s  ");
          delay(1000);
        }
      }

      i = 0;
      sbrosNeVer();
      vvediteParol();
    }
  }
}
