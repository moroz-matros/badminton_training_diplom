#define SIG_OUTPUT A1
#define S3_OUTPUT 13
#define S2_OUTPUT 12
#define S1_OUTPUT 11
#define S0_OUTPUT 10
#define EN_OUTPUT 4

#define SIG_INPUT A0
#define S3_INPUT 9
#define S2_INPUT 8
#define S1_INPUT 7
#define S0_INPUT 6
#define EN_INPUT 5

#define DIODE_GREEN 3
#define DIODE_RED 2

const int cols = 15;
const int rows = 16;
const int mess = 25;
int line = 6;

int n = 0;
int delta_t = 300;
bool massiv[rows][cols];
int massiv2[rows][cols];

void clear_massiv() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      massiv[i][j] = false;
    }
  }
}

void clear_massiv2() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      massiv2[i][j] = 0;
    }
  }
}

const boolean channels [16][4] {
  {0, 0, 0, 0}, //channel 0
  {1, 0, 0, 0}, //channel 1
  {0, 1, 0, 0}, //channel 2
  {1, 1, 0, 0}, //channel 3
  {0, 0, 1, 0}, //channel 4
  {1, 0, 1, 0}, //channel 5
  {0, 1, 1, 0}, //channel 6
  {1, 1, 1, 0}, //channel 7
  {0, 0, 0, 1}, //channel 8
  {1, 0, 0, 1}, //channel 9
  {0, 1, 0, 1}, //channel 10
  {1, 1, 0, 1}, //channel 11
  {0, 0, 1, 1}, //channel 12
  {1, 0, 1, 1}, //channel 13
  {0, 1, 1, 1}, //channel 14
  {1, 1, 1, 1} //channel 15
};



void setup() {
  Serial.begin(115200);
  pinMode(S0_OUTPUT, OUTPUT);
  pinMode(S1_OUTPUT, OUTPUT);
  pinMode(S2_OUTPUT, OUTPUT);
  pinMode(S3_OUTPUT, OUTPUT);

  pinMode(S0_INPUT, OUTPUT);
  pinMode(S1_INPUT, OUTPUT);
  pinMode(S2_INPUT, OUTPUT);
  pinMode(S3_INPUT, OUTPUT);

  pinMode(EN_OUTPUT, OUTPUT);
  pinMode(EN_INPUT, OUTPUT);

  pinMode(SIG_OUTPUT, OUTPUT);
  pinMode(SIG_INPUT, INPUT);

  pinMode(DIODE_RED, OUTPUT);
  pinMode(DIODE_GREEN, OUTPUT);

  n = noise();
  clear_massiv();
}

void loop() {
  //необходимо пройтись по всем портам S0..S3 выхода и подать на C0..C15 платы питания высокий сигнал
  for (int i = 0; i < rows; i++) {
    connect_line_rows(i);
    for (int j = 0; j < cols; j++) {
      connect_line_cols(j);
      int val = analogRead(SIG_INPUT);
      massiv2[i][j] = val;
      if (val > n) {
        massiv[i][j] = true;
      }
    }
  }
  bool flag = false;
  for (int j = 0; j <= line; j++) {
    for (int i = 0; i < rows; i++) {
      if (massiv[i][j] == true) {
        Serial.print("in ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(j);
        Serial.print(" ");
        Serial.println(massiv2[i][j]);
        flag = true;
        break;
      }
    }
  }

  if (flag) {
    clear_massiv();
    clear_massiv2();
    delay(200);
    return;
  }
  for (int i = 0; i < rows; i++) {
    for (int j = line +1; j < cols; j++) {
      if ((j != line) && (massiv[i][j] == true)) {
        Serial.print("out ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(j);
        Serial.print(" ");
        Serial.println(massiv2[i][j]);
        
      }
    }
  }
  clear_massiv();
  clear_massiv2();
}

int noise() {
  int m = 0;
  int val = 0;
  Serial.println("Calibrate...");
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < rows; j++) {
      connect_line_rows(j);
      for (int k = 0; k < cols; k++) {
        connect_line_cols(k);
        val = analogRead(SIG_INPUT);
        if (val > m) {
          m = val;
        }
      }

    }
  }
  Serial.println("Done");
  return m + mess;
}

void connect_line_rows(int i) {
  //запрещаем работу
  digitalWrite(EN_OUTPUT, HIGH);

  digitalWrite(S0_OUTPUT, channels[i][0]);
  digitalWrite(S1_OUTPUT, channels[i][1]);
  digitalWrite(S2_OUTPUT, channels[i][2]);
  digitalWrite(S3_OUTPUT, channels[i][3]);
  digitalWrite(EN_OUTPUT, LOW);
  digitalWrite(SIG_OUTPUT, HIGH);
}

void connect_line_cols(int i) {
  //запрещаем работу
  digitalWrite(EN_INPUT, HIGH);

  digitalWrite(S0_INPUT, channels[i][0]);
  digitalWrite(S1_INPUT, channels[i][1]);
  digitalWrite(S2_INPUT, channels[i][2]);
  digitalWrite(S3_INPUT, channels[i][3]);
  digitalWrite(EN_INPUT, LOW);
}
