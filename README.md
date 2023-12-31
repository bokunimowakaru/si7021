# si7021 drivers for Si7020, Si7021 and HTU21D

These programs provide the way to check the operation of Silicon Labs.'s humidity sensors Si7021.  
Silicon Labs.製 温湿度センサ Si7021用 GPIO直結 I2Cデータ通信サンプルプログラム by bokunimo.net  

## Features

- 開発ボードに、温湿度センサ Si7021 / HTU21 を直結  
  Direct Wiring to Board  
- 温度と湿度をシリアルに出力  
  Measured Results Output to Serial Port  
- 型番判定  
  Output the Model Number Si7020, Si7021 or HTU21D  

## Supported Sensor Devices 対応デバイス

Supported the following sensors on such as GT-21 Modules:  

- Silicon Labs., Si7020 (±4%)  
- Silicon Labs., Si7021 (±3%)  
- Measurement Specialties, HTU20D (±5%)  
- Measurement Specialties, HTU21D (±3%)  

Measurement Specialties (MEAS) 社は、2014年に TE Connectivity が買収した、センサー専業のメーカー。

![Supported Devices](pictures/gy-21.jpg)  

## Supported MCUs:  

- Arduino UNO  
- ESP32 Dev Boards  
- TTGO T-Koala  
- etc...  

![Arduino UNO Board and Si7021](pictures/uno_si7021.jpg)  

### 接続方法 Wiring for Arduino UNO

|Arduino  |Si7021    |
|---------|----------|
|3.3V     |Si7021_VIN|
|GND      |Si7021_GND|
|A4 or SDA|Si7021_SDA|
|A5 or SCL|Si7021_SCL|

## ESP32 Dev Board + Si7021

- ESP32搭載の各種ボードに、Si7021 / HTU21 搭載の温湿度センサを直結
- 温度と湿度をシリアルに出力

![ESP32 Dev Board and Si7021](pictures/esp32_si7021.jpg)  

### 接続方法 Wiring for ESP32 Dev Board

|ESP32 |Si7021    |
|------|----------|
|IO26  |Si7021_VIN|
|IO27  |Si7021_GND|
|IO14  |Si7021_SCL|
|IO12  |Si7021_SDA|

## TTGO T-Koala + Si7021

- ESP32搭載TTGO T-Koalaに、Si7021搭載の温湿度センサを直結
- 温度と湿度をシリアルに出力

![TTGO T-Koala and Si7021](pictures/ttgo_si7021.jpg)  

### 注意

- ソフトを書き込むときは Si7021 を開発ボードから取り外すこと。  
Please disconnect the sensor module from the dev board, when writing the software.  

### 接続方法 Wiring for TTGO T-Koala

|T-Koala|Si7021    |
|-------|----------|
|IO26   |Si7021_VIN|
|IO27   |Si7021_GND|
|IO14   |Si7021_SCL|
|IO12   |Si7021_SDA|

## シリアル出力例(Si7021 正常時)

	Hello!
	User Reg = 0x3a
	Done: Read User Reg
	Heater = OFF
	Done: i2c_si7021_Setup
	ID1 = 0x########
	Done: Read Electronic ID
	ID2 = 0x15ffffff, Si7021
	Done: Read Electronic ID
	VER = 0x20, Firmware version 2.0
	Done: Read Firmware Revision
	temp., humid., heater
	31.7, 88.0, 0.0
	31.7, 88.0, 0.0

## シリアル出力例(HTU21 正常時)

	Hello!
	User Reg = 0x3a
	Done: Read User Reg
	Heater = OFF
	Done: i2c_si7021_Setup
	ID1 = 0x########
	Done: Read Electronic ID
	ID2 = 0x32######, HTU21
	Done: Read Electronic ID
	Done: Read Firmware Revision
	temp., humid., heater
	32.1, 63.0, 0.0
	32.1, 61.4, 0.0
	32.0, 61.3, 0.0

HTU21Dの判定には ID2 の先頭1バイト 0x32 を使用しています。
その他の番号の場合は、Unknownと表示され、温度の取得に失敗する場合があります。

## 不具合事例(エラー時)

	Hello!
	Done: Read User Reg
	Heater = OFF
	ERROR: i2c_si7021_Setup Wire.endTransmission
	Done: i2c_si7021_Setup
	Done: Read Electronic ID
	Done: Read Electronic ID
	Done: Read Firmware Revision
	temp., humid., heater
	ERROR: i2c_si7021_getTemp() Wire.write hum
	ERROR: i2c_si7021_getTemp() Wire.write hum
	-999.0, -999.0, 0.0
	ERROR: i2c_si7021_getTemp() Wire.write hum
	ERROR: i2c_si7021_getTemp() Wire.write hum
	-999.0, -999.0, 0.0

## 関連情報

* 温湿度センサ Si7021 と HTU21D の見分け方：  
[https://bokunimo.net/blog/esp/3797/](https://bokunimo.net/blog/esp/3797/)  

* Instagram, How to check if it's Si7021 or HTU21D:  
[https://www.instagram.com/p/Cug5tYHLDYy/](https://www.instagram.com/p/Cug5tYHLDYy/)

* How to distinguish Si7021 and HTU21D (Google Transrate):  
[https://bokunimo-net.translate.goog/blog/esp/3797/](https://bokunimo-net.translate.goog/blog/esp/3797/?_x_tr_sl=ja&_x_tr_tl=en&_x_tr_hl=ja&_x_tr_pto=wapp)  

* IoT Sensor Core ESP32 by Wataru KUNINO：  
[https://git.bokunimo.com/sens/](https://git.bokunimo.com/sens/)

----------------------------------------------------------------

## GitHub Pages (This Document)

* [https://git.bokunimo.com/si7021/](https://git.bokunimo.com/si7021/)

----------------------------------------------------------------

##  git.bokunimo.com GitHub Pages site
[http://git.bokunimo.com/](http://git.bokunimo.com/)  

----------------------------------------------------------------
by <https://bokunimo.net>
