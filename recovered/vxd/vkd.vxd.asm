; VKD -- reconstructed 32-bit disassembly (VxD / LE format)
; 10 objects, 3768 instructions recovered by recursive descent

Declare_Virtual_Device VKD, 2, 10, VKD_Control, 0000Dh, Undefined_Init_Order
;   DDK version 0400h   init_order 038000000h   flags 00000h
;   service table: obj1:0x1168, 21 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1540  R-X
VKD_Control:
        cmp    eax, 0x13
        jae    loc_1_000C
        jmp    dword ptr [eax*4 + 0xe]       ; -> obj1:0x000E
loc_1_000C:
        clc    
        ret    
        ; ---- dispatch table (19 entries) ----
        dd  loc_3_0018
        dd  loc_3_01CB
        dd  loc_3_037A
        dd  loc_7_0083
        dd  loc_5_0000
        dd  loc_5_000E
        dd  loc_5_0052
        dd  loc_1_000C
        dd  loc_7_002C
        dd  loc_7_0083
        dd  loc_1_000C
        dd  loc_8_0000
        dd  loc_8_000B
        dd  loc_9_0000
        dd  loc_1_000C
        dd  loc_4_00F7
        dd  loc_1_0086
        dd  loc_1_00BB
        dd  loc_1_1077
        db  08Bh, 0C0h
VKD_Get_Shift_State:
        mov    eax, dword ptr [0x1210]       ; -> obj1:0x1210
        add    eax, ebx
        mov    ax, word ptr [eax + 0x36]
        ret    
VKD_Set_Shift_State:
        pushal 
        xchg   ebx, eax
        add    eax, dword ptr [0x1210]       ; -> obj1:0x1210
        xchg   esi, eax
        mov    ax, word ptr [esi + 0x36]
        call   sub_1_0655
        jbe    loc_1_0084
        mov    edx, esi
        call   sub_1_01B0
        clc    
loc_1_0084:
        popal  
        ret    
loc_1_0086:
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        call   sub_4_0698
        or     byte ptr [esi + ebx + 1], 0x40
        mov    dword ptr [0x125c], ebx       ; -> loc_1_125C
        xor    eax, eax
        mov    dword ptr [0x1260], 0         ; -> loc_1_1260
        mov    dword ptr [0x1264], 0         ; -> loc_1_1264
        mov    byte ptr [0x1288], 0          ; -> loc_1_1288
        clc    
        ret    
loc_1_00BB:
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        and    byte ptr [esi + ebx + 1], 0xbf
        cmp    ebx, dword ptr [0x125c]       ; -> loc_1_125C
        jne    loc_1_00D8
        mov    dword ptr [0x125c], 0         ; -> loc_1_125C
loc_1_00D8:
        clc    
        ret    
VKD_Force_Keys:
        pushal 
        jecxz  loc_1_0120
        mov    edx, dword ptr [0x1214]       ; -> obj1:0x1214
        add    edx, dword ptr [0x1210]       ; -> obj1:0x1210
loc_1_00E9:
        cld    
        lodsb  al, byte ptr [esi]
        call   VKD_Put_Byte
        jae    loc_1_0106
        push   esi
        push   eax
        push   ecx
        push   edx
        call   sub_1_01B0
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        call   VKD_Put_Byte
        jb     loc_1_0121
loc_1_0106:
        test   byte ptr [edx + 1], 0x40
        je     loc_1_0119
        push   esi
        push   eax
        push   ecx
        push   edx
        call   sub_1_01B0
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
loc_1_0119:
        loop   loc_1_00E9
        call   sub_1_01B0
loc_1_0120:
        clc    
loc_1_0121:
        popal  
        ret    
        db  090h
VKD_Get_Kbd_Owner:
        mov    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        ret    
VKD_Get_Msg_Key:
        cmp    ebx, dword ptr [0x125c]       ; -> loc_1_125C
        jne    loc_1_0155
        mov    eax, dword ptr [0x1260]       ; -> loc_1_1260
        cmp    eax, dword ptr [0x1264]       ; -> loc_1_1264
        je     loc_1_0155
        add    eax, 2
        and    eax, 0x1f
        mov    dword ptr [0x1260], eax       ; -> loc_1_1260
        movzx  eax, word ptr [eax + 0x1268]  ; -> loc_1_1268
        or     ebx, ebx
        ret    
loc_1_0155:
        xor    eax, eax
        ret    
VKD_Peek_Msg_Key:
        cmp    ebx, dword ptr [0x125c]       ; -> loc_1_125C
        jne    loc_1_017D
        mov    eax, dword ptr [0x1260]       ; -> loc_1_1260
        cmp    eax, dword ptr [0x1264]       ; -> loc_1_1264
        je     loc_1_017D
        add    eax, 2
        and    eax, 0x1f
        movzx  eax, word ptr [eax + 0x1268]  ; -> loc_1_1268
        or     ebx, ebx
        ret    
loc_1_017D:
        xor    eax, eax
        ret    
VKD_Flush_Msg_Key_Queue:
        cmp    ebx, dword ptr [0x125c]       ; -> loc_1_125C
        jne    loc_1_01A4
        mov    dword ptr [0x1260], 0         ; -> loc_1_1260
        mov    dword ptr [0x1264], 0         ; -> loc_1_1264
        mov    byte ptr [0x1288], 0          ; -> loc_1_1288
        ret    
loc_1_01A4:
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_01A8:
        mov    dword ptr [edx + 4], 0
        nop    
sub_1_01B0:
        mov    esi, edx
        sub    edx, dword ptr [0x1210]       ; -> obj1:0x1210
loc_1_01B8:
        test   byte ptr [0x1251], 0x10       ; -> obj1:0x1251
        je     loc_1_0280
        test   byte ptr [esi + 1], 8
        je     loc_1_01D0
        call   sub_4_02B4
loc_1_01D0:
        call   sub_1_03A4
        cmp    edx, dword ptr [0x1214]       ; -> obj1:0x1214
        jne    loc_1_022B
        test   byte ptr [0x1251], 1          ; -> obj1:0x1251
        jne    loc_1_02AA
        mov    ah, byte ptr [esi + 0x34]
        mov    al, byte ptr [esi + 0x35]
        cmp    al, ah
        je     loc_1_01FF
        jb     loc_1_01F9
        add    ah, 0x80
loc_1_01F9:
        add    al, 3
        cmp    al, ah
        jae    loc_1_022B
loc_1_01FF:
        call   sub_1_0320
        jb     loc_1_022B
        call   sub_1_0888
        mov    edi, eax
        and    edi, 0xffffff7f
        cmp    edi, dword ptr [0x1239]       ; -> obj1:0x1239
        jne    loc_1_023E
        test   al, 0x80
        je     loc_1_01D0
        mov    dword ptr [0x1239], 0         ; -> obj1:0x1239
        jmp    loc_1_01D0
loc_1_022B:
        ret    
sub_1_022C:
        call   sub_1_08CF
        je     loc_1_023D
        test   al, 0x80
        jne    loc_1_023D
        VxDCall  VSD.VSD_Bell                    ; cd2001000b00
loc_1_023D:
        ret    
loc_1_023E:
        mov    edi, dword ptr [0x122d]       ; -> obj1:0x122D
        call   sub_1_098A
        call   sub_1_0A24
        jb     loc_1_0292
        test   byte ptr [esi + 1], 0x40
        jne    loc_1_028D
        test   byte ptr [esi + 1], 0xc
        jne    sub_1_022C
        mov    word ptr [esi + 0x36], cx
        test   byte ptr [edx], 0x10
        jne    loc_1_022B
        test   byte ptr [edx], 8
        jne    sub_1_022C
        test   byte ptr [esi + 2], 0x80
        je     sub_1_057E
        call   sub_1_08CF
        jne    loc_1_02E6
        jmp    sub_1_057E
loc_1_0280:
        call   sub_1_0320
        jb     loc_1_02E5
        test   byte ptr [esi + 1], 0x40
        je     loc_1_02E5
loc_1_028D:
        jmp    loc_1_05DF
loc_1_0292:
        push   eax
        push   ebx
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        and    ebx, 0x70
        movzx  eax, word ptr [esi + 0x36]
        call   sub_1_0655
        pop    ebx
        pop    eax
        jmp    loc_1_02CF
loc_1_02AA:
        call   sub_1_0320
        jb     loc_1_02E5
        call   sub_1_098A
        test   word ptr [0x122d], 0x7e0e     ; -> obj1:0x122D
        je     loc_1_02E0
        call   sub_1_0A24
        jb     loc_1_02CF
        call   sub_1_022C
        jmp    loc_1_02D4
loc_1_02CF:
        call   sub_4_0507
loc_1_02D4:
        call   sub_1_0F45
        je     loc_1_02E5
        jmp    loc_1_01B8
loc_1_02E0:
        call   sub_4_05E7
loc_1_02E5:
        ret    
loc_1_02E6:
        test   ah, 0x40
        jne    loc_1_02E5
        btr    dword ptr [esi], 0x19
        jae    loc_1_02F9
        push   esi
        VMMCall  VMM._SignalID                   ; cd2027010100
        pop    ecx
loc_1_02F9:
        test   al, 0x80
        je     loc_1_02E5
        and    byte ptr [esi + 2], 0x7f
        ret    
loc_1_0302:
        test   byte ptr [edx + 2], 0x80
        je     loc_1_031D
        or     byte ptr [edx + 3], 2
loc_1_030C:
        push   5
        push   edx
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
        test   byte ptr [edx + 3], 2
        jne    loc_1_030C
loc_1_031D:
        ret    
        db  08Bh, 0C0h
sub_1_0320:
        bts    dword ptr [0x1251], 1         ; -> obj1:0x1251
        jb     loc_1_0348
loc_1_032A:
        call   sub_1_0F54
        jb     loc_1_0341
        call   sub_1_034A
        je     loc_1_032A
        and    byte ptr [0x1251], 0xfd       ; -> obj1:0x1251
        clc    
        ret    
loc_1_0341:
        and    byte ptr [0x1251], 0xfd       ; -> obj1:0x1251
loc_1_0348:
        stc    
        ret    
sub_1_034A:
        push   ebx
        mov    ebx, dword ptr [0x123d]       ; -> obj1:0x123D
        cmp    al, 0xe0
        je     loc_1_0381
        cmp    al, 0xe1
        je     loc_1_038E
        test   bh, 0x40
        jne    loc_1_0393
        test   bh, 0x80
        je     loc_1_036A
        call   sub_1_08C3
        je     loc_1_037D
loc_1_036A:
        mov    bl, al
        mov    eax, ebx
        mov    dword ptr [0x123d], 0         ; -> obj1:0x123D
        or     bl, 1
        pop    ebx
        ret    
loc_1_037D:
        xor    ebx, ebx
        jmp    loc_1_0384
loc_1_0381:
        or     bh, 0x80
loc_1_0384:
        mov    dword ptr [0x123d], ebx       ; -> obj1:0x123D
        xor    ebx, ebx
        pop    ebx
        ret    
loc_1_038E:
        or     bh, 0x40
        jmp    loc_1_0384
loc_1_0393:
        cmp    al, 0x1d
        je     loc_1_0384
        cmp    al, 0x45
        je     loc_1_0384
        cmp    al, 0x9d
        je     loc_1_0384
        jmp    loc_1_036A
        db  02Eh, 08Bh, 0C0h
sub_1_03A4:
        test   word ptr [esi + 1], 0x110
        jne    loc_1_046A
        bts    dword ptr [esi], 0x1c
        jb     loc_1_046A
        push   eax
        push   ebx
        push   ecx
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        mov    ebx, edx
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   byte ptr [esi + 0x3c], 1
        je     loc_1_03D9
        test   cl, 2
        jne    loc_1_0463
loc_1_03D9:
        test   byte ptr [esi + 0x3b], 1
        jne    loc_1_0442
        test   byte ptr [esi], 7
        jne    loc_1_0463
        test   byte ptr [esi + 0x3c], 0x10
        jne    loc_1_0463
        call   sub_1_0D73
        jae    loc_1_043D
        movzx  eax, byte ptr [esi + 0x34]
        cmp    al, byte ptr [esi + 0x35]
        je     loc_1_0463
        inc    eax
        and    eax, 0x7f
        mov    byte ptr [esi + 0x34], al
        mov    al, byte ptr [esi + eax + 0x7a]
        cmp    dword ptr [0x1255], 0         ; -> obj1:0x1255
        je     loc_1_0419
        push   eax
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        mov    dword ptr [esi + 0x30], eax
        pop    eax
loc_1_0419:
        xor    ah, ah
        test   byte ptr [esi + 1], 8
        jne    loc_1_043D
        mov    ah, al
        and    ah, 0x7f
        call   sub_1_08C3
        je     loc_1_043D
        cmp    ah, 0x1d
        je     loc_1_043D
        cmp    ah, 0x38
        je     loc_1_043D
        VxDCall  VDD.VDD_Set_HCurTrk             ; cd2006000a00
loc_1_043D:
        call   sub_1_0DDE
loc_1_0442:
        test   byte ptr [esi + 0x3c], 1
        je     loc_1_0463
        test   cl, 1
        jne    loc_1_0463
        cmp    ah, 0x38
        jne    loc_1_0456
        or     byte ptr [esi + 2], 1
loc_1_0456:
        mov    ebx, edx
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0463:
        pop    ecx
        pop    ebx
        pop    eax
        and    byte ptr [esi + 3], 0xef
loc_1_046A:
        ret    
        db  090h
sub_1_046C:
        push   ebx
        movzx  ebx, byte ptr [esi + 0x35]
        inc    ebx
        and    ebx, 0x7f
        cmp    bl, byte ptr [esi + 0x34]
        je     loc_1_0484
        mov    byte ptr [esi + 0x35], bl
        mov    byte ptr [esi + ebx + 0x7a], al
        clc    
        jmp    loc_1_0485
loc_1_0484:
        stc    
loc_1_0485:
        pop    ebx
        ret    
        db  090h
sub_1_0488:
        push   eax
        test   byte ptr [esi + 2], 0xc
        je     loc_1_04D3
        cmp    eax, dword ptr [esi + 0x28]
        je     loc_1_04B6
        btr    ax, 7
        jae    loc_1_04BA
        cmp    eax, dword ptr [esi + 0x28]
        je     loc_1_04BA
        cmp    al, 0x2a
        jne    loc_1_04AA
        and    byte ptr [esi + 2], 0xfb
        jmp    loc_1_04B6
loc_1_04AA:
        cmp    al, 0x36
        jne    loc_1_04B6
        and    byte ptr [esi + 2], 0xf7
        jmp    loc_1_04B6
loc_1_04B4:
        pop    edx
        pop    ecx
loc_1_04B6:
        xor    al, al
        pop    eax
        ret    
loc_1_04BA:
        mov    ah, 0x36
        btr    dword ptr [esi], 0x13
        call   sub_1_0565
        mov    ah, 0x2a
        btr    dword ptr [esi], 0x12
        call   sub_1_0565
        mov    eax, dword ptr [esp]
loc_1_04D3:
        test   al, 0x80
        jne    loc_1_04B6
        test   ah, 0x80
        je     loc_1_04B6
        movzx  eax, al
        cmp    al, 0x53
        ja     loc_1_04B6
        sub    al, 0x35
        jl     loc_1_04B6
        bt     dword ptr [0x1241], eax       ; -> obj1:0x1241
        jae    loc_1_04B6
        push   ecx
        push   edx
        movzx  ecx, word ptr [esi + 0x36]
        and    ecx, 0x1a1
        xor    eax, eax
        mov    edx, dword ptr [esp + 8]
        cmp    dl, 0x37
        je     loc_1_0515
        or     al, 1
        cmp    dl, 0x35
        je     loc_1_0515
        test   cl, 0x20
        je     loc_1_0515
        xor    al, al
loc_1_0515:
        and    cl, 0xdf
        cmp    cx, ax
        jne    loc_1_04B4
        and    byte ptr [esi + 2], 0xfd
        mov    ah, 0x2a
        mov    edx, 0x12
        test   cl, 1
        je     loc_1_054F
        movzx  ecx, word ptr [esi + 0x36]
        or     byte ptr [esi + 2], 2
        test   cl, 2
        je     loc_1_0548
        test   ch, 2
        je     loc_1_054F
        bts    dword ptr [esi], edx
        stc    
        call   sub_1_0565
loc_1_0548:
        mov    ah, 0x36
        mov    edx, 0x13
loc_1_054F:
        bts    dword ptr [esi], edx
        stc    
        call   sub_1_0565
        btc    dword ptr [esi], 0x11
        or     al, 1
        pop    edx
        pop    ecx
        pop    eax
        mov    dword ptr [esi + 0x28], eax
        ret    
sub_1_0565:
        jae    loc_1_057D
        mov    al, 0xe0
        call   sub_1_046C
        mov    al, ah
        test   byte ptr [esi + 2], 2
        je     loc_1_0578
        or     al, 0x80
loc_1_0578:
        call   sub_1_046C
loc_1_057D:
        ret    
sub_1_057E:
        test   ah, 0x40
        jne    loc_1_05B3
        test   al, 0x80
        jne    loc_1_058E
        call   sub_1_0488
        jne    loc_1_0593
loc_1_058E:
        test   ah, 0x80
        je     loc_1_059C
loc_1_0593:
        push   eax
        mov    al, 0xe0
        call   sub_1_046C
        pop    eax
loc_1_059C:
        call   sub_1_046C
        test   al, 0x80
        je     loc_1_05AA
        call   sub_1_0488
loc_1_05AA:
        mov    dword ptr [esi + 0x2c], eax
        call   sub_1_03A4
        ret    
loc_1_05B3:
        mov    al, 0xe1
        call   sub_1_046C
        mov    al, 0x1d
        call   sub_1_046C
        mov    al, 0x45
        call   sub_1_046C
        mov    al, 0xe1
        call   sub_1_046C
        mov    al, 0x9d
        call   sub_1_046C
        mov    al, 0xc5
        call   sub_1_046C
        jmp    loc_1_05AA
loc_1_05DF:
        cmp    edx, dword ptr [0x125c]       ; -> loc_1_125C
        jne    loc_1_063D
        push   eax
        push   ebx
        call   sub_1_08CF
        je     loc_1_063A
        test   al, 0x80
        jne    loc_1_062D
        cmp    al, byte ptr [0x1288]         ; -> loc_1_1288
        je     loc_1_063A
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        or     bl, bh
        and    bl, 0xe
        and    ah, 0x80
        or     ah, bl
        mov    ebx, dword ptr [0x1264]       ; -> loc_1_1264
        add    ebx, 2
        and    ebx, 0x1f
        cmp    ebx, dword ptr [0x1260]       ; -> loc_1_1260
        je     loc_1_0634
        mov    dword ptr [0x1264], ebx       ; -> loc_1_1264
        mov    word ptr [ebx + 0x1268], ax   ; -> loc_1_1268
loc_1_062D:
        mov    byte ptr [0x1288], al         ; -> loc_1_1288
        jmp    loc_1_063A
loc_1_0634:
        VxDCall  VSD.VSD_Bell                    ; cd2001000b00
loc_1_063A:
        pop    ebx
        pop    eax
        ret    
loc_1_063D:
        ret    
sub_1_063E:
        push   ebx
        or     ebx, edx
        test   bh, 8
        pop    ebx
        clc    
        je     loc_1_0654
        mov    ah, al
        mov    al, 0xe0
        call   sub_1_1235
        mov    al, ah
loc_1_0654:
        ret    
sub_1_0655:
        push   ecx
        push   edi
        push   edx
        mov    edx, eax
        mov    edi, ebx
        mov    ecx, 0xfe7e
        and    edx, ecx
        and    edi, ecx
        cmp    edi, edx
        je     loc_1_06DF
        mov    ecx, 0xf787
        and    eax, ecx
        cmp    eax, 0x100
        jne    loc_1_06AF
        and    ecx, ebx
        jne    loc_1_06AF
        mov    al, byte ptr [esi + 0x2c]
        and    al, 0x7f
        cmp    al, 0x38
        jne    loc_1_06AF
        or     al, 0x80
        call   sub_1_063E
        jb     loc_1_06AF
        call   sub_1_1235
        jb     loc_1_06DF
        xor    al, 0x80
        call   sub_1_1235
        jb     loc_1_06DF
        or     al, 0x80
        call   sub_1_1235
        jb     loc_1_06DF
        and    edx, 0xfffff7f7
loc_1_06AF:
        mov    ecx, edx
        xor    ecx, edi
        je     loc_1_06D2
        and    ecx, 0x70
        je     loc_1_06C3
        call   sub_1_06E3
        jb     loc_1_06DF
        jmp    loc_1_06AF
loc_1_06C3:
        mov    ecx, edx
        xor    ecx, edi
        je     loc_1_06D2
        call   sub_1_06E3
        jb     loc_1_06DF
        jmp    loc_1_06C3
loc_1_06D2:
        mov    eax, edx
        call   sub_1_096C
        mov    word ptr [esi + 0x36], ax
        or     esi, esi
loc_1_06DF:
        pop    edx
        pop    edi
        pop    ecx
        ret    
sub_1_06E3:
        bsf    ecx, ecx
        xor    ebx, ebx
        inc    ebx
        shl    ebx, cl
        xor    edx, ebx
        shl    ecx, 1
        mov    al, byte ptr [ecx + 0x12e0]   ; -> obj1:0x12E0
        movzx  ecx, byte ptr [ecx + 0x12e1]  ; -> loc_1_12E1
        jmp    loc_1_0702
loc_1_0702:
        and    byte ptr [edi], al
        add    byte ptr [eax], al
        adc    al, byte ptr [edi]
        add    byte ptr [eax], al
        sub    dword ptr [edi], eax
        add    byte ptr [eax], al
        dec    ebx
        pop    es
        add    byte ptr [eax], al
loc_1_0712:
        mov    cl, al
        mov    al, 0xe0
        call   sub_1_1235
        jb     loc_1_077F
        mov    al, cl
loc_1_0720:
        test   bx, di
        jne    loc_1_0779
loc_1_0725:
        or     al, 0x80
        jmp    loc_1_0779
loc_1_0729:
        xchg   bl, bh
        test   bx, dx
        je     loc_1_073C
        or     al, 0x80
        call   sub_1_1235
        jb     loc_1_077F
        xor    al, 0x80
loc_1_073C:
        call   sub_1_1235
        jb     loc_1_077F
        test   bx, di
        jne    loc_1_077F
        je     loc_1_0725
loc_1_074B:
        call   sub_1_063E
        jb     loc_1_077F
        test   bx, di
        jne    loc_1_0779
        or     al, 0x80
        jb     loc_1_077F
        call   sub_1_1235
        jb     loc_1_077F
        xor    al, 0x80
        call   sub_1_1235
        jb     loc_1_077F
        call   sub_1_1235
        jb     loc_1_077F
        or     al, 0x80
        jmp    loc_1_0779
loc_1_0779:
        call   sub_1_1235
loc_1_077F:
        ret    
loc_1_0780:
        mov    edi, ebx
        add    edi, dword ptr [0x1210]       ; -> obj1:0x1210
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        sti    
        test   byte ptr [edi + 1], 8
        je     loc_1_079B
        or     byte ptr [edi + 1], 0x10
        jmp    loc_1_07A6
loc_1_079B:
        mov    eax, dword ptr [0x1224]       ; -> obj1:0x1224
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
loc_1_07A6:
        and    byte ptr [edi], 0xfe
        mov    edx, edi
        jmp    sub_1_01B0
loc_1_07B0:
        call   sub_1_08F6
        mov    eax, 0x417
        add    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax]
        and    al, 0x70
        add    ebx, dword ptr [0x1210]       ; -> obj1:0x1210
        mov    byte ptr [ebx + 0x42], al
        ret    
        db  090h
loc_1_07CC:
        sti    
        mov    edx, ebx
        add    edx, dword ptr [0x1210]       ; -> obj1:0x1210
        mov    esi, edx
        mov    eax, 0x417
        add    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax]
        and    al, 0x70
        cmp    al, byte ptr [esi + 0x42]
        je     loc_1_07EB
        mov    byte ptr [esi + 0x41], al
loc_1_07EB:
        test   byte ptr [esi + 2], 1
        jne    loc_1_0816
        test   byte ptr [esi + 1], 8
        je     loc_1_0811
        mov    al, byte ptr [esi + 0x3a]
        and    al, 0x7f
        cmp    al, 0x38
        mov    eax, dword ptr [esi + 0x4b]
        jne    loc_1_0806
        mov    eax, dword ptr [esi + 0x47]
loc_1_0806:
        mov    esi, 0x824                    ; -> loc_1_0824
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
loc_1_0811:
        jmp    sub_1_01B0
loc_1_0816:
        mov    eax, dword ptr [esi + 0x43]
        mov    esi, 0x3ca                    ; -> loc_4_03CA
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
loc_1_0824:
        and    byte ptr [edx + 1], 0xef
        jmp    sub_1_01B0
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_1231               ; fixup
loc_1_0835:
        push   ebx
        push   esi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    esi, ebx
        add    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        cmp    dword ptr [esi + 0x30], 0
        je     loc_1_0865
        push   eax
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        sub    eax, dword ptr [esi + 0x30]
        cmp    eax, dword ptr [0x1255]       ; -> obj1:0x1255
        pop    eax
        jb     loc_1_0882
        mov    dword ptr [esi + 0x30], 0
loc_1_0865:
        test   byte ptr [esi + 1], 0xc
        jne    loc_1_0873
        pop    esi
        pop    ebx
        jmp    loc_1_1231
loc_1_0873:
        test   byte ptr [esi + 1], 4
        je     loc_1_0882
        and    byte ptr [esi + 1], 0xfe
        call   sub_4_040A
loc_1_0882:
        pop    esi
        pop    ebx
        clc    
        ret    
        db  08Bh, 0C0h
sub_1_0888:
        mov    edi, 0x1228                   ; -> obj1:0x1228
        test   ah, 0x80
        je     loc_1_08AF
        cmp    byte ptr [edi], 0
        je     loc_1_08A4
        cmp    al, 0xdb
        je     loc_1_089F
        cmp    al, 0xdc
        jne    loc_1_08B4
loc_1_089F:
        mov    byte ptr [edi], 0
        jmp    loc_1_08B4
loc_1_08A4:
        cmp    al, 0x5b
        je     loc_1_08AC
        cmp    al, 0x5c
        jne    loc_1_08C2
loc_1_08AC:
        mov    byte ptr [edi], 1
loc_1_08AF:
        cmp    byte ptr [edi], 0
        je     loc_1_08C2
loc_1_08B4:
        mov    edx, dword ptr [0x1229]       ; -> obj1:0x1229
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        add    esi, edx
loc_1_08C2:
        ret    
sub_1_08C3:
        push   eax
        and    al, 0x7f
        cmp    al, 0x2a
        je     loc_1_08CD
        cmp    al, 0x36
        stc    
loc_1_08CD:
        pop    eax
        ret    
sub_1_08CF:
        push   eax
        test   ah, 0x40
        jne    loc_1_08F2
        and    al, 0x7f
        cmp    al, 0x46
        ja     loc_1_08F2
        sub    al, 0x1d
        jb     loc_1_08F2
        movzx  eax, al
        shl    eax, 1
        cmp    word ptr [eax + 0x128c], 0    ; -> loc_1_128C
        je     loc_1_08F2
        xor    eax, eax
        jmp    loc_1_08F4
loc_1_08F2:
        or     al, 1
loc_1_08F4:
        pop    eax
        ret    
sub_1_08F6:
        push   ebx
        call   sub_1_0922
        jb     loc_1_0920
        and    al, 0x70
        mov    ah, al
        add    ebx, dword ptr [0x1210]       ; -> obj1:0x1210
        xor    al, byte ptr [ebx + 0x41]
        je     loc_1_0920
        xor    byte ptr [ebx + 0x41], al
        xor    ah, byte ptr [ebx + 0x36]
        and    ah, al
        je     loc_1_0920
        xor    byte ptr [0x122d], al         ; -> obj1:0x122D
        xor    byte ptr [ebx + 0x36], al
loc_1_0920:
        pop    ebx
        ret    
sub_1_0922:
        test   byte ptr [ebx], 8
        je     loc_1_0933
        mov    eax, dword ptr [0x1210]       ; -> obj1:0x1210
        movzx  eax, word ptr [eax + ebx + 0x36]
        stc    
        ret    
loc_1_0933:
        push   ebx
        mov    ebx, dword ptr [ebx + 4]
        movzx  eax, word ptr [ebx + 0x417]
        mov    bh, byte ptr [ebx + 0x496]
        and    bh, 0xc
        mov    bl, ah
        and    bl, 3
        shl    bl, 2
        and    ah, 0xf0
        test   al, 0xc
        je     loc_1_0960
        or     ah, bh
        or     bh, bl
        je     loc_1_0960
        and    al, 0xf3
        or     al, bl
loc_1_0960:
        test   al, 1
        je     loc_1_096B
        and    al, 0xfe
        or     eax, 0x200
loc_1_096B:
        pop    ebx
sub_1_096C:
        and    ax, 0xfe7e
        test   ax, 0x202
        je     loc_1_0978
        or     al, 1
loc_1_0978:
        test   ax, 0x404
        je     loc_1_0980
        or     al, 0x80
loc_1_0980:
        test   ax, 0x808
        je     loc_1_0989
        or     ah, 1
loc_1_0989:
        ret    
sub_1_098A:
        push   eax
        push   ebx
        push   edx
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        movzx  edx, word ptr [esi + 0x36]
        mov    ecx, eax
        and    al, 0x7f
        cmp    al, 0x46
        ja     loc_1_0A1C
        sub    al, 0x1d
        jb     loc_1_0A1C
        movzx  eax, al
        shl    eax, 1
        mov    ax, word ptr [eax + 0x128c]   ; -> loc_1_128C
        or     eax, eax
        je     loc_1_0A1C
        test   ax, 0x70
        jne    loc_1_09D3
        test   ch, 0x80
        je     loc_1_09C0
        xchg   al, ah
loc_1_09C0:
        test   cl, 0x80
        je     loc_1_09CD
        not    eax
        and    ebx, eax
        and    edx, eax
        jmp    loc_1_0A04
loc_1_09CD:
        or     ebx, eax
        or     edx, eax
        jmp    loc_1_0A04
loc_1_09D3:
        mov    ah, al
        test   bx, 0x80
        jne    loc_1_09E5
        test   byte ptr [0x1251], 1          ; -> obj1:0x1251
        je     loc_1_09E7
loc_1_09E5:
        and    al, 0x8f
loc_1_09E7:
        test   cl, 0x80
        jne    loc_1_09F2
        test   ah, bh
        je     loc_1_09FC
        jmp    loc_1_0A1C
loc_1_09F2:
        and    al, 0x8f
        not    ah
        and    bh, ah
        and    dh, ah
        xor    ah, ah
loc_1_09FC:
        xor    bl, al
        xor    dl, al
        or     bh, ah
        or     dh, ah
loc_1_0A04:
        mov    eax, ebx
        call   sub_1_096C
        mov    ebx, eax
        mov    eax, edx
        call   sub_1_096C
        mov    edx, eax
        mov    dword ptr [0x122d], ebx       ; -> obj1:0x122D
loc_1_0A1C:
        mov    ecx, edx
        pop    edx
        pop    ebx
        pop    eax
        ret    
        db  000h, 000h
sub_1_0A24:
        push   eax
        test   dword ptr [0x122d], 0x7e0e    ; -> obj1:0x122D
        jne    loc_1_0A40
        and    eax, 0x807f
        cmp    eax, 0x8045
        je     loc_1_0A4B
        mov    eax, dword ptr [esp]
loc_1_0A40:
        test   al, 0x80
        je     loc_1_0A4D
        test   byte ptr [0x1251], 1          ; -> obj1:0x1251
loc_1_0A4B:
        je     loc_1_0AC0
loc_1_0A4D:
        pushal 
        mov    edi, esi
        mov    cx, ax
        mov    esi, dword ptr [0x1308]       ; -> loc_1_1308
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_0A8D
        and    cx, 0x807f
loc_1_0A66:
        cmp    cl, byte ptr [eax]
        jne    loc_1_0A85
        cmp    byte ptr [eax + 2], 0xff
        je     loc_1_0A75
        cmp    ch, byte ptr [eax + 2]
        jne    loc_1_0A85
loc_1_0A75:
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        and    bx, word ptr [eax + 4]
        cmp    bx, word ptr [eax + 6]
        je     loc_1_0A90
loc_1_0A85:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_0A66
loc_1_0A8D:
        clc    
        jmp    loc_1_0ABF
loc_1_0A90:
        mov    ebx, dword ptr [eax + 8]
        bt     dword ptr [edi + 0xc], ebx
        jae    loc_1_0AB0
        test   byte ptr [esp + 0x1c], 0x80
        je     loc_1_0A85
        and    byte ptr [eax + 3], 0x7f
        mov    bl, byte ptr [esp + 0x1c]
        mov    byte ptr [0x1314], bl         ; -> loc_1_1314
        jmp    loc_1_0A85
loc_1_0AB0:
        test   byte ptr [eax + 3], 0x40
        jne    loc_4_04EC
        stc    
        mov    dword ptr [esp + 0x10], eax
loc_1_0ABF:
        popal  
loc_1_0AC0:
        pop    eax
        ret    
        db  000h, 000h
loc_1_0AC4:
        mov    esi, ebx
        add    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        cmp    ecx, 4
        jbe    loc_1_0AD7
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0AD7:
        je     loc_1_0AE8
        call   sub_1_10A4
        jb     loc_1_0AE7
        in     al, 0x64
        and    al, 0x14
        or     al, byte ptr [esi + 0x3b]
loc_1_0AE7:
        ret    
loc_1_0AE8:
        movzx  eax, al
        mov    ah, byte ptr [eax + 0x131c]   ; -> loc_1_131C
        or     ah, ah
        je     loc_1_0B80
        mov    byte ptr [esi + 0x3d], al
        mov    dl, ah
        and    dl, 0x1f
        mov    byte ptr [esi + 0x40], dl
        mov    dl, ah
        and    dl, 0xc0
        cmp    dl, 0x40
        je     loc_1_0B59
        cmp    dl, 0xc0
        je     loc_1_0B1B
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        jne    loc_1_0B59
loc_1_0B1B:
        mov    ecx, 0x10000
loc_1_0B20:
        test   byte ptr [0x1251], 4          ; -> obj1:0x1251
        loopne loc_1_0B20
        mov    dword ptr [0x1318], ebx       ; -> loc_1_1318
        cmp    byte ptr [esi + 0x40], 0
        je     loc_1_0B4E
        test   ah, 0x20
        jne    loc_1_0B54
        or     byte ptr [0x1251], 4          ; -> obj1:0x1251
        push   eax
        mov    al, 0xad
        call   sub_1_0F3D
        or     byte ptr [esi + 0x3b], 2
        pop    eax
loc_1_0B4E:
        call   sub_1_0F3D
        ret    
loc_1_0B54:
        or     byte ptr [esi], 0x20
        jmp    loc_1_0BD6
loc_1_0B59:
        test   ah, 0x20
        jne    loc_1_0B7B
        xor    al, al
        movzx  ecx, byte ptr [esi + 0x40]
        cmp    cl, 0x1f
        jne    loc_1_0B6B
        mov    cl, 1
loc_1_0B6B:
        xchg   ecx, eax
        call   sub_1_0D89
        xchg   ecx, eax
        loop   loc_1_0B6B
        mov    edx, ebx
        jmp    loc_1_0D9D
loc_1_0B7B:
        or     byte ptr [esi], 0x40
        jmp    loc_1_0BD6
loc_1_0B80:
        cmp    al, 0xad
        jne    loc_1_0B8A
        or     byte ptr [esi + 0x3c], 0x10
        jmp    loc_1_0BD6
loc_1_0B8A:
        cmp    al, 0xae
        jne    loc_1_0B94
        and    byte ptr [esi + 0x3c], 0xef
        jmp    loc_1_0BD6
loc_1_0B94:
        cmp    al, 0x20
        jne    loc_1_0BA2
        mov    al, byte ptr [esi + 0x3c]
        call   sub_1_0DB8
        jmp    loc_1_0BD6
loc_1_0BA2:
        cmp    al, 0x60
        jne    loc_1_0BAB
        or     byte ptr [esi], 8
        jmp    loc_1_0BD6
loc_1_0BAB:
        cmp    al, 0xd1
        jne    loc_1_0BB4
        or     byte ptr [esi], 0x10
        jmp    loc_1_0BD6
loc_1_0BB4:
        cmp    al, 0xf0
        jb     loc_1_0BC3
        or     al, 2
        test   al, 1
        jne    loc_1_0B4E
        jmp    loc_6_00E0
loc_1_0BC3:
        call   sub_1_10EC
        je     loc_1_0BD6
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        je     loc_1_0B4E
loc_1_0BD6:
        ret    
        db  090h
loc_1_0BD8:
        mov    esi, ebx
        add    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        cmp    ecx, 4
        jbe    loc_1_0BEB
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0BEB:
        je     loc_1_0C3D
        call   sub_1_1100
        jb     loc_1_0C3C
        mov    al, byte ptr [esi + 0x39]
        mov    byte ptr [esi + 0x3b], 0
        and    byte ptr [esi], 0xfe
        push   eax
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   cl, 2
        jne    loc_1_0C2D
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        test   byte ptr [esi + 2], 1
        je     loc_1_0C2D
        push   esi
        mov    eax, dword ptr [esi + 0x43]
        mov    edx, esi
        mov    esi, 0x3ca                    ; -> loc_4_03CA
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        pop    esi
loc_1_0C2D:
        pop    eax
        call   sub_1_0D6A
        je     loc_1_0C3C
        mov    edx, ebx
        jmp    sub_1_03A4
loc_1_0C3C:
        ret    
loc_1_0C3D:
        mov    ecx, dword ptr [esi]
        test   cl, 0x20
        jne    loc_1_0CB2
        test   cl, 0x40
        jne    loc_1_0CDF
        test   cl, 8
        jne    loc_1_0C95
        test   cl, 0x10
        jne    loc_1_0C9D
        test   cl, 6
        je     loc_1_0D03
        btr    ecx, 1
        jae    loc_1_0C84
        test   al, 0x80
        jne    loc_1_0D00
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        jne    loc_1_0D61
        call   sub_1_0FD3
        jmp    loc_1_0D61
loc_1_0C84:
        test   al, 0x80
        jne    loc_1_0D00
        call   sub_1_1014
        and    cl, 0xfb
        jmp    loc_1_0D61
loc_1_0C95:
        and    byte ptr [esi], 0xf7
        mov    byte ptr [esi + 0x3c], al
        jmp    loc_1_0C3C
loc_1_0C9D:
        and    byte ptr [esi], 0xef
        or     al, 2
        push   eax
        mov    al, 0xd1
        call   sub_1_0F3D
        pop    eax
        call   sub_1_0F35
        jmp    loc_1_0C3C
loc_1_0CB2:
        mov    cl, al
        call   sub_1_0D89
        mov    ch, byte ptr [esi + 0x40]
        cmp    ch, 0x1f
        jne    loc_1_0CC7
        or     cl, cl
        je     loc_1_0CD2
        inc    ch
loc_1_0CC7:
        dec    ch
        mov    byte ptr [esi + 0x40], ch
        jne    loc_1_0C3C
loc_1_0CD2:
        call   sub_1_0DE9
        and    byte ptr [esi], 0xdf
        jmp    loc_1_0C3C
loc_1_0CDF:
        mov    cl, byte ptr [esi + 0x40]
        cmp    cl, 0x1f
        jne    loc_1_0CED
        or     cl, cl
        je     loc_1_0CF8
        inc    cl
loc_1_0CED:
        dec    cl
        mov    byte ptr [esi + 0x40], al
        jne    loc_1_0C3C
loc_1_0CF8:
        and    byte ptr [esi], 0xbf
        jmp    loc_1_0C3C
loc_1_0D00:
        and    cl, 0xf8
loc_1_0D03:
        cmp    al, 0x93
        je     loc_1_0D32
        test   byte ptr [esi + 3], 8
        jne    loc_1_0D38
        cmp    al, 0xed
        jb     loc_1_0D38
        ja     loc_1_0D18
        or     ecx, 2
        jmp    loc_1_0D61
loc_1_0D18:
        cmp    al, 0xf3
        jne    loc_1_0D21
        or     ecx, 4
        jmp    loc_1_0D61
loc_1_0D21:
        cmp    al, 0xee
        jne    loc_1_0D29
        mov    al, 0xee
        jmp    loc_1_0D66
loc_1_0D29:
        cmp    al, 0xfe
        jne    loc_1_0D61
        mov    al, byte ptr [esi + 0x3a]
        jmp    loc_1_0D66
loc_1_0D32:
        or     byte ptr [esi + 3], 4
        jmp    loc_1_0D50
loc_1_0D38:
        btr    dword ptr [esi], 0x1a
        jae    loc_1_0D50
        cmp    al, 1
        je     loc_1_0D4C
        cmp    al, 5
        ja     loc_1_0D50
        and    byte ptr [esi + 3], 0xf7
        jmp    loc_1_0D50
loc_1_0D4C:
        or     byte ptr [esi + 3], 8
loc_1_0D50:
        and    byte ptr [esi], 0x80
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        jne    loc_1_0D60
        call   sub_1_0F35
loc_1_0D60:
        ret    
loc_1_0D61:
        or     ecx, 1
        mov    al, 0xfa
loc_1_0D66:
        mov    dword ptr [esi], ecx
        jmp    sub_1_0DB8
sub_1_0D6A:
        push   eax
        mov    al, byte ptr [esi + 0x3e]
        cmp    al, byte ptr [esi + 0x3f]
        pop    eax
        ret    
sub_1_0D73:
        movzx  eax, byte ptr [esi + 0x3e]
        cmp    al, byte ptr [esi + 0x3f]
        stc    
        je     loc_1_0D88
        inc    eax
        and    al, 0x1f
        mov    byte ptr [esi + 0x3e], al
        mov    al, byte ptr [esi + eax + 0x5a]
        clc    
loc_1_0D88:
        ret    
sub_1_0D89:
        movzx  eax, byte ptr [esi + 0x3f]
        inc    eax
        and    al, 0x1f
        cmp    al, byte ptr [esi + 0x3e]
        je     loc_1_0D9C
        mov    byte ptr [esi + 0x3f], al
        mov    byte ptr [esi + eax + 0x5a], cl
loc_1_0D9C:
        ret    
loc_1_0D9D:
        mov    ebx, edx
        mov    esi, edx
        add    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        call   sub_1_0D73
        and    byte ptr [esi + 0x3b], 0xfd
        call   sub_1_0DB8
        ret    
        db  08Bh, 0C0h
sub_1_0DB8:
        call   sub_1_0DDE
        or     byte ptr [esi + 0x3b], 8
        test   byte ptr [esi + 0x3c], 1
        je     loc_1_0DDD
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   cl, 2
        jne    loc_1_0DDD
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0DDD:
        ret    
sub_1_0DDE:
        mov    byte ptr [esi + 0x3b], 1
        mov    byte ptr [esi + 0x39], al
        mov    byte ptr [esi + 0x3a], al
        ret    
sub_1_0DE9:
        mov    al, byte ptr [esi + 0x3d]
        call   sub_1_0F3D
loc_1_0DF1:
        call   sub_1_0D73
        jb     loc_1_0DFF
        call   sub_1_0F35
        jmp    loc_1_0DF1
loc_1_0DFF:
        ret    
VKD_Filter_Keyboard_Input:
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0E04:
        xor    ecx, ecx
        dec    ch
loc_1_0E08:
        in     al, 0x64
        test   al, 1
        jne    loc_1_0E12
        test   al, 2
        loopne loc_1_0E08
loc_1_0E12:
        mov    cl, al
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        cmp    byte ptr [0x1220], 1          ; -> obj1:0x1220
        jne    loc_1_0E2E
        VxDCall  SHELL.SHELL_Update_User_Activity ; cd200c001700
loc_1_0E2E:
        test   cl, 1
        je     loc_1_0EED
        in     al, 0x60
        mov    cl, al
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        test   byte ptr [0x1251], 4          ; -> obj1:0x1251
        jne    loc_1_0EB1
        cmp    cl, 0xfa
        je     loc_1_0E95
        VxDCall  VKD.VKD_Filter_Keyboard_Input   ; cd2012000d00
        jb     loc_1_0EED
        mov    eax, dword ptr [0x1424]       ; -> obj1:0x1424
        inc    eax
        and    eax, 0x7f
        cmp    eax, dword ptr [0x1428]       ; -> obj1:0x1428
        je     loc_1_0E77
        mov    dword ptr [0x1424], eax       ; -> obj1:0x1424
        mov    byte ptr [eax + 0x142c], cl   ; -> obj1:0x142C
loc_1_0E77:
        mov    edx, dword ptr [0x1214]       ; -> obj1:0x1214
        add    edx, esi
        cmp    dword ptr [edx + 4], 0
        jne    loc_1_0EED
        mov    esi, 0x1a8                    ; -> loc_1_01A8
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    dword ptr [edx + 4], esi
        jmp    loc_1_0EED
loc_1_0E95:
        mov    edx, 0x1420                   ; -> obj1:0x1420
        cmp    byte ptr [edx], 0
        je     loc_1_0EF8
        xor    al, al
        xchg   byte ptr [edx + 1], al
        btr    dword ptr [edx], 6
        jae    loc_1_0EEF
        call   sub_1_0F35
        jmp    loc_1_0EED
loc_1_0EB1:
        mov    edx, dword ptr [0x1318]       ; -> loc_1_1318
        add    esi, edx
        call   sub_1_0D89
        mov    ch, byte ptr [esi + 0x40]
        cmp    ch, 0x1f
        jne    loc_1_0ECC
        or     cl, cl
        je     loc_1_0ED3
        inc    ch
loc_1_0ECC:
        dec    ch
        mov    byte ptr [esi + 0x40], ch
        jne    loc_1_0EED
loc_1_0ED3:
        mov    al, 0xae
        call   sub_1_0F3D
        and    byte ptr [0x1251], 0xfb       ; -> obj1:0x1251
loc_1_0EE1:
        mov    esi, 0xd9d                    ; -> loc_1_0D9D
        sti    
        VMMCall  VMM.Call_Global_Event           ; cd2010000100
loc_1_0EED:
        clc    
        ret    
loc_1_0EEF:
        mov    byte ptr [0x1420], 0          ; -> obj1:0x1420
        jmp    loc_1_0EED
loc_1_0EF8:
        mov    edx, dword ptr [0x1214]       ; -> obj1:0x1214
        add    esi, edx
        call   sub_1_0D89
        jmp    loc_1_0EE1
sub_1_0F07:
        push   eax
        push   ecx
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        or     eax, eax
        je     loc_1_0F18
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
loc_1_0F18:
        mov    ecx, 0x10000
loc_1_0F1D:
        in     al, 0x64
        test   al, 2
        loopne loc_1_0F1D
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        or     eax, eax
        je     loc_1_0F32
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_1_0F32:
        pop    ecx
        pop    eax
        ret    
sub_1_0F35:
        call   sub_1_0F07
        out    0x60, al
        ret    
sub_1_0F3D:
        call   sub_1_0F07
        out    0x64, al
        ret    
sub_1_0F45:
        mov    eax, dword ptr [0x1428]       ; -> obj1:0x1428
        cmp    eax, dword ptr [0x1424]       ; -> obj1:0x1424
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_0F54:
        stc    
        pushfd 
        cli    
        mov    eax, dword ptr [0x1428]       ; -> obj1:0x1428
        cmp    eax, dword ptr [0x1424]       ; -> obj1:0x1424
        je     loc_1_0F77
        inc    eax
        and    eax, 0x7f
        mov    dword ptr [0x1428], eax       ; -> obj1:0x1428
        mov    al, byte ptr [eax + 0x142c]   ; -> obj1:0x142C
        and    dword ptr [esp], 0xfffffffe
loc_1_0F77:
        popfd  
        ret    
VKD_Put_Byte:
        pushfd 
        push   ebx
        cli    
        mov    ebx, dword ptr [0x1424]       ; -> obj1:0x1424
        inc    ebx
        and    ebx, 0x7f
        cmp    ebx, dword ptr [0x1428]       ; -> obj1:0x1428
        je     loc_1_0F9F
        mov    dword ptr [0x1424], ebx       ; -> obj1:0x1424
        mov    byte ptr [ebx + 0x142c], al   ; -> obj1:0x142C
        pop    ebx
        popfd  
        clc    
        jmp    loc_1_0FA2
loc_1_0F9F:
        pop    ebx
        popfd  
        stc    
loc_1_0FA2:
        ret    
sub_1_0FA3:
        push   eax
loc_1_0FA4:
        in     al, 0x64
        test   al, 1
        je     loc_1_0FAE
        in     al, 0x60
        jmp    loc_1_0FA4
loc_1_0FAE:
        pop    eax
        ret    
sub_1_0FB0:
        push   eax
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        mov    esi, eax
loc_1_0FB9:
        cmp    byte ptr [0x1420], 0          ; -> obj1:0x1420
        je     loc_1_0FD1
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        sub    eax, esi
        cmp    eax, 0xf4240
        jb     loc_1_0FB9
loc_1_0FD1:
        pop    eax
        ret    
sub_1_0FD3:
        pushal 
        call   sub_1_0FB0
        mov    byte ptr [0x1421], al         ; -> loc_1_1421
        mov    byte ptr [0x1420], 0xc0       ; -> obj1:0x1420
        mov    al, 0xed
        call   sub_1_0F35
        xor    esi, esi
        xchg   dword ptr [0x141c], esi       ; -> obj1:0x141C
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    eax, 0x3e8
        xor    edx, edx
        mov    esi, 0x1055                   ; -> loc_1_1055
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x141c], esi       ; -> obj1:0x141C
        popal  
        ret    
sub_1_1014:
        pushal 
        call   sub_1_0FB0
        mov    byte ptr [0x1421], al         ; -> loc_1_1421
        mov    byte ptr [0x1420], 0xc0       ; -> obj1:0x1420
        mov    al, 0xf3
        call   sub_1_0F35
        xor    esi, esi
        xchg   dword ptr [0x141c], esi       ; -> obj1:0x141C
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    eax, 0x3e8
        xor    edx, edx
        mov    esi, 0x1055                   ; -> loc_1_1055
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x141c], esi       ; -> obj1:0x141C
        popal  
        ret    
loc_1_1055:
        mov    dword ptr [0x141c], 0         ; -> obj1:0x141C
        cmp    byte ptr [0x1420], 0          ; -> obj1:0x1420
        je     loc_1_1076
        xor    eax, eax
        mov    dword ptr [0x1420], eax       ; -> obj1:0x1420
        mov    al, 0xf4
        call   sub_1_0F35
loc_1_1076:
        ret    
loc_1_1077:
        cmp    byte ptr [0x125a], 0          ; -> loc_1_125A
        je     loc_1_10A2
        push   0
        push   2
        push   0x472
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        je     loc_1_109B
        dec    eax
        mov    word ptr [eax], 0x1234
loc_1_109B:
        mov    al, 0xfe
        call   sub_1_0F3D
loc_1_10A2:
        clc    
        ret    
sub_1_10A4:
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        je     loc_1_10B8
        cmp    byte ptr [0x1512], 0xff       ; -> obj1:0x1512
        jne    loc_1_10D9
        clc    
        ret    
loc_1_10B8:
        push   ecx
        pushfd 
        cli    
        call   sub_6_0280
        jae    loc_1_10E6
        mov    al, byte ptr [ecx*2 + 0x14b9] ; -> loc_1_14B9
        mov    ah, al
        and    ah, 0xfd
        mov    byte ptr [ecx*2 + 0x14b9], ah ; -> loc_1_14B9
loc_1_10D5:
        popfd  
        pop    ecx
        stc    
        ret    
loc_1_10D9:
        push   ecx
        pushfd 
        cli    
        in     al, 0x64
        test   al, 1
        je     loc_1_10E6
        test   al, 0x20
        jne    loc_1_10D5
loc_1_10E6:
        popfd  
        pop    ecx
        clc    
        ret    
        db  08Bh, 0C0h
sub_1_10EC:
        cmp    al, 0xa7
        je     loc_1_10FE
        cmp    al, 0xa8
        je     loc_1_10FE
        cmp    al, 0xa9
        je     loc_1_10FE
        cmp    al, 0xd3
        je     loc_1_10FE
        cmp    al, 0xd4
loc_1_10FE:
        ret    
        db  090h
sub_1_1100:
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        je     loc_1_1114
        cmp    byte ptr [0x1512], 0xff       ; -> obj1:0x1512
        jne    loc_1_1151
        clc    
        ret    
loc_1_1114:
        push   ecx
        pushfd 
        cli    
        call   sub_6_0280
        jae    loc_1_1162
        mov    al, byte ptr [ecx*2 + 0x14b8] ; -> obj1:0x14B8
        dec    word ptr [0x14fc]             ; -> obj1:0x14FC
        inc    cx
        and    cx, 0x1f
        mov    word ptr [0x14fa], cx         ; -> obj1:0x14FA
        push   eax
        mov    eax, dword ptr [0x14fe]       ; -> obj1:0x14FE
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    byte ptr [0x1502], 0          ; -> obj1:0x1502
        pop    eax
loc_1_114D:
        popfd  
        pop    ecx
        stc    
        ret    
loc_1_1151:
        push   ecx
        pushfd 
        cli    
        in     al, 0x64
        test   al, 1
        je     loc_1_1162
        test   al, 0x20
        je     loc_1_1162
        in     al, 0x60
        jmp    loc_1_114D
loc_1_1162:
        popfd  
        pop    ecx
        clc    
        ret    
        db  000h, 000h
        dd  VKD_Get_Version          ; fixup
        dd  VKD_Define_Hot_Key       ; fixup
        dd  VKD_Remove_Hot_Key       ; fixup
        dd  VKD_Local_Enable_Hot_Key ; fixup
        dd  VKD_Local_Disable_Hot_Key ; fixup
        dd  VKD_Reflect_Hot_Key      ; fixup
        dd  VKD_Cancel_Hot_Key_State ; fixup
        dd  VKD_Force_Keys           ; fixup
        dd  VKD_Get_Kbd_Owner        ; fixup
        dd  VKD_Define_Paste_Mode    ; fixup
        dd  VKD_Start_Paste          ; fixup
        dd  VKD_Cancel_Paste         ; fixup
        dd  VKD_Get_Msg_Key          ; fixup
        dd  VKD_Peek_Msg_Key         ; fixup
        dd  VKD_Flush_Msg_Key_Queue  ; fixup
        dd  VKD_Enable_Keyboard      ; fixup
        dd  VKD_Disable_Keyboard     ; fixup
        dd  VKD_Get_Shift_State      ; fixup
        dd  VKD_Filter_Keyboard_Input ; fixup
        dd  VKD_Put_Byte             ; fixup
        dd  VKD_Set_Shift_State      ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0000Dh                      ; DDB_Req_Device_Number
        db  2, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VKD     '                  ; DDB_Name
        dd  038000000h                  ; DDB_Init_Order
        dd  VKD_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VKD_PM_API               ; DDB_PM_API_Proc
        dd  obj1:0x1168              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1231:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_1235:
        insb   byte ptr es:[edi], dx
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0x7d5c00], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_125A:
        inc    dword ptr [eax]
loc_1_125C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1260:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1264:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1268:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1288:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_128C:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_12E1:
        add    byte ptr [edx], ch
        add    byte ptr [0x46003800], bl
        or     byte ptr [ebp + 8], al
        cmp    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr ss:[0x46043804], bl
        add    byte ptr [ebp], al
        cmp    al, byte ptr [eax]
loc_1_12FE:
        push   edx
        dec    edi
        push   eax
        push   ecx
        dec    ebx
        dec    esp
        dec    ebp
        inc    edi
        dec    eax
        dec    ecx
loc_1_1308:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_130C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1310:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1314:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_131C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, al
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        rol    ecx, 0xc1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, ah
        loope  0x1361
        loope  0x1363
        loope  0x1365
        loope  0x1367
        loope  0x1369
        loope  0x136b
        loope  0x136d
        loope  0x136f
        loope  0x1371
        loope  0x1373
        loope  0x1375
        loope  0x1377
        loope  0x1379
        loope  0x137b
        loope  0x137d
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13C0:
        add    byte ptr [eax], al
loc_1_13C2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    ecx, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_1411
loc_1_1411:
        add    byte ptr [eax], al
        loopne loc_1_1415
loc_1_1415:
        add    byte ptr [eax], al
        loopne loc_1_1419
loc_1_1419:
        add    byte ptr [eax], al
        loopne loc_1_141D
loc_1_141D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1421:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14B9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1503:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1511:
        add    byte ptr [eax], al
loc_1_1513:
        add    byte ptr [eax], al
loc_1_1515:
        add    byte ptr [eax], al
        add    ch, cl                        ; -> loc_6_01CD
        db  001h, 000h, 000h
        dd  loc_6_01E6               ; fixup
        dd  loc_6_01FA               ; fixup
        dd  loc_6_01AD               ; fixup
        dd  loc_6_0206               ; fixup
        dd  loc_6_01EE               ; fixup
        dd  loc_6_021C               ; fixup
        dd  loc_6_0259               ; fixup
loc_1_1538:
        add    eax, dword ptr [ebx + eax]
        add    eax, dword ptr [esp + eax]
        add    al, 0
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x115  R-X
        db  002h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        arpl   word ptr [eax], ax
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
loc_2_0014:
        push   esp
        push   0x69762065
        jb     0x90
        jne    loc_2_007F
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x65], ch
        jns    0x86
        outsd  dx, dword ptr [esi]
        popal  
        jb     0x8c
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    loc_2_0093
        jb     loc_2_0050
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jbe    0xa5
        jb     loc_2_00B2
        jne    0xa1
        insb   byte ptr es:[edi], dx
        imul   edi, dword ptr [edx + 0x65], 0x72616820
        ja     0xac
        jb     loc_2_00B2
        and    byte ptr [ecx + 0x6e], ch
loc_2_0050:
        je     loc_2_00B7
        jb     0xc6
        jne    0xc6
        je     0x65
        or     ch, byte ptr [esi + 0x75]
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [ecx], dh
        and    byte ptr [esi + 0x6f], ah
        jb     0x86
        je     0xd0
        and    byte ptr gs:[ebx + 0x65], ch
        jns    0xd0
        outsd  dx, dword ptr [esi]
        popal  
        jb     0xd6
        or     eax, 0x5400000a
        push   0x69762065
        jb     0xf3
loc_2_007F:
        jne    0xe2
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x65], ch
        jns    0xe9
        outsd  dx, dword ptr [esi]
        popal  
        jb     loc_2_00EF
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xf6
        jb     0xb3
loc_2_0093:
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        imul   ebp, dword ptr [esi + 0x73], 0x6c6c6174
        and    byte ptr [ecx + 0x74], ch
        jae    loc_2_00C9
        push   0x206b6f6f
        outsw  dx, word ptr [esi]
        jb     0xd2
loc_2_00B2:
        jae    0x123
        je     0x12e
loc_2_00B7:
        popal  
        jb     0x11f
        and    byte ptr [ecx + 0x6e], ch
        je     0x124
        jb     0x133
        jne    0x133
        je     0xd2
        or     ch, byte ptr [esi + 0x75]
        insd   dword ptr es:[edi], dx
loc_2_00C9:
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [ecx], dh
        push   0x726f6620
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x65], ch
        jns    0x13f
        outsd  dx, dword ptr [esi]
        popal  
        jb     0x145
        or     eax, 0x500a0d0a
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x151
        and    byte ptr [edx + 0x75], dh
loc_2_00EF:
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x16f
        and    byte ptr [ebx + 0x65], dl
        je     0x176
        jo     0x123
        jo     0x177
        outsd  dx, dword ptr [esi]
        jb     0x16a
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x67], ah
        popal  
        imul   ebp, dword ptr [esi + 0x2e], 0xa0d
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x8d0  R-X
        db  0B9h, 000h, 000h, 001h, 000h, 0E4h, 064h, 0A8h, 002h, 0E0h, 0FAh, 0C3h
sub_3_000C:
        mov    ecx, 0x10000
loc_3_0011:
        in     al, 0x64
        test   al, 1
        loope  loc_3_0011
        ret    
loc_3_0018:
        mov    dword ptr [0x1229], ebx       ; -> obj1:0x1229
        push   0
        push   0xfa
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_3_0037
loc_3_0031:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_3_0037:
        mov    dword ptr [0x1210], eax       ; -> obj1:0x1210
        xor    eax, eax
        inc    eax
        push   3
        pop    ecx
        xor    esi, esi
        mov    edi, 0x7f8                    ; -> loc_3_07F8
        VMMCall  VMM.Get_Profile_Fixed_Point     ; cd20ad000100
        mov    dword ptr [0x1224], eax       ; -> obj1:0x1224
        mov    eax, 0x1f4
        push   3
        pop    ecx
        mov    edi, 0x8a1                    ; -> obj3:0x08A1
        VMMCall  VMM.Get_Profile_Fixed_Point     ; cd20ad000100
        mov    dword ptr [0x1255], eax       ; -> obj1:0x1255
        or     eax, 0xffffffff
        mov    edi, 0x896                    ; -> obj3:0x0896
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0x125a], al         ; -> loc_1_125A
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        add    esi, ebx
        call   sub_7_0000
        cmp    dword ptr [0x1231], 0         ; -> loc_1_1231
        jne    loc_3_00AB
        mov    eax, 0x10038
        mov    esi, 0x835                    ; -> loc_1_0835
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0x1231], esi       ; -> loc_1_1231
loc_3_00AB:
        xor    eax, eax
        push   0x18
        pop    ecx
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_3_0031
        mov    dword ptr [0x1308], esi       ; -> loc_1_1308
        xor    eax, eax
        push   0xc
        pop    ecx
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_3_0031
        mov    dword ptr [0x130c], esi       ; -> loc_1_130C
        mov    edi, 0x774                    ; -> loc_3_0774
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_01B6
        mov    dword ptr [0x120c], eax       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 8
        je     loc_3_010C
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_3_010C:
        call   0
        jne    loc_3_0140
        mov    al, 0xad
        out    0x64, al
        jmp    loc_3_0119
loc_3_0119:
        in     al, 0x64
        test   al, 1
        je     loc_3_0125
        in     al, 0x60
        jmp    loc_3_0123
loc_3_0123:
        jmp    loc_3_0119
loc_3_0125:
        call   0
        jne    loc_3_0140
        mov    al, 0x20
        out    0x64, al
        jmp    loc_3_0132
loc_3_0132:
        call   0
        jne    loc_3_0140
        call   sub_3_000C
        jne    loc_3_0144
loc_3_0140:
        mov    al, 0x45
        jmp    loc_3_015E
loc_3_0144:
        in     al, 0x60
        jmp    loc_3_0148
loc_3_0148:
        jmp    loc_3_014A
loc_3_014A:
        mov    ah, al
        in     al, 0x64
        jmp    loc_3_0150
loc_3_0150:
        jmp    loc_3_0152
loc_3_0152:
        in     al, 0x60
        jmp    loc_3_0156
loc_3_0156:
        jmp    loc_3_0158
loc_3_0158:
        mov    al, ah
        or     al, 1
        and    al, 0xef
loc_3_015E:
        mov    byte ptr [0x1259], al         ; -> obj1:0x1259
        call   0
        mov    al, 0xae
        out    0x64, al
        mov    dword ptr [0x1214], ebx       ; -> obj1:0x1214
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        add    esi, ebx
        call   sub_1_0922
        mov    dword ptr [0x122d], eax       ; -> obj1:0x122D
        mov    word ptr [esi + 0x36], ax
        or     dword ptr [esi], 0x100000
        and    ax, 0x70
        mov    byte ptr [esi + 0x41], al
        mov    al, byte ptr [0x1259]         ; -> obj1:0x1259
        mov    byte ptr [esi + 0x3c], al
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        call   sub_3_0470
        or     byte ptr [0x1251], 0x10       ; -> obj1:0x1251
        clc    
        ret    
loc_3_01B6:
        movzx  esi, word ptr [0xc]           ; -> loc_2_000C
        add    esi, dword ptr [4]            ; -> loc_2_0004
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_3_01CB:
        call   sub_3_051C
        jb     loc_3_0031
        push   0
        push   0x794                         ; -> obj3:0x0794
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_3_0031
        push   0
        push   0x7a8                         ; -> obj3:0x07A8
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_3_0031
        push   0
        push   0x7bc                         ; -> loc_3_07BC
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_3_0031
        push   0
        push   0x7d0                         ; -> loc_3_07D0
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_3_0031
        call   sub_4_06A2
        call   sub_1_0FA3
        call   sub_4_0698
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_3_0252:
        mov    byte ptr [ebp + 0x1d], 1
        xor    eax, eax
        mov    al, 0x16
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 6
        jb     loc_3_0278
        mov    byte ptr [ebp + 0x1d], 0
        xor    eax, eax
        mov    al, 0x16
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        jmp    loc_3_0252
loc_3_0278:
        mov    dword ptr [ebp + 0x1c], 0x1607
        mov    dword ptr [ebp + 0x10], 0xd
        xor    eax, eax
        mov    dword ptr [ebp + 0x18], eax
        mov    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], eax
        mov    word ptr [ebp + 0x38], ax
        xor    eax, eax
        mov    al, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        je     loc_3_02F6
        movzx  ecx, word ptr [ebp]
        cmp    dword ptr [ecx + eax], 0xeff2e9c
        jne    loc_3_02F6
        cmp    dword ptr [ecx + eax + 6], 0x9d900175
        jne    loc_3_02F6
        movzx  edx, word ptr [ecx + eax + 4]
        add    edx, eax
        mov    dword ptr [0x121c], edx       ; -> obj1:0x121C
        mov    dword ptr [0x7ec], edx        ; -> obj3:0x07EC
        shl    eax, 0xc
        add    ecx, 8
        mov    ax, cx
        mov    dword ptr [0x1218], eax       ; -> obj1:0x1218
        mov    esi, 0x3d4                    ; -> loc_4_03D4
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jae    loc_3_030B
loc_3_02F0:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_3_02F6:
        movzx  esi, word ptr [0xe]           ; -> loc_2_000E
        add    esi, dword ptr [4]            ; -> loc_2_0004
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_3_030B:
        push   0
        push   0x7e4                         ; -> obj3:0x07E4
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_3_02F0
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        add    esi, ebx
        mov    edx, esi
        push   ebx
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        and    ebx, 0x70
        mov    dword ptr [0x122d], ebx       ; -> obj1:0x122D
        mov    ax, word ptr [esi + 0x36]
        call   sub_1_0655
        pop    ebx
        je     loc_3_0360
        call   sub_1_01B0
loc_3_0360:
        mov    al, 1
        VxDCall  VKD.VKD_Define_Paste_Mode       ; cd2009000d00
        mov    eax, 0x15
        mov    esi, 0x6a                     ; -> loc_4_006A
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        clc    
        ret    
loc_3_037A:
        VxDCall  SHELL.SHELL_Get_Version         ; cd2000001700
        jb     loc_3_03B3
        cmp    eax, 0x314
        jb     loc_3_03B3
        inc    byte ptr [0x1220]             ; -> obj1:0x1220
        mov    eax, 5
        sub    edx, edx
        mov    esi, 0x9b                     ; -> loc_7_009B
        VxDCall  SHELL.SHELL_Hook_Properties     ; cd200a001700
        mov    eax, 2
        xor    edx, edx
        mov    esi, 0x40a                    ; -> sub_4_040A
        VxDCall  SHELL.SHELL_Hook_Properties     ; cd200a001700
loc_3_03B3:
        VxDCall  VMM_STRING.Service_0000h        ; cd2000001800
        jb     loc_3_03D5
        cmp    eax, 0x400
        jb     loc_3_03D5
        mov    word ptr [0x3f7], 0x20cd      ; -> loc_4_03F7
        mov    dword ptr [0x3f9], 0x180003   ; -> obj4:0x03F9
loc_3_03D5:
        clc    
        ret    
        db  000h
sub_3_03D8:
        cmp    al, 0x61
        jb     loc_3_03E2
        cmp    al, 0x7a
        ja     loc_3_03E2
        sub    al, 0x20
loc_3_03E2:
        ret    
sub_3_03E3:
        cld    
loc_3_03E4:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_3_03EF
        cmp    al, 0x20
        jbe    loc_3_03E4
        or     al, al
loc_3_03EF:
        ret    
sub_3_03F0:
        pushal 
        or     cl, 0xc0
        cmp    byte ptr [edx], 0
        je     loc_3_046E
        VMMCall  VMM.Convert_Hex_String          ; cd20b0000100
        cmp    eax, 0x100
        jae    loc_3_046E
        mov    ebx, eax
        mov    esi, edx
        call   sub_3_03E3
        je     loc_3_0468
        cmp    al, 0x2c
        jne    loc_3_046E
        call   sub_3_03E3
        je     loc_3_0468
        cmp    al, 0x2d
        jne    loc_3_0425
        or     cl, 0x1f
        jmp    loc_3_043B
loc_3_0425:
        cmp    al, 0x2c
        je     loc_3_0446
        mov    edx, esi
        dec    edx
        VMMCall  VMM.Convert_Decimal_String      ; cd20ac000100
        cmp    eax, 0x1f
        jae    loc_3_046E
        or     cl, al
        mov    esi, edx
loc_3_043B:
        call   sub_3_03E3
        je     loc_3_0468
        cmp    al, 0x2c
        jne    loc_3_046E
loc_3_0446:
        call   sub_3_03E3
        je     loc_3_0468
        call   sub_3_03D8
        cmp    al, 0x46
        je     loc_3_0462
        cmp    al, 0x4e
        jne    loc_3_0468
        and    cl, 0x3f
        or     cl, 0x40
        jmp    loc_3_0468
loc_3_0462:
        and    cl, 0x3f
        or     cl, 0x80
loc_3_0468:
        mov    byte ptr [ebx + 0x131c], cl   ; -> loc_1_131C
loc_3_046E:
        popal  
        ret    
sub_3_0470:
        mov    edx, 0x64
        mov    esi, 0xac4                    ; -> loc_1_0AC4
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x60
        mov    esi, 0xbd8                    ; -> loc_1_0BD8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        xor    eax, eax
        test   ebx, 0x40000
        je     loc_3_04A1
        dec    eax
loc_3_04A1:
        xor    esi, esi
        mov    edi, 0x872                    ; -> loc_3_0872
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     eax, eax
        je     loc_3_04DC
        mov    byte ptr [0x13c0], 1          ; -> loc_1_13C0
        or     byte ptr [0x13c0], 0xc0       ; -> loc_1_13C0
        mov    byte ptr [0x13c1], 0x1f       ; -> obj1:0x13C1
        or     byte ptr [0x13c1], 0xa0       ; -> obj1:0x13C1
        mov    byte ptr [0x13c2], 0          ; -> loc_1_13C2
        or     byte ptr [0x13c2], 0xa0       ; -> loc_1_13C2
loc_3_04DC:
        mov    edi, 0x87d                    ; -> loc_3_087D
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
loc_3_04E7:
        jb     loc_3_04FB
        mov    ecx, 0
        call   sub_3_03F0
        VMMCall  VMM.Get_Next_Profile_String     ; cd20b4000100
        jmp    loc_3_04E7
loc_3_04FB:
        mov    edi, 0x889                    ; -> obj3:0x0889
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
loc_3_0506:
        jb     loc_3_051A
        mov    ecx, 0x20
        call   sub_3_03F0
        VMMCall  VMM.Get_Next_Profile_String     ; cd20b4000100
        jmp    loc_3_0506
loc_3_051A:
        ret    
        db  000h
sub_3_051C:
        pushal 
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jae    loc_3_0767
        mov    dword ptr [0x14b4], ebx       ; -> obj1:0x14B4
        mov    eax, 0xf4                     ; -> sub_6_00F4
        mov    ecx, 0x44b                    ; -> loc_6_044B
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        call   VKD_Disable_Keyboard
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    eax, 0x11
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        test   byte ptr [ebp + 0x1c], 4
        je     loc_3_06BD
        mov    word ptr [ebp + 0x1c], 0xc205
        mov    byte ptr [ebp + 0x11], 3
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    word ptr [ebp + 0x1c], 0xc201
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    word ptr [ebp + 0x1c], 0xc202
        mov    byte ptr [ebp + 0x11], 0
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    word ptr [ebp + 0x1c], 0xc204
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [0x1508], ax         ; -> obj1:0x1508
        mov    word ptr [ebp + 0x1c], 0xc203
        mov    byte ptr [ebp + 0x11], 3
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    esi, 0x3a8                    ; -> loc_6_03A8
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    word ptr [ebp + 0x1c], 0xc207
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    word ptr [ebp + 0x1c], 0xc206
        mov    byte ptr [ebp + 0x11], 1
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    word ptr [ebp + 0x1c], 0xc202
        mov    byte ptr [ebp + 0x11], 2
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    word ptr [ebp + 0x1c], 0xc200
        mov    byte ptr [ebp + 0x11], 1
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_3_06BD
        mov    byte ptr [0x1511], 0xff       ; -> loc_1_1511
loc_3_06BD:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        call   VKD_Enable_Keyboard
        mov    eax, 0x15
        mov    esi, 0x158                    ; -> loc_6_0158
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        jne    loc_3_0767
        mov    edi, 0x8b0                    ; -> obj3:0x08B0
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_0771
        mov    dword ptr [0x14fe], eax       ; -> obj1:0x14FE
        mov    eax, 0x15
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0x14ac], ecx       ; -> obj1:0x14AC
        mov    dword ptr [0x14b0], edx       ; -> obj1:0x14B0
        mov    esi, 0x135                    ; -> loc_6_0135
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x15
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        push   0
        push   7
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        mov    dword ptr [0x150c], eax       ; -> obj1:0x150C
        call   sub_7_00F0
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        test   eax, eax
        je     loc_3_0767
        mov    ecx, 4
        mov    eax, 0xc
        VxDCall  VMD.VMD_Set_Mouse_Type          ; cd2001000c00
loc_3_0767:
        mov    byte ptr [0x1512], 0xff       ; -> obj1:0x1512
        popal  
        clc    
        ret    
loc_3_0771:
        popal  
        stc    
        ret    
loc_3_0774:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 0xe
        add    byte ptr [eax], al
        mov    al, 7
        add    byte ptr [eax], al
        add    byte ptr [edi], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, cl                        ; -> loc_1_07CC
        pop    es
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0x29000004], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4], dh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
loc_3_07BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], 0
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
loc_3_07D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   esi, eax
        add    al, 0
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        db  000h
loc_3_07F8:
        dec    ebx
        jns    loc_3_083E
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        jae    loc_3_0874
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x746c4100
        dec    ebx
        jns    loc_3_0850
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_3_0811
loc_3_0811:
        dec    ebx
        jns    0x865
        popal  
        jae    0x88c
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
        dec    ebx
        jns    loc_3_0875
        popal  
        jae    loc_3_089C
        push   ebx
        imul   ebp, dword ptr [ecx + 0x70], 0x43
        outsd  dx, dword ptr [esi]
        jne    loc_3_089F
        je     loc_3_0833
loc_3_0833:
        dec    ebx
        jns    loc_3_0887
        popal  
        jae    0x8ae
        inc    ebx
        push   edx
        push   ebx
loc_3_083E:
        imul   ebp, dword ptr [ecx + 0x70], 0x43
        outsd  dx, dword ptr [esi]
        jne    0x8b3
        je     loc_3_0847
loc_3_0847:
        dec    ebx
        jns    loc_3_089B
        popal  
        jae    loc_3_08C2
        inc    esp
loc_3_0850:
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_3_0855
loc_3_0855:
        inc    ecx
        insb   byte ptr es:[edi], dx
        je     loc_3_08A9
        popal  
        jae    0x8d0
        inc    esp
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_3_0863
loc_3_0863:
        dec    ebx
        jns    loc_3_08A9
        jne    0x8cf
        jb     0x8b1
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_3_0872
loc_3_0872:
        dec    ebx
        pop    ecx
loc_3_0874:
        inc    edx
loc_3_0875:
        inc    esp
        push   eax
        inc    ecx
        push   ebx
        push   ebx
        push   edi
        inc    esp
        db  000h
loc_3_087D:
        cmp    byte ptr [eax], dh
        xor    al, 0x32
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        inc    ebx
        dec    ebp
loc_3_0887:
        inc    esp
        add    byte ptr [eax], bh
        xor    byte ptr [edx + esi], dh
        push   edi
        push   edx
        dec    ecx
        push   esp
        inc    ebp
        inc    ebx
        dec    ebp
        inc    esp
        add    byte ptr [ebx + 0x59], cl
        inc    edx
        inc    esp
        push   edx
loc_3_089B:
        inc    ebp
loc_3_089C:
        inc    edx
        dec    edi
        dec    edi
loc_3_089F:
        push   esp
        add    byte ptr [ebx + 0x45], cl
        pop    ecx
        dec    ecx
        inc    esp
        dec    esp
        inc    ebp
        inc    esp
loc_3_08A9:
        inc    ebp
        dec    esp
        inc    ecx
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    al, byte ptr [eax]
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08C2:
        add    byte ptr [eax], al
        pushfd 
        add    eax, dword ptr [eax]
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x6ac  R-X
VKD_Disable_Keyboard:
        push   ebx
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        and    byte ptr [0x1251], 0xef       ; -> obj1:0x1251
        mov    edx, 0x64
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x60
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        call   sub_4_06A2
        pop    ebx
        ret    
VKD_Enable_Keyboard:
        mov    edx, 0x64
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x60
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        or     byte ptr [0x1251], 0x10       ; -> obj1:0x1251
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        call   sub_4_0698
        ret    
loc_4_006A:
        mov    edx, dword ptr [0x1210]       ; -> obj1:0x1210
        add    edx, ebx
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ah, 0x4f
        jne    loc_4_0085
        cmp    al, 0xe1
        je     loc_4_00ED
        test   byte ptr [edx + 3], 1
        jne    loc_4_00CD
loc_4_0085:
        cmp    ax, 0x9002
        jne    loc_4_00CB
        cmp    byte ptr [ebp + 0x29], 0xf0
        jb     loc_4_00CB
        mov    ax, word ptr [0x41a]
        cmp    ax, word ptr [0x41c]
        jne    loc_4_00BF
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        sub    word ptr [ebp + 0x24], 2
        VMMCall  VMM.Test_VM_Ints_Enabled        ; cd20f9000100
        jne    loc_4_00BD
        or     byte ptr [edx + 2], 0x40
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_4_00BD:
        clc    
        ret    
loc_4_00BF:
        btr    dword ptr [edx], 0x16
        jae    loc_4_00CB
        VMMCall  VMM.Disable_VM_Ints             ; cd201b000100
loc_4_00CB:
        stc    
        ret    
loc_4_00CD:
        cmp    al, 0xc5
        jne    loc_4_00F1
        xor    eax, eax
        mov    ecx, 0x6b
        mov    esi, 0x302                    ; -> loc_1_0302
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        and    byte ptr [edx + 3], 0xfe
        or     byte ptr [edx + 2], 0x80
        jmp    loc_4_00F1
loc_4_00ED:
        or     byte ptr [edx + 3], 1
loc_4_00F1:
        and    byte ptr [ebp + 0x2c], 0xfe
        clc    
        ret    
loc_4_00F7:
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        jne    loc_4_0105
        call   sub_6_00F4
loc_4_0105:
        test   edx, edx
        je     loc_4_010E
        cmp    edx, 0xd
        jne    loc_4_0156
loc_4_010E:
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        je     loc_4_0156
        call   sub_4_0698
        push   ebx
        mov    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        call   sub_1_08F6
        pop    ebx
        mov    dword ptr [0x1214], ebx       ; -> obj1:0x1214
        mov    edx, dword ptr [0x1210]       ; -> obj1:0x1210
        add    edx, ebx
        test   dword ptr [edx], 0x100000
        je     loc_4_0156
        mov    esi, edx
        movzx  eax, word ptr [edx + 0x36]
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        call   sub_1_0655
        je     loc_4_0158
loc_4_0151:
        call   sub_1_01B0
loc_4_0156:
        clc    
        ret    
loc_4_0158:
        push   eax
        mov    al, byte ptr [edx + 0x34]
        cmp    al, byte ptr [edx + 0x35]
        pop    eax
        je     loc_4_0156
        jmp    loc_4_0151
VKD_Get_Version:
        mov    eax, 0x20a
        clc    
        ret    
VKD_Define_Hot_Key:
        push   ecx
        push   edi
        xor    edi, edi
        test   cl, 0x80
        je     loc_4_018E
        and    cl, 0x7f
loc_4_0177:
        bsf    edi, dword ptr [0x124d]       ; -> obj1:0x124D
        je     loc_4_018B
        btr    dword ptr [0x124d], edi       ; -> obj1:0x124D
        jae    loc_4_0177
        jmp    loc_4_018E
loc_4_018B:
        stc    
        jmp    loc_4_01E7
loc_4_018E:
        push   edi
        push   esi
        mov    edi, eax
        mov    esi, dword ptr [0x1308]       ; -> loc_1_1308
        VMMCall  VMM.List_Allocate               ; cd209d000100
        xchg   edi, eax
        mov    word ptr [edi + 6], bx
        ror    ebx, 0x10
        mov    word ptr [edi + 4], bx
        ror    ebx, 0x10
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], 0
        or     ah, ah
        je     loc_4_01C2
        cmp    ah, 0xff
        je     loc_4_01C2
        mov    ah, 0x80
        or     byte ptr [edi + 1], 0x80
loc_4_01C2:
        mov    byte ptr [edi + 2], ah
        test   cl, 0xf
        jne    loc_4_01CD
        or     cl, 0xf
loc_4_01CD:
        mov    byte ptr [edi + 3], cl
        pop    dword ptr [edi + 0xc]
        mov    dword ptr [edi + 0x14], edx
        pop    dword ptr [edi + 8]
        pop    eax
        push   eax
        mov    dword ptr [edi + 0x10], eax
        mov    eax, edi
        VMMCall  VMM.List_Attach                 ; cd209e000100
        clc    
loc_4_01E7:
        pop    edi
        pop    ecx
        ret    
VKD_Remove_Hot_Key:
        pushal 
        cmp    dword ptr [eax + 8], 0
        je     loc_4_021A
        mov    ecx, dword ptr [eax + 8]
        bts    dword ptr [0x124d], ecx       ; -> obj1:0x124D
        mov    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_4_0207:
        btr    dword ptr [esi + ebx + 0xc], ecx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_4_0207
loc_4_021A:
        mov    esi, dword ptr [0x1308]       ; -> loc_1_1308
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        popal  
        ret    
VKD_Local_Enable_Hot_Key:
        push   ebx
        push   ecx
        mov    ecx, dword ptr [eax + 8]
        cmp    cl, 0
        je     loc_4_0242
        add    ebx, dword ptr [0x1210]       ; -> obj1:0x1210
        btr    dword ptr [ebx + 0xc], ecx
loc_4_0242:
        pop    ecx
        pop    ebx
        ret    
VKD_Local_Disable_Hot_Key:
        push   ebx
        push   ecx
        mov    ecx, dword ptr [eax + 8]
        cmp    cl, 0
        je     loc_4_0259
        add    ebx, dword ptr [0x1210]       ; -> obj1:0x1210
        bts    dword ptr [ebx + 0xc], ecx
loc_4_0259:
        pop    ecx
        pop    ebx
        ret    
VKD_Reflect_Hot_Key:
        pushal 
        mov    esi, ebx
        add    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        mov    edx, ebx
        mov    edi, eax
        movzx  eax, word ptr [esi + 0x36]
        mov    ebx, ecx
        call   sub_1_0655
        call   sub_1_03A4
        movzx  eax, word ptr [edi]
        call   sub_1_057E
        test   byte ptr [edi + 3], 0x80
        je     loc_4_0290
        and    byte ptr [edi + 3], 0x7f
        mov    dword ptr [0x1239], eax       ; -> obj1:0x1239
loc_4_0290:
        or     al, 0x80
        call   sub_1_057E
        call   sub_4_05E7
        popal  
        ret    
VKD_Cancel_Hot_Key_State:
        pushfd 
        pushal 
        mov    esi, dword ptr [0x1214]       ; -> obj1:0x1214
        add    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        call   sub_4_05E7
        popal  
        popfd  
        ret    
sub_4_02B4:
        test   byte ptr [esi + 1], 0x20
        jne    loc_4_032B
        mov    al, byte ptr [esi + 0x35]
        cmp    al, byte ptr [esi + 0x34]
        jne    loc_4_032B
        mov    edi, 0x41a
        add    edi, dword ptr [edx + 4]
        movzx  eax, word ptr [edi]
        movzx  ebx, word ptr [edi + 2]
        cmp    ebx, eax
        je     loc_4_030B
        ja     loc_4_02E6
        mov    edi, 0x480
        add    edi, dword ptr [edx + 4]
        sub    bx, word ptr [edi]
        add    bx, word ptr [edi + 2]
loc_4_02E6:
        sub    ebx, eax
        shr    ebx, 1
        cmp    ebx, 8
        jbe    loc_4_030B
        push   edx
        push   esi
        mov    eax, dword ptr [esi + 0x4f]
        mov    ebx, edx
        mov    edx, esi
        mov    esi, 0x3c1                    ; -> loc_4_03C1
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        pop    esi
        pop    edx
        or     byte ptr [esi + 1], 0x20
        jmp    loc_4_032B
loc_4_030B:
        mov    edi, dword ptr [esi + 0x14]
        cmp    edi, dword ptr [esi + 0x18]
        jb     loc_4_032C
        movzx  eax, word ptr [esi + 0x36]
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        call   sub_1_0655
        xor    eax, eax
        mov    ebx, edx
        call   sub_8_003D
loc_4_032B:
        ret    
loc_4_032C:
        mov    eax, edi
        add    eax, 4
        mov    dword ptr [esi + 0x14], eax
        movzx  eax, word ptr [esi + 0x36]
        movzx  ebx, word ptr [edi + 2]
        cmp    ebx, 0xffff
        jne    loc_4_0346
        xor    ebx, ebx
loc_4_0346:
        pushfd 
        and    ebx, 6
        call   sub_1_0655
        popfd  
        je     loc_4_0363
        mov    al, byte ptr [edi + 1]
        call   sub_1_046C
        or     al, 0x80
        call   sub_1_046C
        jmp    loc_4_032B
loc_4_0363:
        mov    al, 0x38
        call   sub_1_046C
        mov    al, byte ptr [edi]
        aam    0xa
        mov    cl, al
        mov    al, ah
        aam    0xa
        mov    ch, al
        movzx  ebx, ah
        mov    al, byte ptr [ebx + 0x12fe]   ; -> loc_1_12FE
        call   sub_1_046C
        or     al, 0x80
        call   sub_1_046C
        movzx  ebx, ch
        mov    al, byte ptr [ebx + 0x12fe]   ; -> loc_1_12FE
        call   sub_1_046C
        or     al, 0x80
        call   sub_1_046C
        movzx  ebx, cl
        mov    al, byte ptr [ebx + 0x12fe]   ; -> loc_1_12FE
        call   sub_1_046C
        or     al, 0x80
        call   sub_1_046C
        mov    al, 0xb8
        call   sub_1_046C
        jmp    loc_4_032B
loc_4_03C1:
        and    byte ptr [edx + 1], 0xdf
        jmp    sub_1_01B0
loc_4_03CA:
        and    byte ptr [edx + 2], 0xfe
        jmp    sub_1_01B0
        db  090h
loc_4_03D4:
        mov    edi, ebx
        add    edi, dword ptr [0x1210]       ; -> obj1:0x1210
        inc    dword ptr [ebp + 0x24]
        test   byte ptr [edi + 1], 4
        jne    loc_4_040F
        movzx  edx, word ptr [ebp + 0x34]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 0x30]
        add    edx, dword ptr [ebx + 4]
        movzx  edx, word ptr [ecx + edx]
loc_4_03F7:
        VxDCall  VMM_STRING.Service_0000h        ; cd2000001800
loc_4_03FD:
        mov    edx, dword ptr [0x121c]       ; -> obj1:0x121C
        add    edx, dword ptr [ebx + 4]
        mov    word ptr [edx], ax
        ret    
sub_4_040A:
        xor    eax, eax
        inc    eax
        jmp    loc_4_03FD
loc_4_040F:
        test   byte ptr [edi], 0x80
        jne    sub_4_040A
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 0x10
        jne    sub_4_040A
        xor    esi, esi
        xchg   dword ptr [edi + 0x24], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        test   byte ptr [edi + 1], 2
        je     loc_4_044A
        mov    esi, 0x41a
        add    esi, dword ptr [ebx + 4]
        mov    ax, word ptr [esi + 2]
        cmp    ax, word ptr [esi]
        jne    sub_4_040A
        and    byte ptr [edi + 1], 0xfd
loc_4_044A:
        movzx  edx, word ptr [ebp + 0x34]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 0x30]
        add    edx, dword ptr [ebx + 4]
        test   byte ptr [ecx + edx], 0x40
        je     sub_4_040A
        dec    byte ptr [edi + 0x38]
        jne    sub_4_040A
        mov    esi, dword ptr [edi + 0x14]
        cmp    esi, dword ptr [edi + 0x18]
        jb     loc_4_0474
        xor    eax, eax
        call   sub_8_003D
        jmp    sub_4_040A
loc_4_0474:
        cld    
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [edi + 0x14], esi
        cmp    ah, 0xff
        jne    loc_4_0480
        xor    ah, ah
loc_4_0480:
        mov    cl, byte ptr [edi + 0x58]
        cmp    al, 0xd
        je     loc_4_048A
        mov    cl, byte ptr [edi + 0x57]
loc_4_048A:
        mov    byte ptr [edi + 0x38], cl
        call   sub_4_04A8
        or     al, al
        je     loc_4_04A3
        sub    dword ptr [edi + 0x14], 4
        cmp    al, 0xff
        jne    loc_4_04A3
        call   sub_6_00C1
loc_4_04A3:
        jmp    sub_4_040A
sub_4_04A8:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        or     byte ptr [edi], 0x80
        mov    word ptr [ebp + 0x18], ax
        mov    word ptr [ebp + 0x1c], 0x5ff
        xor    eax, eax
        mov    al, 0x16
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    al, byte ptr [ebp + 0x1c]
        and    byte ptr [edi], 0x7f
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
        ret    
loc_4_04EC:
        xchg   ebx, eax
        mov    al, byte ptr [esp + 0x1c]
        mov    ah, 0x40
        btr    eax, 7
        jae    loc_4_04FC
        add    ah, 1
loc_4_04FC:
        call   sub_4_057E
        xchg   ebx, eax
        jmp    loc_1_0A85
sub_4_0507:
        push   eax
        push   ecx
        mov    ecx, ebx
        xchg   dword ptr [0x1310], ecx       ; -> loc_1_1310
        or     ecx, ecx
        je     loc_4_0533
        cmp    ecx, ebx
        je     loc_4_0533
        test   byte ptr [ecx + 3], 8
        je     loc_4_0533
        push   ebx
        mov    ebx, ecx
        mov    al, byte ptr [ebx]
        mov    ah, 3
        call   sub_4_057E
        and    byte ptr [0x1251], 0xfe       ; -> obj1:0x1251
        pop    ebx
loc_4_0533:
        pop    ecx
        or     byte ptr [0x1251], 1          ; -> obj1:0x1251
        xor    ah, ah
        test   al, 0x80
        je     loc_4_054F
        and    byte ptr [ebx + 3], 0x7f
        test   byte ptr [ebx + 3], 2
        je     loc_4_056B
        inc    ah
        jmp    loc_4_0572
loc_4_054F:
        or     byte ptr [ebx + 3], 0x80
        cmp    al, byte ptr [0x1314]         ; -> loc_1_1314
        jne    loc_4_0565
        test   byte ptr [ebx + 3], 4
        je     loc_4_056B
        mov    ah, 2
        jmp    loc_4_0572
loc_4_0565:
        test   byte ptr [ebx + 3], 1
        jne    loc_4_0572
loc_4_056B:
        mov    byte ptr [0x1314], al         ; -> loc_1_1314
        jmp    loc_4_057C
loc_4_0572:
        mov    byte ptr [0x1314], al         ; -> loc_1_1314
        call   sub_4_057E
loc_4_057C:
        pop    eax
        ret    
sub_4_057E:
        pushal 
        xor    edi, edi
        mov    ecx, dword ptr [0x122d]       ; -> obj1:0x122D
        test   byte ptr [ebx + 3], 0x10
        je     loc_4_05DF
        push   eax
        mov    esi, dword ptr [0x130c]       ; -> loc_1_130C
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    edx, eax
        mov    dword ptr [edx], ebx
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [edx + 4], eax
        pop    eax
        mov    word ptr [edx + 8], ax
        mov    word ptr [edx + 0xa], cx
        mov    eax, edx
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        bts    dword ptr [0x1251], 3         ; -> obj1:0x1251
        jb     loc_4_05E5
        mov    eax, 0x10
        mov    ecx, 3
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0x646                    ; -> loc_4_0646
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        jmp    loc_4_05E5
loc_4_05DF:
        mov    edx, dword ptr [ebx + 0x14]
        call   dword ptr [ebx + 0xc]
loc_4_05E5:
        popal  
        ret    
sub_4_05E7:
        xor    ebx, ebx
        xchg   dword ptr [0x1310], ebx       ; -> loc_1_1310
        or     ebx, ebx
        je     loc_4_0645
        mov    al, byte ptr [0x1314]         ; -> loc_1_1314
        test   al, 0x80
        jne    loc_4_0611
        and    byte ptr [ebx + 3], 0x7f
        or     al, 0x80
        call   sub_4_0507
        mov    dword ptr [0x1310], 0         ; -> loc_1_1310
loc_4_0611:
        and    byte ptr [0x1251], 0xfe       ; -> obj1:0x1251
        test   byte ptr [ebx + 3], 8
        je     loc_4_0627
        mov    al, byte ptr [ebx]
        mov    ah, 3
        call   sub_4_057E
loc_4_0627:
        push   edx
        mov    edx, esi
        sub    edx, dword ptr [0x1210]       ; -> obj1:0x1210
        movzx  eax, word ptr [esi + 0x36]
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        call   sub_1_0655
        call   sub_1_03A4
        pop    edx
loc_4_0645:
        ret    
loc_4_0646:
        and    byte ptr [0x1251], 0xf7       ; -> obj1:0x1251
loc_4_064D:
        mov    esi, dword ptr [0x130c]       ; -> loc_1_130C
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_0696
        mov    edx, eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    ebx, dword ptr [edx]
        mov    edi, dword ptr [edx + 4]
        movzx  ecx, word ptr [edx + 8]
        movzx  edx, word ptr [edx + 0xa]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        xchg   edi, eax
        sub    edi, eax
        mov    eax, ecx
        mov    ecx, edx
        mov    edx, dword ptr [ebx + 0x10]
        or     edx, edx
        je     loc_4_068E
        cmp    edi, edx
        ja     loc_4_064D
loc_4_068E:
        mov    edx, dword ptr [ebx + 0x14]
        call   dword ptr [ebx + 0xc]
        jmp    loc_4_064D
loc_4_0696:
        ret    
        db  000h
sub_4_0698:
        push   eax
        mov    al, 0xae
        call   sub_1_0F3D
        pop    eax
        ret    
sub_4_06A2:
        push   eax
        mov    al, 0xad
        call   sub_1_0F3D
        pop    eax
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x126  R-X
loc_5_0000:
        movzx  eax, word ptr [0x417]
        mov    dword ptr [0x122d], eax       ; -> obj1:0x122D
        clc    
        ret    
loc_5_000E:
        mov    eax, dword ptr [0x1218]       ; -> obj1:0x1218
        or     eax, eax
        je     loc_5_001D
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_5_001D:
        mov    eax, dword ptr [0x122d]       ; -> obj1:0x122D
        mov    word ptr [0x417], ax
        call   VKD_Disable_Keyboard
        call   sub_5_005C
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        call   sub_4_0698
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_7_0083
loc_5_0052:
        mov    al, 0xae
        call   sub_1_0F3D
        clc    
        ret    
        db  000h
sub_5_005C:
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        jne    loc_5_0125
        mov    byte ptr [0x1514], 0xff       ; -> obj1:0x1514
        mov    word ptr [0x14fc], 0          ; -> obj1:0x14FC
        mov    eax, dword ptr [0x14fe]       ; -> obj1:0x14FE
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    byte ptr [0x1513], 0xff       ; -> loc_1_1513
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0xc201
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        mov    eax, dword ptr [0x150c]       ; -> obj1:0x150C
        add    eax, ebx
        cmp    byte ptr [eax + 4], 0xff
        je     loc_5_0109
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_5_0109
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 0x1c], 0xc207
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
        mov    word ptr [ebp + 0x1c], 0xc200
        mov    word ptr [ebp + 0x10], 0x100
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [ebp + 0x2c], 0
loc_5_0109:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    byte ptr [0x1513], 0          ; -> loc_1_1513
loc_5_0125:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x44c  R-X
VKD_Define_Paste_Mode:
        push   ebx
        add    ebx, dword ptr [0x1210]       ; -> obj1:0x1210
        and    byte ptr [ebx + 1], 0x7f
        or     al, al
        je     loc_6_0013
        or     byte ptr [ebx + 1], 0x80
loc_6_0013:
        pop    ebx
        ret    
loc_6_0015:
        pop    ecx
        popal  
loc_6_0017:
        stc    
        ret    
VKD_Start_Paste:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_6_0017
        pushal 
        mov    edi, ebx
        add    edi, dword ptr [0x1210]       ; -> obj1:0x1210
        mov    dword ptr [edi + 0x1c], esi
        mov    dword ptr [edi + 0x20], edx
        shl    ecx, 2
        push   ecx
        add    ecx, 0xfff
        shr    ecx, 0xc
        xor    eax, eax
        push   0x80
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        je     loc_6_0015
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], edx
        pop    ecx
        mov    eax, edx
        add    eax, ecx
        mov    dword ptr [edi + 0x18], eax
        xchg   edx, edi
        mov    esi, dword ptr [esp + 0x1c]
        cld    
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xchg   edx, edi
        test   byte ptr [edi + 1], 0x80
        jne    loc_6_00A1
        or     byte ptr [edi + 1], 4
        or     byte ptr [edi + 1], 2
        mov    eax, dword ptr [edi + 0x53]
        mov    edx, ebx
        mov    esi, 0xc1                     ; -> sub_6_00C1
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        mov    dword ptr [edi + 0x24], esi
        mov    byte ptr [edi + 0x38], 1
        call   sub_4_040A
        jmp    loc_6_00B5
loc_6_00A1:
        or     byte ptr [edi + 1], 8
        mov    eax, dword ptr [edi + 0x4b]
        mov    edx, edi
        mov    esi, 0x1b0                    ; -> sub_1_01B0
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
loc_6_00B5:
        popal  
        clc    
        ret    
VKD_Cancel_Paste:
        xor    eax, eax
        inc    eax
        call   sub_8_003D
        ret    
sub_6_00C1:
        mov    ebx, edx
        add    edx, dword ptr [0x1210]       ; -> obj1:0x1210
        mov    dword ptr [edx + 0x24], 0
        mov    eax, dword ptr [edx]
        and    ah, 0xf9
        or     ah, 8
        mov    dword ptr [edx], eax
        jmp    sub_1_01B0
        db  000h
loc_6_00E0:
        pushal 
        xor    ecx, ecx
        mov    eax, 0x1388
        VMMCall  VMM.Close_VM                    ; cd20ec000100
        popal  
        jmp    loc_1_0B59
sub_6_00F4:
        push   eax
        push   ebx
        push   ecx
        test   edx, edx
        je     loc_6_0100
        cmp    edx, 0xc
        jne    loc_6_0131
loc_6_0100:
        xchg   dword ptr [0x14b4], ebx       ; -> obj1:0x14B4
        mov    eax, dword ptr [0x14fe]       ; -> obj1:0x14FE
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x80
        je     loc_6_0131
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    ebx, dword ptr [0x14b4]       ; -> obj1:0x14B4
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        mov    dword ptr [0x1504], ebx       ; -> obj1:0x1504
loc_6_0131:
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_6_0135:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0xc2
        je     loc_6_014F
        mov    ecx, dword ptr [0x14ac]       ; -> obj1:0x14AC
        mov    edx, dword ptr [0x14b0]       ; -> obj1:0x14B0
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_6_014F:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        jmp    loc_6_0163
        db  090h
loc_6_0158:
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ah, 0xc2
        je     loc_6_0163
loc_6_0161:
        stc    
        ret    
loc_6_0163:
        cmp    byte ptr [0x1513], 0xff       ; -> loc_1_1513
        je     loc_6_0161
        mov    esi, dword ptr [0x150c]       ; -> obj1:0x150C
        add    esi, ebx
        movzx  eax, al
        cmp    eax, 7
        ja     loc_6_018C
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        jne    loc_6_01B8
        jmp    dword ptr [eax*4 + 0x1518]    ; -> obj1:0x1518
loc_6_018C:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1d], 1
        clc    
        ret    
loc_6_0197:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1d], 2
        clc    
        ret    
loc_6_01A2:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1d], 5
        clc    
        ret    
loc_6_01AD:
        and    word ptr [ebp + 0x2c], 0xfffe
        mov    byte ptr [ebp + 0x1d], 0
        clc    
        ret    
loc_6_01B8:
        cmp    eax, 7
        je     loc_6_01AD
        mov    al, byte ptr [eax + 0x1538]   ; -> loc_1_1538
        mov    byte ptr [ebp + 0x1d], al
        or     word ptr [ebp + 0x2c], 1
        clc    
        ret    
loc_6_01CD:
        cmp    byte ptr [ebp + 0x11], 1
        ja     loc_6_0197
        jb     loc_6_01E0
        cmp    dword ptr [esi], 0
        je     loc_6_01A2
        mov    byte ptr [esi + 5], 0xff
        jmp    loc_6_01AD
loc_6_01E0:
        mov    byte ptr [esi + 5], 0
        jmp    loc_6_01AD
loc_6_01E6:
        mov    byte ptr [ebp + 0x11], 0
        mov    byte ptr [esi + 6], 0
loc_6_01EE:
        mov    dword ptr [esi], 0
        mov    byte ptr [esi + 5], 0
        jmp    loc_6_01AD
loc_6_01FA:
        cmp    byte ptr [ebp + 0x11], 0
        jne    loc_6_01AD
        mov    byte ptr [esi + 6], 0xff
        jmp    loc_6_01AD
loc_6_0206:
        mov    byte ptr [ebp + 0x11], 0
        cmp    byte ptr [esi + 6], 0xff
        jne    loc_6_01AD
        mov    ax, word ptr [0x1508]         ; -> obj1:0x1508
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_6_01AD
loc_6_021C:
        mov    cl, byte ptr [ebp + 0x11]
        cmp    cl, 2
        ja     loc_6_0197
        je     loc_6_01AD
        test   cl, cl
        jne    loc_6_01AD
        cmp    byte ptr [esi + 5], 0xff
        jne    loc_6_023B
        or     cl, 0x20
loc_6_023B:
        mov    dl, byte ptr [0x1510]         ; -> obj1:0x1510
        add    dl, 2
        and    dl, 5
        or     cl, dl
        mov    byte ptr [ebp + 0x10], cl
        mov    byte ptr [ebp + 0x18], 3
        mov    byte ptr [ebp + 0x14], 0x28
        jmp    loc_6_01AD
loc_6_0259:
        mov    cx, word ptr [ebp + 0x38]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x10]
        mov    dword ptr [esi], ecx
        mov    byte ptr [esi + 4], 0
        test   dword ptr [ebx], 0x20
        je     loc_6_01AD
        mov    byte ptr [esi + 4], 0xff
        jmp    loc_6_01AD
        db  090h
sub_6_0280:
        cmp    ebx, dword ptr [0x1504]       ; -> obj1:0x1504
        jne    loc_6_0292
        cmp    word ptr [0x14fc], 0          ; -> obj1:0x14FC
        ja     loc_6_0294
loc_6_0292:
        clc    
        ret    
loc_6_0294:
        push   eax
        mov    eax, dword ptr [0x14fe]       ; -> obj1:0x14FE
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   cl, 2
        je     loc_6_02BF
        mov    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   cl, 2
        jne    loc_6_02BF
        movzx  ecx, word ptr [0x14fa]        ; -> obj1:0x14FA
        pop    eax
        stc    
        ret    
loc_6_02BF:
        pop    eax
        clc    
        ret    
        db  08Bh, 0C0h
sub_6_02C4:
        cmp    word ptr [0x14fc], 0          ; -> obj1:0x14FC
        je     loc_6_02FC
        mov    bl, byte ptr [0x1503]         ; -> loc_1_1503
        or     bl, byte ptr [0x1502]         ; -> obj1:0x1502
        jne    loc_6_02FC
        not    bl
        mov    byte ptr [0x1502], bl         ; -> obj1:0x1502
        mov    byte ptr [0x1503], bl         ; -> loc_1_1503
        mov    ebx, dword ptr [0x14b4]       ; -> obj1:0x14B4
        mov    dword ptr [0x1504], ebx       ; -> obj1:0x1504
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_6_02FC:
        ret    
        db  02Eh, 08Bh, 0C0h
loc_6_0300:
        in     al, 0x64
        test   al, 1
        je     loc_6_038C
        test   al, 0x20
        je     loc_6_038C
        mov    ah, al
        in     al, 0x60
        cmp    byte ptr [0x1514], 0xff       ; -> obj1:0x1514
        je     loc_6_038E
        inc    byte ptr [0x1516]             ; -> obj1:0x1516
        cmp    byte ptr [0x1516], 3          ; -> obj1:0x1516
        jb     loc_6_0344
        mov    byte ptr [0x1516], 0          ; -> obj1:0x1516
        cmp    word ptr [0x14fc], 0x1e       ; -> obj1:0x14FC
        jb     loc_6_034D
        mov    byte ptr [0x1515], 2          ; -> loc_1_1515
        jmp    loc_6_037A
loc_6_0344:
        cmp    byte ptr [0x1515], 0          ; -> loc_1_1515
        jne    loc_6_0374
loc_6_034D:
        push   ebx
        inc    word ptr [0x14fc]             ; -> obj1:0x14FC
        movzx  ebx, word ptr [0x14f8]        ; -> obj1:0x14F8
        mov    word ptr [ebx*2 + 0x14b8], ax ; -> obj1:0x14B8
        inc    bx
        and    bx, 0x1f
        mov    word ptr [0x14f8], bx         ; -> obj1:0x14F8
        pop    ebx
        jmp    loc_6_037A
loc_6_0374:
        dec    byte ptr [0x1515]             ; -> loc_1_1515
loc_6_037A:
        mov    eax, dword ptr [0x14fe]       ; -> obj1:0x14FE
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        call   sub_6_02C4
        clc    
        ret    
loc_6_038C:
        stc    
        ret    
loc_6_038E:
        mov    eax, dword ptr [0x14fe]       ; -> obj1:0x14FE
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  090h
loc_6_039C:
        mov    byte ptr [0x1503], 0          ; -> loc_1_1503
        jmp    sub_6_02C4
loc_6_03A8:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    edi, esi
        add    di, 0xa
        mov    al, byte ptr [edi]
        mov    byte ptr [0x1510], al         ; -> obj1:0x1510
        cmp    ebx, dword ptr [0x14b4]       ; -> obj1:0x14B4
        jne    loc_6_0445
        add    ebx, dword ptr [0x150c]       ; -> obj1:0x150C
        cmp    byte ptr [ebx + 5], 0xff
        jne    loc_6_0445
        cmp    byte ptr [ebx + 4], 0xff
        je     loc_6_03E9
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_6_03E9:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        add    si, 0xa
        mov    ecx, 4
        push   dword ptr [ebp + 0x30]
loc_6_040A:
        mov    ax, word ptr [esi]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        sub    si, 2
        loop   loc_6_040A
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    dword ptr [ebp + 0x30]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_6_0445:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
loc_6_044B:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x10d  R-X
sub_7_0000:
        mov    dword ptr [esi + 0x43], 5
        mov    dword ptr [esi + 0x47], 0x19
        mov    dword ptr [esi + 0x4b], 3
        mov    dword ptr [esi + 0x4f], 0xc8
        mov    dword ptr [esi + 0x53], 0x3e8
        mov    byte ptr [esi + 0x57], 2
        mov    byte ptr [esi + 0x58], 0xa
        ret    
loc_7_002C:
        call   sub_7_00F0
        add    ebx, dword ptr [0x1210]       ; -> obj1:0x1210
        cmp    byte ptr [ebx + 0x59], 0
        jne    loc_7_0044
        mov    esi, ebx
        call   0
loc_7_0044:
        or     dword ptr [ebx], 0x100000
        mov    al, byte ptr [0x1259]         ; -> obj1:0x1259
        mov    byte ptr [ebx + 0x3c], al
        push   ebx
        sub    ebx, dword ptr [0x1210]       ; -> obj1:0x1210
        call   sub_1_0922
        pop    ebx
        mov    word ptr [ebx + 0x36], ax
        mov    dl, al
        and    dl, 0x70
        mov    byte ptr [ebx + 0x41], dl
        mov    esi, ebx
        mov    edx, ebx
        mov    ebx, dword ptr [0x122d]       ; -> obj1:0x122D
        call   sub_1_0655
        je     loc_7_0081
        call   sub_1_01B0
loc_7_0081:
        clc    
        ret    
loc_7_0083:
        pushal 
        mov    eax, 0xc06
        mov    dl, 0xff
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        call   sub_4_040A
        popal  
        clc    
        ret    
loc_7_009B:
        mov    eax, dword ptr [0x1210]       ; -> obj1:0x1210
        add    eax, ebx
        movzx  ecx, word ptr [edx + 4]
        mov    dword ptr [eax + 0x43], ecx
        movzx  ecx, word ptr [edx + 6]
        mov    dword ptr [eax + 0x47], ecx
        movzx  ecx, word ptr [edx + 8]
        mov    dword ptr [eax + 0x4b], ecx
        movzx  ecx, word ptr [edx + 0xa]
        mov    dword ptr [eax + 0x4f], ecx
        movzx  ecx, word ptr [edx + 0xc]
        mov    dword ptr [eax + 0x53], ecx
        movzx  ecx, byte ptr [edx + 0xe]
        cmp    ecx, 1
        adc    ecx, 0
        mov    byte ptr [eax + 0x57], cl
        mov    cl, byte ptr [edx + 0x10]
        cmp    ecx, 1
        adc    ecx, 0
        mov    byte ptr [eax + 0x58], cl
        mov    byte ptr [eax + 0x59], 1
        test   byte ptr [edx], 1
        sete   al
        VxDCall  VKD.VKD_Define_Paste_Mode       ; cd2009000d00
        ret    
        db  000h
sub_7_00F0:
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        jne    loc_7_010B
        push   ebx
        add    ebx, dword ptr [0x150c]       ; -> obj1:0x150C
        mov    dword ptr [ebx], 0
        mov    byte ptr [ebx + 5], 0
        pop    ebx
loc_7_010B:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0xca  R-X
loc_8_0000:
        xor    eax, eax
        mov    al, 2
        call   sub_8_003D
        clc    
        ret    
loc_8_000B:
        call   sub_8_0080
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        jne    loc_8_002C
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    eax, eax
        mov    al, 0xf
        xor    edx, edx
        VMMCall  VMM.System_Control              ; cd2093000100
        pop    ebx
loc_8_002C:
        mov    eax, dword ptr [0x1210]       ; -> obj1:0x1210
        mov    esi, dword ptr [ebx + eax + 4]
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        clc    
        ret    
sub_8_003D:
        pushal 
        mov    edi, ebx
        add    edi, dword ptr [0x1210]       ; -> obj1:0x1210
        test   byte ptr [edi + 1], 0xc
        je     loc_8_007C
        xor    esi, esi
        mov    dword ptr [edi + 0x14], esi
        mov    dword ptr [edi + 0x18], esi
        xchg   dword ptr [edi + 0x24], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        and    byte ptr [edi + 1], 0xc0
        push   eax
        push   0
        push   dword ptr [edi + 0x10]
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    eax
        cmp    dword ptr [edi + 0x1c], 0
        je     loc_8_007C
        mov    edx, dword ptr [edi + 0x20]
        call   dword ptr [edi + 0x1c]
loc_8_007C:
        popal  
        ret    
        db  000h, 000h
sub_8_0080:
        cmp    byte ptr [0x1511], 0xff       ; -> loc_1_1511
        jne    loc_8_00C8
        cmp    byte ptr [0x1502], 0xff       ; -> obj1:0x1502
        jne    loc_8_00C8
        cmp    ebx, dword ptr [0x1504]       ; -> obj1:0x1504
        jne    loc_8_00C8
        push   eax
        mov    eax, dword ptr [0x14fe]       ; -> obj1:0x14FE
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    byte ptr [0x1502], 0          ; -> obj1:0x1502
        mov    byte ptr [0x1503], 0          ; -> loc_1_1503
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [0x14b4], ebx       ; -> obj1:0x14B4
        pop    ebx
        call   sub_6_02C4
        pop    eax
loc_8_00C8:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 9  vsize=0x10  R-X
loc_9_0000:
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        jne    loc_9_000D
        call   sub_1_08F6
loc_9_000D:
        clc    
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 10  vsize=0x128  R-X
VKD_PM_API:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    eax, 2
        jae    loc_10_0012
        shl    eax, 2
        jmp    loc_10_0018
loc_10_0012:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_10_0018:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 0
        add    byte ptr [eax], al
loc_10_0020:
        mov    dword ptr [ebp + 0x1c], 0x201
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_10_002C:
        pushal 
        and    byte ptr [ebp + 0x2c], 0xfe
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        cmp    byte ptr [0x1228], 0          ; -> obj1:0x1228
        jne    loc_10_0123
        or     ebx, ebx
        je     loc_10_0064
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_10_011E
        cmp    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
        je     loc_10_006A
        jmp    loc_10_011E
loc_10_0064:
        mov    ebx, dword ptr [0x1214]       ; -> obj1:0x1214
loc_10_006A:
        mov    esi, ebx
        add    esi, dword ptr [0x1210]       ; -> obj1:0x1210
        cmp    edx, -1
        je     loc_10_00B5
        and    edx, 0x707e
        mov    eax, dword ptr [0x122d]       ; -> obj1:0x122D
        push   eax
        push   dword ptr [0x1235]            ; -> sub_1_1235
        mov    dword ptr [0x1235], 0xf79     ; -> sub_1_1235
        movzx  eax, word ptr [esi + 0x36]
        push   ebx
        mov    ebx, edx
        mov    byte ptr [esi + 0x3a], 0
        call   sub_1_0655
        pop    ebx
        pop    dword ptr [0x1235]            ; -> sub_1_1235
        push   ecx
        push   edx
        mov    edx, esi
        call   sub_1_01B0
        pop    edx
        pop    ecx
loc_10_00B5:
        mov    al, ch
        movzx  ecx, cl
loc_10_00BA:
        cmp    edx, -1
        je     loc_10_00CE
        test   byte ptr [ebp + 0x14], 1
        je     loc_10_00CE
        push   eax
        mov    al, 0xe0
        call   VKD_Put_Byte
        pop    eax
loc_10_00CE:
        call   VKD_Put_Byte
        jae    loc_10_00DE
        push   edx
        mov    edx, esi
        call   sub_1_01B0
        pop    edx
loc_10_00DE:
        loop   loc_10_00BA
        cmp    edx, -1
        je     loc_10_0115
        push   edx
        mov    edx, esi
        call   sub_1_01B0
        pop    edx
        xchg   dword ptr [esp], ebx
        movzx  eax, word ptr [esi + 0x36]
        push   dword ptr [0x1235]            ; -> sub_1_1235
        mov    dword ptr [0x1235], 0xf79     ; -> sub_1_1235
        mov    byte ptr [esi + 0x3a], 0
        call   sub_1_0655
        pop    dword ptr [0x1235]            ; -> sub_1_1235
        pop    ebx
loc_10_0115:
        mov    edx, esi
        call   sub_1_01B0
        jmp    loc_10_0123
loc_10_011E:
        or     word ptr [ebp + 0x2c], 1
loc_10_0123:
        popal  
        ret    
        db  020h, 056h, 04Bh
VxD__LTEXT ENDS
