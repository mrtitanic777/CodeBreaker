; afvxd_lcode_01.asm -- AFVXD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  056h, 057h, 08Bh, 074h, 024h, 010h, 066h, 083h, 03Eh, 002h, 075h, 021h, 08Bh, 07Ch, 024h, 00Ch
        db  066h, 08Bh, 006h, 066h, 089h, 007h, 083h, 0C7h, 008h, 066h, 08Bh, 046h, 002h, 066h, 089h, 047h
        db  0FAh, 08Bh, 046h, 004h, 089h, 047h, 0FCh, 02Bh, 0C0h, 0ABh, 0ABh, 0EBh, 016h, 08Bh, 044h, 024h
        db  014h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 0C8h, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h
        db  003h, 0F3h, 0A4h, 05Fh, 05Eh, 0C3h
sub_1_0046:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    word ptr [esi], 2
        jne    SHORT loc_1_0076
        mov    edx, dword ptr [esp + 10h]
        mov    ax, word ptr [esi]
        cmp    word ptr [edx], ax
        jne    SHORT loc_1_0072
        mov    ax, word ptr [esi + 2]
        cmp    word ptr [edx + 2], ax
        jne    SHORT loc_1_0072
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [edx + 4], eax
        jne    SHORT loc_1_0072
        jmp    SHORT loc_1_009E
loc_1_0072:
        sub    eax, eax
        jmp    SHORT loc_1_00A3
loc_1_0076:
        mov    edi, dword ptr [esp + 14h]
        sub    edi, 2
        add    esi, 2
        mov    edx, dword ptr [esp + 10h]
        add    edx, 2
loc_1_0087:
        mov    eax, edi
        dec    edi
        or     eax, eax
        jle    SHORT loc_1_009E
        mov    ecx, esi
        inc    esi
        mov    eax, edx
        inc    edx
        mov    al, byte ptr [eax]
        cmp    byte ptr [ecx], al
        je     SHORT loc_1_0087
        sub    eax, eax
        jmp    SHORT loc_1_00A3
loc_1_009E:
        mov    eax, 1
loc_1_00A3:
        pop    edi
        pop    esi
        ret
sub_1_00A6:
        mov    edx, dword ptr [esp + 4]
        cmp    word ptr [edx], 2
        jne    SHORT loc_1_00C3
        cmp    word ptr [edx + 2], 0
        jne    SHORT loc_1_00BF
        cmp    dword ptr [edx + 4], 0
        jne    SHORT loc_1_00BF
        jmp    SHORT loc_1_00E0
loc_1_00BF:
        sub    eax, eax
        jmp    SHORT loc_1_00E5
loc_1_00C3:
        mov    ecx, dword ptr [esp + 8]
        sub    ecx, 2
        add    edx, 2
loc_1_00CD:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        jle    SHORT loc_1_00E0
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_00CD
        sub    eax, eax
        jmp    SHORT loc_1_00E5
loc_1_00E0:
        mov    eax, 1
loc_1_00E5:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00F0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    eax, dword ptr [esi + 90h]
        mov    dword ptr [esp + 10h], eax
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [eax + 40h]
        sub    eax, dword ptr [esp + 10h]
        cmp    eax, 40h
        je     NEAR PTR loc_1_0278
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ecx
        call   sub_1_32A1
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0155
        mov    esi, 2747h
        jmp    NEAR PTR loc_1_027D
loc_1_0155:
        push   dword ptr [esp + 10h]
        call   sub_1_1308
        add    esp, 4
        mov    ebp, eax
        mov    eax, dword ptr [esp + 10h]
        dec    dword ptr [esi + 48h]
        inc    dword ptr [eax + 2ch]
        push   edi
        push   esi
        call   sub_1_341F
        mov    eax, dword ptr [esp + 34h]
        add    esp, 8
        mov    dword ptr [edi + 68h], eax
        mov    ecx, dword ptr [ebp + 10h]
        test   cl, 10h
        je     SHORT loc_1_0192
        dec    dword ptr [esi + 88h]
        inc    dword ptr [edi + 88h]
loc_1_0192:
        mov    ebx, dword ptr [ebp + 3ch]
        mov    dword ptr [edi + 94h], ebp
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [ebp + 24h]
        test   cl, 20h
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 98h], ebx
        mov    dword ptr [ebx], edi
        mov    dword ptr [ebp + 14h], edi
        je     SHORT loc_1_01C7
        or     dword ptr [edi + 3ch], 1
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
        cmp    dword ptr [ebp + 1ch], 60000002h
        jne    SHORT loc_1_01F5
        mov    dword ptr [ebp + 1ch], 60000003h
loc_1_01F5:
        push   0
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
        and    dword ptr [esi + 50h], 0fffffff7h
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 40h]
        sub    eax, dword ptr [esp + 10h]
        cmp    eax, 40h
        je     SHORT loc_1_0223
        push   0
        push   8
        push   esi
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_0223:
        mov    eax, dword ptr [ebx + 2ch]
        sub    eax, ebx
        cmp    eax, 2ch
        je     SHORT loc_1_0254
        mov    eax, dword ptr [ebx + 34h]
        sub    eax, ebx
        cmp    eax, 34h
        je     SHORT loc_1_0244
        push   0
        push   1
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_0244:
        mov    eax, dword ptr [ebx + 3ch]
        sub    eax, ebx
        cmp    eax, 3ch
        je     SHORT loc_1_026E
        push   0
        push   4
        jmp    SHORT loc_1_0265
loc_1_0254:
        test   byte ptr [ebp + 1fh], 10h
        je     SHORT loc_1_026E
        mov    dword ptr [edi + 4ch], 7
        push   0
        push   20h
loc_1_0265:
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_026E:
        mov    eax, dword ptr [esp + 28h]
        sub    esi, esi
        mov    dword ptr [eax], edi
        jmp    SHORT loc_1_027D
loc_1_0278:
        mov    esi, 2733h
loc_1_027D:
        push   dword ptr [esp + 10h]
        call   sub_1_127F
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_0293:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        lea    eax, [esp + 8]
        push   eax
        push   0
        push   1
        push   0fffeh
        push   0
        push   0
        push   0
        mov    eax, dword ptr [edi + 74h]
        push   dword ptr [edi + 7ch]
        mov    eax, dword ptr [eax + 0ch]
        call   dword ptr [eax + 8]
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_034D
        push   dword ptr [esp + 8]
        call   sub_1_3BB0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_02E0
        mov    eax, 2747h
        jmp    SHORT loc_1_034D
loc_1_02E0:
        lea    eax, [esp + 8]
        push   eax
        push   esi
        push   1
        push   0fffeh
        push   0
        push   0
        push   0
        mov    eax, dword ptr [edi + 74h]
        push   dword ptr [edi + 7ch]
        mov    eax, dword ptr [eax + 0ch]
        call   dword ptr [eax + 8]
        add    esp, 20h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_0315
        push   esi
        call   sub_1_3BB6
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_034D
loc_1_0315:
        push   dword ptr [esp + 8]
        push   esi
        push   1
        push   0fffeh
        push   0
        push   0
        mov    eax, dword ptr [esp + 28h]
        push   0
        push   dword ptr [eax + 7ch]
        mov    eax, dword ptr [eax + 74h]
        mov    eax, dword ptr [eax + 0ch]
        call   dword ptr [eax + 18h]
        add    esp, 20h
        mov    edi, eax
        push   esi
        call   sub_1_3BB6
        add    esp, 4
        or     edi, edi
        mov    eax, edi
        jne    SHORT loc_1_034D
        sub    eax, eax
loc_1_034D:
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0360:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 34h]
        mov    dword ptr [esp + 10h], 0
        mov    ebx, dword ptr [edi + 74h]
        mov    eax, dword ptr [edi + 0b4h]
        mov    dword ptr [esp + 18h], eax
        add    eax, 6
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [ebx + 18h]
        mov    esi, dword ptr [esp + 3ch]
        cmp    esi, eax
        jbe    SHORT loc_1_0394
        mov    esi, eax
loc_1_0394:
        cmp    dword ptr [ebx + 14h], esi
        jbe    SHORT loc_1_03A3
        mov    ebp, 271eh
        jmp    NEAR PTR loc_1_0598
loc_1_03A3:
        push   esi
        push   dword ptr [esp + 3ch]
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_03C0
        mov    ebp, 271eh
        jmp    NEAR PTR loc_1_0598
loc_1_03C0:
        mov    eax, dword ptr [esp + 18h]
        lea    ecx, [esi - 2]
        mov    dword ptr [eax], 1
        push   esi
        push   ebp
        mov    word ptr [eax + 4], cx
        push   dword ptr [esp + 1ch]
        call   0
        add    esp, 0ch
        push   esi
        push   ebp
        call   sub_1_3B1F
        add    esp, 8
        mov    ecx, dword ptr [edi + 28h]
        mov    eax, dword ptr [esp + 14h]
        movzx  eax, word ptr [eax]
        cmp    eax, ecx
        je     SHORT loc_1_0401
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_059A
loc_1_0401:
        inc    dword ptr [edi + 88h]
        mov    dword ptr [esp + 20h], ecx
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [edi + 30h]
        mov    dword ptr [esp + 24h], eax
        test   byte ptr [edi + 40h], 20h
        jne    SHORT loc_1_0492
        lea    eax, [esp + 28h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0ch]
        push   dword ptr [esp + 1ch]
        call   dword ptr [eax]
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_0445
        push   edi
        call   sub_1_347B
        add    esp, 4
        jmp    NEAR PTR loc_1_0598
loc_1_0445:
        cmp    dword ptr [esp + 28h], 1
        jne    SHORT loc_1_0453
        cmp    dword ptr [esp + 2ch], 1
        je     SHORT loc_1_0492
loc_1_0453:
        push   esi
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_1002
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_047F
        push   esi
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_0A1A
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0492
loc_1_047F:
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    ebp, 2740h
        jmp    NEAR PTR loc_1_0598
loc_1_0492:
        push   ebx
        db  0E8h, 0F8h, 009h, 000h, 000h
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_04B4
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    ebp, 2747h
        jmp    NEAR PTR loc_1_0598
loc_1_04B4:
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_0502
        push   0
        call   sub_1_1F60
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_04E8
        push   ebx
        call   sub_1_0FBF
        add    esp, 4
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    ebp, 2747h
        jmp    NEAR PTR loc_1_0598
loc_1_04E8:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 48h], eax
        push   edi
        mov    dword ptr [edi + 98h], eax
        push   dword ptr [esp + 14h]
        call   sub_1_1FDE
        add    esp, 8
loc_1_0502:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   edi
        push   ebx
        call   sub_1_105D
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_0557
        push   1
        push   edi
        call   sub_1_3250
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_0540
        push   ebx
        call   sub_1_13B9
        add    esp, 4
        or     esi, esi
        jne    SHORT loc_1_0557
loc_1_0540:
        mov    dword ptr [ebx + 1ch], edi
        mov    dword ptr [edi + 4ch], 2
        push   edi
        call   sub_1_347B
        add    esp, 4
        sub    ebp, ebp
        jmp    SHORT loc_1_0598
loc_1_0557:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_057B
        push   dword ptr [esp + 10h]
        call   sub_1_1FBC
        add    esp, 4
        mov    dword ptr [ebx + 48h], 0
        mov    dword ptr [edi + 98h], 0
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
        add    esp, 20h
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
        mov    edi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [edi + 74h]
        and    dword ptr [edi + 50h], 0ffffffefh
        cmp    dword ptr [edi + 4ch], 1
        jne    SHORT loc_1_0643
        mov    ebp, dword ptr [edi + 0b4h]
        add    ebp, 6
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], esi
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        mov    eax, dword ptr [ebx + 0ch]
        push   dword ptr [edi + 7ch]
        call   dword ptr [eax + 4]
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_0803
        cmp    esi, dword ptr [esp + 10h]
        mov    eax, 271eh
        jne    SHORT loc_1_0619
        push   esi
        push   ebp
        push   edi
        call   sub_1_0360
        add    esp, 0ch
loc_1_0619:
        or     eax, eax
        jne    NEAR PTR loc_1_0803
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_063D
        mov    dword ptr [edi + 54h], 0
        push   0
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
        jmp    SHORT loc_1_0647
loc_1_063D:
        and    dword ptr [edi + 54h], 0ffffffefh
        jmp    SHORT loc_1_0647
loc_1_0643:
        mov    esi, dword ptr [esp + 20h]
loc_1_0647:
        mov    eax, dword ptr [ebx + 18h]
        cmp    esi, eax
        jbe    SHORT loc_1_0650
        mov    esi, eax
loc_1_0650:
        cmp    dword ptr [ebx + 14h], esi
        ja     NEAR PTR loc_1_07FE
        push   esi
        push   dword ptr [esp + 20h]
        call   sub_1_3AD8
        mov    dword ptr [esp + 24h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0673
        jmp    NEAR PTR loc_1_07FE
loc_1_0673:
        mov    ebp, dword ptr [edi + 0b4h]
        push   esi
        add    ebp, 6
        push   dword ptr [esp + 20h]
        push   ebp
        call   0
        add    esp, 0ch
        push   esi
        push   dword ptr [esp + 20h]
        call   sub_1_3B1F
        add    esp, 8
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_06E4
        push   esi
        push   ebp
        call   sub_1_00A6
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_06E4
        mov    eax, dword ptr [edi + 94h]
        or     eax, eax
        je     SHORT loc_1_06C8
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    dword ptr [edi + 94h], 0
loc_1_06C8:
        mov    dword ptr [edi + 18h], 0
        mov    dword ptr [edi + 24h], 0
        mov    dword ptr [edi + 4ch], 2
        sub    eax, eax
        jmp    NEAR PTR loc_1_0803
loc_1_06E4:
        mov    eax, dword ptr [edi + 90h]
        mov    eax, dword ptr [eax + 20h]
        mov    cx, word ptr [ebp]
        cmp    word ptr [eax], cx
        je     SHORT loc_1_0721
        push   dword ptr [ebx]
        push   dword ptr [edi + 30h]
        push   dword ptr [edi + 2ch]
        push   dword ptr [edi + 28h]
        call   sub_1_3920
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0717
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_0803
loc_1_0717:
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0803
loc_1_0721:
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_0792
        mov    eax, dword ptr [edi + 94h]
        or     eax, eax
        jne    SHORT loc_1_076F
        push   0
        push   ebx
        call   sub_1_0890
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_074A
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_0803
loc_1_074A:
        mov    dword ptr [edi + 94h], eax
        mov    dword ptr [eax + 14h], edi
        mov    ecx, dword ptr [edi + 90h]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 1ch], 60000004h
        mov    ecx, dword ptr [eax + 24h]
        mov    dword ptr [edi + 18h], ecx
        mov    dword ptr [eax + 2ch], esi
        mov    dword ptr [edi + 24h], esi
loc_1_076F:
        push   esi
        push   ebp
        push   dword ptr [eax + 24h]
        call   0
        add    esp, 0ch
        mov    dword ptr [edi + 4ch], 6
        push   esi
        push   ebp
        push   edi
        call   sub_1_080B
        add    esp, 0ch
        sub    eax, eax
        jmp    SHORT loc_1_0803
loc_1_0792:
        inc    dword ptr [edi + 88h]
        push   ebx
        call   sub_1_3870
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_07B7
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    eax, 2747h
        jmp    SHORT loc_1_0803
loc_1_07B7:
        push   esi
        push   ebp
        push   ebx
        push   edi
        call   sub_1_349F
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_07EE
        cmp    esi, 0ffh
        je     SHORT loc_1_07EE
        push   ebx
        call   sub_1_38E3
        add    esp, 4
        push   edi
        call   sub_1_347B
        add    esp, 4
        push   esi
        call   sub_1_3720
        add    esp, 4
        jmp    SHORT loc_1_0803
loc_1_07EE:
        push   edi
        call   sub_1_347B
        add    esp, 4
        mov    eax, 2733h
        jmp    SHORT loc_1_0803
loc_1_07FE:
        mov    eax, 271eh
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
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 90h]
        mov    esi, dword ptr [eax + 48h]
        mov    eax, esi
        mov    edi, dword ptr [esi + 2ch]
        sub    eax, edi
        cmp    eax, -2ch
        je     SHORT loc_1_0878
        mov    ebx, dword ptr [esp + 18h]
loc_1_082C:
        mov    ebp, edi
        mov    edi, dword ptr [edi]
        push   dword ptr [esp + 1ch]
        push   ebx
        mov    eax, dword ptr [ebp + 2ch]
        add    eax, 6
        push   eax
        call   sub_1_0046
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_086F
        mov    eax, dword ptr [ebp + 20h]
        sub    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp + 0ch]
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
        cmp    eax, -2ch
        jne    SHORT loc_1_082C
loc_1_0878:
        mov    eax, dword ptr [esi + 2ch]
        sub    eax, esi
        cmp    eax, 2ch
        jne    SHORT loc_1_088A
        mov    eax, dword ptr [esp + 14h]
        and    dword ptr [eax + 50h], 0fffffffeh
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
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_08B2
        push   0
        call   sub_1_1F60
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_08B4
        sub    eax, eax
        jmp    NEAR PTR loc_1_093F
loc_1_08B2:
        sub    ebx, ebx
loc_1_08B4:
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 18h]
        lea    eax, [eax*2 + 40h]
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_08E2
        or     ebx, ebx
        je     SHORT loc_1_08DE
        push   ebx
        call   sub_1_1FBC
        add    esp, 4
loc_1_08DE:
        sub    eax, eax
        jmp    SHORT loc_1_093F
loc_1_08E2:
        mov    eax, dword ptr [esi + 18h]
        lea    ebp, [eax*2 + 40h]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 1ch], 1
        mov    dword ptr [edx + 3ch], ebx
        lea    eax, [edx + 40h]
        mov    dword ptr [edx + 20h], eax
        mov    eax, dword ptr [esi + 18h]
        add    eax, edx
        add    eax, 40h
        mov    dword ptr [edx + 24h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [edx + 2ch], eax
        mov    eax, dword ptr [esi + 18h]
        lea    ecx, [edx + 4]
        mov    dword ptr [edx + 28h], eax
        lea    eax, [esi + 34h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 38h]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [eax], ecx
        mov    eax, edx
        mov    dword ptr [esi + 38h], ecx
        inc    dword ptr [edx + 0ch]
loc_1_093F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0944:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 3ch]
        or     eax, eax
        je     SHORT loc_1_0960
        push   eax
        call   sub_1_1FBC
        add    esp, 4
        mov    dword ptr [esi + 3ch], 0
loc_1_0960:
        mov    edx, dword ptr [esi + 14h]
        mov    dword ptr [esi + 14h], 0
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_1_09EB
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 1ch], 9
        test   eax, 40000000h
        je     SHORT loc_1_09CC
        sub    eax, eax
        sub    ecx, ecx
        cmp    edx, eax
        jne    SHORT loc_1_0990
        mov    ecx, 1
        jmp    SHORT loc_1_09A8
loc_1_0990:
        cmp    word ptr [edx + 10h], 0
        je     SHORT loc_1_09A8
        movzx  eax, word ptr [edx + 12h]
        imul   eax, eax, 3e8h
        cmp    eax, 1
        sbb    ecx, ecx
        neg    ecx
loc_1_09A8:
        or     ecx, ecx
        je     SHORT loc_1_09AE
        jmp    SHORT loc_1_09C3
loc_1_09AE:
        push   eax
        push   esi
        call   sub_1_0CB8
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_09CC
        cmp    eax, 0ffh
        je     SHORT loc_1_09CC
loc_1_09C3:
        push   esi
        call   sub_1_0C90
        add    esp, 4
loc_1_09CC:
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_1_09FA
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_1_09FA
        mov    eax, dword ptr [esi + 10h]
        test   al, 8
        je     SHORT loc_1_09E3
        test   al, 1
        je     SHORT loc_1_09FA
loc_1_09E3:
        push   esi
        call   sub_1_0D19
        jmp    SHORT loc_1_09F7
loc_1_09EB:
        test   byte ptr [esi + 10h], 80h
        jne    SHORT loc_1_09FA
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
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_1_0A19
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
        mov    edi, dword ptr [esp + 14h]
        mov    eax, edi
        mov    ebx, dword ptr [edi + 34h]
        sub    eax, ebx
        cmp    eax, -34h
        je     SHORT loc_1_0A69
        mov    esi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [esp + 1ch]
loc_1_0A36:
        cmp    dword ptr [ebx + 18h], 9
        lea    ecx, [ebx - 4]
        je     SHORT loc_1_0A5E
        mov    eax, dword ptr [ecx + 14h]
        or     eax, eax
        je     SHORT loc_1_0A5E
        cmp    dword ptr [eax + 2ch], ebp
        jne    SHORT loc_1_0A5E
        push   dword ptr [ecx + 28h]
        push   esi
        push   dword ptr [ecx + 20h]
        call   sub_1_0046
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_0A70
loc_1_0A5E:
        mov    ebx, dword ptr [ebx]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -34h
        jne    SHORT loc_1_0A36
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
        jmp    SHORT loc_1_0A6B
sub_1_0A77:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 20h]
        lea    edi, [esp + 0ch]
        mov    ebx, dword ptr [esi]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        lea    eax, [esp + 0ch]
        push   esi
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        call   dword ptr [eax + 8]
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0ADE
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 30h], eax
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [esi + 18h], edi
        push   edi
        call   sub_1_0FDD
        add    esp, 4
        lea    eax, [esp + 0ch]
        push   dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        call   dword ptr [eax + 10h]
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        mov    eax, 0
        je     SHORT loc_1_0ADE
        push   edi
        call   sub_1_0FE5
        add    esp, 4
        mov    eax, esi
loc_1_0ADE:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_0AE5:
        sub    esp, 4ch
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 58h]
        lea    eax, [esp + 2ch]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esp + 50h], 4655424eh
        mov    dword ptr [esp + 40h], eax
        mov    dword ptr [esp + 4ch], 0ah
        lea    eax, [esp + 0ch]
        mov    dword ptr [esp + 44h], eax
        mov    dword ptr [esp + 48h], 0
        mov    dword ptr [esp + 3ch], 4655424eh
        mov    dword ptr [esp + 2ch], 0
        mov    eax, dword ptr [ecx + 18h]
        mov    dword ptr [esp + 38h], eax
        lea    edi, [esp + 1ch]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esp + 30h], eax
        mov    dword ptr [esp + 34h], 0
        mov    eax, dword ptr [esp + 38h]
        add    eax, 0ah
        mov    dword ptr [esp + 8], eax
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 8]
        push   1
        push   eax
        lea    eax, [esp + 48h]
        push   eax
        lea    eax, [esp + 28h]
        push   3
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        call   dword ptr [eax + 3ch]
        movzx  eax, word ptr [esp + 28h]
        add    esp, 14h
        add    eax, 2
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [esi + 28h], eax
        cmp    word ptr [edi], 2
        jne    SHORT loc_1_0B91
        add    edi, 8
        sub    eax, eax
        stosd
        stosd
loc_1_0B91:
        pop    edi
        pop    esi
        add    esp, 4ch
        ret
sub_1_0B97:
        sub    esp, 28h
        push   edi
        mov    edx, dword ptr [esp + 30h]
        mov    eax, dword ptr [esp + 38h]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [esp + 4], eax
        lea    edi, [esp + 8]
        mov    dword ptr [esp + 18h], 400h
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 200h
        mov    dword ptr [esp + 24h], 300h
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esp + 28h], eax
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [edx + 30h]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 4]
        push   4
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        call   dword ptr [eax + 4ch]
        add    esp, 10h
        pop    edi
        add    esp, 28h
        ret
sub_1_0C01:
        sub    esp, 38h
        push   edi
        mov    edx, dword ptr [esp + 40h]
        mov    eax, dword ptr [esp + 44h]
        mov    ecx, dword ptr [edx]
        lea    edi, [esp + 4]
        mov    dword ptr [esp + 14h], 400h
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 200h
        mov    dword ptr [esp + 20h], 300h
        mov    dword ptr [esp + 24h], eax
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [edx + 30h]
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 38h], 4655424eh
        mov    dword ptr [esp + 28h], 0
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esp + 2ch], eax
        mov    dword ptr [esp + 30h], 0
        lea    eax, [esp + 4ch]
        push   0
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        call   dword ptr [eax + 48h]
        add    esp, 14h
        pop    edi
        add    esp, 38h
        ret
sub_1_0C90:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 1ch], 10000008h
        je     SHORT loc_1_0CB7
        cmp    dword ptr [eax + 30h], 0
        je     SHORT loc_1_0CB7
        mov    dword ptr [eax + 1ch], 10000008h
        and    dword ptr [eax + 10h], 0fffffff7h
        push   eax
        call   sub_1_0D19
        add    esp, 4
loc_1_0CB7:
        ret
sub_1_0CB8:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        push   0
        mov    ecx, dword ptr [esi]
        inc    dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esp + 0ch], eax
        db  0C7h, 044h, 024h, 010h, 099h, 00Dh, 000h, 000h
        mov    dword ptr [esp + 14h], esi
        push   0
        lea    eax, [esp + 28h]
        push   4
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        call   dword ptr [eax + 1ch]
        add    esp, 14h
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0D06
        push   0
        push   edi
        push   esi
        call   sub_1_0D99
        add    esp, 0ch
        jmp    SHORT loc_1_0D11
loc_1_0D06:
        cmp    edi, 0ffh
        je     SHORT loc_1_0D11
        dec    dword ptr [esi + 0ch]
loc_1_0D11:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 10h
        ret
sub_1_0D19:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esi]
        inc    dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_1_0D31
        inc    dword ptr [eax + 0ch]
loc_1_0D31:
        or     dword ptr [esi + 10h], 80h
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esp + 8], eax
        db  0C7h, 044h, 024h, 00Ch, 0ABh, 00Dh, 000h, 000h
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 10h], esi
        mov    dword ptr [esi + 30h], 0
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        call   dword ptr [eax + 0ch]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0ffh
        je     SHORT loc_1_0D76
        push   0
        push   edi
        push   esi
        call   sub_1_0DAB
        add    esp, 0ch
loc_1_0D76:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 10h
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
        or     dword ptr [eax + 10h], 2
        call   sub_1_09FC
        add    esp, 4
        ret
sub_1_0DAB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi]
        and    dword ptr [esi + 10h], 0ffffff7fh
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_1_0DD7
        push   eax
        call   sub_1_0FE5
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_1_0FBF
        add    esp, 4
loc_1_0DD7:
        push   esi
        call   sub_1_09FC
        add    esp, 4
        mov    ebx, dword ptr [edi + 2ch]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -2ch
        je     SHORT loc_1_0E1E
        mov    esi, 40000003h
        mov    ebp, 3
loc_1_0DF6:
        lea    eax, [ebx - 4]
        mov    ebx, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 1ch]
        cmp    dword ptr [eax + 18h], esi
        jne    SHORT loc_1_0E15
        or     ecx, ecx
        je     SHORT loc_1_0E15
        cmp    dword ptr [ecx + 4ch], ebp
        jne    SHORT loc_1_0E15
        push   eax
        call   sub_1_127F
        add    esp, 4
loc_1_0E15:
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -2ch
        jne    SHORT loc_1_0DF6
loc_1_0E1E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 0CDh, 020h, 001h, 080h, 088h, 004h, 0CDh, 020h, 002h, 080h, 088h, 004h, 0CDh, 020h, 03Fh
        db  080h, 001h, 000h, 0CDh, 020h, 003h, 080h, 088h, 004h, 0CDh, 020h, 004h, 080h, 088h, 004h, 0CDh
        db  020h, 00Dh, 080h, 040h, 000h, 0CDh, 020h, 00Eh, 080h, 040h, 000h, 0CDh, 020h, 011h, 080h, 040h
        db  000h, 0CDh, 020h, 005h, 080h, 088h, 004h, 0CDh, 020h, 006h, 080h, 088h, 004h, 0A1h, 0D8h, 03Bh
        db  000h, 000h, 0CDh, 020h, 009h, 080h, 088h, 004h
sub_1_0E6B:
        db  0A1h, 0D8h, 03Bh, 000h, 000h
        db  0CDh, 020h, 00Ch, 080h, 088h, 004h
sub_1_0E76:
        db  0A1h, 0D8h, 03Bh, 000h, 000h
        db  0CDh, 020h, 00Bh, 080h, 088h, 004h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 56h], dl
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 18h]
        add    eax, 4ch
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        mov    eax, 0
        je     SHORT loc_1_0F00
        mov    ebx, dword ptr [esi + 18h]
        add    ebx, 4ch
        mov    edi, edx
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        lea    eax, [edx + 4ch]
        mov    dword ptr [edx + 20h], eax
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 18h], 1
        lea    eax, [edx + 38h]
        mov    dword ptr [edx + 3ch], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 40h]
        mov    dword ptr [edx + 44h], eax
        mov    dword ptr [eax], eax
        lea    ecx, [edx + 4]
        lea    eax, [esi + 2ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [eax], ecx
        mov    eax, edx
        mov    dword ptr [esi + 30h], ecx
        inc    dword ptr [edx + 0ch]
loc_1_0F00:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0F04:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi]
        inc    dword ptr [esi + 0ch]
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [esi + 18h], 4
        mov    eax, dword ptr [esi + 38h]
        sub    eax, esi
        cmp    eax, 38h
        je     SHORT loc_1_0F40
loc_1_0F24:
        push   esi
        call   sub_1_132F
        add    esp, 4
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    eax, dword ptr [esi + 38h]
        sub    eax, esi
        cmp    eax, 38h
        jne    SHORT loc_1_0F24
loc_1_0F40:
        mov    eax, dword ptr [esi + 40h]
        sub    eax, esi
        cmp    eax, 40h
        je     SHORT loc_1_0F66
loc_1_0F4A:
        push   esi
        call   sub_1_1308
        add    esp, 4
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    eax, dword ptr [esi + 40h]
        sub    eax, esi
        cmp    eax, 40h
        jne    SHORT loc_1_0F4A
loc_1_0F66:
        mov    eax, dword ptr [esi + 48h]
        or     eax, eax
        je     SHORT loc_1_0F7D
        push   eax
        call   sub_1_1FBC
        add    esp, 4
        mov    dword ptr [esi + 48h], 0
loc_1_0F7D:
        dec    dword ptr [esi + 0ch]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_0FBB
        cmp    ebx, 40000003h
        jne    SHORT loc_1_0FA5
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_0FA5
        push   0
        push   0
        push   0
        push   eax
        mov    eax, dword ptr [edi + 10h]
        call   dword ptr [eax + 38h]
        add    esp, 10h
loc_1_0FA5:
        cmp    dword ptr [esi + 14h], 0
        push   esi
        je     SHORT loc_1_0FB3
        call   sub_1_13B9
        jmp    SHORT loc_1_0FB8
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
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_1_0FDC
        push   ecx
        call   sub_1_0F04
        add    esp, 4
        mov    eax, 1
loc_1_0FDC:
        ret
sub_1_0FDD:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 28h]
        ret
sub_1_0FE5:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 28h]
        dec    ecx
        mov    dword ptr [eax + 28h], ecx
        jne    SHORT loc_1_1001
        cmp    dword ptr [eax + 0ch], 0
        jne    SHORT loc_1_1001
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
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, dword ptr [eax + 2ch]
        sub    eax, ebx
        cmp    eax, -2ch
        je     SHORT loc_1_104F
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 20h]
loc_1_1020:
        cmp    dword ptr [ebx + 14h], 4
        lea    eax, [ebx - 4]
        je     SHORT loc_1_1042
        mov    ecx, dword ptr [eax + 1ch]
        cmp    dword ptr [ecx + 2ch], edi
        jne    SHORT loc_1_1042
        push   ebp
        push   esi
        push   dword ptr [eax + 20h]
        call   sub_1_0046
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1056
loc_1_1042:
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [esp + 14h]
        sub    eax, ebx
        cmp    eax, -2ch
        jne    SHORT loc_1_1020
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
        jmp    SHORT loc_1_1051
sub_1_105D:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 28h]
        lea    edi, [esp + 14h]
        mov    esi, dword ptr [ebx]
        mov    byte ptr [esp + 12h], 1
        mov    byte ptr [esp + 13h], 0
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        lea    eax, [esp + 12h]
        mov    edi, dword ptr [esp + 38h]
        push   eax
        push   edi
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 44h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        call   dword ptr [eax]
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_1_1132
        mov    ebp, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 18h], 40000002h
        mov    dword ptr [ebx + 14h], ebp
        mov    ecx, dword ptr [ebx + 20h]
        mov    eax, dword ptr [esp + 30h]
        mov    word ptr [ecx], ax
        push   ebx
        call   sub_1_113A
        mov    ecx, dword ptr [esp + 30h]
        add    esp, 4
        mov    dword ptr [ecx + 90h], ebx
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ecx + 14h], eax
        mov    edx, dword ptr [ebx + 24h]
        mov    dword ptr [ecx + 20h], edx
        movzx  eax, word ptr [eax]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ecx + 2ch], eax
        mov    dword ptr [ecx + 30h], edi
        push   ebx
        db  068h, 007h, 015h, 000h, 000h
        push   1
        mov    eax, dword ptr [esi + 10h]
        push   ebp
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   ebx
        db  068h, 0E4h, 015h, 000h, 000h
        push   3
        mov    eax, dword ptr [esi + 10h]
        push   ebp
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   ebx
        db  068h, 061h, 016h, 000h, 000h
        push   5
        mov    eax, dword ptr [esi + 10h]
        push   ebp
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   ebx
        db  068h, 0DEh, 016h, 000h, 000h
        push   4
        mov    eax, dword ptr [esi + 10h]
        push   ebp
        call   dword ptr [eax + 38h]
        add    esp, 10h
        sub    eax, eax
loc_1_1132:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_1_113A:
        sub    esp, 4ch
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 58h]
        lea    eax, [esp + 2ch]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esp + 50h], 4655424eh
        mov    dword ptr [esp + 40h], eax
        mov    dword ptr [esp + 4ch], 0ah
        lea    eax, [esp + 0ch]
        mov    dword ptr [esp + 44h], eax
        mov    dword ptr [esp + 48h], 0
        mov    dword ptr [esp + 3ch], 4655424eh
        mov    dword ptr [esp + 2ch], 0
        mov    eax, dword ptr [ecx + 18h]
        mov    dword ptr [esp + 38h], eax
        lea    edi, [esp + 1ch]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esp + 30h], eax
        mov    dword ptr [esp + 34h], 0
        mov    eax, dword ptr [esp + 38h]
        add    eax, 0ah
        mov    dword ptr [esp + 8], eax
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 8]
        push   0
        push   eax
        lea    eax, [esp + 48h]
        push   eax
        lea    eax, [esp + 28h]
        push   3
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        call   dword ptr [eax + 3ch]
        movzx  eax, word ptr [esp + 28h]
        add    esp, 14h
        add    eax, 2
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [esi + 24h], eax
        cmp    word ptr [edi], 2
        jne    SHORT loc_1_11E6
        add    edi, 8
        sub    eax, eax
        stosd
        stosd
loc_1_11E6:
        pop    edi
        pop    esi
        add    esp, 4ch
        ret
sub_1_11EC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        push   1
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 14h], eax
        push   dword ptr [esp + 14h]
        call   sub_1_0890
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_1218
        mov    eax, 1
        jmp    SHORT loc_1_1277
loc_1_1218:
        cmp    dword ptr [ebx + 28h], 1
        sbb    ebp, ebp
        neg    ebp
        push   ebx
        push   esi
        call   sub_1_0A77
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_123D
        push   esi
        call   sub_1_09FC
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_1277
loc_1_123D:
        mov    edi, dword ptr [ebx + 1ch]
        mov    dword ptr [esi + 14h], edi
        push   esi
        push   ebx
        call   sub_1_12E7
        add    esp, 8
        push   edi
        push   dword ptr [esi + 3ch]
        call   sub_1_1FDE
        add    esp, 8
        or     ebp, ebp
        je     SHORT loc_1_1275
        push   ebx
        db  068h, 040h, 014h, 000h, 000h
        push   0
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [ebx + 14h]
        mov    eax, dword ptr [eax + 10h]
        call   dword ptr [eax + 38h]
        add    esp, 10h
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
        mov    esi, dword ptr [esp + 10h]
        sub    eax, eax
        mov    ecx, dword ptr [esi + 2ch]
        mov    ebx, dword ptr [esi + 30h]
        mov    edi, dword ptr [esi + 34h]
        cmp    ecx, eax
        je     SHORT loc_1_12C0
        cmp    edi, ebx
        jbe    SHORT loc_1_12C0
        sub    edi, ebx
        cmp    edi, ecx
        jb     SHORT loc_1_12A1
        mov    edi, ecx
loc_1_12A1:
        or     edi, edi
        je     SHORT loc_1_12B8
loc_1_12A5:
        push   esi
        call   sub_1_11EC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_12B8
        dec    edi
        inc    ebx
        or     edi, edi
        jne    SHORT loc_1_12A5
loc_1_12B8:
        mov    dword ptr [esi + 2ch], edi
        mov    dword ptr [esi + 30h], ebx
        jmp    SHORT loc_1_12C2
loc_1_12C0:
        sub    eax, eax
loc_1_12C2:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_12C6:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 34h]
        lea    eax, [ecx + 40h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 44h]
        mov    dword ptr [edx + 38h], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 44h], esi
        inc    dword ptr [ecx + 30h]
        pop    esi
        ret
sub_1_12E7:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 34h]
        lea    eax, [ecx + 38h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 3ch]
        mov    dword ptr [edx + 38h], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 3ch], esi
        inc    dword ptr [ecx + 30h]
        pop    esi
        ret
sub_1_1308:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 40h]
        lea    edx, [esi + 40h]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 40h
        jne    SHORT loc_1_1320
        sub    eax, eax
        jmp    SHORT loc_1_132D
loc_1_1320:
        mov    ecx, dword ptr [eax]
        sub    eax, 34h
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        dec    dword ptr [esi + 30h]
loc_1_132D:
        pop    esi
        ret
sub_1_132F:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx + 38h]
        lea    esi, [edx + 38h]
        mov    ecx, eax
        sub    ecx, edx
        cmp    ecx, 38h
        jne    SHORT loc_1_1347
        sub    eax, eax
        jmp    SHORT loc_1_1354
loc_1_1347:
        mov    ecx, dword ptr [eax]
        sub    eax, 34h
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        dec    dword ptr [edx + 30h]
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
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 8]
        db  0C7h, 044h, 024h, 00Ch, 01Eh, 014h, 000h, 000h
        mov    dword ptr [esp + 10h], esi
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0ffh
        je     SHORT loc_1_13FB
        push   0
        push   edi
        push   esi
        call   sub_1_141E
        add    esp, 0ch
loc_1_13FB:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 10h
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
        mov    dword ptr [eax + 14h], 0
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
        mov    eax, dword ptr [esp + 20h]
        add    eax, 6
        mov    dword ptr [esp + 10h], eax
        mov    ebx, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 18h]
        sub    ebx, 6
        mov    edi, dword ptr [ebp]
        push   ebp
        call   sub_1_127F
        add    esp, 4
        mov    esi, dword ptr [ebp + 1ch]
        cmp    dword ptr [ebp + 18h], 4
        je     NEAR PTR loc_1_14FA
        mov    eax, dword ptr [ebp + 38h]
        sub    eax, ebp
        cmp    eax, 38h
        je     SHORT loc_1_14FA
        or     esi, esi
        je     SHORT loc_1_14FA
        cmp    dword ptr [edi + 18h], ebx
        jb     SHORT loc_1_14FA
        cmp    dword ptr [edi + 14h], ebx
        ja     SHORT loc_1_14FA
        mov    eax, dword ptr [esp + 10h]
        movzx  eax, word ptr [eax]
        cmp    dword ptr [esi + 28h], eax
        jne    SHORT loc_1_14FA
        push   ebp
        call   sub_1_132F
        mov    edi, eax
        add    esp, 4
        push   ebx
        mov    dword ptr [edi + 1ch], 60000002h
        push   dword ptr [esp + 14h]
        push   dword ptr [edi + 24h]
        call   0
        add    esp, 0ch
        mov    dword ptr [edi + 2ch], ebx
        inc    dword ptr [esi + 48h]
        inc    dword ptr [esi + 88h]
        inc    dword ptr [edi + 0ch]
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esp + 38h]
        or     dword ptr [edi + 10h], 40h
        db  0C7h, 000h, 0A0h, 017h, 000h, 000h
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [eax], edi
        mov    eax, 18h
        jmp    SHORT loc_1_14FF
loc_1_14FA:
        mov    eax, 0c00000d0h
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
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edx + 10h]
        or     ecx, 1
        mov    dword ptr [edx + 10h], ecx
        test   cl, 40h
        je     SHORT loc_1_1520
        jmp    NEAR PTR loc_1_15DF
loc_1_1520:
        test   cl, 2
        je     SHORT loc_1_1539
        cmp    dword ptr [edx + 0ch], 0
        jne    SHORT loc_1_1539
        push   edx
        call   sub_1_0944
        add    esp, 4
        jmp    NEAR PTR loc_1_15DF
loc_1_1539:
        mov    eax, dword ptr [edx + 1ch]
        cmp    eax, 9
        je     NEAR PTR loc_1_15DF
        cmp    eax, 10000008h
        je     NEAR PTR loc_1_15DF
        mov    edi, dword ptr [esp + 24h]
        and    edi, 2
        je     SHORT loc_1_1562
        mov    dword ptr [edx + 1ch], 10000008h
        jmp    SHORT loc_1_1575
loc_1_1562:
        test   cl, 4
        mov    dword ptr [edx + 1ch], 10000007h
        jne    SHORT loc_1_1575
        mov    dword ptr [edx + 1ch], 50000005h
loc_1_1575:
        mov    esi, dword ptr [edx + 14h]
        cmp    eax, 60000002h
        je     SHORT loc_1_15DF
        or     esi, esi
        je     SHORT loc_1_15DF
        or     edi, edi
        je     SHORT loc_1_15A0
        or     ecx, 20h
        mov    dword ptr [edx + 10h], ecx
        cmp    dword ptr [esi + 4ch], 3
        je     SHORT loc_1_15A0
        push   esi
        call   sub_1_35D4
        add    esp, 4
        or     dword ptr [esi + 3ch], 1
loc_1_15A0:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_15BA
        or     edi, edi
        jne    SHORT loc_1_15BA
        mov    eax, dword ptr [esi + 98h]
        mov    ecx, dword ptr [eax + 2ch]
        sub    ecx, eax
        cmp    ecx, 2ch
        jne    SHORT loc_1_15DF
loc_1_15BA:
        cmp    edi, 1
        cmc
        sbb    eax, eax
        and    eax, 2746h
        push   eax
        push   20h
        push   esi
        call   sub_1_3BCC
        add    esp, 0ch
        push   2746h
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
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [edi], 0
        test   byte ptr [esi + 1fh], 20h
        je     SHORT loc_1_164D
        mov    eax, dword ptr [esi + 14h]
        test   byte ptr [eax + 40h], 80h
        je     SHORT loc_1_1615
        push   2746h
        push   20h
        push   eax
        call   sub_1_3BCC
        add    esp, 0ch
        jmp    SHORT loc_1_164D
loc_1_1615:
        db  068h, 0DCh, 03Bh, 000h, 000h
        mov    edi, dword ptr [esi + 3ch]
        push   dword ptr [esp + 2ch]
        push   0
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 2ch]
        push   edi
        call   sub_1_182E
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_1646
        mov    eax, 18h
        or     dword ptr [esi + 10h], 10h
        mov    dword ptr [edi + 44h], esi
        jmp    SHORT loc_1_165E
loc_1_1646:
        mov    eax, 17h
        jmp    SHORT loc_1_165E
loc_1_164D:
        push   esi
        call   sub_1_0C90
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [edi], ecx
loc_1_165E:
        pop    edi
        pop    esi
        ret
loc_1_1661:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [edi], 0
        test   byte ptr [esi + 1fh], 20h
        je     SHORT loc_1_16CA
        mov    eax, dword ptr [esi + 14h]
        test   byte ptr [eax + 40h], 80h
        je     SHORT loc_1_1692
        push   2746h
        push   20h
        push   eax
        call   sub_1_3BCC
        add    esp, 0ch
        jmp    SHORT loc_1_16CA
loc_1_1692:
        db  068h, 0E0h, 03Bh, 000h, 000h
        mov    edi, dword ptr [esi + 3ch]
        push   dword ptr [esp + 2ch]
        push   0
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 2ch]
        push   edi
        call   sub_1_182E
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_16C3
        mov    eax, 18h
        or     dword ptr [esi + 10h], 10h
        mov    dword ptr [edi + 44h], esi
        jmp    SHORT loc_1_16DB
loc_1_16C3:
        mov    eax, 17h
        jmp    SHORT loc_1_16DB
loc_1_16CA:
        push   esi
        call   sub_1_0C90
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 1ch]
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
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 14h]
        sub    ecx, 6
        mov    eax, dword ptr [ebx + 1ch]
        or     eax, eax
        je     SHORT loc_1_1725
        mov    eax, dword ptr [eax + 94h]
        or     eax, eax
        je     SHORT loc_1_1725
        push   ecx
        push   dword ptr [eax + 24h]
        lea    eax, [esi + 6]
        push   eax
        call   sub_1_0046
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1725
        mov    ebp, dword ptr [esp + 34h]
        mov    edi, dword ptr [esp + 30h]
        mov    dword ptr [ebp], edi
        sub    eax, eax
        jmp    SHORT loc_1_179B
loc_1_1725:
        mov    ebp, dword ptr [esp + 34h]
        mov    dword ptr [ebp], 0
        test   byte ptr [ebx + 1bh], 40h
        je     SHORT loc_1_1796
        db  068h, 0DCh, 03Bh, 000h, 000h
        push   0
        push   1
        mov    edi, dword ptr [esp + 3ch]
        push   0
        push   edi
        push   dword ptr [ebx + 48h]
        call   sub_1_182E
        add    esp, 18h
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_178F
        mov    ecx, dword ptr [ebx + 2ch]
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
        add    ebx, 48h
        mov    eax, dword ptr [esp + 48h]
        add    esp, 0ch
        mov    dword ptr [eax], ebx
        mov    eax, 18h
        jmp    SHORT loc_1_179B
loc_1_178F:
        mov    dword ptr [ebp], edi
        sub    eax, eax
        jmp    SHORT loc_1_179B
loc_1_1796:
        mov    eax, 17h
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
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 14h]
        and    dword ptr [esi + 10h], 0ffffffbfh
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_17E7
        push   esi
        push   edi
        call   sub_1_12C6
        add    esp, 8
        push   0
        push   8
        push   ebx
        call   sub_1_3BCC
        add    esp, 0ch
        mov    eax, dword ptr [esi + 10h]
        test   al, 1
        je     SHORT loc_1_17DF
        mov    dword ptr [esi + 1ch], 50000005h
        jmp    SHORT loc_1_180F
loc_1_17DF:
        or     eax, 8
        mov    dword ptr [esi + 10h], eax
        jmp    SHORT loc_1_180F
loc_1_17E7:
        cmp    dword ptr [esi + 1ch], 60000002h
        jne    SHORT loc_1_180F
        mov    dword ptr [esi + 1ch], 1
        dec    dword ptr [ebx + 48h]
        cmp    dword ptr [esi + 0ch], 1
        jbe    SHORT loc_1_180F
        push   esi
        push   dword ptr [ebx + 90h]
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
        mov    ebx, dword ptr [esp + 14h]
        or     ebx, ebx
        je     SHORT loc_1_1893
        cmp    dword ptr [ebx + 14h], 0
        ja     SHORT loc_1_1893
        cmp    dword ptr [ebx + 1ch], 0
        ja     SHORT loc_1_1893
        mov    eax, dword ptr [ebx + 10h]
        cmp    dword ptr [ebx + 0ch], eax
        jb     SHORT loc_1_1893
        mov    ebp, dword ptr [esp + 1ch]
        test   ebp, 400h
        mov    edi, dword ptr [esp + 18h]
        jne    SHORT loc_1_1886
        cmp    edi, 111ch
        jae    SHORT loc_1_1886
        push   111ch
        call   sub_1_3F0F
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_18C8
        push   edi
        call   sub_1_3F0F
        add    esp, 4
        mov    esi, eax
        jmp    SHORT loc_1_189D
loc_1_1886:
        push   edi
        call   sub_1_3F0F
        add    esp, 4
        mov    esi, eax
        jmp    SHORT loc_1_189D
loc_1_1893:
        sub    esi, esi
        mov    edi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [esp + 1ch]
loc_1_189D:
        or     esi, esi
        jne    SHORT loc_1_18C8
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_1_18C4
        test   ebp, 40h
        je     SHORT loc_1_18BB
        add    dword ptr [ebx + 1ch], edi
        and    ebp, 0ffffffdfh
        mov    dword ptr [ebx + 20h], ebp
        jmp    SHORT loc_1_18C4
loc_1_18BB:
        add    dword ptr [ebx + 14h], edi
        and    ebp, 0ffffffbfh
        mov    dword ptr [ebx + 18h], ebp
loc_1_18C4:
        sub    eax, eax
        jmp    SHORT loc_1_1908
loc_1_18C8:
        mov    dword ptr [esi + 18h], ebx
        push   ebp
        call   sub_1_3851
        add    esp, 4
        or     dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 24h]
        inc    dword ptr [eax + 88h]
        or     ecx, ecx
        jne    SHORT loc_1_18EA
        lea    ecx, [esi + 48h]
loc_1_18EA:
        lea    eax, [esi + 5ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ecx + 4], eax
        db  0C7h, 041h, 008h, 00Dh, 019h, 000h, 000h
        mov    dword ptr [ecx + 0ch], esi
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ecx + 10h], eax
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
        mov    ebp, dword ptr [esp + 18h]
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 1
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 44h]
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0fffffffdh
        add    eax, 4
        mov    dword ptr [esp + 10h], eax
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_195F
        mov    dword ptr [ebp + 18h], edi
        push   dword ptr [esp + 20h]
        push   ebp
        call   sub_1_3FF1
        add    esp, 8
        push   ebp
        push   esi
        call   sub_1_202A
        mov    eax, dword ptr [esp + 28h]
        add    esp, 8
        add    dword ptr [esi + 10h], eax
        jmp    SHORT loc_1_1976
loc_1_195F:
        or     dword ptr [ebx + 10h], 20h
        cmp    dword ptr [edi + 4ch], 3
        je     SHORT loc_1_196D
        or     dword ptr [edi + 3ch], 1
loc_1_196D:
        push   ebp
        call   sub_1_3F46
        add    esp, 4
loc_1_1976:
        or     ebx, ebx
        je     SHORT loc_1_197E
        and    dword ptr [ebx + 10h], 0ffffffefh
loc_1_197E:
        mov    esi, dword ptr [edi + 4ch]
        push   edi
        call   sub_1_347B
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_19AD
        cmp    esi, 3
        je     SHORT loc_1_19AD
        push   dword ptr [esp + 1ch]
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_19AD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_19C0:
        mov    edx, dword ptr [esp + 18h]
        sub    edx, 50h
        cmp    dword ptr [edx + 10h], 0
        je     SHORT loc_1_1A33
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_1_19E6
        cmp    eax, 1
        je     SHORT loc_1_1A02
        cmp    eax, 2
        je     SHORT loc_1_1A19
        mov    eax, 2726h
        jmp    SHORT loc_1_1A38
loc_1_19E6:
        mov    ecx, dword ptr [esp + 8]
        lea    eax, [ecx + 14h]
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   ecx
        push   edx
        call   sub_1_1A39
        add    esp, 14h
        jmp    SHORT loc_1_1A38
loc_1_1A02:
        mov    ecx, dword ptr [esp + 8]
        push   dword ptr [ecx + 14h]
        lea    eax, [ecx + 18h]
        push   eax
        push   ecx
        push   edx
        call   sub_1_1B26
        add    esp, 10h
        jmp    SHORT loc_1_1A38
loc_1_1A19:
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_1_1BA6
        add    esp, 10h
        jmp    SHORT loc_1_1A38
loc_1_1A33:
        mov    eax, 2742h
loc_1_1A38:
        ret
sub_1_1A39:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 40h]
        or     eax, eax
        je     NEAR PTR loc_1_1B19
        mov    edi, dword ptr [esp + 44h]
        or     edi, edi
        je     NEAR PTR loc_1_1B19
        mov    ebp, dword ptr [esp + 48h]
        or     ebp, ebp
        je     NEAR PTR loc_1_1B19
        mov    ebx, dword ptr [esp + 4ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_1B19
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [esp + 10h], ecx
        push   14h
        push   eax
        call   sub_1_3AD8
        add    esp, 8
        mov    esi, eax
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_3AD8
        add    esp, 8
        mov    edi, eax
        push   10h
        push   ebx
        call   sub_1_3AD8
        add    esp, 8
        mov    ebx, eax
        or     esi, esi
        je     SHORT loc_1_1AED
        or     edi, edi
        je     SHORT loc_1_1AED
        or     ebx, ebx
        je     SHORT loc_1_1AED
        mov    dword ptr [esp + 34h], 4655424eh
        mov    dword ptr [esp + 24h], 0
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 30h], eax
        mov    dword ptr [esp + 28h], edi
        mov    dword ptr [esp + 2ch], 0
        push   ebx
        lea    eax, [esp + 28h]
        push   ebp
        push   eax
        lea    eax, [esp + 20h]
        push   esi
        push   eax
        mov    eax, dword ptr [esp + 50h]
        mov    eax, dword ptr [eax + 10h]
        call   dword ptr [eax + 48h]
        add    esp, 14h
        mov    ebp, eax
        jmp    SHORT loc_1_1AF2
loc_1_1AED:
        mov    ebp, 0ch
loc_1_1AF2:
        push   10h
        push   ebx
        call   sub_1_3B1F
        add    esp, 8
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_3B1F
        add    esp, 8
        push   14h
        push   esi
        call   sub_1_3B1F
        add    esp, 8
        mov    eax, ebp
        jmp    SHORT loc_1_1B1E
loc_1_1B19:
        mov    eax, 57h
loc_1_1B1E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
sub_1_1B26:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        or     eax, eax
        je     SHORT loc_1_1B99
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_1B99
        push   14h
        push   eax
        call   sub_1_3AD8
        add    esp, 8
        mov    esi, eax
        mov    ebx, dword ptr [esp + 30h]
        push   ebx
        push   edi
        call   sub_1_3AD8
        add    esp, 8
        mov    edi, eax
        or     esi, esi
        je     SHORT loc_1_1B7B
        or     edi, edi
        je     SHORT loc_1_1B7B
        push   ebx
        push   edi
        lea    eax, [esp + 18h]
        push   esi
        push   eax
        mov    eax, dword ptr [esp + 34h]
        mov    eax, dword ptr [eax + 10h]
        call   dword ptr [eax + 4ch]
        add    esp, 10h
        mov    ebp, eax
        jmp    SHORT loc_1_1B80
loc_1_1B7B:
        mov    ebp, 0ch
loc_1_1B80:
        push   ebx
        push   edi
        call   sub_1_3B1F
        add    esp, 8
        push   14h
        push   esi
        call   sub_1_3B1F
        add    esp, 8
        mov    eax, ebp
        jmp    SHORT loc_1_1B9E
loc_1_1B99:
        mov    eax, 57h
loc_1_1B9E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_1BA6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 1ch]
        or     ecx, ecx
        je     SHORT loc_1_1C32
        mov    esi, dword ptr [esp + 20h]
        or     esi, esi
        je     SHORT loc_1_1C32
        mov    edi, dword ptr [esp + 24h]
        or     edi, edi
        je     SHORT loc_1_1C32
        mov    ebx, dword ptr [esp + 28h]
        or     ebx, ebx
        je     SHORT loc_1_1C32
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 10h], eax
        push   dword ptr [esp + 14h]
        push   ecx
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_3AD8
        add    esp, 8
        mov    edi, eax
        or     ebp, ebp
        je     SHORT loc_1_1C0F
        or     edi, edi
        je     SHORT loc_1_1C0F
        push   ebx
        push   edi
        push   esi
        push   ebp
        call   sub_1_3A7A
        add    esp, 10h
        mov    esi, eax
        jmp    SHORT loc_1_1C14
loc_1_1C0F:
        mov    esi, 0ch
loc_1_1C14:
        push   dword ptr [esp + 14h]
        push   ebp
        call   sub_1_3B1F
        add    esp, 8
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_3B1F
        add    esp, 8
        mov    eax, esi
        jmp    SHORT loc_1_1C37
loc_1_1C32:
        mov    eax, 57h
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
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 4004667fh
        je     SHORT loc_1_1C9E
        cmp    eax, 40047307h
        je     SHORT loc_1_1CB5
        jmp    NEAR PTR loc_1_1D65
loc_1_1C9E:
        push   dword ptr [esp + 14h]
        call   sub_1_1D72
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1D6A
loc_1_1CB5:
        mov    edi, dword ptr [esp + 14h]
        cmp    dword ptr [edi + 2ch], 1
        jne    NEAR PTR loc_1_1D65
        test   byte ptr [edi + 40h], 10h
        je     NEAR PTR loc_1_1D65
        mov    eax, dword ptr [edi + 94h]
        or     eax, eax
        jne    SHORT loc_1_1CE6
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    NEAR PTR loc_1_1D6A
loc_1_1CE6:
        lea    ecx, [esp + 8]
        push   8
        push   ecx
        push   5
        push   eax
        call   sub_1_0C01
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1D09
        push   eax
        call   sub_1_3720
        add    esp, 4
        mov    esi, eax
        jmp    SHORT loc_1_1D6A
loc_1_1D09:
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_1D23
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_1D28
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_1_1D6A
loc_1_1D23:
        cmp    dword ptr [esp + 0ch], 0
loc_1_1D28:
        jle    SHORT loc_1_1D36
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_1D6A
loc_1_1D36:
        mov    eax, dword ptr [edi + 98h]
        or     eax, eax
        jne    SHORT loc_1_1D4C
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_1_1D6A
loc_1_1D4C:
        mov    ecx, dword ptr [esp + 0ch]
        neg    ecx
        cmp    dword ptr [eax + 10h], ecx
        mov    eax, 0
        setbe  al
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_1D6A
loc_1_1D65:
        mov    esi, 2726h
loc_1_1D6A:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 8
        ret
sub_1_1D72:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        sub    eax, eax
        mov    edi, dword ptr [esi + 98h]
        cmp    edi, eax
        jne    SHORT loc_1_1D86
        jmp    SHORT loc_1_1DC9
loc_1_1D86:
        cmp    dword ptr [esi + 2ch], 1
        jne    SHORT loc_1_1DB1
        mov    eax, dword ptr [edi + 10h]
        test   byte ptr [esi + 40h], 10h
        jne    SHORT loc_1_1DC0
        mov    edx, dword ptr [edi + 3ch]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -3ch
        je     SHORT loc_1_1DC0
loc_1_1DA1:
        sub    eax, dword ptr [edx + 18h]
        mov    edx, dword ptr [edx]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -3ch
        jne    SHORT loc_1_1DA1
        jmp    SHORT loc_1_1DC0
loc_1_1DB1:
        mov    ecx, dword ptr [edi + 2ch]
        mov    edx, ecx
        sub    edx, edi
        cmp    edx, 2ch
        je     SHORT loc_1_1DC0
        mov    eax, dword ptr [ecx + 20h]
loc_1_1DC0:
        mov    ecx, dword ptr [esi + 38h]
        cmp    eax, ecx
        jbe    SHORT loc_1_1DC9
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
        mov    esi, dword ptr [esp + 14h]
        inc    dword ptr [esi + 88h]
        mov    edi, dword ptr [esi + 90h]
        mov    eax, dword ptr [esp + 18h]
        add    dword ptr [edi + 2ch], eax
        mov    dword ptr [edi + 34h], eax
        push   edi
        call   sub_1_127F
        add    esp, 4
        mov    eax, dword ptr [edi + 38h]
        sub    eax, edi
        cmp    eax, 38h
        jne    SHORT loc_1_1E11
        push   esi
        call   sub_1_347B
        add    esp, 4
        mov    ebx, 2747h
        jmp    SHORT loc_1_1E50
loc_1_1E11:
        mov    ebp, dword ptr [edi + 18h]
        mov    dword ptr [edi + 18h], 40000003h
        push   2
        push   esi
        call   sub_1_3250
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_1E31
        mov    dword ptr [edi + 18h], ebp
        jmp    SHORT loc_1_1E47
loc_1_1E31:
        mov    dword ptr [esi + 4ch], 3
        movzx  eax, byte ptr [esp + 18h]
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [esi + 54h], 37h
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
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        jne    SHORT loc_1_1E6D
        jmp    SHORT loc_1_1EB3
loc_1_1E6D:
        db  0BAh, 010h, 042h, 000h, 000h
        db  08Bh, 035h, 060h, 042h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1EB3
loc_1_1E7C:
        mov    eax, dword ptr [edx]
        mov    ebx, ecx
        mov    edi, dword ptr [eax + 4]
loc_1_1E83:
        mov    al, byte ptr [edi]
        cmp    al, byte ptr [ebx]
        jne    SHORT loc_1_1EA3
        or     al, al
        je     SHORT loc_1_1E9F
        mov    al, byte ptr [edi + 1]
        cmp    al, byte ptr [ebx + 1]
        jne    SHORT loc_1_1EA3
        add    edi, 2
        add    ebx, 2
        or     al, al
        jne    SHORT loc_1_1E83
loc_1_1E9F:
        sub    eax, eax
        jmp    SHORT loc_1_1EA8
loc_1_1EA3:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1EA8:
        or     eax, eax
        je     SHORT loc_1_1EB9
        add    edx, 4
        or     esi, esi
        jne    SHORT loc_1_1E7C
loc_1_1EB3:
        sub    eax, eax
loc_1_1EB5:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1EB9:
        mov    eax, dword ptr [edx]
        jmp    SHORT loc_1_1EB5
loc_1_1EBD:
        push   esi
        push   dword ptr [esp + 8]
        call   sub_1_1E60
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1F2C
        mov    esi, dword ptr [eax + 24h]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, -24h
        je     SHORT loc_1_1EED
        mov    ecx, 0ah
loc_1_1EDF:
        mov    dword ptr [esi - 34h], ecx
        mov    esi, dword ptr [esi]
        mov    edx, eax
        sub    edx, esi
        cmp    edx, -24h
        jne    SHORT loc_1_1EDF
loc_1_1EED:
        mov    esi, dword ptr [eax + 2ch]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, -2ch
        je     SHORT loc_1_1F09
        sub    ecx, ecx
loc_1_1EFB:
        mov    dword ptr [esi + 10h], ecx
        mov    esi, dword ptr [esi]
        mov    edx, eax
        sub    edx, esi
        cmp    edx, -2ch
        jne    SHORT loc_1_1EFB
loc_1_1F09:
        mov    esi, dword ptr [eax + 34h]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, -34h
        je     SHORT loc_1_1F25
        sub    ecx, ecx
loc_1_1F17:
        mov    dword ptr [esi + 2ch], ecx
        mov    esi, dword ptr [esi]
        mov    edx, eax
        sub    edx, esi
        cmp    edx, -34h
        jne    SHORT loc_1_1F17
loc_1_1F25:
        mov    dword ptr [eax + 10h], 0
loc_1_1F2C:
        pop    esi
        ret
loc_1_1F2E:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   edi
        call   sub_1_1E60
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_1F4F
        push   edi
        call   sub_1_3AAF
        add    esp, 4
        mov    dword ptr [esi + 10h], eax
loc_1_1F4F:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1F60:
        push   edi
        push   48h
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_1F75
        sub    eax, eax
        jmp    SHORT loc_1_1FBA
loc_1_1F75:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 12h
        rep stosd
        lea    eax, [edx + 24h]
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 2ch]
        mov    dword ptr [edx + 30h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 34h]
        mov    dword ptr [edx + 38h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 3ch]
        mov    dword ptr [edx + 40h], eax
        mov    dword ptr [eax], eax
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     SHORT loc_1_1FB8
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
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
        mov    ecx, dword ptr [eax + 34h]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [edx + 0ch], eax
        ret
sub_1_1FF5:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi]
        lea    ecx, [edx + 8]
        mov    esi, dword ptr [eax + 74h]
        lea    eax, [edi + 24h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [eax], ecx
        lea    eax, [esi + 3ch]
        mov    dword ptr [edi + 28h], ecx
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [esi + 40h], edx
        pop    edi
        pop    esi
        ret
sub_1_202A:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        test   byte ptr [edx + 1ch], 1
        je     SHORT loc_1_203E
        lea    esi, [ecx + 3ch]
        jmp    SHORT loc_1_2041
loc_1_203E:
        lea    esi, [ecx + 34h]
loc_1_2041:
        lea    eax, [ecx + 2ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 30h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 30h], edx
        mov    dword ptr [edx + 8], esi
        lea    ecx, [edx + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 4], ecx
        pop    esi
        ret
sub_1_2064:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 24h]
        lea    esi, [ecx + 24h]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 24h
        jne    SHORT loc_1_207C
        sub    eax, eax
        jmp    SHORT loc_1_2090
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
        mov    eax, dword ptr [ecx + 2ch]
        lea    esi, [ecx + 2ch]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 2ch
        jne    SHORT loc_1_20AA
        sub    eax, eax
        jmp    SHORT loc_1_20BE
loc_1_20AA:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        mov    edx, dword ptr [eax + 0ch]
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
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 24h]
        sub    eax, edi
        cmp    eax, 24h
        je     SHORT loc_1_2101
        mov    ebx, dword ptr [edi]
loc_1_20D3:
        push   edi
        call   sub_1_2064
        add    esp, 4
        push   eax
        call   sub_1_3F46
        add    esp, 4
        mov    eax, dword ptr [edi + 24h]
        sub    eax, edi
        sub    eax, 24h
        cmp    eax, 1
        sbb    esi, esi
        neg    esi
        push   ebx
        call   sub_1_347B
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_20D3
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
        je     SHORT loc_1_2122
        push   eax
        call   sub_1_3F46
        add    esp, 4
        jmp    SHORT loc_1_210A
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
        mov    eax, dword ptr [esp + 14h]
        sub    edi, edi
        mov    esi, dword ptr [eax + 74h]
        mov    ebx, dword ptr [esp + 20h]
        and    ebx, 80000000h
        jne    SHORT loc_1_217B
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        call   sub_1_3AD8
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        cmp    eax, edi
        jne    SHORT loc_1_217B
        mov    esi, 271eh
        jmp    NEAR PTR loc_1_2211
loc_1_217B:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 2ch], 2
        jne    SHORT loc_1_21F3
        mov    ebp, dword ptr [esp + 24h]
        or     ebp, ebp
        je     SHORT loc_1_21C8
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_1_21C8
        mov    eax, dword ptr [esp + 28h]
        mov    edi, dword ptr [eax]
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, edi
        jae    SHORT loc_1_21A3
        mov    edi, eax
loc_1_21A3:
        cmp    dword ptr [esi + 14h], edi
        ja     SHORT loc_1_21BF
        push   edi
        push   ebp
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_21CE
        mov    esi, 271eh
        jmp    SHORT loc_1_2215
loc_1_21BF:
        sub    edi, edi
        mov    esi, 271eh
        jmp    SHORT loc_1_2215
loc_1_21C8:
        sub    ebp, ebp
        mov    dword ptr [esp + 28h], ebp
loc_1_21CE:
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   ebp
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        call   sub_1_2453
        add    esp, 1ch
        mov    esi, eax
        jmp    SHORT loc_1_2215
loc_1_21F3:
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        call   sub_1_223E
        add    esp, 14h
        mov    esi, eax
loc_1_2211:
        mov    ebp, dword ptr [esp + 24h]
loc_1_2215:
        or     ebx, ebx
        jne    SHORT loc_1_2229
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        call   sub_1_3B1F
        add    esp, 8
loc_1_2229:
        or     edi, edi
        je     SHORT loc_1_2237
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
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 2ch], 0
        mov    esi, dword ptr [esp + 38h]
        mov    dword ptr [esp + 18h], 0
        mov    eax, dword ptr [esi + 74h]
        mov    dword ptr [esp + 30h], eax
        mov    edi, dword ptr [esi + 98h]
        mov    ebx, dword ptr [esp + 44h]
        mov    eax, ebx
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 1ch], eax
        push   edi
        push   esi
        call   sub_1_254D
        add    esp, 8
        and    ebx, 1
        mov    dword ptr [esp + 24h], ebx
        or     ebx, ebx
        jne    SHORT loc_1_2298
        mov    eax, dword ptr [edi + 34h]
        sub    eax, edi
        cmp    eax, 34h
        je     SHORT loc_1_22A9
loc_1_2298:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_22C0
        mov    eax, dword ptr [edi + 3ch]
        sub    eax, edi
        cmp    eax, 3ch
        jne    SHORT loc_1_22C0
loc_1_22A9:
        mov    eax, dword ptr [esi + 94h]
        or     eax, eax
        je     NEAR PTR loc_1_2386
        test   byte ptr [eax + 10h], 1
        jmp    NEAR PTR loc_1_2384
loc_1_22C0:
        mov    eax, dword ptr [esi + 40h]
        and    eax, 10h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 20h], eax
        or     eax, eax
        je     SHORT loc_1_22E9
        lea    eax, [edi + 2ch]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 1ch]
        and    eax, 1
        mov    dword ptr [esp + 28h], eax
        jmp    SHORT loc_1_22FC
loc_1_22E9:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_22F5
        lea    eax, [edi + 3ch]
        jmp    SHORT loc_1_22F8
loc_1_22F5:
        lea    eax, [edi + 34h]
loc_1_22F8:
        mov    dword ptr [esp + 14h], eax
loc_1_22FC:
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, dword ptr [eax]
        cmp    eax, ebx
        je     SHORT loc_1_237F
loc_1_2306:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_1_231D
        mov    ebp, ebx
        mov    eax, dword ptr [ebx + 1ch]
        and    eax, 1
        cmp    eax, dword ptr [esp + 28h]
        je     SHORT loc_1_2320
        jmp    SHORT loc_1_237F
loc_1_231D:
        lea    ebp, [ebx - 8]
loc_1_2320:
        push   dword ptr [esp + 1ch]
        mov    ebx, dword ptr [ebx]
        push   dword ptr [esp + 44h]
        push   ebp
        push   dword ptr [esp + 48h]
        call   sub_1_3F6F
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        mov    eax, dword ptr [ebp + 20h]
        cmp    dword ptr [ebp + 28h], eax
        jne    SHORT loc_1_2367
        sub    dword ptr [edi + 10h], eax
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_3F46
        add    esp, 4
loc_1_2367:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 18h], eax
        sub    dword ptr [esp + 40h], eax
        je     SHORT loc_1_237F
        add    dword ptr [esp + 3ch], eax
        cmp    dword ptr [esp + 14h], ebx
        jne    SHORT loc_1_2306
loc_1_237F:
        cmp    dword ptr [esp + 18h], 0
loc_1_2384:
        jne    SHORT loc_1_238E
loc_1_2386:
        mov    dword ptr [esp + 2ch], 2733h
loc_1_238E:
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_23D3
        push   dword ptr [esp + 30h]
        call   sub_1_29B3
        add    esp, 4
        push   edi
        push   esi
        call   sub_1_254D
        add    esp, 8
        mov    eax, dword ptr [esi + 94h]
        test   byte ptr [eax + 1fh], 10h
        jne    SHORT loc_1_23D3
        mov    eax, dword ptr [edi + 3ch]
        sub    eax, edi
        cmp    eax, 3ch
        jne    SHORT loc_1_23C5
        and    dword ptr [esi + 50h], 0fffffffbh
loc_1_23C5:
        mov    eax, dword ptr [edi + 34h]
        sub    eax, edi
        cmp    eax, 34h
        jne    SHORT loc_1_23D3
        and    dword ptr [esi + 50h], 0fffffffeh
loc_1_23D3:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_243D
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_23EA
        mov    eax, dword ptr [edi + 3ch]
        sub    eax, edi
        sub    eax, 3ch
        jmp    SHORT loc_1_23F2
loc_1_23EA:
        mov    eax, dword ptr [edi + 34h]
        sub    eax, edi
        sub    eax, 34h
loc_1_23F2:
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        je     SHORT loc_1_2413
        cmp    dword ptr [esp + 24h], 1
        sbb    eax, eax
        and    eax, 0fffffffdh
        add    eax, 4
        jmp    SHORT loc_1_2426
loc_1_2413:
        mov    eax, dword ptr [esi + 94h]
        test   byte ptr [eax + 10h], 1
        mov    eax, 20h
        jne    SHORT loc_1_2426
        sub    eax, eax
loc_1_2426:
        or     eax, eax
        je     SHORT loc_1_243D
        mov    ecx, eax
        not    ecx
        and    dword ptr [esi + 50h], ecx
        push   0
        push   eax
        push   esi
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_243D:
        mov    eax, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 2ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
sub_1_2453:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 10h], 0
        mov    eax, dword ptr [esi + 74h]
        mov    edi, dword ptr [esi + 98h]
        mov    dword ptr [esp + 14h], eax
        mov    ebx, dword ptr [edi + 2ch]
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, 2ch
        je     NEAR PTR loc_1_2518
        mov    eax, dword ptr [esp + 2ch]
        or     eax, eax
        je     SHORT loc_1_24B3
        mov    ecx, dword ptr [ebx + 2ch]
        movzx  edx, word ptr [ecx + 4]
        add    edx, 2
        push   edx
        add    ecx, 6
        push   ecx
        push   eax
        call   0
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 2ch]
        movzx  ecx, word ptr [eax + 4]
        mov    eax, dword ptr [esp + 30h]
        add    ecx, 2
        mov    dword ptr [eax], ecx
loc_1_24B3:
        mov    ebp, dword ptr [esp + 28h]
        and    ebp, 2
        push   ebp
        push   dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [esp + 2ch]
        call   sub_1_3F6F
        add    esp, 10h
        mov    eax, dword ptr [ebx + 20h]
        mov    ecx, dword ptr [esp + 34h]
        or     ebp, ebp
        mov    dword ptr [ecx], eax
        jne    SHORT loc_1_2520
        sub    dword ptr [edi + 10h], eax
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 0ch]
        lea    eax, [ebx + 8]
        push   ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_3F46
        add    esp, 4
        push   dword ptr [esp + 14h]
        call   sub_1_29B3
        add    esp, 4
        mov    eax, dword ptr [edi + 2ch]
        sub    eax, edi
        cmp    eax, 2ch
        jne    SHORT loc_1_2520
        and    dword ptr [esi + 50h], 0fffffffeh
        jmp    SHORT loc_1_2520
loc_1_2518:
        mov    dword ptr [esp + 10h], 2733h
loc_1_2520:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_2541
        mov    eax, dword ptr [edi + 2ch]
        sub    eax, edi
        cmp    eax, 2ch
        je     SHORT loc_1_2541
        and    dword ptr [esi + 50h], 0fffffffeh
        push   0
        push   1
        push   esi
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_2541:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_254D:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 38h]
        mov    eax, dword ptr [esi + 74h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax + 1bh], 40h
        je     NEAR PTR loc_1_2662
        mov    edi, dword ptr [esp + 3ch]
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esp + 18h], eax
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_2590
        or     eax, eax
        je     NEAR PTR loc_1_2662
loc_1_2590:
        mov    eax, dword ptr [edi + 10h]
        cmp    dword ptr [edi + 0ch], eax
        jb     NEAR PTR loc_1_2662
        sub    ebx, ebx
loc_1_259E:
        or     ebx, ebx
        je     SHORT loc_1_25A8
        mov    eax, dword ptr [esp + 1ch]
        jmp    SHORT loc_1_25AC
loc_1_25A8:
        mov    eax, dword ptr [esp + 18h]
loc_1_25AC:
        mov    dword ptr [esp + 10h], eax
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_2658
        push   dword ptr [esp + 10h]
        call   sub_1_3F0F
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_2662
        mov    dword ptr [ebp + 18h], esi
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        or     dword ptr [ebp + 1ch], eax
        db  0C7h, 044h, 024h, 028h, 06Ah, 026h, 000h, 000h
        mov    dword ptr [esp + 2ch], ebp
        mov    eax, dword ptr [ebp + 24h]
        lea    ecx, [edi + 18h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 94h]
        inc    dword ptr [esi + 88h]
        or     ebx, ebx
        mov    eax, dword ptr [eax + 30h]
        mov    dword ptr [esp + 24h], eax
        jne    SHORT loc_1_260E
        lea    ecx, [edi + 20h]
loc_1_260E:
        lea    eax, [ebp + 5ch]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 2ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 30h]
        mov    eax, dword ptr [eax + 10h]
        call   dword ptr [eax + 28h]
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_2649
        cmp    eax, 0ffh
        je     SHORT loc_1_2658
        push   ebp
        call   sub_1_3F46
        add    esp, 4
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    SHORT loc_1_2658
loc_1_2649:
        push   dword ptr [esp + 10h]
        push   0
        push   ebp
        call   sub_1_266A
        add    esp, 0ch
loc_1_2658:
        inc    ebx
        cmp    ebx, 1
        jle    NEAR PTR loc_1_259E
loc_1_2662:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
sub_1_266A:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 1
        mov    edi, dword ptr [esi + 98h]
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0fffffffdh
        add    ebx, 4
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_26D5
        or     eax, eax
        je     SHORT loc_1_26A7
        mov    dword ptr [edi + 1ch], 0
        mov    dword ptr [edi + 20h], 0
        jmp    SHORT loc_1_26B5
loc_1_26A7:
        mov    dword ptr [edi + 14h], 0
        mov    dword ptr [edi + 18h], 0
loc_1_26B5:
        push   dword ptr [esp + 1ch]
        push   ebp
        call   sub_1_3FF1
        add    esp, 8
        push   ebp
        push   edi
        call   sub_1_202A
        mov    eax, dword ptr [esp + 24h]
        add    esp, 8
        add    dword ptr [edi + 10h], eax
        jmp    SHORT loc_1_26E2
loc_1_26D5:
        or     dword ptr [esi + 3ch], 1
        push   ebp
        call   sub_1_3F46
        add    esp, 4
loc_1_26E2:
        push   dword ptr [esp + 18h]
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   ebx
        push   esi
        call   sub_1_3BCC
        add    esp, 0ch
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
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [edi + 74h]
        cmp    dword ptr [edi + 2ch], 2
        jne    NEAR PTR loc_1_287B
        mov    ecx, dword ptr [esp + 30h]
        or     ecx, ecx
        jne    NEAR PTR loc_1_27D5
        cmp    dword ptr [edi + 94h], 0
        je     NEAR PTR loc_1_27CB
        mov    ecx, dword ptr [edi + 0b0h]
        mov    dword ptr [ecx], 1
        mov    eax, dword ptr [edi + 24h]
        push   eax
        lea    edx, [eax - 2]
        mov    word ptr [ecx + 4], dx
        push   dword ptr [edi + 18h]
        add    ecx, 6
        push   ecx
        call   0
        add    esp, 0ch
loc_1_2767:
        mov    dword ptr [edi + 0b8h], 1
        cmp    dword ptr [edi + 4ch], 1
        jne    NEAR PTR loc_1_289A
        mov    ebx, dword ptr [edi + 0b4h]
        add    ebx, 6
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [edi + 7ch]
        call   dword ptr [eax + 4]
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        jne    NEAR PTR loc_1_29A9
        push   dword ptr [esp + 10h]
        push   ebx
        push   edi
        call   sub_1_0360
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        jne    NEAR PTR loc_1_29A9
        mov    dword ptr [edi + 54h], 0
        jmp    NEAR PTR loc_1_289A
loc_1_27CB:
        mov    ebp, 2749h
        jmp    NEAR PTR loc_1_29A9
loc_1_27D5:
        mov    eax, dword ptr [esi + 18h]
        mov    ebx, dword ptr [esp + 34h]
        cmp    ebx, eax
        jbe    SHORT loc_1_27E2
        mov    ebx, eax
loc_1_27E2:
        cmp    dword ptr [esi + 14h], ebx
        ja     NEAR PTR loc_1_2871
        push   ebx
        push   ecx
        call   sub_1_3AD8
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_2805
        mov    ebp, 271eh
        jmp    NEAR PTR loc_1_29A9
loc_1_2805:
        mov    eax, dword ptr [edi + 0b0h]
        lea    ecx, [ebx - 2]
        mov    dword ptr [eax], 1
        mov    word ptr [eax + 4], cx
        push   ebx
        push   ebp
        add    eax, 6
        push   eax
        call   0
        add    esp, 0ch
        push   ebx
        push   ebp
        call   sub_1_3B1F
        add    esp, 8
        test   byte ptr [edi + 40h], 1
        jne    NEAR PTR loc_1_2767
        lea    eax, [esp + 14h]
        push   eax
        push   ebx
        mov    eax, dword ptr [edi + 0b0h]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        call   dword ptr [eax]
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        jne    NEAR PTR loc_1_29A9
        cmp    dword ptr [esp + 14h], 2
        jne    NEAR PTR loc_1_2767
        mov    ebp, 271dh
        jmp    NEAR PTR loc_1_29A9
loc_1_2871:
        mov    ebp, 271eh
        jmp    NEAR PTR loc_1_29A9
loc_1_287B:
        mov    dword ptr [edi + 0b8h], 0
        mov    ebx, dword ptr [esp + 28h]
        or     ebx, ebx
        jne    SHORT loc_1_289E
        mov    ecx, dword ptr [esp + 38h]
        sub    ebp, ebp
        mov    dword ptr [ecx], ebp
        jmp    NEAR PTR loc_1_29A9
loc_1_289A:
        mov    ebx, dword ptr [esp + 28h]
loc_1_289E:
        push   ebx
        push   dword ptr [esp + 28h]
        call   sub_1_3AD8
        mov    dword ptr [esp + 2ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_28BD
        mov    ebp, 271eh
        jmp    NEAR PTR loc_1_29A9
loc_1_28BD:
        mov    eax, dword ptr [edi + 98h]
        test   byte ptr [edi + 41h], 1
        jne    SHORT loc_1_28D6
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], ecx
        mov    eax, 0
        ja     SHORT loc_1_28DF
loc_1_28D6:
        push   ebx
        call   sub_1_3F0F
        add    esp, 4
loc_1_28DF:
        or     eax, eax
        je     SHORT loc_1_2910
        push   0
        push   dword ptr [esp + 3ch]
        push   eax
        push   dword ptr [esp + 38h]
        push   ebx
        push   dword ptr [esp + 38h]
        push   edi
        call   sub_1_2A95
        add    esp, 1ch
        push   ebx
        mov    ebp, eax
        push   dword ptr [esp + 28h]
        call   sub_1_3B1F
        add    esp, 8
        jmp    NEAR PTR loc_1_29A9
loc_1_2910:
        and    dword ptr [edi + 50h], 0fffffffdh
        test   byte ptr [edi + 41h], 1
        je     SHORT loc_1_2956
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 0a4h], eax
        mov    dword ptr [edi + 0a8h], ebx
        mov    eax, dword ptr [esp + 2ch]
        lea    ecx, [edi + 9ch]
        mov    dword ptr [edi + 0ach], eax
        or     dword ptr [edi + 8ch], 2
        lea    eax, [esi + 48h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 4ch]
        mov    dword ptr [edi + 0a0h], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 4ch], ecx
        jmp    SHORT loc_1_299E
loc_1_2956:
        mov    eax, dword ptr [edi + 8ch]
        test   al, 4
        jne    SHORT loc_1_2991
        or     eax, 4
        push   ebx
        mov    dword ptr [edi + 8ch], eax
        push   dword ptr [esp + 28h]
        call   sub_1_3B1F
        add    esp, 8
        lea    ecx, [edi + 9ch]
        lea    eax, [esi + 48h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 4ch]
        mov    dword ptr [edi + 0a0h], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 4ch], ecx
        jmp    SHORT loc_1_299E
loc_1_2991:
        push   ebx
        push   dword ptr [esp + 28h]
        call   sub_1_3B1F
        add    esp, 8
loc_1_299E:
        mov    ecx, dword ptr [esp + 38h]
        mov    ebp, 2733h
        mov    dword ptr [ecx], ebx
loc_1_29A9:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_29B3:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    eax, esi
        mov    edi, dword ptr [esi + 48h]
        sub    eax, edi
        cmp    eax, -48h
        je     NEAR PTR loc_1_2A8E
loc_1_29CD:
        lea    ebx, [edi - 9ch]
        mov    edi, dword ptr [edi]
        test   byte ptr [ebx + 8ch], 4
        je     SHORT loc_1_2A0B
        push   0
        push   2
        push   ebx
        call   sub_1_3BCC
        add    esp, 0ch
        and    dword ptr [ebx + 8ch], 0fffffffbh
        mov    ecx, dword ptr [ebx + 0a0h]
        mov    eax, dword ptr [ebx + 9ch]
        add    ebx, 9ch
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        jmp    SHORT loc_1_2A81
loc_1_2A0B:
        mov    eax, dword ptr [ebx + 98h]
        mov    ecx, dword ptr [eax + 8]
        cmp    dword ptr [eax + 4], ecx
        mov    eax, 0
        jb     SHORT loc_1_2A2C
        push   dword ptr [ebx + 0a8h]
        call   sub_1_3F0F
        add    esp, 4
loc_1_2A2C:
        or     eax, eax
        je     SHORT loc_1_2A81
        mov    edx, dword ptr [ebx + 0a0h]
        lea    ecx, [ebx + 9ch]
        push   1
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        lea    ecx, [esp + 10h]
        and    dword ptr [ebx + 8ch], 0fffffffdh
        push   ecx
        push   eax
        push   dword ptr [ebx + 0ach]
        push   dword ptr [ebx + 0a8h]
        push   dword ptr [ebx + 0a4h]
        push   ebx
        call   sub_1_2A95
        add    esp, 1ch
        push   dword ptr [ebx + 0a8h]
        push   dword ptr [ebx + 0a4h]
        call   sub_1_3B1F
        add    esp, 8
loc_1_2A81:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -48h
        jne    NEAR PTR loc_1_29CD
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
        mov    esi, dword ptr [esp + 2ch]
        mov    ebx, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 18h], ebx
        sub    edi, edi
        push   dword ptr [esp + 28h]
        call   sub_1_3838
        add    esp, 4
        or     dword ptr [esi + 1ch], eax
        cmp    dword ptr [ebx + 0b8h], edi
        je     SHORT loc_1_2AFE
        mov    edx, dword ptr [esi + 2ch]
        mov    dword ptr [edx], 1
        mov    ecx, dword ptr [ebx + 0b0h]
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
        add    esp, 0ch
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [esi + 44h], eax
        movzx  eax, word ptr [eax + 4]
        add    eax, 8
        mov    dword ptr [esi + 40h], eax
loc_1_2AFE:
        mov    ecx, dword ptr [ebx + 98h]
        mov    edx, dword ptr [esp + 24h]
        mov    eax, dword ptr [ecx + 8]
        add    eax, edx
        mov    dword ptr [ecx + 8], eax
        mov    ebp, dword ptr [esp + 34h]
        or     ebp, ebp
        jne    SHORT loc_1_2B33
        test   byte ptr [ebx + 41h], 1
        je     SHORT loc_1_2B33
        cmp    dword ptr [ecx + 4], eax
        jae    SHORT loc_1_2B33
        or     dword ptr [ebx + 8ch], 8
        and    dword ptr [ebx + 50h], 0fffffffdh
        mov    edi, 2733h
loc_1_2B33:
        push   edx
        push   dword ptr [esp + 24h]
        push   esi
        call   sub_1_3FB1
        lea    eax, [esp + 20h]
        add    esp, 0ch
        push   eax
        lea    eax, [esp + 14h]
        push   1
        push   eax
        push   esi
        call   sub_1_2B89
        mov    ecx, dword ptr [esp + 40h]
        add    esp, 10h
        mov    dword ptr [ecx], eax
        cmp    eax, -1
        jne    SHORT loc_1_2B6F
        push   dword ptr [esp + 10h]
        call   sub_1_3720
        add    esp, 4
        mov    edi, eax
loc_1_2B6F:
        or     ebp, ebp
        je     SHORT loc_1_2B7F
        push   edi
        push   2
        push   ebx
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_2B7F:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_2B89:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 30h]
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [esi + 74h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esi + 98h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 3ch]
        inc    dword ptr [esi + 88h]
        mov    dword ptr [eax], 0
        db  0C7h, 044h, 024h, 020h, 0D7h, 02Ch, 000h, 000h
        mov    dword ptr [esp + 24h], edi
        mov    ebx, dword ptr [edi + 20h]
        db  0A1h, 030h, 042h, 000h, 000h
        inc    eax
        db  0A3h, 030h, 042h, 000h, 000h
        cmp    dword ptr [esi + 2ch], 2
        jne    SHORT loc_1_2C07
        mov    eax, dword ptr [esi + 90h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [edi + 5ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ebx
        lea    eax, [edi + 30h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        mov    eax, dword ptr [eax + 10h]
        call   dword ptr [eax + 30h]
        add    esp, 14h
        jmp    SHORT loc_1_2C2E
loc_1_2C07:
        mov    eax, dword ptr [esi + 94h]
        mov    eax, dword ptr [eax + 30h]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [edi + 5ch]
        push   eax
        push   ebx
        lea    eax, [esp + 24h]
        push   dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 10h]
        call   dword ptr [eax + 2ch]
        add    esp, 10h
loc_1_2C2E:
        mov    ebp, eax
        mov    eax, dword ptr [esp + 34h]
        cmp    ebp, 0ffh
        mov    dword ptr [eax], ebp
        db  0A1h, 030h, 042h, 000h, 000h
        je     SHORT loc_1_2C44
        dec    eax
loc_1_2C44:
        db  0A3h, 030h, 042h, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_2C6A
        cmp    ebp, 1
        je     SHORT loc_1_2C75
        cmp    ebp, 0ffh
        je     SHORT loc_1_2CA3
        mov    ebx, 0ffffffffh
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    SHORT loc_1_2CA3
loc_1_2C6A:
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    SHORT loc_1_2CA3
loc_1_2C75:
        db  0A3h, 030h, 042h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_2C85
        mov    ebx, 0ffffffffh
        jmp    SHORT loc_1_2CA3
loc_1_2C85:
        mov    eax, dword ptr [esp + 3ch]
        cmp    dword ptr [esp + 38h], 0
        mov    dword ptr [eax], 1
        je     SHORT loc_1_2CA3
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_1_1FF5
        add    esp, 8
loc_1_2CA3:
        or     ebp, ebp
        je     SHORT loc_1_2CAC
        cmp    ebx, -1
        jne    SHORT loc_1_2CCD
loc_1_2CAC:
        mov    eax, dword ptr [esi + 98h]
        mov    ecx, dword ptr [edi + 20h]
        sub    dword ptr [eax + 8], ecx
        push   edi
        call   sub_1_3F46
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_1_29B3
        add    esp, 4
loc_1_2CCD:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_2CD7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 18h]
        mov    ebp, dword ptr [edi + 74h]
        db  0FFh, 00Dh, 030h, 042h, 000h, 000h
        push   ebp
        call   sub_1_2D6F
        add    esp, 4
        mov    ebx, dword ptr [edi + 98h]
        mov    eax, dword ptr [esi + 20h]
        sub    dword ptr [ebx + 8], eax
        push   esi
        call   sub_1_3F46
        add    esp, 4
        push   ebp
        call   sub_1_29B3
        mov    esi, dword ptr [esp + 1ch]
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_2D31
        mov    eax, dword ptr [edi + 94h]
        or     eax, eax
        je     SHORT loc_1_2D31
        push   eax
        call   sub_1_0C90
        add    esp, 4
        or     dword ptr [edi + 3ch], 1
loc_1_2D31:
        mov    eax, dword ptr [edi + 8ch]
        test   al, 8
        je     SHORT loc_1_2D61
        mov    ecx, dword ptr [ebx + 8]
        cmp    dword ptr [ebx + 4], ecx
        jb     SHORT loc_1_2D61
        and    eax, 0fffffff7h
        mov    dword ptr [edi + 8ch], eax
        push   esi
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
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
        mov    ebx, dword ptr [esp + 1ch]
        lea    edi, [ebx + 3ch]
loc_1_2D7D:
        mov    esi, dword ptr [edi]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 3ch
        je     SHORT loc_1_2DF1
        mov    eax, dword ptr [esi]
        lea    ecx, [esi + 8]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 10h]
        mov    ebp, dword ptr [esi + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   0
        push   eax
        push   esi
        call   sub_1_2B89
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_1_2DE8
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_2D7D
        mov    eax, dword ptr [esi + 18h]
        lea    ecx, [esi + 8]
        mov    edx, dword ptr [eax + 98h]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edi
        add    edx, 24h
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edi], esi
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0ch], edx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_1_2DF1
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
        mov    ebx, dword ptr [esp + 18h]
        cmp    ebx, 2
        jbe    SHORT loc_1_2E17
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_2EC5
loc_1_2E17:
        mov    esi, dword ptr [esp + 14h]
        db  0FFh, 034h, 09Dh, 0C0h, 03Ch, 000h, 000h
        push   esi
        call   sub_1_3250
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_2E36
        jmp    NEAR PTR loc_1_2EC3
loc_1_2E36:
        cmp    ebx, 1
        je     SHORT loc_1_2E40
        cmp    ebx, 2
        jne    SHORT loc_1_2E82
loc_1_2E40:
        mov    ecx, dword ptr [esi + 94h]
        or     ecx, ecx
        je     SHORT loc_1_2E82
        mov    edx, dword ptr [ecx + 1ch]
        mov    eax, dword ptr [ecx + 10h]
        mov    ebp, eax
        and    ebp, 1
        cmp    ebp, 1
        sbb    ebp, ebp
        and    ebp, 0fffffffh
        add    ebp, 10000007h
        mov    dword ptr [ecx + 1ch], ebp
        test   edx, 40000000h
        je     SHORT loc_1_2E82
        or     eax, 4
        mov    dword ptr [ecx + 10h], eax
        push   0
        push   ecx
        call   sub_1_0CB8
        add    esp, 8
loc_1_2E82:
        or     ebx, ebx
        je     SHORT loc_1_2E8B
        cmp    ebx, 2
        jne    SHORT loc_1_2EC3
loc_1_2E8B:
        cmp    dword ptr [esi + 2ch], 1
        jne    SHORT loc_1_2EC3
        mov    eax, dword ptr [esi + 98h]
        or     eax, eax
        je     SHORT loc_1_2EC3
        mov    ecx, dword ptr [eax + 2ch]
        sub    ecx, eax
        cmp    ecx, 2ch
        je     SHORT loc_1_2EC3
        push   dword ptr [esi + 94h]
        call   sub_1_0C90
        add    esp, 4
        push   2745h
        push   20h
        push   esi
        call   sub_1_3BCC
        add    esp, 0ch
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
        sub    ecx, 50h
        cmp    dword ptr [ecx + 10h], 0
        je     SHORT loc_1_2EF6
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_1_32A1
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_2EF8
loc_1_2EF6:
        sub    eax, eax
loc_1_2EF8:
        ret
loc_1_2EF9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 90h]
        or     eax, eax
        je     SHORT loc_1_2F14
        cmp    dword ptr [eax + 1ch], esi
        jne    SHORT loc_1_2F14
        mov    dword ptr [eax + 1ch], 0
loc_1_2F14:
        cmp    word ptr [esi + 10h], 0
        je     SHORT loc_1_2F22
        cmp    word ptr [esi + 12h], 0
        je     SHORT loc_1_2F36
loc_1_2F22:
        mov    eax, dword ptr [esi + 98h]
        or     eax, eax
        je     SHORT loc_1_2F52
        mov    ecx, dword ptr [eax + 2ch]
        sub    ecx, eax
        cmp    ecx, 2ch
        je     SHORT loc_1_2F52
loc_1_2F36:
        push   esi
        call   sub_1_35BC
        add    esp, 4
        mov    eax, dword ptr [esi + 94h]
        or     eax, eax
        je     SHORT loc_1_2F52
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
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esp + 2ch]
        mov    edx, ebx
        mov    ebp, dword ptr [esp + 30h]
        inc    dword ptr [esi + 88h]
        mov    eax, dword ptr [ebp]
        cmp    eax, 4
        mov    dword ptr [esp + 10h], eax
        jae    SHORT loc_1_2FA3
        lea    edx, [esp + 14h]
        mov    dword ptr [esp + 10h], 4
loc_1_2FA3:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 18h], eax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, dword ptr [esp + 18h]
        shr    ecx, 2
        rep stosd
        mov    ecx, dword ptr [esp + 18h]
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [esi + 90h]
        or     eax, eax
        mov    ecx, 0
        je     SHORT loc_1_2FD3
        mov    ecx, dword ptr [eax + 14h]
loc_1_2FD3:
        mov    eax, dword ptr [esi + 94h]
        or     eax, eax
        mov    edi, 0
        je     SHORT loc_1_2FE5
        mov    edi, dword ptr [eax + 30h]
loc_1_2FE5:
        lea    eax, [esp + 10h]
        push   eax
        push   edx
        mov    eax, dword ptr [esi + 74h]
        push   dword ptr [esp + 30h]
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [esp + 30h]
        push   edi
        push   ecx
        push   esi
        push   dword ptr [esi + 7ch]
        call   dword ptr [eax + 8]
        add    esp, 20h
        mov    edi, eax
        mov    eax, dword ptr [ebp]
        cmp    eax, 4
        jae    SHORT loc_1_3033
        cmp    eax, 2
        jae    SHORT loc_1_3024
        mov    al, byte ptr [esp + 14h]
        mov    byte ptr [ebx], al
        mov    dword ptr [esp + 10h], 1
        jmp    SHORT loc_1_3033
loc_1_3024:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [ebx], ax
        mov    dword ptr [esp + 10h], 2
loc_1_3033:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebp], eax
        push   esi
        call   sub_1_347B
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_304D:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    ebx, dword ptr [esp + 1ch]
        mov    edx, 1
        add    dword ptr [ebx + 88h], edx
        cmp    dword ptr [esp + 2ch], 4
        jb     SHORT loc_1_307F
        mov    ebp, dword ptr [esp + 28h]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 10h], eax
        jmp    SHORT loc_1_30A4
loc_1_307F:
        cmp    dword ptr [esp + 2ch], 2
        mov    ebp, dword ptr [esp + 28h]
        jb     SHORT loc_1_3090
        movzx  eax, word ptr [ebp]
        jmp    SHORT loc_1_3094
loc_1_3090:
        movzx  eax, byte ptr [ebp]
loc_1_3094:
        mov    dword ptr [esp + 10h], eax
        lea    ebp, [esp + 10h]
        mov    dword ptr [esp + 2ch], 4
loc_1_30A4:
        cmp    dword ptr [esp + 20h], 0ffffh
        jne    NEAR PTR loc_1_31C5
        mov    eax, dword ptr [esp + 24h]
        cmp    eax, 1001h
        ja     SHORT loc_1_30E5
        je     NEAR PTR loc_1_3165
        cmp    eax, 1
        je     SHORT loc_1_3114
        cmp    eax, 4
        je     SHORT loc_1_3114
        cmp    eax, 20h
        je     SHORT loc_1_3114
        cmp    eax, 80h
        je     SHORT loc_1_3114
        cmp    eax, 100h
        je     SHORT loc_1_3123
        jmp    NEAR PTR loc_1_31C5
loc_1_30E5:
        cmp    eax, 1002h
        je     NEAR PTR loc_1_317D
        cmp    eax, 1005h
        jb     NEAR PTR loc_1_31C5
        cmp    eax, 1006h
        jbe    SHORT loc_1_3114
        cmp    eax, 0ff7fh
        je     SHORT loc_1_3114
        cmp    eax, 0ffffff7fh
        jne    NEAR PTR loc_1_31C5
loc_1_3114:
        mov    dword ptr [esp + 14h], 0
        sub    edx, edx
        jmp    NEAR PTR loc_1_31C5
loc_1_3123:
        cmp    dword ptr [ebx + 2ch], 2
        jne    SHORT loc_1_3133
        mov    dword ptr [esp + 14h], 273ah
        jmp    SHORT loc_1_3161
loc_1_3133:
        cmp    dword ptr [ebx + 94h], 0
        je     SHORT loc_1_3161
        push   dword ptr [esp + 2ch]
        push   ebp
        call   sub_1_3224
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   3
        push   dword ptr [ebx + 94h]
        call   sub_1_0B97
        add    esp, 0ch
loc_1_3161:
        sub    edx, edx
        jmp    SHORT loc_1_31C5
loc_1_3165:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 34h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ebx + 98h]
        sub    edx, edx
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_1_31C5
loc_1_317D:
        cmp    dword ptr [esp + 10h], 0ff00h
        jbe    SHORT loc_1_31B1
        mov    dword ptr [esp + 10h], 0ff00h
        mov    eax, dword ptr [esp + 2ch]
        cmp    eax, 4
        jb     SHORT loc_1_319D
        mov    eax, 4
loc_1_319D:
        lea    esi, [esp + 10h]
        mov    edi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_31B1:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 38h], eax
        mov    ecx, dword ptr [ebx + 98h]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
loc_1_31C5:
        or     edx, edx
        je     SHORT loc_1_320F
        mov    eax, dword ptr [ebx + 90h]
        or     eax, eax
        mov    ecx, 0
        je     SHORT loc_1_31DB
        mov    ecx, dword ptr [eax + 14h]
loc_1_31DB:
        mov    eax, dword ptr [ebx + 94h]
        or     eax, eax
        jne    SHORT loc_1_31E9
        sub    eax, eax
        jmp    SHORT loc_1_31EC
loc_1_31E9:
        mov    eax, dword ptr [eax + 30h]
loc_1_31EC:
        push   dword ptr [esp + 2ch]
        push   ebp
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   eax
        push   ecx
        push   ebx
        mov    eax, dword ptr [ebx + 74h]
        push   dword ptr [ebx + 7ch]
        mov    eax, dword ptr [eax + 0ch]
        call   dword ptr [eax + 18h]
        mov    dword ptr [esp + 34h], eax
        add    esp, 20h
loc_1_320F:
        push   ebx
        call   sub_1_347B
        mov    eax, dword ptr [esp + 18h]
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
        je     SHORT loc_1_323F
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_322C
        sub    eax, eax
        jmp    SHORT loc_1_3244
loc_1_323F:
        mov    eax, 1
loc_1_3244:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3250:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        sub    eax, eax
        mov    esi, dword ptr [ecx + 7ch]
        cmp    esi, eax
        je     SHORT loc_1_329D
        mov    edx, dword ptr [esp + 14h]
        test   dword ptr [ecx + 78h], edx
        je     SHORT loc_1_329D
        mov    edi, dword ptr [ecx + 74h]
        mov    eax, dword ptr [ecx + 90h]
        or     eax, eax
        mov    ebx, 0
        je     SHORT loc_1_327E
        mov    ebx, dword ptr [eax + 14h]
loc_1_327E:
        mov    eax, dword ptr [ecx + 94h]
        or     eax, eax
        jne    SHORT loc_1_328C
        sub    eax, eax
        jmp    SHORT loc_1_328F
loc_1_328C:
        mov    eax, dword ptr [eax + 30h]
loc_1_328F:
        push   edx
        push   eax
        push   ebx
        push   ecx
        mov    eax, dword ptr [edi + 0ch]
        push   esi
        call   dword ptr [eax + 10h]
        add    esp, 14h
loc_1_329D:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_32A1:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 38h]
        mov    eax, dword ptr [esi + 20h]
        lea    eax, [eax*2 + 0c4h]
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_32CC
        sub    eax, eax
        jmp    NEAR PTR loc_1_336C
loc_1_32CC:
        mov    edx, dword ptr [esi + 20h]
        lea    ebp, [edx*2 + 0c4h]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    dword ptr [ebx + 74h], esi
        mov    dword ptr [ebx + 38h], 2000h
        mov    dword ptr [ebx + 34h], 2000h
        lea    eax, [ebx + 0c4h]
        mov    dword ptr [ebx + 0b0h], eax
        lea    eax, [edx + ebx]
        add    eax, 0c4h
        mov    dword ptr [ebx + 0b4h], eax
        lea    ecx, [ebx + 80h]
        lea    eax, [esi + 24h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebx + 84h], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 28h], ecx
        call   sub_1_3D30
        lea    eax, [ebx + 78h]
        push   eax
        lea    eax, [ebx + 7ch]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 50h]
        call   dword ptr [eax + 14h]
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_3364
        push   ebx
        call   sub_1_3374
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_336C
loc_1_3364:
        inc    dword ptr [ebx + 88h]
        mov    eax, ebx
loc_1_336C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
sub_1_3374:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 8ch]
        test   al, 2
        je     SHORT loc_1_33A1
        and    eax, 0fffffffdh
        mov    dword ptr [edi + 8ch], eax
        push   dword ptr [edi + 0a8h]
        push   dword ptr [edi + 0a4h]
        call   sub_1_3B1F
        add    esp, 8
loc_1_33A1:
        mov    dword ptr [edi + 98h], 0
        mov    esi, dword ptr [edi + 94h]
        or     esi, esi
        je     SHORT loc_1_33D3
        push   esi
        call   sub_1_09FC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_33C9
        mov    dword ptr [esi + 14h], 0
loc_1_33C9:
        mov    dword ptr [edi + 94h], 0
loc_1_33D3:
        mov    esi, dword ptr [edi + 90h]
        or     esi, esi
        je     SHORT loc_1_3400
        push   esi
        call   sub_1_0FBF
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_33F6
        cmp    dword ptr [esi + 1ch], edi
        jne    SHORT loc_1_33F6
        mov    dword ptr [esi + 1ch], 0
loc_1_33F6:
        mov    dword ptr [edi + 90h], 0
loc_1_3400:
        push   80h
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
        mov    edx, dword ptr [esp + 14h]
        mov    edi, edx
        mov    ebx, dword ptr [esp + 10h]
        mov    esi, ebx
        mov    ecx, 1dh
        rep movsd
        mov    dword ptr [edx + 50h], 0
        mov    dword ptr [edx + 4ch], 6
        mov    dword ptr [edx + 54h], 10h
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [ebx + 8ch]
        mov    dword ptr [edx + 8ch], eax
        mov    ecx, dword ptr [ebx + 90h]
        or     ecx, ecx
        mov    dword ptr [edx + 90h], ecx
        mov    eax, dword ptr [ebx + 74h]
        mov    dword ptr [edx + 74h], eax
        je     SHORT loc_1_3477
        inc    dword ptr [ecx + 0ch]
loc_1_3477:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_347B:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 88h]
        dec    edx
        mov    dword ptr [ecx + 88h], edx
        jne    SHORT loc_1_349E
        push   ecx
        call   sub_1_3374
        add    esp, 4
        mov    eax, 1
loc_1_349E:
        ret
sub_1_349F:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 30h]
        mov    esi, dword ptr [esp + 2ch]
        mov    ebx, dword ptr [edi + 34h]
        mov    ebp, dword ptr [esi + 74h]
        push   dword ptr [esi + 90h]
        push   ebx
        call   sub_1_0A77
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_1_35B4
        mov    dword ptr [ebx + 14h], esi
        mov    ecx, dword ptr [edi + 30h]
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [ecx], 1
        lea    edx, [eax - 2]
        push   eax
        mov    word ptr [ecx + 4], dx
        push   dword ptr [esp + 38h]
        add    ecx, 6
        push   ecx
        call   0
        add    esp, 0ch
        inc    dword ptr [esi + 88h]
        mov    eax, dword ptr [ebx + 3ch]
        mov    dword ptr [esi + 98h], eax
        push   esi
        push   eax
        call   sub_1_1FDE
        add    esp, 8
        mov    dword ptr [esi + 94h], ebx
        mov    dword ptr [esi + 4ch], 5
        and    dword ptr [esi + 50h], 0fffffffdh
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [ebx + 1ch], 60000004h
        inc    dword ptr [ebx + 0ch]
        mov    dword ptr [esp + 14h], 0ffffffffh
        mov    eax, dword ptr [ebx + 30h]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [edi + 18h]
        db  0C7h, 044h, 024h, 01Ch, 02Eh, 036h, 000h, 000h
        mov    dword ptr [esp + 20h], edi
        push   eax
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        call   dword ptr [eax + 18h]
        add    esp, 10h
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_35A5
        cmp    ebp, 0ffh
        je     SHORT loc_1_35B2
        mov    dword ptr [esi + 4ch], 2
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 1ch], eax
        mov    dword ptr [esi + 94h], 0
        mov    dword ptr [esi + 98h], 0
        push   ebx
        call   sub_1_09FC
        add    esp, 4
        push   esi
        call   sub_1_347B
        add    esp, 4
        jmp    SHORT loc_1_35B2
loc_1_35A5:
        push   0
        push   0
        push   edi
        call   sub_1_362E
        add    esp, 0ch
loc_1_35B2:
        mov    eax, ebp
loc_1_35B4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_1_35BC:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 98h]
        or     eax, eax
        je     SHORT loc_1_35D3
        push   eax
        call   sub_1_20C0
        add    esp, 4
loc_1_35D3:
        ret
sub_1_35D4:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 98h]
        or     eax, eax
        je     SHORT loc_1_35EB
        push   eax
        call   sub_1_2105
        add    esp, 4
loc_1_35EB:
        ret
sub_1_35EC:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 84h]
        lea    eax, [edx + 80h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [edx + 8ch]
        test   al, 2
        jne    SHORT loc_1_3611
        test   al, 4
        je     SHORT loc_1_3624
loc_1_3611:
        mov    ecx, dword ptr [edx + 0a0h]
        lea    eax, [edx + 9ch]
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
        mov    ebx, dword ptr [esp + 14h]
        mov    ebp, dword ptr [esp + 18h]
        or     ebp, ebp
        mov    esi, dword ptr [ebx + 34h]
        mov    edi, dword ptr [esi + 14h]
        jne    SHORT loc_1_36AD
        cmp    dword ptr [esi + 1ch], 60000004h
        jne    SHORT loc_1_36AD
        mov    dword ptr [edi + 4ch], 6
        mov    eax, dword ptr [ebx + 30h]
        movzx  ecx, word ptr [eax + 4]
        add    ecx, 2
        push   ecx
        add    eax, 6
        push   eax
        push   dword ptr [esi + 24h]
        call   0
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 30h]
        movzx  eax, word ptr [eax + 4]
        add    eax, 2
        mov    dword ptr [esi + 2ch], eax
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [edi + 20h], eax
        push   esi
        call   sub_1_0AE5
        add    esp, 4
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [edi + 24h], eax
        or     dword ptr [esi + 10h], 8
        push   4
        push   edi
        call   sub_1_3250
        add    esp, 8
        jmp    SHORT loc_1_36D1
loc_1_36AD:
        mov    dword ptr [edi + 4ch], 2
        mov    dword ptr [edi + 94h], 0
        mov    dword ptr [edi + 98h], 0
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
        jne    SHORT loc_1_3714
        push   ebp
        call   sub_1_3720
        add    esp, 4
        push   eax
        push   10h
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
        or     ebp, ebp
        jne    SHORT loc_1_3714
        mov    dword ptr [edi + 54h], 8
        push   0
        push   2
        push   edi
        call   sub_1_3BCC
        add    esp, 0ch
loc_1_3714:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3720:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0feh
        jg     SHORT loc_1_3739
        je     SHORT loc_1_3759
        cmp    eax, 22h
        ja     SHORT loc_1_3739
        db  0FFh, 024h, 085h, 0ACh, 037h, 000h, 000h
loc_1_3739:
        mov    eax, 2726h
        jmp    SHORT loc_1_37AB
loc_1_3740:
        sub    eax, eax
        jmp    SHORT loc_1_37AB
loc_1_3744:
        mov    eax, 2747h
        jmp    SHORT loc_1_37AB
loc_1_374B:
        mov    eax, 2740h
        jmp    SHORT loc_1_37AB
loc_1_3752:
        mov    eax, 2741h
        jmp    SHORT loc_1_37AB
loc_1_3759:
        mov    eax, 2726h
        jmp    SHORT loc_1_37AB
loc_1_3760:
        mov    eax, 2738h
        jmp    SHORT loc_1_37AB
loc_1_3767:
        mov    eax, 274dh
        jmp    SHORT loc_1_37AB
loc_1_376E:
        mov    eax, 2745h
        jmp    SHORT loc_1_37AB
loc_1_3775:
        mov    eax, 2746h
        jmp    SHORT loc_1_37AB
loc_1_377C:
        mov    eax, 274ch
        jmp    SHORT loc_1_37AB
loc_1_3783:
        mov    eax, 2775h
        jmp    SHORT loc_1_37AB
loc_1_378A:
        mov    eax, 2743h
        jmp    SHORT loc_1_37AB
loc_1_3791:
        mov    eax, 2751h
        jmp    SHORT loc_1_37AB
loc_1_3798:
        mov    eax, 2757h
        jmp    SHORT loc_1_37AB
loc_1_379F:
        mov    eax, 2714h
        jmp    SHORT loc_1_37AB
loc_1_37A6:
        mov    eax, 271eh
loc_1_37AB:
        ret
        db  040h, 037h, 000h, 000h, 044h, 037h, 000h, 000h, 04Bh, 037h, 000h, 000h, 052h, 037h, 000h, 000h
        db  044h, 037h, 000h, 000h, 039h, 037h, 000h, 000h, 052h, 037h, 000h, 000h, 059h, 037h, 000h, 000h
        db  039h, 037h, 000h, 000h, 060h, 037h, 000h, 000h, 059h, 037h, 000h, 000h, 059h, 037h, 000h, 000h
        db  039h, 037h, 000h, 000h, 039h, 037h, 000h, 000h, 067h, 037h, 000h, 000h, 059h, 037h, 000h, 000h
        db  059h, 037h, 000h, 000h, 059h, 037h, 000h, 000h, 059h, 037h, 000h, 000h, 06Eh, 037h, 000h, 000h
        db  075h, 037h, 000h, 000h, 07Ch, 037h, 000h, 000h, 083h, 037h, 000h, 000h, 039h, 037h, 000h, 000h
        db  039h, 037h, 000h, 000h, 059h, 037h, 000h, 000h, 059h, 037h, 000h, 000h, 08Ah, 037h, 000h, 000h
        db  091h, 037h, 000h, 000h, 08Ah, 037h, 000h, 000h, 098h, 037h, 000h, 000h, 059h, 037h, 000h, 000h
        db  09Fh, 037h, 000h, 000h, 0A6h, 037h, 000h, 000h, 09Fh, 037h, 000h, 000h
sub_1_3838:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 1
        je     SHORT loc_1_3848
        mov    eax, 20h
loc_1_3848:
        test   ch, 80h
        je     SHORT loc_1_3850
        or     eax, 40h
loc_1_3850:
        ret
sub_1_3851:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 40h
        je     SHORT loc_1_3861
        mov    eax, 1
loc_1_3861:
        test   cl, 10h
        je     SHORT loc_1_386B
        or     eax, 8000h
loc_1_386B:
        ret
        db  0C3h, 000h, 000h, 000h
sub_1_3870:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        push   1
        push   edi
        call   sub_1_0890
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_388D
        sub    eax, eax
        jmp    SHORT loc_1_38DE
loc_1_388D:
        mov    eax, dword ptr [edi + 20h]
        add    eax, 38h
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_38AF
        push   esi
        call   sub_1_0944
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_38DE
loc_1_38AF:
        mov    ebx, dword ptr [edi + 20h]
        mov    edi, edx
        lea    ebp, [ebx + 38h]
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        lea    eax, [edx + 38h]
        mov    dword ptr [edx + 30h], eax
        mov    dword ptr [edx + 10h], ebx
        mov    dword ptr [edx + 14h], eax
        mov    dword ptr [edx + 28h], ebx
        mov    dword ptr [edx + 2ch], eax
        mov    eax, edx
        mov    dword ptr [edx + 34h], esi
loc_1_38DE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_38E3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 34h]
        or     eax, eax
        je     SHORT loc_1_38FF
        push   eax
        call   sub_1_09FC
        add    esp, 4
        mov    dword ptr [esi + 34h], 0
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
        sub    esp, 10h
        mov    eax, dword ptr [esp + 20h]
        sub    eax, 50h
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_39CD
        mov    dword ptr [esp + 0ch], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp], 0
        mov    ecx, dword ptr [eax + 8]
        lea    eax, [esp]
        push   eax
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 24h]
        push   eax
        add    ecx, 8
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 28h]
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [ecx - 8]
        push   ecx
        call   sub_1_39D6
        add    esp, 24h
        or     eax, eax
        jne    SHORT loc_1_39C9
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_3998
        mov    eax, 273fh
        jmp    SHORT loc_1_39D2
loc_1_3998:
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_39A6
        mov    eax, 273ch
        jmp    SHORT loc_1_39D2
loc_1_39A6:
        cmp    dword ptr [esp + 4], 0
        jne    SHORT loc_1_39B4
        mov    eax, 273bh
        jmp    SHORT loc_1_39D2
loc_1_39B4:
        cmp    dword ptr [esp], 0
        je     SHORT loc_1_39C2
        mov    eax, 2739h
        jmp    SHORT loc_1_39D2
loc_1_39C2:
        mov    eax, 2726h
        jmp    SHORT loc_1_39D2
loc_1_39C9:
        sub    eax, eax
        jmp    SHORT loc_1_39D2
loc_1_39CD:
        mov    eax, 2742h
loc_1_39D2:
        add    esp, 10h
        ret
sub_1_39D6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     SHORT loc_1_3A51
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 24h]
        mov    ebx, dword ptr [esp + 2ch]
loc_1_39F5:
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_3A03
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], 1
loc_1_3A03:
        cmp    esi, dword ptr [ecx + 4]
        jne    SHORT loc_1_3A12
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], 1
loc_1_3A12:
        mov    ebp, dword ptr [ecx + 8]
        cmp    ebx, ebp
        jne    SHORT loc_1_3A23
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], 1
loc_1_3A23:
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_3A47
        cmp    esi, dword ptr [ecx + 4]
        jne    SHORT loc_1_3A3A
        cmp    ebx, ebp
        je     SHORT loc_1_3A3A
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [eax], 1
loc_1_3A3A:
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_3A47
        cmp    esi, dword ptr [ecx + 4]
        jne    SHORT loc_1_3A47
        cmp    ebx, ebp
        je     SHORT loc_1_3A58
loc_1_3A47:
        add    ecx, 0ch
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    SHORT loc_1_39F5
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
        jmp    SHORT loc_1_3A53
        db  000h
AFVXD_Service_00:
        mov    ax, 0
        clc
        ret
AFVXD_Control:
        cmp    eax, 1
        db  00Fh, 084h, 075h, 0CEh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 05Ch, 0CEh, 0FEh, 0FFh
        clc
        ret
sub_1_3A7A:
        db  0CDh, 020h, 005h, 080h, 089h, 004h
sub_1_3A80:
        push   0
        push   dword ptr [esp + 8]
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_3A90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 03Eh, 000h
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
        db  0CDh, 020h, 008h, 000h, 088h, 004h
        add    esp, 4
        leave
        ret
AFVXD_Service_01:
        push   ecx
        push   ebx
        push   eax
        db  0E8h, 0E8h, 0C8h, 0FEh, 0FFh
        add    esp, 0ch
        ret
AFVXD_Service_02:
        push   eax
        db  0E8h, 04Fh, 0CCh, 0FEh, 0FFh
        add    esp, 4
        ret
        db  000h, 000h
sub_1_3AD8:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_3B17
        or     ebx, ebx
        je     SHORT loc_1_3B17
        movzx  ecx, ax
        and    ch, 0fh
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_3B1B
        add    eax, esi
loc_1_3B17:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_3B1B:
        xor    eax, eax
        jmp    SHORT loc_1_3B17
sub_1_3B1F:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_3B5A
        or     ebx, ebx
        je     SHORT loc_1_3B5A
        movzx  ecx, ax
        and    ch, 0fh
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_3B63
loc_1_3B5A:
        mov    eax, 1
loc_1_3B5F:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_3B63:
        xor    eax, eax
        jmp    SHORT loc_1_3B5F
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_3BB0:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_3BB6:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_3BC3
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
loc_1_3BC3:
        ret
sub_1_3BC4:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        add    byte ptr [eax], al
sub_1_3BCC:
        db  0CDh, 020h, 003h, 080h, 03Eh, 000h
sub_1_3BD2:
        db  0CDh, 020h, 004h, 080h, 03Eh, 000h
loc_1_3BD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3BDC:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3BE0:
        inc    eax
        add    byte ptr [eax], al
        db  000h, 020h
        cmp    dword ptr [eax], eax
        db  000h, 0D0h
        add    byte ptr cs:[eax], al
        stc
        add    byte ptr cs:[eax], al
        pushad
        add    eax, dword ptr [eax]
        db  000h, 0F0h
        add    byte ptr [eax], al
        db  000h, 0C0h
        db  005h, 000h, 000h, 0D0h, 01Dh
        add    byte ptr [eax], al
        inc    eax
        and    dword ptr [eax], eax
        db  000h, 010h
        daa
        add    byte ptr [eax], al
        mov    al, 5
        add    byte ptr [eax], al
        jo     SHORT loc_1_3C3D
        add    byte ptr [eax], al
        dec    ebp
        xor    byte ptr [eax], al
        db  000h, 080h, 01Ch, 000h, 000h, 000h
        add    byte ptr cs:[eax], al
        rcr    byte ptr [ecx], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        jae    SHORT loc_1_3CA0
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_1_3CA4
        jb     SHORT loc_1_3C99
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
loc_1_3C3D:
        push   ebx
        je     NEAR PTR loc_1_3C9D
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        inc    ecx
        inc    esi
        push   esi
        pop    eax
        inc    esp
        add    byte ptr [eax + eax], bl
        add    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax + edx*2 + 61h], bl
        db  072h, 061h
        insd
        db  065h, 074h, 065h
        db  072h, 073h
        pop    esp
        push   edi
        imul   ebp, dword ptr [esi + 73h], 6b636fh
loc_1_3C98:
        dec    ebp
loc_1_3C99:
        popad
        db  078h, 053h
        outsd
loc_1_3C9D:
        arpl   word ptr [ebx + 41h], bp
loc_1_3CA0:
        db  064h, 064h, 072h, 04Ch
loc_1_3CA4:
        outsb
        db  067h, 074h, 068h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 69h], cl
        outsb
        push   ebx
        outsd
        arpl   word ptr [ebx + 41h], bp
        db  064h, 064h, 072h, 04Ch
        outsb
        db  067h, 074h, 068h
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  060h, 03Ah, 000h, 000h, 0C0h, 03Ah, 000h, 000h, 0CCh, 03Ah, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 095h, 004h, 000h, 000h, 000h, 000h, 041h, 046h, 056h, 058h
        db  044h, 020h, 020h, 020h, 000h, 0C0h, 000h, 054h, 066h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CCh, 03Ch, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3D30:
        push   ebx
        push   esi
        push   edi
        db  0FFh, 005h, 0F8h, 041h, 000h, 000h
        db  0A1h, 05Ch, 042h, 000h, 000h
        db  08Bh, 00Dh, 0F8h, 041h, 000h, 000h
        lea    edi, [eax + ecx*4]
        db  03Bh, 03Dh, 050h, 042h, 000h, 000h
        ja     SHORT loc_1_3D53
        sub    edi, edi
        jmp    SHORT loc_1_3D59
loc_1_3D53:
        db  02Bh, 03Dh, 050h, 042h, 000h, 000h
loc_1_3D59:
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     SHORT loc_1_3DCF
        db  08Bh, 00Dh, 054h, 042h, 000h, 000h
        db  0BEh, 0FCh, 041h, 000h, 000h
        db  0BBh, 050h, 042h, 000h, 000h
loc_1_3D70:
        db  0A1h, 058h, 042h, 000h, 000h
        db  03Bh, 005h, 0E4h, 041h, 000h, 000h
        jbe    SHORT loc_1_3DCF
        db  03Bh, 00Dh, 050h, 042h, 000h, 000h
        jbe    SHORT loc_1_3DCF
        call   sub_1_3BC4
        push   40h
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        db  08Bh, 00Dh, 054h, 042h, 000h, 000h
        je     SHORT loc_1_3DCF
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [eax + 14h], ebx
        mov    dword ptr [eax], esi
        db  08Bh, 015h, 000h, 042h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  08Bh, 015h, 000h, 042h, 000h, 000h
        mov    dword ptr [edx], eax
        db  0A3h, 000h, 042h, 000h, 000h
        mov    eax, edi
        db  0FFh, 005h, 0E4h, 041h, 000h, 000h
        db  0FFh, 005h, 050h, 042h, 000h, 000h
        dec    edi
        or     eax, eax
        jne    SHORT loc_1_3D70
loc_1_3DCF:
        db  0A1h, 044h, 042h, 000h, 000h
        db  08Bh, 00Dh, 0F8h, 041h, 000h, 000h
        lea    edi, [eax + ecx*2]
        db  03Bh, 03Dh, 0F0h, 041h, 000h, 000h
        ja     SHORT loc_1_3DE9
        sub    edi, edi
        jmp    SHORT loc_1_3DEF
loc_1_3DE9:
        db  02Bh, 03Dh, 0F0h, 041h, 000h, 000h
loc_1_3DEF:
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     SHORT loc_1_3E68
        db  08Bh, 00Dh, 038h, 042h, 000h, 000h
        db  0BEh, 0E8h, 041h, 000h, 000h
        db  0BBh, 0F0h, 041h, 000h, 000h
loc_1_3E06:
        db  0A1h, 04Ch, 042h, 000h, 000h
        db  03Bh, 005h, 008h, 042h, 000h, 000h
        jbe    SHORT loc_1_3E68
        db  03Bh, 00Dh, 0F0h, 041h, 000h, 000h
        jbe    SHORT loc_1_3E68
        call   sub_1_3BC4
        push   5b4h
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        db  08Bh, 00Dh, 038h, 042h, 000h, 000h
        je     SHORT loc_1_3E68
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [eax + 14h], ebx
        mov    dword ptr [eax], esi
        db  08Bh, 015h, 0ECh, 041h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  08Bh, 015h, 0ECh, 041h, 000h, 000h
        mov    dword ptr [edx], eax
        db  0A3h, 0ECh, 041h, 000h, 000h
        mov    eax, edi
        db  0FFh, 005h, 008h, 042h, 000h, 000h
        db  0FFh, 005h, 0F0h, 041h, 000h, 000h
        dec    edi
        or     eax, eax
        jne    SHORT loc_1_3E06
loc_1_3E68:
        db  08Bh, 03Dh, 0F4h, 041h, 000h, 000h
        db  003h, 03Dh, 0F8h, 041h, 000h, 000h
        db  03Bh, 03Dh, 004h, 042h, 000h, 000h
        ja     SHORT loc_1_3E80
        sub    edi, edi
        jmp    SHORT loc_1_3E86
loc_1_3E80:
        db  02Bh, 03Dh, 004h, 042h, 000h, 000h
loc_1_3E86:
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     SHORT loc_1_3EFF
        db  08Bh, 00Dh, 0E0h, 041h, 000h, 000h
        db  0BEh, 03Ch, 042h, 000h, 000h
        db  0BBh, 004h, 042h, 000h, 000h
loc_1_3E9D:
        db  0A1h, 00Ch, 042h, 000h, 000h
        db  03Bh, 005h, 034h, 042h, 000h, 000h
        jbe    SHORT loc_1_3EFF
        db  03Bh, 00Dh, 004h, 042h, 000h, 000h
        jbe    SHORT loc_1_3EFF
        call   sub_1_3BC4
        push   111ch
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        db  08Bh, 00Dh, 0E0h, 041h, 000h, 000h
        je     SHORT loc_1_3EFF
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [eax + 14h], ebx
        mov    dword ptr [eax], esi
        db  08Bh, 015h, 040h, 042h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  08Bh, 015h, 040h, 042h, 000h, 000h
        mov    dword ptr [edx], eax
        db  0A3h, 040h, 042h, 000h, 000h
        mov    eax, edi
        db  0FFh, 005h, 034h, 042h, 000h, 000h
        db  0FFh, 005h, 004h, 042h, 000h, 000h
        dec    edi
        or     eax, eax
        jne    SHORT loc_1_3E9D
loc_1_3EFF:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3F03:
        db  0A1h, 0F8h, 041h, 000h, 000h
        dec    eax
        db  0A3h, 0F8h, 041h, 000h, 000h
        ret
sub_1_3F0F:
        push   dword ptr [esp + 4]
        call   sub_1_406B
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_3F43
        mov    dword ptr [eax + 5ch], 0
        mov    ecx, dword ptr [eax + 24h]
        mov    dword ptr [eax + 68h], ecx
        mov    dword ptr [eax + 20h], 0
        mov    dword ptr [eax + 28h], 0
        mov    dword ptr [eax + 1ch], 0
        jmp    SHORT loc_1_3F45
loc_1_3F43:
        sub    eax, eax
loc_1_3F45:
        ret
sub_1_3F46:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 10h]
        or     edx, edx
        je     SHORT loc_1_3F65
        mov    dword ptr [eax], edx
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [eax + 14h]
        inc    dword ptr [eax]
        jmp    SHORT loc_1_3F6E
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
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        mov    ebx, dword ptr [edx + 28h]
        mov    eax, dword ptr [edx + 20h]
        sub    eax, ebx
        cmp    eax, ecx
        jb     SHORT loc_1_3F88
        mov    eax, ecx
loc_1_3F88:
        or     eax, eax
        je     SHORT loc_1_3FAD
        mov    esi, dword ptr [edx + 70h]
        mov    edi, dword ptr [esp + 10h]
        add    esi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_3FAD
        add    dword ptr [edx + 28h], eax
loc_1_3FAD:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3FB1:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 18h]
        mov    ebx, dword ptr [edx + 20h]
        mov    eax, dword ptr [edx + 24h]
        sub    eax, ebx
        cmp    eax, ecx
        jb     SHORT loc_1_3FCA
        mov    eax, ecx
loc_1_3FCA:
        or     eax, eax
        je     SHORT loc_1_3FE7
        mov    edi, dword ptr [edx + 70h]
        add    edi, ebx
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, eax
loc_1_3FE7:
        mov    dword ptr [edx + 20h], ebx
        mov    dword ptr [edx + 68h], ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3FF1:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 20h], ecx
        ret
sub_1_3FFD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 024h, 03Ch, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        add    eax, esi
        add    eax, 74h
        push   eax
        call   sub_1_3BB0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_1_4064
        db  08Bh, 01Dh, 024h, 03Ch, 000h, 000h
        mov    edi, edx
        lea    ebp, [ebx + 74h]
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        lea    eax, [edx + 74h]
        mov    dword ptr [edx + 2ch], eax
        lea    eax, [edx + ebx + 74h]
        mov    dword ptr [edx + 70h], eax
        mov    dword ptr [edx + 24h], esi
        mov    dword ptr [edx + 6ch], 4655424eh
        mov    dword ptr [edx + 5ch], 0
        mov    dword ptr [edx + 68h], esi
        mov    dword ptr [edx + 60h], eax
        mov    dword ptr [edx + 64h], 0
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
        cmp    esi, 40h
        ja     SHORT loc_1_40E0
        db  081h, 03Dh, 0FCh, 041h, 000h, 000h, 0FCh, 041h, 000h, 000h
        je     SHORT loc_1_40A4
        db  0A1h, 0FCh, 041h, 000h, 000h
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  0C7h, 041h, 004h, 0FCh, 041h, 000h, 000h
        db  089h, 00Dh, 0FCh, 041h, 000h, 000h
        db  0FFh, 00Dh, 050h, 042h, 000h, 000h
        jmp    NEAR PTR loc_1_41C8
loc_1_40A4:
        db  08Bh, 015h, 0E4h, 041h, 000h, 000h
        db  039h, 015h, 058h, 042h, 000h, 000h
        jbe    SHORT loc_1_40E0
        push   40h
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_40E0
        db  08Bh, 00Dh, 0E4h, 041h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 0E4h, 041h, 000h, 000h
        db  0C7h, 040h, 010h, 0FCh, 041h, 000h, 000h
        db  0C7h, 040h, 014h, 050h, 042h, 000h, 000h
        jmp    NEAR PTR loc_1_41C8
loc_1_40E0:
        cmp    esi, 5b4h
        ja     SHORT loc_1_4151
        db  081h, 03Dh, 0E8h, 041h, 000h, 000h, 0E8h, 041h, 000h, 000h
        je     SHORT loc_1_4115
        db  0A1h, 0E8h, 041h, 000h, 000h
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  0C7h, 041h, 004h, 0E8h, 041h, 000h, 000h
        db  089h, 00Dh, 0E8h, 041h, 000h, 000h
        db  0FFh, 00Dh, 0F0h, 041h, 000h, 000h
        jmp    NEAR PTR loc_1_41C8
loc_1_4115:
        db  08Bh, 015h, 008h, 042h, 000h, 000h
        db  039h, 015h, 04Ch, 042h, 000h, 000h
        jbe    SHORT loc_1_4151
        push   5b4h
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4151
        db  08Bh, 00Dh, 008h, 042h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 008h, 042h, 000h, 000h
        db  0C7h, 040h, 010h, 0E8h, 041h, 000h, 000h
        db  0C7h, 040h, 014h, 0F0h, 041h, 000h, 000h
        jmp    SHORT loc_1_41C8
loc_1_4151:
        cmp    esi, 111ch
        ja     SHORT loc_1_41BF
        db  081h, 03Dh, 03Ch, 042h, 000h, 000h, 03Ch, 042h, 000h, 000h
        je     SHORT loc_1_4183
        db  0A1h, 03Ch, 042h, 000h, 000h
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  0C7h, 041h, 004h, 03Ch, 042h, 000h, 000h
        db  089h, 00Dh, 03Ch, 042h, 000h, 000h
        db  0FFh, 00Dh, 004h, 042h, 000h, 000h
        jmp    SHORT loc_1_41C8
loc_1_4183:
        db  08Bh, 015h, 034h, 042h, 000h, 000h
        db  039h, 015h, 00Ch, 042h, 000h, 000h
        jbe    SHORT loc_1_41C8
        push   111ch
        call   sub_1_3FFD
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_41C8
        db  08Bh, 00Dh, 034h, 042h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 034h, 042h, 000h, 000h
        db  0C7h, 040h, 010h, 03Ch, 042h, 000h, 000h
        db  0C7h, 040h, 014h, 004h, 042h, 000h, 000h
        jmp    SHORT loc_1_41C8
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
_LTEXT ends
        end
