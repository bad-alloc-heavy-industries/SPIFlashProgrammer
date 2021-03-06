EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DX_MON:SPI-Flash_EP U4
U 1 1 5F8B954B
P 3500 3500
F 0 "U4" H 3200 3750 50  0000 L CNN
F 1 "AT25SF641-MHB-T" H 3550 3150 50  0000 L CNN
F 2 "rhais_package-qfn:UDFN-8-1EP_6x5mm_P1.27mm_EP3.4x4mm" H 3500 3200 50  0001 C CNN
F 3 "https://www.dialog-semiconductor.com/sites/default/files/at25sf641_111.pdf" H 3500 3200 50  0001 C CNN
F 4 "Adesto Technologies" H 3500 3500 50  0001 C CNN "MFR"
F 5 "AT25SF641-MHB-T" H 3500 3500 50  0001 C CNN "MPN"
F 6 "2988544" H 3500 3500 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/adesto-technologies/at25sf641-mhb-t/flash-memory-64mbit-40-to-85deg/dp/2988544" H 3500 3500 50  0001 C CNN "URL_FARNELL"
	1    3500 3500
	1    0    0    -1  
$EndComp
$Comp
L DX_MON:SPI-Flash_EP U5
U 1 1 5F8B969C
P 7500 3500
F 0 "U5" H 7200 3750 50  0000 L CNN
F 1 "AT25SF641-MHB-T" H 7550 3150 50  0000 L CNN
F 2 "rhais_package-qfn:UDFN-8-1EP_6x5mm_P1.27mm_EP3.4x4mm" H 7500 3200 50  0001 C CNN
F 3 "https://www.dialog-semiconductor.com/sites/default/files/at25sf641_111.pdf" H 7500 3200 50  0001 C CNN
F 4 "Adesto Technologies" H 7500 3500 50  0001 C CNN "MFR"
F 5 "AT25SF641-MHB-T" H 7500 3500 50  0001 C CNN "MPN"
F 6 "2988544" H 7500 3500 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/adesto-technologies/at25sf641-mhb-t/flash-memory-64mbit-40-to-85deg/dp/2988544" H 7500 3500 50  0001 C CNN "URL_FARNELL"
	1    7500 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C19
U 1 1 5F8B9B2F
P 5000 3500
F 0 "C19" H 5092 3546 50  0000 L CNN
F 1 "0.1uF" H 5092 3455 50  0000 L CNN
F 2 "rhais_rcl:C0603" H 5000 3500 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1870456.pdf" H 5000 3500 50  0001 C CNN
F 4 "Multicomp" H 5000 3500 50  0001 C CNN "MFR"
F 5 "MC0603B104J250CT" H 5000 3500 50  0001 C CNN "MPN"
F 6 "2310559" H 5000 3500 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0603b104j250ct/cap-0-1-f-25v-5-x7r-0603-reel/dp/2310559" H 5000 3500 50  0001 C CNN "URL_FARNELL"
	1    5000 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C20
U 1 1 5F8BA6EE
P 5500 3500
F 0 "C20" H 5592 3546 50  0000 L CNN
F 1 "0.1uF" H 5592 3455 50  0000 L CNN
F 2 "rhais_rcl:C0603" H 5500 3500 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1870456.pdf" H 5500 3500 50  0001 C CNN
F 4 "Multicomp" H 5500 3500 50  0001 C CNN "MFR"
F 5 "MC0603B104J250CT" H 5500 3500 50  0001 C CNN "MPN"
F 6 "2310559" H 5500 3500 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0603b104j250ct/cap-0-1-f-25v-5-x7r-0603-reel/dp/2310559" H 5500 3500 50  0001 C CNN "URL_FARNELL"
	1    5500 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5F8BAC40
P 5250 3800
F 0 "#PWR029" H 5250 3550 50  0001 C CNN
F 1 "GND" H 5255 3627 50  0000 C CNN
F 2 "" H 5250 3800 50  0001 C CNN
F 3 "" H 5250 3800 50  0001 C CNN
	1    5250 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 5F8BB1D6
P 3500 4000
F 0 "#PWR027" H 3500 3750 50  0001 C CNN
F 1 "GND" H 3505 3827 50  0000 C CNN
F 2 "" H 3500 4000 50  0001 C CNN
F 3 "" H 3500 4000 50  0001 C CNN
	1    3500 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR032
U 1 1 5F8BB66E
P 7500 4000
F 0 "#PWR032" H 7500 3750 50  0001 C CNN
F 1 "GND" H 7505 3827 50  0000 C CNN
F 2 "" H 7500 4000 50  0001 C CNN
F 3 "" H 7500 4000 50  0001 C CNN
	1    7500 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR028
U 1 1 5F8BBB58
P 5250 3200
F 0 "#PWR028" H 5250 3050 50  0001 C CNN
F 1 "+3.3V" H 5265 3373 50  0000 C CNN
F 2 "" H 5250 3200 50  0001 C CNN
F 3 "" H 5250 3200 50  0001 C CNN
	1    5250 3200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR031
U 1 1 5F8BC53F
P 7500 3100
F 0 "#PWR031" H 7500 2950 50  0001 C CNN
F 1 "+3.3V" H 7515 3273 50  0000 C CNN
F 2 "" H 7500 3100 50  0001 C CNN
F 3 "" H 7500 3100 50  0001 C CNN
	1    7500 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR026
U 1 1 5F8BCA35
P 3500 3100
F 0 "#PWR026" H 3500 2950 50  0001 C CNN
F 1 "+3.3V" H 3515 3273 50  0000 C CNN
F 2 "" H 3500 3100 50  0001 C CNN
F 3 "" H 3500 3100 50  0001 C CNN
	1    3500 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3200 5250 3300
Wire Wire Line
	5250 3300 5000 3300
Wire Wire Line
	5000 3300 5000 3400
Wire Wire Line
	5250 3300 5500 3300
Wire Wire Line
	5500 3300 5500 3400
Connection ~ 5250 3300
Wire Wire Line
	5500 3600 5500 3700
Wire Wire Line
	5250 3700 5250 3800
Wire Wire Line
	5250 3700 5500 3700
Wire Wire Line
	5000 3600 5000 3700
Wire Wire Line
	5000 3700 5250 3700
Connection ~ 5250 3700
Wire Wire Line
	3500 3900 3500 4000
Wire Wire Line
	3500 3100 3500 3200
Wire Wire Line
	7500 3100 7500 3200
Wire Wire Line
	7500 3900 7500 4000
Wire Wire Line
	2900 3400 3000 3400
Wire Wire Line
	3100 3600 3000 3600
Wire Wire Line
	3000 3700 3100 3700
Wire Wire Line
	3900 3700 4200 3700
Wire Wire Line
	4000 3500 3900 3500
Wire Wire Line
	3900 3400 4000 3400
Wire Wire Line
	6900 3400 7000 3400
Wire Wire Line
	7100 3600 7000 3600
Wire Wire Line
	7000 3700 7100 3700
Wire Wire Line
	7900 3700 8200 3700
Wire Wire Line
	8000 3500 7900 3500
Wire Wire Line
	7900 3400 8000 3400
$Comp
L Device:R_Small R5
U 1 1 5F8C38FE
P 7000 3200
F 0 "R5" H 6941 3154 50  0000 R CNN
F 1 "10k" H 6941 3245 50  0000 R CNN
F 2 "rhais_rcl:R0603" H 7000 3200 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" H 7000 3200 50  0001 C CNN
F 4 "Multicomp" H 7000 3200 50  0001 C CNN "MFR"
F 5 "MC0063W0603110K" H 7000 3200 50  0001 C CNN "MPN"
F 6 "2130939" H 7000 3200 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0063w0603110k/res-10k-1-0-063w-0603-thick-film/dp/2130939" H 7000 3200 50  0001 C CNN "URL_FARNELL"
	1    7000 3200
	1    0    0    1   
$EndComp
$Comp
L Device:R_Small R4
U 1 1 5F8C4046
P 3000 3200
F 0 "R4" H 2941 3154 50  0000 R CNN
F 1 "10k" H 2941 3245 50  0000 R CNN
F 2 "rhais_rcl:R0603" H 3000 3200 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" H 3000 3200 50  0001 C CNN
F 4 "Multicomp" H 3000 3200 50  0001 C CNN "MFR"
F 5 "MC0063W0603110K" H 3000 3200 50  0001 C CNN "MPN"
F 6 "2130939" H 3000 3200 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0063w0603110k/res-10k-1-0-063w-0603-thick-film/dp/2130939" H 3000 3200 50  0001 C CNN "URL_FARNELL"
	1    3000 3200
	1    0    0    1   
$EndComp
Wire Wire Line
	7000 3300 7000 3400
Connection ~ 7000 3400
Wire Wire Line
	7000 3400 7100 3400
Wire Wire Line
	3000 3300 3000 3400
Connection ~ 3000 3400
Wire Wire Line
	3000 3400 3100 3400
$Comp
L power:+3.3V #PWR030
U 1 1 5F8C5A15
P 7000 3000
F 0 "#PWR030" H 7000 2850 50  0001 C CNN
F 1 "+3.3V" H 7015 3173 50  0000 C CNN
F 2 "" H 7000 3000 50  0001 C CNN
F 3 "" H 7000 3000 50  0001 C CNN
	1    7000 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3000 7000 3100
$Comp
L power:+3.3V #PWR025
U 1 1 5F8C6372
P 3000 3000
F 0 "#PWR025" H 3000 2850 50  0001 C CNN
F 1 "+3.3V" H 3015 3173 50  0000 C CNN
F 2 "" H 3000 3000 50  0001 C CNN
F 3 "" H 3000 3000 50  0001 C CNN
	1    3000 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 3000 3000 3100
$Comp
L Device:R_Small R8
U 1 1 5F90BAC3
P 8100 3400
F 0 "R8" V 7950 3400 50  0000 C CNN
F 1 "10k" V 8025 3400 50  0000 C CNN
F 2 "rhais_rcl:R0603" H 8100 3400 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" H 8100 3400 50  0001 C CNN
F 4 "Multicomp" H 8100 3400 50  0001 C CNN "MFR"
F 5 "MC0063W0603110K" H 8100 3400 50  0001 C CNN "MPN"
F 6 "2130939" H 8100 3400 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0063w0603110k/res-10k-1-0-063w-0603-thick-film/dp/2130939" H 8100 3400 50  0001 C CNN "URL_FARNELL"
	1    8100 3400
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R9
U 1 1 5F90C376
P 8100 3500
F 0 "R9" V 8250 3500 50  0000 C CNN
F 1 "10k" V 8175 3500 50  0000 C CNN
F 2 "rhais_rcl:R0603" H 8100 3500 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" H 8100 3500 50  0001 C CNN
F 4 "Multicomp" H 8100 3500 50  0001 C CNN "MFR"
F 5 "MC0063W0603110K" H 8100 3500 50  0001 C CNN "MPN"
F 6 "2130939" H 8100 3500 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0063w0603110k/res-10k-1-0-063w-0603-thick-film/dp/2130939" H 8100 3500 50  0001 C CNN "URL_FARNELL"
	1    8100 3500
	0    -1   1    0   
$EndComp
$Comp
L Device:R_Small R6
U 1 1 5F9121F8
P 4100 3400
F 0 "R6" V 3950 3400 50  0000 C CNN
F 1 "10k" V 4025 3400 50  0000 C CNN
F 2 "rhais_rcl:R0603" H 4100 3400 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" H 4100 3400 50  0001 C CNN
F 4 "Multicomp" H 4100 3400 50  0001 C CNN "MFR"
F 5 "MC0063W0603110K" H 4100 3400 50  0001 C CNN "MPN"
F 6 "2130939" H 4100 3400 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0063w0603110k/res-10k-1-0-063w-0603-thick-film/dp/2130939" H 4100 3400 50  0001 C CNN "URL_FARNELL"
	1    4100 3400
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R7
U 1 1 5F9121FE
P 4100 3500
F 0 "R7" V 4250 3500 50  0000 C CNN
F 1 "10k" V 4175 3500 50  0000 C CNN
F 2 "rhais_rcl:R0603" H 4100 3500 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" H 4100 3500 50  0001 C CNN
F 4 "Multicomp" H 4100 3500 50  0001 C CNN "MFR"
F 5 "MC0063W0603110K" H 4100 3500 50  0001 C CNN "MPN"
F 6 "2130939" H 4100 3500 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mc0063w0603110k/res-10k-1-0-063w-0603-thick-film/dp/2130939" H 4100 3500 50  0001 C CNN "URL_FARNELL"
	1    4100 3500
	0    -1   1    0   
$EndComp
$Comp
L power:+3.3V #PWR034
U 1 1 5F91313B
P 8300 3300
F 0 "#PWR034" H 8300 3150 50  0001 C CNN
F 1 "+3.3V" H 8315 3473 50  0000 C CNN
F 2 "" H 8300 3300 50  0001 C CNN
F 3 "" H 8300 3300 50  0001 C CNN
	1    8300 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR033
U 1 1 5F9139B2
P 4300 3300
F 0 "#PWR033" H 4300 3150 50  0001 C CNN
F 1 "+3.3V" H 4315 3473 50  0000 C CNN
F 2 "" H 4300 3300 50  0001 C CNN
F 3 "" H 4300 3300 50  0001 C CNN
	1    4300 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3500 4300 3500
Wire Wire Line
	4300 3500 4300 3400
Wire Wire Line
	4200 3400 4300 3400
Connection ~ 4300 3400
Wire Wire Line
	4300 3400 4300 3300
Wire Wire Line
	8200 3500 8300 3500
Wire Wire Line
	8300 3500 8300 3400
Wire Wire Line
	8200 3400 8300 3400
Connection ~ 8300 3400
Wire Wire Line
	8300 3400 8300 3300
Text HLabel 6900 3400 0    50   Input ~ 0
~CS2
Text HLabel 7000 3600 0    50   Input ~ 0
SCK
Text HLabel 7000 3700 0    50   Input ~ 0
COPI
Text HLabel 8200 3700 2    50   Output ~ 0
CIPO
Text HLabel 2900 3400 0    50   Input ~ 0
~CS1
Text HLabel 3000 3600 0    50   Input ~ 0
SCK
Text HLabel 3000 3700 0    50   Input ~ 0
COPI
Text HLabel 4200 3700 2    50   Output ~ 0
CIPO
$EndSCHEMATC
