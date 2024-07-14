#include <stdlib.h>
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led= 2;
static const int led2= 4;
static int led_delay=1000;
//static bool state = LOW;
//task

#define maxbuf 20
void toggleLED(void *parameter)
{
  while (1)
  {
    
    digitalWrite(led, !digitalRead(led));
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

void toggleLED2(void *parameter)
{
  while(1)
  {
    digitalWrite(led2, !digitalRead(led2));
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

void readfromserial(void *parameter)
{
  char c, buf[maxbuf];
  uint8_t idx=0;

  memset(buf,0,maxbuf);
  while(1)
  {
    if(Serial.available()>0)
    {
      c=Serial.read();
      if(c=='\n')
      {
        led_delay=atoi(buf);
        Serial.println(led_delay);
        memset(buf,0,maxbuf);
        idx=0;
      }
      else{
        if(idx<maxbuf-1)
        {
          buf[idx]=c;
          idx++;
        }
      }
    }
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);

  xTaskCreatePinnedToCore(toggleLED, "Toggle LED", 1024, nullptr, 1, nullptr, app_cpu);
  xTaskCreatePinnedToCore(toggleLED2, "Toggle second LED", 1024, nullptr, 1, nullptr, app_cpu);
  xTaskCreatePinnedToCore(readfromserial, "Read From Serial", 1024, nullptr, 1, nullptr, app_cpu);
  vTaskDelete(nullptr);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*digitalWrite(led, !digitalRead(led));
    //vTaskDelay(500 / portTICK_PERIOD_MS);
  delay(1000);

   digitalWrite(led2, !digitalRead(led2));

  //digitalWrite(led, HIGH);
  delay(1000); // this speeds up the simulation*/
}
