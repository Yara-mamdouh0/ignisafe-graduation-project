#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi credentials
#define WIFI_SSID "MR"
#define WIFI_PASSWORD "00000099"

// Firebase configuration
#define API_KEY "AIzaSyCzqq53eTyBlcTq4AG7U1-1qXZqhQ9lkq8"
#define DATABASE_URL "https://fire-5a45f-default-rtdb.firebaseio.com"

// Sensor pins
#define FLAME_SENSOR_PIN 13
#define GAS_SENSOR_PIN 34  // ملاحظة: تم تغييره لدبوس تماثلي (ADC)
#define BUZZER_PIN 15      // دبوس الجرس

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // تأكد من إيقاف الجرس بالبداية

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Sign up
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase signup successful");
    signupOK = true;
  } else {
    Serial.printf("Signup error: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

bool sendDataToFirebase(String path, String value) {
  int retryCount = 0;
  while (retryCount < 3) {
    if (Firebase.RTDB.setString(&fbdo, path, value)) {
      return true;
    }
    retryCount++;
    delay(500);
  }
  return false;
}

void loop() {
  int flameState = digitalRead(FLAME_SENSOR_PIN);
  int gasRawValue = analogRead(GAS_SENSOR_PIN);  // قراءة تناظرية
  int gasPercentage = map(gasRawValue, 0, 4095, 0, 100);  // تحويل لقيمة مئوية 0-100%

  // التحقق من حالة المستشعرات وتشغيل الجرس
  if (flameState == LOW || gasPercentage > 50) { // افترضنا خطر إذا النسبة > 50%
    digitalWrite(BUZZER_PIN, HIGH); // تشغيل الجرس
  } else {
    digitalWrite(BUZZER_PIN, LOW); // إيقاف الجرس
  }

  // إرسال البيانات إلى Firebase كل ثانية
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // إرسال حالة مستشعر اللهب
    if (flameState == HIGH) {
      sendDataToFirebase("FlameSensor/Status", "No Fire");
    } else {
      sendDataToFirebase("FlameSensor/Status", "Fire Detected");
    }

    // إرسال نسبة الغاز كمئوية
    sendDataToFirebase("GasSensor/Percentage", String(gasPercentage));
  }
}
