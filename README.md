![CORE_PHOTO](https://github.com/AlexGyver/GyverCore/blob/master/gyverCoreLogo.jpg)
# GyverCore for ATmega328
 **Версия 1.7.3 от 01.09.2019**  
 Быстрое и лёгкое ядро для Arduino IDE с расширенной конфигурацией.  
 Основано на оригинальном ядре Arduino версии 1.8.9, большинство функций заменены на более быстрые и лёгкие аналоги, убрано всё лишнее и не относящееся к микроконтроллеру ATmega328p, убран почти весь Wiring-мусор, код упрощён и причёсан. Добавлено несколько функций и интересных вариантов компиляции.   
 Разработано by Александр **AlexGyver** и Egor 'Nich1con' Zaharov

## Установка
### Автоматическая
- Открой the Arduino IDE
- Зайди в **Файл > Настройки**
- Вставь этот адрес в **Дополнительные ссылки для менеджера плат**:
    ```
    https://alexgyver.github.io/package_GyverCore_index.json
    ``` 
- Открой **Инструменты > Плата > Менеджер плат...**
- Подожди загрузку списка
- Листай в самый низ, пока не увидишь **GyverCore**
- Жми **Установка**
- Закрой окно
- Выбери плату в **Инструменты > Плата > GyverCore > ATmega328/168 based**
- Готово!
- *Примечание*: файлы ядра находятся по пути C:\Users\Username\AppData\Local\Arduino15\packages\GyverCore\hardware\avr\1.7.3\

### Ручная
- Файлы из папки GyverCore в этом репозитории положить по пути C:\Users\Username\AppData\Local\Arduino15\packages\GyverCore\hardware\avr\1.7.3\

## Изменения
### Облегчено и ускорено
Время выполнения функций, мкс

Функция         | Arduino   | GyverCore | Быстрее в 
----------------|-----------|-----------|----------
millis			| 1.06 us	| 1.00 us	| -
micros			| 1.19 us	| 1.13 us	| -
pinMode         | 2.90 us   | 0.57 us   | 5.09      
digitalWrite    | 2.90 us   | 0.57 us   | 5.09      
digitalRead     | 2.45 us   | 0.50 us   | 4.90      
analogWrite     | 4.15 us   | 1.13 us   | 3.67      
analogRead      | 112.01 us | 5.41 us   | 20.70     
analogReference | 0.00 us   | 0.00 us   | -    
attachInterrupt | 1.20 us   | 1.18 us   | 1.02      
detachInterrupt | 0.82 us   | 0.57 us   | 1.44    
tone			| 5.63 us   | 2.40 us   | 2.3     

Занимаемое место, Flash, байт

Функция         | Arduino | GyverCore | Разница, Flash 
----------------|---------|-----------|---------------
millis			| 26      | 24		  | 2
micros			| 24	  | 20		  | 4
pinMode         | 114     | 24        | 90             
digitalWrite    | 200     | 24        | 176            
digitalRead     | 190     | 24        | 166            
analogWrite     | 406     | 48        | 358            
analogRead      | 32      | 72        | -40            
analogReference | 0       | 22        | -22            
attachInterrupt | 212     | 180       | 32             
detachInterrupt | 198     | 150       | 48         
tone      		| 1410    | 740       | 670       
Serial.begin    | 1028    | 166       | 862            
print long      | 1094    | 326       | 768            
print string    | 2100    | 1484      | 616            
print float     | 2021    | 446       | 1575           
parseInt        | 1030    | 214       | 816            
readString      | 2334    | 1594      | 740            
parseFloat      | 1070    | 246       | 824         

Примечание: **analogRead** и **analogReference** имеют расширенную функциональность и весят чуть больше  
Скетч, состоящий из однократного вызова перечисленных выше функций, занимает 
- Ядро Arduino: 3446 байт (11%) Flash / 217 байт (10%) SRAM
- Ядро GyverCore: 1436 байт (4%) Flash / 94 байт (4%) SRAM  

Пустой скетч:
- Ядро Arduino: 444 байт (1%) Flash / 9 байт (0%) SRAM
- Ядро GyverCore: 202 байт (0%) Flash / 0 байт (0%) SRAM  

Все библиотеки, работа которых зависит от стандартных функций (время, I/O), работают быстрее:
- SPI: отправка байта данных по адресу (опустить latch, отправить адрес, отправить байт, поднять latch) занимает:
	- Стандартное ядро: 14 мкс (71 кГц)
	- GyverCore: 6 мкс (164 кГц)  
	
**uart** является практически полным аналогом Serial, но весит в разы меньше и работает быстрее. Список функций смотри ниже в **Добавлено**.

### Добавлено
- Расширена подсветка синтаксиса (вплоть до названий регистров и битов)
- Макрос **bitToggle**(value, bit), инвертирует состояние бита **bit** в байте **value**
- Быстрая функция **digitalToggle**(pin), инвертирует состояние пина
- Расширенная работа с АЦП
	- **analogStartConvert**(byte pin) - начать преобразование с выбранного пина
	- **analogGet()** - получить преобразованное значение (между analogStartConvert и analogGet можно выполнять действия, в отличие от ожидания в analogRead())
	- **analogPrescaler**(uint8_t prescl) - установить предделитель для АЦП (2, 4, 8, 16, 32, 64, 128) - управляет скоростью работы АЦП (скоростью оцифровки). Prescaler:
		- **2**: 3.04 мкс (частота оцифровки 329 000 кГц)
		- **4**: 4.72 мкс (частота оцифровки 210 000 кГц)
		- **8**: 8.04 мкс (частота оцифровки 125 000 кГц)
		- **16**: 15.12 мкс (частота оцифровки 66 100 кГц)
		- **32**: 28.04 мкс (частота оцифровки 35 600 кГц)
		- **64**: 56.04 мкс (частота оцифровки 17 800 кГц)													
		- **128**: 112 мкс (частота оцифровки 8 900 Гц)
	- В функции **analogRead(pin)** вместо пина можно указать **INTERNAL** (получить значение внутреннего опорного напряжения) или **THERMOMETR** (получить приблизительную температуру МК). *Примечание: нужно установить предделитель 128*
- Добавлен очень быстрый и лёгкий **UART** (аналог классу Serial)
	- **uartBegin()** - запустить соединение по последовательному порту со скоростью 9600
	- **uartBegin(baudrate)** - запустить соединение по последовательному порту со скоростью baudrate
	- **uartEnd()** - выключить сериал
	- **uartPeek()** - вернуть крайний байт из буфера, не убирая его оттуда
	- **uartClear()** - очистить буфер
	- **uartRead()** - вернуть крайний байт из буфера, убрав его оттуда
	- **uartWrite(val)** - запись в порт
	- **uartPrint(val)** - печать в порт (числа, строки, char array)
	- **uartPrintln(val)** - печать в порт с переводом строки
	- **uartAvailable()** - возвразает true, если в буфере что-то есть
	- **uartSetTimeout(val)** - установить таймаут для функций парсинга (по умолчанию 100 мс)
	- **uartParseInt()** - принять целочисленное число
	- **uartReadString()** - принять строку
	- **uartParseFloat()** - принять число float
	- **uartParsePacket(dataArray)** - принять пакет вида **$50 60 70;** в массив dataArray (смотри пример)
	
### Убарно
Убраны всякие сервисные файлы и прочий хлам, не относящийся к ATmega328 (wifi, USB), почищен код. Ядро полностью совместимо с остальными библиотеками, ничего из стандартных функций не вырезано.
	
## Настройки платы
**Bootloader** - выбор загрузчика (требует перезаписи загрузчика):
- **old bootloader** - cтарый загрузчик (стоит на большинстве китайских плат)
- Новый с **optiBoot**, киатйцы тоже потихоньку начинают продавать платы с ним
- **optiBoot v8** - [optiboot](https://github.com/Optiboot/optiboot) самой свежей версии
- Вариант **without bootloader** для прошивки скетча во всю доступную (32 кБ) память МК
---
**Clock** - выбор частоты и источника тактирования (требует перезаписи загрузчика):
- **External 16 MHz** (стандартный вариант для платы Nano 16 МГц)
- **External 8 MHz** (стандартный вариант для платы Nano 8 МГц)
- **Internal 8 MHz** (внутренний генератор: можно работать с голым камнем без кварца)
- **Internal 1 MHz** (внутренний генератор)
- **Internal 128 kHz** (внутренний генератор) - **загрузчик будет стёрт! Используйте without bootloader!**
- Примечания:
	- Функции времени (delay/millis) скорректированы под выбранную частоту
	- После прошивки на частоту 128 кГц дальнейшая загрузка по ISP возможна только с понижением частоты ISP на стороне программатора!
---
**Save EEPROM** - сохранять EEPROM после перепрошивки (очистки) камня  
- **enable** - включить
- **disable** - выключить
---
**Clock Out** - на пине **D8** (NANO/Mini) будет продублировано тактирование с частотой источника  
- **disable** - выключить
- **enable** - включить
---
**System timer** - преднастройка таймера 0:
- **enable** - таймер 0 настроен по умолчанию, работают функции времени delay/millis
- **disable** - вектор прерываний OVF таймера 0 освобождён для пользователя, delay/delayMicroseconds работают, millis/micros - нет
- Примечание: при отключенном таймере 0 функции delay и delayMicroseconds автоматически заменяются на _delay_ms и _delay_us из avr/util.h, а millis и micros заменены на 0
---
**B.O.D.** (Brown-out detector) - reset при падении напряжения (требует перезаписи загрузчика):
- **disable** - отключен
- **1.8V** - сброс при напряжении питания ниже 1.7-2.0V
- **2.7V (default)** - сброс при напряжении питания ниже 2.5-2.9V
- **4.3V** - сброс при напряжении питания ниже 4.1-4.5V
---
**Initialization** - инициализация периферии (таймеры, ацп) в начале скетча:
- **default** - стандартная инициализация
- **disable** - инициализация отключена
---
## Больше контроля!
Для большего контроля за периферией микроконтроллера рекомендую попробовать следующие наши библиотеки:
- **directTimers** - полный контроль над таймерами/счётчиками ATmega328
- **directADC** - полный контроль над АЦП и компаратором ATmega328
- **GyverPWM** - расширенная генерация ШИМ сигнала со всеми настройками и режимами
- **GyverTimer012** - очень простая и лёгкая библиотека для контроля прерываний по таймерам 0/1/2
- **GyverWDT** - полный контроль за WDT (прерывания, перезагрузка)

Скачать все библиотеки можно из [репозитория](https://github.com/AlexGyver/GyverLibs)

## Версии
- 1.0.0
- 1.1.0
	- Восстановлена совместимость с некоторыми библиотеками (макросы pinToMask)
	- Добавлен выбор частоты в меню (поддержка 16/8 МГц плат с корректнным **временем** и **портом**)
- 1.2.0
	- Возвращён pins_arduino.h, который требуют некоторые библиотеки
- 1.3.0
	- Вовзращены ещё некоторые стандартные костыли, но на вес и эффективность ядра они не влияют
- 1.4.0
	- Восстановлена совместимость между номерами пинов для **analogRead** (A-пины и цифры)
- 1.5.0
	- Облегчён **analogRead**
	- Чуть исправлена инициализация watchdog
- 1.6.0
	- Пофикшен **INPUT_PULLUP**
	- Добавлена расширенная подсветка синтаксиса (регистры и байты)
	- Убрана поддержка **ATmega168**
	- Изменена логика выбора платы
	- Добавлена поддержка тактирования от внутреннего генератора:
		- 8 МГц
		- 1 МГц (только для without bootloader)
		- 128 кГц (только для without bootloader)
- 1.7.0
	- Облегчена сборка
	- Добавлено корректное время (**millis**/**delay**) для частоты 128 кГц
	- Функции **millis()** и **micros()** вырезаны при отключении таймера 0 (через меню выбора платы)
	- Функции **delay()** и **delayMicroseconds()** работают при отключении таймера 0 (через меню выбора платы)
	- Вывод в порт корректно работает при отключении таймера 0 (через меню выбора платы)
	- Ускорен **uart** (обычный буфер заменён на циклический) - чтение быстрее в 2 раза!
	- Добавлена настройка **BOD** (через меню выбора платы)
	- Добавлен **Clock Out** - вывод тактирования на пин D8 (через меню выбора платы)
	- Добавлена настройка **Save EEPROM** (через меню выбора платы)
	- Добавлена настройка **Initialization** отключить инициализацию периферии при старте скетча (через меню выбора платы)
	- Огромное спасибо **Pasha13666** за помощь в доработке!
- 1.7.1
	- Поправлены баги с I/O
	- Улучшен uart
- 1.7.2
	- Поправлено отключение millis
- 1.7.3
	- Сокращён аналогРид
	- Убран расширенный ШИМ
	- Убран сброс WDT (не работал)
	- Пофикшены различные баги
	- Убран lightInit
	- Добавлен загрузчик optiBoot v8