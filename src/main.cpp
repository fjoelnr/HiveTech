#include "HiveTech.h"



void setup_wifi() {
  Serial.print("Connecting to " + String(ssid) + " ");
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("");
  Serial.println("WiFi connected with IP address: " + WiFi.localIP().toString());
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("OsoyooCommand");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_deep_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_deep_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

void setup() {

  String msg = "";
  char MsgTMP_BME280[25];
  char MsgHUM_BME280[25];
  char MsgPRS_BME280[25];
  char MsgALT_BME280[25];

  timenow = millis();

  Serial.begin(115200);
  Serial.println("Booting...");
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up every 15 seconds
  */
  esp_deep_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  //Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  /*
  Next we decide what all peripherals to shut down/keep on
  By default, ESP32 will automatically power down the peripherals
  not needed by the wakeup source, but if you want to be a poweruser
  this is for you. Read in detail at the API docs
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  Left the line commented as an example of how to configure peripherals.
  The line below turns off all RTC peripherals in deep sleep.
  */
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");

  setup_wifi();

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");

  client.setServer(mqtt_server, 1883);

  if (!client.connected()) {
        reconnect();
    }
    client.loop();

  Wire.begin();					// Start I2C-BUS


    Serial.println("Detecting BME280 sensor...");

    //***Set up sensor 'A'******************************//
    //commInterface can be I2C_MODE or SPI_MODE
    bme280.settings.commInterface = I2C_MODE;
    bme280.settings.I2CAddress = 0x77;
    bme280.settings.runMode = 3; //  3, Normal mode
    bme280.settings.tStandby = 0; //  0, 0.5ms
    bme280.settings.filter = 0; //  0, filter off
    //tempOverSample can be:
    //  0, skipped
    //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    bme280.settings.tempOverSample = 1;
    //pressOverSample can be:
    //  0, skipped
    //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    bme280.settings.pressOverSample = 1;
    //humidOverSample can be:
    //  0, skipped
    //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    bme280.settings.humidOverSample = 1;


    if (!bme280.beginI2C()) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      Serial.println("BME280 not found");
      delay(5000);
    }
    else {
      Serial.println("BME280 OK");
    }

    msg = bme280.readTempC();
    Serial.print("BME280 Temperature: ");
    msg.toCharArray(MsgTMP_BME280,25);
    Serial.println(MsgTMP_BME280);

    msg = bme280.readFloatHumidity();
    Serial.print("BME280 Humitity: ");
    msg.toCharArray(MsgHUM_BME280,25);
    Serial.println(MsgHUM_BME280);

    msg = bme280.readFloatPressure() / 100;
    Serial.print("BME280 Pressure: ");
    msg.toCharArray(MsgPRS_BME280,25);
    Serial.println(MsgPRS_BME280);

    msg = bme280.readFloatAltitudeMeters();
    Serial.print("BME280 Altitude: ");
    msg.toCharArray(MsgALT_BME280,25);
    Serial.println(MsgALT_BME280);

    client.publish("HT_BME280_Temperature", MsgTMP_BME280);
    client.publish("HT_BME280_Humidity", MsgHUM_BME280);
    client.publish("HT_BME280_Pressure", MsgPRS_BME280);
    client.publish("HT_BME280_Altitude", MsgALT_BME280);

    /*
    Now that we have setup a wake cause and if needed setup the
    peripherals state in deep sleep, we can now start going to
    deep sleep.
    In the case that no wake up sources were provided but deep
    sleep was started, it will sleep forever unless hardware
    reset occurs.
    */

    Serial.println("Going to sleep after " + String(millis() - timenow) + "ms");
    ArduinoOTA.handle();

    delay(5000);

    esp_deep_sleep_start();

    Serial.println("This will never be printed ;)");
}



void loop() {

  //This is not going to be called

  // ArduinoOTA.handle();
  //
  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();
  //
  // long now = millis();
  //
  // if (now - lastMsg > 5000) {
  //   lastMsg = now;
  //   String msg = "";
  //   char MsgTMP_BME280[25];
  //   char MsgHUM_BME280[25];
  //   char MsgPRS_BME280[25];
  //   char MsgALT_BME280[25];
  //
  //   msg = bme280.readTempC();
  //   Serial.print("BME280 Temperature: ");
  //   msg.toCharArray(MsgTMP_BME280,25);
  //   Serial.println(MsgTMP_BME280);
  //
  //   msg = bme280.readFloatHumidity();
  //   Serial.print("BME280 Humitity: ");
  //   msg.toCharArray(MsgHUM_BME280,25);
  //   Serial.println(MsgHUM_BME280);
  //
  //   msg = bme280.readFloatPressure() / 100;
  //   Serial.print("BME280 Pressure: ");
  //   msg.toCharArray(MsgPRS_BME280,25);
  //   Serial.println(MsgPRS_BME280);
  //
  //   msg = bme280.readFloatAltitudeMeters();
  //   Serial.print("BME280 Altitude: ");
  //   msg.toCharArray(MsgALT_BME280,25);
  //   Serial.println(MsgALT_BME280);
  //
  //
  //   client.publish("HT_BME280_Temperature", MsgTMP_BME280);
  //   client.publish("HT_BME280_Humidity", MsgHUM_BME280);
  //   client.publish("HT_BME280_Pressure", MsgPRS_BME280);
  //   client.publish("HT_BME280_Altitude", MsgALT_BME280);
  // }
}
