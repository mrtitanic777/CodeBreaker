; VCOMM -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 3446 instructions recovered by recursive descent

Declare_Virtual_Device VCOMM, 1, 0, VCOMM_Control, 0002Bh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x268, 35 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x434  R-X
VCOMM_Control:
        cmp    eax, 1
        je     loc_3_0000
        cmp    eax, 0x22
        je     loc_7_0429
        cmp    eax, 2
        je     loc_3_00A7
        clc    
        ret    
        db  000h, 000h, 000h
loc_1_0020:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        bts    word ptr [edx + 0x39], 0
        jae    loc_1_0037
        or     dword ptr [edx + 0x28], eax
        jmp    loc_1_0048
loc_1_0037:
        mov    dword ptr [edx + 0x28], eax
        mov    ebx, dword ptr [edx + 0x2c]
        mov    esi, 0x4b                     ; -> loc_1_004B
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
loc_1_0048:
        pop    esi
        leave  
        ret    
loc_1_004B:
        mov    eax, edx
        call   sub_1_01D6
        jne    loc_1_00A8
        and    byte ptr [edx + 0x39], 0xfe
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    eax, dword ptr [edx + 0x20]
        mov    ebx, dword ptr [edx + 0x28]
        mov    ecx, dword ptr [edx + 0x14]
        mov    dword ptr [ebp + 0x14], edx
        movzx  edx, cx
        shr    ecx, 0x10
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x10], bx
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_1_00A8:
        ret    
        db  000h, 000h, 000h
_VCOMM_ClearCommError:
        mov    cl, 6
        jmp    sub_1_00F0
_VCOMM_GetModemStatus:
        mov    cl, 0x12
        jmp    sub_1_00F0
_VCOMM_TransmitCommChar:
        mov    cl, 3
        jmp    sub_1_00F0
_VCOMM_WriteComm:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    cl, 0xd
        jmp    sub_1_00F0
_VCOMM_ReadComm:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    cl, 0xe
        jmp    sub_1_00F0
_VCOMM_GetCommEventMask:
        mov    cl, 0xc
        jmp    sub_1_00F0
_VCOMM_EscapeCommFunction:
        mov    cl, 9
        jmp    sub_1_00F0
_VCOMM_GetCommQueueStatus:
        mov    cl, 5
        jmp    sub_1_00F0
_VCOMM_PurgeComm:
        mov    cl, 0xa
        jmp    sub_1_00F0
_VCOMM_GetLastError:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0xc]
        ret    
_VCOMM_SetCommState:
        mov    cl, 0
        jmp    sub_1_00F0
sub_1_00F0:
        mov    eax, dword ptr [esp + 4]
        call   sub_1_01D6
        jne    loc_1_0114
        movzx  ecx, cl
        cmp    dword ptr [eax + 8], ecx
        jbe    loc_1_010D
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [edx + ecx*4]
        jecxz  loc_1_010D
        jmp    ecx
loc_1_010D:
        mov    dword ptr [eax + 0xc], 0xfffffffa
loc_1_0114:
        xor    eax, eax
        ret    
        db  0C7h, 040h, 00Ch, 000h, 000h, 000h, 000h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 0C3h
sub_1_0124:
        push   esi
        push   edi
        push   ebx
        xor    esi, esi
        mov    edi, dword ptr [esp + 0x14]
        xchg   dword ptr [edi + 0x30], esi
        test   esi, esi
        je     loc_1_01B9
        xor    esi, esi
        xchg   dword ptr [edi + 0x20], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_1_0155
        xor    esi, esi
        xchg   dword ptr [edi + 0x1c], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_1_0155:
        mov    ebx, dword ptr [edi + 0x28]
        call   sub_1_01C5
        jne    loc_1_0190
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, 0x400000
        mov    ecx, 0x100
        mov    esi, 0xaaa                    ; -> loc_4_0AAA
        cmp    dword ptr [esp + 0x18], 1
        je     loc_1_017E
        mov    esi, 0xa0d                    ; -> loc_4_0A0D
loc_1_017E:
        test   byte ptr [edx + 0x39], 0x10
        je     loc_1_0188
        call   esi
        jmp    loc_1_01B9
loc_1_0188:
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        jmp    loc_1_01B9
loc_1_0190:
        mov    edx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [edx + 0x80]
        cmp    dword ptr [esp + 0x18], 1
        je     loc_1_01A7
        mov    esi, dword ptr [edx + 0x84]
loc_1_01A7:
        test   esi, esi
        je     loc_1_01B9
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        je     loc_1_01B9
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_1_01B9:
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_1_01BD:
        cmp    dword ptr [edi + 0x64], 0x534d5446
        ret    
sub_1_01C5:
        cmp    dword ptr [ebx + 0xc], 0x42434854
        jne    loc_1_01D1
        test   byte ptr [ebx], 0x10
loc_1_01D1:
        ret    
sub_1_01D2:
        mov    dword ptr [ecx + 4], eax
        ret    
sub_1_01D6:
        cmp    dword ptr [eax + 0x64], 0x534d5446
loc_1_01DD:
        ret    
loc_1_01DE:
        test   esp, esp
        ret    
        db  000h, 000h, 000h
loc_1_01E4:
        push   edx
        mov    eax, dword ptr [edx + 0x10]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        pop    edx
        mov    ecx, dword ptr [edx + 0x2c]
        mov    eax, dword ptr [edx + 0xc]
        call   sub_1_01D2
        ret    
sub_1_01FB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        btr    word ptr [edx + 0x39], 0
        jae    loc_1_0237
        cmp    dword ptr [edx + 0x64], 0x534d5446
        je     loc_1_021F
        mov    eax, dword ptr [edx + 0x28]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        jmp    loc_1_0237
loc_1_021F:
        mov    ebx, dword ptr [edx + 0x2c]
        mov    eax, 0x400000
        mov    ecx, 0x100
        mov    esi, 0x23b                    ; -> loc_1_023B
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_1_0237:
        pop    ebx
        pop    esi
        leave  
        ret    
loc_1_023B:
        mov    esi, edx
        mov    eax, dword ptr [edx + 0x28]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        push   dword ptr [esi + 0x24]
        push   dword ptr [esi + 0x10]
        push   esi
        VxDCall  BIOS.Service_0011h              ; cd2011002b00
        add    esp, 0xc
        push   0
        push   0
        push   esi
        VxDCall  BIOS.Service_0014h              ; cd2014002b00
        add    esp, 0xc
        ret    
        db  000h, 000h, 000h
        dd  VCOMM_Get_Version        ; fixup
        dd  _VCOMM_Register_Port_Driver ; fixup
        dd  _VCOMM_Acquire_Port      ; fixup
        dd  _VCOMM_Release_Port      ; fixup
        dd  _VCOMM_OpenComm          ; fixup
        dd  _VCOMM_SetCommState      ; fixup
        dd  _VCOMM_GetCommState      ; fixup
        dd  _VCOMM_SetupComm         ; fixup
        dd  _VCOMM_TransmitCommChar  ; fixup
        dd  _VCOMM_CloseComm         ; fixup
        dd  _VCOMM_GetCommQueueStatus ; fixup
        dd  _VCOMM_ClearCommError    ; fixup
        dd  _VCOMM_GetModemStatus    ; fixup
        dd  _VCOMM_GetCommProperties ; fixup
        dd  _VCOMM_EscapeCommFunction ; fixup
        dd  _VCOMM_PurgeComm         ; fixup
        dd  _VCOMM_SetCommEventMask  ; fixup
        dd  _VCOMM_GetCommEventMask  ; fixup
        dd  _VCOMM_WriteComm         ; fixup
        dd  _VCOMM_ReadComm          ; fixup
        dd  _VCOMM_EnableCommNotification ; fixup
        dd  _VCOMM_GetLastError      ; fixup
        dd  _VCOMM_Steal_Port        ; fixup
        dd  _VCOMM_SetReadCallBack   ; fixup
        dd  _VCOMM_SetWriteCallBack  ; fixup
        dd  _VCOMM_Add_Port          ; fixup
        dd  _VCOMM_GetSetCommTimeouts ; fixup
        dd  _VCOMM_SetWriteRequest   ; fixup
        dd  _VCOMM_SetReadRequest    ; fixup
        dd  _VCOMM_Dequeue_Request   ; fixup
        dd  _VCOMM_Enumerate_DevNodes ; fixup
        dd  VCOMM_Map_Win32DCB_To_Ring0 ; fixup
        dd  VCOMM_Map_Ring0DCB_To_Win32 ; fixup
        dd  _VCOMM_Get_Contention_Handler ; fixup
        dd  _VCOMM_Map_Name_To_Resource ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0002Bh                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VCOMM   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VCOMM_Control            ; DDB_Control_Proc
        dd  VCOMM_PM_API             ; DDB_V86_API_Proc
        dd  VCOMM_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x0268              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
        dd  sub_1_01BD               ; fixup
        dd  loc_1_01DD               ; fixup
        dd  loc_1_01DE               ; fixup
loc_1_0354:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    ah, 0xc
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        enter  0xc, 0
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        fmul   qword ptr [eax + eax]
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    al, dh                        ; -> loc_4_0CF0
        or     al, 0
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x20000], al  ; -> loc_4_0D04
        add    byte ptr [eax], al
        sbb    byte ptr [0x20000], cl
        add    byte ptr [eax], al
        sub    al, 0xd
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        or     eax, 0x20000
        add    byte ptr [eax], al
        push   esp
        or     eax, 0x20000
        add    byte ptr [eax], al
        push   0x200000d
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx], bh      ; -> loc_4_0D7C
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x200000d], dl ; -> loc_4_0D90
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx + 0x20000], ah ; -> loc_4_0DA4
        add    byte ptr [eax], al
        mov    eax, 0x400000d
        add    byte ptr [eax], al
        add    ah, cl                        ; -> loc_4_0DCC
        or     eax, 0x20000
        add    byte ptr [eax], al
        loopne 0x3e3
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    ah, dh                        ; -> loc_4_0DF4
        or     eax, 0x60000
        add    byte ptr [eax], al
        or     byte ptr [esi], cl
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        sbb    al, 0xe
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    esp
        push   cs
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        pop    eax
        push   cs
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        insb   byte ptr es:[edi], dx
        push   cs
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [esi], 0
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [esi + ecx + 0x20000], dl ; -> loc_4_0E94
        add    byte ptr [eax], al
        test   al, 0xe
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        mov    esp, 0x800000e
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x106  R-X
loc_2_0000:
        add    al, 0
        add    byte ptr [eax], al
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_000E:
        pop    edi
        add    byte ptr [eax], dh
loc_2_0012:
        int3   
        add    dh, ch
        add    byte ptr [eax], al
        add    byte ptr [eax + ebp*2 + 0x65], dl
        and    byte ptr [ebx + 0x4f], al
        dec    ebp
        and    eax, 0x6f702075
        jb     0x9a
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x73], ah
        jae    0x97
        outsb  dx, byte ptr [si]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        popal  
        and    byte ptr [edi + ecx*2 + 0x53], al
        and    byte ptr [ecx + 0x70], ah
        jo     0xac
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr cs:[edi + ebp*2 + 0x20], al
        jns    loc_2_00BD
        jne    0x70
        ja     0xb3
        outsb  dx, byte ptr [esi]
        je     loc_2_0075
        je     0xc6
        and    byte ptr [edx + 0x65], dh
        popal  
        jae    0xd0
        imul   esp, dword ptr [edi + 0x6e], 0x65687420
        and    byte ptr [eax + 0x6f], dh
        jb     0xdd
        and    byte ptr [edi + ebp*2 + 0x20], dh
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x3f73776f
loc_2_0075:
        add    byte ptr [eax], al
        inc    esp
        jbe    loc_2_00E4
        arpl   word ptr [ebp + 0x20], sp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x68540000
        and    byte ptr gs:[0x6f702073], ah
        jb     0x107
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x73], ah
        jae    loc_2_0104
        outsb  dx, byte ptr [si]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        popal  
        and    byte ptr [edi + ecx*2 + 0x53], al
        and    byte ptr [ecx + 0x70], ah
        jo     0x119
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr cs:[edi + ebp*2 + 0x20], al
        jns    0x12a
        jne    0xdd
loc_2_00BD:
        ja     0x120
        outsb  dx, byte ptr [esi]
        je     loc_2_00E2
        je     0x133
        and    byte ptr [edx + 0x65], dh
        popal  
        jae    0x13d
        imul   esp, dword ptr [edi + 0x6e], 0x65687420
        and    byte ptr [eax + 0x6f], dh
        jb     0x14a
        and    byte ptr [edi + ebp*2 + 0x20], dh
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x3f73776f
loc_2_00E2:
        add    byte ptr [eax], al
loc_2_00E4:
        dec    ecx
        jae    0x107
        imul   esi, dword ptr [eax + 0x6f], 0x6f74206b
        and    byte ptr [edx + 0x65], dh
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jbe    0x15b
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0x11c
        jbe    0x169
        arpl   word ptr [ebp + 0x20], sp
        aas    
loc_2_0104:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x176  R-X
loc_3_0000:
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        jb     loc_3_000D
        mov    dword ptr [4], eax            ; -> obj5:0x0004
loc_3_000D:
        call   sub_3_0048
        call   sub_3_005C
        cmp    dword ptr [0], 0              ; -> obj5:0x0000
        jne    loc_3_0038
        mov    ecx, 8
        mov    eax, 4
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_3_0044
        mov    dword ptr [0], esi            ; -> obj5:0x0000
loc_3_0038:
        mov    esi, 0x344                    ; -> obj1:0x0344
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
        clc    
loc_3_0044:
        ret    
        db  000h, 000h, 000h
sub_3_0048:
        push   0
        push   0
        VMMCall  VMM._CreateMutex                ; cd2001010100
        add    esp, 8
        mov    dword ptr [0x90], eax         ; -> obj5:0x0090
        ret    
        db  000h
sub_3_005C:
        push   0x354                         ; -> loc_1_0354
        push   0x2f4                         ; -> obj1:0x02F4
        VMMCall  VMM._Register_Win32_Services    ; cd2029010100
        add    esp, 8
        ret    
sub_3_0070:
        mov    ecx, 0x34
        mov    eax, 4
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_3_00A6
        mov    dword ptr [0x2f8], esi        ; -> obj5:0x02F8
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        cld    
        mov    esi, edx
        mov    edi, 0x2a4                    ; -> obj5:0x02A4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [0x2f4], edi        ; -> obj5:0x02F4
        mov    byte ptr [0x2fc], 0xff        ; -> obj5:0x02FC
        clc    
loc_3_00A6:
        ret    
loc_3_00A7:
        cmp    byte ptr [0x2fc], 0xff        ; -> obj5:0x02FC
        je     loc_3_00B5
        call   sub_3_0070
loc_3_00B5:
        mov    eax, dword ptr [0x400]
        mov    edx, dword ptr [0x404]
        mov    dword ptr [0x341], eax        ; -> obj5:0x0341
        mov    dword ptr [0x345], edx        ; -> obj5:0x0345
        mov    esi, dword ptr [0x2f8]        ; -> obj5:0x02F8
        test   esi, esi
        je     loc_3_00F6
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_3_00F6
loc_3_00DD:
        mov    ebx, eax
        test   byte ptr [ebx + 0x1e], 2
        jne    loc_3_00EC
        push   ebx
        call   sub_4_0F32
        pop    ecx
loc_3_00EC:
        mov    eax, ebx
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_3_00DD
loc_3_00F6:
        inc    byte ptr [0x30d]              ; -> obj5:0x030D
        clc    
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 032h, 045h, 038h, 068h, 033h, 045h, 038h, 068h, 032h, 046h
        db  038h, 068h, 033h, 046h, 038h, 068h, 052h, 04Fh, 04Fh, 054h, 05Ch, 02Ah, 050h, 04Eh, 050h, 030h
        db  035h, 030h, 030h, 05Ch, 020h, 020h, 020h, 020h, 000h, 042h, 06Fh, 06Fh, 074h, 043h, 06Fh, 06Eh
        db  066h, 069h, 067h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 014h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 0F8h, 003h, 0FFh, 003h, 000h, 000h, 000h, 003h
        db  010h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 001h, 000h, 004h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Ah, 050h, 04Eh, 050h, 030h, 035h, 030h, 030h, 000h, 048h, 061h, 072h
        db  064h, 077h, 061h, 072h, 065h, 049h, 044h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x174d  R-X
VCOMM_Get_Version:
        mov    eax, 0x100
        clc    
        ret    
_VCOMM_Register_Port_Driver:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        jecxz  loc_4_003F
        push   esi
        push   edi
        push   ebx
        push   ecx
        call   sub_4_1505
        pop    ecx
        mov    edx, dword ptr [0xb4]         ; -> obj5:0x00B4
        lea    edx, [edx + 0xc]
        test   byte ptr [0x309], 4           ; -> obj5:0x0309
        je     loc_4_0030
        lea    edx, [edx + 9]
loc_4_0030:
        push   esi
        push   ebx
        push   edi
        push   edx
        push   eax
        push   0
        call   ecx
        add    esp, 0x18
        pop    ebx
        pop    edi
        pop    esi
loc_4_003F:
        leave  
        ret    
_VCOMM_Add_Port:
        push   esi
        xor    eax, eax
        cmp    dword ptr [esp + 0xc], eax
        je     loc_4_0092
        mov    esi, dword ptr [0]            ; -> obj5:0x0000
        or     esi, esi
        jne    loc_4_006E
        mov    ecx, 8
        mov    eax, 4
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        jb     loc_4_0092
        mov    dword ptr [0], esi            ; -> obj5:0x0000
loc_4_006E:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_0092
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_0092
        mov    dword ptr [ecx + 4], eax
loc_4_0092:
        pop    esi
        ret    
_VCOMM_Acquire_Port:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        je     loc_4_012D
        test   byte ptr [ebp + 0x14], 2
        jne    loc_4_00AF
        cmp    eax, 4
        ja     loc_4_012D
loc_4_00AF:
        cmp    dword ptr [4], 0x400          ; -> obj5:0x0004
        jb     loc_4_012D
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x3b
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    ebx, -1
        je     loc_4_0131
        mov    ecx, dword ptr [ebp + 0x14]
        VxDCall  VCD.VCD_Acquire_Port_Windows_Style ; cd2006000e00
        jae    loc_4_012D
        jne    loc_4_012A
        push   eax
        test   byte ptr [ebp + 0x14], 2
        jne    loc_4_00F5
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   0xc0001
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        jmp    loc_4_0109
loc_4_00F5:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        push   0xc0003
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
loc_4_0109:
        xchg   ecx, eax
        call   sub_4_0142
        add    esp, dword ptr [esp]
        cmp    eax, 6
        pop    eax
        jne    loc_4_0126
        mov    ecx, dword ptr [ebp + 0x14]
        or     ecx, 1
        VxDCall  VCD.VCD_Acquire_Port_Windows_Style ; cd2006000e00
        jae    loc_4_012D
loc_4_0126:
        xor    eax, eax
        jmp    loc_4_012D
loc_4_012A:
        mov    eax, dword ptr [ebp + 0xc]
loc_4_012D:
        pop    edi
        pop    esi
        leave  
        ret    
loc_4_0131:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edi, esi
        VxDCall  VCD.VCD_Acquire_Port            ; cd2004000e00
        jae    loc_4_012D
        jne    loc_4_012A
        jmp    loc_4_0126
sub_4_0142:
        pushal 
        movzx  edi, word ptr [0xe]           ; -> loc_2_000E
        add    edi, dword ptr [4]            ; -> loc_2_0004
        xor    esi, esi
        mov    eax, 0x34
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
_VCOMM_Release_Port:
        push   ebp
        mov    ebp, esp
        push   ebx
        cmp    dword ptr [4], 0x400          ; -> obj5:0x0004
        jb     loc_4_018B
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0xffff
        jb     loc_4_018B
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    ebx, -1
        je     loc_4_018E
        VxDCall  VCD.VCD_Free_Port_Windows_Style ; cd2007000e00
loc_4_018B:
        pop    ebx
        leave  
        ret    
loc_4_018E:
        VxDCall  VCD.VCD_Free_Port               ; cd2005000e00
        jmp    loc_4_018B
_VCOMM_Steal_Port:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        VxDCall  VCD.VCD_Steal_Port_Windows_Style ; cd2008000e00
        pop    ebx
        pop    esi
        leave  
        ret    
        db  000h
VCOMM_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0xd
        ja     loc_4_01BC
        jmp    dword ptr [eax*4 + 0x48]      ; -> obj5:0x0048
loc_4_01BC:
        mov    word ptr [ebp + 0x1c], 0
        ret    
loc_4_01C3:
        movzx  eax, word ptr [ebp + 0x10]
        lea    edi, [0x80]                   ; -> obj5:0x0080
        test   al, 0x80
        je     loc_4_01D9
        lea    edi, [0x87]                   ; -> obj5:0x0087
        and    al, 0x7f
loc_4_01D9:
        inc    al
        mov    esi, edi
        add    edi, 3
        cmp    al, 9
        ja     loc_4_01E9
        add    al, 0x30
        stosb  byte ptr es:[edi], al
        jmp    loc_4_0201
loc_4_01E9:
        cmp    al, 0x63
        ja     loc_4_01F9
loc_4_01ED:
        mov    cl, 0xa
        div    cl
        add    ax, 0x3030
        stosw  word ptr es:[edi], ax
        jmp    loc_4_0201
loc_4_01F9:
        mov    byte ptr [edi], 0x31
        inc    edi
        sub    al, 0x64
        jmp    loc_4_01ED
loc_4_0201:
        mov    byte ptr [edi], 0
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   ebx
        push   esi
        VxDCall  BIOS.Service_0004h              ; cd2004002b00
        add    esp, 8
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        ret    
loc_4_0221:
        mov    edi, 0xc                      ; -> obj5:0x000C
        xchg   esi, eax
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    dword ptr [edi], 0x30
        movzx  eax, word ptr [esi + 1]
        mov    dword ptr [edi + 4], eax
        movzx  eax, word ptr [esi + 0xc]
        mov    dword ptr [edi + 8], eax
        movzx  eax, word ptr [esi + 0x10]
        mov    dword ptr [edi + 0xc], eax
        movzx  eax, word ptr [esi + 0x12]
        mov    dword ptr [edi + 0x10], eax
        mov    al, byte ptr [esi + 3]
        mov    byte ptr [edi + 0x16], al
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 0x17], al
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [edi + 0x18], al
        mov    al, byte ptr [esi + 0xe]
        mov    byte ptr [edi + 0x19], al
        mov    al, byte ptr [esi + 0xf]
        mov    byte ptr [edi + 0x1a], al
        mov    al, byte ptr [esi + 0x14]
        mov    byte ptr [edi + 0x1b], al
        mov    al, byte ptr [esi + 0x15]
        mov    byte ptr [edi + 0x1c], al
        mov    al, byte ptr [esi + 0x16]
        mov    byte ptr [edi + 0x1d], al
        movzx  eax, word ptr [esi + 0x17]
        mov    dword ptr [edi + 0x2c], eax
        movzx  eax, word ptr [esi + 6]
        mov    dword ptr [edi + 0x20], eax
        movzx  eax, word ptr [esi + 8]
        mov    dword ptr [edi + 0x24], eax
        movzx  eax, word ptr [esi + 0xa]
        mov    dword ptr [edi + 0x28], eax
        mov    esi, dword ptr [ebp + 0x10]
        push   -1
        push   edi
        push   esi
        VxDCall  BIOS.Service_0005h              ; cd2005002b00
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_4_02B8:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi + 6]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x1c], ax
        xchg   edi, eax
        mov    ax, 0x1c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_4_0322
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x1c], ax
        xchg   edx, eax
        mov    ax, 0x1c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edx, eax
        cmp    edx, -1
        je     loc_4_0322
        movzx  ecx, word ptr [esi + 0xa]
        movzx  eax, word ptr [esi + 0x10]
        mov    esi, dword ptr [esi + 2]
        push   eax
        push   edx
        push   ecx
        push   edi
        push   esi
        VxDCall  BIOS.Service_0007h              ; cd2007002b00
        add    esp, 0x14
        mov    word ptr [ebp + 0x1c], 0xffff
        ret    
loc_4_0322:
        mov    word ptr [ebp + 0x1c], 0
        ret    
loc_4_0329:
        mov    esi, dword ptr [ebp + 0x10]
        movzx  eax, byte ptr [ebp + 0x18]
        push   eax
        push   esi
        VxDCall  BIOS.Service_0008h              ; cd2008002b00
        add    esp, 8
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_4_0343:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        VxDCall  BIOS.Service_0009h              ; cd2009002b00
        add    esp, 4
        ret    
loc_4_0351:
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x1c]
        lea    ebx, [ebp + 0x1c]
        push   ebx
        push   0x3c                          ; -> obj5:0x003C
        push   esi
        VxDCall  BIOS.Service_000Bh              ; cd200b002b00
        add    esp, 0xc
        mov    di, word ptr [ebp + 0x1c]
        mov    dword ptr [ebp + 0x1c], edi
        mov    di, word ptr [ebp + 0x38]
        or     di, word ptr [ebp + 0x14]
        je     loc_4_03A3
        xchg   edi, eax
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_4_03A3
        mov    esi, 0x3c                     ; -> obj5:0x003C
        mov    eax, dword ptr [esi]
        mov    byte ptr [edi], al
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [edi + 1], ax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [edi + 3], ax
loc_4_03A3:
        ret    
loc_4_03A4:
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x1c]
        lea    edx, [ebp + 0x1c]
        push   edx
        push   0
        push   ecx
        push   esi
        VxDCall  BIOS.Service_000Eh              ; cd200e002b00
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 0x1c]
        mov    di, ax
        mov    dword ptr [ebp + 0x1c], edi
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        ret    
loc_4_03D0:
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   esi
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_4_03EA:
        mov    esi, dword ptr [ebp + 0x10]
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        movzx  ecx, word ptr [ebp + 0x18]
        push   eax
        push   ecx
        push   esi
        VxDCall  BIOS.Service_0010h              ; cd2010002b00
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_4_040F:
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp + 0x18]
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        push   ecx
        push   esi
        VxDCall  BIOS.Service_0011h              ; cd2011002b00
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_4_0434:
        mov    esi, dword ptr [ebp + 0x10]
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    edi, dword ptr [esi + 4]
        cmp    dword ptr [edi + 0x1c], 0
        je     loc_4_0452
        push   eax
        push   esi
        call   dword ptr [edi + 0x1c]
        add    esp, 8
loc_4_0452:
        ret    
loc_4_0453:
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 1
        jne    loc_4_0464
        lea    eax, [ebp + 4]
        jmp    loc_4_046E
loc_4_0464:
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
loc_4_046E:
        xor    edi, edi
        xchg   dword ptr [ebp + 0x1c], edi
        lea    ebx, [ebp + 0x1c]
        push   ebx
        push   ecx
        push   eax
        push   esi
        VxDCall  BIOS.Service_0012h              ; cd2012002b00
        add    esp, 0x10
        mov    di, word ptr [ebp + 0x1c]
        mov    dword ptr [ebp + 0x1c], edi
        ret    
loc_4_048B:
        and    byte ptr [ebp + 0x2c], 0xbf
        mov    esi, dword ptr [ebp + 0x10]
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    ax, 0x414
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [edi], 0
        movzx  ecx, word ptr [ebp + 0x18]
        push   edi
        push   ecx
        push   eax
        push   esi
        VxDCall  BIOS.Service_0013h              ; cd2013002b00
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_04C7
        or     byte ptr [ebp + 0x2c], 0x40
loc_4_04C7:
        mov    eax, dword ptr [edi]
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_4_04CE:
        mov    esi, dword ptr [ebp + 0x10]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [esi + 0x2c], ebx
        movzx  ecx, word ptr [ebp + 0x18]
        mov    dword ptr [esi + 0x20], ecx
        mov    edi, dword ptr [ebp]
        mov    dword ptr [esi + 0x14], edi
        movzx  edx, word ptr [ebp + 0x14]
        inc    dx
        dec    edx
        movzx  eax, word ptr [ebp + 4]
        inc    ax
        dec    eax
        xor    ebx, ebx
        jecxz  loc_4_04FE
        mov    ebx, 0x20                     ; -> loc_1_0020
loc_4_04FE:
        push   edx
        push   eax
        push   ecx
        push   ecx
        push   ebx
        push   esi
        VxDCall  BIOS.Service_0014h              ; cd2014002b00
        add    esp, 0xc
        pop    ecx
        pop    eax
        push   ecx
        push   ecx
        push   ebx
        push   eax
        push   esi
        VxDCall  BIOS.Service_0018h              ; cd2018002b00
        add    esp, 0x10
        pop    ecx
        pop    edx
        push   ecx
        push   ebx
        push   edx
        push   esi
        VxDCall  BIOS.Service_0017h              ; cd2017002b00
        add    esp, 0x10
        mov    word ptr [ebp + 0x1c], ax
        ret    
        db  000h, 000h, 000h
sub_4_0534:
        push   eax
        push   edi
        xor    eax, eax
loc_4_0538:
        mov    al, byte ptr [esi]
        mov    ah, byte ptr [edi]
        or     eax, eax
        je     loc_4_054B
        inc    edi
        inc    esi
        or     al, 0x20
        or     ah, 0x20
        cmp    al, ah
        je     loc_4_0538
loc_4_054B:
        pop    edi
        pop    eax
        ret    
_VCOMM_OpenComm:
        enter  0xc, 0
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [0x90]         ; -> obj5:0x0090
        jecxz  loc_4_0569
        push   1
        push   ecx
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
loc_4_0569:
        push   dword ptr [ebp + 8]
        call   sub_4_11E7
        pop    ecx
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp - 8], edx
        mov    edi, eax
        push   edi
        mov    al, 0x5c
        xor    ebx, ebx
        mov    esi, dword ptr [0xbc]         ; -> obj5:0x00BC
        mov    dword ptr [ebp - 0xc], esi
        mov    dword ptr [0xbc], ebx         ; -> obj5:0x00BC
loc_4_058E:
        cmp    byte ptr [edi], bl
        je     loc_4_059E
        scasb  al, byte ptr es:[edi]
        jne    loc_4_058E
        mov    byte ptr [edi - 1], bl
        mov    dword ptr [0xbc], edi         ; -> obj5:0x00BC
loc_4_059E:
        pop    edi
        push   dword ptr [ebp + 8]
        call   sub_4_1406
        pop    ecx
        mov    esi, dword ptr [0]            ; -> obj5:0x0000
        test   esi, esi
        je     loc_4_05BA
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_4_05B8:
        mov    edx, eax
loc_4_05BA:
        mov    eax, 0xffffffff
        je     loc_4_0646
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [edx + 4]
        call   sub_4_0534
        pop    esi
        je     loc_4_05DE
        mov    eax, edx
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jmp    loc_4_05B8
loc_4_05DE:
        xor    edi, edi
        xchg   dword ptr [0xb4], edi         ; -> obj5:0x00B4
        or     edi, edi
        je     loc_4_0616
        push   edx
        test   byte ptr [edi + 0x1e], 0x20
        je     loc_4_0604
        or     byte ptr [edi + 0x1e], 0x10
        push   1
        push   1
        push   dword ptr [edi]
        VxDCall  CONFIGMG.Service_0064h          ; cd2064003300
        add    esp, 0xc
loc_4_0604:
        pop    edx
        test   byte ptr [0x309], 4           ; -> obj5:0x0309
        jne    loc_4_0613
        lea    edi, [edi + 0xc]
        jmp    loc_4_0616
loc_4_0613:
        lea    edi, [edi + 0x15]
loc_4_0616:
        lea    esi, [ebp - 4]
        push   esi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   dword ptr [edx]
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_0636
        mov    ecx, edi
        jecxz  loc_4_0632
        call   sub_4_149D
loc_4_0632:
        mov    eax, dword ptr [esi]
        jmp    loc_4_0646
loc_4_0636:
        mov    dword ptr [eax + 0x64], 0x534d5446
        mov    word ptr [eax + 0x39], 0
        mov    dword ptr [eax + 0x68], edi
loc_4_0646:
        mov    edi, dword ptr [0xbc]         ; -> obj5:0x00BC
        or     edi, edi
        je     loc_4_0655
        dec    edi
        mov    bl, 0x5c
        mov    byte ptr [edi], bl
loc_4_0655:
        mov    edi, dword ptr [ebp - 0xc]
        mov    dword ptr [0xbc], edi         ; -> obj5:0x00BC
        push   dword ptr [ebp - 8]
        call   sub_4_127B
        pop    ecx
        mov    ecx, dword ptr [0x90]         ; -> obj5:0x0090
        jecxz  loc_4_067B
        push   eax
        push   ecx
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        pop    eax
loc_4_067B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
_VCOMM_GetCommState:
        mov    cl, 1
        jmp    sub_1_00F0
_VCOMM_SetupComm:
        mov    cl, 2
        jmp    sub_1_00F0
_VCOMM_CloseComm:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        call   sub_1_01BD
        jne    loc_4_072C
        or     byte ptr [edi + 0x39], 0x10
        push   3
        push   edi
        call   sub_6_01A3
        pop    ecx
        pop    ecx
        xor    esi, esi
        mov    dword ptr [edi + 0x64], esi
        xchg   dword ptr [edi + 0x84], esi
        or     esi, esi
        je     loc_4_06C2
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_06C2:
        xor    esi, esi
        xchg   dword ptr [edi + 0x80], esi
        or     esi, esi
        je     loc_4_06D4
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_06D4:
        push   0x10
        pop    ecx
        mov    edx, dword ptr [edi + 4]
        mov    edx, dword ptr [ecx + edx]
        mov    esi, dword ptr [edi + 0x68]
        push   edi
        call   edx
        pop    ecx
        mov    ecx, esi
        or     eax, eax
        jne    loc_4_06F3
        mov    dword ptr [edi + 0x64], 0x534d5446
        jmp    loc_4_072C
loc_4_06F3:
        jecxz  loc_4_072C
        call   sub_4_149D
        push   eax
        mov    eax, 0xfffffff4
        cmp    dword ptr [ecx - 4], 0x5245454b
        je     loc_4_070E
        mov    eax, 0xffffffeb
loc_4_070E:
        add    eax, ecx
        test   byte ptr [eax + 0x1e], 0x20
        je     loc_4_072B
        and    byte ptr [eax + 0x1e], 0xef
        push   0
        push   eax
        push   0x72f                         ; -> loc_4_072F
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
loc_4_072B:
        pop    eax
loc_4_072C:
        pop    edi
        pop    esi
        ret    
loc_4_072F:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0x1e], 0x10
        jne    loc_4_0748
        push   0
        push   8
        push   dword ptr [eax]
        VxDCall  CONFIGMG.Service_0064h          ; cd2064003300
        add    esp, 0xc
loc_4_0748:
        ret    
_VCOMM_GetCommProperties:
        mov    cl, 8
        jmp    sub_1_00F0
_VCOMM_EnableCommNotification:
        mov    cl, 0xf
        jmp    sub_1_00F0
_VCOMM_SetReadCallBack:
        mov    cl, 0x10
        jmp    sub_1_00F0
_VCOMM_SetWriteCallBack:
        mov    cl, 0x11
        jmp    sub_1_00F0
_VCOMM_SetCommEventMask:
        mov    cl, 0xb
        jmp    sub_1_00F0
_VCOMM_GetSetCommTimeouts:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        lea    esi, [edx + 0x6c]
        mov    ecx, 0x14
        xor    eax, eax
        cmp    dword ptr [esp + 0x14], eax
        je     loc_4_07A2
        and    byte ptr [edx + 0x39], 0xdf
        dec    eax
        cmp    eax, dword ptr [edi]
        jne    loc_4_07A0
        cmp    eax, dword ptr [edi + 4]
        jne    loc_4_07A0
        xor    eax, dword ptr [edi + 8]
        je     loc_4_07A0
        inc    eax
        je     loc_4_07A0
        or     byte ptr [edx + 0x39], 0x20
loc_4_07A0:
        xchg   edi, esi
loc_4_07A2:
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        or     al, 1
        pop    edi
        pop    esi
        ret    
_VCOMM_SetWriteRequest:
        enter  0x10, 0
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        call   sub_1_01BD
        jne    loc_4_0878
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 4], 0
        je     loc_4_082E
        mov    dword ptr [eax + 0x14], edi
        mov    esi, dword ptr [edi + 0x84]
        cmp    esi, ecx
        je     loc_4_07E5
        call   sub_4_0B0E
        jne    loc_4_0836
        je     loc_4_07FD
loc_4_07E5:
        mov    ecx, 0x34
        mov    eax, 5
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_4_0871
        mov    dword ptr [edi + 0x84], esi
loc_4_07FD:
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_4_087F
        jb     loc_4_085C
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   edi
        VxDCall  BIOS.Service_000Ah              ; cd200a002b00
        add    esp, 8
        or     eax, eax
        je     loc_4_0850
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_0843
        push   1
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
        jmp    loc_4_0850
loc_4_082E:
        xor    eax, eax
        mov    dword ptr [edi + 0xc], eax
        dec    eax
        jmp    loc_4_087A
loc_4_0836:
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_4_087F
        jb     loc_4_0871
        mov    dword ptr [ebp - 4], eax
loc_4_0843:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        call   sub_6_03FB
loc_4_0850:
        mov    dword ptr [edi + 0xc], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_4_087A
loc_4_085C:
        call   sub_4_0B0E
        jne    loc_4_0871
        xor    esi, esi
        xchg   dword ptr [edi + 0x84], esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_0871:
        mov    dword ptr [edi + 0xc], 0xfffffffc
loc_4_0878:
        xor    eax, eax
loc_4_087A:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_4_087F:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_08AB
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        xchg   edi, eax
        xchg   ebx, esi
        mov    ecx, 0xd
        push   edi
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        xchg   edi, eax
        mov    esi, ebx
        mov    dword ptr [eax + 0x30], 0x444e4153
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x20], ecx
        clc    
loc_4_08AB:
        ret    
_VCOMM_SetReadRequest:
        enter  0x10, 0
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        call   sub_1_01BD
        jne    loc_4_09CD
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 4], 0
        je     loc_4_096D
        mov    dword ptr [eax + 0x14], edi
        cmp    dword ptr [edi + 0x6c], -1
        jne    loc_4_08E6
        mov    esi, dword ptr [edi + 0x70]
        or     esi, dword ptr [edi + 0x74]
        je     loc_4_0958
loc_4_08E6:
        mov    esi, dword ptr [edi + 0x80]
        cmp    esi, ecx
        je     loc_4_08FD
        call   sub_4_0B0E
        jne    loc_4_098C
        je     loc_4_0919
loc_4_08FD:
        mov    ecx, 0x34
        mov    eax, 5
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_4_09C6
        mov    dword ptr [edi + 0x80], esi
loc_4_0919:
        lea    eax, [ebp - 0x10]
        push   eax
        push   edi
        VxDCall  BIOS.Service_000Ah              ; cd200a002b00
        add    esp, 8
        test   eax, eax
        je     loc_4_09A5
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 4]
        cmp    dword ptr [ebp - 0xc], ecx
        jae    loc_4_0958
        mov    ebx, dword ptr [ebp + 0xc]
        test   byte ptr [edi + 0x39], 0x20
        je     loc_4_094C
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_0958
        mov    dword ptr [ebx + 4], 1
loc_4_094C:
        call   sub_4_087F
        jb     loc_4_09B1
        mov    dword ptr [ebp - 4], eax
        jmp    loc_4_0974
loc_4_0958:
        mov    edx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [edx + 4]
        push   dword ptr [edx]
        push   edi
        VxDCall  BIOS.Service_0013h              ; cd2013002b00
        add    esp, 0x10
loc_4_096D:
        mov    eax, 0xffffffff
        jmp    loc_4_09A8
loc_4_0974:
        mov    edx, eax
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        call   sub_6_03FB
        push   0
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
        jmp    loc_4_09A5
loc_4_098C:
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_4_087F
        jb     loc_4_09C6
        mov    dword ptr [ebp - 4], eax
        mov    edx, eax
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        call   sub_6_03FB
loc_4_09A5:
        mov    eax, dword ptr [ebp - 4]
loc_4_09A8:
        mov    dword ptr [edi + 0xc], 0
        jmp    loc_4_09CF
loc_4_09B1:
        call   sub_4_0B0E
        jne    loc_4_09C6
        xor    esi, esi
        xchg   dword ptr [edi + 0x80], esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_09C6:
        mov    dword ptr [edi + 0xc], 0xfffffffc
loc_4_09CD:
        xor    eax, eax
loc_4_09CF:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_4_09D4:
        push   eax
        push   esi
        push   edi
        mov    edi, eax
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_0A09
loc_4_09E1:
        xor    ecx, ecx
        inc    ecx
        cmp    esi, dword ptr [edi + 0x80]
        je     loc_4_09EE
        mov    cl, 2
loc_4_09EE:
        mov    edx, eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        push   eax
        push   0
        push   ecx
        push   edx
        push   edi
        call   sub_1_0124
        add    esp, 0x10
        pop    eax
        or     eax, eax
        jne    loc_4_09E1
loc_4_0A09:
        pop    edi
        pop    esi
        pop    eax
        ret    
loc_4_0A0D:
        enter  0xc, 0
        mov    edi, edx
        call   sub_1_01BD
        jne    loc_4_0AA8
        push   0
        push   0
        push   -1
        push   edi
        VxDCall  BIOS.Service_0018h              ; cd2018002b00
        add    esp, 0x10
        lea    eax, [ebp - 0xc]
        cli    
        push   eax
        push   edi
        VxDCall  BIOS.Service_000Ah              ; cd200a002b00
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_0A4F
        push   0
        push   edi
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
loc_4_0A4F:
        sti    
        mov    esi, dword ptr [edi + 0x84]
        or     esi, esi
        je     loc_4_0A79
        call   sub_4_0B1A
        je     loc_4_0A6F
        mov    edx, dword ptr [eax + 0xc]
        sub    edx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], edx
        call   sub_4_0AF9
loc_4_0A6F:
        push   1
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
loc_4_0A79:
        test   byte ptr [edi + 0x39], 4
        je     loc_4_0AA8
        mov    esi, dword ptr [edi + 0x84]
        or     esi, esi
        je     loc_4_0A91
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        jne    loc_4_0AA8
loc_4_0A91:
        and    byte ptr [edi + 0x39], 0xfb
        push   dword ptr [edi + 0x90]
        push   0
        push   0
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
loc_4_0AA8:
        leave  
        ret    
loc_4_0AAA:
        mov    edi, edx
        call   sub_1_01BD
        jne    loc_4_0AF8
        push   0
        push   0
        push   -1
        push   edi
        VxDCall  BIOS.Service_0017h              ; cd2017002b00
        add    esp, 0x10
        mov    esi, dword ptr [edi + 0x80]
        or     esi, esi
        je     loc_4_0AF8
        call   sub_4_0B1A
        je     loc_4_0AEE
        lea    edx, [eax + 0xc]
        push   eax
        push   edx
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   edi
        VxDCall  BIOS.Service_0013h              ; cd2013002b00
        add    esp, 0x10
        pop    eax
        call   sub_4_0AF9
loc_4_0AEE:
        push   0
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
loc_4_0AF8:
        ret    
sub_4_0AF9:
        push   eax
        mov    edx, eax
        call   dword ptr [eax + 8]
        pop    eax
        push   eax
        call   sub_4_0C85
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        ret    
sub_4_0B0E:
        pushfd 
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        or     eax, eax
        ret    
sub_4_0B1A:
        pushfd 
        cli    
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        popfd  
        or     eax, eax
        ret    
loc_4_0B26:
        mov    dword ptr [edx + 0x1c], 0
        mov    edi, dword ptr [edx + 0x14]
        call   sub_1_01BD
        jne    loc_4_0B62
        cmp    dword ptr [edi + 0x88], 0
        je     loc_4_0B51
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, dword ptr [edi + 0x88]
        cmp    eax, dword ptr [edi + 0x6c]
        ja     loc_4_0B6D
loc_4_0B51:
        mov    eax, dword ptr [edi + 0x6c]
        mov    esi, 0xb26                    ; -> loc_4_0B26
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edx + 0x1c], esi
loc_4_0B62:
        ret    
loc_4_0B63:
        mov    dword ptr [edx + 0x20], 0
        mov    edi, dword ptr [edx + 0x14]
loc_4_0B6D:
        mov    eax, 1
loc_4_0B72:
        call   sub_1_01BD
        jne    loc_4_0BB4
        or     byte ptr [edi + 0x39], 2
        push   0
        push   eax
        push   edx
        push   edi
        call   sub_1_0124
        add    esp, 0x10
        ret    
loc_4_0B8B:
        mov    dword ptr [edx + 0x20], 0
        mov    edi, dword ptr [edx + 0x14]
        mov    eax, 2
        jmp    loc_4_0B72
sub_4_0B9C:
        push   esi
        mov    edx, ebx
        mov    eax, dword ptr [edi + 0x6c]
        or     eax, eax
        je     loc_4_0BB4
        mov    esi, 0xb26                    ; -> loc_4_0B26
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [ebx + 0x1c], esi
loc_4_0BB4:
        mov    eax, dword ptr [edi + 0x70]
        imul   eax, dword ptr [ebx + 4]
        add    eax, dword ptr [edi + 0x74]
        or     eax, eax
        je     loc_4_0BDB
        mov    edx, ebx
        mov    esi, 0xb63                    ; -> loc_4_0B63
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [ebx + 0x20], esi
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [ebx + 0x18], eax
loc_4_0BDB:
        pop    esi
        ret    
sub_4_0BDD:
        push   esi
        mov    eax, dword ptr [edi + 0x78]
        imul   eax, dword ptr [ebx + 4]
        add    eax, dword ptr [edi + 0x7c]
        or     eax, eax
        je     loc_4_0C05
        mov    edx, ebx
        mov    esi, 0xb8b                    ; -> loc_4_0B8B
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [ebx + 0x20], esi
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [ebx + 0x18], eax
loc_4_0C05:
        pop    esi
        ret    
sub_4_0C07:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        call   sub_1_01BD
        jne    loc_4_0C80
        mov    esi, dword ptr [edi + 0x84]
        cmp    dword ptr [ebp + 0xc], 1
        je     loc_4_0C29
        mov    esi, dword ptr [edi + 0x80]
loc_4_0C29:
        or     esi, esi
        je     loc_4_0C80
        call   sub_4_0B0E
        je     loc_4_0C80
        mov    ebx, eax
        cmp    dword ptr [ebp + 0xc], 1
        je     loc_4_0C56
        call   sub_4_0B9C
        push   ebx
        push   0x124                         ; -> sub_1_0124
        push   dword ptr [ebx + 4]
        push   edi
        VxDCall  BIOS.Service_0017h              ; cd2017002b00
        add    esp, 0x10
        jmp    loc_4_0C80
loc_4_0C56:
        call   sub_4_0BDD
        push   ebx
        push   0x124                         ; -> sub_1_0124
        push   1
        push   edi
        VxDCall  BIOS.Service_0018h              ; cd2018002b00
        add    esp, 0x10
        lea    eax, [ebx + 0xc]
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        push   edi
        VxDCall  BIOS.Service_0012h              ; cd2012002b00
        add    esp, 0x10
loc_4_0C80:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_4_0C85:
        btr    dword ptr [eax + 0x24], 0
        jae    loc_4_0CAC
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        add    ecx, edx
        shr    edx, 0xc
        shr    ecx, 0xc
        sub    ecx, edx
        inc    ecx
        push   0x40000000
        push   ecx
        push   edx
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_4_0CAC:
        ret    
_VCOMM_Dequeue_Request:
        push   ebp
        mov    ebp, esp
        leave  
        ret    
        db  000h, 000h
loc_4_0CB4:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0000
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    8
loc_4_0CC8:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0006
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0xc
loc_4_0CDC:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_003E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x14
loc_4_0CF0:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_006F
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D04:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_008B
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D18:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_009B
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D2C:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0166
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D40:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_018F
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D54:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_01A3
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D68:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0216
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D7C:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_02FA
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0D90:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_031E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0DA4:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0337
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0DB8:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_033D
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x18
loc_4_0DCC:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0393
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0DE0:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0399
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x20
loc_4_0DF4:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0433
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x20
loc_4_0E08:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_047E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x14
loc_4_0E1C:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0484
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0xc
loc_4_0E30:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_049E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0xc
loc_4_0E44:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_04A4
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0E58:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_04A5
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0xc
loc_4_0E6C:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_04B3
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x14
loc_4_0E80:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_04BB
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x14
loc_4_0E94:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_04C3
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x10
loc_4_0EA8:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_04F7
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0xc
loc_4_0EBC:
        pop    ebx
        pop    esi
        pop    edi
        call   sub_6_0533
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   edi
        push   esi
        push   ebx
        ret    0x28
sub_4_0ED0:
        push   esi
        push   ebx
        push   edi
        mov    esi, 0x2fd                    ; -> obj5:0x02FD
        mov    ebx, 0x301                    ; -> obj5:0x0301
        mov    dword ptr [ebx], 0
        push   2
        push   dword ptr [esp + 0x14]
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        test   eax, eax
        jne    loc_4_0F13
        push   eax
        push   eax
        push   2
        push   dword ptr [esi]
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        test   eax, eax
        jne    loc_4_0F13
        mov    eax, dword ptr [ebx]
        movzx  eax, word ptr [eax + 4]
        jmp    loc_4_0F15
loc_4_0F13:
        xor    eax, eax
loc_4_0F15:
        pop    edi
        pop    ebx
        pop    esi
        ret    
sub_4_0F19:
        push   0
        push   0x100
        push   esi
        push   1
        push   eax
        push   0
        push   dword ptr [ebx]
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        ret    
sub_4_0F32:
        sub    esp, 0x34
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 0x44]
        mov    edx, dword ptr [edi]
        mov    dword ptr [0x301], 1          ; -> obj5:0x0301
        push   1
        push   0x301                         ; -> obj5:0x0301
        push   0x349                         ; -> obj5:0x0349
        push   3
        push   0x34a                         ; -> obj5:0x034A
        push   0
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        or     eax, eax
        mov    ebx, edi
        jne    loc_4_109A
        mov    edx, dword ptr [edi]
        push   edx
        call   sub_4_0ED0
        pop    ecx
        test   eax, eax
        je     loc_4_109A
        cmp    byte ptr [0x349], 0           ; -> obj5:0x0349
        jne    loc_4_0FC0
        mov    esi, 0x318                    ; -> obj5:0x0318
        mov    edi, 0x408
        push   3
        pop    ecx
        cld    
        repne scasw ax, word ptr es:[edi]
        jne    loc_4_113A
        neg    ecx
        add    ecx, 0x33
        mov    byte ptr [esi + 3], cl
        push   dword ptr [esp + 0x44]
        push   esi
        call   sub_4_1292
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_101E
        jne    loc_4_113A
loc_4_0FC0:
        call   sub_4_16E6
        jb     loc_4_0FCB
        or     byte ptr [edi + 0x1e], 0x20
loc_4_0FCB:
        mov    esi, 0x312                    ; -> obj5:0x0312
        mov    edi, 0x31d                    ; -> obj5:0x031D
        push   4
        pop    ecx
        cld    
        repne scasw ax, word ptr es:[edi]
        jne    loc_4_0FFB
        neg    ecx
        add    ecx, 0x34
        mov    byte ptr [esi + 3], cl
        mov    edi, 0x341                    ; -> obj5:0x0341
        push   4
        pop    ecx
        repne scasw ax, word ptr es:[edi]
        jne    loc_4_0FF9
        mov    word ptr [edi - 2], 0
loc_4_0FF9:
        jmp    loc_4_100A
loc_4_0FFB:
        cmp    eax, 0x3220
        jne    loc_4_10BE
        mov    byte ptr [esi + 3], 0x33
loc_4_100A:
        push   dword ptr [esp + 0x44]
        push   esi
        call   sub_4_1292
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_4_10FB
loc_4_101E:
        mov    ebx, dword ptr [esp + 0x44]
        mov    eax, 0xce                     ; -> obj5:0x00CE
        call   sub_4_0F19
        cmp    byte ptr [0x349], 2           ; -> obj5:0x0349
        je     loc_4_109A
        mov    edi, esi
        mov    eax, 0x114                    ; -> obj5:0x0114
        mov    edx, dword ptr [ebx]
        call   sub_4_114D
        jne    loc_4_109A
        push   esi
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        push   edx
        push   eax
        push   edi
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        mov    ecx, eax
        inc    ecx
        pop    eax
        pop    edx
        add    eax, 4
        add    eax, ecx
        sub    eax, 0x100
        jae    loc_4_109A
        mov    dword ptr [edx], 0x2820
        add    edx, 2
        push   ecx
        push   edi
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    word ptr [edx], 0x29
        mov    eax, 0x11f                    ; -> obj5:0x011F
        call   sub_4_0F19
        mov    eax, 0xe2                     ; -> obj5:0x00E2
        call   sub_4_0F19
loc_4_109A:
        mov    eax, 0xfe                     ; -> obj5:0x00FE
        mov    edx, dword ptr [ebx]
        call   sub_4_114D
        jne    loc_4_113A
        push   0
        push   0
        push   esi
        push   dword ptr [ebx]
        VxDCall  CONFIGMG._CONFIGMG_Load_DLVxDs  ; cd202f003300
        add    esp, 0x10
        jmp    loc_4_113A
loc_4_10BE:
        mov    edi, esi
        mov    ebx, dword ptr [esp + 0x44]
        mov    eax, 0xce                     ; -> obj5:0x00CE
        mov    edx, dword ptr [ebx]
        call   sub_4_1149
        jne    loc_4_10F9
        lea    edi, [esp + 0xc]
        push   esi
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        inc    eax
        cmp    eax, 0x31
        ja     loc_4_113A
        push   eax
        push   esi
        push   edi
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    esi, edi
        jmp    loc_4_101E
loc_4_10F9:
        mov    esi, edi
loc_4_10FB:
        mov    bl, 0x35
        xor    edi, edi
        mov    byte ptr [esi + 4], 0
loc_4_1103:
        mov    byte ptr [edi + esi + 3], bl
        push   dword ptr [esp + 0x44]
        push   esi
        call   sub_4_1292
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_101E
        inc    bl
        cmp    bl, 0x39
        jbe    loc_4_1103
        mov    bl, 0x30
        or     edi, edi
        jne    loc_4_112D
        mov    byte ptr [esi + 3], 0x30
        inc    edi
loc_4_112D:
        inc    byte ptr [esi + 3]
        mov    byte ptr [esi + 5], 0
        cmp    byte ptr [esi + 3], 0x3a
        jb     loc_4_1103
loc_4_113A:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 0x34
        ret    
sub_4_1141:
        cld    
loc_4_1142:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        jne    loc_4_1142
        ret    
sub_4_1149:
        xor    ecx, ecx
        jmp    loc_4_1150
sub_4_114D:
        xor    ecx, ecx
        inc    ecx
loc_4_1150:
        push   edx
        mov    dword ptr [0x301], 0x100      ; -> obj5:0x0301
        mov    esi, 0x1a3                    ; -> obj5:0x01A3
        push   ecx
        push   0x301                         ; -> obj5:0x0301
        push   esi
        push   1
        push   eax
        push   0
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        pop    edx
        test   eax, eax
        ret    
sub_4_117A:
        mov    edi, esi
        mov    esi, dword ptr [0x305]        ; -> obj5:0x0305
        or     esi, esi
        jne    loc_4_119B
        mov    ecx, 0x18
        xor    eax, eax
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_4_11E4
        mov    dword ptr [0x305], esi        ; -> obj5:0x0305
loc_4_119B:
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_11B8
loc_4_11A3:
        push   esi
        mov    esi, dword ptr [eax]
        call   sub_4_0534
        pop    esi
        jne    loc_4_11B0
        jmp    loc_4_11E4
loc_4_11B0:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_4_11A3
loc_4_11B8:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, eax
        push   0
        push   0xd
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [esi], eax
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 0x10], ecx
        xchg   edi, esi
        xchg   edi, eax
        mov    cl, 0xd
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_4_11E4:
        mov    esi, eax
        ret    
sub_4_11E7:
        enter  0xc, 0
        push   esi
        push   edi
        push   ebx
        lea    edi, [ebp - 4]
        push   edi
        push   0x16c                         ; -> obj5:0x016C
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_1274
        push   1
        push   0x100
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_1268
        mov    esi, eax
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0xc], 0x100
        lea    ebx, [ebp - 8]
        push   eax
        push   esi
        push   ebx
        push   0
        push   dword ptr [ebp + 8]
        push   dword ptr [edi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_125C
        cmp    dword ptr [ebx], 1
        jne    loc_4_125C
        push   dword ptr [edi]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        mov    eax, esi
        mov    edx, esi
loc_4_1257:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_4_125C:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_1268:
        push   dword ptr [ebp - 4]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_4_1274:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        jmp    loc_4_1257
sub_4_127B:
        push   eax
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     loc_4_1290
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_1290:
        pop    eax
        ret    
sub_4_1292:
        enter  8, 0
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_4_11E7
        pop    ecx
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp - 8], edx
        mov    edi, eax
        mov    al, 0x5c
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], ebx
loc_4_12B1:
        cmp    byte ptr [edi], bl
        je     loc_4_12BE
        scasb  al, byte ptr es:[edi]
        jne    loc_4_12B1
        dec    edi
        mov    byte ptr [edi], bl
        mov    dword ptr [ebp - 4], edi
loc_4_12BE:
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [0x2f8]        ; -> obj5:0x02F8
        xor    eax, eax
        test   esi, esi
        je     loc_4_132C
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_132C
loc_4_12D5:
        mov    ebx, eax
        cmp    ebx, dword ptr [ebp + 0xc]
        je     loc_4_130A
        test   byte ptr [ebx + 0x1e], 2
        jne    loc_4_130A
        mov    edx, dword ptr [ebx]
        mov    eax, 0xce                     ; -> obj5:0x00CE
        call   sub_4_1149
        jne    loc_4_12F7
        call   sub_4_0534
        je     loc_4_1323
loc_4_12F7:
        mov    eax, 0xe2                     ; -> obj5:0x00E2
        call   sub_4_1149
        jne    loc_4_130A
        call   sub_4_0534
        je     loc_4_1323
loc_4_130A:
        mov    eax, ebx
        mov    esi, dword ptr [0x2f8]        ; -> obj5:0x02F8
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_4_12D5
        xor    eax, eax
        mov    dword ptr [0x30e], eax        ; -> obj5:0x030E
        jmp    loc_4_132C
loc_4_1323:
        mov    dword ptr [0x30e], ebx        ; -> obj5:0x030E
        xor    eax, eax
        inc    eax
loc_4_132C:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_1337
        mov    bl, 0x5c
        mov    byte ptr [edi], bl
loc_4_1337:
        push   dword ptr [ebp - 8]
        call   sub_4_127B
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_4_1345:
        push   esi
        push   edi
        push   eax
        push   ebx
        test   byte ptr [eax + 0x1e], 2
        jne    loc_4_1401
        and    dword ptr [0x309], 1          ; -> obj5:0x0309
        mov    ebx, eax
        mov    edx, dword ptr [ebx]
        mov    eax, 0xce                     ; -> obj5:0x00CE
        call   sub_4_1149
        jne    loc_4_1399
        call   sub_4_0534
        jne    loc_4_1399
        or     byte ptr [0x309], 2           ; -> obj5:0x0309
        mov    eax, 0xd7                     ; -> obj5:0x00D7
        call   sub_4_114D
        je     loc_4_13D2
        or     byte ptr [0x309], 4           ; -> obj5:0x0309
        mov    eax, 0xef                     ; -> obj5:0x00EF
        call   sub_4_114D
        je     loc_4_13D2
        jne    loc_4_1401
loc_4_1399:
        mov    eax, 0xe2                     ; -> obj5:0x00E2
        call   sub_4_1149
        jne    loc_4_1401
        call   sub_4_0534
        jne    loc_4_1401
        or     byte ptr [0x309], 4           ; -> obj5:0x0309
        mov    eax, 0xef                     ; -> obj5:0x00EF
        call   sub_4_114D
        je     loc_4_13D2
        and    byte ptr [0x309], 0xfb        ; -> obj5:0x0309
        mov    eax, 0xd7                     ; -> obj5:0x00D7
        call   sub_4_114D
        jne    loc_4_1401
loc_4_13D2:
        cmp    dword ptr [0xbc], 0           ; -> obj5:0x00BC
        je     loc_4_13E1
        mov    esi, dword ptr [0xbc]         ; -> obj5:0x00BC
loc_4_13E1:
        push   ebx
        call   sub_4_117A
        pop    ebx
        test   byte ptr [0x309], 4           ; -> obj5:0x0309
        jne    loc_4_13F9
        mov    dword ptr [ebx + 0xc], esi
        inc    byte ptr [ebx + 0x14]
        jmp    loc_4_13FF
loc_4_13F9:
        mov    dword ptr [ebx + 0x15], esi
        inc    byte ptr [ebx + 0x1d]
loc_4_13FF:
        xor    eax, eax
loc_4_1401:
        pop    ebx
        pop    eax
        pop    edi
        pop    esi
        ret    
sub_4_1406:
        push   esi
        push   edi
        push   ebx
        mov    dword ptr [0xb8], edi         ; -> obj5:0x00B8
        mov    esi, dword ptr [0x2f8]        ; -> obj5:0x02F8
        test   esi, esi
        je     loc_4_147F
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_147F
loc_4_1421:
        call   sub_4_1345
        je     loc_4_1432
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_4_1421
        jmp    loc_4_147F
loc_4_1432:
        mov    dword ptr [0xb4], eax         ; -> obj5:0x00B4
        mov    esi, dword ptr [eax + 0xc]
        test   byte ptr [0x309], 4           ; -> obj5:0x0309
        je     loc_4_1446
        mov    esi, dword ptr [eax + 0x15]
loc_4_1446:
        cmp    dword ptr [esi + 4], 0
        je     loc_4_145A
        push   dword ptr [esi + 0x10]
        VxDCall  BIOS.Service_0001h              ; cd2001002b00
        add    esp, 4
        jmp    loc_4_147C
loc_4_145A:
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_4_1483
        pop    esi
        jb     loc_4_147F
        test   byte ptr [0x309], 4           ; -> obj5:0x0309
        jne    loc_4_1473
        mov    esi, dword ptr [esi + 0xc]
        jmp    loc_4_1476
loc_4_1473:
        mov    esi, dword ptr [esi + 0x15]
loc_4_1476:
        mov    dword ptr [esi + 8], edx
        mov    dword ptr [esi + 0xc], eax
loc_4_147C:
        inc    dword ptr [esi + 4]
loc_4_147F:
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_4_1483:
        mov    edi, dword ptr [0x2f4]        ; -> obj5:0x02F4
        call   sub_4_1141
        mov    edx, 0x2a4                    ; -> obj5:0x02A4
        xor    eax, eax
        inc    eax
        VxDCall  APM.Service_0001h               ; cd2001002700
        ret    
sub_4_149D:
        pushal 
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_4_14A7
        dec    dword ptr [eax + 4]
loc_4_14A7:
        dec    byte ptr [ecx + 8]
        jne    loc_4_1503
        xor    eax, eax
        xchg   dword ptr [ecx + 4], eax
        test   eax, eax
        je     loc_4_14C7
        mov    esi, dword ptr [0]            ; -> obj5:0x0000
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_4_14C7:
        mov    eax, dword ptr [esp + 0x18]
        xor    ecx, ecx
        xchg   dword ptr [eax], ecx
        jecxz  loc_4_1503
        cmp    dword ptr [ecx + 4], 0
        jne    loc_4_1503
        xor    ebx, ebx
        dec    ebx
        mov    esi, dword ptr [ecx]
        mov    edi, 0x1a3                    ; -> obj5:0x01A3
        mov    edx, edi
loc_4_14E3:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x2e
        je     loc_4_14F5
        cmp    al, 0x61
        jb     loc_4_14F2
        cmp    al, 0x7a
        ja     loc_4_14F2
        and    al, 0xdf
loc_4_14F2:
        stosb  byte ptr es:[edi], al
        jmp    loc_4_14E3
loc_4_14F5:
        mov    byte ptr [edi], 0
        VxDCall  APM.Service_0002h               ; cd2002002700
        jae    loc_4_1503
        inc    dword ptr [ecx + 4]
loc_4_1503:
        popal  
        ret    
sub_4_1505:
        mov    edi, dword ptr [0xb4]         ; -> obj5:0x00B4
        or     edi, edi
        je     loc_4_1565
        mov    esi, dword ptr [edi + 0xc]
        test   byte ptr [0x309], 4           ; -> obj5:0x0309
        je     loc_4_151E
        mov    esi, dword ptr [edi + 0x15]
loc_4_151E:
        mov    dword ptr [esi + 0x10], ecx
        push   dword ptr [edi]
        call   sub_4_0ED0
        pop    ecx
        mov    edi, eax
        xor    ebx, ebx
        test   eax, eax
        je     loc_4_1556
        mov    esi, 0x301                    ; -> obj5:0x0301
        push   0
        push   0
        push   4
        push   dword ptr [0x2fd]             ; -> obj5:0x02FD
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        test   eax, eax
        jne    loc_4_1556
        mov    ebx, dword ptr [esi]
        movzx  ebx, word ptr [ebx + 2]
loc_4_1556:
        mov    esi, dword ptr [0xb4]         ; -> obj5:0x00B4
        mov    eax, dword ptr [esi]
        mov    esi, dword ptr [0xb8]         ; -> obj5:0x00B8
        ret    
loc_4_1565:
        xor    eax, eax
        ret    
_VCOMM_Enumerate_DevNodes:
        mov    eax, dword ptr [0x2f8]        ; -> obj5:0x02F8
        ret    
_VCOMM_Get_Contention_Handler:
        push   esi
        push   edi
        push   0
        push   dword ptr [esp + 0x10]
        call   sub_4_1292
        pop    ecx
        pop    ecx
        mov    eax, dword ptr [0x30e]        ; -> obj5:0x030E
        test   eax, eax
        je     loc_4_1589
        mov    eax, dword ptr [eax + 4]
loc_4_1589:
        pop    edi
        pop    esi
        ret    
_VCOMM_Map_Name_To_Resource:
        push   esi
        push   edi
        push   ebx
        push   0
        push   dword ptr [esp + 0x14]
        call   sub_4_1292
        pop    ecx
        pop    ecx
        mov    eax, dword ptr [0x30e]        ; -> obj5:0x030E
        test   eax, eax
        je     loc_4_15C7
        cmp    dword ptr [eax + 4], 0x15cc   ; -> loc_4_15CC
        je     loc_4_15C7
        mov    edi, eax
        mov    esi, dword ptr [edi]
        push   esi
        call   sub_4_0ED0
        pop    ecx
        push   eax
        push   esi
        push   dword ptr [esp + 0x18]
        push   0
        call   dword ptr [edi + 4]
        add    esp, 0x10
loc_4_15C7:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_4_15CB:
        ret    
loc_4_15CC:
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, 5
        ja     loc_4_15DC
        jmp    dword ptr [ecx*4 + 0x325]     ; -> obj5:0x0325
loc_4_15DC:
        xor    eax, eax
        ret    
loc_4_15DF:
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_1638
        test   byte ptr [ecx + 0x1e], 8
        jne    loc_4_1638
        bts    word ptr [ecx + 0x1e], 2
        jae    loc_4_1626
        cmp    dword ptr [esp + 0x14], 0
        je     loc_4_1638
        cmp    dword ptr [ecx + 0x20], 0
        je     loc_4_1638
        push   ecx
        push   0
        push   dword ptr [ecx + 0x24]
        call   dword ptr [ecx + 0x20]
        pop    ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_163A
        test   byte ptr [ecx + 0x1e], 4
        je     loc_4_1626
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [ecx + 0x2c], eax
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ecx + 0x30], eax
        or     byte ptr [ecx + 0x1e], 8
loc_4_1626:
        mov    eax, ecx
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x20], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x24], ecx
        jmp    loc_4_163A
loc_4_1638:
        xor    eax, eax
loc_4_163A:
        ret    
loc_4_163B:
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_166F
        mov    eax, dword ptr [esp + 0xc]
        cmp    dword ptr [ecx + 0x2c], eax
        je     loc_4_166B
        btr    word ptr [ecx + 0x1e], 3
        jae    loc_4_1667
        mov    edx, dword ptr [ecx + 0x2c]
        mov    dword ptr [ecx + 0x20], edx
        mov    eax, dword ptr [ecx + 0x30]
        mov    dword ptr [ecx + 0x24], eax
        push   1
        push   eax
        call   edx
        pop    ecx
        pop    ecx
        jmp    loc_4_166F
loc_4_1667:
        and    byte ptr [ecx + 0x1e], 0xfb
loc_4_166B:
        and    byte ptr [ecx + 0x1e], 0xf7
loc_4_166F:
        ret    
loc_4_1670:
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_16AC
        test   byte ptr [ecx + 0x1e], 8
        je     loc_4_16B0
        cmp    dword ptr [ecx + 0x20], 0
        je     loc_4_16AC
        push   ecx
        push   0
        push   dword ptr [ecx + 0x24]
        call   dword ptr [ecx + 0x20]
        pop    ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_16B2
        test   byte ptr [ecx + 0x1e], 8
        je     loc_4_16B2
        mov    eax, dword ptr [ecx + 0x20]
        mov    edx, dword ptr [ecx + 0x24]
        xchg   dword ptr [ecx + 0x2c], eax
        xchg   dword ptr [ecx + 0x30], edx
        mov    dword ptr [ecx + 0x20], eax
        mov    dword ptr [ecx + 0x24], edx
        jmp    loc_4_16B0
loc_4_16AC:
        xor    eax, eax
        jmp    loc_4_16B2
loc_4_16B0:
        or     al, 1
loc_4_16B2:
        ret    
loc_4_16B3:
        xor    eax, eax
        inc    eax
        ret    
loc_4_16B7:
        ret    
sub_4_16B8:
        mov    ecx, dword ptr [edx + 0xc]
        or     ecx, dword ptr [edx + 0x15]
        jne    loc_4_16D5
        mov    esi, dword ptr [0x2f8]        ; -> obj5:0x02F8
        mov    eax, edx
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        ret    
loc_4_16D5:
        mov    eax, 0x3e8
        mov    esi, 0x16b8                   ; -> sub_4_16B8
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        ret    
sub_4_16E6:
        enter  0xc, 0
        push   eax
        push   ebx
        push   ecx
        push   edx
        xor    ebx, ebx
        mov    dword ptr [ebp - 0xc], 4
        lea    ecx, [ebp - 4]
        push   ecx
        push   0x12a                         ; -> obj5:0x012A
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    ebx, eax
        jb     loc_4_1747
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   ecx
        push   eax
        push   0
        push   0
        push   0x156                         ; -> obj5:0x0156
        push   dword ptr [ebp - 4]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        cmp    ebx, eax
        jb     loc_4_1737
        cmp    dword ptr [ebp - 8], 1
loc_4_1737:
        pushfd 
        push   dword ptr [ebp - 4]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        or     eax, eax
        popfd  
loc_4_1747:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        leave  
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x357  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- dispatch table (14 entries) ----
        dd  loc_4_01C3
        dd  loc_4_0221
        dd  loc_4_02B8
        dd  loc_4_0329
        dd  loc_4_0343
        dd  loc_4_0351
        dd  loc_4_03A4
        dd  loc_4_03D0
        dd  loc_4_03EA
        dd  loc_4_040F
        dd  loc_4_0434
        dd  loc_4_0453
        dd  loc_4_048B
        dd  loc_4_04CE
        db  043h, 04Fh, 04Dh, 000h, 000h, 000h, 000h, 04Ch, 050h, 054h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 040h, 000h, 000h
        db  000h, 000h, 020h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 020h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  050h, 052h, 049h, 04Dh, 041h, 052h, 059h, 044h, 052h, 049h, 056h, 045h, 052h, 000h, 050h, 04Fh
        db  052h, 054h, 04Eh, 041h, 04Dh, 045h, 000h, 050h, 04Fh, 052h, 054h, 044h, 052h, 049h, 056h, 045h
        db  052h, 000h, 046h, 052h, 049h, 045h, 04Eh, 044h, 04Ch, 059h, 04Eh, 041h, 04Dh, 045h, 000h, 046h
        db  052h, 049h, 045h, 04Eh, 044h, 04Ch, 059h, 044h, 052h, 049h, 056h, 045h, 052h, 000h, 045h, 04Eh
        db  055h, 04Dh, 045h, 052h, 041h, 054h, 04Fh, 052h, 000h, 043h, 04Fh, 04Eh, 054h, 045h, 04Eh, 054h
        db  049h, 04Fh, 04Eh, 000h, 044h, 072h, 069h, 076h, 065h, 072h, 044h, 065h, 073h, 063h, 000h, 044h
        db  065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h, 053h, 079h, 073h, 074h, 065h, 06Dh
        db  05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h
        db  065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h, 073h, 05Ch, 056h, 078h, 044h, 05Ch
        db  056h, 043h, 04Fh, 04Dh, 04Dh, 000h, 045h, 06Eh, 061h, 062h, 06Ch, 065h, 050h, 06Fh, 077h, 065h
        db  072h, 04Dh, 061h, 06Eh, 061h, 067h, 065h, 06Dh, 065h, 06Eh, 074h, 000h, 053h, 079h, 073h, 074h
        db  065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh
        db  06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 05Ch, 043h, 06Fh, 06Dh
        db  06Dh, 041h, 06Ch, 069h, 061h, 073h, 000h, 03Eh, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 043h, 04Fh, 04Dh, 078h, 000h, 000h, 04Ch, 050h, 054h, 078h, 000h, 0F8h, 003h, 0F8h
        db  002h, 0E8h, 003h, 0E8h, 002h
        ; ---- dispatch table (6 entries) ----
        dd  loc_4_15CB
        dd  loc_4_15DF
        dd  loc_4_1670
        dd  loc_4_163B
        dd  loc_4_16B3
        dd  loc_4_16B7
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 06Fh, 072h
        db  074h, 053h, 075h, 062h, 043h, 06Ch, 061h, 073h, 073h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x5e8  R-X
sub_6_0000:
        mov    eax, 0x100
        ret    
sub_6_0006:
        push   edi
        push   dword ptr [esp + 8]
        VxDCall  BIOS.Service_0004h              ; cd2004002b00
        add    esp, 8
        cmp    eax, -0xa
        jae    loc_6_003D
        push   eax
        push   0
        push   0
        push   0x10
        push   eax
        VxDCall  BIOS.Service_000Eh              ; cd200e002b00
        add    esp, 0x10
        pop    eax
        push   eax
        push   0
        push   0
        push   0x14
        push   eax
        VxDCall  BIOS.Service_000Eh              ; cd200e002b00
        add    esp, 0x10
        pop    eax
loc_6_003D:
        ret    
sub_6_003E:
        push   dword ptr [esp + 0xc]
        push   0
        push   dword ptr [esp + 0x10]
        push   0
        push   dword ptr [esp + 0x14]
        VxDCall  BIOS.Service_0007h              ; cd2007002b00
        add    esp, 0x14
        test   eax, eax
        je     loc_6_006E
        mov    ecx, dword ptr [esp + 4]
        and    byte ptr [ecx + 0x39], 0xf7
        cmp    dword ptr [esp + 0xc], 0
        je     loc_6_006E
        or     byte ptr [ecx + 0x39], 8
loc_6_006E:
        ret    
sub_6_006F:
        push   0xb0                          ; -> obj5:0x00B0
        push   0xb0                          ; -> obj5:0x00B0
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        VxDCall  BIOS.Service_000Eh              ; cd200e002b00
        add    esp, 0x10
        ret    
sub_6_008B:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [edx], eax
        mov    eax, esp
        ret    
sub_6_009B:
        VxDCall  BIOS.Service_800Dh              ; cd200d802b00
VCOMM_Map_Ring0DCB_To_Win32:
        mov    dword ptr [edx], 0x1c
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        xor    ebx, ebx
        movzx  ebx, cx
        shr    bl, 2
        shl    bl, 1
        and    bl, 0xe
        test   cl, 1
        je     loc_6_00C6
        or     bl, 1
loc_6_00C6:
        test   ecx, 0x10000
        je     loc_6_00D1
        or     bl, 0x40
loc_6_00D1:
        test   ecx, 0x20000
        je     loc_6_00DC
        or     bl, 0x80
loc_6_00DC:
        and    bh, 0xf
        test   ecx, 0x80000
        je     loc_6_00EA
        or     bh, 0x40
loc_6_00EA:
        test   cl, 0x80
        jne    loc_6_00FC
        test   ch, 0x20
        je     loc_6_00F9
        or     bl, 0x20
        jmp    loc_6_00FC
loc_6_00F9:
        or     bl, 0x10
loc_6_00FC:
        test   cl, 2
        jne    loc_6_0119
        test   ecx, 0x100000
        je     loc_6_010E
        or     bh, 0x10
        jmp    loc_6_0119
loc_6_010E:
        or     bh, 0x20
        test   ch, 0x40
        jne    loc_6_0119
        or     bh, 0x10
loc_6_0119:
        mov    ecx, ebx
        pop    ebx
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [eax + 0xc]
        mov    word ptr [edx + 0xe], cx
        mov    ecx, dword ptr [eax + 0x10]
        mov    word ptr [edx + 0x10], cx
        mov    cl, byte ptr [eax + 0x16]
        mov    byte ptr [edx + 0x12], cl
        mov    cl, byte ptr [eax + 0x17]
        mov    byte ptr [edx + 0x13], cl
        mov    cl, byte ptr [eax + 0x18]
        mov    byte ptr [edx + 0x14], cl
        mov    cl, byte ptr [eax + 0x19]
        mov    byte ptr [edx + 0x15], cl
        mov    cl, byte ptr [eax + 0x1a]
        mov    byte ptr [edx + 0x16], cl
        mov    cl, byte ptr [eax + 0x1b]
        mov    byte ptr [edx + 0x17], cl
        mov    cl, byte ptr [eax + 0x1c]
        mov    byte ptr [edx + 0x18], cl
        mov    cl, byte ptr [eax + 0x1d]
        mov    byte ptr [edx + 0x19], cl
        mov    cx, word ptr [eax + 0x14]
        mov    word ptr [edx + 0xc], cx
        ret    
sub_6_0166:
        enter  0x30, 0
        lea    eax, [ebp - 0x30]
        push   eax
        push   dword ptr [ebp + 8]
        VxDCall  BIOS.Service_0006h              ; cd2006002b00
        add    esp, 8
        or     eax, eax
        je     loc_6_018D
        lea    eax, [ebp - 0x30]
        mov    edx, dword ptr [ebp + 0xc]
        VxDCall  BIOS.Service_0020h              ; cd2020002b00
        xor    eax, eax
        inc    eax
loc_6_018D:
        leave  
        ret    
sub_6_018F:
        push   0
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        VxDCall  BIOS.Service_001Ah              ; cd201a002b00
        add    esp, 0xc
        ret    
sub_6_01A3:
        push   esi
        mov    ecx, dword ptr [esp + 0xc]
        test   cl, 3
        je     loc_6_01E2
        test   cl, 2
        je     loc_6_01C5
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0x80]
        or     esi, esi
        je     loc_6_01C5
        call   sub_4_09D4
loc_6_01C5:
        test   dword ptr [esp + 0xc], 1
        je     loc_6_01E2
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0x84]
        or     esi, esi
        je     loc_6_01E2
        call   sub_4_09D4
loc_6_01E2:
        test   dword ptr [esp + 0xc], 4
        je     loc_6_01FB
        push   0
        push   dword ptr [esp + 0xc]
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
loc_6_01FB:
        test   dword ptr [esp + 0xc], 8
        je     loc_6_0214
        push   1
        push   dword ptr [esp + 0xc]
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
loc_6_0214:
        pop    esi
        ret    
sub_6_0216:
        mov    eax, dword ptr [esp + 4]
        lea    eax, [eax + 0x30]
        push   eax
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        VxDCall  BIOS.Service_0010h              ; cd2010002b00
        add    esp, 0xc
        test   eax, eax
        je     loc_6_0247
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x10], edx
        push   eax
        call   sub_1_01FB
        pop    ecx
        or     al, 1
loc_6_0247:
        ret    
VCOMM_Map_Win32DCB_To_Ring0:
        mov    dword ptr [edx], 0x30
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        xor    ebx, ebx
        mov    bl, cl
        and    bl, 0xe
        shl    bl, 1
        or     bl, 1
        mov    bh, ch
        and    bh, 0xf
        test   cl, 0x40
        je     loc_6_0274
        or     ebx, 0x10000
loc_6_0274:
        test   cl, 0x80
        je     loc_6_027F
        or     ebx, 0x20000
loc_6_027F:
        test   ch, 0x40
        je     loc_6_028A
        or     ebx, 0x80000
loc_6_028A:
        push   ecx
        and    ecx, 0x30
        shr    cl, 4
        mov    ecx, dword ptr [ecx*4 + 0xa4] ; -> obj5:0x00A4
        or     ebx, ecx
        pop    ecx
        and    ecx, 0x3000
        shr    ecx, 0xc
        mov    ecx, dword ptr [ecx*4 + 0x94] ; -> obj5:0x0094
        or     ebx, ecx
        mov    ecx, ebx
        pop    ebx
        mov    dword ptr [edx + 8], ecx
        movzx  ecx, word ptr [eax + 0xe]
        mov    dword ptr [edx + 0xc], ecx
        movzx  ecx, word ptr [eax + 0x10]
        mov    dword ptr [edx + 0x10], ecx
        mov    cl, byte ptr [eax + 0x12]
        mov    byte ptr [edx + 0x16], cl
        mov    cl, byte ptr [eax + 0x13]
        mov    byte ptr [edx + 0x17], cl
        mov    cl, byte ptr [eax + 0x14]
        mov    byte ptr [edx + 0x18], cl
        mov    cl, byte ptr [eax + 0x15]
        mov    byte ptr [edx + 0x19], cl
        mov    cl, byte ptr [eax + 0x16]
        mov    byte ptr [edx + 0x1a], cl
        mov    cl, byte ptr [eax + 0x17]
        mov    byte ptr [edx + 0x1b], cl
        mov    cl, byte ptr [eax + 0x18]
        mov    byte ptr [edx + 0x1c], cl
        mov    cl, byte ptr [eax + 0x19]
        mov    byte ptr [edx + 0x1d], cl
        mov    cx, word ptr [eax + 0xc]
        mov    word ptr [edx + 0x14], cx
        ret    
sub_6_02FA:
        enter  0x30, 0
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 0x30]
        VxDCall  BIOS.Service_001Fh              ; cd201f002b00
        push   0xfff
        push   edx
        push   dword ptr [ebp + 8]
        VxDCall  BIOS.Service_0005h              ; cd2005002b00
        add    esp, 0xc
        leave  
        ret    
sub_6_031E:
        push   1
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        VxDCall  BIOS.Service_001Ah              ; cd201a002b00
        add    esp, 0xc
        mov    eax, 1
        ret    
sub_6_0337:
        VxDCall  BIOS.Service_8008h              ; cd2008802b00
sub_6_033D:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    eax, dword ptr [edi + 0x10]
        and    eax, dword ptr [edi + 0x30]
        je     loc_6_035D
        push   dword ptr [esp + 0xc]
        push   dword ptr [edi + 0x10]
        push   edi
        VxDCall  BIOS.Service_0011h              ; cd2011002b00
        add    esp, 0xc
        jmp    loc_6_0391
loc_6_035D:
        or     byte ptr [edi + 0x39], 1
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [edi + 0x28], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x20], ecx
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [edi + 0x24], ecx
        mov    ecx, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ecx + 0x2c], edi
        push   ecx
        push   0x1fb                         ; -> sub_1_01FB
        push   ecx
        VxDCall  BIOS.Service_0014h              ; cd2014002b00
        add    esp, 0xc
loc_6_0391:
        pop    edi
        ret    
sub_6_0393:
        VxDCall  BIOS.Service_800Ch              ; cd200c802b00
sub_6_0399:
        enter  0x34, 0
        lea    edx, [ebp - 0x34]
        mov    eax, 0x1e4                    ; -> loc_1_01E4
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + 4], eax
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [edx + 0x24], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edx + 0x10], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [edx + 0x2c], eax
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [edx + 0x28], edi
        pop    edi
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x39], 8
        jne    loc_6_03E6
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_6_03FB
loc_6_03E6:
        lea    eax, [ebp - 0x34]
        push   dword ptr [ebp + 0x14]
        push   eax
        push   dword ptr [ebp + 8]
        VxDCall  BIOS.Service_001Bh              ; cd201b002b00
        add    esp, 0xc
        leave  
        ret    
sub_6_03FB:
        bts    dword ptr [edx + 0x24], 0
        jb     loc_6_0432
        push   edx
        push   eax
        mov    dword ptr [edx + 4], ecx
        add    ecx, eax
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0x40000000
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        test   eax, eax
        pop    ecx
        pop    edx
        je     loc_6_0432
        and    ecx, 0xfff
        add    eax, ecx
        mov    dword ptr [edx], eax
loc_6_0432:
        ret    
sub_6_0433:
        enter  0x34, 0
        lea    edx, [ebp - 0x34]
        mov    eax, 0x1e4                    ; -> loc_1_01E4
        mov    dword ptr [edx + 8], eax
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [edx + 0x24], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edx + 0x10], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [edx + 0x2c], eax
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [edx + 0x28], edi
        pop    edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + 4], eax
        push   dword ptr [ebp + 0x14]
        push   edx
        push   dword ptr [ebp + 8]
        VxDCall  BIOS.Service_001Ch              ; cd201c002b00
        add    esp, 0xc
        leave  
        ret    
sub_6_047E:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
sub_6_0484:
        push   0
        push   0
        push   6
        push   dword ptr [esp + 0x10]
        VxDCall  BIOS.Service_000Eh              ; cd200e002b00
        add    esp, 0x10
        VxDCall  BIOS.Service_8009h              ; cd2009802b00
        ret    
sub_6_049E:
        VxDCall  BIOS.Service_8015h              ; cd2015802b00
sub_6_04A4:
        ret    
sub_6_04A5:
        push   0
        push   dword ptr [esp + 8]
        call   sub_4_1292
        pop    ecx
        pop    ecx
        ret    
sub_6_04B3:
        mov    cl, 0x13
        jmp    sub_1_00F0
        db  0C3h
sub_6_04BB:
        mov    cl, 0x14
        jmp    sub_1_00F0
        db  0C3h
sub_6_04C3:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        xor    edi, edi
        mov    dword ptr [esi], edi
        mov    cl, 0x15
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   sub_1_00F0
        pop    ecx
        pop    ecx
        cmp    dword ptr [esi], edi
        jne    loc_6_04F4
        mov    edi, dword ptr [esp + 0xc]
        btr    word ptr [edi + 0x39], 1
        jae    loc_6_04F4
        mov    dword ptr [esi], 0x461
loc_6_04F4:
        pop    edi
        pop    esi
        ret    
sub_6_04F7:
        push   esi
        push   edi
        xor    eax, eax
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [ecx + 0x64], 0x534d5446
        jne    loc_6_052F
        mov    esi, dword ptr [ecx + 0x84]
        or     esi, esi
        je     loc_6_0530
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_6_0530
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ecx + 0x90], edi
        or     byte ptr [ecx + 0x39], 4
        xor    eax, eax
        inc    eax
        jmp    loc_6_0530
loc_6_052F:
        dec    eax
loc_6_0530:
        pop    edi
        pop    esi
        ret    
sub_6_0533:
        push   esi
        push   edi
        push   0
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_6_05A9
        mov    esi, eax
        mov    ecx, dword ptr [esp + 0x24]
        xor    eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esi], eax
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 8], ecx
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [esi + 0xc], edi
        mov    cl, 0x16
        push   esi
        push   0x5ac                         ; -> loc_6_05AC
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        call   sub_1_00F0
        add    esp, 0x20
        test   eax, eax
        je     loc_6_059B
        cmp    eax, -1
        jne    loc_6_05A9
loc_6_059B:
        push   eax
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    eax
loc_6_05A9:
        pop    edi
        pop    esi
        ret    
loc_6_05AC:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    edi, dword ptr [edx + 0xc]
        mov    esi, 0x5cc                    ; -> loc_6_05CC
        VMMCall  VMM.Schedule_Thread_Event       ; cd20fb000100
        mov    eax, dword ptr [edx + 8]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        pop    edi
        pop    esi
        ret    
loc_6_05CC:
        mov    eax, dword ptr [edx + 0x10]
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x4a8  R-X
        db  056h, 057h, 053h, 0C7h, 005h
        dd  obj5:0x0301              ; fixup
        db  000h, 001h, 000h, 000h, 06Ah, 000h, 068h
        dd  obj5:0x0301              ; fixup
        db  068h
        dd  obj5:0x01A3              ; fixup
        db  06Ah, 001h, 068h
        dd  obj5:0x00E2              ; fixup
        db  06Ah, 000h, 0FFh, 036h, 0CDh, 020h, 03Eh, 000h, 033h, 000h, 083h, 0C4h, 01Ch, 00Bh, 0C0h, 075h
        db  027h, 0A1h
        dd  obj5:0x0301              ; fixup
        db  005h, 00Ch, 000h, 000h, 000h, 0A3h
        dd  obj5:0x0197              ; fixup
        db  0FFh, 074h, 024h, 010h, 068h
        dd  obj5:0x0197              ; fixup
        db  0FFh, 074h, 024h, 01Ch, 0CDh, 020h, 058h, 000h, 033h, 000h, 083h, 0C4h, 00Ch, 0EBh, 053h, 0C7h
        db  005h
        dd  obj5:0x0301              ; fixup
        db  000h, 001h, 000h, 000h, 06Ah, 000h, 068h
        dd  obj5:0x0301              ; fixup
        db  068h
        dd  obj5:0x01A3              ; fixup
        db  06Ah, 001h, 068h
        dd  obj5:0x00CE              ; fixup
        db  06Ah, 000h, 0FFh, 036h, 0CDh, 020h, 03Eh, 000h, 033h, 000h, 083h, 0C4h, 01Ch, 00Bh, 0C0h, 075h
        db  025h, 0A1h
        dd  obj5:0x0301              ; fixup
        db  005h, 00Ch, 000h, 000h, 000h, 0A3h
        dd  obj5:0x0197              ; fixup
        db  0FFh, 074h, 024h, 010h, 068h
        dd  obj5:0x0197              ; fixup
        db  0FFh, 074h, 024h, 01Ch, 0CDh, 020h, 058h, 000h, 033h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 02Ah
        db  0F9h, 074h, 001h, 0F8h, 05Bh, 05Fh, 05Eh, 0C3h
sub_7_00B6:
        sub    esp, 4
        push   esi
        xor    eax, eax
        mov    ecx, dword ptr [edi + 4]
        jecxz  loc_7_0101
        mov    edi, dword ptr [ecx + 4]
        lea    eax, [esp + 4]
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Global_State ; cd2057003300
        add    esp, 8
        mov    eax, 7
        test   dword ptr [esp + 4], 1
        je     loc_7_0101
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0x10001034
        movzx  ecx, word ptr [0x12]          ; -> loc_2_0012
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
loc_7_0101:
        pop    esi
        add    esp, 4
        cmp    eax, 7
        ret    
loc_7_0109:
        sub    esp, 0x14
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 0x24]
        cmp    eax, 1
        jne    loc_7_025A
        mov    ebx, dword ptr [esp + 0x2c]
        mov    eax, 0x109                    ; -> obj5:0x0109
        mov    edx, ebx
        call   sub_4_114D
        mov    edx, 0x15cc                   ; -> loc_4_15CC
        jne    loc_7_01A3
        cmp    byte ptr [esi], 0x2a
        jne    loc_7_0171
        mov    edi, esi
        inc    edi
        mov    cl, 8
loc_7_013D:
        mov    al, byte ptr [edi]
        or     al, al
        je     loc_7_0154
        cmp    al, 0x61
        jb     loc_7_014F
        cmp    al, 0x7a
        ja     loc_7_014F
        and    al, 0xdf
        mov    byte ptr [edi], al
loc_7_014F:
        inc    edi
        dec    cl
        jne    loc_7_013D
loc_7_0154:
        or     cl, cl
        je     loc_7_0160
        mov    byte ptr [edi], 0x20
        inc    edi
        dec    cl
        jne    loc_7_0154
loc_7_0160:
        mov    edi, esi
        inc    edi
        xor    eax, eax
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        jecxz  loc_7_0171
        mov    eax, ecx
        jmp    loc_7_0188
loc_7_0171:
        mov    edx, esi
        xor    eax, eax
        inc    eax
        VxDCall  APM.Service_0001h               ; cd2001002700
        jae    loc_7_0185
        mov    eax, 0x15cc                   ; -> loc_4_15CC
        jmp    loc_7_01A1
loc_7_0185:
        mov    eax, dword ptr [edx + 5]
loc_7_0188:
        mov    ecx, dword ptr [eax + 0x18]
        mov    edx, 0x15cc                   ; -> loc_4_15CC
        jecxz  loc_7_01A3
        push   edx
        xor    eax, eax
        mov    al, 0x30
        call   ecx
        pop    edx
        jb     loc_7_01A3
        cmp    eax, 0x30
        je     loc_7_01A3
loc_7_01A1:
        mov    edx, eax
loc_7_01A3:
        mov    esi, dword ptr [0x2f8]        ; -> obj5:0x02F8
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_7_041E
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, eax
        xor    eax, eax
        mov    edi, esi
        mov    ecx, 0x34
        rep stosb byte ptr es:[edi], al
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi], ebx
        mov    dword ptr [esi + 8], 0x5245454b
        push   0
        push   esi
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Set_Device_Driver_Private_DWord ; cd2049003300
        add    esp, 0xc
        cmp    byte ptr [0x30d], 0           ; -> obj5:0x030D
        je     loc_7_01F1
        push   esi
        call   sub_4_0F32
        pop    ecx
loc_7_01F1:
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_4_0ED0
        pop    ecx
        mov    ebx, eax
        test   eax, eax
        je     loc_7_0247
        push   0
        push   0
        push   4
        push   dword ptr [0x2fd]             ; -> obj5:0x02FD
        push   0x301                         ; -> obj5:0x0301
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        xor    ecx, ecx
        test   eax, eax
        jne    loc_7_022A
        mov    ecx, dword ptr [0x301]        ; -> obj5:0x0301
        movzx  ecx, word ptr [ecx + 2]
loc_7_022A:
        push   ecx
        push   ebx
        push   edi
        push   4
        call   dword ptr [esi + 4]
        add    esp, 0x10
        test   eax, eax
        jne    loc_7_0247
        mov    eax, 0x29
        or     byte ptr [esi + 0x1e], 2
        jmp    loc_7_0417
loc_7_0247:
        push   0x8000
        push   0
        call   0
        pop    ecx
        pop    ecx
        jmp    loc_7_0415
loc_7_025A:
        mov    esi, dword ptr [esp + 0x30]
        test   esi, esi
        je     loc_7_0424
        cmp    eax, 4
        je     loc_7_0270
        cmp    eax, 2
        jne    loc_7_02A3
loc_7_0270:
        or     byte ptr [esi + 0x1e], 2
        push   0x8004
        push   0
        call   0
        pop    ecx
        pop    ecx
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_4_0ED0
        pop    ecx
        test   eax, eax
        je     loc_7_0297
        push   eax
        push   5
        call   dword ptr [esi + 4]
        pop    ecx
        pop    ecx
loc_7_0297:
        mov    edx, esi
        call   sub_4_16B8
        jmp    loc_7_0415
loc_7_02A3:
        cmp    eax, 3
        je     loc_7_03AA
        cmp    eax, 9
        je     loc_7_03F7
        cmp    eax, 0xa
        je     loc_7_0407
        cmp    eax, 8
        je     loc_7_02C9
        jne    loc_7_0424
loc_7_02C9:
        mov    ebx, dword ptr [esp + 0x28]
        and    ebx, 0x7fffffff
        cmp    ebx, 1
        je     loc_7_02E1
        cmp    ebx, 0
        jne    loc_7_0424
loc_7_02E1:
        mov    edx, dword ptr [esp + 0x2c]
        mov    dword ptr [0x301], 1          ; -> obj5:0x0301
        push   1
        push   0x301                         ; -> obj5:0x0301
        push   0x349                         ; -> obj5:0x0349
        push   3
        push   0x34a                         ; -> obj5:0x034A
        push   0
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        cmp    byte ptr [0x349], 0           ; -> obj5:0x0349
        je     loc_7_0424
        mov    eax, dword ptr [esi + 0xc]
        or     eax, dword ptr [esi + 0x15]
        je     loc_7_0424
        lea    eax, [esp + 0x18]
        push   eax
        VxDCall  VPEND.Service_0018h             ; cd2018002600
        add    esp, 4
        cmp    eax, 0
        jne    loc_7_0424
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_7_0424
        lea    eax, [esp + 0x14]
        push   eax
        VxDCall  VPEND.Service_000Eh             ; cd200e002600
        add    esp, 4
        cmp    eax, 0
        jne    loc_7_0424
        mov    dword ptr [esp + 0xc], 0
        lea    eax, [esp + 0xc]
        lea    ecx, [esp + 0x10]
        push   0
        push   0
        push   eax
        push   ecx
        push   dword ptr [esi]
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044003300
        add    esp, 0x14
        mov    eax, 5
        cmp    dword ptr [esp + 0x10], 8
        jne    loc_7_0392
        mov    eax, 7
loc_7_0392:
        cmp    ebx, 0
        jne    loc_7_0398
        dec    eax
loc_7_0398:
        bt     dword ptr [esp + 0x14], eax
        jb     loc_7_0424
        mov    eax, 0x13
        jmp    loc_7_0417
loc_7_03AA:
        mov    dword ptr [esp + 0x1c], 2
        or     byte ptr [esp + 0x1c], 1
        push   0x8001
        push   1
        call   0
        pop    ecx
        pop    ecx
        jb     loc_7_03E3
        mov    ecx, dword ptr [esi + 0xc]
        or     ecx, dword ptr [esi + 0x15]
        je     loc_7_0415
        lea    edi, [esi + 0x15]
        call   sub_7_00B6
        je     loc_7_03E3
        lea    edi, [esi + 0xc]
        call   sub_7_00B6
        jne    loc_7_0415
loc_7_03E3:
        push   0x8002
        push   0
        call   0
        pop    ecx
        pop    ecx
        xor    eax, eax
        mov    al, 0x17
        jmp    loc_7_0417
loc_7_03F7:
        push   0x8002
        push   0
        call   0
        pop    ecx
        pop    ecx
        jmp    loc_7_0415
loc_7_0407:
        push   0x8003
        push   0
        call   0
        pop    ecx
        pop    ecx
loc_7_0415:
        xor    eax, eax
loc_7_0417:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
loc_7_041E:
        xor    eax, eax
        mov    al, 2
        jmp    loc_7_0417
loc_7_0424:
        xor    eax, eax
        inc    eax
        jmp    loc_7_0417
loc_7_0429:
        cmp    edx, 1
        mov    eax, 0x13
        jne    loc_7_04A6
        cmp    byte ptr [0x2fc], 0xff        ; -> obj5:0x02FC
        je     loc_7_0448
        call   sub_3_0070
        mov    eax, 0x13
        jb     loc_7_04A6
loc_7_0448:
        push   3
        push   0
        push   0x109                         ; -> loc_7_0109
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
        call   sub_4_16E6
        jb     loc_7_04A6
        mov    dword ptr [0x301], 1          ; -> obj5:0x0301
        push   eax
        push   1
        push   0x301                         ; -> obj5:0x0301
        push   0x349                         ; -> obj5:0x0349
        push   3
        push   0x34a                         ; -> obj5:0x034A
        push   0
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        pop    eax
        cmp    byte ptr [0x349], 0           ; -> obj5:0x0349
        je     loc_7_04A6
        push   eax
        push   0
        push   8
        push   ebx
        VxDCall  CONFIGMG.Service_0064h          ; cd2064003300
        add    esp, 0xc
        pop    eax
loc_7_04A6:
        stc    
        ret    
VxD__LTEXT ENDS
