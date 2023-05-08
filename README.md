# abs-signal-toEGS51-overCANBUS-for772-6
This is attempt to use Arduino to send CAN-BUS frames to EGS51 with vehicle speed information.
This project started as I attempt to swap merc 3.2 v6 petrol engine eith 772.6 automatic gearbox to Mitsubishi Pajero.
Turns out AT is still in limb mode even I provide abs signal fom two wheels to ABS-ESP module.
In that point I decide to remove ABS-ESP module and use instead Arduino to read abs sensor and compose CAN-BUS frames with vehicle wheel speed information and state of brake light switch.
I wrote arduino sketch to do this and to calculate differences in diff ratio and wheel size.
Every think is working on the bench connected to canHacker software and Instrument cluster fom merc.
Arduino is measuring frequency of impulses.
Sending frames every 20ms.
IC is showing right speed.


Please be easy on me, this is my first arduino project ever.

https://github.com/Szu605/abs-signal-toEGS51-overCANBUS-for772-6/blob/88c9b2023c0074602e8932324628e2023e9577be/absToCan.ino

I facing the problem, when I connected Arduino to car CAN-BUS network seems like generated messages can't get through traffic in the network.
