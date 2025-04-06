#define RXD2 16  // Nhận dữ liệu từ R200M
#define TXD2 17  // Gửi dữ liệu nếu cần
String barcodeData = "";  // Lưu dữ liệu quét được
const int trigPin = 5;
const int echoPin = 18;
const int led=2;
const int longest=50;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
long duration;
float distanceCm;
float distanceInch;
int light;
int dark;

void setup() {
  

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  // Serial2 để giao tiếp với R200M

  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(led,OUTPUT);
    // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

 while (Serial2.available()&&distanceCm<longest) {
    char c = Serial2.read();
    barcodeData += c;

    // Nếu gặp ký tự xuống dòng (thường sau mỗi lần quét), thì in ra
    if (c == '\n' || c == '\r') {
      barcodeData.trim();  // Xoá khoảng trắng, xuống dòng
      if (barcodeData.length() > 0) {
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          // Sets the trigPin on HIGH state for 10 micro seconds
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          
          // Reads the echoPin, returns the sound wave travel time in microseconds
          duration = pulseIn(echoPin, HIGH);
          
          // Calculate the distance
          distanceCm = duration * SOUND_SPEED/2;
        Serial.println("1111 " );
        light=distanceCm*14;
        dark=light;
        digitalWrite(led,HIGH);
        delay(light);
        digitalWrite(led,LOW);
        delay(dark);
          lcd.setCursor(0, 0);
  // print message
        lcd.print(distanceCm);
        lcd.print("cm");
      }
       barcodeData = ""; // Reset lại sau khi xử lý
    }
  }


  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  lcd.setCursor(0, 0);
  // print message
  lcd.print(distanceCm);
  lcd.print("cm");
  delay(400);
}