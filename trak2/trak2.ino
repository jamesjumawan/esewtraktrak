#include <NewPing.h>
#include <HX711_ADC.h>

class NewPing_Customized : public NewPing{
  public:
    
    NewPing_Customized() : NewPing(0,0,0){
    }

    NewPing_Customized(int a, int b, int c) : NewPing(a, b, c){
    }
    
};

class HumanFollower{
  private:
    NewPing_Customized sonar;
    int motorPin[8],
        echoPin, trigPin,
        leftInfra, rightInfra;
    

  public:
    HumanFollower(int mp0, int mp1, int mp2, int mp3, int ep, int tp, int li, int ri){
      motorPin[0] = mp0;
      motorPin[1] = mp1;
      motorPin[2] = mp2;
      motorPin[3] = mp3;

      echoPin = ep;
      trigPin = tp;
      leftInfra = li;
      rightInfra = ri;
      
      sonar = NewPing_Customized(trigPin, echoPin, 100);

      for(int i = 0; i < 4; i++){
        pinMode(motorPin[i], OUTPUT);
      }
      pinMode(leftInfra, INPUT);
      pinMode(rightInfra, INPUT);
    }

    unsigned int takeDistance(){
      return sonar.ping_cm();
    }

    int readRight(){
      return digitalRead(rightInfra);
    }
    
    int readLeft(){
      return digitalRead(leftInfra);
    }

    void moveForward(){
      for(int i = 0; i < 4; i++){
        if(i%2 == 0){
          digitalWrite(motorPin[i], HIGH);
        }
        else{
          digitalWrite(motorPin[i], LOW);
        }
      }
    }

    void turnLeft(){
      for(int i = 0; i < 4; i++){
        if(i == 0 || i == 3){
          digitalWrite(motorPin[i], HIGH);
        }
        else{
          digitalWrite(motorPin[i], LOW);
        }
      }
    }

    void turnRight(){
      for(int i = 0; i < 4; i++){
        if(i == 2 || i == 1){
          digitalWrite(motorPin[i], HIGH);
        }
        else{
          digitalWrite(motorPin[i], LOW);
        }
      }
    }

    void brakes(){
      for(int i = 0; i < 8; i++){
        digitalWrite(motorPin[i], LOW);
      }
    }

    void test(){
      Serial.println();
    }
};

HumanFollower hf(4, 6, 5, 7, 2, 3, 12, 13);

  
const int HX711_dout = 8; //mcu > HX711 dout pin
const int HX711_sck = 9; //mcu > HX711 sck pin


HX711_ADC LoadCell(HX711_dout, HX711_sck);

void setup() {
  Serial.begin(9600); 

  pinMode(HX711_dout, OUTPUT);
  pinMode(HX711_sck, INPUT);
  
  LoadCell.begin();
  
  unsigned long stabilizingtime = 2000;
  boolean _tare = false;

  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(456.58); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
}

void loop() {
  delay(1000);
  
  static boolean newDataReady = 0;
  
  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {
    float i = LoadCell.getData();
    Serial.print("Load_cell output val: ");
    Serial.println(i - 18009.15);
    newDataReady = 0;
  }
  
  unsigned int distance = hf.takeDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  int rightVal = hf.readRight();
  int leftVal = hf.readLeft();

  Serial.print("Right Infrared: ");
  Serial.println(rightVal ? "None" : "Object Detected");
  Serial.print("Left Infrared: ");
  Serial.println(leftVal ? "None" : "Object Detected");
  Serial.println();
  Serial.println();
  Serial.println();

  if(rightVal == 1 && leftVal == 1 && distance <= 50 && distance >= 5)
  {
    hf.moveForward();
  }
  else if(rightVal == 0 && leftVal == 1)
  {
    hf.turnRight();
  }
  else if(rightVal == 1 && leftVal == 0)
  {
    hf.turnLeft();
  }
  else
  {
    hf.brakes();
  }
  
}
