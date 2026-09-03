; VCOND -- reconstructed 32-bit disassembly (VxD / LE format)
; 9 objects, 7569 instructions recovered by recursive descent

Declare_Virtual_Device VCOND, 1, 0, VCOND_Control, 00038h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x3c, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xa0  R-X
VCOND_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x19
        je     loc_1_0017
        clc    
        ret    
VCOND_Get_Version:
        mov    eax, 0x100
        clc    
        ret    
loc_1_0017:
        push   ebx
        call   sub_3_1D62
        clc    
        ret    
        db  000h
sub_1_0020:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_1_0028:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_1_0030:
        pop    edx
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        jmp    edx
        db  000h, 000h
        dd  VCOND_Get_Version        ; fixup
        dd  VCOND_Launch_ConApp_Inherited ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00038h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VCOND   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VCOND_Control            ; DDB_Control_Proc
        dd  VCOND_V86_API            ; DDB_V86_API_Proc
        dd  VCOND_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x003C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x70  R-X
loc_2_0000:
        mov    ecx, 1
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jb     loc_2_0060
        mov    dword ptr [0x1de], eax        ; -> obj4:0x01DE
        mov    esi, 0x2df                    ; -> loc_3_02DF
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_2_0065
        mov    dword ptr [0x1b0], eax        ; -> obj4:0x01B0
        push   0                             ; -> obj4:0x0000
        push   0x44                          ; -> obj1:0x0044
        VMMCall  VMM._Register_Win32_Services    ; cd2029010100
        add    esp, 8
        call   sub_3_160C
        or     eax, eax
        je     loc_2_006A
        mov    dword ptr [0x324], 0          ; -> obj4:0x0324
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x1e6], eax        ; -> loc_4_01E6
        mov    al, 1
        clc    
loc_2_005F:
        ret    
loc_2_0060:
        xor    eax, eax
        stc    
        jmp    loc_2_005F
loc_2_0065:
        xor    eax, eax
        stc    
        jmp    loc_2_005F
loc_2_006A:
        stc    
        jmp    loc_2_005F
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4ca4  R-X
VCOND_V86_API:
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 0x202
        jne    loc_3_0014
        call   sub_3_1078
        jmp    loc_3_0116
loc_3_0014:
        cmp    ax, 0x205
        jne    loc_3_0024
        call   sub_3_1030
        jmp    loc_3_0116
loc_3_0024:
        cmp    ax, 0x206
        jne    loc_3_0034
        call   sub_3_1051
        jmp    loc_3_0116
loc_3_0034:
        cmp    ax, 0x204
        jne    loc_3_0044
        call   sub_3_1026
        jmp    loc_3_0116
loc_3_0044:
        cmp    ax, 0x207
        jne    loc_3_0054
        call   sub_3_108B
        jmp    loc_3_0116
loc_3_0054:
        cmp    ax, 0x208
        jne    loc_3_0064
        call   sub_3_1092
        jmp    loc_3_0116
loc_3_0064:
        cmp    ax, 0x209
        jne    loc_3_0074
        call   sub_3_0F72
        jmp    loc_3_0116
loc_3_0074:
        cmp    ax, 0x20a
        jne    loc_3_0084
        call   sub_3_109F
        jmp    loc_3_0116
loc_3_0084:
        cmp    ax, 0x20b
        jne    loc_3_0094
        call   sub_3_10AA
        jmp    loc_3_0116
loc_3_0094:
        cmp    ax, 0x203
        jne    loc_3_00A1
        call   sub_3_10B4
        jmp    loc_3_0116
loc_3_00A1:
        cmp    ax, 0x20d
        jne    loc_3_00AE
        call   sub_3_10D1
        jmp    loc_3_0116
loc_3_00AE:
        cmp    ax, 0x20e
        jne    loc_3_00BB
        call   sub_3_10D8
        jmp    loc_3_0116
loc_3_00BB:
        cmp    ax, 0x20f
        jne    loc_3_00C8
        call   sub_3_10FD
        jmp    loc_3_0116
loc_3_00C8:
        cmp    ax, 0x210
        jne    loc_3_00D5
        call   sub_3_10F3
        jmp    loc_3_0116
loc_3_00D5:
        cmp    ax, 0x401
        jne    loc_3_00E2
        call   sub_3_11A3
        jmp    loc_3_0117
loc_3_00E2:
        cmp    ax, 0x402
        jne    loc_3_00EF
        call   sub_3_1206
        jmp    loc_3_0117
loc_3_00EF:
        cmp    ax, 0x403
        jne    loc_3_00FC
        call   sub_3_1214
        jmp    loc_3_0117
loc_3_00FC:
        cmp    ax, 0x404
        jne    loc_3_0109
        call   sub_3_123C
        jmp    loc_3_0117
loc_3_0109:
        cmp    ax, 0x405
        jne    loc_3_0116
        call   sub_3_11E7
        jmp    loc_3_0117
loc_3_0116:
        ret    
loc_3_0117:
        mov    word ptr [ebp + 0x1c], ax
        mov    cx, word ptr [ebp + 0x2c]
        shr    ecx, 1
        shl    eax, 1
        shl    ecx, 1
        mov    word ptr [ebp + 0x2c], cx
        ret    
VCOND_PM_API:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_3_019E
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 0x304
        jne    loc_3_0143
        call   sub_3_0EC6
        jmp    loc_3_019E
loc_3_0143:
        cmp    ax, 0x305
        jne    loc_3_0150
        call   sub_3_0EDE
        jmp    loc_3_019E
loc_3_0150:
        cmp    ax, 0x306
        jne    loc_3_015D
        call   sub_3_0EEB
        jmp    loc_3_019E
loc_3_015D:
        cmp    ax, 0x307
        jne    loc_3_016A
        call   sub_3_0F0D
        jmp    loc_3_019E
loc_3_016A:
        cmp    ax, 0x308
        jne    loc_3_0177
        call   sub_3_0F1C
        jmp    loc_3_019E
loc_3_0177:
        cmp    ax, 0x301
        jne    loc_3_0184
        call   sub_3_0EA4
        jmp    loc_3_019E
loc_3_0184:
        cmp    ax, 0x302
        jne    loc_3_0191
        call   sub_3_0EAE
        jmp    loc_3_019E
loc_3_0191:
        cmp    ax, 0x303
        jne    loc_3_019E
        call   sub_3_0EBD
        jmp    loc_3_019E
loc_3_019E:
        ret    
VCOND_Launch_ConApp_Inherited:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   esi
        push   edi
        push   ebx
        call   sub_3_173E
        mov    ebx, 0
        or     eax, eax
        jne    loc_3_01BB
        mov    ebx, 0xffffffff
loc_3_01BB:
        mov    eax, dword ptr [ebp + 0x2c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x1c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x10]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x18]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x14]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 4]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x28]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x24]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x3c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x38]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x40]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [ebp + 0x44]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ah, bl
        mov    al, 0x98
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, 3
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    edx, dword ptr [eax + 2]
        mov    cx, word ptr [ebp + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x30]
        mov    dword ptr [eax + 2], ecx
        ror    edx, 0x10
        mov    eax, edx
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        shr    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ecx, dword ptr [0x70]         ; -> obj1:0x0070
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049000100
        sub    word ptr [ebp + 0x30], 0x40
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cld    
        mov    edi, eax
        mov    esi, dword ptr [0x1ea]        ; -> loc_4_01EA
        xor    ecx, ecx
loc_3_02AE:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        inc    ecx
        or     al, al
        je     loc_3_02B7
        jmp    loc_3_02AE
loc_3_02B7:
        mov    ax, word ptr [ebp + 0x30]
        mov    word ptr [ebp + 0x14], ax
        mov    ax, word ptr [ebp + 0x34]
        mov    word ptr [ebp + 0x3c], ax
        mov    eax, dword ptr [0x1b0]        ; -> obj4:0x01B0
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        shr    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        ret    
loc_3_02DF:
        add    word ptr [ebp + 0x30], 0x40
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    bx, ax
        shl    ebx, 0x10
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    bx, ax
        ror    ebx, 0x10
        add    word ptr [ebp + 0x30], 4
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x44], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x40], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x38], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x3c], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x24], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x28], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 8], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 4], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x14], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x18], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x10], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    word ptr [ebp + 0x1c], ax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    dx, word ptr [ebp + 0x2c]
        and    dx, 1
        and    ax, 0xfffe
        or     ax, dx
        mov    word ptr [ebp + 0x2c], ax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [eax + 2], ebx
        ret    
loc_3_03AA:
        pushal 
        push   ebx
        call   sub_3_4408
        popal  
        jmp    loc_4_01E2
loc_3_03B8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_07DC
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_3_03CC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_081D
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_3_03E0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0842
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_03F4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0883
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_0408:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_085B
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_041C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_08AB
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x1c
loc_3_0430:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_08D6
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_0444:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_08FE
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_0458:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0926
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x1c
loc_3_046C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0951
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_0480:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_09A1
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_0494:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_09C0
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_04A8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_09DF
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_04BC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A01
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_04D0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A23
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_04E4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A45
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_04F8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A64
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_050C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A8C
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_0520:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0AB1
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_3_0534:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0AD6
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_0548:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0AFC
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_055C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0B21
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_0570:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0B34
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_0584:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0B53
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_3_0598:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0BC4
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_05AC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0979
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_05C0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C05
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_05D4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C24
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_05E8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C46
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_3_05FC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0BE3
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_0610:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C4E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_0624:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C70
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_3_0638:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C8A
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x20
loc_3_064C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0CB8
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_3_0660:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_07E2
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_3_0674:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0CDD
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_3_0688:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D17
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_069C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D44
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_3_06B0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D72
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_06C4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D7D
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_06D8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D88
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_06EC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0DB0
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_0700:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0DD8
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_0714:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E00
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_0728:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E1F
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_073C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E3E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_0750:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F25
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x1c
loc_3_0764:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F3E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_0778:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F58
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_078C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F4B
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_07A0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E5D
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_3_07B4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F65
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_3_07C8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C7C
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
sub_3_07DC:
        mov    eax, 0x100
        ret    
sub_3_07E2:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_080A
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_18C6
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_080A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax], 0
        xor    eax, eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_081D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_083D
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2E2C
loc_3_083D:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0842:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_169F
        movsx  eax, ax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_085B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_087E
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_29FE
loc_3_087E:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0883:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_08A6
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_29A6
loc_3_08A6:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_08AB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_08D1
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2A59
loc_3_08D1:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_08D6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_08F9
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2ACA
loc_3_08F9:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_08FE:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0921
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2B1F
loc_3_0921:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0926:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_094C
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2B7A
loc_3_094C:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0951:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0974
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2CD5
loc_3_0974:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0979:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_099C
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2C19
loc_3_099C:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_09A1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_09BB
        push   eax
        call   sub_3_2D5C
loc_3_09BB:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_09C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_09DA
        push   eax
        call   sub_3_2D98
loc_3_09DA:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_09DF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_09FC
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2DE8
loc_3_09FC:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0A01:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0A1E
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2E00
loc_3_0A1E:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0A23:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0A40
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_304B
loc_3_0A40:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0A45:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0A5F
        push   eax
        call   sub_3_3084
loc_3_0A5F:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0A64:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0A87
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_3091
loc_3_0A87:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0A8C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0AAC
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_322D
loc_3_0AAC:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0AB1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0AD1
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_34D6
loc_3_0AD1:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0AD6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0AF7
        push   edx
        push   edx
        push   eax
        call   sub_3_1A4B
loc_3_0AF7:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0AFC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        xor    edx, edx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0B1C
        push   dword ptr [ebp + 0xc]
        push   edx
        push   eax
        call   sub_3_1A4B
loc_3_0B1C:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0B21:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_3_1838
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0B34:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0B4E
        push   eax
        call   sub_3_35A1
loc_3_0B4E:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0B53:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [ebp + 0xc], 0x502
        jne    loc_3_0B7A
        cmp    dword ptr [ebp + 0x10], 2
        jne    loc_3_0B72
        push   dword ptr [ebp + 8]
        call   sub_3_23BB
        jmp    loc_3_0B7A
loc_3_0B72:
        push   dword ptr [ebp + 8]
        call   sub_3_2394
loc_3_0B7A:
        xor    eax, eax
        mov    ax, word ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        xor    edx, edx
        mov    esi, 0x1025                   ; -> loc_3_1025
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0B98:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_0B53
        add    esp, 0xc
        leave  
        ret    0xc
sub_3_0BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        pop    edi
        pop    ebx
        leave  
        ret    4
sub_3_0BC4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0BDE
        push   eax
        call   sub_3_2EF7
loc_3_0BDE:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0BE3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0C00
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2F10
loc_3_0C00:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0C05:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0C1F
        push   eax
        call   sub_3_38B0
loc_3_0C1F:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0C24:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0C41
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2FF6
loc_3_0C41:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0C46:
        movzx  eax, word ptr [0x1b4]         ; -> obj4:0x01B4
        ret    
sub_3_0C4E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0C6B
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_2F2D
loc_3_0C6B:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0C70:
        push   esi
        push   edi
        push   ebx
        call   sub_3_1A6C
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_3_0C7C:
        push   esi
        push   edi
        push   ebx
        push   0
        call   sub_3_1AEB
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_3_0C8A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0CB3
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_39A4
loc_3_0CB3:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0CB8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0CD8
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_1B8A
loc_3_0CD8:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0CDD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, 0x1b6                    ; -> obj4:0x01B6
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 8
        jbe    loc_3_0CF9
        mov    ecx, 8
        int3   
loc_3_0CF9:
        push   ds
        pop    es
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [0x1d6], edi        ; -> obj4:0x01D6
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0x1ea], eax        ; -> loc_4_01EA
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0D17:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0D3F
        push   edx
        push   ecx
        push   0
        push   1
        push   eax
        call   sub_3_3E30
loc_3_0D3F:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0D44:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0D6D
        push   edx
        push   ecx
        push   dword ptr [ebp + 0x10]
        push   2
        push   eax
        call   sub_3_3E30
loc_3_0D6D:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0D72:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0D7D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0D88:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0DAB
        push   0
        push   dword ptr [ebp + 0xc]
        push   0
        push   4
        push   eax
        call   sub_3_3E30
loc_3_0DAB:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0DD3
        push   0
        push   dword ptr [ebp + 0xc]
        push   0
        push   8
        push   eax
        call   sub_3_3E30
loc_3_0DD3:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0DD8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0DFB
        push   0
        push   dword ptr [ebp + 0xc]
        push   0
        push   0x10
        push   eax
        call   sub_3_3E30
loc_3_0DFB:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0E00:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0E1A
        push   eax
        call   sub_3_4306
loc_3_0E1A:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0E1F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0E39
        push   eax
        call   sub_3_438F
loc_3_0E39:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0E3E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0E58
        push   eax
        call   sub_3_45C0
loc_3_0E58:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0E5D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4 + 0x1f0] ; -> loc_4_01F0
        or     eax, eax
        je     loc_3_0E77
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_3_1DE5
loc_3_0E77:
        leave  
        ret    
sub_3_0E79:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, 0x1b6                    ; -> obj4:0x01B6
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ebx*4]
        push   dword ptr [0x1d6]             ; -> obj4:0x01D6
        push   dword ptr [ebp + 0xc]
        push   eax
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    8
sub_3_0EA4:
        call   sub_3_1E87
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_3_0EAE:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x10]
        call   sub_3_1EB0
        ret    
sub_3_0EBD:
        push   dword ptr [ebp + 0x18]
        call   sub_3_38BF
        ret    
sub_3_0EC6:
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        push   dword ptr [ebp + 0x14]
        call   sub_3_3E64
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_3_0EDE:
        push   dword ptr [ebp + 0x14]
        call   sub_3_3E90
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_3_0EEB:
        mov    ebx, dword ptr [ebp + 0x10]
        call   sub_3_1124
        mov    edx, dword ptr [ebp]
        shr    dx, 3
        rol    edx, 0x10
        shr    dx, 3
        push   eax
        push   dword ptr [ebp + 0x18]
        push   edx
        push   ebx
        call   sub_3_38CC
        ret    
sub_3_0F0D:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_3_4438
        ret    
sub_3_0F1C:
        push   dword ptr [ebp + 0x14]
        call   sub_3_4562
        ret    
sub_3_0F25:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_4693
        leave  
        ret    
sub_3_0F3E:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_47F9
        leave  
        ret    
sub_3_0F4B:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_4831
        leave  
        ret    
sub_3_0F58:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_485A
        leave  
        ret    
sub_3_0F65:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_4792
        leave  
        ret    
sub_3_0F72:
        mov    eax, dword ptr [0x1de]        ; -> obj4:0x01DE
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    esi, dword ptr [0x1da]        ; -> obj4:0x01DA
        or     dword ptr [0x1da], 1          ; -> obj4:0x01DA
        mov    eax, dword ptr [0x1de]        ; -> obj4:0x01DE
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        test   esi, 1
        jne    loc_3_0FBA
        mov    eax, 0xa0006
        mov    esi, 0x3aa                    ; -> loc_3_03AA
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_3_0FBA
        mov    dword ptr [0x1e2], esi        ; -> loc_4_01E2
loc_3_0FBA:
        push   ebx
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_3_1BE5
        pop    ebx
        test   word ptr [ebp + 0x18], 1
        je     loc_3_0FD9
        push   ebx
        call   sub_3_1B57
        or     eax, eax
        je     loc_3_101D
        jmp    loc_3_1016
loc_3_0FD9:
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        push   dword ptr [ebp + 0x18]
        push   ebx
        call   sub_3_1969
        or     eax, eax
        je     loc_3_101D
        mov    eax, 0x200000
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, 0x500
        xor    edx, edx
        mov    esi, 0x1025                   ; -> loc_3_1025
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        push   ebx
        call   sub_3_1D96
        mov    word ptr [ebp + 0x18], ax
loc_3_1016:
        mov    word ptr [ebp + 0x1c], 1
loc_3_101C:
        ret    
loc_3_101D:
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_3_101C
loc_3_1025:
        ret    
sub_3_1026:
        push   dword ptr [ebp + 0x18]
        push   ebx
        call   sub_3_25C5
        ret    
sub_3_1030:
        mov    eax, 0x1000
        mov    ecx, 2
        mov    edx, dword ptr [ebp + 0x18]
        mov    esi, 0x1049                   ; -> loc_3_1049
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        ret    
loc_3_1049:
        push   edx
        push   ebx
        call   sub_3_266C
        ret    
sub_3_1051:
        mov    eax, 0x1000
        mov    ecx, 2
        mov    edx, dword ptr [ebp + 0x18]
        mov    esi, 0x106a                   ; -> loc_3_106A
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        ret    
loc_3_106A:
        mov    eax, edx
        shr    edx, 0x10
        push   eax
        push   edx
        push   ebx
        call   sub_3_26ED
        ret    
sub_3_1078:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        push   ebp
        push   ebx
        call   sub_3_23D8
        ret    
sub_3_108B:
        push   ebx
        call   sub_3_25AF
        ret    
sub_3_1092:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x18]
        push   ebx
        call   sub_3_2745
        ret    
sub_3_109F:
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [0x1b4], ax          ; -> obj4:0x01B4
        ret    
sub_3_10AA:
        push   dword ptr [ebp + 0x18]
        push   ebx
        call   sub_3_2F57
        ret    
sub_3_10B4:
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x18]
        call   sub_3_3B4F
        mov    dword ptr [ebp + 0x1c], eax
        ret    
sub_3_10D1:
        push   ebx
        call   sub_3_2779
        ret    
sub_3_10D8:
        movzx  ecx, word ptr [ebp + 0x18]
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   ecx
        push   eax
        push   ebx
        call   sub_3_27D5
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_3_10F3:
        push   ebx
        call   sub_3_285C
        mov    dword ptr [ebp + 0x1c], eax
        ret    
sub_3_10FD:
        push   esi
        mov    esi, ebp
        enter  4, 0
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 0x14]
        VxDCall  IFSMgr.IFSMgr_Win32MapSFTToExtendedHandle ; cd2065004000
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x18], ecx
        mov    dword ptr [esi + 0x1c], eax
        leave  
        pop    esi
        ret    
sub_3_111D:
        VxDCall  VSD.VSD_Bell                    ; cd2001000b00
        ret    
sub_3_1124:
        push   ebx
        mov    ebx, dword ptr [ebx + 4]
        mov    eax, 0xff
        test   byte ptr [ebx + 0x496], 8
        jne    loc_3_113A
        and    ax, 0xfffe
loc_3_113A:
        test   word ptr [ebx + 0x417], 0x200
        jne    loc_3_1149
        and    ax, 0xfffd
loc_3_1149:
        test   byte ptr [ebx + 0x496], 4
        jne    loc_3_1156
        and    ax, 0xfffb
loc_3_1156:
        test   word ptr [ebx + 0x417], 0x100
        jne    loc_3_1165
        and    ax, 0xfff7
loc_3_1165:
        test   word ptr [ebx + 0x417], 3
        jne    loc_3_1174
        and    ax, 0xffef
loc_3_1174:
        test   word ptr [ebx + 0x417], 0x20
        jne    loc_3_1183
        and    ax, 0xffdf
loc_3_1183:
        test   word ptr [ebx + 0x417], 0x10
        jne    loc_3_1192
        and    ax, 0xffbf
loc_3_1192:
        test   word ptr [ebx + 0x417], 0x40
        jne    loc_3_11A1
        and    ax, 0xff7f
loc_3_11A1:
        pop    ebx
        ret    
sub_3_11A3:
        call   sub_3_4C24
        sub    esp, 4
        mov    eax, esp
        push   eax
        mov    ax, 0x3c18
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_3_488D
        pop    ecx
        mov    dword ptr [ebp + 0x10], ecx
        ret    
sub_3_11E7:
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_3_492A
        ret    
sub_3_1206:
        movzx  edx, word ptr [ebp + 0x14]
        push   edx
        push   dword ptr [ebp + 0x10]
        call   sub_3_4BE6
        ret    
sub_3_1214:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_9_0242
        ret    
sub_3_1220:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VxDCall  IFSMgr.IFSMgr_Win32GetVMCurdir  ; cd206b004000
        add    esp, 8
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    8
sub_3_123C:
        push   esi
        mov    esi, ebp
        enter  4, 0
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 0x18]
        push   dword ptr [esi + 0x14]
        VxDCall  IFSMgr.IFSMgr_Win32MapExtendedHandleToSFT ; cd2063004000
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x18], ecx
        mov    dword ptr [esi + 0x1c], eax
        leave  
        pop    esi
        ret    
        db  000h
sub_3_1260:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        add    eax, 0x1c
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   0
        push   eax
        call   sub_1_0020
loc_3_1278:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_3_1285
        xor    eax, eax
        jmp    loc_3_1302
loc_3_1285:
        mov    dword ptr [ebp - 0xc], 1
        mov    ecx, dword ptr [ebp - 0xc]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_1299
        xor    eax, eax
loc_3_1299:
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x18], edi
        test   edi, edi
        jne    loc_3_12B5
        push   0
        push   esi
        call   sub_1_0028
loc_3_12AE:
        add    esp, 8
        xor    eax, eax
        jmp    loc_3_1302
loc_3_12B5:
        mov    ebx, dword ptr [ebp + 0xc]
        push   0
        inc    ebx
        imul   ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_0020
loc_3_12C5:
        add    esp, 8
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    loc_3_12EA
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   0
        push   esi
        call   sub_1_0028
loc_3_12E3:
        add    esp, 8
        xor    eax, eax
        jmp    loc_3_1302
loc_3_12EA:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [esi + 0xc], eax
        add    eax, ebx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [esi], edx
        mov    eax, esi
loc_3_1302:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_130B:
        sub    esp, 0xc
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        push   edi
        push   ebp
        mov    edi, dword ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x18], ecx
        mov    ecx, dword ptr [esi + 0x14]
        cmp    edi, eax
        jbe    loc_3_1336
        sub    edi, eax
        mov    ecx, edi
        xor    edi, edi
        jmp    loc_3_133E
loc_3_1336:
        sub    ecx, dword ptr [esp + 0x10]
        sub    edi, dword ptr [esp + 0x18]
loc_3_133E:
        mov    ebp, dword ptr [esi + 4]
        lea    eax, [ecx + edi]
        sub    edx, edx
        div    ebp
        dec    eax
        mov    edx, dword ptr [esp + 0x24]
        cmp    eax, edx
        jb     loc_3_1353
        mov    eax, edx
loc_3_1353:
        mov    ebx, eax
        mov    dword ptr [esp + 0x14], eax
        imul   ebx, ebp
        cmp    ebx, ecx
        mov    ebp, ebx
        jb     loc_3_1364
        mov    ebp, ecx
loc_3_1364:
        test   ebp, ebp
        je     loc_3_137F
        push   1
        push   ebp
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x1c]
        call   sub_3_1E10
        mov    dword ptr [esi + 0x10], eax
        add    dword ptr [esp + 0x28], ebp
loc_3_137F:
        sub    ebx, ebp
        cmp    ebx, edi
        jb     loc_3_1387
        mov    ebx, edi
loc_3_1387:
        test   ebx, ebx
        je     loc_3_139E
        push   1
        push   ebx
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x24]
        call   sub_3_1E10
        mov    dword ptr [esi + 0x10], eax
loc_3_139E:
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [esi + 0x10], eax
        jne    loc_3_13AC
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 0x10], eax
loc_3_13AC:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    0xc
sub_3_13BA:
        sub    esp, 0xc
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        push   edi
        push   ebp
        mov    eax, dword ptr [esi + 0xc]
        mov    ebx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x18], ecx
        mov    ecx, dword ptr [esi + 0x14]
        cmp    ebx, eax
        jne    loc_3_13E4
        xor    eax, eax
        jmp    loc_3_146D
loc_3_13E4:
        cmp    ebx, dword ptr [esp + 0x10]
        jbe    loc_3_13F4
        sub    ebx, dword ptr [esp + 0x10]
        mov    ecx, ebx
        xor    ebx, ebx
        jmp    loc_3_13FC
loc_3_13F4:
        sub    ecx, dword ptr [esp + 0x10]
        sub    ebx, dword ptr [esp + 0x18]
loc_3_13FC:
        mov    ebp, dword ptr [esi + 4]
        lea    eax, [ecx + ebx]
        sub    edx, edx
        div    ebp
        mov    edx, dword ptr [esp + 0x24]
        cmp    eax, edx
        jb     loc_3_1410
        mov    eax, edx
loc_3_1410:
        mov    edi, eax
        mov    dword ptr [esp + 0x14], eax
        imul   edi, ebp
        cmp    edi, ecx
        mov    ebp, edi
        jb     loc_3_1421
        mov    ebp, ecx
loc_3_1421:
        test   ebp, ebp
        je     loc_3_143C
        push   0
        push   ebp
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x34]
        call   sub_3_1E10
        mov    dword ptr [esi + 0xc], eax
        add    dword ptr [esp + 0x28], ebp
loc_3_143C:
        sub    edi, ebp
        cmp    edi, ebx
        jb     loc_3_1444
        mov    edi, ebx
loc_3_1444:
        test   edi, edi
        je     loc_3_145B
        push   0
        push   edi
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x34]
        call   sub_3_1E10
        mov    dword ptr [esi + 0xc], eax
loc_3_145B:
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [esi + 0xc], eax
        jne    loc_3_1469
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 0xc], eax
loc_3_1469:
        mov    eax, dword ptr [esp + 0x14]
loc_3_146D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    0xc
sub_3_1477:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0x10], ecx
        jne    loc_3_1487
        xor    eax, eax
        jmp    loc_3_149B
loc_3_1487:
        push   0
        push   dword ptr [eax + 4]
        push   ecx
        push   dword ptr [esp + 0x14]
        call   sub_3_1E10
        mov    eax, 1
loc_3_149B:
        ret    8
sub_3_149E:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0x10]
        cmp    dword ptr [eax + 0xc], edx
        jne    loc_3_14AE
        xor    eax, eax
        jmp    loc_3_14CE
loc_3_14AE:
        lea    ecx, [eax + 4]
        cmp    dword ptr [eax + 8], edx
        jne    loc_3_14B9
        mov    edx, dword ptr [eax + 0x14]
loc_3_14B9:
        sub    edx, dword ptr [ecx]
        push   0
        push   dword ptr [ecx]
        push   edx
        push   dword ptr [esp + 0x14]
        call   sub_3_1E10
        mov    eax, 1
loc_3_14CE:
        ret    8
sub_3_14D1:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0x10], ecx
        jne    loc_3_14E1
        xor    eax, eax
        jmp    loc_3_14F5
loc_3_14E1:
        push   0
        push   dword ptr [eax + 4]
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_3_1E10
        mov    eax, 1
loc_3_14F5:
        ret    8
sub_3_14F8:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0x10]
        cmp    dword ptr [ecx + 0xc], edx
        jne    loc_3_1508
        xor    eax, eax
        jmp    loc_3_1528
loc_3_1508:
        lea    eax, [ecx + 4]
        cmp    dword ptr [ecx + 8], edx
        jne    loc_3_1513
        mov    edx, dword ptr [ecx + 0x14]
loc_3_1513:
        sub    edx, dword ptr [eax]
        push   0
        push   dword ptr [eax]
        push   dword ptr [esp + 0x10]
        push   edx
        call   sub_3_1E10
        mov    eax, 1
loc_3_1528:
        ret    8
sub_3_152B:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
sub_3_153B:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    eax, dword ptr [edx + 0xc]
        mov    esi, dword ptr [edx + 0x10]
        cmp    eax, esi
        jne    loc_3_154E
        xor    eax, eax
        jmp    loc_3_1572
loc_3_154E:
        mov    ecx, dword ptr [esp + 0xc]
        test   ecx, ecx
        je     loc_3_1572
        add    ecx, dword ptr [edx + 4]
        cmp    dword ptr [edx + 0x14], ecx
        jne    loc_3_1561
        mov    ecx, dword ptr [edx + 8]
loc_3_1561:
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_3_1572:
        pop    esi
        ret    8
sub_3_1576:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   0
        push   dword ptr [esi + 8]
        call   sub_1_0028
loc_3_1599:
        add    esp, 8
        push   0
        push   esi
        call   sub_1_0028
loc_3_15A4:
        add    esp, 8
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_3_15AE:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [edi + 0x10]
        mov    edx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 0x14]
        cmp    ecx, eax
        jne    loc_3_15C8
        xor    eax, eax
        jmp    loc_3_15DE
loc_3_15C8:
        jbe    loc_3_15D2
        sub    ecx, eax
        mov    esi, ecx
        xor    ecx, ecx
        jmp    loc_3_15D6
loc_3_15D2:
        sub    esi, eax
        sub    ecx, edx
loc_3_15D6:
        lea    eax, [esi + ecx]
        sub    edx, edx
        div    dword ptr [edi + 4]
loc_3_15DE:
        pop    edi
        pop    esi
        ret    4
sub_3_15E3:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x10]
        cmp    dword ptr [ecx + 0xc], eax
        je     loc_3_1604
        cmp    dword ptr [ecx + 8], eax
        jne    loc_3_15F7
        mov    eax, dword ptr [ecx + 0x14]
loc_3_15F7:
        sub    eax, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, 1
        jmp    loc_3_1606
loc_3_1604:
        xor    eax, eax
loc_3_1606:
        ret    4
        db  000h, 000h, 000h
sub_3_160C:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        mov    ecx, 0x40
        sub    esp, 0x14
        push   edi
        mov    edi, 0                        ; -> loc_4_01F0
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [ebp - 0x14], eax
        mov    ecx, dword ptr [ebp - 0x14]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_1634
        xor    eax, eax
loc_3_1634:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> loc_4_02F8
        test   eax, eax
        jne    loc_3_1647
        xor    eax, eax
        jmp    loc_3_169A
loc_3_1647:
        mov    dword ptr [ebp - 0x10], 0
        mov    ecx, dword ptr [ebp - 0x10]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_165B
        xor    eax, eax
loc_3_165B:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0], eax            ; -> loc_4_02F4
        test   eax, eax
        jne    loc_3_166E
        xor    eax, eax
        jmp    loc_3_169A
loc_3_166E:
        push   0
        push   8
        push   4
        call   sub_3_1260
        mov    dword ptr [0], eax            ; -> loc_4_02F0
        test   eax, eax
        mov    eax, 1
        jne    loc_3_169A
        mov    eax, dword ptr [0]            ; -> loc_4_02F4
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        xor    eax, eax
loc_3_169A:
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_169F:
        mov    dx, 0x40
        push   esi
        push   edi
        mov    eax, 0                        ; -> loc_4_01F0
        mov    esi, dword ptr [esp + 0xc]
loc_3_16AE:
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     loc_3_16BE
        cmp    dword ptr [ecx], esi
        jne    loc_3_16BE
        mov    dword ptr [eax], 0
loc_3_16BE:
        add    eax, 4
        dec    dx
        jne    loc_3_16AE
        xor    edi, edi
        xor    eax, eax
loc_3_16C9:
        movsx  ecx, di
        cmp    dword ptr [ecx*4], eax        ; -> loc_4_01F0
        je     loc_3_16E6
        inc    di
        cmp    di, 0x40
        jl     loc_3_16C9
loc_3_16DD:
        mov    ax, 0xffff
loc_3_16E1:
        pop    edi
        pop    esi
        ret    8
loc_3_16E6:
        push   0
        push   0x60
        call   sub_1_0020
loc_3_16EF:
        add    esp, 8
        test   eax, eax
        je     loc_3_16DD
        xor    edx, edx
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 0x10], edx
        mov    dword ptr [eax + 0x14], edx
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], edx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 0xc], edx
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [eax + 0x28], esi
        mov    dword ptr [eax + 0x5c], edx
        mov    dword ptr [eax + 0x1c], 0x12
        mov    word ptr [eax + 0x18], 0x50
        mov    dword ptr [eax + 0x58], edx
        movsx  edx, di
        mov    word ptr [eax + 0x1a], 0x19
        mov    dword ptr [edx*4], eax        ; -> loc_4_01F0
        mov    ax, di
        jmp    loc_3_16E1
sub_3_173E:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_3_193C
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_3_175B
        xor    eax, eax
        jmp    loc_3_182E
loc_3_175B:
        mov    esi, dword ptr [esp + 0x20]
        push   0
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_1_0020
loc_3_1774:
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_3_1784
        xor    eax, eax
        jmp    loc_3_182E
loc_3_1784:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebp
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        push   0
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [esp + 0x20]
        mov    ecx, 0xffffffff
        mov    edi, esi
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_1_0020
loc_3_17BE:
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        test   eax, eax
        jne    loc_3_17D8
        push   0
        push   ebp
        call   sub_1_0028
loc_3_17D1:
        add    esp, 8
        xor    eax, eax
        jmp    loc_3_182E
loc_3_17D8:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x10]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebx + 0x10]
        test   eax, eax
        je     loc_3_180D
        push   0
        push   eax
        call   sub_1_0028
loc_3_180A:
        add    esp, 8
loc_3_180D:
        mov    eax, dword ptr [ebx + 0x14]
        test   eax, eax
        je     loc_3_181F
        push   0
        push   eax
        call   sub_1_0028
loc_3_181C:
        add    esp, 8
loc_3_181F:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x10], ebp
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, 1
loc_3_182E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
sub_3_1838:
        movsx  eax, word ptr [esp + 4]
        push   esi
        lea    eax, [eax*4]                  ; -> loc_4_01F0
        mov    esi, dword ptr [eax]
        test   esi, esi
        jne    loc_3_184F
        xor    eax, eax
        jmp    loc_3_18C2
loc_3_184F:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_1862
        push   eax
        call   sub_3_303F
loc_3_1862:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     loc_3_186F
        push   eax
        call   sub_3_20A2
loc_3_186F:
        mov    eax, dword ptr [esi + 0xc]
        test   eax, eax
        je     loc_3_187C
        push   eax
        call   sub_3_3F60
loc_3_187C:
        mov    eax, dword ptr [esi + 0x14]
        test   eax, eax
        je     loc_3_188E
        push   0
        push   eax
        call   sub_1_0028
loc_3_188B:
        add    esp, 8
loc_3_188E:
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_3_18A0
        push   0
        push   eax
        call   sub_1_0028
loc_3_189D:
        add    esp, 8
loc_3_18A0:
        mov    eax, dword ptr [esi + 0x5c]
        test   eax, eax
        je     loc_3_18B2
        push   0
        push   eax
        call   sub_1_0028
loc_3_18AF:
        add    esp, 8
loc_3_18B2:
        push   0
        push   esi
        call   sub_1_0028
loc_3_18BA:
        add    esp, 8
        mov    eax, 1
loc_3_18C2:
        pop    esi
        ret    4
sub_3_18C6:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [edx + 0x14]
        test   edi, edi
        je     loc_3_1934
        mov    ebx, dword ptr [edx + 0x10]
        test   ebx, ebx
        je     loc_3_1934
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x14]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x18]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [edx + 0x3c]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, 1
        jmp    loc_3_1936
loc_3_1934:
        xor    eax, eax
loc_3_1936:
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
sub_3_193C:
        mov    eax, dword ptr [esp + 4]
        push   esi
        test   eax, eax
        je     loc_3_195F
        xor    edx, edx
        mov    ecx, 0                        ; -> loc_4_01F0
loc_3_194C:
        mov    esi, dword ptr [ecx]
        test   esi, esi
        je     loc_3_1956
        cmp    dword ptr [esi], eax
        je     loc_3_1965
loc_3_1956:
        inc    edx
        add    ecx, 4
        cmp    edx, 0x40
        jl     loc_3_194C
loc_3_195F:
        xor    eax, eax
loc_3_1961:
        pop    esi
        ret    4
loc_3_1965:
        mov    eax, dword ptr [ecx]
        jmp    loc_3_1961
sub_3_1969:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_193C
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_3_1988
        xor    eax, eax
        jmp    loc_3_1A42
loc_3_1988:
        cmp    dword ptr [ebx + 4], 0
        jne    loc_3_19A2
        push   esi
        call   sub_3_2024
        mov    dword ptr [ebx + 4], eax
        test   eax, eax
        jne    loc_3_19A2
        xor    eax, eax
        jmp    loc_3_1A42
loc_3_19A2:
        cmp    dword ptr [ebx + 8], 0
        jne    loc_3_19BB
        call   sub_3_3010
        mov    dword ptr [ebx + 8], eax
        test   eax, eax
        jne    loc_3_19BB
        xor    eax, eax
        jmp    loc_3_1A42
loc_3_19BB:
        cmp    dword ptr [ebx + 0xc], 0
        jne    loc_3_19D1
        call   sub_3_3EAD
        mov    dword ptr [ebx + 0xc], eax
        test   eax, eax
        jne    loc_3_19D1
        xor    eax, eax
        jmp    loc_3_1A42
loc_3_19D1:
        mov    dword ptr [ebx + 0x54], 0
        mov    word ptr [ebx + 0x3a], 0
        mov    esi, dword ptr [ebp + 0x10]
        test   esi, esi
        je     loc_3_1A3D
        lea    edx, [ebx + 0x3c]
        mov    ecx, 6
        mov    edi, edx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ax, word ptr [edx]
        cmp    ax, 2
        je     loc_3_1A20
        cmp    ax, 1
        je     loc_3_1A20
        mov    ax, word ptr [edx + 8]
        cmp    ax, 2
        je     loc_3_1A20
        cmp    ax, 1
        je     loc_3_1A20
        mov    ax, word ptr [edx + 0x10]
        cmp    ax, 2
        je     loc_3_1A20
        cmp    ax, 1
        jne    loc_3_1A3D
loc_3_1A20:
        mov    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_1A34
        xor    eax, eax
loc_3_1A34:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x54], eax
loc_3_1A3D:
        mov    eax, 1
loc_3_1A42:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_1A4B:
        mov    eax, 0x10c
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 0xc]
        cmp    dword ptr [esp + 0xc], 1
        adc    eax, -1
        push   eax
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    0xc
sub_3_1A6C:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_4_02F4
        mov    ebp, esp
        sub    esp, 0x18
        mov    dword ptr [ebp - 0x14], 1
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    edx, dword ptr [0]            ; -> loc_4_02F0
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   edx
        call   sub_3_13BA
        test   eax, eax
        mov    eax, dword ptr [0]            ; -> loc_4_02F0
        mov    ecx, dword ptr [eax + 0x18]
        je     loc_3_1AD9
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        cmp    dword ptr [ebp - 4], 0
        je     loc_3_1AE5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x28]
        jmp    loc_3_1AE7
loc_3_1AD9:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_1AE5:
        xor    eax, eax
loc_3_1AE7:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1AEB:
        push   ebp
        mov    edx, dword ptr [0]            ; -> loc_4_02F0
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        cmp    dword ptr [ebp + 8], 0
        je     loc_3_1B1F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x28]
        test   eax, eax
        je     loc_3_1B1F
        or     byte ptr [eax + 0x19], 1
loc_3_1B1F:
        lea    eax, [ebp + 8]
        push   eax
        push   1
        push   edx
        call   sub_3_130B
        mov    ecx, dword ptr [0]            ; -> loc_4_02F0
        mov    edx, dword ptr [ecx + 0x18]
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, dword ptr [0]            ; -> loc_4_02F4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_1B57:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_3_193C
        mov    edi, eax
        test   edi, edi
        je     loc_3_1B79
        push   0
        push   0
        push   esi
        call   sub_3_1969
        test   eax, eax
        jne    loc_3_1B79
        xor    edi, edi
loc_3_1B79:
        push   edi
        call   sub_3_1AEB
        cmp    edi, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        inc    eax
        ret    4
sub_3_1B8A:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [ecx + 0x34]
        cmp    word ptr [ebx], 0
        mov    esi, ebx
        je     loc_3_1BA4
loc_3_1B9D:
        inc    esi
        cmp    word ptr [esi], 0
        jne    loc_3_1B9D
loc_3_1BA4:
        sub    esi, ebx
        mov    edi, dword ptr [ecx + 0x58]
        add    esi, 2
        lea    eax, [esi + edi]
        cmp    dword ptr [esp + 0x1c], eax
        jb     loc_3_1BDB
        test   edi, edi
        je     loc_3_1BCB
        mov    ebp, dword ptr [esp + 0x18]
        push   1
        push   edi
        push   dword ptr [ecx + 0x5c]
        push   ebp
        call   sub_3_1E10
        jmp    loc_3_1BCF
loc_3_1BCB:
        mov    ebp, dword ptr [esp + 0x18]
loc_3_1BCF:
        push   1
        add    ebp, edi
        push   esi
        push   ebx
        push   ebp
        call   sub_3_1E10
loc_3_1BDB:
        lea    eax, [esi + edi]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
sub_3_1BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x184
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        mov    dword ptr [ebp - 0x10], 1
        je     loc_3_1D59
        movzx  eax, word ptr [ebp + 0xc]
        shl    eax, 4
        mov    ecx, dword ptr [ebp - 8]
        add    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [ecx + 0x34], eax
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_1C2E
        mov    dword ptr [ebp - 0x10], 0
loc_3_1C2E:
        pop    ebx
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_3_1D59
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0x58], 0
        je     loc_3_1C65
        mov    eax, dword ptr [ecx + 0x5c]
        test   eax, eax
        je     loc_3_1C65
        push   0
        push   eax
        call   sub_1_0028
loc_3_1C51:
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x58], 0
        mov    dword ptr [ecx + 0x5c], 0
loc_3_1C65:
        mov    esi, 0                        ; -> loc_4_02FC
        lea    edi, [ebp - 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        xor    esi, esi
        lea    ebx, [ebp - 0x80]
loc_3_1C74:
        lea    eax, [ebp - 0x184]
        push   eax
        push   esi
        call   sub_3_1220
        test   eax, eax
        jne    loc_3_1CAA
        lea    edi, [ebp - 0x184]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    eax, ecx
        mov    dword ptr [ebx], ecx
        test   eax, eax
        je     loc_3_1CB0
        add    eax, 8
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ecx + 0x58], eax
        jmp    loc_3_1CB0
loc_3_1CAA:
        mov    dword ptr [ebx], 0
loc_3_1CB0:
        add    ebx, 4
        inc    esi
        lea    eax, [ebp - 0x18]
        cmp    ebx, eax
        jb     loc_3_1C74
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 0x58]
        test   eax, eax
        je     loc_3_1D59
        push   0
        push   eax
        call   sub_1_0020
loc_3_1CD1:
        add    esp, 8
        mov    ebx, eax
        mov    ecx, dword ptr [ebp - 8]
        test   ebx, ebx
        mov    dword ptr [ecx + 0x5c], ebx
        jne    loc_3_1CE9
        mov    dword ptr [ecx + 0x58], 0
        jmp    loc_3_1D59
loc_3_1CE9:
        mov    dword ptr [ebp - 4], 0
loc_3_1CF0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + eax*4 - 0x80]
        mov    dword ptr [ebp - 0xc], ecx
        test   ecx, ecx
        je     loc_3_1D50
        lea    edi, [ebp - 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        add    ebx, 7
        and    ecx, 3
        push   ebx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        add    al, 0x41
        mov    byte ptr [ebx - 3], al
        mov    byte ptr [ebx - 6], al
        call   sub_3_1220
        test   eax, eax
        je     loc_3_1D4C
        push   0
        sub    ebx, 7
        push   0                             ; -> obj4:0x0304
        call   sub_3_1F1E
        jmp    loc_3_1D50
loc_3_1D4C:
        add    ebx, dword ptr [ebp - 0xc]
        inc    ebx
loc_3_1D50:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0x1a
        jl     loc_3_1CF0
loc_3_1D59:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_1D62:
        mov    edx, 0                        ; -> loc_4_01F0
        mov    eax, dword ptr [esp + 4]
loc_3_1D6B:
        mov    ecx, dword ptr [edx]
        test   ecx, ecx
        je     loc_3_1D75
        cmp    dword ptr [ecx], eax
        je     loc_3_1D82
loc_3_1D75:
        add    edx, 4
        cmp    edx, 0x100                    ; -> loc_4_02F0
        jb     loc_3_1D6B
        jmp    loc_3_1D93
loc_3_1D82:
        push   dword ptr [ecx + 0x28]
        push   5
        mov    dword ptr [ecx + 0x20], 0
        call   sub_3_0E79
loc_3_1D93:
        ret    4
sub_3_1D96:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        cmp    dword ptr [eax + 0x54], 0
        mov    edx, eax
        je     loc_3_1DDB
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edx + 0x54]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [edx + 0x54]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    dword ptr [edx + 0x54], 0
loc_3_1DDB:
        mov    ax, word ptr [edx + 0x3a]
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_1DE5:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3a], cx
        mov    eax, dword ptr [eax + 0x54]
        test   eax, eax
        je     loc_3_1E09
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_1E09:
        mov    esp, ebp
        pop    ebp
        ret    8
        db  000h
sub_3_1E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [ebp - 4], esi
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_3_1E30
        mov    dword ptr [ebp - 4], edi
loc_3_1E30:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_1E3B:
        push   esi
        mov    ecx, 0xffffffff
        mov    esi, dword ptr [esp + 8]
        push   edi
        push   0
        mov    edi, esi
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_1_0020
loc_3_1E56:
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        je     loc_3_1E80
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_1E80:
        mov    eax, edx
        pop    edi
        pop    esi
        ret    4
sub_3_1E87:
        cmp    dword ptr [0], 0              ; -> obj4:0x0324
        jne    loc_3_1EAD
        push   1
        push   0x300
        call   sub_1_0020
loc_3_1E9C:
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj4:0x0324
        test   eax, eax
        mov    eax, 1
        jne    loc_3_1EAF
loc_3_1EAD:
        xor    eax, eax
loc_3_1EAF:
        ret    
sub_3_1EB0:
        mov    cl, byte ptr [esp + 4]
        mov    edx, dword ptr [0]            ; -> obj4:0x0324
        and    cl, 0x7f
        test   edx, edx
        je     loc_3_1EDE
        mov    ax, word ptr [esp + 8]
        cmp    ax, 5
        ja     loc_3_1EDE
        movzx  ecx, cl
        movzx  eax, ax
        imul   ecx, ecx, 6
        add    ecx, eax
        mov    al, byte ptr [esp + 0xc]
        mov    byte ptr [ecx + edx], al
loc_3_1EDE:
        ret    0xc
sub_3_1EE1:
        mov    ax, word ptr [esp + 4]
        push   esi
        and    ax, 0x7f
        mov    esi, dword ptr [0]            ; -> obj4:0x0324
        test   esi, esi
        jne    loc_3_1EFA
        xor    ax, ax
        jmp    loc_3_1F1A
loc_3_1EFA:
        mov    cx, word ptr [esp + 0xc]
        cmp    cx, 5
        jbe    loc_3_1F0A
        xor    ax, ax
        jmp    loc_3_1F1A
loc_3_1F0A:
        movzx  edx, ax
        movzx  eax, cx
        imul   edx, edx, 6
        add    edx, eax
        movzx  ax, byte ptr [edx + esi]
loc_3_1F1A:
        pop    esi
        ret    8
sub_3_1F1E:
        ret    8
loc_3_1F21:
        ret    4
sub_3_1F24:
        push   esi
        xor    al, al
        mov    esi, dword ptr [0]            ; -> obj4:0x0324
        mov    dl, byte ptr [esp + 8]
loc_3_1F31:
        movzx  ecx, al
        imul   ecx, ecx, 6
        add    ecx, esi
        cmp    byte ptr [ecx], dl
        je     loc_3_1F5E
        cmp    byte ptr [ecx + 1], dl
        je     loc_3_1F5E
        cmp    byte ptr [ecx + 2], dl
        je     loc_3_1F5E
        cmp    byte ptr [ecx + 3], dl
        je     loc_3_1F5E
        cmp    byte ptr [ecx + 4], dl
        je     loc_3_1F5E
        cmp    byte ptr [ecx + 5], dl
        je     loc_3_1F5E
        inc    al
        cmp    al, 0x80
        jb     loc_3_1F31
        xor    al, al
loc_3_1F5E:
        pop    esi
        ret    4
sub_3_1F62:
        mov    edx, dword ptr [esp + 4]
        push   esi
        push   edi
        xor    esi, esi
        mov    al, byte ptr [edx]
        test   al, al
        jne    loc_3_1F77
        mov    eax, 2
        jmp    loc_3_1F90
loc_3_1F77:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    edx, ecx
        add    esi, ecx
        cmp    byte ptr [edx], 0
        jne    loc_3_1F77
        lea    eax, [esi + 1]
loc_3_1F90:
        pop    edi
        pop    esi
        ret    4
sub_3_1F95:
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, esi
        dec    esi
        test   eax, eax
        je     loc_3_1FBC
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
loc_3_1FA9:
        mov    cl, byte ptr [eax]
        cmp    byte ptr [edx], cl
        jne    loc_3_1FC0
        test   cl, cl
        je     loc_3_1FBC
        inc    eax
        inc    edx
        mov    ecx, esi
        dec    esi
        test   ecx, ecx
        jne    loc_3_1FA9
loc_3_1FBC:
        xor    eax, eax
loc_3_1FBE:
        pop    esi
        ret    
loc_3_1FC0:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [edx]
        sub    eax, ecx
        jmp    loc_3_1FBE
sub_3_1FCA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        test   esi, esi
        je     loc_3_2016
        mov    ebx, dword ptr [esp + 0x18]
        test   ebx, ebx
        je     loc_3_2016
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        cmp    byte ptr [esi], 0
        lea    ebp, [ecx - 1]
        je     loc_3_2016
loc_3_1FF3:
        push   ebp
        push   ebx
        push   esi
        call   sub_3_1F95
        add    esp, 0xc
        test   eax, eax
        je     loc_3_201F
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    esi, ecx
        cmp    byte ptr [esi], 0
        jne    loc_3_1FF3
loc_3_2016:
        xor    eax, eax
loc_3_2018:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_3_201F:
        mov    eax, esi
        jmp    loc_3_2018
        db  000h
sub_3_2024:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   0x14
        push   0x100
        push   6
        call   sub_3_1260
        mov    esi, eax
        test   esi, esi
        jne    loc_3_2043
        xor    eax, eax
        jmp    loc_3_209B
loc_3_2043:
        call   sub_3_21DF
        mov    dword ptr [esi + 0x28], eax
        test   eax, eax
        jne    loc_3_2059
        push   esi
        call   sub_3_1576
        xor    eax, eax
        jmp    loc_3_209B
loc_3_2059:
        mov    dword ptr [ebp - 8], 0x100
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_206D
        xor    eax, eax
loc_3_206D:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], eax
        test   eax, eax
        jne    loc_3_208C
        push   dword ptr [esi + 0x28]
        call   sub_3_226C
        push   esi
        call   sub_3_1576
        xor    eax, eax
        jmp    loc_3_209B
loc_3_208C:
        mov    dword ptr [esi + 0x1c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, esi
loc_3_209B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_20A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0x28]
        call   sub_3_226C
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   esi
        call   sub_3_1576
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_20D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        mov    dword ptr [ebp - 0xc], 1
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ebp - 0x18], ax
        mov    dword ptr [ebp - 0x16], ecx
        lea    eax, [ebp - 0x18]
        push   eax
        push   1
        push   esi
        call   sub_3_130B
        push   dword ptr [esi + 0x24]
        inc    dword ptr [esi + 0x1c]
        call   sub_3_0BB0
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_3_2142:
        ret    4
loc_3_2145:
        ret    4
loc_3_2148:
        push   esi
        xor    ecx, ecx
        mov    edx, 0                        ; -> loc_4_01F0
        mov    eax, dword ptr [esp + 8]
loc_3_2154:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi], eax
        je     loc_3_216C
        add    edx, 4
        inc    ecx
        cmp    edx, 0x100                    ; -> loc_4_02F0
        jb     loc_3_2154
        xor    eax, eax
loc_3_2168:
        pop    esi
        ret    4
loc_3_216C:
        mov    eax, dword ptr [ecx*4]        ; -> loc_4_01F0
        mov    eax, dword ptr [eax + 4]
        jmp    loc_3_2168
sub_3_2178:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   1
        push   esi
        call   sub_3_13BA
        cmp    eax, 1
        mov    eax, 0
        jne    loc_3_21AB
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, 1
loc_3_21AB:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_21B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_15E3
        test   eax, eax
        je     loc_3_21D8
        mov    eax, dword ptr [esi + 0x20]
        dec    dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_21D8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_21DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   0xc
        push   0x32
        push   1
        call   sub_3_1260
        mov    edx, eax
        test   edx, edx
        jne    loc_3_21FA
        xor    eax, eax
        jmp    loc_3_2268
loc_3_21FA:
        mov    dword ptr [ebp - 0x14], 0
        mov    ecx, dword ptr [ebp - 0x14]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_220E
        xor    eax, eax
loc_3_220E:
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [edx + 0x20], eax
        test   eax, eax
        jne    loc_3_2225
        push   edx
        call   sub_3_1576
        xor    eax, eax
        jmp    loc_3_2268
loc_3_2225:
        mov    dword ptr [ebp - 0xc], 1
        mov    ecx, dword ptr [ebp - 0xc]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_2239
        xor    eax, eax
loc_3_2239:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edx + 0x24], eax
        test   eax, eax
        jne    loc_3_225F
        mov    eax, dword ptr [edx + 0x20]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   edx
        call   sub_3_1576
        xor    eax, eax
        jmp    loc_3_2268
loc_3_225F:
        mov    dword ptr [edx + 0x1c], 0
        mov    eax, edx
loc_3_2268:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_226C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   ecx
        call   sub_3_1576
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_22A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x10]
loc_3_22C6:
        push   ebx
        push   esi
        push   edi
        call   sub_3_130B
        mov    ecx, eax
        test   ecx, ecx
        jne    loc_3_231D
        mov    edx, dword ptr [edi + 0x1c]
        mov    eax, dword ptr [edi + 0x18]
        inc    edx
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [edi + 0x1c], edx
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        dec    edx
        mov    dword ptr [edi + 0x1c], edx
        jmp    loc_3_22C6
loc_3_231D:
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_2337:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_13BA
        cmp    dword ptr [esi + 0x1c], 0
        mov    ecx, eax
        je     loc_3_237C
        test   ecx, ecx
        je     loc_3_237C
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_237C:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_2394:
        push   esi
        push   dword ptr [esp + 8]
        call   sub_3_193C
        mov    esi, eax
        test   esi, esi
        je     loc_3_23B7
        cmp    dword ptr [esi + 4], 0
        je     loc_3_23B7
        push   esi
        call   sub_3_2878
        push   eax
        push   esi
        call   sub_3_2937
loc_3_23B7:
        pop    esi
        ret    4
sub_3_23BB:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        je     loc_3_23D2
        cmp    dword ptr [eax + 4], 0
        je     loc_3_23D2
        or     byte ptr [eax + 0x1c], 0x20
loc_3_23D2:
        ret    4
        db  000h, 000h, 000h
sub_3_23D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
loc_3_23E4:
        push   edi
        call   sub_3_193C
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_24FA
        test   byte ptr [ebx + 0x1c], 0x20
        jne    loc_3_24AB
        mov    esi, dword ptr [ebx + 4]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        lea    eax, [ebp - 0x28]
        push   eax
        push   esi
        call   sub_3_1477
        test   eax, eax
        je     loc_3_24B6
        movzx  eax, word ptr [ebp - 0x28]
        sub    eax, 0x101
        cmp    eax, 0x11
        ja     loc_3_24E1
        movzx  ecx, byte ptr [eax]           ; -> obj3:0x259D
        jmp    dword ptr [ecx*4]             ; -> obj3:0x2579
loc_3_2447:
        mov    cx, word ptr [ebp - 0x26]
        mov    eax, dword ptr [ebp - 0x26]
        shr    eax, 0x10
        and    ax, 0xff
        shr    cx, 8
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 0x26]
        shr    eax, 0x18
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebp - 0x26]
        and    eax, 0xff
        cmp    word ptr [ebp - 0x28], 0x10e
        push   eax
        push   dword ptr [ebp - 4]
        push   ecx
        push   ebx
        jne    loc_3_2485
        call   sub_3_2BE5
        jmp    loc_3_248A
loc_3_2485:
        call   sub_3_2CA0
loc_3_248A:
        lea    eax, [ebp - 0x28]
        push   eax
        push   esi
        call   sub_3_2178
        mov    ecx, dword ptr [esi + 0x18]
        dec    dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_3_23E4
loc_3_24AB:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x1c], 0x108
        jmp    loc_3_24FA
loc_3_24B6:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x18]
        mov    word ptr [ecx + 0x1c], 0x108
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_3_24FA
loc_3_24D0:
        mov    ax, word ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x1c], ax
        mov    edx, dword ptr [ebp - 0x26]
        mov    dword ptr [ecx + 0x18], edx
loc_3_24E1:
        lea    eax, [ebp - 0x28]
        push   eax
        push   esi
        call   sub_3_2178
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_24FA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_3_2503:
        mov    ecx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x10]
        mov    word ptr [ecx + 0x1c], 0x107
        mov    ax, word ptr [ebp - 0x26]
        mov    word ptr [ecx + 0x18], ax
        movzx  ecx, word ptr [ebp - 0x26]
        push   ecx
        push   dword ptr [esi + 0x28]
        call   sub_3_2337
        jmp    loc_3_24E1
loc_3_2526:
        mov    word ptr [ebp - 0x28], 0x10a
        lea    eax, [ebp - 0x28]
        push   eax
        push   esi
        call   sub_3_14D1
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    eax, dword ptr [ebp - 0x1c]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x1c], 0x108
        mov    eax, dword ptr [ebp - 0x26]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_3_24FA
loc_3_255F:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x1c], 0x108
        jmp    loc_3_24FA
        ; ---- dispatch table (9 entries) ----
        dd  loc_3_24D0
        dd  loc_3_24D0
        dd  loc_3_2503
        dd  loc_3_2526
        dd  loc_3_255F
        dd  loc_3_24D0
        dd  loc_3_2447
        dd  loc_3_24D0
        dd  loc_3_24E1
        db  000h, 000h, 000h, 008h, 001h, 001h, 002h, 008h, 003h, 004h, 005h, 005h, 005h, 006h, 006h, 007h
        db  007h, 007h
sub_3_25AF:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        je     loc_3_25C2
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x1c]
loc_3_25C2:
        ret    4
sub_3_25C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 0x14], si
        mov    dword ptr [ebp - 0x10], esi
        mov    word ptr [ebp - 0xc], si
        lea    edi, [ebp - 4]
        mov    word ptr [ebp - 0xa], si
        xor    eax, eax
        mov    word ptr [ebp - 8], si
        mov    word ptr [ebp - 6], si
        stosd  dword ptr es:[edi], eax
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 0x14], 0x8000
        mov    bx, word ptr [ebp + 0xc]
        mov    edi, eax
        mov    ax, bx
        push   1
        shr    ax, 8
        lea    ecx, [ebp - 0x14]
        mov    word ptr [ebp - 0xa], ax
        push   ecx
        mov    word ptr [ebp - 0xc], si
        push   edi
        mov    byte ptr [ebp - 6], bl
        call   sub_3_32BE
        cmp    eax, 1
        je     loc_3_2629
        call   sub_3_111D
loc_3_2629:
        cmp    bx, 0xd
        jne    loc_3_2663
        push   edi
        call   sub_3_3084
        test   al, 1
        je     loc_3_2663
        push   edi
        call   sub_3_3084
        test   al, 2
        je     loc_3_2663
        push   1
        lea    eax, [ebp - 0x14]
        mov    byte ptr [ebp - 6], 0xa
        mov    word ptr [ebp - 0xc], 1
        push   eax
        push   edi
        call   sub_3_32BE
        cmp    eax, 1
        je     loc_3_2663
        call   sub_3_111D
loc_3_2663:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_266C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 0x14], 2
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, esi
        mov    edi, esi
        shr    ecx, 0x10
        and    edi, 0x3e00
        and    cx, 0xff
        mov    word ptr [ebp - 0x10], cx
        mov    ecx, esi
        shr    ecx, 0x18
        mov    word ptr [ebp - 0xe], cx
        mov    ecx, edi
        shr    ecx, 9
        mov    dword ptr [ebp - 0xc], ecx
        mov    ecx, esi
        and    ecx, 0xff
        and    esi, 0x8000
        push   1
        lea    edx, [ebp - 0x14]
        push   edx
        mov    dword ptr [ebp - 8], ecx
        push   eax
        cmp    esi, 1
        sbb    ecx, ecx
        neg    ecx
        mov    dword ptr [ebp - 4], ecx
        call   sub_3_32BE
        cmp    eax, 1
        je     loc_3_26E5
        test   esi, esi
        je     loc_3_26E5
        test   edi, edi
        je     loc_3_26E5
        call   sub_3_111D
loc_3_26E5:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_26ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 0x14], 1
        mov    esi, eax
        mov    cl, byte ptr [ebp + 0xc]
        mov    word ptr [ebp - 0xc], 1
        mov    al, cl
        and    al, 0x80
        cmp    al, 1
        sbb    eax, eax
        lea    edx, [ebp - 0x10]
        and    cx, 0x7f
        push   edx
        neg    eax
        mov    word ptr [ebp - 8], cx
        mov    dword ptr [ebp - 0x10], eax
        movzx  eax, word ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        call   sub_3_3697
        push   1
        lea    ecx, [ebp - 0x14]
        push   ecx
        push   esi
        call   sub_3_32BE
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_2745:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        je     loc_3_2776
        mov    cx, word ptr [esp + 8]
        push   dword ptr [eax + 0x28]
        push   4
        mov    dx, word ptr [esp + 0x12]
        mov    word ptr [eax + 0x18], cx
        movzx  ecx, word ptr [esp + 0x14]
        mov    word ptr [eax + 0x1a], dx
        mov    dword ptr [eax + 0x2c], ecx
        call   sub_3_0E79
loc_3_2776:
        ret    0xc
sub_3_2779:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    esi, eax
        test   esi, esi
        je     loc_3_27CE
        and    byte ptr [esi + 0x1c], 0xbf
        cmp    dword ptr [esi + 8], 0
        je     loc_3_279E
        push   esi
        call   sub_3_38B0
loc_3_279E:
        mov    ecx, dword ptr [esi + 0x28]
        test   ecx, ecx
        je     loc_3_27CE
        mov    eax, dword ptr [ecx + 0x18]
        test   al, 0x80
        je     loc_3_27CE
        mov    dword ptr [ebp - 4], 1
        or     ah, 4
        mov    dword ptr [ecx + 0x18], eax
        mov    ecx, dword ptr [0]            ; -> loc_4_02F8
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
loc_3_27CE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_27D5:
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_3_193C
        mov    edi, eax
        test   edi, edi
        jne    loc_3_27F3
        xor    ax, ax
        jmp    loc_3_2852
loc_3_27F3:
        mov    ebp, dword ptr [edi + 0x1c]
        and    ebp, 0x80
        cmp    dword ptr [esp + 0x20], 0
        je     loc_3_2846
        mov    esi, dword ptr [esp + 0x1c]
loc_3_2807:
        lea    eax, [esp + 0x13]
        push   1
        push   -1
        push   eax
        push   edi
        call   sub_3_3091
        test   eax, eax
        je     loc_3_2846
        mov    cl, byte ptr [esp + 0x13]
        mov    byte ptr [esi], cl
        cmp    byte ptr [esp + 0x13], 0
        je     loc_3_2832
        push   dword ptr [esp + 0x13]
        call   sub_3_1F24
        jmp    loc_3_2835
loc_3_2832:
        shr    eax, 0x18
loc_3_2835:
        inc    bx
        mov    byte ptr [esi + 1], al
        movzx  eax, bx
        add    esi, 2
        cmp    eax, dword ptr [esp + 0x20]
        jb     loc_3_2807
loc_3_2846:
        and    byte ptr [edi + 0x1c], 0x7f
        cmp    ebp, 1
        sbb    eax, eax
        and    ax, bx
loc_3_2852:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
sub_3_285C:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        jne    loc_3_286D
        xor    eax, eax
        jmp    loc_3_2873
loc_3_286D:
        push   eax
        call   sub_3_35E8
loc_3_2873:
        ret    4
        db  000h, 000h
sub_3_2878:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x28
        cmp    dword ptr [eax + 0x20], 0
        push   esi
        mov    edx, dword ptr [eax + 4]
        jne    loc_3_28A7
        mov    dword ptr [ebp - 0x24], 1
        mov    eax, dword ptr [0]            ; -> loc_4_02F8
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp - 0x24]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
loc_3_28A7:
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        cmp    dword ptr [edx + 0x1c], 0
        jne    loc_3_28D4
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        xor    eax, eax
        jmp    loc_3_2930
loc_3_28D4:
        mov    dword ptr [ebp - 0x18], 0
        mov    ecx, dword ptr [ebp - 0x18]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_28E8
        xor    eax, eax
loc_3_28E8:
        mov    dword ptr [ebp - 0x14], eax
        mov    esi, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   esi
        push   0x109
        push   edx
        call   sub_3_20D0
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [ebp - 0xc], esi
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    eax, 1
loc_3_2930:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_2937:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        cmp    dword ptr [ebp + 0xc], 0
        push   esi
        je     loc_3_297A
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        lea    eax, [ebp - 0x10]
        dec    dword ptr [esi + 0x1c]
        push   eax
        push   esi
        call   sub_3_2178
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_297A:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_2981:
        mov    edx, dword ptr [esp + 4]
        movsx  ecx, word ptr [esp + 0xa]
        movsx  eax, word ptr [edx + 0x18]
        imul   eax, ecx
        mov    ecx, dword ptr [edx + 0x2c]
        lea    eax, [eax + ecx*8]
        movsx  ecx, word ptr [esp + 8]
        add    eax, ecx
        add    eax, eax
        add    eax, dword ptr [edx + 0x20]
        ret    8
sub_3_29A6:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        push   dword ptr [esp + 0x1c]
        push   esi
        mov    edi, eax
        call   sub_3_2981
        mov    ebx, dword ptr [esp + 0x18]
        test   ebx, ebx
        je     loc_3_29E9
        mov    edx, dword ptr [esp + 0x14]
loc_3_29D3:
        mov    cl, byte ptr [eax]
        dec    ebx
        inc    eax
        mov    byte ptr [edx], cl
        movzx  cx, byte ptr [eax]
        mov    word ptr [edx + 2], cx
        inc    eax
        add    edx, 4
        test   ebx, ebx
        jne    loc_3_29D3
loc_3_29E9:
        push   edi
        push   esi
        call   sub_3_2937
        push   1
        push   esi
        call   sub_3_2F75
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
sub_3_29FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 0x10]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        cld    
        or     ecx, ecx
loc_3_2A35:
        je     loc_3_2A3C
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    esi
        dec    ecx
        jmp    loc_3_2A35
loc_3_2A3C:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_2A59:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 0x10]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        cld    
        cmp    dword ptr [ebp + 0x18], 0
        jne    loc_3_2AA3
        or     ecx, ecx
loc_3_2A96:
        je     loc_3_2AAD
        lodsw  ax, word ptr [esi]
        mov    al, ah
        xor    ah, ah
        stosw  word ptr es:[edi], ax
        dec    ecx
        jmp    loc_3_2A96
loc_3_2AA3:
        jecxz  loc_3_2AAD
loc_3_2AA5:
        lodsw  ax, word ptr [esi]
        mov    al, ah
        stosb  byte ptr es:[edi], al
        dec    ecx
        jne    loc_3_2AA5
loc_3_2AAD:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
sub_3_2ACA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        push   dword ptr [esp + 0x1c]
        push   esi
        mov    edi, eax
        call   sub_3_2981
        mov    ebx, dword ptr [esp + 0x18]
        test   ebx, ebx
        je     loc_3_2B0A
        mov    edx, dword ptr [esp + 0x14]
loc_3_2AF7:
        mov    cl, byte ptr [edx]
        dec    ebx
        add    edx, 4
        mov    byte ptr [eax], cl
        inc    eax
        mov    cl, byte ptr [edx - 2]
        mov    byte ptr [eax], cl
        inc    eax
        test   ebx, ebx
        jne    loc_3_2AF7
loc_3_2B0A:
        push   edi
        push   esi
        call   sub_3_2937
        push   1
        push   esi
        call   sub_3_2F75
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
sub_3_2B1F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 0x10]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x14]
        cld    
        or     ecx, ecx
loc_3_2B56:
        je     loc_3_2B5D
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    edi
        dec    ecx
        jmp    loc_3_2B56
loc_3_2B5D:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_2B7A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 0x10]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x14]
        cld    
        cmp    dword ptr [ebp + 0x18], 0
        jne    loc_3_2BC0
        or     ecx, ecx
loc_3_2BB7:
        je     loc_3_2BC8
        lodsw  ax, word ptr [esi]
        inc    edi
        stosb  byte ptr es:[edi], al
        dec    ecx
        jmp    loc_3_2BB7
loc_3_2BC0:
        jecxz  loc_3_2BC8
loc_3_2BC2:
        lodsb  al, byte ptr [esi]
        inc    edi
        stosb  byte ptr es:[edi], al
        dec    ecx
        jne    loc_3_2BC2
loc_3_2BC8:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
sub_3_2BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_3_2981
        mov    dword ptr [ebp - 4], eax
        push   ds
        pop    es
        mov    al, byte ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x14]
        cld    
        or     ecx, ecx
loc_3_2C08:
        je     loc_3_2C0F
        stosb  byte ptr es:[edi], al
        inc    edi
        dec    ecx
        jmp    loc_3_2C08
loc_3_2C0F:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_2C19:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 0x1c], 1
        jbe    loc_3_2C69
        cmp    edi, 0x100
        jae    loc_3_2C69
        movzx  ebx, byte ptr [ebp + 0xc]
        movsx  eax, word ptr [ebp + 0x12]
        shl    eax, 0x10
        push   0
        or     ebx, eax
        push   esi
        movsx  eax, word ptr [ebp + 0x10]
        shl    ebx, 8
        shl    eax, 0x10
        or     ebx, eax
        call   sub_3_2F75
        or     ebx, edi
        push   ebx
        push   0x10e
        push   dword ptr [esi + 4]
        call   sub_3_20D0
        jmp    loc_3_2C8F
loc_3_2C69:
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        mov    ebx, eax
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_2BE5
        mov    edi, eax
        push   ebx
        push   esi
        call   sub_3_2937
loc_3_2C8F:
        push   1
        push   esi
        call   sub_3_2F75
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
sub_3_2CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_3_2981
        mov    dword ptr [ebp - 4], eax
        push   ds
        pop    es
        mov    ax, word ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x14]
        cld    
        or     ecx, ecx
loc_3_2CC4:
        je     loc_3_2CCB
        inc    edi
        stosb  byte ptr es:[edi], al
        dec    ecx
        jmp    loc_3_2CC4
loc_3_2CCB:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_2CD5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 0x1c], 1
        jbe    loc_3_2D25
        cmp    edi, 0x100
        jae    loc_3_2D25
        movsx  ebx, word ptr [ebp + 0x12]
        movzx  eax, word ptr [ebp + 0xc]
        shl    ebx, 0x10
        push   0
        or     ebx, eax
        push   esi
        movsx  eax, word ptr [ebp + 0x10]
        shl    ebx, 8
        shl    eax, 0x10
        or     ebx, eax
        call   sub_3_2F75
        or     ebx, edi
        push   ebx
        push   0x10f
        push   dword ptr [esi + 4]
        call   sub_3_20D0
        jmp    loc_3_2D4B
loc_3_2D25:
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        mov    ebx, eax
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_2CA0
        mov    edi, eax
        push   ebx
        push   esi
        call   sub_3_2937
loc_3_2D4B:
        push   1
        push   esi
        call   sub_3_2F75
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
sub_3_2D5C:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_3_2878
        mov    ecx, dword ptr [esi + 0x20]
        push   eax
        add    ecx, 0x450
        push   esi
        movzx  dx, byte ptr [ecx]
        mov    word ptr [esp + 0xc], dx
        movzx  cx, byte ptr [ecx + 1]
        mov    word ptr [esp + 0xe], cx
        call   sub_3_2937
        mov    eax, dword ptr [esp + 4]
        pop    esi
        add    esp, 4
        ret    4
sub_3_2D98:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   esi
        call   sub_3_2878
        mov    edi, eax
        mov    ecx, dword ptr [esi + 0x20]
        mov    al, byte ptr [ecx + 0x460]
        mov    dl, byte ptr [ecx + 0x461]
        and    eax, 0x1f
        and    edx, 0x1f
        movzx  ecx, byte ptr [ecx + 0x485]
        sub    eax, edx
        inc    eax
        imul   eax, eax, 0x64
        cdq    
        idiv   ecx
        mov    bx, ax
        test   byte ptr [esi + 0x1c], 2
        je     loc_3_2DD8
        or     bh, 0x80
loc_3_2DD8:
        push   edi
        push   esi
        call   sub_3_2937
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_2DE8:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   0x101
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_2E00:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        test   ax, ax
        je     loc_3_2E14
        or     byte ptr [ecx + 0x1c], 2
        jmp    loc_3_2E18
loc_3_2E14:
        and    byte ptr [ecx + 0x1c], 0xfd
loc_3_2E18:
        movzx  eax, ax
        push   eax
        push   0x102
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_2E2C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x20
        cmp    dword ptr [ebp + 0x10], 0
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 4]
        mov    esi, dword ptr [edi + 0x28]
        je     loc_3_2EEE
loc_3_2E49:
        mov    dword ptr [ebp - 0x14], 1
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_3_22A0
        mov    ebx, eax
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        lea    eax, [ebp - 0x20]
        push   eax
        push   edi
        call   sub_3_149E
        test   eax, eax
        je     loc_3_2EB8
        cmp    word ptr [ebp - 0x20], 0x107
        jne    loc_3_2EB8
        lea    eax, [ebp - 0x20]
        add    dword ptr [ebp - 0x1e], ebx
        push   eax
        push   edi
        call   sub_3_14F8
        mov    ecx, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_3_2ED3
loc_3_2EB8:
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   ebx
        push   0x107
        push   edi
        call   sub_3_20D0
loc_3_2ED3:
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        add    dword ptr [ebp + 0xc], ebx
        sub    dword ptr [ebp + 0x10], ebx
        jne    loc_3_2E49
loc_3_2EEE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_2EF7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_2878
        push   eax
        push   esi
        call   sub_3_2937
        mov    eax, dword ptr [esi + 0x18]
        pop    esi
        ret    4
sub_3_2F10:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   0x110
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        mov    eax, 1
        ret    8
sub_3_2F2D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        push   0x111
        push   dword ptr [esi + 4]
        call   sub_3_20D0
        push   esi
        call   sub_3_2878
        push   eax
        push   esi
        call   sub_3_2937
        mov    eax, dword ptr [esi + 0x30]
        pop    esi
        ret    8
sub_3_2F57:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        mov    cx, word ptr [esp + 8]
        mov    dx, word ptr [esp + 0xa]
        mov    word ptr [eax + 0x30], cx
        mov    word ptr [eax + 0x32], dx
        ret    8
sub_3_2F75:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        test   byte ptr [eax + 0x1c], 0x10
        push   esi
        mov    esi, dword ptr [eax + 4]
        je     loc_3_2FEF
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        lea    eax, [ebp - 0x14]
        push   eax
        push   esi
        call   sub_3_149E
        test   eax, eax
        je     loc_3_2FD2
        cmp    word ptr [ebp - 0x14], 0x10d
        jne    loc_3_2FD2
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 0x12], eax
        je     loc_3_2FD2
        push   esi
        call   sub_3_21B2
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_3_2FEF
loc_3_2FD2:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   dword ptr [ebp + 0xc]
        push   0x10d
        push   esi
        call   sub_3_20D0
loc_3_2FEF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_2FF6:
        movzx  eax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        push   eax
        push   0x103
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_3010:
        push   0x24
        push   0x100
        push   0x14
        call   sub_3_1260
        test   eax, eax
        jne    loc_3_3026
        xor    eax, eax
        jmp    loc_3_303E
loc_3_3026:
        mov    dword ptr [eax + 0x1c], 7
        mov    word ptr [eax + 0x2c], 0xffff
        xor    ecx, ecx
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x34], ecx
        mov    dword ptr [eax + 0x38], ecx
loc_3_303E:
        ret    
sub_3_303F:
        push   dword ptr [esp + 4]
        call   sub_3_1576
        ret    4
sub_3_304B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 8]
        test   ecx, ecx
        je     loc_3_3080
        and    byte ptr [esi + 0x1c], 0xbf
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0x1c], eax
        test   al, 0x10
        jne    loc_3_3078
        push   0
        push   esi
        call   sub_3_3589
        push   0
        push   esi
        call   sub_3_2F75
        jmp    loc_3_3080
loc_3_3078:
        push   1
        push   esi
        call   sub_3_3589
loc_3_3080:
        pop    esi
        ret    8
sub_3_3084:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 0x1c]
        ret    4
sub_3_3091:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 8]
        xor    eax, eax
        mov    dword ptr [ebp - 0xc], eax
        mov    byte ptr [ebp - 1], al
        cmp    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        je     loc_3_30B7
        and    byte ptr [eax + 0x1c], 0x7f
        jmp    loc_3_30BB
loc_3_30B7:
        or     byte ptr [eax + 0x1c], 0x80
loc_3_30BB:
        cmp    dword ptr [ebp + 0x10], -1
        jne    loc_3_30D2
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_3_30D2
        mov    eax, 1
        mov    dword ptr [ebp + 0x10], eax
        mov    dword ptr [ebp - 0xc], eax
loc_3_30D2:
        mov    eax, dword ptr [ebx + 0x18]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        push   ebx
        call   sub_3_3968
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    dword ptr [ebp + 0x14], 0
        mov    dword ptr [ebp - 0x18], eax
        jne    loc_3_3104
        push   dword ptr [ebp + 8]
        call   sub_3_393F
loc_3_3104:
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_3_31D6
loc_3_3115:
        lea    eax, [ebp - 0x30]
        push   eax
        push   1
        push   ebx
        call   sub_3_13BA
        test   eax, eax
        je     loc_3_31D6
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_3_319B
        cmp    word ptr [ebp - 0x30], 1
        jne    loc_3_3159
        cmp    dword ptr [ebp - 0x2c], 0
        je     loc_3_3159
        cmp    word ptr [ebp - 0x24], 0x2e
        jne    loc_3_3159
        test   byte ptr [ebp - 0x20], 0xc
        je     loc_3_3159
        mov    byte ptr [ebp - 0x22], 3
        mov    word ptr [ebp - 0x30], 0x8000
        mov    word ptr [ebp - 0x28], 1
loc_3_3159:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_3_31A1
        cmp    word ptr [ebp - 0x30], 0x8000
        jne    loc_3_319B
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_3_3173
        test   byte ptr [ebp - 0x28], 1
        jne    loc_3_319B
loc_3_3173:
        cmp    byte ptr [ebp - 0x22], 0
        jne    loc_3_317F
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_3_31CA
loc_3_317F:
        mov    cl, byte ptr [ebp - 0x22]
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp + 0xc]
        mov    byte ptr [eax], cl
        cmp    byte ptr [ebp - 0x22], 0
        jne    loc_3_31CA
        mov    al, byte ptr [ebp - 0x26]
        mov    byte ptr [ebp - 1], al
        jmp    loc_3_31CA
loc_3_319B:
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_3_31CA
loc_3_31A1:
        cmp    word ptr [ebp - 0x30], 0x8000
        je     loc_3_31CA
        cmp    word ptr [ebp - 0x30], 2
        jne    loc_3_31B6
        test   byte ptr [ebx + 0x1c], 0x10
        je     loc_3_31CA
loc_3_31B6:
        lea    esi, [ebp - 0x30]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        add    dword ptr [ebp + 0xc], 0x14
        inc    dword ptr [ebp - 8]
loc_3_31CA:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 0x10], eax
        ja     loc_3_3115
loc_3_31D6:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_3_3205
        push   ebx
        call   sub_3_3968
        test   eax, eax
        jne    loc_3_3205
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0x18]
        mov    dword ptr [ebp - 0x14], edx
        mov    eax, dword ptr [ebp - 0x14]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   dword ptr [ebp + 8]
        call   sub_3_3578
        jmp    loc_3_321A
loc_3_3205:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0x18]
        mov    dword ptr [ebp - 0x10], edx
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_321A:
        movsx  eax, byte ptr [ebp - 1]
        shl    eax, 0x18
        pop    edi
        or     eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_322D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        push   dword ptr [ebp + 8]
        call   sub_3_393F
        mov    dword ptr [ebp - 4], 0
        push   0
        push   esi
        call   sub_3_153B
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_329D
        mov    edi, dword ptr [ebp + 0xc]
loc_3_3271:
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 0x10]
        jae    loc_3_329D
        cmp    word ptr [ebx], 0x8000
        je     loc_3_3290
        push   1
        push   0x14
        push   ebx
        push   edi
        call   sub_3_1E10
        mov    edi, eax
        inc    dword ptr [ebp - 4]
loc_3_3290:
        push   ebx
        push   esi
        call   sub_3_153B
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_3_3271
loc_3_329D:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0x18]
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_32BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        test   edi, edi
        jne    loc_3_32D8
        xor    eax, eax
        jmp    loc_3_34CD
loc_3_32D8:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x14], ecx
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        push   edi
        call   sub_3_3968
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_3_34AC
        mov    ebx, dword ptr [ebp + 0xc]
loc_3_3319:
        mov    cx, word ptr [ebx]
        cmp    cx, 1
        jne    loc_3_3370
        test   byte ptr [ebx + 0x10], 0xc
        je     loc_3_3370
        mov    ax, word ptr [ebx + 0xc]
        cmp    ax, 0x46
        jne    loc_3_334B
        cmp    dword ptr [ebx + 4], 0
        je     loc_3_349A
        push   dword ptr [esi + 0x28]
        push   3
        call   sub_3_0E79
        jmp    loc_3_349A
loc_3_334B:
        cmp    ax, 0x2e
        jne    loc_3_3370
        test   byte ptr [edi + 0x1c], 1
        je     loc_3_3370
        cmp    dword ptr [ebx + 4], 0
        je     loc_3_349A
        push   dword ptr [esi + 0x28]
        push   2
        call   sub_3_0E79
        jmp    loc_3_349A
loc_3_3370:
        test   byte ptr [edi + 0x1c], 0x10
        jne    loc_3_3380
        cmp    cx, 2
        je     loc_3_349A
loc_3_3380:
        cmp    cx, 2
        jne    loc_3_341F
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [edi + 0x34]
        not    ecx
        and    ecx, eax
        mov    dword ptr [edi + 0x34], eax
        je     loc_3_33DD
        cmp    dword ptr [edi + 0x28], ecx
        jne    loc_3_33C4
        mov    ax, word ptr [edi + 0x20]
        cmp    word ptr [ebx + 4], ax
        jne    loc_3_33C4
        mov    ax, word ptr [edi + 0x22]
        cmp    word ptr [ebx + 6], ax
        jne    loc_3_33C4
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [edi + 0x24]
        cmp    eax, dword ptr [0]            ; -> obj4:0x0328
        jae    loc_3_33C4
        or     byte ptr [ebx + 0x10], 2
loc_3_33C4:
        mov    ax, word ptr [ebx + 4]
        mov    dx, word ptr [ebx + 6]
        mov    word ptr [edi + 0x20], ax
        mov    word ptr [edi + 0x22], dx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edi + 0x28], ecx
loc_3_33DD:
        lea    eax, [ebp - 0x28]
        push   eax
        push   edi
        call   sub_3_149E
        test   eax, eax
        je     loc_3_341F
        mov    eax, dword ptr [ebp - 0x20]
        cmp    dword ptr [ebx + 8], eax
        jne    loc_3_341F
        mov    eax, dword ptr [ebp - 0x1c]
        cmp    dword ptr [ebx + 0xc], eax
        jne    loc_3_341F
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [ebx + 0x10], eax
        jne    loc_3_341F
        mov    ax, word ptr [ebx + 4]
        lea    edx, [ebp - 0x28]
        mov    word ptr [ebp - 0x24], ax
        push   edx
        mov    cx, word ptr [ebx + 6]
        push   edi
        mov    word ptr [ebp - 0x22], cx
        call   sub_3_14F8
        jmp    loc_3_349A
loc_3_341F:
        lea    eax, [ebp - 0x28]
        push   eax
        push   edi
        call   sub_3_149E
        test   eax, eax
        je     loc_3_3464
        cmp    word ptr [ebx], 1
        jne    loc_3_3464
        cmp    dword ptr [ebx + 4], 0
        je     loc_3_3464
        cmp    word ptr [ebp - 0x28], 1
        jne    loc_3_3464
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_3_3464
        mov    ax, word ptr [ebp - 0x1c]
        cmp    word ptr [ebx + 0xc], ax
        jne    loc_3_3464
        mov    ax, word ptr [ebx + 8]
        lea    ecx, [ebp - 0x28]
        add    word ptr [ebp - 0x20], ax
        push   ecx
        push   edi
        call   sub_3_14F8
        jmp    loc_3_349A
loc_3_3464:
        push   ebx
        push   1
        push   edi
        call   sub_3_130B
        cmp    eax, 1
        jne    loc_3_34AC
        mov    ax, word ptr [ebx]
        cmp    ax, 1
        je     loc_3_3493
        cmp    ax, 2
        je     loc_3_3493
        cmp    ax, 4
        je     loc_3_3493
        cmp    ax, 8
        je     loc_3_3493
        cmp    ax, 0x10
        jne    loc_3_349A
loc_3_3493:
        mov    dword ptr [ebp - 0xc], 1
loc_3_349A:
        add    ebx, 0x14
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 0x10]
        jb     loc_3_3319
loc_3_34AC:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_3_34CA
        push   esi
        call   sub_3_3567
loc_3_34CA:
        mov    eax, dword ptr [ebp - 4]
loc_3_34CD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_34D6:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebp, ebp
        mov    word ptr [esp + 0x10], bp
        mov    dword ptr [esp + 0x14], ebp
        mov    word ptr [esp + 0x18], bp
        lea    edi, [esp + 0x20]
        mov    word ptr [esp + 0x1a], bp
        xor    eax, eax
        mov    word ptr [esp + 0x1c], bp
        xor    ebx, ebx
        mov    word ptr [esp + 0x1e], bp
        stosd  dword ptr es:[edi], eax
        cmp    dword ptr [esp + 0x30], ebp
        je     loc_3_355B
        mov    esi, dword ptr [esp + 0x28]
        mov    edi, dword ptr [esp + 0x2c]
loc_3_3513:
        push   1
        push   edi
        push   esi
        call   sub_3_32BE
        cmp    eax, 1
        jne    loc_3_355B
        cmp    word ptr [edi], 1
        jne    loc_3_3551
        cmp    dword ptr [edi + 4], ebp
        je     loc_3_3551
        lea    ecx, [esp + 0x10]
        push   1
        mov    word ptr [esp + 0x14], 0x8000
        mov    al, byte ptr [edi + 0xe]
        push   ecx
        mov    word ptr [esp + 0x20], bp
        push   esi
        mov    byte ptr [esp + 0x2a], al
        call   sub_3_32BE
        cmp    eax, 1
        jne    loc_3_355B
loc_3_3551:
        inc    ebx
        add    edi, 0x14
        cmp    ebx, dword ptr [esp + 0x30]
        jb     loc_3_3513
loc_3_355B:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    0xc
sub_3_3567:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0x28]
        push   1
        call   sub_3_0E79
        ret    4
sub_3_3578:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0x28]
        push   0
        call   sub_3_0E79
        ret    4
sub_3_3589:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   0x106
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_35A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_393F
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        push   esi
        call   sub_3_3968
        mov    ecx, eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_35E8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    ebx, esi
        xor    eax, eax
        xor    edi, edi
loc_3_3612:
        push   eax
        push   ebx
        call   sub_3_153B
        test   eax, eax
        je     loc_3_362B
        movzx  ecx, word ptr [eax]
        cmp    ecx, 0x8000
        jne    loc_3_3612
        inc    edi
        jmp    loc_3_3612
loc_3_362B:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_3645:
        cmp    dword ptr [0], 0              ; -> obj4:0x032C
        push   esi
        jne    loc_3_3690
        xor    esi, esi
        mov    dword ptr [0], esi            ; -> obj8:0x0000
loc_3_3657:
        push   4
        push   esi
        call   sub_3_1EE1
        test   ax, ax
        jne    loc_3_367C
        push   5
        push   esi
        call   sub_3_1EE1
        test   ax, ax
        jne    loc_3_367C
        inc    si
        cmp    si, 0x80
        jb     loc_3_3657
        jmp    loc_3_3686
loc_3_367C:
        mov    dword ptr [0], 1              ; -> obj8:0x0000
loc_3_3686:
        mov    dword ptr [0], 1              ; -> obj4:0x032C
loc_3_3690:
        mov    eax, dword ptr [0]            ; -> obj8:0x0000
        pop    esi
        ret    
sub_3_3697:
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        push   ebp
        mov    ecx, dword ptr [esi + 0xc]
        mov    ebx, ecx
        mov    ebp, ecx
        and    ebx, 0x10
        and    ebp, 0xc
        mov    edi, ecx
        mov    eax, ecx
        and    edi, 3
        and    eax, 1
        mov    edx, ecx
        mov    dword ptr [esp + 0x14], eax
        mov    eax, ecx
        and    edx, 0x100
        and    eax, 0x20
        test   eax, eax
        je     loc_3_36D6
        test   ebx, ebx
        je     loc_3_36DA
        test   eax, eax
        jne    loc_3_36EE
loc_3_36D6:
        test   ebx, ebx
        je     loc_3_36EE
loc_3_36DA:
        test   edi, edi
        jne    loc_3_36EE
        test   ebp, ebp
        jne    loc_3_36EE
        mov    dword ptr [esp + 0x10], 1
        test   edx, edx
        je     loc_3_36F6
loc_3_36EE:
        mov    dword ptr [esp + 0x10], 0
loc_3_36F6:
        test   cl, 0x80
        je     loc_3_3702
        cmp    ebx, 1
        sbb    ebx, ebx
        neg    ebx
loc_3_3702:
        mov    ax, word ptr [esi + 8]
        push   3
        push   eax
        call   sub_3_1EE1
        mov    word ptr [esi + 0xa], 0
        mov    word ptr [esi + 6], ax
        call   sub_3_3645
        test   eax, eax
        je     loc_3_3733
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_3733
        cmp    ebx, 1
        sbb    ecx, ecx
        add    cx, 5
        jmp    loc_3_3742
loc_3_3733:
        mov    cx, 2
        test   ebp, ebp
        jne    loc_3_3742
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    cx
loc_3_3742:
        movzx  edx, word ptr [esi + 6]
        cmp    edx, 0x6a
        je     loc_3_377A
        cmp    edx, 0x6b
        je     loc_3_3788
        cmp    edx, 0x6d
        je     loc_3_37A6
loc_3_3755:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_3845
        sub    edx, 0xc
        cmp    edx, 0x22
        ja     loc_3_3845
        movzx  eax, byte ptr [edx]           ; -> obj3:0x388D
        jmp    dword ptr [eax*4]             ; -> obj3:0x385D
loc_3_377A:
        test   cx, cx
        jne    loc_3_3755
        mov    byte ptr [esi + 0xa], 0x2a
        jmp    loc_3_3853
loc_3_3788:
        lea    eax, [ecx - 2]
        cmp    edi, 1
        sbb    edi, edi
        neg    edi
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        test   eax, edi
        je     loc_3_3755
        mov    byte ptr [esi + 0xa], 0x2b
        jmp    loc_3_3853
loc_3_37A6:
        lea    eax, [ecx - 2]
        cmp    edi, 1
        sbb    edi, edi
        neg    edi
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        test   eax, edi
        je     loc_3_3755
        mov    byte ptr [esi + 0xa], 0x2d
        jmp    loc_3_3853
loc_3_37C4:
        mov    byte ptr [esi + 0xa], 0x35
        mov    word ptr [esi + 6], 0x65
        jmp    loc_3_3853
loc_3_37D3:
        mov    byte ptr [esi + 0xa], 0x39
        mov    word ptr [esi + 6], 0x69
        jmp    loc_3_3853
loc_3_37DF:
        mov    byte ptr [esi + 0xa], 0x33
        mov    word ptr [esi + 6], 0x63
        jmp    loc_3_3853
loc_3_37EB:
        mov    byte ptr [esi + 0xa], 0x31
        mov    word ptr [esi + 6], 0x61
        jmp    loc_3_3853
loc_3_37F7:
        mov    byte ptr [esi + 0xa], 0x37
        mov    word ptr [esi + 6], 0x67
        jmp    loc_3_3853
loc_3_3803:
        mov    byte ptr [esi + 0xa], 0x34
        mov    word ptr [esi + 6], 0x64
        jmp    loc_3_3853
loc_3_380F:
        mov    byte ptr [esi + 0xa], 0x38
        mov    word ptr [esi + 6], 0x68
        jmp    loc_3_3853
loc_3_381B:
        mov    byte ptr [esi + 0xa], 0x36
        mov    word ptr [esi + 6], 0x66
        jmp    loc_3_3853
loc_3_3827:
        mov    byte ptr [esi + 0xa], 0x32
        mov    word ptr [esi + 6], 0x62
        jmp    loc_3_3853
loc_3_3833:
        mov    byte ptr [esi + 0xa], 0x30
        mov    word ptr [esi + 6], 0x60
        jmp    loc_3_3853
loc_3_383F:
        mov    byte ptr [esi + 0xa], 0x2e
        jmp    loc_3_3853
loc_3_3845:
        mov    ax, word ptr [esi + 8]
        push   ecx
        push   eax
        call   sub_3_1EE1
        mov    byte ptr [esi + 0xa], al
loc_3_3853:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
        ; ---- dispatch table (12 entries) ----
        dd  loc_3_37C4
        dd  loc_3_37D3
        dd  loc_3_37DF
        dd  loc_3_37EB
        dd  loc_3_37F7
        dd  loc_3_3803
        dd  loc_3_380F
        dd  loc_3_381B
        dd  loc_3_3827
        dd  loc_3_3833
        dd  loc_3_383F
        dd  loc_3_3845
        db  000h, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh
        db  00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 00Bh, 00Bh, 00Bh
        db  00Bh, 009h, 00Ah
sub_3_38B0:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 8]
        call   sub_3_152B
        ret    4
sub_3_38BF:
        movzx  eax, word ptr [esp + 4]
        mov    dword ptr [0], eax            ; -> obj4:0x0328
        ret    4
sub_3_38CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 0x14], 2
        mov    cx, word ptr [ebp + 0xc]
        mov    dx, word ptr [ebp + 0xe]
        mov    word ptr [ebp - 0x10], cx
        mov    word ptr [ebp - 0xe], dx
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], 0
        movzx  ecx, dx
        shr    edx, 0x10
        mov    dword ptr [ebp - 0xc], ecx
        test   dx, 0xfffe
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 8], ecx
        jne    loc_3_3917
        mov    dword ptr [ebp - 4], 1
loc_3_3917:
        push   1
        lea    ecx, [ebp - 0x14]
        push   ecx
        push   eax
        call   sub_3_32BE
        cmp    eax, 1
        je     loc_3_3939
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_3939
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_3_3939
        call   sub_3_111D
loc_3_3939:
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_393F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 0x1c], 0x10
        je     loc_3_3964
        mov    eax, dword ptr [esi + 0x1c]
        test   al, 0x40
        jne    loc_3_3964
        test   al, 0x10
        je     loc_3_3964
        push   1
        push   esi
        call   sub_3_2F75
        or     byte ptr [esi + 0x1c], 0x40
loc_3_3964:
        pop    esi
        ret    4
sub_3_3968:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
loc_3_3972:
        push   eax
        push   esi
        call   sub_3_153B
        test   eax, eax
        je     loc_3_399C
        movzx  ecx, word ptr [eax]
        cmp    ecx, 1
        jl     loc_3_3972
        cmp    ecx, 2
        jle    loc_3_3999
        cmp    ecx, 4
        je     loc_3_3999
        cmp    ecx, 8
        je     loc_3_3999
        cmp    ecx, 0x10
        jne    loc_3_3972
loc_3_3999:
        inc    edi
        jmp    loc_3_3972
loc_3_399C:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
        db  000h
sub_3_39A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   1
        push   0x1c
        call   sub_1_0020
loc_3_39B6:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_3_3AAD
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_3_3AAD
        push   eax
        call   sub_3_1E3B
        mov    edi, eax
        mov    dword ptr [esi], eax
        test   edi, edi
        je     loc_3_3AA2
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        jne    loc_3_39F0
        mov    dword ptr [esi + 4], 0
        jmp    loc_3_3A01
loc_3_39F0:
        push   eax
        call   sub_3_1E3B
        mov    dword ptr [esi + 4], eax
        test   eax, eax
        je     loc_3_3A96
loc_3_3A01:
        mov    eax, dword ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_3_3A11
        mov    dword ptr [esi + 8], 0
        jmp    loc_3_3A1E
loc_3_3A11:
        push   eax
        call   sub_3_1E3B
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        je     loc_3_3A84
loc_3_3A1E:
        mov    ecx, dword ptr [ebp + 0x18]
        lea    ebx, [esi + 0xc]
        test   ecx, ecx
        jne    loc_3_3A30
        mov    dword ptr [ebx], 0
        jmp    loc_3_3A40
loc_3_3A30:
        push   edi
        push   dword ptr [esi + 8]
        push   ecx
        call   sub_3_3D08
        mov    dword ptr [ebx], eax
        test   eax, eax
        je     loc_3_3A72
loc_3_3A40:
        mov    dword ptr [ebp - 0x14], 0
        mov    ecx, dword ptr [ebp - 0x14]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_3A54
        xor    eax, eax
loc_3_3A54:
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + 0x10], eax
        test   eax, eax
        jne    loc_3_3AB8
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     loc_3_3A72
        push   0
        push   eax
        call   sub_1_0028
loc_3_3A6F:
        add    esp, 8
loc_3_3A72:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_3A84
        push   0
        push   eax
        call   sub_1_0028
loc_3_3A81:
        add    esp, 8
loc_3_3A84:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     loc_3_3A96
        push   0
        push   eax
        call   sub_1_0028
loc_3_3A93:
        add    esp, 8
loc_3_3A96:
        push   0
        push   dword ptr [esi]
        call   sub_1_0028
loc_3_3A9F:
        add    esp, 8
loc_3_3AA2:
        push   0
        push   esi
        call   sub_1_0028
loc_3_3AAA:
        add    esp, 8
loc_3_3AAD:
        xor    eax, eax
loc_3_3AAF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_3_3AB8:
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   0x105
        push   dword ptr [eax + 4]
        call   sub_3_20D0
        mov    dword ptr [ebp - 8], 1
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    edi, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esi + 0x14]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    edx, dword ptr [esi + 0x10]
        mov    dword ptr [ebp - 4], edx
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     loc_3_3B0D
        push   0
        push   eax
        call   sub_1_0028
loc_3_3B0A:
        add    esp, 8
loc_3_3B0D:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_3B1F
        push   0
        push   eax
        call   sub_1_0028
loc_3_3B1C:
        add    esp, 8
loc_3_3B1F:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     loc_3_3B31
        push   0
        push   eax
        call   sub_1_0028
loc_3_3B2E:
        add    esp, 8
loc_3_3B31:
        push   0
        push   dword ptr [esi]
        call   sub_1_0028
loc_3_3B3A:
        add    esp, 8
        push   0
        push   esi
        call   sub_1_0028
loc_3_3B45:
        add    esp, 8
        mov    eax, edi
        jmp    loc_3_3AAF
sub_3_3B4F:
        push   ebp
        movzx  eax, word ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        cmp    eax, 1
        push   esi
        push   edi
        je     loc_3_3B98
        cmp    eax, 2
        je     loc_3_3BDF
        cmp    eax, 3
        je     loc_3_3C46
        cmp    eax, 4
        je     loc_3_3C88
        cmp    eax, 5
        je     loc_3_3C9E
        cmp    eax, 6
        je     loc_3_3CC0
        cmp    eax, 7
        je     loc_3_3CE1
        jmp    loc_3_3D00
loc_3_3B98:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     loc_3_3BD8
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0x14]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx - 1]
        jmp    loc_3_3D00
loc_3_3BD8:
        xor    eax, eax
        jmp    loc_3_3D00
loc_3_3BDF:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        test   edx, edx
        je     loc_3_3C3F
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0x7f
        ja     loc_3_3C0E
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    esi, [ecx - 1]
        jmp    loc_3_3C13
loc_3_3C0E:
        mov    esi, 0x7f
loc_3_3C13:
        push   1
        mov    edi, dword ptr [ebp + 0x14]
        push   esi
        push   edx
        push   edi
        call   sub_3_1E10
        lea    eax, [edi + esi]
        cmp    esi, 0x7f
        jae    loc_3_3C31
        mov    byte ptr [eax], 0xd
        mov    byte ptr [eax + 1], 0
        jmp    loc_3_3C38
loc_3_3C31:
        mov    byte ptr [eax - 1], 0xd
        mov    byte ptr [eax], 0
loc_3_3C38:
        mov    eax, esi
        jmp    loc_3_3D00
loc_3_3C3F:
        xor    eax, eax
        jmp    loc_3_3D00
loc_3_3C46:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 4]
        test   edx, edx
        je     loc_3_3C84
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0x14]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx - 1]
        jmp    loc_3_3D00
loc_3_3C84:
        xor    eax, eax
        jmp    loc_3_3D00
loc_3_3C88:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        test   eax, eax
        jne    loc_3_3C96
        xor    eax, eax
        jmp    loc_3_3D00
loc_3_3C96:
        push   eax
        call   sub_3_1F62
        jmp    loc_3_3D00
loc_3_3C9E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        test   eax, eax
        je     loc_3_3CB9
        push   1
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        push   eax
        push   dword ptr [ebp + 0x14]
        call   sub_3_1E10
        jmp    loc_3_3CBC
loc_3_3CB9:
        mov    esi, dword ptr [ebp + 0x10]
loc_3_3CBC:
        mov    eax, esi
        jmp    loc_3_3D00
loc_3_3CC0:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [eax + 0x14], esi
        mov    dword ptr [eax + 0x18], 0
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_3_3D00
loc_3_3CE1:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [eax + 0x14], esi
        mov    dword ptr [eax + 0x18], 1
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_3D00:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_3D08:
        sub    esp, 0x10
        cmp    dword ptr [esp + 0x18], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        jne    loc_3_3D1E
        mov    dword ptr [esp + 0x28], 0     ; -> loc_4_033C
loc_3_3D1E:
        mov    ebx, dword ptr [esp + 0x24]
        push   ebx
        call   sub_3_1F62
        mov    dword ptr [esp + 0x14], eax
        push   0                             ; -> obj4:0x0330
        push   ebx
        call   sub_3_1FCA
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_3_3D4C
        mov    dword ptr [esp + 0x10], 1
        mov    ebp, dword ptr [esp + 0x14]
        dec    ebp
        jmp    loc_3_3D67
loc_3_3D4C:
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        sub    ebp, ebx
        mov    eax, dword ptr [esp + 0x14]
        not    ecx
        sub    eax, ebp
        sub    eax, ecx
        mov    dword ptr [esp + 0x10], eax
loc_3_3D67:
        mov    edi, dword ptr [esp + 0x28]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        mov    edi, dword ptr [esp + 0x2c]
        sub    eax, eax
        not    ecx
        mov    dword ptr [esp + 0x18], ecx
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        mov    eax, dword ptr [esp + 0x18]
        push   0
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 0x20], ecx
        lea    edx, [eax + ecx + 8]
        mov    ecx, dword ptr [esp + 0x14]
        add    ecx, ebp
        add    ecx, edx
        push   ecx
        call   sub_1_0020
loc_3_3DA6:
        add    esp, 8
        test   eax, eax
        je     loc_3_3E23
        mov    edi, eax
        mov    esi, ebx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        lea    edx, [ebp + eax]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0                        ; -> obj4:0x0330
        mov    edi, edx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    edx, 8
        mov    ebp, dword ptr [esp + 0x1c]
        mov    edi, edx
        mov    esi, dword ptr [esp + 0x2c]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x18]
        mov    edi, edx
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [esp + 0x10]
        sub    ebx, ebp
        mov    edi, edx
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, ebp
        shr    ecx, 2
        add    esi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_3E23:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    0xc
        db  000h, 000h, 000h
sub_3_3E30:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x10
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [edx + 0xc]
        call   sub_3_4064
        mov    esp, ebp
        pop    ebp
        ret    0x14
sub_3_3E64:
        push   esi
        push   dword ptr [esp + 8]
        call   sub_3_193C
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        push   dword ptr [eax + 0xc]
        call   sub_3_4201
        test   eax, eax
        mov    eax, 1
        jne    loc_3_3E8C
        xor    eax, eax
        mov    dword ptr [esi], 0
loc_3_3E8C:
        pop    esi
        ret    8
sub_3_3E90:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        jne    loc_3_3EA2
        xor    ax, ax
        jmp    loc_3_3EAA
loc_3_3EA2:
        push   dword ptr [eax + 0xc]
        call   sub_3_4285
loc_3_3EAA:
        ret    4
sub_3_3EAD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   0x10
        push   0x10
        push   0x10
        call   sub_3_1260
        mov    edx, eax
        test   edx, edx
        je     loc_3_3F4F
        mov    dword ptr [ebp - 0x20], 0x10
        mov    ecx, dword ptr [ebp - 0x20]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_3EDC
        xor    eax, eax
loc_3_3EDC:
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [edx + 0x1c], eax
        test   eax, eax
        je     loc_3_3F49
        mov    dword ptr [ebp - 0x18], 0
        mov    ecx, dword ptr [ebp - 0x18]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_3EFD
        xor    eax, eax
loc_3_3EFD:
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [edx + 0x20], eax
        test   eax, eax
        je     loc_3_3F3A
        mov    dword ptr [ebp - 0x10], 0
        mov    ecx, dword ptr [ebp - 0x10]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_3F1E
        xor    eax, eax
loc_3_3F1E:
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [edx + 0x24], eax
        test   eax, eax
        jne    loc_3_3F55
        mov    eax, dword ptr [edx + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_3_3F3A:
        mov    eax, dword ptr [edx + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_3_3F49:
        push   edx
        call   sub_3_1576
loc_3_3F4F:
        xor    eax, eax
loc_3_3F51:
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_3F55:
        mov    dword ptr [edx + 0x28], 0
        mov    eax, edx
        jmp    loc_3_3F51
sub_3_3F60:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   ecx
        call   sub_3_1576
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_3FA3:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x24
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [edx + 0x10]
        cmp    dword ptr [edx + 0xc], eax
        jne    loc_3_404F
        mov    eax, dword ptr [edx + 0x28]
        test   al, 2
        je     loc_3_401C
        and    al, 0xfd
        mov    ecx, dword ptr [edx + 0x18]
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [ebp - 0x20], ecx
        or     al, 0x10
        mov    dword ptr [edx + 0x28], eax
        or     al, 0x20
        mov    dword ptr [edx + 0x28], eax
        mov    eax, dword ptr [ebp - 0x20]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, dword ptr [edx + 0x20]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    dword ptr [ebp - 0x14], 1
        mov    eax, dword ptr [edx + 0x24]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        jmp    loc_3_405E
loc_3_401C:
        test   al, 0x10
        je     loc_3_404F
        or     al, 0x20
        mov    ecx, dword ptr [edx + 0x18]
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edx + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        jmp    loc_3_405E
loc_3_404F:
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_405E:
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4064:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        mov    dword ptr [ebp - 4], 1
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [edi]
        cmp    edx, 8
        jne    loc_3_409E
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        or     byte ptr [esi + 0x28], 4
        jmp    loc_3_41C5
loc_3_409E:
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], 1
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x18]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        cmp    edx, 1
        jne    loc_3_4198
        lea    eax, [ebp - 0x30]
        push   eax
        push   esi
        call   sub_3_149E
        test   eax, eax
        je     loc_3_4198
        cmp    dword ptr [ebp - 0x30], 1
        jne    loc_3_4198
        movsx  edx, word ptr [ebp - 0x26]
        movsx  ebx, word ptr [edi + 0xa]
        lea    ecx, [edx - 1]
        cmp    ebx, ecx
        jl     loc_3_410B
        movsx  eax, word ptr [ebp - 0x22]
        inc    eax
        cmp    eax, ebx
        jge    loc_3_4135
loc_3_410B:
        movsx  eax, word ptr [edi + 0xe]
        cmp    eax, ecx
        jl     loc_3_411C
        movsx  ecx, word ptr [ebp - 0x22]
        inc    ecx
        cmp    ecx, eax
        jge    loc_3_4135
loc_3_411C:
        dec    ebx
        cmp    ebx, edx
        jg     loc_3_4128
        lea    ecx, [eax + 1]
        cmp    ecx, edx
        jge    loc_3_4135
loc_3_4128:
        movsx  ecx, word ptr [ebp - 0x22]
        cmp    ecx, ebx
        jl     loc_3_4198
        inc    eax
        cmp    eax, edx
        jl     loc_3_4198
loc_3_4135:
        mov    ax, word ptr [edi + 8]
        mov    cx, word ptr [ebp - 0x28]
        cmp    ax, cx
        jl     loc_3_4145
        mov    ax, cx
loc_3_4145:
        mov    cx, word ptr [edi + 0xa]
        mov    word ptr [ebp - 0x28], ax
        mov    ax, word ptr [ebp - 0x26]
        cmp    ax, cx
        jl     loc_3_4159
        mov    ax, cx
loc_3_4159:
        mov    cx, word ptr [ebp - 0x24]
        mov    word ptr [ebp - 0x26], ax
        mov    ax, word ptr [edi + 0xc]
        cmp    ax, cx
        jg     loc_3_416D
        mov    ax, cx
loc_3_416D:
        mov    cx, word ptr [ebp - 0x22]
        mov    word ptr [ebp - 0x24], ax
        mov    ax, word ptr [edi + 0xe]
        cmp    ax, cx
        jg     loc_3_4181
        mov    ax, cx
loc_3_4181:
        mov    word ptr [ebp - 0x22], ax
        lea    eax, [ebp - 0x30]
        push   eax
        push   esi
        call   sub_3_14F8
        mov    dword ptr [ebp - 4], 0
        jmp    loc_3_41B0
loc_3_4198:
        test   byte ptr [esi + 0x28], 0x40
        je     loc_3_41A7
        mov    dword ptr [ebp - 4], 0
        jmp    loc_3_41B0
loc_3_41A7:
        push   edi
        push   1
        push   esi
        call   sub_3_130B
loc_3_41B0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_41C5
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_41C5:
        mov    eax, dword ptr [esi + 0x28]
        test   al, 0x10
        je     loc_3_41E9
        and    al, 0xef
        mov    dword ptr [esi + 0x28], eax
        test   al, 0x20
        je     loc_3_41E9
        and    al, 0xdf
        mov    ecx, dword ptr [esi + 0x24]
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_41E9:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_4201:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_3FA3
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        je     loc_3_424C
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    ecx, 1
        jmp    loc_3_426C
loc_3_424C:
        mov    eax, dword ptr [esi + 0x28]
        test   al, 4
        je     loc_3_4265
        mov    ecx, 1
        and    al, 0xfb
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [edi], 8
        jmp    loc_3_426C
loc_3_4265:
        and    al, 0xfe
        xor    ecx, ecx
        mov    dword ptr [esi + 0x28], eax
loc_3_426C:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    eax, ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_4285:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        xor    si, si
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_3_3FA3
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
loc_3_42B0:
        push   ebx
        push   edi
        call   sub_3_153B
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_42C2
        or     si, word ptr [ebx]
        jmp    loc_3_42B0
loc_3_42C2:
        test   si, si
        jne    loc_3_42CB
        and    byte ptr [edi + 0x28], 0xfe
loc_3_42CB:
        mov    eax, dword ptr [edi + 0x28]
        test   al, 4
        je     loc_3_42DB
        or     si, 8
        and    al, 0xfb
        mov    dword ptr [edi + 0x28], eax
loc_3_42DB:
        mov    eax, dword ptr [edi + 0x28]
        test   al, 8
        je     loc_3_42EB
        or     si, 0x20
        and    al, 0xf7
        mov    dword ptr [edi + 0x28], eax
loc_3_42EB:
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4306:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        push   esi
        mov    esi, dword ptr [edx + 0xc]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [esi + 0x28]
        test   al, 1
        jne    loc_3_4353
        or     al, 1
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   0
        push   0x41a
        push   dword ptr [edx]
        call   sub_3_0B98
        jmp    loc_3_4362
loc_3_4353:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_4362:
        mov    eax, dword ptr [esi + 0x28]
        test   al, 0x80
        je     loc_3_4388
        mov    dword ptr [ebp - 4], 1
        or     ah, 1
        mov    ecx, dword ptr [esi + 0x20]
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
loc_3_4388:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_438F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        mov    edx, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x14], ecx
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [edx + 0x10]
        cmp    dword ptr [edx + 0xc], eax
        je     loc_3_43E6
        or     byte ptr [edx + 0x28], 2
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edx + 0x20]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        jmp    loc_3_4402
loc_3_43E6:
        mov    eax, dword ptr [edx + 0x28]
        mov    ecx, dword ptr [edx + 0x18]
        and    al, 0xfd
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [edx + 0x28], eax
        or     al, 0x10
        mov    dword ptr [edx + 0x28], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_4402:
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4408:
        push   esi
        mov    edx, 0                        ; -> loc_4_01F0
        xor    esi, esi
        mov    ecx, dword ptr [esp + 8]
loc_3_4414:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     loc_3_4425
        cmp    dword ptr [eax], ecx
        jne    loc_3_4425
        mov    eax, dword ptr [eax + 0xc]
        test   eax, eax
        jne    loc_3_4430
loc_3_4425:
        inc    esi
        add    edx, 4
        cmp    esi, 0x40
        jl     loc_3_4414
        jmp    loc_3_4434
loc_3_4430:
        or     byte ptr [eax + 0x28], 8
loc_3_4434:
        pop    esi
        ret    4
sub_3_4438:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        test   eax, eax
        je     loc_3_455B
        movzx  edx, word ptr [ebp + 0xc]
        cmp    edx, 3
        je     loc_3_454B
        cmp    edx, 4
        je     loc_3_4551
        cmp    edx, 7
        je     loc_3_455B
        cmp    edx, 8
        je     loc_3_4557
        mov    esi, dword ptr [eax + 0xc]
        test   esi, esi
        je     loc_3_455B
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        cmp    edx, 1
        je     loc_3_44AF
        cmp    edx, 2
        je     loc_3_4503
        cmp    edx, 5
        je     loc_3_4512
        cmp    edx, 6
        je     loc_3_4518
        jmp    loc_3_453A
loc_3_44AF:
        test   byte ptr [esi + 0x28], 0x40
        jne    loc_3_455B
        lea    eax, [ebp - 0x24]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        je     loc_3_44E8
loc_3_44C9:
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        lea    eax, [ebp - 0x24]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        jne    loc_3_44C9
loc_3_44E8:
        or     byte ptr [esi + 0x28], 0x40
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   esi
        call   sub_3_3FA3
        jmp    loc_3_455B
loc_3_4503:
        mov    eax, dword ptr [esi + 0x28]
        and    al, 0xbf
        mov    dword ptr [esi + 0x28], eax
        and    al, 0xfe
        mov    dword ptr [esi + 0x28], eax
        jmp    loc_3_453A
loc_3_4512:
        or     byte ptr [esi + 0x28], 0x80
        jmp    loc_3_453A
loc_3_4518:
        mov    eax, dword ptr [esi + 0x28]
        and    al, 0x7f
        test   ah, 1
        mov    dword ptr [esi + 0x28], eax
        je     loc_3_453A
        and    ah, 0xfe
        mov    ecx, dword ptr [esi + 0x20]
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_453A:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        jmp    loc_3_455B
loc_3_454B:
        and    byte ptr [eax + 0x1c], 0xef
        jmp    loc_3_455B
loc_3_4551:
        or     byte ptr [eax + 0x1c], 0x10
        jmp    loc_3_455B
loc_3_4557:
        and    byte ptr [eax + 0x1c], 0xdf
loc_3_455B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_4562:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    esi, dword ptr [eax + 0xc]
        mov    edx, eax
        and    byte ptr [esi + 0x28], 0xfe
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    eax, dword ptr [esi + 0x10]
        cmp    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [esi + 0x18]
        je     loc_3_45AD
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   edx
        call   sub_3_4306
        jmp    loc_3_45B9
loc_3_45AD:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_45B9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_45C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        lea    eax, [ebp - 0x20]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        je     loc_3_4611
loc_3_45F2:
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        lea    eax, [ebp - 0x20]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        jne    loc_3_45F2
loc_3_4611:
        test   byte ptr [esi + 0x28], 0x20
        je     loc_3_462A
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        and    byte ptr [esi + 0x28], 0xdf
loc_3_462A:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4640:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        test   esi, esi
        jne    loc_3_4650
        mov    esi, 0                        ; -> loc_4_0340
loc_3_4650:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        push   0
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        mov    ebx, ecx
        call   sub_1_0020
loc_3_4667:
        add    esp, 8
        test   eax, eax
        jne    loc_3_4672
        xor    eax, eax
        jmp    loc_3_468D
loc_3_4672:
        mov    edi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, 1
loc_3_468D:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_3_4693:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   1
        push   0x4c
        call   sub_1_0020
loc_3_46A5:
        add    esp, 8
        mov    ebx, eax
        xor    ecx, ecx
        test   ebx, ebx
        je     loc_3_4787
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [ebx], 0x52444952
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ebx + 0x48], eax
        mov    dword ptr [ebp - 0x18], ecx
        mov    ecx, dword ptr [ebp - 0x18]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_46D3
        xor    eax, eax
loc_3_46D3:
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebx + 8], eax
        test   eax, eax
        je     loc_3_477C
        mov    dword ptr [ebp - 0x10], 0
        mov    ecx, dword ptr [ebp - 0x10]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_46F8
        xor    eax, eax
loc_3_46F8:
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0xc], eax
        test   eax, eax
        je     loc_3_476D
        push   dword ptr [ebp + 8]
        lea    esi, [ebx + 0x20]
        push   esi
        call   sub_3_4640
        test   eax, eax
        je     loc_3_475E
        push   dword ptr [ebp + 0xc]
        lea    edi, [ebx + 0x24]
        push   edi
        call   sub_3_4640
        test   eax, eax
        je     loc_3_4752
        push   dword ptr [ebp + 0x10]
        lea    ecx, [ebx + 0x28]
        push   ecx
        call   sub_3_4640
        test   eax, eax
        je     loc_3_4746
        lea    edi, [ebx + 0x2c]
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, ebx
        jmp    loc_3_4789
loc_3_4746:
        push   0
        push   dword ptr [edi]
        call   sub_1_0028
loc_3_474F:
        add    esp, 8
loc_3_4752:
        push   0
        push   dword ptr [esi]
        call   sub_1_0028
loc_3_475B:
        add    esp, 8
loc_3_475E:
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_3_476D:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_3_477C:
        push   0
        push   ebx
        call   sub_1_0028
loc_3_4784:
        add    esp, 8
loc_3_4787:
        xor    eax, eax
loc_3_4789:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
sub_3_4792:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0x28]
        mov    dword ptr [esi], 0
        call   sub_1_0028
loc_3_47AC:
        add    esp, 8
        push   0
        push   dword ptr [esi + 0x24]
        call   sub_1_0028
loc_3_47B9:
        add    esp, 8
        push   0
        push   dword ptr [esi + 0x20]
        call   sub_1_0028
loc_3_47C6:
        add    esp, 8
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   0
        push   esi
        call   sub_1_0028
loc_3_47EF:
        add    esp, 8
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_47F9:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4831:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], 1
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_485A:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ecx + 4]
        test   al, 2
        je     loc_3_4873
        push   ecx
        call   sub_3_4792
        jmp    loc_3_4887
loc_3_4873:
        or     al, 1
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_4887:
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_488D:
        push   ebp
        mov    eax, 0x80000002
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 4], 1
        jne    loc_3_4924
        mov    ecx, dword ptr [edx + 0x48]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    edi, dword ptr [edx + 0x20]
        mov    dword ptr [eax], ecx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [edx + 0x24]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0x10]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [edx + 0x28]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0x14]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [edx + 0x2c]
        mov    edi, dword ptr [ebp + 0x18]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
loc_3_4924:
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
sub_3_492A:
        mov    edx, dword ptr [esp + 4]
        push   edi
        test   byte ptr [edx + 4], 1
        je     loc_3_493C
        mov    eax, 0x80000002
        jmp    loc_3_4984
loc_3_493C:
        mov    edi, dword ptr [edx + 0x20]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        cmp    ecx, 0xffff
        jbe    loc_3_4959
        mov    eax, 0x80000001
        jmp    loc_3_4984
loc_3_4959:
        mov    eax, dword ptr [esp + 0xc]
        mov    edi, dword ptr [edx + 0x24]
        mov    word ptr [eax], cx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, 0x80000001
        cmp    ecx, 0xffff
        ja     loc_3_4984
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax], cx
        xor    eax, eax
loc_3_4984:
        pop    edi
        ret    0xc
sub_3_4988:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     loc_3_499F
        mov    eax, 0x80000002
        jmp    loc_3_49FC
loc_3_499F:
        mov    al, byte ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 0x12], cx
        mov    edx, dword ptr [ebp + 0x10]
        mov    byte ptr [esi + 0x10], al
        mov    dword ptr [esi + 0x14], edx
        mov    byte ptr [esi + 0x11], 1
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    edx, dword ptr [0]            ; -> obj1:0x009C
        test   edx, edx
        je     loc_3_49D5
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_3_49D5:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        test   edx, edx
        je     loc_3_49F9
        push   1
        call   sub_1_0030
loc_3_49F9:
        mov    eax, dword ptr [esi + 0x18]
loc_3_49FC:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_4A03:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     loc_3_4A1A
        mov    eax, 0x80000002
        jmp    loc_3_4A77
loc_3_4A1A:
        mov    al, byte ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 0x12], cx
        mov    edx, dword ptr [ebp + 0x10]
        mov    byte ptr [esi + 0x10], al
        mov    dword ptr [esi + 0x14], edx
        mov    byte ptr [esi + 0x11], 5
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    edx, dword ptr [0]            ; -> obj1:0x009C
        test   edx, edx
        je     loc_3_4A50
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_3_4A50:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        test   edx, edx
        je     loc_3_4A74
        push   1
        call   sub_1_0030
loc_3_4A74:
        mov    eax, dword ptr [esi + 0x18]
loc_3_4A77:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_4A7E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     loc_3_4A95
        mov    eax, 0x80000002
        jmp    loc_3_4AF2
loc_3_4A95:
        mov    al, byte ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 0x12], cx
        mov    edx, dword ptr [ebp + 0x10]
        mov    byte ptr [esi + 0x10], al
        mov    dword ptr [esi + 0x14], edx
        mov    byte ptr [esi + 0x11], 2
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    edx, dword ptr [0]            ; -> obj1:0x009C
        test   edx, edx
        je     loc_3_4ACB
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_3_4ACB:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        test   edx, edx
        je     loc_3_4AEF
        push   1
        call   sub_1_0030
loc_3_4AEF:
        mov    eax, dword ptr [esi + 0x18]
loc_3_4AF2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_3_4AF9:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 4], 1
        je     loc_3_4B0A
        mov    eax, 0x80000002
        jmp    loc_3_4B30
loc_3_4B0A:
        mov    ecx, dword ptr [esp + 8]
        test   ecx, ecx
        je     loc_3_4B1E
        cmp    ecx, 1
        je     loc_3_4B24
        cmp    ecx, 2
        je     loc_3_4B2A
        jmp    loc_3_4B2E
loc_3_4B1E:
        inc    word ptr [eax + 0x2e]
        jmp    loc_3_4B2E
loc_3_4B24:
        inc    word ptr [eax + 0x36]
        jmp    loc_3_4B2E
loc_3_4B2A:
        inc    word ptr [eax + 0x3e]
loc_3_4B2E:
        xor    eax, eax
loc_3_4B30:
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     loc_3_4B4A
        mov    eax, 0x80000002
        jmp    loc_3_4BAF
loc_3_4B4A:
        mov    ecx, dword ptr [ebp + 0xc]
        test   ecx, ecx
        je     loc_3_4BB6
        cmp    ecx, 1
        je     loc_3_4BC5
        cmp    ecx, 2
        je     loc_3_4BD4
        mov    eax, dword ptr [ebp - 0x10]
loc_3_4B5E:
        test   eax, eax
        jne    loc_3_4BAC
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [esi + 0x10], cl
        mov    byte ptr [esi + 0x11], 4
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        mov    edx, dword ptr [0]            ; -> obj1:0x009C
        test   edx, edx
        je     loc_3_4B88
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_3_4B88:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        test   edx, edx
        je     loc_3_4BAC
        push   1
        call   sub_1_0030
loc_3_4BAC:
        mov    eax, dword ptr [esi + 0x18]
loc_3_4BAF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_3_4BB6:
        mov    ax, word ptr [esi + 0x2e]
        dec    ax
        mov    word ptr [esi + 0x2e], ax
        movzx  eax, ax
        jmp    loc_3_4B5E
loc_3_4BC5:
        mov    ax, word ptr [esi + 0x36]
        dec    ax
        mov    word ptr [esi + 0x36], ax
        movzx  eax, ax
        jmp    loc_3_4B5E
loc_3_4BD4:
        mov    ax, word ptr [esi + 0x3e]
        dec    ax
        mov    word ptr [esi + 0x3e], ax
        movzx  eax, ax
        jmp    loc_3_4B5E
sub_3_4BE6:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ecx + 4]
        test   al, 1
        je     loc_3_4BFF
        push   ecx
        call   sub_3_4792
        jmp    loc_3_4C17
loc_3_4BFF:
        mov    byte ptr [ecx + 0x11], 3
        or     al, 2
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_3_4C17:
        call   sub_9_0276
loc_3_4C1C:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_4C24:
        push   ebx
        cmp    byte ptr [0x3c0], 0           ; -> obj4:0x03C0
        jne    loc_3_4C57
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_3_4C5E
        push   0x358                         ; -> loc_4_0358
        push   0x22
        push   0                             ; -> loc_9_0000
        VxDCall  IFSMgr.IFSMgr_RegisterCFSD      ; cd2062004000
        add    esp, 0xc
        cmp    eax, 0
        jl     loc_3_4C5E
        mov    byte ptr [0x3c0], 1           ; -> obj4:0x03C0
loc_3_4C57:
        mov    eax, 1
loc_3_4C5C:
        pop    ebx
        ret    
loc_3_4C5E:
        xor    eax, eax
        jmp    loc_3_4C5C
sub_3_4C62:
        VxDCall  IFSMgr.IFSMgr_FSDUnmountCFSD    ; cd2050804000
sub_3_4C68:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [0x1e6]        ; -> loc_4_01E6
        add    eax, ebx
        pop    ebx
        ret    
sub_3_4C78:
        push   ebx
        VxDCall  PAGEFILE.PageFile_Get_Version   ; cd2000002100
        cmp    bl, 2
        je     loc_3_4C88
        xor    eax, eax
loc_3_4C86:
        pop    ebx
        ret    
loc_3_4C88:
        mov    eax, 1
        jmp    loc_3_4C86
loc_3_4C8F:
        xchg   dword ptr [esp + 4], esi
        mov    word ptr [esi + 0x1a], 0x1f
        mov    eax, 0x1f
        xchg   dword ptr [esp + 4], esi
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x3c4  RW-
        db  035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_3_03B8               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_03CC               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_3_03E0               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_03F4               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_0408               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_041C               ; fixup
        db  005h, 000h, 000h, 000h
        dd  loc_3_0430               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_0444               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_0458               ; fixup
        db  005h, 000h, 000h, 000h
        dd  loc_3_046C               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_0480               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_0494               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_04A8               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_04BC               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_04D0               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_04E4               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_04F8               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_050C               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_0520               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_3_0534               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_0548               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_055C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_0570               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_0584               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_3_0598               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_05AC               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_05C0               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_05D4               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_05E8               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_05FC               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_0610               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_0624               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_0638               ; fixup
        db  006h, 000h, 000h, 000h
        dd  loc_3_064C               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_3_0660               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_3_0674               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_3_0688               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_069C               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_3_06B0               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_06C4               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_06D8               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_06EC               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_0700               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_0714               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_0728               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_073C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_0750               ; fixup
        db  005h, 000h, 000h, 000h
        dd  loc_3_0764               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_0778               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_078C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_07A0               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_3_07B4               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_3_07C8               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_4_01E2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_01E6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_01EA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_01F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_02F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_02F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_02F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_02FC:
        cmp    eax, 0x583d3a58
        cmp    bl, byte ptr [eax + eax + 0x56]
        inc    ebx
        dec    edi
        dec    esi
        inc    esp
        cmp    ah, byte ptr [eax]
        inc    ebx
        jne    0x380
        jb     loc_4_0375
        outsb  dx, byte ptr [esi]
        je     loc_4_0333
        inc    esp
        imul   esi, dword ptr [edx + 0x65], 0x726f7463
        jns    0x33d
        inc    edi
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    dword ptr gs:[eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dl, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4d], al
        inc    esp
loc_4_0333:
        dec    esp
        dec    ecx
        dec    esi
        inc    ebp
        cmp    eax, 0
loc_4_033C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0340:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0344:
        adc    al, byte ptr [eax]
        push   eax
        add    byte ptr [ecx], cl
        push   eax
        add    byte ptr [ebp], al
        push   ebx
        add    byte ptr [eax + eax + 0x44], dl
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
loc_4_0358:
        inc    esp
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        jg     loc_3_4C8F
        dec    esp
        add    byte ptr [eax], al
        dd  loc_3_4C8F               ; fixup
        dd  loc_3_4C8F               ; fixup
        dd  loc_3_4C8F               ; fixup
loc_4_0375:
        dec    esp
        add    byte ptr [eax], al
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [edi - 0x70ffffb4], cl ; -> loc_3_4C8F
        dec    esp
        add    byte ptr [eax], al
        dd  loc_3_4C8F               ; fixup
        dd  loc_9_0235               ; fixup
        dd  loc_3_4C8F               ; fixup
        dd  loc_9_019A               ; fixup
        dd  loc_3_4C8F               ; fixup
        dd  loc_3_4C8F               ; fixup
        dd  loc_3_4C8F               ; fixup
        db  00Ah, 003h, 010h, 008h
        dd  loc_3_4C8F               ; fixup
        dd  loc_9_00A4               ; fixup
        dd  loc_3_4C8F               ; fixup
        dd  loc_3_4C8F               ; fixup
        dd  loc_3_4C8F               ; fixup
        dd  loc_9_00CD               ; fixup
        dd  loc_3_4C8F               ; fixup
        db  000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x24  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 015h, 0B4h, 062h, 0CDh, 021h, 08Bh, 0D3h, 066h, 0C1h, 0E2h, 010h
        db  0BAh, 050h, 000h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 0B8h, 001h, 080h, 033h, 0DBh
        db  033h, 0F6h, 0C3h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x1b6  RW-
        db  02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h
        db  020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h
        db  04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h
        db  06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h, 020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch
        db  074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h, 04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h
        db  02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h
        db  020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h
        db  04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h
        db  06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h, 020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch
        db  074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h, 04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h
        db  02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h
        db  020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h
        db  04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h
        db  06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h, 020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch
        db  074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h, 04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h
        db  02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h
        db  020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h
        db  04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h
        db  06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h, 020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch
        db  074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h, 04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h
        db  02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h
        db  020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h
        db  04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h
        db  06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h, 020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch
        db  074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h, 04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h
        db  02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h
        db  020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h
        db  04Eh, 041h, 04Dh, 045h, 053h, 020h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0x510  RW-
        dd  sub_3_130B               ; fixup
        db  0AFh, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 003h, 000h, 00Bh, 014h
        dd  sub_3_13BA               ; fixup
        db  0BDh, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 003h, 000h, 00Bh, 014h
        dd  sub_3_1477               ; fixup
        db  027h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_149E               ; fixup
        db  033h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_14D1               ; fixup
        db  027h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_14F8               ; fixup
        db  033h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_152B               ; fixup
        db  010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_153B               ; fixup
        db  03Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 005h, 001h
        dd  loc_3_15AE               ; fixup
        db  035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 002h, 002h
        dd  sub_3_15E3               ; fixup
        db  026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_169F               ; fixup
        db  09Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 00Bh, 002h
        dd  sub_3_173E               ; fixup
        db  0FAh, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 003h, 000h, 007h, 014h
        dd  sub_3_1838               ; fixup
        db  08Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 006h, 001h
        dd  sub_3_18C6               ; fixup
        db  076h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 007h, 003h
        dd  sub_3_193C               ; fixup
        db  02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 005h, 001h
        dd  sub_3_1A4B               ; fixup
        db  021h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  sub_3_1B57               ; fixup
        db  033h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 002h, 002h
        dd  sub_3_1B8A               ; fixup
        db  05Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 00Bh, 014h
        dd  sub_3_1D62               ; fixup
        db  034h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_1E3B               ; fixup
        db  04Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 00Bh, 002h
        dd  sub_3_1E87               ; fixup
        db  029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  sub_3_1EB0               ; fixup
        db  031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  sub_3_1EE1               ; fixup
        db  03Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 006h, 001h
        dd  sub_3_1F1E               ; fixup
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  loc_3_1F21               ; fixup
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_1F24               ; fixup
        db  03Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 003h, 001h
        dd  sub_3_1F62               ; fixup
        db  033h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 008h, 002h
        dd  sub_3_1F95               ; fixup
        db  035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 001h, 001h
        dd  sub_3_1FCA               ; fixup
        db  059h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 00Ah, 014h
        dd  loc_3_2142               ; fixup
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_3_2145               ; fixup
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_3_2148               ; fixup
        db  030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 003h, 001h
        dd  sub_3_2394               ; fixup
        db  027h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  sub_3_23BB               ; fixup
        db  01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_25AF               ; fixup
        db  016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_2745               ; fixup
        db  034h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  sub_3_27D5               ; fixup
        db  087h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 003h, 000h, 00Ah, 014h
        dd  sub_3_285C               ; fixup
        db  01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_2981               ; fixup
        db  025h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_29A6               ; fixup
        db  058h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 007h, 003h
        dd  sub_3_2ACA               ; fixup
        db  055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 007h, 003h
        dd  sub_3_2D5C               ; fixup
        db  03Ch, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 004h, 001h
        dd  sub_3_2D98               ; fixup
        db  050h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 007h, 003h
        dd  sub_3_2DE8               ; fixup
        db  018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_2E00               ; fixup
        db  02Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_2EF7               ; fixup
        db  019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  sub_3_2F10               ; fixup
        db  01Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_2F2D               ; fixup
        db  02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 001h, 001h
        dd  sub_3_2F57               ; fixup
        db  01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_2FF6               ; fixup
        db  01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_3010               ; fixup
        db  02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  sub_3_303F               ; fixup
        db  00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_304B               ; fixup
        db  039h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 001h, 001h
        dd  sub_3_3084               ; fixup
        db  00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_34D6               ; fixup
        db  091h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 003h, 000h, 007h, 014h
        dd  sub_3_3567               ; fixup
        db  011h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_3578               ; fixup
        db  011h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_3589               ; fixup
        db  018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_3_3645               ; fixup
        db  052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 001h
        dd  sub_3_3697               ; fixup
        db  019h, 002h, 000h, 000h, 002h, 000h, 000h, 000h, 001h, 000h, 00Bh, 014h
        dd  sub_3_38B0               ; fixup
        db  00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_38BF               ; fixup
        db  00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_393F               ; fixup
        db  029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  sub_3_3968               ; fixup
        db  03Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 008h, 002h
        dd  sub_3_3D08               ; fixup
        db  025h, 001h, 000h, 000h, 004h, 000h, 000h, 000h, 003h, 000h, 00Ch, 014h
        dd  sub_3_3E64               ; fixup
        db  02Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 001h, 001h
        dd  sub_3_3E90               ; fixup
        db  01Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_3_4408               ; fixup
        db  030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 006h, 001h
        dd  sub_3_4640               ; fixup
        db  053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 007h, 003h
        dd  sub_3_492A               ; fixup
        db  05Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 005h, 001h
        dd  sub_3_4AF9               ; fixup
        db  03Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  loc_9_0000               ; fixup
        db  04Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 008h, 001h
        dd  loc_9_004C               ; fixup
        db  058h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  loc_9_00A4               ; fixup
        db  029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  loc_9_00CD               ; fixup
        db  053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  loc_9_0120               ; fixup
        db  03Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  loc_9_015D               ; fixup
        db  03Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 001h
        dd  loc_9_019A               ; fixup
        db  09Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 00Bh, 014h
        dd  loc_9_0235               ; fixup
        db  00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_9_0242               ; fixup
        db  034h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 001h, 001h
        dd  sub_9_0276               ; fixup
        db  026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 8  vsize=0x4  RW-
        db  04Ch, 001h, 005h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 9  vsize=0x29c  R-X
loc_9_0000:
        cmp    dword ptr [0], 0              ; -> obj1:0x0094
        push   edi
        mov    edi, dword ptr [esp + 8]
        je     loc_9_0016
        mov    word ptr [edi + 0x1a], 0x11c
        jmp    loc_9_0046
loc_9_0016:
        mov    dword ptr [edi + 0x10], 0     ; -> obj4:0x0360
        mov    dword ptr [edi + 0x1c], 0x50495045
        mov    dword ptr [0], 0              ; -> obj1:0x0098
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [0], eax            ; -> obj1:0x0094
        call   sub_3_4C78
loc_9_003B:
        mov    word ptr [edi + 0x1a], 0
        mov    dword ptr [0], eax            ; -> obj1:0x009C
loc_9_0046:
        movsx  eax, word ptr [edi + 0x1a]
        pop    edi
        ret    
loc_9_004C:
        push   esi
        call   sub_3_4C68
loc_9_0052:
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        jne    loc_9_0064
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 0x1a], 1
        jmp    loc_9_009E
loc_9_0064:
        mov    esi, dword ptr [esp + 8]
        inc    dword ptr [0]                 ; -> obj1:0x0098
        push   dword ptr [ecx + 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 0x10]
        push   dword ptr [ecx]
        mov    dword ptr [eax + 4], 0        ; -> loc_9_015D
        mov    dword ptr [eax + 8], 0        ; -> obj4:0x03A0
        mov    dword ptr [eax], 0            ; -> loc_9_0120
        call   sub_3_4AF9
loc_9_0098:
        mov    word ptr [esi + 0x1a], 0
loc_9_009E:
        movsx  eax, word ptr [esi + 0x1a]
        pop    esi
        ret    
loc_9_00A4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 4], 2
        mov    eax, dword ptr [esi + 0x20]
        jne    loc_9_00C3
        push   0
        dec    dword ptr [0]                 ; -> obj1:0x0098
        push   eax
        call   sub_1_0028
loc_9_00C0:
        add    esp, 8
loc_9_00C3:
        mov    word ptr [esi + 0x1a], 0
        xor    eax, eax
        pop    esi
        ret    
loc_9_00CD:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 0x1a], 1
        cmp    byte ptr [esi + 4], 0x23
        jne    loc_9_011A
        mov    edx, dword ptr [esi]
        cmp    edx, 1
        jne    loc_9_011A
        mov    ecx, dword ptr [esi + 0x20]
        push   dword ptr [ecx + 4]
        push   dword ptr [esi + 0x14]
        push   edx
        push   dword ptr [ecx]
        call   sub_3_4A03
loc_9_00F6:
        test   eax, 0x80000000
        jne    loc_9_010E
        mov    dword ptr [esi + 0x24], 0xcccccccc
        mov    word ptr [esi + 0x1a], 0
        mov    dword ptr [esi], eax
        jmp    loc_9_011A
loc_9_010E:
        mov    word ptr [esi + 0x1a], 1
        mov    dword ptr [esi], 0
loc_9_011A:
        movsx  eax, word ptr [esi + 0x1a]
        pop    esi
        ret    
loc_9_0120:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 0x20]
        push   dword ptr [ecx + 4]
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi]
        push   dword ptr [ecx]
        call   sub_3_4988
loc_9_0137:
        test   eax, 0x80000000
        jne    loc_9_014A
        mov    dword ptr [esi], eax
        add    esi, 0x1a
        mov    word ptr [esi], 0
        jmp    loc_9_0158
loc_9_014A:
        mov    dword ptr [esi], 0
        add    esi, 0x1a
        mov    word ptr [esi], 1
loc_9_0158:
        movsx  eax, word ptr [esi]
        pop    esi
        ret    
loc_9_015D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 0x20]
        push   dword ptr [ecx + 4]
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi]
        push   dword ptr [ecx]
        call   sub_3_4A7E
loc_9_0174:
        test   eax, 0x80000000
        jne    loc_9_0187
        mov    dword ptr [esi], eax
        add    esi, 0x1a
        mov    word ptr [esi], 0
        jmp    loc_9_0195
loc_9_0187:
        mov    dword ptr [esi], 0
        add    esi, 0x1a
        mov    word ptr [esi], 1
loc_9_0195:
        movsx  eax, word ptr [esi]
        pop    esi
        ret    
loc_9_019A:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [edx + 0x28]
        mov    ebx, dword ptr [edx + 0x20]
        mov    al, byte ptr [edx + 4]
        test   al, al
        jne    loc_9_021A
        mov    word ptr [esi + 0x14], 0x80a0
        mov    ecx, dword ptr [ebx + 4]
        test   ecx, ecx
        jne    loc_9_01C2
        mov    word ptr [esi + 0x14], 0x80a1
loc_9_01C2:
        cmp    ecx, 1
        jne    loc_9_01CB
        or     byte ptr [esi + 0x14], 0x42
loc_9_01CB:
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, edi
        and    eax, 0xffff0000
        cmp    eax, 0x50490000
        jne    loc_9_0224
        mov    eax, dword ptr [esi + 0x10]
        mov    ebp, eax
        and    ebp, 0xffff0000
        cmp    ebp, 0x50450000
        jne    loc_9_0224
        or     edi, 0x20200000
        or     eax, 0x20200000
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [esi + 0x1c], edi
        shl    ecx, 0x10
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 0x18], ebx
        and    eax, 0xffff
        or     ecx, eax
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [esi + 0x14], ecx
        jmp    loc_9_0224
loc_9_021A:
        cmp    al, 0xa
        jne    loc_9_0224
        mov    word ptr [esi + 0x14], 0x10e0
loc_9_0224:
        and    byte ptr [esi + 0x2c], 0xfe
        mov    word ptr [edx + 0x1a], 0
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_9_0235:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0
        xor    eax, eax
        ret    
sub_9_0242:
        push   esi
        push   1
        push   0xc
        call   sub_1_0020
loc_9_024C:
        add    esp, 8
        mov    esi, eax
        call   sub_3_4C68
loc_9_0256:
        test   esi, esi
        je     loc_9_026D
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [eax], esi
        xor    eax, eax
        jmp    loc_9_0272
loc_9_026D:
        mov    eax, 0x80000000
loc_9_0272:
        pop    esi
        ret    8
sub_9_0276:
        cmp    dword ptr [0], 0              ; -> obj1:0x0098
        jne    loc_9_029B
        mov    eax, dword ptr [0]            ; -> obj1:0x0094
        test   eax, eax
        je     loc_9_029B
        push   eax
        call   sub_3_4C62
loc_9_028E:
        mov    dword ptr [0], 0              ; -> obj1:0x0094
        add    esp, 4
loc_9_029B:
        ret    
VxD__LTEXT ENDS
