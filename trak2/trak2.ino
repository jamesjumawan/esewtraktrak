#include <NewPing.h>

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
    HumanFollower(int mp0, int mp1, int mp2, int mp3, int mp4, int mp5, int mp6, int mp7,
                  int ep, int tp, int li, int ri){
      motorPin[0] = mp0;
      motorPin[1] = mp1;
      motorPin[2] = mp2;
      motorPin[3] = mp3;
      motorPin[4] = mp4;
      motorPin[5] = mp5;
      motorPin[6] = mp6;
      motorPin[7] = mp7;

      echoPin = ep;
      trigPin = tp;
      leftInfra = li;
      rightInfra = ri;
      
      sonar = NewPing_Customized(trigPin, echoPin, 100);

      for(int i = 0; i < 8; i++){
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
      for(int i = 0; i < 8; i++){
        if(i%2 == 0){
          digitalWrite(motorPin[i], HIGH);
        }
        else{
          digitalWrite(motorPin[i], LOW);
        }
      }
    }

    void turnLeft(){
      for(int i = 0; i < 8; i++){
        if(i == 2 || i == 6){
          digitalWrite(motorPin[i], HIGH);
        }
        else{
          digitalWrite(motorPin[i], LOW);
        }
      }
    }

    void turnRight(){
      for(int i = 0; i < 8; i++){
        if(i == 0 || i == 4){
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

HumanFollower hf(5, 6, 9, 10, 4, 7, 13, 8, 2, 3, 11, 12);  

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(50);
  
  unsigned int distance = hf.takeDistance();

  Serial.println(distance);

  int rightVal = hf.readRight();
  int leftVal = hf.readLeft();

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
