#define N_PIXELS  8 // LED 핀 갯수 설정
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
const int led[] = {4,5,6,7,8,9,10,11};  

void setup() 
{
   Serial.begin(9600);

   for(int i=0; i<N_PIXELS; i++)
  {
    pinMode(led[i],OUTPUT); 
  }
}
 
 
void loop() 
{

   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // 1024 이상이면 버림
      {
         if (sample > signalMax) // 최댓값 갱신
         {
            signalMax = sample;  // 최댓값이 기존 최댓값보다 높으면 갱신
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // 최소값 갱신
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak 진폭
  /* double volts = (peakToPeak * 3.3)  1024;  // 볼트로 변환할 경우 식  */
   Serial.println(peakToPeak); //시리얼 출력
   
  int height = peakToPeak/20;  // 높이만큼 led출력해준다. 나누기값 조정으로 민감도 조정 가능(가변 저항으로 기능 추가 예정)
  for(int i=0; i<N_PIXELS; i++)
  {
    if(i < height) 
    {
      digitalWrite(led[i], HIGH);
    }
    else 
    {
      digitalWrite(led[i], LOW); 
    }
  }
}
