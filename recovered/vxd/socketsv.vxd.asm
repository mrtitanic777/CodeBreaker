; SOCKETSV -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 4340 instructions recovered by recursive descent

Declare_Virtual_Device SOCKETSV, 4, 0, SOCKETSV_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2b8  R-X
        dd  loc_2_14BC               ; fixup
        dd  loc_2_14DA               ; fixup
        dd  loc_2_1527               ; fixup
        dd  loc_2_1544               ; fixup
        dd  loc_2_1604               ; fixup
        dd  loc_2_1689               ; fixup
        dd  loc_2_1866               ; fixup
        dd  loc_2_1ADC               ; fixup
        dd  loc_2_1BE8               ; fixup
        dd  loc_2_1CE5               ; fixup
        dd  loc_2_1D53               ; fixup
        dd  loc_2_1DDE               ; fixup
        dd  loc_2_2007               ; fixup
        dd  loc_2_2118               ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SOCKETSV'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  SOCKETSV_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 055h, 08Bh, 0ECh, 083h, 0ECh, 004h, 08Ah, 045h
        db  00Ch, 002h, 045h, 010h, 08Bh, 055h, 008h, 09Ch, 0FAh, 0EEh, 042h, 0ECh, 09Dh, 088h, 045h, 0FFh
        db  08Ah, 045h, 0FFh, 08Bh, 0E5h, 05Dh, 0C3h
sub_1_00A7:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 0xc]
        add    al, byte ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        pushfd 
        cli    
        out    dx, al
        inc    edx
        mov    al, byte ptr [ebp + 0x14]
        out    dx, al
        popfd  
        mov    al, byte ptr [ebp + 0x14]
        pop    ebp
        ret    
sub_1_00C1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   0x88
        movzx  si, al
        add    esp, 0xc
        mov    al, byte ptr [ebp + 0x10]
        inc    al
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   0x88
        movzx  ax, al
        shl    ax, 8
        add    esp, 0xc
        or     si, ax
        mov    ax, si
        pop    esi
        pop    ebp
        ret    
sub_1_00FF:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    si, word ptr [ebp + 0x14]
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_00A7
        mov    ax, si
        add    esp, 0x10
        shr    ax, 8
        push   eax
        mov    al, byte ptr [ebp + 0x10]
        inc    al
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_00A7
        mov    ax, si
        add    esp, 0x10
        pop    esi
        pop    ebp
        ret    
sub_1_013B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 4], 0x5353534d
        je     loc_1_015C
        mov    byte ptr [edi + 0x1d], 1
        jmp    loc_1_0211
loc_1_015C:
        xor    ebx, ebx
        mov    word ptr [edi + 0x18], bx
        cmp    byte ptr [esi + 0x38], bl
        jbe    loc_1_020D
loc_1_016B:
        movzx  ecx, bl
        movzx  edx, bl
        push   4
        mov    al, byte ptr [ecx + esi + 0x28]
        push   eax
        push   dword ptr [esi + edx*4 + 0x18]
        call   0x88
        movzx  ecx, bl
        movzx  edx, bl
        add    esp, 0xc
        and    al, 0xf
        or     byte ptr [ecx + esi + 0x2c], al
        push   5
        mov    cl, byte ptr [edx + esi + 0x28]
        movzx  edx, bl
        push   ecx
        push   dword ptr [esi + edx*4 + 0x18]
        call   0x88
        movzx  ecx, bl
        add    esp, 0xc
        mov    byte ptr [ebp - 1], al
        mov    dl, byte ptr [ecx + esi + 0x2c]
        and    dl, al
        je     loc_1_0202
        mov    al, 1
        mov    cl, bl
        shl    al, cl
        movzx  ax, al
        or     word ptr [edi + 0x18], ax
        test   dl, 3
        je     loc_1_0202
        movzx  ecx, bl
        mov    al, byte ptr [ebp - 1]
        and    al, 0xfc
        push   eax
        push   5
        mov    al, byte ptr [ecx + esi + 0x28]
        movzx  ecx, bl
        push   eax
        push   dword ptr [esi + ecx*4 + 0x18]
        call   sub_1_00A7
        movzx  ecx, bl
        add    esp, 0x10
        mov    al, byte ptr [ecx + esi + 0x28]
        movzx  ecx, bl
        push   dword ptr [ebp - 1]
        push   5
        push   eax
        push   dword ptr [esi + ecx*4 + 0x18]
        call   sub_1_00A7
        add    esp, 0x10
loc_1_0202:
        inc    bl
        cmp    byte ptr [esi + 0x38], bl
        ja     loc_1_016B
loc_1_020D:
        mov    byte ptr [edi + 0x1d], 0
loc_1_0211:
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_021D
        and    byte ptr [edi + 0x20], 0xfe
        jmp    loc_1_0221
loc_1_021D:
        or     byte ptr [edi + 0x20], 1
loc_1_0221:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
SOCKETSV_Control:
        cmp    eax, 0x22
        jne    loc_1_0236
        push   edx
        push   ebx
        call   sub_2_23F3
loc_1_0234:
        stc    
        ret    
loc_1_0236:
        cmp    eax, 0x1b
        jne    loc_1_0244
        call   sub_2_23D6
loc_1_0240:
        cmp    eax, 1
        ret    
loc_1_0244:
        clc    
        ret    
loc_1_0246:
        pushfd 
        pushal 
        mov    ebp, esp
        cmp    ah, 0x9e
        jne    loc_1_025A
        push   ebp
        push   esi
        call   sub_1_013B
        pop    ecx
        pop    ecx
        jmp    loc_1_02B5
loc_1_025A:
        cmp    dword ptr [esi + 4], 0x5353534d
        je     loc_1_026D
        mov    byte ptr [ebp + 0x1d], 1
loc_1_0267:
        or     dword ptr [ebp + 0x20], 1
        jmp    loc_1_02B5
loc_1_026D:
        bts    dword ptr [esi + 8], 0x1f
        jae    loc_1_027A
        mov    byte ptr [ebp + 0x1d], 0x18
        jmp    loc_1_0267
loc_1_027A:
        sub    ah, 0x83
        jae    loc_1_0289
loc_1_027F:
        mov    byte ptr [ebp + 0x1d], 0x15
        or     dword ptr [ebp + 0x20], 1
        jmp    loc_1_02AE
loc_1_0289:
        cmp    ah, 0xe
        jbe    loc_1_029E
        cmp    ah, 0x1a
        jne    loc_1_027F
        push   ebp
        push   esi
        call   sub_2_2234
loc_1_029A:
        pop    ecx
        pop    ecx
        jmp    loc_1_02AE
loc_1_029E:
        movzx  eax, ah
        mov    eax, dword ptr [eax*4]        ; -> obj1:0x0000
        push   ebp
        push   esi
        call   eax
        pop    ecx
        pop    ecx
loc_1_02AE:
        and    dword ptr [esi + 8], 0x7fffffff
loc_1_02B5:
        popal  
        popfd  
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2a92  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x3a], 0x82
        jne    loc_2_0081
        mov    edi, dword ptr [esi + 0x18]
        push   0x1f
        mov    al, byte ptr [esi + 0x28]
        push   eax
        push   edi
        call   loc_2_0023
loc_2_0023:
        add    esp, 0xc
        mov    cl, byte ptr [esi + 0x28]
        mov    byte ptr [ebp - 1], al
        push   0x1f
        push   ecx
        push   edi
        call   loc_2_0035
loc_2_0035:
        add    esp, 0xc
        xor    al, byte ptr [ebp - 1]
        and    al, 0xc0
        cmp    al, 0xc0
        jne    loc_2_0081
        push   0x1f
        mov    al, byte ptr [esi + 0x28]
        push   eax
        push   edi
        call   loc_2_004D
loc_2_004D:
        add    esp, 0xc
        xor    al, byte ptr [ebp - 1]
        test   al, 0xc0
        jne    loc_2_0081
        push   ebx
        lea    eax, [ebp - 4]
        push   ebx
        mov    dword ptr [ebp - 8], ebx
        lea    ebx, [ebp - 8]
        push   ebx
        push   eax
        push   dword ptr [esi]
        call   sub_2_2A24
        mov    byte ptr [esi + 0x39], 2
        add    esp, 0x14
        cmp    dword ptr [ebp - 4], 4
        je     loc_2_007C
        mov    byte ptr [esi + 0x39], 1
loc_2_007C:
        mov    ebx, 1
loc_2_0081:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_008A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [esi + 0xc], 0      ; -> obj3:0x0024
        mov    dword ptr [esi + 0x10], 0     ; -> obj3:0x0060
        cmp    byte ptr [esi + 0x39], 2
        jne    loc_2_00B8
        mov    dword ptr [esi + 0x14], 0     ; -> obj3:0x0094
loc_2_00B8:
        or     byte ptr [esi + 0xb], 4
        mov    bl, byte ptr [esi + 0x38]
        cmp    bl, 1
        jbe    loc_2_00DD
        push   0x1f
        mov    al, byte ptr [esi + 0x28]
        push   eax
        push   dword ptr [esi + 0x18]
        call   loc_2_00D2
loc_2_00D2:
        add    esp, 0xc
        test   al, 0x20
        jne    loc_2_00DD
        mov    byte ptr [esi + 0x3b], 0
loc_2_00DD:
        mov    byte ptr [ebp - 1], 0
        test   bl, bl
        je     loc_2_019E
loc_2_00E9:
        movzx  ebx, byte ptr [ebp - 1]
        push   0x16
        mov    edi, dword ptr [esi + ebx*4 + 0x18]
        mov    al, byte ptr [ebx + esi + 0x28]
        push   eax
        push   edi
        call   loc_2_00FE
loc_2_00FE:
        add    esp, 0xc
        or     al, 0x10
        mov    cl, byte ptr [ebx + esi + 0x28]
        push   eax
        push   0x16
        push   ecx
        push   edi
        call   sub_1_00A7
loc_2_0111:
        add    esp, 0x10
        cmp    byte ptr [esi + 0x39], 2
        jne    loc_2_0149
        push   0x25
        mov    al, byte ptr [ebx + esi + 0x28]
        push   0x2e
        push   eax
        push   edi
        call   sub_1_00A7
loc_2_0129:
        add    esp, 0x10
        mov    cl, byte ptr [ebx + esi + 0x28]
        push   0x2f
        push   ecx
        push   edi
        call   loc_2_0139
loc_2_0139:
        add    esp, 0xc
        and    al, 3
        cmp    al, 3
        jne    loc_2_0149
        mov    dword ptr [ebp - 8], 0
loc_2_0149:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_018F
        cmp    byte ptr [esi + 0x39], 2
        mov    eax, 0                        ; -> obj3:0x01A8
        je     loc_2_015F
        mov    eax, 0                        ; -> loc_3_019C
loc_2_015F:
        push   eax
        mov    al, byte ptr [ebx + esi + 0x28]
        push   eax
        push   edi
        call   sub_2_01A8
        add    esp, 0xc
        cmp    byte ptr [esi + 0x39], 2
        je     loc_2_018F
        test   byte ptr [esi + 8], 2
        jne    loc_2_018F
        push   1
        mov    al, byte ptr [ebx + esi + 0x28]
        push   0x1e
        push   eax
        push   dword ptr [esi + ebx*4 + 0x18]
        call   sub_1_00A7
loc_2_018C:
        add    esp, 0x10
loc_2_018F:
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 0x38], al
        ja     loc_2_00E9
loc_2_019E:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_01A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        cmp    byte ptr [esi], 0
        je     loc_2_01EE
loc_2_01B5:
        mov    al, byte ptr [esi]
        mov    bl, al
        and    bl, 0x7f
        test   al, 0x80
        jne    loc_2_01D3
        push   ebx
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   loc_2_01CC
loc_2_01CC:
        add    esp, 0xc
        test   al, al
        jne    loc_2_01E6
loc_2_01D3:
        mov    al, byte ptr [esi + 1]
        push   eax
        push   ebx
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_00A7
loc_2_01E3:
        add    esp, 0x10
loc_2_01E6:
        add    esi, 2
        cmp    byte ptr [esi], 0
        jne    loc_2_01B5
loc_2_01EE:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_01F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 1
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0xc]
        push   2
        mov    edi, dword ptr [eax + esi*4 + 0x18]
        mov    al, byte ptr [esi + eax + 0x28]
        push   eax
        push   edi
        call   loc_2_021A
loc_2_021A:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], al
        push   0x16
        mov    al, byte ptr [esi + ecx + 0x28]
        mov    bl, 0
        push   eax
        push   edi
        call   loc_2_0232
loc_2_0232:
        add    esp, 0xc
        mov    cl, byte ptr [ebp + 0x10]
        cmp    cl, 1
        lea    edx, [ecx - 3]
        adc    bl, 0xff
        and    bl, 0x10
        cmp    dl, 1
        sbb    dl, dl
        and    dl, 2
        test   cl, cl
        je     loc_2_025A
        cmp    byte ptr [ebp + 0x14], cl
        jne    loc_2_025A
        or     bl, 1
        jmp    loc_2_0272
loc_2_025A:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_026F
        cmp    byte ptr [ebp + 0x14], 2
        je     loc_2_026F
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0272
loc_2_026F:
        or     bl, byte ptr [ebp + 0x14]
loc_2_0272:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_02F3
        mov    cl, byte ptr [ebp - 1]
        and    cl, 0xa0
        or     bl, cl
        mov    cl, al
        and    cl, 0xfd
        or     dl, cl
        test   bl, 0x10
        jne    loc_2_028F
        and    bl, 0x5f
loc_2_028F:
        cmp    bl, byte ptr [ebp - 1]
        jne    loc_2_0298
        cmp    dl, al
        je     loc_2_02F3
loc_2_0298:
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   0x16
        mov    al, byte ptr [esi + eax + 0x28]
        push   eax
        push   edi
        call   sub_1_00A7
loc_2_02A9:
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        push   ebx
        mov    al, byte ptr [esi + ecx + 0x28]
        push   2
        push   eax
        push   edi
        call   sub_1_00A7
loc_2_02BD:
        add    esp, 0x10
        push   0
        push   0x493e0
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 0x10
        je     loc_2_02F3
        mov    al, bl
        and    al, 0xa0
        cmp    al, 0xa0
        je     loc_2_02F3
        or     bl, 0xa0
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        push   2
        mov    al, byte ptr [esi + eax + 0x28]
        push   eax
        push   edi
        call   sub_1_00A7
loc_2_02F0:
        add    esp, 0x10
loc_2_02F3:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_02FD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        movzx  edi, byte ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        push   2
        mov    al, byte ptr [edi + esi + 0x28]
        push   eax
        push   dword ptr [esi + edi*4 + 0x18]
        call   loc_2_031A
loc_2_031A:
        add    esp, 0xc
        mov    bl, al
        mov    cl, byte ptr [edi + esi + 0x28]
        push   0x16
        push   ecx
        push   dword ptr [esi + edi*4 + 0x18]
        call   loc_2_032F
loc_2_032F:
        add    esp, 0xc
        test   bl, 0x10
        jne    loc_2_033F
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax], 0
        jmp    loc_2_0350
loc_2_033F:
        and    al, 2
        cmp    al, 1
        mov    eax, dword ptr [ebp + 0x10]
        sbb    cl, cl
        and    cl, 0xfe
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_0350:
        and    bl, 3
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    bl, 1
        mov    byte ptr [ecx], bl
        jne    loc_2_0363
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    loc_2_036B
loc_2_0363:
        cmp    bl, 3
        jne    loc_2_036B
        mov    byte ptr [ecx], 0
loc_2_036B:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 0x18]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax], cl
        pop    ebp
        ret    
loc_2_0377:
        movzx  eax, byte ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        push   0x16
        mov    dl, byte ptr [eax + ecx + 0x28]
        push   edx
        push   dword ptr [ecx + eax*4 + 0x18]
        call   loc_2_0390
loc_2_0390:
        add    esp, 0xc
        and    al, 1
        cmp    al, 1
        mov    al, 2
        adc    al, 0xff
        ret    
loc_2_039C:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    bl, byte ptr [esp + 0xc]
        push   esi
        movzx  ecx, bl
        push   0xa
        push   0x2e
        mov    esi, dword ptr [eax + ecx*4 + 0x18]
        push   0x40
        push   esi
        call   sub_1_00A7
loc_2_03B9:
        add    esp, 0x10
        push   0x2f
        push   0x40
        push   esi
        call   loc_2_03C6
loc_2_03C6:
        add    esp, 0xc
        lea    ecx, [ebx*2]
        shr    al, cl
        and    al, 3
        pop    esi
        sub    al, 3
        pop    ebx
        cmp    al, 1
        sbb    al, al
        add    al, 2
        ret    
loc_2_03DF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0xc]
        shr    dword ptr [esp + 0x14], 0x18
        mov    ebp, esp
        push   ebx
        push   esi
        mov    bl, byte ptr [ecx + eax + 0x28]
        mov    esi, dword ptr [eax + ecx*4 + 0x18]
        mov    al, byte ptr [ebp + 0x10]
        add    al, 5
        push   eax
        push   0x2e
        push   ebx
        push   esi
        call   sub_1_00A7
loc_2_0409:
        add    esp, 0x10
        push   dword ptr [ebp + 0x14]
        push   0x2f
        push   ebx
        push   esi
        call   sub_1_00A7
loc_2_0418:
        add    esp, 0x10
        xor    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_0421:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        movzx  ecx, byte ptr [esp + 0xc]
        push   esi
        mov    bl, byte ptr [ecx + eax + 0x28]
        mov    esi, dword ptr [eax + ecx*4 + 0x18]
        mov    al, byte ptr [esp + 0x14]
        add    al, 5
        push   eax
        push   0x2e
        push   ebx
        push   esi
        call   sub_1_00A7
loc_2_0444:
        add    esp, 0x10
        push   0x2f
        push   ebx
        push   esi
        call   loc_2_0450
loc_2_0450:
        movzx  eax, al
        shl    eax, 0x18
        add    esp, 0xc
        pop    esi
        pop    ebx
        ret    
sub_2_045C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebp, ebp
        mov    edi, dword ptr [esp + 0x18]
        test   byte ptr [edi + 0xb], 0x10
        je     loc_2_04DE
        mov    bl, byte ptr [edi + 0x28]
        mov    esi, dword ptr [edi + 0x18]
        add    esi, 0x8000
        push   ebp
        push   ebx
        push   esi
        call   loc_2_0483
loc_2_0483:
        mov    byte ptr [esp + 0x1f], al
        add    esp, 0xc
        push   0x55
        push   ebp
        push   ebx
        push   esi
        call   sub_1_00A7
loc_2_0494:
        add    esp, 0x10
        push   ebp
        push   ebx
        push   esi
        call   loc_2_049F
loc_2_049F:
        add    esp, 0xc
        cmp    al, 0x55
        jne    loc_2_04CE
        push   0xaa
        push   ebp
        push   ebx
        push   esi
        call   sub_1_00A7
loc_2_04B3:
        add    esp, 0x10
        push   ebp
        push   ebx
        push   esi
        call   loc_2_04BE
loc_2_04BE:
        add    esp, 0xc
        cmp    al, 0xaa
        jne    loc_2_04CE
        mov    byte ptr [edi + 0x39], 3
        mov    ebp, 1
loc_2_04CE:
        push   dword ptr [esp + 0x13]
        push   0
        push   ebx
        push   esi
        call   sub_1_00A7
loc_2_04DB:
        add    esp, 0x10
loc_2_04DE:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_2_04E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [esi + 0xc], 0      ; -> obj3:0x0038
        mov    dword ptr [esi + 0x10], 0     ; -> obj3:0x0060
        or     byte ptr [esi + 0xb], 4
        cmp    byte ptr [esi + 0x38], 0
        je     loc_2_0557
loc_2_0510:
        movzx  eax, byte ptr [ebp - 1]
        push   0
        push   0
        mov    bl, byte ptr [eax + esi + 0x28]
        mov    edi, dword ptr [esi + eax*4 + 0x18]
        add    edi, 0x8000
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_052D:
        add    esp, 0x10
        push   0
        push   1
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_053B:
        add    esp, 0x10
        push   0x10
        push   2
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0549:
        add    esp, 0x10
        mov    al, byte ptr [esi + 0x38]
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], al
        jb     loc_2_0510
loc_2_0557:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0563:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  edi, byte ptr [ebp + 0xc]
        push   2
        mov    al, byte ptr [esi + edi + 0x28]
        push   eax
        mov    bl, 0x10
        push   dword ptr [esi + edi*4 + 0x18]
        call   loc_2_058C
loc_2_058C:
        add    esp, 0xc
        mov    cl, byte ptr [ebp + 0x10]
        cmp    cl, 1
        je     loc_2_05A2
        sub    cl, 3
        cmp    cl, 1
        sbb    bl, bl
        and    bl, 0x18
loc_2_05A2:
        mov    cl, byte ptr [ebp + 0x14]
        cmp    cl, 1
        jne    loc_2_05AF
        or     bl, 1
        jmp    loc_2_05C4
loc_2_05AF:
        cmp    cl, 2
        jne    loc_2_05B9
        or     bl, 2
        jmp    loc_2_05C4
loc_2_05B9:
        test   cl, cl
        je     loc_2_05C4
        mov    dword ptr [ebp - 4], 0
loc_2_05C4:
        cmp    dword ptr [ebp - 4], 1
        jne    loc_2_0624
        mov    cl, al
        and    cl, 0xa0
        or     bl, cl
        test   bl, 0x10
        jne    loc_2_05D9
        and    bl, 0x5f
loc_2_05D9:
        cmp    bl, al
        je     loc_2_0624
        push   ebx
        mov    al, byte ptr [esi + edi + 0x28]
        push   2
        push   eax
        push   dword ptr [esi + edi*4 + 0x18]
        call   sub_1_00A7
loc_2_05EE:
        add    esp, 0x10
        push   0
        push   0x493e0
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 0x10
        je     loc_2_0624
        mov    al, bl
        and    al, 0xa0
        cmp    al, 0xa0
        je     loc_2_0624
        or     bl, 0xa0
        mov    al, byte ptr [esi + edi + 0x28]
        push   ebx
        push   2
        push   eax
        push   dword ptr [esi + edi*4 + 0x18]
        call   sub_1_00A7
loc_2_0621:
        add    esp, 0x10
loc_2_0624:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_062E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0xc]
        mov    ebp, esp
        push   2
        mov    dl, byte ptr [eax + ecx + 0x28]
        push   edx
        push   dword ptr [eax + ecx*4 + 0x18]
        call   loc_2_064A
loc_2_064A:
        add    esp, 0xc
        mov    dl, al
        and    dl, 0x18
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dl, 0x10
        mov    byte ptr [ecx], dl
        jne    loc_2_0661
        mov    byte ptr [ecx], 1
        jmp    loc_2_066C
loc_2_0661:
        cmp    dl, 0x18
        mov    byte ptr [ecx], 3
        je     loc_2_066C
        mov    byte ptr [ecx], 0
loc_2_066C:
        and    al, 3
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    al, 1
        mov    byte ptr [ecx], al
        jne    loc_2_067C
        mov    byte ptr [ecx], 1
        jmp    loc_2_0686
loc_2_067C:
        cmp    al, 2
        mov    byte ptr [ecx], 2
        je     loc_2_0686
        mov    byte ptr [ecx], 0
loc_2_0686:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 0x18]
        pop    ebp
        mov    byte ptr [eax], cl
        ret    
loc_2_068F:
        movzx  ecx, byte ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        push   3
        mov    dl, byte ptr [eax + ecx + 0x28]
        mov    eax, dword ptr [eax + ecx*4 + 0x18]
        push   edx
        add    eax, 0x8000
        push   eax
        call   loc_2_06AE
loc_2_06AE:
        add    esp, 0xc
        and    al, 4
        cmp    al, 1
        mov    al, 2
        adc    al, 0xff
        ret    
sub_2_06BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x3a], 0x83
        jne    loc_2_0793
        mov    eax, dword ptr [edi + 0x18]
        mov    cl, byte ptr [edi + 0x28]
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [ebp - 1], cl
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0xe
        add    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 0x37
        add    al, byte ptr [ebp - 1]
        out    dx, al
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   loc_2_06F9
loc_2_06F9:
        add    esp, 0xc
        mov    byte ptr [ebp - 2], al
        xor    al, 8
        push   eax
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   sub_1_00A7
loc_2_070E:
        add    esp, 0x10
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   loc_2_071D
loc_2_071D:
        add    esp, 0xc
        xor    al, byte ptr [ebp - 2]
        test   al, 8
        je     loc_2_0780
        push   0x3a
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   loc_2_0734
loc_2_0734:
        add    esp, 0xc
        mov    bl, al
        and    bl, 0x3f
        mov    al, bl
        or     al, 0xc0
        push   eax
        push   0x3a
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   sub_1_00A7
loc_2_074E:
        add    esp, 0x10
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   loc_2_075D
loc_2_075D:
        add    esp, 0xc
        and    al, 0xf
        cmp    al, 0xc
        jne    loc_2_076F
        mov    byte ptr [edi + 0x39], 4
        mov    esi, 1
loc_2_076F:
        push   ebx
        push   0x3a
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   sub_1_00A7
loc_2_077D:
        add    esp, 0x10
loc_2_0780:
        push   dword ptr [ebp - 2]
        push   0
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        call   sub_1_00A7
loc_2_0790:
        add    esp, 0x10
loc_2_0793:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_079C:
        push   esi
        push   0x2f
        mov    esi, dword ptr [esp + 0xc]
        mov    al, byte ptr [esi + 0x28]
        push   eax
        push   dword ptr [esi + 0x18]
        call   loc_2_07AF
loc_2_07AF:
        add    esp, 0xc
        test   al, 0x40
        je     loc_2_07C4
        mov    dword ptr [esi + 0xc], 0      ; -> obj3:0x0024
        mov    dword ptr [esi + 0x10], 0     ; -> obj3:0x0060
loc_2_07C4:
        mov    eax, 1
        pop    esi
        ret    
loc_2_07CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 1
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0xc]
        push   2
        mov    edi, dword ptr [eax + esi*4 + 0x18]
        mov    al, byte ptr [eax + esi + 0x28]
        push   eax
        push   edi
        call   loc_2_07F3
loc_2_07F3:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], al
        push   0x2f
        mov    al, byte ptr [ecx + esi + 0x28]
        mov    bl, 0
        push   eax
        push   edi
        call   loc_2_080B
loc_2_080B:
        add    esp, 0xc
        mov    cl, byte ptr [ebp + 0x10]
        cmp    cl, 1
        lea    edx, [ecx - 3]
        adc    bl, 0xff
        and    bl, 0x10
        cmp    dl, 1
        sbb    dl, dl
        neg    dl
        test   cl, cl
        je     loc_2_0832
        cmp    byte ptr [ebp + 0x14], cl
        jne    loc_2_0832
        or     bl, 1
        jmp    loc_2_084A
loc_2_0832:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0847
        cmp    byte ptr [ebp + 0x14], 2
        je     loc_2_0847
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_084A
loc_2_0847:
        or     bl, byte ptr [ebp + 0x14]
loc_2_084A:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_08CB
        mov    cl, byte ptr [ebp - 1]
        and    cl, 0xa0
        or     bl, cl
        mov    cl, al
        and    cl, 0xfc
        or     dl, cl
        test   bl, 0x10
        jne    loc_2_0867
        and    bl, 0x5f
loc_2_0867:
        cmp    bl, byte ptr [ebp - 1]
        jne    loc_2_0870
        cmp    dl, al
        je     loc_2_08CB
loc_2_0870:
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   0x2f
        mov    al, byte ptr [eax + esi + 0x28]
        push   eax
        push   edi
        call   sub_1_00A7
loc_2_0881:
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        push   ebx
        mov    al, byte ptr [ecx + esi + 0x28]
        push   2
        push   eax
        push   edi
        call   sub_1_00A7
loc_2_0895:
        add    esp, 0x10
        push   0
        push   0x493e0
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 0x10
        je     loc_2_08CB
        mov    al, bl
        and    al, 0xa0
        cmp    al, 0xa0
        je     loc_2_08CB
        or     bl, 0xa0
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        push   2
        mov    al, byte ptr [eax + esi + 0x28]
        push   eax
        push   edi
        call   sub_1_00A7
loc_2_08C8:
        add    esp, 0x10
loc_2_08CB:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_08D5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        push   2
        mov    al, byte ptr [esi + edi + 0x28]
        push   eax
        push   dword ptr [edi + esi*4 + 0x18]
        call   loc_2_08F2
loc_2_08F2:
        add    esp, 0xc
        mov    bl, al
        mov    cl, byte ptr [esi + edi + 0x28]
        push   0x2f
        push   ecx
        push   dword ptr [edi + esi*4 + 0x18]
        call   loc_2_0907
loc_2_0907:
        add    esp, 0xc
        and    al, 3
        test   bl, 0x10
        jne    loc_2_0919
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax], 0
        jmp    loc_2_0928
loc_2_0919:
        cmp    al, 1
        mov    eax, dword ptr [ebp + 0x10]
        sbb    cl, cl
        and    cl, 0xfe
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_0928:
        and    bl, 3
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    bl, 1
        mov    byte ptr [ecx], bl
        jne    loc_2_093B
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    loc_2_0943
loc_2_093B:
        cmp    bl, 3
        jne    loc_2_0943
        mov    byte ptr [ecx], 0
loc_2_0943:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 0x18]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax], cl
        pop    ebp
        ret    
loc_2_094F:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    bl, byte ptr [esp + 0xc]
        push   0x1f
        movzx  ecx, bl
        mov    dl, byte ptr [eax + ecx + 0x28]
        push   edx
        push   dword ptr [eax + ecx*4 + 0x18]
        call   loc_2_096B
loc_2_096B:
        add    esp, 0xc
        lea    ecx, [ebx*2]
        shr    al, cl
        and    al, 3
        pop    ebx
        sub    al, 3
        cmp    al, 1
        sbb    al, al
        add    al, 2
        ret    
sub_2_0983:
        xor    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    byte ptr [ecx + 0x3a], 0x8a
        jne    loc_2_0998
        mov    byte ptr [ecx + 0x39], 5
        mov    eax, 1
loc_2_0998:
        ret    
loc_2_0999:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0xc], 0      ; -> obj3:0x0024
        mov    dword ptr [eax + 0x10], 0     ; -> obj3:0x0060
        mov    eax, 1
        ret    
loc_2_09B1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   ebx
        push   esi
        movzx  esi, byte ptr [ebp + 0xc]
        push   edi
        push   2
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + edi + 0x28]
        mov    bl, 0
        push   eax
        push   dword ptr [edi + esi*4 + 0x18]
        call   loc_2_09DA
loc_2_09DA:
        add    esp, 0xc
        mov    cl, byte ptr [ebp + 0x10]
        cmp    cl, 1
        adc    bl, 0xff
        and    bl, 0x10
        cmp    cl, 1
        jne    loc_2_09F3
        or     bl, 4
        jmp    loc_2_0A00
loc_2_09F3:
        lea    edx, [ecx - 3]
        cmp    dl, 1
        sbb    dl, dl
        and    dl, 0xc
        or     bl, dl
loc_2_0A00:
        test   cl, cl
        je     loc_2_0A0E
        cmp    byte ptr [ebp + 0x14], cl
        jne    loc_2_0A0E
        or     bl, 3
        jmp    loc_2_0A31
loc_2_0A0E:
        cmp    byte ptr [ebp + 0x14], 1
        jne    loc_2_0A19
        or     bl, 1
        jmp    loc_2_0A31
loc_2_0A19:
        cmp    byte ptr [ebp + 0x14], 2
        jne    loc_2_0A24
        or     bl, 2
        jmp    loc_2_0A31
loc_2_0A24:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0A31
        mov    dword ptr [ebp - 4], 0
loc_2_0A31:
        cmp    dword ptr [ebp - 4], 1
        jne    loc_2_0A94
        mov    cl, al
        and    cl, 0xa0
        or     bl, cl
        test   bl, 0x10
        jne    loc_2_0A46
        and    bl, 0x5f
loc_2_0A46:
        or     bl, 0x40
        cmp    bl, al
        je     loc_2_0A94
        push   ebx
        mov    al, byte ptr [esi + edi + 0x28]
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 0x18]
        call   sub_1_00A7
loc_2_0A5E:
        add    esp, 0x10
        push   0
        push   0x493e0
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 0x10
        je     loc_2_0A94
        mov    al, bl
        and    al, 0xa0
        cmp    al, 0xa0
        je     loc_2_0A94
        or     bl, 0xa0
        mov    al, byte ptr [esi + edi + 0x28]
        push   ebx
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 0x18]
        call   sub_1_00A7
loc_2_0A91:
        add    esp, 0x10
loc_2_0A94:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0A9E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0xc]
        mov    ebp, esp
        push   2
        mov    dl, byte ptr [ecx + eax + 0x28]
        push   edx
        push   dword ptr [eax + ecx*4 + 0x18]
        call   loc_2_0ABA
loc_2_0ABA:
        add    esp, 0xc
        test   al, 0x10
        je     loc_2_0ADF
        mov    dl, al
        mov    ecx, dword ptr [ebp + 0x10]
        and    dl, 0xc
        mov    byte ptr [ecx], dl
        cmp    dl, 4
        jne    loc_2_0AD5
        mov    byte ptr [ecx], 1
        jmp    loc_2_0AE5
loc_2_0AD5:
        cmp    dl, 0xc
        jne    loc_2_0AE2
        mov    byte ptr [ecx], 3
        jmp    loc_2_0AE5
loc_2_0ADF:
        mov    ecx, dword ptr [ebp + 0x10]
loc_2_0AE2:
        mov    byte ptr [ecx], 0
loc_2_0AE5:
        and    al, 3
        mov    edx, dword ptr [ebp + 0x14]
        cmp    al, 3
        mov    byte ptr [edx], al
        jne    loc_2_0AF6
        mov    al, byte ptr [ecx]
        mov    byte ptr [edx], al
        jmp    loc_2_0B09
loc_2_0AF6:
        cmp    al, 1
        jne    loc_2_0AFF
        mov    byte ptr [edx], 1
        jmp    loc_2_0B09
loc_2_0AFF:
        cmp    al, 2
        mov    byte ptr [edx], 2
        je     loc_2_0B09
        mov    byte ptr [edx], 0
loc_2_0B09:
        mov    cl, byte ptr [edx]
        mov    eax, dword ptr [ebp + 0x18]
        pop    ebp
        mov    byte ptr [eax], cl
        ret    
loc_2_0B12:
        movzx  ecx, byte ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        push   0x17
        mov    dl, byte ptr [eax + ecx + 0x28]
        push   edx
        push   dword ptr [eax + ecx*4 + 0x18]
        call   loc_2_0B2B
loc_2_0B2B:
        add    esp, 0xc
        and    al, 3
        sub    al, 3
        cmp    al, 1
        sbb    al, al
        add    al, 2
        ret    
sub_2_0B39:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, 0x40000000
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 8]
        test   ecx, eax
        jne    loc_2_0C08
        or     eax, ecx
        lea    edi, [esi + 0x34]
        xor    ebx, ebx
        push   1
        mov    word ptr [edi], bx
        lea    ecx, [ebp - 0x18]
        mov    dword ptr [ebp - 4], 0x10
        mov    dword ptr [esi + 8], eax
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   3
        push   dword ptr [0]                 ; -> obj3:0x0004
        push   ebx
        push   dword ptr [esi]
        call   sub_2_2A34
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_0BB2
        cmp    dword ptr [ebp - 4], 0x10
        jne    loc_2_0BB2
        xor    cl, cl
loc_2_0B92:
        movzx  edx, cl
        mov    al, byte ptr [ebp + edx - 0x18]
        mov    byte ptr [edx + esi + 0x3d], al
        test   al, al
        je     loc_2_0BAB
        mov    ax, 1
        shl    ax, cl
        or     word ptr [edi], ax
loc_2_0BAB:
        inc    cl
        cmp    cl, 0x10
        jb     loc_2_0B92
loc_2_0BB2:
        xor    ebx, ebx
        cmp    word ptr [edi], bx
        jne    loc_2_0C08
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 4], 4
        push   ecx
        push   3
        push   dword ptr [0]                 ; -> obj3:0x0008
        push   ebx
        push   dword ptr [esi]
        call   sub_2_2A34
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_0C08
        cmp    dword ptr [ebp - 8], ebx
        je     loc_2_0C08
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [edi], ax
loc_2_0BEC:
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        je     loc_2_0C01
        movzx  eax, bl
        mov    byte ptr [eax + esi + 0x3d], bl
loc_2_0C01:
        inc    bl
        cmp    bl, 0x10
        jb     loc_2_0BEC
loc_2_0C08:
        mov    ax, word ptr [esi + 0x34]
        lea    edi, [esi + 0x30]
        test   ax, ax
        jne    loc_2_0C1D
        push   esi
        call   sub_2_0DC6
        add    esp, 4
loc_2_0C1D:
        or     word ptr [edi], ax
        xor    ecx, ecx
        cmp    word ptr [edi], cx
        jne    loc_2_0C4D
        cmp    dword ptr [esi + 0x18], ecx
        je     loc_2_0C4D
        mov    word ptr [edi], 0x698
loc_2_0C31:
        movzx  eax, word ptr [edi]
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        je     loc_2_0C46
        movzx  eax, cl
        mov    byte ptr [eax + esi + 0x3d], cl
loc_2_0C46:
        inc    cl
        cmp    cl, 0x10
        jb     loc_2_0C31
loc_2_0C4D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0C54:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x3a], 0x83
        jne    loc_2_0DBF
        test   byte ptr [esi + 0x3b], 0x1f
        je     loc_2_0DBF
        push   0
        push   0                             ; -> sub_2_101F
        push   0                             ; -> sub_2_0C54
        call   sub_2_2A44
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0DBF
        pushfd 
        cli    
        in     al, 0xa1
        mov    ah, al
        in     al, 0x21
        push   eax
        mov    al, 0xff
        out    0xa1, al
        out    0x21, al
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi + 0x38]
        test   bl, bl
        je     loc_2_0D02
loc_2_0CAA:
        movzx  ecx, byte ptr [ebp - 1]
        movzx  edx, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + ecx + 0x28]
        push   3
        mov    edi, dword ptr [esi + edx*4 + 0x18]
        mov    byte ptr [ebp - 8], al
        push   dword ptr [ebp - 8]
        push   edi
        call   loc_2_0CC8
loc_2_0CC8:
        movzx  ecx, byte ptr [ebp - 1]
        add    esp, 0xc
        mov    byte ptr [ebp + ecx - 0x10], al
        push   0x40
        push   3
        push   dword ptr [ebp - 8]
        push   edi
        call   sub_1_00A7
loc_2_0CE0:
        add    esp, 0x10
        mov    al, byte ptr [esi + 0x3b]
        shl    al, 4
        or     al, 8
        push   eax
        push   5
        push   dword ptr [ebp - 8]
        push   edi
        call   sub_1_00A7
loc_2_0CF7:
        add    esp, 0x10
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     loc_2_0CAA
loc_2_0D02:
        mov    byte ptr [ebp - 1], 0
        test   bl, bl
        je     loc_2_0D5A
loc_2_0D0A:
        movzx  edi, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + 0x3b]
        mov    cl, byte ptr [esi + edi + 0x28]
        and    al, 0x1f
        push   eax
        push   ecx
        push   dword ptr [esi + edi*4 + 0x18]
        mov    al, byte ptr [esi + 0x3a]
        push   eax
        call   sub_2_0EDD
        add    esp, 0x10
        test   ax, ax
        je     loc_2_0D4E
        mov    al, byte ptr [esi + 0x3b]
        mov    cl, byte ptr [esi + edi + 0x28]
        and    al, 0x1f
        push   eax
        push   ecx
        push   dword ptr [esi + edi*4 + 0x18]
        mov    al, byte ptr [esi + 0x3a]
        push   eax
        call   sub_2_0EDD
        add    esp, 0x10
        test   ax, ax
        jne    loc_2_0D52
loc_2_0D4E:
        mov    byte ptr [esi + 0x3b], 0
loc_2_0D52:
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     loc_2_0D0A
loc_2_0D5A:
        mov    byte ptr [ebp - 1], 0
        test   bl, bl
        je     loc_2_0DA3
loc_2_0D62:
        movzx  eax, byte ptr [ebp - 1]
        movzx  ecx, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + eax + 0x28]
        push   0
        mov    edi, dword ptr [esi + ecx*4 + 0x18]
        push   5
        mov    byte ptr [ebp - 0xc], al
        push   dword ptr [ebp - 0xc]
        push   edi
        call   sub_1_00A7
loc_2_0D82:
        add    esp, 0x10
        movzx  eax, byte ptr [ebp - 1]
        push   dword ptr [ebp + eax - 0x10]
        push   3
        push   dword ptr [ebp - 0xc]
        push   edi
        call   sub_1_00A7
loc_2_0D98:
        add    esp, 0x10
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     loc_2_0D62
loc_2_0DA3:
        pop    eax
        out    0x21, al
        mov    al, ah
        out    0xa1, al
        popfd  
        push   0
        push   0                             ; -> sub_2_101F
        push   0                             ; -> sub_2_0C54
        call   sub_2_2A3C
        add    esp, 0xc
loc_2_0DBF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0DC6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 4], bx
        push   ebx
        push   0                             ; -> sub_2_101F
        push   0                             ; -> sub_2_0DC6
        call   sub_2_2A44
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0ED2
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x18]
        test   edi, edi
        je     loc_2_0E08
        mov    al, byte ptr [esi + 0x28]
        mov    cl, byte ptr [esi + 0x3a]
        mov    byte ptr [ebp - 2], al
        mov    byte ptr [ebp - 1], cl
        jmp    loc_2_0E25
loc_2_0E08:
        mov    edi, 0x3e0
        push   0
        mov    byte ptr [ebp - 2], 0
        push   edi
        call   sub_2_12FD
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        mov    word ptr [esi + 0x36], 0xceb8
loc_2_0E25:
        push   dword ptr [ebp - 1]
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        je     loc_2_0EBE
        pushfd 
        cli    
        in     al, 0xa1
        mov    ah, al
        in     al, 0x21
        push   eax
        mov    al, 0xff
        out    0xa1, al
        out    0x21, al
        xor    bl, bl
loc_2_0E49:
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        test   word ptr [esi + 0x36], ax
        mov    word ptr [ebp - 6], ax
        je     loc_2_0EA2
        test   word ptr [esi + 0x32], ax
        jne    loc_2_0EA2
        push   ebx
        push   dword ptr [ebp - 2]
        push   edi
        push   dword ptr [ebp - 1]
        call   sub_2_0EDD
        mov    word ptr [ebp - 0xa], ax
        add    esp, 0x10
        test   ax, ax
        je     loc_2_0EA2
        bsf    ax, word ptr [ebp - 0xa]
        mov    word ptr [ebp - 8], ax
        movzx  eax, word ptr [ebp - 8]
        mov    cx, word ptr [ebp - 6]
        mov    byte ptr [eax + esi + 0x3d], bl
        mov    ax, 1
        or     word ptr [esi + 0x32], cx
        mov    cl, byte ptr [ebp - 8]
        shl    ax, cl
        or     word ptr [ebp - 4], ax
loc_2_0EA2:
        inc    bl
        cmp    bl, 0x10
        jb     loc_2_0E49
        movzx  eax, word ptr [ebp - 4]
        push   eax
        call   sub_2_2A4C
        add    esp, 4
        pop    eax
        out    0x21, al
        mov    al, ah
        out    0xa1, al
        popfd  
loc_2_0EBE:
        push   0
        push   0                             ; -> sub_2_101F
        push   0                             ; -> sub_2_0DC6
        call   sub_2_2A3C
        add    esp, 0xc
loc_2_0ED2:
        mov    ax, word ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0EDD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        xor    si, si
        push   edi
        push   5
        mov    bl, byte ptr [ebp + 0x10]
        push   ebx
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        call   loc_2_0EF8
loc_2_0EF8:
        add    esp, 0xc
        mov    byte ptr [ebp - 2], al
        push   3
        push   ebx
        push   edi
        call   loc_2_0F07
loc_2_0F07:
        add    esp, 0xc
        mov    byte ptr [ebp - 1], al
        push   0x40
        push   3
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0F18:
        add    esp, 0x10
        mov    al, byte ptr [ebp + 0x14]
        shl    al, 4
        or     al, 8
        mov    byte ptr [ebp - 0xc], al
        push   dword ptr [ebp - 0xc]
        push   5
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0F32:
        add    esp, 0x10
        push   4
        push   ebx
        push   edi
        call   loc_2_0F3E
loc_2_0F3E:
        add    esp, 0xc
        call   sub_2_1000
        cmp    byte ptr [ebp + 8], 0x82
        mov    word ptr [ebp - 6], ax
        je     loc_2_0F8A
        push   0x20
        push   0x16
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0F5B:
        add    esp, 0x10
        call   sub_2_1000
        mov    word ptr [ebp - 4], ax
        push   4
        push   ebx
        push   edi
        call   loc_2_0F70
loc_2_0F70:
        add    esp, 0xc
        call   sub_2_1000
        mov    si, ax
        mov    ax, word ptr [ebp - 4]
        xor    si, word ptr [ebp - 4]
        xor    ax, word ptr [ebp - 6]
        and    si, ax
loc_2_0F8A:
        test   si, si
        jne    loc_2_0FD8
        push   0
        push   5
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0F9A:
        add    esp, 0x10
        call   sub_2_1000
        push   dword ptr [ebp - 0xc]
        push   5
        mov    word ptr [ebp - 4], ax
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0FB2:
        add    esp, 0x10
        push   4
        push   ebx
        push   edi
        call   loc_2_0FBE
loc_2_0FBE:
        add    esp, 0xc
        call   sub_2_1000
        mov    si, ax
        mov    ax, word ptr [ebp - 4]
        xor    si, word ptr [ebp - 4]
        xor    ax, word ptr [ebp - 6]
        and    si, ax
loc_2_0FD8:
        push   dword ptr [ebp - 2]
        push   5
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0FE4:
        add    esp, 0x10
        push   dword ptr [ebp - 1]
        push   3
        push   ebx
        push   edi
        call   sub_1_00A7
loc_2_0FF3:
        mov    ax, si
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1000:
        sub    esp, 4
        mov    al, 0xa
        out    0xa0, al
        in     al, 0xa0
        mov    ah, al
        mov    al, 0xa
        out    0x20, al
        in     al, 0x20
        mov    word ptr [esp + 2], ax
        mov    ax, word ptr [esp + 2]
        add    esp, 4
        ret    
sub_2_101F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 1
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        push   ebx
        push   esi
        movzx  esi, byte ptr [ebp + 0xc]
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [edi + 0x39]
        shl    eax, 5
        push   edi
        call   sub_3_00E8
        add    esp, 8
        mov    dl, byte ptr [ebp + 0x10]
        mov    cl, dl
        mov    bl, dl
        and    cl, 7
        and    bl, 0x10
        mov    byte ptr [ebp - 1], cl
        je     loc_2_1088
        cmp    al, 1
        je     loc_2_1079
        test   byte ptr [edi + 8], 1
        je     loc_2_1079
        sub    al, 2
        cmp    al, 1
        sbb    al, al
        and    al, 3
        mov    byte ptr [ebp - 1], al
        jmp    loc_2_107D
loc_2_1079:
        mov    byte ptr [ebp - 1], 1
loc_2_107D:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], al
        jmp    loc_2_10C7
loc_2_1088:
        test   dl, 0x20
        jne    loc_2_10B4
        cmp    byte ptr [ebp - 1], 1
        jne    loc_2_1097
        cmp    al, 1
        je     loc_2_10B4
loc_2_1097:
        cmp    byte ptr [ebp - 1], 3
        jne    loc_2_10A1
        cmp    al, 2
        je     loc_2_10B4
loc_2_10A1:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_2_10AB
        test   al, al
        je     loc_2_10B4
loc_2_10AB:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_10C7
loc_2_10B4:
        mov    al, byte ptr [ebp + 0x14]
        and    al, 0x70
        shr    al, 4
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp + 0x14]
        and    al, 7
        mov    byte ptr [ebp - 3], al
loc_2_10C7:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_1145
        movzx  eax, byte ptr [ebp - 3]
        movzx  ecx, byte ptr [ebp - 2]
        push   eax
        push   ecx
        movzx  edx, byte ptr [ebp - 1]
        movzx  eax, byte ptr [edi + 0x39]
        shl    eax, 5
        push   edx
        push   esi
        push   edi
        call   sub_3_00E0
        add    esp, 0x14
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 1
        jne    loc_2_1131
        test   bl, bl
        je     loc_2_1131
        cmp    byte ptr [ebp - 1], 3
        jne    loc_2_1131
        movzx  eax, byte ptr [edi + 0x39]
        shl    eax, 5
        push   esi
        push   edi
        call   sub_3_00E8
        add    esp, 8
        cmp    al, 1
        jne    loc_2_1131
        movzx  eax, byte ptr [edi + 0x39]
        shl    eax, 5
        push   1
        push   1
        push   1
        push   esi
        push   edi
        call   sub_3_00E0
        add    esp, 0x14
        mov    dword ptr [ebp - 8], eax
loc_2_1131:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_1145
        test   bl, bl
        je     loc_2_1141
        or     byte ptr [edi + 0xb], 0x20
        jmp    loc_2_1145
loc_2_1141:
        and    byte ptr [edi + 0xb], 0xdf
loc_2_1145:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_114F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        lea    ecx, [ebp - 2]
        push   edi
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        movzx  esi, byte ptr [ebp + 0xc]
        movzx  eax, byte ptr [edi + 0x39]
        shl    eax, 5
        push   ecx
        push   ebx
        push   esi
        push   edi
        call   sub_3_00E4
        movzx  eax, byte ptr [edi + 0x39]
        shl    eax, 5
        add    esp, 0x14
        push   esi
        push   edi
        call   sub_3_00E8
        add    esp, 8
        mov    cl, byte ptr [ebp - 2]
        shl    cl, 4
        mov    edx, dword ptr [ebp + 0x14]
        or     cl, byte ptr [ebp - 1]
        test   byte ptr [edi + 0xb], 0x20
        mov    byte ptr [edx], cl
        je     loc_2_11A8
        or     byte ptr [ebx], 0x10
        jmp    loc_2_11C6
loc_2_11A8:
        cmp    al, 1
        jne    loc_2_11B1
        cmp    byte ptr [ebx], 1
        jne    loc_2_11C3
loc_2_11B1:
        cmp    al, 2
        jne    loc_2_11BA
        cmp    byte ptr [ebx], 3
        jne    loc_2_11C3
loc_2_11BA:
        test   al, al
        jne    loc_2_11C6
        cmp    byte ptr [ebx], 0
        je     loc_2_11C6
loc_2_11C3:
        or     byte ptr [ebx], 0x20
loc_2_11C6:
        cmp    al, 1
        je     loc_2_11D1
        cmp    al, 2
        jne    loc_2_11D1
        or     byte ptr [ebx], 0x40
loc_2_11D1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_11D8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        push   2
        mov    al, byte ptr [esi + edi + 0x28]
        mov    bl, 0
        push   eax
        push   dword ptr [edi + esi*4 + 0x18]
        call   loc_2_11F7
loc_2_11F7:
        add    esp, 0xc
        mov    cl, byte ptr [ebp + 0x10]
        cmp    cl, 1
        adc    bl, 0xff
        and    bl, 0x10
        test   cl, cl
        je     loc_2_1214
        cmp    byte ptr [ebp + 0x14], cl
        jne    loc_2_1214
        or     bl, 1
        jmp    loc_2_1217
loc_2_1214:
        or     bl, byte ptr [ebp + 0x14]
loc_2_1217:
        test   cl, cl
        je     loc_2_1225
        cmp    byte ptr [ebp + 0x18], cl
        jne    loc_2_1225
        or     bl, 4
        jmp    loc_2_122D
loc_2_1225:
        mov    cl, byte ptr [ebp + 0x18]
        shl    cl, 2
        or     bl, cl
loc_2_122D:
        mov    cl, al
        and    cl, 0xa0
        or     bl, cl
        test   bl, 0x10
        jne    loc_2_123C
        and    bl, 0x5f
loc_2_123C:
        or     bl, 0x40
        cmp    bl, al
        je     loc_2_128A
        push   ebx
        mov    al, byte ptr [esi + edi + 0x28]
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 0x18]
        call   sub_1_00A7
loc_2_1254:
        add    esp, 0x10
        push   0
        push   0x493e0
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 0x10
        je     loc_2_128A
        mov    al, bl
        and    al, 0xa0
        cmp    al, 0xa0
        je     loc_2_128A
        or     bl, 0xa0
        mov    al, byte ptr [esi + edi + 0x28]
        push   ebx
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 0x18]
        call   sub_1_00A7
loc_2_1287:
        add    esp, 0x10
loc_2_128A:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_1294:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0xc]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    dl, byte ptr [ecx + eax + 0x28]
        push   2
        push   edx
        push   dword ptr [eax + ecx*4 + 0x18]
        call   loc_2_12B2
loc_2_12B2:
        add    esp, 0xc
        mov    dl, al
        and    dl, 0x10
        mov    ecx, dword ptr [ebp + 0x10]
        shr    dl, 4
        mov    bl, al
        and    bl, 3
        mov    byte ptr [ecx], dl
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    bl, 1
        mov    byte ptr [ecx], bl
        jne    loc_2_12D5
        mov    byte ptr [ecx], dl
        jmp    loc_2_12DD
loc_2_12D5:
        cmp    bl, 3
        jne    loc_2_12DD
        mov    byte ptr [ecx], 0
loc_2_12DD:
        and    al, 0xc
        mov    esi, dword ptr [ebp + 0x18]
        shr    al, 2
        mov    byte ptr [esi], al
        cmp    al, 1
        jne    loc_2_12EF
        mov    byte ptr [esi], dl
        jmp    loc_2_12F6
loc_2_12EF:
        cmp    al, 3
        jne    loc_2_12F6
        mov    byte ptr [ecx], 0
loc_2_12F6:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_12FA:
        mov    al, 1
        ret    
sub_2_12FD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   0
        push   dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   loc_2_1310
loc_2_1310:
        add    esp, 0xc
        mov    bl, al
        push   1
        push   dword ptr [ebp + 0xc]
        push   esi
        call   loc_2_1320
loc_2_1320:
        add    esp, 0xc
        push   ebx
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        jne    loc_2_136E
        push   1
        push   0x1e
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_00A7
loc_2_133D:
        add    esp, 0x10
        push   0
        push   0x1e
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_00A7
loc_2_134D:
        add    esp, 0x10
        push   0
        push   dword ptr [ebp + 0xc]
        push   esi
        call   loc_2_135B
loc_2_135B:
        add    esp, 0xc
        mov    bl, al
        push   1
        push   dword ptr [ebp + 0xc]
        push   esi
        call   loc_2_136B
loc_2_136B:
        add    esp, 0xc
loc_2_136E:
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_1374:
        mov    al, byte ptr [esp + 4]
        cmp    al, 0x8a
        je     loc_2_1390
        cmp    al, 0x87
        je     loc_2_1390
        cmp    al, 0x8f
        je     loc_2_1390
        cmp    al, 0x82
        jb     loc_2_138C
        cmp    al, 0x84
        jbe    loc_2_1390
loc_2_138C:
        xor    eax, eax
        jmp    loc_2_1395
loc_2_1390:
        mov    eax, 1
loc_2_1395:
        ret    
sub_2_1396:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    word ptr [esi + 0x36], 0xceb8
        mov    edi, 1
        mov    al, byte ptr [esi + 0x3b]
        mov    dword ptr [esi + 0xc], 0      ; -> obj3:0x0010
        mov    dword ptr [esi + 0x10], 0     ; -> obj3:0x004C
        mov    dword ptr [esi + 0x14], 0     ; -> obj3:0x0074
        mov    cl, al
        and    cl, 0x1f
        test   al, 0x80
        je     loc_2_13D6
        movzx  eax, cl
        mov    al, byte ptr [eax + esi + 0x3d]
        test   al, al
        je     loc_2_13D6
        mov    cl, al
loc_2_13D6:
        test   cl, cl
        jne    loc_2_13E3
        mov    byte ptr [5], 0               ; -> loc_3_01B9
        jmp    loc_2_13EF
loc_2_13E3:
        shl    cl, 4
        or     cl, 8
        mov    byte ptr [5], cl              ; -> loc_3_01B9
loc_2_13EF:
        xor    bl, bl
        cmp    byte ptr [esi + 0x38], bl
        jbe    loc_2_1416
loc_2_13F6:
        movzx  ecx, bl
        push   0                             ; -> obj3:0x01B4
        inc    bl
        mov    al, byte ptr [esi + ecx + 0x28]
        push   eax
        push   dword ptr [esi + ecx*4 + 0x18]
        call   sub_2_146D
        add    esp, 0xc
        cmp    byte ptr [esi + 0x38], bl
        ja     loc_2_13F6
loc_2_1416:
        push   esi
        call   0
        add    esp, 4
        test   eax, eax
        jne    loc_2_144E
        push   esi
        call   sub_2_045C
        add    esp, 4
        test   eax, eax
        jne    loc_2_144E
        push   esi
        call   sub_2_06BA
        add    esp, 4
        test   eax, eax
        jne    loc_2_144E
        push   esi
        call   sub_2_0983
        add    esp, 4
        test   eax, eax
        jne    loc_2_144E
        mov    byte ptr [esi + 0x39], 0
loc_2_144E:
        movzx  eax, byte ptr [esi + 0x39]
        shl    eax, 5
        mov    eax, dword ptr [eax]          ; -> obj3:0x00DC
        test   eax, eax
        je     loc_2_1467
        push   esi
        call   eax
        add    esp, 4
        mov    edi, eax
loc_2_1467:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_146D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        cmp    byte ptr [esi], 0
        je     loc_2_14B6
loc_2_147D:
        mov    bl, byte ptr [esi]
        inc    esi
        mov    cl, bl
        mov    al, byte ptr [esi]
        inc    esi
        dec    bl
        mov    byte ptr [ebp - 1], al
        test   cl, cl
        je     loc_2_14B1
loc_2_148E:
        mov    eax, esi
        mov    cl, byte ptr [ebp - 1]
        inc    esi
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [eax]
        push   eax
        push   ecx
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_00A7
loc_2_14A6:
        add    esp, 0x10
        mov    cl, bl
        dec    bl
        test   cl, cl
        jne    loc_2_148E
loc_2_14B1:
        cmp    byte ptr [esi], 0
        jne    loc_2_147D
loc_2_14B6:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_14BC:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    word ptr [eax + 0x10], 0x501
        mov    byte ptr [eax + 0x1c], cl
        mov    byte ptr [eax + 0x18], cl
        mov    byte ptr [eax + 0x19], 1
        and    byte ptr [eax + 0x20], 0xfe
        mov    byte ptr [eax + 0x1d], cl
        ret    
loc_2_14DA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
        mov    word ptr [esi + 0x18], bx
        mov    cl, 7
        mov    word ptr [esi + 0x14], bx
        mov    al, byte ptr [edi + 0x38]
        push   edi
        mov    byte ptr [esi + 0x10], al
        imul   cl
        mov    byte ptr [esi + 0x11], al
        call   sub_2_0B39
        add    esp, 4
        mov    ecx, dword ptr [edi + 0xc]
        movzx  eax, word ptr [edi + 0x30]
        push   0x14
        mov    dword ptr [ecx + 2], eax
        push   ecx
        push   dword ptr [esi]
        call   sub_2_23A7
        add    esp, 0xc
        mov    byte ptr [esi + 0x1d], bl
        and    byte ptr [esi + 0x20], 0xfe
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_1527:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ax, byte ptr [edx + 0x3b]
        mov    word ptr [ecx], ax
        mov    byte ptr [ecx + 0x15], 0
        mov    byte ptr [ecx + 0x1d], 0
        and    byte ptr [ecx + 0x20], 0xfe
        ret    
loc_2_1544:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    cx, word ptr [esi]
        test   cl, 0x20
        jne    loc_2_15E8
        test   cl, 0x40
        je     loc_2_15E8
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [edi + 0x3b]
        xor    al, cl
        test   al, 0x1f
        jne    loc_2_15E8
        mov    ax, cx
        mov    byte ptr [edi + 0x3b], cl
        and    al, 0x1f
        movzx  edx, al
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [edx + edi + 0x3d]
        test   al, al
        je     loc_2_158A
        mov    byte ptr [ebp - 1], al
loc_2_158A:
        shl    byte ptr [ebp - 1], 4
        or     byte ptr [ebp - 1], 8
        test   cl, 0x80
        jne    loc_2_159B
        mov    byte ptr [ebp - 1], 0
loc_2_159B:
        mov    byte ptr [ebp - 2], 0
        cmp    byte ptr [edi + 0x38], 0
        je     loc_2_15E0
loc_2_15A5:
        movzx  ebx, byte ptr [ebp - 2]
        push   5
        mov    al, byte ptr [ebx + edi + 0x28]
        push   eax
        push   dword ptr [edi + ebx*4 + 0x18]
        call   loc_2_15B9
loc_2_15B9:
        add    esp, 0xc
        and    al, 7
        or     al, byte ptr [ebp - 1]
        mov    cl, byte ptr [ebx + edi + 0x28]
        push   eax
        push   5
        push   ecx
        push   dword ptr [edi + ebx*4 + 0x18]
        call   sub_1_00A7
loc_2_15D2:
        add    esp, 0x10
        mov    cl, byte ptr [edi + 0x38]
        inc    byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 2], cl
        jb     loc_2_15A5
loc_2_15E0:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_15EE
loc_2_15E8:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 6
loc_2_15EE:
        cmp    byte ptr [eax], 0
        jne    loc_2_15F9
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_15FD
loc_2_15F9:
        or     byte ptr [esi + 0x20], 1
loc_2_15FD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1604:
        push   ebx
        mov    cl, 7
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    bl, 7
        mov    dl, byte ptr [esi + 0x15]
        mov    al, dl
        sub    ah, ah
        div    cl
        mov    cl, al
        mov    al, dl
        sub    ah, ah
        div    bl
        cmp    byte ptr [esi + 0x11], 0xff
        mov    al, ah
        jne    loc_2_1670
        mov    edx, dword ptr [esp + 0x10]
        cmp    byte ptr [edx + 0x38], cl
        jbe    loc_2_1670
        cmp    al, 5
        jb     loc_2_1645
        mov    eax, 0                        ; -> obj3:0x00B4
        mov    edx, 0x18
        mov    byte ptr [esi + 0x10], 0x84
        jmp    loc_2_1651
loc_2_1645:
        mov    byte ptr [esi + 0x10], 0x83
        mov    eax, dword ptr [edx + 0x14]
        mov    edx, 0x20
loc_2_1651:
        mov    di, 1
        push   edx
        shl    di, cl
        mov    word ptr [esi + 0x18], di
        push   eax
        push   dword ptr [esi]
        call   sub_2_23A7
        add    esp, 0xc
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_1676
loc_2_1670:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x11
loc_2_1676:
        cmp    byte ptr [eax], 0
        jne    loc_2_1681
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_1685
loc_2_1681:
        or     byte ptr [esi + 0x20], 1
loc_2_1685:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_1689:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    dl, 7
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    bl, 7
        mov    cl, byte ptr [edi + 0x11]
        mov    al, cl
        sub    ah, ah
        mov    esi, dword ptr [ebp + 8]
        div    dl
        mov    dl, al
        mov    al, cl
        sub    ah, ah
        div    bl
        mov    byte ptr [ebp - 1], ah
        cmp    byte ptr [esi + 0x38], dl
        ja     loc_2_16C0
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0x11
        jmp    loc_2_1850
loc_2_16C0:
        mov    al, dl
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edi + 0x15], 0
        or     al, 0x10
        mov    byte ptr [edi + 0x10], al
        mov    al, 1
        shl    al, cl
        mov    byte ptr [ebp - 5], al
        cmp    cl, 5
        jb     loc_2_1781
        mov    byte ptr [edi + 0x15], 1
        add    al, al
        push   6
        mov    byte ptr [ebp - 5], al
        movzx  eax, dl
        mov    byte ptr [edi + 0x14], 0
        mov    bl, byte ptr [esi + eax + 0x28]
        mov    esi, dword ptr [esi + eax*4 + 0x18]
        push   ebx
        push   esi
        call   loc_2_16FF
loc_2_16FF:
        add    esp, 0xc
        test   byte ptr [ebp - 5], al
        je     loc_2_170B
        mov    byte ptr [edi + 0x15], 3
loc_2_170B:
        mov    al, byte ptr [ebp - 1]
        mov    cl, byte ptr [ebp - 8]
        sub    al, 5
        push   7
        push   ebx
        cmp    al, 1
        sbb    byte ptr [ebp - 8], cl
        push   esi
        and    byte ptr [ebp - 8], 0xf1
        add    byte ptr [ebp - 8], 0x10
        call   loc_2_1729
loc_2_1729:
        add    esp, 0xc
        test   byte ptr [ebp - 8], al
        je     loc_2_1735
        or     byte ptr [edi + 0x15], 4
loc_2_1735:
        sub    byte ptr [ebp - 1], 5
        mov    al, byte ptr [ebp - 1]
        lea    eax, [eax*4 + 8]
        push   eax
        push   ebx
        push   esi
        call   sub_1_00C1
loc_2_174B:
        movzx  ecx, ax
        add    esp, 0xc
        mov    al, byte ptr [ebp - 1]
        shl    al, 2
        mov    dword ptr [edi], ecx
        add    al, 0xa
        push   eax
        push   ebx
        push   esi
        call   sub_1_00C1
loc_2_1763:
        movzx  ecx, ax
        add    esp, 0xc
        mov    eax, ecx
        mov    dword ptr [edi + 0x18], ecx
        test   eax, eax
        je     loc_2_184A
        sub    eax, dword ptr [edi]
        inc    eax
        mov    dword ptr [edi + 0x18], eax
        jmp    loc_2_184A
loc_2_1781:
        movzx  eax, dl
        mov    ebx, eax
        push   6
        mov    dword ptr [ebp - 0xc], eax
        mov    cl, byte ptr [esi + ebx + 0x28]
        push   ecx
        push   dword ptr [esi + ebx*4 + 0x18]
        call   loc_2_1799
loc_2_1799:
        add    esp, 0xc
        test   byte ptr [ebp - 5], al
        je     loc_2_17A5
        mov    byte ptr [edi + 0x15], 2
loc_2_17A5:
        mov    al, byte ptr [ebp - 1]
        mov    cl, byte ptr [esi + ebx + 0x28]
        lea    eax, [eax*8 + 0x10]
        push   eax
        push   ecx
        push   dword ptr [esi + ebx*4 + 0x18]
        call   sub_1_00C1
loc_2_17BE:
        add    esp, 0xc
        xor    ecx, ecx
        mov    cx, ax
        and    ecx, 0xfff
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [edi], ecx
        test   ah, 0x80
        je     loc_2_17DA
        or     byte ptr [edi + 0x15], 4
loc_2_17DA:
        mov    al, byte ptr [ebp - 1]
        mov    cl, byte ptr [esi + ebx + 0x28]
        shl    al, 3
        add    al, 0x12
        push   eax
        push   ecx
        push   dword ptr [esi + ebx*4 + 0x18]
        call   sub_1_00C1
loc_2_17F1:
        add    esp, 0xc
        lea    ecx, [edi + 0x18]
        xor    edx, edx
        mov    dx, ax
        and    edx, 0xfff
        mov    dword ptr [ecx], edx
        je     loc_2_180C
        sub    edx, dword ptr [ebp - 8]
        inc    edx
        mov    dword ptr [ecx], edx
loc_2_180C:
        shr    ax, 0xe
        movzx  edx, ax
        movzx  eax, byte ptr [esi + 0x39]
        shl    eax, 5
        mov    bl, byte ptr [edx]            ; -> obj3:0x00CC
        shl    dword ptr [ecx], 0xc
        mov    byte ptr [edi + 0x14], bl
        mov    ebx, dword ptr [ebp - 8]
        shl    ebx, 0xc
        mov    dword ptr [edi], ebx
        mov    ecx, dword ptr [eax + 0x14]   ; -> loc_3_00F0
        test   ecx, ecx
        je     loc_2_184A
        movzx  eax, byte ptr [ebp - 1]
        push   eax
        push   dword ptr [ebp - 0xc]
        push   esi
        call   ecx
        add    esp, 0xc
        or     eax, ebx
        mov    dword ptr [edi], eax
loc_2_184A:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_1850:
        cmp    byte ptr [eax], 0
        jne    loc_2_185B
        and    byte ptr [edi + 0x20], 0xfe
        jmp    loc_2_185F
loc_2_185B:
        or     byte ptr [edi + 0x20], 1
loc_2_185F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1866:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    dl, 7
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [edi + 0x11]
        mov    al, cl
        sub    ah, ah
        div    dl
        mov    byte ptr [ebp - 2], al
        mov    al, cl
        sub    ah, ah
        mov    ecx, dword ptr [ebp + 8]
        div    dl
        mov    byte ptr [ebp - 1], ah
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ecx + 0x38], al
        ja     loc_2_189D
        mov    byte ptr [edi + 0x1d], 0x11
        jmp    loc_2_1AC5
loc_2_189D:
        mov    al, byte ptr [edi + 0x10]
        test   al, 0x10
        je     loc_2_1AC1
        and    al, 0xf
        cmp    al, byte ptr [ebp - 2]
        jne    loc_2_1AC1
        mov    byte ptr [edi + 0x1d], 0
        cmp    byte ptr [ebp - 1], 5
        jb     loc_2_19A2
        mov    al, byte ptr [edi + 0x15]
        test   al, 1
        jne    loc_2_18D1
        mov    byte ptr [edi + 0x1d], 0xd
        jmp    loc_2_1A95
loc_2_18D1:
        push   0                             ; -> obj3:0x00B4
        push   eax
        push   dword ptr [edi + 0x18]
        push   dword ptr [edi]
        call   sub_2_22A1
        add    esp, 0x10
        test   al, al
        jne    loc_2_1A92
        mov    ecx, dword ptr [ebp + 8]
        push   0
        movzx  eax, byte ptr [ebp - 2]
        push   dword ptr [ebp - 1]
        mov    bl, byte ptr [eax + ecx + 0x28]
        mov    esi, dword ptr [ecx + eax*4 + 0x18]
        push   ebx
        push   esi
        call   sub_2_2362
        mov    ax, word ptr [edi]
        add    esp, 0x10
        mov    cl, byte ptr [ebp - 1]
        push   eax
        lea    edx, [ecx*4 - 0xc]
        push   edx
        push   ebx
        push   esi
        call   sub_1_00FF
loc_2_1920:
        add    esp, 0x10
        mov    eax, dword ptr [edi + 0x18]
        test   eax, eax
        je     loc_2_1931
        add    ax, word ptr [edi]
        dec    ax
        jmp    loc_2_1935
loc_2_1931:
        mov    ax, 0xffff
loc_2_1935:
        push   eax
        mov    al, byte ptr [ebp - 1]
        shl    al, 2
        sub    al, 0xa
        push   eax
        push   ebx
        push   esi
        call   sub_1_00FF
loc_2_1946:
        add    esp, 0x10
        test   byte ptr [edi + 0x15], 4
        jne    loc_2_1955
        mov    byte ptr [ebp - 5], 0
        jmp    loc_2_196C
loc_2_1955:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 8]
        and    eax, 0x4000000
        cmp    eax, 1
        sbb    al, al
        and    al, 8
        add    al, 3
        mov    byte ptr [ebp - 5], al
loc_2_196C:
        mov    byte ptr [ebp - 9], 0xf0
        cmp    byte ptr [ebp - 1], 5
        jbe    loc_2_197E
        shl    byte ptr [ebp - 5], 4
        mov    byte ptr [ebp - 9], 0xf
loc_2_197E:
        push   7
        push   ebx
        push   esi
        call   loc_2_1987
loc_2_1987:
        add    esp, 0xc
        and    al, byte ptr [ebp - 9]
        or     al, byte ptr [ebp - 5]
        push   eax
        push   7
        push   ebx
        push   esi
        call   sub_1_00A7
loc_2_199A:
        add    esp, 0x10
        jmp    loc_2_1A95
loc_2_19A2:
        test   byte ptr [edi + 0x15], 1
        je     loc_2_19B1
        mov    byte ptr [edi + 0x1d], 0xd
        jmp    loc_2_1A95
loc_2_19B1:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [edi + 0x15]
        push   dword ptr [ecx + 0x14]
        push   edx
        push   dword ptr [edi + 0x18]
        push   dword ptr [edi]
        call   sub_2_22A1
        add    esp, 0x10
        test   al, al
        jne    loc_2_1A95
        lea    eax, [ebp - 5]
        mov    cl, byte ptr [edi + 0x14]
        push   eax
        push   ecx
        call   sub_2_22FB
        add    esp, 8
        test   al, al
        jne    loc_2_1A95
        mov    ecx, dword ptr [ebp + 8]
        push   0
        push   dword ptr [ebp - 1]
        movzx  eax, byte ptr [ebp - 2]
        mov    dword ptr [ebp - 0xc], eax
        mov    bl, byte ptr [eax + ecx + 0x28]
        mov    esi, dword ptr [ecx + eax*4 + 0x18]
        push   ebx
        push   esi
        call   sub_2_2362
        add    esp, 0x10
        mov    eax, dword ptr [edi]
        shr    eax, 0xc
        and    ax, 0xfff
        test   byte ptr [edi + 0x15], 4
        je     loc_2_1A1B
        or     ah, 0x80
loc_2_1A1B:
        push   eax
        mov    al, byte ptr [ebp - 1]
        lea    eax, [eax*8 + 0x10]
        push   eax
        push   ebx
        push   esi
        call   sub_1_00FF
loc_2_1A2E:
        mov    ax, 0xfff
        add    esp, 0x10
        cmp    dword ptr [edi + 0x18], 0
        je     loc_2_1A48
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [edi + 0x18]
        dec    eax
        shr    eax, 0xc
        and    ax, 0xfff
loc_2_1A48:
        movzx  cx, byte ptr [ebp - 5]
        shl    cx, 0xe
        or     ax, cx
        push   eax
        mov    al, byte ptr [ebp - 1]
        shl    al, 3
        add    al, 0x12
        push   eax
        push   ebx
        push   esi
        call   sub_1_00FF
loc_2_1A65:
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ecx + 0x39]
        shl    eax, 5
        mov    ecx, dword ptr [eax + 0x10]   ; -> loc_3_00EC
        test   ecx, ecx
        je     loc_2_1A95
        push   dword ptr [edi]
        movzx  edx, byte ptr [ebp - 1]
        push   edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp + 8]
        call   ecx
        add    esp, 0x10
        test   al, al
        je     loc_2_1A95
loc_2_1A92:
        mov    byte ptr [edi + 0x1d], al
loc_2_1A95:
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_2_1AD1
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ebp - 2]
        mov    cl, byte ptr [edi + 0x15]
        mov    bl, byte ptr [eax + edx + 0x28]
        and    ecx, 2
        push   ecx
        push   dword ptr [ebp - 1]
        push   ebx
        push   dword ptr [edx + eax*4 + 0x18]
        call   sub_2_2362
        add    esp, 0x10
        jmp    loc_2_1AC5
loc_2_1AC1:
        mov    byte ptr [edi + 0x1d], 0xb
loc_2_1AC5:
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_2_1AD1
        and    byte ptr [edi + 0x20], 0xfe
        jmp    loc_2_1AD5
loc_2_1AD1:
        or     byte ptr [edi + 0x20], 1
loc_2_1AD5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1ADC:
        sub    esp, 4
        mov    cl, 7
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        mov    dl, byte ptr [edi + 0x11]
        mov    al, dl
        sub    ah, ah
        mov    esi, dword ptr [esp + 0x18]
        div    cl
        mov    bl, al
        mov    al, dl
        sub    ah, ah
        div    cl
        mov    byte ptr [esp + 0x13], ah
        cmp    byte ptr [esi + 0x38], bl
        jbe    loc_2_1BCB
        cmp    ah, 5
        jae    loc_2_1BCB
        cmp    byte ptr [edi + 0x10], 0
        je     loc_2_1B25
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 8
        jmp    loc_2_1BD1
loc_2_1B25:
        movzx  ecx, bl
        mov    al, byte ptr [esp + 0x13]
        mov    dl, byte ptr [esi + ecx + 0x28]
        shl    al, 3
        add    al, 0x14
        push   eax
        push   edx
        movzx  eax, bl
        push   dword ptr [esi + eax*4 + 0x18]
        call   sub_1_00C1
loc_2_1B43:
        movzx  edx, bl
        mov    bp, ax
        mov    cl, byte ptr [esp + 0x1f]
        add    esp, 0xc
        lea    eax, [ecx*8 + 0x10]
        mov    cl, byte ptr [esi + edx + 0x28]
        push   eax
        movzx  eax, bl
        push   ecx
        push   dword ptr [esi + eax*4 + 0x18]
        call   sub_1_00C1
loc_2_1B69:
        and    ax, 0xfff
        add    esp, 0xc
        mov    byte ptr [edi + 0x14], 0
        add    ax, bp
        and    ax, 0x3fff
        test   bp, 0x8000
        mov    word ptr [edi], ax
        je     loc_2_1B89
        mov    byte ptr [edi + 0x14], 4
loc_2_1B89:
        test   bp, 0x4000
        je     loc_2_1B94
        or     byte ptr [edi + 0x14], 1
loc_2_1B94:
        movzx  eax, bl
        movzx  ecx, bl
        push   6
        mov    al, byte ptr [esi + eax + 0x28]
        push   eax
        push   dword ptr [esi + ecx*4 + 0x18]
        call   loc_2_1BAA
loc_2_1BAA:
        movzx  ebx, al
        mov    cl, byte ptr [esp + 0x1f]
        add    esp, 0xc
        mov    edx, 1
        shl    edx, cl
        test   edx, ebx
        je     loc_2_1BC3
        or     byte ptr [edi + 0x14], 2
loc_2_1BC3:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_1BD1
loc_2_1BCB:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0x11
loc_2_1BD1:
        cmp    byte ptr [eax], 0
        jne    loc_2_1BDC
        and    byte ptr [edi + 0x20], 0xfe
        jmp    loc_2_1BE0
loc_2_1BDC:
        or     byte ptr [edi + 0x20], 1
loc_2_1BE0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_2_1BE8:
        sub    esp, 8
        mov    cl, 7
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x20]
        mov    dl, byte ptr [edi + 0x11]
        mov    al, dl
        sub    ah, ah
        mov    esi, dword ptr [esp + 0x1c]
        div    cl
        mov    bl, al
        mov    al, dl
        sub    ah, ah
        div    cl
        mov    byte ptr [esp + 0x13], ah
        cmp    byte ptr [esi + 0x38], bl
        jbe    loc_2_1CC8
        cmp    ah, 5
        jae    loc_2_1CC8
        cmp    byte ptr [edi + 0x10], 0
        je     loc_2_1C31
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 8
        jmp    loc_2_1CCE
loc_2_1C31:
        mov    bp, word ptr [edi]
        test   bp, 0xc000
        je     loc_2_1C46
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 7
        jmp    loc_2_1CCE
loc_2_1C46:
        movzx  eax, byte ptr [edi + 0x14]
        test   eax, 0xfffffff8
        je     loc_2_1C59
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 2
        jmp    loc_2_1CCE
loc_2_1C59:
        movzx  ecx, bl
        mov    al, byte ptr [esp + 0x13]
        mov    dl, byte ptr [esi + ecx + 0x28]
        lea    eax, [eax*8 + 0x10]
        push   eax
        push   edx
        movzx  eax, bl
        push   dword ptr [esi + eax*4 + 0x18]
        call   sub_1_00C1
loc_2_1C79:
        and    ax, 0xfff
        add    esp, 0xc
        sub    bp, ax
        and    bp, 0x3fff
        test   byte ptr [edi + 0x14], 4
        je     loc_2_1C93
        or     bp, 0x8000
loc_2_1C93:
        test   byte ptr [edi + 0x14], 1
        je     loc_2_1C9E
        or     bp, 0x4000
loc_2_1C9E:
        mov    al, byte ptr [esp + 0x13]
        push   ebp
        movzx  ecx, bl
        shl    al, 3
        mov    dl, byte ptr [esi + ecx + 0x28]
        add    al, 0x14
        push   eax
        push   edx
        movzx  eax, bl
        push   dword ptr [esi + eax*4 + 0x18]
        call   sub_1_00FF
loc_2_1CBD:
        add    esp, 0x10
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_1CCE
loc_2_1CC8:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0x11
loc_2_1CCE:
        cmp    byte ptr [eax], 0
        jne    loc_2_1CD9
        and    byte ptr [edi + 0x20], 0xfe
        jmp    loc_2_1CDD
loc_2_1CD9:
        or     byte ptr [edi + 0x20], 1
loc_2_1CDD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_2_1CE5:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        mov    al, byte ptr [esi + 0x10]
        cmp    byte ptr [ebx + 0x38], al
        ja     loc_2_1D00
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_1D40
loc_2_1D00:
        mov    byte ptr [esi + 0x11], 0xf0
        mov    ecx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [esi]
        mov    byte ptr [esi + 0x15], 0xf0
        mov    byte ptr [esi + 0x14], 0
        movzx  eax, word ptr [ebx + 0x30]
        push   0x12
        mov    dword ptr [ecx + 2], eax
        push   ecx
        push   edi
        call   sub_2_23A7
        movzx  eax, byte ptr [ebx + 0x39]
        shl    eax, 5
        add    esp, 0xc
        cmp    dword ptr [eax + 0x18], 0     ; -> sub_3_00F4
        je     loc_2_1D3A
        mov    word ptr [edi + 0x10], 1
loc_2_1D3A:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_1D40:
        cmp    byte ptr [eax], 0
        jne    loc_2_1D4B
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_1D4F
loc_2_1D4B:
        or     byte ptr [esi + 0x20], 1
loc_2_1D4F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_1D53:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x10]
        cmp    byte ptr [edi + 0x38], al
        ja     loc_2_1D72
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_1DC8
loc_2_1D72:
        push   esi
        push   edi
        call   sub_2_2082
        add    esp, 8
        mov    bl, byte ptr [esi + 0x10]
        movzx  ax, byte ptr [edi + 0x3c]
        movzx  ecx, bl
        mov    word ptr [esi + 8], ax
        push   5
        mov    al, byte ptr [ecx + edi + 0x28]
        push   eax
        push   dword ptr [edi + ecx*4 + 0x18]
        call   loc_2_1D9B
loc_2_1D9B:
        shl    al, 4
        add    esp, 0xc
        lea    ecx, [ebp - 1]
        lea    edx, [ebp - 2]
        push   ecx
        mov    byte ptr [esi + 0x11], al
        push   edx
        push   ebx
        push   edi
        call   sub_2_114F
        add    esp, 0x10
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0x19], cl
        mov    byte ptr [esi + 0x18], al
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_1DC8:
        cmp    byte ptr [eax], 0
        jne    loc_2_1DD3
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_1DD7
loc_2_1DD3:
        or     byte ptr [esi + 0x20], 1
loc_2_1DD7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1DDE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ax, word ptr [edi]
        mov    cl, byte ptr [edi + 0x10]
        shr    ax, 8
        and    al, 3
        cmp    byte ptr [esi + 0x38], cl
        mov    byte ptr [ebp - 1], al
        ja     loc_2_1E0C
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_1FF1
loc_2_1E0C:
        mov    al, byte ptr [edi + 0x19]
        and    al, 0x30
        cmp    al, 0x30
        jne    loc_2_1E20
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 2
        jmp    loc_2_1FF1
loc_2_1E20:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_1FEB
        cmp    byte ptr [ebp - 1], 3
        jne    loc_2_1E3B
        cmp    word ptr [edi + 8], 1
        jae    loc_2_1FEB
loc_2_1E3B:
        mov    ax, word ptr [edi]
        mov    cl, byte ptr [edi + 0x18]
        and    al, 0x1f
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 0x19]
        and    al, 7
        mov    byte ptr [ebp - 9], al
        mov    al, cl
        and    al, 0x70
        shr    al, 4
        mov    byte ptr [ebp - 5], al
        mov    al, cl
        and    al, 7
        cmp    byte ptr [ebp - 1], 1
        mov    byte ptr [ebp - 3], al
        je     loc_2_1E86
        test   byte ptr [edi], 0x80
        je     loc_2_1E86
        cmp    byte ptr [ebp - 2], 0xf
        ja     loc_2_1E7B
        movzx  eax, byte ptr [ebp - 2]
        cmp    byte ptr [eax + esi + 0x3d], 0
        jne    loc_2_1E86
loc_2_1E7B:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 6
        jmp    loc_2_1FF1
loc_2_1E86:
        mov    edx, dword ptr [esi + 0xc]
        movzx  ax, byte ptr [ebp - 9]
        mov    bx, word ptr [edx + 0xa]
        cmp    bx, ax
        jbe    loc_2_1FE3
        movzx  eax, byte ptr [ebp - 9]
        test   byte ptr [edx + eax*2 + 0xd], 0x80
        je     loc_2_1FE3
        movzx  ax, byte ptr [ebp - 5]
        cmp    bx, ax
        jbe    loc_2_1FDB
        movzx  eax, byte ptr [ebp - 5]
        test   byte ptr [edx + eax*2 + 0xd], 0x40
        je     loc_2_1FDB
        movzx  ax, byte ptr [ebp - 3]
        cmp    bx, ax
        jbe    loc_2_1FDB
        movzx  eax, byte ptr [ebp - 3]
        test   byte ptr [edx + eax*2 + 0xd], 0x20
        je     loc_2_1FDB
        push   ecx
        mov    al, byte ptr [edi + 0x19]
        push   eax
        mov    cl, byte ptr [edi + 0x10]
        push   ecx
        push   esi
        call   sub_2_101F
        add    esp, 0x10
        test   eax, eax
        je     loc_2_1FD3
        mov    al, byte ptr [edi + 0x15]
        push   5
        movzx  ebx, byte ptr [edi + 0x10]
        not    al
        and    byte ptr [ebx + esi + 0x2c], al
        mov    cl, byte ptr [ebx + esi + 0x28]
        push   ecx
        push   dword ptr [esi + ebx*4 + 0x18]
        call   loc_2_1F1B
loc_2_1F1B:
        add    esp, 0xc
        and    al, 0xf0
        mov    cl, byte ptr [edi + 0x11]
        mov    dl, byte ptr [ebx + esi + 0x28]
        shr    cl, 4
        or     al, cl
        push   eax
        push   5
        push   edx
        push   dword ptr [esi + ebx*4 + 0x18]
        call   sub_1_00A7
loc_2_1F39:
        add    esp, 0x10
        mov    cl, byte ptr [ebx + esi + 0x28]
        push   3
        push   ecx
        push   dword ptr [esi + ebx*4 + 0x18]
        call   loc_2_1F4C
loc_2_1F4C:
        add    esp, 0xc
        and    al, 0xd0
        cmp    byte ptr [ebp - 1], 2
        je     loc_2_1F5D
        cmp    byte ptr [ebp - 1], 3
        jne    loc_2_1F6C
loc_2_1F5D:
        or     al, 0x20
        test   byte ptr [edi], 0x80
        je     loc_2_1F6C
        movzx  ecx, byte ptr [ebp - 2]
        or     al, byte ptr [ecx + esi + 0x3d]
loc_2_1F6C:
        push   eax
        push   3
        mov    al, byte ptr [ebx + esi + 0x28]
        push   eax
        push   dword ptr [esi + ebx*4 + 0x18]
        call   sub_1_00A7
loc_2_1F7D:
        add    esp, 0x10
        cmp    byte ptr [ebp - 1], 3
        jne    loc_2_1FA4
        mov    cl, byte ptr [edi + 8]
        push   esi
        mov    byte ptr [esi + 0x3c], cl
        movzx  eax, byte ptr [esi + 0x39]
        movzx  ecx, cl
        or     byte ptr [esi + 0xb], 8
        lea    ebx, [ecx + eax*4]
        call   sub_3_00F4
        jmp    loc_2_1FC8
loc_2_1FA4:
        mov    eax, dword ptr [esi + 8]
        test   eax, 0x8000000
        je     loc_2_1FCB
        and    eax, 0xf7ffffff
        push   esi
        mov    dword ptr [esi + 8], eax
        movzx  eax, byte ptr [esi + 0x39]
        shl    eax, 5
        mov    byte ptr [esi + 0x3c], 0
        call   sub_3_00F8
loc_2_1FC8:
        add    esp, 4
loc_2_1FCB:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_1FF1
loc_2_1FD3:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xe
        jmp    loc_2_1FF1
loc_2_1FDB:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xf
        jmp    loc_2_1FF1
loc_2_1FE3:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xe
        jmp    loc_2_1FF1
loc_2_1FEB:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xd
loc_2_1FF1:
        cmp    byte ptr [eax], 0
        jne    loc_2_1FFC
        and    byte ptr [edi + 0x20], 0xfe
        jmp    loc_2_2000
loc_2_1FFC:
        or     byte ptr [edi + 0x20], 1
loc_2_2000:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_2007:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    al, byte ptr [esi + 0x10]
        cmp    byte ptr [edi + 0x38], al
        ja     loc_2_2021
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_2070
loc_2_2021:
        push   esi
        push   edi
        call   sub_2_2082
        movzx  eax, byte ptr [esi + 0x10]
        add    esp, 8
        mov    cl, byte ptr [eax + edi + 0x28]
        push   1
        push   ecx
        push   dword ptr [edi + eax*4 + 0x18]
        call   loc_2_203F
loc_2_203F:
        add    esp, 0xc
        lea    ecx, [esi + 0x11]
        mov    dl, al
        shl    dl, 4
        not    dl
        and    dl, 0x30
        test   al, 0x10
        mov    byte ptr [ecx], dl
        je     loc_2_205A
        or     dl, 1
        mov    byte ptr [ecx], dl
loc_2_205A:
        test   al, 0x20
        je     loc_2_2061
        or     byte ptr [ecx], 0x40
loc_2_2061:
        and    al, 0xc
        cmp    al, 0xc
        jne    loc_2_206A
        or     byte ptr [ecx], 0x80
loc_2_206A:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_2070:
        cmp    byte ptr [eax], 0
        jne    loc_2_207B
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_207F
loc_2_207B:
        or     byte ptr [esi + 0x20], 1
loc_2_207F:
        pop    edi
        pop    esi
        ret    
sub_2_2082:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x10]
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        push   ebp
        movzx  esi, byte ptr [ebx + 0x10]
        push   4
        lea    ebp, [edi + esi*4 + 0x18]
        mov    al, byte ptr [esi + edi + 0x28]
        push   eax
        push   dword ptr [ebp]
        call   loc_2_20A5
loc_2_20A5:
        add    esp, 0xc
        and    al, 0xf
        mov    byte ptr [ebx + 0x14], 0
        push   3
        or     byte ptr [esi + edi + 0x2c], al
        mov    al, byte ptr [esi + edi + 0x2c]
        mov    cl, byte ptr [esi + edi + 0x28]
        shl    al, 4
        push   ecx
        mov    esi, 1
        mov    byte ptr [ebx + 0x15], al
        push   dword ptr [ebp]
        call   loc_2_20D0
loc_2_20D0:
        add    esp, 0xc
        test   al, 0x20
        je     loc_2_20E8
        mov    ecx, dword ptr [edi + 8]
        and    ecx, 0x8000000
        cmp    ecx, 1
        sbb    esi, esi
        add    esi, 3
loc_2_20E8:
        shl    si, 8
        and    al, 0xf
        mov    word ptr [ebx], si
        je     loc_2_2113
        xor    cl, cl
loc_2_20F5:
        movzx  edx, cl
        cmp    byte ptr [edx + edi + 0x3d], al
        je     loc_2_2107
        inc    cl
        cmp    cl, 0x10
        jb     loc_2_20F5
        jmp    loc_2_2113
loc_2_2107:
        movzx  ax, cl
        or     ax, si
        or     al, 0xc0
        mov    word ptr [ebx], ax
loc_2_2113:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_2118:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        mov    ebx, dword ptr [esp + 0x14]
        push   ebp
        mov    al, byte ptr [esi + 0x10]
        cmp    byte ptr [ebx + 0x38], al
        ja     loc_2_213A
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_221D
loc_2_213A:
        movzx  edi, byte ptr [esi + 0x10]
        push   1
        mov    ebp, dword ptr [ebx + edi*4 + 0x18]
        mov    al, byte ptr [edi + ebx + 0x28]
        push   eax
        push   ebp
        call   loc_2_214F
loc_2_214F:
        add    esp, 0xc
        and    al, 0xc
        cmp    al, 0xc
        je     loc_2_2163
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x14
        jmp    loc_2_221D
loc_2_2163:
        push   0x20
        mov    al, byte ptr [edi + ebx + 0x28]
        push   6
        push   eax
        push   ebp
        call   sub_1_00A7
loc_2_2172:
        add    esp, 0x10
        mov    cl, byte ptr [edi + ebx + 0x28]
        push   3
        push   ecx
        push   ebp
        call   loc_2_2182
loc_2_2182:
        add    esp, 0xc
        and    al, 0xcf
        mov    byte ptr [esp + 0x13], al
        movzx  eax, byte ptr [ebx + 0x39]
        shl    eax, 5
        mov    eax, dword ptr [eax + 0x1c]   ; -> sub_3_00F8
        test   eax, eax
        je     loc_2_21A2
        push   ebx
        call   eax
        add    esp, 4
loc_2_21A2:
        push   dword ptr [esp + 0x13]
        push   3
        and    byte ptr [ebx + 0xb], 0xf7
        mov    byte ptr [ebx + 0x3c], 0
        mov    al, byte ptr [edi + ebx + 0x28]
        push   eax
        push   dword ptr [ebx + edi*4 + 0x18]
        call   sub_1_00A7
loc_2_21BE:
        add    esp, 0x10
        mov    cl, byte ptr [esi + 0x10]
        push   0
        push   0x10
        push   ecx
        push   ebx
        call   sub_2_101F
        add    esp, 0x10
        mov    ebp, dword ptr [ebx + edi*4 + 0x18]
        mov    al, byte ptr [edi + ebx + 0x28]
        and    byte ptr [esp + 0x13], 0xbf
        push   dword ptr [esp + 0x13]
        push   3
        push   eax
        push   ebp
        call   sub_1_00A7
loc_2_21EC:
        add    esp, 0x10
        push   0
        push   0x7d0
        call   sub_2_2A2C
        or     byte ptr [esp + 0x1b], 0x40
        add    esp, 8
        mov    cl, byte ptr [edi + ebx + 0x28]
        push   dword ptr [esp + 0x13]
        push   3
        push   ecx
        push   ebp
        call   sub_1_00A7
loc_2_2214:
        add    esp, 0x10
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_221D:
        cmp    byte ptr [eax], 0
        jne    loc_2_2228
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_222C
loc_2_2228:
        or     byte ptr [esi + 0x20], 1
loc_2_222C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_2_2234:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        cmp    byte ptr [esi + 0x10], 0
        je     loc_2_2249
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x15
        jmp    loc_2_228E
loc_2_2249:
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   eax
        call   sub_2_2A54
        add    esp, 4
        mov    ebx, eax
        mov    edi, dword ptr [esi]
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        mov    word ptr [esi + 0x14], 0x100
        push   ebx
        push   eax
        push   edi
        call   sub_2_23A7
        mov    ax, word ptr [edi]
        add    esp, 0xc
        cmp    bx, ax
        jae    loc_2_2280
        mov    byte ptr [edi + ebx + 4], 0
        jmp    loc_2_2288
loc_2_2280:
        movzx  eax, ax
        mov    byte ptr [edi + eax + 3], 0
loc_2_2288:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_228E:
        cmp    byte ptr [eax], 0
        jne    loc_2_2299
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_229D
loc_2_2299:
        or     byte ptr [esi + 0x20], 1
loc_2_229D:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_22A1:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x14]
        lea    esi, [eax + ecx - 1]
        cmp    dword ptr [edx + 2], eax
        ja     loc_2_22BD
        cmp    dword ptr [edx + 6], eax
        jae    loc_2_22C7
loc_2_22BD:
        test   byte ptr [ebp + 0x10], 2
        jne    loc_2_22F6
        test   eax, eax
        jne    loc_2_22F6
loc_2_22C7:
        cmp    dword ptr [edx + 0xa], ecx
        ja     loc_2_22D1
        cmp    dword ptr [edx + 0xe], ecx
        jae    loc_2_22DB
loc_2_22D1:
        test   byte ptr [ebp + 0x10], 2
        jne    loc_2_22F2
        test   ecx, ecx
        jne    loc_2_22F2
loc_2_22DB:
        cmp    dword ptr [edx + 6], esi
        jb     loc_2_22EE
        xor    esi, eax
        test   esi, 0xff000000
        jne    loc_2_22EE
        xor    al, al
        jmp    loc_2_22F8
loc_2_22EE:
        mov    al, 0xa
        jmp    loc_2_22F8
loc_2_22F2:
        mov    al, 0xa
        jmp    loc_2_22F8
loc_2_22F6:
        mov    al, 3
loc_2_22F8:
        pop    esi
        pop    ebp
        ret    
sub_2_22FB:
        push   ebp
        mov    al, 0x17
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    cl, byte ptr [ebp + 8]
        test   cl, 0x80
        jne    loc_2_235D
        mov    dl, cl
        and    dl, 7
        and    cl, 0x78
        mov    byte ptr [ebp - 2], dl
        mov    byte ptr [ebp - 1], cl
        jne    loc_2_2328
        movzx  ecx, dl
        mov    cl, byte ptr [ecx]            ; -> obj3:0x00D0
        mov    byte ptr [ebp - 1], cl
loc_2_2328:
        xor    dl, dl
loc_2_232A:
        movzx  ecx, dl
        mov    cl, byte ptr [ecx]            ; -> obj3:0x00CC
        mov    bl, cl
        and    bl, 7
        mov    byte ptr [ebp - 3], bl
        mov    bl, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 3], bl
        ja     loc_2_2356
        jne    loc_2_234D
        and    cl, 0x78
        cmp    cl, byte ptr [ebp - 1]
        jae    loc_2_2356
loc_2_234D:
        inc    dl
        cmp    dl, 4
        jb     loc_2_232A
        jmp    loc_2_235D
loc_2_2356:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], dl
        xor    al, al
loc_2_235D:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2362:
        push   ebp
        mov    cl, byte ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        mov    bl, 1
        shl    bl, cl
        cmp    cl, 5
        jb     loc_2_2375
        add    bl, bl
loc_2_2375:
        push   6
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   loc_2_2382
loc_2_2382:
        add    esp, 0xc
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_238F
        or     al, bl
        jmp    loc_2_2393
loc_2_238F:
        not    bl
        and    al, bl
loc_2_2393:
        push   eax
        push   6
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_00A7
loc_2_23A1:
        add    esp, 0x10
        pop    ebx
        pop    ebp
        ret    
sub_2_23A7:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        movzx  eax, word ptr [edi]
        mov    word ptr [edi + 2], dx
        cmp    edx, eax
        jle    loc_2_23BE
        mov    edx, eax
loc_2_23BE:
        add    edi, 4
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    
sub_2_23D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        VxDCall  CONFIGMG._CONFIGMG_Get_Version  ; cd2000003300
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x400
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret    
sub_2_23F3:
        cmp    dword ptr [esp + 8], 2
        push   esi
        push   edi
        jne    loc_2_2461
        mov    edi, 0x13
        push   1
        push   0x4d
        call   sub_2_2A6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_2_243A
        push   esi
        push   dword ptr [esp + 0x10]
        call   sub_2_26B9
        add    esp, 8
        test   eax, eax
        je     loc_2_243A
        push   3
        push   esi
        push   0                             ; -> loc_2_246D
        push   dword ptr [esp + 0x18]
        call   sub_2_2A64
        add    esp, 0x10
        mov    edi, eax
loc_2_243A:
        cmp    edi, 0x13
        jne    loc_2_2466
        test   esi, esi
        je     loc_2_2466
        lea    ecx, [esi + 0x4d]
        mov    eax, esi
        push   0
        push   ecx
        push   eax
        call   sub_2_2A44
        add    esp, 0xc
        push   0
        push   esi
        call   sub_2_2A5C
        add    esp, 8
        jmp    loc_2_2466
loc_2_2461:
        mov    edi, 1
loc_2_2466:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    8
loc_2_246D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        test   eax, eax
        push   ebx
        mov    dword ptr [ebp - 8], 0
        push   esi
        push   edi
        je     loc_2_24A4
        cmp    eax, 1
        je     loc_2_24B4
        cmp    eax, 4
        je     loc_2_2508
        cmp    eax, 8
        je     loc_2_252A
        mov    dword ptr [ebp - 8], 1
        jmp    loc_2_2644
loc_2_24A4:
        push   dword ptr [ebp + 0x14]
        call   sub_2_264E
        add    esp, 4
        jmp    loc_2_2644
loc_2_24B4:
        mov    esi, dword ptr [ebp + 0x14]
        push   esi
        call   sub_2_26EA
        add    esp, 4
        test   eax, eax
        jne    loc_2_24D0
        mov    dword ptr [ebp - 8], 0x13
        jmp    loc_2_2644
loc_2_24D0:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_2_2644
        push   esi
        call   sub_2_29EC
        add    esp, 4
        test   eax, eax
        je     loc_2_24F3
        mov    dword ptr [ebp - 8], 0x13
        jmp    loc_2_2644
loc_2_24F3:
        push   0
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_2_2A74
        add    esp, 0xc
        jmp    loc_2_2644
loc_2_2508:
        mov    esi, dword ptr [ebp + 0x14]
        push   0
        lea    eax, [esi + 0x4d]
        push   eax
        push   esi
        call   sub_2_2A3C
        add    esp, 0xc
        push   0
        push   esi
        call   sub_2_2A5C
        add    esp, 8
        jmp    loc_2_2644
loc_2_252A:
        and    byte ptr [ebp + 0xf], 0x7f
        cmp    dword ptr [ebp + 0xc], 7
        mov    esi, dword ptr [ebp + 0x14]
        je     loc_2_253D
        cmp    dword ptr [ebp + 0xc], 8
        jne    loc_2_258A
loc_2_253D:
        cmp    byte ptr [esi + 0x39], 1
        jne    loc_2_254C
        push   esi
        call   sub_2_008A
        add    esp, 4
loc_2_254C:
        mov    bl, byte ptr [esi + 0x28]
        mov    edi, dword ptr [esi + 0x18]
        push   0x10
        push   ebx
        push   edi
        call   sub_1_00C1
loc_2_255B:
        add    esp, 0xc
        test   ax, ax
        je     loc_2_2576
        push   0                             ; -> obj3:0x00D8
        push   ebx
        push   edi
        call   sub_2_29B6
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_258A
loc_2_2576:
        push   esi
        call   sub_2_1396
        add    esp, 4
        test   eax, eax
        jne    loc_2_258A
        mov    dword ptr [ebp - 8], 0x13
loc_2_258A:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_2644
        test   byte ptr [esi + 0xb], 2
        je     loc_2_2644
        cmp    dword ptr [ebp + 0xc], 4
        je     loc_2_2600
        cmp    dword ptr [ebp + 0xc], 5
        je     loc_2_2600
        cmp    dword ptr [ebp + 0xc], 6
        je     loc_2_25BA
        cmp    dword ptr [ebp + 0xc], 7
        jne    loc_2_2644
loc_2_25BA:
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi + 0x38]
        test   bl, bl
        je     loc_2_2644
loc_2_25C5:
        movzx  edi, byte ptr [ebp - 1]
        push   3
        mov    al, byte ptr [edi + esi + 0x28]
        push   eax
        push   dword ptr [esi + edi*4 + 0x18]
        call   loc_2_25D9
loc_2_25D9:
        add    esp, 0xc
        test   al, 0x80
        je     loc_2_25F6
        and    al, 0x7f
        mov    cl, byte ptr [edi + esi + 0x28]
        push   eax
        push   3
        push   ecx
        push   dword ptr [esi + edi*4 + 0x18]
        call   sub_1_00A7
loc_2_25F3:
        add    esp, 0x10
loc_2_25F6:
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     loc_2_25C5
        jmp    loc_2_2644
loc_2_2600:
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi + 0x38]
        test   bl, bl
        je     loc_2_2644
loc_2_260B:
        movzx  edi, byte ptr [ebp - 1]
        push   3
        mov    al, byte ptr [edi + esi + 0x28]
        push   eax
        push   dword ptr [esi + edi*4 + 0x18]
        call   loc_2_261F
loc_2_261F:
        add    esp, 0xc
        test   al, 0x20
        je     loc_2_263C
        or     al, 0x80
        mov    cl, byte ptr [edi + esi + 0x28]
        push   eax
        push   3
        push   ecx
        push   dword ptr [esi + edi*4 + 0x18]
        call   sub_1_00A7
loc_2_2639:
        add    esp, 0x10
loc_2_263C:
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     loc_2_260B
loc_2_2644:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_264E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_0B39
        mov    si, word ptr [edi + 0x30]
        add    esp, 4
        test   si, si
        je     loc_2_26B3
        push   1
        lea    ecx, [ebp - 4]
        push   dword ptr [edi]
        push   ecx
        call   sub_2_2A8C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_26B3
        xor    edi, edi
loc_2_2681:
        push   edi
        lea    ecx, [ebp - 8]
        push   edi
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_2A84
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_269F
        mov    eax, dword ptr [ebp - 8]
        and    word ptr [eax + 4], si
loc_2_269F:
        push   edi
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_2A7C
        add    esp, 0xc
        test   eax, eax
        je     loc_2_2681
loc_2_26B3:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_26B9:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0x10]
        lea    eax, [esi + 0x4d]
        push   eax
        push   esi
        call   sub_2_2A44
        add    esp, 0xc
        test   eax, eax
        mov    eax, 0
        je     loc_2_26E8
        mov    dword ptr [esi + 4], 0x5353534d
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        mov    eax, 1
loc_2_26E8:
        pop    esi
        ret    
sub_2_26EA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        push   2
        mov    edi, dword ptr [ebp + 8]
        lea    ecx, [ebp - 0x14]
        push   dword ptr [edi]
        push   ecx
        call   sub_2_2A8C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_28CB
        push   esi
        lea    ecx, [ebp - 0xc]
        push   esi
        push   4
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_2_2A84
        add    esp, 0x14
        test   eax, eax
        je     loc_2_2734
        cmp    eax, 0xf
        jne    loc_2_28CB
        test   eax, eax
        jne    loc_2_273F
loc_2_2734:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [edi + 0x3b], al
        jmp    loc_2_2743
loc_2_273F:
        mov    byte ptr [edi + 0x3b], 0
loc_2_2743:
        push   0
        lea    ecx, [ebp - 0xc]
        push   0
        push   2
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_2_2A84
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_28CB
        mov    byte ptr [edi + 0x38], 0
loc_2_2764:
        mov    eax, dword ptr [ebp - 0xc]
        xor    bl, bl
        mov    ax, word ptr [eax + 4]
        cmp    byte ptr [edi + 0x3a], bl
        mov    word ptr [ebp - 4], ax
        jne    loc_2_279C
        movzx  eax, ax
        push   0
        push   eax
        call   sub_2_12FD
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        push   dword ptr [ebp - 1]
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        je     loc_2_279C
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edi + 0x3a], al
loc_2_279C:
        mov    ecx, dword ptr [ebp - 0xc]
        movzx  eax, byte ptr [ecx + 0xa]
        movzx  ecx, word ptr [ebp - 4]
        shl    eax, 8
        push   eax
        push   ecx
        push   edi
        call   sub_2_28D4
        add    esp, 0xc
loc_2_27B5:
        movzx  eax, word ptr [ebp - 4]
        push   ebx
        push   eax
        call   sub_2_12FD
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        push   dword ptr [ebp - 1]
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        je     loc_2_2807
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [edi + 0x3a], al
        jne    loc_2_2807
        movzx  eax, word ptr [ebp - 4]
        push   ebx
        push   eax
        push   edi
        call   sub_2_291B
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2807
        movzx  ecx, byte ptr [edi + 0x38]
        movzx  eax, word ptr [ebp - 4]
        mov    dword ptr [edi + ecx*4 + 0x18], eax
        movzx  edx, byte ptr [edi + 0x38]
        mov    byte ptr [edx + edi + 0x28], bl
        inc    byte ptr [edi + 0x38]
loc_2_2807:
        add    bl, 0x40
        cmp    byte ptr [edi + 0x38], 4
        jae    loc_2_2837
        test   bl, bl
        jne    loc_2_27B5
        cmp    byte ptr [edi + 0x38], 4
        jae    loc_2_2837
        push   0
        lea    ecx, [ebp - 0xc]
        push   0
        push   2
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_2A84
        add    esp, 0x14
        test   eax, eax
        je     loc_2_2764
loc_2_2837:
        cmp    byte ptr [edi + 0x38], 0
        je     loc_2_28CB
        push   1
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 0x1c]
        mov    dword ptr [ebp - 0x10], 4
        push   ecx
        push   3
        push   dword ptr [0]                 ; -> obj3:0x000C
        push   0
        push   dword ptr [edi]
        call   sub_2_2A34
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_287B
        cmp    dword ptr [ebp - 0x10], 4
        jne    loc_2_287B
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xffff
        or     dword ptr [edi + 8], eax
loc_2_287B:
        VxDCall  VPEND.Service_0000h             ; cd2000002600
        mov    word ptr [ebp - 6], ax
        cmp    word ptr [ebp - 6], 0
        je     loc_2_28A7
        lea    eax, [ebp - 0x18]
        push   eax
        VxDCall  VPEND.Service_000Eh             ; cd200e002600
        add    esp, 4
        or     eax, eax
        jne    loc_2_28A7
        test   byte ptr [ebp - 0x18], 0xc0
        je     loc_2_28A7
        or     byte ptr [edi + 0xb], 2
loc_2_28A7:
        push   edi
        call   sub_2_0B39
        add    esp, 4
        push   edi
        call   sub_2_1396
        add    esp, 4
        test   eax, eax
        je     loc_2_28CB
        push   edi
        mov    esi, 1
        call   sub_2_0C54
        add    esp, 4
loc_2_28CB:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_28D4:
        push   esi
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 8]
        test   edx, 0x10000000
        jne    loc_2_2914
        cmp    byte ptr [ecx + 0x3a], 0x84
        jne    loc_2_2919
        cmp    byte ptr [ecx + 0x38], 0
        jbe    loc_2_2901
        mov    esi, dword ptr [ecx + 0x18]
        sub    esi, dword ptr [esp + 0xc]
        cmp    esi, 0xffff8000
        je     loc_2_290B
loc_2_2901:
        cmp    dword ptr [esp + 0x10], 0x8000
        jne    loc_2_2919
loc_2_290B:
        or     edx, 0x10000000
        mov    dword ptr [ecx + 8], edx
loc_2_2914:
        mov    eax, 1
loc_2_2919:
        pop    esi
        ret    
sub_2_291B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0x10
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   loc_2_2931
loc_2_2931:
        xor    edi, edi
        add    esp, 0xc
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], al
        movzx  ebx, byte ptr [esi + 0x38]
        dec    ebx
        js     loc_2_29AD
loc_2_2943:
        test   edi, edi
        jne    loc_2_29AD
        push   0x10
        mov    al, byte ptr [esi + ebx + 0x28]
        push   eax
        push   dword ptr [esi + ebx*4 + 0x18]
        call   loc_2_2957
loc_2_2957:
        add    esp, 0xc
        cmp    al, byte ptr [ebp - 1]
        jne    loc_2_29AA
        mov    al, byte ptr [ebp - 1]
        not    al
        mov    byte ptr [ebp - 8], al
        push   dword ptr [ebp - 8]
        push   0x10
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_00A7
loc_2_2977:
        add    esp, 0x10
        mov    cl, byte ptr [esi + ebx + 0x28]
        push   0x10
        push   ecx
        push   dword ptr [esi + ebx*4 + 0x18]
        call   loc_2_298A
loc_2_298A:
        add    esp, 0xc
        cmp    al, byte ptr [ebp - 8]
        jne    loc_2_2997
        mov    edi, 1
loc_2_2997:
        push   dword ptr [ebp - 1]
        push   0x10
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_00A7
loc_2_29A7:
        add    esp, 0x10
loc_2_29AA:
        dec    ebx
        jns    loc_2_2943
loc_2_29AD:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_29B6:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0x14]
        cmp    byte ptr [esi], 0
        je     loc_2_29E7
loc_2_29C3:
        mov    al, byte ptr [esi]
        push   eax
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        call   loc_2_29D3
loc_2_29D3:
        add    esp, 0xc
        test   al, al
        jne    loc_2_29E2
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_2_29C3
        jmp    loc_2_29E7
loc_2_29E2:
        mov    edi, 1
loc_2_29E7:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_2_29EC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        sub    esp, 8
        mov    ebx, esp
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebx], 3
        mov    dword ptr [ebx + 2], esi
        lea    esi, [0]                      ; -> loc_1_0246
        xor    edx, edx
        mov    ecx, 6
        mov    eax, 0x32
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        add    esp, 8
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
        db  0CCh, 0CCh
sub_2_2A24:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_2_2A2C:
        VxDCall  CONFIGMG._CONFIGMG_Yield        ; cd2015803300
        int3   
        int3   
sub_2_2A34:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_2A3C:
        VMMCall  VMM.Service_8195h               ; cd2095810100
        int3   
        int3   
sub_2_2A44:
        VMMCall  VMM.Service_8194h               ; cd2094810100
        int3   
        int3   
sub_2_2A4C:
        VxDCall  VPICD.Service_8019h             ; cd2019800300
        int3   
        int3   
sub_2_2A54:
        VMMCall  VMM._lstrlen                    ; cd207c810100
        int3   
        int3   
sub_2_2A5C:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_2_2A64:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        int3   
        int3   
sub_2_2A6C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_2_2A74:
        VxDCall  CONFIGMG._CONFIGMG_Set_Device_Driver_Private_DWord ; cd2049803300
        int3   
        int3   
sub_2_2A7C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Log_Conf ; cd201b803300
        int3   
        int3   
sub_2_2A84:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_2_2A8C:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x256  RW-
        dd  obj3:0x01EC              ; fixup
        dd  obj3:0x0234              ; fixup
        dd  obj3:0x0240              ; fixup
        dd  obj3:0x024C              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 0E0h, 032h, 0E0h
        db  078h, 060h, 000h, 0E0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h
        db  000h, 0E0h, 032h, 0E0h, 078h, 060h, 021h, 0E0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 004h, 000h, 000h, 0E0h, 032h, 0E0h, 078h, 060h, 021h, 080h, 003h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 041h, 001h, 000h, 000h, 000h, 000h
        db  023h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 041h, 001h
        db  000h, 000h, 000h, 000h, 01Fh, 008h, 000h, 000h, 001h, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 010h
        db  000h, 000h, 0FFh, 0FFh, 0FEh, 000h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 010h
        db  000h, 000h, 072h, 008h, 01Fh, 008h, 000h, 000h, 001h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 010h
        db  000h, 000h, 0FFh, 0FFh, 0FEh, 000h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 010h
        db  000h, 000h, 072h, 008h, 01Fh, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 001h, 000h
        db  000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 010h, 000h, 042h, 052h, 062h, 072h
        db  0FFh, 032h, 02Ah, 022h, 00Ah, 0FFh, 0FFh, 0FFh, 002h, 005h, 007h, 000h, 000h, 000h, 000h, 000h
sub_3_00E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_00EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_00F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_019C:
        sahf   
        add    byte ptr [edx], bh
        add    dword ptr [ebx], edi
        push   es
        cmp    eax, 0xf3e01
        add    byte ptr [esi + 0x3b013a10], bl
        push   es
        cmp    eax, 0xf3e01
        add    byte ptr [edx + eax], dl
        inc    eax
        add    byte ptr [eax], al
loc_3_01B9:
        add    byte ptr [eax], ah
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 018h, 0FFh
        db  00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 020h, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 028h, 0FFh
        db  00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 030h, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 045h
        db  078h, 043h, 041h, 020h, 056h, 031h, 02Eh, 031h, 030h, 02Eh, 020h, 028h, 043h, 029h, 020h, 043h
        db  06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 04Dh, 049h, 043h, 052h, 04Fh, 053h, 04Fh
        db  046h, 054h, 020h, 043h, 06Fh, 072h, 070h, 02Eh, 02Ch, 020h, 031h, 039h, 039h, 035h, 02Eh, 020h
        db  041h, 06Ch, 06Ch, 020h, 052h, 069h, 067h, 068h, 074h, 073h, 020h, 052h, 065h, 073h, 065h, 072h
        db  076h, 065h, 064h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 043h, 049h, 052h, 051h, 04Dh, 061h
        db  070h, 000h, 000h, 050h, 043h, 049h, 043h, 04Fh, 070h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 050h
        db  043h, 049h, 043h, 046h, 06Ch, 061h, 067h, 073h, 000h
VxD__LDATA ENDS
