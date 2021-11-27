int V1 = 2;
int R1 = 4;
int V2 = 6;
int R2 = 8;
int V3 = 3;
int R3 = 5;
int V4 = 7;
int R4 = 9;

int S1 = A3;
int S2 = A2;
int S3 = A5;
int S4 = A4;
int led1,led2,led3,led4;
void setup() {
  pinMode(V1, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(V2, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(V3, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(V4, OUTPUT);
  pinMode(R4, OUTPUT);
  
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  // se escribe el valor que este en el puerto
  led1 = digitalRead(S1);
  led2 = digitalRead(S2);
  led3 = digitalRead(S3);
  led4 =digitalRead(S4);
  
  digitalWrite(V1, ~led1&1);
  digitalWrite(R1, led1);

  digitalWrite(V2, ~led2&1);
  digitalWrite(R2, led2);

  digitalWrite(V3, ~led3&1);
  digitalWrite(R3, led3);

  digitalWrite(V4, ~led4&1);
  digitalWrite(R4, led4);

  // mandando por uart

  Serial.print(led1);
  Serial.print(",");
  Serial.print(led2);
  Serial.print(",");
  Serial.print(led3);
  Serial.print(",");
  Serial.println(led4);

  delay(250);

  
}
