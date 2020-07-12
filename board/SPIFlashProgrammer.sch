EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5250 5150 0    39   Output ~ 0
TDO
Text HLabel 5250 5050 0    39   Input ~ 0
TDI
Text HLabel 5250 4950 0    39   Input ~ 0
TMS
Text HLabel 5250 4850 0    39   Input ~ 0
TCK
Text HLabel 6250 4050 2    39   Input ~ 0
SCK
Text HLabel 6250 4150 2    39   Input ~ 0
~SPI_CS
Text HLabel 6250 4250 2    39   Input ~ 0
COPI
Text HLabel 6250 4350 2    39   Output ~ 0
CIPO
Text HLabel 6250 4450 2    39   Input ~ 0
~SRESET
Text HLabel 6250 4850 2    39   Output ~ 0
TCK
Text HLabel 6250 4950 2    39   Output ~ 0
TMS
Text HLabel 6250 5050 2    39   Output ~ 0
TDI
Text HLabel 6250 5150 2    39   Input ~ 0
TDO
Wire Wire Line
	5250 3350 6250 3350
Wire Wire Line
	6250 3450 5250 3450
Wire Wire Line
	5250 3550 6250 3550
Wire Wire Line
	6250 3650 5250 3650
Wire Wire Line
	5250 4050 6250 4050
Wire Wire Line
	6250 4150 5250 4150
Wire Wire Line
	5250 4250 6250 4250
Wire Wire Line
	6250 4350 5250 4350
Wire Wire Line
	5250 4450 6250 4450
Wire Wire Line
	6250 4850 5250 4850
Wire Wire Line
	5250 4950 6250 4950
Wire Wire Line
	6250 5050 5250 5050
Wire Wire Line
	5250 5150 6250 5150
$Sheet
S 4250 3250 1000 2000
U 5F0A81B1
F0 "Processor" 39
F1 "Processor.sch" 39
F2 "SCLK" O R 5250 4050 39 
F3 "~SPI_CS" O R 5250 4150 39 
F4 "COPI" O R 5250 4250 39 
F5 "CIPO" I R 5250 4350 39 
F6 "~SRESET" O R 5250 4450 39 
F7 "USB_ID" I R 5250 3450 39 
F8 "VBUS" I R 5250 3350 39 
F9 "D-" B R 5250 3650 39 
F10 "D+" B R 5250 3550 39 
$EndSheet
$Sheet
S 6250 3250 1000 2000
U 5F0A82B6
F0 "Connectors" 50
F1 "Connectors.sch" 50
F2 "D-" B L 6250 3650 39 
F3 "D+" B L 6250 3550 39 
F4 "USB_ID" O L 6250 3450 39 
F5 "VBUS" O L 6250 3350 39 
$EndSheet
$EndSCHEMATC
