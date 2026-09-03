; SECPROV -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2120 instructions recovered by recursive descent

Declare_Virtual_Device SECPROV, 4, 0, SECPROV_Control, 00493h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A002FF00h   flags 00000h
;   service table: obj1:0x9c, 7 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x64a  R-X
SECPROV_Control:
        sub    eax, 2
        cmp    eax, 0x22
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (34 entries) ----
        dd  loc_2_01E0
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_3_0F08
        db  000h, 000h, 000h
        dd  SECPROV_Service_00       ; fixup
        dd  SECPROV_Service_01       ; fixup
        dd  SECPROV_Service_02       ; fixup
        dd  SECPROV_Service_03       ; fixup
        dd  SECPROV_Service_04       ; fixup
        dd  SECPROV_Service_05       ; fixup
        dd  SECPROV_Service_06       ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00493h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SECPROV '                  ; DDB_Name
        dd  0A002FF00h                  ; DDB_Init_Order
        dd  SECPROV_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x009C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0130:
        dec    esi
        inc    ebp
        push   esp
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bl            ; -> loc_1_061A
        push   es
        add    byte ptr [eax], al
loc_1_0144:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0148:
        add    al, 1
        add    byte ptr [eax], al
loc_1_014C:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_0150:
        add    al, 0
        add    byte ptr [eax], al
loc_1_0154:
        dec    esp
        add    dword ptr [eax], eax
        add    byte ptr [eax + 1], dl        ; -> loc_1_0150
        add    byte ptr [eax], al
loc_1_015C:
        sbb    al, byte ptr [esi]
        add    byte ptr [eax], al
        inc    esp
        add    dword ptr [eax], eax
        add    byte ptr [edx], bl            ; -> loc_1_061A
        push   es
        add    byte ptr [eax], al
        inc    esp
        add    dword ptr [eax], eax
        add    byte ptr [eax + 0x1480000], cl ; -> loc_1_050C
        add    byte ptr [eax], al
loc_1_0174:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0178:
        add    byte ptr [eax], al
loc_1_017A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0504:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0508:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_050C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0610:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0614:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0618:
        add    byte ptr [eax], al
loc_1_061A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x42c  R-X
        db  083h, 0ECh, 010h, 056h, 057h, 08Dh, 044h, 024h, 00Ch, 050h, 068h
        dd  obj2:0x03A8              ; fixup
        db  068h, 002h, 000h, 000h, 080h, 0E8h, 0CBh, 002h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 00Fh
        db  085h, 07Eh, 001h, 000h, 000h, 0C7h, 044h, 024h, 010h, 004h, 000h, 000h, 000h, 08Dh, 044h, 024h
        db  010h, 050h, 08Dh, 044h, 024h, 018h, 050h, 08Dh, 044h, 024h, 010h, 050h, 06Ah, 000h, 08Bh, 044h
        db  024h, 01Ch, 068h
        dd  loc_2_03BC               ; fixup
        db  050h, 0E8h, 0A8h, 002h, 000h, 000h, 083h, 0C4h, 018h, 08Bh, 0F0h, 00Bh, 0F6h, 00Fh, 085h, 016h
        db  001h, 000h, 000h, 083h, 07Ch, 024h, 008h, 004h, 074h, 016h, 083h, 07Ch, 024h, 008h, 003h, 00Fh
        db  085h, 0FBh, 000h, 000h, 000h, 083h, 07Ch, 024h, 010h, 004h, 00Fh, 085h, 0F0h, 000h, 000h, 000h
        db  083h, 07Ch, 024h, 014h, 003h, 00Fh, 085h, 0E5h, 000h, 000h, 000h, 068h
        dd  loc_1_0144               ; fixup
        db  0A1h
        dd  obj1:0x0140              ; fixup
        db  050h, 08Dh, 044h, 024h, 010h, 050h, 06Ah, 000h, 08Bh, 044h, 024h, 01Ch, 068h
        dd  obj2:0x03CC              ; fixup
        db  050h, 0E8h, 052h, 002h, 000h, 000h, 083h, 0C4h, 018h, 08Bh, 0F0h, 00Bh, 0F6h, 00Fh, 085h, 0C0h
        db  000h, 000h, 000h, 083h, 07Ch, 024h, 008h, 001h, 00Fh, 085h, 0A6h, 000h, 000h, 000h, 0C6h, 005h
        dd  obj1:0x0619              ; fixup
        db  05Ch, 0C6h, 005h
        dd  loc_1_0618               ; fixup
        db  05Ch, 0A1h
        dd  loc_1_0144               ; fixup
        db  040h, 0BEh
        dd  loc_1_0618               ; fixup
        db  0BFh
        dd  loc_1_0178               ; fixup
        db  08Bh, 0C8h, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 0A1h
        dd  loc_1_0144               ; fixup
        db  0B9h
        dd  loc_2_03D8               ; fixup
        db  0C6h, 080h
        dd  obj1:0x0179              ; fixup
        db  05Ch, 08Bh, 0D0h, 081h, 0C2h
        dd  loc_1_017A               ; fixup
        db  08Bh, 001h, 089h, 002h, 08Bh, 041h, 004h, 089h, 042h, 004h, 08Bh, 041h, 008h, 089h, 042h, 008h
        db  08Bh, 041h, 00Ch, 089h, 042h, 00Ch, 066h, 08Bh, 041h, 010h, 066h, 089h, 042h, 010h, 0A1h
        dd  loc_1_0144               ; fixup
        db  083h, 0C0h, 014h, 0A3h
        dd  loc_1_0614               ; fixup
        db  068h
        dd  loc_1_0148               ; fixup
        db  08Dh, 044h, 024h, 00Ch, 068h
        dd  loc_1_050C               ; fixup
        db  050h, 06Ah, 000h, 08Bh, 044h, 024h, 01Ch, 068h
        dd  loc_2_03EC               ; fixup
        db  050h, 0E8h, 0AAh, 001h, 000h, 000h, 083h, 0C4h, 018h, 08Bh, 0F0h, 00Bh, 0F6h, 075h, 01Ch, 083h
        db  07Ch, 024h, 008h, 001h, 074h, 015h, 00Bh, 0F6h, 075h, 011h, 0EBh, 00Ah, 00Bh, 0F6h, 075h, 00Bh
        db  0EBh, 004h, 00Bh, 0F6h, 075h, 005h, 0BEh, 0FFh, 0FFh, 0FFh, 0FFh, 08Bh, 044h, 024h, 00Ch, 050h
        db  0E8h, 073h, 001h, 000h, 000h, 083h, 0C4h, 004h, 0E8h
        dd  loc_3_03F0               ; fixup
        db  06Ah, 001h, 0E8h, 07Ch, 001h, 000h, 000h, 0A3h
        dd  loc_1_0610               ; fixup
        db  00Bh, 0C0h, 075h, 005h, 0BEh, 0FFh, 0FFh, 0FFh, 0FFh, 0C7h, 005h
        dd  loc_1_0504               ; fixup
        db  000h, 000h, 000h, 000h, 08Bh, 0C6h, 05Fh, 05Eh, 083h, 0C4h, 010h, 0C3h, 08Dh, 064h, 024h, 000h
        db  08Dh, 064h, 024h, 000h
sub_2_01B0:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   4
        push   0x368                         ; -> loc_2_0368
        push   0x314                         ; -> loc_2_0314
        push   0
        push   0x3fc                         ; -> loc_2_03FC
        call   sub_2_02FC
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_01D9
        sub    eax, eax
loc_2_01D9:
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_2_01E0:
        mov    dword ptr [0x114], 0x50       ; -> obj1:0x0114
        mov    dword ptr [0x118], 0xa0       ; -> obj1:0x0118
        mov    ecx, 1
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jb     loc_2_027F
        mov    dword ptr [0x108], eax        ; -> obj1:0x0108
        VxDCall  Dev_0491.Service_0000h          ; cd2000009104
        jb     loc_2_027D
        call   0
        or     ax, ax
        jne    loc_2_027B
        call   sub_2_0289
        jb     loc_2_0279
        call   sub_2_01B0
        or     eax, eax
        jne    loc_2_0286
        mov    eax, dword ptr [0x114]        ; -> obj1:0x0114
        add    eax, dword ptr [0x118]        ; -> obj1:0x0118
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_2_0281
        mov    dword ptr [0x11c], eax        ; -> obj1:0x011C
        add    eax, dword ptr [0x114]        ; -> obj1:0x0114
        mov    dword ptr [0x120], eax        ; -> obj1:0x0120
        mov    byte ptr [0x125], 0           ; -> obj1:0x0125
        VxDCall  Dev_0492.Service_0000h          ; cd2000009204
        jb     loc_2_0277
        mov    esi, 0xc05                    ; -> sub_3_0C05
        mov    edi, 0xcbe                    ; -> loc_3_0CBE
        mov    ebx, 0xc05                    ; -> sub_3_0C05
        VxDCall  Dev_0492.Service_0001h          ; cd2001009204
loc_2_0277:
        clc    
loc_2_0278:
        ret    
loc_2_0279:
        jmp    loc_2_0286
loc_2_027B:
        jmp    loc_2_0286
loc_2_027D:
        jmp    loc_2_0286
loc_2_027F:
        jmp    loc_2_0286
loc_2_0281:
        jmp    loc_2_0286
        db  0F8h, 0EBh, 0D4h
loc_2_0286:
        stc    
        jmp    loc_2_0278
sub_2_0289:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 0x1c], 0xea00
        mov    word ptr [ebp + 0x10], 0
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebp + 0x10]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    ax, 1
        jb     loc_2_02E3
        VxDCall  Dev_048E.Service_0000h          ; cd2000008e04
loc_2_02E3:
        ret    
        db  0CDh, 020h, 048h, 081h, 001h, 000h, 000h, 000h, 0CDh, 020h, 049h, 081h, 001h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 081h, 001h, 000h, 000h, 000h
sub_2_02FC:
        VMMCall  VMM._RegCreateDynKey            ; cd2076810100
        add    byte ptr [eax], al
        pop    edx
        pop    ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  000h
loc_2_0314:
        inc    eax
        or     al, byte ptr [eax]
        add    byte ptr [eax + 0xa], al      ; -> loc_3_0A40
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_032C:
        push   eax
        insb   byte ptr es:[edi], dx
        popal  
        je     loc_2_0397
        outsd  dx, dword ptr [esi]
        jb     loc_2_03A1
        pop    edi
        push   esp
        jns    0x3a8
        add    byte ptr gs:[eax], al
        db  000h
loc_2_033C:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3a2
        imul   ebp, dword ptr [esi + 0x65], 0x72
loc_2_0348:
        inc    ecx
        jb     0x3b2
        jae    loc_2_03C2
        pop    edi
        push   ebx
        jb     0x3ca
        jb     loc_2_0357
loc_2_0357:
        add    byte ptr [ecx + 0x64], al
        jb     loc_2_03C2
        jae    0x3d2
        pop    edi
        inc    edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_0368:
        sub    al, 3
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        push   esp
        add    dword ptr [eax], eax
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], bh      ; -> loc_2_033C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    esp
        add    dword ptr [eax], eax
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 3], cl        ; -> loc_2_0348
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr fs:[eax], eax
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_0397:
        add    byte ptr [eax + 3], bl        ; -> obj2:0x0358
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        insb   byte ptr es:[edi], dx
loc_2_03A1:
        add    dword ptr [eax], eax
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x65], dl
        arpl   word ptr [ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x5c], 0x766f7250
        db  069h, 064h, 065h, 072h, 000h, 000h, 000h
loc_2_03BC:
        push   eax
        insb   byte ptr es:[edi], dx
        popal  
        je     0x427
        outsd  dx, dword ptr [esi]
loc_2_03C2:
        jb     0x431
        pop    edi
        push   esp
        jns    0x438
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        je     0x432
        imul   ebp, dword ptr [esi + 0x65], 0x72
loc_2_03D8:
        push   edi
        dec    ecx
        dec    esi
        inc    esp
        dec    edi
        push   edi
        push   ebx
        pop    edi
        push   eax
        inc    ecx
        push   ebx
        push   ebx
        push   esp
        dec    eax
        push   edx
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_03EC:
        inc    ecx
        jb     0x456
        jae    0x466
        pop    edi
        inc    edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_03FC:
        push   ebx
        arpl   word ptr gs:[ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x5c], 0x766f7250
        imul   esp, dword ptr [ebp + 0x72], 0x4e000000
        je     0x48b
        outsd  dx, dword ptr [esi]
        jb     0x482
        add    byte ptr [esi + 0x57], cl
        xor    eax, dword ptr [ecx + 0x63]
        arpl   word ptr [edi + 0x75], bp
        outsb  dx, byte ptr [esi]
        je     0x476
        jb     0x49c
        jb     loc_2_0429
loc_2_0429:
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xfaa  R-X
SECPROV_Service_01:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x24], 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x28], eax
        sub    eax, 4
        mov    dword ptr [esp + 0x18], 0
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax], 4
        mov    esi, dword ptr [esp + 0x38]
        push   esi
        call   sub_3_0690
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_3_006B
        call   sub_3_0C05
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        je     loc_3_0067
        push   esi
        call   sub_3_04A0
        add    esp, 4
        mov    edi, eax
loc_3_0067:
        or     edi, edi
        je     loc_3_0090
loc_3_006B:
        lea    ebx, [edi + 0x40]
        mov    eax, dword ptr [edi + 0x3c]
        mov    dword ptr [esp + 0x1c], eax
        shl    eax, 2
        cmp    dword ptr [esp + 0x20], eax
        jae    loc_3_0086
        mov    dword ptr [esp + 0x24], 2
loc_3_0086:
        sub    dword ptr [esp + 0x20], eax
        jmp    loc_3_009C
        db  08Dh, 064h, 024h, 000h
loc_3_0090:
        mov    dword ptr [esp + 0x24], 1
        mov    ebx, dword ptr [esp + 0x2c]
loc_3_009C:
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_3_0198
        mov    ebp, dword ptr [esp + 0x3c]
        add    ebp, 4
        mov    ecx, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], eax
        lea    eax, [ebp + eax*4]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x1c]
        dec    dword ptr [esp + 0x1c]
        or     eax, eax
        je     loc_3_0198
loc_3_00D0:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_3_07A0
        add    esp, 4
        or     eax, eax
        je     loc_3_0150
        mov    edx, eax
loc_3_00E1:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x14], ecx
        mov    eax, ecx
        cmp    dword ptr [esp + 0x20], eax
        jb     loc_3_0190
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x10]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, 4
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebp], eax
        add    ebp, 4
        add    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x14]
        sub    dword ptr [esp + 0x20], eax
loc_3_013E:
        mov    eax, dword ptr [esp + 0x1c]
        dec    dword ptr [esp + 0x1c]
        or     eax, eax
        jne    loc_3_00D0
        jmp    loc_3_0198
        db  08Dh, 064h, 024h, 000h
loc_3_0150:
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_3_0164
        call   sub_3_0C05
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        je     loc_3_0184
loc_3_0164:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_3_0410
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_3_00E1
        mov    ecx, dword ptr [esp + 0x3c]
        add    ebx, 4
        dec    dword ptr [ecx]
        jmp    loc_3_013E
loc_3_0184:
        mov    dword ptr [esp + 0x24], 4
        jmp    loc_3_0198
        db  08Bh, 0FFh
loc_3_0190:
        mov    dword ptr [esp + 0x24], 2
loc_3_0198:
        mov    ecx, dword ptr [esp + 0x28]
        mov    eax, dword ptr [esp + 0x40]
        sub    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x24]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_01C0:
        cmp    dword ptr [0x504], 0          ; -> loc_1_0504
        je     loc_3_01DE
        mov    dword ptr [0x504], 0          ; -> loc_1_0504
        mov    eax, dword ptr [0x610]        ; -> loc_1_0610
        push   eax
        call   sub_3_0F60
loc_3_01DE:
        ret    
        db  090h
SECPROV_Service_02:
        push   esi
        push   1
        mov    eax, dword ptr [0x610]        ; -> loc_1_0610
        push   eax
        call   sub_3_0F6C
        call   sub_3_0C05
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        push   0xc
        mov    word ptr [eax], 0x100
        mov    byte ptr [eax + 2], 0x17
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        push   eax
        push   3
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        push   eax
        push   0x17
        call   sub_3_0E1F
        add    esp, 0x14
        mov    esi, eax
        or     esi, esi
        jne    loc_3_0262
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esp + 0xc]
        push   0
        mov    dword ptr [eax], 8
        push   0x1388
        push   0x1c0                         ; -> loc_3_01C0
        call   sub_3_0F78
        mov    dword ptr [0x508], eax        ; -> loc_1_0508
        mov    dword ptr [0x504], 1          ; -> loc_1_0504
        mov    eax, esi
        pop    esi
        ret    
loc_3_0262:
        mov    eax, dword ptr [0x610]        ; -> loc_1_0610
        push   eax
        call   sub_3_0F60
        mov    eax, esi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_03:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [0x11c]        ; -> obj1:0x011C
        cmp    dword ptr [esp + 0x24], 8
        je     loc_3_02A0
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_02A0:
        cmp    byte ptr [0x125], 0           ; -> obj1:0x0125
        jne    loc_3_02B3
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_02B3:
        mov    byte ptr [ebx + 2], 0x4a
        mov    eax, dword ptr [esp + 0x20]
        lea    edx, [ebx + 3]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        mov    ecx, 0xffffffff
        mov    word ptr [ebx + 0xb], 0x100
        mov    ebp, dword ptr [esp + 0x18]
        mov    edi, ebp
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    byte ptr [ebx + 0xd], cl
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0xe]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, byte ptr [ebx + 0xd]
        and    eax, 0xffff00ff
        add    ax, 0xc
        push   eax
        call   sub_3_0B09
        add    esp, 4
        mov    word ptr [ebx], ax
        push   0
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 0xd]
        add    eax, 0xe
        push   eax
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        push   eax
        push   0x17
        call   sub_3_0E1F
        add    esp, 0x14
        or     eax, eax
        mov    eax, dword ptr [esp + 0x30]
        je     loc_3_0350
        mov    word ptr [eax], 5
        jmp    loc_3_035E
        db  08Dh, 049h, 000h
loc_3_0350:
        mov    word ptr [eax], 0
        push   ebp
        call   sub_3_04A0
        add    esp, 4
loc_3_035E:
        cmp    dword ptr [0x504], 0          ; -> loc_1_0504
        je     loc_3_0387
        mov    dword ptr [0x504], 0          ; -> loc_1_0504
        mov    eax, dword ptr [0x508]        ; -> loc_1_0508
        push   eax
        call   sub_3_0F94
        mov    eax, dword ptr [0x610]        ; -> loc_1_0610
        push   eax
        call   sub_3_0F60
loc_3_0387:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
SECPROV_Service_06:
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_04:
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_0730
        add    esp, 4
        sub    eax, eax
        ret    
SECPROV_Service_05:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 3
        mov    eax, dword ptr [0x144]        ; -> loc_1_0144
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x140]        ; -> obj1:0x0140
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [0x144]        ; -> loc_1_0144
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [0x140]        ; -> obj1:0x0140
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [0x148]        ; -> loc_1_0148
        mov    dword ptr [ecx + 0x14], eax
        sub    eax, eax
        mov    dword ptr [ecx + 0x18], 0x50c ; -> loc_1_050C
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_03F0:
        mov    dword ptr [0x174], 0          ; -> loc_1_0174
        mov    dword ptr [0x1c0], 0          ; -> loc_1_01C0
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0410:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        mov    dword ptr [esp + 4], eax
        mov    word ptr [eax], 0x500
        add    dword ptr [esp + 4], 2
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax], 0x36
        inc    dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 4]
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [0x118]        ; -> obj1:0x0118
        push   eax
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        push   eax
        push   7
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        push   eax
        push   0x17
        call   sub_3_0E1F
        add    esp, 0x14
        or     eax, eax
        je     loc_3_0465
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
loc_3_0465:
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        add    eax, 6
        push   eax
        push   eax
        call   sub_3_0AB7
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        add    eax, 6
        push   eax
        push   esi
        call   sub_3_07D0
        add    esp, 8
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        add    eax, 6
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_04A0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0x200
        push   0x1d0
        call   sub_3_0F48
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_04D0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_04D0:
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 4]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [ebx + 0x34], 0
        call   sub_3_0FA4
        mov    dword ptr [ebx + 0x38], eax
        mov    dword ptr [ebx + 0x3c], 0
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        mov    dword ptr [esp + 0x10], eax
        add    dword ptr [esp + 0x10], 2
        mov    eax, dword ptr [esp + 0x10]
        lea    ebp, [ebx + 0x40]
        mov    byte ptr [eax], 0x3d
        inc    dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        mov    word ptr [eax], 0x100
        add    dword ptr [esp + 0x10], 2
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x10]
        inc    edi
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esp + 0x10]
        dec    ecx
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [esp + 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        inc    eax
        add    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x10]
        inc    dword ptr [esp + 0x10]
        mov    byte ptr [eax], 1
        mov    edx, 0                        ; -> obj4:0x0000
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 0xd
        inc    dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    ax, word ptr [edx + 0xc]
        mov    word ptr [ecx + 0xc], ax
        add    dword ptr [esp + 0x10], 0xd
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        sub    ax, 2
        push   eax
        call   sub_3_0B09
        mov    ecx, dword ptr [0x11c]        ; -> obj1:0x011C
        add    esp, 4
        mov    word ptr [ecx], ax
        mov    esi, dword ptr [0x120]        ; -> obj1:0x0120
loc_3_05E2:
        mov    eax, dword ptr [0x118]        ; -> obj1:0x0118
        push   eax
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        sub    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        push   eax
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        push   eax
        push   0x17
        call   sub_3_0E1F
        add    esp, 0x14
        or     eax, eax
        jne    loc_3_0638
        cmp    dword ptr [esi], 0
        je     loc_3_062D
        mov    ecx, esi
loc_3_0614:
        cmp    dword ptr [ebx + 0x3c], 0x64
        jae    loc_3_062D
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp], edx
        add    ebp, 4
        add    ecx, 4
        inc    dword ptr [ebx + 0x3c]
        cmp    dword ptr [ecx], 0
        jne    loc_3_0614
loc_3_062D:
        cmp    byte ptr [esi + 0x80], 1
        jne    loc_3_0641
        jmp    loc_3_05E2
loc_3_0638:
        cmp    eax, 0xec
        jne    loc_3_0641
        sub    eax, eax
loc_3_0641:
        or     eax, eax
        jne    loc_3_0677
        push   0x200
        mov    eax, dword ptr [ebx + 0x3c]
        lea    eax, [eax*4 + 0x40]
        push   eax
        push   ebx
        call   sub_3_0F58
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0664
        mov    ebx, eax
loc_3_0664:
        push   ebx
        call   sub_3_0700
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_0677:
        push   0
        push   ebx
        call   sub_3_0F50
        add    esp, 8
        sub    ebx, ebx
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_3_0690:
        push   ebx
        push   esi
        push   edi
        mov    edx, 0x174                    ; -> loc_1_0174
        mov    eax, dword ptr [0x174]        ; -> loc_1_0174
        or     eax, eax
        je     loc_3_06F9
        mov    esi, dword ptr [esp + 0x10]
loc_3_06A5:
        lea    ecx, [eax + 4]
        mov    edi, esi
loc_3_06AA:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [edi]
        jne    loc_3_06CA
        or     bl, bl
        je     loc_3_06C6
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    loc_3_06CA
        add    ecx, 2
        add    edi, 2
        or     bl, bl
        jne    loc_3_06AA
loc_3_06C6:
        sub    ecx, ecx
        jmp    loc_3_06CF
loc_3_06CA:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_06CF:
        or     ecx, ecx
        je     loc_3_06E0
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_3_06A5
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_3_06E0:
        cmp    eax, dword ptr [0x174]        ; -> loc_1_0174
        je     loc_3_06F9
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [0x174]        ; -> loc_1_0174
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x174], eax        ; -> loc_1_0174
loc_3_06F9:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_3_0700:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 4]
        push   eax
        call   sub_3_0730
        add    esp, 4
        mov    eax, dword ptr [0x174]        ; -> loc_1_0174
        mov    dword ptr [esi], eax
        mov    dword ptr [0x174], esi        ; -> loc_1_0174
        mov    eax, esi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0730:
        push   ebx
        push   esi
        push   edi
        mov    ecx, 0x174                    ; -> loc_1_0174
        mov    edx, dword ptr [0x174]        ; -> loc_1_0174
        or     edx, edx
        je     loc_3_078F
        mov    esi, dword ptr [esp + 0x10]
loc_3_0746:
        lea    eax, [edx + 4]
        mov    edi, esi
loc_3_074B:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [edi]
        jne    loc_3_076B
        or     bl, bl
        je     loc_3_0767
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    loc_3_076B
        add    eax, 2
        add    edi, 2
        or     bl, bl
        jne    loc_3_074B
loc_3_0767:
        sub    eax, eax
        jmp    loc_3_0770
loc_3_076B:
        sbb    eax, eax
        sbb    eax, -1
loc_3_0770:
        or     eax, eax
        je     loc_3_0780
        mov    ecx, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_3_0746
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0780:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        push   0
        push   edx
        call   sub_3_0F50
        add    esp, 8
loc_3_078F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_07A0:
        mov    eax, 0x1c4                    ; -> loc_1_01C4
        sub    edx, edx
        cmp    edx, dword ptr [0x1c0]        ; -> loc_1_01C0
        jae    loc_3_07CA
        mov    ecx, dword ptr [esp + 4]
loc_3_07B3:
        cmp    dword ptr [eax + 0x30], ecx
        je     loc_3_07C4
        inc    edx
        add    eax, 0x34
        cmp    edx, dword ptr [0x1c0]        ; -> loc_1_01C0
        jb     loc_3_07B3
loc_3_07C4:
        cmp    edx, dword ptr [0x1c0]        ; -> loc_1_01C0
loc_3_07CA:
        jne    loc_3_07CE
        sub    eax, eax
loc_3_07CE:
        ret    
        db  090h
sub_3_07D0:
        push   esi
        push   edi
        cmp    dword ptr [0x1c0], 0x10       ; -> loc_1_01C0
        jb     loc_3_07E0
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_3_07E0:
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_3_07A0
        add    esp, 4
        or     eax, eax
        jne    loc_3_0833
        mov    eax, dword ptr [0x1c0]        ; -> loc_1_01C0
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    edx, [eax*4 + 0x1c4]          ; -> loc_1_01C4
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 0x30], esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        inc    dword ptr [0x1c0]             ; -> loc_1_01C0
        mov    eax, edx
loc_3_0833:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0840:
        sub    esp, 0x70
        push   esi
        push   edi
        call   sub_3_0C05
        mov    byte ptr [esp + 0xa], 0x35
        mov    word ptr [esp + 0xb], 0x100
        mov    edx, dword ptr [esp + 0x7c]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    byte ptr [esp + 0xd], cl
        cmp    cl, 0x30
        jbe    loc_3_0879
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 0x70
        ret    
loc_3_0879:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0xe]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  esi, byte ptr [esp + 0xd]
        lea    eax, [esi + 4]
        add    esi, 6
        push   eax
        call   sub_3_0B09
        mov    word ptr [esp + 0xc], ax
        add    esp, 4
        lea    eax, [esp + 0x40]
        push   0x36
        push   eax
        lea    eax, [esp + 0x10]
        push   esi
        push   eax
        push   0x17
        call   sub_3_0E1F
        add    esp, 0x14
        or     eax, eax
        je     loc_3_08D8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 0x70
        ret    
loc_3_08D8:
        mov    eax, dword ptr [esp + 0x40]
        pop    edi
        pop    esi
        add    esp, 0x70
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_08F0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [eax]
        push   eax
        call   SECPROV_Service_02
        add    esp, 8
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0910:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x1c], 4
        je     loc_3_0928
        mov    eax, 0x32
        pop    esi
        add    esp, 4
        ret    
loc_3_0928:
        lea    eax, [esp + 6]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   SECPROV_Service_03
        add    esp, 0x20
        xor    edx, edx
        mov    ecx, dword ptr [esi + 0x20]
        mov    dx, word ptr [esp + 6]
        pop    esi
        mov    dword ptr [ecx], edx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0970:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        call   SECPROV_Service_04
        add    esp, 0xc
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0990:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx], 4
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], 3
        sub    eax, eax
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_09B0:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [0x144]        ; -> loc_1_0144
        cmp    dword ptr [eax], ecx
        jae    loc_3_09D0
        mov    eax, 0x6f
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_09D0:
        mov    eax, dword ptr [0x144]        ; -> loc_1_0144
        mov    edi, dword ptr [edx]
        mov    esi, dword ptr [0x140]        ; -> obj1:0x0140
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [0x144]        ; -> loc_1_0144
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0A00:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_3_0840
        add    esp, 4
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    loc_3_0A20
        mov    eax, 1
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_3_0A20:
        sub    eax, eax
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0A30:
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0A40:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        sub    eax, eax
        mov    ebx, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    edx, dword ptr [esp + 0x18]
        cmp    dword ptr [esp + 0x1c], eax
        je     loc_3_0AA8
loc_3_0A58:
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        add    dword ptr [edi], ecx
        mov    ebp, dword ptr [edx]
        cmp    ebx, ebp
        jb     loc_3_0A93
        mov    edi, dword ptr [esp + 0x20]
        mov    esi, dword ptr [esi]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [edx]
        add    dword ptr [esp + 0x20], ecx
        sub    ebx, dword ptr [edx]
        jmp    loc_3_0A9F
loc_3_0A93:
        mov    dword ptr [edx + 8], 0
        mov    eax, 0xea
loc_3_0A9F:
        add    edx, 0xc
        dec    dword ptr [esp + 0x1c]
        jne    loc_3_0A58
loc_3_0AA8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
SECPROV_Service_00:
        mov    eax, 0x400
        clc    
        ret    
sub_3_0AB7:
        VxDCall  Dev_0491.Service_800Fh          ; cd200f809104
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
loc_3_0AC8:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        jne    loc_3_0AC8
        pop    edi
        pop    esi
        ret    
sub_3_0AD1:
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
loc_3_0ADC:
        mov    al, byte ptr [edi]
        cmp    al, byte ptr [esi]
        jne    loc_3_0AED
        inc    edi
        inc    esi
        or     al, al
        jne    loc_3_0ADC
        sub    eax, eax
loc_3_0AEA:
        pop    edi
        pop    esi
        ret    
loc_3_0AED:
        mov    eax, 0xffffffff
        jmp    loc_3_0AEA
        db  057h, 08Bh, 07Ch, 024h, 008h, 02Bh, 0C0h, 08Bh, 0C8h, 049h, 0F2h, 0AEh, 0F7h, 0D9h, 083h, 0E9h
        db  002h, 08Bh, 0C1h, 05Fh, 0C3h
sub_3_0B09:
        mov    eax, dword ptr [esp + 4]
        xchg   al, ah
        ret    
sub_3_0B10:
        enter  0x34, 0
        lea    ebx, [ebp - 0x34]
        mov    dword ptr [ebx], 0x618        ; -> loc_1_0618
        mov    dword ptr [ebx + 0x1c], 0x5f03
        mov    dword ptr [ebx + 0x10], 0
        mov    word ptr [ebx + 0x14], 0
        push   ebx
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        pop    eax
        mov    ax, word ptr [ebx + 0x2c]
        test   ax, 1
        jne    loc_3_0B6F
        mov    eax, dword ptr [ebx + 0x1c]
        shr    eax, 0x10
        mov    byte ptr [0x125], al          ; -> obj1:0x0125
        mov    word ptr [0x126], 0           ; -> obj1:0x0126
        mov    dword ptr [ebx + 0x1c], 0x5f03
        mov    word ptr [ebx + 0x14], 0x203
        push   ebx
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        pop    eax
        clc    
loc_3_0B6D:
        leave  
        ret    
loc_3_0B6F:
        mov    byte ptr [0x125], 0           ; -> obj1:0x0125
        stc    
        jmp    loc_3_0B6D
sub_3_0B79:
        enter  0x34, 0
        lea    ebx, [ebp - 0x34]
        mov    dword ptr [ebx], 0x618        ; -> loc_1_0618
        mov    dword ptr [ebx + 0x1c], 0x5f03
        mov    dword ptr [ebx + 0x10], 0
        mov    word ptr [ebx + 0x14], 0x201
        push   ebx
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        pop    eax
        mov    ax, word ptr [ebx + 0x2c]
        test   ax, 1
        jne    loc_3_0BC3
        mov    eax, dword ptr [ebx + 0x1c]
        shr    eax, 0x10
        mov    byte ptr [0x125], al          ; -> obj1:0x0125
        mov    word ptr [0x126], 0x200       ; -> obj1:0x0126
        clc    
loc_3_0BC1:
        leave  
        ret    
loc_3_0BC3:
        mov    byte ptr [0x125], 0           ; -> obj1:0x0125
        stc    
        jmp    loc_3_0BC1
sub_3_0BCD:
        enter  0x34, 0
        push   ebx
        mov    byte ptr [0x125], 0           ; -> obj1:0x0125
        lea    ebx, [ebp - 0x34]
        mov    dword ptr [ebx], 0x618        ; -> loc_1_0618
        mov    dword ptr [ebx + 0x1c], 0x5f03
        mov    dword ptr [ebx + 0x10], 0
        mov    word ptr [ebx + 0x14], 0x203
        push   ebx
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        pop    ebx
        mov    ax, word ptr [ebx + 0x2c]
        pop    ebx
        leave  
        ret    
sub_3_0C05:
        enter  0x34, 0
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [0x108]        ; -> obj1:0x0108
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        movzx  eax, byte ptr [0x125]         ; -> obj1:0x0125
        or     eax, eax
        je     loc_3_0C76
        lea    ebx, [ebp - 0x34]
        shl    eax, 0x10
        mov    ax, 0xe9fe
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        mov    dword ptr [ebx], eax
        push   ebx
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        add    esp, 4
        lea    ebx, [ebp - 0x34]
        mov    ax, word ptr [ebx + 0x2c]
        test   ax, 1
        jne    loc_3_0C76
        push   dword ptr [0x11c]             ; -> obj1:0x011C
        push   dword ptr [0x140]             ; -> obj1:0x0140
        call   sub_3_0AD1
        add    esp, 8
        or     eax, eax
        jne    loc_3_0C76
        cmp    word ptr [0x126], 0           ; -> obj1:0x0126
        je     loc_3_0C84
        jmp    loc_3_0CA5
loc_3_0C76:
        call   sub_3_0B10
        jae    loc_3_0C84
        call   sub_3_0B79
        jb     loc_3_0CBC
loc_3_0C84:
        call   sub_3_0D40
        mov    edx, dword ptr [eax]
        or     edx, edx
        jne    loc_3_0CA5
        cmp    word ptr [0x126], 0           ; -> obj1:0x0126
        jne    loc_3_0CA0
        call   sub_3_0B79
        jb     loc_3_0CBC
loc_3_0CA0:
        call   sub_3_0DDC
loc_3_0CA5:
        mov    eax, dword ptr [0x108]        ; -> obj1:0x0108
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        movzx  eax, byte ptr [0x125]         ; -> obj1:0x0125
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0CBC:
        jmp    loc_3_0CA5
loc_3_0CBE:
        ret    
        db  057h, 056h, 08Bh, 03Dh
        dd  obj1:0x011C              ; fixup
        db  0C6h, 047h, 002h, 03Dh, 083h, 0C7h, 003h, 066h, 0C7h, 007h, 000h, 004h, 08Bh, 00Dh
        dd  loc_1_0144               ; fixup
        db  049h, 088h, 04Fh, 002h, 083h, 0C7h, 003h, 08Bh, 035h
        dd  obj1:0x0140              ; fixup
        db  0F3h, 0A4h, 0C6h, 007h, 001h, 0B9h, 00Bh, 000h, 000h, 000h, 047h, 088h, 00Fh, 047h, 0BEh
        dd  loc_1_0130               ; fixup
        db  0F3h, 0A4h, 08Bh, 035h
        dd  obj1:0x011C              ; fixup
        db  02Bh, 0FEh, 066h, 08Bh, 0C7h, 066h, 083h, 0E8h, 002h, 086h, 0E0h, 066h, 089h, 006h, 0FFh, 035h
        dd  obj1:0x0118              ; fixup
        db  0FFh, 035h
        dd  obj1:0x0120              ; fixup
        db  057h, 0FFh, 035h
        dd  obj1:0x011C              ; fixup
        db  0B8h, 017h, 000h, 000h, 000h, 050h, 0E8h, 0F2h, 000h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h
        db  075h, 008h, 0A1h
        dd  obj1:0x0120              ; fixup
        db  05Eh, 05Fh, 0C3h, 02Bh, 0C0h, 0EBh, 0F9h
sub_3_0D40:
        enter  0x34, 0
        lea    edi, [ebp - 0x34]
        movzx  eax, byte ptr [0x125]         ; -> obj1:0x0125
        shl    eax, 0x10
        mov    ax, 0xe9ff
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        mov    dword ptr [edi], eax
        push   edi
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        pop    eax
        mov    ax, word ptr [edi + 0x2c]
        and    ax, 1
        jne    loc_3_0DB5
        mov    edi, dword ptr [0x120]        ; -> obj1:0x0120
        mov    eax, dword ptr [0x11c]        ; -> obj1:0x011C
        cmp    byte ptr [edi + 0x1c], 0
        jne    loc_3_0DC4
        mov    cl, byte ptr [edi + 0x1b]
        mov    byte ptr [eax + 2], 0x16
        mov    word ptr [eax], 0x200
        mov    byte ptr [eax + 3], cl
        mov    edx, 4
loc_3_0D96:
        push   dword ptr [0x118]             ; -> obj1:0x0118
        push   dword ptr [0x120]             ; -> obj1:0x0120
        push   edx
        push   dword ptr [0x11c]             ; -> obj1:0x011C
        push   0x17
        call   sub_3_0E1F
        add    esp, 0x14
        or     eax, eax
loc_3_0DB5:
        mov    eax, dword ptr [0x120]        ; -> obj1:0x0120
        je     loc_3_0DC2
        mov    dword ptr [eax], 0
loc_3_0DC2:
        leave  
        ret    
loc_3_0DC4:
        mov    byte ptr [eax + 2], 0x1c
        mov    word ptr [eax], 0x500
        mov    cx, word ptr [edi + 0x1b]
        mov    word ptr [eax + 3], cx
        mov    edx, 7
        jmp    loc_3_0D96
sub_3_0DDC:
        enter  0x34, 0
        push   ebx
        mov    eax, 0x178                    ; -> loc_1_0178
        lea    edx, [ebp - 0x34]
        mov    dword ptr [edx], eax
        mov    eax, 0x13c                    ; -> obj1:0x013C
        mov    dword ptr [edx + 4], eax
        mov    ax, 0x5f03
        mov    dword ptr [edx + 0x1c], eax
        mov    byte ptr [edx + 0x10], 0
        mov    word ptr [edx + 0x18], 1
        mov    word ptr [edx + 0x14], 0x201
        push   edx
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        pop    edx
        movzx  eax, word ptr [edx + 0x2c]
        and    al, 1
        or     al, al
        jne    loc_3_0E1C
loc_3_0E1C:
        pop    ebx
        leave  
        ret    
sub_3_0E1F:
        enter  0x34, 0
        push   ebx
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [0x128], eax        ; -> obj1:0x0128
        mov    eax, dword ptr [ebp + 0xc]
        lea    ebx, [ebp - 0x34]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [ebx + 0x18], ax
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [ebx + 0x14], ax
        movzx  eax, byte ptr [0x125]         ; -> obj1:0x0125
        shl    eax, 0x10
        mov    ah, 0xf2
        mov    al, byte ptr [ebp + 8]
        mov    dword ptr [ebx + 0x1c], eax
        push   ebx
        VxDCall  Dev_048E.Service_0001h          ; cd2001008e04
        pop    eax
        mov    eax, dword ptr [0x12c]        ; -> obj1:0x012C
        or     eax, eax
        jne    loc_3_0E97
        mov    ax, word ptr [0x126]          ; -> obj1:0x0126
        cmp    ax, 0
        je     loc_3_0E97
        mov    eax, 0xea60
        mov    edx, 0
        push   esi
        mov    esi, 0xe9e                    ; -> loc_3_0E9E
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x12c], esi        ; -> obj1:0x012C
        pop    esi
loc_3_0E97:
        movzx  eax, byte ptr [ebx + 0x1c]
        pop    ebx
        leave  
        ret    
loc_3_0E9E:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, dword ptr [0x128]        ; -> obj1:0x0128
        cmp    eax, 0x7530
        ja     loc_3_0ECF
        mov    eax, 0xea60
        mov    edx, 0
        push   esi
        mov    esi, 0xe9e                    ; -> loc_3_0E9E
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x12c], esi        ; -> obj1:0x012C
        pop    esi
        ret    
loc_3_0ECF:
        mov    dword ptr [0x12c], 0          ; -> obj1:0x012C
        mov    eax, 0
        push   eax
        push   eax
        push   0xeef                         ; -> loc_3_0EEF
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        ret    
loc_3_0EEF:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, dword ptr [0x128]        ; -> obj1:0x0128
        cmp    eax, 0x7530
        jbe    loc_3_0F07
        call   sub_3_0BCD
loc_3_0F07:
        ret    
loc_3_0F08:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 0
        jne    loc_3_0F17
        mov    ecx, 0x400
        jmp    loc_3_0F1C
loc_3_0F17:
        cmp    ecx, -1
        jne    loc_3_0F20
loc_3_0F1C:
        xor    eax, eax
        clc    
        ret    
loc_3_0F20:
        cmp    ecx, 0x64
        jl     loc_3_0F3F
        cmp    ecx, 0x6c
        jge    loc_3_0F3F
        sub    ecx, 0x64
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        call   sub_4_0010
        add    esp, 8
        clc    
        ret    
loc_3_0F3F:
        mov    eax, 1
        clc    
        ret    
        db  000h, 000h
sub_3_0F48:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_3_0F50:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_3_0F58:
        VMMCall  VMM._HeapReAllocate             ; cd2050800100
        add    byte ptr [eax], al
sub_3_0F60:
        pop    edx
        pop    eax
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        jmp    edx
        db  000h, 000h
sub_3_0F6C:
        pop    edx
        pop    eax
        pop    ecx
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        jmp    edx
        db  000h
sub_3_0F78:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_3_0F94:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_3_0FA4:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf800100
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x30  RW-
        db  047h, 052h, 04Fh, 055h, 050h, 053h, 05Fh, 049h, 027h, 04Dh, 05Fh, 049h, 04Eh, 000h, 000h, 000h
sub_4_0010:
        lock or byte ptr [eax], al
        add    byte ptr [eax], dl            ; -> loc_3_0910
        or     dword ptr [eax], eax
        add    byte ptr [eax + 9], dh        ; -> loc_3_0970
        add    byte ptr [eax], al
        nop    
        or     dword ptr [eax], eax
        add    byte ptr [eax + 9], dh        ; -> loc_3_09B0
        or     al, byte ptr [eax]
        add    byte ptr [eax], dh            ; -> loc_3_0A30
        or     al, byte ptr [eax]
        add    byte ptr [eax], dh            ; -> loc_3_0A30
        or     ch, byte ptr [esi]
        push   eax
VxD__LDATA ENDS
