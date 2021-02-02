#include "RunningMedian.h"

RunningMedian samples = RunningMedian(5);

long count = 0;
int Ldr_Pin = 34;
int mediaanLDR = 0;



void setup()
{
  Serial.begin(115200);
  Serial.print(F("Running Median Version: "));
  Serial.println(RUNNING_MEDIAN_VERSION);
}

void loop()
{
  mediaan_LDR();
  delay(1000);
  Serial.println(mediaanLDR);
}

void mediaan_LDR()
{
  if (count == 5){ 
    Serial.println(F("\nmsec \tAnR \tSize \tCnt \tLow \tAvg \tAvg(3) \tMed \tHigh"));
    count = 0;
    mediaanLDR = samples.getMedian();
    samples.clear();
    };
  count++;

  long x = (analogRead(Ldr_Pin)/4095.0)*100;

  samples.add(x);

  float l = samples.getLowest();
  float m = samples.getMedian();
  float a = samples.getAverage();
  float a3 = samples.getAverage(3);
  float h = samples.getHighest();
  int s = samples.getSize();
  int c = samples.getCount();

  Serial.print(millis());
  Serial.print('\t');
  Serial.print(x);
  Serial.print('\t');
  Serial.print(s);
  Serial.print('\t');
  Serial.print(c);
  Serial.print('\t');
  Serial.print(l);
  Serial.print('\t');
  Serial.print(a, 2);
  Serial.print('\t');
  Serial.print(a3, 2);
  Serial.print('\t');
  Serial.print(m);
  Serial.print('\t');
  Serial.println(h);
  delay(100);
}
