//////////////////////////// КОМПЬЮТЕРНЫЕ КОЛОНКИ (MINI MUSIC CENTER) ///////////////////////////
////////////////////////////////////// ВЕРСИЯ (VERSION) 1.8 /////////////////////////////////////
///////////////////// КОД ОТ АНАТОЛИЯ НЕВЗОРОВА (CODE BY ANATOLY NEVZOROFF) /////////////////////
////////////////////////////// https://github.com/AnatolyNevzoroff //////////////////////////////


//КОДЫ КНОПОК (БОЛЬШОЙ ПУЛЬТ)
#define IR_VOL_UP 0x8F807    //Кнопка ГРОМКОСТЬ "+" (БОЛЬШЕ)
#define IR_VOL_DOWN 0x802FD  //Кнопка ГРОМКОСТЬ "-" (МЕНЬШЕ)
#define IR_MENU_UP 0x8F00F   //Кнопка ВВЕРХ ПО МЕНЮ
#define IR_MENU_DOWN 0x808F7 //Кнопка ВНИЗ ПО МЕНЮ
#define IR_PREV_MENU 0x8827D //Кнопка ПРЕДЫДУЩЕЕ МЕНЮ
#define IR_NEXT_MENU 0x8E817 //Кнопка СЛЕДУЮЩЕЕ МЕНЮ
#define IR_POWER 0x800FF     //Кнопка POWER/STANDBY (ON/OFF)
#define IR_MUTE 0x858A7      //Кнопка MUTE (ON/OFF)
#define IR_PHONE 0x8B847     //Кнопка HEADPHONE (наушники)
#define IR_MAIN_MENU 0x8D827 //Кнопка ГЛАВНОЕ МЕНЮ
#define IR_FFFFF 0xFFFFFFFF  //Команда ПОВТОРА

#define text_ON "ON "
#define text_OFF "OFF"
#define text_Q "Q-FACTOR "


#define ENCODER_PIN_A 2    //D2 К ВЫВОДУ ЭНКОДЕРА (DT ИЛИ S1) 
#define ENCODER_PIN_B 3    //D3 К ВЫВОДУ ЭНКОДЕРА (CLK ИЛИ S2)
#define ENCODER_key 4      //D4 КНОПКА ПЕРЕМЕЩЕНИЯ ПО МЕНЮ (КНОПКА ЭНКОДЕРА SW ДУБЛИРУЕТ IR_MENU_DOWN)
#define POWER_key 5        //D5 КНОПКА ВКЛЮЧЕНИЯ POWER/STANDBY (ДУБЛИРУЕТ IR_POWER)
#define MENU_key 6         //D6 КНОПКА ПЕРЕКЛЮЧЕНИЯ МЕЖДУ МЕНЮ (ДУБЛИРУЕТ IR_NEXT_MENU)
#define PHONE_key 7        //D7 КНОПКА ПЕРЕКЛЮЧЕНИЯ МЕЖДУ НАУШНИКАМИ И УСИЛИТЕЛЕМ (ДУБЛИРУЕТ IR_PHONE)
#define MUTE_key 8         //D8 КНОПКА ТИШИНА MUTE (ДУБЛИРУЕТ IR_MUTE)
#define BT_PWR_out 9       //D9 ПИТАНИЕ И ИНДИКАТОР BLUETOOTH МОДУЛЯ ON/OFF & LED (Синий)
#define MUTE_LED_out 10    //D10 ИНДИКАТОР ОТКЛЮЧЕНИЯ ЗВУКА MUTE LED ON/OFF (Жёлтый)
#define PHONE_LED_out 11   //D11 ПИТАНИЕ И ИНДИКАТОР НАУШНИКОВ HEADPHONES ON/OFF & LED (Зелёный)
#define POWER_LED_out 12   //D12 ИНДИКАТОР ВКЛЮЧЕНИЯ POWER LED ON/OFF (Красный)
#define POWER_RELAY_out 13 //D13 ПИТАНИЕ УСИЛИТЕЛЯ POWER RELAY ON/OFF (Белый)
#define IR_INPUT 14        //D14 (А0) ВХОД ДЛЯ IR ПРИЁМНИКА
#define AMP_START_out 15   //D15 (А1) УПРАВЛЯЕМ ВКЛЮЧЕНИЕМ УСИЛИТЕЛЕЙ С ЗАДЕРЖКОЙ (Оранжевый)


#define p10000 10000L        //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА ТИП ДАННЫХ "L" (long)
#define p2000 2000L          //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА 
#define p1000 1000L          //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА 
#define START_AMP_PAUSE 800L //ЗАДЕРЖКА ПЕРЕД ВКЛЮЧЕНИЕМ УСИЛИТЕЛЕЙ МОЩНОСТИ
#define LongPress 500L       //ДЛИТЕЛЬНАЯ СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА КНОПКИ
#define ShortPress 200L      //КОРОТКАЯ СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА КНОПКИ
#define IR_LED_delay 100L    //ЗАДЕРЖКА ДЛЯ ИНДИКАЦИИ ЧТО КОМАНДЫ С IR ПДУ ПОЛУЧЕНЫ

// #define LOG_ENABLE //ВЫВОД ДАННЫХ В СЕРИЙНЫЙ ПОРТ ДЛЯ ОТКЛЮЧЕНИЯ УДАЛИТЬ


#include <Wire.h>//(МОЖНО НЕ УКАЗЫВАТЬ, ЗАПУСКАЕТСЯ ИЗ ДРУГИХ БИБЛИОТЕК)
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "Rotary.h"
#include <BD37534FV.h>// https://github.com/liman324/BD37534FV/archive/master.zip (ЕСТЬ В АРХИВЕ)
// ВНИМАНИЕ!!! ИСПОЛЬЗУЕТСЯ ВЕРСИЯ 2.2.3.  БИБЛИТЕКИ IRremote.h  (ЕСТЬ В АРХИВЕ)
#include <IRremote.h>// https://github.com/Arduino-IRremote/Arduino-IRremote  ONLY VERSION 2.2.3.!!!
#include <DS3231.h>// http://www.rinkydinkelectronics.com/download.php?f=DS3231.zip (ЕСТЬ В АРХИВЕ)


Rotary encoder=Rotary(ENCODER_PIN_A, ENCODER_PIN_B);//ОБЪЯВЛЯЕМ ПИНЫ ЭНКОДЕРА DT (S2) и CLK (S1)
LiquidCrystal_I2C lcd(0x27,16,2);//Устанавливаем дисплей
IRrecv irrecv(IR_INPUT);//Указываем D14 вывод подключения IR приемника
decode_results ir;//Объявляем ir
BD37534FV bd;//Объявляем bd для бибиотеки аудиопроцессора
DS3231 rtc(SDA,SCL);Time tmr;//Подключаем модуль часов и объявляем tmr

//unsigned long от 0 до 4294967295
uint32_t timer_AUTORET,timer_MENU,timer_BTN,timer_WATCH,timer_IR;
uint32_t irkey;//ПЕРЕМЕННАЯ ДЛЯ ВРЕМЕННОГО ХРАНЕНИЯ КОДОВ КНОПОК ПДУ

//int от -32768 до 32767  
//int16_t ;

//char от -128 до 127
int8_t phone,Mute,smenu0,smenu1,smenu2,smenu3,smenu4,smenu5,smenu6,smenu7,smenu8,smenu9;
int8_t MAIN_menu,BD_menu,FAST_menu,MUTE_menu,WATCH_menu,SET_TIME_DATE_menu;
int8_t trig0,trig1,trig2,trig3,trig4,trig5,trig6,trig7,trig8;
int8_t treb_c,mid_c,bas_c,treb_q,mid_q,bas_q;
int8_t Loudness,Loudness_FR,R_amp,L_amp,R_phon,L_phon,Volume,bass,middle,treble;
int8_t Fmenu=1,TrueP,style_watch,encoderCount;
int8_t setime,sethour,setmin,setdate,setmon,setdow,setyear;

//byte от 0 до 255
uint8_t Connect[5],Input,ShowInput,InputIDX,Gain;//МАССИВ И ПЕРЕМЕННЫЕ ДЛЯ ЧУСТВИТЕЛЬНОСТИ ВХОДОВ
uint8_t MUTE_animat,power,setwatch,lastMIN,Wait_return,LCD_light;
uint8_t pointer,triangle,arrow;//СПЕЦСИМВОЛЫ
uint8_t MENU_btn,MENU_btn_status,PHONE_btn,PHONE_btn_status,MUTE_btn,MUTE_btn_status;
uint8_t POWER_btn,POWER_btn_status,ENCODER_btn,ENCODER_btn_status;

//boolean от false до true 
bool FL_autoret,FL_start_MENU,FL_start_POWER=true,FL_delay_MENU,FL_delay_ENCODER;
bool FL_IR_led,FL_com1,FL_com2;





/////////////////////////////////////////////////////////////////////////////////////////////////
//                                          S E T U P                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){

#ifdef LOG_ENABLE
Serial.begin(9600);
#endif


//Wire.begin();//ЗАПУСКАЕМ ШИНУ I2C (МОЖНО НЕ УКАЗЫВАТЬ, ЗАПУСКАЕТСЯ ИЗ ДРУГИХ БИБЛИОТЕК)
irrecv.enableIRIn();//ЗАПУСКАЕМ ПРИЁЁМ ДАНННЫХ С ИНФРАКРАСНОГО ПРИЁМНИКА
rtc.begin();//ЗАПУСКАЕМ МОДУЛЬ ЧАСОВ
lcd.init();//ЗАПУСКАЕМ ЖИДКОКРИСТАЛИЧЕСКИЙ ЭКРАН


pinMode(ENCODER_PIN_A,INPUT);//КОНТАКТ ЭНКОДЕРА (ЕСЛИ ЭНКОДЕР БЕЗ ПИТАНИЯ ТО INPUT_PULLUP)
pinMode(ENCODER_PIN_B,INPUT);//КОНТАКТ ЭНКОДЕРА (ЕСЛИ ЭНКОДЕР БЕЗ ПИТАНИЯ ТО INPUT_PULLUP)
pinMode(ENCODER_key,INPUT);//D4 КНОПКА ПОДМЕНЮ SUBMENU KEY (Кнопка SW энкодера)
pinMode(POWER_key,INPUT_PULLUP);//D5 КНОПКА ВКЛЮЧЕНИЯ POWER/STANDBY KEY
pinMode(MENU_key,INPUT_PULLUP);//D6 КНОПКА МЕНЮ MENU KEY
pinMode(PHONE_key,INPUT_PULLUP);//D7 КНОПКА НАУШНИКИ HEADPHONES KEY
pinMode(MUTE_key,INPUT_PULLUP);//D8 КНОПКА ТИШИНА MUTE KEY
pinMode(BT_PWR_out,OUTPUT);//D9 ПИТАНИЕ МОДУЛЯ BLUETOOTH POWER ON/OFF И ИНДИКАТОР БТ (Синий)
pinMode(MUTE_LED_out,OUTPUT);//D10 ИНДИКАТОР ОТКЛЮЧЕНИЯ ЗВУКА MUTE LED ON/OFF (Жёлтый)
pinMode(PHONE_LED_out,OUTPUT);//D11 ПИТАНИЕ НАУШНИКОВ HEADPHONES ON/OFF И ИНДИКАТОР LED (Зелёный)
pinMode(POWER_LED_out,OUTPUT);//D12 ИНДИКАТОР ВКЛЮЧЕНИЯ POWER LED ON/OFF (Красный)
pinMode(POWER_RELAY_out,OUTPUT);//D13 ПИТАНИЕ УСИЛИТЕЛЯ POWER RELAY ON/OFF (Белый)
pinMode(IR_INPUT,INPUT);//D14 ВХОД ИК ДАТЧИКА INPUT INFRARED RECEIVER
pinMode(AMP_START_out,OUTPUT);//D15 ДЛЯ ЗАДЕРЖКИ ВЫХОДА TDA7293 ИЗ STANDBY НА 0,7 сек. (Оранжевый)

digitalWrite(POWER_RELAY_out,HIGH);//ОТКЛЮЧАЕМ ПИТАНИЕ СИЛОВЫХ БП ПИТАЮЩИХ ТОЛЬКО УСИЛИТЕЛЬ


//ФУНКЦИЯ "rotaryEncoder" ВЫЗЫВАЕМАЯ ПРЕРЫВАНИЕМ 
attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A),rotaryEncoder,CHANGE);
attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B),rotaryEncoder,CHANGE);


/////////////////////////////////////////////////////////////////////////////////////////////////
//                                          EEPROM READ                                        //
//                           СЧИТЫВАЕМ ЗНАЧЕНИЯ ПЕРЕМЕННЫХ ИЗ EEPROM                           //
/////////////////////////////////////////////////////////////////////////////////////////////////
//for(int i=0;i<51;i++){EEPROM.update(i,0);}//очистка памяти при первом включении 
Volume=EEPROM.read(0)-79;
treble=EEPROM.read(1)-20;middle=EEPROM.read(2)-20;bass=EEPROM.read(3)-20;Input=EEPROM.read(4);
style_watch=EEPROM.read(8);Loudness=EEPROM.read(9);Loudness_FR=EEPROM.read(10);
R_amp=EEPROM.read(11)-79;R_phon=EEPROM.read(13)-79;
L_amp=EEPROM.read(12)-79;L_phon=EEPROM.read(14)-79;
treb_c=EEPROM.read(16);mid_c=EEPROM.read(17);bas_c=EEPROM.read(18);
treb_q=EEPROM.read(20);mid_q=EEPROM.read(21);bas_q=EEPROM.read(22);
Wait_return=EEPROM.read(25);LCD_light=EEPROM.read(26);phone=EEPROM.read(27);
for(byte d=0;d<5;d++){Connect[d]=EEPROM.read(30+d);}
//if(Wait_return<5)Wait_return=15;//Установка Wait_return=15 сек.


//СОЗДАЁМ И ЗАПИСЫВАЕМ В ПАМЯТЬ LCD 1602 СПЕЦСИМВОЛЫ
byte v1[8]={31,31,31,31,31,31,31,31};lcd.createChar(1,v1);//ПОЛНОЕ ЗАПОЛНЕНИЕ СИМВОЛА
byte v2[8]={31,31,31, 0, 0, 0, 0, 0};lcd.createChar(2,v2);//ВЕРХНЯЯ ПЕРЕКЛАДИНА
byte v3[8]={ 0, 0, 0, 0, 0,31,31,31};lcd.createChar(3,v3);//НИЖНЯЯ ПЕРЕКЛАДИНА
byte v4[8]={31,31, 0, 0, 0, 0,31,31};lcd.createChar(4,v4);//ВЕРХНЯЯ И НИЖНЯЯ ПЕРЕКЛАДИНА
byte v5[8]={ 0, 0, 0, 0, 0, 0,31,31};lcd.createChar(5,v5);//НИЖНЯЯ ТОНКАЯ ПЕРЕКЛАДИНА
byte v6[8]={24,28,30,31,30,28,24, 0};lcd.createChar(6,v6);//ТРЕУГОЛЬНИК (ЗАПОЛНЕННЫЙ) 
byte v7[8]={24,28,14, 7,14,28,24, 0};lcd.createChar(7,v7);//СТРЕЛОЧКА (ПОЛАЯ)
triangle=uint8_t(6);//ТРЕУГОЛЬНИК (ЗАПОЛНЕННЫЙ) 
arrow=uint8_t(7);   //СТРЕЛОЧКА (ПОЛАЯ) 

}//END SETUP 






/////////////////////////////////////////////////////////////////////////////////////////////////
//                                           L O O P                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

//mill=millis();//ПЕРЕЗАПИСЬ ПЕРЕМЕННОЙ mill

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                      IR REMOTE CONTROL                                      //
//                      ОБРАБАТЫВАЕМ КОМАНДЫ С ИНФРАКРАСНОГО ПРИЁМНИКА                         //
/////////////////////////////////////////////////////////////////////////////////////////////////
if(irrecv.decode(&ir)){//ЕСЛИ ПРИ ОПРОСЕ IR ПРИЁМНИКА В ЕГО БУФЕРЕ ИМЕЕТСЯ КОД КНОПКИ

#ifdef LOG_ENABLE
Serial.print("0x");Serial.println(ir.value,HEX);//В МОНИТОРЕ ПОРТА МОЖНО ПОСМОТРЕТЬ КОД КНОПКИ 
#endif

//ЗАЖИГАЕМ ИНДИКАТОР "MUTE" ВНЕ ЗАВИСИМОСТИ ОТ СОДЕРЖИМОГО БУФЕРА IR ПРИЁМНИКА
digitalWrite(MUTE_LED_out,HIGH);FL_IR_led=true;

//ЕСЛИ С ПДУ ПОЛУЧЕН КОД КНОПКИ НЕ ЯВЛЯЮЩИЙСЯ КОДОМ ПОВТОРА, ЗАПИСЫВАЕМ КОД КНОПКИ 
//ВО ВРЕМЕННУЮ ПЕРЕМЕННУЮ irkey, И БЛОКИРУЕМ МК НА ПРИЁМ ЛЮБЫХ КОДОВ СОЗДАВАЯ ПАУЗУ, 
//ТЕМ САМЫМ ПРЕДОТВРАЩАЕМ ВЫПОЛНЕНИЕ КОДОВ ПОВТОРА С ОЧЕНЬ КОРОТКОЙ ПАУЗОЙ ЗАДАННОЙ ПДУ
if(ir.value!=IR_FFFFF){irkey=ir.value;delay(ShortPress);}

//СБРАСЫВАЕМ ТЕКУЩЕЕ ЗНАЧЕНИЕ ПОЛУЧЕННОГО КОДА И ПЕРЕКЛЮЧАЕМ ИК ПРИЁМНИК НА ПРИЁМ СЛЕДУЮЩЕГО
irrecv.resume();}

//ГАСИМ ИНДИКАТОР "MUTE" ПОСЛЕ ЗАДЕРЖКИ ОДНОКРАТНО МГНОВЕННО, ВСЕ ПОСЛЕДУЮЩИЕ СПУСТЯ IR_LED_delay
if(FL_IR_led==true&&millis()-timer_IR>IR_LED_delay){
digitalWrite(MUTE_LED_out,LOW);timer_IR=millis();
   FL_IR_led=false;}




/////////////////////////////////////////////////////////////////////////////////////////////////
//                                КНОПКА ВКЛЮЧЕНИЯ POWER ON/OFF                                //
/////////////////////////////////////////////////////////////////////////////////////////////////
POWER_btn=digitalRead(POWER_key);
if(POWER_btn!=POWER_btn_status||ir.value==IR_POWER){delay(30);POWER_btn=digitalRead(POWER_key);
if(POWER_btn==LOW&&POWER_btn_status==HIGH||ir.value==IR_POWER){
power=!power;FL_start_POWER=true;ir.value=0;
  }POWER_btn_status=POWER_btn;}






//////////////////////////////////////////////////////////////////////////////////////////////////
//                                       РЕЖИМ POWER ON                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////
if(power==true){

//УСТАНАВЛИВАЕМ НАЧАЛЬНЫЕ ПАРАМЕТРЫ ПРИ ВКЛЮЧЕНИИ 
if(FL_start_POWER==true){
Gain=Connect[Input];Mute=false;//ОТКЛЮЧАЕМ РЕЖИМ ТИШИНЫ (MUTE OFF)
lcd.backlight();PHONE_MODE();encoderCount=0;smenu0=0;FL_com1=true;
MAIN_menu=true;FL_start_MENU=true;BD_menu=false;FAST_menu=false;FL_autoret=false;
digitalWrite(POWER_LED_out,HIGH);//POWER LED ON 
audio();
   FL_start_POWER=false;}




/////////////////////////////////////////////////////////////////////////////////////////////////
//                                            MUTE KEY                                         //
//                           КНОПКА ПЕРЕКЛЮЧЕНИЯ РЕЖИМА MUTE ON/OFF                            //
/////////////////////////////////////////////////////////////////////////////////////////////////
MUTE_btn=digitalRead(MUTE_key);
if(MUTE_btn!=MUTE_btn_status||ir.value==IR_MUTE){delay(30);MUTE_btn=digitalRead(MUTE_key);
if(MUTE_btn==LOW&&MUTE_btn_status==HIGH||ir.value==IR_MUTE){
Mute=!Mute;bd.setIn_gain(Gain,Mute);ir.value=0;lcd.clear();
switch(Mute){
//MUTE OFF 
  case 0: MAIN_menu=true;MUTE_menu=false;if(BD_menu){Fmenu=BD_menu;}BD_menu=false;FAST_menu=false;
digitalWrite(MUTE_LED_out,LOW);smenu0=0;FL_start_MENU=true;break;
//MUTE ON 
  case 1: MUTE_menu=true;MAIN_menu=false;if(BD_menu){Fmenu=BD_menu;}BD_menu=false;FAST_menu=false;
digitalWrite(MUTE_LED_out,HIGH);FL_autoret=false;break;}
  }MUTE_btn_status=MUTE_btn;}




/////////////////////////////////////////////////////////////////////////////////////////////////
//                                         MODE MUTE OFF                                       //
//                  РЕЖИМ "MUTE" ВЫКЛЮЧЕН, АКТИВИРУЕМ ВСЕ КНОПКИ И РЕЖИМЫ                      //
/////////////////////////////////////////////////////////////////////////////////////////////////
if(Mute==false){




/////////////////////////////////////////////////////////////////////////////////////////////////
//                                 HEADPHONES / AMPLIFIER KEY                                  //
//                    КНОПКА ПЕРЕКЛЮЧЕНИЯ МЕЖДУ НАУШНИКАМИ И УСИЛИТЕЛЕМ                        //
/////////////////////////////////////////////////////////////////////////////////////////////////
PHONE_btn=digitalRead(PHONE_key);
if(PHONE_btn!=PHONE_btn_status||ir.value==IR_PHONE){delay(30);PHONE_btn=digitalRead(PHONE_key);
if(PHONE_btn==LOW&&PHONE_btn_status==HIGH||ir.value==IR_PHONE){
if(MAIN_menu==false){if(BD_menu){Fmenu=BD_menu;}BD_menu=false;FAST_menu=false;
MAIN_menu=true;smenu0=0;FL_autoret=false;lcd.clear();}
else{phone=!phone;PHONE_MODE();}
ir.value=0;FL_start_MENU=true;
  }PHONE_btn_status=PHONE_btn;}




////////////////////////////////////////////////////////////////////////////////////////////////
//                   КНОПКИ ПЕРЕКЛЮЧЕНИЯ МЕЖДУ МЕНЮ ПАРАМЕТРОВ НА IR ПУЛЬТЕ                   //
////////////////////////////////////////////////////////////////////////////////////////////////
//СЛЕДУЮЩЕЕ ИЛИ ПРЕДЫДУЩЕЕ МЕНЮ 
if(ir.value==IR_PREV_MENU||ir.value==IR_NEXT_MENU){
MAIN_menu=false;FAST_menu=false;pointer=triangle;
if(BD_menu==false){BD_menu=Fmenu;}else{
if(ir.value==IR_NEXT_MENU){BD_menu++;if(BD_menu>6){BD_menu=1;}}
if(ir.value==IR_PREV_MENU){BD_menu--;if(BD_menu<1){BD_menu=6;}}}
FL_autoret=true;timer_AUTORET=millis();ir.value=0;FL_start_MENU=true;lcd.clear();
}//END




////////////////////////////////////////////////////////////////////////////////////////////////
//                                           MENU KEY                                         //
//  КНОПКА ПОСЛЕДОВАТЕЛЬНОГО ПЕРЕКЛЮЧЕНИЯ МЕЖДУ МЕНЮ НАСТРОЙКИ BD37534FV И ПРОЧИХ ПАРАМЕТРОВ  //
//                 ИЛИ ВЫЗОВ РЕЖИМА БЫСТРОГО ПЕРЕКЛЮЧЕНИЯ МЕНЮ (ДЛЯ ЭНКОДЕРА)                 //
////////////////////////////////////////////////////////////////////////////////////////////////

MENU_btn=digitalRead(MENU_key);
if(MENU_btn!=MENU_btn_status){delay(30);MENU_btn=digitalRead(MENU_key);
if(MENU_btn==LOW&&MENU_btn_status==HIGH){
FL_delay_MENU=true;timer_BTN=millis();//ПОДНИМАЕМ ФЛАГ КНОПКИ, ОБНУЛЯЕМ ТАЙМЕР
  }MENU_btn_status=MENU_btn;}


//ЕСЛИ КНОПКА "MENU" БЫЛА НАЖАТА, ОТПУЩЕНА, И ЗАКОНЧИЛАСЬ "КОРОТКАЯ ПАУЗА" 
if(FL_delay_MENU==true&&MENU_btn==HIGH&&millis()-timer_BTN>ShortPress){FL_delay_MENU=false;
MAIN_menu=false;//ОТКЛЮЧАЕМ ГЛАВНОЕ МЕНЮ
FAST_menu=false;pointer=triangle;//ОТКЛЮЧАЕМ МЕНЮ БЫСТРОГО ПЕРЕКЛЮЧЕНИЯ
if(BD_menu==false){BD_menu=Fmenu;}else{BD_menu++;if(BD_menu>6){BD_menu=1;}}
FL_autoret=true;timer_AUTORET=millis();FL_start_MENU=true;lcd.clear();
}//END   


//УДЕРЖИВАЯ КНОПКУ "MENU" БОЛЕ ЧЕМ "ДОЛГОЕ НАЖАТИЕ" ПЕРЕХОДИМ В РЕЖИМ 
//БЫСТРОГО ПЕРЕКЛЮЧЕНИЯ МЕЖДУ МЕНЮ С ПОМОЩЬЮ ЭНКОДЕРА
if(FL_delay_MENU==true&&MENU_btn==LOW&&millis()-timer_BTN>LongPress){FL_delay_MENU=false;
if(BD_menu){Fmenu=BD_menu;}
MAIN_menu=false;BD_menu=false;pointer=arrow;FAST_menu=true;
FL_autoret=true;timer_AUTORET=millis();FL_com2=true;lcd.clear();
}//END




////////////////////////////////////////////////////////////////////////////////////////////////
//                                        MODE FAST MENU                                      //
//                           РЕЖИМ БЫСТРОГО ПЕРЕКЛЮЧЕНИЯ МЕЖДУ МЕНЮ                           //
////////////////////////////////////////////////////////////////////////////////////////////////
if(FAST_menu==true){
//КНОПКОЙ ЭНКОДЕРА ПЕРЕКЛЮЧАЕМСЯ ИЗ РЕЖИМА БЫСТРОГО МЕНЮ В ТЕКУЩЕЕ МЕНЮ 
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status){delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH){
BD_menu=Fmenu;pointer=triangle;FAST_menu=false;timer_AUTORET=millis();FL_com2=true;
  }ENCODER_btn_status=ENCODER_btn;}

if(encoderCount!=0){Fmenu=Fmenu+encoderCount;timer_AUTORET=millis();FL_com2=true;
encoderCount=0;if(Fmenu>6){Fmenu=1;}if(Fmenu<1){Fmenu=6;}lcd.clear();}

if(FL_com2==true){
switch(Fmenu){
  case 1: ShowBDmenu_1();break;
  case 2: ShowBDmenu_2();break;
  case 3: ShowBDmenu_3();break;
  case 4: ShowBDmenu_4();break;
  case 5: ShowBDmenu_5();break;
  case 6: ShowBDmenu_6();break;}
//  case 7: ShowBDmenu_7();break;case 8: ShowBDmenu_8();break;case 9: ShowBDmenu_9();break;}
  FL_com2=false;}

}//END FAST MENU




/////////////////////////////////////////////////////////////////////////////////////////////////
//                                   RETURN TO THE MAIN MENU                                   //
//      ВОЗВРАТ В ГЛАВНОЕ МЕНЮ (ЧЕРЕЗ ЗАДАННЫЙ ИНТЕРВАЛ БЕЗДЕЙСТВИЯ ИЛИ КНОПКОЙ С ПУЛЬТА)      //
/////////////////////////////////////////////////////////////////////////////////////////////////
if(FL_autoret==true&&millis()-timer_AUTORET>Wait_return*p1000||ir.value==IR_MAIN_MENU){
if(BD_menu){Fmenu=BD_menu;}FAST_menu=false;BD_menu=false;ir.value=0;
if(MAIN_menu==false){lcd.clear();}
MAIN_menu=true;smenu0=0;FL_start_MENU=true;pointer=triangle;
   FL_autoret=false;}




}//END MODE "MUTE" 




/////////////////////////////////////////////////////////////////////////////////////////////////
//                                        РЕЖИМ "MUTE"                                         //
//                     ПОПЕРЕМЕННО ВЫВОДИМ НА ЭКРАН ДАТУ/ВРЕМЯ ИЛИ "MUTE"                      //
/////////////////////////////////////////////////////////////////////////////////////////////////
if(Mute==true){
if(millis()>timer_MENU){MUTE_animat=!MUTE_animat;timer_MENU=millis()+p2000;
lcd.setCursor(0,MUTE_animat);lcd.print(F("      MUTE      "));
if(MUTE_animat){timedow();}else{datetemp();}}}







////////////////////////////////////////////////////////////////////////////////////////////////
//                                  ГЛАВНОЕ МЕНЮ (MAIN MENU)                                  //
//              VOLUME -79...+15 dB / INPUT SELECTION / GAIN INPUT 0...+20 dB                 //
////////////////////////////////////////////////////////////////////////////////////////////////
if(MAIN_menu==true){
if(FL_start_MENU==true){ShowVOLUME_menu();ShowINPUT_menu();ShowInput=Input;
   FL_start_MENU=false;}


//ОБРАБАТЫВАЕМ НАЖАТИЕ НА КНОПКУ ЭНКОДЕРА
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status){delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH){
FL_delay_ENCODER=true;timer_BTN=millis();//ПОДНИМАЕМ ФЛАГ КНОПКИ, ОБНУЛЯЕМ ТАЙМЕР
  }ENCODER_btn_status=ENCODER_btn;}


//ЕСЛИ КНОПКА ЭНКОДЕРА БЫЛА НАЖАТА, ОТПУЩЕНА, И ЗАКОНЧИЛАСЬ "КОРОТКАЯ ПАУЗА"
if(FL_delay_ENCODER==true&&ENCODER_btn==HIGH&&millis()-timer_BTN>ShortPress){
//ПЕРЕКЛЮЧАЕМСЯ МЕЖДУ ВЫБОРОМ ВХОДА И УРОВНЕМ ГРОМКОСТИ    
smenu0++;if(smenu0>1){smenu0=0;}FL_com1=true;
   FL_delay_ENCODER=false;}


//УДЕРЖИВАЯ КНОПКУ ЭНКОДЕРА БОЛЕ ЧЕМ "ДОЛГОЕ НАЖАТИЕ" 
//ПЕРЕКЛЮЧАЕМСЯ В РЕЖИМ ИЗМЕНЕНИЯ ЧУСТВИТЕЛЬНОСТИ ВХОДОВ
if(FL_delay_ENCODER==true&&ENCODER_btn_status==LOW&&millis()-timer_BTN>LongPress){
if(smenu0==2){smenu0=1;}
else{smenu0=2;}FL_com1=true;
   FL_delay_ENCODER=false;}


//ПЕРЕКЛЮЧАЕМСЯ МЕЖДУ РЕЖИМАМИ КНОПКАМИ С ПУЛЬТА
if(ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
//ВАРИАНТ 1. КНОПКИ "ВНИЗ" И "ВВЕРХ" ДЕЙСТВУЮТ ОДИНАКОВО (МНЕ ТАК УДОБНЕЕ)
smenu0++;if(smenu0>2){smenu0=0;}ir.value=0;FL_com1=true;}
//ВАРИАНТ 2. КНОПКИ ДЕЙСТВУЮТ В СВОИХ НАПРАВЛЕНИЯХ
//if(ir.value==IR_MENU_UP){smenu0--;if(smenu0<0){smenu0=2;}ir.value=0;FL_com1=true;}


//ОБРАБАТЫВАЕМ КОМАНДЫ С КНОПКИ ЭНКОДЕРА И КНОПОК ПУЛЬТА
if(FL_com1==true){
switch(smenu0){

//РЕЖИМ РЕГУЛИРОВКИ ГРОМКОСТИ И ФИЗИЧЕСКОГО ПЕРЕКЛЮЧЕНИЯ ВХОДА (ЕСЛИ ОН БЫЛ ИЗМЕНЁН)
  case 0: trig0=Volume;pointer=triangle;
if(ShowInput!=Input){Input=ShowInput;bd.setIn(Input-1);}
FL_autoret=false;BT_LED();break;

//РЕЖИМ ВЫБОРА ВХОДА БЕЗ ФАКТИЧЕСКОГО ПЕРЕКЛЮЧЕНИЯ 
  case 1: trig0=Input;InputIDX=Input;pointer=arrow;cl_TMR();break;

//РЕЖИМ РЕГУЛИРОВКИ УСИЛЕНИЯ ВЫБРАННОГО (ИЛИ ОСТАВЛЕННОГО БЕЗ ИЗМЕНЕНИЯ) ВХОДА.
//ЕСЛИ ВХОД БЫЛ ИЗМЕНЁН, ФИЗИЧЕСКИ ПЕРЕКЛЮЧАЕМ ЕГО АУДИОПРОЦЕССОРОМ
  case 2: if(ShowInput!=Input){Input=ShowInput;bd.setIn(Input-1);}
Gain=Connect[Input];trig0=Gain;pointer=triangle;cl_TMR();BT_LED();break;}

//ПЕРЕПЕЧАТЫВАЕМ МЕНЮ ПРИ КАЖДОМ ИЗМЕНЕНИИ РЕЖИМА
ShowVOLUME_menu();ShowINPUT_menu();
   FL_com1=false;}


//ЭНКОДЕРОМ ИЛИ КНОПКАМИ ПУЛЬТА МЕНЯЕМ ПАРАМЕТРЫ РЕЖИМОВ
if(encoderCount!=0){trig0=trig0+encoderCount;encoderCount=0;FL_com2=true;}
if(ir.value==IR_VOL_UP||ir.value==IR_FFFFF&&irkey==IR_VOL_UP){trig0++;ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN||ir.value==IR_FFFFF&&irkey==IR_VOL_DOWN){trig0--;ir.value=0;FL_com2=true;}


if(FL_com2==true){
switch(smenu0){
//МЕНЯЕМ ГРОМКОСТЬ
  case 0: Volume=constrain(trig0,-79,15);bd.setVol(Volume);trig0=Volume;
ShowVOLUME_menu();break;
//МЕНЯЕМ ПЕРЕМЕННУЮ ОТВЕЧАЮЩУЮ ТОЛЬКО ЗА ОТОБРАЖЕНИЕ ВЫБИРАЕМОГО ВХОДА (БЕЗ ПЕРЕКЛЮЧЕНИЯ)
  case 1: if(trig0>4){trig0=1;}if(trig0<1){trig0=4;}InputIDX=trig0;timer_AUTORET=millis();
ShowINPUT_menu();break;
//МЕНЯЕМ ЧУСТВИТЕЛЬНОСТЬ ВЫБРАННОГО ВХОДА
  case 2: Gain=constrain(trig0,0,20);trig0=Gain;Connect[Input]=Gain;bd.setIn_gain(Gain,Mute);
timer_AUTORET=millis();
ShowINPUT_menu();break;}
   FL_com2=false;}


}//END MAIN MENU




////////////////////////////////////////////////////////////////////////////////////////////////
//                         МЕНЮ НАСТРОЙКИ ЭКВАЛАЙЗЕРА (EQUALIZER MENU)                        //
//                              BASS, MIDDLE, TREBLE -20...+20 dB                             //
////////////////////////////////////////////////////////////////////////////////////////////////
if(BD_menu==1){
if(FL_start_MENU==true){ShowBDmenu_1();FL_start_MENU=false;}

ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status||ir.value==IR_MENU_DOWN){
delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH||ir.value==IR_MENU_DOWN){
smenu1++;if(smenu1>2){smenu1=0;}FL_com1=true;
  }ENCODER_btn_status=ENCODER_btn;}

if(ir.value==IR_MENU_UP){smenu1--;if(smenu1<0){smenu1=2;}FL_com1=true;}

if(FL_com1==true){ir.value=0;cl_TMR();
  switch(smenu1){
    case 0: trig1=bass;break;
    case 1: trig1=middle;break;
    case 2: trig1=treble;break;}
ShowBDmenu_1();
   FL_com1=false;}

if(encoderCount!=0){trig1=trig1+encoderCount;encoderCount=0;FL_com2=true;}
if(ir.value==IR_VOL_UP||ir.value==IR_FFFFF&&irkey==IR_VOL_UP){trig1++;ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN||ir.value==IR_FFFFF&&irkey==IR_VOL_DOWN){trig1--;ir.value=0;FL_com2=true;}

if(FL_com2==true){
trig1=constrain(trig1,-20,20);cl_TMR();//Для BD37033FV trig1=constrain(trig1,-15,15);
  switch(smenu1){
    case 0: bass=trig1;break;
    case 1: middle=trig1;break;
    case 2: treble=trig1;break;}
audio();ShowBDmenu_1();
   FL_com2=false;}
}//END EQUALIZER MENU




////////////////////////////////////////////////////////////////////////////////////////////////
//                    LOUDNESS FREQUENCY & GAIN / WAIT RETURN TO MAIN MENU                    //
//  УРОВЕНЬ ФОРМИРОВАНИЯ АЧХ И ЧАСТОТЫ СРЕЗА ДЛЯ РЕЖИМА LOUDNESS, ПАУЗА ПЕРЕД АВТОВОЗВРАТОМ   // 
////////////////////////////////////////////////////////////////////////////////////////////////
if(BD_menu==2){
if(FL_start_MENU==true){ShowBDmenu_2();FL_start_MENU=false;}

//КНОПКОЙ ЭНКОДЕРА ВЫБИРАЕМ МЕЖДУ ПАРАМЕТРАМИ LOUDNESS И АВТОВОЗВРАТОМ
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status||ir.value==IR_MENU_DOWN){
delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH||ir.value==IR_MENU_DOWN){
smenu2++;if(smenu2>2){smenu2=0;}FL_com1=true;
  }ENCODER_btn_status=ENCODER_btn;}

if(ir.value==IR_MENU_UP){smenu2--;if(smenu2<0){smenu2=2;}FL_com1=true;}

if(FL_com1==true){ir.value=0;cl_TMR();
  switch(smenu2){
    case 0: trig2=Loudness;break;
    case 1: trig2=Loudness_FR;break;
    case 2: trig2=Wait_return;break;}
ShowBDmenu_2();
   FL_com1=false;}

if(encoderCount!=0){trig2=trig2+encoderCount;encoderCount=0;FL_com2=true;}
if(ir.value==IR_VOL_UP||ir.value==IR_FFFFF&&irkey==IR_VOL_UP){trig2++;ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN||ir.value==IR_FFFFF&&irkey==IR_VOL_DOWN){trig2--;ir.value=0;FL_com2=true;}

if(FL_com2==true){cl_TMR();
switch(smenu2){
  case 0: Loudness=constrain(trig2,0,20);trig2=Loudness;break;
  case 1: if(trig2>3){trig2=0;}if(trig2<0){trig2=3;}Loudness_FR=trig2;break;
  case 2: Wait_return=constrain(trig2,5,30);trig2=Wait_return;break;}
audio();ShowBDmenu_2();
   FL_com2=false;}
}//END LOUDNESS FREQUENCY & GAIN AND WAIT RETURN TO MAIN MENU 




////////////////////////////////////////////////////////////////////////////////////////////////
//                                    BASS CENTER & Q-FACTOR                                  //
//  МЕНЮ ВЫБОРА ЧАСТОТЫ РЕГУЛИРОВКИ В ОБЛАСТИ НИЗКИХ ЧАСТОТ И ДОБРОТНОСТИ ВЫБРАННОЙ ЧАСТОТЫ   // 
////////////////////////////////////////////////////////////////////////////////////////////////
if(BD_menu==3){
if(FL_start_MENU==true){ShowBDmenu_3();FL_start_MENU=false;}
//КНОПКОЙ ЭНКОДЕРА ВЫБИРАЕМ МЕЖДУ ВЫБОРОМ ЧАСТОТЫ И ДОБРОТНОСТЬЮ
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
smenu3=!smenu3;ir.value=0;cl_TMR();FL_com1=true;
  }ENCODER_btn_status=ENCODER_btn;}

if(FL_com1==true){
  switch(smenu3){
    case 0: trig3=bas_c;break;
    case 1: trig3=bas_q;break;}
ShowBDmenu_3();
   FL_com1=false;}

if(encoderCount!=0){trig3=trig3+encoderCount;encoderCount=0;cl_TMR();FL_com2=true;}
if(ir.value==IR_VOL_UP){trig3++;cl_TMR();ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN){trig3--;cl_TMR();ir.value=0;FL_com2=true;}

if(FL_com2==true){if(trig3>3){trig3=0;}if(trig3<0){trig3=3;};
  switch(smenu3){
    case 0: bas_c=trig3;break;
    case 1: bas_q=trig3;break;}
audio();ShowBDmenu_3();
   FL_com2=false;}
}//END BASS CENTER & Q-FACTOR MENU    




////////////////////////////////////////////////////////////////////////////////////////////////
//                                MIDDLE CENTER & Q-FACTOR                                    //
//  МЕНЮ ВЫБОРА ЧАСТОТЫ РЕГУЛИРОВКИ В ОБЛАСТИ СРЕДНИХ ЧАСТОТ И ДОБРОТНОСТИ ВЫБРАННОЙ ЧАСТОТЫ  // 
////////////////////////////////////////////////////////////////////////////////////////////////
if(BD_menu==4){
if(FL_start_MENU==true){ShowBDmenu_4();FL_start_MENU=false;}
//КНОПКОЙ ЭНКОДЕРА ВЫБИРАЕМ МЕЖДУ ВЫБОРОМ ЧАСТОТЫ И ДОБРОТНОСТЬЮ
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
smenu4=!smenu4;ir.value=0;cl_TMR();FL_com1=true;
  }ENCODER_btn_status=ENCODER_btn;}

if(FL_com1==true){
  switch(smenu4){
    case 0: trig4=mid_c;break;
    case 1: trig4=mid_q;break;}
ShowBDmenu_4();
   FL_com1=false;}
   
if(encoderCount!=0){trig4=trig4+encoderCount;cl_TMR();encoderCount=0;FL_com2=true;}
if(ir.value==IR_VOL_UP){trig4++;cl_TMR();ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN){trig4--;cl_TMR();ir.value=0;FL_com2=true;}

if(FL_com2==true){if(trig4>3){trig4=0;}if(trig4<0){trig4=3;}
  switch(smenu4){
    case 0: mid_c=trig4;break;
    case 1: mid_q=trig4;break;}
audio();ShowBDmenu_4();
   FL_com2=false;}
}//END MIDDLE CENTER & Q-FACTOR MENU




////////////////////////////////////////////////////////////////////////////////////////////////
//                                  TREBLE CENTER & Q-FACTOR                                  //
//  МЕНЮ ВЫБОРА ЧАСТОТЫ РЕГУЛИРОВКИ В ОБЛАСТИ СРЕДНИХ ЧАСТОТ И ДОБРОТНОСТИ ВЫБРАННОЙ ЧАСТОТЫ  // 
////////////////////////////////////////////////////////////////////////////////////////////////
if(BD_menu==5){
if(FL_start_MENU==true){ShowBDmenu_5();FL_start_MENU=false;}
//КНОПКОЙ ЭНКОДЕРА ВЫБИРАЕМ МЕЖДУ ВЫБОРОМ ЧАСТОТЫ И ДОБРОТНОСТЬЮ
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
smenu5=!smenu5;ir.value=0;cl_TMR();FL_com1=true;
  }ENCODER_btn_status=ENCODER_btn;}

if(FL_com1==true){
  switch(smenu5){
    case 0: trig5=treb_c;break;
    case 1: trig5=treb_q;break;}
ShowBDmenu_5();
   FL_com1=false;}

if(encoderCount!=0){trig5=trig5+encoderCount;cl_TMR();encoderCount=0;FL_com2=true;}
if(ir.value==IR_VOL_UP){trig5++;cl_TMR();ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN){trig5--;cl_TMR();ir.value=0;FL_com2=true;}

if(FL_com2==true){
  switch(smenu5){
    case 0: if(trig5>3){trig5=0;}if(trig5<0){trig5=3;}treb_c=trig5;break;
    case 1: if(trig5>1){trig5=0;}if(trig5<0){trig5=1;}treb_q=trig5;break;}
audio();ShowBDmenu_5();
   FL_com2=false;}
}//END TREBLE CENTER & Q-FACTOR MENU




////////////////////////////////////////////////////////////////////////////////////////////////
//                                    OUTPUT SELECTION LEVEL                                  //
//          РЕГУЛИРОВКИ УРОВНЯ ВЫХОДНОГО СИГНАЛА ДЛЯ УСИЛИТЕЛЯ И НАУШНИКОВ (ПОКАНАЛЬНО)       //
////////////////////////////////////////////////////////////////////////////////////////////////
if(BD_menu==6){
if(FL_start_MENU==true){ShowBDmenu_6();FL_start_MENU=false;}
//КНОПКОЙ ЭНКОДЕРА ВЫБИРАЕМ КАНАЛЫ РЕГУЛИРОВКИ УРОВНЯ ВЫХОДНОГО СИГНАЛА
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status||ir.value==IR_MENU_DOWN){
delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH||ir.value==IR_MENU_DOWN){
smenu6++;if(smenu6>3){smenu6=0;}FL_com1=true;
  }ENCODER_btn_status=ENCODER_btn;}

if(ir.value==IR_MENU_UP){smenu6--;if(smenu6<0){smenu6=3;}FL_com1=true;}

if(FL_com1==true){ir.value=0;cl_TMR();
  switch(smenu6){
    case 0: trig6=L_amp;break;
    case 1: trig6=R_amp;break;
    case 2: trig6=L_phon;break;
    case 3: trig6=R_phon;break;}
ShowBDmenu_6();
   FL_com1=false;}

if(encoderCount!=0){trig6=trig6+encoderCount;encoderCount=0;FL_com2=true;}
if(ir.value==IR_VOL_UP||ir.value==IR_FFFFF&&irkey==IR_VOL_UP){trig6++;ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN||ir.value==IR_FFFFF&&irkey==IR_VOL_DOWN){trig6--;ir.value=0;FL_com2=true;}

if(FL_com2==true){trig6=constrain(trig6,-79,15);cl_TMR();
  switch(smenu6){
    case 0: L_amp=trig6;break;
    case 1: R_amp=trig6;break;
    case 2: L_phon=trig6;break;
    case 3: R_phon=trig6;break;}
audio();ShowBDmenu_6();
   FL_com2=false;}
}//END OUTPUT SELECTION LEVEL MENU




}//END POWER ON







/////////////////////////////////////////////////////////////////////////////////////////////////
//                                         POWER OFF                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////
if(power==false){

//НАЧАЛЬНЫЕ ПАРАМЕТРЫ В РЕЖИМЕ STANDBY
if(FL_start_POWER==true){
Mute=1;audio();EEPROM_UPDATE();FL_autoret=false;MUTE_menu=false;BD_menu=false;
WATCH_menu=true;SET_TIME_DATE_menu=false;lastMIN=60;lcd.clear();
digitalWrite(AMP_START_out,LOW);//D15 (А1) ПЕРЕВОДИМ УСИЛИТЕЛИ В РЕЖИМ STANDBY (Оранжевый OFF)
digitalWrite(BT_PWR_out,LOW);//D9 ОТКЛЮЧАЕМ ПИТАНИЕ МОДУЛЯ И ИНДИКАТОРА BLUETOOTH (Синий OFF)
digitalWrite(MUTE_LED_out,LOW);//D10 ОТКЛЮЧАЕМ ИНДИКАТОР ПРИГЛУШЕНИЯ ЗВУКА MUTE (Жёлтый OFF)
digitalWrite(PHONE_LED_out,LOW);//D11 ОТКЛЮЧАЕМ ПИТАНИЕ И ИНДИКАТОР НАУШНИКОВ HEADPHONES (Зелёный OFF)
digitalWrite(POWER_RELAY_out,HIGH);//D13 ОТКЛЮЧАЕМ ПИТАНИЕ УСИЛИТЕЛЯ POWER RELAY (Белый OFF)
digitalWrite(POWER_LED_out,LOW);//D12 ОТКЛЮЧАЕМ ИНДИКАТОР ВКЛЮЧЕНИЯ POWER LED (Красный OFF)
if(LCD_light==0){lcd.noBacklight();}else{lcd.backlight();}//ПОДСВЕТКА ЭКРАНА В РЕЖИМЕ STANDBY ON/OFF
   FL_start_POWER=false;}




/////////////////////////////////////////////////////////////////////////////////////////////////
// КНОПКОЙ "MENU" ИЛИ КНОПКАМИ ВЫБОРА МЕНЮ НА ПУЛЬТЕ ПЕРЕХОДИМ В МЕНЮ УСТАНОВКИ ВРЕМЕНИ И ДАТЫ //
/////////////////////////////////////////////////////////////////////////////////////////////////
MENU_btn=digitalRead(MENU_key);
if(MENU_btn!=MENU_btn_status||ir.value==IR_PREV_MENU||ir.value==IR_NEXT_MENU){
delay(50);MENU_btn=digitalRead(MENU_key);
if(MENU_btn==LOW&&MENU_btn_status==HIGH||ir.value==IR_PREV_MENU||ir.value==IR_NEXT_MENU){
WATCH_menu=!WATCH_menu;SET_TIME_DATE_menu=!SET_TIME_DATE_menu;
if(WATCH_menu==false){get_time();setwatch=0;SET_TIME_DATE_menu=true;
FL_com1=true;FL_com2=true;}lastMIN=60;ir.value=0;lcd.clear();
  }MENU_btn_status=MENU_btn;}




////////////////////////////////////////////////////////////////////////////////////////////////
//                                    LCD LIGHT ON/OFF                                        //
//            КНОПКА ПОДСВЕТКИ ЭКРАНА В РЕЖИМЕ STANDBY ON/OFF (КНОПКОЙ "MUTE")                //
////////////////////////////////////////////////////////////////////////////////////////////////
MUTE_btn=digitalRead(MUTE_key);
if(MUTE_btn!=MUTE_btn_status||ir.value==IR_MUTE){delay(30);MUTE_btn=digitalRead(MUTE_key);
if(MUTE_btn==LOW&&MUTE_btn_status==HIGH||ir.value==IR_MUTE){
LCD_light=!LCD_light;LCD_light?(lcd.backlight()):(lcd.noBacklight());ir.value=0;
  }MUTE_btn_status=MUTE_btn;}




////////////////////////////////////////////////////////////////////////////////////////////////
//                          TIME & DATE / BIG WATCH (MODE POWER OFF)                          //
//      МЕНЮ ДЕМОНСТРАЦИИ ДВУХ ТИПОВ ЧАСОВ (БОЛЬШИЕ ЦИФРЫ ИЛИ ВРЕМЯ, ДАТА, ТЕМПЕРАТУРА)       //
////////////////////////////////////////////////////////////////////////////////////////////////
if(WATCH_menu==true){

//КНОПКОЙ ЭНКОДЕРА ИЛИ КНОПКАМИ ПЕРЕКЛЮЧЕНИЯ ВНУТРИ МЕНЮ НА ПУЛЬТЕ, ВЫБИРАЕМ ТИП ЧАСОВ
ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
delay(15);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH||ir.value==IR_MENU_DOWN||ir.value==IR_MENU_UP){
style_watch=!style_watch;timer_WATCH=millis()+p1000;lastMIN=60;ir.value=0;lcd.clear();
  }ENCODER_btn_status=ENCODER_btn;}


//ПЕЧАТАЕМ ВРЕМЯ КАЖДУЮ СЕКУНДУ
if(millis()-timer_WATCH>=p1000){timer_WATCH=millis();
switch(style_watch){
  case 0: tmr=rtc.getTime();ShowSECONDS();if(lastMIN!=tmr.min){BIG_watch();lastMIN=tmr.min;}break;
  case 1: timedow();datetemp();break;}}

}//END MENU ВЫБОРА ТИПА ЧАСОВ




////////////////////////////////////////////////////////////////////////////////////////////////
//                               МЕНЮ УСТАНОВКИ ВРЕМЕНИ И ДАТЫ                                //
///////////////////////////////////// SET TIME & DATE MENU /////////////////////////////////////
if(SET_TIME_DATE_menu==true){

ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn!=ENCODER_btn_status){delay(10);ENCODER_btn=digitalRead(ENCODER_key);
if(ENCODER_btn==LOW&&ENCODER_btn_status==HIGH){
smenu9++;if(smenu9>6){smenu9=0;}FL_com1=true;FL_com2=true;lcd.clear();
  }ENCODER_btn_status=ENCODER_btn;}

if(ir.value==IR_MENU_DOWN){
smenu9++;if(smenu9>6){smenu9=0;}FL_com1=true;FL_com2=true;ir.value=0;lcd.clear();}
if(ir.value==IR_MENU_UP){
smenu9--;if(smenu9<0){smenu9=6;}FL_com1=true;FL_com2=true;ir.value=0;lcd.clear();}

if(FL_com1==true){
  switch(smenu9){
    case 0: setime=sethour;break;
    case 1: setime=setmin;break;
    case 2: setime=setdate;break;
    case 3: setime=setmon;break;
    case 4: setime=setyear;break;
    case 5: setime=setdow;break;
    case 6: setime=setwatch;break;}
   FL_com1=false;}
   
if(encoderCount!=0){setime=setime+encoderCount;encoderCount=0;FL_com2=true;}
if(ir.value==IR_VOL_UP||ir.value==IR_FFFFF&&irkey==IR_VOL_UP){setime++;ir.value=0;FL_com2=true;}
if(ir.value==IR_VOL_DOWN||ir.value==IR_FFFFF&&irkey==IR_VOL_DOWN){setime--;ir.value=0;FL_com2=true;}

if(FL_com2==true){
lcd.setCursor(0,0);lcd.print(F("SET  TIME & DATE"));
lcd.setCursor(0,1);
switch(smenu9){
  case 0:sethour=setime;if(sethour>23){sethour=0;}if(sethour<0){sethour=23;}
lcd.print(F("HOUR "));lcd.print(sethour);lcd.print(' ');FL_com1=true;break;
  case 1:setmin=setime;if(setmin>59){setmin=0;}if(setmin<0){setmin=59;}
lcd.print(F("MINUTE "));lcd.print(setmin);lcd.print(' ');FL_com1=true;break;
  case 2:setdate=setime;if(setdate>31){setdate=1;}if(setdate<1){setdate=31;}
lcd.print(F("DAY MONTH "));lcd.print(setdate);lcd.print(' ');FL_com1=true;break;
  case 3:setmon=setime;if(setmon>12){setmon=1;}if(setmon<1){setmon=12;}
lcd.print(F("MONTH "));lcd.print(setmon);lcd.print(' ');FL_com1=true;break;
  case 4:setyear=setime;setyear=constrain(setyear,23,50);
lcd.print(F("YEAR 20"));lcd.print(setyear);FL_com1=true;break;
  case 5:setdow=setime;if(setdow>7){setdow=1;}if(setdow<1){setdow=7;}
lcd.print(F("DAY WEEK "));lcd.print(setdow);FL_com1=true;break;
  case 6:setwatch=setime;lcd.print(F("TAP +/- TO SAVE!"));FL_com1=true;break;}
if(setwatch!=0){set_time();smenu9=0;setwatch=0;lcd.clear();lcd.print(F("TIME & DATE SAVE"));
delay(2000);WATCH_menu=true;SET_TIME_DATE_menu=false;lcd.clear();}
  FL_com2=false;}
}//END SET TIME & DATE MENU



}//END POWER OFF 




}//END LOOP 


///////////////////////////////////////// THE END ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
