
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
// #include "DHT.h"
#include <SimpleTimer.h>
#include "Wire.h"
#include "BH1750.h"

// BLE Credentials
const char *service_name = "PROV_12345";
const char *pop = "1234567";

// define the Device Names
char deviceName_1[] = "LED1";
char deviceName_2[] = "LED2";
char deviceName_3[] = "LED3";
char deviceName_4[] = "LED4";

// define the GPIO connected with Relays
static uint8_t RelayPin1 = 18;  //D23
static uint8_t RelayPin2 = 5;  //D22
static uint8_t RelayPin3 = 23;  //D21
static uint8_t RelayPin4 = 19;  //D19


static uint8_t gpio_reset = 0;
// static uint8_t DHTPIN = 18;
static uint8_t wifiLed = 2;
// bool led_pin_state = false;

bool wifi_connected = 0;

float lux = 0;

/* Variable for reading pin status*/
// Relay State
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW; //Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW; //Define integer to remember the toggle

// DHT dht(DHTPIN, DHT11);

SimpleTimer Timer;
BH1750 lightMeter;

 //declare devices
 //The framework provides some standard device types like switch, lightbulb, fan, temperature sensor.
static Switch my_switch1(deviceName_1, &RelayPin1);
static Switch my_switch2(deviceName_2, &RelayPin2);
static Switch my_switch3(deviceName_3, &RelayPin3);
static Switch my_switch4(deviceName_4, &RelayPin4);



void sysProvEvent(arduino_event_t *sys_event)
{
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
      printQR(service_name, pop, "ble");
#else
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
      printQR(service_name, pop, "softap");
#endif
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.printf("\nConnected to Wi-Fi!\n");
      wifi_connected = 1;
      digitalWrite(wifiLed, true);
      delay(500);
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV: {
        Serial.println("\nReceived Wi-Fi credentials");
        Serial.print("\tSSID : ");
        Serial.println((const char *) sys_event->event_info.prov_cred_recv.ssid);
        Serial.print("\tPassword : ");
        Serial.println((char const *) sys_event->event_info.prov_cred_recv.password);
        break;
      }
  }
}

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
  detachInterrupt(0,)
  const char *device_name = device->getDeviceName();
  Serial.println(device_name);
  const char *param_name = param->getParamName();

  if (strcmp(device_name, deviceName_1) == 0) {
    Serial.printf("LED1 = %s\n", val.val.b ? "true" : "false");
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_1 = val.val.b;
      (toggleState_1 == false) ? digitalWrite(RelayPin1, LOW) : digitalWrite(RelayPin1, HIGH);
      param->updateAndReport(val);
    }
  } else if (strcmp(device_name, deviceName_2) == 0) {
    Serial.printf("LED2 = %s\n", val.val.b ? "true" : "false");
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_2 = val.val.b;
      (toggleState_2 == false) ? digitalWrite(RelayPin2, LOW) : digitalWrite(RelayPin2, HIGH);
      param->updateAndReport(val);
    }
  } else if (strcmp(device_name, deviceName_3) == 0) {
    Serial.printf("LED3 = %s\n", val.val.b ? "true" : "false");
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_3 = val.val.b;
      (toggleState_3 == false) ? digitalWrite(RelayPin3, LOW) : digitalWrite(RelayPin3, HIGH);
      param->updateAndReport(val);
    }
  } else if (strcmp(device_name, deviceName_4) == 0) {
    Serial.printf("LED4 = %s\n", val.val.b ? "true" : "false");
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_4 = val.val.b;
      (toggleState_4 == false) ? digitalWrite(RelayPin4, LOW) : digitalWrite(RelayPin4, HIGH);
      param->updateAndReport(val);
    }
  }
}

void readSensor()
{
  lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(100);
}

void manual_control(float lux1)
{

  if ((lux1 >0) && (lux1 < 58)) {



        digitalWrite(RelayPin1, HIGH);

        // delay(5000);
        // digitalWrite(RelayPin1, LOW);
        toggleState_1 = 1;
        my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
    }
    else{
      digitalWrite(RelayPin1, LOW);
      toggleState_1 = 0;
        my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
    }
    
  //   if
  // digitalWrite(RelayPin1, LOW);
  // // digitalWrite(RelayPin2, LOW);
  // // digitalWrite(RelayPin3, LOW);
  // // digitalWrite(RelayPin4, LOW);
  // toggleState_1 = 1;
  // // toggleState_2 = 1;
  // // toggleState_3 = 1;
  // // toggleState_4 = 1;
  // my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
  // // my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
  // // my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
  // // my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
//   }
//   else{
//   digitalWrite(RelayPin1, HIGH);
//   // digitalWrite(RelayPin2, HIGH);
//   // digitalWrite(RelayPin3, HIGH);
//   // digitalWrite(RelayPin4, HIGH);
//   toggleState_1 = 0;
//   // toggleState_2 = 0;
//   // toggleState_3 = 0;
//   // toggleState_4 = 0;
//   my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
//   // my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
//   // my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
//   // my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
//   }
}

void setup()
{
  Serial.begin(115200);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));

  // Set the Relays GPIOs as output mode
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(wifiLed, OUTPUT);
  pinMode(gpio_reset, INPUT);

  // Write to the GPIOs the default state on booting
  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);
  digitalWrite(wifiLed, LOW);

  
  //declare node
  Node my_node;
  my_node = RMaker.initNode("SMART LIGHT");

 //Standard switch device
  my_switch1.addCb(write_callback);
  my_switch2.addCb(write_callback);
  my_switch3.addCb(write_callback);
  my_switch4.addCb(write_callback);

 //Add switch device to the node
  my_node.addDevice(my_switch1);
  my_node.addDevice(my_switch2);
  my_node.addDevice(my_switch3);
  my_node.addDevice(my_switch4);


  //This is optional
  RMaker.setTimeZone("Asia/Ho_Chi_Minh");
  RMaker.enableOTA(OTA_USING_PARAMS);
  //If you want to enable scheduling, set time zone for your region using setTimeZone().
  //The list of available values are provided here https://rainmaker.espressif.com/docs/time-service.html
  // RMaker.setTimeZone("Asia/Shanghai");
  // Alternatively, enable the Timezone service and let the phone apps set the appropriate timezone
  RMaker.enableTZService();
  RMaker.enableSchedule();

  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();

  // Timer for Sending Sensor Data
  Timer.setInterval(5000);

  WiFi.onEvent(sysProvEvent);

#if CONFIG_IDF_TARGET_ESP32
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#endif

  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);

}


void loop()
{
  // if (Timer.isReady() && wifi_connected) {                    
  //   Serial.println("Sending Sensor Data");

  //   float hum = dht.readHumidity();
  //   float temp = dht.readTemperature();

  //   Serial.print("Temperature: "); 
  //   Serial.println(temp);
  //   Serial.print("Humidity: "); 
  //   Serial.println(hum);

  //   temperature.updateAndReportParam("Temperature", temp);
  //   humidity.updateAndReportParam("Temperature", hum);
  //   Timer.reset();                        
  // }

  // Read GPIO0 (external button to reset device
  if (digitalRead(gpio_reset) == LOW) { //Push button pressed
    Serial.printf("Reset Button Pressed!\n");
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();

    if ((endTime - startTime) > 10000) {
      // If key pressed for more than 10secs, reset all
      Serial.printf("Reset to factory.\n");
      wifi_connected = 0;
      RMakerFactoryReset(2);
    } else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      wifi_connected = 0;
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    }
  }
  delay(100);

  if (WiFi.status() != WL_CONNECTED)
  {
    //Serial.println("WiFi Not Connected");
    digitalWrite(wifiLed, false);
  }
  else
  {
    //Serial.println("WiFi Connected");
    digitalWrite(wifiLed, true);
    delay(50);
    digitalWrite(wifiLed, false);
    if (Timer.isReady()) {
      //Serial.println("Sending Sensor Data");
      readSensor();

      Timer.reset();      // Reset a second timer
    }
  }
  delay(1000);
  manual_control(lux);
}
