
Débit ~ 800 Kbits/sec sans headers/encapsulation

RMAP -> direct to memory

6 SW routers -> connection by subsection of 25 sensors
-> Too high power consumption and space, use of a CAN bus or i2c preferable
![[Drawing 2023-02-10 15.37.22.excalidraw|600]]


![[Drawing 2023-02-10 18.51.31.excalidraw|650]]

8,64 GB/Day -> Min mass storage of 16 GB -> 128 Gb -> 

Transmission 1 fois par jour : NAND Flash 1 day storage : https://www.3d-plus.com/data/doc/products/references/3dds_0761_2.pdf
2.5W max power -> maybe 2 chips for supplemental storage

Temp -> SDRAM ? 

Cas de télémétrie : OSEF
	temps de passage : ?
	X band ? 


Power Budget :
	Capteurs : Hamamatsu 512px 1mW/capteur -> 50px -> .20 mW estimation x50 = 10mW 
	ADC : 150mW x 50 = 7,5 W
	


CAN interface chip : 
transmitter SN55HVD233-SEP 3.3-V 

GR740 -> routeur SpaceWire intégré

SDRAM + EEPROM
