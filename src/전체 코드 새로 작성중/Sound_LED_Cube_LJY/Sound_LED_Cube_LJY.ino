//타이머 인터럽트 사용을 위한 라이브러리.
#include <MsTimer2.h>

//갯수 설정
const int n_reg = 6;          //레지스터 갯수 설정
const int n_height = 6;       //Led높이 설정
const int t_sample = 50;      //샘플링 시간(50ms)

//핀 설정
//센서 핀
int s_pot = A1;               //가변저항 입력 핀
int s_mic = A0;               //마이크 입력 핀
//시프트 레지스터 핀
int reg_latchpin = 2;         //latch핀
int reg_clockpin = 3;         //clock핀
int reg_mrpin = 4;            //초기화 핀 (active low)
const int reg_datapin[] = {5,6,7,8,9,10};   //데이터가 전송되는 핀

//센서의 입력 값 저장 변수
unsigned int input_pot;         //가변저항 입력
unsigned int input_mic;         //마이크 입력

//데이터 부분
//인터럽트에서 사용할 변수
unsigned int timer_time = 0;     //인터럽트에서 증가하는 시간
unsigned int timer_LED  = 0;     //시간을 %6한 결과값
byte data_led[] = {0,0,0,0,0,0}; //Led를 출력하기 위한 byte값. shiftout함수로 전할 데이터임.

///////////////////////////////////////////////////////////////////////////////////////

//타이머 인터럽트 시간마다 실행하는 함수. LED의 깜빡임 구현.
void timer_function()
{
  timer_time++;                 //시간을 계속 증가
  timer_LED = timer_time%6;     //증가하는 시간을 %6연산을 해서 따로 저장(어떤 LED를 켤지 정하기 위해서)
  //LED를 전부 초기화하고 시작 mrpin은 activelow이기때문에 low후 high로 전환
  digitalWrite(reg_mrpin,LOW);
  digitalWrite(reg_mrpin,HIGH);

  //LED라인이 켜 질 순서라면
  //시프트 레지스터에 loop에서 계산된 값 만큼의 높이를 출력시켜줌
  //시프트 레지스터에 데이터 출력을 하기 위해선 latch핀을 low로 만든 후 shiftout 후 다시 latch핀을 high로 변경
  //clock은 함수 내에서 clock핀으로 자동 생성해줌
  digitalWrite(reg_latchpin,LOW);
  shiftOut(reg_datapin[timer_LED],reg_clockpin,LSBFIRST,data_led[timer_LED]);
  digitalWrite(reg_latchpin.HIGH);
}
///////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  //핀 모드 설정
  pinMode(s_pot,INPUT);
  pinMode(s_mic,INPUT);
  pinMode(reg_latchpin,OUTPUT);
  pinMode(reg_clockpin,OUTPUT);
  pinMode(reg_mrpin,OUTPUT);
  for(int i=0;i<6;i++)
  { pinMode(reg_datapin[i],OUTPUT); }

  digitalWrite(reg_mrpin,HIGH);             //MR핀을 비활성화해줌

  //타이머 사용을 위한 함수
  MsTimer2::set(5,timer_function);       //5ms마다 timer_function함수 실행
  MsTimer2::start();
}
/////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  

}
