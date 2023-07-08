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

int _i2c_si7021_mode = 7021;	// 7013, 7020, 7021, HTU21=-21

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
//	Wire.write(0xF5);	// Measure Relative Humidity, No Hold Master Mode
	Wire.write(0xE5);	// Measure Relative Humidity, Hold Master Mode
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
	_i2c_si7021_hum = (float)hum / 65536. * 125. - 6.;
	
	delay(18);					// 15ms以上

	if(_i2c_si7021_mode >= 0){
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0xE0);		// Read Temperature Value from Previous RH Measurement
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
	}else{
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0xE3);
		if(Wire.endTransmission()){
			Serial.println("ERROR: i2c_si7021_getTemp() Wire.write for HTU21 temp ");
			return -999.;
		}
		
		delay(30);					// 15ms以上
		Wire.requestFrom(I2C_si7021,2);
		i = Wire.available();
		if(i<2){
			Serial.printf("ERROR: i2c_si7021_getTemp() Wire.requestFrom for HTU21 temp, i=%d\n",i);
			return -999.;
		}
	}
	temp = Wire.read();
	temp <<= 8;
	temp += Wire.read();

	return (float)temp / 65535. * 175.72 - 46.85;
}

float i2c_si7021_getHum(){
	float ret;
	if( _i2c_si7021_hum < 0) _i2c_si7021_temp = i2c_si7021_getTemp();
	ret = _i2c_si7021_hum;
	_i2c_si7021_hum = -999;
	return ret;
}


float i2c_si7021_heater(){
	float ma;
	uint8_t user = 0;
	
	// Read RH/T User Register 1 (0xE7)
	Wire.beginTransmission(I2C_si7021);
	Wire.write(0xE7);
	Wire.endTransmission();
	delay(18);					// 15ms以上
	Wire.requestFrom(I2C_si7021,1);
	if(Wire.available()>=1){
		user = Wire.read();
		Serial.printf("User Reg = 0x%02x\n",user);
	}
	delay(18);					// 15ms以上
	Serial.println("Done: Read User Reg");
	if((user & 0x04) == 0){
		Serial.println("Heater = OFF");
		return 0.0;
	}
	
	// Read Heater Control Register (0x11)
	Wire.beginTransmission(I2C_si7021);
	Wire.write(0x11);
	Wire.endTransmission();
	delay(18);					// 15ms以上
	Wire.requestFrom(I2C_si7021,1);
	if(Wire.available()>=1){
		uint8_t heater = Wire.read();
		ma = (float)heater * (94.20 - 3.09) / 15 + 3.09;
		Serial.printf("Heater = 0x%02x, %.1f mA\n",heater,ma);
	}
	delay(18);					// 15ms以上
	Serial.println("Done: Heater Control Register");
	return ma;
}

float i2c_si7021_heater(int current){
	boolean ret;
	if(current>16) current = 16;
	if(current<0) current = 0;
	
	if(current > 0){
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0x51);
		Wire.write((byte)(current - 1));
		ret = (Wire.endTransmission() == 0);
		if(!ret) Serial.println("ERROR: i2c_si7021_heater Wire.endTransmission");
		Serial.println("Done: i2c_si7021_heater");
		delay(18);					// 15ms以上
	}
	
	Wire.beginTransmission(I2C_si7021);
	Wire.write(0xE6);
	if(current > 0){
		Wire.write(0x3E);	// d2 HTRE=1
	}else{
		Wire.write(0x3A);	// d2 HTRE=0
	}
	ret = (Wire.endTransmission() == 0);
	if(!ret){
		Serial.println("ERROR: i2c_si7021_Setup Wire.endTransmission");
		return 0.0;
	}
	Serial.println("Done: i2c_si7021_Setup");
	delay(18);					// 15ms以上
	return i2c_si7021_heater();
}

boolean i2c_si7021_Setup(int PIN_SDA = 21, int PIN_SCL = 22);

boolean i2c_si7021_Setup(int PIN_SDA, int PIN_SCL){
	delay(2);					// 1ms以上
	boolean ret = Wire.begin(PIN_SDA,PIN_SCL); // I2Cインタフェースの使用を開始
	if(!ret) Serial.println("ERROR: i2c_si7021_Setup Wire.begin");
	delay(35);					// 15ms以上
	if(ret){
		i2c_si7021_heater();
		
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0xE6);
		Wire.write(0x3A);
		ret = (Wire.endTransmission() == 0);
		if(!ret) Serial.println("ERROR: i2c_si7021_Setup Wire.endTransmission");
		Serial.println("Done: i2c_si7021_Setup");
		delay(18);					// 15ms以上
		
		// Read Electronic ID 1st Byte (0xFA 0x0F)
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0xFA);
		Wire.write(0x0F);
		Wire.endTransmission();
		delay(18);					// 15ms以上
		Wire.requestFrom(I2C_si7021,8);
		if(Wire.available()>=8){
			uint32_t id = Wire.read();
			id <<= 8;
			Wire.read(); // CRC 読み捨て
			id += Wire.read();
			id <<= 8;
			Wire.read(); // CRC 読み捨て
			id += Wire.read();
			id <<= 8;
			Wire.read(); // CRC 読み捨て
			id += Wire.read();
			Wire.read(); // CRC 読み捨て
			Serial.printf("ID1 = 0x%08x\n",id);
		}
		delay(18);					// 15ms以上
		Serial.println("Done: Read Electronic ID");
		
		
		// Read Electronic ID 2nd Byte (0xFC 0xC9)
		Wire.beginTransmission(I2C_si7021);
		Wire.write(0xFC);
		Wire.write(0xC9);
		Wire.endTransmission();
		delay(18);					// 15ms以上
		Wire.requestFrom(I2C_si7021,6);
		if(Wire.available()>=6){
			uint32_t id = Wire.read();
			id <<= 8;
			id += Wire.read();
			id <<= 8;
			Wire.read(); // CRC 読み捨て
			id += Wire.read();
			id <<= 8;
			id += Wire.read();
			Wire.read(); // CRC 読み捨て
			Serial.printf("ID2 = 0x%08x, ",id);
			// 0x0D=13=Si7013
			// 0x14=20=Si7020
			// 0x15=21=Si7021
			if(id>>24 ==0x0D){
				Serial.println("Si7013");
				_i2c_si7021_mode = 7013;
			}else if(id>>24 ==0x14){
				Serial.println("Si7020");
				_i2c_si7021_mode = 7020;
			}else if(id>>24 ==0x15){
				 Serial.println("Si7021");
				_i2c_si7021_mode = 7021;
			}else if(id>>24 ==0x32){
				 Serial.println("HTU21");
				_i2c_si7021_mode = -21;
			}else Serial.println("Unknown");
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
