/////////////////////////////////////// ФУНКЦИИ / FUNCTIONS ////////////////////////////////////

//ФУНКЦИЯ ОБРАБОТКИ ПРЕРЫВАНИЯ ОТ СИГНАЛОВ С ЭНКОДЕРА
void rotaryEncoder(){
uint8_t encoderStatus=encoder.process();
if(encoderStatus){encoderCount=(encoderStatus==DIR_CW) ? 1 : -1;}}



//ОБНУЛЯЕМ ТАЙМЕР АВТОВОЗВРАТА В ОСНОВНОЕ МЕНЮ, ПОДНИМАЕМ ФЛАГ
void cl_TMR(){timer_AUTORET=millis();FL_autoret=true;}



//ФУНКЦИЯ ОБНУЛЕНИЯ ПОЛОЖЕНИЯ ЭНКОДЕРА
//void cl_ENC(){oldPos=0;encoderCount=0;myEnc.write(0);}



//ФУНКЦИЯ "ПРАВИЛЬНОЙ ПЕЧАТИ" ЦИФР С УЧЁТОМ ЗНАКА "+"
void TRUE_print(){
if(TrueP>-10&&TrueP<10){lcd.print(' ');}
if(TrueP==0){lcd.print(' ');}
if(TrueP>0){lcd.print('+');}
lcd.print(TrueP);}



//ФУНКЦИЯ ПЕРЕКЛЮЧЕНИЯ МЕЖДУ НАУШНИКАМИ И УСИЛИТЕЛЯМИ МОЩНОСТИ
void PHONE_MODE(){
lcd.clear();lcd.setCursor(2,0);lcd.print("AMPLIFIER ");
switch(phone){

//AMPLIFIER ON / HEADPHONES OFF 
case 0: lcd.print(text_ON);
digitalWrite(PHONE_LED_out,LOW);//ОТКЛЮЧАЕМ ИНДИКАТОР И ПИТАНИЕ УСИЛИТЕЛЯ ДЛЯ НАУШНИКОВ
digitalWrite(POWER_RELAY_out,LOW);//ЧЕРЕЗ РЕЛЕ ПОДАЁМ ПИТАНИЕ НА БП УСИЛИТЕЛЕЙ МОЩНОСТИ
delay(START_AMP_PAUSE);//ЖДЁМ СТАБИЛИЗАЦИИ ПИТАНИЯ
digitalWrite(AMP_START_out,HIGH);//ВКЛЮЧАЕМ TDA7293 ПОДАВ НА 9 ВЫВОД "STANBY" +5В   
break;

//AMPLIFIER OFF / HEADPHONES ON 
case 1: lcd.print(text_OFF);
digitalWrite(AMP_START_out,LOW);//ОТКЛЮЧАЕМ TDA7293 ПОДАВ НА 9 ВЫВОД "STANBY" ЗЕМЛЮ (GND)
delay(START_AMP_PAUSE);//ЖДАТЬ НЕОБЯЗАТЕЛЬНО, НО НАДПИЬ НА ЭКРАНЕ "AMPLIFIER OFF" УДЕРЖИВАЕМ
digitalWrite(POWER_RELAY_out,HIGH);//ЧЕРЕЗ РЕЛЕ ОТКЛЮЧАЕМ ПИТАНИЕ НА БП УСИЛИТЕЛЕЙ МОЩНОСТИ
digitalWrite(PHONE_LED_out,HIGH);//ВКЛЮЧАЕМ ИНДИКАТОР И ПИТАНИЕ УСИЛИТЕЛЯ ДЛЯ НАУШНИКОВ
break;}
lcd.clear();}



//ВКЛЮЧАЕМ ИЛИ ОТКЛЮЧАЕМ BLUETOOTH MODUL И СИНИЙ ИНДИКАТОР ON/OFF ЕСЛИ ВЫБРАН ВХОД "BT"
void BT_LED(){
if(Input==1){digitalWrite(BT_PWR_out,HIGH);}else{digitalWrite(BT_PWR_out,LOW);}}



//ПЕЧАТАЕМ НАСТРОЙКУ ГРОМКОСТИ
void ShowVOLUME_menu(){
uint8_t i,d1,d2,d3,d4,d5,d6,e1,e2,e3;
int8_t a[3];
//УРОВЕНЬ ГРОМКОСТИ
lcd.setCursor(0,0);
if(smenu0==0){lcd.write(triangle);}else{lcd.print(' ');}
if(Volume<=0){lcd.print("VOLUME");}else{lcd.print("GAIN  ");}
a[0]=(Volume+84)/10;
a[1]=(Volume+84)%10;
//ЦИКЛ ФОРМИРУЮЩИЙ БОЛЬШИЕ ЦИФРЫ ГРОМКОСТИ
for(i=0;i<2;i++){
switch(i){
  case 0: e1=9, e2=10,e3=11;break;
  case 1: e1=13,e2=14,e3=15;break;}
switch(a[i]){
  case 0: d1=1,d2=2,d3=1, d4=1, d5=3, d6=1;break;
  case 1: d1=2,d2=1,d3=32,d4=5, d5=1, d6=5;break;
  case 2: d1=2,d2=4,d3=1, d4=1, d5=3, d6=3;break;
  case 3: d1=2,d2=4,d3=1, d4=3, d5=3, d6=1;break;
  case 4: d1=1,d2=3,d3=1, d4=32,d5=32,d6=1;break;
  case 5: d1=1,d2=4,d3=2, d4=3, d5=3, d6=1;break;
  case 6: d1=1,d2=4,d3=2, d4=1, d5=3, d6=1;break;
  case 7: d1=2,d2=2,d3=1, d4=32,d5=32,d6=1;break;
  case 8: d1=1,d2=4,d3=1, d4=1, d5=3, d6=1;break;
  case 9: d1=1,d2=4,d3=1, d4=3, d5=3, d6=1;break;}
lcd.setCursor(e1,0);lcd.write(uint8_t(d1));
lcd.setCursor(e2,0);lcd.write(uint8_t(d2));
lcd.setCursor(e3,0);lcd.write(uint8_t(d3));
lcd.setCursor(e1,1);lcd.write(uint8_t(d4));
lcd.setCursor(e2,1);lcd.write(uint8_t(d5));
lcd.setCursor(e3,1);lcd.write(uint8_t(d6));}//КОНЕЦ ЦИКЛА
}//END BIG VOLUME DIGITS



//ФУНКЦИЯ ИНДИКАЦИИИ ОБОЗНАЧЕНИЯ ТЕКУЩЕГО ВХОДА И УРОВНЯ ЧУСТВИТЕЛЬНОСТИ НА НЁМ
void ShowINPUT_menu(){
lcd.setCursor(0,1);
if(smenu0==1||smenu0==2){lcd.write(pointer);}else{lcd.print(' ');}
if(smenu0!=1){ShowInput=Input;}else{ShowInput=InputIDX;}
  switch(ShowInput){
    case 1: lcd.print("BT ");break;
    case 2: lcd.print("RCA");break;
    case 3: lcd.print("Jk1");break;
    case 4: lcd.print("Jk2");break;}
Gain=Connect[ShowInput];TrueP=Gain;TRUE_print();
}//END MAIN MENU 



//ФУНКЦИЯ ПЕЧАТИ НАСТРОЕК ЭКВАЛАЙЗЕРА
void ShowBDmenu_1(){
lcd.setCursor(1,0); lcd.print("BAS");
lcd.setCursor(7,0); lcd.print("MID");
lcd.setCursor(13,0);lcd.print("TRE");

if(smenu1==0){
lcd.setCursor(0,0);lcd.write(pointer);lcd.setCursor(0,1);lcd.write(pointer);}
else{lcd.setCursor(0,0);lcd.print(' ');lcd.setCursor(0,1);lcd.print(' ');}
TrueP=bass;TRUE_print();

if(smenu1==1){
lcd.setCursor(6,0);lcd.write(pointer);lcd.setCursor(6,1);lcd.write(pointer);}
else{lcd.setCursor(6,0);lcd.print(' ');lcd.setCursor(6,1);lcd.print(' ');}
TrueP=middle;TRUE_print();

if(smenu1==2){
lcd.setCursor(12,0);lcd.write(pointer);lcd.setCursor(12,1);lcd.write(pointer);}
else{lcd.setCursor(12,0);lcd.print(' ');lcd.setCursor(12,1);lcd.print(' ');}
TrueP=treble;TRUE_print();
}//END MENU 1



//ФУНКЦИЯ ВЫВОДА НА ЭКРАН МЕНЮ № 2
void ShowBDmenu_2(){
//УРОВЕНЬ ФОРМИРОВАНИЯ АЧХ ДЛЯ РЕЖИМА LOUDNESS
lcd.setCursor(0,0);
if(smenu2==0){lcd.write(pointer);}else{lcd.print(' ');}
lcd.print("UP ");
if(Loudness<10){lcd.print(' ');}lcd.print(Loudness);
//ЧАСТОТА СРЕЗА ДЛЯ РЕЖИМА LOUDNESS
lcd.setCursor(7,0);
if(smenu2==1){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("FR ");
  switch(Loudness_FR){
    case 0: lcd.print("250Hz");break;
    case 1: lcd.print("400Hz");break;
    case 2: lcd.print("800Hz");break;
    case 3: lcd.print("CLOSE");break;}
//ЗАДЕРЖКА ПЕРЕД АВТОВОЗВРАТОМ В ОСНОВНОЕ МЕНЮ В СЕКУНДАХ 
lcd.setCursor(0,1);
if(smenu2==2){lcd.write(pointer);}else{lcd.print(' ');}
lcd.print("RETURN ");
if(Wait_return<10){lcd.print(' ');}lcd.print(Wait_return);
}//END MENU 2



//ФУНКЦИЯ ВЫВОДА НА ЭКРАН МЕНЮ № 3
void ShowBDmenu_3(){
lcd.setCursor(0,0);
if(smenu3==0){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("BASS ");
  switch(bas_c){
    case 0: lcd.print(" 6");break;
    case 1: lcd.print(" 8");break;
    case 2: lcd.print("10");break;
    case 3: lcd.print("12");break;}
lcd.print("0 Hz");
lcd.setCursor(0,1);
if(smenu3==1){lcd.write(pointer);}else{lcd.print(' ');}lcd.print(text_Q);
  switch(bas_q){
    case 0: lcd.print("0.5");break;
    case 1: lcd.print("1.0");break;
    case 2: lcd.print("1.5");break;
    case 3: lcd.print("2.0");break;}
}//END MENU 3



//ФУНКЦИЯ ВЫВОДА НА ЭКРАН МЕНЮ № 4
void ShowBDmenu_4(){
lcd.setCursor(0,0);
if(smenu4==0){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("MIDDLE ");
  switch(mid_c){
    case 0: lcd.print(" 5");break;
    case 1: lcd.print("10");break;
    case 2: lcd.print("15");break;
    case 3: lcd.print("25");break;}
lcd.print("00 Hz");
lcd.setCursor(0,1);
if(smenu4==1){lcd.write(pointer);}else{lcd.print(' ');}lcd.print(text_Q);
  switch(mid_q){
    case 0: lcd.print("0.75");break;
    case 1: lcd.print("1.00");break;
    case 2: lcd.print("1.25");break;
    case 3: lcd.print("1.50");break;}
}//END MENU 4



//ФУНКЦИЯ ВЫВОДА НА ЭКРАН МЕНЮ № 5
void ShowBDmenu_5(){
lcd.setCursor(0,0);
if(smenu5==0){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("TREBLE ");
  switch(treb_c){
    case 0: lcd.print(" 75");break;
    case 1: lcd.print("100");break;
    case 2: lcd.print("125");break;
    case 3: lcd.print("150");break;}
lcd.print("00 Hz");
lcd.setCursor(0,1);
if(smenu5==1){lcd.write(pointer);}else{lcd.print(' ');}lcd.print(text_Q);
  switch(treb_q){
    case 0: lcd.print("0.75");break;
    case 1: lcd.print("1.25");break;}
}//END MENU 5



//ФУНКЦИЯ ПЕЧАТИ УРОВНЕЙ ВЫХОДОВ НА УСИЛИТЕЛИ
void ShowBDmenu_6(){
lcd.setCursor(0,0);
if(smenu6==0){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("LSP");
lcd.setCursor(4,0);TrueP=L_amp;TRUE_print();

lcd.setCursor(9,0);
if(smenu6==1){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("RSP");
lcd.setCursor(13,0);TrueP=R_amp;TRUE_print();

lcd.setCursor(0,1);
if(smenu6==2){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("LHP");
lcd.setCursor(4,1);TrueP=L_phon;TRUE_print();

lcd.setCursor(9,1);
if(smenu6==3){lcd.write(pointer);}else{lcd.print(' ');}lcd.print("RHP");
lcd.setCursor(13,1);TrueP=R_phon;TRUE_print();
}//END MENU 6



//ФУНКЦИЯ ЗАПРОСА ТЕКУЩИХ ЗНАЧЕНИЙ ВРЕМЕНИ И ДАТЫ ИЗ МОДУЛЯ DS3231
void get_time(){
tmr=rtc.getTime();
sethour=tmr.hour;
setmin=tmr.min;
setdate=tmr.date;
setmon=tmr.mon;
setyear=(tmr.year-2000);
setdow=tmr.dow;
}//END MENU



//ФУНКЦИЯ ЗАПИСИ ТЕКУЩИХ ЗНАЧЕНИЙ ВРЕМЕНИ И ДАТЫ В МОДУЛЬ DS3231
void set_time(){
rtc.setTime(sethour,setmin,0);
rtc.setDate(setdate,setmon,(setyear+2000));
rtc.setDOW(setdow);
}//END MENU



//ФУНКЦИЯ ВЫВОДА НА ЭКРАН ТЕКУЩИХ ЗНАЧЕНИЙ ВРЕМЕНИ И ДНЯ НЕДЕЛИ ИЗ МОДУЛЯ DS3231
void timedow(){lcd.setCursor(0,0);
lcd.print(rtc.getTimeStr());lcd.print(' ');lcd.print(rtc.getDOWStr());
}//END MENU



//ФУНКЦИЯ ВЫВОДА НА ЭКРАН ТЕКУЩИХ ЗНАЧЕНИЙ ДАТЫ И ТЕМПЕРАТУРЫ ИЗ МОДУЛЯ DS3231
void datetemp(){lcd.setCursor(0,1);
lcd.print(rtc.getDateStr());lcd.print("  ");
lcd.print(rtc.getTemp(),0);lcd.print((char)223);lcd.print('C');
}//END MENU



//ФУНКЦИЯ ПЕЧАТИ БОЛЬШИХ ЧАСОВ (ДЛЯ LCD 1602)
void BIG_watch(){
int8_t sign[5];uint8_t i,d1,d2,d3,d4,d5,d6,e1,e2,e3;
//tmr=rtc.getTime();//ЗАПРАШИВАЕМ У МОДУЛЯ DS3231 АКТУАЛЬНОЕ ВРЕМЯ
//ЗАДАЁМ КОНКРЕТНЫЕ ЗНАЧЕНИЯ ЧЕТЫРЁХ ПЕРЕМЕННЫХ ДЛЯ МАССИВА sign
sign[0]=tmr.hour/10;//ПЕРВАЯ ЦИФРА ЧАСОВ
sign[1]=tmr.hour%10;//ВТОРАЯ ЦИФРА ЧАСОВ
sign[2]=tmr.min/10;//ПЕРВАЯ ЦИФРА МИНУТ
sign[3]=tmr.min%10;//ВТОРАЯ ЦИФРА МИНУТ
//ЗА ОДИН ИЗ 4 ПРОХОДОВ ЦИКЛА ПЕЧАТАЕМ ТОЛЬКО ОДНУ ЦИФРУ
for(i=0;i<4;i++){
switch(i){
  case 0: e1=0, e2=1, e3=2; break;//ПОЗИЦИИ КУРСОРА ТРЁХ СИМВОЛОВ ПЕРВОЙ ЦИФРЫ ЧАСОВ
  case 1: e1=4, e2=5, e3=6; break;//ПОЗИЦИИ КУРСОРА ТРЁХ СИМВОЛОВ ВТОРОЙ ЦИФРЫ ЧАСОВ
  case 2: e1=9, e2=10,e3=11;break;//ПОЗИЦИИ КУРСОРА ТРЁХ СИМВОЛОВ ПЕРВОЙ ЦИФРЫ МИНУТ
  case 3: e1=13,e2=14,e3=15;break;}//ПОЗИЦИИ КУРСОРА ТРЁХ СИМВОЛОВ ВТОРОЙ ЦИФРЫ МИНУТ
//ИЗ МАССИВА sign ВЫБИРАЕМ КОНКРЕТНОЕ ЗНАЧЕНИЕ И ЗАПОЛНЯЕМ 6 ПЕРЕМЕННЫХ СПЕЦСИМВОЛАМИ
switch(sign[i]){
  case 0: d1=1,d2=2,d3=1, d4=1, d5=3, d6=1;break;
  case 1: d1=2,d2=1,d3=32,d4=5, d5=1, d6=5;break;
  case 2: d1=2,d2=4,d3=1, d4=1, d5=3, d6=3;break;
  case 3: d1=2,d2=4,d3=1, d4=3, d5=3, d6=1;break;
  case 4: d1=1,d2=3,d3=1, d4=32,d5=32,d6=1;break;
  case 5: d1=1,d2=4,d3=2, d4=3, d5=3, d6=1;break;
  case 6: d1=1,d2=4,d3=2, d4=1, d5=3, d6=1;break;
  case 7: d1=2,d2=2,d3=1, d4=32,d5=32,d6=1;break;
  case 8: d1=1,d2=4,d3=1, d4=1, d5=3, d6=1;break;
  case 9: d1=1,d2=4,d3=1, d4=3, d5=3, d6=1;break;}
//ПЕЧАТАЕМ 6 ВЫБРАННЫХ СПЕЦСИМВОЛОВ, СНАЧАЛА В 3 ПОЗИЦИЯХ ВЕРХНЕЙ СТРОКИ ЗАТЕМ В НИЖНЕЙ
lcd.setCursor(e1,0);lcd.write(uint8_t(d1));
lcd.setCursor(e2,0);lcd.write(uint8_t(d2));
lcd.setCursor(e3,0);lcd.write(uint8_t(d3));
lcd.setCursor(e1,1);lcd.write(uint8_t(d4));
lcd.setCursor(e2,1);lcd.write(uint8_t(d5));
lcd.setCursor(e3,1);lcd.write(uint8_t(d6));}//КОНЕЦ ЦИКЛА
}//END MENU


//ИНДИКАЦИЯ СЕКУНД МЕЖДУ БОЛЬШИМИ ЦИФРАМИ
void ShowSECONDS(){
lcd.setCursor(7,1);if(tmr.sec<10){lcd.print('0');}lcd.print(tmr.sec);
}//END MENU



/////////////////////////////////////////////////////////////////////////////////////////////////
//                                          EEPROM UPDATE                                      //
//                         ОБНОВЛЕНИЕ (ПЕРЕЗАПИСЬ) ПАРАМЕТРОВ В EEPROM                         //
/////////////////////////////////////////////////////////////////////////////////////////////////
void EEPROM_UPDATE(){
EEPROM.update(0,Volume+79);
EEPROM.update(1,treble+20);EEPROM.update(2,middle+20);EEPROM.update(3,bass+20);
EEPROM.update(4,Input);
EEPROM.update(8,style_watch);EEPROM.update(9,Loudness);EEPROM.update(10,Loudness_FR);
EEPROM.update(11,R_amp+79);EEPROM.update(13,R_phon+79);
EEPROM.update(12,L_amp+79);EEPROM.update(14,L_phon+79);
EEPROM.update(16,treb_c);EEPROM.update(17,mid_c);EEPROM.update(18,bas_c);
EEPROM.update(20,treb_q);EEPROM.update(21,mid_q);EEPROM.update(22,bas_q);
EEPROM.update(25,Wait_return);EEPROM.update(26,LCD_light);
EEPROM.update(27,phone);
//ЦИКЛ ПЕРЕЗАПИСИ УРОВНЯ ЧУСТВИТЕЛЬНОСТИ ДЛЯ КАЖДОГО ИЗ 4 ВХОДОВ
for(byte d=0;d<5;d++){EEPROM.update(30+d,Connect[d]);}
}//END EEPROM UPDATE  



/////////////////////////////////////////////////////////////////////////////////////////////////
//                      ФУНКЦИЯ ЗАПИСИ ПАРАМЕТРОВ В АУДИОПРОЦЕССОР BD37534FV                   //
/////////////////////////////////////////////////////////////////////////////////////////////////
void audio(){

bd.setIn(Input-1);//Input Selector = default in IC (0) //0...6

bd.setIn_gain(Gain,Mute);//Input Gain, Mute ON/OFF = default in IC (0,0)
   //Input Gain = 0 dB ... 20 dB, (0...20)
   //Mute ON/OFF = OFF / ON, (0 / 1)

bd.setVol(Volume);   //Volume Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setFront_1(R_amp); //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setFront_2(L_amp); //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setRear_1(R_phon); //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setRear_2(L_phon); //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
//bd.setSub(subwoofer);   //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default IC (-∞dB)

bd.setBass_setup(bas_q,bas_c);//Bass setup, default in IC (0,0)
   //Bass Q = 0.5 / 1.0 / 1.5 / 2.0, (0...3)
   //Bass fo = 60Hz / 80Hz / 100Hz / 120Hz, (0...3)
bd.setMiddle_setup(mid_q,mid_c);//Middle setup, default in IC (0,0)
   //Middle Q = 0.75 / 1.0 / 1.25 / 1.5, (0...3)
   //Middle fo = 500Hz / 1kHz / 1.5kHz / 2.5kHz, (0...3)
bd.setTreble_setup(treb_q,treb_c);//Treble setup, default in IC (0,0)
   //Treble Q = 0.75 / 1.25, (0 / 1)
   //Treble fo = 7.5kHz / 10kHz / 12.5kHz / 15kHz, (0...3)

bd.setBass_gain(bass);      //Bass gain = -20 dB ... +20 dB, (-20 ... 20) default in IC (0)
bd.setMiddle_gain(middle);//Middle gain = -20 dB ... +20 dB, (-20 ... 20) default in IC (0)
bd.setTreble_gain(treble);  //Treble gain = -20 dB ... +20 dB, (-20 ... 20) default in IC (0)
bd.setLoudness_gain(Loudness); //Loudness Gain = 0 dB ... +20 dB, (0 ... 20) default in IC (0)
bd.setLoudness_f(Loudness_FR); //Initial setup 3 = default in IC (0)
   //250Hz / 400Hz / 800Hz / Prohibition, (0...3)
//bd.setSetup_2(sub_f,sub_out,0,faza);//Initial setup 2, default in IC (0,0,0,0)
   //Subwoofer LPF fc = OFF / 55Hz / 85Hz / 120Hz / 160Hz, (0...4)
   //Subwoofer Output Select = LPF / Front / Rear / Prohibition, (0...3)
   //Level Meter RESET = HOLD / REST, (0 / 1)
   //LPF Phase = 0 / 180, (0 / 1)

//bd.mix(); //Level Meter RESET = default in IC (0,2,1)

//bd.setSetup_1(0,2,1);//Initial setup 1 = default in IC (0,2,1)
   //Advanced switch time of Mute = 0.6 msec / 1.0 msec / 1.4 msec / 3.2 msec, (0...3)
   //Advanced switch time of Input = 4.7 msec / 7.1 msec / 11.2 msec / 14.4 msec, (0...3)
   //Advanced switch ON/OFF = OFF / ON, (0 / 1)
//bd.System_Reset();
}//END FUNCTION AUDIO



///////////////////////////////////////// END FUNCTIONS /////////////////////////////////////////
