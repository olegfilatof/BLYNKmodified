1.  We all love Blynk. But any tool can be improved, so I developed ways to work around some stupid limitations.
  Now with my help you can dynamically change Blynk ID directly in the WEB menu when you first connect.
   Proof of concept, only for ESP32 Wroom controller: flash file BOLVANKA_Blynk_WEB_auth_OK.ino.merged.bin by this way: 
   Install: "Flash Download Tool", after start: ESP32, development. Chose controller COM port on the screen and 
   path to file BOLVANKA_Blynk_WEB_auth_OK.ino.merged.bin. On the left of file name- chose Marked area by V. 
   On the right of file path: input address 0x0000 and press START. 
    After reconnect ESP32 Wroom, find wifi "MY_BLYNK", no passport. 
     Input: wifi SSID, wifi PASSWORD, Blynk parameters. Switch "START BLYNK in work" - ON. And vualia:
    Blynk connected successfuly to your 
     account. All for people now! 
2. Shimansky programmed the same header to wifi and GSM.
  Stupid bug that prevents smooth switching from Wi-Fi to GSM connections and vice versa.
   Fixed, see ESP32_WiFi_SSL-BLYNK_OK.ino code.
   Make a switch in your account V0, connect 18 pin to modem TX and 19pin to modem RXн. 
The modem must be connected to a separate power source 5VDC, 2A. 
Switch V0 in the blynk console and watch in the console how wifi or GSM is connecting in real time, reboot - no need.
 Everything for people, even from Blynk we can be made a candy.
3. The worst code I've ever seen is definitely TinyGSM...
If your mobile account is not topped up, or your modem can't connect to the network,
 this goes on forever and ever... For example, your car's battery will die in an underground parking.
I fixed that too, see the correct modem connection in the code.
Even TinyGSM can be turned into a gem!

4. I use the method I found to view coordinates on the map in the browser, if I have this data in my free Blynk account.

   All these functions can be added to your beautiful code for a small reward. Write to olegfilatov@gmx.com
===========================================================================================================================

2 Шиманский запрограммировал один и тот же хедер для Wi-Fi и GSM.
Глупый баг, препятствующий плавному переключению с Wi-Fi на GSM и наоборот.
Исправлено, см. код ESP32_WiFi_SSL-BLYNK_OK.ino.
  Сделайте переключатель в вашем аккаунте V0, подключите 18-й контакт к TX модема, а 19-й — к RXн модема.
Модем должен быть подключен к отдельному источнику питания 5 В постоянного тока, 2 А.
  Переключите V0 в консоли Blynk и наблюдайте в консоли, как подключается Wi-Fi или GSM в реальном времени, без перезагрузки ESP.
Всё для людей, даже из Blynk можно сделать конфетку.

3. Самый плохой код, который я когда-либо видел- это, безусловно, TinyGSM...
 Если мобильный счёт не пополнен, либо модем не может соединиться с сетью- это продолжается циклично вечно...
 К примеру, аккумулятор Вашего авто обязательно разрядиться в подземном паркинге если использовать сигнализацию на основе Blynk.
 Я исправил и это, смотри правильное подключение модема в коде. 
Даже из TinyGSM можно сделать конфетку!

4. Я использую найденный мною способ смотреть координаты на карте в браузере, при наличии этих данных в бесплатном аккаунте Blynk.
 
   Все эти функции могут быть добавлены в Ваш прекрасный код за небольшое вознаграждение. Пишите на почту olegfilatov@gmx.com
   
