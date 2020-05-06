#include <ArduinoJson.h>
#include<SoftwareSerial.h> //https://m.blog.naver.com/PostView.nhn?blogId=darknisia&logNo=220808977305&proxyReferer=https%3A%2F%2Fwww.google.com%2F
#include<Arduino.h>       //https://studymake.tistory.com/227
#include<Servo.h>
#define DEBUG true

SoftwareSerial bluetooth(11,10);

Servo m1;
Servo m2;
Servo m3;
Servo m4;

int m1_starting_angle=100;

int m2_starting_angle=160;

int m3_no_grap=150;
int m3_grap=80;

int m4_starting_angle=120;
int m4_hit_angle=175;

void setup() {
  // put your setup code here, to run once:
  m1.attach(9); 
  m2.attach(3); 
  m3.attach(5); 
  m4.attach(6);  

  m3.write(m3_grap);
  delay(500);
  m1.write(m1_starting_angle);
  delay(500);
  m2.write(m2_starting_angle);
  delay(500);
  m4.write(m4_starting_angle);
  delay(500);
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("end setup");
}

void loop() {
      String response="";
    int temp=0;
 //   Serial.println("Time is running!!");
    if (bluetooth.available()) {
        long int time=millis(); //현재 시간 설정 
        int timeout = 3000; //ms
        while((time+timeout)>millis()) {
            while(bluetooth.available()) {
                char c = bluetooth.read();
                response+=c;
            }
        }
        Serial.println("response :" + response);     
    }

    //Json파싱 시작
        String seperator="@";
    int startindex = response.indexOf(seperator);
    if (startindex != -1){
        String json_raw_data = response.substring(startindex + 1); //@바로 앞 인덱스
        if (DEBUG){
            Serial.println("@@@json_raw_data@@@");
            Serial.println(json_raw_data);
        }  
        StaticJsonBuffer<4500> jsonBuffer;
        JsonObject& json_data = jsonBuffer.parseObject(json_raw_data); 
    //Json파싱 끝 

    
    int state = atoi(json_data["state"]);
        if (DEBUG){
            Serial.print("jsondata!\n");
            Serial.print(state);
            Serial.print("here\n");
            Serial.print("\n");
        }

     for(int i=1;i<=state;i++){
            String st(i);
            String temp= json_data[st+"_t"];//"car" or "arm" 
            if (DEBUG){
                Serial.print("jsondata2!\n");
                Serial.print(temp);
                Serial.print("\n");
            }
            
            if (DEBUG){
                Serial.print("Arm is running!\n");
                Serial.print("\n");
            } 
 
            ////////////////////////////oldversion
            String c_motor =json_data[String(i)+"_m"]; 
            int t_degree=atoi(json_data[String(i)+"_d"]);//목표 각도 
  
            if(DEBUG){
                Serial.print(c_motor);
                Serial.print("\n");
                Serial.print(t_degree);
                Serial.print("\n");
            }
            
            Rotate_Motor(c_motor,t_degree);
        } 
    jsonBuffer.clear();

        
    }

          

}

void Rotate_Motor(String c_motor,int t_degree){
      //모터 각도 이동 및 저장
      if(c_motor=="M1"){
        m1.write(t_degree);
        Serial.print("angle change\n");
        delay(2000);
  
        m4.write(m4_hit_angle);
        Serial.print("hit piano(do)\n");
        delay(2000);

        m4.write(m4_starting_angle);
        Serial.print("moving to staring point\n");
        delay(2000);
      }else if(c_motor=="M2"){ 
        m2.write(t_degree);
      }else if(c_motor=="M3"){
        m3.write(t_degree);
      }else if(c_motor=="M4"){
        m4.write(t_degree);
      }
} 