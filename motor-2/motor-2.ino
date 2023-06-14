

#define BLYNK_TEMPLATE_ID "TMPLk6g3uKP4"
#define BLYNK_TEMPLATE_NAME "Nodemcu Motor"
#define BLYNK_AUTH_TOKEN "n_dvQcW500HZ5WRHdBXcjW1MXDznUd1u"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "AndroidAP7B35";
char pass[] = "vishnu@1999";

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  if(value == 1){
    digitalWrite(D1, HIGH);
  }else{
   digitalWrite(D1, LOW);
  }
  }
BLYNK_WRITE(V1)
{
  int value = param.asInt();
  if(value == 0){
    digitalWrite(D2, LOW);
  }else{
   digitalWrite(D2, HIGH);
  }
  }
void setup()
{
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  
}

void loop()
{
  Blynk.run();

}