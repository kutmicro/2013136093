//2013136093 이재영 2016_2학기 마이크로프로세서
//https://circuits.io/circuits/2997022-the-unnamed-circuit/edit#breadboard

const int N_LEDLINE = 6;                       // 레지스터 갯수 설정
const int sampleWindow = 50;                     // (50 mS = 20Hz) 샘플링 시간 결정 50ms동안 센서 값을 읽음
unsigned int sample;
//테스트용 led    const int led[] = {4,5,6,7,8,9,10,11};  
int pot = A1;                                    //가변저항 입력
int sound = A0;                                  //오디오센서 입력

// 시프트 레지스터 설정
int latchPin = 2;
int clockPin = 3;
const int LedData[] = {4,5,6,7,8,9};


void setup() 
{
  Serial.begin(9600);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT); 
  for(int i=0; i<N_LEDLINE; i++)
  {
    pinMode(LedData[i],OUTPUT); 
  }
}
 
 
void loop() 
{
   unsigned int sensitive = analogRead(pot);       // 가변저항의 값을 감도로 받음.
   unsigned long startMillis= millis();            // 시작 시간을 잰다. millis() 함수가 현재시간(아두이노가 시작된 후부터 경과된 시간)을 밀리초 단위로(1/1000 초) 리턴 
   unsigned int peakToPeak = 0;                    // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   while (millis() - startMillis < sampleWindow)   // 50ms 지정된 시간만큼 데이터 샘플링
   {
      sample = analogRead(sound);
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
   peakToPeak = signalMax - signalMin;             // max - min = peak-peak 진폭 = 소리크기
  
   Serial.println(peakToPeak/sensitive);           //시리얼 출력
  int height = peakToPeak/(sensitive/10);          // 높이만큼 led출력해준다. 가변 저항으로 민감도 변경
  
  for(int i=0; i<N_LEDLINE; i++)
  {
    if(i < height) 
    { digitalWrite(LedData[i], HIGH); }
    else 
    { digitalWrite(LedData[i], LOW);  }
  }
}
