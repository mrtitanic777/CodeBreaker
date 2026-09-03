; IRLAMP -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 4817 instructions recovered by recursive descent

Declare_Virtual_Device IRLAMP, 1, 0, IRLAMP_Control, 004A2h, Undefined_Init_Order
;   DDK version 0400h   init_order 030000001h   flags 00000h
;   service table: obj1:0x288, 38 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2324  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0200:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0230:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x14]
        VxDCall  APM.Service_0001h               ; cd2001002700
        mov    dword ptr [ebp - 4], eax
        jb     loc_1_025F
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ebp - 4], 0
loc_1_025F:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0267
loc_1_0267:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh
sub_1_0270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        xor    edx, edx
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
        dd  IRLAMP_Service_00        ; fixup
        dd  IRLAMP_Service_01        ; fixup
        dd  IRLAMP_Service_02        ; fixup
        dd  IRLAMP_Service_03        ; fixup
        dd  IRLAMP_Service_04        ; fixup
        dd  IRLAMP_Service_05        ; fixup
        dd  IRLAMP_Service_06        ; fixup
        dd  IRLAMP_Service_07        ; fixup
        dd  IRLAMP_Service_08        ; fixup
        dd  IRLAMP_Service_09        ; fixup
        dd  IRLAMP_Service_10        ; fixup
        dd  IRLAMP_Service_11        ; fixup
        dd  IRLAMP_Service_12        ; fixup
        dd  IRLAMP_Service_13        ; fixup
        dd  IRLAMP_Service_14        ; fixup
        dd  IRLAMP_Service_15        ; fixup
        dd  IRLAMP_Service_16        ; fixup
        dd  IRLAMP_Service_17        ; fixup
        dd  IRLAMP_Service_18        ; fixup
        dd  IRLAMP_Service_19        ; fixup
        dd  IRLAMP_Service_20        ; fixup
        dd  IRLAMP_Service_21        ; fixup
        dd  IRLAMP_Service_22        ; fixup
        dd  IRLAMP_Service_23        ; fixup
        dd  IRLAMP_Service_24        ; fixup
        dd  IRLAMP_Service_25        ; fixup
        dd  IRLAMP_Service_26        ; fixup
        dd  IRLAMP_Service_27        ; fixup
        dd  IRLAMP_Service_28        ; fixup
        dd  IRLAMP_Service_29        ; fixup
        dd  IRLAMP_Service_30        ; fixup
        dd  IRLAMP_Service_31        ; fixup
        dd  IRLAMP_Service_32        ; fixup
        dd  IRLAMP_Service_33        ; fixup
        dd  IRLAMP_Service_34        ; fixup
        dd  IRLAMP_Service_35        ; fixup
        dd  IRLAMP_Service_36        ; fixup
        dd  IRLAMP_Service_37        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  004A2h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'IRLAMP  '                  ; DDB_Name
        dd  030000001h                  ; DDB_Init_Order
        dd  IRLAMP_Control           ; DDB_Control_Proc
        dd  IRLAMP_V86_API           ; DDB_V86_API_Proc
        dd  IRLAMP_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x0288              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0390:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0770:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0778:
        inc    esp
        jbe    loc_1_07E5
        arpl   word ptr [ebp], sp
        add    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x656d614e
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07AC:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x823
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax + 0x61], dl
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    ecx
        outsb  dx, byte ptr [esi]
        jb     loc_1_0820
        jb     loc_1_0826
        add    byte ptr fs:[eax], al
loc_1_07C4:
        dec    ecx
        jb     loc_1_080C
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x64]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x65], dl
        popal  
        jb     loc_1_0838
        push   0x45676e69
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x64]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07E4:
        push   eax
loc_1_07E5:
        outsb  dx, byte ptr [esi]
        jo     0x82d
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x64]
        add    byte ptr [eax], al
loc_1_07F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0800:
        dec    ecx
        push   edx
        dec    esp
        inc    ecx
        push   eax
        inc    esi
        push   edx
        dec    ebp
        push   esi
        pop    eax
        inc    esp
loc_1_080C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0820:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0826:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0838:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0880:
        dec    edi
        inc    ebp
        dec    ebp
        inc    esp
        dec    edi
        dec    esi
        inc    edi
        dec    esp
        push   esi
        pop    eax
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0900:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        imul   esi, dword ptr [edx + 0x6d], 0x652e6e6f
        js     loc_1_098E
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x52], cl
        dec    ebp
        dec    edi
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x52], cl
        dec    esp
        inc    ecx
        dec    ebp
        push   eax
        push   esi
        pop    eax
        inc    esp
        add    byte ptr [eax], al
loc_1_0954:
        inc    ebp
        outsb  dx, byte ptr [esi]
        jne    0x9c5
        pop    esp
        dec    ecx
        outsb  dx, byte ptr [esi]
        jb     loc_1_09BF
        jb     0x9c5
        pop    esp
        inc    esi
        jb     0x9c6
        insd   dword ptr es:[edi], dx
        jb     loc_1_0969
loc_1_0969:
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 0x69], al
        jbe    0x9d6
        jb     loc_1_0973
loc_1_0973:
        add    byte ptr [ebp + 0x6e], al
        jne    loc_1_09E5
        pop    esp
        dec    ecx
        outsb  dx, byte ptr [esi]
        jb     0x9df
        jb     loc_1_09E5
        pop    esp
        inc    esi
        jb     0x9e6
        insd   dword ptr es:[edi], dx
        jb     loc_1_0989
loc_1_0989:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x70], dl
loc_1_098E:
        inc    esp
        jb     0x9fd
        jbe    loc_1_09FB
        jb     loc_1_0998
loc_1_0998:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_1_0A0F
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax + 0x61], dl
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    ecx
        outsb  dx, byte ptr [esi]
        jb     loc_1_0A0C
        jb     0xa12
        add    byte ptr fs:[eax], al
loc_1_09B0:
        dec    esp
        imul   ebp, dword ptr [ebp + 0x69], 0x65705374
        add    byte ptr fs:[eax], al
loc_1_09BC:
        push   ebx
        jo     loc_1_0A24
loc_1_09BF:
        dec    esp
        imul   ebp, dword ptr [ebp + 0x69], 0x53000074
        jns    loc_1_0A3E
        je     loc_1_0A32
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_1_0A44
        jb     loc_1_0A39
        outsb  dx, byte ptr [esi]
        je     loc_1_0A1A
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_1_0A4D
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0xa3d
        push   ebx
        jb     0xa5b
loc_1_09E5:
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        push   esi
        dec    esi
        inc    ebp
        push   esp
        push   ebx
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], al
        insd   dword ptr es:[edi], dx
loc_1_09FB:
        insd   dword ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        je     loc_1_0A00
loc_1_0A00:
        push   ebx
        jns    0xa76
        je     0xa6a
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_1_0A7C
        jb     loc_1_0A71
loc_1_0A0C:
        outsb  dx, byte ptr [esi]
        je     0xa52
loc_1_0A0F:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0xa85
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_1_0A75
        push   ebx
loc_1_0A1A:
        jb     loc_1_0A93
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
loc_1_0A24:
        inc    esp
        pop    esp
        push   esi
        dec    esi
        inc    ebp
        push   esp
        push   ebx
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], al
loc_1_0A32:
        insd   dword ptr es:[edi], dx
        jo     loc_1_0AAA
        je     0xa9c
        jb     0xa87
loc_1_0A39:
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
loc_1_0A3E:
        add    byte ptr [eax], al
loc_1_0A40:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A44:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        int3   
        int3   
        int3   
        int3   
        int3   
loc_1_0A4D:
        int3   
        int3   
        int3   
IRLAMP_Control:
        cmp    eax, 0x1b
        je     loc_1_0A7C
        cmp    eax, 1
        je     loc_1_0A7C
        cmp    eax, 0x23
        jne    loc_1_0A6C
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_15C0
        cmp    eax, 1
        ret    
loc_1_0A6C:
        cmp    eax, 0x22
        jne    loc_1_0A7A
loc_1_0A71:
        push   edx
        push   ebx
        db  0E8h
        dd  loc_2_0530               ; fixup
loc_1_0A75:
        add    byte ptr [eax], al
        add    cl, bh
        ret    
loc_1_0A7A:
        clc    
        ret    
loc_1_0A7C:
        clc    
        ret    
IRLAMP_V86_API:
        push   esi
        xchg   esi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    dword ptr [ebp + 0x1c], eax
        pop    esi
        clc    
        ret    
IRLAMP_PM_API:
        push   esi
        xchg   esi, eax
loc_1_0A93:
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [ebp + 0x1c]
        and    eax, 0xffff
        mov    dword ptr [ebp + 0x1c], eax
        pop    esi
loc_1_0AAA:
        clc    
        ret    
sub_1_0AAC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0xffffffff
        mov    edx, dword ptr [ebp + 8]
        VxDCall  APM.Service_0002h               ; cd2002002700
        jb     loc_1_0AC4
        xor    eax, eax
loc_1_0AC4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0AC9:
        VxDCall  SHELL._SHELL_ShellExecute       ; cd2007801700
        int3   
sub_1_0AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_0AD9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     loc_1_0B02
        jmp    loc_1_0AD9
loc_1_0B02:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0B07:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 4
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1C8C
loc_1_0B32:
        add    esp, 0x18
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0B79
        cmp    dword ptr [ebp - 8], 4
        jne    loc_1_0B72
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0B64
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 1
        jmp    loc_1_0B6D
loc_1_0B64:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
loc_1_0B6D:
        jmp    loc_1_0B79
loc_1_0B72:
        mov    dword ptr [ebp - 0x10], 1
loc_1_0B79:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_0B81
loc_1_0B81:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0B86:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0], 0              ; -> obj1:0x0374
        mov    eax, dword ptr [0]            ; -> obj1:0x0374
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0], eax            ; -> obj1:0x0370
        lea    eax, [ebp - 0xc]
        push   eax
        push   0                             ; -> loc_1_07AC
        push   0x80000001
        call   sub_2_1C9C
loc_1_0BBC:
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0C48
        push   0                             ; -> obj1:0x0370
        push   0                             ; -> loc_1_07C4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0B07
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0C37
        cmp    dword ptr [0], 0              ; -> obj1:0x0370
        je     loc_1_0C37
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj1:0x07D0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0B07
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0C37
        push   0                             ; -> obj1:0x0374
        push   0                             ; -> loc_1_07E4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0B07
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
loc_1_0C37:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_1C94
loc_1_0C40:
        add    esp, 4
        jmp    loc_1_0C6C
loc_1_0C48:
        mov    dword ptr [0], 1              ; -> obj1:0x0374
        mov    eax, dword ptr [0]            ; -> obj1:0x0374
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0], eax            ; -> obj1:0x0370
        mov    eax, 1
        jmp    loc_1_0C87
loc_1_0C6C:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0C80
        mov    eax, 1
        jmp    loc_1_0C82
loc_1_0C80:
        xor    eax, eax
loc_1_0C82:
        jmp    loc_1_0C87
loc_1_0C87:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0C8C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x0378
        jne    loc_1_0CB3
        mov    dword ptr [0], 1              ; -> obj1:0x0378
        call   sub_1_0B86
        mov    dword ptr [0], eax            ; -> obj1:0x0378
loc_1_0CB3:
        mov    eax, dword ptr [0]            ; -> obj1:0x0370
        jmp    loc_1_0CBD
loc_1_0CBD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0CC2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0], 0              ; -> obj1:0x0384
        mov    eax, dword ptr [0]            ; -> obj1:0x0384
        mov    dword ptr [0], eax            ; -> obj1:0x0380
        mov    dword ptr [0], 0              ; -> obj1:0x0388
        mov    dword ptr [0], 0              ; -> obj1:0x038C
        mov    dword ptr [0], 0              ; -> loc_1_0770
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0CFF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_0D16
        mov    byte ptr [8], 0               ; -> obj1:0x0088
loc_1_0D16:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0D1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0DDB
loc_1_0D2F:
        mov    dword ptr [0x1c], 0           ; -> obj1:0x005C
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0x20], eax         ; -> obj1:0x0060
        mov    byte ptr [0x2c], 4            ; -> obj1:0x006C
        mov    dword ptr [0x28], 0x1388      ; -> obj1:0x0068
        mov    dword ptr [0x2d], 0           ; -> obj1:0x006D
        mov    byte ptr [0], 6               ; -> obj1:0x0190
        mov    dword ptr [1], 0              ; -> obj1:0x0191
        mov    byte ptr [5], 0xa             ; -> obj1:0x0195
        mov    dword ptr [6], 0              ; -> obj1:0x0196
        mov    word ptr [0x33], 0x109        ; -> obj1:0x0073
        mov    dword ptr [0x35], 0           ; -> obj1:0x0075
        push   0                             ; -> obj1:0x0040
        call   IRLAMP_Service_24
        add    esp, 4
        jmp    loc_1_0DF4
loc_1_0DA3:
        mov    al, byte ptr [7]              ; -> obj1:0x0087
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx], al
        mov    al, byte ptr [8]              ; -> obj1:0x0088
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 1], al
        xor    eax, eax
        mov    al, byte ptr [8]              ; -> obj1:0x0088
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        lea    edx, [9]                      ; -> obj1:0x0089
        add    ecx, 2
        call   sub_2_02F3
loc_1_0DD1:
        jmp    loc_1_0DF4
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_0DDB:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0D2F
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_0DA3
        jmp    loc_1_0DF4
loc_1_0DF4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0DF9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0E04:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        jmp    loc_1_0E11
loc_1_0E11:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0E16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> obj1:0x0388
        mov    eax, dword ptr [0]            ; -> obj1:0x0388
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0EC4
loc_1_0E34:
        mov    dword ptr [0], 0              ; -> obj1:0x0380
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        mov    dword ptr [0], eax            ; -> obj1:0x0384
        jmp    loc_1_0F19
loc_1_0E4D:
        mov    dword ptr [0], 0              ; -> obj1:0x038C
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj1:0x0380
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        cmp    dword ptr [0], eax            ; -> obj1:0x0384
        je     loc_1_0E8A
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        mov    dword ptr [0], eax            ; -> obj1:0x0384
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        push   eax
        push   0
        call   sub_1_0D1B
        add    esp, 8
loc_1_0E8A:
        jmp    loc_1_0F19
loc_1_0E8F:
        call   sub_2_1CA4
loc_1_0E94:
        mov    dword ptr [0], eax            ; -> obj1:0x038C
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj1:0x0380
        jmp    loc_1_0F19
loc_1_0EA6:
        mov    dword ptr [0], 0              ; -> obj1:0x0380
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        mov    dword ptr [0], eax            ; -> obj1:0x0384
        jmp    loc_1_0F19
        db  0E9h, 055h, 000h, 000h, 000h
loc_1_0EC4:
        sub    dword ptr [ebp - 4], 0x17
        cmp    dword ptr [ebp - 4], 0x19
        ja     loc_1_0EA6
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x0EFF
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x0EE7
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_0E34
        dd  loc_1_0E4D
        dd  loc_1_0E8F
        dd  loc_1_0E34
        dd  loc_1_0E4D
        dd  loc_1_0EA6
        db  000h, 000h, 001h, 001h, 002h, 005h, 003h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h
        db  005h, 005h, 005h, 005h, 005h, 005h, 005h, 004h, 004h, 004h
loc_1_0F19:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0F1E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0E04
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        call   sub_1_0DF9
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0F58
loc_1_0F55:
        inc    dword ptr [ebp - 4]
loc_1_0F58:
        cmp    dword ptr [ebp - 4], 0x14
        jae    loc_1_0F88
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4]
        cmp    dword ptr [eax + eax*8], 0    ; -> loc_1_03E8
        je     loc_1_0F83
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4]
        dec    dword ptr [eax + eax*8]       ; -> loc_1_03E8
loc_1_0F83:
        jmp    loc_1_0F55
loc_1_0F88:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0F97
loc_1_0F94:
        inc    dword ptr [ebp - 4]
loc_1_0F97:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_0FC2
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4
        push   eax
        call   sub_1_0E04
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0F94
loc_1_0FC2:
        call   sub_1_0DF9
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [0]            ; -> obj1:0x037C
        je     loc_1_100B
        cmp    dword ptr [0], 0              ; -> obj3:0x0130
        je     loc_1_100B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    dword ptr [0], eax            ; -> obj1:0x037C
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0130
        push   eax
        call   sub_2_1CAC
loc_1_1008:
        add    esp, 8
loc_1_100B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1010:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0x2d
        jne    loc_1_1025
        call   sub_1_0CC2
loc_1_1025:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_102A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0                             ; -> obj1:0x0000
        call   sub_1_0AD0
        add    esp, 8
        cmp    dword ptr [0], 0              ; -> loc_1_0770
        jne    loc_1_10D1
        push   0                             ; -> sub_1_0390
        call   IRLAMP_Service_28
        add    esp, 4
        mov    dword ptr [0x1c], 0           ; -> loc_1_03E4
        mov    dword ptr [0xc], 0            ; -> loc_1_03D4
        mov    dword ptr [8], 0              ; -> loc_1_03D0
        mov    dword ptr [0x18], 0           ; -> loc_1_03E0
        push   0                             ; -> obj1:0x0000
        push   0
        push   0                             ; -> loc_1_03C8
        push   0                             ; -> sub_1_0390
        push   0xffff
        call   IRLAMP_Service_04
        add    esp, 0x14
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_10C7
        mov    dword ptr [0], 1              ; -> loc_1_0770
        jmp    loc_1_10D1
loc_1_10C7:
        mov    dword ptr [0], 0              ; -> loc_1_0770
loc_1_10D1:
        call   sub_1_1F21
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_10DE
loc_1_10DE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_10E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0x41
        je     loc_1_1114
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0x41
        mov    eax, 0x18
        jmp    loc_1_1128
loc_1_1114:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_102A
        add    esp, 4
        jmp    loc_1_1128
loc_1_1128:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_112F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [0], 0              ; -> loc_1_0770
        je     loc_1_1166
        push   0                             ; -> sub_1_0390
        call   IRLAMP_Service_05
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [0], 0              ; -> loc_1_0770
loc_1_1166:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_116E
loc_1_116E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_1175:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0x10
        je     loc_1_11AD
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0x10
        mov    eax, 0x18
        jmp    loc_1_12AA
loc_1_11AD:
        mov    dword ptr [0], 1              ; -> obj1:0x0378
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_11D8
        push   0
        call   IRLAMP_Service_32
        add    esp, 4
        mov    dword ptr [0], 0              ; -> obj1:0x0374
loc_1_11D8:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     loc_1_122C
        cmp    dword ptr [0], 0              ; -> obj1:0x0370
        jne    loc_1_122C
        mov    dword ptr [0], 1              ; -> obj1:0x0370
        call   IRLAMP_Service_02
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_121D
        mov    dword ptr [0], 1              ; -> obj1:0x0370
        jmp    loc_1_1227
loc_1_121D:
        mov    dword ptr [0], 0              ; -> obj1:0x0370
loc_1_1227:
        jmp    loc_1_1271
loc_1_122C:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_1271
        cmp    dword ptr [0], 0              ; -> obj1:0x0370
        je     loc_1_1271
        call   IRLAMP_Service_03
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1267
        mov    dword ptr [0], 1              ; -> obj1:0x0370
        jmp    loc_1_1271
loc_1_1267:
        mov    dword ptr [0], 0              ; -> obj1:0x0370
loc_1_1271:
        cmp    dword ptr [0], 0              ; -> obj1:0x0370
        je     loc_1_129D
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   IRLAMP_Service_32
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [0], eax            ; -> obj1:0x0374
        jmp    loc_1_12A2
loc_1_129D:
        call   sub_1_0CC2
loc_1_12A2:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_12AA
loc_1_12AA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_12B1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax], 0x370
        je     loc_1_12E5
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], 0x370
        mov    eax, 0x18
        jmp    loc_1_150C
loc_1_12E5:
        cmp    dword ptr [0], 0              ; -> obj1:0x0380
        je     loc_1_13FE
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x228], 1
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x36c], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x32e], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x32e]
        and    eax, 0xff
        cmp    eax, 0x21
        jl     loc_1_1339
        mov    eax, 0x21
loc_1_1339:
        push   eax
        mov    edx, dword ptr [0]            ; -> obj1:0x0380
        mov    ecx, dword ptr [ebp - 0x10]
        add    edx, 8
        add    ecx, 0x32f
        call   sub_2_02F3
loc_1_1351:
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x22c
        push   eax
        push   1
        call   sub_1_0D1B
        add    esp, 8
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        push   eax
        call   IRLAMP_Service_17
        add    esp, 8
        mov    ecx, 0x3e8
        mov    eax, dword ptr [ebp - 0x1c]
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x350], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x350], 3
        jbe    loc_1_13A5
        mov    eax, dword ptr [ebp - 0x10]
        sub    dword ptr [eax + 0x350], 3
loc_1_13A5:
        mov    eax, dword ptr [0]            ; -> obj1:0x038C
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x360], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0380
        push   eax
        call   IRLAMP_Service_20
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x14]
        sub    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x358], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x14]
        sub    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x35c], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x354], eax
        jmp    loc_1_14DF
loc_1_13FE:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x228], 0
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x36c
        push   eax
        push   0
        call   sub_1_2289
        add    esp, 8
        call   IRLAMP_Service_16
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_14DF
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_1450
loc_1_144D:
        inc    dword ptr [ebp - 0xc]
loc_1_1450:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0xc]
        jle    loc_1_14DF
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 0x2c]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, ecx
        shl    ecx, 5
        add    ecx, edx
        add    ecx, edx
        mov    edx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + edx + 8], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        shl    eax, 5
        add    eax, ecx
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + ecx + 8]
        and    eax, 0xff
        cmp    eax, 0x21
        jl     loc_1_14AF
        mov    eax, 0x21
loc_1_14AF:
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 0xc]
        mov    edx, ebx
        shl    ebx, 5
        add    ebx, edx
        add    ebx, edx
        mov    esi, dword ptr [ebp - 0x10]
        lea    edx, [eax + ecx + 0xc]
        lea    ecx, [ebx + esi + 9]
        call   sub_2_02F3
loc_1_14DA:
        jmp    loc_1_144D
loc_1_14DF:
        call   IRLAMP_Service_19
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x364], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x368], eax
        xor    eax, eax
        jmp    loc_1_150C
loc_1_150C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_1513:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0xc
        je     loc_1_1544
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0xc
        mov    eax, 0x18
        jmp    loc_1_1561
loc_1_1544:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_16FC
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_1561
loc_1_1561:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_1568:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        push   1
        call   sub_1_2289
        add    esp, 8
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        mov    dword ptr [eax], 4
        xor    eax, eax
        jmp    loc_1_1592
loc_1_1592:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_1599:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        push   2
        call   sub_1_2289
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_15B9
loc_1_15B9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_15C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_15E6
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_163B
loc_1_15E6:
        cmp    dword ptr [ebp + 8], -1
        jne    loc_1_15FC
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_163B
loc_1_15FC:
        cmp    dword ptr [ebp + 8], 0x100
        jb     loc_1_1634
        cmp    dword ptr [ebp + 8], 0x106
        ja     loc_1_1634
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   sub_1_0390
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_163B
loc_1_1634:
        mov    dword ptr [ebp - 4], 0x32
loc_1_163B:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1643
loc_1_1643:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
IRLAMP_Service_00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        clc    
        mov    eax, 0x100
        jmp    loc_1_165B
loc_1_165B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  003h, 0B6h, 000h, 000h, 000h
sub_1_1665:
        push   ecx
        push   esi
        sub    esp, 4
        mov    esi, esp
        pushfd 
        cli    
        in     al, 0x61
        mov    byte ptr [esi], al
        and    al, 0xfc
        out    0x61, al
        mov    al, 0xe8
        out    0x43, al
        in     al, 0x42
        or     al, 0x80
        and    al, 0xbf
        mov    byte ptr [esi + 1], al
        or     al, 0x30
        out    0x43, al
        in     al, 0x42
        mov    byte ptr [esi + 2], al
        mov    al, byte ptr [edi + 2]
        out    0x42, al
        in     al, 0x42
        mov    byte ptr [esi + 3], al
        mov    al, byte ptr [edi + 3]
        out    0x42, al
        mov    al, byte ptr [edi + 1]
        out    0x43, al
        mov    ah, al
        test   ah, 0x10
        je     loc_1_16AC
        mov    al, byte ptr [edi + 2]
        out    0x42, al
loc_1_16AC:
        test   ah, 0x20
        je     loc_1_16B6
        mov    al, byte ptr [edi + 3]
        out    0x42, al
loc_1_16B6:
        mov    ah, byte ptr [edi]
        and    ah, 3
        in     al, 0x61
        and    al, 0xfc
        or     al, ah
        out    0x61, al
        popfd  
        mov    ecx, 4
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    esp, 4
        pop    esi
        pop    ecx
        sub    eax, eax
        ret    
sub_1_16D4:
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    edi, 0                        ; -> obj1:0x1660
        call   sub_1_1665
        mov    byte ptr [0], 0               ; -> obj1:0x1664
        push   0                             ; -> obj1:0x1660
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_16FC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   es
        mov    edi, 0                        ; -> obj1:0x1660
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [edi + 3], ah
        call   sub_1_1665
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 0
        lea    esi, [0]                      ; -> sub_1_16D4
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        or     esi, esi
        jne    loc_1_1735
        call   sub_1_16D4
        jmp    loc_1_1756
loc_1_1735:
        mov    byte ptr [0], 1               ; -> obj1:0x1664
        push   0
        push   0                             ; -> obj1:0x1660
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        lea    esp, [esp + 8]
        cmp    byte ptr [0], 0               ; -> obj1:0x1664
        jne    loc_1_1735
loc_1_1756:
        pop    es
        pop    esi
        pop    edi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_1760:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0                             ; -> loc_1_0900
        call   sub_1_0AC9
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
IRLAMP_Service_02:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        call   sub_1_0C8C
        test   eax, eax
        jne    loc_1_179F
        mov    eax, 0x2d
        jmp    loc_1_1946
loc_1_179F:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_193E
        mov    dword ptr [ebp - 8], 0x80
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> loc_1_0880
        call   sub_1_1D22
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1802
        cmp    dword ptr [0], 0              ; -> loc_1_0A40
        jne    loc_1_1802
        push   0                             ; -> loc_1_0880
        call   sub_1_1AE4
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1802
        mov    dword ptr [0], 1              ; -> loc_1_07F4
loc_1_1802:
        cmp    dword ptr [0], 0              ; -> loc_1_07F8
        jne    loc_1_185D
        mov    dword ptr [ebp - 8], 0x80
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> loc_1_0800
        call   sub_1_1CC0
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_183E
        mov    eax, 0x2a
        jmp    loc_1_1946
loc_1_183E:
        push   0                             ; -> loc_1_0800
        call   sub_1_1A77
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_185D
        jmp    loc_1_1926
loc_1_185D:
        call   sub_1_1B51
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_1874
        jmp    loc_1_18FB
loc_1_1874:
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_07F8
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        call   dword ptr [eax + 0x64]
        add    esp, 0xc
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_18A7
        call   sub_1_1C6E
        jmp    loc_1_18FB
        db  0E9h, 054h, 000h, 000h, 000h
loc_1_18A7:
        mov    dword ptr [0], 1              ; -> loc_1_07F0
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_1D84
        add    esp, 4
        test   eax, eax
        je     loc_1_18D3
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   2
        call   sub_1_2289
        add    esp, 8
loc_1_18D3:
        call   sub_1_1F21
        push   0                             ; -> obj1:0x092C
        call   sub_1_102A
        add    esp, 4
        push   0
        push   0
        push   0                             ; -> loc_1_1760
        call   sub_1_0200
        add    esp, 0xc
        jmp    loc_1_1926
loc_1_18FB:
        call   sub_1_1BBA
        mov    dword ptr [0], 0              ; -> loc_1_07F8
        cmp    dword ptr [0], 0              ; -> loc_1_07F4
        je     loc_1_1926
        call   sub_1_1C14
        mov    dword ptr [0], 0              ; -> loc_1_07F4
loc_1_1926:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_231C
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_231C
        add    esp, 4
loc_1_193E:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_1946
loc_1_1946:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
IRLAMP_Service_01:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_07F8
        jne    loc_1_1970
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_07F8
        mov    eax, 1
        jmp    loc_1_1977
loc_1_1970:
        xor    eax, eax
        jmp    loc_1_1977
loc_1_1977:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_07F8
        je     loc_1_19A3
        mov    dword ptr [0], 0              ; -> loc_1_07F8
        mov    eax, 1
        jmp    loc_1_19AA
loc_1_19A3:
        xor    eax, eax
        jmp    loc_1_19AA
loc_1_19AA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
IRLAMP_Service_33:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_07FC
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
IRLAMP_Service_03:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        je     loc_1_1A24
        mov    dword ptr [0], 0              ; -> loc_1_07F0
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        call   dword ptr [eax + 0x68]
        mov    dword ptr [ebp - 4], eax
        call   sub_1_1C6E
        call   sub_1_1BBA
        cmp    dword ptr [0], 0              ; -> loc_1_07F4
        je     loc_1_1A1A
        call   sub_1_1C14
        mov    dword ptr [0], 0              ; -> loc_1_07F4
loc_1_1A1A:
        mov    dword ptr [0], 0              ; -> loc_1_07FC
loc_1_1A24:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1A2C
loc_1_1A2C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_1A40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_1A49:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     loc_1_1A72
        jmp    loc_1_1A49
loc_1_1A72:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1A77:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x0938
        jne    loc_1_1AD8
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0230
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1ABA
        mov    eax, 0x2a
        jmp    loc_1_1ADF
loc_1_1ABA:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        push   0                             ; -> obj1:0x01C0
        call   sub_1_1A40
        add    esp, 8
        mov    dword ptr [0], 1              ; -> obj1:0x0938
loc_1_1AD8:
        xor    eax, eax
        jmp    loc_1_1ADF
loc_1_1ADF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1AE4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x0940
        jne    loc_1_1B45
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0230
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1B27
        mov    eax, 0x35
        jmp    loc_1_1B4C
loc_1_1B27:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        push   0                             ; -> obj1:0x01E0
        call   sub_1_1A40
        add    esp, 8
        mov    dword ptr [0], 1              ; -> obj1:0x0940
loc_1_1B45:
        xor    eax, eax
        jmp    loc_1_1B4C
loc_1_1B4C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1B51:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x093C
        jne    loc_1_1BAE
        push   1
        push   0                             ; -> obj1:0x0948
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0230
        add    esp, 0x10
        test   eax, eax
        je     loc_1_1B90
        mov    eax, 1
        jmp    loc_1_1BB5
loc_1_1B90:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        push   0                             ; -> obj1:0x01A0
        call   sub_1_1A40
        add    esp, 8
        mov    dword ptr [0], 1              ; -> obj1:0x093C
loc_1_1BAE:
        xor    eax, eax
        jmp    loc_1_1BB5
loc_1_1BB5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1BBA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x0938
        je     loc_1_1BFE
        push   0                             ; -> obj1:0x01C0
        call   sub_1_0AAC
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1BF4
        mov    eax, 1
        jmp    loc_1_1C0F
loc_1_1BF4:
        mov    dword ptr [0], 0              ; -> obj1:0x0938
loc_1_1BFE:
        mov    dword ptr [0], 0              ; -> loc_1_07F8
        xor    eax, eax
        jmp    loc_1_1C0F
loc_1_1C0F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1C14:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x0940
        je     loc_1_1C58
        push   0                             ; -> obj1:0x01E0
        call   sub_1_0AAC
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1C4E
        mov    eax, 1
        jmp    loc_1_1C69
loc_1_1C4E:
        mov    dword ptr [0], 0              ; -> obj1:0x0940
loc_1_1C58:
        mov    dword ptr [0], 0              ; -> loc_1_0A40
        xor    eax, eax
        jmp    loc_1_1C69
loc_1_1C69:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1C6E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x093C
        je     loc_1_1CB2
        push   0                             ; -> obj1:0x01A0
        call   sub_1_0AAC
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1CA8
        mov    eax, 1
        jmp    loc_1_1CB9
loc_1_1CA8:
        mov    dword ptr [0], 0              ; -> obj1:0x093C
loc_1_1CB2:
        xor    eax, eax
        jmp    loc_1_1CB9
loc_1_1CB9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_1CC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> loc_1_0954
        push   0x80000002
        call   sub_2_1C9C
loc_1_1CDC:
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_1D16
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0                             ; -> obj1:0x096C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1C8C
loc_1_1D03:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_1D16
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1D1D
loc_1_1D16:
        xor    eax, eax
        jmp    loc_1_1D1D
loc_1_1D1D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1D22:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj1:0x0974
        push   0x80000002
        call   sub_2_1C9C
loc_1_1D3E:
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_1D78
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0                             ; -> obj1:0x098C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1C8C
loc_1_1D65:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_1D78
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1D7F
loc_1_1D78:
        xor    eax, eax
        jmp    loc_1_1D7F
loc_1_1D7F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1D84:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> loc_1_0998
        push   0x80000001
        call   sub_2_1C9C
loc_1_1DA9:
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_1E47
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0                             ; -> loc_1_09B0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1C8C
loc_1_1DD7:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_1E47
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     loc_1_1E47
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0                             ; -> loc_1_09BC
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1C8C
loc_1_1E11:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_1E47
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        test   dword ptr [ebp - 0x10], eax
        jne    loc_1_1E3E
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_1E4E
        db  0E9h, 009h, 000h, 000h, 000h
loc_1_1E3E:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
loc_1_1E47:
        xor    eax, eax
        jmp    loc_1_1E4E
loc_1_1E4E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1E53:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj1:0x09C8
        push   0x80000002
        call   sub_2_1C9C
loc_1_1E6F:
        add    esp, 0xc
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1EAD
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0                             ; -> obj1:0x09F8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1C8C
loc_1_1E9B:
        add    esp, 0x18
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1C94
loc_1_1EAA:
        add    esp, 4
loc_1_1EAD:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_1EB5
loc_1_1EB5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1EBA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> loc_1_0A00
        push   0x80000002
        call   sub_2_1C9C
loc_1_1ED6:
        add    esp, 0xc
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1F14
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0                             ; -> obj1:0x0A30
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1C8C
loc_1_1F02:
        add    esp, 0x18
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1C94
loc_1_1F11:
        add    esp, 4
loc_1_1F14:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_1F1C
loc_1_1F1C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1F21:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x108
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0x1e
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        call   sub_1_1EBA
        add    esp, 8
        test   eax, eax
        jne    loc_1_1F79
        push   0
        lea    eax, [ebp - 0x108]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   IRLAMP_Service_21
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1F79
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1FCC
loc_1_1F79:
        mov    dword ptr [ebp - 8], 0x100
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        call   sub_1_1E53
        add    esp, 8
        test   eax, eax
        jne    loc_1_1FC5
        push   0
        lea    eax, [ebp - 0x108]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   IRLAMP_Service_27
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1FC5
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1FCC
loc_1_1FC5:
        xor    eax, eax
        jmp    loc_1_1FCC
loc_1_1FCC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
IRLAMP_Service_28:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_1FE3
        mov    eax, 0x2d
        ret    
loc_1_1FE3:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 4]
IRLAMP_Service_29:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_1FFA
        mov    eax, 0x2d
        ret    
loc_1_1FFA:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 8]
IRLAMP_Service_30:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2011
        mov    eax, 0x2d
        ret    
loc_1_2011:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0xc]
IRLAMP_Service_04:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2028
        mov    eax, 0x2d
        ret    
loc_1_2028:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x10]
IRLAMP_Service_05:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_203F
        mov    eax, 0x2d
        ret    
loc_1_203F:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x14]
IRLAMP_Service_06:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2056
        mov    eax, 0x2d
        ret    
loc_1_2056:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x18]
IRLAMP_Service_07:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_206D
        mov    eax, 0x2d
        ret    
loc_1_206D:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x1c]
IRLAMP_Service_08:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2084
        mov    eax, 0x2d
        ret    
loc_1_2084:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x20]
IRLAMP_Service_09:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_209B
        mov    eax, 0x2d
        ret    
loc_1_209B:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x24]
IRLAMP_Service_12:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_20B2
        mov    eax, 0x2d
        ret    
loc_1_20B2:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x28]
IRLAMP_Service_10:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_20C9
        mov    eax, 0x2d
        ret    
loc_1_20C9:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x2c]
IRLAMP_Service_11:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_20E0
        mov    eax, 0x2d
        ret    
loc_1_20E0:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x30]
IRLAMP_Service_13:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_20F7
        mov    eax, 0x2d
        ret    
loc_1_20F7:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x34]
IRLAMP_Service_15:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_210E
        mov    eax, 0x2d
        ret    
loc_1_210E:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x38]
IRLAMP_Service_16:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2125
        mov    eax, 0
        ret    
loc_1_2125:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x3c]
IRLAMP_Service_17:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_213C
        mov    eax, 0x2d
        ret    
loc_1_213C:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x40]
IRLAMP_Service_18:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2153
        mov    eax, 0x2d
        ret    
loc_1_2153:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x44]
IRLAMP_Service_19:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_216A
        mov    eax, 0
        ret    
loc_1_216A:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x48]
IRLAMP_Service_20:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2181
        mov    eax, 0
        ret    
loc_1_2181:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x4c]
IRLAMP_Service_21:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2198
        mov    eax, 0x2d
        ret    
loc_1_2198:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x50]
IRLAMP_Service_31:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_21AF
        mov    eax, 0x2d
        ret    
loc_1_21AF:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x54]
IRLAMP_Service_22:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_21C6
        mov    eax, 0x2d
        ret    
loc_1_21C6:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x58]
IRLAMP_Service_32:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_21DD
        mov    eax, 0x2d
        ret    
loc_1_21DD:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x5c]
IRLAMP_Service_23:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_21F4
        mov    eax, 0x2d
        ret    
loc_1_21F4:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x60]
IRLAMP_Service_24:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_220B
        mov    eax, 0x2d
        ret    
loc_1_220B:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x6c]
IRLAMP_Service_25:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2222
        mov    eax, 0x2d
        ret    
loc_1_2222:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x70]
IRLAMP_Service_26:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2239
        mov    eax, 0x2d
        ret    
loc_1_2239:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x74]
IRLAMP_Service_27:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2250
        mov    eax, 0x2d
        ret    
loc_1_2250:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x78]
IRLAMP_Service_14:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2267
        mov    eax, 0x2d
        ret    
loc_1_2267:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x7c]
IRLAMP_Service_34:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_227E
        mov    eax, 0x2d
        ret    
loc_1_227E:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x80]
sub_1_2289:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        jne    loc_1_2298
        mov    eax, 0x2d
        ret    
loc_1_2298:
        mov    eax, dword ptr [0]            ; -> loc_1_07FC
        jmp    dword ptr [eax + 0x84]
        db  0CCh
sub_1_22A4:
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
        db  0CCh
IRLAMP_Service_35:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0], eax            ; -> loc_1_0A40
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_0A44
        ret    
IRLAMP_Service_36:
        mov    dword ptr [0], 0              ; -> loc_1_0A40
        mov    dword ptr [0], 0              ; -> loc_1_0A44
        ret    
IRLAMP_Service_37:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_0A40
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_0A44
        mov    dword ptr [eax], ecx
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_231C:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1d1a  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_001D
        xor    eax, eax
        jmp    loc_2_0033
loc_2_001D:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx - 1]
        jmp    loc_2_0033
loc_2_0033:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0xc], dl
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0062
        mov    eax, 0xffffffff
        jmp    loc_2_009F
loc_2_0062:
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     loc_2_0095
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ebp - 0xc]
        cmp    eax, ecx
        jne    loc_2_008A
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_009F
loc_2_008A:
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp - 4]
        jmp    loc_2_0062
loc_2_0095:
        mov    eax, 0xffffffff
        jmp    loc_2_009F
loc_2_009F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0xc], dl
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_00CB
        xor    eax, eax
        jmp    loc_2_0105
loc_2_00CB:
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     loc_2_00FE
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ebp - 0xc]
        cmp    eax, ecx
        jne    loc_2_00F3
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_2_0105
loc_2_00F3:
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp - 4]
        jmp    loc_2_00CB
loc_2_00FE:
        xor    eax, eax
        jmp    loc_2_0105
loc_2_0105:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_010A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_012D
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0134
loc_2_012D:
        xor    eax, eax
        jmp    loc_2_0168
loc_2_0134:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 8]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_2_0161
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_2_0161
        mov    eax, 1
        jmp    loc_2_0163
loc_2_0161:
        xor    eax, eax
loc_2_0163:
        jmp    loc_2_0168
loc_2_0168:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_2_016F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_018A
        jmp    loc_2_01D1
loc_2_018A:
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     loc_2_01CC
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        cmp    eax, 0x61
        jl     loc_2_01C4
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        cmp    eax, 0x7a
        jg     loc_2_01C4
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        sub    eax, 0x20
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
loc_2_01C4:
        inc    dword ptr [ebp - 4]
        jmp    loc_2_018A
loc_2_01CC:
        jmp    loc_2_01D1
loc_2_01D1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_01D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    edx, edi
        mov    ebx, ecx
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_021B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    edx, edi
        mov    edi, dword ptr [ebp - 4]
        mov    esi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0254:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0x10], dl
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 4], 0
loc_2_026A:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_02B3
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 0xc], eax
        jle    loc_2_02B3
        mov    eax, dword ptr [ebp - 0xc]
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ebp - 0x10]
        cmp    eax, ecx
        jne    loc_2_02AB
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0xc]
        jmp    loc_2_02AE
loc_2_02AB:
        inc    dword ptr [ebp - 0xc]
loc_2_02AE:
        jmp    loc_2_026A
loc_2_02B3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_2_02BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0xc], dl
        mov    dword ptr [ebp - 8], ecx
loc_2_02C9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_02EC
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        jmp    loc_2_02C9
loc_2_02EC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_2_02F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], edx
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_0310
loc_2_030D:
        dec    dword ptr [ebp - 4]
loc_2_0310:
        cmp    dword ptr [ebp - 4], 0
        jbe    loc_2_032F
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 8]
        jmp    loc_2_030D
loc_2_032F:
        mov    eax, dword ptr [ebp + 8]
        jmp    loc_2_0337
loc_2_0337:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  0CCh, 0CCh
        push   ebp
        mov    ebp, esp
        sub    esp, 0x114
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x54]
        push   eax
        push   0                             ; -> obj3:0x0000
        push   0x80000002
        call   sub_2_1C9C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0521
        mov    dword ptr [ebp - 0x100], 0
loc_2_0374:
        mov    eax, dword ptr [ebp - 0x100]
        mov    dword ptr [ebp - 0x114], eax
        inc    dword ptr [ebp - 0x100]
        push   0x50
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x114]
        push   eax
        mov    eax, dword ptr [ebp - 0x54]
        push   eax
        call   sub_2_1CBC
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0515
        lea    eax, [ebp - 0xac]
        push   eax
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x54]
        push   eax
        call   sub_2_1C9C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0510
        mov    dword ptr [ebp - 0x104], 0x50
        lea    eax, [ebp - 0x104]
        push   eax
        lea    eax, [ebp - 0xfc]
        push   eax
        push   0
        push   0
        push   0                             ; -> obj3:0x0014
        mov    eax, dword ptr [ebp - 0xac]
        push   eax
        call   sub_2_1C8C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_0501
        mov    edx, 0                        ; -> obj3:0x001C
        lea    ecx, [ebp - 0xa4]
        call   sub_2_021B
        lea    edx, [ebp - 0xfc]
        lea    ecx, [ebp - 0xa4]
        call   sub_2_01D6
        lea    eax, [ebp - 0xa8]
        push   eax
        lea    eax, [ebp - 0xa4]
        push   eax
        push   0x80000002
        call   sub_2_1C9C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0501
        mov    dword ptr [ebp - 0x10c], 4
        mov    dword ptr [ebp - 0x108], 0
        lea    eax, [ebp - 0x10c]
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        push   0
        push   0
        push   0                             ; -> obj3:0x0048
        mov    eax, dword ptr [ebp - 0xa8]
        push   eax
        call   sub_2_1C8C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_04F2
        cmp    dword ptr [ebp - 0x108], 0
        je     loc_2_04C4
        mov    dword ptr [ebp - 0x110], 0x1a
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        push   0                             ; -> obj3:0x0058
        push   1
        push   0
        push   0                             ; -> obj3:0x0074
        mov    eax, dword ptr [ebp - 0xac]
        push   eax
        call   sub_2_1CB4
        add    esp, 0x18
        jmp    loc_2_04F2
loc_2_04C4:
        mov    dword ptr [ebp - 0x110], 0x1a
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        push   0                             ; -> obj3:0x0084
        push   1
        push   0
        push   0                             ; -> obj3:0x00A0
        mov    eax, dword ptr [ebp - 0xac]
        push   eax
        call   sub_2_1CB4
        add    esp, 0x18
loc_2_04F2:
        mov    eax, dword ptr [ebp - 0xa8]
        push   eax
        call   sub_2_1C94
        add    esp, 4
loc_2_0501:
        mov    eax, dword ptr [ebp - 0xac]
        push   eax
        call   sub_2_1C94
        add    esp, 4
loc_2_0510:
        jmp    loc_2_0374
loc_2_0515:
        mov    eax, dword ptr [ebp - 0x54]
        push   eax
        call   sub_2_1C94
        add    esp, 4
loc_2_0521:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
loc_2_0530:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_2_0552
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_0C68
        jmp    loc_2_055C
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_2_0552:
        mov    eax, 1
        jmp    loc_2_055C
loc_2_055C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_2_0570:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], 0
        push   1
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   3
        push   0                             ; -> obj3:0x013C
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1CC4
        add    esp, 0x1c
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_05D5
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 0
        je     loc_2_05C6
        jmp    loc_2_05C6
loc_2_05C6:
        mov    eax, 1
        jmp    loc_2_05DC
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_05D5:
        xor    eax, eax
        jmp    loc_2_05DC
loc_2_05DC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_05E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0x28
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        push   1
        push   0                             ; -> obj3:0x014C
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_1CC4
        add    esp, 0x1c
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0622
        jmp    loc_2_06B8
loc_2_0622:
        cmp    dword ptr [ebp - 8], 6
        jne    loc_2_0650
        lea    ecx, [ebp - 0x30]
        call   sub_2_016F
        push   6
        mov    edx, 0                        ; -> obj3:0x0154
        lea    ecx, [ebp - 0x30]
        call   sub_2_010A
        mov    dword ptr [ebp - 0x34], eax
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_2_0655
loc_2_0650:
        jmp    loc_2_06B8
loc_2_0655:
        mov    dword ptr [ebp - 8], 0x28
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        push   1
        push   0                             ; -> obj3:0x015C
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1CC4
        add    esp, 0x1c
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_068D
        jmp    loc_2_06B8
loc_2_068D:
        push   1
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        push   1
        push   0                             ; -> obj3:0x0168
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
        mov    dword ptr [ebp - 4], eax
loc_2_06B8:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_06D1
        mov    eax, 1
        jmp    loc_2_06D8
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_06D1:
        xor    eax, eax
        jmp    loc_2_06D8
loc_2_06D8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_06DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_2_0570
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_084A
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        jne    loc_2_07C9
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_2_07AB
loc_2_071A:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_0731
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> obj3:0x0134
        jmp    loc_2_0739
loc_2_0731:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> obj3:0x0138
loc_2_0739:
        xor    eax, eax
        jmp    loc_2_0969
loc_2_0740:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        je     loc_2_075A
        cmp    dword ptr [0], 0              ; -> obj1:0x0374
        jne    loc_2_0764
loc_2_075A:
        mov    eax, 1
        jmp    loc_2_0969
loc_2_0764:
        mov    eax, dword ptr [0]            ; -> obj3:0x0134
        cmp    dword ptr [ebp + 0x10], eax
        jne    loc_2_0784
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_096E
        jmp    loc_2_079F
loc_2_0784:
        mov    eax, dword ptr [0]            ; -> obj3:0x0138
        cmp    dword ptr [ebp + 0x10], eax
        jne    loc_2_079F
        mov    edx, 2
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_096E
loc_2_079F:
        xor    eax, eax
        jmp    loc_2_0969
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_07AB:
        cmp    dword ptr [ebp - 0x18], 1
        je     loc_2_071A
        cmp    dword ptr [ebp - 0x18], 5
        je     loc_2_0740
        jmp    loc_2_07C4
loc_2_07C4:
        jmp    loc_2_0845
loc_2_07C9:
        mov    eax, dword ptr [0]            ; -> obj3:0x0134
        cmp    dword ptr [ebp + 0x10], eax
        jne    loc_2_0845
        push   0
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_2_1CDC
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0800
        mov    eax, 0x13
        jmp    loc_2_0969
loc_2_0800:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 0x10], eax
        je     loc_2_0816
        mov    eax, 1
        jmp    loc_2_0969
loc_2_0816:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_2_0836
loc_2_0821:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_2_05E1
        add    esp, 8
        jmp    loc_2_0845
loc_2_0836:
        cmp    dword ptr [ebp - 0x1c], 1
        je     loc_2_0821
        jmp    loc_2_0845
loc_2_0845:
        jmp    loc_2_095F
loc_2_084A:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        jne    loc_2_095F
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_2_0937
loc_2_0861:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> obj3:0x0130
        xor    eax, eax
        jmp    loc_2_0969
loc_2_0870:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xfffffff
        cmp    eax, 7
        jne    loc_2_089B
        call   IRLAMP_Service_16
loc_2_0886:
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    word ptr [eax], 0
        mov    dword ptr [0], 0              ; -> obj1:0x037C
loc_2_089B:
        cmp    dword ptr [0], 0              ; -> obj3:0x0128
        jne    loc_2_08C3
        mov    dword ptr [0], 1              ; -> obj3:0x0128
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_2_0AD6
        add    esp, 4
        jmp    loc_2_08FD
loc_2_08C3:
        cmp    dword ptr [0], 0              ; -> obj3:0x0134
        je     loc_2_08E0
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0134
        push   eax
        call   sub_2_1CAC
        add    esp, 8
loc_2_08E0:
        cmp    dword ptr [0], 0              ; -> obj3:0x0138
        je     loc_2_08FD
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0138
        push   eax
        call   sub_2_1CAC
        add    esp, 8
loc_2_08FD:
        cmp    dword ptr [0], 0              ; -> loc_1_07F0
        je     loc_2_0917
        cmp    dword ptr [0], 0              ; -> obj1:0x0374
        jne    loc_2_0921
loc_2_0917:
        mov    eax, 1
        jmp    loc_2_0969
loc_2_0921:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_096E
        xor    eax, eax
        jmp    loc_2_0969
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_0937:
        cmp    dword ptr [ebp - 0x20], 1
        je     loc_2_0861
        cmp    dword ptr [ebp - 0x20], 5
        je     loc_2_089B
        cmp    dword ptr [ebp - 0x20], 8
        je     loc_2_0870
        jmp    loc_2_095A
loc_2_095A:
        jmp    loc_2_095F
loc_2_095F:
        mov    eax, 1
        jmp    loc_2_0969
loc_2_0969:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_096E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x20], edx
        mov    dword ptr [ebp - 0x1c], ecx
        mov    dword ptr [ebp - 0x18], 0     ; -> obj3:0x0278
        mov    dword ptr [ebp - 0x10], 0
        call   sub_2_19CB
        mov    dword ptr [ebp - 8], eax
        push   0
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_2_1CEC
        add    esp, 8
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_2_09B7
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_2_0AD6
        add    esp, 4
loc_2_09B7:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_09C6
        jmp    loc_2_0AA8
loc_2_09C6:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_09DC
loc_2_09D2:
        inc    dword ptr [ebp - 4]
        add    dword ptr [ebp - 0x18], 0xc0
loc_2_09DC:
        cmp    dword ptr [ebp - 4], 8
        jae    loc_2_0AA8
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [eax], 0
        je     loc_2_0AA8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1AFB
        add    esp, 4
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [eax + 0x6c], 0
        je     loc_2_0A9B
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffffff66
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_2_0A39
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_2_0A75
loc_2_0A39:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_2_0A57
        test   byte ptr [ebp - 0xc], 0x10
        je     loc_2_0A57
        test   byte ptr [ebp - 0x20], 1
        jne    loc_2_0A75
loc_2_0A57:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_2_0A96
        test   byte ptr [ebp - 0xc], 8
        je     loc_2_0A96
        test   byte ptr [ebp - 0x20], 2
        je     loc_2_0A96
loc_2_0A75:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_2_0CC5
        add    esp, 8
loc_2_0A96:
        jmp    loc_2_0AA0
loc_2_0A9B:
        jmp    loc_2_09D2
loc_2_0AA0:
        inc    dword ptr [ebp - 0x10]
        jmp    loc_2_09D2
loc_2_0AA8:
        push   0
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_2_1CE4
        add    esp, 8
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_0ACA
        mov    eax, 1
        jmp    loc_2_0ACC
loc_2_0ACA:
        xor    eax, eax
loc_2_0ACC:
        jmp    loc_2_0AD1
loc_2_0AD1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0AD6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x110
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0AF1
loc_2_0AEE:
        inc    dword ptr [ebp - 8]
loc_2_0AF1:
        cmp    dword ptr [ebp - 8], 1
        jg     loc_2_0C59
        mov    edx, 0                        ; -> obj3:0x0174
        lea    ecx, [ebp - 0x10c]
        call   sub_2_021B
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0B2A
        mov    edx, 0                        ; -> obj3:0x0180
        lea    ecx, [ebp - 0x10c]
        call   sub_2_01D6
        jmp    loc_2_0B3A
loc_2_0B2A:
        mov    edx, 0                        ; -> obj3:0x0184
        lea    ecx, [ebp - 0x10c]
        call   sub_2_01D6
loc_2_0B3A:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0x10c]
        push   eax
        lea    eax, [ebp - 0x110]
        push   eax
        call   sub_2_1CF4
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0B68
        jmp    loc_2_0AEE
loc_2_0B68:
        push   0
        push   5
        push   0                             ; -> obj3:0x0188
        push   1
        push   0                             ; -> obj3:0x0190
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0BF6
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0110
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        mov    eax, dword ptr [0]            ; -> obj3:0x0110
        push   eax
        push   1
        push   0                             ; -> obj3:0x0198
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x00DC
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        mov    eax, dword ptr [0]            ; -> obj3:0x00DC
        push   eax
        push   1
        push   0                             ; -> obj3:0x01A4
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
        jmp    loc_2_0C54
loc_2_0BF6:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0124
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        mov    eax, dword ptr [0]            ; -> obj3:0x0124
        push   eax
        push   1
        push   0                             ; -> obj3:0x01B4
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x00FC
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        mov    eax, dword ptr [0]            ; -> obj3:0x00FC
        push   eax
        push   1
        push   0                             ; -> obj3:0x01C0
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
loc_2_0C54:
        jmp    loc_2_0AEE
loc_2_0C59:
        mov    eax, 1
        jmp    loc_2_0C63
loc_2_0C63:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0C68:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], ecx
        push   1
        push   0                             ; -> loc_2_06DD
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1D04
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0C99
        jmp    loc_2_0CB8
loc_2_0C99:
        push   2
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1CFC
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0CB8
loc_2_0CB8:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_0CC0
loc_2_0CC0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_0CC5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x120
        push   ebx
        push   esi
        push   edi
        lea    ecx, [ebp - 0x108]
        mov    edx, dword ptr [0]            ; -> obj3:0x00BC
        call   sub_2_021B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x70]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_0D0B
        mov    edx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 0x108]
        add    edx, 0x70
        call   sub_2_01D6
        jmp    loc_2_0D1B
loc_2_0D0B:
        mov    edx, 0                        ; -> obj3:0x01D0
        lea    ecx, [ebp - 0x108]
        call   sub_2_01D6
loc_2_0D1B:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        lea    eax, [ebp - 0x110]
        push   eax
        call   sub_2_1CF4
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        jne    loc_2_0D4C
        jmp    loc_2_0FF7
loc_2_0D4C:
        cmp    dword ptr [ebp - 8], 0x10
        jne    loc_2_0D5B
        jmp    loc_2_0FF7
loc_2_0D5B:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_0D6A
        jmp    loc_2_0FF7
loc_2_0D6A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xc], 0
        je     loc_2_0D9F
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x10
        push   eax
        push   1
        push   0                             ; -> obj3:0x01DC
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
loc_2_0D9F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x6c], 0
        je     loc_2_0DD4
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x6c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x70
        push   eax
        push   1
        push   0                             ; -> obj3:0x01E8
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
loc_2_0DD4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x88], 0
        je     loc_2_0E50
        push   8
        mov    edx, 0                        ; -> obj3:0x01F4
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x8c
        call   sub_2_010A
        test   eax, eax
        je     loc_2_0E23
        push   9
        mov    edx, 0                        ; -> obj3:0x01FC
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x8c
        call   sub_2_02F3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x88], 8
loc_2_0E23:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x88]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        push   1
        push   0                             ; -> obj3:0x0208
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
loc_2_0E50:
        test   byte ptr [ebp + 0xc], 2
        je     loc_2_0FA8
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 4], 8
        je     loc_2_0FA8
        mov    edx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 0x108]
        add    edx, 0xa0
        call   sub_2_021B
        push   0x1c
        mov    edx, 0                        ; -> obj3:0x0218
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x10
        call   sub_2_010A
        test   eax, eax
        je     loc_2_0EB1
        push   0xf
        mov    edx, 0                        ; -> obj3:0x0234
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x10
        call   sub_2_02F3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc], 0xe
loc_2_0EB1:
        mov    edx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 0x108]
        add    edx, 0x10
        call   sub_2_01D6
        mov    dword ptr [ebp - 0x11c], 0
        push   0
        lea    eax, [ebp - 0x11c]
        push   eax
        lea    ecx, [ebp - 0x108]
        call   0
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        call   sub_2_1D14
        add    esp, 0x10
        test   eax, eax
        je     loc_2_0F02
        mov    dword ptr [ebp - 0x11c], 0xdead
loc_2_0F02:
        mov    eax, dword ptr [ebp - 0x11c]
        push   eax
        push   0                             ; -> obj3:0x0244
        lea    eax, [ebp - 0x118]
        push   eax
        call   sub_2_1D0C
        add    esp, 0xc
        lea    ecx, [ebp - 0x108]
        call   0
        mov    dword ptr [ebp - 0x120], eax
        cmp    dword ptr [ebp - 0x120], 0x14
        jbe    loc_2_0F45
        mov    dword ptr [ebp - 0x120], 0x14
loc_2_0F45:
        mov    eax, dword ptr [ebp - 0x120]
        mov    byte ptr [ebp + eax - 0x108], 0
        mov    eax, dword ptr [ebp - 0x120]
        push   eax
        push   0x5f
        lea    ecx, [ebp - 0x108]
        mov    edx, 0x20
        call   sub_2_0254
        lea    edx, [ebp - 0x118]
        lea    ecx, [ebp - 0x108]
        call   sub_2_01D6
        push   0
        mov    eax, dword ptr [ebp - 0x120]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        push   1
        push   0                             ; -> obj3:0x0248
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
loc_2_0FA8:
        mov    dword ptr [ebp - 4], 0x100
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_2_1B47
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0FF7
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        push   3
        push   0                             ; -> obj3:0x0258
        push   0
        mov    eax, dword ptr [ebp - 0x110]
        push   eax
        call   sub_2_1CCC
        add    esp, 0x1c
loc_2_0FF7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xfffffff8
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        dec    eax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        test   ecx, eax
        je     loc_2_1037
        jmp    loc_2_10CA
loc_2_1037:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   al, 8
        je     loc_2_1068
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 8
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        mov    edx, 0                        ; -> obj3:0x0264
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_2_02F3
        jmp    loc_2_10CA
loc_2_1068:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   al, 0x10
        je     loc_2_1099
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 6
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        mov    edx, 0                        ; -> obj3:0x026C
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_2_02F3
        jmp    loc_2_10CA
loc_2_1099:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   al, 0x40
        je     loc_2_10CA
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 4
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        mov    edx, 0                        ; -> obj3:0x0274
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_2_02F3
        jmp    loc_2_10CA
loc_2_10CA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh
loc_2_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, 0                        ; -> obj3:0x0920
        add    eax, 4
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        add    eax, 4
        push   eax
        mov    ecx, 0                        ; -> obj3:0x0920
        mov    edx, dword ptr [ebp + 0xc]
        call   sub_2_02F3
        mov    dword ptr [0], 0              ; -> obj3:0x0B24
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1115:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, 0                        ; -> obj3:0x0920
        add    eax, 4
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x10], 0
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> obj3:0x0920
        test   eax, eax
        jne    loc_2_1148
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_2_1707
loc_2_1148:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_115B
loc_2_1154:
        inc    dword ptr [ebp - 8]
        add    dword ptr [ebp - 0xc], 0x29
loc_2_115B:
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> obj3:0x089C
        cmp    eax, dword ptr [ebp - 8]
        jle    loc_2_16FF
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> obj3:0x0920
        cmp    eax, dword ptr [ebp - 8]
        jle    loc_2_16FF
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   al, 4
        je     loc_2_11A5
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
        jmp    loc_2_1154
loc_2_11A5:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   al, 1
        je     loc_2_16E9
        mov    dword ptr [ebp - 0x14], 0
loc_2_11BF:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 1    ; -> obj3:0x0278
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 1
        and    eax, 0xffff
        mov    dword ptr [0x18], eax         ; -> obj3:0x0B00
        mov    dword ptr [0x1c], 0           ; -> obj3:0x0B04
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [0x20], eax         ; -> obj3:0x0B08
        mov    dword ptr [0x24], 0           ; -> obj3:0x0B0C
        mov    dword ptr [0x28], 0xafc8      ; -> obj3:0x0B10
        mov    byte ptr [0x2c], 4            ; -> obj3:0x0B14
        mov    byte ptr [0], 3               ; -> obj3:0x0B28
        mov    dword ptr [1], 0              ; -> obj3:0x0B29
        mov    byte ptr [5], 8               ; -> obj3:0x0B2D
        mov    dword ptr [6], 0              ; -> obj3:0x0B2E
        mov    dword ptr [0x2d], 0           ; -> obj3:0x0B15
        mov    word ptr [0x33], 0x10a        ; -> obj3:0x0B1B
        mov    dword ptr [0x35], 0           ; -> obj3:0x0B1D
        mov    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        push   0                             ; -> obj3:0x0AE8
        call   IRLAMP_Service_24
loc_2_1264:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_2_12FB
loc_2_1274:
        cmp    dword ptr [0], 0              ; -> obj3:0x0B24
        je     loc_2_12A8
        cmp    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        je     loc_2_1296
        jmp    loc_2_12A8
loc_2_1296:
        push   0xafc8
        call   sub_2_1C2F
        add    esp, 4
        jmp    loc_2_1274
loc_2_12A8:
        cmp    dword ptr [0], 0xffe          ; -> obj3:0x0B24
        jne    loc_2_12D3
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_2_12D3
        mov    dword ptr [ebp - 0x14], 1
        jmp    loc_2_11BF
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_12D3:
        cmp    dword ptr [0], 0              ; -> obj3:0x0B24
        je     loc_2_12F6
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
        jmp    loc_2_1154
loc_2_12F6:
        jmp    loc_2_1311
loc_2_12FB:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
        jmp    loc_2_1154
loc_2_1311:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 2
        and    eax, 0xffff
        mov    dword ptr [0x18], eax         ; -> obj3:0x0B00
        mov    dword ptr [0x1c], 0           ; -> obj3:0x0B04
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [0x20], eax         ; -> obj3:0x0B08
        mov    dword ptr [0x24], 0           ; -> obj3:0x0B0C
        mov    dword ptr [0x28], 0xafc8      ; -> obj3:0x0B10
        mov    byte ptr [0x2c], 4            ; -> obj3:0x0B14
        mov    byte ptr [0], 3               ; -> obj3:0x0B28
        mov    dword ptr [1], 0              ; -> obj3:0x0B29
        mov    byte ptr [5], 4               ; -> obj3:0x0B2D
        mov    dword ptr [6], 0              ; -> obj3:0x0B2E
        mov    dword ptr [0x2d], 0           ; -> obj3:0x0B15
        mov    word ptr [0x33], 0x10a        ; -> obj3:0x0B1B
        mov    dword ptr [0x35], 0           ; -> obj3:0x0B1D
        mov    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        push   0                             ; -> obj3:0x0AE8
        call   IRLAMP_Service_24
loc_2_13A5:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_2_13EE
loc_2_13B5:
        cmp    dword ptr [0], 0              ; -> obj3:0x0B24
        je     loc_2_13E9
        cmp    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        je     loc_2_13D7
        jmp    loc_2_13E9
loc_2_13D7:
        push   0xafc8
        call   sub_2_1C2F
        add    esp, 4
        jmp    loc_2_13B5
loc_2_13E9:
        jmp    loc_2_1404
loc_2_13EE:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
        jmp    loc_2_1154
loc_2_1404:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 4
        and    eax, 0xffff
        mov    dword ptr [0x18], eax         ; -> obj3:0x0B00
        mov    dword ptr [0x1c], 0           ; -> obj3:0x0B04
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [0x20], eax         ; -> obj3:0x0B08
        mov    dword ptr [0x24], 0           ; -> obj3:0x0B0C
        mov    dword ptr [0x28], 0xafc8      ; -> obj3:0x0B10
        mov    byte ptr [0x2c], 4            ; -> obj3:0x0B14
        mov    byte ptr [0], 3               ; -> obj3:0x0B28
        mov    dword ptr [1], 0              ; -> obj3:0x0B29
        mov    byte ptr [5], 7               ; -> obj3:0x0B2D
        mov    dword ptr [6], 0              ; -> obj3:0x0B2E
        mov    dword ptr [0x2d], 0           ; -> obj3:0x0B15
        mov    word ptr [0x33], 0x10a        ; -> obj3:0x0B1B
        mov    dword ptr [0x35], 0           ; -> obj3:0x0B1D
        mov    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        push   0                             ; -> obj3:0x0AE8
        call   IRLAMP_Service_24
loc_2_1498:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_2_14E1
loc_2_14A8:
        cmp    dword ptr [0], 0              ; -> obj3:0x0B24
        je     loc_2_14DC
        cmp    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        je     loc_2_14CA
        jmp    loc_2_14DC
loc_2_14CA:
        push   0xafc8
        call   sub_2_1C2F
        add    esp, 4
        jmp    loc_2_14A8
loc_2_14DC:
        jmp    loc_2_14F7
loc_2_14E1:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
        jmp    loc_2_1154
loc_2_14F7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 5
        and    eax, 0xffff
        mov    dword ptr [0x18], eax         ; -> obj3:0x0B00
        mov    dword ptr [0x1c], 0           ; -> obj3:0x0B04
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [0x20], eax         ; -> obj3:0x0B08
        mov    dword ptr [0x24], 0           ; -> obj3:0x0B0C
        mov    dword ptr [0x28], 0xafc8      ; -> obj3:0x0B10
        mov    byte ptr [0x2c], 4            ; -> obj3:0x0B14
        mov    byte ptr [0], 3               ; -> obj3:0x0B28
        mov    dword ptr [1], 0              ; -> obj3:0x0B29
        mov    byte ptr [5], 7               ; -> obj3:0x0B2D
        mov    dword ptr [6], 0              ; -> obj3:0x0B2E
        mov    dword ptr [0x2d], 0           ; -> obj3:0x0B15
        mov    word ptr [0x33], 0x10a        ; -> obj3:0x0B1B
        mov    dword ptr [0x35], 0           ; -> obj3:0x0B1D
        mov    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        push   0                             ; -> obj3:0x0AE8
        call   IRLAMP_Service_24
loc_2_158B:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_2_15D4
loc_2_159B:
        cmp    dword ptr [0], 0              ; -> obj3:0x0B24
        je     loc_2_15CF
        cmp    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        je     loc_2_15BD
        jmp    loc_2_15CF
loc_2_15BD:
        push   0xafc8
        call   sub_2_1C2F
        add    esp, 4
        jmp    loc_2_159B
loc_2_15CF:
        jmp    loc_2_15EA
loc_2_15D4:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
        jmp    loc_2_1154
loc_2_15EA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 6
        and    eax, 0xffff
        mov    dword ptr [0x18], eax         ; -> obj3:0x0B00
        mov    dword ptr [0x1c], 0           ; -> obj3:0x0B04
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [0x20], eax         ; -> obj3:0x0B08
        mov    dword ptr [0x24], 0           ; -> obj3:0x0B0C
        mov    dword ptr [0x28], 0xafc8      ; -> obj3:0x0B10
        mov    byte ptr [0x2c], 4            ; -> obj3:0x0B14
        mov    byte ptr [0], 3               ; -> obj3:0x0B28
        mov    dword ptr [1], 0              ; -> obj3:0x0B29
        mov    byte ptr [5], 0xc             ; -> obj3:0x0B2D
        mov    dword ptr [6], 0              ; -> obj3:0x0B2E
        mov    dword ptr [0x2d], 0           ; -> obj3:0x0B15
        mov    word ptr [0x33], 0x10a        ; -> obj3:0x0B1B
        mov    dword ptr [0x35], 0           ; -> obj3:0x0B1D
        mov    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        push   0                             ; -> obj3:0x0AE8
        call   IRLAMP_Service_24
loc_2_167E:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_2_16C7
loc_2_168E:
        cmp    dword ptr [0], 0              ; -> obj3:0x0B24
        je     loc_2_16C2
        cmp    dword ptr [0], 0xfff          ; -> obj3:0x0B24
        je     loc_2_16B0
        jmp    loc_2_16C2
loc_2_16B0:
        push   0xafc8
        call   sub_2_1C2F
        add    esp, 4
        jmp    loc_2_168E
loc_2_16C2:
        jmp    loc_2_16DD
loc_2_16C7:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
        jmp    loc_2_1154
loc_2_16DD:
        mov    dword ptr [ebp - 0x10], 1
        jmp    loc_2_16FA
loc_2_16E9:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        mov    dword ptr [eax + eax*2], 0    ; -> obj3:0x0278
loc_2_16FA:
        jmp    loc_2_1154
loc_2_16FF:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_2_1707
loc_2_1707:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_170C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0], 0              ; -> obj3:0x0B24
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_1721:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_1742
loc_2_1735:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> obj3:0x0B24
        jmp    loc_2_17AB
loc_2_1742:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0x2e
        ja     loc_2_17AB
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj2:0x177C
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj2:0x1764
        ; ---- dispatch table (6 entries) ----
        dd  loc_2_1735
        dd  loc_2_1735
        dd  loc_2_1735
        dd  loc_2_1735
        dd  loc_2_1735
        dd  loc_2_17AB
        db  000h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 001h, 005h
        db  002h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 003h, 005h, 005h, 005h, 005h, 005h
        db  005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 004h, 004h
loc_2_17AB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_17B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_2_17BB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x18]
        and    eax, 0xffff
        shr    eax, 8
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x18]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        je     loc_2_1814
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj3:0x0B24
        cmp    dword ptr [ebp - 4], 1
        jne    loc_2_180F
        mov    dword ptr [0], 0xffe          ; -> obj3:0x0B24
loc_2_180F:
        jmp    loc_2_19C6
loc_2_1814:
        xor    eax, eax
        mov    al, byte ptr [8]              ; -> obj3:0x0B40
        mov    dword ptr [ebp - 8], eax
        mov    eax, 0                        ; -> obj3:0x0B38
        add    eax, 9
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_2_198D
loc_2_1834:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 0x10
        jl     loc_2_1845
        mov    eax, 0x10
loc_2_1845:
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 0x10]
        lea    ecx, [eax + eax*2 + 0x70]     ; -> obj3:0x02E8
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 6
        mov    dword ptr [ecx + ecx*2 + 0x6c], eax ; -> obj3:0x02E4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 6
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [eax + ecx + 0x71], 0 ; -> obj3:0x02E9
        jmp    loc_2_19BC
loc_2_1884:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 0x5c
        jl     loc_2_1895
        mov    eax, 0x5c
loc_2_1895:
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 0x10]
        lea    ecx, [eax + eax*2 + 0x10]     ; -> obj3:0x0288
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 6
        mov    dword ptr [ecx + ecx*2 + 0xc], eax ; -> obj3:0x0284
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 6
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [eax + ecx + 0x11], 0 ; -> obj3:0x0289
        jmp    loc_2_19BC
loc_2_18D4:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 4
        jl     loc_2_18E5
        mov    eax, 4
loc_2_18E5:
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 0x10]
        lea    ecx, [eax + eax*2 + 0x84]     ; -> obj3:0x02FC
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 6
        mov    dword ptr [ecx + ecx*2 + 0x80], eax ; -> obj3:0x02F8
        jmp    loc_2_19BC
loc_2_1910:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 0x10
        jl     loc_2_1921
        mov    eax, 0x10
loc_2_1921:
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 0x10]
        lea    ecx, [eax + eax*2 + 0x8c]     ; -> obj3:0x0304
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 6
        mov    dword ptr [ecx + ecx*2 + 0x88], eax ; -> obj3:0x0300
        jmp    loc_2_19BC
loc_2_194C:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 0x20
        jl     loc_2_195D
        mov    eax, 0x20
loc_2_195D:
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 0x10]
        lea    ecx, [eax + eax*2 + 0xa0]     ; -> obj3:0x0318
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 6
        mov    dword ptr [ecx + ecx*2 + 0x9c], eax ; -> obj3:0x0314
        jmp    loc_2_19BC
        db  0E9h, 02Fh, 000h, 000h, 000h
loc_2_198D:
        dec    dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x14], 5
        ja     loc_2_19BC
        mov    eax, dword ptr [ebp - 0x14]
        jmp    dword ptr [eax*4]             ; -> obj2:0x19A4
        ; ---- dispatch table (6 entries) ----
        dd  loc_2_1834
        dd  loc_2_1884
        dd  loc_2_19BC
        dd  loc_2_18D4
        dd  loc_2_1910
        dd  loc_2_194C
loc_2_19BC:
        mov    dword ptr [0], 0              ; -> obj3:0x0B24
loc_2_19C6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_19CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        call   sub_2_1BB5
        mov    eax, dword ptr [0]            ; -> obj3:0x091C
        push   eax
        call   IRLAMP_Service_28
loc_2_19F2:
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_1A0A
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_2_1AF6
loc_2_1A0A:
        mov    eax, dword ptr [0]            ; -> obj3:0x0A9C
        push   eax
        mov    eax, dword ptr [0]            ; -> obj3:0x091C
        push   eax
        call   IRLAMP_Service_29
loc_2_1A1B:
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_1A36
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_1AD7
loc_2_1A36:
        push   0                             ; -> obj3:0x0C8C
        push   0
        push   0                             ; -> obj3:0x0878
        mov    eax, dword ptr [0]            ; -> obj3:0x091C
        push   eax
        push   0xffff
        call   IRLAMP_Service_04
loc_2_1A52:
        add    esp, 0x14
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_1A6D
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_1AC6
loc_2_1A6D:
        call   IRLAMP_Service_16
loc_2_1A72:
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_1A8B
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1AC6
loc_2_1A8B:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        add    eax, 4
        push   eax
        mov    ecx, 0                        ; -> obj3:0x0920
        mov    edx, dword ptr [ebp - 0x10]
        call   sub_2_02F3
        call   sub_2_1115
        test   eax, eax
        jne    loc_2_1AC6
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1AC6
loc_2_1AC6:
        mov    eax, dword ptr [0]            ; -> obj3:0x091C
        push   eax
        call   IRLAMP_Service_05
loc_2_1AD1:
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
loc_2_1AD7:
        mov    eax, dword ptr [0]            ; -> obj3:0x0A9C
        push   eax
        mov    eax, dword ptr [0]            ; -> obj3:0x091C
        push   eax
        call   IRLAMP_Service_30
loc_2_1AE8:
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_1AF6
loc_2_1AF6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1AFB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> obj3:0x0920
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_2_1B1C
        xor    eax, eax
        jmp    loc_2_1B42
loc_2_1B1C:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, 0                        ; -> obj3:0x0920
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        jmp    loc_2_1B42
loc_2_1B42:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1B47:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> obj3:0x0920
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_2_1B66
        jmp    loc_2_1BB0
loc_2_1B66:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax], 0x29
        jae    loc_2_1B80
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
        jmp    loc_2_1BB0
loc_2_1B80:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, 0                        ; -> obj3:0x0920
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0x29
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_2_02F3
loc_2_1BB0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1BB5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0], 0x20           ; -> obj3:0x0878
        mov    dword ptr [4], 0              ; -> obj3:0x087C
        mov    dword ptr [8], 0              ; -> obj3:0x0880
        mov    dword ptr [0xc], 0            ; -> obj3:0x0884
        mov    dword ptr [0x10], 0           ; -> obj3:0x0888
        mov    dword ptr [0x14], 0           ; -> obj3:0x088C
        mov    dword ptr [0x18], 0           ; -> obj3:0x0890
        mov    dword ptr [0x1c], 0           ; -> obj3:0x0894
        push   0x71
        mov    ecx, 0                        ; -> obj3:0x08A8
        xor    edx, edx
        call   sub_2_02BA
        push   0x600
        mov    ecx, 0                        ; -> obj3:0x0278
        xor    edx, edx
        call   sub_2_02BA
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1C2F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        call   sub_1_0270
loc_2_1C3D:
        add    eax, dword ptr [ebp + 8]
        adc    edx, 0
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 8], edx
loc_2_1C49:
        call   sub_1_0270
loc_2_1C4E:
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        ja     loc_2_1C78
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jne    loc_2_1C7D
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0xc], eax
        jb     loc_2_1C7D
loc_2_1C78:
        jmp    loc_2_1C87
loc_2_1C7D:
        call   sub_1_22A4
loc_2_1C82:
        jmp    loc_2_1C49
loc_2_1C87:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_2_1C8C:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_2_1C94:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_2_1C9C:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_2_1CA4:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf800100
        int3   
        int3   
sub_2_1CAC:
        VxDCall  CONFIGMG._CONFIGMG_Reenumerate_DevNode ; cd2033803300
        int3   
        int3   
sub_2_1CB4:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_2_1CBC:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        int3   
        int3   
sub_2_1CC4:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_1CCC:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_2_1CD4:
        VMMCall  VMM._lstrlen                    ; cd207c810100
        int3   
        int3   
sub_2_1CDC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        int3   
        int3   
sub_2_1CE4:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        int3   
        int3   
sub_2_1CEC:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        int3   
        int3   
sub_2_1CF4:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        int3   
        int3   
sub_2_1CFC:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        int3   
        int3   
sub_2_1D04:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        int3   
        int3   
sub_2_1D0C:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_2_1D14:
        VxDCall  CONFIGMG._CONFIGMG_Get_CRC_CheckSum ; cd2031803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0xc93  RW-
        db  045h, 06Eh, 075h, 06Dh, 05Ch, 052h, 06Fh, 06Fh, 074h, 05Ch, 049h, 06Eh, 066h, 072h, 061h, 072h
        db  065h, 064h, 000h, 000h, 044h, 072h, 069h, 076h, 065h, 072h, 000h, 000h, 053h, 079h, 073h, 074h
        db  065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh
        db  06Ch, 053h, 065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h, 073h, 05Ch, 043h, 06Ch
        db  061h, 073h, 073h, 05Ch, 000h, 000h, 000h, 000h, 050h, 06Fh, 072h, 074h, 053h, 075h, 062h, 043h
        db  06Ch, 061h, 073h, 073h, 000h, 000h, 000h, 000h, 056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h
        db  049h, 06Eh, 066h, 072h, 061h, 072h, 065h, 064h, 020h, 043h, 04Fh, 04Dh, 020h, 050h, 06Fh, 072h
        db  074h, 000h, 000h, 000h, 046h, 052h, 049h, 045h, 04Eh, 044h, 04Ch, 059h, 04Eh, 041h, 04Dh, 045h
        db  000h, 000h, 000h, 000h, 056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 049h, 06Eh, 066h, 072h
        db  061h, 072h, 065h, 064h, 020h, 050h, 052h, 04Eh, 020h, 050h, 06Fh, 072h, 074h, 000h, 000h, 000h
        db  046h, 052h, 049h, 045h, 04Eh, 044h, 04Ch, 059h, 04Eh, 041h, 04Dh, 045h, 000h, 000h, 000h, 000h
        db  049h, 06Eh, 066h, 072h, 061h, 072h, 065h, 064h, 05Ch, 000h, 000h, 000h
        dd  obj3:0x00B0              ; fixup
        db  056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 049h, 06Eh, 066h, 072h, 061h, 072h, 065h, 064h
        db  020h, 043h, 04Fh, 04Dh, 020h, 070h, 06Fh, 072h, 074h, 000h, 000h, 000h
        dd  obj3:0x00C0              ; fixup
        db  056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 049h, 06Eh, 066h, 072h, 061h, 072h, 065h, 064h
        db  020h, 04Ch, 050h, 054h, 020h, 070h, 06Fh, 072h, 074h, 000h, 000h, 000h
        dd  obj3:0x00E0              ; fixup
        db  049h, 06Eh, 066h, 072h, 061h, 072h, 065h, 064h, 05Ch, 043h, 04Fh, 04Dh, 000h, 000h, 000h, 000h
        dd  obj3:0x0100              ; fixup
        db  049h, 06Eh, 066h, 072h, 061h, 072h, 065h, 064h, 05Ch, 04Ch, 050h, 054h, 000h, 000h, 000h, 000h
        dd  obj3:0x0114              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 050h, 06Fh, 072h, 074h, 053h, 075h, 062h, 043h, 06Ch, 061h, 073h, 073h
        db  000h, 000h, 000h, 000h, 043h, 06Ch, 061h, 073h, 073h, 000h, 000h, 000h, 04Dh, 04Fh, 044h, 045h
        db  04Dh, 000h, 000h, 000h, 050h, 06Fh, 072h, 074h, 04Eh, 061h, 06Dh, 065h, 000h, 000h, 000h, 000h
        db  041h, 074h, 074h, 061h, 063h, 068h, 065h, 064h, 054h, 06Fh, 000h, 000h, 049h, 06Eh, 066h, 072h
        db  061h, 072h, 065h, 064h, 05Ch, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 000h, 04Ch, 050h, 054h, 000h
        db  050h, 06Fh, 072h, 074h, 073h, 000h, 000h, 000h, 043h, 06Ch, 061h, 073h, 073h, 000h, 000h, 000h
        db  048h, 061h, 072h, 064h, 077h, 061h, 072h, 065h, 049h, 044h, 000h, 000h, 046h, 072h, 069h, 065h
        db  06Eh, 064h, 06Ch, 079h, 04Eh, 061h, 06Dh, 065h, 000h, 000h, 000h, 000h, 048h, 061h, 072h, 064h
        db  077h, 061h, 072h, 065h, 049h, 044h, 000h, 000h, 046h, 072h, 069h, 065h, 06Eh, 064h, 06Ch, 079h
        db  04Eh, 061h, 06Dh, 065h, 000h, 000h, 000h, 000h, 050h, 04Eh, 050h, 05Fh, 04Eh, 055h, 04Ch, 04Ch
        db  000h, 000h, 000h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h
        db  048h, 061h, 072h, 064h, 077h, 061h, 072h, 065h, 049h, 044h, 000h, 000h, 070h, 06Eh, 070h, 038h
        db  032h, 039h, 034h, 000h, 02Ah, 070h, 06Eh, 070h, 038h, 032h, 039h, 034h, 000h, 000h, 000h, 000h
        db  043h, 06Fh, 06Dh, 070h, 061h, 074h, 069h, 062h, 06Ch, 065h, 049h, 044h, 073h, 000h, 000h, 000h
        db  048h, 065h, 077h, 06Ch, 065h, 074h, 074h, 02Dh, 050h, 061h, 063h, 06Bh, 061h, 072h, 064h, 020h
        db  04Ch, 061h, 073h, 065h, 072h, 04Ah, 065h, 074h, 020h, 035h, 050h, 000h, 048h, 050h, 020h, 04Ch
        db  061h, 073h, 065h, 072h, 04Ah, 065h, 074h, 020h, 035h, 050h, 000h, 000h, 025h, 034h, 058h, 000h
        db  043h, 06Fh, 06Dh, 070h, 061h, 074h, 069h, 062h, 06Ch, 065h, 049h, 044h, 073h, 000h, 000h, 000h
        db  04Ch, 04Dh, 044h, 065h, 076h, 049h, 06Eh, 066h, 06Fh, 000h, 000h, 000h, 050h, 072h, 069h, 06Eh
        db  074h, 065h, 072h, 000h, 04Dh, 06Fh, 064h, 065h, 06Dh, 000h, 000h, 000h, 04Eh, 065h, 074h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        dd  obj3:0x0278              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj3:0x08A8              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj3:0x08D9              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
        dd  obj3:0x0B28              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 06Eh, 050h, 000h
        db  044h, 065h, 076h, 069h, 063h, 065h, 049h, 044h, 000h, 000h, 000h, 000h, 050h, 06Eh, 050h, 000h
        db  04Eh, 061h, 06Dh, 065h, 000h, 000h, 000h, 000h, 050h, 06Eh, 050h, 000h, 043h, 06Fh, 06Dh, 070h
        db  043h, 06Eh, 074h, 000h, 050h, 06Eh, 050h, 000h, 043h, 06Fh, 06Dh, 070h, 023h, 030h, 031h, 000h
        db  050h, 06Eh, 050h, 000h, 04Dh, 061h, 06Eh, 075h, 066h, 061h, 063h, 074h, 075h, 072h, 065h, 072h
        db  000h, 000h, 000h, 000h, 049h, 052h, 045h, 04Eh, 055h, 04Dh, 000h
VxD__LDATA ENDS
