#include <NewPing.h>

#define MASK 5

double data[Mask] = {0.0,};

NewPing sonar(8, 9, 200);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("MoveAvg,Sonar");

}

double MoveFilter(int mask)
{
  double sum = 0.0, movefilter = 0.0;
  for (int i = 0; i < mask; i++)
  {
    sum += data[i];
  }
  movefilter = sum / (double)mask;

  return movefilter;
}

void loop() {
  // put your main code here, to run repeatedly:
  double movefilter = 0.0;
  int i =0;
  
  for(i =0; i<Mask; i++)
  {
    data[i] =  sonar.ping_cm();
  }
  movefilter = MoveFilter(Mask);
  Serial.print(movefilter);
  Serial.print(",");
  Serial.println(sonar.ping_cm());


  if(i >= Mask)
  {
    i =0;
  }
  delay(200);

}
