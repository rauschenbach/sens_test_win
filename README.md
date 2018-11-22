# sens_test_win
Управление платой сенсоров
Используемые пины
===== VCP ======
PORT_A-->
PORT_D-->
PA.08 
PA.09
PA.10
PA.11 
PA.12
PD.05 

===== I2C =====
PORTB-->
PB.06
PB.09

===== LED =====
PORTD-->
PD.12
PD.13
PD.14
PD.15

===== SD Card =====
PORTC-->
PORTD-->
PORTH-->
PC.08 
PC.09 
PC.10 
PC.11 
PC.12 
PD.02 
PH.13


====== ADC =====
PORTC-->
PC.02

====== LMP 91k =====
PORTD-->
PD.00 - откл
PD.01 - откл
PD.02 - откл
PD.03 - откл

PD.06
PD.07
PD.08
PD.09

===== USART Dynament =====
PORTA-->
PA.02
PA.03

====== KEYB ======
PORTB--> строки
PB.04
PB.05

PORTD--> столбцы
PD.00 - A
PD.01 - B
PD.02 - C
PD.03 - D

===== SPI Disp =====
PORTB--> Управление. 
Перекинули с PA2 и PA3
PB.00 - Reset
PB.01 - CD

PORTB--> SPI
PB.10 - SCK
PB.14 - MISO, для этого дисплея не нужен
PB.15 - MOSI

