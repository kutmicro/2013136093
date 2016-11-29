//2013136093 이재영 2016_2학기 마이크로프로세서

//타이머 인터럽트 사용을 위한 라이브러리.
//타이머 인터럽트는 LED점멸에 사용할 예정
#include <MsTimer2.h>

const int N_LEDLINE = 6;                         // 레지스터 갯수 설정
const int N_HEIGHT = 6;                          // 높이 설정
const int sampleWindow = 50;                     // (50 mS = 20Hz) 샘플링 시간 결정 50ms동안 센서 값을 읽음
unsigned int sample;
int pot = A1;                                    //가변저항 입력
int sound = A0;                                  //오디오센서 입력

// 시프트 레지스터 설정
int latchPin = 2;
int clockPin = 3;
const int LedData[] = {4,5,6,7,8,9};
//출력할 시프트 데이터 배열.
uint8_t shiftData[] = { 00000000, 00000000, 00000000, 00000000, 00000000, 00000000, 00000000, 00000000 }; 

//타이머 인터럽트에 사용할 변수 선언
static unsigned int TimerInt = 0;    //실질적으로 증가하는 시간
static unsigned int IsLedOn = 0;    //증가하는 시간을 6으로 mod연산한 결과를 저장.

//LED라인 6개의 전원 여부를 결정할 BOOL변수 6개///필요없을수도있음
bool LEDON[] = { false, false, false, false, false, false };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//타이머에서 지정된 시간이 지나면 실행하게 하는 인터럽트 함수
//타이머에서 계속 일정시간마다 TimerInt변수를 증가시킨다.
//증가하는 변수를 %6연산을 해서 IsLedOn변수에 저장.
//이 변수는 0~5의 값을 가지게 되는데 각각의 숫자로 바뀔때마다 해당하는 라인의 bool배열을 true로 만들어 줌.
//항상 실행할 때 모든 bool배열을 false로 초기화 하기 때문에 한 번에 여러 줄이 on되는 경우는 없음
//인터럽트로 LED의 깜빡임을 구현
void timerISR()
{ 
  TimerInt++; 
  IsLedOn = TimerInt%6;
  for(int i = 0; i<6 ; i++) 
  { 
    //처음에 Led를 전부 다 꺼줌
    LEDON[i] = false;
    shiftOut(LedData[i],clockPin,LSBFIRST,00000000); 
  }
  //해당하는 LED라인에 불을 켜 준다.
  LEDON[IsLedOn] = true;
  shiftOut(LedData[IsLedOn],clockPin,LSBFIRST,shiftData[IsLedOn]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
  //시리얼 시작과 사용하는 핀들의 PINMODE설정
  Serial.begin(9600);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT); 
  for(int i=0; i<N_LEDLINE; i++)
  { pinMode(LedData[i],OUTPUT); }

  //타이머 인터럽트 세팅과 스타트 함수.
  MsTimer2::set(15,timerISR);      //15ms마다 timerISR함수를 실행한다.
  MsTimer2::start();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void loop() 
{ 
   analogReference(DEFAULT);                       // ADC의 기준전압을 사용하는 전압으로 바꿔줌(5V혹은 3.3V)
   unsigned int sensitive = analogRead(pot);       // 가변저항의 값을 감도로 받음.
   unsigned long startMillis= millis();            // 시작 시간을 잰다. millis() 함수가 현재시간(아두이노가 시작된 후부터 경과된 시간)을 밀리초 단위로(1/1000 초) 리턴 
   unsigned int peakToPeak = 0;                    // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   while (millis() - startMillis < sampleWindow)   // 50ms 지정된 시간만큼 데이터 샘플링
   {
      analogReference(INTERNAL1V1);                //ADC의 기준전압을 내장 1.1V 로 설정(오디오센서 출력이 0.8V이기 때문)
      sample = analogRead(sound);
      if (sample < 1024)                           // 1024 이상이면 버림
      {
         if (sample > signalMax)                   // 최댓값 갱신
         { signalMax = sample; }                    // 최댓값이 기존 최댓값보다 높으면 갱신
         else if (sample < signalMin)
         { signalMin = sample; }                    // 최소값 갱신
      }
   }
   peakToPeak = signalMax - signalMin;             // max - min = peak-peak 진폭 = 소리크기
  
   Serial.println(peakToPeak/sensitive);           //시리얼 출력
  int height = peakToPeak/(sensitive/10);          // 높이만큼 led출력해준다. 가변 저항으로 민감도 변경
  if(height > 6) { height = 6; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//시프트 레지스터 출력부분 BOOL조건에 만족하는 라인만 출력. shift데이터를 미리 만들어놓고 배열로 한 칸씩 밀기.
//LED LINE을 한 칸씩 밀어줌(옆으로 밀리는 효과를 위해)
  for(int i=0; i<N_LEDLINE; i++)
  { shiftData[i+1] = shiftData[i]; }

//높이에 따라 레지스터에 전송할 데이터를 바꿀 함수.
  switch(height)
  {
    case 0: shiftData[0] = 00000000; break;
    case 1: shiftData[0] = 00000001; break;
    case 2: shiftData[0] = 00000011; break;
    case 3: shiftData[0] = 00000111; break;
    case 4: shiftData[0] = 00001111; break;
    case 5: shiftData[0] = 00011111; break;
    case 6: shiftData[0] = 00111111; break;
  }
}
