//KHAI BÁO BIẾN
int pump = 9;
char abc[15];
volatile int flow_frequency_1;
unsigned int l_hour_1; 
unsigned char flowsensor_1 = 2;
unsigned long currentTime_1;
unsigned long cloopTime_1;
float estimated_l_hour_1;
float e1_k, e1_k1, e1_k2;
int i, PWM1, PWM2;
int cau_hinh, che_do_1, che_do_2;
float Setpoint1, Setpoint2;
String SP1, SP2, pwm1;
String KP2, KI2, KD2;
float kp2, ki2, kd2;
float P2, I2, D2;
float T_2=1;
float e1, e2, e2l;
//CÁC THƯ VIỆN
//BỘ LỌC KALMAN
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.001);
SimpleKalmanFilter simpleKalmanFilter2(2, 2, 0.001);
//MÀN HÌNH LCD
//CHƯƠNG TRÌNH CON GỬI DỮ LIỆU TỚI SERIALCOMINSTRUMENTS
//CHƯƠNG TRÌNH CON GỬI DỮ LIỆU KIỂU INT
void SendInt(byte instrNr, int Wert)
{
  Serial.print('#');
  Serial.print(instrNr);
  Serial.print('M');
  Serial.print(Wert);
  Serial.println('<');
}
//CHƯƠNG TRÌNH CON GỬI DỮ LIỆU KIỂU FLOAT
void SendFloat(byte instrNr1, float Wert1) 
{ 
  Serial.print('#'); 
  Serial.print(instrNr1); 
  Serial.print('M'); 
  Serial.print(Wert1); 
  Serial.print('<');
}
//CHƯƠNG TRÌNH CON GỬI DỮ LIỆU KIỂU STRING
void SendString(byte instrNr2, String Wert2)
{
  Serial.print('#');
  Serial.print(instrNr2);
  Serial.print('M');
  Serial.print(Wert2);
  Serial.println('<');
}
//CHƯƠNG TRÌNH CON NHẬN DỮ LIỆU TỪ SERIALCOMINSTRUMENTS
void serialEvent()
{
  for (int i = 0; i<15;i=i+1) 
  {abc[i] = Serial.read();    
  delay(10);}
}
//CHƯƠNG TRÌNH HÀM NGẮT CHO CẢM BIẾN
void flow_1 ()
{
   flow_frequency_1++;
}
void setup()
{
  Serial.begin(115200);
  pinMode(flowsensor_1, INPUT);
  digitalWrite(flowsensor_1, HIGH);
  attachInterrupt(0, flow_1, RISING);
  sei();
  currentTime_1 = millis();
  cloopTime_1 = currentTime_1;
  pinMode(pump, OUTPUT);
}
void loop()
{
//XỬ LÝ TÍN HIỆU CẢM BIẾN
    currentTime_1 = millis();
   if(currentTime_1 >= (cloopTime_1 + 1000))
   {
      cloopTime_1 = currentTime_1;
      l_hour_1 = ((flow_frequency_1*0.171+1.8716)*60);
      flow_frequency_1 = 0;
   }
  estimated_l_hour_1 = simpleKalmanFilter1.updateEstimate(l_hour_1);
  estimated_l_hour_1 = simpleKalmanFilter1.updateEstimate(estimated_l_hour_1);
  estimated_l_hour_1 = simpleKalmanFilter1.updateEstimate(estimated_l_hour_1);
  estimated_l_hour_1 = simpleKalmanFilter1.updateEstimate(estimated_l_hour_1);
//TÍN HIỆU ĐÈN BÁO VÀ LƯU LƯỢNG CỦA DÒNG QUÁ TRÌNH VỚI DÒNG ĐIỀU KHIỂN
//Tín hiệu đèn báo 
if(estimated_l_hour_1 > 200){
      SendInt(13, 200);}
  else {SendInt(13,000);}
//Lưu lượng của dòng
  SendInt (17, estimated_l_hour_1);
  SendInt (18, estimated_l_hour_1/60);
//CẤU HÌNH THỨ NHẤT
//Đọc công tắc cấu hình
if ((abc[1] == '2') && (abc[2] == '4') && (abc[4] == '1')){
    cau_hinh = 1;
    SendString(9,"Configuration 1 - ON");
    SendInt(11, 200);}
if ((abc[1] == '2') && (abc[2] == '4') && (abc[4] == '0')){
    SendInt(11,000);
    cau_hinh = 0;}
//Đọc chế độ điều khiển
if ((abc[1] == '2') && (abc[2] == '5') && (abc[4] == '1')){
    che_do_1 = 1;
    SendString(9,"Configuration 1 - AUTO");}
else if ((abc[1] == '2') && (abc[2] == '5') && (abc[4] == '0')){
    che_do_1 = 0;}
if ((abc[1] == '2') && (abc[2] == '6') && (abc[4] == '1')){
    che_do_1 = 2;
    SendString(40,"Configuration 1 - MANUAL");}
else if ((abc[1] == '2') && (abc[2] == '6') && (abc[4] == '0')){
    che_do_1 = 0;}
//Đọc giá trị setpoint
if ((abc[1] == '1')&& (abc[2] == '6')) {
      i = 4;
      while (abc[i] != '<') {
        SP1 += abc[i]; 
        i=i+1;  }
      Setpoint1 = SP1.toFloat();
      SP1 = '\n';
      SendFloat(40,Setpoint1);}
//Đọc giá trị PWM
if ((abc[1] == '3')&& (abc[2] == '0')) {
      i = 4;
      while (abc[i] != '<') {
        pwm1 += abc[i]; 
        i=i+1;  }
      PWM1 = pwm1.toFloat();
      pwm1 = '\n';
      SendFloat(9,PWM1);}
//CẤU HÌNH THỨ 2
//Đọc công tắc cấu hình
//Đọc công tắc cấu hình
if ((abc[1] == '2') && (abc[2] == '7') && (abc[4] == '1')){
    cau_hinh = 1;
    SendString(9,"Configuration 2 - ON");
    SendInt(12, 200);}
if ((abc[1] == '2') && (abc[2] == '7') && (abc[4] == '0')){
    SendInt(12,000);
    cau_hinh = 0;}
//Đọc chế độ điều khiển
if ((abc[1] == '2') && (abc[2] == '8') && (abc[4] == '1')){
    che_do_2 = 1;
    SendString(9,"Set Value - AUTO");}
else if((abc[1] == '2') && (abc[2] == '8') && (abc[4] == '0')){
    che_do_2 = 0;}
if ((abc[1] == '2') && (abc[2] == '9') && (abc[4] == '1')){
    che_do_2 = 2;
    SendString(40,"Set Value - MANUAL");}
else if((abc[1] == '2') && (abc[2] == '9') && (abc[4] == '0')){
    che_do_2 = 0;}
//Đọc giá trị setpoint
if ((abc[1] == '3')&& (abc[2] == '2')) {
      i = 4;
      while (abc[i] != '<') {
        SP2 += abc[i]; 
        i=i+1;  }
      Setpoint2 = SP2.toFloat();
      SP2 = '\n';
      SendFloat(40,Setpoint2);}
//Đọc giá trị Kp, Ki, Kd
if ((abc[1] == '3')&& (abc[2] == '3')) {
      i = 4;
      while (abc[i] != '<') {
        KP2 += abc[i]; 
        i=i+1;  }
      kp2 = KP2.toFloat();
      KP2 = '\n';
      SendFloat(9,kp2);}  
if ((abc[1] == '3')&& (abc[2] == '4')) {
      i = 4;
      while (abc[i] != '<') {
      KI2 += abc[i]; 
      i=i+1;  }
     ki2 = KI2.toFloat();
     KI2 = '\n';
     SendFloat(9,ki2);}
if ((abc[1] == '2')&& (abc[2] == '5')) {
      i = 4;
        while (abc[i] != '<') {
        KD2 += abc[i]; 
        i=i+1; }
     kd2 = KD2.toFloat();
     KD2 = '\n';
     SendFloat(9,kd2);}
//THỰC THI CẤU HÌNH THỨ 1
//Thực thi cấu hình thứ nhất chế độ auto
if ((cau_hinh == 1) && (che_do_1 == 1)){
  PWM1 =Setpoint1*0.1292-9.7032;
  analogWrite(pump, PWM1);
  e1=abs(Setpoint1-estimated_l_hour_1);
  SendFloat(36,e1);
  SendInt(16,PWM1);}
//Thực thi cấu hình thứ nhất chế độ manual
else if ((cau_hinh == 1)&& (che_do_1 == 2))
{ analogWrite(pump, PWM1);
  e1=abs(Setpoint1-estimated_l_hour_1);
  SendFloat(36,e1);
  SendInt(16,PWM1);}
//THỰC THI CẤU HÌNH THỨ 2
//Thực thi cấu hình thứ 2 chế độ auto
else if ((cau_hinh == 2) && (che_do_2 == 1)){
  e2 = Setpoint2 - estimated_l_hour_1;
  SendFloat(37,abs(Setpoint2 - estimated_l_hour_1));
  P2 = 0.25*e2;                   
  I2 += 0.0003*T_2/2.0*(e2 + e2l);
  D2 = 0.2/T_2*(e2 - e2l);
  PWM2 = P2 + I2 + D2;
  e2l = e2; 
  PWM2 = constrain(PWM2, 25, 255);
  analogWrite(pump, PWM2);
  SendInt(16, PWM2);
  }
//Thực thi cấu hình thứ 2 chế độ manual
else if ((cau_hinh == 2) && (che_do_2 == 2)){
  e2 = Setpoint2 - estimated_l_hour_1;
  SendFloat(37,abs(Setpoint2 - estimated_l_hour_1));
  P2 = kp2*e2;                  
  I2 += ki2*T_2/2.0*(e2 + e2l);
  D2 = kd2/T_2*(e2 - e2l);
  PWM2 = P2 + I2 + D2;
  e2l = e2; 
  PWM2 = constrain(PWM2, 25, 255);
  analogWrite(pump, PWM2);
  SendInt(16, PWM2);
}
abc[1] = '0';
}
