# SimpleRobotDrive

- [x] Connection working
- [x] Joystic working 
- [x] Motors spinning
- [x] MVP
- [x] Fit control & motors in a box and budget
- [ ] Field test driving
- [ ] Weapon control working
- [ ] Weapong spinning
- [ ] Assemble whole robot
- [ ] Improve driving code


## 1 Connection working
After first test it seems there is a change that communication is fast enough to try the streeing.
```
11:39:09.416 -> Y: 7
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 8
11:39:09.416 -> Y: 2
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 7
11:39:09.416 -> Y: 8
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 7
11:39:09.416 -> Y: 5
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 3
11:39:09.416 -> Y: 14
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 7
11:39:09.416 -> Y: 16
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 5
11:39:09.416 -> Y: 16
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 15
11:39:09.416 -> Y: 1
11:39:09.416 -> 
11:39:09.416 -> Bytes received: 2
11:39:09.416 -> X: 18
11:39:09.463 -> Y: 2
11:39:09.463 -> 
11:39:09.463 -> Bytes received: 2
```
## 2 Joystic working 

```
12:36:53.478 -> Y: 150
12:36:53.478 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 56
12:36:53.526 -> Y: 150
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 56
12:36:53.526 -> Y: 150
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 56
12:36:53.526 -> Y: 150
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 55
12:36:53.526 -> Y: 149
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 49
12:36:53.526 -> Y: 147
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 46
12:36:53.526 -> Y: 144
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 41
12:36:53.526 -> Y: 140
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 31
12:36:53.526 -> Y: 150
12:36:53.526 -> 
12:36:53.526 -> Bytes received: 2
12:36:53.526 -> X: 28
12:36:53.526 -> Y: 141
12:36:53.526 -> 
12:36:53.572 -> Bytes received: 2
12:36:53.572 -> X: 17
12:36:53.572 -> Y: 149
```
## 3 Motors spinning

Motor driver working.
Drives well.
Next step should be upgrading to more simple driver.
There is 2 options: 

| Part      | Cost      | weight        | size    | A/channel   |A/peak   | supply Voltage   |  
| :---:     | :---:     | :---:         | :---:     |:---:      |:---:    |:---: |
| Dual Way Bidirectional Brushed Esc    | 5.0€      | ~11g   |43 * 25 * 8mm     |?      |5A     |6-19V   |
| Dual Channel MINI-L298N DC Motor Driver | 0,26€   | 2.6g   |24.7 * 21 * 5mm   |1.5A   |2.5A   |2V-10V   |

Both reduce complexity. Bruched esc could power the ESP32 the same way as big L982N driver.
Conclusion: I will try to use the MINI-L298N driver first.
Seems like ESP32 runs happy but hot powered directly form LiPo battery. But it is not preferred.
- [ ] Try to use just bare ESP32 module with 3.3V power step down.

## 4 Fit everything in a box and budget

| Part      | Cost      | 
| :---:     | :---:     | 
| ESP32 x 2  | 5.6- 9.4€      | 
| Dual Channel MINI-L298N DC Motor Driver | 0,26€   | 
| Lipo | 5€   | 
| 7.4V 800mA*2 XH-3P BMS 2S USB Charger | 1.22€   | 
| Joystic Dual axis | 0,65€   | 
| N20 Motors x 2 | 7€ | 
| TOTAL FOR NOW: | 24€ | 

Postages are not included. 

## 5 Field test
## 6 Weapon control working
## 7 Weapong spinning
## 8 Assemble whole robot
## 9 Improve driving code

## ecountered problems

```
E (44798) gpio: gpio_set_level(226): GPIO output gpio_num error
solution: I tried to use input only pins as output pins.
```