; AFVXD -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 6159 instructions recovered by recursive descent

Declare_Virtual_Device AFVXD, 0, 0, AFVXD_Control, 00495h, Undefined_Init_Order
;   DDK version 0400h   init_order 05400C000h   flags 00000h
;   service table: obj1:0x3ccc, 3 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4268  R-X
        db  056h, 057h, 08Bh, 074h, 024h, 010h, 066h, 083h, 03Eh, 002h, 075h, 021h, 08Bh, 07Ch, 024h, 00Ch
        db  066h, 08Bh, 006h, 066h, 089h, 007h, 083h, 0C7h, 008h, 066h, 08Bh, 046h, 002h, 066h, 089h, 047h
        db  0FAh, 08Bh, 046h, 004h, 089h, 047h, 0FCh, 02Bh, 0C0h, 0ABh, 0ABh, 0EBh, 016h, 08Bh, 044h, 024h
        db  014h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 0C8h, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h
        db  003h, 0F3h, 0A4h, 05Fh, 05Eh, 0C3h
sub_1_0046:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    word ptr [esi], 2
        jne    loc_1_0076
        mov    edx, dword ptr [esp + 0x10]
        mov    ax, word ptr [esi]
        cmp    word ptr [edx], ax
        jne    loc_1_0072
        mov    ax, word ptr [esi + 2]
        cmp    word ptr [edx + 2], ax
        jne    loc_1_0072
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [edx + 4], eax
        jne    loc_1_0072
        jmp    loc_1_009E
loc_1_0072:
        sub    eax, eax
        jmp    loc_1_00A3
loc_1_0076:
        mov    edi, dword ptr [esp + 0x14]
        sub    edi, 2
        add    esi, 2
        mov    edx, dword ptr [esp + 0x10]
        add    edx, 2
loc_1_0087:
        mov    eax, edi
        dec    edi
        or     eax, eax
        jle    loc_1_009E
        mov    ecx, esi
        inc    esi
        mov    eax, edx
        inc    edx
        mov    al, byte ptr [eax]
        cmp    byte ptr [ecx], al
        je     loc_1_0087
        sub    eax, eax
        jmp    loc_1_00A3
loc_1_009E:
        mov    eax, 1
loc_1_00A3:
        pop    edi
        pop    esi
        ret    
sub_1_00A6:
        mov    edx, dword ptr [esp + 4]
        cmp    word ptr [edx], 2
        jne    loc_1_00C3
        cmp    word ptr [edx + 2], 0
        jne    loc_1_00BF
        cmp    dword ptr [edx + 4], 0
        jne    loc_1_00BF
        jmp    loc_1_00E0
loc_1_00BF:
        sub    eax, eax
        jmp    loc_1_00E5
loc_1_00C3:
        mov    ecx, dword ptr [esp + 8]
        sub    ecx, 2
        add    edx, 2
loc_1_00CD:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        jle    loc_1_00E0
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0
        je     loc_1_00CD
        sub    eax, eax
        jmp    loc_1_00E5
loc_1_00E0:
        mov    eax, 1
loc_1_00E5:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00F0:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esi + 0x90]
        mov    dword ptr [esp + 0x10], eax
        mov    ecx, dword ptr [esi + 0x74]
        mov    eax, dword ptr [eax + 0x40]
        sub    eax, dword ptr [esp + 0x10]
        cmp    eax, 0x40
        je     loc_1_0278
        mov    eax, dword ptr [esi + 0x28]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   ecx
        call   sub_1_32A1
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0155
        mov    esi, 0x2747
        jmp    loc_1_027D
loc_1_0155:
        push   dword ptr [esp + 0x10]
        call   sub_1_1308
        add    esp, 4
        mov    ebp, eax
        mov    eax, dword ptr [esp + 0x10]
        dec    dword ptr [esi + 0x48]
        inc    dword ptr [eax + 0x2c]
        push   edi
        push   esi
        call   sub_1_341F
        mov    eax, dword ptr [esp + 0x34]
        add    esp, 8
        mov    dword ptr [edi + 0x68], eax
        mov    ecx, dword ptr [ebp + 0x10]
        test   cl, 0x10
        je     loc_1_0192
        dec    dword ptr [esi + 0x88]
        inc    dword ptr [edi + 0x88]
loc_1_0192:
        mov    ebx, dword ptr [ebp + 0x3c]
        mov    dword ptr [edi + 0x94], ebp
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [ebp + 0x24]
        test   cl, 0x20
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [ebp + 0x2c]
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edi + 0x98], ebx
        mov    dword ptr [ebx], edi
        mov    dword ptr [ebp + 0x14], edi
        je     loc_1_01C7
        or     dword ptr [edi + 0x3c], 1
loc_1_01C7:
        push   ebp
        call   sub_1_0AE5
        add    esp, 4
        push   8
        push   edi
        call   sub_1_3250
        add    esp, 8
        push   esi
        push   edi
        call   sub_1_0293
        add    esp, 8
        cmp    dword ptr [ebp + 0x1c], 0x60000002
        jne    loc_1_01F5
        mov    dword ptr [ebp + 0x1c], 0x60000003
loc_1_01F5:
        push   0
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
        and    dword ptr [esi + 0x50], 0xfffffff7
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x40]
        sub    eax, dword ptr [esp + 0x10]
        cmp    eax, 0x40
        je     loc_1_0223
        push   0
        push   8
        push   esi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_0223:
        mov    eax, dword ptr [ebx + 0x2c]
        sub    eax, ebx
        cmp    eax, 0x2c
        je     loc_1_0254
        mov    eax, dword ptr [ebx + 0x34]
        sub    eax, ebx
        cmp    eax, 0x34
        je     loc_1_0244
        push   0
        push   1
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_0244:
        mov    eax, dword ptr [ebx + 0x3c]
        sub    eax, ebx
        cmp    eax, 0x3c
        je     loc_1_026E
        push   0
        push   4
        jmp    loc_1_0265
loc_1_0254:
        test   byte ptr [ebp + 0x1f], 0x10
        je     loc_1_026E
        mov    dword ptr [edi + 0x4c], 7
        push   0
        push   0x20
loc_1_0265:
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_026E:
        mov    eax, dword ptr [esp + 0x28]
        sub    esi, esi
        mov    dword ptr [eax], edi
        jmp    loc_1_027D
loc_1_0278:
        mov    esi, 0x2733
loc_1_027D:
        push   dword ptr [esp + 0x10]
        call   sub_1_127F
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_0293:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        lea    eax, [esp + 8]
        push   eax
        push   0
        push   1
        push   0xfffe
        push   0
        push   0
        push   0
        mov    eax, dword ptr [edi + 0x74]
        push   dword ptr [edi + 0x7c]
        mov    eax, dword ptr [eax + 0xc]
        call   dword ptr [eax + 8]
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_034D
        push   dword ptr [esp + 8]
        call   sub_1_3BB0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_02E0
        mov    eax, 0x2747
        jmp    loc_1_034D
loc_1_02E0:
        lea    eax, [esp + 8]
        push   eax
        push   esi
        push   1
        push   0xfffe
        push   0
        push   0
        push   0
        mov    eax, dword ptr [edi + 0x74]
        push   dword ptr [edi + 0x7c]
        mov    eax, dword ptr [eax + 0xc]
        call   dword ptr [eax + 8]
        add    esp, 0x20
        mov    edi, eax
        or     edi, edi
        je     loc_1_0315
        push   esi
        call   sub_1_3BB6
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_034D
loc_1_0315:
        push   dword ptr [esp + 8]
        push   esi
        push   1
        push   0xfffe
        push   0
        push   0
        mov    eax, dword ptr [esp + 0x28]
        push   0
        push   dword ptr [eax + 0x7c]
        mov    eax, dword ptr [eax + 0x74]
        mov    eax, dword ptr [eax + 0xc]
        call   dword ptr [eax + 0x18]
        add    esp, 0x20
        mov    edi, eax
        push   esi
        call   sub_1_3BB6
        add    esp, 4
        or     edi, edi
        mov    eax, edi
        jne    loc_1_034D
        sub    eax, eax
loc_1_034D:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0360:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x34]
        mov    dword ptr [esp + 0x10], 0
        mov    ebx, dword ptr [edi + 0x74]
        mov    eax, dword ptr [edi + 0xb4]
        mov    dword ptr [esp + 0x18], eax
        add    eax, 6
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [ebx + 0x18]
        mov    esi, dword ptr [esp + 0x3c]
        cmp    esi, eax
        jbe    loc_1_0394
        mov    esi, eax
loc_1_0394:
        cmp    dword ptr [ebx + 0x14], esi
        jbe    loc_1_03A3
        mov    ebp, 0x271e
        jmp    loc_1_0598
loc_1_03A3:
        push   esi
        push   dword ptr [esp + 0x3c]
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_03C0
        mov    ebp, 0x271e
        jmp    loc_1_0598
loc_1_03C0:
        mov    eax, dword ptr [esp + 0x18]
        lea    ecx, [esi - 2]
        mov    dword ptr [eax], 1
        push   esi
        push   ebp
        mov    word ptr [eax + 4], cx
        push   dword ptr [esp + 0x1c]
        call   0
        add    esp, 0xc
        push   esi
        push   ebp
        call   sub_1_3B1F
        add    esp, 8
        mov    ecx, dword ptr [edi + 0x28]
        mov    eax, dword ptr [esp + 0x14]
        movzx  eax, word ptr [eax]
        cmp    eax, ecx
        je     loc_1_0401
        mov    eax, 0x2726
        jmp    loc_1_059A
loc_1_0401:
        inc    dword ptr [edi + 0x88]
        mov    dword ptr [esp + 0x20], ecx
        mov    eax, dword ptr [edi + 0x2c]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [edi + 0x30]
        mov    dword ptr [esp + 0x24], eax
        test   byte ptr [edi + 0x40], 0x20
        jne    loc_1_0492
        lea    eax, [esp + 0x28]
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0xc]
        push   dword ptr [esp + 0x1c]
        call   dword ptr [eax]
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_0445
        push   edi
        call   sub_1_347B
        add    esp, 4
        jmp    loc_1_0598
loc_1_0445:
        cmp    dword ptr [esp + 0x28], 1
        jne    loc_1_0453
        cmp    dword ptr [esp + 0x2c], 1
        je     loc_1_0492
loc_1_0453:
        push   esi
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_1002
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_047F
        push   esi
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_0A1A
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0492
loc_1_047F:
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    ebp, 0x2740
        jmp    loc_1_0598
loc_1_0492:
        push   ebx
        call   0xe90
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_04B4
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    ebp, 0x2747
        jmp    loc_1_0598
loc_1_04B4:
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_0502
        push   0
        call   sub_1_1F60
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        or     eax, eax
        jne    loc_1_04E8
        push   ebx
        call   sub_1_0FBF
        add    esp, 4
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    ebp, 0x2747
        jmp    loc_1_0598
loc_1_04E8:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x48], eax
        push   edi
        mov    dword ptr [edi + 0x98], eax
        push   dword ptr [esp + 0x14]
        call   sub_1_1FDE
        add    esp, 8
loc_1_0502:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   edi
        push   ebx
        call   sub_1_105D
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_1_0557
        push   1
        push   edi
        call   sub_1_3250
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_1_0540
        push   ebx
        call   sub_1_13B9
        add    esp, 4
        or     esi, esi
        jne    loc_1_0557
loc_1_0540:
        mov    dword ptr [ebx + 0x1c], edi
        mov    dword ptr [edi + 0x4c], 2
        push   edi
        call   sub_1_347B
        add    esp, 4
        sub    ebp, ebp
        jmp    loc_1_0598
loc_1_0557:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_057B
        push   dword ptr [esp + 0x10]
        call   sub_1_1FBC
        add    esp, 4
        mov    dword ptr [ebx + 0x48], 0
        mov    dword ptr [edi + 0x98], 0
loc_1_057B:
        push   ebx
        call   sub_1_0FBF
        add    esp, 4
        push   edi
        call   sub_1_347B
        add    esp, 4
        push   esi
        call   sub_1_3720
        add    esp, 4
        mov    ebp, eax
loc_1_0598:
        mov    eax, ebp
loc_1_059A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_05B0:
        sub    eax, eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_05C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [edi + 0x74]
        and    dword ptr [edi + 0x50], 0xffffffef
        cmp    dword ptr [edi + 0x4c], 1
        jne    loc_1_0643
        mov    ebp, dword ptr [edi + 0xb4]
        add    ebp, 6
        mov    esi, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x10], esi
        lea    eax, [esp + 0x10]
        push   eax
        push   ebp
        mov    eax, dword ptr [ebx + 0xc]
        push   dword ptr [edi + 0x7c]
        call   dword ptr [eax + 4]
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_0803
        cmp    esi, dword ptr [esp + 0x10]
        mov    eax, 0x271e
        jne    loc_1_0619
        push   esi
        push   ebp
        push   edi
        call   sub_1_0360
        add    esp, 0xc
loc_1_0619:
        or     eax, eax
        jne    loc_1_0803
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_063D
        mov    dword ptr [edi + 0x54], 0
        push   0
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
        jmp    loc_1_0647
loc_1_063D:
        and    dword ptr [edi + 0x54], 0xffffffef
        jmp    loc_1_0647
loc_1_0643:
        mov    esi, dword ptr [esp + 0x20]
loc_1_0647:
        mov    eax, dword ptr [ebx + 0x18]
        cmp    esi, eax
        jbe    loc_1_0650
        mov    esi, eax
loc_1_0650:
        cmp    dword ptr [ebx + 0x14], esi
        ja     loc_1_07FE
        push   esi
        push   dword ptr [esp + 0x20]
        call   sub_1_3AD8
        mov    dword ptr [esp + 0x24], eax
        add    esp, 8
        or     eax, eax
        jne    loc_1_0673
        jmp    loc_1_07FE
loc_1_0673:
        mov    ebp, dword ptr [edi + 0xb4]
        push   esi
        add    ebp, 6
        push   dword ptr [esp + 0x20]
        push   ebp
        call   0
        add    esp, 0xc
        push   esi
        push   dword ptr [esp + 0x20]
        call   sub_1_3B1F
        add    esp, 8
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_06E4
        push   esi
        push   ebp
        call   sub_1_00A6
        add    esp, 8
        or     eax, eax
        je     loc_1_06E4
        mov    eax, dword ptr [edi + 0x94]
        or     eax, eax
        je     loc_1_06C8
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    dword ptr [edi + 0x94], 0
loc_1_06C8:
        mov    dword ptr [edi + 0x18], 0
        mov    dword ptr [edi + 0x24], 0
        mov    dword ptr [edi + 0x4c], 2
        sub    eax, eax
        jmp    loc_1_0803
loc_1_06E4:
        mov    eax, dword ptr [edi + 0x90]
        mov    eax, dword ptr [eax + 0x20]
        mov    cx, word ptr [ebp]
        cmp    word ptr [eax], cx
        je     loc_1_0721
        push   dword ptr [ebx]
        push   dword ptr [edi + 0x30]
        push   dword ptr [edi + 0x2c]
        push   dword ptr [edi + 0x28]
        call   sub_1_3920
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0717
        mov    eax, 0x273f
        jmp    loc_1_0803
loc_1_0717:
        mov    eax, 0x2726
        jmp    loc_1_0803
loc_1_0721:
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_0792
        mov    eax, dword ptr [edi + 0x94]
        or     eax, eax
        jne    loc_1_076F
        push   0
        push   ebx
        call   sub_1_0890
        add    esp, 8
        or     eax, eax
        jne    loc_1_074A
        mov    eax, 0x2747
        jmp    loc_1_0803
loc_1_074A:
        mov    dword ptr [edi + 0x94], eax
        mov    dword ptr [eax + 0x14], edi
        mov    ecx, dword ptr [edi + 0x90]
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x1c], 0x60000004
        mov    ecx, dword ptr [eax + 0x24]
        mov    dword ptr [edi + 0x18], ecx
        mov    dword ptr [eax + 0x2c], esi
        mov    dword ptr [edi + 0x24], esi
loc_1_076F:
        push   esi
        push   ebp
        push   dword ptr [eax + 0x24]
        call   0
        add    esp, 0xc
        mov    dword ptr [edi + 0x4c], 6
        push   esi
        push   ebp
        push   edi
        call   sub_1_080B
        add    esp, 0xc
        sub    eax, eax
        jmp    loc_1_0803
loc_1_0792:
        inc    dword ptr [edi + 0x88]
        push   ebx
        call   sub_1_3870
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_07B7
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    eax, 0x2747
        jmp    loc_1_0803
loc_1_07B7:
        push   esi
        push   ebp
        push   ebx
        push   edi
        call   sub_1_349F
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_1_07EE
        cmp    esi, 0xff
        je     loc_1_07EE
        push   ebx
        call   sub_1_38E3
        add    esp, 4
        push   edi
        call   sub_1_347B
        add    esp, 4
        push   esi
        call   sub_1_3720
        add    esp, 4
        jmp    loc_1_0803
loc_1_07EE:
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    eax, 0x2733
        jmp    loc_1_0803
loc_1_07FE:
        mov    eax, 0x271e
loc_1_0803:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_080B:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x90]
        mov    esi, dword ptr [eax + 0x48]
        mov    eax, esi
        mov    edi, dword ptr [esi + 0x2c]
        sub    eax, edi
        cmp    eax, -0x2c
        je     loc_1_0878
        mov    ebx, dword ptr [esp + 0x18]
loc_1_082C:
        mov    ebp, edi
        mov    edi, dword ptr [edi]
        push   dword ptr [esp + 0x1c]
        push   ebx
        mov    eax, dword ptr [ebp + 0x2c]
        add    eax, 6
        push   eax
        call   sub_1_0046
        add    esp, 0xc
        or     eax, eax
        je     loc_1_086F
        mov    eax, dword ptr [ebp + 0x20]
        sub    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_3F46
        add    esp, 4
loc_1_086F:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x2c
        jne    loc_1_082C
loc_1_0878:
        mov    eax, dword ptr [esi + 0x2c]
        sub    eax, esi
        cmp    eax, 0x2c
        jne    loc_1_088A
        mov    eax, dword ptr [esp + 0x14]
        and    dword ptr [eax + 0x50], 0xfffffffe
loc_1_088A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h
sub_1_0890:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_08B2
        push   0
        call   sub_1_1F60
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_08B4
        sub    eax, eax
        jmp    loc_1_093F
loc_1_08B2:
        sub    ebx, ebx
loc_1_08B4:
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi + 0x18]
        lea    eax, [eax*2 + 0x40]
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_08E2
        or     ebx, ebx
        je     loc_1_08DE
        push   ebx
        call   sub_1_1FBC
        add    esp, 4
loc_1_08DE:
        sub    eax, eax
        jmp    loc_1_093F
loc_1_08E2:
        mov    eax, dword ptr [esi + 0x18]
        lea    ebp, [eax*2 + 0x40]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 0x1c], 1
        mov    dword ptr [edx + 0x3c], ebx
        lea    eax, [edx + 0x40]
        mov    dword ptr [edx + 0x20], eax
        mov    eax, dword ptr [esi + 0x18]
        add    eax, edx
        add    eax, 0x40
        mov    dword ptr [edx + 0x24], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [edx + 0x2c], eax
        mov    eax, dword ptr [esi + 0x18]
        lea    ecx, [edx + 4]
        mov    dword ptr [edx + 0x28], eax
        lea    eax, [esi + 0x34]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [eax], ecx
        mov    eax, edx
        mov    dword ptr [esi + 0x38], ecx
        inc    dword ptr [edx + 0xc]
loc_1_093F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0944:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x3c]
        or     eax, eax
        je     loc_1_0960
        push   eax
        call   sub_1_1FBC
        add    esp, 4
        mov    dword ptr [esi + 0x3c], 0
loc_1_0960:
        mov    edx, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 0x14], 0
        cmp    dword ptr [esi + 0x30], 0
        je     loc_1_09EB
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x1c], 9
        test   eax, 0x40000000
        je     loc_1_09CC
        sub    eax, eax
        sub    ecx, ecx
        cmp    edx, eax
        jne    loc_1_0990
        mov    ecx, 1
        jmp    loc_1_09A8
loc_1_0990:
        cmp    word ptr [edx + 0x10], 0
        je     loc_1_09A8
        movzx  eax, word ptr [edx + 0x12]
        imul   eax, eax, 0x3e8
        cmp    eax, 1
        sbb    ecx, ecx
        neg    ecx
loc_1_09A8:
        or     ecx, ecx
        je     loc_1_09AE
        jmp    loc_1_09C3
loc_1_09AE:
        push   eax
        push   esi
        call   sub_1_0CB8
        add    esp, 8
        or     eax, eax
        je     loc_1_09CC
        cmp    eax, 0xff
        je     loc_1_09CC
loc_1_09C3:
        push   esi
        call   sub_1_0C90
        add    esp, 4
loc_1_09CC:
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_1_09FA
        cmp    dword ptr [esi + 0x30], 0
        je     loc_1_09FA
        mov    eax, dword ptr [esi + 0x10]
        test   al, 8
        je     loc_1_09E3
        test   al, 1
        je     loc_1_09FA
loc_1_09E3:
        push   esi
        call   sub_1_0D19
        jmp    loc_1_09F7
loc_1_09EB:
        test   byte ptr [esi + 0x10], 0x80
        jne    loc_1_09FA
        push   esi
        call   sub_1_0D7E
loc_1_09F7:
        add    esp, 4
loc_1_09FA:
        pop    esi
        ret    
sub_1_09FC:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_1_0A19
        push   ecx
        call   sub_1_0944
        add    esp, 4
        mov    eax, 1
loc_1_0A19:
        ret    
sub_1_0A1A:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, edi
        mov    ebx, dword ptr [edi + 0x34]
        sub    eax, ebx
        cmp    eax, -0x34
        je     loc_1_0A69
        mov    esi, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [esp + 0x1c]
loc_1_0A36:
        cmp    dword ptr [ebx + 0x18], 9
        lea    ecx, [ebx - 4]
        je     loc_1_0A5E
        mov    eax, dword ptr [ecx + 0x14]
        or     eax, eax
        je     loc_1_0A5E
        cmp    dword ptr [eax + 0x2c], ebp
        jne    loc_1_0A5E
        push   dword ptr [ecx + 0x28]
        push   esi
        push   dword ptr [ecx + 0x20]
        call   sub_1_0046
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_0A70
loc_1_0A5E:
        mov    ebx, dword ptr [ebx]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -0x34
        jne    loc_1_0A36
loc_1_0A69:
        sub    eax, eax
loc_1_0A6B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0A70:
        mov    eax, 1
        jmp    loc_1_0A6B
sub_1_0A77:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x20]
        lea    edi, [esp + 0xc]
        mov    ebx, dword ptr [esi]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        lea    eax, [esp + 0xc]
        push   esi
        push   eax
        mov    eax, dword ptr [ebx + 0x10]
        call   dword ptr [eax + 8]
        add    esp, 8
        or     eax, eax
        jne    loc_1_0ADE
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0x30], eax
        mov    edi, dword ptr [esp + 0x24]
        mov    dword ptr [esi + 0x18], edi
        push   edi
        call   sub_1_0FDD
        add    esp, 4
        lea    eax, [esp + 0xc]
        push   dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x10]
        call   dword ptr [eax + 0x10]
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        mov    eax, 0
        je     loc_1_0ADE
        push   edi
        call   sub_1_0FE5
        add    esp, 4
        mov    eax, esi
loc_1_0ADE:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_0AE5:
        sub    esp, 0x4c
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x58]
        lea    eax, [esp + 0x2c]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esp + 0x50], 0x4655424e
        mov    dword ptr [esp + 0x40], eax
        mov    dword ptr [esp + 0x4c], 0xa
        lea    eax, [esp + 0xc]
        mov    dword ptr [esp + 0x44], eax
        mov    dword ptr [esp + 0x48], 0
        mov    dword ptr [esp + 0x3c], 0x4655424e
        mov    dword ptr [esp + 0x2c], 0
        mov    eax, dword ptr [ecx + 0x18]
        mov    dword ptr [esp + 0x38], eax
        lea    edi, [esp + 0x1c]
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [esp + 0x30], eax
        mov    dword ptr [esp + 0x34], 0
        mov    eax, dword ptr [esp + 0x38]
        add    eax, 0xa
        mov    dword ptr [esp + 8], eax
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 8]
        push   1
        push   eax
        lea    eax, [esp + 0x48]
        push   eax
        lea    eax, [esp + 0x28]
        push   3
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        call   dword ptr [eax + 0x3c]
        movzx  eax, word ptr [esp + 0x28]
        add    esp, 0x14
        add    eax, 2
        mov    edi, dword ptr [esi + 0x20]
        mov    dword ptr [esi + 0x28], eax
        cmp    word ptr [edi], 2
        jne    loc_1_0B91
        add    edi, 8
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
loc_1_0B91:
        pop    edi
        pop    esi
        add    esp, 0x4c
        ret    
sub_1_0B97:
        sub    esp, 0x28
        push   edi
        mov    edx, dword ptr [esp + 0x30]
        mov    eax, dword ptr [esp + 0x38]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [esp + 4], eax
        lea    edi, [esp + 8]
        mov    dword ptr [esp + 0x18], 0x400
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0x200
        mov    dword ptr [esp + 0x24], 0x300
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [esp + 0x28], eax
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [edx + 0x30]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 4]
        push   4
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        call   dword ptr [eax + 0x4c]
        add    esp, 0x10
        pop    edi
        add    esp, 0x28
        ret    
sub_1_0C01:
        sub    esp, 0x38
        push   edi
        mov    edx, dword ptr [esp + 0x40]
        mov    eax, dword ptr [esp + 0x44]
        mov    ecx, dword ptr [edx]
        lea    edi, [esp + 4]
        mov    dword ptr [esp + 0x14], 0x400
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0x200
        mov    dword ptr [esp + 0x20], 0x300
        mov    dword ptr [esp + 0x24], eax
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [edx + 0x30]
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 0x38], 0x4655424e
        mov    dword ptr [esp + 0x28], 0
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [esp + 0x2c], eax
        mov    dword ptr [esp + 0x30], 0
        lea    eax, [esp + 0x4c]
        push   0
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        call   dword ptr [eax + 0x48]
        add    esp, 0x14
        pop    edi
        add    esp, 0x38
        ret    
sub_1_0C90:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x1c], 0x10000008
        je     loc_1_0CB7
        cmp    dword ptr [eax + 0x30], 0
        je     loc_1_0CB7
        mov    dword ptr [eax + 0x1c], 0x10000008
        and    dword ptr [eax + 0x10], 0xfffffff7
        push   eax
        call   sub_1_0D19
        add    esp, 4
loc_1_0CB7:
        ret    
sub_1_0CB8:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        push   0
        mov    ecx, dword ptr [esi]
        inc    dword ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [esp + 0xc], eax
        mov    dword ptr [esp + 0x10], 0xd99 ; -> sub_1_0D99
        mov    dword ptr [esp + 0x14], esi
        push   0
        lea    eax, [esp + 0x28]
        push   4
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        call   dword ptr [eax + 0x1c]
        add    esp, 0x14
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0D06
        push   0
        push   edi
        push   esi
        call   sub_1_0D99
        add    esp, 0xc
        jmp    loc_1_0D11
loc_1_0D06:
        cmp    edi, 0xff
        je     loc_1_0D11
        dec    dword ptr [esi + 0xc]
loc_1_0D11:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
sub_1_0D19:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esi]
        inc    dword ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 0x18]
        or     eax, eax
        je     loc_1_0D31
        inc    dword ptr [eax + 0xc]
loc_1_0D31:
        or     dword ptr [esi + 0x10], 0x80
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0xc], 0xdab  ; -> sub_1_0DAB
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 0x10], esi
        mov    dword ptr [esi + 0x30], 0
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        call   dword ptr [eax + 0xc]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0xff
        je     loc_1_0D76
        push   0
        push   edi
        push   esi
        call   sub_1_0DAB
        add    esp, 0xc
loc_1_0D76:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
sub_1_0D7E:
        mov    ecx, dword ptr [esp + 4]
        push   ecx
        mov    edx, dword ptr [ecx + 8]
        lea    eax, [ecx + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        call   sub_1_3BB6
        add    esp, 4
        ret    
sub_1_0D99:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 0x10], 2
        call   sub_1_09FC
        add    esp, 4
        ret    
sub_1_0DAB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi]
        and    dword ptr [esi + 0x10], 0xffffff7f
        mov    eax, dword ptr [esi + 0x18]
        or     eax, eax
        je     loc_1_0DD7
        push   eax
        call   sub_1_0FE5
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_1_0FBF
        add    esp, 4
loc_1_0DD7:
        push   esi
        call   sub_1_09FC
        add    esp, 4
        mov    ebx, dword ptr [edi + 0x2c]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -0x2c
        je     loc_1_0E1E
        mov    esi, 0x40000003
        mov    ebp, 3
loc_1_0DF6:
        lea    eax, [ebx - 4]
        mov    ebx, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 0x1c]
        cmp    dword ptr [eax + 0x18], esi
        jne    loc_1_0E15
        or     ecx, ecx
        je     loc_1_0E15
        cmp    dword ptr [ecx + 0x4c], ebp
        jne    loc_1_0E15
        push   eax
        call   sub_1_127F
        add    esp, 4
loc_1_0E15:
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -0x2c
        jne    loc_1_0DF6
loc_1_0E1E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 0CDh, 020h, 001h, 080h, 088h, 004h, 0CDh, 020h, 002h, 080h, 088h, 004h, 0CDh, 020h, 03Fh
        db  080h, 001h, 000h, 0CDh, 020h, 003h, 080h, 088h, 004h, 0CDh, 020h, 004h, 080h, 088h, 004h, 0CDh
        db  020h, 00Dh, 080h, 040h, 000h, 0CDh, 020h, 00Eh, 080h, 040h, 000h, 0CDh, 020h, 011h, 080h, 040h
        db  000h, 0CDh, 020h, 005h, 080h, 088h, 004h, 0CDh, 020h, 006h, 080h, 088h, 004h, 0A1h
        dd  loc_1_3BD8               ; fixup
        db  0CDh, 020h, 009h, 080h, 088h, 004h
sub_1_0E6B:
        mov    eax, dword ptr [0x3bd8]       ; -> loc_1_3BD8
        VxDCall  Dev_0488.Service_800Ch          ; cd200c808804
sub_1_0E76:
        mov    eax, dword ptr [0x3bd8]       ; -> loc_1_3BD8
        VxDCall  Dev_0488.Service_800Bh          ; cd200b808804
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x56], dl
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x18]
        add    eax, 0x4c
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        mov    eax, 0
        je     loc_1_0F00
        mov    ebx, dword ptr [esi + 0x18]
        add    ebx, 0x4c
        mov    edi, edx
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    eax, [edx + 0x4c]
        mov    dword ptr [edx + 0x20], eax
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 0x18], 1
        lea    eax, [edx + 0x38]
        mov    dword ptr [edx + 0x3c], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x40]
        mov    dword ptr [edx + 0x44], eax
        mov    dword ptr [eax], eax
        lea    ecx, [edx + 4]
        lea    eax, [esi + 0x2c]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [eax], ecx
        mov    eax, edx
        mov    dword ptr [esi + 0x30], ecx
        inc    dword ptr [edx + 0xc]
loc_1_0F00:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0F04:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi]
        inc    dword ptr [esi + 0xc]
        mov    ebx, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x18], 4
        mov    eax, dword ptr [esi + 0x38]
        sub    eax, esi
        cmp    eax, 0x38
        je     loc_1_0F40
loc_1_0F24:
        push   esi
        call   sub_1_132F
        add    esp, 4
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    eax, dword ptr [esi + 0x38]
        sub    eax, esi
        cmp    eax, 0x38
        jne    loc_1_0F24
loc_1_0F40:
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, esi
        cmp    eax, 0x40
        je     loc_1_0F66
loc_1_0F4A:
        push   esi
        call   sub_1_1308
        add    esp, 4
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, esi
        cmp    eax, 0x40
        jne    loc_1_0F4A
loc_1_0F66:
        mov    eax, dword ptr [esi + 0x48]
        or     eax, eax
        je     loc_1_0F7D
        push   eax
        call   sub_1_1FBC
        add    esp, 4
        mov    dword ptr [esi + 0x48], 0
loc_1_0F7D:
        dec    dword ptr [esi + 0xc]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_0FBB
        cmp    ebx, 0x40000003
        jne    loc_1_0FA5
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_1_0FA5
        push   0
        push   0
        push   0
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
loc_1_0FA5:
        cmp    dword ptr [esi + 0x14], 0
        push   esi
        je     loc_1_0FB3
        call   sub_1_13B9
        jmp    loc_1_0FB8
loc_1_0FB3:
        call   sub_1_1403
loc_1_0FB8:
        add    esp, 4
loc_1_0FBB:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0FBF:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_1_0FDC
        push   ecx
        call   sub_1_0F04
        add    esp, 4
        mov    eax, 1
loc_1_0FDC:
        ret    
sub_1_0FDD:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x28]
        ret    
sub_1_0FE5:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x28]
        dec    ecx
        mov    dword ptr [eax + 0x28], ecx
        jne    loc_1_1001
        cmp    dword ptr [eax + 0xc], 0
        jne    loc_1_1001
        push   eax
        call   sub_1_0F04
        add    esp, 4
loc_1_1001:
        ret    
sub_1_1002:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [eax + 0x2c]
        sub    eax, ebx
        cmp    eax, -0x2c
        je     loc_1_104F
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x20]
loc_1_1020:
        cmp    dword ptr [ebx + 0x14], 4
        lea    eax, [ebx - 4]
        je     loc_1_1042
        mov    ecx, dword ptr [eax + 0x1c]
        cmp    dword ptr [ecx + 0x2c], edi
        jne    loc_1_1042
        push   ebp
        push   esi
        push   dword ptr [eax + 0x20]
        call   sub_1_0046
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1056
loc_1_1042:
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, ebx
        cmp    eax, -0x2c
        jne    loc_1_1020
loc_1_104F:
        sub    eax, eax
loc_1_1051:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1056:
        mov    eax, 1
        jmp    loc_1_1051
sub_1_105D:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x28]
        lea    edi, [esp + 0x14]
        mov    esi, dword ptr [ebx]
        mov    byte ptr [esp + 0x12], 1
        mov    byte ptr [esp + 0x13], 0
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        lea    eax, [esp + 0x12]
        mov    edi, dword ptr [esp + 0x38]
        push   eax
        push   edi
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x44]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        call   dword ptr [eax]
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_1132
        mov    ebp, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 0x18], 0x40000002
        mov    dword ptr [ebx + 0x14], ebp
        mov    ecx, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [esp + 0x30]
        mov    word ptr [ecx], ax
        push   ebx
        call   sub_1_113A
        mov    ecx, dword ptr [esp + 0x30]
        add    esp, 4
        mov    dword ptr [ecx + 0x90], ebx
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [ecx + 0x14], eax
        mov    edx, dword ptr [ebx + 0x24]
        mov    dword ptr [ecx + 0x20], edx
        movzx  eax, word ptr [eax]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [ecx + 0x2c], eax
        mov    dword ptr [ecx + 0x30], edi
        push   ebx
        push   0x1507                        ; -> loc_1_1507
        push   1
        mov    eax, dword ptr [esi + 0x10]
        push   ebp
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   ebx
        push   0x15e4                        ; -> loc_1_15E4
        push   3
        mov    eax, dword ptr [esi + 0x10]
        push   ebp
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   ebx
        push   0x1661                        ; -> loc_1_1661
        push   5
        mov    eax, dword ptr [esi + 0x10]
        push   ebp
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   ebx
        push   0x16de                        ; -> loc_1_16DE
        push   4
        mov    eax, dword ptr [esi + 0x10]
        push   ebp
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        sub    eax, eax
loc_1_1132:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_1_113A:
        sub    esp, 0x4c
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x58]
        lea    eax, [esp + 0x2c]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esp + 0x50], 0x4655424e
        mov    dword ptr [esp + 0x40], eax
        mov    dword ptr [esp + 0x4c], 0xa
        lea    eax, [esp + 0xc]
        mov    dword ptr [esp + 0x44], eax
        mov    dword ptr [esp + 0x48], 0
        mov    dword ptr [esp + 0x3c], 0x4655424e
        mov    dword ptr [esp + 0x2c], 0
        mov    eax, dword ptr [ecx + 0x18]
        mov    dword ptr [esp + 0x38], eax
        lea    edi, [esp + 0x1c]
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [esp + 0x30], eax
        mov    dword ptr [esp + 0x34], 0
        mov    eax, dword ptr [esp + 0x38]
        add    eax, 0xa
        mov    dword ptr [esp + 8], eax
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 8]
        push   0
        push   eax
        lea    eax, [esp + 0x48]
        push   eax
        lea    eax, [esp + 0x28]
        push   3
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        call   dword ptr [eax + 0x3c]
        movzx  eax, word ptr [esp + 0x28]
        add    esp, 0x14
        add    eax, 2
        mov    edi, dword ptr [esi + 0x20]
        mov    dword ptr [esi + 0x24], eax
        cmp    word ptr [edi], 2
        jne    loc_1_11E6
        add    edi, 8
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
loc_1_11E6:
        pop    edi
        pop    esi
        add    esp, 0x4c
        ret    
sub_1_11EC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        push   1
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 0x14], eax
        push   dword ptr [esp + 0x14]
        call   sub_1_0890
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    loc_1_1218
        mov    eax, 1
        jmp    loc_1_1277
loc_1_1218:
        cmp    dword ptr [ebx + 0x28], 1
        sbb    ebp, ebp
        neg    ebp
        push   ebx
        push   esi
        call   sub_1_0A77
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_1_123D
        push   esi
        call   sub_1_09FC
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_1277
loc_1_123D:
        mov    edi, dword ptr [ebx + 0x1c]
        mov    dword ptr [esi + 0x14], edi
        push   esi
        push   ebx
        call   sub_1_12E7
        add    esp, 8
        push   edi
        push   dword ptr [esi + 0x3c]
        call   sub_1_1FDE
        add    esp, 8
        or     ebp, ebp
        je     loc_1_1275
        push   ebx
        push   0x1440                        ; -> loc_1_1440
        push   0
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [ebx + 0x14]
        mov    eax, dword ptr [eax + 0x10]
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
loc_1_1275:
        sub    eax, eax
loc_1_1277:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_127F:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        sub    eax, eax
        mov    ecx, dword ptr [esi + 0x2c]
        mov    ebx, dword ptr [esi + 0x30]
        mov    edi, dword ptr [esi + 0x34]
        cmp    ecx, eax
        je     loc_1_12C0
        cmp    edi, ebx
        jbe    loc_1_12C0
        sub    edi, ebx
        cmp    edi, ecx
        jb     loc_1_12A1
        mov    edi, ecx
loc_1_12A1:
        or     edi, edi
        je     loc_1_12B8
loc_1_12A5:
        push   esi
        call   sub_1_11EC
        add    esp, 4
        or     eax, eax
        jne    loc_1_12B8
        dec    edi
        inc    ebx
        or     edi, edi
        jne    loc_1_12A5
loc_1_12B8:
        mov    dword ptr [esi + 0x2c], edi
        mov    dword ptr [esi + 0x30], ebx
        jmp    loc_1_12C2
loc_1_12C0:
        sub    eax, eax
loc_1_12C2:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_12C6:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 0x34]
        lea    eax, [ecx + 0x40]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 0x44]
        mov    dword ptr [edx + 0x38], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 0x44], esi
        inc    dword ptr [ecx + 0x30]
        pop    esi
        ret    
sub_1_12E7:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 0x34]
        lea    eax, [ecx + 0x38]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 0x3c]
        mov    dword ptr [edx + 0x38], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 0x3c], esi
        inc    dword ptr [ecx + 0x30]
        pop    esi
        ret    
sub_1_1308:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x40]
        lea    edx, [esi + 0x40]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 0x40
        jne    loc_1_1320
        sub    eax, eax
        jmp    loc_1_132D
loc_1_1320:
        mov    ecx, dword ptr [eax]
        sub    eax, 0x34
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        dec    dword ptr [esi + 0x30]
loc_1_132D:
        pop    esi
        ret    
sub_1_132F:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx + 0x38]
        lea    esi, [edx + 0x38]
        mov    ecx, eax
        sub    ecx, edx
        cmp    ecx, 0x38
        jne    loc_1_1347
        sub    eax, eax
        jmp    loc_1_1354
loc_1_1347:
        mov    ecx, dword ptr [eax]
        sub    eax, 0x34
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        dec    dword ptr [edx + 0x30]
loc_1_1354:
        pop    esi
        ret    
        db  083h, 0ECh, 024h, 057h, 08Bh, 054h, 024h, 02Ch, 08Bh, 044h, 024h, 030h, 08Bh, 00Ah, 08Dh, 07Ch
        db  024h, 004h, 0C7h, 044h, 024h, 014h, 001h, 004h, 000h, 000h, 0C7h, 044h, 024h, 018h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 01Ch, 000h, 002h, 000h, 000h, 0C7h, 044h, 024h, 020h, 000h, 002h
        db  000h, 000h, 089h, 044h, 024h, 024h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Bh, 042h, 014h, 089h
        db  044h, 024h, 004h, 0FFh, 074h, 024h, 038h, 08Dh, 044h, 024h, 018h, 0FFh, 074h, 024h, 038h, 050h
        db  08Dh, 044h, 024h, 010h, 050h, 08Bh, 041h, 010h, 0FFh, 050h, 04Ch, 083h, 0C4h, 010h, 05Fh, 083h
        db  0C4h, 024h, 0C3h
sub_1_13B9:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 0xc], 0x141e ; -> sub_1_141E
        mov    dword ptr [esp + 0x10], esi
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0xff
        je     loc_1_13FB
        push   0
        push   edi
        push   esi
        call   sub_1_141E
        add    esp, 0xc
loc_1_13FB:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
sub_1_1403:
        mov    edx, dword ptr [esp + 4]
        push   edx
        mov    ecx, dword ptr [edx + 8]
        lea    eax, [edx + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_3BB6
        add    esp, 4
        ret    
sub_1_141E:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    dword ptr [eax + 0x14], 0
        call   sub_1_1403
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1440:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x20]
        add    eax, 6
        mov    dword ptr [esp + 0x10], eax
        mov    ebx, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x18]
        sub    ebx, 6
        mov    edi, dword ptr [ebp]
        push   ebp
        call   sub_1_127F
        add    esp, 4
        mov    esi, dword ptr [ebp + 0x1c]
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_14FA
        mov    eax, dword ptr [ebp + 0x38]
        sub    eax, ebp
        cmp    eax, 0x38
        je     loc_1_14FA
        or     esi, esi
        je     loc_1_14FA
        cmp    dword ptr [edi + 0x18], ebx
        jb     loc_1_14FA
        cmp    dword ptr [edi + 0x14], ebx
        ja     loc_1_14FA
        mov    eax, dword ptr [esp + 0x10]
        movzx  eax, word ptr [eax]
        cmp    dword ptr [esi + 0x28], eax
        jne    loc_1_14FA
        push   ebp
        call   sub_1_132F
        mov    edi, eax
        add    esp, 4
        push   ebx
        mov    dword ptr [edi + 0x1c], 0x60000002
        push   dword ptr [esp + 0x14]
        push   dword ptr [edi + 0x24]
        call   0
        add    esp, 0xc
        mov    dword ptr [edi + 0x2c], ebx
        inc    dword ptr [esi + 0x48]
        inc    dword ptr [esi + 0x88]
        inc    dword ptr [edi + 0xc]
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esp + 0x38]
        or     dword ptr [edi + 0x10], 0x40
        mov    dword ptr [eax], 0x17a0       ; -> loc_1_17A0
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [eax], edi
        mov    eax, 0x18
        jmp    loc_1_14FF
loc_1_14FA:
        mov    eax, 0xc00000d0
loc_1_14FF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_1507:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [edx + 0x10]
        or     ecx, 1
        mov    dword ptr [edx + 0x10], ecx
        test   cl, 0x40
        je     loc_1_1520
        jmp    loc_1_15DF
loc_1_1520:
        test   cl, 2
        je     loc_1_1539
        cmp    dword ptr [edx + 0xc], 0
        jne    loc_1_1539
        push   edx
        call   sub_1_0944
        add    esp, 4
        jmp    loc_1_15DF
loc_1_1539:
        mov    eax, dword ptr [edx + 0x1c]
        cmp    eax, 9
        je     loc_1_15DF
        cmp    eax, 0x10000008
        je     loc_1_15DF
        mov    edi, dword ptr [esp + 0x24]
        and    edi, 2
        je     loc_1_1562
        mov    dword ptr [edx + 0x1c], 0x10000008
        jmp    loc_1_1575
loc_1_1562:
        test   cl, 4
        mov    dword ptr [edx + 0x1c], 0x10000007
        jne    loc_1_1575
        mov    dword ptr [edx + 0x1c], 0x50000005
loc_1_1575:
        mov    esi, dword ptr [edx + 0x14]
        cmp    eax, 0x60000002
        je     loc_1_15DF
        or     esi, esi
        je     loc_1_15DF
        or     edi, edi
        je     loc_1_15A0
        or     ecx, 0x20
        mov    dword ptr [edx + 0x10], ecx
        cmp    dword ptr [esi + 0x4c], 3
        je     loc_1_15A0
        push   esi
        call   sub_1_35D4
        add    esp, 4
        or     dword ptr [esi + 0x3c], 1
loc_1_15A0:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_15BA
        or     edi, edi
        jne    loc_1_15BA
        mov    eax, dword ptr [esi + 0x98]
        mov    ecx, dword ptr [eax + 0x2c]
        sub    ecx, eax
        cmp    ecx, 0x2c
        jne    loc_1_15DF
loc_1_15BA:
        cmp    edi, 1
        cmc    
        sbb    eax, eax
        and    eax, 0x2746
        push   eax
        push   0x20
        push   esi
        call   sub_1_3BCC
        add    esp, 0xc
        push   0x2746
        push   esi
        call   sub_1_3BD2
        add    esp, 8
loc_1_15DF:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_15E4:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x20]
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [edi], 0
        test   byte ptr [esi + 0x1f], 0x20
        je     loc_1_164D
        mov    eax, dword ptr [esi + 0x14]
        test   byte ptr [eax + 0x40], 0x80
        je     loc_1_1615
        push   0x2746
        push   0x20
        push   eax
        call   sub_1_3BCC
        add    esp, 0xc
        jmp    loc_1_164D
loc_1_1615:
        push   0x3bdc                        ; -> loc_1_3BDC
        mov    edi, dword ptr [esi + 0x3c]
        push   dword ptr [esp + 0x2c]
        push   0
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x2c]
        push   edi
        call   sub_1_182E
        add    esp, 0x18
        or     eax, eax
        je     loc_1_1646
        mov    eax, 0x18
        or     dword ptr [esi + 0x10], 0x10
        mov    dword ptr [edi + 0x44], esi
        jmp    loc_1_165E
loc_1_1646:
        mov    eax, 0x17
        jmp    loc_1_165E
loc_1_164D:
        push   esi
        call   sub_1_0C90
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [edi], ecx
loc_1_165E:
        pop    edi
        pop    esi
        ret    
loc_1_1661:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x20]
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [edi], 0
        test   byte ptr [esi + 0x1f], 0x20
        je     loc_1_16CA
        mov    eax, dword ptr [esi + 0x14]
        test   byte ptr [eax + 0x40], 0x80
        je     loc_1_1692
        push   0x2746
        push   0x20
        push   eax
        call   sub_1_3BCC
        add    esp, 0xc
        jmp    loc_1_16CA
loc_1_1692:
        push   0x3be0                        ; -> loc_1_3BE0
        mov    edi, dword ptr [esi + 0x3c]
        push   dword ptr [esp + 0x2c]
        push   0
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x2c]
        push   edi
        call   sub_1_182E
        add    esp, 0x18
        or     eax, eax
        je     loc_1_16C3
        mov    eax, 0x18
        or     dword ptr [esi + 0x10], 0x10
        mov    dword ptr [edi + 0x44], esi
        jmp    loc_1_16DB
loc_1_16C3:
        mov    eax, 0x17
        jmp    loc_1_16DB
loc_1_16CA:
        push   esi
        call   sub_1_0C90
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [edi], ecx
loc_1_16DB:
        pop    edi
        pop    esi
        ret    
loc_1_16DE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x14]
        sub    ecx, 6
        mov    eax, dword ptr [ebx + 0x1c]
        or     eax, eax
        je     loc_1_1725
        mov    eax, dword ptr [eax + 0x94]
        or     eax, eax
        je     loc_1_1725
        push   ecx
        push   dword ptr [eax + 0x24]
        lea    eax, [esi + 6]
        push   eax
        call   sub_1_0046
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1725
        mov    ebp, dword ptr [esp + 0x34]
        mov    edi, dword ptr [esp + 0x30]
        mov    dword ptr [ebp], edi
        sub    eax, eax
        jmp    loc_1_179B
loc_1_1725:
        mov    ebp, dword ptr [esp + 0x34]
        mov    dword ptr [ebp], 0
        test   byte ptr [ebx + 0x1b], 0x40
        je     loc_1_1796
        push   0x3bdc                        ; -> loc_1_3BDC
        push   0
        push   1
        mov    edi, dword ptr [esp + 0x3c]
        push   0
        push   edi
        push   dword ptr [ebx + 0x48]
        call   sub_1_182E
        add    esp, 0x18
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_178F
        mov    ecx, dword ptr [ebx + 0x2c]
        mov    dword ptr [ecx], 1
        mov    ax, word ptr [esi + 4]
        mov    word ptr [ecx + 4], ax
        movzx  eax, ax
        add    eax, 2
        push   eax
        lea    eax, [esi + 6]
        push   eax
        add    ecx, 6
        push   ecx
        call   0
        add    ebx, 0x48
        mov    eax, dword ptr [esp + 0x48]
        add    esp, 0xc
        mov    dword ptr [eax], ebx
        mov    eax, 0x18
        jmp    loc_1_179B
loc_1_178F:
        mov    dword ptr [ebp], edi
        sub    eax, eax
        jmp    loc_1_179B
loc_1_1796:
        mov    eax, 0x17
loc_1_179B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_17A0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 0x14]
        and    dword ptr [esi + 0x10], 0xffffffbf
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_17E7
        push   esi
        push   edi
        call   sub_1_12C6
        add    esp, 8
        push   0
        push   8
        push   ebx
        call   sub_1_3BCC
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x10]
        test   al, 1
        je     loc_1_17DF
        mov    dword ptr [esi + 0x1c], 0x50000005
        jmp    loc_1_180F
loc_1_17DF:
        or     eax, 8
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_1_180F
loc_1_17E7:
        cmp    dword ptr [esi + 0x1c], 0x60000002
        jne    loc_1_180F
        mov    dword ptr [esi + 0x1c], 1
        dec    dword ptr [ebx + 0x48]
        cmp    dword ptr [esi + 0xc], 1
        jbe    loc_1_180F
        push   esi
        push   dword ptr [ebx + 0x90]
        call   sub_1_12E7
        add    esp, 8
loc_1_180F:
        push   edi
        call   sub_1_0FBF
        add    esp, 4
        push   esi
        call   sub_1_09FC
        add    esp, 4
        push   ebx
        call   sub_1_347B
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_182E:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        or     ebx, ebx
        je     loc_1_1893
        cmp    dword ptr [ebx + 0x14], 0
        ja     loc_1_1893
        cmp    dword ptr [ebx + 0x1c], 0
        ja     loc_1_1893
        mov    eax, dword ptr [ebx + 0x10]
        cmp    dword ptr [ebx + 0xc], eax
        jb     loc_1_1893
        mov    ebp, dword ptr [esp + 0x1c]
        test   ebp, 0x400
        mov    edi, dword ptr [esp + 0x18]
        jne    loc_1_1886
        cmp    edi, 0x111c
        jae    loc_1_1886
        push   0x111c
        call   sub_1_3F0F
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_18C8
        push   edi
        call   sub_1_3F0F
        add    esp, 4
        mov    esi, eax
        jmp    loc_1_189D
loc_1_1886:
        push   edi
        call   sub_1_3F0F
        add    esp, 4
        mov    esi, eax
        jmp    loc_1_189D
loc_1_1893:
        sub    esi, esi
        mov    edi, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [esp + 0x1c]
loc_1_189D:
        or     esi, esi
        jne    loc_1_18C8
        cmp    dword ptr [esp + 0x20], 0
        jne    loc_1_18C4
        test   ebp, 0x40
        je     loc_1_18BB
        add    dword ptr [ebx + 0x1c], edi
        and    ebp, 0xffffffdf
        mov    dword ptr [ebx + 0x20], ebp
        jmp    loc_1_18C4
loc_1_18BB:
        add    dword ptr [ebx + 0x14], edi
        and    ebp, 0xffffffbf
        mov    dword ptr [ebx + 0x18], ebp
loc_1_18C4:
        sub    eax, eax
        jmp    loc_1_1908
loc_1_18C8:
        mov    dword ptr [esi + 0x18], ebx
        push   ebp
        call   sub_1_3851
        add    esp, 4
        or     dword ptr [esi + 0x1c], eax
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 0x24]
        inc    dword ptr [eax + 0x88]
        or     ecx, ecx
        jne    loc_1_18EA
        lea    ecx, [esi + 0x48]
loc_1_18EA:
        lea    eax, [esi + 0x5c]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], 0x190d   ; -> loc_1_190D
        mov    dword ptr [ecx + 0xc], esi
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, esi
loc_1_1908:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_190D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x18]
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x1c]
        and    eax, 1
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 0x44]
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xfffffffd
        add    eax, 4
        mov    dword ptr [esp + 0x10], eax
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_195F
        mov    dword ptr [ebp + 0x18], edi
        push   dword ptr [esp + 0x20]
        push   ebp
        call   sub_1_3FF1
        add    esp, 8
        push   ebp
        push   esi
        call   sub_1_202A
        mov    eax, dword ptr [esp + 0x28]
        add    esp, 8
        add    dword ptr [esi + 0x10], eax
        jmp    loc_1_1976
loc_1_195F:
        or     dword ptr [ebx + 0x10], 0x20
        cmp    dword ptr [edi + 0x4c], 3
        je     loc_1_196D
        or     dword ptr [edi + 0x3c], 1
loc_1_196D:
        push   ebp
        call   sub_1_3F46
        add    esp, 4
loc_1_1976:
        or     ebx, ebx
        je     loc_1_197E
        and    dword ptr [ebx + 0x10], 0xffffffef
loc_1_197E:
        mov    esi, dword ptr [edi + 0x4c]
        push   edi
        call   sub_1_347B
        add    esp, 4
        or     eax, eax
        jne    loc_1_19AD
        cmp    esi, 3
        je     loc_1_19AD
        push   dword ptr [esp + 0x1c]
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   dword ptr [esp + 0x14]
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_19AD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_19C0:
        mov    edx, dword ptr [esp + 0x18]
        sub    edx, 0x50
        cmp    dword ptr [edx + 0x10], 0
        je     loc_1_1A33
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_1_19E6
        cmp    eax, 1
        je     loc_1_1A02
        cmp    eax, 2
        je     loc_1_1A19
        mov    eax, 0x2726
        jmp    loc_1_1A38
loc_1_19E6:
        mov    ecx, dword ptr [esp + 8]
        lea    eax, [ecx + 0x14]
        push   eax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   ecx
        push   edx
        call   sub_1_1A39
        add    esp, 0x14
        jmp    loc_1_1A38
loc_1_1A02:
        mov    ecx, dword ptr [esp + 8]
        push   dword ptr [ecx + 0x14]
        lea    eax, [ecx + 0x18]
        push   eax
        push   ecx
        push   edx
        call   sub_1_1B26
        add    esp, 0x10
        jmp    loc_1_1A38
loc_1_1A19:
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        call   sub_1_1BA6
        add    esp, 0x10
        jmp    loc_1_1A38
loc_1_1A33:
        mov    eax, 0x2742
loc_1_1A38:
        ret    
sub_1_1A39:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x40]
        or     eax, eax
        je     loc_1_1B19
        mov    edi, dword ptr [esp + 0x44]
        or     edi, edi
        je     loc_1_1B19
        mov    ebp, dword ptr [esp + 0x48]
        or     ebp, ebp
        je     loc_1_1B19
        mov    ebx, dword ptr [esp + 0x4c]
        or     ebx, ebx
        je     loc_1_1B19
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [esp + 0x10], ecx
        push   0x14
        push   eax
        call   sub_1_3AD8
        add    esp, 8
        mov    esi, eax
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_3AD8
        add    esp, 8
        mov    edi, eax
        push   0x10
        push   ebx
        call   sub_1_3AD8
        add    esp, 8
        mov    ebx, eax
        or     esi, esi
        je     loc_1_1AED
        or     edi, edi
        je     loc_1_1AED
        or     ebx, ebx
        je     loc_1_1AED
        mov    dword ptr [esp + 0x34], 0x4655424e
        mov    dword ptr [esp + 0x24], 0
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 0x30], eax
        mov    dword ptr [esp + 0x28], edi
        mov    dword ptr [esp + 0x2c], 0
        push   ebx
        lea    eax, [esp + 0x28]
        push   ebp
        push   eax
        lea    eax, [esp + 0x20]
        push   esi
        push   eax
        mov    eax, dword ptr [esp + 0x50]
        mov    eax, dword ptr [eax + 0x10]
        call   dword ptr [eax + 0x48]
        add    esp, 0x14
        mov    ebp, eax
        jmp    loc_1_1AF2
loc_1_1AED:
        mov    ebp, 0xc
loc_1_1AF2:
        push   0x10
        push   ebx
        call   sub_1_3B1F
        add    esp, 8
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_3B1F
        add    esp, 8
        push   0x14
        push   esi
        call   sub_1_3B1F
        add    esp, 8
        mov    eax, ebp
        jmp    loc_1_1B1E
loc_1_1B19:
        mov    eax, 0x57
loc_1_1B1E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
sub_1_1B26:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x28]
        or     eax, eax
        je     loc_1_1B99
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_1B99
        push   0x14
        push   eax
        call   sub_1_3AD8
        add    esp, 8
        mov    esi, eax
        mov    ebx, dword ptr [esp + 0x30]
        push   ebx
        push   edi
        call   sub_1_3AD8
        add    esp, 8
        mov    edi, eax
        or     esi, esi
        je     loc_1_1B7B
        or     edi, edi
        je     loc_1_1B7B
        push   ebx
        push   edi
        lea    eax, [esp + 0x18]
        push   esi
        push   eax
        mov    eax, dword ptr [esp + 0x34]
        mov    eax, dword ptr [eax + 0x10]
        call   dword ptr [eax + 0x4c]
        add    esp, 0x10
        mov    ebp, eax
        jmp    loc_1_1B80
loc_1_1B7B:
        mov    ebp, 0xc
loc_1_1B80:
        push   ebx
        push   edi
        call   sub_1_3B1F
        add    esp, 8
        push   0x14
        push   esi
        call   sub_1_3B1F
        add    esp, 8
        mov    eax, ebp
        jmp    loc_1_1B9E
loc_1_1B99:
        mov    eax, 0x57
loc_1_1B9E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_1BA6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x1c]
        or     ecx, ecx
        je     loc_1_1C32
        mov    esi, dword ptr [esp + 0x20]
        or     esi, esi
        je     loc_1_1C32
        mov    edi, dword ptr [esp + 0x24]
        or     edi, edi
        je     loc_1_1C32
        mov    ebx, dword ptr [esp + 0x28]
        or     ebx, ebx
        je     loc_1_1C32
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 0x10], eax
        push   dword ptr [esp + 0x14]
        push   ecx
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_3AD8
        add    esp, 8
        mov    edi, eax
        or     ebp, ebp
        je     loc_1_1C0F
        or     edi, edi
        je     loc_1_1C0F
        push   ebx
        push   edi
        push   esi
        push   ebp
        call   sub_1_3A7A
        add    esp, 0x10
        mov    esi, eax
        jmp    loc_1_1C14
loc_1_1C0F:
        mov    esi, 0xc
loc_1_1C14:
        push   dword ptr [esp + 0x14]
        push   ebp
        call   sub_1_3B1F
        add    esp, 8
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_3B1F
        add    esp, 8
        mov    eax, esi
        jmp    loc_1_1C37
loc_1_1C32:
        mov    eax, 0x57
loc_1_1C37:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  056h, 08Bh, 044h, 024h, 008h, 00Bh, 0C0h, 074h, 029h, 08Bh, 074h, 024h, 00Ch, 00Bh, 0F6h, 074h
        db  021h, 0FFh, 036h, 050h, 0E8h, 080h, 01Eh, 000h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 007h
        db  0B8h, 01Eh, 027h, 000h, 000h, 0EBh, 00Dh, 0FFh, 036h, 050h, 0E8h, 0B1h, 01Eh, 000h, 000h, 083h
        db  0C4h, 008h, 02Bh, 0C0h, 05Eh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
loc_1_1C80:
        sub    esp, 8
        push   esi
        push   edi
        sub    esi, esi
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 0x4004667f
        je     loc_1_1C9E
        cmp    eax, 0x40047307
        je     loc_1_1CB5
        jmp    loc_1_1D65
loc_1_1C9E:
        push   dword ptr [esp + 0x14]
        call   sub_1_1D72
        mov    ecx, dword ptr [esp + 0x20]
        add    esp, 4
        mov    dword ptr [ecx], eax
        jmp    loc_1_1D6A
loc_1_1CB5:
        mov    edi, dword ptr [esp + 0x14]
        cmp    dword ptr [edi + 0x2c], 1
        jne    loc_1_1D65
        test   byte ptr [edi + 0x40], 0x10
        je     loc_1_1D65
        mov    eax, dword ptr [edi + 0x94]
        or     eax, eax
        jne    loc_1_1CE6
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_1D6A
loc_1_1CE6:
        lea    ecx, [esp + 8]
        push   8
        push   ecx
        push   5
        push   eax
        call   sub_1_0C01
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1D09
        push   eax
        call   sub_1_3720
        add    esp, 4
        mov    esi, eax
        jmp    loc_1_1D6A
loc_1_1D09:
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_1D23
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_1D28
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_1D6A
loc_1_1D23:
        cmp    dword ptr [esp + 0xc], 0
loc_1_1D28:
        jle    loc_1_1D36
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 0
        jmp    loc_1_1D6A
loc_1_1D36:
        mov    eax, dword ptr [edi + 0x98]
        or     eax, eax
        jne    loc_1_1D4C
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_1D6A
loc_1_1D4C:
        mov    ecx, dword ptr [esp + 0xc]
        neg    ecx
        cmp    dword ptr [eax + 0x10], ecx
        mov    eax, 0
        setbe  al
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], eax
        jmp    loc_1_1D6A
loc_1_1D65:
        mov    esi, 0x2726
loc_1_1D6A:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 8
        ret    
sub_1_1D72:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        sub    eax, eax
        mov    edi, dword ptr [esi + 0x98]
        cmp    edi, eax
        jne    loc_1_1D86
        jmp    loc_1_1DC9
loc_1_1D86:
        cmp    dword ptr [esi + 0x2c], 1
        jne    loc_1_1DB1
        mov    eax, dword ptr [edi + 0x10]
        test   byte ptr [esi + 0x40], 0x10
        jne    loc_1_1DC0
        mov    edx, dword ptr [edi + 0x3c]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -0x3c
        je     loc_1_1DC0
loc_1_1DA1:
        sub    eax, dword ptr [edx + 0x18]
        mov    edx, dword ptr [edx]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -0x3c
        jne    loc_1_1DA1
        jmp    loc_1_1DC0
loc_1_1DB1:
        mov    ecx, dword ptr [edi + 0x2c]
        mov    edx, ecx
        sub    edx, edi
        cmp    edx, 0x2c
        je     loc_1_1DC0
        mov    eax, dword ptr [ecx + 0x20]
loc_1_1DC0:
        mov    ecx, dword ptr [esi + 0x38]
        cmp    eax, ecx
        jbe    loc_1_1DC9
        mov    eax, ecx
loc_1_1DC9:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h
loc_1_1DD0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        inc    dword ptr [esi + 0x88]
        mov    edi, dword ptr [esi + 0x90]
        mov    eax, dword ptr [esp + 0x18]
        add    dword ptr [edi + 0x2c], eax
        mov    dword ptr [edi + 0x34], eax
        push   edi
        call   sub_1_127F
        add    esp, 4
        mov    eax, dword ptr [edi + 0x38]
        sub    eax, edi
        cmp    eax, 0x38
        jne    loc_1_1E11
        push   esi
        call   sub_1_347B
        add    esp, 4
        mov    ebx, 0x2747
        jmp    loc_1_1E50
loc_1_1E11:
        mov    ebp, dword ptr [edi + 0x18]
        mov    dword ptr [edi + 0x18], 0x40000003
        push   2
        push   esi
        call   sub_1_3250
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_1E31
        mov    dword ptr [edi + 0x18], ebp
        jmp    loc_1_1E47
loc_1_1E31:
        mov    dword ptr [esi + 0x4c], 3
        movzx  eax, byte ptr [esp + 0x18]
        mov    dword ptr [esi + 0x44], eax
        mov    dword ptr [esi + 0x54], 0x37
loc_1_1E47:
        push   esi
        call   sub_1_347B
        add    esp, 4
loc_1_1E50:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1E60:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        jne    loc_1_1E6D
        jmp    loc_1_1EB3
loc_1_1E6D:
        mov    edx, 0x4210                   ; -> obj1:0x4210
        mov    esi, dword ptr [0x4260]       ; -> obj1:0x4260
        or     esi, esi
        je     loc_1_1EB3
loc_1_1E7C:
        mov    eax, dword ptr [edx]
        mov    ebx, ecx
        mov    edi, dword ptr [eax + 4]
loc_1_1E83:
        mov    al, byte ptr [edi]
        cmp    al, byte ptr [ebx]
        jne    loc_1_1EA3
        or     al, al
        je     loc_1_1E9F
        mov    al, byte ptr [edi + 1]
        cmp    al, byte ptr [ebx + 1]
        jne    loc_1_1EA3
        add    edi, 2
        add    ebx, 2
        or     al, al
        jne    loc_1_1E83
loc_1_1E9F:
        sub    eax, eax
        jmp    loc_1_1EA8
loc_1_1EA3:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1EA8:
        or     eax, eax
        je     loc_1_1EB9
        add    edx, 4
        or     esi, esi
        jne    loc_1_1E7C
loc_1_1EB3:
        sub    eax, eax
loc_1_1EB5:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1EB9:
        mov    eax, dword ptr [edx]
        jmp    loc_1_1EB5
loc_1_1EBD:
        push   esi
        push   dword ptr [esp + 8]
        call   sub_1_1E60
        add    esp, 4
        or     eax, eax
        je     loc_1_1F2C
        mov    esi, dword ptr [eax + 0x24]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, -0x24
        je     loc_1_1EED
        mov    ecx, 0xa
loc_1_1EDF:
        mov    dword ptr [esi - 0x34], ecx
        mov    esi, dword ptr [esi]
        mov    edx, eax
        sub    edx, esi
        cmp    edx, -0x24
        jne    loc_1_1EDF
loc_1_1EED:
        mov    esi, dword ptr [eax + 0x2c]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, -0x2c
        je     loc_1_1F09
        sub    ecx, ecx
loc_1_1EFB:
        mov    dword ptr [esi + 0x10], ecx
        mov    esi, dword ptr [esi]
        mov    edx, eax
        sub    edx, esi
        cmp    edx, -0x2c
        jne    loc_1_1EFB
loc_1_1F09:
        mov    esi, dword ptr [eax + 0x34]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, -0x34
        je     loc_1_1F25
        sub    ecx, ecx
loc_1_1F17:
        mov    dword ptr [esi + 0x2c], ecx
        mov    esi, dword ptr [esi]
        mov    edx, eax
        sub    edx, esi
        cmp    edx, -0x34
        jne    loc_1_1F17
loc_1_1F25:
        mov    dword ptr [eax + 0x10], 0
loc_1_1F2C:
        pop    esi
        ret    
loc_1_1F2E:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   edi
        call   sub_1_1E60
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_1F4F
        push   edi
        call   sub_1_3AAF
        add    esp, 4
        mov    dword ptr [esi + 0x10], eax
loc_1_1F4F:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1F60:
        push   edi
        push   0x48
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_1F75
        sub    eax, eax
        jmp    loc_1_1FBA
loc_1_1F75:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x12
        rep stosd dword ptr es:[edi], eax
        lea    eax, [edx + 0x24]
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x2c]
        mov    dword ptr [edx + 0x30], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x34]
        mov    dword ptr [edx + 0x38], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x3c]
        mov    dword ptr [edx + 0x40], eax
        mov    dword ptr [eax], eax
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     loc_1_1FB8
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
loc_1_1FB8:
        mov    eax, edx
loc_1_1FBA:
        pop    edi
        ret    
sub_1_1FBC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_20C0
        add    esp, 4
        push   esi
        call   sub_1_2105
        add    esp, 4
        push   esi
        call   sub_1_2124
        add    esp, 4
        pop    esi
        ret    
sub_1_1FDE:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [eax + 0x34]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [eax + 0x38]
        mov    dword ptr [edx + 0xc], eax
        ret    
sub_1_1FF5:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi]
        lea    ecx, [edx + 8]
        mov    esi, dword ptr [eax + 0x74]
        lea    eax, [edi + 0x24]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi + 0x28]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [eax], ecx
        lea    eax, [esi + 0x3c]
        mov    dword ptr [edi + 0x28], ecx
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi + 0x40]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [esi + 0x40], edx
        pop    edi
        pop    esi
        ret    
sub_1_202A:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        test   byte ptr [edx + 0x1c], 1
        je     loc_1_203E
        lea    esi, [ecx + 0x3c]
        jmp    loc_1_2041
loc_1_203E:
        lea    esi, [ecx + 0x34]
loc_1_2041:
        lea    eax, [ecx + 0x2c]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0x30]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x30], edx
        mov    dword ptr [edx + 8], esi
        lea    ecx, [edx + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 4], ecx
        pop    esi
        ret    
sub_1_2064:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x24]
        lea    esi, [ecx + 0x24]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 0x24
        jne    loc_1_207C
        sub    eax, eax
        jmp    loc_1_2090
loc_1_207C:
        mov    ecx, dword ptr [eax]
        sub    eax, 8
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
loc_1_2090:
        pop    esi
        ret    
sub_1_2092:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x2c]
        lea    esi, [ecx + 0x2c]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 0x2c
        jne    loc_1_20AA
        sub    eax, eax
        jmp    loc_1_20BE
loc_1_20AA:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        mov    edx, dword ptr [eax + 0xc]
        lea    ecx, [eax + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
loc_1_20BE:
        pop    esi
        ret    
sub_1_20C0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 0x24]
        sub    eax, edi
        cmp    eax, 0x24
        je     loc_1_2101
        mov    ebx, dword ptr [edi]
loc_1_20D3:
        push   edi
        call   sub_1_2064
        add    esp, 4
        push   eax
        call   sub_1_3F46
        add    esp, 4
        mov    eax, dword ptr [edi + 0x24]
        sub    eax, edi
        sub    eax, 0x24
        cmp    eax, 1
        sbb    esi, esi
        neg    esi
        push   ebx
        call   sub_1_347B
        add    esp, 4
        or     esi, esi
        je     loc_1_20D3
loc_1_2101:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2105:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_1_210A:
        push   esi
        call   sub_1_2092
        add    esp, 4
        or     eax, eax
        je     loc_1_2122
        push   eax
        call   sub_1_3F46
        add    esp, 4
        jmp    loc_1_210A
loc_1_2122:
        pop    esi
        ret    
sub_1_2124:
        push   dword ptr [esp + 4]
        call   sub_1_3BB6
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2140:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        sub    edi, edi
        mov    esi, dword ptr [eax + 0x74]
        mov    ebx, dword ptr [esp + 0x20]
        and    ebx, 0x80000000
        jne    loc_1_217B
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        call   sub_1_3AD8
        mov    dword ptr [esp + 0x20], eax
        add    esp, 8
        cmp    eax, edi
        jne    loc_1_217B
        mov    esi, 0x271e
        jmp    loc_1_2211
loc_1_217B:
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [eax + 0x2c], 2
        jne    loc_1_21F3
        mov    ebp, dword ptr [esp + 0x24]
        or     ebp, ebp
        je     loc_1_21C8
        cmp    dword ptr [esp + 0x28], 0
        je     loc_1_21C8
        mov    eax, dword ptr [esp + 0x28]
        mov    edi, dword ptr [eax]
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, edi
        jae    loc_1_21A3
        mov    edi, eax
loc_1_21A3:
        cmp    dword ptr [esi + 0x14], edi
        ja     loc_1_21BF
        push   edi
        push   ebp
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_21CE
        mov    esi, 0x271e
        jmp    loc_1_2215
loc_1_21BF:
        sub    edi, edi
        mov    esi, 0x271e
        jmp    loc_1_2215
loc_1_21C8:
        sub    ebp, ebp
        mov    dword ptr [esp + 0x28], ebp
loc_1_21CE:
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   ebp
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        call   sub_1_2453
        add    esp, 0x1c
        mov    esi, eax
        jmp    loc_1_2215
loc_1_21F3:
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        call   sub_1_223E
        add    esp, 0x14
        mov    esi, eax
loc_1_2211:
        mov    ebp, dword ptr [esp + 0x24]
loc_1_2215:
        or     ebx, ebx
        jne    loc_1_2229
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        call   sub_1_3B1F
        add    esp, 8
loc_1_2229:
        or     edi, edi
        je     loc_1_2237
        push   edi
        push   ebp
        call   sub_1_3B1F
        add    esp, 8
loc_1_2237:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_223E:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x2c], 0
        mov    esi, dword ptr [esp + 0x38]
        mov    dword ptr [esp + 0x18], 0
        mov    eax, dword ptr [esi + 0x74]
        mov    dword ptr [esp + 0x30], eax
        mov    edi, dword ptr [esi + 0x98]
        mov    ebx, dword ptr [esp + 0x44]
        mov    eax, ebx
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x1c], eax
        push   edi
        push   esi
        call   sub_1_254D
        add    esp, 8
        and    ebx, 1
        mov    dword ptr [esp + 0x24], ebx
        or     ebx, ebx
        jne    loc_1_2298
        mov    eax, dword ptr [edi + 0x34]
        sub    eax, edi
        cmp    eax, 0x34
        je     loc_1_22A9
loc_1_2298:
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_22C0
        mov    eax, dword ptr [edi + 0x3c]
        sub    eax, edi
        cmp    eax, 0x3c
        jne    loc_1_22C0
loc_1_22A9:
        mov    eax, dword ptr [esi + 0x94]
        or     eax, eax
        je     loc_1_2386
        test   byte ptr [eax + 0x10], 1
        jmp    loc_1_2384
loc_1_22C0:
        mov    eax, dword ptr [esi + 0x40]
        and    eax, 0x10
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x20], eax
        or     eax, eax
        je     loc_1_22E9
        lea    eax, [edi + 0x2c]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0x1c]
        and    eax, 1
        mov    dword ptr [esp + 0x28], eax
        jmp    loc_1_22FC
loc_1_22E9:
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_22F5
        lea    eax, [edi + 0x3c]
        jmp    loc_1_22F8
loc_1_22F5:
        lea    eax, [edi + 0x34]
loc_1_22F8:
        mov    dword ptr [esp + 0x14], eax
loc_1_22FC:
        mov    eax, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [eax]
        cmp    eax, ebx
        je     loc_1_237F
loc_1_2306:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_231D
        mov    ebp, ebx
        mov    eax, dword ptr [ebx + 0x1c]
        and    eax, 1
        cmp    eax, dword ptr [esp + 0x28]
        je     loc_1_2320
        jmp    loc_1_237F
loc_1_231D:
        lea    ebp, [ebx - 8]
loc_1_2320:
        push   dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [ebx]
        push   dword ptr [esp + 0x44]
        push   ebp
        push   dword ptr [esp + 0x48]
        call   sub_1_3F6F
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 0x20]
        cmp    dword ptr [ebp + 0x28], eax
        jne    loc_1_2367
        sub    dword ptr [edi + 0x10], eax
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_3F46
        add    esp, 4
loc_1_2367:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x18], eax
        sub    dword ptr [esp + 0x40], eax
        je     loc_1_237F
        add    dword ptr [esp + 0x3c], eax
        cmp    dword ptr [esp + 0x14], ebx
        jne    loc_1_2306
loc_1_237F:
        cmp    dword ptr [esp + 0x18], 0
loc_1_2384:
        jne    loc_1_238E
loc_1_2386:
        mov    dword ptr [esp + 0x2c], 0x2733
loc_1_238E:
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_23D3
        push   dword ptr [esp + 0x30]
        call   sub_1_29B3
        add    esp, 4
        push   edi
        push   esi
        call   sub_1_254D
        add    esp, 8
        mov    eax, dword ptr [esi + 0x94]
        test   byte ptr [eax + 0x1f], 0x10
        jne    loc_1_23D3
        mov    eax, dword ptr [edi + 0x3c]
        sub    eax, edi
        cmp    eax, 0x3c
        jne    loc_1_23C5
        and    dword ptr [esi + 0x50], 0xfffffffb
loc_1_23C5:
        mov    eax, dword ptr [edi + 0x34]
        sub    eax, edi
        cmp    eax, 0x34
        jne    loc_1_23D3
        and    dword ptr [esi + 0x50], 0xfffffffe
loc_1_23D3:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_243D
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_23EA
        mov    eax, dword ptr [edi + 0x3c]
        sub    eax, edi
        sub    eax, 0x3c
        jmp    loc_1_23F2
loc_1_23EA:
        mov    eax, dword ptr [edi + 0x34]
        sub    eax, edi
        sub    eax, 0x34
loc_1_23F2:
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        je     loc_1_2413
        cmp    dword ptr [esp + 0x24], 1
        sbb    eax, eax
        and    eax, 0xfffffffd
        add    eax, 4
        jmp    loc_1_2426
loc_1_2413:
        mov    eax, dword ptr [esi + 0x94]
        test   byte ptr [eax + 0x10], 1
        mov    eax, 0x20
        jne    loc_1_2426
        sub    eax, eax
loc_1_2426:
        or     eax, eax
        je     loc_1_243D
        mov    ecx, eax
        not    ecx
        and    dword ptr [esi + 0x50], ecx
        push   0
        push   eax
        push   esi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_243D:
        mov    eax, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x2c]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
sub_1_2453:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [esp + 0x10], 0
        mov    eax, dword ptr [esi + 0x74]
        mov    edi, dword ptr [esi + 0x98]
        mov    dword ptr [esp + 0x14], eax
        mov    ebx, dword ptr [edi + 0x2c]
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, 0x2c
        je     loc_1_2518
        mov    eax, dword ptr [esp + 0x2c]
        or     eax, eax
        je     loc_1_24B3
        mov    ecx, dword ptr [ebx + 0x2c]
        movzx  edx, word ptr [ecx + 4]
        add    edx, 2
        push   edx
        add    ecx, 6
        push   ecx
        push   eax
        call   0
        add    esp, 0xc
        mov    eax, dword ptr [ebx + 0x2c]
        movzx  ecx, word ptr [eax + 4]
        mov    eax, dword ptr [esp + 0x30]
        add    ecx, 2
        mov    dword ptr [eax], ecx
loc_1_24B3:
        mov    ebp, dword ptr [esp + 0x28]
        and    ebp, 2
        push   ebp
        push   dword ptr [esp + 0x28]
        push   ebx
        push   dword ptr [esp + 0x2c]
        call   sub_1_3F6F
        add    esp, 0x10
        mov    eax, dword ptr [ebx + 0x20]
        mov    ecx, dword ptr [esp + 0x34]
        or     ebp, ebp
        mov    dword ptr [ecx], eax
        jne    loc_1_2520
        sub    dword ptr [edi + 0x10], eax
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 0xc]
        lea    eax, [ebx + 8]
        push   ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_3F46
        add    esp, 4
        push   dword ptr [esp + 0x14]
        call   sub_1_29B3
        add    esp, 4
        mov    eax, dword ptr [edi + 0x2c]
        sub    eax, edi
        cmp    eax, 0x2c
        jne    loc_1_2520
        and    dword ptr [esi + 0x50], 0xfffffffe
        jmp    loc_1_2520
loc_1_2518:
        mov    dword ptr [esp + 0x10], 0x2733
loc_1_2520:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_2541
        mov    eax, dword ptr [edi + 0x2c]
        sub    eax, edi
        cmp    eax, 0x2c
        je     loc_1_2541
        and    dword ptr [esi + 0x50], 0xfffffffe
        push   0
        push   1
        push   esi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_2541:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_254D:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x38]
        mov    eax, dword ptr [esi + 0x74]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esi + 0x90]
        test   byte ptr [eax + 0x1b], 0x40
        je     loc_1_2662
        mov    edi, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [esp + 0x18], eax
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_2590
        or     eax, eax
        je     loc_1_2662
loc_1_2590:
        mov    eax, dword ptr [edi + 0x10]
        cmp    dword ptr [edi + 0xc], eax
        jb     loc_1_2662
        sub    ebx, ebx
loc_1_259E:
        or     ebx, ebx
        je     loc_1_25A8
        mov    eax, dword ptr [esp + 0x1c]
        jmp    loc_1_25AC
loc_1_25A8:
        mov    eax, dword ptr [esp + 0x18]
loc_1_25AC:
        mov    dword ptr [esp + 0x10], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_2658
        push   dword ptr [esp + 0x10]
        call   sub_1_3F0F
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_2662
        mov    dword ptr [ebp + 0x18], esi
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        or     dword ptr [ebp + 0x1c], eax
        mov    dword ptr [esp + 0x28], 0x266a ; -> sub_1_266A
        mov    dword ptr [esp + 0x2c], ebp
        mov    eax, dword ptr [ebp + 0x24]
        lea    ecx, [edi + 0x18]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 0x94]
        inc    dword ptr [esi + 0x88]
        or     ebx, ebx
        mov    eax, dword ptr [eax + 0x30]
        mov    dword ptr [esp + 0x24], eax
        jne    loc_1_260E
        lea    ecx, [edi + 0x20]
loc_1_260E:
        lea    eax, [ebp + 0x5c]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x2c]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 0x30]
        mov    eax, dword ptr [eax + 0x10]
        call   dword ptr [eax + 0x28]
        add    esp, 0x10
        or     eax, eax
        je     loc_1_2649
        cmp    eax, 0xff
        je     loc_1_2658
        push   ebp
        call   sub_1_3F46
        add    esp, 4
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    loc_1_2658
loc_1_2649:
        push   dword ptr [esp + 0x10]
        push   0
        push   ebp
        call   sub_1_266A
        add    esp, 0xc
loc_1_2658:
        inc    ebx
        cmp    ebx, 1
        jle    loc_1_259E
loc_1_2662:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
sub_1_266A:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x1c]
        and    eax, 1
        mov    edi, dword ptr [esi + 0x98]
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0xfffffffd
        add    ebx, 4
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_26D5
        or     eax, eax
        je     loc_1_26A7
        mov    dword ptr [edi + 0x1c], 0
        mov    dword ptr [edi + 0x20], 0
        jmp    loc_1_26B5
loc_1_26A7:
        mov    dword ptr [edi + 0x14], 0
        mov    dword ptr [edi + 0x18], 0
loc_1_26B5:
        push   dword ptr [esp + 0x1c]
        push   ebp
        call   sub_1_3FF1
        add    esp, 8
        push   ebp
        push   edi
        call   sub_1_202A
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 8
        add    dword ptr [edi + 0x10], eax
        jmp    loc_1_26E2
loc_1_26D5:
        or     dword ptr [esi + 0x3c], 1
        push   ebp
        call   sub_1_3F46
        add    esp, 4
loc_1_26E2:
        push   dword ptr [esp + 0x18]
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   ebx
        push   esi
        call   sub_1_3BCC
        add    esp, 0xc
        push   esi
        call   sub_1_347B
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2710:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x20]
        mov    esi, dword ptr [edi + 0x74]
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_287B
        mov    ecx, dword ptr [esp + 0x30]
        or     ecx, ecx
        jne    loc_1_27D5
        cmp    dword ptr [edi + 0x94], 0
        je     loc_1_27CB
        mov    ecx, dword ptr [edi + 0xb0]
        mov    dword ptr [ecx], 1
        mov    eax, dword ptr [edi + 0x24]
        push   eax
        lea    edx, [eax - 2]
        mov    word ptr [ecx + 4], dx
        push   dword ptr [edi + 0x18]
        add    ecx, 6
        push   ecx
        call   0
        add    esp, 0xc
loc_1_2767:
        mov    dword ptr [edi + 0xb8], 1
        cmp    dword ptr [edi + 0x4c], 1
        jne    loc_1_289A
        mov    ebx, dword ptr [edi + 0xb4]
        add    ebx, 6
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [esp + 0x10], eax
        lea    eax, [esp + 0x10]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0xc]
        push   dword ptr [edi + 0x7c]
        call   dword ptr [eax + 4]
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_29A9
        push   dword ptr [esp + 0x10]
        push   ebx
        push   edi
        call   sub_1_0360
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_29A9
        mov    dword ptr [edi + 0x54], 0
        jmp    loc_1_289A
loc_1_27CB:
        mov    ebp, 0x2749
        jmp    loc_1_29A9
loc_1_27D5:
        mov    eax, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esp + 0x34]
        cmp    ebx, eax
        jbe    loc_1_27E2
        mov    ebx, eax
loc_1_27E2:
        cmp    dword ptr [esi + 0x14], ebx
        ja     loc_1_2871
        push   ebx
        push   ecx
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_2805
        mov    ebp, 0x271e
        jmp    loc_1_29A9
loc_1_2805:
        mov    eax, dword ptr [edi + 0xb0]
        lea    ecx, [ebx - 2]
        mov    dword ptr [eax], 1
        mov    word ptr [eax + 4], cx
        push   ebx
        push   ebp
        add    eax, 6
        push   eax
        call   0
        add    esp, 0xc
        push   ebx
        push   ebp
        call   sub_1_3B1F
        add    esp, 8
        test   byte ptr [edi + 0x40], 1
        jne    loc_1_2767
        lea    eax, [esp + 0x14]
        push   eax
        push   ebx
        mov    eax, dword ptr [edi + 0xb0]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        call   dword ptr [eax]
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_29A9
        cmp    dword ptr [esp + 0x14], 2
        jne    loc_1_2767
        mov    ebp, 0x271d
        jmp    loc_1_29A9
loc_1_2871:
        mov    ebp, 0x271e
        jmp    loc_1_29A9
loc_1_287B:
        mov    dword ptr [edi + 0xb8], 0
        mov    ebx, dword ptr [esp + 0x28]
        or     ebx, ebx
        jne    loc_1_289E
        mov    ecx, dword ptr [esp + 0x38]
        sub    ebp, ebp
        mov    dword ptr [ecx], ebp
        jmp    loc_1_29A9
loc_1_289A:
        mov    ebx, dword ptr [esp + 0x28]
loc_1_289E:
        push   ebx
        push   dword ptr [esp + 0x28]
        call   sub_1_3AD8
        mov    dword ptr [esp + 0x2c], eax
        add    esp, 8
        or     eax, eax
        jne    loc_1_28BD
        mov    ebp, 0x271e
        jmp    loc_1_29A9
loc_1_28BD:
        mov    eax, dword ptr [edi + 0x98]
        test   byte ptr [edi + 0x41], 1
        jne    loc_1_28D6
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], ecx
        mov    eax, 0
        ja     loc_1_28DF
loc_1_28D6:
        push   ebx
        call   sub_1_3F0F
        add    esp, 4
loc_1_28DF:
        or     eax, eax
        je     loc_1_2910
        push   0
        push   dword ptr [esp + 0x3c]
        push   eax
        push   dword ptr [esp + 0x38]
        push   ebx
        push   dword ptr [esp + 0x38]
        push   edi
        call   sub_1_2A95
        add    esp, 0x1c
        push   ebx
        mov    ebp, eax
        push   dword ptr [esp + 0x28]
        call   sub_1_3B1F
        add    esp, 8
        jmp    loc_1_29A9
loc_1_2910:
        and    dword ptr [edi + 0x50], 0xfffffffd
        test   byte ptr [edi + 0x41], 1
        je     loc_1_2956
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edi + 0xa4], eax
        mov    dword ptr [edi + 0xa8], ebx
        mov    eax, dword ptr [esp + 0x2c]
        lea    ecx, [edi + 0x9c]
        mov    dword ptr [edi + 0xac], eax
        or     dword ptr [edi + 0x8c], 2
        lea    eax, [esi + 0x48]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0x4c]
        mov    dword ptr [edi + 0xa0], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x4c], ecx
        jmp    loc_1_299E
loc_1_2956:
        mov    eax, dword ptr [edi + 0x8c]
        test   al, 4
        jne    loc_1_2991
        or     eax, 4
        push   ebx
        mov    dword ptr [edi + 0x8c], eax
        push   dword ptr [esp + 0x28]
        call   sub_1_3B1F
        add    esp, 8
        lea    ecx, [edi + 0x9c]
        lea    eax, [esi + 0x48]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0x4c]
        mov    dword ptr [edi + 0xa0], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x4c], ecx
        jmp    loc_1_299E
loc_1_2991:
        push   ebx
        push   dword ptr [esp + 0x28]
        call   sub_1_3B1F
        add    esp, 8
loc_1_299E:
        mov    ecx, dword ptr [esp + 0x38]
        mov    ebp, 0x2733
        mov    dword ptr [ecx], ebx
loc_1_29A9:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_29B3:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, esi
        mov    edi, dword ptr [esi + 0x48]
        sub    eax, edi
        cmp    eax, -0x48
        je     loc_1_2A8E
loc_1_29CD:
        lea    ebx, [edi - 0x9c]
        mov    edi, dword ptr [edi]
        test   byte ptr [ebx + 0x8c], 4
        je     loc_1_2A0B
        push   0
        push   2
        push   ebx
        call   sub_1_3BCC
        add    esp, 0xc
        and    dword ptr [ebx + 0x8c], 0xfffffffb
        mov    ecx, dword ptr [ebx + 0xa0]
        mov    eax, dword ptr [ebx + 0x9c]
        add    ebx, 0x9c
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        jmp    loc_1_2A81
loc_1_2A0B:
        mov    eax, dword ptr [ebx + 0x98]
        mov    ecx, dword ptr [eax + 8]
        cmp    dword ptr [eax + 4], ecx
        mov    eax, 0
        jb     loc_1_2A2C
        push   dword ptr [ebx + 0xa8]
        call   sub_1_3F0F
        add    esp, 4
loc_1_2A2C:
        or     eax, eax
        je     loc_1_2A81
        mov    edx, dword ptr [ebx + 0xa0]
        lea    ecx, [ebx + 0x9c]
        push   1
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        lea    ecx, [esp + 0x10]
        and    dword ptr [ebx + 0x8c], 0xfffffffd
        push   ecx
        push   eax
        push   dword ptr [ebx + 0xac]
        push   dword ptr [ebx + 0xa8]
        push   dword ptr [ebx + 0xa4]
        push   ebx
        call   sub_1_2A95
        add    esp, 0x1c
        push   dword ptr [ebx + 0xa8]
        push   dword ptr [ebx + 0xa4]
        call   sub_1_3B1F
        add    esp, 8
loc_1_2A81:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x48
        jne    loc_1_29CD
loc_1_2A8E:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_2A95:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x2c]
        mov    ebx, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x18], ebx
        sub    edi, edi
        push   dword ptr [esp + 0x28]
        call   sub_1_3838
        add    esp, 4
        or     dword ptr [esi + 0x1c], eax
        cmp    dword ptr [ebx + 0xb8], edi
        je     loc_1_2AFE
        mov    edx, dword ptr [esi + 0x2c]
        mov    dword ptr [edx], 1
        mov    ecx, dword ptr [ebx + 0xb0]
        mov    ax, word ptr [ecx + 4]
        add    ecx, 6
        mov    word ptr [edx + 4], ax
        movzx  eax, ax
        add    eax, 2
        push   eax
        push   ecx
        add    edx, 6
        push   edx
        call   0
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [esi + 0x44], eax
        movzx  eax, word ptr [eax + 4]
        add    eax, 8
        mov    dword ptr [esi + 0x40], eax
loc_1_2AFE:
        mov    ecx, dword ptr [ebx + 0x98]
        mov    edx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [ecx + 8]
        add    eax, edx
        mov    dword ptr [ecx + 8], eax
        mov    ebp, dword ptr [esp + 0x34]
        or     ebp, ebp
        jne    loc_1_2B33
        test   byte ptr [ebx + 0x41], 1
        je     loc_1_2B33
        cmp    dword ptr [ecx + 4], eax
        jae    loc_1_2B33
        or     dword ptr [ebx + 0x8c], 8
        and    dword ptr [ebx + 0x50], 0xfffffffd
        mov    edi, 0x2733
loc_1_2B33:
        push   edx
        push   dword ptr [esp + 0x24]
        push   esi
        call   sub_1_3FB1
        lea    eax, [esp + 0x20]
        add    esp, 0xc
        push   eax
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        push   esi
        call   sub_1_2B89
        mov    ecx, dword ptr [esp + 0x40]
        add    esp, 0x10
        mov    dword ptr [ecx], eax
        cmp    eax, -1
        jne    loc_1_2B6F
        push   dword ptr [esp + 0x10]
        call   sub_1_3720
        add    esp, 4
        mov    edi, eax
loc_1_2B6F:
        or     ebp, ebp
        je     loc_1_2B7F
        push   edi
        push   2
        push   ebx
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_2B7F:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_2B89:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x30]
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [esi + 0x74]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esi + 0x98]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0x3c]
        inc    dword ptr [esi + 0x88]
        mov    dword ptr [eax], 0
        mov    dword ptr [esp + 0x20], 0x2cd7 ; -> loc_1_2CD7
        mov    dword ptr [esp + 0x24], edi
        mov    ebx, dword ptr [edi + 0x20]
        mov    eax, dword ptr [0x4230]       ; -> obj1:0x4230
        inc    eax
        mov    dword ptr [0x4230], eax       ; -> obj1:0x4230
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_1_2C07
        mov    eax, dword ptr [esi + 0x90]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [edi + 0x5c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   ebx
        lea    eax, [edi + 0x30]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        mov    eax, dword ptr [eax + 0x10]
        call   dword ptr [eax + 0x30]
        add    esp, 0x14
        jmp    loc_1_2C2E
loc_1_2C07:
        mov    eax, dword ptr [esi + 0x94]
        mov    eax, dword ptr [eax + 0x30]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [edi + 0x5c]
        push   eax
        push   ebx
        lea    eax, [esp + 0x24]
        push   dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 0x10]
        call   dword ptr [eax + 0x2c]
        add    esp, 0x10
loc_1_2C2E:
        mov    ebp, eax
        mov    eax, dword ptr [esp + 0x34]
        cmp    ebp, 0xff
        mov    dword ptr [eax], ebp
        mov    eax, dword ptr [0x4230]       ; -> obj1:0x4230
        je     loc_1_2C44
        dec    eax
loc_1_2C44:
        mov    dword ptr [0x4230], eax       ; -> obj1:0x4230
        or     ebp, ebp
        je     loc_1_2C6A
        cmp    ebp, 1
        je     loc_1_2C75
        cmp    ebp, 0xff
        je     loc_1_2CA3
        mov    ebx, 0xffffffff
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    loc_1_2CA3
loc_1_2C6A:
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    loc_1_2CA3
loc_1_2C75:
        mov    dword ptr [0x4230], eax       ; -> obj1:0x4230
        or     eax, eax
        jne    loc_1_2C85
        mov    ebx, 0xffffffff
        jmp    loc_1_2CA3
loc_1_2C85:
        mov    eax, dword ptr [esp + 0x3c]
        cmp    dword ptr [esp + 0x38], 0
        mov    dword ptr [eax], 1
        je     loc_1_2CA3
        push   edi
        push   dword ptr [esp + 0x18]
        call   sub_1_1FF5
        add    esp, 8
loc_1_2CA3:
        or     ebp, ebp
        je     loc_1_2CAC
        cmp    ebx, -1
        jne    loc_1_2CCD
loc_1_2CAC:
        mov    eax, dword ptr [esi + 0x98]
        mov    ecx, dword ptr [edi + 0x20]
        sub    dword ptr [eax + 8], ecx
        push   edi
        call   sub_1_3F46
        add    esp, 4
        push   dword ptr [esp + 0x10]
        call   sub_1_29B3
        add    esp, 4
loc_1_2CCD:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_2CD7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 0x18]
        mov    ebp, dword ptr [edi + 0x74]
        dec    dword ptr [0x4230]            ; -> obj1:0x4230
        push   ebp
        call   sub_1_2D6F
        add    esp, 4
        mov    ebx, dword ptr [edi + 0x98]
        mov    eax, dword ptr [esi + 0x20]
        sub    dword ptr [ebx + 8], eax
        push   esi
        call   sub_1_3F46
        add    esp, 4
        push   ebp
        call   sub_1_29B3
        mov    esi, dword ptr [esp + 0x1c]
        add    esp, 4
        or     esi, esi
        je     loc_1_2D31
        mov    eax, dword ptr [edi + 0x94]
        or     eax, eax
        je     loc_1_2D31
        push   eax
        call   sub_1_0C90
        add    esp, 4
        or     dword ptr [edi + 0x3c], 1
loc_1_2D31:
        mov    eax, dword ptr [edi + 0x8c]
        test   al, 8
        je     loc_1_2D61
        mov    ecx, dword ptr [ebx + 8]
        cmp    dword ptr [ebx + 4], ecx
        jb     loc_1_2D61
        and    eax, 0xfffffff7
        mov    dword ptr [edi + 0x8c], eax
        push   esi
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_2D61:
        push   edi
        call   sub_1_347B
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2D6F:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        lea    edi, [ebx + 0x3c]
loc_1_2D7D:
        mov    esi, dword ptr [edi]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0x3c
        je     loc_1_2DF1
        mov    eax, dword ptr [esi]
        lea    ecx, [esi + 8]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 0x10]
        mov    ebp, dword ptr [esi + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   0
        push   eax
        push   esi
        call   sub_1_2B89
        add    esp, 0x10
        cmp    eax, -1
        je     loc_1_2DE8
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_2D7D
        mov    eax, dword ptr [esi + 0x18]
        lea    ecx, [esi + 8]
        mov    edx, dword ptr [eax + 0x98]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edi
        add    edx, 0x24
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edi], esi
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0xc], edx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_1_2DF1
loc_1_2DE8:
        push   ebp
        call   sub_1_35BC
        add    esp, 4
loc_1_2DF1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2E00:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        cmp    ebx, 2
        jbe    loc_1_2E17
        mov    eax, 0x2726
        jmp    loc_1_2EC5
loc_1_2E17:
        mov    esi, dword ptr [esp + 0x14]
        push   dword ptr [ebx*4 + 0x3cc0]    ; -> obj1:0x3CC0
        push   esi
        call   sub_1_3250
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_1_2E36
        jmp    loc_1_2EC3
loc_1_2E36:
        cmp    ebx, 1
        je     loc_1_2E40
        cmp    ebx, 2
        jne    loc_1_2E82
loc_1_2E40:
        mov    ecx, dword ptr [esi + 0x94]
        or     ecx, ecx
        je     loc_1_2E82
        mov    edx, dword ptr [ecx + 0x1c]
        mov    eax, dword ptr [ecx + 0x10]
        mov    ebp, eax
        and    ebp, 1
        cmp    ebp, 1
        sbb    ebp, ebp
        and    ebp, 0xfffffff
        add    ebp, 0x10000007
        mov    dword ptr [ecx + 0x1c], ebp
        test   edx, 0x40000000
        je     loc_1_2E82
        or     eax, 4
        mov    dword ptr [ecx + 0x10], eax
        push   0
        push   ecx
        call   sub_1_0CB8
        add    esp, 8
loc_1_2E82:
        or     ebx, ebx
        je     loc_1_2E8B
        cmp    ebx, 2
        jne    loc_1_2EC3
loc_1_2E8B:
        cmp    dword ptr [esi + 0x2c], 1
        jne    loc_1_2EC3
        mov    eax, dword ptr [esi + 0x98]
        or     eax, eax
        je     loc_1_2EC3
        mov    ecx, dword ptr [eax + 0x2c]
        sub    ecx, eax
        cmp    ecx, 0x2c
        je     loc_1_2EC3
        push   dword ptr [esi + 0x94]
        call   sub_1_0C90
        add    esp, 4
        push   0x2745
        push   0x20
        push   esi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_2EC3:
        mov    eax, edi
loc_1_2EC5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_2ED0:
        mov    ecx, dword ptr [esp + 4]
        sub    ecx, 0x50
        cmp    dword ptr [ecx + 0x10], 0
        je     loc_1_2EF6
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_32A1
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_2EF8
loc_1_2EF6:
        sub    eax, eax
loc_1_2EF8:
        ret    
loc_1_2EF9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x90]
        or     eax, eax
        je     loc_1_2F14
        cmp    dword ptr [eax + 0x1c], esi
        jne    loc_1_2F14
        mov    dword ptr [eax + 0x1c], 0
loc_1_2F14:
        cmp    word ptr [esi + 0x10], 0
        je     loc_1_2F22
        cmp    word ptr [esi + 0x12], 0
        je     loc_1_2F36
loc_1_2F22:
        mov    eax, dword ptr [esi + 0x98]
        or     eax, eax
        je     loc_1_2F52
        mov    ecx, dword ptr [eax + 0x2c]
        sub    ecx, eax
        cmp    ecx, 0x2c
        je     loc_1_2F52
loc_1_2F36:
        push   esi
        call   sub_1_35BC
        add    esp, 4
        mov    eax, dword ptr [esi + 0x94]
        or     eax, eax
        je     loc_1_2F52
        push   eax
        call   sub_1_0C90
        add    esp, 4
loc_1_2F52:
        push   esi
        call   sub_1_35D4
        add    esp, 4
        push   esi
        call   sub_1_347B
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2F70:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        mov    ebx, dword ptr [esp + 0x2c]
        mov    edx, ebx
        mov    ebp, dword ptr [esp + 0x30]
        inc    dword ptr [esi + 0x88]
        mov    eax, dword ptr [ebp]
        cmp    eax, 4
        mov    dword ptr [esp + 0x10], eax
        jae    loc_1_2FA3
        lea    edx, [esp + 0x14]
        mov    dword ptr [esp + 0x10], 4
loc_1_2FA3:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x18], eax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x18]
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [esp + 0x18]
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [esi + 0x90]
        or     eax, eax
        mov    ecx, 0
        je     loc_1_2FD3
        mov    ecx, dword ptr [eax + 0x14]
loc_1_2FD3:
        mov    eax, dword ptr [esi + 0x94]
        or     eax, eax
        mov    edi, 0
        je     loc_1_2FE5
        mov    edi, dword ptr [eax + 0x30]
loc_1_2FE5:
        lea    eax, [esp + 0x10]
        push   eax
        push   edx
        mov    eax, dword ptr [esi + 0x74]
        push   dword ptr [esp + 0x30]
        mov    eax, dword ptr [eax + 0xc]
        push   dword ptr [esp + 0x30]
        push   edi
        push   ecx
        push   esi
        push   dword ptr [esi + 0x7c]
        call   dword ptr [eax + 8]
        add    esp, 0x20
        mov    edi, eax
        mov    eax, dword ptr [ebp]
        cmp    eax, 4
        jae    loc_1_3033
        cmp    eax, 2
        jae    loc_1_3024
        mov    al, byte ptr [esp + 0x14]
        mov    byte ptr [ebx], al
        mov    dword ptr [esp + 0x10], 1
        jmp    loc_1_3033
loc_1_3024:
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [ebx], ax
        mov    dword ptr [esp + 0x10], 2
loc_1_3033:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebp], eax
        push   esi
        call   sub_1_347B
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_304D:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        mov    ebx, dword ptr [esp + 0x1c]
        mov    edx, 1
        add    dword ptr [ebx + 0x88], edx
        cmp    dword ptr [esp + 0x2c], 4
        jb     loc_1_307F
        mov    ebp, dword ptr [esp + 0x28]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 0x10], eax
        jmp    loc_1_30A4
loc_1_307F:
        cmp    dword ptr [esp + 0x2c], 2
        mov    ebp, dword ptr [esp + 0x28]
        jb     loc_1_3090
        movzx  eax, word ptr [ebp]
        jmp    loc_1_3094
loc_1_3090:
        movzx  eax, byte ptr [ebp]
loc_1_3094:
        mov    dword ptr [esp + 0x10], eax
        lea    ebp, [esp + 0x10]
        mov    dword ptr [esp + 0x2c], 4
loc_1_30A4:
        cmp    dword ptr [esp + 0x20], 0xffff
        jne    loc_1_31C5
        mov    eax, dword ptr [esp + 0x24]
        cmp    eax, 0x1001
        ja     loc_1_30E5
        je     loc_1_3165
        cmp    eax, 1
        je     loc_1_3114
        cmp    eax, 4
        je     loc_1_3114
        cmp    eax, 0x20
        je     loc_1_3114
        cmp    eax, 0x80
        je     loc_1_3114
        cmp    eax, 0x100
        je     loc_1_3123
        jmp    loc_1_31C5
loc_1_30E5:
        cmp    eax, 0x1002
        je     loc_1_317D
        cmp    eax, 0x1005
        jb     loc_1_31C5
        cmp    eax, 0x1006
        jbe    loc_1_3114
        cmp    eax, 0xff7f
        je     loc_1_3114
        cmp    eax, 0xffffff7f
        jne    loc_1_31C5
loc_1_3114:
        mov    dword ptr [esp + 0x14], 0
        sub    edx, edx
        jmp    loc_1_31C5
loc_1_3123:
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_3133
        mov    dword ptr [esp + 0x14], 0x273a
        jmp    loc_1_3161
loc_1_3133:
        cmp    dword ptr [ebx + 0x94], 0
        je     loc_1_3161
        push   dword ptr [esp + 0x2c]
        push   ebp
        call   sub_1_3224
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   3
        push   dword ptr [ebx + 0x94]
        call   sub_1_0B97
        add    esp, 0xc
loc_1_3161:
        sub    edx, edx
        jmp    loc_1_31C5
loc_1_3165:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x34], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [ebx + 0x98]
        sub    edx, edx
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_31C5
loc_1_317D:
        cmp    dword ptr [esp + 0x10], 0xff00
        jbe    loc_1_31B1
        mov    dword ptr [esp + 0x10], 0xff00
        mov    eax, dword ptr [esp + 0x2c]
        cmp    eax, 4
        jb     loc_1_319D
        mov    eax, 4
loc_1_319D:
        lea    esi, [esp + 0x10]
        mov    edi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_31B1:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x38], eax
        mov    ecx, dword ptr [ebx + 0x98]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], eax
loc_1_31C5:
        or     edx, edx
        je     loc_1_320F
        mov    eax, dword ptr [ebx + 0x90]
        or     eax, eax
        mov    ecx, 0
        je     loc_1_31DB
        mov    ecx, dword ptr [eax + 0x14]
loc_1_31DB:
        mov    eax, dword ptr [ebx + 0x94]
        or     eax, eax
        jne    loc_1_31E9
        sub    eax, eax
        jmp    loc_1_31EC
loc_1_31E9:
        mov    eax, dword ptr [eax + 0x30]
loc_1_31EC:
        push   dword ptr [esp + 0x2c]
        push   ebp
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   eax
        push   ecx
        push   ebx
        mov    eax, dword ptr [ebx + 0x74]
        push   dword ptr [ebx + 0x7c]
        mov    eax, dword ptr [eax + 0xc]
        call   dword ptr [eax + 0x18]
        mov    dword ptr [esp + 0x34], eax
        add    esp, 0x20
loc_1_320F:
        push   ebx
        call   sub_1_347B
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_3224:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_1_322C:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        je     loc_1_323F
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0
        je     loc_1_322C
        sub    eax, eax
        jmp    loc_1_3244
loc_1_323F:
        mov    eax, 1
loc_1_3244:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3250:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        sub    eax, eax
        mov    esi, dword ptr [ecx + 0x7c]
        cmp    esi, eax
        je     loc_1_329D
        mov    edx, dword ptr [esp + 0x14]
        test   dword ptr [ecx + 0x78], edx
        je     loc_1_329D
        mov    edi, dword ptr [ecx + 0x74]
        mov    eax, dword ptr [ecx + 0x90]
        or     eax, eax
        mov    ebx, 0
        je     loc_1_327E
        mov    ebx, dword ptr [eax + 0x14]
loc_1_327E:
        mov    eax, dword ptr [ecx + 0x94]
        or     eax, eax
        jne    loc_1_328C
        sub    eax, eax
        jmp    loc_1_328F
loc_1_328C:
        mov    eax, dword ptr [eax + 0x30]
loc_1_328F:
        push   edx
        push   eax
        push   ebx
        push   ecx
        mov    eax, dword ptr [edi + 0xc]
        push   esi
        call   dword ptr [eax + 0x10]
        add    esp, 0x14
loc_1_329D:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_32A1:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x38]
        mov    eax, dword ptr [esi + 0x20]
        lea    eax, [eax*2 + 0xc4]
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_32CC
        sub    eax, eax
        jmp    loc_1_336C
loc_1_32CC:
        mov    edx, dword ptr [esi + 0x20]
        lea    ebp, [edx*2 + 0xc4]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    dword ptr [ebx + 0x74], esi
        mov    dword ptr [ebx + 0x38], 0x2000
        mov    dword ptr [ebx + 0x34], 0x2000
        lea    eax, [ebx + 0xc4]
        mov    dword ptr [ebx + 0xb0], eax
        lea    eax, [edx + ebx]
        add    eax, 0xc4
        mov    dword ptr [ebx + 0xb4], eax
        lea    ecx, [ebx + 0x80]
        lea    eax, [esi + 0x24]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0x28]
        mov    dword ptr [ebx + 0x84], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x28], ecx
        call   sub_1_3D30
        lea    eax, [ebx + 0x78]
        push   eax
        lea    eax, [ebx + 0x7c]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x50]
        call   dword ptr [eax + 0x14]
        add    esp, 0x18
        or     eax, eax
        je     loc_1_3364
        push   ebx
        call   sub_1_3374
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_336C
loc_1_3364:
        inc    dword ptr [ebx + 0x88]
        mov    eax, ebx
loc_1_336C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
sub_1_3374:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x8c]
        test   al, 2
        je     loc_1_33A1
        and    eax, 0xfffffffd
        mov    dword ptr [edi + 0x8c], eax
        push   dword ptr [edi + 0xa8]
        push   dword ptr [edi + 0xa4]
        call   sub_1_3B1F
        add    esp, 8
loc_1_33A1:
        mov    dword ptr [edi + 0x98], 0
        mov    esi, dword ptr [edi + 0x94]
        or     esi, esi
        je     loc_1_33D3
        push   esi
        call   sub_1_09FC
        add    esp, 4
        or     eax, eax
        jne    loc_1_33C9
        mov    dword ptr [esi + 0x14], 0
loc_1_33C9:
        mov    dword ptr [edi + 0x94], 0
loc_1_33D3:
        mov    esi, dword ptr [edi + 0x90]
        or     esi, esi
        je     loc_1_3400
        push   esi
        call   sub_1_0FBF
        add    esp, 4
        or     eax, eax
        jne    loc_1_33F6
        cmp    dword ptr [esi + 0x1c], edi
        jne    loc_1_33F6
        mov    dword ptr [esi + 0x1c], 0
loc_1_33F6:
        mov    dword ptr [edi + 0x90], 0
loc_1_3400:
        push   0x80
        push   edi
        call   sub_1_3250
        add    esp, 8
        push   edi
        call   sub_1_35EC
        add    esp, 4
        call   sub_1_3F03
        pop    edi
        pop    esi
        ret    
sub_1_341F:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x14]
        mov    edi, edx
        mov    ebx, dword ptr [esp + 0x10]
        mov    esi, ebx
        mov    ecx, 0x1d
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [edx + 0x50], 0
        mov    dword ptr [edx + 0x4c], 6
        mov    dword ptr [edx + 0x54], 0x10
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [ebx + 0x8c]
        mov    dword ptr [edx + 0x8c], eax
        mov    ecx, dword ptr [ebx + 0x90]
        or     ecx, ecx
        mov    dword ptr [edx + 0x90], ecx
        mov    eax, dword ptr [ebx + 0x74]
        mov    dword ptr [edx + 0x74], eax
        je     loc_1_3477
        inc    dword ptr [ecx + 0xc]
loc_1_3477:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_347B:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 0x88]
        dec    edx
        mov    dword ptr [ecx + 0x88], edx
        jne    loc_1_349E
        push   ecx
        call   sub_1_3374
        add    esp, 4
        mov    eax, 1
loc_1_349E:
        ret    
sub_1_349F:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x30]
        mov    esi, dword ptr [esp + 0x2c]
        mov    ebx, dword ptr [edi + 0x34]
        mov    ebp, dword ptr [esi + 0x74]
        push   dword ptr [esi + 0x90]
        push   ebx
        call   sub_1_0A77
        add    esp, 8
        or     eax, eax
        jne    loc_1_35B4
        mov    dword ptr [ebx + 0x14], esi
        mov    ecx, dword ptr [edi + 0x30]
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [ecx], 1
        lea    edx, [eax - 2]
        push   eax
        mov    word ptr [ecx + 4], dx
        push   dword ptr [esp + 0x38]
        add    ecx, 6
        push   ecx
        call   0
        add    esp, 0xc
        inc    dword ptr [esi + 0x88]
        mov    eax, dword ptr [ebx + 0x3c]
        mov    dword ptr [esi + 0x98], eax
        push   esi
        push   eax
        call   sub_1_1FDE
        add    esp, 8
        mov    dword ptr [esi + 0x94], ebx
        mov    dword ptr [esi + 0x4c], 5
        and    dword ptr [esi + 0x50], 0xfffffffd
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [ebx + 0x1c], 0x60000004
        inc    dword ptr [ebx + 0xc]
        mov    dword ptr [esp + 0x14], 0xffffffff
        mov    eax, dword ptr [ebx + 0x30]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [edi + 0x18]
        mov    dword ptr [esp + 0x1c], 0x362e ; -> sub_1_362E
        mov    dword ptr [esp + 0x20], edi
        push   eax
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        call   dword ptr [eax + 0x18]
        add    esp, 0x10
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_35A5
        cmp    ebp, 0xff
        je     loc_1_35B2
        mov    dword ptr [esi + 0x4c], 2
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x1c], eax
        mov    dword ptr [esi + 0x94], 0
        mov    dword ptr [esi + 0x98], 0
        push   ebx
        call   sub_1_09FC
        add    esp, 4
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    loc_1_35B2
loc_1_35A5:
        push   0
        push   0
        push   edi
        call   sub_1_362E
        add    esp, 0xc
loc_1_35B2:
        mov    eax, ebp
loc_1_35B4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_1_35BC:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x98]
        or     eax, eax
        je     loc_1_35D3
        push   eax
        call   sub_1_20C0
        add    esp, 4
loc_1_35D3:
        ret    
sub_1_35D4:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x98]
        or     eax, eax
        je     loc_1_35EB
        push   eax
        call   sub_1_2105
        add    esp, 4
loc_1_35EB:
        ret    
sub_1_35EC:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 0x84]
        lea    eax, [edx + 0x80]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [edx + 0x8c]
        test   al, 2
        jne    loc_1_3611
        test   al, 4
        je     loc_1_3624
loc_1_3611:
        mov    ecx, dword ptr [edx + 0xa0]
        lea    eax, [edx + 0x9c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
loc_1_3624:
        push   edx
        call   sub_1_3BB6
        add    esp, 4
        ret    
sub_1_362E:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [esp + 0x18]
        or     ebp, ebp
        mov    esi, dword ptr [ebx + 0x34]
        mov    edi, dword ptr [esi + 0x14]
        jne    loc_1_36AD
        cmp    dword ptr [esi + 0x1c], 0x60000004
        jne    loc_1_36AD
        mov    dword ptr [edi + 0x4c], 6
        mov    eax, dword ptr [ebx + 0x30]
        movzx  ecx, word ptr [eax + 4]
        add    ecx, 2
        push   ecx
        add    eax, 6
        push   eax
        push   dword ptr [esi + 0x24]
        call   0
        add    esp, 0xc
        mov    eax, dword ptr [ebx + 0x30]
        movzx  eax, word ptr [eax + 4]
        add    eax, 2
        mov    dword ptr [esi + 0x2c], eax
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [esi + 0x28]
        mov    dword ptr [edi + 0x20], eax
        push   esi
        call   sub_1_0AE5
        add    esp, 4
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [edi + 0x24], eax
        or     dword ptr [esi + 0x10], 8
        push   4
        push   edi
        call   sub_1_3250
        add    esp, 8
        jmp    loc_1_36D1
loc_1_36AD:
        mov    dword ptr [edi + 0x4c], 2
        mov    dword ptr [edi + 0x94], 0
        mov    dword ptr [edi + 0x98], 0
        push   esi
        call   sub_1_09FC
        add    esp, 4
loc_1_36D1:
        push   ebx
        call   sub_1_38E3
        add    esp, 4
        push   edi
        call   sub_1_347B
        add    esp, 4
        or     eax, eax
        jne    loc_1_3714
        push   ebp
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   0x10
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
        or     ebp, ebp
        jne    loc_1_3714
        mov    dword ptr [edi + 0x54], 8
        push   0
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0xc
loc_1_3714:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3720:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0xfe
        jg     loc_1_3739
        je     loc_1_3759
        cmp    eax, 0x22
        ja     loc_1_3739
        jmp    dword ptr [eax*4 + 0x37ac]    ; -> obj1:0x37AC
loc_1_3739:
        mov    eax, 0x2726
        jmp    loc_1_37AB
loc_1_3740:
        sub    eax, eax
        jmp    loc_1_37AB
loc_1_3744:
        mov    eax, 0x2747
        jmp    loc_1_37AB
loc_1_374B:
        mov    eax, 0x2740
        jmp    loc_1_37AB
loc_1_3752:
        mov    eax, 0x2741
        jmp    loc_1_37AB
loc_1_3759:
        mov    eax, 0x2726
        jmp    loc_1_37AB
loc_1_3760:
        mov    eax, 0x2738
        jmp    loc_1_37AB
loc_1_3767:
        mov    eax, 0x274d
        jmp    loc_1_37AB
loc_1_376E:
        mov    eax, 0x2745
        jmp    loc_1_37AB
loc_1_3775:
        mov    eax, 0x2746
        jmp    loc_1_37AB
loc_1_377C:
        mov    eax, 0x274c
        jmp    loc_1_37AB
loc_1_3783:
        mov    eax, 0x2775
        jmp    loc_1_37AB
loc_1_378A:
        mov    eax, 0x2743
        jmp    loc_1_37AB
loc_1_3791:
        mov    eax, 0x2751
        jmp    loc_1_37AB
loc_1_3798:
        mov    eax, 0x2757
        jmp    loc_1_37AB
loc_1_379F:
        mov    eax, 0x2714
        jmp    loc_1_37AB
loc_1_37A6:
        mov    eax, 0x271e
loc_1_37AB:
        ret    
        ; ---- dispatch table (35 entries) ----
        dd  loc_1_3740
        dd  loc_1_3744
        dd  loc_1_374B
        dd  loc_1_3752
        dd  loc_1_3744
        dd  loc_1_3739
        dd  loc_1_3752
        dd  loc_1_3759
        dd  loc_1_3739
        dd  loc_1_3760
        dd  loc_1_3759
        dd  loc_1_3759
        dd  loc_1_3739
        dd  loc_1_3739
        dd  loc_1_3767
        dd  loc_1_3759
        dd  loc_1_3759
        dd  loc_1_3759
        dd  loc_1_3759
        dd  loc_1_376E
        dd  loc_1_3775
        dd  loc_1_377C
        dd  loc_1_3783
        dd  loc_1_3739
        dd  loc_1_3739
        dd  loc_1_3759
        dd  loc_1_3759
        dd  loc_1_378A
        dd  loc_1_3791
        dd  loc_1_378A
        dd  loc_1_3798
        dd  loc_1_3759
        dd  loc_1_379F
        dd  loc_1_37A6
        dd  loc_1_379F
sub_1_3838:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 1
        je     loc_1_3848
        mov    eax, 0x20
loc_1_3848:
        test   ch, 0x80
        je     loc_1_3850
        or     eax, 0x40
loc_1_3850:
        ret    
sub_1_3851:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 0x40
        je     loc_1_3861
        mov    eax, 1
loc_1_3861:
        test   cl, 0x10
        je     loc_1_386B
        or     eax, 0x8000
loc_1_386B:
        ret    
        db  0C3h, 000h, 000h, 000h
sub_1_3870:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        push   1
        push   edi
        call   sub_1_0890
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    loc_1_388D
        sub    eax, eax
        jmp    loc_1_38DE
loc_1_388D:
        mov    eax, dword ptr [edi + 0x20]
        add    eax, 0x38
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_38AF
        push   esi
        call   sub_1_0944
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_38DE
loc_1_38AF:
        mov    ebx, dword ptr [edi + 0x20]
        mov    edi, edx
        lea    ebp, [ebx + 0x38]
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    eax, [edx + 0x38]
        mov    dword ptr [edx + 0x30], eax
        mov    dword ptr [edx + 0x10], ebx
        mov    dword ptr [edx + 0x14], eax
        mov    dword ptr [edx + 0x28], ebx
        mov    dword ptr [edx + 0x2c], eax
        mov    eax, edx
        mov    dword ptr [edx + 0x34], esi
loc_1_38DE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_38E3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x34]
        or     eax, eax
        je     loc_1_38FF
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    dword ptr [esi + 0x34], 0
loc_1_38FF:
        push   esi
        call   sub_1_390A
        add    esp, 4
        pop    esi
        ret    
sub_1_390A:
        push   dword ptr [esp + 4]
        call   sub_1_3BB6
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3920:
        sub    esp, 0x10
        mov    eax, dword ptr [esp + 0x20]
        sub    eax, 0x50
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_39CD
        mov    dword ptr [esp + 0xc], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp], 0
        mov    ecx, dword ptr [eax + 8]
        lea    eax, [esp]
        push   eax
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x24]
        push   eax
        add    ecx, 8
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x28]
        push   eax
        push   dword ptr [esp + 0x2c]
        push   dword ptr [ecx - 8]
        push   ecx
        call   sub_1_39D6
        add    esp, 0x24
        or     eax, eax
        jne    loc_1_39C9
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_3998
        mov    eax, 0x273f
        jmp    loc_1_39D2
loc_1_3998:
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_39A6
        mov    eax, 0x273c
        jmp    loc_1_39D2
loc_1_39A6:
        cmp    dword ptr [esp + 4], 0
        jne    loc_1_39B4
        mov    eax, 0x273b
        jmp    loc_1_39D2
loc_1_39B4:
        cmp    dword ptr [esp], 0
        je     loc_1_39C2
        mov    eax, 0x2739
        jmp    loc_1_39D2
loc_1_39C2:
        mov    eax, 0x2726
        jmp    loc_1_39D2
loc_1_39C9:
        sub    eax, eax
        jmp    loc_1_39D2
loc_1_39CD:
        mov    eax, 0x2742
loc_1_39D2:
        add    esp, 0x10
        ret    
sub_1_39D6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     loc_1_3A51
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [esp + 0x2c]
loc_1_39F5:
        cmp    edx, dword ptr [ecx]
        jne    loc_1_3A03
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], 1
loc_1_3A03:
        cmp    esi, dword ptr [ecx + 4]
        jne    loc_1_3A12
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], 1
loc_1_3A12:
        mov    ebp, dword ptr [ecx + 8]
        cmp    ebx, ebp
        jne    loc_1_3A23
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], 1
loc_1_3A23:
        cmp    edx, dword ptr [ecx]
        jne    loc_1_3A47
        cmp    esi, dword ptr [ecx + 4]
        jne    loc_1_3A3A
        cmp    ebx, ebp
        je     loc_1_3A3A
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [eax], 1
loc_1_3A3A:
        cmp    edx, dword ptr [ecx]
        jne    loc_1_3A47
        cmp    esi, dword ptr [ecx + 4]
        jne    loc_1_3A47
        cmp    ebx, ebp
        je     loc_1_3A58
loc_1_3A47:
        add    ecx, 0xc
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    loc_1_39F5
loc_1_3A51:
        sub    eax, eax
loc_1_3A53:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3A58:
        mov    eax, 1
        jmp    loc_1_3A53
        db  000h
AFVXD_Service_00:
        mov    ax, 0
        clc    
        ret    
AFVXD_Control:
        cmp    eax, 1
        je     loc_2_08E4
        cmp    eax, 0x1b
        je     loc_2_08D4
        clc    
        ret    
sub_1_3A7A:
        VxDCall  Dev_0489.Service_8005h          ; cd2005808904
sub_1_3A80:
        push   0
        push   dword ptr [esp + 8]
        VMMCall  VMM._GetDemandPageInfo          ; cd206f000100
        add    esp, 8
        ret    
sub_1_3A90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        VxDCall  Dev_003E.Service_0001h          ; cd2001003e00
        sbb    eax, eax
        inc    eax
        pop    ecx
        pop    ebx
        leave  
        ret    
sub_1_3AAF:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0488.Service_0008h          ; cd2008008804
        add    esp, 4
        leave  
        ret    
AFVXD_Service_01:
        push   ecx
        push   ebx
        push   eax
        call   sub_2_03B0
        add    esp, 0xc
        ret    
AFVXD_Service_02:
        push   eax
        call   sub_2_0721
        add    esp, 4
        ret    
        db  000h, 000h
sub_1_3AD8:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_3B17
        or     ebx, ebx
        je     loc_1_3B17
        movzx  ecx, ax
        and    ch, 0xf
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_3B1B
        add    eax, esi
loc_1_3B17:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_3B1B:
        xor    eax, eax
        jmp    loc_1_3B17
sub_1_3B1F:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_3B5A
        or     ebx, ebx
        je     loc_1_3B5A
        movzx  ecx, ax
        and    ch, 0xf
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_3B63
loc_1_3B5A:
        mov    eax, 1
loc_1_3B5F:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_3B63:
        xor    eax, eax
        jmp    loc_1_3B5F
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_3BB0:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_3BB6:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_3BC3
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
loc_1_3BC3:
        ret    
sub_1_3BC4:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        add    byte ptr [eax], al
sub_1_3BCC:
        VxDCall  Dev_003E.Service_8003h          ; cd2003803e00
sub_1_3BD2:
        VxDCall  Dev_003E.Service_8004h          ; cd2004803e00
loc_1_3BD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3BDC:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3BE0:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah            ; -> sub_1_3920
        cmp    dword ptr [eax], eax
        add    al, dl                        ; -> loc_1_2ED0
        add    byte ptr cs:[eax], al
        stc    
        add    byte ptr cs:[eax], al
        pushal 
        add    eax, dword ptr [eax]
        add    al, dh                        ; -> loc_1_00F0
        add    byte ptr [eax], al
        add    al, al                        ; -> loc_1_05C0
        add    eax, 0x1dd00000               ; -> loc_1_1DD0
        add    byte ptr [eax], al
        inc    eax
        and    dword ptr [eax], eax
        add    byte ptr [eax], dl            ; -> loc_1_2710
        daa    
        add    byte ptr [eax], al
        mov    al, 5
        add    byte ptr [eax], al
        jo     loc_1_3C3D
        add    byte ptr [eax], al
        dec    ebp
        xor    byte ptr [eax], al
        add    byte ptr [eax + 0x1c], al     ; -> loc_1_1C80
        add    byte ptr cs:[eax], al
        rcr    byte ptr [ecx], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    loc_1_3CA0
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_1_3CA4
        jb     loc_1_3C99
        outsb  dx, byte ptr [esi]
        je     0x3c7a
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3cad
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_1_3C3D:
        push   ebx
        je     loc_1_3C9D
        push   ebx
        jb     0x3cbb
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        inc    ecx
        inc    esi
        push   esi
        pop    eax
        inc    esp
        add    byte ptr [eax + eax], bl
        add    byte ptr [ebx + 0x79], dl
        jae    0x3cd0
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x3cd4
        jb     0x3cc9
        outsb  dx, byte ptr [esi]
        je     0x3caa
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3cdd
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x3ccd
        push   ebx
        jb     0x3ceb
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax + edx*2 + 0x61], bl
        jb     0x3cea
        insd   dword ptr es:[edi], dx
        je     0x3cf2
        jb     0x3d02
        pop    esp
        push   edi
        imul   ebp, dword ptr [esi + 0x73], 0x6b636f
loc_1_3C98:
        dec    ebp
loc_1_3C99:
        popal  
        js     0x3cef
        outsd  dx, dword ptr [esi]
loc_1_3C9D:
        arpl   word ptr [ebx + 0x41], bp
loc_1_3CA0:
        jb     0x3cf0
loc_1_3CA4:
        outsb  dx, byte ptr gs:[esi]
        je     0x3d11
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x69], cl
        outsb  dx, byte ptr [esi]
        push   ebx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x41], bp
        jb     0x3d04
        outsb  dx, byte ptr gs:[esi]
        je     0x3d25
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  AFVXD_Service_00         ; fixup
        dd  AFVXD_Service_01         ; fixup
        dd  AFVXD_Service_02         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00495h                      ; DDB_Req_Device_Number
        db  0, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'AFVXD   '                  ; DDB_Name
        dd  05400C000h                  ; DDB_Init_Order
        dd  AFVXD_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x3CCC              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3D30:
        push   ebx
        push   esi
        push   edi
        inc    dword ptr [0x41f8]            ; -> obj1:0x41F8
        mov    eax, dword ptr [0x425c]       ; -> obj1:0x425C
        mov    ecx, dword ptr [0x41f8]       ; -> obj1:0x41F8
        lea    edi, [eax + ecx*4]
        cmp    edi, dword ptr [0x4250]       ; -> obj1:0x4250
        ja     loc_1_3D53
        sub    edi, edi
        jmp    loc_1_3D59
loc_1_3D53:
        sub    edi, dword ptr [0x4250]       ; -> obj1:0x4250
loc_1_3D59:
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     loc_1_3DCF
        mov    ecx, dword ptr [0x4254]       ; -> obj1:0x4254
        mov    esi, 0x41fc                   ; -> obj1:0x41FC
        mov    ebx, 0x4250                   ; -> obj1:0x4250
loc_1_3D70:
        mov    eax, dword ptr [0x4258]       ; -> obj1:0x4258
        cmp    eax, dword ptr [0x41e4]       ; -> obj1:0x41E4
        jbe    loc_1_3DCF
        cmp    ecx, dword ptr [0x4250]       ; -> obj1:0x4250
        jbe    loc_1_3DCF
        call   sub_1_3BC4
        push   0x40
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        mov    ecx, dword ptr [0x4254]       ; -> obj1:0x4254
        je     loc_1_3DCF
        mov    dword ptr [eax + 0x10], esi
        mov    dword ptr [eax + 0x14], ebx
        mov    dword ptr [eax], esi
        mov    edx, dword ptr [0x4200]       ; -> obj1:0x4200
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [0x4200]       ; -> obj1:0x4200
        mov    dword ptr [edx], eax
        mov    dword ptr [0x4200], eax       ; -> obj1:0x4200
        mov    eax, edi
        inc    dword ptr [0x41e4]            ; -> obj1:0x41E4
        inc    dword ptr [0x4250]            ; -> obj1:0x4250
        dec    edi
        or     eax, eax
        jne    loc_1_3D70
loc_1_3DCF:
        mov    eax, dword ptr [0x4244]       ; -> obj1:0x4244
        mov    ecx, dword ptr [0x41f8]       ; -> obj1:0x41F8
        lea    edi, [eax + ecx*2]
        cmp    edi, dword ptr [0x41f0]       ; -> obj1:0x41F0
        ja     loc_1_3DE9
        sub    edi, edi
        jmp    loc_1_3DEF
loc_1_3DE9:
        sub    edi, dword ptr [0x41f0]       ; -> obj1:0x41F0
loc_1_3DEF:
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     loc_1_3E68
        mov    ecx, dword ptr [0x4238]       ; -> obj1:0x4238
        mov    esi, 0x41e8                   ; -> obj1:0x41E8
        mov    ebx, 0x41f0                   ; -> obj1:0x41F0
loc_1_3E06:
        mov    eax, dword ptr [0x424c]       ; -> obj1:0x424C
        cmp    eax, dword ptr [0x4208]       ; -> obj1:0x4208
        jbe    loc_1_3E68
        cmp    ecx, dword ptr [0x41f0]       ; -> obj1:0x41F0
        jbe    loc_1_3E68
        call   sub_1_3BC4
        push   0x5b4
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        mov    ecx, dword ptr [0x4238]       ; -> obj1:0x4238
        je     loc_1_3E68
        mov    dword ptr [eax + 0x10], esi
        mov    dword ptr [eax + 0x14], ebx
        mov    dword ptr [eax], esi
        mov    edx, dword ptr [0x41ec]       ; -> obj1:0x41EC
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [0x41ec]       ; -> obj1:0x41EC
        mov    dword ptr [edx], eax
        mov    dword ptr [0x41ec], eax       ; -> obj1:0x41EC
        mov    eax, edi
        inc    dword ptr [0x4208]            ; -> obj1:0x4208
        inc    dword ptr [0x41f0]            ; -> obj1:0x41F0
        dec    edi
        or     eax, eax
        jne    loc_1_3E06
loc_1_3E68:
        mov    edi, dword ptr [0x41f4]       ; -> obj1:0x41F4
        add    edi, dword ptr [0x41f8]       ; -> obj1:0x41F8
        cmp    edi, dword ptr [0x4204]       ; -> obj1:0x4204
        ja     loc_1_3E80
        sub    edi, edi
        jmp    loc_1_3E86
loc_1_3E80:
        sub    edi, dword ptr [0x4204]       ; -> obj1:0x4204
loc_1_3E86:
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     loc_1_3EFF
        mov    ecx, dword ptr [0x41e0]       ; -> obj1:0x41E0
        mov    esi, 0x423c                   ; -> obj1:0x423C
        mov    ebx, 0x4204                   ; -> obj1:0x4204
loc_1_3E9D:
        mov    eax, dword ptr [0x420c]       ; -> obj1:0x420C
        cmp    eax, dword ptr [0x4234]       ; -> obj1:0x4234
        jbe    loc_1_3EFF
        cmp    ecx, dword ptr [0x4204]       ; -> obj1:0x4204
        jbe    loc_1_3EFF
        call   sub_1_3BC4
        push   0x111c
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        mov    ecx, dword ptr [0x41e0]       ; -> obj1:0x41E0
        je     loc_1_3EFF
        mov    dword ptr [eax + 0x10], esi
        mov    dword ptr [eax + 0x14], ebx
        mov    dword ptr [eax], esi
        mov    edx, dword ptr [0x4240]       ; -> obj1:0x4240
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [0x4240]       ; -> obj1:0x4240
        mov    dword ptr [edx], eax
        mov    dword ptr [0x4240], eax       ; -> obj1:0x4240
        mov    eax, edi
        inc    dword ptr [0x4234]            ; -> obj1:0x4234
        inc    dword ptr [0x4204]            ; -> obj1:0x4204
        dec    edi
        or     eax, eax
        jne    loc_1_3E9D
loc_1_3EFF:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3F03:
        mov    eax, dword ptr [0x41f8]       ; -> obj1:0x41F8
        dec    eax
        mov    dword ptr [0x41f8], eax       ; -> obj1:0x41F8
        ret    
sub_1_3F0F:
        push   dword ptr [esp + 4]
        call   sub_1_406B
        add    esp, 4
        or     eax, eax
        je     loc_1_3F43
        mov    dword ptr [eax + 0x5c], 0
        mov    ecx, dword ptr [eax + 0x24]
        mov    dword ptr [eax + 0x68], ecx
        mov    dword ptr [eax + 0x20], 0
        mov    dword ptr [eax + 0x28], 0
        mov    dword ptr [eax + 0x1c], 0
        jmp    loc_1_3F45
loc_1_3F43:
        sub    eax, eax
loc_1_3F45:
        ret    
sub_1_3F46:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0x10]
        or     edx, edx
        je     loc_1_3F65
        mov    dword ptr [eax], edx
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [eax + 0x14]
        inc    dword ptr [eax]
        jmp    loc_1_3F6E
loc_1_3F65:
        push   eax
        call   sub_1_41CA
        add    esp, 4
loc_1_3F6E:
        ret    
sub_1_3F6F:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [edx + 0x28]
        mov    eax, dword ptr [edx + 0x20]
        sub    eax, ebx
        cmp    eax, ecx
        jb     loc_1_3F88
        mov    eax, ecx
loc_1_3F88:
        or     eax, eax
        je     loc_1_3FAD
        mov    esi, dword ptr [edx + 0x70]
        mov    edi, dword ptr [esp + 0x10]
        add    esi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_3FAD
        add    dword ptr [edx + 0x28], eax
loc_1_3FAD:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3FB1:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [edx + 0x20]
        mov    eax, dword ptr [edx + 0x24]
        sub    eax, ebx
        cmp    eax, ecx
        jb     loc_1_3FCA
        mov    eax, ecx
loc_1_3FCA:
        or     eax, eax
        je     loc_1_3FE7
        mov    edi, dword ptr [edx + 0x70]
        add    edi, ebx
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, eax
loc_1_3FE7:
        mov    dword ptr [edx + 0x20], ebx
        mov    dword ptr [edx + 0x68], ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3FF1:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x20], ecx
        ret    
sub_1_3FFD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x3c24]       ; -> obj1:0x3C24
        mov    esi, dword ptr [esp + 0x14]
        add    eax, esi
        add    eax, 0x74
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     loc_1_4064
        mov    ebx, dword ptr [0x3c24]       ; -> obj1:0x3C24
        mov    edi, edx
        lea    ebp, [ebx + 0x74]
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    eax, [edx + 0x74]
        mov    dword ptr [edx + 0x2c], eax
        lea    eax, [edx + ebx + 0x74]
        mov    dword ptr [edx + 0x70], eax
        mov    dword ptr [edx + 0x24], esi
        mov    dword ptr [edx + 0x6c], 0x4655424e
        mov    dword ptr [edx + 0x5c], 0
        mov    dword ptr [edx + 0x68], esi
        mov    dword ptr [edx + 0x60], eax
        mov    dword ptr [edx + 0x64], 0
loc_1_4064:
        mov    eax, edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_406B:
        push   esi
        sub    eax, eax
        mov    esi, dword ptr [esp + 8]
        cmp    esi, 0x40
        ja     loc_1_40E0
        cmp    dword ptr [0x41fc], 0x41fc    ; -> obj1:0x41FC
        je     loc_1_40A4
        mov    eax, dword ptr [0x41fc]       ; -> obj1:0x41FC
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], 0x41fc   ; -> obj1:0x41FC
        mov    dword ptr [0x41fc], ecx       ; -> obj1:0x41FC
        dec    dword ptr [0x4250]            ; -> obj1:0x4250
        jmp    loc_1_41C8
loc_1_40A4:
        mov    edx, dword ptr [0x41e4]       ; -> obj1:0x41E4
        cmp    dword ptr [0x4258], edx       ; -> obj1:0x4258
        jbe    loc_1_40E0
        push   0x40
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        je     loc_1_40E0
        mov    ecx, dword ptr [0x41e4]       ; -> obj1:0x41E4
        inc    ecx
        mov    dword ptr [0x41e4], ecx       ; -> obj1:0x41E4
        mov    dword ptr [eax + 0x10], 0x41fc ; -> obj1:0x41FC
        mov    dword ptr [eax + 0x14], 0x4250 ; -> obj1:0x4250
        jmp    loc_1_41C8
loc_1_40E0:
        cmp    esi, 0x5b4
        ja     loc_1_4151
        cmp    dword ptr [0x41e8], 0x41e8    ; -> obj1:0x41E8
        je     loc_1_4115
        mov    eax, dword ptr [0x41e8]       ; -> obj1:0x41E8
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], 0x41e8   ; -> obj1:0x41E8
        mov    dword ptr [0x41e8], ecx       ; -> obj1:0x41E8
        dec    dword ptr [0x41f0]            ; -> obj1:0x41F0
        jmp    loc_1_41C8
loc_1_4115:
        mov    edx, dword ptr [0x4208]       ; -> obj1:0x4208
        cmp    dword ptr [0x424c], edx       ; -> obj1:0x424C
        jbe    loc_1_4151
        push   0x5b4
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        je     loc_1_4151
        mov    ecx, dword ptr [0x4208]       ; -> obj1:0x4208
        inc    ecx
        mov    dword ptr [0x4208], ecx       ; -> obj1:0x4208
        mov    dword ptr [eax + 0x10], 0x41e8 ; -> obj1:0x41E8
        mov    dword ptr [eax + 0x14], 0x41f0 ; -> obj1:0x41F0
        jmp    loc_1_41C8
loc_1_4151:
        cmp    esi, 0x111c
        ja     loc_1_41BF
        cmp    dword ptr [0x423c], 0x423c    ; -> obj1:0x423C
        je     loc_1_4183
        mov    eax, dword ptr [0x423c]       ; -> obj1:0x423C
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], 0x423c   ; -> obj1:0x423C
        mov    dword ptr [0x423c], ecx       ; -> obj1:0x423C
        dec    dword ptr [0x4204]            ; -> obj1:0x4204
        jmp    loc_1_41C8
loc_1_4183:
        mov    edx, dword ptr [0x4234]       ; -> obj1:0x4234
        cmp    dword ptr [0x420c], edx       ; -> obj1:0x420C
        jbe    loc_1_41C8
        push   0x111c
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        je     loc_1_41C8
        mov    ecx, dword ptr [0x4234]       ; -> obj1:0x4234
        inc    ecx
        mov    dword ptr [0x4234], ecx       ; -> obj1:0x4234
        mov    dword ptr [eax + 0x10], 0x423c ; -> obj1:0x423C
        mov    dword ptr [eax + 0x14], 0x4204 ; -> obj1:0x4204
        jmp    loc_1_41C8
loc_1_41BF:
        push   esi
        call   sub_1_3FFD
        add    esp, 4
loc_1_41C8:
        pop    esi
        ret    
sub_1_41CA:
        push   dword ptr [esp + 4]
        call   sub_1_3BB6
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x928  R-X
        db  0C7h, 005h
        dd  obj1:0x4200              ; fixup
        dd  obj1:0x41FC              ; fixup
        db  0C7h, 005h
        dd  obj1:0x41FC              ; fixup
        dd  obj1:0x41FC              ; fixup
        db  0C7h, 005h
        dd  obj1:0x41EC              ; fixup
        dd  obj1:0x41E8              ; fixup
        db  0C7h, 005h
        dd  obj1:0x41E8              ; fixup
        dd  obj1:0x41E8              ; fixup
        db  0C7h, 005h
        dd  obj1:0x4240              ; fixup
        dd  obj1:0x423C              ; fixup
        db  0C7h, 005h
        dd  obj1:0x423C              ; fixup
        dd  obj1:0x423C              ; fixup
        db  0A1h
        dd  obj1:0x4248              ; fixup
        db  083h, 0F8h, 001h, 074h, 015h, 083h, 0F8h, 002h, 074h, 06Fh, 083h, 0F8h, 003h, 00Fh, 084h, 0C2h
        db  000h, 000h, 000h, 02Bh, 0C0h, 0E9h, 01Ah, 001h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4258              ; fixup
        db  050h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x424C              ; fixup
        db  028h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x420C              ; fixup
        db  014h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4254              ; fixup
        db  018h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4238              ; fixup
        db  010h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x41E0              ; fixup
        db  008h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x425C              ; fixup
        db  008h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4244              ; fixup
        db  004h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x41F4              ; fixup
        db  002h, 000h, 000h, 000h, 0E9h, 0B6h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4258              ; fixup
        db  040h, 001h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x424C              ; fixup
        db  0A0h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x420C              ; fixup
        db  050h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4254              ; fixup
        db  060h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4238              ; fixup
        db  040h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x41E0              ; fixup
        db  020h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x425C              ; fixup
        db  020h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4244              ; fixup
        db  010h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x41F4              ; fixup
        db  008h, 000h, 000h, 000h, 0EBh, 05Ah, 0C7h, 005h
        dd  obj1:0x4258              ; fixup
        db  080h, 002h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x424C              ; fixup
        db  040h, 001h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x420C              ; fixup
        db  0A0h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4254              ; fixup
        db  0C0h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4238              ; fixup
        db  080h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x41E0              ; fixup
        db  040h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x425C              ; fixup
        db  040h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x4244              ; fixup
        db  020h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x41F4              ; fixup
        db  010h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 0C3h, 000h, 000h
sub_2_0178:
        VxDCall  Dev_0488.Service_0000h          ; cd2000008804
        jb     loc_2_0194
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_0194
        VxDCall  Dev_0488.Service_000Ah          ; cd200a008804
        mov    dword ptr [0x3bd8], eax       ; -> loc_1_3BD8
        clc    
loc_2_0194:
        cmc    
        sbb    eax, eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_01A0:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [edx + ecx - 1]
        cmp    eax, edx
        je     loc_2_01C7
        mov    cl, byte ptr [esp + 0xc]
loc_2_01BE:
        cmp    byte ptr [eax], cl
        je     loc_2_01C9
        dec    eax
        cmp    eax, edx
        jne    loc_2_01BE
loc_2_01C7:
        sub    eax, eax
loc_2_01C9:
        pop    edi
        ret    
sub_2_01CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x350
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x34]
        mov    dword ptr [ebp - 0x44], eax
        mov    dword ptr [ebp - 0x40], 0x3c28 ; -> obj1:0x3C28
        mov    dword ptr [ebp - 0x3c], 0x80000002
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        je     loc_2_020E
        sub    eax, eax
        jmp    loc_2_03A9
loc_2_020E:
        call   sub_2_0888
        or     eax, eax
        jne    loc_2_021E
        sub    eax, eax
        jmp    loc_2_03A9
loc_2_021E:
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 0x350]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   0x5c
        lea    eax, [ebp - 0x350]
        push   eax
        call   sub_2_01A0
        add    esp, 8
        mov    byte ptr [eax], 0
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0xc], 0x104
        mov    dword ptr [ebp - 8], 0x104
loc_2_026C:
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x30], eax
        lea    eax, [ebp - 0x24c]
        mov    dword ptr [ebp - 0x2c], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x24], 0
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x20], eax
        lea    eax, [ebp - 0x148]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0x14], eax
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegEnumValue               ; cd2050010100
        add    esp, 0x20
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_0364
        cmp    dword ptr [ebp - 0x10], 1
        jne    loc_2_036E
        lea    edi, [ebp - 0x350]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 0x148]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x3c54                   ; -> obj1:0x3C54
        lea    edi, [ebp - 0x148]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        movsw  word ptr es:[edi], word ptr [esi]
        lea    edi, [ebp - 0x24c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [ebp - 0x148]
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
        lea    eax, [ebp - 0x148]
        push   eax
        call   sub_2_0899
        add    esp, 4
        jmp    loc_2_036E
loc_2_0364:
        cmp    ebx, 0xea
        jne    loc_2_036E
        sub    ebx, ebx
loc_2_036E:
        mov    dword ptr [ebp - 0xc], 0x104
        mov    dword ptr [ebp - 8], 0x104
        inc    dword ptr [ebp - 4]
        or     ebx, ebx
        je     loc_2_026C
        mov    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0x38], eax
        push   dword ptr [ebp - 0x38]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        cmp    dword ptr [0x4260], 1         ; -> obj1:0x4260
        sbb    eax, eax
        inc    eax
loc_2_03A9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_03B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x150
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x4260]       ; -> obj1:0x4260
        mov    esi, 0x4210                   ; -> obj1:0x4210
        lea    eax, [ebx - 1]
        or     ebx, ebx
        je     loc_2_040C
loc_2_03CE:
        mov    ecx, dword ptr [esi]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
loc_2_03D5:
        mov    dl, byte ptr [ecx]
        cmp    dl, byte ptr [edi]
        jne    loc_2_03F5
        or     dl, dl
        je     loc_2_03F1
        mov    dl, byte ptr [ecx + 1]
        cmp    dl, byte ptr [edi + 1]
        jne    loc_2_03F5
        add    ecx, 2
        add    edi, 2
        or     dl, dl
        jne    loc_2_03D5
loc_2_03F1:
        sub    ecx, ecx
        jmp    loc_2_03FA
loc_2_03F5:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_2_03FA:
        or     ecx, ecx
        je     loc_2_0718
        add    esi, 4
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        jne    loc_2_03CE
loc_2_040C:
        cmp    ebx, 8
        jb     loc_2_0416
        jmp    loc_2_0718
loc_2_0416:
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [ebp + 8]
        lea    edx, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edx, [edx + ecx - 1]
        add    edx, 0x52
        push   edx
        call   sub_1_3BB0
        add    esp, 4
        mov    ebx, eax
        mov    dword ptr [esi], ebx
        or     ebx, ebx
        je     loc_2_0718
        lea    edx, [ebx + 0x50]
        mov    dword ptr [ebx], edx
        mov    edi, dword ptr [ebp + 8]
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
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [ebx]
        lea    eax, [ecx + eax - 1]
        inc    eax
        mov    dword ptr [ebx + 4], eax
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebx + 4]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 0xc], esi
        push   dword ptr [ebp + 0xc]
        call   sub_1_3AAF
        add    esp, 4
        mov    dword ptr [ebx + 0x10], eax
        push   0
        push   0
        call   dword ptr [esi + 0xc]
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_3BB0
        add    esp, 4
        mov    dword ptr [ebx + 8], eax
        or     eax, eax
        je     loc_2_070F
        push   edi
        push   eax
        call   dword ptr [esi + 0xc]
        add    esp, 8
        cmp    eax, edi
        jne    loc_2_06F9
        mov    eax, dword ptr [ebx + 8]
        cmp    dword ptr [eax + 4], 3
        jne    loc_2_06F9
        mov    esi, 0x3c58                   ; -> obj1:0x3C58
        lea    edi, [ebp - 0x150]
        mov    ecx, 9
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0x3c80                   ; -> obj1:0x3C80
        lea    edi, [ebp - 0x150]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        movsw  word ptr es:[edi], word ptr [esi]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [ebp - 0x150]
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
        mov    esi, 0x3c84                   ; -> obj1:0x3C84
        lea    edi, [ebp - 0x150]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x4c], eax
        lea    eax, [ebp - 0x150]
        mov    dword ptr [ebp - 0x48], eax
        mov    dword ptr [ebp - 0x44], 0x80000002
        push   dword ptr [ebp - 0x4c]
        push   dword ptr [ebp - 0x48]
        push   dword ptr [ebp - 0x44]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_06F9
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x40], eax
        lea    edi, [ebx + 0x18]
        mov    dword ptr [ebp - 0x3c], edi
        lea    eax, [ebp - 0x28]
        mov    dword ptr [ebp - 0x38], eax
        mov    dword ptr [ebp - 0x34], 0
        mov    dword ptr [ebp - 0x30], 0x3c98 ; -> loc_1_3C98
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x2c], eax
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        push   dword ptr [ebp - 0x38]
        push   dword ptr [ebp - 0x34]
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x2c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_06E1
        cmp    dword ptr [ebp - 4], 4
        jne    loc_2_06E1
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x24], eax
        lea    esi, [ebx + 0x14]
        mov    dword ptr [ebp - 0x20], esi
        lea    eax, [ebp - 0x28]
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x14], 0x3cac ; -> obj1:0x3CAC
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_066B
        cmp    dword ptr [ebp - 4], 4
        je     loc_2_066F
loc_2_066B:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
loc_2_066F:
        mov    ecx, dword ptr [edi]
        cmp    dword ptr [0x3c20], ecx       ; -> obj1:0x3C20
        jae    loc_2_0687
        mov    dword ptr [0x3c20], ecx       ; -> obj1:0x3C20
        lea    eax, [ecx + 6]
        mov    dword ptr [0x3c24], eax       ; -> obj1:0x3C24
loc_2_0687:
        mov    eax, dword ptr [esi]
        add    eax, 6
        mov    dword ptr [ebx + 0x1c], eax
        add    ecx, 6
        mov    dword ptr [ebx + 0x20], ecx
        push   0x3be4                        ; -> obj1:0x3BE4
        push   dword ptr [ebx]
        call   sub_1_3A90
        add    esp, 8
        cmp    eax, 1
        jne    loc_2_06F9
        mov    eax, dword ptr [0x4260]       ; -> obj1:0x4260
        inc    eax
        mov    dword ptr [0x4260], eax       ; -> obj1:0x4260
        lea    eax, [ebx + 0x24]
        mov    dword ptr [ebx + 0x28], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x2c]
        mov    dword ptr [ebx + 0x30], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x34]
        mov    dword ptr [ebx + 0x38], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x3c]
        mov    dword ptr [ebx + 0x40], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x48]
        mov    dword ptr [ebx + 0x4c], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x10]
        jmp    loc_2_071A
loc_2_06E1:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_06F9:
        push   dword ptr [ebx + 8]
        call   sub_1_3BB6
        add    esp, 4
        push   ebx
        call   sub_1_3BB6
        add    esp, 4
        jmp    loc_2_0718
loc_2_070F:
        push   ebx
        call   sub_1_3BB6
        add    esp, 4
loc_2_0718:
        sub    eax, eax
loc_2_071A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0721:
        push   ebx
        push   esi
        push   edi
        mov    edi, 0x4210                   ; -> obj1:0x4210
        mov    ecx, dword ptr [0x4260]       ; -> obj1:0x4260
        or     ecx, ecx
        je     loc_2_0771
        mov    edx, dword ptr [esp + 0x10]
loc_2_0737:
        mov    eax, dword ptr [edi]
        mov    esi, edx
        mov    eax, dword ptr [eax]
loc_2_073D:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [esi]
        jne    loc_2_075D
        or     bl, bl
        je     loc_2_0759
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    loc_2_075D
        add    eax, 2
        add    esi, 2
        or     bl, bl
        jne    loc_2_073D
loc_2_0759:
        sub    eax, eax
        jmp    loc_2_0762
loc_2_075D:
        sbb    eax, eax
        sbb    eax, -1
loc_2_0762:
        or     eax, eax
        je     loc_2_076D
        add    edi, 4
        or     ecx, ecx
        jne    loc_2_0737
loc_2_076D:
        or     ecx, ecx
        jne    loc_2_0775
loc_2_0771:
        sub    eax, eax
        jmp    loc_2_079E
loc_2_0775:
        push   dword ptr [edi]
        call   sub_1_3BB6
        add    esp, 4
        mov    eax, dword ptr [0x4260]       ; -> obj1:0x4260
        dec    eax
        mov    dword ptr [0x4260], eax       ; -> obj1:0x4260
        lea    ecx, [eax*4]
        lea    esi, [edi + 4]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, 1
loc_2_079E:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_07B0:
        sub    esp, 0x40
        lea    eax, [esp]
        push   eax
        call   sub_1_3A80
        add    esp, 4
        cmp    dword ptr [esp + 4], 0x400
        ja     loc_2_07D6
        mov    dword ptr [0x4248], 1         ; -> obj1:0x4248
        jmp    loc_2_07F4
loc_2_07D6:
        cmp    dword ptr [esp + 4], 0x1000
        mov    dword ptr [0x4248], 2         ; -> obj1:0x4248
        jbe    loc_2_07F4
        mov    dword ptr [0x4248], 3         ; -> obj1:0x4248
loc_2_07F4:
        call   sub_2_0178
        or     eax, eax
        jne    loc_2_0804
        sub    eax, eax
        jmp    loc_2_0884
loc_2_0804:
        push   0x1ebd                        ; -> loc_1_1EBD
        call   sub_1_0E76
        add    esp, 4
        or     eax, eax
        je     loc_2_0819
        sub    eax, eax
        jmp    loc_2_0884
loc_2_0819:
        push   0x1f2e                        ; -> loc_1_1F2E
        call   sub_1_0E6B
        add    esp, 4
        or     eax, eax
        je     loc_2_0838
        push   0
        call   sub_1_0E76
        add    esp, 4
        sub    eax, eax
        jmp    loc_2_0884
loc_2_0838:
        call   0
        or     eax, eax
        jne    loc_2_0859
        push   0
        call   sub_1_0E6B
        add    esp, 4
        push   0
        call   sub_1_0E76
        add    esp, 4
        sub    eax, eax
        jmp    loc_2_0884
loc_2_0859:
        call   sub_2_01CB
        or     eax, eax
        jne    loc_2_087A
        push   0
        call   sub_1_0E6B
        add    esp, 4
        push   0
        call   sub_1_0E76
        add    esp, 4
        sub    eax, eax
        jmp    loc_2_0884
loc_2_087A:
        call   sub_1_3BC4
        mov    eax, 1
loc_2_0884:
        add    esp, 0x40
        ret    
sub_2_0888:
        push   ebp
        push   esi
        push   edi
        push   ebx
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        mov    eax, edx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_0899:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        sbb    eax, eax
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_08B4:
        mov    eax, 0x488
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        jecxz  loc_2_08C3
        clc    
        ret    
loc_2_08C3:
        mov    edx, 0x91c                    ; -> loc_2_091C
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        ret    
loc_2_08D4:
        cmp    byte ptr [0x3d28], 0          ; -> obj1:0x3D28
        jne    loc_2_091A
        mov    byte ptr [0x3d28], 1          ; -> obj1:0x3D28
loc_2_08E4:
        mov    edi, 0x4264                   ; -> obj1:0x4264
        mov    ecx, 0x4264                   ; -> obj1:0x4264
        sub    ecx, edi
        shr    ecx, 2
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        call   sub_2_08B4
        jb     loc_2_0919
        VxDCall  Dev_0488.Service_0000h          ; cd2000008804
        jb     loc_2_0919
        VxDCall  Dev_003E.Service_0000h          ; cd2000003e00
        jb     loc_2_0919
        call   sub_2_07B0
        cmp    eax, 1
        jmp    loc_2_091A
loc_2_0919:
        stc    
loc_2_091A:
        ret    
        db  000h
loc_2_091C:
        push   esi
        push   esp
        inc    esp
        dec    ecx
        xor    edi, dword ptr cs:[eax]
        and    al, byte ptr ss:[ecx + 0x46]
        push   esi
VxD__LTEXT ENDS
