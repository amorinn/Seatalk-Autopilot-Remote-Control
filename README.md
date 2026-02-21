# Seatalk Autopilot Remote Control

An Arduino-based **433 MHz remote control** for Raymarine Seatalk1 autopilots; this enhanced fork introduces a **3D-printable marine enclosure**, **button combo support** for advanced commands, added **circuit protection**, and optimized code refinements.

### Credits & Acknowledgements
This project is a fork entirely based on the excellent concept and original work by [AK-Homberger](https://github.com/AK-Homberger/Seatalk-Autopilot-Remote-Control). 
A huge thanks to the author for creating such a solid and functional foundation. The goal of this fork is simply to build upon that great work, polishing the existing code and adding new features to expand its capabilities. 
## Compatibility
This remote control is **meant to work** with the entire family of Raymarine and Autohelm autopilots that communicate via the **SeaTalk 1** (ST1) protocol. Since it uses the standard command set of the 3-wire SeaTalk network, it is compatible with:

* **Tiller Pilots:** ST1000, ST1000+, ST2000, ST2000+
* **Wheel Pilots:** ST3000, ST4000, ST4000+, ST4000 MkII
* **Control Heads & Inboard Pilots:** ST5000+, ST6000 series (ST6001, ST6002), ST7000 series, and ST8000 series.
* **Course Computers:** SmartPilot S1, S2, S3 (when connected through a SeaTalk 1 control head).

> [!NOTE]  
> **Testing Status:** While the protocol is universal for these models, the project has been physically and personally tested and verified on the **ST2000+** and **ST4000+**. If you use it with another model from the list, your feedback is welcome!

> [!WARNING]  
> **Not for SeaTalkNG / NMEA2000:** This device is not natively compatible with modern Raymarine Evolution (EV-100, 200, etc.) pilots unless your boat's network includes a SeaTalk 1 to SeaTalkNG converter.


<table>
  <tr>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/aabcd19a-d1d6-4f8d-8ae6-18f05df91c54" width="500" alt="PCB montada en caja horizontal" />
    </td>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/b1523065-9105-476e-bec9-e7b1354621f0" width="280" alt="Vista vertical de la caja" />
    </td>
  </tr>
  <tr>
    <td colspan="2" align="center">
      <img src="https://github.com/AK-Homberger/Seatalk-Autopilot-Remote-Control/blob/master/Remote%20Pilot%20Schematics.png" width="800" alt="Esquema del circuito" />
    </td>
  </tr>
</table>



# Hardware
## PCB

The pcb is available at [Aisler](https://aisler.net/p/LCDCUVMF) (credit to AK-Homberger).

Minimum number is three pieces for about 24 EUR. So it makes sense to buy once and share with others interested in the remote control.
The circuit diagram and the PCB layout are shown in the hardware section of this repository.

Additionally, a complete kit is available at The Low Cost Sailor [shop](https://www.thelowcostsailor.com/product/kit-componentes-control-remoto/), offering both a DIY assembly set and a pre-finished **Plug & Play** version. A video [tutorial](https://www.youtube.com/watch?v=tlvjnMNMi7c&t=742s) is also available covering the build and setup.
*Note: I am not affiliated with this store, and this is not a promotion. It is mentioned simply as a potentially convenient option for some users.*

>[!TIP]
>If you are building just one unit and are handy with a soldering iron, I recommend prototyping on a breadboard and then using a perfboard for the final permanent circuit.

> [!WARNING]
> I also recommend the implementation of a DIODE in the same position as mine, to protect against plugin it the wrong way, is in the optional section of the hardware.

The circuit implements mainly an interface between the Seatalk bus (12 Volt) and the Arduino (5 Volt).
The easiest way was to use the 74LS07 open collector driver device. This is much easier than most of the other implementations I have seen so far. The PCB also contains the voltage regulator (12-14 Volt from Seatalk bus to 5 Volt for the Arduino) and a 433 MHz receiver. Please select/buy the right ProMicro device (5 Volt). There are also 3.3 Volt devices available. But the 3.3 Volt devices will not work.

The PCB also allows the connection of a standard OLED device (SSD1306) to show the current apparent wind speed. But this is optional.

The voltage regulator (7805) may get quite hot (90 mA * 9 Volt = 0,81 Watt). This is usually not a problem, because it within the specification of the 7805 (< 1,4 Watt).

An alternative which is not getting hot is the following replacement which has the same pin layout as the 7805 is the [78E50](https://www.reichelt.de/de/en/dc-dc-converter-5-v-1-a-single-r-78e50-10-p177268.html?&nbc=1) but is a lot more expensive.

## Remote
Most **433 MHz RF remotes** are compatible with this system. While the variety on sites like AliExpress is nearly infinite, I suggest any of this two main approaches:

* **6-Button Rugged Remote (Recommended):** The best way to access all functions (+1, -1, +10, -10, Auto, and Standby) in a single, durable device.
* **4-Button Remote:** A great compact alternative if you prefer a simpler setup or a customized layout.
* And [My personal favourite](https://es.aliexpress.com/item/1005009013294964.html?spm=a2g0o.detail.similar_items.1.5bdeN0qoN0qo2m&utparam-url=scene%3Aimage_search%7Cquery_from%3Adetail_bigimg%7Cx_object_id%3A1005009013294964%7C_p_origin_prod%3A&algo_pvid=fdcfd253-bcb1-4387-8f99-b40e3dd30076&algo_exp_id=fdcfd253-bcb1-4387-8f99-b40e3dd30076&pdp_ext_f=%7B%22order%22%3A%22136%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%217.04%212.04%21%21%218.14%212.36%21%40211b804117714186864287306ecd19%2112000047580332644%21sea%21ES%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ad1c112d5%3Bm03_new_user%3A-29895%3BpisId%3A5000000197846863) 
  
# Parts:
## Full list for buy, build and play.
| Ref | Component |
| :--- | :--- |
| **J1** | Connector [AKL 101-03, 3-pin, 5.08 mm](https://www.reichelt.com/index.html?ACTION=446&LA=0&nbc=1&q=akl%20101-03) | 
| **J2** | [RXB6 433 MHz receiver](https://www.makershop.de/en/module/funk/rxb6-433mhz-antenne/) |
| **J4** | [Arduino Pro Micro, 5 Volt](https://eckstein-shop.de/HIMALAYA-basic-Pro-micro-5V-16MHz-Arduino-mini-Leonardo-compatible-board) |
| **U1** | [7805 Voltage Regulator](https://www.reichelt.com/de/en/voltage-regulator-up-to-1-5-a-positive-to-220--a-7805-p23443.html?&trstct=pos_0&nbc=1) or cooler alternative [78E50](https://www.reichelt.de/de/en/shop/product/dc_dc-converter_5_v_1_a_single-177268?nbc=1) |
| **U2** | [74LS07](https://csd-electronics.de/ICs/Logik/74LS/74LS07-DIP14::283.html?pk_campaign=google_shopping&pk_kwd=74LS07-DIP14&gclid=EAIaIQobChMIrLSWjb-O7AIVird3Ch0ClQEdEAQYBCABEgJCdPD_BwE) |
| **D1** | [Zener diode 5.1 Volt (0.5 Watt)](https://www.reichelt.com/de/en/zener-diode-0-5-w-5-1-v-zf-5-1-p23137.html?&trstct=pos_0&nbc=1) |
| **D2** | [LED, red, 3 mm](https://www.reichelt.com/de/en/led-3-mm-low-current-red-led-3mm-2ma-rt-p21626.html?&trstct=pos_8&nbc=1) |
| **R1** | [Resistor 10 KOhm, 1/4 watt](https://www.reichelt.com/de/en/carbon-film-resistor-1-4w-5-10-kilo-ohms-1-4w-10k-p1338.html?&nbc=1) |
| **R2** | [Resistor 68 KOhm, 1/4 watt](https://www.reichelt.com/de/en/carbon-film-resistor-1-4-w-5-68-kohm-1-4w-68k-p1462.html?&nbc=1) |
| **R3** | [Resistor 27 KOhm, 1/4 watt](https://www.reichelt.com/de/en/carbon-film-resistor-1-4-w-5-27-kohm-1-4w-27k-p1392.html?&nbc=1) |
| **R4** | [Resistor 270 Ohm, 1/4 watt](https://www.reichelt.com/de/en/carbon-film-resistor-1-4-w-5-270-ohm-1-4w-270-p1390.html?&nbc=1) |
| **R5** | [Resistor 10 KOhm, 1/4 watt](https://www.reichelt.com/de/en/carbon-film-resistor-1-4w-5-10-kilo-ohms-1-4w-10k-p1338.html?&nbc=1) |
| **PCB**| [AK-Homeberger's PCB](https://aisler.net/p/LCDCUVMF) with OLED display optional |
| **REMOTE** | [Ruged 6 buttons](https://es.aliexpress.com/item/1005009013294964.html?spm=a2g0o.detail.similar_items.1.5bdeN0qoN0qo2m&utparam-url=scene%3Aimage_search%7Cquery_from%3Adetail_bigimg%7Cx_object_id%3A1005009013294964%7C_p_origin_prod%3A&algo_pvid=fdcfd253-bcb1-4387-8f99-b40e3dd30076&algo_exp_id=fdcfd253-bcb1-4387-8f99-b40e3dd30076&pdp_ext_f=%7B%22order%22%3A%22136%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%217.04%212.04%21%21%218.14%212.36%21%40211b804117714186864287306ecd19%2112000047580332644%21sea%21ES%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ad1c112d5%3Bm03_new_user%3A-29895%3BpisId%3A5000000197846863) or [4 butons waterproof](https://es.aliexpress.com/item/1005010156871771.html?spm=a2g0o.productlist.main.5.7a3c4IOl4IOlCP&algo_pvid=b34e57af-ccd6-41d6-bc73-9232bf2d0279&algo_exp_id=b34e57af-ccd6-41d6-bc73-9232bf2d0279-4&pdp_ext_f=%7B%22order%22%3A%22150%22%2C%22spu_best_type%22%3A%22price%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%215.72%210.99%21%21%2145.54%217.89%21%40211b613917714225694304450e2589%2112000051351934102%21sea%21ES%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ad1c112d5%3Bm03_new_user%3A-29895%3BpisId%3A5000000197846864&curPageLogUid=xsjyORh92BUG&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005010156871771%7C_p_origin_prod%3A)|
| **PRICE** | ~15€ components + 25€ 3 pack PCB + ~3€ Remote = ~45€ |

> [!TIP]
> You can find all of this much cheaper on **Aliexpress**, but links are always changing.
> And for cheaper price, instead of using a custom pcb, use a prefboard if you are good with soldering.

### Optional Add-ons

Depending on your build method, you can add these optional components to enhance your remote control system:

* **OLED Display:** [0.96 inch, I2C](https://www.makershop.de/en/display/oled/096-oled-display-blau/)
  > *Note: Only compatible with [AK-Homberger's PCB](https://aisler.net/p/LCDCUVMF) or custom DIY breadboard/perfboard setups.*

**Circuit Protection (DIY Builds Only)**
* **D3 Diode:** [1N4004](https://www.reichelt.com/de/en/shop/product/rectifier_diode_400v_1a_do-41-219373) (Prevents reverse polarity issues).
* **Fuse:** [0.5A Inline Fuse](https://es.aliexpress.com/item/1005010492582080.html?spm=a2g0o.productlist.main.3.438fdUeFdUeFu8&algo_pvid=f24d7c4a-50f5-4df1-8a2c-61d917488a63&algo_exp_id=f24d7c4a-50f5-4df1-8a2c-61d917488a63-2&pdp_ext_f=%7B%22order%22%3A%2231%22%2C%22spu_best_type%22%3A%22price%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%215.73%210.99%21%21%2145.59%217.83%21%40211b804117714228488618751ecd5f%2112000052583890671%21sea%21ES%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ad1c112d5%3Bm03_new_user%3A-29895%3BpisId%3A5000000197846864&curPageLogUid=kPdnxjrabVO2&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005010492582080%7C_p_origin_prod%3A) (Recommended to protect the SeaTalk network).
  
**Programming & Testing**
* **433MHz Transmitter:** Useful if you need to sniff codes or test the receiver module.
  > *Links:* [Option 1](https://www.makershop.de/en/module/funk/superheterodyne-433-mhz-sender/) | [Option 2](https://www.makershop.de/en/module/funk/433-sender-empfaenger/)


## Resin coating for marine protection. (recomendation)
If you are planing to install and forget, from experience i recommend to seal coat the pcb with epoxi or any dielectric kind of silicone.

<img width="700" height="456" alt="image" src="https://github.com/user-attachments/assets/47812d7d-f0f9-4a01-a70c-6cba8433522a" />

>[!CAUTION]
>**Do not cover the L7805**. So it can disipate heat. 

## 3D Printable Case
Added a 3D printable `.STL` case designed for marine environments. Features a silicone-sealable cover, cable exit, and wall mount hole. I recommend also sealing the cable exit. 


<div align="center">
  <img width="400" alt="Marine Case 3D Model" src="https://github.com/user-attachments/assets/2432d8fe-2833-4310-a58b-6275fc6986b7" />
</div>


> [!WARNING]
> **Not physically tested yet.** Designed in OpenSCAD matching exact measurements, but print and use at your own discretion.

# Software
The used Seatalk codes are derived from [Thomas Knauf](http://www.thomasknauf.de/rap/seatalk2.htm ). Many thanks for this excellent piece of work.

The software is sending a beep to the Raymarine devices (Seatalk alarm function). This helps to get a feedback regarding the recognition of of a pressed key. The LED on the PCB blinks in addition if keys are successfully recognised. This should also help with trouble shooting. It is also possible to connect an active 5 Volt buzzer to pin 20 (A2) and GND of the ProMicro.

Since version 1.1 it supports now also Auto and Standby remote control. You have to use a 6 button KeyFob or an additional 2 or 4 button remote for controling Auto/Standby function.
The Seatalk communication is a 9 bit serial protocol. This is not directly supported from Arduino. But I found a working 9 bit hardware serial implementation. For convenience, I have added the working source code here, because there are different versions available (in github and elsewhere).

>[!CAUTION]
>The 9 bit hardware serial implementaion is depending on the version of the Ardunio IDE and the ProMicro board information. IDE 1.6.13  and board version 1.6.15 are working with the files in "NewHWSworking". Later versions (e.g. 1.8.10) changed the hardware serial implementation and the 9 bit changes will work work with files in "HardwareSerial9bit". For the Leonardo board please use the **Arduino AVR Boards** version 1.8.3. Since version 1.8.4 the code has changed again and the 9 bit implementation is not working.

For newer IDE versions the three files from "HardwareSerial9bit" (or "NewHWSworking" for older IDE) have to be copied into the Arduino directory on your local PC e.g.: "C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino".

As Arduino board for the ProMicro you can either use the standard Arduino "Leonardo" board information or the ProMicro  [SparkFun implementation](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide). Both will work.
Make sure that you install the ProMicro Board Addon using the Board Manager as described in the hookup guide: https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide#windows_boardaddon

The software is using several additional Arduino libraries (RCSwitch, Adafruit_GFX, Adafruit_SSD1306). These libraries have to be downloaded and installed. The easiest way is to add the libraries with the IDE Library Manager.

- https://github.com/sui77/rc-switch
- https://github.com/adafruit/Adafruit-GFX-Library
- https://github.com/adafruit/Adafruit_SSD1306
# USAGE
## Programing the 433Mhz remote.
Start uploading the `433_Programing.ino` into the arduino pro micro. Once uploaded you shoud start the serial monitor in the pro micro port and star pressing one by one each of the buttons on your remote. And anotating the keys id's.
Then you have to open the `AP_main.ino` and complete the key ids. 



# Programming the 433 MHz KeyFob

You should change the four codes in the software to individual random numbers. The same codes have to be copied to the "_433_Programming_New.ino". Only for programming you will also need a 433 MHz sender connected to the ProMicro.
I used a ProMicro on a "bread board" to program the codes. 

![Connections](https://github.com/AK-Homberger/Seatalk-Autopilot-Remote-Control/blob/master/Programming%20connections.JPG)

Send each code individually and programm the keys of the remote according to the programming description of your KeyFob. Press "Send" button in serial console for next key. 

Key 1 = button A = -1 degree

Key 2 = button B =  +1 degree

Key 3 = button C = -10 degrees

Key 4 = button D = +10 degrees

If you use Auto/Standby function you also have to program the two additional buttons accordingly. After programming you can check directly if codes are programmed correctly. Just press the key on the remote and check if the shown value is correct.

Have fun with the remote control.

Please be aware that this repository is only for educational purpose, to learn how to use the Seatalk protocol. Use it on your own risk and do not use it for critical systems in real life environments.



# Updates:

- Version 1.5 - 16.12.20: Added command receive from USB-Serial to allow control from AvNav.
- Version 1.4 - 28.11.20: Changed R2 and R3 back to 68 KOhm / 27 kOhm. 6.8 KOhm would impact the Seatalk high voltage level too much.
- Version 1.4 - 27.11.20: Changed R2 and R3 to 6.8 KOhm to eliminate problems with some 74LS07 devices (see issue "R3 wrong value?" for details).
- Version 1.4 - 27.07.20: Use of millis() funtion to avoid delay() and timer counter in loop(). Improved detection of 433MHz keys.
- Version 1.3 - 13.12.19: Added definition for certain OLED displays that require this to work properly.
- Version 1.2 - 21.09.19: Added buzzer support on pin 20 (active 5 Volt buzzer works without additional transistor).
- Version 1.1 - 03.08.19: Added support for Auto and Standby (Key 5 and 6).
- Added new programming code (_433_Programming_New.ino) to simplify programming of KeyFob without recompiling and upload for each key.
