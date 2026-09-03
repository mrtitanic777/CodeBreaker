; CDTSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 188 instructions recovered by recursive descent

Declare_Virtual_Device CDTSD, 3, 10, CDTSD_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x178  R-X
CDTSD_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        je     loc_3_0000
        cmp    si, 4
        je     loc_3_00FC
        cmp    si, 2
        je     loc_3_010C
        call   sub_1_003D
        ret    
sub_1_003D:
        mov    word ptr [ebx + 2], 0xffff
        ret    
sub_1_0044:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0078:
        mov    edi, dword ptr [esp + 4]
        cmp    word ptr [edi + 0x68], 0
        ja     loc_1_0093
loc_1_0083:
        mov    eax, dword ptr [edi + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [edi + 0x10], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_0093:
        jmp    loc_1_0083
        db  000h, 000h, 000h, 055h, 083h, 0ECh, 00Ch, 08Bh, 0ECh, 066h, 0C7h, 045h, 000h, 007h, 000h, 089h
        db  045h, 008h, 055h, 0FFh, 015h
        dd  sub_1_0044               ; fixup
        db  08Bh, 075h, 004h, 00Fh, 0B7h, 045h, 002h, 083h, 0C4h, 010h, 05Dh, 0C3h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'CDTSD   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  CDTSD_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x60  R-X
loc_2_0000:
        mov    dword ptr [0x34], 0xc         ; -> obj2:0x0034
        mov    dword ptr [0x38], 0x44        ; -> obj2:0x0038
        push   0x28                          ; -> loc_2_0028
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        xor    eax, eax
        ret    
        db  000h, 000h, 000h
loc_2_0028:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
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
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x1bc  R-X
loc_3_0000:
        sub    esp, 4
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_00E5
        test   dword ptr [esi + 0x1c], 0x23
        je     loc_3_0021
        mov    eax, dword ptr [esi + 0x1c]
        jmp    loc_3_00E5
loc_3_0021:
        mov    edx, dword ptr [0x60]         ; -> loc_1_0060
        mov    al, 5
        call   sub_3_0194
        or     ah, ah
        je     loc_3_00F4
        bsf    eax, dword ptr [edx + 0x48]
        je     loc_3_00E9
        push   eax
        push   edx
        lea    edx, [0]                      ; -> obj4:0x0000
        add    al, 0x41
        mov    byte ptr [esp + 8], al
        mov    byte ptr [esp + 9], al
        mov    byte ptr [esp + 0xa], 0
        lea    ecx, [esp + 8]
        push   0
        push   3
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        pop    edx
        pop    eax
loc_3_0073:
        btr    dword ptr [edx + 0x48], eax
        mov    byte ptr [esi + 0x15], al
        inc    byte ptr [0x1c]               ; -> obj4:0x001C
        or     dword ptr [esi + 0x20], 0xc000
        mov    dword ptr [esi + 0x28], 0
        push   ebp
        sub    esp, 0x18
        mov    ebp, esp
        mov    dword ptr [ebp + 4], esi
        mov    dword ptr [ebp + 8], 0x78     ; -> loc_1_0078
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 0x16], al
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 0xc], eax
        xor    eax, eax
        mov    word ptr [ebp + 0x10], ax
        mov    eax, dword ptr [esi + 0x1c]
        and    eax, 0xffffffe3
        mov    dword ptr [ebp + 0x12], eax
        mov    word ptr [ebp], 5
        push   ebp
        call   sub_1_0044
        add    esp, 0x10
        mov    ebp, esp
        mov    word ptr [ebp], 6
        mov    dword ptr [ebp + 4], esi
        mov    al, byte ptr [esi + 0x15]
        mov    byte ptr [ebp + 8], al
        push   ebp
        call   sub_1_0044
        add    esp, 0x10
        pop    ebp
loc_3_00E5:
        add    esp, 4
        ret    
loc_3_00E9:
        xor    al, al
        call   sub_3_0194
        or     ah, ah
        jne    loc_3_00E5
loc_3_00F4:
        movzx  eax, al
        jmp    loc_3_0073
loc_3_00FC:
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_010B
        dec    byte ptr [0x1c]               ; -> obj4:0x001C
loc_3_010B:
        ret    
loc_3_010C:
        cmp    byte ptr [0x1c], 0            ; -> obj4:0x001C
        jne    loc_3_011B
        mov    word ptr [ebx + 2], 0xffff
loc_3_011B:
        ret    
        db  060h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 073h, 008h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch
        db  024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0CDh, 020h, 082h, 000h, 001h, 000h, 0C6h
        db  046h, 01Dh, 052h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h
        db  05Eh, 038h, 0C1h, 0E3h, 004h, 00Fh, 0B7h, 04Eh, 010h, 003h, 0D9h, 0CDh, 020h, 086h, 000h, 001h
        db  000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch
        db  00Fh, 0B7h, 043h, 016h, 00Fh, 0B7h, 053h, 018h, 0C1h, 0E2h, 004h, 003h, 0C2h, 0BFh
        dd  obj1:0x010C              ; fixup
        db  0BAh, 058h, 000h, 000h, 000h, 00Fh, 0B6h, 04Bh, 021h, 088h, 00Dh
        dd  obj1:0x0174              ; fixup
        db  0ABh, 003h, 0C2h, 0E2h, 0FBh, 061h, 0C3h
sub_3_0194:
        push   ebx
        sub    esp, 0xc
        mov    ebx, esp
        mov    dword ptr [ebx + 4], esi
        mov    byte ptr [ebx + 0xa], al
        mov    word ptr [ebx], 0x10
        push   ebx
        call   sub_1_0044
        mov    ax, word ptr [ebx + 2]
        or     ah, al
        mov    al, byte ptr [ebx + 8]
        add    esp, 0x10
        pop    ebx
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x20  RW-
        db  055h, 073h, 065h, 072h, 044h, 072h, 069h, 076h, 065h, 04Ch, 065h, 074h, 074h, 065h, 072h, 041h
        db  073h, 073h, 069h, 067h, 06Eh, 06Dh, 065h, 06Eh, 074h, 022h, 043h, 044h, 054h, 053h, 044h, 020h
VxD__LDATA ENDS
