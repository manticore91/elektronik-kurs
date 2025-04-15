#define ADC_Pin 0
#define LED_PIN 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int ADCvalue = analogRead(ADC_Pin);
  Serial.println("ADC: ");
  Serial.print(ADCvalue);

}
