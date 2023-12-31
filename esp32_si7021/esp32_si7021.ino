/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。(詳細は別記)
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

I2C接続の温湿度センサの値を読み取る
SILICON LABS社 Si7021
							   Copyright (c) 2017-2019 Wataru KUNINO
							   https://bokunimo.net/bokunimowakaru/
*********************************************************************/
/*
		sensors_pin_reset("IO27","Si7021_GND");
		sensors_pin_reset("IO14","Si7021_SCL");
		sensors_pin_reset("IO12","Si7021_SDA");
		sensors_pin_reset("IO26","Si7021_VIN");
*/

float heater = 0.0;

void setup(){
	Serial.begin(115200);
	Serial.println("Hello!");
	pinMode(27,OUTPUT);
	digitalWrite(27,LOW);
	pinMode(26,OUTPUT);
	digitalWrite(26,HIGH);
	i2c_si7021_Setup(12, 14);
	Serial.printf("temp., humid., heater\n");
}

void loop(){
	float tmp = i2c_si7021_getTemp();
	float hum = i2c_si7021_getHum();
	Serial.printf("%0.1f, %0.1f, %.1f\n", tmp, hum, heater);
	if(hum >= 99. && heater == false){
		heater = i2c_si7021_heater(1);
	}else if(hum <= 80. && heater == true){
		heater = i2c_si7021_heater(0);
	}
	delay(5000);
}
