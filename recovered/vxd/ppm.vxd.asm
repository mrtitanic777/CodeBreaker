; PPM -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 4283 instructions recovered by recursive descent

Declare_Virtual_Device PPM, 1, 0, PPM_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2890  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PPM     '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  PPM_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 070h, 050h, 06Fh, 072h, 074h, 020h, 03Dh, 020h
        db  023h, 065h, 061h, 078h, 00Dh, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  sub_1_03E1               ; fixup
        dd  loc_3_06A3               ; fixup
        dd  loc_1_077B               ; fixup
        dd  loc_1_0856               ; fixup
        dd  loc_3_06CB               ; fixup
        dd  sub_1_037B               ; fixup
        dd  loc_1_089D               ; fixup
        dd  loc_3_07CF               ; fixup
        dd  loc_3_07F7               ; fixup
        dd  loc_1_08C6               ; fixup
        dd  loc_3_082C               ; fixup
        dd  loc_3_088C               ; fixup
        dd  loc_1_0B2A               ; fixup
        dd  loc_1_0B58               ; fixup
        dd  loc_1_0C09               ; fixup
        dd  loc_3_08B8               ; fixup
        dd  loc_3_091A               ; fixup
        dd  loc_3_0971               ; fixup
        dd  loc_3_09C3               ; fixup
        dd  loc_3_0000               ; fixup
        dd  loc_3_0046               ; fixup
        dd  loc_3_09EB               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00F0:
        inc    eax
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 001h, 010h, 00Ch, 000h, 007h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 07Fh, 000h, 000h
loc_1_0134:
        nop    
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        add    eax, dword ptr [eax + 0x9080706]
        or     cl, byte ptr [ebx]
        or     al, 0xd
        push   cs
        movups xmm2, xmmword ptr [ecx]
        adc    dl, byte ptr [ebx]
        adc    al, 0x15
        push   ss
        pop    ss
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 0x1d
        push   ds
        pop    ds
        and    byte ptr [ecx], ah
        and    ah, byte ptr [ebx]
        and    al, 0x25
        daa    
        sub    byte ptr [ecx], ch
        sub    ch, byte ptr [ebx]
        sub    al, 0x2d
        das    
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        cmp    bh, byte ptr [ebx]
        cmp    al, 0x3d
        aas    
        add    byte ptr [ecx], al
        add    al, byte ptr [ebx]
        add    al, 5
        push   es
        pop    es
        or     byte ptr [ecx], cl
        or     cl, byte ptr [ebx]
        or     al, 0xd
        push   cs
        movups xmm2, xmmword ptr [ecx]
        adc    dl, byte ptr [ebx]
        adc    al, 0x15
        push   ss
        pop    ss
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 0x1d
        push   ds
        pop    ds
        and    byte ptr [ecx], ah
        and    ah, byte ptr [ebx]
        and    al, 0x25
        daa    
        sub    byte ptr [ecx], ch
        sub    ch, byte ptr [ebx]
        sub    al, 0x2d
        das    
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        cmp    bh, byte ptr [ebx]
        cmp    al, 0x3d
        aas    
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
loc_1_0238:
        add    byte ptr [eax], 0
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax + 0x34000000], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], al
        aas    
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4], al
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     dword ptr [eax], 0
        add    byte ptr [eax], al
        aas    
        add    byte ptr [eax], al
        add    byte ptr [eax + edx + 0x3f000000], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x20], al
        aas    
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x200], cl
        retf   0
        db  000h, 08Eh, 000h, 004h, 000h, 000h, 03Fh, 000h, 000h, 000h, 093h, 000h, 008h, 000h, 000h, 03Fh
        db  000h, 000h, 000h, 094h, 000h, 010h, 000h, 000h, 03Fh, 000h, 000h, 000h, 095h, 000h, 020h, 000h
        db  000h, 041h, 000h, 000h, 000h, 097h, 000h, 040h, 000h, 000h, 03Fh, 000h, 000h, 000h, 096h, 000h
        db  001h, 000h, 000h, 03Fh, 000h, 000h, 000h, 08Dh, 000h, 000h
loc_1_02B8:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x10000], cl
        aas    
        add    byte ptr [eax], al
        db  000h, 098h, 000h, 000h
PPM_Control:
        cmp    eax, 0xb
        jne    loc_1_02DB
        push   ebx
        call   sub_1_1B96
        cmp    eax, 1
        ret    
loc_1_02DB:
        cmp    eax, 0x1b
        jne    loc_1_02E9
        call   sub_3_0080
loc_1_02E5:
        cmp    eax, 1
        ret    
loc_1_02E9:
        cmp    eax, 0x1c
        jne    loc_1_02F7
        call   sub_3_0113
loc_1_02F3:
        cmp    eax, 1
        ret    
loc_1_02F7:
        clc    
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_02FC:
        push   edx
        call   sub_1_10E0
        add    esp, 4
        ret    
        db  08Bh, 0C0h
loc_1_0308:
        push   edx
        call   sub_1_1042
        add    esp, 4
        ret    
        db  08Bh, 0C0h
loc_1_0314:
        push   edx
        push   ebx
        push   eax
        call   sub_1_0FC8
        add    esp, 0xc
        cmp    eax, 1
        ret    
        db  055h, 0A1h
        dd  obj1:0x0068              ; fixup
        db  08Bh, 0ECh, 083h, 0ECh, 004h, 050h, 0E8h, 0D8h, 024h, 000h, 000h, 089h, 045h, 0FCh, 085h, 0C0h
        db  074h, 022h, 08Bh, 045h, 0FCh, 068h
        dd  obj1:0x0050              ; fixup
        db  0E8h, 0BCh, 024h, 000h, 000h, 0FFh, 075h, 0FCh, 0FFh, 035h
        dd  obj1:0x0068              ; fixup
        db  0E8h, 09Eh, 024h, 000h, 000h, 089h, 045h, 0FCh, 085h, 0C0h, 075h, 0DEh, 0B8h, 001h, 000h, 000h
        db  000h, 08Bh, 0E5h, 05Dh, 0C3h, 0C3h
sub_1_0367:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jae    loc_1_0378
        xor    eax, eax
loc_1_0378:
        pop    edi
        pop    ebp
        ret    
sub_1_037B:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        test   edx, edx
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        je     loc_1_03D1
        mov    eax, dword ptr [ecx + 0xf8]
        mov    bl, byte ptr [ecx + 0x102]
        shr    bl, 4
        mov    al, byte ptr [eax]
        movzx  esi, bl
        shr    al, 4
        xor    ebx, ebx
        movzx  eax, al
        not    eax
        and    eax, esi
        mov    bl, byte ptr [ecx + 0xfc]
        and    ebx, 0x60
        mov    esi, dword ptr [ecx + 0x4c]
        or     eax, ebx
        mov    dword ptr [edx + 4], esi
        xor    ebx, ebx
        mov    bl, byte ptr [ecx + 0x100]
        and    ebx, 0x18
        or     eax, ebx
        mov    ebx, dword ptr [ecx + 0x58]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 8], ebx
loc_1_03D1:
        mov    eax, 1
        pop    esi
        mov    dword ptr [ecx + 0xc], 0
        pop    ebx
        pop    ebp
        ret    
sub_1_03E1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_040B
        push   esi
        call   sub_1_17B2
        add    esp, 4
        test   eax, eax
        jne    loc_1_040B
        mov    dword ptr [esi + 0xc], 0xfffffffb
        xor    eax, eax
        jmp    loc_1_0776
loc_1_040B:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x177], 0
        mov    dword ptr [esi + 0x94], 0x30
        test   cl, 1
        je     loc_1_0430
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x98], edx
loc_1_0430:
        mov    ebx, ecx
        and    ebx, 2
        je     loc_1_0440
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 0x9c], edx
loc_1_0440:
        test   cl, 4
        je     loc_1_044E
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0xa0], edx
loc_1_044E:
        test   cl, 8
        je     loc_1_045C
        mov    edx, dword ptr [eax + 0x10]
        mov    dword ptr [esi + 0xa4], edx
loc_1_045C:
        mov    edi, ecx
        and    edi, 0x70
        je     loc_1_047E
        mov    dl, byte ptr [eax + 0x16]
        mov    byte ptr [esi + 0xaa], dl
        mov    dl, byte ptr [eax + 0x17]
        mov    byte ptr [esi + 0xab], dl
        mov    dl, byte ptr [eax + 0x18]
        mov    byte ptr [esi + 0xac], dl
loc_1_047E:
        test   cl, 0x80
        je     loc_1_048C
        mov    dl, byte ptr [eax + 0x19]
        mov    byte ptr [esi + 0xad], dl
loc_1_048C:
        test   ch, 1
        je     loc_1_049A
        mov    dl, byte ptr [eax + 0x1a]
        mov    byte ptr [esi + 0xae], dl
loc_1_049A:
        test   ch, 2
        je     loc_1_04A8
        mov    dl, byte ptr [eax + 0x1b]
        mov    byte ptr [esi + 0xaf], dl
loc_1_04A8:
        test   ch, 4
        je     loc_1_04B6
        mov    dl, byte ptr [eax + 0x1c]
        mov    byte ptr [esi + 0xb0], dl
loc_1_04B6:
        test   ch, 8
        je     loc_1_04C4
        mov    dl, byte ptr [eax + 0x1d]
        mov    byte ptr [esi + 0xb1], dl
loc_1_04C4:
        test   ch, 0x10
        je     loc_1_04D2
        mov    dl, byte ptr [eax + 0x1e]
        mov    byte ptr [esi + 0xb2], dl
loc_1_04D2:
        test   ch, 0x20
        je     loc_1_04E0
        mov    edx, dword ptr [eax + 0x20]
        mov    dword ptr [esi + 0xb4], edx
loc_1_04E0:
        test   ch, 0x80
        je     loc_1_04EE
        mov    edx, dword ptr [eax + 0x28]
        mov    dword ptr [esi + 0xbc], edx
loc_1_04EE:
        test   ch, 0x40
        je     loc_1_04FC
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0xb8], edx
loc_1_04FC:
        test   ecx, 0x10000
        je     loc_1_050D
        mov    edx, dword ptr [eax + 0x2c]
        mov    dword ptr [esi + 0xc0], edx
loc_1_050D:
        test   ch, 0xe0
        mov    ax, word ptr [eax + 0x14]
        mov    word ptr [esi + 0xa8], ax
        je     loc_1_0587
        mov    edx, dword ptr [esi + 0xbc]
        mov    al, 0
        mov    cl, 0
        cmp    edx, 1
        adc    al, 0xff
        and    al, 0x20
        cmp    dword ptr [esi + 0xb8], 1
        adc    cl, 0xff
        and    cl, 0x10
        or     al, cl
        mov    cl, 0
        cmp    dword ptr [esi + 0xb4], 1
        adc    cl, 0xff
        and    cl, 0x80
        or     al, cl
        not    edx
        mov    byte ptr [esi + 0x103], al
        mov    eax, dword ptr [esi + 0xb4]
        not    eax
        cmp    eax, 1
        sbb    al, al
        and    al, 0x80
        cmp    edx, 1
        sbb    cl, cl
        and    cl, 0x20
        or     al, cl
        mov    ecx, dword ptr [esi + 0xb8]
        not    ecx
        cmp    ecx, 1
        sbb    cl, cl
        and    cl, 0x10
        or     al, cl
        mov    byte ptr [esi + 0x104], al
loc_1_0587:
        test   edi, edi
        je     loc_1_0617
        movzx  eax, byte ptr [esi + 0xaa]
        cmp    eax, 7
        je     loc_1_05A6
        mov    byte ptr [esi + 0xfe], 0xff
        mov    cl, 3
        jmp    loc_1_05AF
loc_1_05A6:
        mov    byte ptr [esi + 0xfe], 0x7f
        mov    cl, 2
loc_1_05AF:
        movzx  eax, byte ptr [esi + 0xac]
        cmp    eax, 2
        jne    loc_1_05BE
        or     cl, 4
loc_1_05BE:
        movzx  eax, byte ptr [esi + 0xab]
        cmp    eax, 1
        je     loc_1_05DB
        cmp    eax, 2
        je     loc_1_05E0
        cmp    eax, 3
        je     loc_1_05E5
        cmp    eax, 4
        je     loc_1_05EA
        jmp    loc_1_05ED
loc_1_05DB:
        or     cl, 8
        jmp    loc_1_05ED
loc_1_05E0:
        or     cl, 0x18
        jmp    loc_1_05ED
loc_1_05E5:
        or     cl, 0x28
        jmp    loc_1_05ED
loc_1_05EA:
        or     cl, 0x38
loc_1_05ED:
        test   cl, 8
        je     loc_1_05FB
        or     byte ptr [esi + 0x9c], 4
        jmp    loc_1_0602
loc_1_05FB:
        and    byte ptr [esi + 0x9c], 0xfb
loc_1_0602:
        cmp    byte ptr [esi + 0x179], cl
        je     loc_1_0617
        or     byte ptr [esi + 0x181], 4
        mov    byte ptr [esi + 0x179], cl
loc_1_0617:
        test   ebx, ebx
        je     loc_1_068F
        mov    ebx, dword ptr [esi + 0x9c]
        mov    eax, ebx
        mov    ecx, ebx
        and    eax, 2
        and    ecx, 0x80
        shr    ecx, 7
        or     al, cl
        mov    cl, al
        xor    cl, 3
        cmp    byte ptr [esi + 0x17a], cl
        je     loc_1_064D
        or     byte ptr [esi + 0x181], 1
        mov    byte ptr [esi + 0x17a], cl
loc_1_064D:
        mov    ecx, ebx
        mov    edx, ebx
        and    ecx, 0x2000
        cmp    ecx, 1
        sbb    cl, cl
        and    edx, 0x4000
        not    al
        inc    cl
        cmp    edx, 1
        mov    dl, 0
        adc    dl, 0xff
        and    dl, 2
        or     cl, dl
        and    cl, al
        mov    eax, ebx
        and    eax, 8
        and    ebx, 0x10
        add    eax, eax
        add    ebx, ebx
        or     al, bl
        mov    byte ptr [esi + 0x101], cl
        mov    byte ptr [esi + 0x102], al
loc_1_068F:
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, dword ptr [esi + 0xa4]
        mov    dword ptr [esi + 0xec], eax
        mov    al, byte ptr [esi + 0xfc]
        test   al, 2
        jne    loc_1_06B0
        or     al, 4
        mov    byte ptr [esi + 0xfc], al
loc_1_06B0:
        or     byte ptr [esi + 0x181], 0x10
        call   sub_1_2824
        mov    edi, eax
loc_1_06BE:
        cli    
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti    
        call   sub_1_2824
        sub    eax, edi
        cmp    eax, 0xc8
        jb     loc_1_06BE
        lea    edi, [esi + 0x146]
        push   edi
        call   sub_1_1D06
        push   edi
        call   sub_1_1D06
        mov    byte ptr [esi + 0x100], 0
        mov    ecx, dword ptr [esi + 0xf8]
        test   byte ptr [esi + 0x9d], 0x80
        mov    byte ptr [ecx], al
        jne    loc_1_0739
        mov    cl, byte ptr [esi + 0x102]
        mov    eax, dword ptr [esi + 0xf8]
        mov    al, byte ptr [eax]
        and    al, cl
        cmp    al, cl
        je     loc_1_0739
        cmp    cl, 0x30
        jne    loc_1_0732
        test   byte ptr [esi + 0xfc], 1
        jne    loc_1_0732
        cmp    al, 0x10
        jne    loc_1_0732
        and    cl, 0xdf
        mov    byte ptr [esi + 0x102], cl
        jmp    loc_1_0739
loc_1_0732:
        mov    byte ptr [esi + 0x100], 0xa0
loc_1_0739:
        cli    
        mov    byte ptr [esi + 0x177], 0xd
        push   edi
        call   sub_1_1D1C
        sti    
        test   byte ptr [esi + 0x11b], 2
        jne    loc_1_076A
        push   esi
        call   sub_1_0FB2
        add    esp, 4
        or     byte ptr [esi + 0x11b], 2
        push   esi
        call   sub_1_18FA
        add    esp, 4
loc_1_076A:
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
loc_1_0776:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_077B:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 6
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x4c]
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_1_07DC
        mov    bl, byte ptr [esi + 0x11b]
        test   bl, 8
        je     loc_1_07BF
        mov    ebx, dword ptr [esi + 0x3c]
        push   0
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        push   ebx
        call   sub_1_2834
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_07DF
        mov    eax, ebx
        mov    edi, dword ptr [esi + 0x40]
        jmp    loc_1_07DF
loc_1_07BF:
        push   0
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        call   sub_1_282C
        add    esp, 8
        test   eax, eax
        je     loc_1_07DF
        or     bl, 8
        mov    byte ptr [esi + 0x11b], bl
        jmp    loc_1_07DF
loc_1_07DC:
        mov    edi, dword ptr [ebp + 0x10]
loc_1_07DF:
        mov    dword ptr [esi + 0x3c], eax
        mov    dword ptr [esi + 0x40], edi
        mov    eax, dword ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_1_0831
        mov    bl, byte ptr [esi + 0x11b]
        test   bl, 4
        je     loc_1_0814
        mov    ebx, dword ptr [esi + 0x44]
        push   0
        mov    edi, dword ptr [ebp + 0x18]
        push   edi
        push   ebx
        call   sub_1_2834
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0834
        mov    eax, ebx
        mov    edi, dword ptr [esi + 0x48]
        jmp    loc_1_0834
loc_1_0814:
        push   0
        mov    edi, dword ptr [ebp + 0x18]
        push   edi
        call   sub_1_282C
        add    esp, 8
        test   eax, eax
        je     loc_1_0834
        or     bl, 4
        mov    byte ptr [esi + 0x11b], bl
        jmp    loc_1_0834
loc_1_0831:
        mov    edi, dword ptr [ebp + 0x18]
loc_1_0834:
        mov    dword ptr [esi + 0x44], eax
        mov    dword ptr [esi + 0x48], edi
        test   eax, eax
        je     loc_1_0845
        or     byte ptr [esi + 0x11b], 0x10
loc_1_0845:
        mov    eax, 1
        pop    edi
        mov    dword ptr [esi + 0xc], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_0856:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0xfc]
        test   al, 0x40
        je     loc_1_0872
        mov    dword ptr [esi + 0xc], 0x4000
        xor    eax, eax
        jmp    loc_1_089A
loc_1_0872:
        cli    
        or     al, 0x40
        mov    cl, byte ptr [ebp + 0xc]
        push   esi
        mov    byte ptr [esi + 0xfc], al
        mov    byte ptr [esi + 0xff], cl
        call   sub_1_191E
        add    esp, 4
        sti    
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
loc_1_089A:
        pop    esi
        pop    ebp
        ret    
loc_1_089D:
        push   ebp
        mov    ebp, esp
        push   esi
        cli    
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti    
        mov    ecx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   esi
        mov    dword ptr [eax], ecx
        call   sub_1_037B
        add    esp, 8
        pop    esi
        pop    ebp
        ret    
loc_1_08C6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        cmp    edi, 0xc8
        ja     loc_1_08E8
        je     loc_1_0A71
        cmp    edi, 0x15
        ja     loc_1_08E8
        jmp    dword ptr [edi*4]             ; -> obj1:0x0AD2
loc_1_08E8:
        mov    esi, dword ptr [ebp + 8]
loc_1_08EB:
        mov    dword ptr [esi + 0xc], 0xffffffec
        xor    eax, eax
        jmp    loc_1_0ACE
loc_1_08F9:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 0x100], 8
        jmp    loc_1_0A9C
loc_1_0908:
        mov    esi, dword ptr [ebp + 8]
        and    byte ptr [esi + 0x100], 0xf7
        jmp    loc_1_0A9C
loc_1_0917:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 0x17a]
        mov    al, cl
        or     al, 2
        cmp    al, cl
        je     loc_1_0A9C
        or     byte ptr [esi + 0x181], 1
        mov    byte ptr [esi + 0x17a], al
        jmp    loc_1_0A9C
loc_1_093E:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 0x17a]
        movzx  eax, cl
        mov    edx, eax
        and    dl, 0xfd
        cmp    edx, eax
        je     loc_1_0A9C
        or     byte ptr [esi + 0x181], 1
        and    cl, 0xfd
        mov    byte ptr [esi + 0x17a], cl
        jmp    loc_1_0A9C
loc_1_096C:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 0x17a]
        mov    al, cl
        or     al, 1
        cmp    al, cl
        je     loc_1_0A9C
        or     byte ptr [esi + 0x181], 1
        mov    byte ptr [esi + 0x17a], al
        jmp    loc_1_0A9C
loc_1_0993:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 0x17a]
        movzx  eax, cl
        mov    edx, eax
        and    dl, 0xfe
        cmp    edx, eax
        je     loc_1_0A9C
        or     byte ptr [esi + 0x181], 1
        and    cl, 0xfe
        mov    byte ptr [esi + 0x17a], cl
        jmp    loc_1_0A9C
loc_1_09C1:
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 0x113]
        shl    eax, 0x10
        or     eax, dword ptr [esi + 0xc4]
        mov    dword ptr [ecx], eax
        jmp    loc_1_0A9C
loc_1_09DD:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 0x100], 0x40
        mov    al, byte ptr [esi + 0x17a]
        or     al, 0x40
        cmp    byte ptr [esi + 0x179], al
        je     loc_1_0A9C
        or     byte ptr [esi + 0x181], 4
        mov    byte ptr [esi + 0x179], al
        jmp    loc_1_0A9C
loc_1_0A0D:
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, byte ptr [esi + 0x179]
        mov    cl, byte ptr [esi + 0x17a]
        and    byte ptr [esi + 0x100], 0xbf
        movzx  eax, cl
        and    al, 0xbf
        cmp    eax, edx
        je     loc_1_0A9C
        or     byte ptr [esi + 0x181], 4
        and    cl, 0xbf
        mov    byte ptr [esi + 0x179], cl
        jmp    loc_1_0A9C
loc_1_0A3F:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 0x11b], 0x20
        jmp    loc_1_0A9C
loc_1_0A4B:
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x12e], eax
        jmp    loc_1_0A9C
loc_1_0A59:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 0x11b], 0x40
        jmp    loc_1_0A9C
loc_1_0A65:
        mov    esi, dword ptr [ebp + 8]
        and    byte ptr [esi + 0x11b], 0xdf
        jmp    loc_1_0A9C
loc_1_0A71:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x4c], 0
        jbe    loc_1_0A8E
        mov    eax, dword ptr [esi + 0x3c]
        mov    ecx, dword ptr [esi + 0x50]
        movzx  edx, byte ptr [eax + ecx]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], edx
        jmp    loc_1_08EB
loc_1_0A8E:
        mov    dword ptr [esi + 0xc], 0
        xor    eax, eax
        jmp    loc_1_0ACE
loc_1_0A99:
        mov    esi, dword ptr [ebp + 8]
loc_1_0A9C:
        cli    
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti    
        cmp    edi, 0xa
        je     loc_1_0AC2
        cmp    edi, 0xc8
        je     loc_1_0AC2
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_0AC2
        mov    eax, dword ptr [esi + 0x34]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], eax
loc_1_0AC2:
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
loc_1_0ACE:
        pop    edi
        pop    esi
        pop    ebp
        ret    
        ; ---- dispatch table (22 entries) ----
        dd  loc_1_0A99
        dd  loc_1_08F9
        dd  loc_1_0908
        dd  loc_1_0917
        dd  loc_1_093E
        dd  loc_1_096C
        dd  loc_1_0993
        dd  loc_1_0A99
        dd  loc_1_08E8
        dd  loc_1_08E8
        dd  loc_1_09C1
        dd  loc_1_0A99
        dd  loc_1_09DD
        dd  loc_1_0A0D
        dd  loc_1_08E8
        dd  loc_1_08E8
        dd  loc_1_0A3F
        dd  loc_1_08E8
        dd  loc_1_08E8
        dd  loc_1_0A4B
        dd  loc_1_0A59
        dd  loc_1_0A65
loc_1_0B2A:
        push   ebp
        mov    ebp, esp
        push   esi
        cli    
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ecx + 0xf4]
        mov    edx, dword ptr [esi]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp + 0xc]
        not    eax
        and    eax, edx
        mov    dword ptr [esi], eax
        sti    
        mov    eax, 1
        pop    esi
        mov    dword ptr [ecx + 0xc], 0
        pop    ebp
        ret    
loc_1_0B58:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x11b], 0x10
        jne    loc_1_0B9D
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x44], eax
        xor    eax, eax
        mov    dword ptr [esi + 0x48], edi
        mov    dword ptr [esi + 0x58], edi
        mov    dword ptr [esi + 0x60], eax
        mov    dword ptr [esi + 0x5c], eax
        mov    dword ptr [ecx], edi
        test   edi, edi
        je     loc_1_0BEC
        cmp    dword ptr [esi + 0xe0], edi
        ja     loc_1_0BEC
        and    byte ptr [esi + 0xc9], 0xfd
        jmp    loc_1_0BEC
loc_1_0B9D:
        push   esi
        call   sub_1_1969
        add    esp, 4
        test   al, al
        je     loc_1_0BB5
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0
        jmp    loc_1_0BEC
loc_1_0BB5:
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_0D05
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 0x14]
        test   eax, eax
        mov    dword ptr [ecx], eax
        je     loc_1_0BE4
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0xe0], ecx
        ja     loc_1_0BE4
        and    byte ptr [esi + 0xc9], 0xfd
loc_1_0BE4:
        cmp    edi, eax
        je     loc_1_0BEC
        or     byte ptr [esi + 0x35], 1
loc_1_0BEC:
        cli    
        push   esi
        call   sub_1_191E
        add    esp, 4
        sti    
        mov    eax, 1
        pop    edi
        mov    dword ptr [esi + 0xc], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0C09:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x11b], 0x40
        jne    loc_1_0C34
        mov    eax, dword ptr [esi + 0x34]
        test   eax, eax
        je     loc_1_0C34
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0xc], eax
        xor    eax, eax
        mov    dword ptr [ecx], 0
        jmp    loc_1_0C8E
loc_1_0C34:
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_0EE2
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 0x14]
        test   eax, eax
        mov    dword ptr [ecx], eax
        je     loc_1_0C6E
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0xd4], eax
        jbe    loc_1_0C63
        and    byte ptr [esi + 0xc9], 0xfe
loc_1_0C63:
        push   esi
        call   sub_1_1B18
        add    esp, 4
        jmp    loc_1_0C82
loc_1_0C6E:
        test   byte ptr [esi + 0x9c], 1
        jne    loc_1_0C82
        test   byte ptr [esi + 0xfc], 0x20
        je     loc_1_0C82
        mov    dword ptr [ecx], edi
loc_1_0C82:
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
loc_1_0C8E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_0C94:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   esi
        push   eax
        call   sub_1_280C
        test   eax, eax
        je     loc_1_0CCF
        mov    ebx, 0x20
        mov    esi, 0xffffffff
loc_1_0CAF:
        test   byte ptr [eax + 0x11b], bl
        jne    loc_1_0CBF
        cmp    dword ptr [eax + 0xd4], esi
        jne    loc_1_0CE7
loc_1_0CBF:
        push   eax
        push   dword ptr [0]                 ; -> obj1:0x0068
        call   sub_1_27F4
        test   eax, eax
        jne    loc_1_0CAF
loc_1_0CCF:
        mov    eax, dword ptr [0]            ; -> obj1:0x0064
        push   eax
        call   sub_1_2858
        mov    dword ptr [0], 0              ; -> obj1:0x0064
loc_1_0CE4:
        pop    esi
        pop    ebx
        ret    
loc_1_0CE7:
        cmp    dword ptr [0], 0              ; -> obj1:0x0064
        jne    loc_1_0CE4
        push   0
        push   0x64
        push   0                             ; -> loc_1_1A89
        call   sub_1_283C
        mov    dword ptr [0], eax            ; -> obj1:0x0064
        jmp    loc_1_0CE4
sub_1_0D05:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [ecx + 0x58]
        mov    edx, dword ptr [ecx + 0x48]
        push   ebx
        mov    dword ptr [ebp - 8], eax
        push   esi
        mov    dword ptr [ebp - 4], edx
        push   edi
        cmp    eax, edx
        jae    loc_1_0DA7
        mov    eax, edx
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp + 0x10]
        jae    loc_1_0D33
        mov    dword ptr [ebp + 0x10], eax
loc_1_0D33:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ecx + 0x60]
        add    eax, edi
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_0D4F
        mov    edx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 0x10]
        sub    edx, edi
        sub    ebx, edx
        jmp    loc_1_0D54
loc_1_0D4F:
        mov    edx, dword ptr [ebp + 0x10]
        xor    ebx, ebx
loc_1_0D54:
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx + 0x44]
        add    edi, ecx
        mov    dword ptr [ebp - 0xc], ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        test   ebx, ebx
        je     loc_1_0D8A
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp - 0xc]
        add    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0D8A:
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        div    dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x60], edx
        cli    
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0x14]
        add    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x58], edx
        mov    dword ptr [eax], edx
        sti    
        jmp    loc_1_0DAE
loc_1_0DA7:
        mov    dword ptr [ebp + 0x10], 0
loc_1_0DAE:
        mov    eax, dword ptr [ebp + 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_0DB8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x58]
        test   eax, eax
        je     loc_1_0E3D
        cmp    dword ptr [ebp + 0x10], eax
        jbe    loc_1_0DD0
        mov    dword ptr [ebp + 0x10], eax
loc_1_0DD0:
        mov    ecx, dword ptr [ebx + 0x5c]
        mov    edx, dword ptr [ebx + 0x48]
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, ecx
        cmp    eax, edx
        jbe    loc_1_0DE8
        sub    edx, ecx
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, edx
        jmp    loc_1_0DED
loc_1_0DE8:
        mov    edx, dword ptr [ebp + 0x10]
        xor    eax, eax
loc_1_0DED:
        mov    esi, dword ptr [ebx + 0x44]
        mov    edi, dword ptr [ebp + 0xc]
        add    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        test   eax, eax
        je     loc_1_0E1D
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebx + 0x44]
        add    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0E1D:
        mov    eax, dword ptr [ebx + 0x5c]
        sub    edx, edx
        add    eax, dword ptr [ebp + 0x10]
        div    dword ptr [ebx + 0x48]
        mov    dword ptr [ebx + 0x5c], edx
        cli    
        mov    ecx, dword ptr [ebx + 0x58]
        mov    eax, dword ptr [ebp + 0x14]
        sub    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 0x58], ecx
        mov    dword ptr [eax], ecx
        sti    
        jmp    loc_1_0E44
loc_1_0E3D:
        mov    dword ptr [ebp + 0x10], 0
loc_1_0E44:
        mov    eax, dword ptr [ebp + 0x10]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x4c]
        mov    edx, dword ptr [ebx + 0x40]
        cmp    eax, edx
        jae    loc_1_0ED3
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, dword ptr [ebp + 0x10]
        jae    loc_1_0E6B
        mov    dword ptr [ebp + 0x10], ecx
loc_1_0E6B:
        mov    edi, dword ptr [ebx + 0x54]
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, edi
        cmp    eax, edx
        jbe    loc_1_0E80
        sub    edx, edi
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, edx
        jmp    loc_1_0E85
loc_1_0E80:
        mov    edx, dword ptr [ebp + 0x10]
        xor    eax, eax
loc_1_0E85:
        add    edi, dword ptr [ebx + 0x3c]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        test   eax, eax
        je     loc_1_0EB3
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebx + 0x3c]
        add    esi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0EB3:
        mov    eax, dword ptr [ebx + 0x54]
        sub    edx, edx
        add    eax, dword ptr [ebp + 0x10]
        div    dword ptr [ebx + 0x40]
        mov    dword ptr [ebx + 0x54], edx
        cli    
        mov    ecx, dword ptr [ebx + 0x4c]
        mov    edx, dword ptr [ebp + 0x14]
        add    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 0x4c], ecx
        mov    dword ptr [edx], ecx
        sti    
        jmp    loc_1_0EDA
loc_1_0ED3:
        mov    dword ptr [ebp + 0x10], 0
loc_1_0EDA:
        mov    eax, dword ptr [ebp + 0x10]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_0EE2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x4c]
        test   eax, eax
        je     loc_1_0F67
        cmp    eax, dword ptr [ebp + 0x10]
        jae    loc_1_0EFA
        mov    dword ptr [ebp + 0x10], eax
loc_1_0EFA:
        mov    ecx, dword ptr [ebx + 0x50]
        mov    edx, dword ptr [ebx + 0x40]
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, ecx
        cmp    eax, edx
        jbe    loc_1_0F12
        sub    edx, ecx
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, edx
        jmp    loc_1_0F17
loc_1_0F12:
        mov    edx, dword ptr [ebp + 0x10]
        xor    eax, eax
loc_1_0F17:
        mov    esi, dword ptr [ebx + 0x3c]
        mov    edi, dword ptr [ebp + 0xc]
        add    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        test   eax, eax
        je     loc_1_0F47
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebx + 0x3c]
        add    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0F47:
        mov    eax, dword ptr [ebx + 0x50]
        sub    edx, edx
        add    eax, dword ptr [ebp + 0x10]
        div    dword ptr [ebx + 0x40]
        mov    dword ptr [ebx + 0x50], edx
        cli    
        mov    ecx, dword ptr [ebx + 0x4c]
        mov    eax, dword ptr [ebp + 0x14]
        sub    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 0x4c], ecx
        mov    dword ptr [eax], ecx
        sti    
        jmp    loc_1_0F6E
loc_1_0F67:
        mov    dword ptr [ebp + 0x10], 0
loc_1_0F6E:
        mov    eax, dword ptr [ebp + 0x10]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_0F76:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xc4]
        lea    edx, [ecx + 2]
        in     al, dx
        test   al, 0x10
        je     loc_1_0F98
        lea    edx, [ecx + 1]
        in     al, dx
        test   al, 0x40
        mov    eax, 1
        jne    loc_1_0F9A
loc_1_0F98:
        xor    eax, eax
loc_1_0F9A:
        pop    ebp
        ret    
sub_1_0F9C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0xc4]
        add    edx, 2
        pop    ebp
        in     al, dx
        and    al, 0xef
        out    dx, al
        ret    
sub_1_0FB2:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0xc4]
        add    edx, 2
        pop    ebp
        in     al, dx
        or     al, 0x10
        out    dx, al
        ret    
sub_1_0FC8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        test   byte ptr [esi + 0x3b], 1
        jne    loc_1_103B
        push   esi
        call   sub_1_0F76
        add    esp, 4
        test   eax, eax
        je     loc_1_1022
        push   esi
        call   sub_1_0F9C
        add    esp, 4
        mov    eax, dword ptr [esi + 0x13e]
        test   eax, eax
        je     loc_1_1008
        push   eax
        call   sub_1_2858
        mov    dword ptr [esi + 0x13e], 0
loc_1_1008:
        cmp    dword ptr [esi + 0x142], 0
        jne    loc_1_1022
        push   esi
        push   0                             ; -> loc_1_02FC
        call   sub_1_2868
        mov    dword ptr [esi + 0x142], eax
loc_1_1022:
        mov    eax, dword ptr [esi + 0x117]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    eax, 1
        jmp    loc_1_103D
loc_1_103B:
        xor    eax, eax
loc_1_103D:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1042:
        push   ebp
        mov    ebp, esp
        push   esi
        cli    
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x13e], 0
        cmp    dword ptr [esi + 0x142], 0
        jne    loc_1_1085
        cmp    dword ptr [esi + 0x117], 0
        je     loc_1_106F
        push   esi
        call   sub_1_0F9C
        add    esp, 4
loc_1_106F:
        mov    dword ptr [esi + 0x142], 0xffffffff
        sti    
        push   esi
        call   sub_1_10E0
        add    esp, 4
        jmp    loc_1_1086
loc_1_1085:
        sti    
loc_1_1086:
        pop    esi
        pop    ebp
        ret    
sub_1_1089:
        push   ebp
        mov    ebp, esp
        push   esi
        cli    
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x142], 0
        jne    loc_1_10DC
        cmp    dword ptr [esi + 0x117], 0
        je     loc_1_10AC
        push   esi
        call   sub_1_0F9C
        add    esp, 4
loc_1_10AC:
        mov    eax, dword ptr [esi + 0x13e]
        test   eax, eax
        je     loc_1_10C6
        push   eax
        call   sub_1_2858
        mov    dword ptr [esi + 0x13e], 0
loc_1_10C6:
        mov    dword ptr [esi + 0x142], 0xffffffff
        sti    
        push   esi
        call   sub_1_10E0
        add    esp, 4
        jmp    loc_1_10DD
loc_1_10DC:
        sti    
loc_1_10DD:
        pop    esi
        pop    ebp
        ret    
sub_1_10E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xf4]
        lea    ecx, [esi + 0x146]
        push   ecx
        mov    edi, dword ptr [eax]
        call   sub_1_1D32
        mov    ebx, eax
        mov    ecx, dword ptr [esi + 0xf0]
        mov    eax, dword ptr [esi + 0xf4]
        and    ecx, dword ptr [eax]
        test   byte ptr [esi + 0xc9], 0x40
        mov    dword ptr [eax], ecx
        je     loc_1_1129
        not    edi
        test   edi, ecx
        je     loc_1_1129
        push   4
        push   esi
        call   sub_1_1A00
        add    esp, 8
loc_1_1129:
        cli    
        test   ebx, ebx
        je     loc_1_113C
        lea    eax, [esi + 0x13a]
        mov    dword ptr [eax], 0x3e8
        jmp    loc_1_1168
loc_1_113C:
        cmp    dword ptr [esi + 0x117], 0
        je     loc_1_115C
        push   esi
        call   sub_1_0FB2
        add    esp, 4
        lea    eax, [esi + 0x13a]
        mov    dword ptr [eax], 0x64
        jmp    loc_1_1168
loc_1_115C:
        lea    eax, [esi + 0x13a]
        mov    dword ptr [eax], 0xa
loc_1_1168:
        push   esi
        push   dword ptr [eax]
        push   0                             ; -> loc_1_0308
        call   sub_1_283C
        mov    dword ptr [esi + 0x142], 0
        mov    dword ptr [esi + 0x13e], eax
        sti    
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_118B:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [eax + 0xf8]
        push   edi
        mov    dl, byte ptr [ebp + 0xc]
        mov    esi, dword ptr [eax + 0xf4]
        mov    byte ptr [ecx], dl
        mov    cl, dl
        mov    bl, dl
        and    cl, 0x80
        mov    edi, 0
        cmp    cl, 1
        mov    ecx, 0
        adc    ecx, -1
        and    bl, 0x40
        and    ecx, 0x1000
        cmp    bl, 1
        adc    edi, -1
        mov    bl, dl
        and    edi, 0x100
        and    bl, 2
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 4
        and    edi, 0x10
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 8
        and    edi, 0x2000
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 1
        and    edi, 0x20
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 0x20
        and    edi, 8
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 0x10
        and    edi, 0x800
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        adc    edi, -1
        and    edi, 0x400
        or     ecx, edi
        or     dword ptr [esi], ecx
        mov    cl, byte ptr [eax + 0x102]
        test   cl, cl
        je     loc_1_127F
        and    dl, cl
        cmp    dl, cl
        jne    loc_1_1276
        and    byte ptr [eax + 0x100], 0x5f
        jmp    loc_1_127F
loc_1_1276:
        or     byte ptr [eax + 0x100], 0x20
        jmp    loc_1_128C
loc_1_127F:
        cmp    dword ptr [eax + 0x58], 0
        jbe    loc_1_128C
        or     byte ptr [eax + 0x177], 2
loc_1_128C:
        push   eax
        call   sub_1_27EC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_129C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [eax + 0xf4]
        mov    bl, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx]
        or     cl, 0x80
        test   bl, 0xe
        mov    dword ptr [edx], ecx
        je     loc_1_12BF
        mov    byte ptr [eax + 0x106], bl
loc_1_12BF:
        test   bl, 0x10
        je     loc_1_12C9
        or     cl, 0x40
        mov    dword ptr [edx], ecx
loc_1_12C9:
        cmp    dword ptr [eax + 0x58], 0
        jbe    loc_1_12D6
        or     byte ptr [eax + 0x177], 2
loc_1_12D6:
        push   eax
        call   sub_1_27EC
        add    esp, 4
        pop    ebx
        pop    ebp
        ret    8
loc_1_12E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_2824
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x12e]
        and    byte ptr [esi + 0xc9], 0x7f
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [esi + 0x106]
        mov    byte ptr [ebp - 1], al
        test   al, al
        je     loc_1_1327
        mov    byte ptr [esi + 0x106], 0
        mov    al, byte ptr [esi + 0xfd]
        and    al, byte ptr [ebp - 1]
        movzx  ecx, al
        or     dword ptr [esi + 0x34], ecx
loc_1_1327:
        mov    eax, dword ptr [esi + 0xf4]
        mov    dword ptr [ebp - 8], 0
        or     byte ptr [eax], 1
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_1550
loc_1_1341:
        mov    eax, dword ptr [esi + 0x4c]
        cmp    dword ptr [esi + 0x40], eax
        jbe    loc_1_1550
        mov    dl, byte ptr [esi + 0xfc]
        test   dl, 0x20
        jne    loc_1_1550
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp + 0xc]
        dec    dword ptr [ebp + 0x10]
        test   byte ptr [ebp - 1], 4
        mov    cl, byte ptr [eax]
        je     loc_1_145B
        test   byte ptr [esi + 0x9d], 4
        je     loc_1_145B
        mov    cl, byte ptr [esi + 0xaf]
loc_1_1387:
        movsx  eax, byte ptr [esi + 0xb1]
        movzx  edx, cl
        cmp    eax, edx
        jne    loc_1_139E
        mov    eax, dword ptr [esi + 0xf4]
        or     byte ptr [eax], 2
loc_1_139E:
        test   byte ptr [esi + 0x11b], 1
        jne    loc_1_1546
        mov    edx, dword ptr [esi + 0x3c]
        mov    eax, dword ptr [esi + 0x54]
        mov    byte ptr [edx + eax], cl
        mov    eax, dword ptr [esi + 0x54]
        inc    eax
        mov    dword ptr [esi + 0x54], eax
        cmp    dword ptr [esi + 0x40], eax
        ja     loc_1_13C7
        mov    dword ptr [esi + 0x54], 0
loc_1_13C7:
        mov    edi, dword ptr [esi + 0x4c]
        inc    edi
        mov    dword ptr [esi + 0x4c], edi
        cmp    dword ptr [esi + 0xec], edi
        ja     loc_1_1432
        mov    dl, byte ptr [esi + 0x100]
        test   dl, 0x12
        jne    loc_1_1432
        mov    cl, byte ptr [esi + 0x101]
        test   cl, cl
        je     loc_1_141B
        movzx  eax, byte ptr [esi + 0x17a]
        movzx  ebx, cl
        not    ebx
        and    ebx, eax
        cmp    ebx, eax
        je     loc_1_1412
        not    cl
        or     byte ptr [esi + 0x181], 1
        and    cl, byte ptr [esi + 0x17a]
        mov    byte ptr [esi + 0x17a], cl
loc_1_1412:
        or     dl, 2
        mov    byte ptr [esi + 0x100], dl
loc_1_141B:
        test   byte ptr [esi + 0x9d], 2
        je     loc_1_1432
        or     byte ptr [esi + 0x100], 1
        or     byte ptr [esi + 0x177], 2
loc_1_1432:
        cmp    dword ptr [esi + 0xd4], edi
        ja     loc_1_153F
        test   byte ptr [esi + 0xc9], 1
        jne    loc_1_1546
        push   1
        push   esi
        call   sub_1_1A00
        add    esp, 8
        jmp    loc_1_1546
loc_1_145B:
        and    cl, byte ptr [esi + 0xfe]
        mov    edi, dword ptr [esi + 0x9c]
        test   edi, 0x800
        je     loc_1_1477
        test   cl, cl
        je     loc_1_1546
loc_1_1477:
        test   edi, 1
        jne    loc_1_149B
        movsx  eax, byte ptr [esi + 0xb0]
        movzx  ebx, cl
        cmp    eax, ebx
        jne    loc_1_149B
        or     dl, 0x20
        mov    byte ptr [esi + 0xfc], dl
        jmp    loc_1_1546
loc_1_149B:
        mov    eax, edi
        and    eax, 0x100
        je     loc_1_14F8
        movsx  edx, byte ptr [esi + 0xae]
        movzx  ebx, cl
        cmp    edx, ebx
        jne    loc_1_14F8
        mov    cl, byte ptr [esi + 0x100]
        or     cl, 8
        test   edi, 0x60
        mov    byte ptr [esi + 0x100], cl
        je     loc_1_1546
        mov    eax, dword ptr [esi + 0x4c]
        cmp    dword ptr [esi + 0xa0], eax
        jb     loc_1_1546
        or     byte ptr [esi + 0x177], 2
        and    cl, 0xf7
        mov    byte ptr [esi + 0x100], cl
        and    cl, 0xef
        mov    byte ptr [esi + 0x100], cl
        or     cl, 4
        mov    byte ptr [esi + 0x100], cl
        jmp    loc_1_1546
loc_1_14F8:
        test   eax, eax
        je     loc_1_1387
        movsx  eax, byte ptr [esi + 0xad]
        movzx  edx, cl
        cmp    eax, edx
        jne    loc_1_1387
        mov    al, byte ptr [esi + 0x100]
        and    al, 0xf7
        test   edi, 0x60
        mov    byte ptr [esi + 0x100], al
        je     loc_1_1530
        and    al, 0xf7
        mov    byte ptr [esi + 0x100], al
loc_1_1530:
        cmp    dword ptr [esi + 0x58], 0
        jbe    loc_1_1546
        or     byte ptr [esi + 0x177], 2
        jmp    loc_1_1546
loc_1_153F:
        and    byte ptr [esi + 0xc9], 0xfe
loc_1_1546:
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_1341
loc_1_1550:
        cmp    dword ptr [esi + 0x58], 0
        jbe    loc_1_155D
        or     byte ptr [esi + 0x177], 2
loc_1_155D:
        push   esi
        call   sub_1_27ED
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1572:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    al, byte ptr [esi + 0x100]
        test   al, 0x60
        jne    loc_1_16FF
        mov    ecx, dword ptr [esi + 0x9c]
        mov    dword ptr [ebp - 0xc], ecx
        and    ecx, 0x60
        mov    dword ptr [ebp - 0x10], ecx
        cmp    ecx, edi
        jne    loc_1_15F9
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    ecx, edi
        je     loc_1_15C8
        test   al, 4
        je     loc_1_15C8
        and    al, 0xeb
        mov    dl, byte ptr [esi + 0xad]
        mov    byte ptr [esi + 0x100], al
        mov    eax, dword ptr [ebp + 0xc]
        dec    ecx
        mov    edi, 1
        mov    byte ptr [eax], dl
        inc    eax
        jmp    loc_1_15CB
loc_1_15C8:
        mov    eax, dword ptr [ebp + 0xc]
loc_1_15CB:
        test   ecx, ecx
        je     loc_1_15FF
        mov    dl, byte ptr [esi + 0x100]
        test   dl, 1
        je     loc_1_15FF
        and    dl, 0xfe
        mov    bl, byte ptr [esi + 0xae]
        dec    ecx
        inc    edi
        mov    byte ptr [esi + 0x100], dl
        mov    byte ptr [eax], bl
        or     dl, 0x10
        inc    eax
        mov    byte ptr [esi + 0x100], dl
        jmp    loc_1_15FF
loc_1_15F9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
loc_1_15FF:
        mov    dl, byte ptr [esi + 0x100]
        test   dl, 0x6d
        mov    byte ptr [ebp - 8], dl
        jne    loc_1_16FF
        test   ecx, ecx
        je     loc_1_1634
        mov    dl, byte ptr [esi + 0xfc]
        test   dl, 0x40
        je     loc_1_1634
        and    dl, 0xbf
        mov    bl, byte ptr [esi + 0xff]
        dec    ecx
        inc    edi
        mov    byte ptr [esi + 0xfc], dl
        mov    byte ptr [eax], bl
        inc    eax
loc_1_1634:
        mov    edx, dword ptr [esi + 0x58]
        test   edx, edx
        jne    loc_1_1644
        mov    ebx, dword ptr [esi + 0xf4]
        or     byte ptr [ebx], 4
loc_1_1644:
        test   ecx, ecx
        je     loc_1_16FF
        test   edx, edx
        je     loc_1_16FF
        test   byte ptr [esi + 0xfc], 0x12
        jne    loc_1_1676
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1676
        lea    edx, [ebp - 4]
        push   edx
        push   ecx
        push   eax
        push   esi
        call   sub_1_0DB8
        add    esp, 0x10
        add    edi, eax
        jmp    loc_1_16D6
loc_1_1676:
        test   byte ptr [ebp - 0xc], 0x40
        je     loc_1_16AB
        mov    ecx, dword ptr [esi + 0x107]
        cmp    dword ptr [esi + 0xa0], ecx
        jne    loc_1_16AB
        mov    cl, byte ptr [ebp - 8]
        mov    dl, byte ptr [esi + 0xae]
        or     cl, 8
        inc    edi
        mov    byte ptr [esi + 0x100], cl
        mov    byte ptr [eax], dl
        mov    dword ptr [esi + 0x107], 0
        jmp    loc_1_16D0
loc_1_16AB:
        mov    ebx, dword ptr [esi + 0x5c]
        mov    ecx, dword ptr [esi + 0x44]
        inc    dword ptr [esi + 0x107]
        mov    cl, byte ptr [ecx + ebx]
        inc    ebx
        mov    byte ptr [eax], cl
        mov    dword ptr [esi + 0x5c], ebx
        cmp    dword ptr [esi + 0x48], ebx
        ja     loc_1_16CC
        mov    dword ptr [esi + 0x5c], 0
loc_1_16CC:
        dec    edx
        mov    dword ptr [esi + 0x58], edx
loc_1_16D0:
        mov    eax, dword ptr [esi + 0x58]
        mov    dword ptr [ebp - 4], eax
loc_1_16D6:
        mov    eax, dword ptr [esi + 0xf4]
        or     byte ptr [eax + 1], 0x40
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0xe0], ecx
        jbe    loc_1_16FF
        test   byte ptr [esi + 0xc9], 2
        jne    loc_1_16FF
        push   2
        push   esi
        call   sub_1_1A00
        add    esp, 8
loc_1_16FF:
        test   edi, edi
        jne    loc_1_170A
        and    byte ptr [esi + 0x177], 0xfd
loc_1_170A:
        push   esi
        call   sub_1_27EE
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_171E:
        push   ebp
        mov    cl, byte ptr [esp + 0xc]
        movzx  eax, cl
        mov    ebp, esp
        and    al, 0xbf
        cmp    eax, 0xaf
        je     loc_1_1738
        cmp    eax, 0xbf
        jne    loc_1_1744
loc_1_1738:
        push   dword ptr [ebp + 0x10]
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_1_27EF
loc_1_1744:
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_174A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x126]
        test   ecx, ecx
        je     loc_1_1783
        mov    eax, dword ptr [esi + 0x12a]
        test   eax, eax
        je     loc_1_1783
        push   1
        push   esi
        push   0                             ; -> loc_1_1895
        push   eax
        push   1
        call   ecx
        add    esp, 0x14
        mov    dword ptr [esi + 0x10b], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    loc_1_1788
loc_1_1783:
        mov    eax, 1
loc_1_1788:
        pop    esi
        pop    ebp
        ret    
sub_1_178B:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x10b], 0
        je     loc_1_17AE
        push   0                             ; -> loc_1_1895
        push   dword ptr [eax + 0x12a]
        push   3
        call   dword ptr [eax + 0x126]
loc_1_17AE:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_17B2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 0x3b]
        test   bl, 1
        je     loc_1_17E8
        push   0                             ; -> loc_1_1895
        push   dword ptr [esi + 0x12a]
        push   2
        call   dword ptr [esi + 0x126]
        add    esp, 0xc
        test   eax, eax
        je     loc_1_17E4
        and    bl, 0xfe
        mov    byte ptr [esi + 0x3b], bl
        jmp    loc_1_17E8
loc_1_17E4:
        xor    eax, eax
        jmp    loc_1_17ED
loc_1_17E8:
        mov    eax, 1
loc_1_17ED:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_17F1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        cli    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x142]
        test   eax, eax
        je     loc_1_1816
        push   eax
        call   sub_1_2880
        mov    dword ptr [esi + 0x142], 0
loc_1_1816:
        mov    eax, dword ptr [esi + 0x13e]
        test   eax, eax
        je     loc_1_1830
        push   eax
        call   sub_1_2858
        mov    dword ptr [esi + 0x13e], 0
loc_1_1830:
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_1843
        push   esi
        call   sub_1_17B2
        add    esp, 4
        test   eax, eax
        je     loc_1_1855
loc_1_1843:
        push   esi
        call   sub_1_18AF
        add    esp, 4
        push   esi
        call   sub_1_0F9C
        add    esp, 4
loc_1_1855:
        mov    eax, dword ptr [esi + 0x117]
        test   eax, eax
        je     loc_1_1875
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        mov    dword ptr [esi + 0x117], 0
loc_1_1875:
        sti    
        lea    eax, [esi + 0x146]
        push   eax
        call   sub_1_1CAD
        push   esi
        call   sub_1_178B
        add    esp, 4
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1895:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_18A9
        or     byte ptr [eax + 0x3b], 1
        jmp    loc_1_18AD
loc_1_18A9:
        and    byte ptr [eax + 0x3b], 0xfe
loc_1_18AD:
        pop    ebp
        ret    
sub_1_18AF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        mov    ecx, dword ptr [eax + 0x117]
        push   ebx
        test   ecx, ecx
        je     loc_1_18F3
        mov    eax, dword ptr [0]            ; -> obj1:0x0060
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ebx, dword ptr [ebp - 8]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        xchg   ecx, eax
        test   al, 4
        jne    loc_1_18F3
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        mov    eax, 1
        jmp    loc_1_18F5
loc_1_18F3:
        xor    eax, eax
loc_1_18F5:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_18FA:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [eax + 0x117]
        test   eax, eax
        je     loc_1_191A
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_1_191A:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_191E:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_194B
        push   esi
        call   sub_1_17B2
        add    esp, 4
        test   eax, eax
        jne    loc_1_194B
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x5c]
        mov    dword ptr [esi + 0x58], eax
        mov    dword ptr [esi + 0x107], eax
        mov    dword ptr [esi + 0x60], ecx
        jmp    loc_1_1966
loc_1_194B:
        mov    al, byte ptr [esi + 0x177]
        test   al, 2
        jne    loc_1_1966
        or     al, 2
        push   esi
        mov    byte ptr [esi + 0x177], al
        call   sub_1_1089
        add    esp, 4
loc_1_1966:
        pop    esi
        pop    ebp
        ret    
sub_1_1969:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    byte ptr [ebp - 1], al
        push   esi
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp + 8]
loc_1_197C:
        mov    dword ptr [ebp - 0xc], 0xb
loc_1_1983:
        mov    eax, dword ptr [esi + 0xf8]
        mov    bl, byte ptr [eax]
        not    bl
        and    bl, byte ptr [esi + 0x103]
        je     loc_1_19EC
        cli    
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti    
        test   bl, 0x80
        je     loc_1_19B4
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0xb4], eax
        ja     loc_1_19B4
        or     byte ptr [ebp - 1], 0x80
loc_1_19B4:
        test   bl, 0x20
        je     loc_1_19C8
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0xbc], eax
        ja     loc_1_19C8
        or     byte ptr [ebp - 1], 0x40
loc_1_19C8:
        test   bl, 0x10
        je     loc_1_19DC
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0xb8], eax
        ja     loc_1_19DC
        or     byte ptr [ebp - 1], 0x20
loc_1_19DC:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_19F0
        dec    dword ptr [ebp - 0xc]
        jne    loc_1_1983
        inc    dword ptr [ebp - 8]
        jmp    loc_1_197C
loc_1_19EC:
        xor    al, al
        jmp    loc_1_19FA
loc_1_19F0:
        movzx  eax, byte ptr [ebp - 1]
        or     dword ptr [esi + 0x34], eax
        mov    al, byte ptr [ebp - 1]
loc_1_19FA:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1A00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pushal 
        movzx  edx, byte ptr [ebp + 0xc]
        cmp    edx, 1
        je     loc_1_1A1C
        cmp    edx, 2
        je     loc_1_1A3E
        cmp    edx, 4
        je     loc_1_1A60
        jmp    loc_1_1A83
loc_1_1A1C:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xf4]
        or     byte ptr [ecx + 0xc9], 1
        push   dword ptr [eax]
        push   edx
        push   dword ptr [ecx + 0xdc]
        push   ecx
        call   dword ptr [ecx + 0xd8]
        jmp    loc_1_1A80
loc_1_1A3E:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xf4]
        or     byte ptr [ecx + 0xc9], 2
        push   dword ptr [eax]
        push   edx
        push   dword ptr [ecx + 0xe8]
        push   ecx
        call   dword ptr [ecx + 0xe4]
        jmp    loc_1_1A80
loc_1_1A60:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xf4]
        or     byte ptr [ecx + 0xc9], 4
        push   dword ptr [eax]
        push   edx
        push   dword ptr [ecx + 0xd0]
        push   ecx
        call   dword ptr [ecx + 0xcc]
loc_1_1A80:
        add    esp, 0x10
loc_1_1A83:
        popal  
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_1A89:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   esi
        push   edi
        xor    esi, esi
        push   eax
        mov    dword ptr [0], esi            ; -> obj1:0x0064
        call   sub_1_280C
        cmp    eax, esi
        mov    ebx, eax
        je     loc_1_1B01
        mov    edi, 0xffffffff
loc_1_1AAA:
        test   byte ptr [ebx + 0x11b], 0x20
        jne    loc_1_1AEF
        cmp    dword ptr [ebx + 0xd4], edi
        je     loc_1_1AEF
        cmp    dword ptr [ebx + 0x4c], esi
        je     loc_1_1AEF
        mov    eax, dword ptr [ebx + 0xc8]
        test   ah, 1
        jne    loc_1_1AEF
        test   ah, 0x80
        je     loc_1_1AE6
        and    ah, 0x7f
        push   1
        push   ebx
        mov    dword ptr [ebx + 0xc8], eax
        call   sub_1_1A00
        add    esp, 8
        jmp    loc_1_1AEF
loc_1_1AE6:
        or     ah, 0x80
        mov    dword ptr [ebx + 0xc8], eax
loc_1_1AEF:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        call   sub_1_27F4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_1AAA
loc_1_1B01:
        push   0
        push   0x64
        push   0                             ; -> loc_1_1A89
        call   sub_1_283C
        pop    edi
        mov    dword ptr [0], eax            ; -> obj1:0x0064
        pop    esi
        pop    ebx
        ret    
sub_1_1B18:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [edx + 0x9c]
        and    esi, 0x60
        je     loc_1_1B37
        lea    ecx, [edx + 0x100]
        test   byte ptr [ecx], 0x12
        jne    loc_1_1B46
loc_1_1B37:
        test   esi, esi
        jne    loc_1_1B92
        lea    ecx, [edx + 0x100]
        test   byte ptr [ecx], 0x12
        je     loc_1_1B92
loc_1_1B46:
        mov    eax, dword ptr [edx + 0x4c]
        cmp    dword ptr [edx + 0xa0], eax
        jb     loc_1_1B92
        cli    
        mov    al, byte ptr [edx + 0x101]
        test   al, al
        je     loc_1_1B78
        mov    bl, byte ptr [edx + 0x17a]
        or     al, bl
        cmp    al, bl
        je     loc_1_1B75
        or     byte ptr [edx + 0x181], 1
        mov    byte ptr [edx + 0x17a], al
loc_1_1B75:
        and    byte ptr [ecx], 0xfd
loc_1_1B78:
        mov    al, byte ptr [ecx]
        test   esi, esi
        test   al, 0x10
        je     loc_1_1B88
        and    al, 0xef
        mov    byte ptr [ecx], al
        or     al, 4
        mov    byte ptr [ecx], al
loc_1_1B88:
        push   edx
        call   sub_1_191E
        add    esp, 4
        sti    
loc_1_1B92:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_1B96:
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        call   sub_1_280C
        test   eax, eax
        je     loc_1_1BB5
loc_1_1BA5:
        push   eax
        push   dword ptr [0]                 ; -> obj1:0x0068
        call   sub_1_27F4
        test   eax, eax
        jne    loc_1_1BA5
loc_1_1BB5:
        mov    eax, 1
        ret    4
sub_1_1BBD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0x68c
        call   sub_1_23B6
        push   0x29
        mov    dword ptr [0], eax            ; -> obj1:0x0130
        call   sub_1_23B6
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 0x6b
        mov    edi, esi
        mov    dword ptr [0], eax            ; -> loc_1_0134
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [esi + 0xd0]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [esi + 0xcb], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [esi + 4], edx
        mov    edx, dword ptr [ebp + 0x24]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [esi + 0xc], ecx
        mov    dword ptr [esi + 0x10], edx
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    dword ptr [esi + 0x18], eax
        call   sub_1_2419
        mov    dword ptr [esi + 0x1c], eax
        test   eax, eax
        jne    loc_1_1C36
        mov    eax, 0x1005
        jmp    loc_1_1CA7
loc_1_1C36:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 8
        jne    loc_1_1C47
        mov    dword ptr [esi + 0x1c], 8
        jmp    loc_1_1C53
loc_1_1C47:
        cmp    eax, 4
        jne    loc_1_1C53
        mov    dword ptr [esi + 0x1c], 4
loc_1_1C53:
        cmp    dword ptr [esi + 0x1c], 8
        jne    loc_1_1C65
        mov    dword ptr [esi + 0xc2], 0     ; -> loc_1_26F1
        jmp    loc_1_1C6F
loc_1_1C65:
        mov    dword ptr [esi + 0xc2], 0     ; -> loc_1_2578
loc_1_1C6F:
        mov    dword ptr [esi + 0x37], 0     ; -> loc_1_2471
        mov    dword ptr [esi + 0x24], 0xffffffff
        mov    byte ptr [esi + 0x16e], 0x40
        mov    dword ptr [esi + 0xc6], 0
        mov    dword ptr [esi + 0x3b], 0x5c3a
        push   esi
        call   sub_1_232D
        push   esi
        call   sub_1_2306
        push   esi
        call   sub_1_1D32
loc_1_1CA7:
        pop    edi
        pop    esi
        pop    ebp
        ret    0x24
sub_1_1CAD:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
loc_1_1CB4:
        or     byte ptr [esi + 0x3d], 1
        push   esi
        call   sub_1_1D32
        test   eax, eax
        je     loc_1_1CB4
        pop    esi
        pop    ebp
        ret    4
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli    
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 0x3d], 2
        je     loc_1_1CDD
        mov    eax, 0x1007
        jmp    loc_1_1CFF
loc_1_1CDD:
        mov    al, byte ptr [ebp + 0xc]
        lea    edi, [edx + 0x83]
        mov    esi, dword ptr [ebp + 0x10]
        mov    ecx, 0xf
        mov    byte ptr [edx + 0x82], al
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        or     byte ptr [edx + 0x3d], 2
        xor    eax, eax
loc_1_1CFF:
        sti    
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
sub_1_1D06:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [ecx + 0x36]
        mov    dl, al
        pop    ebp
        and    dl, 0xf0
        mov    byte ptr [ecx + 0x36], dl
        ret    4
sub_1_1D1C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [ecx + 0x35]
        mov    dl, al
        pop    ebp
        and    dl, 0xe1
        mov    byte ptr [ecx + 0x35], dl
        ret    4
sub_1_1D32:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x24]
        inc    eax
        mov    dword ptr [edi + 0x24], eax
        jne    loc_1_1E7B
        push   edi
        call   sub_1_1E92
loc_1_1D52:
        test   dword ptr [edi + 0x3b], 0x7fbf
        je     loc_1_1D6C
        cli    
        mov    edx, dword ptr [edi + 0x18]
        add    edx, 2
        in     al, dx
        and    al, 0xf7
        out    dx, al
        sti    
        mov    ebx, 1
loc_1_1D6C:
        mov    edx, dword ptr [edi + 0x18]
        inc    edx
        in     al, dx
        test   al, 0x40
        je     loc_1_1DA8
        push   edi
        call   sub_1_1F17
        mov    eax, dword ptr [edi + 0x20]
        test   eax, eax
        jne    loc_1_1D8D
        mov    dword ptr [edi + 0x2c], 0
        xor    esi, esi
        jmp    loc_1_1DA8
loc_1_1D8D:
        cmp    eax, 0x1001
        jne    loc_1_1E7B
        mov    eax, dword ptr [edi + 0x2c]
        inc    eax
        mov    dword ptr [edi + 0x2c], eax
        cmp    eax, 0x10
        jae    loc_1_1E74
loc_1_1DA8:
        test   byte ptr [edi + 0x31], 2
        je     loc_1_1DD6
        test   byte ptr [edi + 0x3c], 0x80
        jne    loc_1_1DD6
        test   byte ptr [edi + 0x41], 2
        jne    loc_1_1DD6
        movzx  eax, byte ptr [edi + 0xca]
        push   eax
        lea    ecx, [edi + 0x43]
        push   ecx
        push   dword ptr [edi]
        call   dword ptr [edi + 8]
        test   eax, eax
        je     loc_1_1DD6
        or     byte ptr [edi + 0x3c], 0x80
        mov    byte ptr [edi + 0x42], al
loc_1_1DD6:
        cmp    dword ptr [edi + 0x3b], 0
        je     loc_1_1DF7
        push   edi
        call   sub_1_21CA
        mov    eax, dword ptr [edi + 0x20]
        test   eax, eax
        jne    loc_1_1E43
        mov    dword ptr [edi + 0x2c], 0
        mov    dword ptr [edi + 0x28], 0
loc_1_1DF7:
        mov    edx, dword ptr [edi + 0x18]
        inc    edx
        in     al, dx
        test   al, 0x40
        jne    loc_1_1D52
        mov    eax, dword ptr [edi + 0x3b]
        test   eax, 0xffff7fff
        jne    loc_1_1D52
        test   ah, 0x80
        je     loc_1_1E21
        test   byte ptr [edi + 0x41], 2
        je     loc_1_1D52
loc_1_1E21:
        test   byte ptr [edi + 0x41], 1
        jne    loc_1_1E7B
        test   byte ptr [edi + 0x16e], 0x40
        jne    loc_1_1E7B
        test   ebx, ebx
        jne    loc_1_1E7B
        inc    esi
        cmp    dword ptr [0], esi            ; -> obj1:0x0130
        jae    loc_1_1D52
        jmp    loc_1_1E7B
loc_1_1E43:
        cmp    eax, 1
        je     loc_1_1E88
        cmp    eax, 0x1001
        jne    loc_1_1E5D
        mov    eax, dword ptr [edi + 0x2c]
        inc    eax
        mov    dword ptr [edi + 0x2c], eax
        cmp    eax, 0x10
        jb     loc_1_1DF7
        jmp    loc_1_1E74
loc_1_1E5D:
        cmp    eax, 0x1003
        jne    loc_1_1E7B
        mov    eax, dword ptr [edi + 0x28]
        inc    eax
        mov    dword ptr [edi + 0x28], eax
        cmp    eax, 0x20
        jb     loc_1_1D52
loc_1_1E74:
        mov    dword ptr [edi + 0x20], 0x1004
loc_1_1E7B:
        mov    eax, dword ptr [edi + 0x20]
        dec    dword ptr [edi + 0x24]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1E88:
        xor    eax, eax
        mov    dword ptr [edi + 0x2c], eax
        mov    dword ptr [edi + 0x28], eax
        jmp    loc_1_1E7B
sub_1_1E92:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x31], 8
        je     loc_1_1EB1
        test   byte ptr [esi + 0x36], 0xf
        je     loc_1_1EB1
        push   esi
        call   sub_1_1D06
        push   eax
        push   dword ptr [esi]
        call   dword ptr [esi + 0x10]
loc_1_1EB1:
        test   byte ptr [esi + 0x31], 4
        je     loc_1_1EC9
        test   byte ptr [esi + 0x35], 0x1e
        je     loc_1_1EC9
        push   esi
        call   sub_1_1D1C
        push   eax
        push   dword ptr [esi]
        call   dword ptr [esi + 0xc]
loc_1_1EC9:
        test   byte ptr [esi + 0x31], 1
        je     loc_1_1F12
        mov    al, byte ptr [esi + 0xcf]
        test   al, al
        je     loc_1_1F12
        movzx  eax, al
        push   eax
        push   dword ptr [esi + 0xcb]
        push   dword ptr [esi]
        call   dword ptr [esi + 4]
        mov    cl, byte ptr [esi + 0xcf]
        add    dword ptr [esi + 0xcb], eax
        sub    cl, al
        mov    byte ptr [esi + 0xcf], cl
        test   cl, cl
        jne    loc_1_1F12
        lea    eax, [esi + 0xd0]
        push   esi
        mov    dword ptr [esi + 0xcb], eax
        call   sub_1_232D
loc_1_1F12:
        pop    esi
        pop    ebp
        ret    4
sub_1_1F17:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        lea    esi, [ebx + 0x16f]
        lea    edi, [ebx + 0x16e]
        push   esi
        push   edi
        push   dword ptr [ebx + 0x18]
        call   dword ptr [ebx + 0xc2]
        mov    dword ptr [ebx + 0x20], eax
        test   eax, eax
        jne    loc_1_2154
        mov    cl, byte ptr [edi]
        test   cl, 0x80
        jne    loc_1_1FCA
        movzx  eax, cl
        test   byte ptr [ebx + 0x31], 1
        movzx  edi, byte ptr [eax]           ; -> obj1:0x0138
        je     loc_1_1F6E
        cmp    byte ptr [ebx + 0xcf], 0
        jne    loc_1_1F6E
        push   edi
        push   esi
        push   dword ptr [ebx]
        call   dword ptr [ebx + 4]
        jmp    loc_1_1F70
loc_1_1F6E:
        xor    eax, eax
loc_1_1F70:
        sub    edi, eax
        mov    dword ptr [ebp - 4], edi
        je     loc_1_2154
        mov    edi, dword ptr [ebx + 0xcb]
        mov    ecx, ebx
        movzx  esi, byte ptr [ebx + 0xcf]
        sub    ecx, edi
        sub    ecx, esi
        add    ecx, 0x14e
        cmp    ecx, dword ptr [ebp - 4]
        jae    loc_1_1F9C
        mov    dword ptr [ebp - 4], ecx
loc_1_1F9C:
        mov    edx, dword ptr [ebp - 4]
        add    edi, esi
        lea    esi, [ebx + eax + 0x16f]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        push   ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dl, byte ptr [ebp - 4]
        add    byte ptr [ebx + 0xcf], dl
        call   sub_1_234B
        jmp    loc_1_2154
loc_1_1FCA:
        movzx  edx, cl
        mov    eax, edx
        and    al, 0xbf
        sub    eax, 0x81
        cmp    eax, 0x30
        ja     loc_1_1FE9
        movzx  eax, byte ptr [eax]           ; -> obj1:0x2199
        jmp    dword ptr [eax*4]             ; -> obj1:0x215D
loc_1_1FE9:
        mov    edx, dword ptr [ebx + 0x14]
        test   edx, edx
        je     loc_1_2154
        push   esi
        push   ecx
        push   dword ptr [ebx]
        call   edx
        jmp    loc_1_2154
loc_1_1FFF:
        or     byte ptr [ebx + 0x3b], 1
        jmp    loc_1_2154
loc_1_2008:
        test   byte ptr [ebx + 0x3b], 1
        jne    loc_1_2013
        mov    al, byte ptr [esi]
        mov    byte ptr [ebx + 0x34], al
loc_1_2013:
        or     byte ptr [ebx + 0xc6], 1
        jmp    loc_1_2154
loc_1_201F:
        or     byte ptr [ebx + 0x3b], 4
        jmp    loc_1_2154
loc_1_2028:
        test   byte ptr [ebx + 0x3b], 4
        jne    loc_1_2033
        mov    al, byte ptr [esi]
        mov    byte ptr [ebx + 0x33], al
loc_1_2033:
        or     byte ptr [ebx + 0xc6], 2
        jmp    loc_1_2154
loc_1_203F:
        mov    cl, byte ptr [ebx + 0x36]
        mov    al, byte ptr [esi]
        and    cl, 0xf
        mov    dl, al
        and    dl, 0xfb
        and    al, 0x44
        or     dl, cl
        mov    byte ptr [ebx + 0x36], cl
        or     byte ptr [ebx + 0xc6], 4
        mov    byte ptr [ebx + 0x36], dl
        cmp    al, 4
        jne    loc_1_2067
        or     dl, 4
        mov    byte ptr [ebx + 0x36], dl
loc_1_2067:
        test   byte ptr [ebx + 0x31], 8
        je     loc_1_2154
        test   byte ptr [ebx + 0x36], 0xf
        je     loc_1_2154
        push   ebx
        call   sub_1_1D06
        push   eax
        push   dword ptr [ebx]
        call   dword ptr [ebx + 0x10]
        jmp    loc_1_2154
loc_1_208C:
        mov    cl, byte ptr [ebx + 0x35]
        mov    al, byte ptr [esi]
        or     byte ptr [ebx + 0xc6], 8
        and    cl, 0x1e
        or     al, cl
        mov    byte ptr [ebx + 0x35], cl
        test   byte ptr [ebx + 0x31], 4
        mov    byte ptr [ebx + 0x35], al
        je     loc_1_2154
        test   al, 0x1e
        je     loc_1_2154
        push   ebx
        call   sub_1_1D1C
        push   eax
        push   dword ptr [ebx]
        call   dword ptr [ebx + 0xc]
        jmp    loc_1_2154
loc_1_20C6:
        or     byte ptr [ebx + 0xc6], 0x10
        jmp    loc_1_2154
loc_1_20D2:
        push   ebx
        call   sub_1_2306
        jmp    loc_1_2154
loc_1_20DA:
        push   ebx
        call   sub_1_22DF
        jmp    loc_1_2154
loc_1_20E2:
        test   byte ptr [ebx + 0x3c], 2
        jne    loc_1_20F0
        mov    al, byte ptr [esi]
        mov    byte ptr [ebx + 0xca], al
loc_1_20F0:
        or     byte ptr [ebx + 0xc6], 0x80
        jmp    loc_1_2154
loc_1_20F9:
        or     byte ptr [ebx + 0x3c], 0x20
        jmp    loc_1_2154
loc_1_20FF:
        push   ebx
        call   sub_1_2368
        or     byte ptr [ebx + 0xc7], 4
        jmp    loc_1_2154
loc_1_210E:
        movzx  eax, byte ptr [edx]           ; -> obj1:0x0138
        lea    edi, [ebx + 0x14e]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        or     byte ptr [ebx + 0xc7], 1
        jmp    loc_1_2154
loc_1_2132:
        movzx  eax, byte ptr [edx]           ; -> obj1:0x0138
        lea    edi, [ebx + 0x15e]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        or     byte ptr [ebx + 0xc7], 2
loc_1_2154:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        ; ---- dispatch table (15 entries) ----
        dd  loc_1_1FFF
        dd  loc_1_2008
        dd  loc_1_201F
        dd  loc_1_2028
        dd  loc_1_203F
        dd  loc_1_208C
        dd  loc_1_20C6
        dd  loc_1_20D2
        dd  loc_1_20DA
        dd  loc_1_20E2
        dd  loc_1_20F9
        dd  loc_1_20FF
        dd  loc_1_210E
        dd  loc_1_2132
        dd  loc_1_1FE9
        db  000h, 001h, 00Eh, 002h, 003h, 00Eh, 004h, 00Eh, 005h, 00Eh, 006h, 007h, 008h, 009h, 00Eh, 00Eh
        db  00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Ah, 00Bh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh
        db  00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Ch
        db  00Dh
sub_1_21CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x3b]
        test   eax, 0x7fbf
        je     loc_1_221D
        mov    edi, 0                        ; -> loc_1_0238
loc_1_21E2:
        test   dword ptr [edi], eax
        jne    loc_1_21F6
        add    edi, 9
        cmp    edi, 0x7e                     ; -> obj1:0x02B6
        jb     loc_1_21E2
        jmp    loc_1_22D8
loc_1_21F6:
        mov    eax, dword ptr [edi + 4]
        mov    cl, byte ptr [edi + 8]
        add    eax, esi
        push   eax
        push   ecx
        push   dword ptr [esi + 0x18]
        call   dword ptr [esi + 0x37]
        mov    dword ptr [esi + 0x20], eax
        test   eax, eax
        jne    loc_1_22D8
        mov    eax, dword ptr [edi]
        not    eax
        and    dword ptr [esi + 0x3b], eax
        jmp    loc_1_22D8
loc_1_221D:
        test   ah, 0x80
        je     loc_1_2269
        test   byte ptr [esi + 0x41], 2
        jne    loc_1_2269
        lea    edi, [esi + 0x43]
        mov    al, byte ptr [esi + 0x42]
        push   edi
        push   eax
        push   dword ptr [esi + 0x18]
        call   dword ptr [esi + 0x37]
        mov    dword ptr [esi + 0x20], eax
        test   eax, eax
        jne    loc_1_22D8
        test   byte ptr [esi + 0x31], 2
        mov    eax, 0
        je     loc_1_225A
        movzx  eax, byte ptr [esi + 0xca]
        push   eax
        push   edi
        push   dword ptr [esi]
        call   dword ptr [esi + 8]
loc_1_225A:
        test   eax, eax
        je     loc_1_2263
        mov    byte ptr [esi + 0x42], al
        jmp    loc_1_22D8
loc_1_2263:
        and    byte ptr [esi + 0x3c], 0x7f
        jmp    loc_1_22D8
loc_1_2269:
        test   eax, 0x20000
        je     loc_1_2291
        lea    eax, [esi + 0x83]
        mov    cl, byte ptr [esi + 0x82]
        push   eax
        push   ecx
        push   dword ptr [esi + 0x18]
        call   dword ptr [esi + 0x37]
        mov    dword ptr [esi + 0x20], eax
        test   eax, eax
        jne    loc_1_22D8
        and    byte ptr [esi + 0x3d], 0xfd
        jmp    loc_1_22D8
loc_1_2291:
        test   eax, 0x10040
        je     loc_1_22D8
        mov    edi, 0                        ; -> loc_1_02B8
loc_1_229D:
        test   dword ptr [edi], eax
        jne    loc_1_22AE
        add    edi, 9
        cmp    edi, 0x12                     ; -> obj1:0x02CA
        jb     loc_1_229D
        jmp    loc_1_22D8
loc_1_22AE:
        mov    bl, byte ptr [edi + 8]
        mov    eax, dword ptr [edi + 4]
        add    eax, esi
        push   eax
        push   ebx
        push   dword ptr [esi + 0x18]
        call   dword ptr [esi + 0x37]
        mov    dword ptr [esi + 0x20], eax
        test   eax, eax
        jne    loc_1_22D8
        mov    eax, dword ptr [edi]
        not    eax
        and    dword ptr [esi + 0x3b], eax
        cmp    bl, 0x98
        jne    loc_1_22D8
        mov    dword ptr [esi + 0x20], 1
loc_1_22D8:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_22DF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xc6]
        or     cl, 0x40
        pop    ebp
        and    byte ptr [eax + 0x41], 0xfd
        mov    dword ptr [eax + 0xc6], ecx
        and    cl, 0xdf
        mov    dword ptr [eax + 0xc6], ecx
        ret    4
sub_1_2306:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xc6]
        or     cl, 0x20
        pop    ebp
        or     byte ptr [eax + 0x41], 2
        mov    dword ptr [eax + 0xc6], ecx
        and    cl, 0xbf
        mov    dword ptr [eax + 0xc6], ecx
        ret    4
sub_1_232D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x3b]
        or     ch, 1
        pop    ebp
        and    byte ptr [eax + 0x41], 0xfe
        mov    dword ptr [eax + 0x3b], ecx
        and    cl, 0x7f
        mov    dword ptr [eax + 0x3b], ecx
        ret    4
sub_1_234B:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x3b]
        or     al, 0x80
        pop    ebp
        or     byte ptr [ecx + 0x41], 1
        mov    dword ptr [ecx + 0x3b], eax
        and    ah, 0xfe
        mov    dword ptr [ecx + 0x3b], eax
        ret    4
sub_1_2368:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x41]
        mov    cl, byte ptr [esi + 0x16f]
        xor    cl, al
        test   cl, 1
        je     loc_1_2390
        push   esi
        test   al, 1
        je     loc_1_238B
        call   sub_1_234B
        jmp    loc_1_2390
loc_1_238B:
        call   sub_1_232D
loc_1_2390:
        mov    al, byte ptr [esi + 0x16f]
        mov    cl, byte ptr [esi + 0x41]
        xor    cl, al
        test   cl, 2
        je     loc_1_23B1
        push   esi
        test   al, 2
        je     loc_1_23AC
        call   sub_1_2306
        jmp    loc_1_23B1
loc_1_23AC:
        call   sub_1_22DF
loc_1_23B1:
        pop    esi
        pop    ebp
        ret    4
sub_1_23B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        cli    
        mov    dx, 0x61
        in     al, dx
        mov    bl, al
        and    al, 0xfe
        out    dx, al
        mov    al, 0xb0
        out    0x43, al
        mov    eax, dword ptr [ebp + 8]
        out    0x42, al
        mov    al, ah
        out    0x42, al
        mov    ecx, 0x7fff
        in     al, dx
        or     al, 1
        out    dx, al
loc_1_23DE:
        in     al, dx
        test   al, 0x20
        loope  loc_1_23DE
        mov    al, bl
        out    dx, al
        mov    eax, 0x7fff
        sub    eax, ecx
        mov    dword ptr [ebp - 4], eax
        sti    
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_23FB:
        push   ebp
        mov    cl, 0xff
        mov    ebp, esp
        push   ebx
        xor    bl, bl
        mov    edx, dword ptr [ebp + 8]
loc_1_2406:
        mov    al, cl
        out    dx, al
        in     al, dx
        xor    al, cl
        or     bl, al
        dec    cl
        jne    loc_1_2406
        mov    al, bl
        pop    ebx
        pop    ebp
        ret    4
sub_1_2419:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 2]
        mov    edx, esi
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 3]
        in     al, dx
        and    al, 0xfe
        push   edi
        out    dx, al
        mov    edx, esi
        in     al, dx
        and    al, 0xdf
        out    dx, al
        call   sub_1_23FB
        test   al, al
        je     loc_1_2445
        xor    ecx, ecx
        jmp    loc_1_2463
loc_1_2445:
        mov    edx, esi
        push   edi
        in     al, dx
        or     al, 0x20
        out    dx, al
        call   sub_1_23FB
        sub    al, 0xff
        mov    edx, esi
        cmp    al, 1
        in     al, dx
        sbb    ecx, ecx
        and    al, 0xdf
        out    dx, al
        and    ecx, 4
        add    ecx, 4
loc_1_2463:
        mov    edx, esi
        mov    al, bl
        out    dx, al
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2471:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cld    
        cli    
        mov    bx, ds
        mov    es, bx
        xor    ebx, ebx
        mov    edx, dword ptr [ebp + 8]
        mov    bl, byte ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    bh, byte ptr [ebx]            ; -> obj1:0x0138
        or     bh, bh
        mov    edi, 0x1002
        je     loc_1_255C
        add    dx, 2
        in     al, dx
        and    al, 0x10
        mov    ah, al
        and    ah, 0xf7
        mov    edi, 0x1003
        add    dx, -1
        mov    ecx, dword ptr [0]            ; -> loc_1_0134
loc_1_24BB:
        in     al, dx
        test   al, 0x80
        loopne loc_1_24BB
        je     loc_1_255C
        test   al, 0x40
        jne    loc_1_255C
        sub    dx, 1
        add    edx, 2
        mov    al, ah
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> loc_1_0134
loc_1_24E1:
        in     al, dx
        test   al, 0x80
        loopne loc_1_24E1
        je     loc_1_255C
        test   al, 0x40
        jne    loc_1_255C
        mov    edi, 0x1001
        add    edx, -1
        mov    al, bl
        out    dx, al
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        and    al, 0xfe
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_250B:
        in     al, dx
        test   al, 0x80
        loopne loc_1_250B
        jne    loc_1_255C
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2518:
        in     al, dx
        test   al, 0x80
        loope  loc_1_2518
        je     loc_1_255C
loc_1_251F:
        add    edx, -1
        outsb  dx, byte ptr [esi]
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2534:
        in     al, dx
        test   al, 0x80
        loopne loc_1_2534
        jne    loc_1_255C
        add    edx, 1
        mov    al, ah
        and    al, 0xfe
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_254C:
        in     al, dx
        test   al, 0x80
        loope  loc_1_254C
        je     loc_1_255C
        dec    bh
        jne    loc_1_251F
        mov    edi, 0
loc_1_255C:
        mov    edx, dword ptr [ebp + 8]
        add    dx, 2
        mov    al, ah
        or     al, 8
        out    dx, al
        sti    
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2578:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cld    
        cli    
        mov    bx, ds
        mov    es, bx
        xor    ebx, ebx
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        add    dx, 2
        in     al, dx
        mov    bh, al
        and    bh, 8
        and    al, 0x10
        mov    ah, al
        or     ah, 8
        mov    esi, 0x1001
        mov    al, ah
        or     al, 1
        out    dx, al
        and    al, 0xfe
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_25B8:
        in     al, dx
        test   al, 0x80
        loope  loc_1_25B8
        je     loc_1_26D7
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_25C9:
        in     al, dx
        test   al, 0x80
        loopne loc_1_25C9
        jne    loc_1_26D7
        in     al, dx
        shl    al, 1
        and    al, 0xf0
        mov    bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_25EC:
        in     al, dx
        test   al, 0x80
        loope  loc_1_25EC
        je     loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0xfe
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2608:
        in     al, dx
        test   al, 0x80
        loopne loc_1_2608
        jne    loc_1_26D7
        in     al, dx
        shr    al, 3
        and    al, 0xf
        or     bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_262C:
        in     al, dx
        test   al, 0x80
        loope  loc_1_262C
        je     loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0xfe
        out    dx, al
        mov    byte ptr [edi], bl
        and    ah, 0xf7
        or     ah, bh
        xor    bh, bh
        mov    bh, byte ptr [ebx]            ; -> obj1:0x0138
        or     bh, bh
        jne    loc_1_2659
        mov    esi, 0x1002
        jmp    loc_1_26D7
loc_1_2659:
        mov    edi, dword ptr [ebp + 0x10]
loc_1_265C:
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2665:
        in     al, dx
        test   al, 0x80
        loopne loc_1_2665
        jne    loc_1_26D7
        in     al, dx
        shl    al, 1
        and    al, 0xf0
        mov    bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2684:
        in     al, dx
        test   al, 0x80
        loope  loc_1_2684
        je     loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0xfe
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_269C:
        in     al, dx
        test   al, 0x80
        loopne loc_1_269C
        jne    loc_1_26D7
        in     al, dx
        shr    al, 3
        and    al, 0xf
        or     bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_26BC:
        in     al, dx
        test   al, 0x80
        loope  loc_1_26BC
        je     loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0xfe
        out    dx, al
        mov    al, bl
        stosb  byte ptr es:[edi], al
        dec    bh
        jne    loc_1_265C
        mov    esi, 0
loc_1_26D7:
        mov    edx, dword ptr [ebp + 8]
        add    dx, 2
        mov    al, ah
        out    dx, al
        sti    
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_26F1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cld    
        cli    
        mov    bx, ds
        mov    es, bx
        xor    ebx, ebx
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        add    dx, 2
        in     al, dx
        mov    bh, al
        and    bh, 8
        and    al, 0x10
        mov    ah, al
        or     ah, 0x2c
        mov    esi, 0x1001
        mov    al, ah
        or     al, 1
        out    dx, al
        and    al, 0xfe
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2731:
        in     al, dx
        test   al, 0x80
        loope  loc_1_2731
        je     loc_1_27CD
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2742:
        in     al, dx
        test   al, 0x80
        loopne loc_1_2742
        jne    loc_1_27CD
        add    edx, -1
        in     al, dx
        mov    bl, al
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2764:
        in     al, dx
        test   al, 0x80
        loope  loc_1_2764
        je     loc_1_27CD
        add    edx, 1
        mov    al, ah
        and    al, 0xfe
        out    dx, al
        mov    byte ptr [edi], bl
        and    ah, 0xf7
        or     ah, bh
        xor    bh, bh
        mov    bh, byte ptr [ebx]            ; -> obj1:0x0138
        or     bh, bh
        jne    loc_1_278D
        mov    esi, 0x1002
        jmp    loc_1_27CD
loc_1_278D:
        mov    edi, dword ptr [ebp + 0x10]
loc_1_2790:
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_2799:
        in     al, dx
        test   al, 0x80
        loopne loc_1_2799
        jne    loc_1_27CD
        add    edx, -1
        insb   byte ptr es:[edi], dx
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        mov    ecx, dword ptr [0]            ; -> obj1:0x0130
loc_1_27B5:
        in     al, dx
        test   al, 0x80
        loope  loc_1_27B5
        je     loc_1_27CD
        add    edx, 1
        mov    al, ah
        and    al, 0xfe
        out    dx, al
        dec    bh
        jne    loc_1_2790
        mov    esi, 0
loc_1_27CD:
        mov    edx, dword ptr [ebp + 8]
        add    dx, 2
        mov    al, ah
        out    dx, al
        sti    
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_27E7:
        ret    
sub_1_27E8:
        ret    
sub_1_27E9:
        ret    
sub_1_27EA:
        ret    
sub_1_27EB:
        ret    
sub_1_27EC:
        ret    
sub_1_27ED:
        ret    
sub_1_27EE:
        ret    
sub_1_27EF:
        ret    
        db  0C3h
sub_1_27F1:
        ret    
        db  0CCh, 0CCh
sub_1_27F4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CDh, 020h, 0F3h, 080h, 001h, 000h, 0CCh, 0CCh
sub_1_280C:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_1_281C:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_1_2824:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf800100
        int3   
        int3   
sub_1_282C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_1_2834:
        VMMCall  VMM._HeapReAllocate             ; cd2050800100
        int3   
        int3   
sub_1_283C:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  0CCh, 0CCh, 0CCh
sub_1_2858:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  0CCh
sub_1_2868:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    eax, esi
        pop    esi
        ret    8
        db  0CCh, 0CCh, 0CCh
sub_1_2880:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        pop    esi
        ret    4
        db  0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xe6  R-X
sub_2_0000:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0010:
        VxDCall  BIOS.Service_8001h              ; cd2001802b00
        int3   
        int3   
sub_2_0018:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        pop    esi
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_2_002C:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh
sub_2_0038:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_0048:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0058:
        VxDCall  CONFIGMG._CONFIGMG_Get_Res_Des_Data ; cd2022803300
        int3   
        int3   
sub_2_0060:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_2_0068:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_2_0070:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_0078:
        VxDCall  BIOS.Service_8022h              ; cd2022802b00
        int3   
        int3   
sub_2_0080:
        VxDCall  BIOS.Service_8021h              ; cd2021802b00
        int3   
        int3   
sub_2_0088:
        VxDCall  BIOS.Service_8019h              ; cd2019802b00
        int3   
        int3   
sub_2_0090:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_2_0098:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_00A8:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, ecx
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_2_00BC:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_00CC:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_2_00E0:
        VMMCall  VMM._HeapFree                   ; cd2051800100
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x9f1  R-X
loc_3_0000:
        xor    eax, eax
        mov    edx, dword ptr [esp + 0xc]
        push   0x34
        pop    ecx
        cmp    dword ptr [edx], ecx
        mov    dword ptr [edx], ecx
        jb     loc_3_0043
        mov    edx, dword ptr [esp + 8]
        or     edx, edx
        je     loc_3_0043
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [edx + 0x2c], eax
        mov    byte ptr [edx + 0x30], al
        mov    al, 0x34
        mov    dword ptr [edx], eax
        mov    al, 1
        mov    dword ptr [edx + 0x24], eax
        shl    eax, 8
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [esp + 4]
        lea    eax, [eax + 0x94]
        lea    edx, [edx + 8]
        VxDCall  BIOS.Service_0020h              ; cd2020002b00
loc_3_0043:
        or     al, 1
        ret    
loc_3_0046:
        sub    esp, 0x30
        xor    eax, eax
        mov    edx, dword ptr [esp + 0x34]
        mov    dword ptr [edx + 0xc], 0xffffffeb
        cmp    dword ptr [esp + 0x3c], 0x34
        jb     loc_3_007C
        lea    edx, [esp]
        mov    eax, dword ptr [esp + 0x38]
        lea    eax, [eax + 8]
        VxDCall  BIOS.Service_001Fh              ; cd201f002b00
        push   -1
        push   edx
        push   dword ptr [esp + 0x3c]
        call   sub_1_03E1
loc_3_0079:
        add    esp, 0xc
loc_3_007C:
        add    esp, 0x30
        ret    
sub_3_0080:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [0], 0              ; -> obj1:0x0060
        je     loc_3_0096
        mov    eax, 1
        jmp    loc_3_010F
loc_3_0096:
        call   sub_2_002C
loc_3_009B:
        mov    dword ptr [0], eax            ; -> obj1:0x0060
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 4], 0
        je     loc_3_010D
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0
        je     loc_3_010D
        call   sub_1_27E7
loc_3_00CB:
        push   0x2f4
        push   4
        call   sub_2_0018
loc_3_00D7:
        mov    dword ptr [0], eax            ; -> obj1:0x0068
        test   eax, eax
        je     loc_3_010D
        push   0                             ; -> loc_3_0160
        call   sub_2_0010
loc_3_00EA:
        add    esp, 4
        test   eax, eax
        je     loc_3_00F8
        mov    eax, 1
        jmp    loc_3_010F
loc_3_00F8:
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        call   sub_2_0000
loc_3_0103:
        mov    dword ptr [0], 0              ; -> obj1:0x0068
loc_3_010D:
        xor    eax, eax
loc_3_010F:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0113:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        test   eax, eax
        je     loc_3_0159
        push   eax
        call   sub_2_0048
loc_3_0123:
        mov    esi, eax
        test   esi, esi
        je     loc_3_0144
loc_3_0129:
        push   esi
        call   sub_3_0433
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   esi
        push   eax
        call   sub_2_0038
loc_3_013E:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_0129
loc_3_0144:
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        call   sub_2_0000
loc_3_014F:
        mov    dword ptr [0], 0              ; -> obj1:0x0068
loc_3_0159:
        mov    eax, 1
        pop    esi
        ret    
loc_3_0160:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        cmp    dword ptr [ebp + 8], 0
        push   ebx
        push   esi
        push   edi
        je     loc_3_0176
        xor    eax, eax
        jmp    loc_3_0236
loc_3_0176:
        mov    esi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp + 0x1c]
        lea    edi, [ebp - 0x40]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0xc], ecx
        xor    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        push   eax
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0x40]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x14], 0x20
        push   1
        push   0                             ; -> obj4:0x0000
        push   0
        push   edi
        call   sub_2_0070
loc_3_01B3:
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_021E
        push   0                             ; -> obj4:0x0008
        lea    eax, [ebp - 0x40]
        push   eax
        call   sub_3_023D
        add    esp, 8
        test   eax, eax
        jne    loc_3_021E
        push   2
        lea    eax, [ebp - 0x10]
        push   edi
        push   eax
        call   sub_2_0068
loc_3_01DB:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 4], ecx
        test   eax, eax
        jne    loc_3_021E
loc_3_01E8:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_0060
loc_3_01FA:
        add    esp, 0x14
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_3_021E
        push   0
        lea    eax, [ebp - 0x20]
        push   0xc
        push   eax
        push   dword ptr [ebp - 4]
        call   sub_2_0058
loc_3_0213:
        movzx  esi, word ptr [ebp - 0x1c]
        add    esp, 0x10
        test   ebx, ebx
        je     loc_3_01E8
loc_3_021E:
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        push   esi
        push   dword ptr [ebp + 0x10]
        push   edi
        call   sub_3_027F
        add    esp, 0x14
        mov    eax, 1
loc_3_0236:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_023D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
loc_3_0248:
        mov    bl, byte ptr [eax]
        mov    dl, byte ptr [ecx]
        inc    eax
        inc    ecx
        test   bl, bl
        je     loc_3_0274
        test   dl, dl
        je     loc_3_0274
        cmp    bl, 0x61
        jl     loc_3_0263
        cmp    bl, 0x7a
        jg     loc_3_0263
        sub    bl, 0x20
loc_3_0263:
        cmp    dl, 0x61
        jl     loc_3_0270
        cmp    dl, 0x7a
        jg     loc_3_0270
        sub    dl, 0x20
loc_3_0270:
        cmp    dl, bl
        je     loc_3_0248
loc_3_0274:
        movsx  eax, bl
        movsx  ecx, dl
        sub    eax, ecx
        pop    ebx
        pop    ebp
        ret    
sub_3_027F:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   eax
        call   sub_2_00A8
loc_3_0293:
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_042C
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        mov    edi, ebx
        call   sub_2_0098
loc_3_02AB:
        xor    eax, eax
        mov    ecx, 0xbd
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [ebx + 2], 0x10a
        mov    dword ptr [ebx + 4], 0        ; -> obj1:0x0090
        mov    dword ptr [ebx + 8], 0x17
        mov    dword ptr [ebx + 0x94], 0x30
        mov    byte ptr [ebx + 0xad], 0x11
        mov    byte ptr [ebx + 0xae], 0x13
        mov    dword ptr [ebx + 0xd4], 0xffffffff
        mov    byte ptr [ebx + 0x124], 0xc0
        mov    byte ptr [ebx + 0x125], 0x10
        mov    word ptr [ebx], 0x94
        push   eax
        mov    byte ptr [ebp - 4], 2
        lea    ecx, [ebp - 4]
        mov    byte ptr [ebp - 2], al
        mov    byte ptr [ebp - 3], 0x10
        mov    byte ptr [ebp - 1], 0x80
        mov    dword ptr [ebp - 8], 4
        lea    eax, [ebp - 8]
        push   eax
        push   ecx
        push   3
        push   0                             ; -> obj4:0x0010
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_0070
loc_3_032D:
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_037E
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 0x124], al
        cmp    byte ptr [ebp - 2], 0
        je     loc_3_034A
        or     byte ptr [ebx + 0xfc], 1
loc_3_034A:
        cmp    byte ptr [ebp - 4], 1
        jne    loc_3_0359
        or     byte ptr [ebx + 0xfc], 4
        jmp    loc_3_0366
loc_3_0359:
        cmp    byte ptr [ebp - 4], 0
        jne    loc_3_0366
        or     byte ptr [ebx + 0xfc], 6
loc_3_0366:
        cmp    byte ptr [ebp - 3], 1
        ja     loc_3_0375
        or     byte ptr [ebx + 0xfc], 0x10
        jmp    loc_3_037E
loc_3_0375:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [ebx + 0x125], al
loc_3_037E:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        push   0
        mov    edi, dword ptr [ebp + 0x18]
        mov    dword ptr [ebx + 0xc4], eax
        mov    dword ptr [ebx + 0x113], ecx
        mov    ecx, 0xffffffff
        sub    eax, eax
        mov    dword ptr [ebx + 0x132], 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_2_0090
loc_3_03B0:
        add    esp, 8
        mov    edx, eax
        mov    dword ptr [ebx + 0x120], eax
        test   edx, edx
        je     loc_3_0423
        mov    edi, dword ptr [ebp + 0x18]
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
        push   edx
        and    ecx, 3
        push   0                             ; -> loc_3_046A
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [ebp + 0xc]
        call   sub_2_0088
loc_3_03EF:
        add    esp, 0xc
        push   ebx
        call   sub_1_27E8
loc_3_03F8:
        add    esp, 4
        push   dword ptr [ebp + 0x18]
        call   sub_2_0080
loc_3_0403:
        add    esp, 4
        mov    dword ptr [ebx + 0x126], eax
        test   eax, eax
        je     loc_3_042C
        push   dword ptr [ebp + 0x18]
        call   sub_2_0078
loc_3_0418:
        add    esp, 4
        mov    dword ptr [ebx + 0x12a], eax
        jmp    loc_3_042C
loc_3_0423:
        push   ebx
        call   sub_3_0433
        add    esp, 4
loc_3_042C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0433:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x120]
        test   eax, eax
        je     loc_3_044F
        push   0
        push   eax
        call   sub_2_00E0
loc_3_044C:
        add    esp, 8
loc_3_044F:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        call   sub_2_00CC
loc_3_045B:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        call   sub_2_00BC
loc_3_0467:
        pop    esi
        pop    ebp
        ret    
loc_3_046A:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        call   sub_1_280C
loc_3_047B:
        mov    esi, eax
        test   esi, esi
        je     loc_3_04CB
        mov    edi, dword ptr [ebp + 8]
loc_3_0484:
        mov    eax, edi
        mov    edx, dword ptr [esi + 0x120]
loc_3_048C:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_3_04AC
        or     cl, cl
        je     loc_3_04A8
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_3_04AC
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_3_048C
loc_3_04A8:
        xor    eax, eax
        jmp    loc_3_04B1
loc_3_04AC:
        sbb    eax, eax
        sbb    eax, -1
loc_3_04B1:
        test   eax, eax
        je     loc_3_04C7
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0068
        push   eax
        call   sub_1_27F4
loc_3_04C1:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_0484
loc_3_04C7:
        test   esi, esi
        jne    loc_3_04E2
loc_3_04CB:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xc], 0xfffffff6
        mov    dword ptr [eax], 0xfffffff6
        xor    eax, eax
        jmp    loc_3_069E
loc_3_04E2:
        cmp    dword ptr [esi + 0x64], 0x534d5446
        jne    loc_3_0502
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xc], 0xfffffffe
        mov    dword ptr [eax], 0xfffffffe
        xor    eax, eax
        jmp    loc_3_069E
loc_3_0502:
        push   esi
        call   sub_1_174A
loc_3_0508:
        add    esp, 4
        test   eax, eax
        jne    loc_3_0526
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xc], 0xfffffffe
        mov    dword ptr [eax], 0xfffffffe
        xor    eax, eax
        jmp    loc_3_069E
loc_3_0526:
        mov    eax, dword ptr [esi + 0x113]
        test   eax, eax
        je     loc_3_0562
        mov    word ptr [0], ax              ; -> obj1:0x0070
        push   0                             ; -> obj1:0x0070
        mov    dword ptr [4], 0              ; -> obj1:0x0074
        mov    word ptr [2], 4               ; -> obj1:0x0072
        mov    dword ptr [0x1c], esi         ; -> obj1:0x008C
        call   sub_1_0367
loc_3_0559:
        add    esp, 4
        mov    dword ptr [esi + 0x117], eax
loc_3_0562:
        lea    edi, [esi + 0x4c]
        xor    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        xor    ecx, ecx
        mov    eax, 0xffffffff
        mov    byte ptr [esi + 0x100], cl
        mov    byte ptr [esi + 0x11b], cl
        mov    byte ptr [esi + 0x3b], cl
        mov    dword ptr [esi + 0xe0], ecx
        mov    dword ptr [esi + 0xc8], ecx
        mov    dword ptr [esi + 0xd4], eax
        mov    dword ptr [esi + 0xa0], eax
        mov    dword ptr [esi + 0xa4], eax
        lea    edx, [esi + 0x88]
        lea    ebx, [esi + 0x30]
        mov    dword ptr [esi + 0x12e], edx
        mov    dword ptr [esi + 0xf4], ebx
        mov    dword ptr [esi + 0x9c], 1
        lea    eax, [esi + 0x38]
        mov    dword ptr [esi + 0x34], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0xf8], eax
        mov    edi, 0xa
        mov    dword ptr [esi + 0x11c], ecx
        mov    dword ptr [esi + 0x98], 0x1c200
        mov    byte ptr [esi + 0xaa], 8
        and    byte ptr [esi + 0xfc], 1
loc_3_05F3:
        push   0                             ; -> loc_1_171E
        lea    edx, [esi + 0x146]
        push   0                             ; -> loc_1_118B
        push   0                             ; -> loc_1_129C
        push   0                             ; -> loc_1_1572
        push   0                             ; -> loc_1_12E4
        push   esi
        push   dword ptr [esi + 0x132]
        push   dword ptr [esi + 0xc4]
        push   edx
        call   sub_1_1BBD
loc_3_0625:
        cmp    eax, 0x1004
        mov    ebx, eax
        jne    loc_3_0631
        dec    edi
        jne    loc_3_05F3
loc_3_0631:
        test   ebx, ebx
        je     loc_3_066E
        push   esi
        call   sub_1_178B
loc_3_063B:
        add    esp, 4
        cmp    ebx, 0x1005
        jne    loc_3_065A
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xc], 0xfffffff6
        mov    dword ptr [eax], 0xfffffff6
        xor    eax, eax
        jmp    loc_3_069E
loc_3_065A:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xc], 0xfffffffe
        mov    dword ptr [eax], 0xfffffffe
        xor    eax, eax
        jmp    loc_3_069E
loc_3_066E:
        mov    eax, dword ptr [esi + 0x162]
        push   esi
        mov    dword ptr [esi + 0x132], eax
        call   sub_1_27F1
loc_3_0680:
        add    esp, 4
        push   esi
        call   sub_1_27EA
loc_3_0689:
        add    esp, 4
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xc], 0
        mov    dword ptr [eax], 0
        mov    eax, esi
loc_3_069E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_3_06A3:
        push   ebp
        mov    ecx, 0xc
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        lea    esi, [eax + 0x94]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_3_06CB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_27EB
loc_3_06DA:
        add    esp, 4
        xor    edi, edi
        push   esi
        call   sub_1_27E9
loc_3_06E5:
        add    esp, 4
        mov    dword ptr [esi + 0x34], edi
        or     byte ptr [esi + 0x11b], 1
        push   esi
        mov    dword ptr [esi + 0x4c], edi
        call   sub_1_1B18
loc_3_06FB:
        add    esp, 4
        mov    dword ptr [esi + 0xc], edi
        test   byte ptr [esi + 0x100], 0x80
        jne    loc_3_0745
        mov    ebx, dword ptr [esi + 0x58]
loc_3_070D:
        call   sub_1_2824
loc_3_0712:
        mov    edi, eax
loc_3_0714:
        cmp    dword ptr [esi + 0x58], 0
        jbe    loc_3_0745
        cli    
        push   esi
        call   sub_1_1089
loc_3_0721:
        add    esp, 4
        sti    
        mov    eax, dword ptr [esi + 0x58]
        cmp    ebx, eax
        je     loc_3_0730
        mov    ebx, eax
        jmp    loc_3_070D
loc_3_0730:
        call   sub_1_2824
loc_3_0735:
        sub    eax, edi
        cmp    eax, 0x7530
        jbe    loc_3_0714
        mov    dword ptr [esi + 0xc], 0xfffffffe
loc_3_0745:
        call   sub_1_2824
loc_3_074A:
        mov    edi, eax
loc_3_074C:
        cli    
        push   esi
        call   sub_1_1089
loc_3_0753:
        add    esp, 4
        sti    
        call   sub_1_2824
loc_3_075C:
        sub    eax, edi
        cmp    eax, 0xc8
        jb     loc_3_074C
        push   esi
        call   sub_1_17F1
loc_3_076B:
        add    esp, 4
        mov    bl, byte ptr [esi + 0x11b]
        test   bl, 8
        je     loc_3_078F
        push   0
        and    bl, 0xf7
        push   dword ptr [esi + 0x3c]
        call   sub_1_281C
loc_3_0786:
        add    esp, 8
        mov    byte ptr [esi + 0x11b], bl
loc_3_078F:
        mov    bl, byte ptr [esi + 0x11b]
        test   bl, 4
        je     loc_3_07B0
        push   0
        and    bl, 0xfb
        push   dword ptr [esi + 0x44]
        call   sub_1_281C
loc_3_07A7:
        add    esp, 8
        mov    byte ptr [esi + 0x11b], bl
loc_3_07B0:
        or     byte ptr [esi + 0x11b], 0x20
        call   sub_1_0C94
loc_3_07BC:
        push   esi
        call   sub_3_0433
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_3_07CF:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0xf8]
        mov    dword ptr [ecx + 0xc], 0
        mov    dl, byte ptr [eax]
        mov    eax, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [ecx + 0xf8], eax
        mov    byte ptr [eax], dl
        mov    eax, 1
        ret    
loc_3_07F7:
        push   ebp
        mov    ecx, 0x10
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, 0                        ; -> loc_1_00F0
        mov    edx, dword ptr [ebp + 0xc]
        mov    edi, edx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebp
        mov    dword ptr [ecx + 0xc], 0
        mov    eax, dword ptr [ecx + 0x40]
        mov    dword ptr [edx + 0x30], eax
        mov    eax, dword ptr [ecx + 0x48]
        mov    dword ptr [edx + 0x2c], eax
        mov    eax, 1
        ret    
loc_3_082C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_3_085F
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x58]
        mov    ecx, dword ptr [edi]
        cli    
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        sti    
        test   ecx, ecx
        je     loc_3_087C
        cmp    dword ptr [esi + 0xe0], eax
        je     loc_3_087C
        push   2
        push   esi
        call   sub_1_1A00
loc_3_085A:
        add    esp, 8
        jmp    loc_3_087C
loc_3_085F:
        cli    
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        lea    edi, [esi + 0x4c]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        sti    
        and    byte ptr [esi + 0xc9], 0xfe
        push   esi
        call   sub_1_1B18
loc_3_0879:
        add    esp, 4
loc_3_087C:
        mov    eax, 1
        pop    edi
        mov    dword ptr [esi + 0xc], 0
        pop    esi
        pop    ebp
        ret    
loc_3_088C:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0xf0], eax
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_3_08AA
        mov    dword ptr [ecx + 0xf4], eax
loc_3_08AA:
        mov    eax, 1
        pop    ebp
        mov    dword ptr [ecx + 0xc], 0
        ret    
loc_3_08B8:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [esi + 0xc8]
        mov    dword ptr [esi + 0xcc], eax
        and    dh, 0xbf
        mov    dword ptr [esi + 0xd0], ecx
        mov    dword ptr [esi + 0xc8], edx
        test   eax, eax
        je     loc_3_090B
        cli    
        or     dh, 0x40
        mov    dword ptr [esi + 0xc8], edx
        sti    
        mov    eax, dword ptr [esi + 0xf4]
        mov    ecx, dword ptr [eax]
        test   dword ptr [esi + 0xf0], ecx
        je     loc_3_090B
        push   4
        push   esi
        call   sub_1_1A00
loc_3_0908:
        add    esp, 8
loc_3_090B:
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    
loc_3_091A:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    ecx, -1
        mov    edx, dword ptr [ebp + 8]
        je     loc_3_0932
        mov    eax, dword ptr [edx + 0x40]
        cmp    ecx, eax
        jb     loc_3_0932
        mov    ecx, eax
loc_3_0932:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        jne    loc_3_093E
        mov    ecx, 0xffffffff
loc_3_093E:
        mov    dword ptr [edx + 0xd8], eax
        mov    dword ptr [edx + 0xd4], ecx
        cmp    dword ptr [edx + 0x4c], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edx + 0xdc], eax
        jne    loc_3_0965
        mov    eax, dword ptr [edx + 0x12e]
        mov    dword ptr [eax], 0
loc_3_0965:
        call   sub_1_0C94
loc_3_096A:
        mov    eax, 1
        pop    ebp
        ret    
loc_3_0971:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    edx, -1
        mov    ecx, dword ptr [ebp + 8]
        je     loc_3_0992
        test   byte ptr [ecx + 0x11b], 0x10
        je     loc_3_0992
        mov    eax, dword ptr [ecx + 0x48]
        cmp    eax, edx
        jae    loc_3_0992
        mov    edx, eax
loc_3_0992:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        jne    loc_3_099B
        xor    edx, edx
loc_3_099B:
        mov    dword ptr [ecx + 0xe0], edx
        mov    dword ptr [ecx + 0xe4], eax
        mov    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [ecx + 0x58], edx
        mov    dword ptr [ecx + 0xe8], eax
        jae    loc_3_09BC
        or     byte ptr [ecx + 0xc9], 2
loc_3_09BC:
        mov    eax, 1
        pop    ebp
        ret    
loc_3_09C3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 0x146
        push   eax
        call   sub_1_1D06
loc_3_09D5:
        xor    ecx, ecx
        mov    cl, al
        mov    eax, dword ptr [ebp + 0xc]
        and    ecx, 0xf0
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 1
        ret    
loc_3_09EB:
        mov    eax, 1
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x19  RW-
        db  043h, 06Ch, 061h, 073h, 073h, 000h, 000h, 000h, 04Dh, 06Fh, 064h, 065h, 06Dh, 000h, 000h, 000h
        db  053h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 000h
VxD__LDATA ENDS
