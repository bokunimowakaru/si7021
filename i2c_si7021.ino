/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。(詳細は別記)
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

I2C接続の温湿度センサの値を読み取る
SILICON LABS社 Si7021

参考文献
https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf

							   Copyright (c) 2017-2023 Wataru KUNINO
							   https://bokunimo.net/bokunimowakaru/
*********************************************************************/

#include <Wire.h> 
#define I2C_si7021 0x40 		   // Si7021 の I2C アドレス 

volatile float _i2c_si7021_hum = -999;
volatile float _i2c_si7021_temp = -999;

float i2c_si7021_getTemp(){
	int temp,hum,i;
	if( _i2c_si7021_temp >= -100 ){
		float ret;
		ret = _i2c_si7021_temp;
		_i2c_si7021_temp = -999;
		return ret;
	}
	_i2c_si7021_hum=-999.;
	_i2c_si7021_temp = -999;
	Wire.beginTransmission(I2C_si7021);
	Wire.write(0xF5);
	if(Wire.endTransmission()){
		Serial.println("ERROR: i2c_si7021_getTemp() Wire.write hum");
		return -999.;
	}
	
	delay(30);					// 15ms以上
	Wire.requestFrom(I2C_si7021,2);
	i = Wire.available();
	if(i<2){
		Serial.printf("ERROR: i2c_si7021_getTemp() Wire.requestFrom hum, i=%d\n",i);
		return -999.;
	}
	hum = Wire.read();
	hum <<= 8;
	hum += Wire.read();
	
	delay(18);					// 15ms以上
	Wire.beginTransmission(I2C_si7021);
	Wire.write(0xE0);
	if(Wire.endTransmission()){
		Serial.println("ERROR: i2c_si7021_getTemp() Wire.write temp");
		return -999.;
	}
	
	delay(30);					// 15ms以上
	Wire.requestFrom(I2C_si7021,2);
	i = Wire.available();
	if(i<2){
		Serial.printf("ERROR: i2c_si7021_getTemp() Wire.requestFrom temp, i=%d\n",i);
		return -999.;
	}
	temp = Wire.read();
	temp <<= 8;
	temp += Wire.read();

	_i2c_si7021_hum = (float)hum / 65536. * 125. - 6.;
	return (float)temp / 65535. * 175.72 - 46.85;
}

float i2c_si7021_getHum(){
	float ret;
	if( _i2c_si7021_hum < 0) _i2c_si7021_temp = i2c_si7021_getTemp();
	ret = _i2c_si7021_hum;
	_i2c_si7021_hum = -999;
	return ret;
}

boolean i2c_si7021_Setup(int PIN_SDA = 21, int PIN_SCL = 22);

boolean i2c_si7021_Setup(int PIN_SDA, int PIN_SCL){
	delay(2);					// 1ms以上
	boolean ret = Wire.begin(PIN_SDA,PIN_SCL); // I2Cインタフェースの使用を開始
	if(!ret) Serial.println("ERROR: i2c_si7021_Setup Wire.begin");
	delay(18);					// 15ms以上
	if(ret){
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0xE6);
		Wire.write(0x3A);
		ret = (Wire.endTransmission() == 0);
		if(!ret) Serial.println("ERROR: i2c_si7021_Setup Wire.endTransmission");
		Serial.println("Done: i2c_si7021_Setup");
		delay(18);					// 15ms以上
		
		// Read Electronic ID 2nd Byte (0xFC 0xC9)
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0xFC);
		Wire.write(0xC9);
		Wire.endTransmission();
		delay(18);					// 15ms以上
		Wire.requestFrom(I2C_si7021,4);
		if(Wire.available()>=4){
			uint32_t id = Wire.read();
			id <<= 8;
			id += Wire.read();
			id <<= 8;
			id += Wire.read(); // CRC 読み捨て
			id += Wire.read();
			id <<= 8;
			id += Wire.read();
			Serial.printf("ID = 0x%08x, ",id);
			// 0x0D=13=Si7013
			// 0x14=20=Si7020
			// 0x15=21=Si7021
			if(id>>24 ==0x0D) Serial.println("Si7013");
			else if(id>>24 ==0x14) Serial.println("Si7020");
			else if(id>>24 ==0x15) Serial.println("Si7021");
			else Serial.println("Unknown");
		}
		delay(18);					// 15ms以上
		Serial.println("Done: Read Electronic ID");

		// Read Firmware Revision  (0x84 0xB8)
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0x84);
		Wire.write(0xB8);
		Wire.endTransmission();
		delay(18);					// 15ms以上
		Wire.requestFrom(I2C_si7021,1);
		if(Wire.available()>=1){
			uint8_t ver = Wire.read();
			Serial.printf("VER = 0x%02x, ",ver);
			// 0xFF = Firmware version 1.0
			// 0x20 = Firmware version 2.0
			if(ver ==0xFF) Serial.println("Firmware version 1.0");
			else if(ver ==0x20) Serial.println("Firmware version 2.0");
			else Serial.println("Unknown");
		}
		delay(18);					// 15ms以上
		Serial.println("Done: Read Firmware Revision");
	}
	return ret;
}