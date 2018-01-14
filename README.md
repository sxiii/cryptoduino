## Cryptoduino project description (English)
This project aims to create an arduino cryptocurrency notificator display that shows you current 
rates of different cryptocurrencies and notifies if something drastically changes.

![ImgA](http://imgur.com/dRceN7b.png)

## BOM (Bill of materials)
1) Arduino Mega 2560
2) Ethernet shield
3) Serial display (screen) 4004 (40 symbols, 4 rows)
4) Potentiometer to control backlight contrast (up to 4.7 kOhm)
5) Two cablepins for power
6) One cablepin for backlight enabling
7) FET Transistor (for example, irf540n) for backlight

![ImgB](http://imgur.com/RTQFlgV.png)

## Connecting schematics
1) Insert screen into the middle of arduino (pins 32~47)
2) Connect screen power to 5v and ground pins (on 4004 screen it's in bottom left if you look at the screen)
3) Connect Gate pin of the FET transistor (irf540n) to A13 pin, Drain pin to the Kathode of the backlight
and the Source pin to ground
4) Connect potentiometer to pins 44 and 43

Flash the firmware and it should work: 

![Img1](https://imgur.com/UENnyzJ.png)

This version can only show rates (4 currencies: high, low and last trade at a time).

## Contributing
You're welcome to contribute to this project. Look at the task list for ideas.

## Task list (to-do)
* Make the button to change displayed data (screen switcher)
* Add some arbitrage options to compare rate on different exchanges

![Img2](https://imgur.com/40uHZSj.png)

## Current contributors
* @Leinlawun (Sergey S.) - software
* Radiotechnik (Dmitry G.) - hardware
* @dmimary (Maria D.) - wooden box
* @sxiii (Den I.) - project idea, support, integration & updates

## Описание: Russian
Данный проект разработан как удобный нотификатор на основе ардуино и дислея, который умеет 
показывать текущие курсы различных криптовалют и сообщает если происходят значительные изменения.
Для сборки проекта вам необходим последовательный дисплей 4004, ардуино мега, ethernet shield 
и пара других компонентов (см. файл arduino.scheme). Данные берем с биржи bittrex но можно 
адаптировать и другие. В настоящий момент только отображает курсы на экране (4 криптовалютных 
пары: high, low и последняя цена).

![Img3](https://imgur.com/kgKMCVL.png)
