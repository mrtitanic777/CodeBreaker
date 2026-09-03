; SECPROV -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1276 instructions recovered by recursive descent

Declare_Virtual_Device SECPROV, 4, 0, SECPROV_Control, 00493h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A002FF00h   flags 00000h
;   service table: obj1:0x9c, 7 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x6ce  R-X
SECPROV_Control:
        sub    eax, 2
        cmp    eax, 0x22
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (34 entries) ----
        dd  loc_2_00A0
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
        dd  loc_3_08B8
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x30000006], bl ; -> obj1:0x069E
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], al
        add    byte ptr [eax], al
loc_1_0150:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_0154:
        add    al, 0
        add    byte ptr [eax], al
loc_1_0158:
        push   eax
        add    dword ptr [eax], eax
        add    byte ptr [ecx + eax], dl      ; -> loc_1_0154
        add    byte ptr [esi + 0x48000006], bl ; -> obj1:0x069E
        add    dword ptr [eax], eax
        add    byte ptr [esi + 0x48000006], bl ; -> obj1:0x069E
        add    dword ptr [eax], eax
        add    byte ptr [eax + 0x4c000005], dl ; -> obj1:0x0590
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x295  R-X
        db  083h, 0ECh, 008h, 0B8h
        dd  loc_2_020C               ; fixup
        db  0BAh
        dd  obj1:0x017C              ; fixup
        db  08Bh, 008h, 089h, 00Ah, 08Bh, 040h, 004h, 089h, 042h, 004h, 08Dh, 044h, 024h, 000h, 050h, 068h
        dd  obj2:0x0214              ; fixup
        db  068h, 002h, 000h, 000h, 080h, 0E8h, 02Dh, 001h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h
        db  032h, 0C7h, 044h, 024h, 004h, 001h, 000h, 000h, 000h, 08Dh, 044h, 024h, 004h, 06Ah, 004h, 050h
        db  06Ah, 004h, 08Bh, 044h, 024h, 00Ch, 068h
        dd  obj2:0x0228              ; fixup
        db  050h, 0E8h, 016h, 001h, 000h, 000h, 08Bh, 044h, 024h, 014h, 083h, 0C4h, 014h, 050h, 0E8h, 001h
        db  001h, 000h, 000h, 083h, 0C4h, 004h, 02Bh, 0C0h, 083h, 0C4h, 008h, 0C3h, 08Dh, 064h, 024h, 000h
        db  08Dh, 064h, 024h, 000h
sub_2_0070:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   4
        push   0x1cc                         ; -> loc_2_01CC
        push   0x178                         ; -> loc_2_0178
        push   0
        push   0x268                         ; -> loc_2_0268
        call   sub_2_0170
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_0099
        sub    eax, eax
loc_2_0099:
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_2_00A0:
        mov    dword ptr [0x114], 0x50       ; -> obj1:0x0114
        mov    dword ptr [0x118], 0xa0       ; -> obj1:0x0118
        mov    ecx, 1
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jb     loc_2_00F2
        mov    dword ptr [0x108], eax        ; -> obj1:0x0108
        VxDCall  Dev_0491.Service_0000h          ; cd2000009104
        jb     loc_2_00F0
        call   0
        or     ax, ax
        jne    loc_2_00EE
        call   sub_2_0070
        or     eax, eax
        jne    loc_2_00F9
        jmp    loc_2_00EA
        db  0C6h, 005h
        dd  obj1:0x0125              ; fixup
        db  000h
loc_2_00EA:
        clc    
loc_2_00EB:
        ret    
        db  0EBh, 00Bh
loc_2_00EE:
        jmp    loc_2_00F9
loc_2_00F0:
        jmp    loc_2_00F9
loc_2_00F2:
        jmp    loc_2_00F9
        db  0EBh, 003h, 0F8h, 0EBh, 0F1h
loc_2_00F9:
        stc    
        jmp    loc_2_00EB
        db  083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0CDh
        db  020h, 082h, 000h, 001h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 066h, 0C7h
        db  045h, 01Ch, 000h, 0EAh, 066h, 0C7h, 045h, 010h, 000h, 000h, 0B8h, 021h, 000h, 000h, 000h, 0CDh
        db  020h, 084h, 000h, 001h, 000h, 066h, 08Bh, 045h, 010h, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h
        db  08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 066h, 083h
        db  0F8h, 001h, 072h, 006h, 0CDh, 020h, 000h, 000h, 08Eh, 004h, 0C3h, 000h, 0CDh, 020h, 048h, 081h
        db  001h, 000h, 000h, 000h, 0CDh, 020h, 049h, 081h, 001h, 000h, 000h, 000h, 0CDh, 020h, 04Eh, 081h
        db  001h, 000h, 000h, 000h
sub_2_0170:
        VMMCall  VMM._RegCreateDynKey            ; cd2076810100
        add    byte ptr [eax], al
loc_2_0178:
        lock add eax, dword ptr [eax]
        add    al, dh                        ; -> loc_3_03F0
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x6c], dl
        popal  
        je     0x1fb
        outsd  dx, dword ptr [esi]
        jb     0x205
        pop    edi
        push   esp
        jns    loc_2_020C
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        je     loc_2_0206
        imul   ebp, dword ptr [esi + 0x65], 0x72
loc_2_01AC:
        inc    ecx
        jb     loc_2_0216
        jae    0x226
        pop    edi
        push   ebx
        jb     loc_2_022E
        jb     loc_2_01BB
loc_2_01BB:
        add    byte ptr [ecx + 0x64], al
        jb     0x226
        jae    0x236
        pop    edi
        inc    edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_01CC:
        nop    
        add    dword ptr [eax], eax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        pop    eax
        add    dword ptr [eax], eax
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], ah        ; -> obj2:0x01A0
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], ah        ; -> obj1:0x0160
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], ch        ; -> obj1:0x0168
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        mov    esp, 1
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], dh        ; -> obj1:0x0170
loc_2_0206:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_020C:
        cmp    al, 0x4c
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x6c], sp
        add    byte ptr ds:[ebx + 0x65], dl
loc_2_0216:
        arpl   word ptr [ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x5c], 0x766f7250
        imul   esp, dword ptr [ebp + 0x72], 0x50000000
        insb   byte ptr es:[edi], dx
        popal  
        je     0x293
        outsd  dx, dword ptr [esi]
loc_2_022E:
        jb     0x29d
        pop    edi
        push   esp
        jns    0x2a4
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        je     0x29e
        imul   ebp, dword ptr [esi + 0x65], 0x72
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
        inc    ecx
        jb     0x2c2
        jae    0x2d2
        pop    edi
        inc    edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_0268:
        push   ebx
        arpl   word ptr gs:[ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x5c], 0x766f7250
        imul   esp, dword ptr [ebp + 0x72], 0x4e000000
        je     0x2f7
        outsd  dx, dword ptr [esi]
        jb     0x2ee
        add    byte ptr [esi + 0x57], cl
        xor    eax, dword ptr [ecx + 0x63]
        arpl   word ptr [edi + 0x75], bp
        outsb  dx, byte ptr [esi]
        je     0x2e2
        jb     0x308
        jb     0x295
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x91a  R-X
        db  053h, 056h, 057h, 055h, 08Bh, 05Ch, 024h, 01Ch, 08Bh, 054h, 024h, 018h, 08Bh, 02Bh, 03Bh, 0D5h
        db  072h, 00Eh, 0B8h, 002h, 000h, 000h, 000h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 064h, 024h, 000h
        db  08Bh, 074h, 024h, 014h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h
        db  0D1h, 049h, 02Bh, 0E9h, 04Dh, 089h, 02Bh, 03Bh, 0D5h, 072h, 00Ah, 0B8h, 002h, 000h, 000h, 000h
        db  05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh
        db  0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh, 0FDh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh
        db  0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 003h, 089h, 002h, 0FFh, 00Bh, 02Bh, 0C0h, 05Dh, 05Fh
        db  05Eh, 05Bh, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_01:
        sub    esp, 0x154
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x174]
        mov    esi, 0                        ; -> obj4:0x0000
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [esp + 0x170]
        add    eax, ebx
        mov    dword ptr [esp + 0x20], eax
        lea    ebp, [ebx + 4]
        lea    edi, [esp + 0x64]
        mov    ecx, 0xb
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 0x16c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0x64]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, 0x30                     ; -> obj4:0x0030
        lea    edi, [esp + 0x64]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi - 1], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 3], eax
        dec    edi
        mov    eax, dword ptr [esp + 0x16c]
        mov    dword ptr [ebx], 0
        cmp    byte ptr [eax], 0
        jne    loc_3_0127
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x154
        ret    
loc_3_0127:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x68]
        push   eax
        push   0x80000002
        call   sub_3_08F8
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        je     loc_3_0151
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x154
        ret    
loc_3_0151:
        mov    dword ptr [esp + 0x14], 0x40
        mov    dword ptr [esp + 0x18], 0x100
        lea    eax, [esp + 0x18]
        push   eax
        sub    edi, edi
        lea    eax, [esp + 0x68]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x20]
        push   edi
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        push   edi
        push   eax
        call   sub_3_0908
        add    esp, 0x20
        cmp    eax, edi
        jne    loc_3_01DD
        nop    
loc_3_0190:
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   ebp
        push   eax
        call   0
        add    esp, 0xc
        mov    esi, eax
        cmp    esi, 2
        je     loc_3_01DD
        add    ebp, 4
        lea    eax, [esp + 0x18]
        push   eax
        inc    dword ptr [ebx]
        lea    eax, [esp + 0x68]
        push   eax
        inc    edi
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x20]
        push   0
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        push   edi
        push   eax
        call   sub_3_0908
        add    esp, 0x20
        or     eax, eax
        je     loc_3_0190
loc_3_01DD:
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_3_0900
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x154
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 083h, 03Dh
        dd  obj1:0x0588              ; fixup
        db  000h, 074h, 015h, 0C7h, 005h
        dd  obj1:0x0588              ; fixup
        db  000h, 000h, 000h, 000h, 0A1h
        dd  obj1:0x0694              ; fixup
        db  050h, 0E8h, 0F2h, 006h, 000h, 000h, 0C3h, 090h
SECPROV_Service_02:
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_03:
        mov    eax, 5
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_06:
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_04:
        sub    eax, eax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_05:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, 0x17c                    ; -> obj1:0x017C
        mov    dword ptr [edx], 1
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edx + 8], 0x17c    ; -> obj1:0x017C
        mov    dword ptr [edx + 0xc], 2
        mov    dword ptr [edx + 0x10], 0x140 ; -> obj1:0x0140
        sub    eax, eax
        pop    edi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x0178              ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x0244              ; fixup
        db  000h, 000h, 000h, 000h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_02C0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [eax]
        push   eax
        call   SECPROV_Service_02
        add    esp, 8
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_02E0:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x1c], 4
        je     loc_3_02F8
        mov    eax, 0x32
        pop    esi
        add    esp, 4
        ret    
loc_3_02F8:
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
loc_3_0340:
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
loc_3_0360:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx], 4
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], 1
        sub    eax, eax
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0380:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [0x148]        ; -> obj1:0x0148
        cmp    dword ptr [eax], ecx
        jae    loc_3_03A0
        mov    eax, 0x6f
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_03A0:
        mov    eax, dword ptr [0x148]        ; -> obj1:0x0148
        mov    edi, dword ptr [edx]
        mov    esi, dword ptr [0x144]        ; -> obj1:0x0144
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [0x148]        ; -> obj1:0x0148
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_03D0:
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_03E0:
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_03F0:
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
        je     loc_3_0458
loc_3_0408:
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        add    dword ptr [edi], ecx
        mov    ebp, dword ptr [edx]
        cmp    ebx, ebp
        jb     loc_3_0443
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
        jmp    loc_3_044F
loc_3_0443:
        mov    dword ptr [edx + 8], 0
        mov    eax, 0xea
loc_3_044F:
        add    edx, 0xc
        dec    dword ptr [esp + 0x1c]
        jne    loc_3_0408
loc_3_0458:
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
        db  0CDh, 020h, 00Fh, 080h, 091h, 004h, 056h, 057h, 0FCh, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 074h, 024h
        db  010h, 0ACh, 0AAh, 00Ah, 0C0h, 075h, 0FAh, 05Fh, 05Eh, 0C3h, 056h, 057h, 0FCh, 08Bh, 07Ch, 024h
        db  00Ch, 08Bh, 074h, 024h, 010h, 08Ah, 007h, 03Ah, 006h, 075h, 00Bh, 047h, 046h, 00Ah, 0C0h, 075h
        db  0F4h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 0EBh, 0F6h, 057h, 08Bh, 07Ch
        db  024h, 008h, 02Bh, 0C0h, 08Bh, 0C8h, 049h, 0F2h, 0AEh, 0F7h, 0D9h, 083h, 0E9h, 002h, 08Bh, 0C1h
        db  05Fh, 0C3h, 08Bh, 044h, 024h, 004h, 086h, 0E0h, 0C3h, 0C8h, 034h, 000h, 000h, 08Dh, 05Dh, 0CCh
        db  0C7h, 003h
        dd  obj1:0x069C              ; fixup
        db  0C7h, 043h, 01Ch, 003h, 05Fh, 000h, 000h, 0C7h, 043h, 010h, 000h, 000h, 000h, 000h, 066h, 0C7h
        db  043h, 014h, 000h, 000h, 053h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 058h, 066h, 08Bh, 043h, 02Ch
        db  066h, 0A9h, 001h, 000h, 075h, 02Ch, 08Bh, 043h, 01Ch, 0C1h, 0E8h, 010h, 0A2h
        dd  obj1:0x0125              ; fixup
        db  066h, 0C7h, 005h
        dd  obj1:0x0126              ; fixup
        db  000h, 000h, 0C7h, 043h, 01Ch, 003h, 05Fh, 000h, 000h, 066h, 0C7h, 043h, 014h, 003h, 002h, 053h
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h, 058h, 0F8h, 0C9h, 0C3h, 0C6h, 005h
        dd  obj1:0x0125              ; fixup
        db  000h, 0F9h, 0EBh, 0F4h, 0C8h, 034h, 000h, 000h, 08Dh, 05Dh, 0CCh, 0C7h, 003h
        dd  obj1:0x069C              ; fixup
        db  0C7h, 043h, 01Ch, 003h, 05Fh, 000h, 000h, 0C7h, 043h, 010h, 000h, 000h, 000h, 000h, 066h, 0C7h
        db  043h, 014h, 001h, 002h, 053h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 058h, 066h, 08Bh, 043h, 02Ch
        db  066h, 0A9h, 001h, 000h, 075h, 017h, 08Bh, 043h, 01Ch, 0C1h, 0E8h, 010h, 0A2h
        dd  obj1:0x0125              ; fixup
        db  066h, 0C7h, 005h
        dd  obj1:0x0126              ; fixup
        db  000h, 002h, 0F8h, 0C9h, 0C3h, 0C6h, 005h
        dd  obj1:0x0125              ; fixup
        db  000h, 0F9h, 0EBh, 0F4h
sub_3_057D:
        enter  0x34, 0
        push   ebx
        mov    byte ptr [0x125], 0           ; -> obj1:0x0125
        lea    ebx, [ebp - 0x34]
        mov    dword ptr [ebx], 0x69c        ; -> obj1:0x069C
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
        db  0C8h, 034h, 000h, 000h, 056h, 057h, 053h, 0A1h
        dd  obj1:0x0108              ; fixup
        db  0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 00Fh, 0B6h, 005h
        dd  obj1:0x0125              ; fixup
        db  00Bh, 0C0h, 074h, 04Fh, 08Dh, 05Dh, 0CCh, 0C1h, 0E0h, 010h, 066h, 0B8h, 0FEh, 0E9h, 089h, 043h
        db  01Ch, 0A1h
        dd  obj1:0x011C              ; fixup
        db  089h, 003h, 053h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 083h, 0C4h, 004h, 08Dh, 05Dh, 0CCh, 066h
        db  08Bh, 043h, 02Ch, 066h, 0A9h, 001h, 000h, 075h, 024h, 0FFh, 035h
        dd  obj1:0x011C              ; fixup
        db  0FFh, 035h
        dd  obj1:0x0144              ; fixup
        db  0E8h, 06Eh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 00Ch, 066h, 083h, 03Dh
        dd  obj1:0x0126              ; fixup
        db  000h, 074h, 010h, 0EBh, 02Fh, 0E8h, 095h, 0FEh, 0FFh, 0FFh, 073h, 007h, 0E8h, 0F7h, 0FEh, 0FFh
        db  0FFh, 072h, 038h, 0E8h, 0B7h, 000h, 000h, 000h, 08Bh, 010h, 00Bh, 0D2h, 075h, 016h, 066h, 083h
        db  03Dh
        dd  obj1:0x0126              ; fixup
        db  000h, 075h, 007h, 0E8h, 0DBh, 0FEh, 0FFh, 0FFh, 072h, 01Ch, 0E8h, 037h, 001h, 000h, 000h, 0A1h
        dd  obj1:0x0108              ; fixup
        db  0CDh, 020h, 028h, 000h, 001h, 000h, 00Fh, 0B6h, 005h
        dd  obj1:0x0125              ; fixup
        db  05Bh, 05Fh, 05Eh, 0C9h, 0C3h, 0EBh, 0E7h, 0C3h, 057h, 056h, 08Bh, 03Dh
        dd  obj1:0x011C              ; fixup
        db  0C6h, 047h, 002h, 03Dh, 083h, 0C7h, 003h, 066h, 0C7h, 007h, 000h, 004h, 08Bh, 00Dh
        dd  obj1:0x0148              ; fixup
        db  049h, 088h, 04Fh, 002h, 083h, 0C7h, 003h, 08Bh, 035h
        dd  obj1:0x0144              ; fixup
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
        db  05Eh, 05Fh, 0C3h, 02Bh, 0C0h, 0EBh, 0F9h, 0C8h, 034h, 000h, 000h, 08Dh, 07Dh, 0CCh, 00Fh, 0B6h
        db  005h
        dd  obj1:0x0125              ; fixup
        db  0C1h, 0E0h, 010h, 066h, 0B8h, 0FFh, 0E9h, 089h, 047h, 01Ch, 0A1h
        dd  obj1:0x0120              ; fixup
        db  089h, 007h, 057h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 058h, 066h, 08Bh, 047h, 02Ch, 066h, 083h
        db  0E0h, 001h, 075h, 044h, 08Bh, 03Dh
        dd  obj1:0x0120              ; fixup
        db  0A1h
        dd  obj1:0x011C              ; fixup
        db  080h, 07Fh, 01Ch, 000h, 075h, 042h, 08Ah, 04Fh, 01Bh, 0C6h, 040h, 002h, 016h, 066h, 0C7h, 000h
        db  000h, 002h, 088h, 048h, 003h, 0BAh, 004h, 000h, 000h, 000h, 0FFh, 035h
        dd  obj1:0x0118              ; fixup
        db  0FFh, 035h
        dd  obj1:0x0120              ; fixup
        db  052h, 0FFh, 035h
        dd  obj1:0x011C              ; fixup
        db  06Ah, 017h, 0E8h, 06Fh, 000h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 0A1h
        dd  obj1:0x0120              ; fixup
        db  074h, 006h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C9h, 0C3h, 0C6h, 040h, 002h, 01Ch, 066h, 0C7h
        db  000h, 000h, 005h, 066h, 08Bh, 04Fh, 01Bh, 066h, 089h, 048h, 003h, 0BAh, 007h, 000h, 000h, 000h
        db  0EBh, 0BAh, 0C8h, 034h, 000h, 000h, 053h, 0B8h
        dd  obj1:0x01FC              ; fixup
        db  08Dh, 055h, 0CCh, 089h, 002h, 0B8h
        dd  obj1:0x013C              ; fixup
        db  089h, 042h, 004h, 066h, 0B8h, 003h, 05Fh, 089h, 042h, 01Ch, 0C6h, 042h, 010h, 000h, 066h, 0C7h
        db  042h, 018h, 001h, 000h, 066h, 0C7h, 042h, 014h, 001h, 002h, 052h, 0CDh, 020h, 001h, 000h, 08Eh
        db  004h, 05Ah, 00Fh, 0B7h, 042h, 02Ch, 024h, 001h, 00Ah, 0C0h, 075h, 000h, 05Bh, 0C9h, 0C3h, 0C8h
        db  034h, 000h, 000h, 053h, 0CDh, 020h, 0CFh, 000h, 001h, 000h, 0A3h
        dd  obj1:0x0128              ; fixup
        db  08Bh, 045h, 00Ch, 08Dh, 05Dh, 0CCh, 089h, 043h, 004h, 08Bh, 045h, 014h, 089h, 003h, 08Bh, 045h
        db  010h, 066h, 089h, 043h, 018h, 08Bh, 045h, 018h, 066h, 089h, 043h, 014h, 00Fh, 0B6h, 005h
        dd  obj1:0x0125              ; fixup
        db  0C1h, 0E0h, 010h, 0B4h, 0F2h, 08Ah, 045h, 008h, 089h, 043h, 01Ch, 053h, 0CDh, 020h, 001h, 000h
        db  08Eh, 004h, 058h, 0A1h
        dd  obj1:0x012C              ; fixup
        db  00Bh, 0C0h, 075h, 029h, 066h, 0A1h
        dd  obj1:0x0126              ; fixup
        db  066h, 083h, 0F8h, 000h, 074h, 01Dh, 0B8h, 060h, 0EAh, 000h, 000h, 0BAh, 000h, 000h, 000h, 000h
        db  056h, 0BEh
        dd  loc_3_084E               ; fixup
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h, 089h, 035h
        dd  obj1:0x012C              ; fixup
        db  05Eh, 00Fh, 0B6h, 043h, 01Ch, 05Bh, 0C9h, 0C3h
loc_3_084E:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, dword ptr [0x128]        ; -> obj1:0x0128
        cmp    eax, 0x7530
        ja     loc_3_087F
        mov    eax, 0xea60
        mov    edx, 0
        push   esi
        mov    esi, 0x84e                    ; -> loc_3_084E
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x12c], esi        ; -> obj1:0x012C
        pop    esi
        ret    
loc_3_087F:
        mov    dword ptr [0x12c], 0          ; -> obj1:0x012C
        mov    eax, 0
        push   eax
        push   eax
        push   0x89f                         ; -> loc_3_089F
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        ret    
loc_3_089F:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, dword ptr [0x128]        ; -> obj1:0x0128
        cmp    eax, 0x7530
        jbe    loc_3_08B7
        call   sub_3_057D
loc_3_08B7:
        ret    
loc_3_08B8:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 0
        jne    loc_3_08C7
        mov    ecx, 0x400
        jmp    loc_3_08CC
loc_3_08C7:
        cmp    ecx, -1
        jne    loc_3_08D0
loc_3_08CC:
        xor    eax, eax
        clc    
        ret    
loc_3_08D0:
        cmp    ecx, 0x64
        jl     loc_3_08EF
        cmp    ecx, 0x6c
        jge    loc_3_08EF
        sub    ecx, 0x64
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        call   sub_4_0038
        add    esp, 8
        clc    
        ret    
loc_3_08EF:
        mov    eax, 1
        clc    
        ret    
        db  000h, 000h
sub_3_08F8:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_3_0900:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_3_0908:
        VMMCall  VMM._RegEnumValue               ; cd2050810100
        add    byte ptr [eax], al
        pop    edx
        pop    eax
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        jmp    edx
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x58  RW-
        db  053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h
        db  073h, 05Ch, 057h, 033h, 053h, 076h, 063h, 05Ch, 055h, 073h, 065h, 072h, 073h, 05Ch, 000h, 000h
        db  05Ch, 047h, 072h, 06Fh, 075h, 070h, 073h, 000h
sub_4_0038:
        rol    byte ptr [edx], 0
        add    al, ah                        ; -> loc_3_02E0
        add    al, byte ptr [eax]
        add    byte ptr [eax + 3], al        ; -> loc_3_0340
        add    byte ptr [eax], al
        pushal 
        add    eax, dword ptr [eax]
        add    byte ptr [eax - 0x2ffffffd], al ; -> loc_3_0380
        add    eax, dword ptr [eax]
        add    al, ah                        ; -> loc_3_03E0
        add    eax, dword ptr [eax]
        add    al, ah                        ; -> loc_3_03E0
        add    ebp, dword ptr [ecx]
        push   eax
VxD__LDATA ENDS
