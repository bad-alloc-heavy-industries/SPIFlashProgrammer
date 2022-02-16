EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title "Top-level Schematic"
Date ""
Rev "R1"
Comp "Rachel Mant"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6000 3350 6900 3350
Wire Wire Line
	7000 3450 6000 3450
Wire Wire Line
	6000 3550 7000 3550
Wire Wire Line
	7000 3650 6000 3650
Wire Wire Line
	6000 4050 7000 4050
Wire Wire Line
	7000 4150 6000 4150
Wire Wire Line
	6000 4250 7000 4250
Wire Wire Line
	7000 4350 6000 4350
Wire Wire Line
	6000 4450 7000 4450
Wire Wire Line
	7000 4850 6000 4850
Wire Wire Line
	6000 4950 7000 4950
Wire Wire Line
	7000 5050 6000 5050
Wire Wire Line
	6000 5150 7000 5150
$Sheet
S 7000 3250 1000 2000
U 5F0A82B6
F0 "Connectors" 50
F1 "Connectors.sch" 50
F2 "D-" B L 7000 3650 39 
F3 "D+" B L 7000 3550 39 
F4 "USB_ID" O L 7000 3450 39 
F5 "VBUS" O L 7000 3350 39 
F6 "CIPO" O L 7000 4350 39 
F7 "SCLK" I L 7000 4050 39 
F8 "~SPI_CS" I L 7000 4150 39 
F9 "~SRESET" I L 7000 4450 39 
F10 "COPI" I L 7000 4250 39 
F11 "TMS" O L 7000 4950 39 
F12 "TCK" O L 7000 4850 39 
F13 "TDO" I L 7000 5150 39 
F14 "TDI" O L 7000 5050 39 
F15 "~SRST" O L 7000 4750 39 
F16 "~SPI_PRESENT" O L 7000 3950 39 
$EndSheet
Wire Wire Line
	6000 4750 7000 4750
$Sheet
S 7000 2650 1000 200 
U 5F6E308F
F0 "Power Supply" 50
F1 "Power_Supply.sch" 50
F2 "VBUS" I L 7000 2750 39 
$EndSheet
Wire Wire Line
	7000 2750 6900 2750
Wire Wire Line
	6900 2750 6900 3350
Connection ~ 6900 3350
Wire Wire Line
	6900 3350 7000 3350
Wire Wire Line
	4000 3850 5000 3850
Wire Wire Line
	5000 3950 4000 3950
Wire Wire Line
	4000 4050 5000 4050
Wire Wire Line
	5000 4250 4000 4250
Wire Wire Line
	4000 4350 5000 4350
$Sheet
S 5000 3250 1000 2000
U 5F0A81B1
F0 "Processor" 39
F1 "Processor.sch" 39
F2 "SCLK" O R 6000 4050 39 
F3 "~SPI_CS" O R 6000 4150 39 
F4 "COPI" O R 6000 4250 39 
F5 "CIPO" I R 6000 4350 39 
F6 "~SRESET" O R 6000 4450 39 
F7 "USB_ID" I R 6000 3450 39 
F8 "VBUS" I R 6000 3350 39 
F9 "D-" B R 6000 3650 39 
F10 "D+" B R 6000 3550 39 
F11 "TCK" I R 6000 4850 39 
F12 "TMS" I R 6000 4950 39 
F13 "TDI" I R 6000 5050 39 
F14 "TDO" O R 6000 5150 39 
F15 "~SRST" I R 6000 4750 39 
F16 "FLASH_CIPO" I L 5000 4050 39 
F17 "FLASH_COPI" O L 5000 3950 39 
F18 "FLASH_CLK" O L 5000 3850 39 
F19 "~FLASH_CS1" O L 5000 4250 39 
F20 "~FLASH_CS2" O L 5000 4350 39 
F21 "~SPI_PRESENT" I R 6000 3950 39 
$EndSheet
$Sheet
S 3000 3750 1000 700 
U 5F8B7B2C
F0 "Onboard Flash" 50
F1 "Onboard_Flash.sch" 50
F2 "SCK" I R 4000 3850 39 
F3 "COPI" I R 4000 3950 39 
F4 "CIPO" O R 4000 4050 39 
F5 "~CS1" I R 4000 4250 39 
F6 "~CS2" I R 4000 4350 39 
$EndSheet
Wire Wire Line
	6000 3950 7000 3950
$EndSCHEMATC
