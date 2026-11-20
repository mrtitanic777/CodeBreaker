; VMM.INC test
Early_Const     EQU   5H
SYS_CRITICAL_INIT EQU 0H
Device_Init	EQU	1H  
  Init_Complete EQU 2h
Sys_VM_Init EQU 3H
Sys_VM_Terminate EQU 4H
SOME_BIT EQU 5H
SOME_FLAG EQU 6H
A_FLAG_ID EQU 7H
Some_Id EQU 8H
DIOC_Thing EQU 9H
Dioc_Lower EQU 0AH
dioc_lower2 EQU 0BH
Duplicate_Value EQU 4H
Another EQU 3Fh
AtLimit EQU 40H
Big EQU 0FFFFFFFFFFFFFFFFFFFFH
Zero_Padded EQU 000000000000000000000000000000012H
Hex_Only EQU 0DEADH
Name_With_9_Digits_123 EQU 0CH
9Bad EQU 0DH
Bad Space EQU 0EH
X EQU 0FH junk
Y EQU 10H ; comment
Z	equ	11H
W EQU 12
Trailing_Space EQU 13H 	 
SYSTEM_EXIT2 EQU 14H
After_Exit EQU 15H
