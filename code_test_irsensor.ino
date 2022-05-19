//ir sensor 
const int irsensor = 34;
int ValueIrsensor = 0;

void setup() {
  Serial.begin(9600);
  pinMode(irsensor, INPUT);

}

void loop() {
  ValueIrsensor = digitalRead(irsensor);

  Serial.println(ValueIrsensor); // irsensor detect value 0 and irsensor no detect value 1
  delay(1000); // 1000 = 1 sec
}
