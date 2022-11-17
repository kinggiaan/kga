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
//String SP1, SP2, pwm1;
//String KP2, KI2, KD2;
float kp2, ki2, kd2;
float P2, I2, D2;
float T_2=1;
float e1, e2, e2l;

// Biến Xử lý tín hiệu điều khiển 
int code;
int value;


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
void XulyABC(char abc[15]){
    char code_s[2];
    code_s[0] = abc[1];
    code_s[1] = abc[2];
    code = (int)code_s[0]*10 + (int)code_s[1];
    value = (int)abc[4];
}
//Function cho Setpoint, PWM, Kp, Ki, Kd
void ReadValue_Setpoint_PWM_Kp_Ki_kd(int ma_lenh){
    int iter = 4;
    string S;// tuong tu SP2 KP2
    while(abc[iter] != '<'){
        S += abc[iter];
        iter++;
    }

    // Cau hinh 1
    if ( ma_lenh == 30) {
        PWM1 = S.toFloat();
        //Gui tin hieu PWM 
        SendFloat( 9, PWM1);
    }
    else if ( ma_lenh == 16 ){
        Setpoint1 = S.toFloat();
        //Gui tin hieu Setpoint 1
        SendFloat(40, Setpoint1);
    }

    //Cau hinh 2
    else if (ma_lenh == 32){
        Setpoint2 = S.toFloat();
        //Gui tin hieu Setpoint 2
        SendFloat(40, Setpoint2);
    }
    else if (ma_lenh == 33){
        kp2 = S.toFloat();
        //Gui tin hieu Kp
        SendFloat(9, kp2);
    }
    else if (ma_lenh == 34){
        ki2 = S.toFloat();
        //Gui tin hieu Ki
        SendFloat(9, ki2);
    }
    else if (ma_lenh == 35){
        kd2 = S.toFloat();
        //Gui tin hieu Kp
        SendFloat(9, kd2);
    }
       
}
//CHƯƠNG TRÌNH CON NHẬN DỮ LIỆU TỪ SERIALCOMINSTRUMENTS
void serialEvent()
{
  for (int i = 0; i<15;i=i+1) 
  {abc[i] = Serial.read();    
  delay(10);}
  XulyABC(abc); ///Thêm mới
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


//CÁC CẤU HÌNH
switch(code){
    //CẤU HÌNH THỨ NHẤT
    case 24://Đọc công tắc cấu hình
        if(value) {
            cau_hinh = 1;
            SendString(9,"Configuration 1 - ON");
            SendInt(11, 200);
        }
        else{
            SendInt(11,000);
            cau_hinh = 0; 
        }
    break;

    case 25://Đọc chế độ điều khiển
        if(value){
            che_do_1 = 1;
            SendString(9,"Configuration 1 - AUTO");
        }
        else{
          che_do_1 = 0;  
        }
    break;

    case 26:
        if(value){
            che_do_1 = 2;
            SendString(40,"Configuration 1 - MANUAL");
        }
        else{
          che_do_1 = 0;  
        }
    break;

    case 16://Đọc giá trị setpoint
        ReadValue_Setpoint_PWM_Kp_Ki_kd(16);
    break;

    case 30://Đọc giá trị PWM
        ReadValue_Setpoint_PWM_Kp_Ki_kd(30);
    break;

    //Cấu hình 2
    

    case 27://Đọc công tắc cấu hình
        if(value){
            cau_hinh = 1;
            SendString(9,"Configuration 2 - ON");
            SendInt(12, 200);
    }
        else{
            SendInt(12,000);
            cau_hinh = 0; 
        }
    break;
   
    case 28: //Đọc chế độ điều khiển
        if(value){
             che_do_2 = 1;
            SendString(9,"Set Value - AUTO");
        }
        else{
            che_do_2 = 0; 
        }
    break;

    case 29:
        if(value){
            che_do_2 = 2;
            SendString(40,"Set Value - MANUAL");
        }
        else{
            che_do_2 = 0; 
        }
    break;

    
    case 32://Đọc giá trị setpoint 2
        ReadValue_Setpoint_PWM_Kp_Ki_kd(32);
    break;

    //Đọc giá trị Kp, Ki, Kd
    case 33:
        ReadValue_Setpoint_PWM_Kp_Ki_kd(33);
    break;

    case 34:
        ReadValue_Setpoint_PWM_Kp_Ki_kd(34);
    break;

    case 35:
        ReadValue_Setpoint_PWM_Kp_Ki_kd(35);
    break;
    
    default: //Nếu code lỗi sẽ chạy vào TH default
    break;
}
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
