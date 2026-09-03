; vdhcp_lcode_01.asm -- VDHCP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        db  0CDh, 020h, 001h, 080h, 088h, 004h
sub_1_0006:
        db  0CDh, 020h, 002h, 080h, 088h, 004h
sub_1_000C:
        db  0CDh, 020h, 03Fh, 080h, 001h, 000h
        db  0CDh, 020h, 003h, 080h, 088h, 004h
        db  0CDh, 020h, 004h, 080h, 088h, 004h
sub_1_001E:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_0024:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
sub_1_002A:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        db  0CDh, 020h, 005h, 080h, 088h, 004h
sub_1_0036:
        db  0CDh, 020h, 00Dh, 080h, 088h, 004h
sub_1_003C:
        db  0CDh, 020h, 006h, 080h, 088h, 004h
        db  0A1h, 0C8h, 028h, 000h, 000h
        db  0CDh, 020h, 009h, 080h, 088h, 004h
loc_1_004D:
        db  0A1h, 0C8h, 028h, 000h, 000h
        db  0CDh, 020h, 00Ch, 080h, 088h, 004h
loc_1_0058:
        db  0A1h, 0C8h, 028h, 000h, 000h
        db  0CDh, 020h, 00Bh, 080h, 088h, 004h
        add    byte ptr [ebx + 56h], dl
        push   edi
        push   ebp
        mov    bx, word ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 14h]
        or     ecx, ecx
        jne    SHORT loc_1_0080
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0080:
        db  08Bh, 015h, 084h, 02Bh, 000h, 000h
        db  081h, 0FAh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_1_0105
loc_1_008E:
        cmp    dword ptr [edx + 11h], ecx
        je     SHORT loc_1_0098
        cmp    ecx, -1
        jne    SHORT loc_1_00FB
loc_1_0098:
        mov    edi, dword ptr [edx + 51h]
        mov    esi, dword ptr [edi + 2ah]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, -2ah
        je     SHORT loc_1_00F6
        movzx  eax, bx
loc_1_00AA:
        movzx  ebp, byte ptr [esi + 8]
        cmp    ebp, eax
        je     SHORT loc_1_00C0
        mov    esi, dword ptr [esi]
        mov    ebp, edi
        sub    ebp, esi
        cmp    ebp, -2ah
        jne    SHORT loc_1_00AA
        jmp    SHORT loc_1_00F6
        db  090h
loc_1_00C0:
        cmp    bx, 2bh
        jne    SHORT loc_1_0110
        mov    ebx, dword ptr [esp + 18h]
        shr    ebx, 8
        lea    edi, [esi + 0ah]
        movzx  esi, byte ptr [esi + 9]
        or     esi, esi
        je     SHORT loc_1_00F6
        movzx  ebp, bx
        nop
loc_1_00DC:
        movzx  eax, byte ptr [edi]
        cmp    eax, ebp
        je     SHORT loc_1_00F2
        movzx  eax, byte ptr [edi + 1]
        sub    esi, eax
        lea    eax, [eax + eax*2]
        add    edi, eax
        or     esi, esi
        jne    SHORT loc_1_00DC
loc_1_00F2:
        or     esi, esi
        jne    SHORT loc_1_0134
loc_1_00F6:
        cmp    ecx, -1
        jne    SHORT loc_1_0158
loc_1_00FB:
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_1_008E
loc_1_0105:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0110:
        add    esi, 0ah
        mov    ecx, dword ptr [esp + 20h]
        push   ecx
        movzx  eax, byte ptr [esi - 1]
        push   eax
        push   esi
        push   dword ptr [ecx]
        push   dword ptr [esp + 2ch]
        db  0E8h, 04Fh, 015h, 0FFh, 0FFh
        add    esp, 14h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_0134:
        add    edi, 2
        mov    ecx, dword ptr [esp + 20h]
        push   ecx
        movzx  eax, byte ptr [edi - 1]
        push   eax
        push   edi
        push   dword ptr [ecx]
        push   dword ptr [esp + 2ch]
        db  0E8h, 02Bh, 015h, 0FFh, 0FFh
        add    esp, 14h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_0158:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_0164:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, 1
        je     SHORT loc_1_0180
        cmp    eax, 2
        je     NEAR PTR loc_1_0214
        mov    eax, 1ah
        pop    edi
        pop    esi
        ret
loc_1_0180:
        cmp    dword ptr [esp + 18h], 8
        jne    NEAR PTR loc_1_020C
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_020C
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        je     SHORT loc_1_01C4
        db  08Bh, 03Dh, 084h, 02Bh, 000h, 000h
        db  081h, 0FFh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_1_01B9
loc_1_01AA:
        cmp    dword ptr [edi + 11h], ecx
        je     SHORT loc_1_01C6
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_1_01AA
loc_1_01B9:
        pop    edi
        mov    eax, 3
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_01C4:
        sub    edi, edi
loc_1_01C6:
        push   14h
        db  0E8h, 0D3h, 017h, 0FFh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_01DC
        mov    eax, 1
        pop    edi
        pop    esi
        ret
loc_1_01DC:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 10h], edi
        db  0C7h, 000h, 070h, 02Bh, 000h, 000h
        db  08Bh, 00Dh, 074h, 02Bh, 000h, 000h
        mov    dword ptr [eax + 4], ecx
        db  08Bh, 00Dh, 074h, 02Bh, 000h, 000h
        mov    dword ptr [ecx], eax
        db  0A3h, 074h, 02Bh, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_020C:
        pop    edi
        mov    eax, 1ah
        pop    esi
        ret
loc_1_0214:
        mov    esi, dword ptr [esp + 14h]
        push   esi
        db  068h, 084h, 02Bh, 000h, 000h
        call   sub_1_0364
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0234
        mov    eax, 1ah
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_0234:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 11h], ecx
        push   dword ptr [esi + 18h]
        lea    ecx, [esi + 1ch]
        push   ecx
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_1_042C
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0258
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_0258:
        cmp    eax, 8
        jne    SHORT loc_1_0268
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_0268:
        pop    edi
        mov    eax, 1ah
        pop    esi
        ret
sub_1_0270:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 02Dh, 070h, 02Bh, 000h, 000h
        db  081h, 0FDh, 070h, 02Bh, 000h, 000h
        je     SHORT loc_1_02B3
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 1ch]
loc_1_028E:
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_0299
        cmp    esi, eax
        jne    SHORT loc_1_02A8
loc_1_0299:
        push   dword ptr [esp + 20h]
        push   ebx
        push   edi
        push   dword ptr [ebp + 0ch]
        call   dword ptr [ebp + 8]
        add    esp, 10h
loc_1_02A8:
        mov    ebp, dword ptr [ebp]
        db  081h, 0FDh, 070h, 02Bh, 000h, 000h
        jne    SHORT loc_1_028E
loc_1_02B3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_02B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 51h]
        mov    dword ptr [ebp - 14h], 301h
        mov    ecx, dword ptr [esi + 0ah]
        mov    dword ptr [ebp - 10h], ecx
        mov    dword ptr [ebp - 0ch], 200h
        mov    dword ptr [ebp - 8], 100h
        cmp    dword ptr [ebp + 0ch], 101h
        jne    SHORT loc_1_035C
        mov    dword ptr [ebp - 4], 101h
        push   3
        push   eax
        call   sub_1_0814
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_035C
        mov    cl, byte ptr [eax + 9]
        shr    cl, 2
        movzx  ebx, cl
        lea    edi, [eax + 0ah]
        or     ebx, ebx
        jle    SHORT loc_1_035C
loc_1_0311:
        mov    dword ptr [ebp - 44h], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 3ch], 1
        mov    dword ptr [ebp - 28h], 3
        mov    dword ptr [ebp - 24h], 2
        mov    eax, dword ptr [esi + 6]
        mov    dword ptr [ebp - 40h], eax
        push   30h
        lea    eax, [ebp - 44h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        db  0A1h, 054h, 030h, 000h, 000h
        call   dword ptr [eax + 4ch]
        add    esp, 10h
        add    edi, 4
        dec    ebx
        jne    SHORT loc_1_0311
loc_1_035C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_0364:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        mov    eax, dword ptr [ebx]
        cmp    eax, ebx
        je     SHORT loc_1_039C
        mov    ebp, dword ptr [esp + 18h]
loc_1_0376:
        mov    edx, dword ptr [eax + 0dh]
        cmp    dword ptr [ebp], edx
        jne    SHORT loc_1_0396
        lea    esi, [ebp + 4]
        mov    ecx, edx
        shr    ecx, 2
        mov    edi, dword ptr [eax + 9]
        repe cmpsd
        jne    SHORT loc_1_0394
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
loc_1_0394:
        je     SHORT loc_1_039E
loc_1_0396:
        mov    eax, dword ptr [eax]
        cmp    eax, ebx
        jne    SHORT loc_1_0376
loc_1_039C:
        sub    eax, eax
loc_1_039E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_03A4:
        push   esi
        push   0
        db  0E8h, 01Ch, 014h, 0FFh, 0FFh
        add    esp, 4
        db  08Bh, 015h, 04Ch, 030h, 000h, 000h
        db  081h, 0FAh, 04Ch, 030h, 000h, 000h
        je     SHORT loc_1_03CC
loc_1_03BD:
        cmp    dword ptr [edx + 31h], eax
        jl     SHORT loc_1_03DC
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 04Ch, 030h, 000h, 000h
        jne    SHORT loc_1_03BD
loc_1_03CC:
        pop    esi
        db  0A1h, 080h, 02Bh, 000h, 000h
        db  0FFh, 005h, 080h, 02Bh, 000h, 000h
        ret
        db  08Dh, 049h, 000h
loc_1_03DC:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 51h]
        mov    esi, dword ptr [eax + 0eh]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        db  0E8h, 08Eh, 015h, 0FFh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_03FC:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        push   7
        db  068h, 0E0h, 028h, 000h, 000h
        push   37h
        push   dword ptr [esp + 18h]
        call   sub_1_12A8
        add    esp, 14h
        push   esi
        push   4
        db  068h, 0E8h, 028h, 000h, 000h
        push   2bh
        push   eax
        call   sub_1_12A8
        add    esp, 14h
        pop    esi
        ret
sub_1_042C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, dword ptr [eax + 51h]
        push   dword ptr [esp + 18h]
        push   eax
        call   sub_1_0814
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_048C
        movzx  ecx, byte ptr [eax + 9]
        mov    ebx, dword ptr [esp + 20h]
        cmp    ecx, ebx
        jne    SHORT loc_1_0474
        lea    esi, [eax + 0ah]
        mov    ecx, ebx
        shr    ecx, 2
        mov    edi, dword ptr [esp + 1ch]
        repe cmpsd
        jne    SHORT loc_1_0474
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_0474
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0474:
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_0024
        add    esp, 4
        jmp    SHORT loc_1_0490
        db  08Dh, 049h, 000h
loc_1_048C:
        mov    ebx, dword ptr [esp + 20h]
loc_1_0490:
        mov    ax, bx
        add    ax, 0ah
        push   eax
        db  0E8h, 003h, 009h, 0FFh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_04B0
        mov    eax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_04B0:
        mov    byte ptr [eax + 9], bl
        mov    cl, byte ptr [esp + 18h]
        mov    byte ptr [eax + 8], cl
        lea    edi, [eax + 0ah]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        add    ebp, 2ah
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebp], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_04E8:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 51h]
        mov    eax, dword ptr [esi + 2ah]
        sub    eax, esi
        cmp    eax, 2ah
        je     SHORT loc_1_051A
loc_1_04FA:
        mov    eax, dword ptr [esi + 2ah]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 2ah]
        sub    eax, esi
        cmp    eax, 2ah
        jne    SHORT loc_1_04FA
loc_1_051A:
        pop    esi
        ret
sub_1_051C:
        sub    eax, eax
        ret
        db  090h
sub_1_0520:
        sub    ecx, ecx
        mov    eax, dword ptr [esp + 8]
loc_1_0526:
        db  00Fh, 0B6h, 091h, 0E0h, 028h, 000h, 000h
        cmp    edx, eax
        je     SHORT loc_1_0540
        inc    ecx
        cmp    ecx, 7
        jb     SHORT loc_1_0526
        mov    eax, 57h
        ret
        db  08Dh, 049h, 000h
loc_1_0540:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_1_042C
        add    esp, 10h
        ret
        db  08Bh, 0FFh, 02Bh, 0C0h, 0C3h, 090h
sub_1_055C:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 51h]
        cmp    dword ptr [ecx], -1
        je     SHORT loc_1_056C
        sub    eax, eax
        ret
        db  090h
loc_1_056C:
        cmp    dword ptr [eax + 35h], 0
        je     SHORT loc_1_0580
        mov    eax, dword ptr [eax + 11h]
        push   eax
        push   ecx
        call   sub_1_27A4
        add    esp, 8
        ret
loc_1_0580:
        sub    eax, eax
        push   eax
        push   ecx
        call   sub_1_27A4
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_0590:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 51h]
        mov    eax, dword ptr [esi]
        cmp    eax, -1
        je     SHORT loc_1_05B0
        push   eax
        call   sub_1_08A0
        mov    dword ptr [esi], 0ffffffffh
        call   sub_1_1178
loc_1_05B0:
        sub    eax, eax
        pop    esi
        ret
sub_1_05B4:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 51h]
        cmp    dword ptr [edi + 35h], 0
        jne    SHORT loc_1_05EB
        push   dword ptr [edi + 15h]
        push   dword ptr [edi + 11h]
        movzx  eax, word ptr [esi + 4]
        push   eax
        call   sub_1_0F03
        add    esp, 0ch
        push   101h
        push   edi
        call   sub_1_02B8
        add    esp, 8
        mov    dword ptr [edi + 35h], 1
loc_1_05EB:
        cmp    dword ptr [esi], -1
        je     SHORT loc_1_05F8
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_05F8:
        push   dword ptr [edi + 11h]
        push   esi
        call   sub_1_27A4
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_061A
        push   0
        push   3e8h
        push   edi
        call   sub_1_0844
        add    esp, 0ch
loc_1_061A:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_0620:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 51h]
        mov    eax, dword ptr [esi]
        cmp    eax, -1
        je     SHORT loc_1_063C
        push   eax
        call   sub_1_08A0
        mov    dword ptr [esi], 0ffffffffh
loc_1_063C:
        cmp    dword ptr [edi + 35h], 0
        je     SHORT loc_1_065A
        push   0
        push   0
        movzx  eax, word ptr [esi + 4]
        push   eax
        call   sub_1_0F03
        add    esp, 0ch
        mov    dword ptr [edi + 35h], 0
loc_1_065A:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_0660:
        push   0
        db  0E8h, 061h, 011h, 0FFh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, -1
        mov    edx, dword ptr [esp + 4]
        jne    SHORT loc_1_067C
        jmp    SHORT loc_1_0685
        db  08Dh, 049h, 000h
loc_1_067C:
        add    ecx, eax
        mov    dword ptr [edx + 41h], ecx
        cmp    ecx, eax
        jge    SHORT loc_1_068C
loc_1_0685:
        mov    dword ptr [edx + 41h], 7fffffffh
loc_1_068C:
        lea    ecx, [edx + 39h]
        db  0C7h, 001h, 060h, 02Bh, 000h, 000h
        db  0A1h, 064h, 02Bh, 000h, 000h
        mov    dword ptr [edx + 3dh], eax
        db  0A1h, 064h, 02Bh, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 064h, 02Bh, 000h, 000h
        push   0
        push   0
        db  0E8h, 065h, 004h, 0FFh, 0FFh
        add    esp, 8
        ret
        db  090h
sub_1_06B8:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esi + 11h]
        mov    dword ptr [esp + 14h], eax
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_1_071C
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_06DC
        mov    eax, dword ptr [esp + 24h]
loc_1_06DC:
        mov    dword ptr [esi + 1dh], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esi + 11h], eax
        mov    edi, dword ptr [esp + 20h]
        or     edi, edi
        je     SHORT loc_1_06FC
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        je     SHORT loc_1_06FC
        mov    eax, dword ptr [eax]
        jmp    SHORT loc_1_0708
        db  08Dh, 049h, 000h
loc_1_06FC:
        push   dword ptr [esp + 24h]
        call   sub_1_1390
        add    esp, 4
loc_1_0708:
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 15h], eax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esi + 19h], eax
        jmp    SHORT loc_1_0720
loc_1_071C:
        mov    edi, dword ptr [esp + 20h]
loc_1_0720:
        or     edi, edi
        je     SHORT loc_1_0738
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_1_0738
        push   dword ptr [eax]
        call   sub_1_0E7C
        mov    ebp, eax
        jmp    SHORT loc_1_073D
        db  08Bh, 0FFh
loc_1_0738:
        mov    ebp, 0e10h
loc_1_073D:
        mov    dword ptr [esi + 21h], ebp
        push   0
        db  0E8h, 081h, 010h, 0FFh, 0FFh
        add    esp, 4
        mov    ebx, eax
        mov    dword ptr [esi + 25h], ebx
        or     edi, edi
        je     SHORT loc_1_0764
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_1_0764
        push   dword ptr [eax]
        call   sub_1_0E7C
        jmp    SHORT loc_1_0768
        db  090h
loc_1_0764:
        mov    eax, ebp
        shr    eax, 1
loc_1_0768:
        add    eax, ebx
        cmp    eax, ebx
        jge    SHORT loc_1_0773
        mov    eax, 7fffffffh
loc_1_0773:
        or     edi, edi
        mov    dword ptr [esi + 29h], eax
        je     SHORT loc_1_078C
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_078C
        push   dword ptr [eax]
        call   sub_1_0E7C
        jmp    SHORT loc_1_079C
        db  08Bh, 0FFh
loc_1_078C:
        sub    edx, edx
        lea    eax, [ebp*4]
        mov    ecx, 5
        div    ecx
loc_1_079C:
        add    eax, ebx
        cmp    eax, ebx
        jge    SHORT loc_1_07A7
        mov    eax, 7fffffffh
loc_1_07A7:
        mov    dword ptr [esi + 2dh], eax
        lea    eax, [ebx + ebp]
        cmp    eax, ebx
        jge    SHORT loc_1_07B6
        mov    eax, 7fffffffh
loc_1_07B6:
        mov    dword ptr [esi + 31h], eax
        push   0
        push   esi
        db  0E8h, 0E7h, 019h, 0FFh, 0FFh
        add    esp, 8
        mov    edi, eax
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_1_0809
        push   esi
        call   sub_1_0620
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_080B
        push   esi
        call   sub_1_05B4
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_07F4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_07F4:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 1ch]
        push   esi
        call   sub_1_0270
        add    esp, 10h
loc_1_0809:
        mov    eax, edi
loc_1_080B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
sub_1_0814:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 51h]
        mov    eax, dword ptr [esi + 2ah]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, -2ah
        je     SHORT loc_1_083F
        mov    edx, dword ptr [esp + 0ch]
loc_1_082C:
        movzx  ecx, byte ptr [eax + 8]
        cmp    ecx, edx
        je     SHORT loc_1_0841
        mov    eax, dword ptr [eax]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, -2ah
        jne    SHORT loc_1_082C
loc_1_083F:
        sub    eax, eax
loc_1_0841:
        pop    esi
        ret
        db  090h
sub_1_0844:
        ret
        db  08Dh, 049h, 000h
sub_1_0848:
        push   edi
        db  083h, 03Dh, 054h, 030h, 000h, 000h, 000h
        jne    SHORT loc_1_0868
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 042h, 027h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    edi
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_0868:
        push   7ah
        db  0E8h, 031h, 011h, 0FFh, 0FFh
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_1_088C
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 1eh
        rep stosd
        stosw
        mov    eax, edx
        pop    edi
        ret    0ch
        db  090h
loc_1_088C:
        pop    edi
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 047h, 027h, 000h, 000h
        mov    eax, 0ffffffffh
        ret    0ch
        db  090h
sub_1_08A0:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 2eh]
        or     eax, eax
        je     SHORT loc_1_08C8
        db  08Bh, 00Dh, 054h, 030h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_08C8
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        call   dword ptr [ecx + 4]
        add    esp, 4
loc_1_08C8:
        push   esi
        db  0E8h, 0B2h, 010h, 0FFh, 0FFh
        add    esp, 4
        pop    esi
        add    esp, 10h
        ret    4
sub_1_08D8:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        db  08Bh, 03Dh, 054h, 030h, 000h, 000h
        or     edi, edi
        jne    SHORT loc_1_0904
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 042h, 027h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        add    esp, 10h
        ret    0ch
        db  08Bh, 0FFh
loc_1_0904:
        lea    edx, [esi + 18h]
        mov    dword ptr [edx], 1
        mov    word ptr [esi + 1ch], 0eh
        mov    word ptr [esi + 1eh], 2
        mov    eax, dword ptr [esp + 20h]
        mov    cx, word ptr [eax + 2]
        mov    word ptr [esi + 20h], cx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 22h], eax
        db  068h, 080h, 029h, 000h, 000h
        push   11h
        push   edx
        lea    eax, [esp + 14h]
        push   eax
        call   dword ptr [edi]
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0958
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 047h, 027h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        add    esp, 10h
        ret    0ch
loc_1_0958:
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 2eh], eax
        db  068h, 014h, 00Ch, 000h, 000h
        push   4
        push   eax
        db  0A1h, 054h, 030h, 000h, 000h
        call   dword ptr [eax + 38h]
        add    esp, 10h
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 10h
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_0980:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0ch], 0
        mov    esi, dword ptr [esp + 24h]
        db  08Bh, 015h, 054h, 030h, 000h, 000h
        or     edx, edx
        jne    SHORT loc_1_09B4
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 042h, 027h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    18h
loc_1_09B4:
        lea    edi, [esi + 32h]
        mov    dword ptr [esi + 42h], 4655424eh
        mov    dword ptr [edi], 0
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [esi + 3eh], ecx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esi + 36h], eax
        mov    dword ptr [esi + 3ah], 0
        mov    dword ptr [esi + 18h], 1
        mov    word ptr [esi + 1ch], 0eh
        mov    word ptr [esi + 1eh], 2
        mov    eax, dword ptr [esp + 34h]
        mov    bx, word ptr [eax + 2]
        mov    word ptr [esi + 20h], bx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 22h], eax
        mov    dword ptr [esi + 10h], 16h
        lea    eax, [esi + 18h]
        mov    dword ptr [esi + 14h], eax
        db  0C7h, 044h, 024h, 014h, 01Ch, 00Eh, 000h, 000h
        mov    dword ptr [esp + 18h], 0
        mov    eax, dword ptr [esi + 2eh]
        mov    dword ptr [esp + 10h], eax
        push   edi
        lea    eax, [esp + 10h]
        push   eax
        push   ecx
        push   esi
        lea    eax, [esp + 20h]
        push   eax
        call   dword ptr [edx + 30h]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_0A48
        cmp    eax, 1
        je     SHORT loc_1_0A58
        cmp    eax, 0ffh
        je     SHORT loc_1_0A48
        jmp    SHORT loc_1_0A58
        db  090h
loc_1_0A48:
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    18h
        db  08Dh, 049h, 000h
loc_1_0A58:
        pop    edi
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 047h, 027h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    18h
sub_1_0A70:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        db  083h, 03Dh, 0D8h, 02Dh, 000h, 000h, 000h
        je     SHORT loc_1_0A88
        db  0BAh, 0D8h, 02Dh, 000h, 000h
        jmp    SHORT loc_1_0A98
        db  090h
loc_1_0A88:
        db  0BAh, 08Ch, 02Bh, 000h, 000h
        db  083h, 03Dh, 08Ch, 02Bh, 000h, 000h, 000h
        jne    SHORT loc_1_0A98
        sub    edx, edx
loc_1_0A98:
        or     edx, edx
        je     SHORT loc_1_0AD0
        mov    eax, dword ptr [esp + 18h]
        sub    eax, 240h
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 240h
        lea    esi, [edx + 8]
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [edx], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
        db  090h
loc_1_0AD0:
        lea    edi, [esi + 6eh]
        mov    dword ptr [edi], 1
        mov    dword ptr [esi + 72h], 0
        mov    dword ptr [esi + 76h], 0
        mov    dword ptr [esi + 5ah], 0
        mov    dword ptr [esi + 62h], 0
        db  0A1h, 0C8h, 028h, 000h, 000h
        mov    dword ptr [esi + 5eh], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 66h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 6ah], eax
        and    dword ptr [esi + 4ah], 0fffffffdh
        lea    ebx, [esi + 4eh]
        push   esi
        db  068h, 020h, 00Eh, 000h, 000h
        db  0FFh, 035h, 030h, 02Bh, 000h, 000h
        push   ebx
        call   0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0B40
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 047h, 027h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_1_0B40:
        push   edi
        mov    dword ptr [esi + 46h], edi
        db  089h, 03Dh, 034h, 02Bh, 000h, 000h
        db  089h, 01Dh, 068h, 02Bh, 000h, 000h
        call   sub_1_0036
        add    esp, 4
        db  0C7h, 005h, 068h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 034h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        or     eax, eax
        jne    SHORT loc_1_0B81
        db  0C7h, 005h, 07Ch, 02Bh, 000h, 000h, 079h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
loc_1_0B81:
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
        db  090h
sub_1_0B88:
        db  083h, 03Dh, 0CCh, 028h, 000h, 000h, 000h
        je     SHORT loc_1_0BB4
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        shl    ecx, 3
        add    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 030h, 02Bh, 000h, 000h
        mov    eax, 1
        ret    14h
loc_1_0BB4:
        mov    eax, 1
        db  0C7h, 005h, 030h, 02Bh, 000h, 000h, 0E8h, 003h, 000h, 000h
        ret    14h
        db  08Bh, 0FFh
sub_1_0BC8:
        db  0A1h, 07Ch, 02Bh, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_0BD0:
        sub    eax, eax
        ret    14h
        db  08Dh, 049h, 000h
sub_1_0BD8:
        db  0A1h, 068h, 02Bh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0BF4
        push   eax
        call   6
        add    esp, 4
        db  0C7h, 005h, 068h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0BF4:
        db  0A1h, 034h, 02Bh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0C12
        push   0
        push   eax
        call   sub_1_003C
        add    esp, 8
        db  0C7h, 005h, 034h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0C12:
        ret
        db  090h
loc_1_0C14:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 38h]
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [esp + 14h], 16h
        jb     NEAR PTR loc_1_0CF0
        push   43h
        call   sub_1_0E70
        mov    ecx, dword ptr [esp + 18h]
        cmp    ax, word ptr [ecx + 8]
        jne    NEAR PTR loc_1_0CF0
        mov    eax, dword ptr [esp + 2ch]
        cmp    eax, 0efh
        jb     NEAR PTR loc_1_0CF0
        db  083h, 03Dh, 08Ch, 02Bh, 000h, 000h, 000h
        je     SHORT loc_1_0C7E
        db  0BEh, 08Ch, 02Bh, 000h, 000h
        db  0BFh, 0D8h, 02Dh, 000h, 000h
        mov    ecx, 92h
        rep movsd
        db  0C7h, 005h, 08Ch, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0C7E:
        db  0C7h, 005h, 05Ch, 02Bh, 000h, 000h, 04Eh, 042h, 055h, 046h
        db  0C7h, 005h, 04Ch, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 058h, 02Bh, 000h, 000h, 040h, 002h, 000h, 000h
        db  0C7h, 005h, 050h, 02Bh, 000h, 000h, 094h, 02Bh, 000h, 000h
        db  0C7h, 005h, 054h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 028h, 030h, 000h, 000h, 0FCh, 00Ch, 000h, 000h
        db  0A3h, 024h, 030h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 02Ch, 030h, 000h, 000h
        db  0C7h, 005h, 020h, 030h, 000h, 000h, 04Ch, 02Bh, 000h, 000h
        db  0C7h, 005h, 030h, 030h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 003h, 020h, 030h, 000h, 000h
        or     dword ptr [eax + 4ah], 1
        mov    eax, 18h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0CF0:
        pop    edi
        mov    eax, 17h
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_0CFC:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 2ch]
        and    dword ptr [ebx + 4ah], 0fffffffeh
        mov    eax, dword ptr [esp + 30h]
        or     eax, eax
        je     SHORT loc_1_0D40
        db  0A3h, 07Ch, 02Bh, 000h, 000h
        mov    eax, dword ptr [ebx + 46h]
        or     eax, eax
        je     NEAR PTR loc_1_0E12
        push   0
        push   eax
        call   sub_1_003C
        add    esp, 8
        mov    dword ptr [ebx + 46h], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_0D40:
        db  0BEh, 0B0h, 02Bh, 000h, 000h
        db  00Fh, 0B6h, 005h, 096h, 02Bh, 000h, 000h
        mov    dword ptr [esp + 14h], eax
        db  00Fh, 0B6h, 005h, 096h, 02Bh, 000h, 000h
        lea    edi, [esp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 14h]
        push   eax
        db  068h, 084h, 02Bh, 000h, 000h
        call   sub_1_0364
        add    esp, 8
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     SHORT loc_1_0DDC
        db  0C7h, 005h, 08Ch, 02Bh, 000h, 000h, 001h, 000h, 000h, 000h
        mov    ebp, dword ptr [esp + 34h]
        db  089h, 02Dh, 090h, 02Bh, 000h, 000h
        cmp    dword ptr [ebx + 46h], 0
        je     SHORT loc_1_0DE0
        db  083h, 03Dh, 0D8h, 02Dh, 000h, 000h, 000h
        db  0B8h, 0D8h, 02Dh, 000h, 000h
        jne    SHORT loc_1_0DB0
        db  0B8h, 08Ch, 02Bh, 000h, 000h
loc_1_0DB0:
        mov    ebp, dword ptr [eax + 4]
        lea    esi, [eax + 8]
        mov    edi, dword ptr [ebx + 66h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    dword ptr [eax], 0
        lea    eax, [ebx + 4eh]
        push   eax
        call   6
        add    esp, 4
        jmp    SHORT loc_1_0DE0
        db  090h
loc_1_0DDC:
        mov    ebp, dword ptr [esp + 34h]
loc_1_0DE0:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_0DEF
        test   byte ptr [ebx + 4ah], 2
        je     SHORT loc_1_0DEF
        sub    ebp, ebp
loc_1_0DEF:
        mov    eax, dword ptr [ebx + 46h]
        or     eax, eax
        je     SHORT loc_1_0E12
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_0E01
        or     ebp, ebp
        jne    SHORT loc_1_0E12
loc_1_0E01:
        push   ebp
        push   eax
        call   sub_1_003C
        add    esp, 8
        mov    dword ptr [ebx + 46h], 0
loc_1_0E12:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_0E1C:
        ret
        db  08Dh, 049h, 000h
loc_1_0E20:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 4ah]
        test   cl, 1
        je     SHORT loc_1_0E34
        or     ecx, 2
        mov    dword ptr [eax + 4ah], ecx
        ret
        db  090h
loc_1_0E34:
        push   0
        push   15h
        push   eax
        call   sub_1_0CFC
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
sub_1_0E44:
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        and    eax, 0ff00h
        mov    ecx, edx
        shl    ecx, 10h
        or     eax, ecx
        shl    eax, 8
        mov    ecx, edx
        and    ecx, 0ff0000h
        shr    edx, 10h
        or     ecx, edx
        shr    ecx, 8
        or     eax, ecx
        ret    4
        db  08Bh, 0FFh
sub_1_0E70:
        mov    cx, word ptr [esp + 4]
        mov    ah, cl
        mov    al, ch
        ret    4
sub_1_0E7C:
        push   dword ptr [esp + 4]
        call   sub_1_0E44
        ret    4
        db  0FFh, 074h, 024h, 004h, 0E8h, 0DFh, 0FFh, 0FFh, 0FFh, 0C2h, 004h, 000h
VDHCP_PM_API:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  edi, word ptr [ebp + 1ch]
        movzx  eax, word ptr [ebp + 10h]
        push   eax
        movzx  eax, word ptr [ebp + 38h]
        push   eax
        push   ebx
        call   sub_1_1188
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_0EF9
        or     eax, eax
        je     SHORT loc_1_0ECD
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   eax
        call   sub_1_11D0
        add    esp, 8
        or     eax, eax
        cmp    eax, -1
        je     SHORT loc_1_0EF9
loc_1_0ECD:
        mov    esi, eax
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   esi
        push   edi
        call   sub_1_16B8
        add    esp, 0ch
        mov    word ptr [ebp + 1ch], ax
        or     esi, esi
        je     SHORT loc_1_0EF4
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   esi
        call   sub_1_1217
        add    esp, 8
loc_1_0EF4:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_0EF9:
        cmp    eax, -1
        mov    word ptr [ebp + 1ch], ax
        jmp    SHORT loc_1_0EF4
loc_1_0F02:
        ret
sub_1_0F03:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 004h, 000h, 089h, 004h
        add    esp, 0ch
        pop    ebp
        ret
sub_1_0F1D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        db  0C7h, 005h, 078h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp + 8]
        db  068h, 045h, 00Fh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
loc_1_0F45:
        push   ebp
        mov    ebp, esp
        pushfd
        pushad
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 10001034h
        mov    ecx, dword ptr [ebp + 8]
        db  08Bh, 03Dh, 07Ch, 029h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        call   sub_1_0F6D
        popad
        popfd
        pop    ebp
        ret
sub_1_0F6D:
        cmp    eax, 7
        je     SHORT loc_1_0F7C
        db  0C7h, 005h, 078h, 02Bh, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_0F7C:
        db  0E8h, 07Fh, 012h, 0FFh, 0FFh
        ret
VDHCP_Service_00:
        mov    ax, 100h
        clc
        ret
VDHCP_Service_01:
        db  0E9h, 0D7h, 0F0h, 0FFh, 0FFh
VDHCP_Service_02:
        jmp    NEAR PTR loc_1_0164
VDHCP_Service_03:
        db  0E9h, 061h, 0FEh, 0FEh, 0FFh
VDHCP_Control:
        cmp    eax, 1
        db  00Fh, 084h, 03Ch, 0FAh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 033h, 0FAh, 0FEh, 0FFh
        cmp    eax, 23h
        je     SHORT loc_1_100D
        cmp    eax, 4
        je     NEAR PTR loc_1_0F02
        clc
        ret
        db  0CDh, 020h, 011h, 001h, 001h, 000h, 03Dh, 000h, 000h, 000h, 020h, 0B8h, 000h, 000h, 000h, 000h
        db  076h, 01Ch, 053h, 0CDh, 020h, 004h, 000h, 015h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 0F7h
        db  003h, 020h, 000h, 000h, 000h, 075h, 003h, 003h, 043h, 004h, 00Fh, 0B7h, 000h, 05Bh, 0C3h
sub_1_0FE8:
        push   edi
        push   esi
        mov    eax, 0
        mov    ebx, 0
        mov    ecx, 40h
        mov    edx, 0
        mov    esi, dword ptr [esp + 0ch]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    edi
        ret
loc_1_100D:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 1
        jb     SHORT loc_1_105A
        cmp    ecx, 3
        ja     SHORT loc_1_105A
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_1034
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_1_11D0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_1078
loc_1_1034:
        mov    dword ptr [esi + 10h], eax
        mov    ecx, dword ptr [esi + 14h]
        push   ecx
        push   eax
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        call   sub_1_16B8
        add    esp, 0ch
        push   eax
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        call   sub_1_1217
        add    esp, 8
        pop    eax
        jmp    SHORT loc_1_107F
loc_1_105A:
        cmp    ecx, 0
        je     SHORT loc_1_106B
        cmp    ecx, -1
        je     SHORT loc_1_1074
        mov    eax, 1
        jmp    SHORT loc_1_107F
loc_1_106B:
        xor    eax, eax
        mov    ecx, 0
        jmp    SHORT loc_1_107F
loc_1_1074:
        xor    eax, eax
        jmp    SHORT loc_1_107F
loc_1_1078:
        mov    eax, 6f8h
        jmp    SHORT loc_1_107F
loc_1_107F:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1083:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        cmp    ebx, 44ch
        jne    SHORT loc_1_10A2
        db  00Fh, 0B7h, 005h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_112E
loc_1_10A2:
        cmp    ebx, 44dh
        jne    SHORT loc_1_10B9
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_112E
loc_1_10B9:
        cmp    ebx, 44eh
        jne    SHORT loc_1_10D0
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_112E
loc_1_10D0:
        cmp    ebx, 44fh
        jne    SHORT loc_1_10E7
        db  00Fh, 0B7h, 005h, 012h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_112E
loc_1_10E7:
        cmp    ebx, 450h
        jne    SHORT loc_1_10FE
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_112E
loc_1_10FE:
        cmp    ebx, 451h
        jne    SHORT loc_1_1115
        db  00Fh, 0B7h, 005h, 016h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_112E
loc_1_1115:
        cmp    ebx, 452h
        jne    SHORT loc_1_112C
        db  00Fh, 0B7h, 005h, 018h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_112E
loc_1_112C:
        xor    eax, eax
loc_1_112E:
        pop    ebx
        ret
sub_1_1130:
        db  0B8h, 0F0h, 029h, 000h, 000h
        push   eax
        db  0CDh, 020h, 008h, 000h, 088h, 004h
        add    esp, 4
        cmp    eax, 0
        jne    SHORT loc_1_114A
        mov    eax, 1
        ret
loc_1_114A:
        db  0A3h, 054h, 030h, 000h, 000h
        mov    eax, 0
        ret
loc_1_1155:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        db  0CDh, 020h, 007h, 000h, 089h, 004h
        add    esp, 8
        ret
sub_1_1169:
        mov    eax, dword ptr [esp + 4]
        push   eax
        db  0CDh, 020h, 008h, 000h, 089h, 004h
        add    esp, 4
        ret
sub_1_1178:
        mov    eax, 0ffffffffh
        push   eax
        db  0CDh, 020h, 008h, 000h, 089h, 004h
        add    esp, 4
        ret
sub_1_1188:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 14h]
        movzx  eax, word ptr [esp + 18h]
        movzx  esi, word ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_11C4
        test   dword ptr [ebx], 20h
        je     SHORT loc_1_11C9
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_11C4
        add    eax, esi
        cmp    eax, 110000h
        jae    SHORT loc_1_11C4
loc_1_11C1:
        add    eax, dword ptr [ebx + 4]
loc_1_11C4:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_11C9:
        shl    eax, 4
        add    eax, esi
        jmp    SHORT loc_1_11C1
sub_1_11D0:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_120F
        or     ebx, ebx
        je     SHORT loc_1_120F
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
        je     SHORT loc_1_1213
        add    eax, esi
loc_1_120F:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_1213:
        xor    eax, eax
        jmp    SHORT loc_1_120F
sub_1_1217:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_1252
        or     ebx, ebx
        je     SHORT loc_1_1252
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
        je     SHORT loc_1_125B
loc_1_1252:
        mov    eax, 1
loc_1_1257:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_125B:
        xor    eax, eax
        jmp    SHORT loc_1_1257
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_12A8:
        push   ebx
        push   esi
        push   edi
        mov    cl, byte ptr [esp + 14h]
        cmp    cl, 0ffh
        jne    SHORT loc_1_12C0
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 0ffh
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_12C0:
        or     cl, cl
        jne    SHORT loc_1_12E0
        mov    eax, dword ptr [esp + 10h]
        lea    ecx, [eax + 1]
        mov    edi, dword ptr [esp + 20h]
        dec    edi
        cmp    edi, ecx
        jbe    SHORT loc_1_131C
        mov    byte ptr [eax], 0
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_12E0:
        mov    bl, byte ptr [esp + 1ch]
        movzx  edx, bl
        mov    eax, dword ptr [esp + 10h]
        lea    esi, [edx + eax]
        add    esi, 2
        mov    edi, dword ptr [esp + 20h]
        dec    edi
        cmp    esi, edi
        jae    SHORT loc_1_131C
        mov    byte ptr [eax], cl
        mov    byte ptr [eax + 1], bl
        lea    edi, [eax + 2]
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [eax + 1]
        lea    eax, [ecx + eax + 2]
loc_1_131C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1320:
        sub    esp, 14h
        push   esi
        push   edi
        mov    al, byte ptr [esp + 24h]
        mov    byte ptr [esp + 8], al
        mov    dl, byte ptr [esp + 2ch]
        movzx  eax, dl
        lea    edi, [esp + 9]
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 30h]
        inc    dl
        push   edx
        lea    eax, [esp + 10h]
        push   eax
        push   3dh
        push   dword ptr [esp + 30h]
        call   sub_1_12A8
        add    esp, 14h
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Bh, 0FFh
sub_1_136C:
        mov    eax, dword ptr [esp + 4]
        lea    edx, [eax + 4]
        mov    ecx, dword ptr [esp + 8]
        dec    ecx
        cmp    edx, ecx
        jae    SHORT loc_1_138C
        mov    byte ptr [eax], 63h
        inc    eax
        mov    byte ptr [eax], 82h
        inc    eax
        mov    byte ptr [eax], 53h
        inc    eax
        mov    byte ptr [eax], 63h
        inc    eax
loc_1_138C:
        ret
        db  08Dh, 049h, 000h
sub_1_1390:
        mov    eax, dword ptr [esp + 4]
        test   eax, 80000000h
        jne    SHORT loc_1_13A4
        mov    eax, 0ff000000h
        ret
        db  08Dh, 049h, 000h
loc_1_13A4:
        mov    ecx, eax
        and    ecx, 0c0000000h
        cmp    ecx, 80000000h
        jne    SHORT loc_1_13BC
        mov    eax, 0ffff0000h
        ret
        db  08Bh, 0FFh
loc_1_13BC:
        and    eax, 0e0000000h
        cmp    eax, 0c0000000h
        jne    SHORT loc_1_13D0
        mov    eax, 0ffffff00h
        ret
        db  08Bh, 0FFh
loc_1_13D0:
        mov    eax, 0ffffffffh
        ret
        db  08Bh, 0FFh
sub_1_13D8:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        cmp    dword ptr [esp + 28h], esi
        jne    SHORT loc_1_13F4
        mov    eax, 6fh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_13F4:
        mov    edi, 4
        db  08Bh, 01Dh, 084h, 02Bh, 000h, 000h
        db  081h, 0FBh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_1_1465
        nop
loc_1_1408:
        inc    esi
        mov    eax, dword ptr [ebx + 0dh]
        add    eax, 2ch
        add    edi, eax
        mov    dword ptr [esp + 18h], 0
        lea    eax, [esp + 18h]
        push   eax
        push   0
        push   6
        push   dword ptr [ebx + 11h]
        db  0E8h, 03Ah, 0ECh, 0FFh, 0FFh
        add    esp, 10h
        cmp    eax, 21h
        jne    SHORT loc_1_1436
        add    edi, dword ptr [esp + 18h]
loc_1_1436:
        lea    eax, [esp + 18h]
        mov    dword ptr [esp + 18h], 0
        push   eax
        push   0
        push   0fh
        push   dword ptr [ebx + 11h]
        db  0E8h, 015h, 0ECh, 0FFh, 0FFh
        add    esp, 10h
        cmp    eax, 21h
        jne    SHORT loc_1_145B
        add    edi, dword ptr [esp + 18h]
loc_1_145B:
        mov    ebx, dword ptr [ebx]
        db  081h, 0FBh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_1_1408
loc_1_1465:
        cmp    edi, dword ptr [esp + 28h]
        jbe    SHORT loc_1_1488
        cmp    dword ptr [esp + 28h], 4
        jb     SHORT loc_1_1478
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax], edi
loc_1_1478:
        pop    ebp
        mov    eax, 6fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_1488:
        lea    ebx, [esi + esi*4]
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax], esi
        add    eax, 4
        mov    dword ptr [esp + 1ch], eax
        lea    ebx, [ebx + ebx*8]
        sub    ebx, esi
        add    ebx, 4
        db  0A1h, 084h, 02Bh, 000h, 000h
        mov    dword ptr [esp + 10h], eax
        db  03Dh, 084h, 02Bh, 000h, 000h
        je     NEAR PTR loc_1_15C7
loc_1_14B4:
        mov    ebp, dword ptr [esp + 1ch]
        add    dword ptr [esp + 1ch], 2ch
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax + 0dh]
        mov    dword ptr [ebp + 4], edx
        mov    eax, dword ptr [eax + 11h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 21h]
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 25h]
        mov    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 31h]
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 19h]
        mov    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp], ebx
        mov    edi, dword ptr [esp + 24h]
        add    edi, ebx
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 9]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        add    ebx, dword ptr [eax + 0dh]
        mov    eax, dword ptr [esp + 28h]
        sub    eax, ebx
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        add    eax, ebx
        push   eax
        push   6
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax + 11h]
        db  0E8h, 026h, 0EBh, 0FFh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_1558
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [ebp + 20h], ebx
        add    ebx, dword ptr [esp + 14h]
        jmp    SHORT loc_1_1566
        db  08Dh, 049h, 000h
loc_1_1558:
        mov    dword ptr [ebp + 1ch], 0
        mov    dword ptr [ebp + 20h], 0
loc_1_1566:
        mov    eax, dword ptr [esp + 28h]
        sub    eax, ebx
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        add    eax, ebx
        push   eax
        push   0fh
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax + 11h]
        db  0E8h, 0DAh, 0EAh, 0FFh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_15A4
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 24h], eax
        mov    dword ptr [ebp + 28h], ebx
        add    ebx, dword ptr [esp + 14h]
        jmp    SHORT loc_1_15B2
        db  08Dh, 049h, 000h
loc_1_15A4:
        mov    dword ptr [ebp + 24h], 0
        mov    dword ptr [ebp + 28h], 0
loc_1_15B2:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 10h], eax
        db  03Dh, 084h, 02Bh, 000h, 000h
        jne    NEAR PTR loc_1_14B4
loc_1_15C7:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_15D4:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 015h, 084h, 02Bh, 000h, 000h
        db  081h, 0FAh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_1_1614
        mov    ebx, dword ptr [esp + 10h]
loc_1_15E9:
        mov    eax, dword ptr [ebx + 4]
        cmp    dword ptr [edx + 0dh], eax
        jne    SHORT loc_1_160A
        mov    edi, dword ptr [ebx]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [edx + 9]
        add    edi, ebx
        repe cmpsd
        jne    SHORT loc_1_1608
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_1_1608:
        je     SHORT loc_1_1620
loc_1_160A:
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_1_15E9
loc_1_1614:
        pop    edi
        mov    eax, 3
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_1620:
        mov    ecx, dword ptr [edx + 39h]
        mov    eax, dword ptr [edx + 3dh]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [edx + 11h], 0
        push   0
        push   edx
        jne    SHORT loc_1_1640
        call   sub_1_21B8
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1640:
        call   sub_1_22A4
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_164C:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 084h, 02Bh, 000h, 000h
        db  081h, 0FBh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_1_169D
        mov    edx, dword ptr [esp + 10h]
loc_1_1661:
        mov    eax, dword ptr [edx + 4]
        cmp    dword ptr [ebx + 0dh], eax
        jne    SHORT loc_1_1693
        mov    edi, dword ptr [edx]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebx + 9]
        add    edi, edx
        repe cmpsd
        jne    SHORT loc_1_1693
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_1693
        mov    ecx, dword ptr [ebx + 39h]
        mov    eax, dword ptr [ebx + 3dh]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [ebx + 11h], 0
        jne    SHORT loc_1_16A8
loc_1_1693:
        mov    ebx, dword ptr [ebx]
        db  081h, 0FBh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_1_1661
loc_1_169D:
        pop    edi
        mov    eax, 3
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_16A8:
        push   ebx
        call   sub_1_2180
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_16B8:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        je     SHORT loc_1_16D4
        cmp    eax, 2
        je     SHORT loc_1_16E8
        cmp    eax, 3
        je     SHORT loc_1_16FC
        mov    eax, 57h
        ret
        db  08Dh, 049h, 000h
loc_1_16D4:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_13D8
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_16E8:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_15D4
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_16FC:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_164C
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_1710:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 50000000h
        jne    SHORT loc_1_1775
        call   sub_1_1780
        or     esi, esi
        je     SHORT loc_1_1775
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_1753
        call   sub_1_17AA
loc_1_174C:
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
loc_1_1753:
        push   esi
        mov    edx, esi
        mov    eax, 1000h
        mov    ecx, 0
        db  0BEh, 098h, 017h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        test   esi, esi
        pop    esi
        jne    SHORT loc_1_174C
        call   sub_1_1791
loc_1_1775:
        push   dword ptr [ebp + 0ch]
        call   dword ptr [ebp + 8]
        add    esp, 4
        jmp    SHORT loc_1_174C
sub_1_1780:
        db  08Bh, 035h, 020h, 02Bh, 000h, 000h
        db  0C7h, 005h, 020h, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_1791:
        db  089h, 035h, 020h, 02Bh, 000h, 000h
        ret
loc_1_1798:
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    esi, edx
        call   sub_1_17AA
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_1_17AA:
        push   esi
        mov    ecx, 1000h
        mov    edx, esi
        db  0BBh, 0C1h, 017h, 000h, 000h
        mov    esi, ebx
        db  0CDh, 020h, 013h, 000h, 02Ah, 000h
        pop    esi
        ret
loc_1_17C1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [esi]
        add    esp, 4
        call   sub_1_1791
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
sub_1_17E0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        lea    esi, [eax + eax*4]
        lea    esi, [esi + esi*4]
        lea    esi, [esi + esi*4]
        shl    esi, 5
        db  0E8h, 0B2h, 0FFh, 0FEh, 0FFh
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 4
        mov    ecx, 7fffh
        sub    edx, edx
        div    ecx
        lea    eax, [eax + esi - 1]
        push   eax
        db  0E8h, 03Bh, 000h, 0FFh, 0FFh
        add    esp, 4
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_181C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 10h], esi
        mov    ebx, dword ptr [esp + 20h]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 6
        rep stosd
        push   dword ptr [esp + 18h]
        call   sub_1_051C
        add    esp, 4
        mov    ebp, dword ptr [esp + 24h]
        or     ebp, ebp
        je     NEAR PTR loc_1_193D
        cmp    byte ptr [esi], 63h
        jne    NEAR PTR loc_1_193D
        cmp    byte ptr [esi + 1], 82h
        jne    NEAR PTR loc_1_193D
        cmp    byte ptr [esi + 2], 53h
        jne    NEAR PTR loc_1_193D
        cmp    byte ptr [esi + 3], 63h
        jne    NEAR PTR loc_1_193D
        add    esi, 4
        cmp    byte ptr [esi], 0ffh
        je     NEAR PTR loc_1_193D
loc_1_1885:
        mov    al, byte ptr [esi]
        or     al, al
        je     SHORT loc_1_189C
        cmp    al, 0ffh
        je     SHORT loc_1_189C
        lea    edx, [esi + 1]
        movzx  ecx, byte ptr [edx]
        lea    edi, [ecx + esi + 2]
        jmp    SHORT loc_1_18A1
        db  090h
loc_1_189C:
        lea    edx, [esi + 1]
        mov    edi, edx
loc_1_18A1:
        mov    ecx, edi
        sub    ecx, dword ptr [esp + 10h]
        cmp    ecx, ebp
        jg     NEAR PTR loc_1_193D
        movzx  ecx, al
        cmp    ecx, 36h
        jg     SHORT loc_1_18CC
        je     SHORT loc_1_191C
        cmp    ecx, 1
        je     SHORT loc_1_1904
        cmp    ecx, 33h
        je     SHORT loc_1_190C
        cmp    ecx, 35h
        je     SHORT loc_1_1914
        jmp    SHORT loc_1_18D6
        db  08Bh, 0FFh
loc_1_18CC:
        cmp    ecx, 3ah
        je     SHORT loc_1_1924
        cmp    ecx, 3bh
        je     SHORT loc_1_192C
loc_1_18D6:
        movzx  eax, byte ptr [edx]
        push   eax
        lea    eax, [esi + 2]
        push   eax
        push   ecx
        push   dword ptr [esp + 24h]
        call   sub_1_0520
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1932
        push   esi
        push   3eah
        db  068h, 024h, 02Bh, 000h, 000h
        db  0E8h, 0C1h, 000h, 0FFh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_1_1932
loc_1_1904:
        add    esi, 2
        mov    dword ptr [ebx + 4], esi
        jmp    SHORT loc_1_1932
loc_1_190C:
        add    esi, 2
        mov    dword ptr [ebx + 8], esi
        jmp    SHORT loc_1_1932
loc_1_1914:
        add    esi, 2
        mov    dword ptr [ebx], esi
        jmp    SHORT loc_1_1932
        db  090h
loc_1_191C:
        add    esi, 2
        mov    dword ptr [ebx + 0ch], esi
        jmp    SHORT loc_1_1932
loc_1_1924:
        add    esi, 2
        mov    dword ptr [ebx + 10h], esi
        jmp    SHORT loc_1_1932
loc_1_192C:
        add    esi, 2
        mov    dword ptr [ebx + 14h], esi
loc_1_1932:
        mov    esi, edi
        cmp    byte ptr [edi], 0ffh
        jne    NEAR PTR loc_1_1885
loc_1_193D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_1948:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edx + 4dh]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 89h
        rep stosd
        mov    byte ptr [ebx], 1
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [edx + 45h]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [edx + 0dh]
        lea    edi, [ebx + 1ch]
        mov    esi, dword ptr [edx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [edx + 0dh]
        mov    byte ptr [ebx + 2], al
        lea    eax, [ebx + 0ech]
        add    ebx, 224h
        push   ebx
        push   eax
        call   sub_1_136C
        add    esp, 8
        mov    byte ptr [esp + 0fh], 1
        push   ebx
        push   1
        lea    ecx, [esp + 17h]
        push   ecx
        push   35h
        push   eax
        call   sub_1_12A8
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_19C4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_1948
        add    esp, 4
        mov    ebx, dword ptr [esi + 4dh]
        add    ebx, 224h
        push   ebx
        push   dword ptr [esi + 0dh]
        push   dword ptr [esi + 9]
        mov    cl, byte ptr [esi + 8]
        push   ecx
        push   eax
        call   sub_1_1320
        add    esp, 14h
        mov    edx, eax
        lea    eax, [esi + 1dh]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_1A0D
        push   ebx
        push   4
        push   eax
        push   32h
        push   edx
        call   sub_1_12A8
        add    esp, 14h
        mov    edx, eax
loc_1_1A0D:
        db  08Bh, 02Dh, 058h, 030h, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_1A34
        push   ebx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebp
        push   0ch
        push   edx
        call   sub_1_12A8
        add    esp, 14h
        mov    edx, eax
loc_1_1A34:
        push   ebx
        push   0
        push   0
        push   0ffh
        push   edx
        call   sub_1_12A8
        add    esp, 14h
        sub    eax, dword ptr [esi + 4dh]
        push   dword ptr [esp + 18h]
        push   eax
        push   esi
        call   sub_1_24B8
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_1A60:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edx + 4dh]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 89h
        rep stosd
        mov    byte ptr [ebx], 1
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [edx + 45h]
        cmp    dword ptr [esp + 18h], 0
        mov    word ptr [ebx + 8], ax
        je     SHORT loc_1_1A95
        mov    eax, dword ptr [edx + 1dh]
        mov    dword ptr [ebx + 0ch], eax
loc_1_1A95:
        mov    eax, dword ptr [edx + 0dh]
        lea    edi, [ebx + 1ch]
        mov    esi, dword ptr [edx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [edx + 0dh]
        mov    byte ptr [ebx + 2], al
        lea    eax, [ebx + 0ech]
        add    ebx, 224h
        push   ebx
        push   eax
        call   sub_1_136C
        add    esp, 8
        mov    byte ptr [esp + 0fh], 3
        push   ebx
        push   1
        lea    ecx, [esp + 17h]
        push   ecx
        push   35h
        push   eax
        call   sub_1_12A8
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_1AE8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        push   edi
        mov    esi, dword ptr [esp + 18h]
        push   esi
        call   sub_1_1A60
        add    esp, 8
        mov    ebx, dword ptr [esi + 4dh]
        add    ebx, 224h
        push   ebx
        push   dword ptr [esi + 0dh]
        push   dword ptr [esi + 9]
        mov    cl, byte ptr [esi + 8]
        push   ecx
        push   eax
        call   sub_1_1320
        add    esp, 14h
        mov    edx, eax
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_1B3D
        or     edi, edi
        jne    SHORT loc_1_1B3D
        push   ebx
        push   4
        lea    eax, [esp + 24h]
        push   eax
        push   32h
        push   edx
        call   sub_1_12A8
        add    esp, 14h
        mov    edx, eax
loc_1_1B3D:
        cmp    dword ptr [esp + 20h], -1
        je     SHORT loc_1_1B59
        push   ebx
        push   4
        lea    eax, [esp + 28h]
        push   eax
        push   36h
        push   edx
        call   sub_1_12A8
        add    esp, 14h
        mov    edx, eax
loc_1_1B59:
        db  08Bh, 02Dh, 058h, 030h, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_1B80
        push   ebx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebp
        push   0ch
        push   edx
        call   sub_1_12A8
        add    esp, 14h
        mov    edx, eax
loc_1_1B80:
        push   ebx
        push   edx
        call   sub_1_03FC
        add    esp, 8
        push   ebx
        push   0
        push   0
        push   0ffh
        push   eax
        call   sub_1_12A8
        add    esp, 14h
        push   dword ptr [esp + 18h]
        sub    eax, dword ptr [esi + 4dh]
        push   eax
        push   esi
        call   sub_1_24B8
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_1BB4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [ebx + 4dh]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 89h
        rep stosd
        mov    byte ptr [ebp], 1
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 1], al
        mov    eax, dword ptr [ebx + 45h]
        mov    word ptr [ebp + 8], ax
        push   8000h
        call   sub_1_0E70
        mov    word ptr [ebp + 0ah], ax
        mov    eax, dword ptr [ebx + 11h]
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [ebx + 0dh]
        lea    edi, [ebp + 1ch]
        mov    esi, dword ptr [ebx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [ebx + 0dh]
        mov    byte ptr [ebp + 2], al
        lea    eax, [ebp + 0ech]
        lea    esi, [ebp + 224h]
        push   esi
        push   eax
        call   sub_1_136C
        add    esp, 8
        mov    byte ptr [esp + 13h], 7
        push   esi
        push   1
        lea    ecx, [esp + 1bh]
        push   ecx
        push   35h
        push   eax
        call   sub_1_12A8
        add    esp, 14h
        push   esi
        push   4
        lea    ecx, [ebx + 19h]
        push   ecx
        push   36h
        push   eax
        call   sub_1_12A8
        add    esp, 14h
        push   esi
        push   dword ptr [ebx + 0dh]
        push   dword ptr [ebx + 9]
        mov    cl, byte ptr [ebx + 8]
        push   ecx
        push   eax
        call   sub_1_1320
        add    esp, 14h
        push   esi
        push   0
        push   0
        push   0ffh
        push   eax
        call   sub_1_12A8
        add    esp, 14h
        sub    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_1C80:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], 0
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_1_1BB4
        add    esp, 4
        lea    ecx, [esp + 4]
        push   ecx
        push   eax
        push   esi
        call   sub_1_24B8
        add    esp, 0ch
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_1CB0:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 18h], 0ffffffffh
        mov    dword ptr [esp + 20h], 0ffffffffh
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 28h], 40h
        push   0
        db  0E8h, 0EAh, 0FAh, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 2ch], eax
        mov    esi, dword ptr [esp + 34h]
        mov    dword ptr [esi + 45h], 0
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 38h]
loc_1_1CF6:
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        call   sub_1_19C4
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_1_1FA4
        mov    ecx, 4
        sub    ecx, ebx
        mov    eax, dword ptr [esp + 28h]
        sub    edx, edx
        div    ecx
        mov    ebp, eax
        push   0
        db  0E8h, 0A4h, 0FAh, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 14h], eax
        or     ebp, ebp
        je     NEAR PTR loc_1_1DFA
        nop
loc_1_1D34:
        mov    eax, ebp
        mov    dword ptr [esp + 10h], 240h
        sub    eax, 5
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 5
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_1_2588
        add    esp, 10h
        cmp    eax, 79h
        je     NEAR PTR loc_1_1DFA
        or     eax, eax
        jne    NEAR PTR loc_1_1FA4
        mov    eax, dword ptr [esp + 10h]
        sub    eax, 0ech
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4dh]
        add    eax, 0ech
        push   eax
        push   esi
        call   sub_1_181C
        add    esp, 10h
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_1DE0
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_1DE0
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_1_1DA1
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 24h], eax
loc_1_1DA1:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        mov    ecx, 0
        je     SHORT loc_1_1DAF
        mov    ecx, dword ptr [eax]
loc_1_1DAF:
        mov    eax, dword ptr [esi + 4dh]
        mov    edx, dword ptr [eax + 10h]
        cmp    dword ptr [esp + 18h], -1
        jne    SHORT loc_1_1DC8
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 20h], edx
loc_1_1DC8:
        mov    eax, dword ptr [esi + 1dh]
        or     eax, eax
        je     SHORT loc_1_1E44
        cmp    eax, edx
        je     SHORT loc_1_1E44
        cmp    ebx, 2
        jae    SHORT loc_1_1E44
        and    eax, ecx
        and    ecx, edx
        cmp    eax, ecx
        jne    SHORT loc_1_1E44
loc_1_1DE0:
        push   0
        db  0E8h, 0E1h, 0F9h, 0FEh, 0FFh
        add    esp, 4
        sub    ebp, eax
        add    ebp, dword ptr [esp + 14h]
        mov    dword ptr [esp + 14h], eax
        jne    NEAR PTR loc_1_1D34
loc_1_1DFA:
        inc    ebx
        push   ebx
        call   sub_1_17E0
        add    esp, 4
        push   0
        db  0E8h, 0BDh, 0F9h, 0FEh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [esp + 2ch]
        sub    dword ptr [esp + 28h], eax
        add    dword ptr [esp + 28h], ecx
        mov    edx, eax
        sub    edx, ecx
        add    dword ptr [esi + 45h], edx
        mov    dword ptr [esp + 2ch], eax
        cmp    ebx, 4
        jb     NEAR PTR loc_1_1CF6
        cmp    dword ptr [esp + 20h], -1
        jne    SHORT loc_1_1E50
        mov    eax, 79h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
loc_1_1E44:
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 20h], edx
loc_1_1E50:
        push   0
        mov    dword ptr [esp + 20h], 0
        mov    dword ptr [esp + 2ch], 40h
        db  0E8h, 061h, 0F9h, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 2ch], eax
        sub    ebp, ebp
loc_1_1E70:
        push   0
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 28h]
        push   eax
        push   esi
        call   sub_1_1AE8
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_1_1FA4
        mov    ecx, 4
        sub    ecx, ebp
        mov    eax, dword ptr [esp + 28h]
        sub    edx, edx
        div    ecx
        mov    ebx, eax
        push   0
        db  0E8h, 020h, 0F9h, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 14h], eax
        or     ebx, ebx
        je     NEAR PTR loc_1_1F6E
        nop
loc_1_1EB8:
        mov    eax, ebx
        mov    dword ptr [esp + 10h], 240h
        sub    eax, 5
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 5
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_1_2588
        add    esp, 10h
        cmp    eax, 79h
        je     NEAR PTR loc_1_1F6E
        or     eax, eax
        jne    NEAR PTR loc_1_1FA4
        mov    eax, dword ptr [esp + 10h]
        sub    eax, 0ech
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4dh]
        add    eax, 0ech
        push   eax
        push   esi
        call   sub_1_181C
        add    esp, 10h
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_1F54
        mov    al, byte ptr [eax]
        cmp    al, 5
        jne    SHORT loc_1_1F50
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_1_1F24
        mov    ecx, dword ptr [eax]
        jmp    SHORT loc_1_1F28
loc_1_1F24:
        mov    ecx, dword ptr [esp + 18h]
loc_1_1F28:
        mov    edx, dword ptr [esi + 4dh]
        mov    eax, dword ptr [esp + 20h]
        cmp    dword ptr [edx + 10h], eax
        jne    SHORT loc_1_1F54
        cmp    ecx, dword ptr [esp + 18h]
        jne    SHORT loc_1_1F54
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_1_1FAC
        push   dword ptr [eax]
        call   sub_1_0E7C
        mov    dword ptr [esi + 21h], eax
        jmp    SHORT loc_1_1FB3
        db  08Dh, 049h, 000h
loc_1_1F50:
        cmp    al, 6
        je     SHORT loc_1_1FCC
loc_1_1F54:
        push   0
        db  0E8h, 06Dh, 0F8h, 0FEh, 0FFh
        add    esp, 4
        sub    ebx, eax
        add    ebx, dword ptr [esp + 14h]
        mov    dword ptr [esp + 14h], eax
        jne    NEAR PTR loc_1_1EB8
loc_1_1F6E:
        inc    ebp
        push   ebp
        call   sub_1_17E0
        add    esp, 4
        push   0
        db  0E8h, 049h, 0F8h, 0FEh, 0FFh
        add    esp, 4
        sub    dword ptr [esp + 28h], eax
        mov    ecx, dword ptr [esp + 28h]
        add    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 28h], ecx
        mov    dword ptr [esp + 2ch], eax
        cmp    ebp, 4
        jb     NEAR PTR loc_1_1E70
        mov    eax, 79h
loc_1_1FA4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_1FAC:
        mov    dword ptr [esi + 21h], 0e10h
loc_1_1FB3:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 19h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 11h], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_1FCC:
        pop    ebp
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
sub_1_1FDC:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 20h], 18h
        push   0
        db  0E8h, 0CEh, 0F7h, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 24h], eax
        mov    esi, dword ptr [esp + 2ch]
        mov    dword ptr [esi + 45h], 0
        mov    dword ptr [esp + 1ch], 0
        mov    edi, dword ptr [esp + 30h]
loc_1_2018:
        push   dword ptr [esi + 35h]
        push   -1
        push   dword ptr [esi + 1dh]
        lea    eax, [esp + 24h]
        push   eax
        push   esi
        call   sub_1_1AE8
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_1_214C
        mov    ecx, 2
        sub    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 20h]
        sub    edx, edx
        div    ecx
        mov    ebp, eax
        push   0
        db  0E8h, 078h, 0F7h, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 14h], eax
        or     ebp, ebp
        je     NEAR PTR loc_1_210B
        nop
loc_1_2060:
        mov    eax, ebp
        mov    dword ptr [esp + 10h], 240h
        sub    eax, 5
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 5
        push   eax
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_1_2588
        add    esp, 10h
        cmp    eax, 79h
        je     SHORT loc_1_210B
        or     eax, eax
        jne    NEAR PTR loc_1_214C
        mov    eax, dword ptr [esp + 10h]
        sub    eax, 0ech
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4dh]
        add    eax, 0ech
        push   eax
        push   esi
        call   sub_1_181C
        add    esp, 10h
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_20F1
        cmp    byte ptr [eax], 5
        jne    SHORT loc_1_20E8
        mov    ebx, 0ffffffffh
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_1_20CA
        mov    ebx, dword ptr [eax]
loc_1_20CA:
        mov    eax, dword ptr [esi + 4dh]
        mov    ecx, dword ptr [esi + 11h]
        cmp    dword ptr [eax + 10h], ecx
        jne    SHORT loc_1_20F1
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_1_2154
        push   dword ptr [eax]
        call   sub_1_0E7C
        mov    dword ptr [esi + 21h], eax
        jmp    SHORT loc_1_215B
loc_1_20E8:
        or     eax, eax
        je     SHORT loc_1_20F1
        cmp    byte ptr [eax], 6
        je     SHORT loc_1_2170
loc_1_20F1:
        push   0
        db  0E8h, 0D0h, 0F6h, 0FEh, 0FFh
        add    esp, 4
        sub    ebp, eax
        add    ebp, dword ptr [esp + 14h]
        mov    dword ptr [esp + 14h], eax
        jne    NEAR PTR loc_1_2060
loc_1_210B:
        mov    ebx, dword ptr [esp + 1ch]
        inc    ebx
        push   ebx
        call   sub_1_17E0
        add    esp, 4
        push   0
        db  0E8h, 0A8h, 0F6h, 0FEh, 0FFh
        add    esp, 4
        mov    edx, eax
        mov    ecx, dword ptr [esp + 24h]
        sub    dword ptr [esp + 20h], edx
        add    dword ptr [esp + 20h], ecx
        sub    edx, ecx
        add    dword ptr [esi + 45h], edx
        mov    dword ptr [esp + 24h], eax
        mov    dword ptr [esp + 1ch], ebx
        cmp    ebx, 2
        jb     NEAR PTR loc_1_2018
        mov    eax, 79h
loc_1_214C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_1_2154:
        mov    dword ptr [esi + 21h], 0e10h
loc_1_215B:
        cmp    ebx, -1
        je     SHORT loc_1_2163
        mov    dword ptr [esi + 19h], ebx
loc_1_2163:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_2170:
        pop    ebp
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
sub_1_2180:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_055C
        add    esp, 4
        push   esi
        call   sub_1_1C80
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_21B5
        push   1
        push   -1
        push   0
        push   0
        push   esi
        call   sub_1_06B8
        add    esp, 14h
        push   esi
        call   sub_1_0590
        add    esp, 4
loc_1_21B5:
        pop    esi
        ret
        db  090h
sub_1_21B8:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 28h]
        push   esi
        call   sub_1_04E8
        add    esp, 4
        push   esi
        call   sub_1_055C
        add    esp, 4
        lea    eax, [esp + 0ch]
        push   eax
        push   esi
        call   sub_1_1CB0
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_2238
        push   1
        push   dword ptr [esi + 19h]
        push   dword ptr [esi + 11h]
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_1_06B8
        add    esp, 14h
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        db  0C7h, 046h, 049h, 0A4h, 022h, 000h, 000h
        db  083h, 03Dh, 0D4h, 02Dh, 000h, 000h, 000h
        je     SHORT loc_1_227C
        push   dword ptr [esi + 31h]
        push   dword ptr [esi + 11h]
        push   450h
        db  0E8h, 05Fh, 0F3h, 0FEh, 0FFh
        add    esp, 0ch
        db  0C7h, 005h, 0D4h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_227C
loc_1_2238:
        push   0
        db  0C7h, 005h, 0D4h, 02Dh, 000h, 000h, 001h, 000h, 000h, 000h
        push   -1
        push   44eh
        db  0E8h, 038h, 0F3h, 0FEh, 0FFh
        add    esp, 0ch
        cmp    eax, 12ch
        mov    ebx, 0
        ja     SHORT loc_1_2266
        mov    ebx, 12ch
        sub    ebx, eax
loc_1_2266:
        push   edi
        push   3ebh
        push   esi
        call   sub_1_0844
        add    esp, 0ch
        db  0C7h, 046h, 049h, 0B8h, 021h, 000h, 000h
loc_1_227C:
        push   ebx
        push   esi
        call   sub_1_0660
        add    esp, 8
        push   esi
        call   sub_1_0590
        add    esp, 4
        mov    eax, dword ptr [esp + 2ch]
        or     eax, eax
        je     SHORT loc_1_2299
        mov    dword ptr [eax], ebx
loc_1_2299:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
sub_1_22A4:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 2ch]
        push   esi
        call   sub_1_055C
        add    esp, 4
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        call   sub_1_1FDC
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_232C
        cmp    dword ptr [esi + 35h], 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   dword ptr [esi + 19h]
        push   dword ptr [esi + 11h]
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        call   sub_1_06B8
        add    esp, 14h
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        db  0C7h, 046h, 049h, 0A4h, 022h, 000h, 000h
        db  083h, 03Dh, 0D4h, 02Dh, 000h, 000h, 000h
        je     NEAR PTR loc_1_2490
        push   dword ptr [esi + 31h]
        push   dword ptr [esi + 11h]
        push   451h
        db  0E8h, 070h, 0F2h, 0FEh, 0FFh
        add    esp, 0ch
        db  0C7h, 005h, 0D4h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2490
        db  08Bh, 0FFh
loc_1_232C:
        push   edi
        cmp    edi, 5
        jne    NEAR PTR loc_1_23DC
        push   3ech
        push   esi
        call   sub_1_0844
        add    esp, 0ch
        push   1
        push   -1
        push   0
        push   0
        push   esi
        call   sub_1_06B8
        add    esp, 14h
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        call   sub_1_1CB0
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_23B4
        db  0C7h, 005h, 0D4h, 02Dh, 000h, 000h, 001h, 000h, 000h, 000h
        push   0
        push   -1
        push   44eh
        db  0E8h, 007h, 0F2h, 0FEh, 0FFh
        add    esp, 0ch
        cmp    eax, 12ch
        mov    ebx, 0
        ja     SHORT loc_1_2397
        mov    ebx, 12ch
        sub    ebx, eax
loc_1_2397:
        push   edi
        push   3ebh
        push   esi
        call   sub_1_0844
        add    esp, 0ch
        db  0C7h, 046h, 049h, 0B8h, 021h, 000h, 000h
        jmp    NEAR PTR loc_1_2490
        db  08Bh, 0FFh
loc_1_23B4:
        push   1
        push   dword ptr [esi + 19h]
        push   dword ptr [esi + 11h]
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        call   sub_1_06B8
        add    esp, 14h
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        jmp    NEAR PTR loc_1_2489
        db  08Bh, 0FFh
loc_1_23DC:
        push   3edh
        push   esi
        call   sub_1_0844
        add    esp, 0ch
        push   0
        db  0E8h, 0D7h, 0F3h, 0FEh, 0FFh
        add    esp, 4
        mov    ebp, eax
        cmp    dword ptr [esi + 31h], ebp
        jge    SHORT loc_1_2458
        mov    ebx, dword ptr [esi + 11h]
        push   0
        push   3e9h
        push   esi
        call   sub_1_0844
        add    esp, 0ch
        push   0
        push   ebx
        push   44dh
        db  0E8h, 06Dh, 0F1h, 0FEh, 0FFh
        add    esp, 0ch
        cmp    eax, 12ch
        mov    ebx, 0
        ja     SHORT loc_1_2431
        mov    ebx, 12ch
        sub    ebx, eax
loc_1_2431:
        push   1
        push   -1
        push   0
        push   0
        push   esi
        call   sub_1_06B8
        add    esp, 14h
        db  0C7h, 005h, 0D4h, 02Dh, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 046h, 049h, 0B8h, 021h, 000h, 000h
        jmp    SHORT loc_1_2490
        db  08Dh, 049h, 000h
loc_1_2458:
        cmp    dword ptr [esi + 35h], 0
        jne    SHORT loc_1_2469
        push   esi
        call   sub_1_05B4
        add    esp, 4
        mov    edi, eax
loc_1_2469:
        push   esi
        call   sub_1_2718
        add    esp, 4
        mov    ebx, eax
        cmp    dword ptr [esi + 2dh], ebp
        jge    SHORT loc_1_2483
        db  0C7h, 005h, 0D4h, 02Dh, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_2483:
        or     ebx, ebx
        jne    SHORT loc_1_2489
        sub    ebx, ebx
loc_1_2489:
        db  0C7h, 046h, 049h, 0A4h, 022h, 000h, 000h
loc_1_2490:
        push   ebx
        push   esi
        call   sub_1_0660
        add    esp, 8
        push   esi
        call   sub_1_0590
        add    esp, 4
        mov    eax, dword ptr [esp + 30h]
        or     eax, eax
        je     SHORT loc_1_24AD
        mov    dword ptr [eax], ebx
loc_1_24AD:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  090h
sub_1_24B8:
        sub    esp, 10h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 24h]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_24D9
        db  0E8h, 0DDh, 0F2h, 0FEh, 0FFh
        mov    esi, eax
        shl    esi, 10h
        db  0E8h, 0D3h, 0F2h, 0FEh, 0FFh
        add    esi, eax
        mov    dword ptr [edi], esi
loc_1_24D9:
        mov    eax, dword ptr [edi]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esi + 4dh]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [esp + 8], 2
        push   43h
        call   sub_1_0E70
        mov    word ptr [esp + 0ah], ax
        cmp    dword ptr [esi + 35h], 0
        je     SHORT loc_1_251C
        push   0
        db  0E8h, 0C3h, 0F2h, 0FEh, 0FFh
        add    esp, 4
        cmp    dword ptr [esi + 2dh], eax
        jge    SHORT loc_1_2510
        jmp    SHORT loc_1_251C
        db  090h
loc_1_2510:
        mov    eax, dword ptr [esi + 19h]
        mov    dword ptr [esp + 0ch], eax
        jmp    SHORT loc_1_2524
        db  08Dh, 049h, 000h
loc_1_251C:
        mov    dword ptr [esp + 0ch], 0ffffffffh
loc_1_2524:
        sub    eax, eax
        lea    edi, [esp + 10h]
        stosd
        stosd
        cmp    dword ptr [esp + 0ch], -1
        jne    SHORT loc_1_2543
        mov    eax, dword ptr [esi + 51h]
        movzx  eax, word ptr [eax + 4]
        push   eax
        call   sub_1_1169
        add    esp, 4
loc_1_2543:
        mov    ecx, dword ptr [esp + 20h]
        sub    ecx, 12ch
        sbb    eax, eax
        not    eax
        and    ecx, eax
        add    ecx, 12ch
        push   10h
        lea    eax, [esp + 0ch]
        push   eax
        push   0
        push   ecx
        push   dword ptr [esi + 4dh]
        mov    eax, dword ptr [esi + 51h]
        push   dword ptr [eax]
        call   sub_1_0980
        cmp    eax, -1
        jne    SHORT loc_1_2580
        call   sub_1_0BC8
        pop    edi
        pop    esi
        add    esp, 10h
        ret
loc_1_2580:
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 10h
        ret
sub_1_2588:
        sub    esp, 124h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 10h
        push   0
        db  0E8h, 027h, 0F2h, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 10h], eax
        mov    ebx, dword ptr [esp + 144h]
        mov    ecx, ebx
        mov    edi, dword ptr [esp + 138h]
        mov    eax, dword ptr [edi + 51h]
        mov    ebp, dword ptr [eax]
        mov    dword ptr [esp + 30h], 0
        mov    dword ptr [esp + 34h], ebp
        inc    dword ptr [esp + 30h]
        mov    esi, dword ptr [esp + 13ch]
loc_1_25D4:
        mov    dword ptr [esp + 18h], ecx
        mov    dword ptr [esp + 1ch], 0
        lea    eax, [esp + 18h]
        push   eax
        push   0
        push   0
        lea    eax, [esp + 3ch]
        push   eax
        push   0
        call   sub_1_0B88
        or     eax, eax
        je     SHORT loc_1_2648
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   0
        push   dword ptr [esi]
        push   dword ptr [edi + 4dh]
        push   ebp
        call   sub_1_0A70
        cmp    eax, -1
        je     SHORT loc_1_2658
        mov    edx, dword ptr [edi + 4dh]
        mov    ecx, dword ptr [esp + 140h]
        cmp    dword ptr [edx + 4], ecx
        je     SHORT loc_1_2668
        push   0
        db  0E8h, 09Dh, 0F1h, 0FEh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [esp + 10h]
        sub    ecx, eax
        add    ecx, ebx
        jns    SHORT loc_1_25D4
        mov    eax, 79h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 124h
        ret
loc_1_2648:
        pop    ebp
        mov    eax, 79h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 124h
        ret
loc_1_2658:
        call   sub_1_0BC8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 124h
        ret
loc_1_2668:
        mov    dword ptr [esi], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 124h
        ret
        db  090h
loc_1_2678:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, 0ffffffffh
        db  0C7h, 005h, 0D4h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 0AFh, 0DCh, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_1_2698
        jmp    SHORT loc_1_26FE
        db  090h
loc_1_2698:
        db  08Bh, 03Dh, 084h, 02Bh, 000h, 000h
        db  081h, 0FFh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_1_26FC
loc_1_26A6:
        cmp    dword ptr [edi + 11h], 0
        lea    eax, [esp + 8]
        push   eax
        push   edi
        jne    SHORT loc_1_26D0
        call   sub_1_21B8
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_26E6
        cmp    eax, 79h
        je     SHORT loc_1_26E6
        cmp    eax, 5
        je     SHORT loc_1_26E6
        mov    esi, 0ffffffffh
        jmp    SHORT loc_1_26FE
        db  090h
loc_1_26D0:
        call   sub_1_22A4
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_26E6
        cmp    eax, 79h
        je     SHORT loc_1_26E6
        cmp    eax, 5
        jne    SHORT loc_1_2710
loc_1_26E6:
        mov    edi, dword ptr [edi]
        mov    ecx, dword ptr [esp + 8]
        sub    ecx, esi
        sbb    eax, eax
        and    ecx, eax
        add    esi, ecx
        db  081h, 0FFh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_1_26A6
loc_1_26FC:
        sub    eax, eax
loc_1_26FE:
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        je     SHORT loc_1_2708
        mov    dword ptr [ecx], esi
loc_1_2708:
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_2710:
        mov    esi, 0ffffffffh
        jmp    SHORT loc_1_26FE
        db  090h
sub_1_2718:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 11h], 0
        jne    SHORT loc_1_272C
        mov    eax, 12ch
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_272C:
        cmp    dword ptr [esi + 21h], -1
        jne    SHORT loc_1_273C
        mov    eax, 7fffffffh
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_273C:
        push   0
        db  0E8h, 085h, 0F0h, 0FEh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [esi + 29h]
        cmp    eax, ecx
        jge    SHORT loc_1_2754
        sub    ecx, eax
        mov    eax, ecx
        pop    esi
        ret
        db  090h
loc_1_2754:
        mov    ecx, dword ptr [esi + 2dh]
        cmp    eax, ecx
        jge    SHORT loc_1_2774
        sub    ecx, eax
        mov    eax, ecx
        cdq
        sub    eax, edx
        sar    eax, 1
        cmp    eax, 12ch
        jg     SHORT loc_1_27A1
        mov    eax, 12ch
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_2774:
        mov    ecx, dword ptr [esi + 31h]
        cmp    eax, ecx
        jge    SHORT loc_1_279C
        mov    dword ptr [esi + 19h], 0ffffffffh
        sub    ecx, eax
        mov    eax, ecx
        cdq
        sub    eax, edx
        sar    eax, 1
        cmp    eax, 12ch
        jg     SHORT loc_1_27A1
        mov    eax, 12ch
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_279C:
        mov    eax, 12ch
loc_1_27A1:
        pop    esi
        ret
        db  090h
sub_1_27A4:
        sub    esp, 14h
        push   esi
        push   edi
        push   11h
        push   2
        push   2
        call   sub_1_0848
        mov    esi, eax
        cmp    esi, -1
        jne    SHORT loc_1_27C8
        call   sub_1_0BC8
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_27C8:
        push   4
        mov    dword ptr [esp + 0ch], 1
        lea    eax, [esp + 0ch]
        push   eax
        push   4
        push   0ffffh
        push   esi
        call   sub_1_0BD0
        or     eax, eax
        je     SHORT loc_1_2800
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_2800:
        push   4
        lea    eax, [esp + 0ch]
        push   eax
        push   20h
        push   0ffffh
        push   esi
        call   sub_1_0BD0
        or     eax, eax
        je     SHORT loc_1_2830
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_2830:
        mov    edi, dword ptr [esp + 24h]
        or     edi, edi
        jne    SHORT loc_1_2870
        mov    dword ptr [esp + 8], 4d2h
        push   4
        lea    eax, [esp + 0ch]
        push   eax
        push   8000h
        push   0ffffh
        push   esi
        call   sub_1_0BD0
        or     eax, eax
        je     SHORT loc_1_2870
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 14h
        ret
loc_1_2870:
        push   44h
        mov    word ptr [esp + 10h], 2
        call   sub_1_0E70
        mov    word ptr [esp + 0eh], ax
        mov    dword ptr [esp + 10h], edi
        lea    edi, [esp + 14h]
        sub    eax, eax
        stosd
        stosd
        push   10h
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        call   sub_1_08D8
        or     eax, eax
        je     SHORT loc_1_28B8
        call   sub_1_0BC8
        mov    edi, eax
        push   esi
        call   sub_1_08A0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_28B8:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h
loc_1_28CC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_28D0:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        db  000h
loc_1_28D8:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    ecx, dword ptr [edi]
        push   es
        sub    al, 2eh
        das
        add    byte ptr [edi], dh
        add    al, byte ptr [eax]
        add    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        je     SHORT loc_1_293E
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_1_2961
        inc    ebx
        outsd
        outsb
        je     SHORT loc_1_297C
        outsd
        insb
        pop    esp
        inc    ebx
        outsd
        insd
        db  070h, 075h
        db  074h, 065h
        db  072h, 04Eh
        popad
        insd
        pop    esp
        inc    ebx
        outsd
        insd
        db  070h, 075h
        je     SHORT loc_1_2986
        db  072h, 04Eh
        popad
        insd
        add    byte ptr gs:[eax], al
loc_1_2928:
        inc    ebx
        outsd
        insd
        db  070h, 075h
        db  074h, 065h
        db  072h, 04Eh
        popad
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_2938:
        inc    esp
        inc    ebp
        push   ebx
        push   esp
        add    byte ptr [eax], al
loc_1_293E:
        add    byte ptr [eax], al
loc_1_2940:
        dec    esp
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + 4], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
loc_1_2961:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2978:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_297C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2980:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2984:
        add    byte ptr [eax], al
loc_1_2986:
        add    byte ptr [eax], al
loc_1_2988:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  082h, 00Fh, 000h, 000h, 088h, 00Fh, 000h, 000h, 08Dh, 00Fh, 000h, 000h, 092h, 00Fh, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 09Ah, 004h, 003h, 000h, 000h, 000h, 056h, 044h, 048h, 043h
        db  050h, 020h, 020h, 020h, 000h, 0B5h, 000h, 054h, 097h, 00Fh, 000h, 000h, 094h, 00Eh, 000h, 000h
        db  094h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Ch, 029h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_29F0:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        jae    SHORT loc_1_2A7C
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_1_2A75
        outsb
        db  074h, 043h
        outsd
        outsb
        je     SHORT loc_1_2A89
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        jb     NEAR PTR loc_1_2A97
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        inc    esp
        dec    eax
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_2A30:
        push   esi
        db  065h, 072h, 073h
        db  069h, 06Fh, 06Eh, 000h
loc_1_2A38:
        push   ebx
        jns    SHORT loc_1_2AAE
        je     SHORT loc_1_2AA2
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_1_2AB4
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_1_2AAD
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        inc    esp
        dec    eax
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_2A64:
        push   eax
        outsd
        jo     SHORT loc_1_2ADD
        jo     SHORT loc_1_2AB0
        insb
        popad
        add    byte ptr [bx + si], al
        add    byte ptr [eax + ebp*2 + 63h], al
        jo     SHORT loc_1_2ABE
loc_1_2A75:
        outsb
        outsw
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A7C:
        dec    edi
        db  070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 6f666e49h
        add    byte ptr [eax], al
loc_1_2A88:
        push   ebx
loc_1_2A89:
        jns    SHORT loc_1_2AFE
        db  074h, 065h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        je     SHORT loc_1_2ADA
loc_1_2A97:
        outsd
        outsb
        je     SHORT loc_1_2B0D
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
loc_1_2AA2:
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
loc_1_2AAD:
        pop    esp
loc_1_2AAE:
        inc    esp
        dec    eax
loc_1_2AB0:
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_2AB4:
        push   ebx
        jns    SHORT loc_1_2B2A
        je     SHORT loc_1_2B1E
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_1_2B30
loc_1_2ABE:
        jb     SHORT loc_1_2B25
        outsb
        je     SHORT loc_1_2B06
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
        pop    esp
loc_1_2ADA:
        inc    esp
        dec    eax
        inc    ebx
loc_1_2ADD:
        push   eax
        add    byte ptr [eax], al
loc_1_2AE0:
        push   esi
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 706f5000h
        db  075h, 070h
        inc    esi
        insb
        popad
        add    byte ptr [bx + si], al
        db  000h
loc_1_2AF4:
        inc    esp
        push   6e497063h
        outsw
        add    byte ptr [eax], al
loc_1_2AFE:
        add    byte ptr [eax], al
loc_1_2B00:
        inc    esp
        push   6e497063h
loc_1_2B06:
        outsw
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B0C:
        dec    edi
loc_1_2B0D:
        db  070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 6f666e49h
        add    byte ptr [eax], al
loc_1_2B18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B1E:
        add    byte ptr [eax], al
loc_1_2B20:
        sbb    byte ptr [ebx], ch
        add    byte ptr [eax], al
loc_1_2B24:
        inc    esp
loc_1_2B25:
        push   7063h
loc_1_2B2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B34:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B58:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B5C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B94:
        add    byte ptr [eax], al
loc_1_2B96:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2DD4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2DD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        lodsb
        add    byte ptr [ebx + 1], dl
        int3
        add    dword ptr [edx], ebp
        add    al, byte ptr [esi - 0ffd1bfeh]
        add    dl, byte ptr [eax + ebp*2 + 69h]
        db  073h, 020h
        inc    esp
        dec    eax
        inc    ebx
loc_1_3024:
        push   eax
        and    byte ptr [ebx + 6ch], ah
loc_1_3028:
        imul   esp, dword ptr [ebp + 6eh], 61772074h
        db  073h, 020h
        db  075h, 06Eh
        popad
loc_1_3034:
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [edi + 62h], ch
        db  074h, 061h
        imul   ebp, dword ptr [esi + 20h], 49206e61h
        push   eax
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
loc_1_304C:
        outsd
        db  072h, 06Bh
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [ecx + 20h], ah
        inc    esp
loc_1_3060:
        dec    eax
        inc    ebx
        push   eax
        db  020h
_LTEXT ends
        end
