; CBSS -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 3426 instructions recovered by recursive descent

Declare_Virtual_Device CBSS, 4, 0, CBSS_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1ac  R-X
        dd  loc_2_1144               ; fixup
        dd  loc_2_1162               ; fixup
        dd  loc_2_11AA               ; fixup
        dd  loc_2_11C7               ; fixup
        dd  loc_2_1229               ; fixup
        dd  loc_2_1290               ; fixup
        dd  loc_2_13B7               ; fixup
        dd  loc_2_1595               ; fixup
        dd  loc_2_162A               ; fixup
        dd  loc_2_16C6               ; fixup
        dd  loc_2_172E               ; fixup
        dd  loc_2_17B4               ; fixup
        dd  loc_2_1AA9               ; fixup
        dd  loc_2_1BEA               ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'CBSS    '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  CBSS_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 053h
        db  081h, 078h, 004h, 043h, 042h, 053h, 053h, 074h, 009h, 08Bh, 045h, 00Ch, 0C6h, 040h, 01Dh, 001h
        db  0EBh, 065h, 08Dh, 048h, 025h, 08Bh, 050h, 018h, 08Ah, 09Ah, 004h, 008h, 000h, 000h, 080h, 0E3h
        db  00Fh, 008h, 019h, 08Bh, 01Ah, 080h, 0E3h, 006h, 080h, 0FBh, 006h, 074h, 005h, 0F6h, 002h, 006h
        db  074h, 003h, 080h, 009h, 008h, 0F6h, 040h, 00Bh, 004h, 0C7h, 002h, 00Fh, 000h, 000h, 000h, 074h
        db  007h, 0C7h, 042h, 004h, 006h, 000h, 000h, 000h, 08Ah, 001h, 084h, 082h, 005h, 008h, 000h, 000h
        db  075h, 018h, 0A8h, 008h, 074h, 009h, 08Bh, 042h, 004h, 024h, 006h, 03Ch, 006h, 074h, 00Bh, 08Bh
        db  045h, 00Ch, 066h, 0C7h, 040h, 018h, 000h, 000h, 0EBh, 009h, 08Bh, 045h, 00Ch, 066h, 0C7h, 040h
        db  018h, 001h, 000h, 0C6h, 040h, 01Dh, 000h, 080h, 078h, 01Dh, 000h, 075h, 006h, 080h, 060h, 020h
        db  0FEh, 0EBh, 004h, 080h, 048h, 020h, 001h, 05Bh, 05Dh, 0C3h, 0CCh, 0CCh
CBSS_Control:
        cmp    eax, 0x22
        jne    loc_1_012A
        push   edx
        push   ebx
        call   sub_2_1EB0
loc_1_0128:
        stc    
        ret    
loc_1_012A:
        cmp    eax, 0x1b
        jne    loc_1_0138
        call   sub_2_1E93
loc_1_0134:
        cmp    eax, 1
        ret    
loc_1_0138:
        clc    
        ret    
loc_1_013A:
        pushfd 
        pushal 
        mov    ebp, esp
        cmp    ah, 0x9e
        jne    loc_1_014E
        push   ebp
        push   esi
        call   0x88
        pop    ecx
        pop    ecx
        jmp    loc_1_01A9
loc_1_014E:
        cmp    dword ptr [esi + 4], 0x53534243
        je     loc_1_0161
        mov    byte ptr [ebp + 0x1d], 1
loc_1_015B:
        or     dword ptr [ebp + 0x20], 1
        jmp    loc_1_01A9
loc_1_0161:
        bts    dword ptr [esi + 8], 0x1f
        jae    loc_1_016E
        mov    byte ptr [ebp + 0x1d], 0x18
        jmp    loc_1_015B
loc_1_016E:
        sub    ah, 0x83
        jae    loc_1_017D
loc_1_0173:
        mov    byte ptr [ebp + 0x1d], 0x15
        or     dword ptr [ebp + 0x20], 1
        jmp    loc_1_01A2
loc_1_017D:
        cmp    ah, 0xe
        jbe    loc_1_0192
        cmp    ah, 0x1a
        jne    loc_1_0173
        push   ebp
        push   esi
        call   sub_2_1D07
loc_1_018E:
        pop    ecx
        pop    ecx
        jmp    loc_1_01A2
loc_1_0192:
        movzx  eax, ah
        mov    eax, dword ptr [eax*4]        ; -> obj1:0x0000
        push   ebp
        push   esi
        call   eax
        pop    ecx
        pop    ecx
loc_1_01A2:
        and    dword ptr [esi + 8], 0x7fffffff
loc_1_01A9:
        popal  
        popfd  
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2212  R-X
loc_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x22], 0xdea0
        cmp    byte ptr [esi + 0x24], 1
        jne    loc_2_0038
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   8
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 0x18
        cmp    byte ptr [ebp - 1], 4
        jae    loc_2_0038
        or     byte ptr [esi + 0xa], 1
loc_2_0038:
        mov    edi, dword ptr [esi]
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   0xc
        push   ebx
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        cmp    byte ptr [ebp - 1], bl
        jne    loc_2_006C
        push   ebx
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 8
        push   1
        push   eax
        push   0xc
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
loc_2_006C:
        push   0
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 0xc0
        push   1
        push   eax
        push   0x90
        push   1
        push   edi
        call   sub_2_21A4
        mov    byte ptr [ebp - 1], 0x28
        add    esp, 0x18
        test   byte ptr [esi + 0xa], 1
        jne    loc_2_0096
        mov    byte ptr [ebp - 1], 0x38
loc_2_0096:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   0x91
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        mov    ecx, dword ptr [esi + 0x18]
        lea    eax, [ebp - 1]
        mov    ebx, 0x92
        or     byte ptr [ecx + 0x803], 0x10
        push   0
        push   1
        push   eax
        push   ebx
        push   0
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        mov    al, byte ptr [ebp - 1]
        and    al, 6
        jne    loc_2_00F2
        push   0
        lea    eax, [ebp - 1]
        or     byte ptr [ebp - 1], 2
        push   1
        push   eax
        push   ebx
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        jmp    loc_2_0101
loc_2_00F2:
        cmp    al, 6
        jne    loc_2_0101
        mov    eax, dword ptr [esi + 8]
        or     eax, 0x20000
        mov    dword ptr [esi + 8], eax
loc_2_0101:
        mov    eax, dword ptr [esi + 0x18]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 0x840], 0
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0112:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x18]
        test   byte ptr [esi + 0xa], 1
        mov    ecx, dword ptr [eax + 8]
        jne    loc_2_01CC
        test   cl, 0x10
        je     loc_2_01CC
        cmp    byte ptr [ebp + 0xc], 2
        jne    loc_2_0148
        mov    dword ptr [ebp - 4], 0x400
        jmp    loc_2_0177
loc_2_0148:
        cmp    byte ptr [ebp + 0xc], 3
        jne    loc_2_0157
        mov    dword ptr [ebp - 4], 0x800
        jmp    loc_2_0177
loc_2_0157:
        cmp    byte ptr [ebp + 0xc], 4
        jne    loc_2_0166
        mov    dword ptr [ebp - 4], 0x1000
        jmp    loc_2_0177
loc_2_0166:
        mov    al, byte ptr [ebp + 0xc]
        sub    al, 5
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0x2000
        mov    dword ptr [ebp - 4], eax
loc_2_0177:
        and    ecx, 0x3c00
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_01CC
        test   dword ptr [ebp - 4], ecx
        jne    loc_2_01CC
        mov    eax, ecx
        not    eax
        inc    eax
        and    eax, ecx
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_2_01CC
        mov    edi, dword ptr [esi]
        push   0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, 0x80
        push   ebx
        push   0
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        lea    eax, [ebp - 4]
        or     byte ptr [ebp - 2], 0x20
        push   0
        push   4
        push   eax
        push   ebx
        push   1
        mov    ebx, 1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
loc_2_01CC:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_2_0D62
        add    esp, 0x10
        mov    edi, eax
        test   ebx, ebx
        je     loc_2_0217
        mov    esi, dword ptr [esi]
        push   0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, 0x80
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        lea    eax, [ebp - 4]
        and    byte ptr [ebp - 2], 0xdf
        push   0
        push   4
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
loc_2_0217:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0220:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    ecx, [ebp - 1]
        push   esi
        mov    ebx, 0x91
        mov    esi, dword ptr [eax]
        push   0
        push   1
        push   ecx
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        lea    eax, [ebp - 1]
        or     byte ptr [ebp - 1], 0x40
        push   0
        push   1
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0265:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    ecx, [ebp - 1]
        push   esi
        mov    ebx, 0x91
        mov    esi, dword ptr [eax]
        push   0
        push   1
        push   ecx
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        lea    eax, [ebp - 1]
        and    byte ptr [ebp - 1], 0xbf
        push   0
        push   1
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_02AA:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x18]
        mov    bl, byte ptr [esp + 0x14]
        shl    bl, 4
        lea    ecx, [esi + 4]
        lea    eax, [esi + 0x805]
        mov    edi, dword ptr [ecx]
        mov    dl, byte ptr [eax]
        mov    dword ptr [ecx], 1
        mov    byte ptr [eax], bl
        mov    dword ptr [esi + 0xc], 0x4001
        mov    byte ptr [eax], dl
        test   byte ptr [esi + 0x804], 0
        mov    dword ptr [esi], 0xf
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_02EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   2
        push   eax
        push   0x3e
        mov    esi, dword ptr [edi]
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        lea    ecx, [ebp - 2]
        and    word ptr [ebp - 2], 0xf7ff
        push   0
        push   2
        push   ecx
        push   0x3e
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        mov    ecx, dword ptr [edi + 0x18]
        lea    eax, [ecx + 0x816]
        mov    dl, byte ptr [eax]
        or     dl, 0x10
        mov    byte ptr [eax], dl
        test   byte ptr [ecx + 0x93a], 0x18
        je     loc_2_034E
        add    ecx, 0x925
        mov    al, byte ptr [ecx]
        or     al, 0x80
        mov    byte ptr [ecx], al
loc_2_034E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0354:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [esi + 8]
        and    cl, 0x30
        cmp    cl, 0x20
        jne    loc_2_0389
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_0D62
        add    esp, 0x10
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_0439
loc_2_0389:
        lea    edi, [esi + 0x802]
        add    esi, 0x816
        mov    dword ptr [ebp - 0xc], 1
        mov    dl, byte ptr [edi]
        mov    bl, 0
        cmp    byte ptr [ebp + 0xc], 1
        adc    bl, 0xff
        mov    al, byte ptr [esi]
        and    bl, 0x10
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp + 0xc]
        sub    al, 3
        cmp    al, 1
        sbb    cl, cl
        and    cl, 2
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_2_03CE
        mov    al, byte ptr [ebp + 0xc]
        cmp    byte ptr [ebp + 0x10], al
        jne    loc_2_03CE
        or     bl, 1
        jmp    loc_2_03E6
loc_2_03CE:
        cmp    byte ptr [ebp + 0x10], 1
        jne    loc_2_03D9
        or     bl, 2
        jmp    loc_2_03E6
loc_2_03D9:
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_2_03E6
        mov    dword ptr [ebp - 0xc], 0
loc_2_03E6:
        cmp    dword ptr [ebp - 0xc], 1
        jne    loc_2_0439
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 8], dl
        and    byte ptr [ebp - 8], 0xa0
        or     bl, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        and    byte ptr [ebp - 8], 0xfd
        or     cl, byte ptr [ebp - 8]
        test   bl, 0x10
        jne    loc_2_040B
        and    bl, 0x5f
loc_2_040B:
        cmp    dl, bl
        jne    loc_2_0414
        cmp    byte ptr [ebp - 1], cl
        je     loc_2_0439
loc_2_0414:
        push   0
        mov    byte ptr [esi], cl
        push   0x493e0
        mov    byte ptr [edi], bl
        call   sub_2_21AC
        add    esp, 8
        test   bl, 0x10
        je     loc_2_0439
        mov    al, bl
        and    al, 0xa0
        cmp    al, 0xa0
        je     loc_2_0439
        or     bl, 0xa0
        mov    byte ptr [edi], bl
loc_2_0439:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0443:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx + 0x18]
        mov    ecx, dword ptr [eax + 8]
        and    cl, 0x30
        cmp    cl, 0x20
        jne    loc_2_046C
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_2_0DD9
        add    esp, 0x10
        jmp    loc_2_04BE
loc_2_046C:
        mov    dl, byte ptr [eax + 0x802]
        mov    al, byte ptr [eax + 0x816]
        test   dl, 0x10
        jne    loc_2_0485
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 0
        jmp    loc_2_0493
loc_2_0485:
        and    al, 2
        cmp    al, 1
        mov    eax, dword ptr [ebp + 0xc]
        sbb    cl, cl
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_0493:
        and    dl, 3
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dl, 1
        mov    byte ptr [ecx], dl
        jne    loc_2_04A6
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    loc_2_04B7
loc_2_04A6:
        cmp    dl, 2
        jne    loc_2_04B0
        mov    byte ptr [ecx], 1
        jmp    loc_2_04B7
loc_2_04B0:
        test   dl, dl
        je     loc_2_04B7
        mov    byte ptr [ecx], 0
loc_2_04B7:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [eax], cl
loc_2_04BE:
        pop    ebp
        ret    
loc_2_04C0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x18]
        add    eax, 0x816
        mov    cl, byte ptr [eax]
        or     cl, 1
        mov    byte ptr [eax], cl
        ret    
loc_2_04D4:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x18]
        add    eax, 0x816
        mov    cl, byte ptr [eax]
        and    cl, 0xfe
        mov    byte ptr [eax], cl
        ret    
loc_2_04E8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        or     byte ptr [ecx + 0x83e], 1
        ret    
loc_2_04F7:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [edi + 8]
        and    cl, 0x30
        cmp    cl, 0x20
        jne    loc_2_0528
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_0D62
        add    esp, 0x10
        mov    esi, eax
        jmp    loc_2_05AA
loc_2_0528:
        mov    esi, 1
        add    edi, 0x802
        mov    cl, byte ptr [ebp + 0xc]
        mov    bl, 0
        mov    al, byte ptr [edi]
        cmp    cl, 1
        adc    bl, 0xff
        and    bl, 0x10
        cmp    cl, 3
        jne    loc_2_054B
        or     bl, 8
loc_2_054B:
        test   cl, cl
        je     loc_2_0559
        cmp    byte ptr [ebp + 0x10], cl
        jne    loc_2_0559
        or     bl, 1
        jmp    loc_2_056C
loc_2_0559:
        cmp    byte ptr [ebp + 0x10], 1
        jne    loc_2_0564
        or     bl, 2
        jmp    loc_2_056C
loc_2_0564:
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_2_056C
        xor    esi, esi
loc_2_056C:
        cmp    esi, 1
        jne    loc_2_05AA
        mov    cl, al
        and    cl, 0xa0
        or     bl, cl
        test   bl, 0x10
        jne    loc_2_0580
        and    bl, 0x5f
loc_2_0580:
        or     bl, 0x40
        cmp    al, bl
        je     loc_2_05AA
        push   0
        mov    byte ptr [edi], bl
        push   0x493e0
        call   sub_2_21AC
        add    esp, 8
        test   bl, 0x10
        je     loc_2_05AA
        mov    al, bl
        and    al, 0xa0
        cmp    al, 0xa0
        je     loc_2_05AA
        or     bl, 0xa0
        mov    byte ptr [edi], bl
loc_2_05AA:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_05B1:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx + 0x18]
        mov    ecx, dword ptr [eax + 8]
        and    cl, 0x30
        cmp    cl, 0x20
        jne    loc_2_05DA
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_2_0DD9
        add    esp, 0x10
        jmp    loc_2_0628
loc_2_05DA:
        mov    dl, byte ptr [eax + 0x802]
        test   dl, 0x10
        jne    loc_2_05ED
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 0
        jmp    loc_2_05FD
loc_2_05ED:
        mov    al, dl
        and    al, 8
        cmp    al, 1
        mov    eax, dword ptr [ebp + 0xc]
        sbb    cl, cl
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_05FD:
        and    dl, 3
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dl, 1
        mov    byte ptr [ecx], dl
        jne    loc_2_0610
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    loc_2_0621
loc_2_0610:
        cmp    dl, 2
        jne    loc_2_061A
        mov    byte ptr [ecx], 1
        jmp    loc_2_0621
loc_2_061A:
        test   dl, dl
        je     loc_2_0621
        mov    byte ptr [ecx], 0
loc_2_0621:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [eax], cl
loc_2_0628:
        pop    ebp
        ret    
loc_2_062A:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x18]
        mov    byte ptr [eax + 0x83c], 3
        or     byte ptr [eax + 0x816], 2
        ret    
loc_2_0640:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x18]
        mov    byte ptr [eax + 0x83c], 0
        and    byte ptr [eax + 0x816], 0xfd
        ret    
loc_2_0656:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        or     byte ptr [ecx + 0x803], 0x10
        ret    
loc_2_0665:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        mov    byte ptr [ecx + 0x83b], 3
        ret    
loc_2_0674:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        mov    byte ptr [ecx + 0x83b], 0
        ret    
loc_2_0683:
        sub    esp, 4
        lea    eax, [esp + 3]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   0
        push   1
        push   eax
        push   0x50
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 0x18
        test   byte ptr [esp + 7], 0x80
        jne    loc_2_06B0
        mov    dword ptr [esi + 0x10], 0     ; -> obj3:0x0024
loc_2_06B0:
        pop    esi
        add    esp, 4
        ret    
loc_2_06B5:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0x18]
        mov    eax, dword ptr [esi + 8]
        and    al, 0x30
        cmp    al, 0x10
        jne    loc_2_0746
        mov    edi, 1
        add    esi, 0x802
        mov    al, byte ptr [ebp + 0xc]
        mov    bl, 0x10
        mov    cl, byte ptr [esi]
        cmp    al, 2
        je     loc_2_06EC
        lea    edx, [eax - 3]
        cmp    dl, 1
        sbb    bl, bl
        and    bl, 8
loc_2_06EC:
        test   al, al
        je     loc_2_06FA
        cmp    byte ptr [ebp + 0x10], al
        jne    loc_2_06FA
        or     bl, 1
        jmp    loc_2_070D
loc_2_06FA:
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_2_070D
        cmp    byte ptr [ebp + 0x10], 1
        jne    loc_2_070B
        or     bl, 2
        jmp    loc_2_070D
loc_2_070B:
        xor    edi, edi
loc_2_070D:
        cmp    edi, 1
        jne    loc_2_075A
        mov    al, cl
        and    al, 0x80
        or     bl, al
        test   bl, 0x18
        jne    loc_2_0720
        and    bl, 0x7f
loc_2_0720:
        cmp    bl, cl
        je     loc_2_075A
        push   0
        mov    byte ptr [esi], bl
        push   0x493e0
        call   sub_2_21AC
        add    esp, 8
        test   bl, 0x18
        je     loc_2_075A
        test   bl, 0x80
        jne    loc_2_075A
        or     bl, 0x80
        mov    byte ptr [esi], bl
        jmp    loc_2_075A
loc_2_0746:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_2_0D62
        add    esp, 0x10
        mov    edi, eax
loc_2_075A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_2_0761:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx + 0x18]
        mov    ecx, dword ptr [eax + 8]
        and    cl, 0x30
        cmp    cl, 0x10
        jne    loc_2_07C2
        mov    dl, byte ptr [eax + 0x802]
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, dl
        and    cl, 0x18
        mov    byte ptr [eax], cl
        cmp    cl, 0x10
        jne    loc_2_0790
        mov    byte ptr [eax], 2
        jmp    loc_2_079B
loc_2_0790:
        cmp    cl, 8
        mov    byte ptr [eax], 3
        je     loc_2_079B
        mov    byte ptr [eax], 0
loc_2_079B:
        and    dl, 3
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dl, 1
        mov    byte ptr [ecx], dl
        jne    loc_2_07AE
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    loc_2_07B9
loc_2_07AE:
        cmp    dl, 2
        mov    byte ptr [ecx], 1
        je     loc_2_07B9
        mov    byte ptr [ecx], 0
loc_2_07B9:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [eax], cl
        jmp    loc_2_07D1
loc_2_07C2:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_2_0DD9
loc_2_07D1:
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_07D5:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        or     byte ptr [ecx + 0x81e], 0x20
        ret    
loc_2_07E4:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        and    byte ptr [ecx + 0x81e], 0xdf
        ret    
loc_2_07F3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    ecx, [ebp - 2]
        push   esi
        mov    ebx, 0x84
        mov    esi, dword ptr [eax]
        push   0
        push   2
        push   ecx
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        lea    eax, [ebp - 2]
        or     byte ptr [ebp - 2], 0x30
        push   0
        push   2
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 0x18
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0838:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        or     byte ptr [ecx + 0x82f], 8
        ret    
loc_2_0847:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        and    byte ptr [ecx + 0x82f], 0xf7
        ret    
sub_2_0856:
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
        jne    loc_2_0925
        or     eax, ecx
        lea    edi, [esi + 0x20]
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
        call   sub_2_21B4
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_08CF
        cmp    dword ptr [ebp - 4], 0x10
        jne    loc_2_08CF
        xor    cl, cl
loc_2_08AF:
        movzx  edx, cl
        mov    al, byte ptr [ebp + edx - 0x18]
        mov    byte ptr [edx + esi + 0x28], al
        test   al, al
        je     loc_2_08C8
        mov    ax, 1
        shl    ax, cl
        or     word ptr [edi], ax
loc_2_08C8:
        inc    cl
        cmp    cl, 0x10
        jb     loc_2_08AF
loc_2_08CF:
        xor    ebx, ebx
        cmp    word ptr [edi], bx
        jne    loc_2_0925
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
        call   sub_2_21B4
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_0925
        cmp    dword ptr [ebp - 8], ebx
        je     loc_2_0925
        mov    eax, dword ptr [ebp - 8]
        xor    cl, cl
        mov    word ptr [edi], ax
loc_2_090B:
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        je     loc_2_091E
        movzx  eax, cl
        mov    byte ptr [eax + esi + 0x28], cl
loc_2_091E:
        inc    cl
        cmp    cl, 0x10
        jb     loc_2_090B
loc_2_0925:
        mov    ax, word ptr [esi + 0x20]
        test   ax, ax
        jne    loc_2_0937
        push   esi
        call   sub_2_0942
        add    esp, 4
loc_2_0937:
        or     word ptr [esi + 0x1c], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0942:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x24], 4
        jne    loc_2_0983
        mov    ebx, dword ptr [edi]
        push   esi
        push   2
        lea    eax, [ebp - 2]
        push   eax
        push   0x3e
        push   esi
        push   ebx
        call   sub_2_21A4
        add    esp, 0x18
        lea    ecx, [ebp - 2]
        or     byte ptr [ebp - 1], 8
        push   esi
        push   2
        push   ecx
        push   0x3e
        push   1
        push   ebx
        call   sub_2_21A4
        add    esp, 0x18
loc_2_0983:
        push   0
        push   0                             ; -> sub_2_0B9D
        push   0                             ; -> sub_2_0942
        call   sub_2_21CC
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0A29
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
loc_2_09B0:
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        test   word ptr [edi + 0x22], ax
        je     loc_2_09FA
        test   word ptr [edi + 0x1e], ax
        jne    loc_2_09FA
        push   ebx
        push   edi
        call   sub_2_0A6A
        mov    word ptr [ebp - 6], ax
        add    esp, 8
        test   ax, ax
        je     loc_2_09FA
        bsf    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 4], ax
        movzx  eax, word ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 4]
        mov    byte ptr [eax + edi + 0x28], bl
        mov    ax, 1
        shl    ax, cl
        or     si, ax
        or     word ptr [edi + 0x1e], ax
loc_2_09FA:
        inc    bl
        cmp    bl, 0x10
        jb     loc_2_09B0
        movzx  eax, si
        push   eax
        call   sub_2_21C4
        add    esp, 4
        pop    eax
        out    0x21, al
        mov    al, ah
        out    0xa1, al
        popfd  
        push   0
        push   0                             ; -> sub_2_0B9D
        push   0                             ; -> sub_2_0942
        call   sub_2_21BC
        add    esp, 0xc
loc_2_0A29:
        cmp    byte ptr [edi + 0x24], 4
        jne    loc_2_0A60
        mov    edi, dword ptr [edi]
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 2]
        push   2
        push   eax
        push   0x3e
        push   ebx
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        lea    ecx, [ebp - 2]
        and    word ptr [ebp - 2], 0xf7ff
        push   ebx
        push   2
        push   ecx
        push   0x3e
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
loc_2_0A60:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0A6A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 0x18
        sub    esp, 0x14
        mov    dword ptr [ebp - 0xc], eax
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [eax]
        mov    ecx, dword ptr [edi + 4]
        lea    esi, [edi + 0x805]
        mov    dword ptr [ebp - 0x14], ecx
        mov    al, byte ptr [esi]
        lea    ecx, [edi + 0x803]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp + 0xc]
        mov    dl, byte ptr [ecx]
        shl    al, 4
        mov    byte ptr [ebp - 1], dl
        or     al, 8
        mov    byte ptr [ecx], 0x40
        mov    byte ptr [ebp - 0x10], al
        mov    byte ptr [esi], al
        mov    dword ptr [edi + 4], 6
        test   byte ptr [edi + 0x804], bl
        mov    dword ptr [edi], 0xf
        call   sub_2_0B7E
        cmp    byte ptr [edi + 0x800], 0x82
        mov    word ptr [ebp - 4], ax
        je     loc_2_0B05
        mov    byte ptr [edi + 0x816], 0x20
        call   sub_2_0B7E
        test   byte ptr [edi + 0x804], bl
        mov    word ptr [ebp - 6], ax
        mov    dword ptr [edi], 0xf
        call   sub_2_0B7E
        mov    bx, word ptr [ebp - 6]
        xor    bx, ax
        mov    ax, word ptr [ebp - 6]
        xor    ax, word ptr [ebp - 4]
        and    bx, ax
loc_2_0B05:
        test   bx, bx
        jne    loc_2_0B57
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [esi], 0
        test   byte ptr [eax + 0xa], 2
        je     loc_2_0B22
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_2_02AA
        add    esp, 8
loc_2_0B22:
        call   sub_2_0B7E
        mov    si, ax
        mov    dl, byte ptr [ebp - 0x10]
        mov    bx, si
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [eax]
        mov    byte ptr [ecx + 0x805], dl
        test   byte ptr [ecx + 0x804], 0
        mov    dword ptr [ecx], 0xf
        call   sub_2_0B7E
        xor    bx, ax
        xor    si, word ptr [ebp - 4]
        and    bx, si
loc_2_0B57:
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [eax]
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 4], edx
        mov    dl, byte ptr [ebp - 1]
        mov    byte ptr [ecx + 0x805], al
        mov    ax, bx
        mov    byte ptr [ecx + 0x803], dl
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0B7E:
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
sub_2_0B9D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    byte ptr [ebp - 3], 0
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        lea    ecx, [ebp - 5]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], 0
        push   0
        push   eax
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        push   ecx
        push   esi
        call   sub_3_00C0
        add    esp, 0x10
        mov    edi, eax
        cmp    edi, 1
        jne    loc_2_0CCC
        mov    al, byte ptr [ebp + 0xc]
        mov    cl, al
        mov    bl, al
        and    cl, 7
        and    bl, 0x10
        mov    byte ptr [ebp - 2], cl
        je     loc_2_0BF7
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 4], al
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], al
        jmp    loc_2_0C20
loc_2_0BF7:
        test   al, 0x20
        jne    loc_2_0C0D
        cmp    byte ptr [ebp - 5], 2
        je     loc_2_0C0D
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        je     loc_2_0C0D
        xor    edi, edi
        jmp    loc_2_0C20
loc_2_0C0D:
        mov    al, byte ptr [ebp + 0x10]
        and    al, 0x70
        shr    al, 4
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ebp + 0x10]
        and    al, 7
        mov    byte ptr [ebp - 4], al
loc_2_0C20:
        cmp    edi, 1
        jne    loc_2_0CB9
        cmp    byte ptr [ebp - 2], 3
        ja     loc_2_0C3B
        cmp    byte ptr [ebp - 3], 3
        ja     loc_2_0C3B
        cmp    byte ptr [ebp - 4], 3
        jbe    loc_2_0C3D
loc_2_0C3B:
        xor    edi, edi
loc_2_0C3D:
        cmp    edi, 1
        jne    loc_2_0CCC
        movzx  eax, byte ptr [ebp - 4]
        movzx  ecx, byte ptr [ebp - 3]
        push   eax
        push   ecx
        movzx  edx, byte ptr [ebp - 2]
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        push   edx
        push   esi
        call   sub_3_00B8
        add    esp, 0x10
        mov    edi, eax
        cmp    edi, 1
        jne    loc_2_0CCC
        test   bl, bl
        je     loc_2_0CB9
        cmp    byte ptr [ebp - 2], 3
        jne    loc_2_0CB9
        cmp    byte ptr [ebp - 5], 1
        jne    loc_2_0CB9
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   0
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        push   esi
        call   sub_3_00C0
        add    esp, 0x10
        test   eax, eax
        je     loc_2_0CB9
        cmp    byte ptr [ebp - 1], 2
        jne    loc_2_0CB9
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        push   2
        push   2
        push   2
        push   esi
        call   sub_3_00B8
        add    esp, 0x10
        mov    edi, eax
loc_2_0CB9:
        cmp    edi, 1
        jne    loc_2_0CCC
        test   bl, bl
        je     loc_2_0CC8
        or     byte ptr [esi + 0xb], 0x20
        jmp    loc_2_0CCC
loc_2_0CC8:
        and    byte ptr [esi + 0xb], 0xdf
loc_2_0CCC:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0CD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 3]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        mov    edi, dword ptr [ebp + 0xc]
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        push   edi
        push   esi
        call   sub_3_00BC
        add    esp, 0x10
        mov    cl, byte ptr [ebp - 4]
        shl    cl, 4
        mov    eax, dword ptr [ebp + 0x10]
        or     cl, byte ptr [ebp - 3]
        push   0
        lea    edx, [ebp - 1]
        mov    byte ptr [eax], cl
        push   edx
        lea    ecx, [ebp - 2]
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        push   ecx
        push   esi
        call   sub_3_00C0
        add    esp, 0x10
        test   byte ptr [esi + 0xb], 0x20
        je     loc_2_0D31
        or     byte ptr [edi], 0x10
        jmp    loc_2_0D42
loc_2_0D31:
        cmp    byte ptr [ebp - 2], 2
        je     loc_2_0D42
        mov    al, byte ptr [edi]
        cmp    byte ptr [ebp - 1], al
        je     loc_2_0D42
        or     al, 0x20
        mov    byte ptr [edi], al
loc_2_0D42:
        cmp    byte ptr [ebp - 1], 2
        je     loc_2_0D5C
        cmp    byte ptr [ebp - 1], 3
        jne    loc_2_0D53
        or     byte ptr [edi], 0x40
        jmp    loc_2_0D5C
loc_2_0D53:
        cmp    byte ptr [ebp - 1], 4
        jne    loc_2_0D5C
        or     byte ptr [edi], 0x80
loc_2_0D5C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0D62:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        mov    ebx, 1
        push   edi
        mov    esi, dword ptr [eax + 0x18]
        movzx  edi, byte ptr [esp + 0x14]
        movzx  eax, byte ptr [esp + 0x18]
        shl    edi, 4
        mov    ecx, dword ptr [esi + 0x10]
        test   byte ptr [esi + 8], 0
        or     edi, eax
        mov    eax, ecx
        and    al, 0x88
        or     edi, eax
        cmp    edi, ecx
        je     loc_2_0DD3
        push   0
        mov    dword ptr [esi + 0x10], edi
        push   0x493e0
        call   sub_2_21AC
        add    esp, 8
        mov    eax, dword ptr [esi + 8]
        test   ah, 2
        je     loc_2_0DAF
        xor    ebx, ebx
        jmp    loc_2_0DD3
loc_2_0DAF:
        and    eax, 0x30
        cmp    eax, 0x10
        jne    loc_2_0DC8
        test   edi, 0x70
        je     loc_2_0DCC
        or     byte ptr [esi + 0x802], 0xa0
        jmp    loc_2_0DD3
loc_2_0DC8:
        test   eax, eax
        jne    loc_2_0DD3
loc_2_0DCC:
        and    byte ptr [esi + 0x802], 0x5f
loc_2_0DD3:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_0DD9:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [eax + 0x18]
        mov    edx, dword ptr [eax + 0x10]
        test   byte ptr [eax + 8], 0
        mov    ebx, edx
        mov    eax, dword ptr [ebp + 0xc]
        shr    ebx, 4
        and    dl, 7
        and    bl, 7
        mov    ecx, dword ptr [ebp + 0x10]
        mov    byte ptr [eax], bl
        cmp    bl, 3
        mov    byte ptr [ecx], dl
        ja     loc_2_0E0A
        cmp    bl, 1
        jne    loc_2_0E0D
loc_2_0E0A:
        mov    byte ptr [eax], 0
loc_2_0E0D:
        cmp    dl, 3
        jbe    loc_2_0E15
        mov    byte ptr [ecx], 0
loc_2_0E15:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebx
        pop    ebp
        mov    byte ptr [eax], cl
        ret    
sub_2_0E1F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        mov    ebx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 8]
        test   ebx, ebx
        je     loc_2_0E50
        mov    eax, dword ptr [edi + 0x18]
        push   0
        push   0x493e0
        mov    dword ptr [eax + 0xc], 0x4000
        call   sub_2_21AC
        add    esp, 8
loc_2_0E50:
        mov    eax, dword ptr [edi + 0x18]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, ecx
        and    eax, 0x30
        cmp    eax, 0x10
        jne    loc_2_0E66
        mov    byte ptr [ebp - 2], 1
        jmp    loc_2_0E8B
loc_2_0E66:
        cmp    eax, 0x20
        jne    loc_2_0E71
        mov    byte ptr [ebp - 2], 2
        jmp    loc_2_0E8B
loc_2_0E71:
        cmp    eax, 0x30
        jne    loc_2_0E87
        mov    al, byte ptr [edi + 0x24]
        cmp    al, 8
        je     loc_2_0E81
        cmp    al, 9
        jne    loc_2_0E87
loc_2_0E81:
        mov    byte ptr [ebp - 2], 2
        jmp    loc_2_0E8B
loc_2_0E87:
        mov    byte ptr [ebp - 2], 0
loc_2_0E8B:
        mov    eax, ecx
        and    eax, 0xf003ffff
        shr    eax, 0x12
        and    ecx, eax
        test   ch, 4
        je     loc_2_0EA2
        mov    byte ptr [ebp - 1], 2
        jmp    loc_2_0ECA
loc_2_0EA2:
        test   ch, 8
        je     loc_2_0EAD
        mov    byte ptr [ebp - 1], 3
        jmp    loc_2_0ECA
loc_2_0EAD:
        test   ch, 0x10
        je     loc_2_0EB8
        mov    byte ptr [ebp - 1], 4
        jmp    loc_2_0ECA
loc_2_0EB8:
        and    ecx, 0x2000
        mov    al, 0
        cmp    ecx, 1
        adc    al, 0xff
        and    al, 5
        mov    byte ptr [ebp - 1], al
loc_2_0ECA:
        cmp    byte ptr [ebp - 2], 0
        je     loc_2_0EF4
        cmp    byte ptr [ebp - 1], 0
        jne    loc_2_0EF4
        mov    esi, 0
        test   ebx, ebx
        jne    loc_2_0EF4
        push   1
        lea    eax, [ebp - 1]
        push   eax
        lea    ecx, [ebp - 2]
        push   ecx
        push   edi
        call   sub_2_0E1F
        add    esp, 0x10
        mov    esi, eax
loc_2_0EF4:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_2_0F00
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax], cl
loc_2_0F00:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_2_0F0C
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax], cl
loc_2_0F0C:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0F15:
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 4]
        shr    edx, 0x18
        mov    eax, dword ptr [eax + 0x18]
        movzx  ecx, byte ptr [esp + 8]
        mov    byte ptr [ecx + eax + 0x840], dl
        xor    al, al
        ret    
loc_2_0F32:
        mov    eax, dword ptr [esp + 4]
        movzx  ecx, byte ptr [esp + 8]
        mov    edx, dword ptr [eax + 0x18]
        movzx  eax, byte ptr [ecx + edx + 0x840]
        shl    eax, 0x18
        ret    
sub_2_0F4A:
        push   esi
        mov    eax, 1
        push   edi
        xor    dl, dl
        mov    edi, dword ptr [esp + 0xc]
loc_2_0F57:
        cmp    dl, 5
        jae    loc_2_0F86
        mov    esi, dword ptr [edi + 0x18]
        movzx  ecx, dl
        lea    esi, [ecx + esi + 0x840]
        mov    cl, byte ptr [esi]
        mov    byte ptr [esi], 0x55
        cmp    byte ptr [esi], 0x55
        jne    loc_2_0F7B
        mov    byte ptr [esi], 0xaa
        cmp    byte ptr [esi], 0xaa
        je     loc_2_0F7D
loc_2_0F7B:
        xor    eax, eax
loc_2_0F7D:
        inc    dl
        mov    byte ptr [esi], cl
        cmp    eax, 1
        je     loc_2_0F57
loc_2_0F86:
        pop    edi
        pop    esi
        ret    
sub_2_0F89:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        cmp    byte ptr [esi], 0
        je     loc_2_0FC7
        mov    eax, dword ptr [esp + 0x14]
loc_2_0F9A:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    cl, dl
        mov    bl, byte ptr [esi]
        inc    esi
        dec    dl
        test   cl, cl
        je     loc_2_0FC2
        mov    edi, dword ptr [eax + 0x18]
loc_2_0FAB:
        movzx  ebp, bl
        mov    cl, byte ptr [esi]
        inc    bl
        inc    esi
        mov    byte ptr [ebp + edi + 0x800], cl
        mov    cl, dl
        dec    dl
        test   cl, cl
        jne    loc_2_0FAB
loc_2_0FC2:
        cmp    byte ptr [esi], 0
        jne    loc_2_0F9A
loc_2_0FC7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_0FCC:
        xor    al, al
        mov    edx, dword ptr [esp + 4]
        cmp    dword ptr [0], 0              ; -> obj3:0x00B0
        je     loc_2_0FFA
loc_2_0FDB:
        movzx  ecx, al
        imul   ecx, ecx, 0x24
        cmp    dword ptr [ecx], edx          ; -> obj3:0x00B0
        je     loc_2_0FFA
        inc    al
        movzx  ecx, al
        imul   ecx, ecx, 0x24
        cmp    dword ptr [ecx], 0            ; -> obj3:0x00B0
        jne    loc_2_0FDB
loc_2_0FFA:
        movzx  ecx, al
        imul   ecx, ecx, 0x24
        cmp    dword ptr [ecx], edx          ; -> obj3:0x00B0
        je     loc_2_100A
        xor    al, al
loc_2_100A:
        ret    
sub_2_100B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        lea    eax, [ebp - 0xc]
        mov    word ptr [esi + 0x22], 0xdeb8
        push   edi
        push   4
        push   eax
        push   edi
        push   edi
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 0x18
        push   dword ptr [ebp - 0xc]
        call   sub_2_0FCC
        add    esp, 4
        mov    byte ptr [esi + 0x24], al
        movzx  eax, al
        imul   eax, eax, 0x24
        mov    dword ptr [esi + 0xc], 0      ; -> obj3:0x0010
        cmp    dword ptr [eax + 0x14], edi   ; -> loc_3_00C4
        je     loc_2_105E
        mov    dword ptr [esi + 0x14], 0     ; -> obj3:0x006C
        jmp    loc_2_109D
loc_2_105E:
        push   esi
        call   sub_2_0F4A
        add    esp, 4
        test   eax, eax
        je     loc_2_1096
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        mov    dword ptr [eax + 0x14], 0     ; -> loc_3_00C4
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        mov    dword ptr [eax + 0x18], 0     ; -> loc_3_00C8
        mov    dword ptr [esi + 0x14], 0     ; -> obj3:0x006C
        jmp    loc_2_109D
loc_2_1096:
        mov    dword ptr [esi + 0x14], 0     ; -> obj3:0x004C
loc_2_109D:
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        cmp    dword ptr [eax + 0x1c], 0     ; -> sub_3_00CC
        mov    dword ptr [esi + 0x10], 0     ; -> obj3:0x0024
        je     loc_2_10BB
        mov    dword ptr [esi + 0x10], 0     ; -> obj3:0x0038
loc_2_10BB:
        push   0                             ; -> loc_3_023C
        xor    ebx, ebx
        push   esi
        call   sub_2_0F89
        add    esp, 8
        mov    eax, dword ptr [esi + 0x18]
        mov    edi, dword ptr [esi]
        push   ebx
        push   2
        mov    dword ptr [eax + 0x10], ebx
        mov    dword ptr [eax + 4], ebx
        test   byte ptr [eax + 0x804], bl
        mov    dword ptr [eax], 0xf
        lea    eax, [ebp - 2]
        push   eax
        push   0x3e
        push   ebx
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        lea    ecx, [ebp - 2]
        or     word ptr [ebp - 2], 0x480
        push   ebx
        push   2
        push   ecx
        push   0x3e
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], ebx
        push   ebx
        push   4
        push   ecx
        push   0x44
        push   1
        push   edi
        call   sub_2_21A4
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        add    esp, 0x18
        mov    eax, dword ptr [eax + 4]      ; -> obj3:0x00B4
        test   eax, eax
        je     loc_2_113D
        push   esi
        call   eax
        add    esp, 4
loc_2_113D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1144:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    word ptr [eax + 0x10], 0x501
        mov    byte ptr [eax + 0x1c], cl
        mov    byte ptr [eax + 0x18], cl
        mov    byte ptr [eax + 0x19], 1
        and    byte ptr [eax + 0x20], 0xfe
        mov    byte ptr [eax + 0x1d], cl
        ret    
loc_2_1162:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
        mov    word ptr [esi + 0x18], bx
        push   edi
        mov    byte ptr [esi + 0x10], 1
        mov    byte ptr [esi + 0x11], 7
        mov    word ptr [esi + 0x14], bx
        call   sub_2_0856
        add    esp, 4
        mov    eax, dword ptr [edi + 0xc]
        movzx  ecx, word ptr [edi + 0x1c]
        push   0x14
        mov    dword ptr [eax + 2], ecx
        push   eax
        push   dword ptr [esi]
        call   sub_2_1E64
        add    esp, 0xc
        mov    byte ptr [esi + 0x1d], bl
        and    byte ptr [esi + 0x20], 0xfe
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_11AA:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ax, byte ptr [edx + 0x26]
        mov    word ptr [ecx], ax
        mov    byte ptr [ecx + 0x15], 0
        mov    byte ptr [ecx + 0x1d], 0
        and    byte ptr [ecx + 0x20], 0xfe
        ret    
loc_2_11C7:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    ax, word ptr [esi]
        test   al, 0x20
        jne    loc_2_1212
        test   al, 0x40
        je     loc_2_1212
        mov    edx, dword ptr [esp + 8]
        mov    cl, byte ptr [edx + 0x26]
        xor    cl, al
        test   cl, 0x1f
        jne    loc_2_1212
        mov    byte ptr [edx + 0x26], al
        test   al, 0x80
        je     loc_2_11FC
        mov    eax, dword ptr [edx + 0x18]
        or     byte ptr [edx + 0xb], 4
        mov    dword ptr [eax + 4], 6
        jmp    loc_2_120A
loc_2_11FC:
        mov    ecx, dword ptr [edx + 0x18]
        and    byte ptr [edx + 0xb], 0xfb
        mov    dword ptr [ecx + 4], 0
loc_2_120A:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_1218
loc_2_1212:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 6
loc_2_1218:
        cmp    byte ptr [eax], 0
        jne    loc_2_1223
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_1227
loc_2_1223:
        or     byte ptr [esi + 0x20], 1
loc_2_1227:
        pop    esi
        ret    
loc_2_1229:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0x11], 0xff
        jne    loc_2_1279
        mov    al, byte ptr [esi + 0x15]
        cmp    al, 7
        jae    loc_2_1279
        cmp    al, 5
        jb     loc_2_124F
        mov    eax, 0                        ; -> obj3:0x008C
        mov    ecx, 0x18
        mov    byte ptr [esi + 0x10], 0x84
        jmp    loc_2_125F
loc_2_124F:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, 0x20
        mov    byte ptr [esi + 0x10], 0x83
        mov    eax, dword ptr [eax + 0x14]
loc_2_125F:
        mov    word ptr [esi + 0x18], 1
        push   ecx
        push   eax
        push   dword ptr [esi]
        call   sub_2_1E64
        add    esp, 0xc
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_127F
loc_2_1279:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x11
loc_2_127F:
        cmp    byte ptr [eax], 0
        jne    loc_2_128A
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_128E
loc_2_128A:
        or     byte ptr [esi + 0x20], 1
loc_2_128E:
        pop    esi
        ret    
loc_2_1290:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    cl, byte ptr [esi + 0x11]
        cmp    cl, 7
        jb     loc_2_12AB
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x11
        jmp    loc_2_13A3
loc_2_12AB:
        lea    edi, [esi + 0x15]
        mov    al, 1
        shl    al, cl
        mov    byte ptr [edi], 0
        cmp    cl, 5
        mov    byte ptr [esi + 0x10], 0x10
        jb     loc_2_1319
        add    al, al
        mov    edx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [edx + 0x18]
        mov    byte ptr [edi], 1
        mov    byte ptr [esi + 0x14], 0
        test   byte ptr [edx + 0x806], al
        je     loc_2_12D9
        mov    byte ptr [edi], 3
loc_2_12D9:
        lea    eax, [ecx - 5]
        cmp    al, 1
        sbb    al, al
        and    al, 0xf1
        add    al, 0x10
        test   byte ptr [edx + 0x807], al
        je     loc_2_12EF
        or     byte ptr [edi], 4
loc_2_12EF:
        movzx  eax, cl
        lea    eax, [edx + eax*4 + 0x7f4]
        movzx  ecx, word ptr [eax]
        movzx  eax, word ptr [eax + 2]
        mov    dword ptr [esi], ecx
        test   eax, eax
        lea    ecx, [esi + 0x18]
        mov    dword ptr [ecx], eax
        je     loc_2_139D
        sub    eax, dword ptr [esi]
        inc    eax
        mov    dword ptr [ecx], eax
        jmp    loc_2_139D
loc_2_1319:
        mov    edx, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [edx + 0x18]
        test   byte ptr [ebx + 0x806], al
        je     loc_2_132B
        mov    byte ptr [edi], 2
loc_2_132B:
        movzx  ecx, cl
        lea    ebp, [ebx + ecx*8 + 0x810]
        xor    ebx, ebx
        mov    ax, word ptr [ebp]
        mov    bx, ax
        and    ebx, 0xfff
        test   ah, 0x80
        mov    dword ptr [esi], ebx
        je     loc_2_134E
        or     byte ptr [edi], 4
loc_2_134E:
        mov    ax, word ptr [ebp + 2]
        lea    edi, [esi + 0x18]
        xor    ebp, ebp
        mov    bp, ax
        and    ebp, 0xfff
        mov    dword ptr [edi], ebp
        je     loc_2_1369
        sub    ebp, ebx
        inc    ebp
        mov    dword ptr [edi], ebp
loc_2_1369:
        shl    ebx, 0xc
        shr    ax, 0xe
        mov    dword ptr [esi], ebx
        movzx  ebp, ax
        shl    dword ptr [edi], 0xc
        mov    al, byte ptr [ebp]            ; -> obj3:0x00A4
        mov    byte ptr [esi + 0x14], al
        movzx  eax, byte ptr [edx + 0x24]
        imul   eax, eax, 0x24
        mov    eax, dword ptr [eax + 0x18]   ; -> loc_3_00C8
        test   eax, eax
        je     loc_2_139D
        push   ecx
        push   edx
        call   eax
        add    esp, 8
        or     eax, ebx
        mov    dword ptr [esi], eax
loc_2_139D:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_13A3:
        cmp    byte ptr [eax], 0
        jne    loc_2_13AE
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_13B2
loc_2_13AE:
        or     byte ptr [esi + 0x20], 1
loc_2_13B2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_13B7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [esi + 0x11]
        cmp    bl, 7
        jb     loc_2_13D6
        lea    edi, [esi + 0x1d]
        mov    byte ptr [edi], 0x11
        jmp    loc_2_157F
loc_2_13D6:
        mov    al, byte ptr [esi + 0x10]
        test   al, 0x10
        je     loc_2_1579
        test   al, 0xf
        jne    loc_2_1579
        lea    edi, [esi + 0x1d]
        cmp    bl, 5
        mov    byte ptr [edi], 0
        jb     loc_2_149A
        mov    al, byte ptr [esi + 0x15]
        test   al, 1
        jne    loc_2_1407
        mov    byte ptr [edi], 0xd
        jmp    loc_2_155A
loc_2_1407:
        push   0                             ; -> obj3:0x008C
        push   eax
        push   dword ptr [esi + 0x18]
        push   dword ptr [esi]
        call   sub_2_1D74
        add    esp, 0x10
        test   al, al
        jne    loc_2_1558
        push   0
        mov    al, byte ptr [esi + 0x11]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_1E34
        movzx  eax, byte ptr [esi + 0x11]
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0x18]
        mov    edx, dword ptr [ecx + 0x18]
        test   ebx, ebx
        lea    ecx, [edx + eax*4 + 0x7f4]
        mov    ax, word ptr [esi]
        mov    word ptr [ecx], ax
        je     loc_2_1458
        add    ax, bx
        dec    ax
        jmp    loc_2_145C
loc_2_1458:
        mov    ax, 0xffff
loc_2_145C:
        mov    word ptr [ecx + 2], ax
        mov    al, byte ptr [esi + 0x15]
        and    al, 4
        mov    ecx, dword ptr [ebp + 8]
        cmp    al, 1
        mov    al, 0
        adc    al, 0xff
        and    al, 0xb
        cmp    byte ptr [ecx + 0x24], 5
        jne    loc_2_147E
        test   byte ptr [ecx + 0xb], 8
        je     loc_2_147E
        and    al, 0xfd
loc_2_147E:
        cmp    byte ptr [esi + 0x11], 5
        mov    cl, 0xf0
        jbe    loc_2_148B
        shl    al, 4
        mov    cl, 0xf
loc_2_148B:
        add    edx, 0x807
        and    byte ptr [edx], cl
        or     byte ptr [edx], al
        jmp    loc_2_155A
loc_2_149A:
        test   byte ptr [esi + 0x15], 1
        je     loc_2_14A8
        mov    byte ptr [edi], 0xd
        jmp    loc_2_155A
loc_2_14A8:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + 0x15]
        push   dword ptr [ecx + 0x14]
        push   edx
        push   dword ptr [esi + 0x18]
        push   dword ptr [esi]
        call   sub_2_1D74
        add    esp, 0x10
        test   al, al
        jne    loc_2_1558
        lea    eax, [ebp - 1]
        mov    cl, byte ptr [esi + 0x14]
        push   eax
        push   ecx
        call   sub_2_1DCD
        add    esp, 8
        test   al, al
        jne    loc_2_1558
        push   0
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_2_1E34
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        movzx  edx, bl
        mov    eax, dword ptr [ecx + 0x18]
        lea    ecx, [eax + edx*8 + 0x810]
        mov    eax, dword ptr [esi]
        shr    eax, 0xc
        and    ax, 0xfff
        test   byte ptr [esi + 0x15], 4
        je     loc_2_150B
        or     ah, 0x80
loc_2_150B:
        mov    word ptr [ecx], ax
        cmp    dword ptr [esi + 0x18], 0
        mov    ax, 0xfff
        je     loc_2_1525
        mov    eax, dword ptr [esi]
        add    eax, dword ptr [esi + 0x18]
        dec    eax
        shr    eax, 0xc
        and    ax, 0xfff
loc_2_1525:
        movzx  bx, byte ptr [ebp - 1]
        shl    bx, 0xe
        or     ax, bx
        mov    word ptr [ecx + 2], ax
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ecx + 0x24]
        imul   eax, eax, 0x24
        mov    ecx, dword ptr [eax + 0x14]   ; -> loc_3_00C4
        test   ecx, ecx
        je     loc_2_155A
        push   dword ptr [esi]
        push   edx
        push   dword ptr [ebp + 8]
        call   ecx
        add    esp, 0xc
        test   al, al
        je     loc_2_155A
loc_2_1558:
        mov    byte ptr [edi], al
loc_2_155A:
        cmp    byte ptr [edi], 0
        jne    loc_2_158A
        xor    eax, eax
        mov    cl, byte ptr [esi + 0x11]
        mov    al, byte ptr [esi + 0x15]
        and    eax, 2
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_2_1E34
        add    esp, 0xc
        jmp    loc_2_157F
loc_2_1579:
        lea    edi, [esi + 0x1d]
        mov    byte ptr [edi], 0xb
loc_2_157F:
        cmp    byte ptr [edi], 0
        jne    loc_2_158A
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_158E
loc_2_158A:
        or     byte ptr [esi + 0x20], 1
loc_2_158E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1595:
        mov    edx, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    cl, byte ptr [edx + 0x11]
        cmp    cl, 5
        jb     loc_2_15AB
        lea    eax, [edx + 0x1d]
        mov    byte ptr [eax], 0x11
        jmp    loc_2_1618
loc_2_15AB:
        cmp    byte ptr [edx + 0x10], 0
        je     loc_2_15B9
        lea    eax, [edx + 0x1d]
        mov    byte ptr [eax], 8
        jmp    loc_2_1618
loc_2_15B9:
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [eax + 0x18]
        movzx  eax, cl
        mov    di, word ptr [esi + eax*8 + 0x814]
        lea    eax, [esi + eax*8 + 0x810]
        mov    ax, word ptr [eax]
        and    ax, 0xfff
        add    ax, di
        and    ax, 0x3fff
        mov    word ptr [edx], ax
        lea    eax, [edx + 0x14]
        test   di, 0x8000
        mov    byte ptr [eax], 0
        je     loc_2_15F3
        mov    byte ptr [eax], 4
loc_2_15F3:
        test   di, 0x4000
        je     loc_2_15FD
        or     byte ptr [eax], 1
loc_2_15FD:
        movzx  esi, byte ptr [esi + 0x806]
        mov    edi, 1
        shl    edi, cl
        test   edi, esi
        je     loc_2_1612
        or     byte ptr [eax], 2
loc_2_1612:
        lea    eax, [edx + 0x1d]
        mov    byte ptr [eax], 0
loc_2_1618:
        cmp    byte ptr [eax], 0
        jne    loc_2_1623
        and    byte ptr [edx + 0x20], 0xfe
        jmp    loc_2_1627
loc_2_1623:
        or     byte ptr [edx + 0x20], 1
loc_2_1627:
        pop    edi
        pop    esi
        ret    
loc_2_162A:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    al, byte ptr [esi + 0x11]
        cmp    al, 5
        jb     loc_2_163F
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x11
        jmp    loc_2_16B4
loc_2_163F:
        cmp    byte ptr [esi + 0x10], 0
        je     loc_2_164D
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 8
        jmp    loc_2_16B4
loc_2_164D:
        mov    di, word ptr [esi]
        test   di, 0xc000
        je     loc_2_165F
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 7
        jmp    loc_2_16B4
loc_2_165F:
        mov    cl, byte ptr [esi + 0x14]
        movzx  edx, cl
        test   edx, 0xfffffff8
        je     loc_2_1675
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 2
        jmp    loc_2_16B4
loc_2_1675:
        movzx  edx, al
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        lea    eax, [eax + edx*8 + 0x810]
        mov    dx, word ptr [eax]
        and    dx, 0xfff
        sub    di, dx
        and    di, 0x3fff
        test   cl, 4
        je     loc_2_16A0
        or     di, 0x8000
loc_2_16A0:
        test   cl, 1
        je     loc_2_16AA
        or     di, 0x4000
loc_2_16AA:
        mov    word ptr [eax + 4], di
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_16B4:
        cmp    byte ptr [eax], 0
        jne    loc_2_16BF
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_16C3
loc_2_16BF:
        or     byte ptr [esi + 0x20], 1
loc_2_16C3:
        pop    edi
        pop    esi
        ret    
loc_2_16C6:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0x10], 0
        je     loc_2_16D9
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_171D
loc_2_16D9:
        mov    byte ptr [esi + 0x11], 0xf0
        mov    byte ptr [esi + 0x15], 0xf0
        mov    byte ptr [esi + 0x14], 0
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, word ptr [eax + 0x1c]
        mov    edx, dword ptr [eax + 0x10]
        mov    al, byte ptr [eax + 0x26]
        and    al, 0x1f
        push   0x12
        push   edx
        mov    dword ptr [edx + 2], ecx
        push   dword ptr [esi]
        cmp    al, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0xffff
        mov    dword ptr [edx + 6], eax
        call   sub_2_1E64
        add    esp, 0xc
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_171D:
        cmp    byte ptr [eax], 0
        jne    loc_2_1728
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_172C
loc_2_1728:
        or     byte ptr [esi + 0x20], 1
loc_2_172C:
        pop    esi
        ret    
loc_2_172E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x10], 0
        je     loc_2_1747
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_179F
loc_2_1747:
        push   esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1B49
        movzx  ax, byte ptr [edi + 0x27]
        mov    word ptr [esi + 8], ax
        add    esp, 8
        mov    eax, dword ptr [edi + 0x18]
        mov    cl, byte ptr [eax + 0x805]
        mov    eax, dword ptr [eax + 4]
        and    cl, 7
        and    al, 6
        shl    cl, 4
        cmp    al, 6
        mov    byte ptr [esi + 0x11], cl
        jne    loc_2_177C
        or     byte ptr [esi + 0x11], 0x80
loc_2_177C:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 2]
        push   eax
        push   ecx
        push   edi
        call   sub_2_0CD5
        add    esp, 0xc
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0x19], cl
        mov    byte ptr [esi + 0x18], al
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_179F:
        cmp    byte ptr [eax], 0
        jne    loc_2_17AA
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_17AE
loc_2_17AA:
        or     byte ptr [esi + 0x20], 1
loc_2_17AE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_17B4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        cmp    byte ptr [edi + 0x10], 0
        je     loc_2_17D1
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_1A93
loc_2_17D1:
        mov    dl, byte ptr [edi + 0x19]
        mov    al, dl
        and    al, 0x30
        cmp    al, 0x30
        jne    loc_2_17E7
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 2
        jmp    loc_2_1A93
loc_2_17E7:
        mov    ax, word ptr [edi]
        and    ah, 3
        cmp    ah, 3
        jne    loc_2_1804
        cmp    word ptr [edi + 8], 1
        jb     loc_2_1804
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xd
        jmp    loc_2_1A93
loc_2_1804:
        mov    ax, word ptr [edi]
        and    al, 0x1f
        mov    byte ptr [ebp - 2], al
        mov    ax, word ptr [edi]
        mov    cl, byte ptr [edi + 0x18]
        shr    ax, 8
        and    al, 3
        mov    byte ptr [ebp - 7], al
        mov    al, dl
        and    al, 7
        mov    byte ptr [ebp - 5], al
        mov    al, cl
        and    al, 0x70
        shr    al, 4
        mov    byte ptr [ebp - 3], al
        mov    al, cl
        and    al, 7
        cmp    byte ptr [ebp - 7], 1
        mov    byte ptr [ebp - 4], al
        je     loc_2_187A
        test   byte ptr [edi], 0x80
        je     loc_2_187A
        mov    ax, word ptr [edi]
        and    ax, 0x40
        je     loc_2_185D
        cmp    byte ptr [ebp - 2], 0xf
        ja     loc_2_186F
        mov    esi, dword ptr [ebp + 8]
        movzx  ebx, byte ptr [ebp - 2]
        cmp    byte ptr [ebx + esi + 0x28], 0
        je     loc_2_186F
        jmp    loc_2_1860
loc_2_185D:
        mov    esi, dword ptr [ebp + 8]
loc_2_1860:
        test   ax, ax
        jne    loc_2_187D
        mov    al, byte ptr [esi + 0x26]
        and    al, 0x1f
        cmp    al, byte ptr [ebp - 2]
        je     loc_2_187D
loc_2_186F:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 6
        jmp    loc_2_1A93
loc_2_187A:
        mov    esi, dword ptr [ebp + 8]
loc_2_187D:
        mov    ebx, dword ptr [esi + 0xc]
        movzx  ax, byte ptr [ebp - 5]
        cmp    word ptr [ebx + 0xa], ax
        jbe    loc_2_1A8D
        movzx  eax, ax
        test   byte ptr [ebx + eax*2 + 0xd], 0x80
        je     loc_2_1A8D
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 4], al
        jne    loc_2_1A85
        movzx  ax, al
        cmp    word ptr [ebx + 0xa], ax
        jbe    loc_2_1A85
        movzx  eax, ax
        test   byte ptr [ebx + eax*2 + 0xd], 0x40
        je     loc_2_1A85
        push   ecx
        push   edx
        push   esi
        call   sub_2_0B9D
        add    esp, 0xc
        test   eax, eax
        je     loc_2_1A8D
        mov    al, byte ptr [edi + 0x15]
        mov    edx, dword ptr [esi + 0x18]
        not    al
        mov    bl, byte ptr [edi + 0x11]
        and    byte ptr [esi + 0x25], al
        mov    cl, bl
        mov    al, byte ptr [edx + 0x805]
        and    cl, 0x70
        and    al, 0xf0
        shr    cl, 4
        or     cl, al
        test   bl, 0x80
        mov    byte ptr [edx + 0x805], cl
        je     loc_2_1910
        lea    ebx, [esi + 8]
        or     byte ptr [edx + 4], 6
        or     byte ptr [ebx + 3], 4
        jmp    loc_2_191B
loc_2_1910:
        lea    ebx, [esi + 8]
        and    byte ptr [edx + 4], 0xf9
        and    byte ptr [ebx + 3], 0xfb
loc_2_191B:
        add    edx, 0x803
        mov    al, byte ptr [edx]
        mov    cl, al
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0xd0
        and    cl, 0xf
        cmp    byte ptr [ebp - 7], 2
        je     loc_2_193B
        cmp    byte ptr [ebp - 7], 3
        jne    loc_2_1953
loc_2_193B:
        or     byte ptr [ebp - 1], 0x20
        mov    ax, word ptr [edi]
        and    al, 0xc0
        cmp    al, 0xc0
        jne    loc_2_1953
        movzx  eax, byte ptr [ebp - 2]
        mov    al, byte ptr [eax + esi + 0x28]
        or     byte ptr [ebp - 1], al
loc_2_1953:
        test   byte ptr [ebx + 2], 2
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edx], al
        je     loc_2_1975
        test   cl, cl
        je     loc_2_1975
        mov    al, byte ptr [ebp - 1]
        and    al, 0xf
        cmp    al, cl
        je     loc_2_1975
        push   ecx
        push   esi
        call   sub_2_02AA
        add    esp, 8
loc_2_1975:
        cmp    byte ptr [ebp - 7], 3
        jne    loc_2_1999
        movzx  ecx, byte ptr [esi + 0x24]
        mov    al, byte ptr [edi + 8]
        push   esi
        mov    byte ptr [esi + 0x27], al
        imul   ecx, ecx, 0x24
        movzx  eax, al
        or     byte ptr [ebx + 3], 8
        call   sub_3_00CC
        jmp    loc_2_19BB
loc_2_1999:
        mov    eax, dword ptr [ebx]
        test   eax, 0x8000000
        je     loc_2_19BE
        and    eax, 0xf7ffffff
        push   esi
        mov    dword ptr [ebx], eax
        movzx  eax, byte ptr [esi + 0x24]
        imul   eax, eax, 0x24
        mov    byte ptr [esi + 0x27], 0
        call   sub_3_00D0
loc_2_19BB:
        add    esp, 4
loc_2_19BE:
        cmp    byte ptr [ebp - 2], 0
        je     loc_2_19DF
        cmp    byte ptr [ebp - 7], 2
        je     loc_2_19D0
        cmp    byte ptr [ebp - 7], 3
        jne    loc_2_19DF
loc_2_19D0:
        mov    ax, word ptr [edi]
        and    al, 0xc0
        cmp    al, 0x80
        jne    loc_2_19DF
        or     byte ptr [ebx + 3], 0x10
        jmp    loc_2_19E3
loc_2_19DF:
        and    byte ptr [ebx + 3], 0xef
loc_2_19E3:
        mov    ebx, dword ptr [ebx]
        push   0
        test   ebx, 0x10000
        je     loc_2_1A33
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   0x91
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 0x18
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [ecx + 8]
        and    al, 0x30
        cmp    al, 0x20
        je     loc_2_1A20
        test   ebx, 0x10000000
        jne    loc_2_1A20
        and    byte ptr [ebp - 1], 0xef
        jmp    loc_2_1A24
loc_2_1A20:
        or     byte ptr [ebp - 1], 0x10
loc_2_1A24:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   0x91
        jmp    loc_2_1A71
loc_2_1A33:
        push   2
        lea    eax, [ebp - 8]
        push   eax
        push   0x3e
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 0x18
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [ecx + 8]
        and    al, 0x30
        cmp    al, 0x20
        je     loc_2_1A61
        test   ebx, 0x10000000
        jne    loc_2_1A61
        or     byte ptr [ebp - 8], 0x80
        jmp    loc_2_1A67
loc_2_1A61:
        and    word ptr [ebp - 8], 0xff7f
loc_2_1A67:
        push   0
        lea    eax, [ebp - 8]
        push   2
        push   eax
        push   0x3e
loc_2_1A71:
        push   1
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 0x18
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0
        jmp    loc_2_1A93
loc_2_1A85:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xf
        jmp    loc_2_1A93
loc_2_1A8D:
        lea    eax, [edi + 0x1d]
        mov    byte ptr [eax], 0xe
loc_2_1A93:
        cmp    byte ptr [eax], 0
        jne    loc_2_1A9E
        and    byte ptr [edi + 0x20], 0xfe
        jmp    loc_2_1AA2
loc_2_1A9E:
        or     byte ptr [edi + 0x20], 1
loc_2_1AA2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1AA9:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        cmp    byte ptr [esi + 0x10], 0
        je     loc_2_1ABE
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_1B36
loc_2_1ABE:
        mov    edi, dword ptr [esp + 0x10]
        push   esi
        push   edi
        lea    ebx, [esi + 0x11]
        call   sub_2_1B49
        add    esp, 8
        mov    ecx, dword ptr [edi + 0x18]
        mov    byte ptr [ebx], 0
        mov    edx, dword ptr [ecx + 8]
        mov    eax, edx
        and    eax, 0x30
        cmp    eax, 0x10
        jne    loc_2_1B11
        mov    cl, byte ptr [ecx + 0x801]
        mov    al, cl
        shl    al, 4
        not    al
        and    al, 0x30
        test   cl, 0x10
        mov    byte ptr [ebx], al
        je     loc_2_1AFC
        or     al, 1
        mov    byte ptr [ebx], al
loc_2_1AFC:
        test   cl, 0x20
        je     loc_2_1B04
        or     byte ptr [ebx], 0x40
loc_2_1B04:
        and    cl, 0xc
        cmp    cl, 0xc
        jne    loc_2_1B30
        or     byte ptr [ebx], 0x80
        jmp    loc_2_1B30
loc_2_1B11:
        cmp    eax, 0x20
        jne    loc_2_1B30
        test   dh, 1
        je     loc_2_1B1E
        mov    byte ptr [ebx], 0x30
loc_2_1B1E:
        mov    al, byte ptr [ebx]
        and    dl, 6
        or     al, 0x40
        cmp    dl, 6
        mov    byte ptr [ebx], al
        je     loc_2_1B30
        or     al, 0x80
        mov    byte ptr [ebx], al
loc_2_1B30:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_1B36:
        cmp    byte ptr [eax], 0
        jne    loc_2_1B41
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_1B45
loc_2_1B41:
        or     byte ptr [esi + 0x20], 1
loc_2_1B45:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_1B49:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        lea    ecx, [esi + 0x25]
        mov    eax, dword ptr [esi + 0x18]
        mov    dl, byte ptr [eax + 0x804]
        and    dl, 0xf
        or     byte ptr [ecx], dl
        mov    edx, dword ptr [eax]
        and    dl, 6
        cmp    dl, 6
        jne    loc_2_1B6D
        or     byte ptr [ecx], 8
loc_2_1B6D:
        mov    cl, byte ptr [ecx]
        mov    edx, dword ptr [esp + 0x10]
        shl    cl, 4
        mov    dword ptr [eax], 0xf
        mov    byte ptr [edx + 0x15], cl
        xor    ecx, ecx
        mov    word ptr [edx], cx
        mov    byte ptr [edx + 0x14], cl
        mov    cl, byte ptr [eax + 0x803]
        mov    eax, dword ptr [eax + 8]
        and    al, 0x30
        cmp    al, 0x10
        jne    loc_2_1BB7
        test   cl, 0x20
        mov    eax, 1
        je     loc_2_1BB0
        mov    eax, dword ptr [esi + 8]
        and    eax, 0x8000000
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 3
loc_2_1BB0:
        shl    ax, 8
        mov    word ptr [edx], ax
loc_2_1BB7:
        and    cl, 0xf
        je     loc_2_1BD3
        xor    al, al
loc_2_1BBE:
        movzx  edi, al
        cmp    byte ptr [edi + esi + 0x28], cl
        je     loc_2_1BCF
        inc    al
        cmp    al, 0x10
        jb     loc_2_1BBE
        jmp    loc_2_1BE7
loc_2_1BCF:
        or     al, 0xc0
        jmp    loc_2_1BE0
loc_2_1BD3:
        test   byte ptr [esi + 0xb], 0x10
        je     loc_2_1BE7
        mov    al, byte ptr [esi + 0x26]
        and    al, 0x1f
        or     al, 0x80
loc_2_1BE0:
        movzx  ax, al
        or     word ptr [edx], ax
loc_2_1BE7:
        pop    edi
        pop    esi
        ret    
loc_2_1BEA:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        push   ebp
        cmp    byte ptr [esi + 0x10], 0
        je     loc_2_1C06
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xb
        jmp    loc_2_1CF0
loc_2_1C06:
        mov    edi, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [edi + 0x18]
        test   byte ptr [ebp + 8], 6
        je     loc_2_1C1E
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x14
        jmp    loc_2_1CF0
loc_2_1C1E:
        mov    dword ptr [ebp + 0xc], 0x4000
        push   0
        push   0x493e0
        call   sub_2_21AC
        test   byte ptr [ebp + 8], 0
        movzx  eax, byte ptr [edi + 0x24]
        imul   eax, eax, 0x24
        mov    byte ptr [ebp + 0x806], 0x20
        and    byte ptr [ebp + 0x803], 0xdf
        add    esp, 8
        mov    eax, dword ptr [eax + 0x20]   ; -> sub_3_00D0
        test   eax, eax
        je     loc_2_1C5D
        push   edi
        call   eax
        add    esp, 4
loc_2_1C5D:
        and    byte ptr [edi + 0xb], 0xf7
        mov    byte ptr [edi + 0x27], 0
        push   0
        push   0x10
        push   edi
        call   sub_2_0B9D
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1C7E
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0xe
        jmp    loc_2_1CF0
loc_2_1C7E:
        mov    ebp, dword ptr [edi + 0x18]
        xor    ebx, ebx
        add    ebp, 0x803
        mov    edi, dword ptr [edi]
        lea    eax, [esp + 0x12]
        push   ebx
        push   2
        and    byte ptr [ebp], 0xbf
        push   eax
        push   0x3e
        push   ebx
        push   edi
        call   sub_2_21A4
        or     byte ptr [esp + 0x2a], 0x40
        lea    ecx, [esp + 0x2a]
        add    esp, 0x18
        push   ebx
        push   2
        push   ecx
        push   0x3e
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        push   ebx
        push   0x7d0
        call   sub_2_21AC
        lea    ecx, [esp + 0x1a]
        add    esp, 8
        and    word ptr [esp + 0x12], 0xffbf
        push   ebx
        push   2
        push   ecx
        push   0x3e
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 0x18
        lea    eax, [esi + 0x1d]
        or     byte ptr [ebp], 0x40
        mov    byte ptr [eax], bl
loc_2_1CF0:
        cmp    byte ptr [eax], 0
        jne    loc_2_1CFB
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_1CFF
loc_2_1CFB:
        or     byte ptr [esi + 0x20], 1
loc_2_1CFF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_2_1D07:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        cmp    byte ptr [esi + 0x10], 0
        je     loc_2_1D1C
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0x15
        jmp    loc_2_1D61
loc_2_1D1C:
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   eax
        call   sub_2_21D4
        add    esp, 4
        mov    ebx, eax
        mov    edi, dword ptr [esi]
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        mov    word ptr [esi + 0x14], 0x100
        push   ebx
        push   eax
        push   edi
        call   sub_2_1E64
        mov    ax, word ptr [edi]
        add    esp, 0xc
        cmp    bx, ax
        jae    loc_2_1D53
        mov    byte ptr [edi + ebx + 4], 0
        jmp    loc_2_1D5B
loc_2_1D53:
        movzx  eax, ax
        mov    byte ptr [edi + eax + 3], 0
loc_2_1D5B:
        lea    eax, [esi + 0x1d]
        mov    byte ptr [eax], 0
loc_2_1D61:
        cmp    byte ptr [eax], 0
        jne    loc_2_1D6C
        and    byte ptr [esi + 0x20], 0xfe
        jmp    loc_2_1D70
loc_2_1D6C:
        or     byte ptr [esi + 0x20], 1
loc_2_1D70:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_1D74:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x14]
        lea    eax, [ecx + edx - 1]
        cmp    dword ptr [esi + 2], ecx
        ja     loc_2_1D90
        cmp    dword ptr [esi + 6], ecx
        jae    loc_2_1D9A
loc_2_1D90:
        test   byte ptr [ebp + 0x10], 2
        jne    loc_2_1DC8
        test   ecx, ecx
        jne    loc_2_1DC8
loc_2_1D9A:
        cmp    dword ptr [esi + 0xa], edx
        ja     loc_2_1DA4
        cmp    dword ptr [esi + 0xe], edx
        jae    loc_2_1DAE
loc_2_1DA4:
        test   byte ptr [ebp + 0x10], 2
        jne    loc_2_1DC4
        test   edx, edx
        jne    loc_2_1DC4
loc_2_1DAE:
        cmp    dword ptr [esi + 6], eax
        jb     loc_2_1DC0
        xor    eax, ecx
        test   eax, 0xff000000
        jne    loc_2_1DC0
        xor    al, al
        jmp    loc_2_1DCA
loc_2_1DC0:
        mov    al, 0xa
        jmp    loc_2_1DCA
loc_2_1DC4:
        mov    al, 0xa
        jmp    loc_2_1DCA
loc_2_1DC8:
        mov    al, 3
loc_2_1DCA:
        pop    esi
        pop    ebp
        ret    
sub_2_1DCD:
        push   ebp
        mov    al, 0x17
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    cl, byte ptr [ebp + 8]
        test   cl, 0x80
        jne    loc_2_1E2F
        mov    dl, cl
        and    dl, 7
        and    cl, 0x78
        mov    byte ptr [ebp - 2], dl
        mov    byte ptr [ebp - 1], cl
        jne    loc_2_1DFA
        movzx  ecx, dl
        mov    cl, byte ptr [ecx]            ; -> obj3:0x00A8
        mov    byte ptr [ebp - 1], cl
loc_2_1DFA:
        xor    dl, dl
loc_2_1DFC:
        movzx  ecx, dl
        mov    cl, byte ptr [ecx]            ; -> obj3:0x00A4
        mov    bl, cl
        and    bl, 7
        mov    byte ptr [ebp - 3], bl
        mov    bl, byte ptr [ebp - 2]
        cmp    bl, byte ptr [ebp - 3]
        jb     loc_2_1E28
        jne    loc_2_1E1F
        and    cl, 0x78
        cmp    cl, byte ptr [ebp - 1]
        jae    loc_2_1E28
loc_2_1E1F:
        inc    dl
        cmp    dl, 4
        jb     loc_2_1DFC
        jmp    loc_2_1E2F
loc_2_1E28:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], dl
        xor    al, al
loc_2_1E2F:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1E34:
        push   ebp
        mov    dl, 1
        mov    ebp, esp
        mov    cl, byte ptr [esp + 0xc]
        shl    dl, cl
        cmp    cl, 5
        jb     loc_2_1E46
        add    dl, dl
loc_2_1E46:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0x10], 0
        mov    ecx, dword ptr [eax + 0x18]
        je     loc_2_1E5A
        or     byte ptr [ecx + 0x806], dl
        jmp    loc_2_1E62
loc_2_1E5A:
        not    dl
        and    byte ptr [ecx + 0x806], dl
loc_2_1E62:
        pop    ebp
        ret    
sub_2_1E64:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        movzx  eax, word ptr [edi]
        mov    word ptr [edi + 2], dx
        cmp    eax, edx
        jge    loc_2_1E7B
        mov    edx, eax
loc_2_1E7B:
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
sub_2_1E93:
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
sub_2_1EB0:
        cmp    dword ptr [esp + 8], 2
        push   esi
        push   edi
        jne    loc_2_1F1E
        mov    edi, 0x13
        push   1
        push   0x38
        call   sub_2_21EC
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_2_1EF7
        push   esi
        push   dword ptr [esp + 0x10]
        call   sub_2_2006
        add    esp, 8
        test   eax, eax
        je     loc_2_1EF7
        push   3
        push   esi
        push   0                             ; -> loc_2_1F2A
        push   dword ptr [esp + 0x18]
        call   sub_2_21E4
        add    esp, 0x10
        mov    edi, eax
loc_2_1EF7:
        cmp    edi, 0x13
        jne    loc_2_1F23
        test   esi, esi
        je     loc_2_1F23
        lea    ecx, [esi + 0x38]
        mov    eax, esi
        push   0
        push   ecx
        push   eax
        call   sub_2_21BC
        add    esp, 0xc
        push   0
        push   esi
        call   sub_2_21DC
        add    esp, 8
        jmp    loc_2_1F23
loc_2_1F1E:
        mov    edi, 1
loc_2_1F23:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    8
loc_2_1F2A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        cmp    eax, 1
        je     loc_2_1F4E
        cmp    eax, 4
        je     loc_2_1F98
        cmp    eax, 8
        je     loc_2_1FB7
        mov    esi, 1
        jmp    loc_2_2000
loc_2_1F4E:
        mov    edi, dword ptr [ebp + 0x14]
        push   edi
        call   sub_2_2037
        add    esp, 4
        test   eax, eax
        jne    loc_2_1F68
        mov    esi, 0x13
        jmp    loc_2_2000
loc_2_1F68:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_2_2000
        push   edi
        call   sub_2_216C
        add    esp, 4
        test   eax, eax
        je     loc_2_1F86
        mov    esi, 0x13
        jmp    loc_2_2000
loc_2_1F86:
        push   0
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_2_21F4
        add    esp, 0xc
        jmp    loc_2_2000
loc_2_1F98:
        mov    edi, dword ptr [ebp + 0x14]
        push   0
        lea    eax, [edi + 0x38]
        push   eax
        push   edi
        call   sub_2_21BC
        add    esp, 0xc
        push   0
        push   edi
        call   sub_2_21DC
        add    esp, 8
        jmp    loc_2_2000
loc_2_1FB7:
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0xc]
        and    edx, 0x7fffffff
        test   byte ptr [eax + 0xb], 2
        je     loc_2_2000
        cmp    edx, 4
        je     loc_2_1FD3
        cmp    edx, 5
        jne    loc_2_1FE6
loc_2_1FD3:
        mov    ecx, dword ptr [eax + 0x18]
        add    ecx, 0x803
        test   byte ptr [ecx], 0x20
        je     loc_2_1FE6
        or     byte ptr [ecx], 0x80
        jmp    loc_2_2000
loc_2_1FE6:
        cmp    edx, 6
        je     loc_2_1FF0
        cmp    edx, 7
        jne    loc_2_2000
loc_2_1FF0:
        mov    eax, dword ptr [eax + 0x18]
        add    eax, 0x803
        test   byte ptr [eax], 0x80
        je     loc_2_2000
        and    byte ptr [eax], 0x7f
loc_2_2000:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_2006:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0x10]
        lea    eax, [esi + 0x38]
        push   eax
        push   esi
        call   sub_2_21CC
        add    esp, 0xc
        test   eax, eax
        mov    eax, 0
        je     loc_2_2035
        mov    dword ptr [esi + 4], 0x53534243
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        mov    eax, 1
loc_2_2035:
        pop    esi
        ret    
sub_2_2037:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        push   2
        lea    eax, [ebp - 0x1c]
        mov    edi, dword ptr [esi]
        push   edi
        push   eax
        call   sub_2_220C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2163
        push   ebx
        lea    ecx, [ebp - 0x10]
        push   ebx
        push   0x43
        push   dword ptr [ebp - 0x1c]
        push   ecx
        call   sub_2_21FC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_2163
        push   ebx
        lea    ecx, [ebp - 8]
        push   ebx
        push   1
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_21FC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_2163
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        sub    eax, ecx
        inc    eax
        push   eax
        push   ecx
        call   sub_2_2204
        add    esp, 0xc
        mov    dword ptr [esi + 0x18], eax
        cmp    eax, -1
        jne    loc_2_20BB
        mov    dword ptr [esi + 0x18], ebx
        jmp    loc_2_2163
loc_2_20BB:
        push   0
        lea    ecx, [ebp - 8]
        push   0
        push   4
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_21FC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_20DD
        cmp    eax, 0xf
        jne    loc_2_2163
loc_2_20DD:
        mov    dword ptr [ebp - 0xc], 4
        test   eax, eax
        jne    loc_2_20F3
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [esi + 0x26], al
        jmp    loc_2_20F7
loc_2_20F3:
        mov    byte ptr [esi + 0x26], 0
loc_2_20F7:
        push   1
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 0x18]
        push   ecx
        push   3
        push   dword ptr [0]                 ; -> obj3:0x000C
        push   0
        push   edi
        call   sub_2_21B4
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_2129
        cmp    dword ptr [ebp - 0xc], 4
        jne    loc_2_2129
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        or     dword ptr [esi + 8], eax
loc_2_2129:
        VxDCall  VPEND.Service_0000h             ; cd2000002600
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0
        je     loc_2_2155
        lea    eax, [ebp - 0x14]
        push   eax
        VxDCall  VPEND.Service_000Eh             ; cd200e002600
        add    esp, 4
        or     eax, eax
        jne    loc_2_2155
        test   byte ptr [ebp - 0x14], 0xc0
        je     loc_2_2155
        or     byte ptr [esi + 0xb], 2
loc_2_2155:
        push   esi
        mov    ebx, 1
        call   sub_2_100B
        add    esp, 4
loc_2_2163:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_216C:
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
        lea    esi, [0]                      ; -> loc_1_013A
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
sub_2_21A4:
        VxDCall  CONFIGMG._CONFIGMG_Call_Enumerator_Function ; cd2054803300
        int3   
        int3   
sub_2_21AC:
        VxDCall  CONFIGMG._CONFIGMG_Yield        ; cd2015803300
        int3   
        int3   
sub_2_21B4:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_21BC:
        VMMCall  VMM.Service_8195h               ; cd2095810100
        int3   
        int3   
sub_2_21C4:
        VxDCall  VPICD.Service_8019h             ; cd2019800300
        int3   
        int3   
sub_2_21CC:
        VMMCall  VMM.Service_8194h               ; cd2094810100
        int3   
        int3   
sub_2_21D4:
        VMMCall  VMM._lstrlen                    ; cd207c810100
        int3   
        int3   
sub_2_21DC:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_2_21E4:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        int3   
        int3   
sub_2_21EC:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_2_21F4:
        VxDCall  CONFIGMG._CONFIGMG_Set_Device_Driver_Private_DWord ; cd2049803300
        int3   
        int3   
sub_2_21FC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_2_2204:
        VMMCall  VMM._MapPhysToLinear            ; cd206c800100
        int3   
        int3   
sub_2_220C:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x2ea  RW-
        dd  obj3:0x0274              ; fixup
        dd  obj3:0x02C8              ; fixup
        dd  obj3:0x02D4              ; fixup
        dd  obj3:0x02E0              ; fixup
        db  008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 0E0h, 078h, 060h
        db  032h, 0E0h, 021h, 0E0h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 001h, 000h, 041h, 001h, 000h, 000h, 000h, 000h, 01Fh, 008h, 000h, 000h
        db  001h, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 010h, 000h, 000h, 0FFh, 0FFh, 0FEh, 000h, 000h, 010h
        db  000h, 000h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 000h, 072h, 008h, 01Fh, 008h, 000h, 000h
        db  001h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 010h, 000h, 000h, 0FFh, 0FFh, 0FEh, 000h, 000h, 010h
        db  000h, 000h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 000h, 072h, 008h, 01Fh, 000h, 000h, 000h
        db  000h, 000h, 0FFh, 0FFh, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h
        db  000h, 000h, 010h, 000h, 042h, 052h, 062h, 072h, 0FFh, 032h, 02Ah, 022h, 00Ah, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
sub_3_00B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_00C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_00C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esp
        adc    byte ptr [edx], dl
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esp
        adc    byte ptr [0xac], dl           ; -> loc_2_0000
        add    byte ptr [eax], al            ; -> loc_2_0112
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0DD9
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0E1F
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0220
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0265
        add    byte ptr [eax], al
        add    byte ptr [eax + edx + 0x13], cl
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    edx, dword ptr [eax]
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jns    0x177
        or     al, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [ecx], 0x66
        add    al, 0                         ; -> loc_2_07F3
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0D62
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0DD9
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0E1F
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0F15
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0F32
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0838
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0847
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x310610], dh ; -> loc_2_0656
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0D62
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0DD9
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_2_0E1F
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0F15
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0F32
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0665
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0674
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x10], al
        and    al, 0xc8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebp
        adc    byte ptr [eax + ecx*8], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_023C:
        adc    al, 2
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 018h, 0FFh
        db  00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 020h, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 028h, 0FFh
        db  00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 030h, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 043h
        db  061h, 072h, 064h, 042h, 075h, 073h, 020h, 053h, 06Fh, 063h, 06Bh, 065h, 074h, 020h, 053h, 065h
        db  072h, 076h, 069h, 063h, 065h, 073h, 020h, 028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h
        db  069h, 067h, 068h, 074h, 020h, 04Dh, 049h, 043h, 052h, 04Fh, 053h, 04Fh, 046h, 054h, 020h, 043h
        db  06Fh, 072h, 070h, 02Eh, 02Ch, 020h, 031h, 039h, 039h, 035h, 02Eh, 020h, 041h, 06Ch, 06Ch, 020h
        db  052h, 069h, 067h, 068h, 074h, 073h, 020h, 052h, 065h, 073h, 065h, 072h, 076h, 065h, 064h, 000h
        db  000h, 000h, 000h, 043h, 042h, 053h, 053h, 049h, 052h, 051h, 04Dh, 061h, 070h, 000h, 000h, 043h
        db  042h, 053h, 053h, 04Fh, 070h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 043h, 042h, 053h, 053h, 046h
        db  06Ch, 061h, 067h, 073h, 000h
VxD__LDATA ENDS
