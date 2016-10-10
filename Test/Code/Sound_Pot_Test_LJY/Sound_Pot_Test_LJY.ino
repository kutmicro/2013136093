//2013136093 이재영 2016_2학기 마이크로프로세서

#define N_PIXELS  8                              // LED 핀 갯수 설정
const int sampleWindow = 50;                     // (50 mS = 20Hz) 샘플링 시간 결정 50ms동안 센서 값을 읽음
unsigned int sample;
const int led[] = {4,5,6,7,8,9,10,11};  
int pot = A1;                                    //가변저항 입력

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
   unsigned int sensitive = analogRead(pot);       // 가변저항의 값을 감도로 받음.
   unsigned long startMillis= millis();            // 시작 시간을 잰다. millis함수는 아두이노 시작 시간을 측정
   unsigned int peakToPeak = 0;                    // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   while (millis() - startMillis < sampleWindow)   // 50ms 지정된 시간만큼 데이터 샘플링
   {
      sample = analogRead(0);
      if (sample < 1024)                           // 1024 이상이면 버림
      {
         if (sample > signalMax)                   // 최댓값 갱신
         {
            signalMax = sample;                    // 최댓값이 기존 최댓값보다 높으면 갱신
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                    // 최소값 갱신
         }
      }
   }
   peakToPeak = signalMax - signalMin;             // max - min = peak-peak 진폭
  
   Serial.println(peakToPeak);                    //시리얼 출력
   Serial.println(sensitive); 
  int height = peakToPeak/(sensitive/10);          // 높이만큼 led출력해준다. 가변 저항으로 민감도 변경
  
  for(int i=0; i<N_PIXELS; i++)
  {
    if(i < height) 
    { digitalWrite(led[i], HIGH); }
    else 
    { digitalWrite(led[i], LOW);  }
  }
}
