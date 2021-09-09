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

int n = 0;

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
  Serial.begin(9600);
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
  /*
    for (int i = 0; i < 15; i++) {
      digitalWrite(EN_OUTPUT, HIGH);
      digitalWrite(EN_INPUT, HIGH);
      digitalWrite(S0_OUTPUT, channels[i][0]);
      digitalWrite(S1_OUTPUT, channels[i][1]);
      digitalWrite(S2_OUTPUT, channels[i][2]);
      digitalWrite(S3_OUTPUT, channels[i][3]);
      digitalWrite(SIG_OUTPUT, HIGH);

      digitalWrite(EN_OUTPUT, LOW);
    }
  */
  n = noise();
}

void loop() {
  //необходимо пройтись по всем портам S0..S3 выхода и подать на C0..C11 (C16) платы питания высокий сигнал
  for (int i = 0; i < 15; i++) {
    connect_line_cols(i);
    for (int j = 0; j < 16; j++) {
      connect_line_rows(j);
      int val = analogRead(SIG_INPUT);
      if (val > n) {
        int diode = DIODE_GREEN;
        digitalWrite(diode, HIGH);
        while (val > n) {
          Serial.print(val);
          Serial.print(" ");
          Serial.println(n);
          val = analogRead(SIG_INPUT);
        }
        digitalWrite(diode, LOW);
      }
    }

  }
}

int noise() {
  int m = 0;
  int val = 0;
  Serial.println("Calibrate...");
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < 15; j++) {
      connect_line_cols(j);
      for (int k = 0; k < 16; k++) {
        connect_line_rows(k);
        val = analogRead(SIG_INPUT);
        if (val > m) {
          m = val;
        }
      }

    }
  }
  Serial.println("Done");
  return m + 35;
}

void connect_line_cols(int i) {
  //запрещаем работу
  digitalWrite(EN_OUTPUT, HIGH);

  digitalWrite(S0_OUTPUT, channels[i][0]);
  digitalWrite(S1_OUTPUT, channels[i][1]);
  digitalWrite(S2_OUTPUT, channels[i][2]);
  digitalWrite(S3_OUTPUT, channels[i][3]);
  digitalWrite(EN_OUTPUT, LOW);
  digitalWrite(SIG_OUTPUT, HIGH);
}

void connect_line_rows(int i) {
  //запрещаем работу
  digitalWrite(EN_INPUT, HIGH);

  digitalWrite(S0_INPUT, channels[i][0]);
  digitalWrite(S1_INPUT, channels[i][1]);
  digitalWrite(S2_INPUT, channels[i][2]);
  digitalWrite(S3_INPUT, channels[i][3]);
  digitalWrite(EN_INPUT, LOW);
}
