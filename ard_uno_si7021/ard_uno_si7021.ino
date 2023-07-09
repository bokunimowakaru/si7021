/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。(詳細は別記)
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

I2C接続の温湿度センサの値を読み取る
SILICON LABS社 Si7021
							   Copyright (c) 2017-2019 Wataru KUNINO
							   https://bokunimo.net/bokunimowakaru/
*********************************************************************/

void setup(){
	Serial.begin(115200);
	Serial.println("Hello!");
	pinMode(27,OUTPUT);
	digitalWrite(27,LOW);
	pinMode(26,OUTPUT);
	digitalWrite(26,HIGH);
	i2c_si7021_Setup();
	Serial.println("temp., humid.");
}

void loop(){
	float tmp = i2c_si7021_getTemp();
	float hum = i2c_si7021_getHum();
	Serial.print(tmp,1);
	Serial.print(", ");
	Serial.print(hum,1);
	Serial.println();
	delay(5000);
}
