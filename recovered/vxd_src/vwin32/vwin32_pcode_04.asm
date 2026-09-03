; vwin32_pcode_04.asm -- VWIN32 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0CC9
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0014:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 085h, 005h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    30h
loc_4_0028:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0CCF
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_003C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2488
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0050:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_24D0
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0064:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_24DC
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_4_0078:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_254A
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_008C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_25FC
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    2ch
loc_4_00A0:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 06Bh, 007h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_00B4:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 07Ah, 007h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_00C8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2782
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_00DC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0D3B
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_00F0:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 028h, 00Ah, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_4_0104:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 060h, 00Ah, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0118:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 083h, 00Ah, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_4_012C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0B33
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    20h
loc_4_0140:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 048h, 006h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0154:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2C75
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_0168:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 074h, 005h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_017C:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 0BAh, 005h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0190:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 021h, 00Bh, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_01A4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2C8C
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_01B8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2C9F
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_4_01CC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CDB
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_4_01E0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CF6
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_01F4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2D09
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0208:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2D17
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_021C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CB5
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_0230:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CC8
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_0244:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 0BCh, 003h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0258:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_26AD
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_026C:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 07Ch, 00Ah, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_0280:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3C17
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_4_0294:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3CC0
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    1ch
loc_4_02A8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3DA0
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_02BC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3A08
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_02D0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2947
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_02E4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2F66
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_02F8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2D2A
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_030C:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 042h, 006h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0320:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_251A
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0334:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_18BF
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0348:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_1A5C
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_035C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0B50
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_4_0370:
        pop    edi
        pop    esi
        pop    ebx
        db  0E8h, 00Dh, 006h, 0FCh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_0384:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0B65
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0398:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_449B
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_4_03AC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_44ED
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_4_03C0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_4513
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
        db  000h, 000h, 000h, 000h, 00Fh, 009h, 000h, 000h, 027h, 009h, 000h, 000h, 008h, 009h, 000h, 000h
loc_4_03E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        call   _VWIN32_WaitSingleObject
        pop    ebx
        leave
        ret
loc_4_03F9:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        call   _VWIN32_SetWin32Event
        pop    ebx
        leave
        ret
loc_4_0408:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        call   sub_4_2C1E
        pop    ebx
        leave
        ret
loc_4_0417:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        call   sub_4_2BAF
        pop    ebx
        leave
        ret
loc_4_0426:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   1
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        pop    ebx
        leave
        ret
loc_4_043B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        pop    ebx
        leave
        ret
loc_4_044E:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    NEAR PTR loc_4_0505
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [edi + 2ch], 2ah
        push   1
        push   0
        db  0CDh, 020h, 001h, 001h, 001h, 000h
        add    esp, 8
        db  0A3h, 0B6h, 011h, 000h, 000h
        push   1
        push   0
        db  0CDh, 020h, 001h, 001h, 001h, 000h
        add    esp, 8
        db  0A3h, 0B2h, 011h, 000h, 000h
        push   ebx
        mov    ah, 51h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        movzx  eax, bx
        db  0A3h, 09Eh, 011h, 000h, 000h
        pop    ebx
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        shr    eax, 4
        db  0A3h, 0A2h, 011h, 000h, 000h
        db  0A1h, 080h, 002h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_04C8
        call   sub_4_20B4
loc_4_04C8:
        db  0A3h, 094h, 002h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0505
        call   sub_4_2F8B
        push   10h
        push   1
        push   80080000h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        shr    eax, 0ch
        db  0A3h, 0C4h, 002h, 000h, 000h
        push   20060000h
        push   0
        push   2
        push   1
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
loc_4_0505:
        clc
        ret
loc_4_0507:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_0578
        db  083h, 03Dh, 094h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_0578
        db  0BEh, 0C8h, 011h, 000h, 000h
        db  0CDh, 020h, 0EAh, 000h, 001h, 000h
        push   10h
        push   0bff70000h
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        call   sub_4_245A
        db  0A1h, 0B2h, 011h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_054B
        push   eax
        db  0CDh, 020h, 002h, 001h, 001h, 000h
        add    esp, 4
loc_4_054B:
        db  0A1h, 0B6h, 011h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_055E
        push   eax
        db  0CDh, 020h, 002h, 001h, 001h, 000h
        add    esp, 4
loc_4_055E:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 0ECh, 010h, 000h, 000h
        sub    eax, eax
        xchg   dword ptr [edi], eax
        or     eax, eax
        je     SHORT loc_4_0578
        push   eax
        call   sub_4_25E4
loc_4_0578:
        mov    eax, 1
        clc
        ret
VWIN32_PM_API:
        movzx  eax, byte ptr [ebp + 1dh]
        cmp    eax, 15h
        ja     SHORT loc_4_058F
        db  0FFh, 024h, 085h, 0D0h, 002h, 000h, 000h
loc_4_058F:
        jmp    SHORT loc_4_0597
loc_4_0591:
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_4_0597:
        or     word ptr [ebp + 2ch], 1
        ret
loc_4_059D:
        mov    eax, 103h
        mov    dword ptr [ebp + 1ch], eax
        db  0A1h, 094h, 002h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        jmp    SHORT loc_4_0591
loc_4_05AF:
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        call   sub_4_05BF
        mov    dword ptr [ebp + 1ch], eax
        jmp    SHORT loc_4_0591
sub_4_05BF:
        db  089h, 03Dh, 014h, 011h, 000h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        mov    edx, edi
        db  003h, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx + 1ch], 0
        jl     SHORT loc_4_05E4
        cmp    dword ptr [edx + 14h], 101h
        je     SHORT loc_4_05F0
loc_4_05E4:
        push   ecx
        push   edi
        db  0E8h, 04Bh, 002h, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_4_05F3
loc_4_05F0:
        mov    dword ptr [edx + 14h], ecx
loc_4_05F3:
        push   -1
        db  0E8h, 019h, 002h, 0FCh, 0FFh
        add    esp, 4
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    edx, edi
        db  003h, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx]
        xor    eax, eax
        xchg   dword ptr [edx + 14h], eax
        cmp    eax, 101h
        jne    SHORT loc_4_062D
        db  066h, 08Bh, 00Dh, 00Ah, 011h, 000h, 000h
        db  00Fh, 0B7h, 015h, 008h, 011h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
loc_4_062D:
        ret
loc_4_062E:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        jmp    SHORT loc_4_05F3
loc_4_0636:
        push   ds
        mov    ds, word ptr [ebp + 34h]
        mov    esi, dword ptr [ebp + 30h]
        mov    ecx, dword ptr [ebp + 18h]
        sub    esp, ecx
        shr    ecx, 2
        mov    edi, esp
        cld
        rep movsd
        push   ss
        pop    ds
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 1
        ja     SHORT loc_4_066E
        jne    SHORT loc_4_065E
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        jmp    SHORT loc_4_0678
loc_4_065E:
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        jmp    SHORT loc_4_0678
loc_4_0666:
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        jmp    SHORT loc_4_0678
loc_4_066E:
        cmp    al, 2
        ja     SHORT loc_4_0666
        db  0CDh, 020h, 01Fh, 001h, 001h, 000h
loc_4_0678:
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ebp + 30h], esi
        add    esp, ecx
        pop    ds
        mov    dword ptr [ebp + 1ch], eax
        ret
loc_4_0685:
        db  0A1h, 0C8h, 002h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        jmp    NEAR PTR loc_4_0591
loc_4_0693:
        push   dword ptr [ebp + 10h]
        db  0E8h, 0BBh, 002h, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp + 1ch], eax
        jmp    NEAR PTR loc_4_0591
loc_4_06A6:
        push   dword ptr [ebp + 10h]
        db  0E8h, 076h, 002h, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp + 1ch], eax
        jmp    NEAR PTR loc_4_0591
loc_4_06B9:
        db  0E8h, 0FAh, 001h, 0FCh, 0FFh
        mov    dword ptr [ebp + 1ch], eax
        jmp    NEAR PTR loc_4_0591
loc_4_06C6:
        push   dword ptr [ebp + 10h]
        db  0E8h, 029h, 002h, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp + 1ch], eax
        jmp    NEAR PTR loc_4_0591
loc_4_06D9:
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        push   ebx
        call   sub_4_2C75
        add    esp, 8
        jmp    NEAR PTR loc_4_0591
loc_4_06EE:
        db  0FFh, 035h, 0D8h, 010h, 000h, 000h
        push   dword ptr [ebp + 10h]
        call   sub_4_2991
        add    esp, 8
        jmp    NEAR PTR loc_4_0591
loc_4_0704:
        db  0FFh, 035h, 0D8h, 010h, 000h, 000h
        push   dword ptr [ebp + 10h]
        call   sub_4_2ADF
        add    esp, 8
        jmp    NEAR PTR loc_4_0591
loc_4_071A:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        call   sub_4_2CC8
        add    esp, 8
        jmp    NEAR PTR loc_4_0591
loc_4_072D:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        call   sub_4_2CB5
        add    esp, 8
        jmp    NEAR PTR loc_4_0591
loc_4_0740:
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 14h]
        db  0CDh, 020h, 062h, 001h, 001h, 000h
        jmp    NEAR PTR loc_4_0591
loc_4_0754:
        db  0CDh, 020h, 060h, 001h, 001h, 000h
        jmp    NEAR PTR loc_4_0591
loc_4_075F:
        db  0CDh, 020h, 084h, 001h, 001h, 000h
        jmp    NEAR PTR loc_4_0591
loc_4_076A:
        mov    ax, word ptr [ebp + 38h]
        db  066h, 0A3h, 064h, 003h, 000h, 000h
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        db  0A3h, 040h, 003h, 000h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  0A3h, 0B8h, 002h, 000h, 000h
        mov    ax, word ptr [ebp]
        db  0A3h, 050h, 003h, 000h, 000h
        mov    ax, word ptr [ebp + 8]
        db  0A3h, 054h, 003h, 000h, 000h
        mov    ax, word ptr [ebp + 18h]
        db  0A3h, 058h, 003h, 000h, 000h
        movzx  eax, word ptr [ebp + 3ch]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_4_0597
        mov    edx, dword ptr [ebp + 4]
        movzx  ecx, dx
        add    ecx, eax
        db  089h, 00Dh, 0B0h, 002h, 000h, 000h
        shr    edx, 10h
        add    edx, eax
        db  089h, 015h, 0B4h, 002h, 000h, 000h
        db  0A1h, 0C0h, 002h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        jmp    NEAR PTR loc_4_0591
loc_4_07E4:
        db  0CDh, 020h, 051h, 000h, 040h, 000h
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        jmp    NEAR PTR loc_4_0591
loc_4_07FA:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 18h]
        test   ebx, ebx
        jne    NEAR PTR loc_4_090F
        db  00Bh, 01Dh, 00Ch, 011h, 000h, 000h
        jne    SHORT loc_4_0830
        xchg   dword ptr [esp + 1ch], ebx
        db  087h, 01Dh, 00Ch, 011h, 000h, 000h
        xchg   dword ptr [esp + 34h], ebx
        db  087h, 01Dh, 010h, 011h, 000h, 000h
        db  0BEh, 0D4h, 003h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
loc_4_0830:
        mov    eax, dword ptr [esp + 28h]
        inc    eax
        je     NEAR PTR loc_4_08F2
        mov    ecx, dword ptr [esp + 24h]
        jecxz  loc_4_088F
        db  0CDh, 020h, 03Fh, 001h, 001h, 000h
        mov    ebx, eax
loc_4_0849:
        test   ebx, ebx
        je     NEAR PTR loc_4_0904
        mov    esi, dword ptr [esp + 20h]
        lea    edi, [ebx + 0ch]
        mov    ecx, 8
loc_4_085D:
        lodsb
        or     al, al
        jne    SHORT loc_4_0869
        cmp    byte ptr [edi], 20h
        je     SHORT loc_4_08BF
        jmp    SHORT loc_4_088B
loc_4_0869:
        cmp    al, 61h
        jb     SHORT loc_4_0873
        cmp    al, 7ah
        ja     SHORT loc_4_0873
        and    al, 0dfh
loc_4_0873:
        mov    dl, byte ptr [edi]
        cmp    dl, 61h
        jb     SHORT loc_4_0882
        cmp    dl, 7ah
        ja     SHORT loc_4_0882
        and    dl, 0dfh
loc_4_0882:
        cmp    al, dl
        jne    SHORT loc_4_088B
        inc    edi
        loop   loc_4_085D
        jmp    SHORT loc_4_08BF
loc_4_088B:
        mov    ebx, dword ptr [ebx]
        jmp    SHORT loc_4_0849
loc_4_088F:
        mov    al, 1
        mov    edx, dword ptr [esp + 20h]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_4_0904
        mov    ebx, eax
        lea    esi, [eax + 0ch]
        mov    ecx, 8
        mov    edi, dword ptr [esp + 28h]
        mov    dword ptr [edi], 5c2e5c5ch
        add    edi, 4
loc_4_08B4:
        lodsb
        cmp    al, 20h
        je     SHORT loc_4_08BC
        stosb
        loop   loc_4_08B4
loc_4_08BC:
        xor    al, al
        stosb
loc_4_08BF:
        mov    edi, dword ptr [esp + 40h]
        lea    esi, [ebx + 0ch]
        movsd
        movsd
        mov    dword ptr [esp + 18h], ebx
        mov    eax, 23h
        mov    ecx, 0
        mov    dword ptr [esp + 1ch], ecx
        xor    edx, edx
        lea    esi, [esp + 10h]
        push   ebx
        call   dword ptr [ebx + 18h]
        pop    ebx
        test   eax, eax
        mov    eax, ebx
        je     SHORT loc_4_093B
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_093B
loc_4_08F2:
        mov    edx, dword ptr [esp + 20h]
        xor    ebx, ebx
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        jb     SHORT loc_4_093B
        xor    eax, eax
        jmp    SHORT loc_4_093B
loc_4_0904:
        xor    eax, eax
        jmp    SHORT loc_4_093B
loc_4_0908:
        mov    eax, 37h
        jmp    SHORT loc_4_093B
loc_4_090F:
        or     dl, byte ptr [ebx + 18h]
        mov    eax, dword ptr [ebx + 18h]
        or     dl, byte ptr [eax]
        mov    esi, dword ptr [esp + 40h]
        lea    edi, [ebx + 0ch]
        mov    ecx, 8
        repe cmpsb
        jne    SHORT loc_4_0908
loc_4_0927:
        mov    eax, 23h
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 38h]
        lea    esi, [esp + 10h]
        call   dword ptr [ebx + 18h]
loc_4_093B:
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [edi + 1ch], eax
        pop    edi
        pop    esi
        pop    ebx
        ret    34h
loc_4_0948:
        xor    eax, eax
        mov    ecx, 103h
        ret
loc_4_0950:
        mov    ecx, dword ptr [esi + 0ch]
        jecxz  loc_4_0948
        inc    ecx
        jecxz  loc_4_09A2
        dec    ecx
        mov    edx, 21h
        dec    ecx
        jecxz  loc_4_09DE
        mov    edx, 25h
        dec    ecx
        jecxz  loc_4_09DE
        mov    edx, 26h
        dec    ecx
        jecxz  loc_4_09DE
        mov    edx, 13h
        dec    ecx
        jecxz  loc_4_09DE
        dec    ecx
        jecxz  loc_4_09A9
        mov    edx, 21h
        dec    ecx
        jecxz  loc_4_09DE
        mov    eax, 32h
        jmp    NEAR PTR loc_4_0B32
loc_4_098E:
        mov    eax, 7ah
        jmp    NEAR PTR loc_4_0B32
loc_4_0998:
        mov    eax, 57h
        jmp    NEAR PTR loc_4_0B32
loc_4_09A2:
        xor    eax, eax
        jmp    NEAR PTR loc_4_0B32
loc_4_09A9:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, dword ptr [esi + 18h]
        or     eax, dword ptr [esi + 20h]
        or     eax, dword ptr [esi + 24h]
        jne    SHORT loc_4_0998
        cmp    dword ptr [esi + 14h], 4
        jne    SHORT loc_4_0998
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_4_0998
        push   -1
        push   dword ptr [eax]
        db  0FFh, 035h, 018h, 011h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        xor    eax, eax
        jmp    NEAR PTR loc_4_0B32
loc_4_09DE:
        cmp    dword ptr [esi + 14h], 1ch
        jb     SHORT loc_4_098E
        cmp    dword ptr [esi + 1ch], 1ch
        jb     SHORT loc_4_098E
        mov    ecx, dword ptr [esi + 18h]
        jecxz  loc_4_0998
        mov    ecx, dword ptr [esi + 10h]
        jecxz  loc_4_0998
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        push   esi
        mov    edi, dword ptr [esi]
        push   edi
        lea    edi, [edi + 10h]
        mov    esi, dword ptr [esi + 10h]
        cld
        movsd
        movsd
        movsd
        movsd
        pop    edi
        push   edi
        movsd
        movsd
        pop    edi
        cmp    edx, 21h
        jne    NEAR PTR loc_4_0AF4
        pop    esi
        push   esi
        cmp    dword ptr [esi + 0ch], 6
        je     NEAR PTR loc_4_0AF0
        mov    byte ptr [edi + 1dh], 44h
        cmp    byte ptr [edi + 1ch], 0dh
        jne    NEAR PTR loc_4_0AF4
        mov    ecx, dword ptr [edi + 18h]
        and    ch, 0bfh
        cmp    ch, 8
        jne    NEAR PTR loc_4_0AF4
        cmp    byte ptr [edi + 11h], 2
        jne    SHORT loc_4_0A59
        cmp    cl, 4ah
        je     SHORT loc_4_0A68
        cmp    cl, 4bh
        je     SHORT loc_4_0A68
loc_4_0A59:
        cmp    cl, 6ah
        je     SHORT loc_4_0AA0
        cmp    cl, 6bh
        je     SHORT loc_4_0AA0
        jmp    NEAR PTR loc_4_0AF4
loc_4_0A68:
        push   edx
        db  08Bh, 01Dh, 0D8h, 010h, 000h, 000h
        push   ebx
        db  0FFh, 035h, 09Ch, 002h, 000h, 000h
        call   sub_4_45A9
        push   ebx
        db  0FFh, 035h, 0A4h, 002h, 000h, 000h
        call   sub_4_45A9
        pop    edx
        db  0FEh, 005h, 0CCh, 002h, 000h, 000h
        mov    eax, edx
        db  0CDh, 020h, 054h, 001h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_4_0AEE
        jmp    SHORT loc_4_0AFC
loc_4_0AA0:
        mov    eax, edx
        db  0CDh, 020h, 054h, 001h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_4_0AFC
        cmp    word ptr [ebp + 1ch], 440dh
        jne    SHORT loc_4_0AC1
        db  080h, 03Dh, 0CCh, 002h, 000h, 000h, 000h
        jne    SHORT loc_4_0AC8
loc_4_0AC1:
        cmp    word ptr [ebp + 1ch], 1
        jne    SHORT loc_4_0AFC
loc_4_0AC8:
        db  08Bh, 01Dh, 0D8h, 010h, 000h, 000h
        push   ebx
        db  0FFh, 035h, 0A4h, 002h, 000h, 000h
        call   sub_4_45C3
        push   ebx
        db  0FFh, 035h, 09Ch, 002h, 000h, 000h
        call   sub_4_45C3
        db  0FEh, 00Dh, 0CCh, 002h, 000h, 000h
        jmp    SHORT loc_4_0AFC
loc_4_0AEE:
        jmp    SHORT loc_4_0AC8
loc_4_0AF0:
        mov    byte ptr [edi + 1dh], 73h
loc_4_0AF4:
        mov    eax, edx
        db  0CDh, 020h, 054h, 001h, 001h, 000h
loc_4_0AFC:
        pop    esi
        push   esi
        mov    edi, dword ptr [esi + 18h]
        mov    esi, dword ptr [esi]
        push   esi
        lea    esi, [esi + 10h]
        cld
        movsd
        movsd
        movsd
        movsd
        pop    esi
        push   esi
        movsd
        movsd
        pop    esi
        mov    eax, dword ptr [esi + 2ch]
        stosd
        pop    esi
        mov    ecx, dword ptr [esi + 20h]
        jecxz  loc_4_0B21
        mov    dword ptr [ecx], 1ch
loc_4_0B21:
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        xor    eax, eax
loc_4_0B32:
        ret
sub_4_0B33:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 031h, 000h, 040h, 000h
        leave
        ret
sub_4_0B50:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 0d700h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        pop    ebx
        leave
        ret
sub_4_0B65:
        db  0CDh, 020h, 015h, 000h, 02Ah, 000h
        ret
loc_4_0B6C:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        or     dword ptr [edi], 40000h
        db  08Bh, 01Dh, 0D8h, 010h, 000h, 000h
        test   dword ptr [ebx + 44h], 20h
        je     SHORT loc_4_0B8D
        or     dword ptr [edi], 80000h
loc_4_0B8D:
        mov    esi, dword ptr [edi]
        and    esi, 30000h
        and    dword ptr [edi], 0fffcffffh
        test   dword ptr [ebx + 44h], 400h
        je     SHORT loc_4_0BAA
        or     dword ptr [edi], 10000h
loc_4_0BAA:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 1ch], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [edx + 18h], ecx
        push   dword ptr [edx + 40h]
        cmp    ah, 3fh
        je     SHORT loc_4_0BEC
        cmp    ah, 40h
        je     SHORT loc_4_0BEC
loc_4_0BC9:
        mov    eax, 21h
        db  0CDh, 020h, 054h, 001h, 001h, 000h
loc_4_0BD4:
        pop    eax
        mov    edx, dword ptr [esp + 4]
        mov    word ptr [edx + 40h], ax
        and    dword ptr [edi], 0fff3ffffh
        or     esi, esi
        je     SHORT loc_4_0BE9
        or     dword ptr [edi], esi
loc_4_0BE9:
        ret    10h
loc_4_0BEC:
        push   esi
        push   ebx
        push   edx
        push   eax
        push   ecx
        mov    ebx, dword ptr [edx + 10h]
        mov    esi, dword ptr [edx + 14h]
        db  0CDh, 020h, 033h, 000h, 040h, 000h
        pop    ecx
        pop    eax
        jb     SHORT loc_4_0C38
        sub    ah, 3fh
        movzx  eax, ah
        add    eax, 0d602h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        pop    edx
        push   edx
        push   eax
        mov    dword ptr [edx + 1ch], eax
        sbb    eax, eax
        and    eax, 1
        and    word ptr [edx + 2ch], 0fffeh
        or     word ptr [edx + 2ch], ax
        test   eax, eax
        pop    eax
        je     SHORT loc_4_0C37
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    word ptr [edi + 34h], ax
        pop    edi
loc_4_0C37:
        clc
loc_4_0C38:
        pop    edx
        pop    ebx
        pop    esi
        jb     SHORT loc_4_0BC9
        jmp    SHORT loc_4_0BD4
loc_4_0C3F:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 1ch], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [edx + 18h], ecx
        mov    eax, 31h
        db  0CDh, 020h, 054h, 001h, 001h, 000h
        ret    10h
loc_4_0C5F:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 1ch], eax
        mov    eax, 41h
        db  0CDh, 020h, 054h, 001h, 001h, 000h
        ret    0ch
loc_4_0C78:
        mov    esi, dword ptr [esp + 0ch]
        db  0CDh, 020h, 085h, 001h, 001h, 000h
        ret    0ch
loc_4_0C85:
        ret
loc_4_0C86:
        mov    ebx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_4_0CC6
        mov    al, 2eh
        xor    ah, ah
        mov    ebx, 400h
        xor    cl, cl
        db  0BEh, 085h, 00Ch, 000h, 000h
        xor    edx, edx
        xor    edi, edi
        db  0CDh, 020h, 001h, 000h, 00Dh, 000h
        jb     SHORT loc_4_0CC6
        mov    esi, eax
        mov    ebx, dword ptr [esp + 0ch]
        mov    cx, 400h
        db  0CDh, 020h, 005h, 000h, 00Dh, 000h
        mov    eax, esi
        db  0CDh, 020h, 002h, 000h, 00Dh, 000h
loc_4_0CC6:
        ret    0ch
sub_4_0CC9:
        mov    eax, 103h
        ret
sub_4_0CCF:
        push   esi
        push   edi
        push   ebx
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_4_0CDF:
        push   edi
        mov    edi, dword ptr [esp + 10h]
        db  0CDh, 020h, 003h, 000h, 011h, 000h
        mov    edi, dword ptr [esp + 8]
        mov    dword ptr [edi + 1ch], eax
        pop    edi
        ret    0ch
loc_4_0CF5:
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 14h]
        db  0CDh, 020h, 004h, 000h, 011h, 000h
        pop    edi
        ret    10h
loc_4_0D08:
        db  0CDh, 020h, 000h, 000h, 011h, 000h
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 1ch], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        ret    0ch
loc_4_0D1E:
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 1ch], 1
        ret    0ch
sub_4_0D3B:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        db  003h, 01Dh, 0ECh, 010h, 000h, 000h
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [ebp - 4], ebx
        or     ebx, ebx
        je     NEAR PTR loc_4_0E99
        db  0BEh, 02Eh, 006h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        db  0A3h, 008h, 011h, 000h, 000h
        jae    SHORT loc_4_0D7B
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_4_0E99
loc_4_0D7B:
        lea    eax, [edi + 2ah]
        mov    dword ptr [ebx + 40h], eax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 24h]
        db  0A3h, 098h, 002h, 000h, 000h
        db  089h, 01Dh, 0E0h, 010h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx], eax
        db  0A3h, 0D8h, 010h, 000h, 000h
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 4], eax
        db  0A3h, 0DCh, 010h, 000h, 000h
        db  0A3h, 04Ch, 003h, 000h, 000h
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [edx + 0ch], edi
        db  0C7h, 042h, 010h, 0D8h, 010h, 000h, 000h
        mov    eax, dword ptr [edx + 14h]
        db  0A3h, 0E4h, 010h, 000h, 000h
        mov    ax, word ptr [eax]
        mov    word ptr [ebx + 72h], ax
        mov    eax, dword ptr [edx + 18h]
        db  0A3h, 0A8h, 004h, 000h, 000h
        mov    eax, dword ptr [edx + 28h]
        db  0A3h, 09Ch, 002h, 000h, 000h
        mov    eax, dword ptr [edx + 2ch]
        db  0A3h, 0A4h, 002h, 000h, 000h
        mov    eax, dword ptr [edx + 30h]
        db  0A3h, 0A8h, 002h, 000h, 000h
        mov    eax, dword ptr [edx + 34h]
        db  0A3h, 0ACh, 002h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 48h], ebx
        mov    eax, dword ptr [edx + 38h]
        add    eax, ebx
        db  0A3h, 0A0h, 002h, 000h, 000h
        db  0BFh, 0ACh, 003h, 000h, 000h
        mov    esi, dword ptr [edx + 40h]
        mov    ecx, 18h
        rep movsd
        mov    esi, dword ptr [edx + 44h]
        mov    ecx, 18h
        rep movsd
        db  0BEh, 0ACh, 003h, 000h, 000h
        mov    edi, esi
        mov    ecx, 30h
loc_4_0E2D:
        lodsd
        cmp    eax, 3fffffh
        ja     SHORT loc_4_0E37
        add    eax, ebx
loc_4_0E37:
        stosd
        loop   loc_4_0E2D
        db  0CDh, 020h, 051h, 000h, 040h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 3ch], eax
        db  0CDh, 020h, 0B9h, 000h, 001h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 1eh], ax
        db  066h, 0A3h, 0D4h, 010h, 000h, 000h
        shl    eax, 4
        push   0
        push   0
        push   92h
        push   0ffh
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 1ch], ax
        db  066h, 0A3h, 0D6h, 010h, 000h, 000h
        db  0BEh, 0A8h, 009h, 000h, 000h
        db  0CDh, 020h, 004h, 001h, 001h, 000h
loc_4_0E99:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_VWIN32_CopyMem:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, ecx
        xor    eax, eax
loc_4_0EB3:
        rep movsb
loc_4_0EB5:
        inc    eax
loc_4_0EB6:
        sub    edx, ecx
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        je     SHORT loc_4_0EC2
        mov    dword ptr [esi], edx
loc_4_0EC2:
        pop    edi
        pop    esi
        ret
sub_4_0EC5:
        db  0CDh, 020h, 01Eh, 081h, 001h, 000h
sub_4_0ECB:
        db  0CDh, 020h, 01Fh, 081h, 001h, 000h
sub_4_0ED1:
        db  0CDh, 020h, 025h, 081h, 001h, 000h
sub_4_0ED7:
        db  0CDh, 020h, 033h, 081h, 001h, 000h
sub_4_0EDD:
        db  0CDh, 020h, 06Eh, 080h, 001h, 000h
        db  0CDh, 020h, 0FAh, 080h, 001h, 000h
sub_4_0EE9:
        db  0CDh, 020h, 027h, 081h, 001h, 000h
sub_4_0EEF:
        db  0CDh, 020h, 034h, 081h, 001h, 000h
sub_4_0EF5:
        db  0CDh, 020h, 02Eh, 081h, 001h, 000h
sub_4_0EFB:
        db  0CDh, 020h, 02Fh, 081h, 001h, 000h
sub_4_0F01:
        db  0CDh, 020h, 007h, 080h, 011h, 000h
        add    byte ptr [esi + 40h], bh
        jmp    SHORT loc_4_0F10
loc_4_0F0F:
        ret
loc_4_0F10:
        sti
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        cmp    dword ptr [edi + 2ch], 2ah
        jne    SHORT loc_4_0F7C
        or     byte ptr [ebp + 2dh], 2
        cmp    esi, 2ch
        je     NEAR PTR loc_4_0FE6
        cmp    esi, 30h
        je     NEAR PTR loc_4_0FE6
        db  08Bh, 03Dh, 0E0h, 010h, 000h, 000h
        call   sub_4_1D79
        jb     SHORT loc_4_0F7B
        call   sub_4_1FAC
        jb     SHORT loc_4_0F7B
        db  066h, 0A1h, 060h, 003h, 000h, 000h
        cmp    ax, word ptr [ebp + 28h]
        jne    SHORT loc_4_0F82
        db  066h, 0A1h, 062h, 003h, 000h, 000h
        cmp    ax, word ptr [ebp + 34h]
        jne    SHORT loc_4_0FBB
        db  083h, 03Dh, 028h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_0FBB
        shr    esi, 2
        push   0fffebadh
        push   ebp
        push   esi
        db  0FFh, 015h, 028h, 003h, 000h, 000h
        add    esp, 0ch
loc_4_0F7B:
        ret
loc_4_0F7C:
        db  0FFh, 0A6h, 068h, 003h, 000h, 000h
loc_4_0F82:
        cmp    byte ptr [edi + 78h], 2
        jae    SHORT loc_4_0FBB
        lar    eax, word ptr [ebp + 34h]
        test   eax, 400000h
        jne    SHORT loc_4_0FAD
        lar    eax, word ptr [ebp + 28h]
        test   eax, 400000h
        jne    SHORT loc_4_0FAD
loc_4_0F9E:
        inc    byte ptr [edi + 78h]
        push   edi
        db  0FFh, 096h, 068h, 003h, 000h, 000h
        pop    edi
        dec    byte ptr [edi + 78h]
        ret
loc_4_0FAD:
        cmp    word ptr [ebp + 32h], 0
        jne    SHORT loc_4_0FBB
        cmp    word ptr [ebp + 26h], 0
        je     SHORT loc_4_0F9E
loc_4_0FBB:
        shr    esi, 2
        push   esi
        push   ebp
        db  0CDh, 020h, 00Bh, 000h, 02Ah, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_0FE5
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_4_0FE5
        push   1
        push   edi
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
loc_4_0FE5:
        ret
loc_4_0FE6:
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_4_103F
        mov    eax, dword ptr [ebp + 20h]
        or     al, 3
        lar    ecx, eax
        jne    NEAR PTR loc_4_1149
        test   ch, 80h
        jne    NEAR PTR loc_4_10F6
        db  08Bh, 03Dh, 0B4h, 002h, 000h, 000h
        movzx  ecx, word ptr [edi]
        jecxz  loc_4_103F
        cmp    ax, cx
        jne    SHORT loc_4_103F
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edi, eax
        push   0
        push   ebx
        push   edi
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        or     dh, 80h
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        jmp    NEAR PTR loc_4_10F6
loc_4_103F:
        db  083h, 03Dh, 040h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_4_1149
        lar    eax, word ptr [ebp + 34h]
        jne    NEAR PTR loc_4_1149
        test   eax, 400000h
        jne    NEAR PTR loc_4_1159
        cmp    word ptr [ebp + 30h], 20h
        jb     NEAR PTR loc_4_1149
        mov    ebx, dword ptr [ebp + 20h]
        or     bx, bx
        je     SHORT loc_4_10AA
        db  08Bh, 03Dh, 0D8h, 010h, 000h, 000h
        push   edi
        db  0FFh, 035h, 0A8h, 002h, 000h, 000h
        call   sub_4_45A9
        mov    eax, ebx
        or     al, 3
        lar    eax, eax
        jne    SHORT loc_4_1097
        test   ah, 80h
        je     SHORT loc_4_1097
        or     esi, 1
loc_4_1097:
        push   edi
        db  0FFh, 035h, 0A8h, 002h, 000h, 000h
        call   sub_4_45C3
        test   si, 1
        jne    SHORT loc_4_10F6
loc_4_10AA:
        sub    word ptr [ebp + 30h], 12h
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     NEAR PTR loc_4_1149
        db  0A1h, 044h, 003h, 000h, 000h
        stosd
        mov    eax, ebx
        stosw
        mov    eax, dword ptr [ebp + 24h]
        stosd
        movzx  eax, word ptr [ebp + 28h]
        stosd
        mov    eax, dword ptr [ebp + 2ch]
        stosd
        db  0A1h, 040h, 003h, 000h, 000h
        ror    eax, 10h
        mov    word ptr [ebp + 28h], ax
        shr    eax, 10h
        mov    dword ptr [ebp + 24h], eax
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        and    byte ptr [ebp + 2dh], 0feh
loc_4_10F6:
        ret
loc_4_10F7:
        mov    edi, dword ptr [ebp + 2ch]
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        cmp    esi, -1
        je     SHORT loc_4_1139
        add    esi, 2
        lodsd
        mov    dword ptr [ebp + 24h], eax
        lodsd
        mov    dword ptr [ebp + 28h], eax
        lodsd
        mov    ecx, eax
        xchg   word ptr [ebp + 2ch], cx
        xor    ecx, eax
        test   ch, 2
        je     SHORT loc_4_1134
        test   ah, 2
        je     SHORT loc_4_1134
        and    word ptr [ebp + 2ch], 0fdffh
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_4_1134:
        add    word ptr [ebp + 30h], 0eh
loc_4_1139:
        or     esi, 0ffffffffh
        test   edi, 1
        jne    SHORT loc_4_115C
        mov    esi, 2ch
loc_4_1149:
        db  066h, 0A1h, 060h, 003h, 000h, 000h
        cmp    ax, word ptr [ebp + 28h]
        jne    NEAR PTR loc_4_0F7C
loc_4_1159:
        shr    esi, 2
loc_4_115C:
        push   esi
        push   ebp
        db  0CDh, 020h, 00Bh, 000h, 02Ah, 000h
        add    esp, 8
        ret
sub_4_1168:
        db  0A1h, 0D8h, 010h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_1185
        mov    ebx, dword ptr [eax + 80h]
loc_4_1177:
        or     ebx, ebx
        je     SHORT loc_4_1185
        test   dword ptr [ebx + 4], edx
        jne    SHORT loc_4_1184
        mov    ebx, dword ptr [ebx]
        jmp    SHORT loc_4_1177
loc_4_1184:
        stc
loc_4_1185:
        ret
sub_4_1186:
        push   ebx
        push   edi
        call   sub_4_1168
        jae    SHORT loc_4_11C5
        mov    ebx, dword ptr [eax + 80h]
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    edi, dword ptr [edi + 10h]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 24h], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [edi + 28h], ax
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [edi + 30h], eax
        mov    eax, dword ptr [ebx + 14h]
        mov    word ptr [edi + 34h], ax
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [edi + 1ch], eax
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [edi + 14h], eax
        stc
loc_4_11C5:
        pop    edi
        pop    ebx
        ret
loc_4_11C8:
        push   edi
        push   ebp
        call   sub_4_46F8
        cmp    eax, 1
        ret
loc_4_11D3:
        push   1
        push   0ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        mov    edx, eax
        je     SHORT loc_4_1205
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 10h]
        db  0BEh, 02Dh, 012h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 8], esi
loc_4_1205:
        mov    ebx, dword ptr [esp + 4]
        mov    dword ptr [ebx + 1ch], edx
        ret    14h
loc_4_120F:
        mov    edx, dword ptr [esp + 0ch]
        xor    esi, esi
        xchg   dword ptr [edx + 8], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        ret    0ch
loc_4_122D:
        mov    ebx, edx
        xor    esi, esi
        xchg   dword ptr [ebx + 8], esi
        or     esi, esi
        je     SHORT loc_4_125B
        db  08Bh, 03Dh, 03Ch, 003h, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_125B
        push   edi
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        push   edi
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_4_125B:
        ret
loc_4_125C:
        mov    edx, dword ptr [esp + 0ch]
        call   sub_4_1186
        ret    0ch
loc_4_1268:
        mov    eax, dword ptr [esp + 0ch]
        db  0A3h, 028h, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        db  066h, 0A3h, 060h, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        db  066h, 0A3h, 062h, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        db  0A3h, 038h, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 1ch]
        db  0A3h, 02Ch, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 20h]
        db  0A3h, 030h, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 24h]
        db  0A3h, 034h, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        db  0A3h, 03Ch, 003h, 000h, 000h
        db  0A1h, 0ECh, 010h, 000h, 000h
        mov    ebx, dword ptr [esp + 4]
        mov    dword ptr [ebx + 1ch], eax
        ret    28h
loc_4_12C1:
        db  0C7h, 005h, 03Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_4_0591
VWIN32_TerminateApp:
        pushad
        mov    ebx, dword ptr [edi + 14h]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_1353
        mov    edx, 20h
        call   sub_4_1186
        jb     SHORT loc_4_134C
        db  08Bh, 01Dh, 010h, 011h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_12F6
        cmp    ebx, edi
        jne    SHORT loc_4_130A
loc_4_12F6:
        db  08Bh, 01Dh, 03Ch, 003h, 000h, 000h
        or     ebx, ebx
        jne    SHORT loc_4_130A
        push   edi
        db  0CDh, 020h, 00Ah, 001h, 001h, 000h
        mov    ebx, edi
        pop    edi
loc_4_130A:
        mov    esi, edi
        db  003h, 035h, 0ECh, 010h, 000h, 000h
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     SHORT loc_4_1362
        mov    esi, dword ptr [esi]
        push   ebx
        push   esi
        db  0FFh, 035h, 030h, 003h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        db  03Bh, 01Dh, 03Ch, 003h, 000h, 000h
        jne    SHORT loc_4_133D
        push   ebx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_4_133D:
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        jne    SHORT loc_4_134A
        db  0E8h, 0BEh, 0F2h, 0FBh, 0FFh
loc_4_134A:
        popad
        ret
loc_4_134C:
        db  0CDh, 020h, 070h, 001h, 001h, 000h
        ret
loc_4_1353:
        mov    eax, 0bb8h
        xor    ecx, ecx
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
        jmp    SHORT loc_4_134A
loc_4_1362:
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_136B:
        mov    ax, 3408h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_4_139B
        mov    edx, 8
        call   sub_4_1168
        jae    SHORT loc_4_139B
        db  0A1h, 048h, 003h, 000h, 000h
        mov    dword ptr [edi + 34h], eax
        xor    ax, ax
        mov    word ptr [ebp + 1ch], ax
        jmp    NEAR PTR loc_4_0591
loc_4_139B:
        db  0BEh, 035h, 017h, 000h, 000h
        call   sub_4_1609
        mov    word ptr [ebp + 1ch], ax
        or     eax, eax
        jne    SHORT loc_4_13C5
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        cmp    dword ptr [edi + 24h], 1
        jbe    SHORT loc_4_13C5
        push   1
        push   edi
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
loc_4_13C5:
        jmp    NEAR PTR loc_4_0591
loc_4_13CA:
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    ebx, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    ecx, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    edx, eax
        mov    si, word ptr [ebp + 30h]
        mov    di, word ptr [ebp + 34h]
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    eax, edi
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, esi
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, edx
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, ecx
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        xor    eax, eax
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  066h, 08Bh, 00Dh, 05Ah, 003h, 000h, 000h
        db  00Fh, 0B7h, 015h, 058h, 003h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        add    word ptr [ebp + 30h], 2
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    ebx, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    ecx, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    edx, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    esi, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    edi, eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    word ptr [ebp + 34h], di
        mov    word ptr [ebp + 30h], si
        mov    word ptr [ebp + 2ch], dx
        mov    word ptr [ebp + 28h], cx
        mov    word ptr [ebp + 24h], bx
        ret
loc_4_1482:
        mov    word ptr [ebp + 1ch], 0
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   ebx, eax
        cmp    ebx, -1
        je     SHORT loc_4_14A8
        or     word ptr [ebx + 8], 4000h
        db  0CDh, 020h, 01Ah, 000h, 02Ah, 000h
        mov    word ptr [ebp + 1ch], ax
loc_4_14A8:
        jmp    NEAR PTR loc_4_0591
VWIN32_SysErrorBox:
        push   esi
        push   edi
        push   ecx
        push   ebx
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        pop    ebx
        je     SHORT loc_4_14C7
        xor    eax, eax
        push   eax
        push   eax
        jmp    SHORT loc_4_14ED
loc_4_14C7:
        db  08Bh, 03Dh, 0D8h, 010h, 000h, 000h
        push   edi
        db  0FFh, 035h, 0A4h, 002h, 000h, 000h
        call   sub_4_449B
        add    esp, 8
        push   eax
        push   edi
        db  0FFh, 035h, 09Ch, 002h, 000h, 000h
        call   sub_4_449B
        add    esp, 8
        push   eax
loc_4_14ED:
        push   ebx
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        mov    esi, edi
        pop    ebx
        jecxz  loc_4_1511
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        cmp    edi, esi
        je     SHORT loc_4_1507
        xor    ecx, ecx
        jmp    SHORT loc_4_1511
loc_4_1507:
        push   ecx
loc_4_1508:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        loop   loc_4_1508
        pop    ecx
loc_4_1511:
        push   ecx
        mov    dword ptr [ebx], 0ffffffffh
        db  0A1h, 03Ch, 003h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_154C
        push   eax
        push   ebx
        db  0FFh, 035h, 034h, 003h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        db  0FFh, 035h, 03Ch, 003h, 000h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_4_1541:
        lea    esi, [ebx]
        db  0E8h, 0ACh, 0ECh, 0FBh, 0FFh
        jae    SHORT loc_4_1541
        mov    esi, eax
loc_4_154C:
        pop    ecx
        jecxz  loc_4_1570
loc_4_154F:
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        loop   loc_4_154F
        push   esi
        push   edx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xor    edx, edx
        xor    esi, esi
        mov    eax, 0fh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    edx
        pop    esi
loc_4_1570:
        db  08Bh, 03Dh, 0D8h, 010h, 000h, 000h
        pop    eax
        push   eax
        push   edi
        db  0FFh, 035h, 09Ch, 002h, 000h, 000h
        call   sub_4_44ED
        add    esp, 0ch
        pop    eax
        push   eax
        push   edi
        db  0FFh, 035h, 0A4h, 002h, 000h, 000h
        call   sub_4_44ED
        add    esp, 0ch
        mov    eax, esi
        pop    ebx
        pop    ecx
        pop    edi
        pop    esi
        ret
loc_4_159F:
        push   ebp
        mov    ebp, esp
        db  0BEh, 007h, 017h, 000h, 000h
        call   sub_4_1609
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 1ch], eax
        leave
        ret    14h
_VWIN32_FaultPopup:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], -1
        je     SHORT loc_4_15E9
        cmp    dword ptr [ebp + 0ch], 1fh
        jae    SHORT loc_4_15E9
        db  0CDh, 020h, 000h, 000h, 002h, 000h
        or     eax, eax
        je     SHORT loc_4_15E9
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    cx, 12h
        db  0CDh, 020h, 002h, 000h, 002h, 000h
        mov    eax, 1
        jb     SHORT loc_4_1604
loc_4_15E9:
        db  0BEh, 0F2h, 017h, 000h, 000h
        call   sub_4_1609
        cmp    eax, 1
        jne    SHORT loc_4_1604
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 001h, 000h, 002h, 000h
loc_4_1604:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_1609:
        push   0
        push   16bh
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_16FD
        mov    ebx, eax
        call   esi
        mov    edx, 8
        db  066h, 0A1h, 060h, 003h, 000h, 000h
        cmp    ax, word ptr [ebx + 0c8h]
        jne    SHORT loc_4_164D
        db  066h, 0A1h, 062h, 003h, 000h, 000h
        cmp    ax, word ptr [ebx + 0d4h]
        jne    SHORT loc_4_164D
        mov    edx, 10h
loc_4_164D:
        call   sub_4_1186
        mov    eax, 0
        jb     NEAR PTR loc_4_16ED
        call   sub_4_181E
        db  083h, 03Dh, 03Ch, 003h, 000h, 000h, 000h
        je     SHORT loc_4_1681
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0CDh, 020h, 00Bh, 001h, 001h, 000h
        je     SHORT loc_4_1681
        db  03Bh, 03Dh, 03Ch, 003h, 000h, 000h
        jne    SHORT loc_4_16A9
loc_4_1681:
        push   ebx
        mov    esi, dword ptr [ebx + 4]
        mov    edi, dword ptr [ebx + 0c4h]
        mov    ebx, dword ptr [ebx + 0c8h]
        movzx  edx, byte ptr [ebx + 16ah]
        xor    edx, 4
        and    edx, 4
        or     edx, 1
        db  0E8h, 0A6h, 0EAh, 0FBh, 0FFh
        pop    ebx
        jmp    SHORT loc_4_16ED
loc_4_16A9:
        db  08Bh, 03Dh, 0E0h, 010h, 000h, 000h
        mov    dword ptr [ebx], edi
        mov    dword ptr [ebx + 164h], 0ffffffffh
        db  0FFh, 035h, 03Ch, 003h, 000h, 000h
        push   ebx
        db  0FFh, 035h, 02Ch, 003h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        db  0FFh, 035h, 03Ch, 003h, 000h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        lea    esi, [ebx + 164h]
        db  0E8h, 009h, 0EBh, 0FBh, 0FFh
        jae    SHORT loc_4_16FE
loc_4_16ED:
        mov    esi, eax
        push   0
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, esi
loc_4_16FD:
        ret
loc_4_16FE:
        or     byte ptr [ebx + 16ah], 8
        jmp    SHORT loc_4_16FD
loc_4_1707:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebx + 4], eax
        xor    eax, eax
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [ebp + 18h], eax
        je     SHORT loc_4_1719
        or     al, 2
loc_4_1719:
        mov    byte ptr [ebx + 16ah], al
        mov    esi, dword ptr [ebp + 10h]
        or     esi, esi
        je     NEAR PTR loc_4_1808
        lea    edi, [ebx + 0ch]
        mov    ecx, 33h
        rep movsd
        ret
loc_4_1735:
        mov    al, byte ptr [ebp + 1ch]
        and    al, 1
        or     al, 4
        mov    byte ptr [ebx + 16ah], al
        cmp    edi, -1
        je     NEAR PTR loc_4_17F1
        movzx  eax, word ptr [edi + 28h]
        mov    dword ptr [ebx + 4], eax
        movzx  eax, word ptr [edi + 32h]
        mov    dword ptr [ebx + 8], eax
        movzx  eax, word ptr [edi]
        mov    dword ptr [ebx + 98h], eax
        movzx  eax, word ptr [edi + 2]
        mov    dword ptr [ebx + 9ch], eax
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [ebx + 0a0h], eax
        movzx  eax, word ptr [edi + 6]
        mov    dword ptr [ebx + 0a4h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0a8h], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 0ach], eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebx + 0b0h], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ebx + 0b4h], eax
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [ebx + 0bch], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 0c0h], eax
        movzx  eax, word ptr [edi + 34h]
        mov    dword ptr [ebx + 0c4h], eax
        movzx  eax, word ptr [edi + 36h]
        mov    dword ptr [ebx + 0c8h], eax
        movzx  eax, word ptr [edi + 38h]
        mov    dword ptr [ebx + 0cch], eax
        movzx  eax, word ptr [edi + 3ah]
        mov    dword ptr [ebx + 0d0h], eax
        movzx  eax, word ptr [edi + 3ch]
        mov    dword ptr [ebx + 0d4h], eax
loc_4_17F1:
        ret
loc_4_17F2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 4], eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebx + 8], eax
        mov    byte ptr [ebx + 16ah], 4
loc_4_1808:
        lea    eax, [ebx + 0ch]
        mov    dword ptr [eax], 0ffffffffh
        push   eax
        push   0
        db  0CDh, 020h, 003h, 000h, 02Ah, 000h
        add    esp, 8
        ret
sub_4_181E:
        mov    esi, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [esi + 120h], 0
        mov    dword ptr [esi + 10ch], 0
        push   0
        push   dword ptr [esi + 0d4h]
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_1885
        mov    edx, dword ptr [esi + 0d0h]
        lar    ecx, word ptr [esi + 0d4h]
        test   ecx, 400000h
        jne    SHORT loc_4_1869
        movzx  edx, dx
loc_4_1869:
        add    eax, edx
        lea    edi, [esi + 124h]
        lea    edx, [esi + 120h]
        push   edx
        push   40h
        push   edi
        push   eax
        db  0CDh, 020h, 005h, 000h, 02Ah, 000h
        add    esp, 10h
loc_4_1885:
        push   0
        push   dword ptr [esi + 0c8h]
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_18BC
        add    eax, dword ptr [esi + 0c4h]
        lea    edi, [esi + 110h]
        lea    edx, [esi + 10ch]
        push   edx
        push   10h
        push   edi
        push   eax
        db  0CDh, 020h, 005h, 000h, 02Ah, 000h
        add    esp, 10h
loc_4_18BC:
        mov    ebx, esi
        ret
sub_4_18BF:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 0E0h, 010h, 000h, 000h
        or     byte ptr [esi + 2dh], 42h
        push   35h
        db  0FFh, 035h, 0FCh, 010h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        db  0FEh, 005h, 075h, 004h, 000h, 000h
        jne    SHORT loc_4_194F
        db  0BFh, 050h, 011h, 000h, 000h
        mov    ecx, 3ch
        xor    al, al
        rep stosb
        db  08Bh, 01Dh, 020h, 011h, 000h, 000h
loc_4_18F9:
        lea    edi, [ebx + 8]
        mov    ecx, 24h
        xor    al, al
        rep stosb
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_4_18F9
        db  0BFh, 020h, 011h, 000h, 000h
        lea    ebx, [edi + 30h]
        db  0A1h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 07Fh, 001h, 001h, 000h
        db  0A1h, 0DCh, 010h, 000h, 000h
        add    eax, 60h
        mov    dword ptr [edi + 8], eax
        mov    ebx, dword ptr [edi]
loc_4_192B:
        call   sub_4_1958
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_4_192B
        call   sub_4_1B19
        db  08Bh, 01Dh, 020h, 011h, 000h, 000h
loc_4_1941:
        mov    edx, dword ptr [ebx + 4]
        call   sub_4_2C1E
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_4_1941
loc_4_194F:
        db  0B8h, 020h, 011h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_1958:
        push   edi
        mov    eax, 7d0h
        mov    edx, ebx
        db  0BEh, 01Ch, 002h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        mov    dword ptr [ebx + 18h], esi
        mov    edx, dword ptr [ebx + 4]
        call   sub_4_2BAF
        mov    edi, dword ptr [ebx + 8]
        or     edi, edi
        je     SHORT loc_4_1982
        call   sub_4_1D59
loc_4_1982:
        xor    esi, esi
        xchg   dword ptr [ebx + 18h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    edi
        ret
loc_4_198F:
        lea    esi, [edx + 20h]
        db  0C7h, 046h, 004h, 0A3h, 019h, 000h, 000h
        mov    dword ptr [esi + 8], edx
        push   esi
        call   sub_4_1E85
        ret
loc_4_19A3:
        mov    edi, dword ptr [edx + 4]
        mov    edi, dword ptr [edi + 8]
        or     edi, edi
        je     SHORT loc_4_19CD
        mov    eax, 0bb8h
        db  0BEh, 042h, 002h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        mov    dword ptr [edx + 18h], esi
        mov    dword ptr [edx + 8], edi
        db  0B9h, 0DCh, 019h, 000h, 000h
        call   sub_4_1CBF
loc_4_19CD:
        ret
loc_4_19CE:
        xor    edi, edi
        xchg   dword ptr [edx + 8], edi
        or     edi, edi
        je     SHORT loc_4_19DC
        call   sub_4_1D59
loc_4_19DC:
        lea    esi, [edx + 20h]
        db  0C7h, 046h, 004h, 0F0h, 019h, 000h, 000h
        mov    dword ptr [esi + 8], edx
        push   esi
        call   sub_4_1E85
        ret
loc_4_19F0:
        xor    esi, esi
        xchg   dword ptr [edx + 18h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    ebx, edx
loc_4_19FD:
        mov    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_4_1A5B
        mov    dword ptr [ebx + 0ch], eax
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [ebx + 14h], esi
        mov    edi, dword ptr [eax + 60h]
        mov    ecx, dword ptr [edi + 28h]
        mov    dword ptr [ebx + 1ch], ecx
        mov    dword ptr [edi + 28h], 0
        or     byte ptr [edi + 2dh], 4
        mov    edi, dword ptr [eax + 5ch]
        or     edi, edi
        je     SHORT loc_4_1A38
        push   edi
        call   sub_4_1BC1
        mov    ecx, dword ptr [edi + 10h]
        or     byte ptr [ecx + 2dh], 2
loc_4_1A38:
        mov    edx, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx + 0ch]
        call   sub_4_2C24
        dec    esi
        jne    SHORT loc_4_1A38
        mov    edi, dword ptr [ebx + 0ch]
        jmp    SHORT loc_4_1A55
loc_4_1A4B:
        mov    eax, dword ptr [ebx + 4]
        mov    eax, dword ptr [eax + 8]
        cmp    eax, edi
        je     SHORT loc_4_19FD
loc_4_1A55:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_4_1A4B
loc_4_1A5B:
        ret
sub_4_1A5C:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        je     SHORT loc_4_1AB0
        db  0FEh, 00Dh, 075h, 004h, 000h, 000h
        jns    SHORT loc_4_1A97
        mov    esi, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edi]
loc_4_1A75:
        call   sub_4_1AB4
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_4_1A75
        mov    dword ptr [edi + 8], 0
        push   edi
        call   sub_4_1B7A
        pop    edi
        lea    ebx, [edi + 30h]
        db  0CDh, 020h, 080h, 001h, 001h, 000h
loc_4_1A97:
        db  08Bh, 035h, 0E0h, 010h, 000h, 000h
        and    byte ptr [esi + 2dh], 0bdh
        db  0FFh, 035h, 0FCh, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
loc_4_1AB0:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_1AB4:
        push   edi
        mov    edi, dword ptr [ebx + 14h]
        or     edi, edi
        je     SHORT loc_4_1AE1
        mov    ecx, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [ecx + 40h]
        mov    ecx, dword ptr [ecx + 60h]
        and    byte ptr [ecx + 2dh], 0fbh
        cmp    eax, esi
        je     SHORT loc_4_1AE1
        mov    eax, dword ptr [ebx + 1ch]
        add    dword ptr [ecx + 28h], eax
loc_4_1AD3:
        mov    edx, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx + 0ch]
        call   sub_4_2BB5
        dec    edi
        jne    SHORT loc_4_1AD3
loc_4_1AE1:
        pop    edi
        ret
loc_4_1AE3:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    edx, edx
        xor    esi, esi
        mov    eax, 0fh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        ret    8
loc_4_1AFB:
        push   dword ptr [esp + 0ch]
        call   sub_4_1C1B
        ret    0ch
loc_4_1B07:
        call   sub_4_1B19
        ret    8
loc_4_1B0F:
        xor    esi, esi
        call   sub_4_1B7A
        ret    8
sub_4_1B19:
        db  0FEh, 005h, 066h, 003h, 000h, 000h
        jne    SHORT loc_4_1B79
        xor    esi, esi
loc_4_1B23:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
loc_4_1B29:
        db  0CDh, 020h, 00Ch, 001h, 001h, 000h
        jb     SHORT loc_4_1B23
        db  0CDh, 020h, 013h, 001h, 001h, 000h
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        je     SHORT loc_4_1B79
        mov    ebx, dword ptr [edi + 14h]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_1B29
        mov    eax, edi
        db  003h, 005h, 0ECh, 010h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        db  03Bh, 005h, 04Ch, 003h, 000h, 000h
        je     SHORT loc_4_1B29
        push   edi
        call   sub_4_1BC1
        inc    esi
        test   esi, 0fh
        jne    SHORT loc_4_1B29
        xor    eax, eax
        inc    eax
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        jmp    SHORT loc_4_1B29
loc_4_1B79:
        ret
sub_4_1B7A:
        db  0FEh, 00Dh, 066h, 003h, 000h, 000h
        jns    SHORT loc_4_1BC0
loc_4_1B82:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
loc_4_1B88:
        db  0CDh, 020h, 00Ch, 001h, 001h, 000h
        jb     SHORT loc_4_1B82
        db  0CDh, 020h, 013h, 001h, 001h, 000h
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        je     SHORT loc_4_1BC0
        mov    ebx, dword ptr [edi + 14h]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_1B88
        mov    eax, edi
        db  003h, 005h, 0ECh, 010h, 000h, 000h
        mov    eax, dword ptr [eax]
        cmp    esi, dword ptr [eax + 4]
        je     SHORT loc_4_1B88
        push   edi
        call   sub_4_1C1B
        jmp    SHORT loc_4_1B88
loc_4_1BC0:
        ret
sub_4_1BC1:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ebx, edi
        db  003h, 01Dh, 0ECh, 010h, 000h, 000h
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_4_1C15
        test   byte ptr [ebx + 2dh], 2
        jne    SHORT loc_4_1C15
        bts    dword ptr [ebx + 2ch], 8
        jb     SHORT loc_4_1C15
        push   edi
        db  0E8h, 0FBh, 0EAh, 0FBh, 0FFh
        add    esp, 4
        test   byte ptr [ebx + 2ch], 4
        je     SHORT loc_4_1C15
        xor    eax, eax
        xchg   dword ptr [ebx + 24h], eax
        or     eax, eax
        je     SHORT loc_4_1C00
        db  0E8h, 044h, 0EEh, 0FBh, 0FFh
loc_4_1C00:
        push   3
        push   edi
        push   0
        db  068h, 0A5h, 006h, 000h, 000h
        db  0E8h, 094h, 0EFh, 0FBh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebx + 24h], eax
loc_4_1C15:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_4_1C1B:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ebx, edi
        db  003h, 01Dh, 0ECh, 010h, 000h, 000h
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_4_1C6F
        test   byte ptr [ebx + 2dh], 4
        jne    SHORT loc_4_1C6F
        btr    dword ptr [ebx + 2ch], 8
        jae    SHORT loc_4_1C6F
        test   byte ptr [ebx + 2ch], 4
        je     SHORT loc_4_1C66
        xor    eax, eax
        xchg   dword ptr [ebx + 24h], eax
        or     eax, eax
        je     SHORT loc_4_1C51
        db  0E8h, 0F3h, 0EDh, 0FBh, 0FFh
loc_4_1C51:
        push   2
        push   edi
        push   0
        db  068h, 0A5h, 006h, 000h, 000h
        db  0E8h, 043h, 0EFh, 0FBh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebx + 24h], eax
loc_4_1C66:
        push   edi
        db  0E8h, 0D2h, 0EAh, 0FBh, 0FFh
        add    esp, 4
loc_4_1C6F:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_4_1C75:
        push   0
        push   2ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_1CB5
        mov    edx, dword ptr [esp + 10h]
        db  08Bh, 035h, 020h, 011h, 000h, 000h
        mov    ebx, 0ffff0badh
loc_4_1C95:
        cmp    edx, dword ptr [esi + 10h]
        jb     SHORT loc_4_1CA2
        mov    ebx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_4_1C95
loc_4_1CA2:
        mov    dword ptr [eax], esi
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 10h], edx
        db  0B8h, 020h, 011h, 000h, 000h
loc_4_1CB5:
        mov    ebx, dword ptr [esp + 4]
        mov    dword ptr [ebx + 1ch], eax
        ret    10h
sub_4_1CBF:
        mov    esi, dword ptr [edi + 60h]
        mov    edi, dword ptr [edi + 5ch]
        or     edi, edi
        je     SHORT loc_4_1D1A
sub_4_1CC9:
        cmp    dword ptr [esi + 68h], 0
        jne    SHORT loc_4_1D1A
        and    byte ptr [esi + 2dh], 0efh
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_4_1D1A
        test   byte ptr [esi + 2dh], 8
        jne    SHORT loc_4_1D1A
        test   byte ptr [esi + 2ch], 10h
        jne    SHORT loc_4_1D1A
        mov    eax, dword ptr [esi + 4]
        db  03Bh, 005h, 04Ch, 003h, 000h, 000h
        je     SHORT loc_4_1D1A
        mov    dword ptr [esi + 68h], ecx
        mov    dword ptr [esi + 64h], edx
        mov    edx, esi
        mov    ebx, edi
        xor    eax, eax
        mov    edi, 1388h
        mov    ecx, 1d0h
        db  00Bh, 00Dh, 01Ch, 011h, 000h, 000h
        db  0BEh, 01Bh, 01Dh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [edx + 6ch], esi
loc_4_1D1A:
        ret
loc_4_1D1B:
        mov    ecx, 0
        xchg   dword ptr [edx + 6ch], ecx
        jecxz  loc_4_1D51
        jb     SHORT loc_4_1D51
        btr    dword ptr [edx + 2ch], 0ch
        jb     SHORT loc_4_1D51
        mov    ax, word ptr [ebp + 28h]
        db  066h, 03Bh, 005h, 060h, 003h, 000h, 000h
        je     SHORT loc_4_1D51
        db  0CDh, 020h, 0C1h, 000h, 001h, 000h
        je     SHORT loc_4_1D4C
        xor    bx, bx
        mov    ax, 20h
        int    41h
loc_4_1D4C:
        or     byte ptr [ebp + 2dh], 1
        ret
loc_4_1D51:
        mov    dword ptr [edx + 68h], 0
        ret
sub_4_1D59:
        mov    eax, dword ptr [edi + 60h]
        mov    edi, dword ptr [edi + 5ch]
        or     edi, edi
        je     SHORT loc_4_1D78
        xor    esi, esi
        xchg   dword ptr [eax + 6ch], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        cmp    dword ptr [eax + 68h], 0
        je     SHORT loc_4_1D78
        or     byte ptr [eax + 2dh], 10h
loc_4_1D78:
        ret
sub_4_1D79:
        cmp    esi, 4
        jne    NEAR PTR loc_4_1E83
        cmp    dword ptr [edi + 68h], 0
        je     NEAR PTR loc_4_1E83
        btr    dword ptr [edi + 2ch], 0ch
        jb     NEAR PTR loc_4_1E70
        cmp    dword ptr [edi + 28h], 0
        je     NEAR PTR loc_4_1E52
        lar    eax, word ptr [ebp + 34h]
        test   eax, 400000h
        jne    NEAR PTR loc_4_1E7D
        lar    eax, word ptr [ebp + 28h]
        test   eax, 400000h
        jne    NEAR PTR loc_4_1E7D
        mov    ax, word ptr [ebp + 28h]
        mov    cx, ax
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 24h]
        db  03Bh, 00Dh, 054h, 003h, 000h, 000h
        je     SHORT loc_4_1E52
        db  066h, 03Bh, 005h, 064h, 003h, 000h, 000h
        je     NEAR PTR loc_4_1E7D
        db  066h, 03Bh, 005h, 0AEh, 010h, 000h, 000h
        je     NEAR PTR loc_4_1E7D
        cmp    ax, word ptr [edi + 70h]
        je     NEAR PTR loc_4_1E7D
        mov    word ptr [edi + 70h], ax
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], ax
        and    byte ptr [ebp + 2dh], 0feh
        db  066h, 08Bh, 00Dh, 052h, 003h, 000h, 000h
        db  00Fh, 0B7h, 015h, 050h, 003h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        or     ax, ax
        jne    SHORT loc_4_1E7D
loc_4_1E52:
        mov    word ptr [edi + 70h], 0
        and    byte ptr [ebp + 2dh], 0feh
        xor    ecx, ecx
        xchg   dword ptr [edi + 68h], ecx
        jecxz  loc_4_1E81
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [edi + 64h]
        call   ecx
        pop    edi
        pop    esi
        pop    ebx
        jmp    SHORT loc_4_1E81
loc_4_1E70:
        and    byte ptr [ebp + 2dh], 0feh
        mov    dword ptr [edi + 68h], 0
        jmp    SHORT loc_4_1E81
loc_4_1E7D:
        or     byte ptr [ebp + 2dh], 1
loc_4_1E81:
        stc
        ret
loc_4_1E83:
        clc
        ret
sub_4_1E85:
        sub    esp, 0cch
        push   ebx
        push   esi
        push   edi
        push   35h
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        db  0CDh, 020h, 008h, 001h, 001h, 000h
loc_4_1EA5:
        db  0CDh, 020h, 013h, 001h, 001h, 000h
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        je     NEAR PTR loc_4_1F6C
        mov    ebx, dword ptr [edi + 14h]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_1EA5
        mov    edx, edi
        db  003h, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     SHORT loc_4_1EA5
        test   byte ptr [edx + 2dh], 40h
        jne    SHORT loc_4_1EA5
        test   byte ptr [edx + 2dh], 20h
        jne    SHORT loc_4_1EA5
        lea    eax, [esp + 0ch]
        mov    dword ptr [esp + 0ch], 10001h
        push   eax
        push   edi
        db  0CDh, 020h, 003h, 000h, 02Ah, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_1EA5
        push   0
        push   dword ptr [esp + 0cch]
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_1EA5
        add    eax, dword ptr [esp + 0c4h]
        db  0BEh, 0ACh, 003h, 000h, 000h
loc_4_1F1B:
        db  081h, 0FEh, 06Ch, 004h, 000h, 000h
        je     SHORT loc_4_1F61
        cmp    eax, dword ptr [esi]
        jb     SHORT loc_4_1F5C
        cmp    eax, dword ptr [esi + 4]
        ja     SHORT loc_4_1F5C
        cmp    eax, dword ptr [esi + 8]
        je     SHORT loc_4_1F61
        call   sub_4_209B
        mov    ebx, dword ptr [esi + 0ch]
        mov    byte ptr [ebx], 0cch
        mov    edx, edi
        db  003h, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx]
        bts    dword ptr [edx + 2ch], 0dh
        jb     NEAR PTR loc_4_1EA5
        db  0FFh, 005h, 06Ch, 004h, 000h, 000h
        jmp    NEAR PTR loc_4_1EA5
loc_4_1F5C:
        add    esi, 10h
        jmp    SHORT loc_4_1F1B
loc_4_1F61:
        push   edi
        call   sub_4_1BC1
        jmp    NEAR PTR loc_4_1EA5
loc_4_1F6C:
        mov    esi, dword ptr [esp + 0dch]
        mov    edx, esi
        db  087h, 035h, 05Ch, 003h, 000h, 000h
        mov    dword ptr [edx], esi
        db  0FEh, 005h, 074h, 004h, 000h, 000h
        db  083h, 03Dh, 070h, 004h, 000h, 000h, 000h
        jne    SHORT loc_4_1F91
        call   sub_4_200A
loc_4_1F91:
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0cch
        ret    4
sub_4_1FAC:
        cmp    esi, 0ch
        jne    SHORT loc_4_1FE2
        movzx  eax, word ptr [ebp + 28h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_1FE2
        add    eax, dword ptr [ebp + 24h]
        dec    eax
        db  0BAh, 0ACh, 003h, 000h, 000h
loc_4_1FD0:
        db  081h, 0FAh, 06Ch, 004h, 000h, 000h
        je     SHORT loc_4_1FE2
        cmp    eax, dword ptr [edx + 0ch]
        je     SHORT sub_4_1FE4
        add    edx, 10h
        jmp    SHORT loc_4_1FD0
loc_4_1FE2:
        clc
        ret
sub_4_1FE4:
        db  08Bh, 015h, 0E0h, 010h, 000h, 000h
        btr    dword ptr [edx + 2ch], 0dh
        jae    NEAR PTR loc_4_2099
        db  0FFh, 00Dh, 06Ch, 004h, 000h, 000h
        jne    NEAR PTR loc_4_2086
        db  083h, 03Dh, 070h, 004h, 000h, 000h, 000h
        jne    SHORT loc_4_2086
sub_4_200A:
        db  083h, 03Dh, 06Ch, 004h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_2099
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        db  087h, 035h, 05Ch, 003h, 000h, 000h
loc_4_2022:
        or     esi, esi
        je     SHORT loc_4_2058
        db  0FEh, 00Dh, 074h, 004h, 000h, 000h
        push   35h
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        mov    edx, dword ptr [esi + 8]
        push   esi
        call   dword ptr [esi + 4]
        pop    esi
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        mov    esi, dword ptr [esi]
        jmp    SHORT loc_4_2022
loc_4_2058:
        db  0FEh, 005h, 066h, 003h, 000h, 000h
        call   sub_4_1B7A
        db  0FFh, 035h, 04Ch, 003h, 000h, 000h
        call   sub_4_4581
        db  0BBh, 0ACh, 003h, 000h, 000h
loc_4_2073:
        db  081h, 0FBh, 06Ch, 004h, 000h, 000h
        je     SHORT loc_4_2096
        mov    esi, dword ptr [ebx + 0ch]
        mov    byte ptr [esi], 90h
        add    ebx, 10h
        jmp    SHORT loc_4_2073
loc_4_2086:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        push   edi
        call   sub_4_1BC1
        pop    edi
        jmp    SHORT loc_4_2099
loc_4_2096:
        pop    edi
        pop    esi
        pop    ebx
loc_4_2099:
        stc
        ret
sub_4_209B:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 0C1h, 000h, 001h, 000h
        je     SHORT loc_4_20AF
        xor    bx, bx
        mov    ax, 20h
        int    41h
loc_4_20AF:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h
sub_4_20B4:
        enter  4, 0
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        or     dword ptr [edi], 0c0000h
        call   sub_4_236D
        or     eax, eax
        je     SHORT loc_4_2143
        push   10h
        push   8fh
        push   0bff70h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_2143
        db  0BEh, 08Ch, 011h, 000h, 000h
        mov    ebx, 1
loc_4_20F1:
        mov    ax, 3da0h
        mov    edx, dword ptr [esi + 4]
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2129
        mov    ecx, dword ptr [esi + 0ch]
        db  066h, 089h, 004h, 04Dh, 09Ch, 011h, 000h, 000h
        call   sub_4_2147
        or     eax, eax
        je     SHORT loc_4_2129
        add    esi, 10h
        dec    ebx
        jne    SHORT loc_4_20F1
        mov    eax, 1
loc_4_2121:
        and    dword ptr [edi], 0fff3ffffh
        leave
        ret
loc_4_2129:
        call   sub_4_2433
        push   10h
        push   0bff70000h
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        call   sub_4_245A
loc_4_2143:
        sub    eax, eax
        jmp    SHORT loc_4_2121
sub_4_2147:
        enter  0ch, 0
        push   ebx
        push   edi
        push   60040000h
        mov    eax, dword ptr [esi + 0ch]
        shl    eax, 16h
        ror    eax, 3
        push   eax
        db  0FFh, 035h, 0A6h, 011h, 000h, 000h
        push   1
        mov    eax, dword ptr [esi + 8]
        shr    eax, 0ch
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     NEAR PTR loc_4_22DD
        call   sub_4_22E1
        jb     NEAR PTR loc_4_22DD
loc_4_2187:
        test   dword ptr [ebx + 24h], 80000000h
        je     SHORT loc_4_21A8
        mov    dword ptr [ebp - 0ch], 60060000h
        db  0A1h, 0AAh, 011h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_4_21BF
loc_4_21A8:
        mov    dword ptr [ebp - 0ch], 60040000h
        db  0A1h, 0A6h, 011h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 0AEh, 011h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
loc_4_21BF:
        test   dword ptr [ebx + 24h], 80h
        je     SHORT loc_4_21E3
        mov    eax, dword ptr [ebx + 10h]
        or     eax, eax
        jne    SHORT loc_4_21D2
        mov    eax, dword ptr [ebx + 8]
loc_4_21D2:
        add    eax, 0fffh
        and    eax, 0fffff000h
        xor    edx, edx
        jmp    NEAR PTR loc_4_22A3
loc_4_21E3:
        cmp    dword ptr [ebx + 10h], 1000h
        jb     SHORT loc_4_2226
        push   dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebx + 14h]
        shr    eax, 9
        mov    edx, dword ptr [esi + 0ch]
        shl    edx, 16h
        or     eax, edx
        ror    eax, 3
        push   eax
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebx + 10h]
        shr    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        add    eax, dword ptr [esi + 8]
        shr    eax, 0ch
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     NEAR PTR loc_4_22DD
loc_4_2226:
        mov    eax, dword ptr [ebx + 10h]
        test   eax, 0fffh
        je     SHORT loc_4_2281
        push   dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebx + 10h]
        and    eax, 0fffff000h
        add    eax, dword ptr [ebx + 14h]
        shr    eax, 9
        mov    edx, dword ptr [esi + 0ch]
        shl    edx, 16h
        or     edx, eax
        mov    eax, dword ptr [ebx + 10h]
        shr    eax, 9
        neg    eax
        and    eax, 7
        shl    eax, 13h
        or     eax, edx
        ror    eax, 3
        push   eax
        push   dword ptr [ebp - 4]
        push   1
        mov    eax, dword ptr [ebx + 10h]
        and    eax, 0fffff000h
        add    eax, dword ptr [ebx + 0ch]
        add    eax, dword ptr [esi + 8]
        shr    eax, 0ch
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_4_22DD
loc_4_2281:
        jmp    SHORT loc_4_22CA
        db  08Bh, 043h, 008h, 005h, 0FFh, 00Fh, 000h, 000h, 025h, 000h, 0F0h, 0FFh, 0FFh, 08Bh, 053h, 010h
        db  081h, 0C2h, 0FFh, 00Fh, 000h, 000h, 081h, 0E2h, 000h, 0F0h, 0FFh, 0FFh, 02Bh, 0C2h, 076h, 027h
loc_4_22A3:
        shr    eax, 0ch
        mov    ecx, eax
        push   dword ptr [ebp - 0ch]
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebx + 0ch]
        add    eax, dword ptr [esi + 8]
        add    eax, edx
        shr    eax, 0ch
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_4_22DD
loc_4_22CA:
        add    ebx, 28h
        dec    edi
        jne    NEAR PTR loc_4_2187
        mov    eax, 1
loc_4_22D9:
        pop    edi
        pop    ebx
        leave
        ret
loc_4_22DD:
        sub    eax, eax
        jmp    SHORT loc_4_22D9
sub_4_22E1:
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 3ch]
        cmp    eax, 0f40h
        ja     SHORT loc_4_236B
        add    edx, eax
        cmp    dword ptr [edx], 4550h
        jne    SHORT loc_4_236B
        mov    eax, dword ptr [edx + 34h]
        cmp    eax, dword ptr [esi + 8]
        jne    SHORT loc_4_236B
        movzx  eax, word ptr [edx + 6]
        imul   eax, eax, 28h
        add    eax, 18h
        movzx  ecx, word ptr [edx + 14h]
        add    eax, ecx
        add    eax, edx
        sub    eax, dword ptr [esi + 8]
        cmp    eax, 1000h
        ja     SHORT loc_4_236B
        mov    ebx, 18h
        movzx  ecx, word ptr [edx + 14h]
        add    ebx, ecx
        add    ebx, edx
        movzx  ecx, word ptr [edx + 6]
loc_4_232D:
        test   dword ptr [ebx + 10h], 1ffh
        jne    SHORT loc_4_236B
        mov    eax, dword ptr [ebx + 14h]
        add    eax, dword ptr [ebx + 10h]
        cmp    eax, 8f000h
        ja     SHORT loc_4_236B
        mov    eax, dword ptr [ebx + 0ch]
        test   eax, 0fffh
        jne    SHORT loc_4_236B
        add    eax, dword ptr [ebx + 8]
        cmp    eax, 8f000h
        ja     SHORT loc_4_236B
        add    ebx, 28h
        loop   loc_4_232D
        lea    ebx, [edx + 18h]
        movzx  ecx, word ptr [edx + 14h]
        add    ebx, ecx
        movzx  edi, word ptr [edx + 6]
        clc
        ret
loc_4_236B:
        stc
        ret
sub_4_236D:
        push   ebx
        db  068h, 085h, 004h, 000h, 000h
        push   1
        db  0CDh, 020h, 021h, 001h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2406
        db  08Bh, 01Dh, 085h, 004h, 000h, 000h
        db  0B8h, 068h, 002h, 000h, 000h
        db  0A3h, 085h, 004h, 000h, 000h
        db  068h, 085h, 004h, 000h, 000h
        db  0CDh, 020h, 020h, 001h, 001h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_2406
        db  0A3h, 0AAh, 011h, 000h, 000h
        sub    eax, eax
        db  0C7h, 005h, 089h, 004h, 000h, 000h, 068h, 002h, 000h, 000h
        db  0A3h, 091h, 004h, 000h, 000h
        db  0A3h, 099h, 004h, 000h, 000h
        db  0A3h, 09Dh, 004h, 000h, 000h
        db  0C7h, 005h, 0A1h, 004h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 085h, 004h, 000h, 000h
        db  0CDh, 020h, 020h, 001h, 001h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_2408
        db  0A3h, 0A6h, 011h, 000h, 000h
        db  089h, 01Dh, 085h, 004h, 000h, 000h
        db  068h, 085h, 004h, 000h, 000h
        db  0CDh, 020h, 020h, 001h, 001h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_2408
        db  0A3h, 0AEh, 011h, 000h, 000h
loc_4_2406:
        pop    ebx
        ret
loc_4_2408:
        db  0FFh, 035h, 0AAh, 011h, 000h, 000h
        db  0CDh, 020h, 022h, 001h, 001h, 000h
        add    esp, 4
        db  083h, 03Dh, 0A6h, 011h, 000h, 000h, 000h
        je     SHORT loc_4_242F
        db  0FFh, 035h, 0A6h, 011h, 000h, 000h
        db  0CDh, 020h, 022h, 001h, 001h, 000h
        add    esp, 4
loc_4_242F:
        sub    eax, eax
        jmp    SHORT loc_4_2406
sub_4_2433:
        mov    ecx, 1
        sub    ebx, ebx
loc_4_243A:
        db  066h, 08Bh, 004h, 05Dh, 09Ch, 011h, 000h, 000h
        cmp    ax, -1
        je     SHORT loc_4_2456
        push   ebx
        mov    ebx, eax
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
loc_4_2456:
        inc    ebx
        loop   loc_4_243A
        ret
sub_4_245A:
        db  0FFh, 035h, 0A6h, 011h, 000h, 000h
        db  0CDh, 020h, 022h, 001h, 001h, 000h
        add    esp, 4
        db  0FFh, 035h, 0AAh, 011h, 000h, 000h
        db  0CDh, 020h, 022h, 001h, 001h, 000h
        add    esp, 4
        db  0FFh, 035h, 0AEh, 011h, 000h, 000h
        db  0CDh, 020h, 022h, 001h, 001h, 000h
        add    esp, 4
        ret
sub_4_2488:
        db  0A1h, 0BCh, 011h, 000h, 000h
        mov    ecx, 35h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        db  08Bh, 015h, 0E0h, 010h, 000h, 000h
        test   byte ptr [edx + 2dh], 1
        jne    SHORT loc_4_24B1
        cmp    dword ptr [edx + 28h], 0
        jne    SHORT loc_4_24B0
        cmp    dword ptr [edx + 20h], 0
        jne    SHORT loc_4_24B1
loc_4_24B0:
        ret
loc_4_24B1:
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0E8h, 0B6h, 0E2h, 0FBh, 0FFh
        pop    edi
        or     byte ptr [edx + 2ch], 4
        push   eax
        db  0E8h, 0D7h, 0E1h, 0FBh, 0FFh
        jmp    SHORT sub_4_2488
sub_4_24D0:
        db  0A1h, 0BCh, 011h, 000h, 000h
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        ret
sub_4_24DC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_4_24F5
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     SHORT loc_4_24F5
        mov    edx, dword ptr [eax + 4]
        db  089h, 015h, 018h, 011h, 000h, 000h
loc_4_24F5:
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 004h, 011h, 000h, 000h
        sub    ecx, ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jb     SHORT loc_4_2516
        db  0A3h, 0BCh, 011h, 000h, 000h
loc_4_2514:
        leave
        ret
loc_4_2516:
        sub    eax, eax
        jmp    SHORT loc_4_2514
sub_4_251A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 010h, 011h, 000h, 000h
        mov    eax, 31h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        leave
        ret
loc_4_2532:
        db  0C7h, 005h, 010h, 011h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 32h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        ret    8
sub_4_254A:
        push   ebp
        mov    ebp, esp
        db  0A1h, 0BCh, 011h, 000h, 000h
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        leave
        ret
sub_4_255A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   1
        push   79h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_25E0
        mov    esi, eax
        sub    ecx, ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jb     SHORT loc_4_25D4
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4], eax
        or     byte ptr [esi + 62h], 2
        or     eax, eax
        jne    SHORT loc_4_25A9
        xor    ecx, ecx
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        push   ebx
        jmp    SHORT loc_4_25AF
loc_4_25A9:
        push   eax
        call   sub_4_4312
loc_4_25AF:
        mov    word ptr [esi + 76h], ax
        test   dword ptr [ebp + 14h], 2
        je     SHORT loc_4_25C0
        or     dword ptr [esi + 2ch], 40h
loc_4_25C0:
        test   dword ptr [ebp + 14h], 1
        je     SHORT loc_4_25CD
        or     dword ptr [esi + 2ch], 20h
loc_4_25CD:
        mov    eax, esi
loc_4_25CF:
        pop    esi
        leave
        ret    10h
loc_4_25D4:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_25E0:
        sub    eax, eax
        jmp    SHORT loc_4_25CF
sub_4_25E4:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 18h]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        pop    edi
        leave
        ret    4
loc_4_25F9:
        push   edi
        pop    edi
        ret
sub_4_25FC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 0ECh, 010h, 000h, 000h
        mov    edi, dword ptr [edi]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_255A
        or     eax, eax
        je     SHORT loc_4_265D
        mov    dword ptr [edi + 0ch], eax
        mov    ecx, dword ptr [ebp + 28h]
        mov    dword ptr [ecx], eax
        or     byte ptr [eax + 2eh], 2
        push   eax
        db  068h, 0F9h, 025h, 000h, 000h
        push   2ah
        push   0
        push   0
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        db  0CDh, 020h, 005h, 001h, 001h, 000h
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_4_2662
        mov    ecx, dword ptr [edi + 0ch]
        and    byte ptr [ecx + 2eh], 0fdh
loc_4_265D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_2662:
        push   dword ptr [edi + 0ch]
        call   sub_4_25E4
        mov    dword ptr [edi + 0ch], 0
        sub    eax, eax
        stc
        jmp    SHORT loc_4_265D
loc_4_2676:
        mov    ebx, dword ptr [ebp + 10h]
        xor    eax, eax
        db  0A3h, 014h, 011h, 000h, 000h
        db  0A2h, 0C0h, 011h, 000h, 000h
        push   dword ptr [ebx]
        db  0FFh, 035h, 09Ch, 002h, 000h, 000h
        call   sub_4_45C3
        db  0CDh, 020h, 00Ah, 001h, 001h, 000h
        push   0
        push   edi
        db  0E8h, 096h, 0E1h, 0FBh, 0FFh
        add    esp, 8
        push   ebx
        call   sub_4_26AD
        add    esp, 4
        ret
sub_4_26AD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx]
        push   dword ptr [esi + 40h]
        push   dword ptr [ebx + 14h]
        call   sub_4_1A5C
        add    esp, 8
        mov    edi, dword ptr [esi + 64h]
        or     edi, edi
        je     SHORT loc_4_26E2
        test   dword ptr [esi + 44h], 800h
        je     SHORT loc_4_26E2
        push   10h
        push   edi
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_26E2:
        xor    edi, edi
        xchg   dword ptr [esi + 5ch], edi
        db  08Bh, 015h, 0ECh, 010h, 000h, 000h
        mov    esi, dword ptr [edi + edx]
        mov    dword ptr [esi + 0ch], ebx
        push   edi
        db  0CDh, 020h, 007h, 001h, 001h, 000h
        add    esp, 4
loc_4_26FD:
        mov    edx, dword ptr [esp + 4]
        xor    eax, eax
        xchg   dword ptr [edx + 0ch], eax
        or     eax, eax
        je     SHORT loc_4_2717
        mov    edi, dword ptr [edx]
        call   sub_4_1D59
        push   edx
        call   sub_4_431D
loc_4_2717:
        ret    4
loc_4_271A:
        xor    eax, eax
        xchg   dword ptr [edx + 10h], eax
        or     eax, eax
        je     SHORT loc_4_273F
        mov    ebx, dword ptr [edx]
        mov    ebx, dword ptr [ebx + 5ch]
        xor    eax, eax
        mov    edi, 2710h
        mov    ecx, 130h
        db  0BEh, 040h, 027h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_4_273F:
        ret
loc_4_2740:
        jb     SHORT loc_4_2781
        xor    eax, eax
        xchg   dword ptr [edx + 0ch], eax
        or     eax, eax
        je     SHORT loc_4_2765
        mov    ebx, edx
        db  0E8h, 0F2h, 0E2h, 0FBh, 0FFh
        mov    edi, dword ptr [ebx]
        call   sub_4_1D59
        db  0CDh, 020h, 070h, 001h, 001h, 000h
        push   ebx
        call   sub_4_431D
loc_4_2765:
        ret
loc_4_2766:
        xor    eax, eax
        xchg   dword ptr [edx + 0ch], eax
        or     eax, eax
        je     SHORT loc_4_2781
        mov    edi, dword ptr [edx]
        mov    edi, dword ptr [edi + 5ch]
        mov    ebx, edx
        db  0E8h, 0C9h, 0E2h, 0FBh, 0FFh
        push   ebx
        call   sub_4_431D
loc_4_2781:
        ret
sub_4_2782:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        db  08Bh, 035h, 0ECh, 010h, 000h, 000h
        add    esi, edi
        mov    esi, dword ptr [esi]
        or     byte ptr [esi + 2eh], 4
        xor    eax, eax
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [ebx + 0ch], eax
        mov    edx, esi
        db  0E8h, 0CEh, 0DFh, 0FBh, 0FFh
        push   2
        push   edi
        push   ebx
        db  068h, 0FDh, 026h, 000h, 000h
        db  0E8h, 0EAh, 0E3h, 0FBh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 40h]
        test   dword ptr [eax + 20h], 80h
        je     SHORT loc_4_27FF
        push   esi
        mov    eax, 1388h
        mov    edx, ebx
        db  0BEh, 01Ah, 027h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebx + 10h], esi
        pop    esi
        push   ebx
        push   esi
        push   edi
        db  0B9h, 066h, 027h, 000h, 000h
        call   sub_4_1CC9
        pop    edi
        pop    esi
        pop    ebx
        mov    eax, 800000h
        db  0CDh, 020h, 014h, 001h, 001h, 000h
loc_4_27FF:
        or     byte ptr [esi + 2dh], 2
        and    byte ptr [esi + 2dh], 0fbh
        push   edi
        call   sub_4_1C1B
        cmp    dword ptr [esi + 1ch], 0
        jg     SHORT loc_4_281E
        push   -1
        push   edi
        db  0E8h, 01Bh, 0E0h, 0FBh, 0FFh
        add    esp, 8
loc_4_281E:
        xor    eax, eax
        xchg   dword ptr [esi + 44h], eax
        or     eax, eax
        je     SHORT loc_4_2831
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_4_2831:
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_4_2847
        mov    dword ptr [esi + 20h], 1
        push   edi
        db  0E8h, 0FAh, 0DEh, 0FBh, 0FFh
        add    esp, 4
loc_4_2847:
        btr    dword ptr [esi + 2ch], 4
        jae    SHORT loc_4_285E
        lea    eax, [esi + 289816h]
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_4_285E:
        and    byte ptr [esi + 2eh], 0fbh
        lea    eax, [esi + 569595h]
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2877:
        push   edi
        push   esi
        push   ebx
        push   eax
        lea    edx, [eax + 0ch]
        mov    edi, edx
        mov    esi, dword ptr [ebx + 34h]
        mov    ah, byte ptr [esi - 4]
        cmp    ah, 0fh
        ja     SHORT loc_4_288D
        xor    ah, ah
loc_4_288D:
        mov    ecx, dword ptr [edx]
        jecxz  loc_4_28A0
        mov    esi, dword ptr [ecx + 34h]
        mov    al, byte ptr [esi - 4]
        cmp    ah, al
        jg     SHORT loc_4_28A0
        lea    edx, [ecx + 54h]
        jmp    SHORT loc_4_288D
loc_4_28A0:
        cmp    edi, edx
        jne    SHORT loc_4_28E4
        lea    eax, [edi - 0ch]
        db  03Bh, 005h, 09Ch, 002h, 000h, 000h
        je     SHORT loc_4_28E4
        push   edx
        mov    edx, eax
        jecxz  loc_4_28C1
        mov    edi, dword ptr [ecx + 5ch]
        or     edi, edi
        je     SHORT loc_4_28C1
        db  0CDh, 020h, 06Ah, 001h, 001h, 000h
loc_4_28C1:
        push   ebx
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 5ch]
        or     ebx, ebx
        je     SHORT loc_4_28E2
        mov    edi, dword ptr [edx + 8]
        or     edi, edi
        je     SHORT loc_4_28E2
        mov    edi, dword ptr [edi + 5ch]
        or     edi, edi
        je     SHORT loc_4_28E2
        db  0CDh, 020h, 063h, 001h, 001h, 000h
loc_4_28E2:
        pop    ebx
        pop    edx
loc_4_28E4:
        mov    dword ptr [ebx + 54h], ecx
        mov    dword ptr [edx], ebx
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_4_28EE:
        push   eax
        push   esi
        push   edi
        lea    edx, [eax + 0ch]
        cmp    dword ptr [edx], ebx
        jne    SHORT loc_4_292F
        mov    esi, eax
        mov    edi, dword ptr [ebx + 5ch]
        db  0CDh, 020h, 06Ah, 001h, 001h, 000h
        mov    edi, dword ptr [esi + 8]
        or     edi, edi
        je     SHORT loc_4_292F
        mov    ecx, dword ptr [ebx + 54h]
        jecxz  loc_4_292F
        push   ebx
        mov    edx, esi
        mov    edi, dword ptr [edi + 5ch]
        or     edi, edi
        je     SHORT loc_4_292B
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 5ch]
        or     ebx, ebx
        je     SHORT loc_4_292B
        db  0CDh, 020h, 063h, 001h, 001h, 000h
loc_4_292B:
        pop    ebx
        lea    edx, [esi + 0ch]
loc_4_292F:
        mov    ecx, dword ptr [edx]
        jecxz  loc_4_2943
        cmp    ecx, ebx
        je     SHORT loc_4_293C
        lea    edx, [ecx + 54h]
        jmp    SHORT loc_4_292F
loc_4_293C:
        xor    ecx, ecx
        xchg   dword ptr [ebx + 54h], ecx
        mov    dword ptr [edx], ecx
loc_4_2943:
        pop    edi
        pop    esi
        pop    eax
        ret
sub_4_2947:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   35h
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
loc_4_295F:
        mov    ebx, dword ptr [eax + 0ch]
        or     ebx, ebx
        je     SHORT loc_4_297F
        call   sub_4_28EE
        push   eax
        push   80h
        push   dword ptr [ebx + 5ch]
        db  0E8h, 0BDh, 0DEh, 0FBh, 0FFh
        add    esp, 8
        pop    eax
        jmp    SHORT loc_4_295F
loc_4_297F:
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        pop    ebx
        leave
        ret
sub_4_2991:
        push   ebp
        mov    ebp, esp
        push   ebx
loc_4_2995:
        push   35h
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        db  08Bh, 01Dh, 0D8h, 010h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
loc_4_29AF:
        cmp    dword ptr [eax + 4], -1
        je     NEAR PTR loc_4_2A91
        mov    dword ptr [ebx + 58h], 2
        call   sub_4_2877
loc_4_29C5:
        or     byte ptr [eax + 3], 80h
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        call   sub_4_1FE4
        push   -1
        db  0E8h, 0ACh, 0DDh, 0FBh, 0FFh
        add    esp, 4
        push   35h
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        db  08Bh, 015h, 0E0h, 010h, 000h, 000h
        and    byte ptr [edx + 2ch], 0fdh
        mov    edx, dword ptr [edx + 14h]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_4_2A16
        and    byte ptr [eax + 3], 7fh
        jmp    NEAR PTR loc_4_2AC6
loc_4_2A16:
        cmp    edx, 40000001h
        je     SHORT loc_4_2A36
        cmp    edx, -1
        jne    SHORT loc_4_2A7E
        mov    edx, dword ptr [ebx + 60h]
        cmp    dword ptr [edx + 28h], 0
        jne    SHORT loc_4_2A7E
        call   sub_4_28EE
        jmp    NEAR PTR loc_4_2ACC
loc_4_2A36:
        mov    edx, dword ptr [ebx + 60h]
        test   byte ptr [edx + 2dh], 1
        jne    SHORT loc_4_2A53
        cmp    dword ptr [edx + 20h], 0
        je     NEAR PTR loc_4_29AF
        cmp    dword ptr [edx + 28h], 0
        jne    NEAR PTR loc_4_29AF
loc_4_2A53:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0E8h, 01Ah, 0DDh, 0FBh, 0FFh
        pop    edi
        or     byte ptr [edx + 2ch], 4
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        push   eax
        db  0E8h, 02Ch, 0DCh, 0FBh, 0FFh
        jmp    NEAR PTR loc_4_2995
loc_4_2A7E:
        cmp    dword ptr [eax + 4], -1
        jne    NEAR PTR loc_4_29C5
        and    byte ptr [eax + 3], 7fh
        call   sub_4_28EE
loc_4_2A91:
        push   edi
        push   eax
        mov    ebx, dword ptr [eax + 0ch]
        db  03Bh, 005h, 09Ch, 002h, 000h, 000h
        je     SHORT loc_4_2AC0
        or     ebx, ebx
        je     SHORT loc_4_2AC0
        mov    edx, eax
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [edi + 5ch]
        or     edi, edi
        je     SHORT loc_4_2AC0
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 5ch]
        or     ebx, ebx
        je     SHORT loc_4_2AC0
        db  0CDh, 020h, 063h, 001h, 001h, 000h
loc_4_2AC0:
        pop    eax
        pop    edi
        add    dword ptr [eax + 4], 2
loc_4_2AC6:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 8], ebx
loc_4_2ACC:
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        clc
        pop    ebx
        leave
        ret
sub_4_2ADF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   35h
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
loc_4_2AF4:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0ch]
        or     ecx, ecx
        je     SHORT loc_4_2B6C
        mov    ebx, ecx
        mov    edx, dword ptr [ebx + 60h]
        test   byte ptr [edx + 2dh], 1
        jne    SHORT loc_4_2B15
        cmp    dword ptr [edx + 20h], 0
        je     SHORT loc_4_2B2C
        cmp    dword ptr [edx + 28h], 0
        jne    SHORT loc_4_2B2C
loc_4_2B15:
        call   sub_4_28EE
        push   40000001h
        push   dword ptr [ebx + 5ch]
        db  0E8h, 00Fh, 0DDh, 0FBh, 0FFh
        add    esp, 8
        jmp    SHORT loc_4_2AF4
loc_4_2B2C:
        push   edi
        push   eax
        mov    edi, dword ptr [ebx + 5ch]
        db  0CDh, 020h, 06Ah, 001h, 001h, 000h
        pop    eax
        pop    edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 34h]
        mov    edx, dword ptr [edx]
        mov    ecx, dword ptr [ebx + 34h]
        cmp    dword ptr [ecx], edx
        ja     SHORT loc_4_2B50
        jb     SHORT loc_4_2B5C
        test   byte ptr [eax + 3], 80h
        je     SHORT loc_4_2B5C
loc_4_2B50:
        call   sub_4_28EE
        mov    dword ptr [eax + 8], ebx
        add    dword ptr [eax + 4], 2
loc_4_2B5C:
        push   0
        push   dword ptr [ebx + 5ch]
        db  0E8h, 0D0h, 0DCh, 0FBh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
loc_4_2B6C:
        dec    dword ptr [eax + 4]
        db  0FFh, 035h, 0F8h, 010h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        clc
        pop    ebx
        leave
        ret
loc_4_2B82:
        db  0FFh, 035h, 0D8h, 010h, 000h, 000h
        push   dword ptr [esp + 10h]
        call   sub_4_2991
        add    esp, 8
        ret    0ch
loc_4_2B97:
        db  0FFh, 035h, 0D8h, 010h, 000h, 000h
        push   dword ptr [esp + 10h]
        call   sub_4_2ADF
        add    esp, 8
        ret    0ch
VWIN32_EnterCrstR0:
        mov    edx, dword ptr [edx + 0ch]
sub_4_2BAF:
        db  08Bh, 00Dh, 0D8h, 010h, 000h, 000h
sub_4_2BB5:
        db  0FFh, 005h, 070h, 004h, 000h, 000h
        dec    dword ptr [edx + 10h]
        je     SHORT loc_4_2BFD
        cmp    dword ptr [edx + 8], ecx
        je     SHORT loc_4_2C00
        db  0FFh, 00Dh, 070h, 004h, 000h, 000h
        db  03Bh, 015h, 0ACh, 002h, 000h, 000h
        je     SHORT loc_4_2BE5
        push   ecx
        push   edx
        call   sub_4_2991
        add    esp, 8
        db  0FFh, 005h, 070h, 004h, 000h, 000h
        jmp    SHORT loc_4_2C03
loc_4_2BE5:
        push   ecx
        db  0A1h, 0BCh, 011h, 000h, 000h
        mov    ecx, 35h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        pop    ecx
        db  0FFh, 005h, 070h, 004h, 000h, 000h
loc_4_2BFD:
        mov    dword ptr [edx + 8], ecx
loc_4_2C00:
        inc    dword ptr [edx + 4]
loc_4_2C03:
        db  0FFh, 00Dh, 070h, 004h, 000h, 000h
        jne    SHORT loc_4_2C19
        db  080h, 03Dh, 074h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_2C19
        call   sub_4_200A
loc_4_2C19:
        clc
        ret
VWIN32_LeaveCrstR0:
        mov    edx, dword ptr [edx + 0ch]
sub_4_2C1E:
        db  08Bh, 00Dh, 0D8h, 010h, 000h, 000h
sub_4_2C24:
        db  0FFh, 005h, 070h, 004h, 000h, 000h
        dec    dword ptr [edx + 4]
        je     SHORT loc_4_2C34
        inc    dword ptr [edx + 10h]
        jmp    SHORT loc_4_2C5D
loc_4_2C34:
        xor    eax, eax
        mov    dword ptr [edx + 8], eax
        inc    dword ptr [edx + 10h]
        jg     SHORT loc_4_2C5D
        db  03Bh, 015h, 0ACh, 002h, 000h, 000h
        je     SHORT loc_4_2C52
        push   ecx
        push   edx
        call   sub_4_2ADF
        add    esp, 8
        jmp    SHORT loc_4_2C5D
loc_4_2C52:
        db  0A1h, 0BCh, 011h, 000h, 000h
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_4_2C5D:
        db  0FFh, 00Dh, 070h, 004h, 000h, 000h
        jne    SHORT loc_4_2C73
        db  080h, 03Dh, 074h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_2C73
        call   sub_4_200A
loc_4_2C73:
        clc
        ret
sub_4_2C75:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2C8C:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 067h, 001h, 001h, 000h
        pop    edi
        leave
        ret
sub_4_2C9F:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 062h, 001h, 001h, 000h
        pop    edi
        leave
        ret
sub_4_2CB5:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 068h, 001h, 001h, 000h
        pop    edi
        leave
        ret
sub_4_2CC8:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 06Dh, 001h, 001h, 000h
        pop    edi
        leave
        ret
sub_4_2CDB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 063h, 001h, 001h, 000h
        pop    edi
        pop    ebx
        leave
        ret
sub_4_2CF6:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 06Ah, 001h, 001h, 000h
        pop    edi
        leave
        ret
sub_4_2D09:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 065h, 001h, 001h, 000h
        leave
        ret
sub_4_2D17:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 06Bh, 001h, 001h, 000h
        pop    edi
        leave
        ret
sub_4_2D2A:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 18h]
        mov    esi, dword ptr [ebx + 14h]
        push   dword ptr [ebx + 4]
        or     esi, esi
        je     SHORT loc_4_2D57
        or     byte ptr [edi + 3], 10h
        push   dword ptr [ebx]
        cmp    dword ptr [ebx + 10h], 0
        jne    SHORT loc_4_2D5A
        push   dword ptr [ebx + 0ch]
        db  0E8h, 004h, 0DCh, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_2D66
loc_4_2D57:
        push   dword ptr [ebx + 10h]
loc_4_2D5A:
        push   0
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_2D66:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        pop    ecx
        call   ecx
        add    esp, 4
        pop    edi
        leave
        ret
_VWIN32_CreateRing0Thread:
        push   ebx
        push   edx
        push   ecx
        push   1
        push   1ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    ecx
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_4_2E3F
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 10h], esi
        mov    ebx, eax
        or     esi, esi
        je     SHORT loc_4_2DB7
        push   -1
        push   ebx
        db  0FFh, 035h, 004h, 011h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        jmp    NEAR PTR loc_4_2E3F
loc_4_2DB7:
        db  0E8h, 0FCh, 0DAh, 0FBh, 0FFh
        or     eax, eax
        je     SHORT loc_4_2E2B
        push   eax
        mov    dword ptr [ebx + 0ch], eax
        push   -1
        push   ebx
        db  0FFh, 035h, 004h, 011h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        db  08Bh, 015h, 09Ch, 002h, 000h, 000h
        db  0A1h, 0D8h, 010h, 000h, 000h
        cmp    dword ptr [edx + 8], eax
        jne    SHORT loc_4_2E1A
        mov    ecx, dword ptr [edx + 4]
        push   ecx
loc_4_2DEA:
        push   edx
        push   ecx
        push   eax
        push   eax
        push   edx
        call   sub_4_45C3
        pop    eax
        pop    ecx
        pop    edx
        loop   loc_4_2DEA
        push   edx
        push   eax
        push   dword ptr [ebx + 0ch]
        db  0E8h, 021h, 0DBh, 0FBh, 0FFh
        add    esp, 4
        pop    eax
        pop    edx
        pop    ecx
loc_4_2E09:
        push   edx
        push   ecx
        push   eax
        push   eax
        push   edx
        call   sub_4_45A9
        pop    eax
        pop    ecx
        pop    edx
        loop   loc_4_2E09
        jmp    SHORT loc_4_2E25
loc_4_2E1A:
        push   dword ptr [ebx + 0ch]
        db  0E8h, 002h, 0DBh, 0FBh, 0FFh
        add    esp, 4
loc_4_2E25:
        db  0E8h, 0CDh, 0DAh, 0FBh, 0FFh
        pop    eax
loc_4_2E2B:
        push   dword ptr [ebx + 14h]
        push   dword ptr [ebx + 18h]
        push   0
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        pop    eax
loc_4_2E3F:
        pop    ebx
        ret
loc_4_2E41:
        cmp    dword ptr [edi + 2ch], 2ah
        jne    SHORT loc_4_2E9D
        db  0A1h, 0ECh, 010h, 000h, 000h
        lea    esi, [edi + eax]
        mov    edx, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        cmp    dword ptr [edi + 2ch], 2ah
        jne    SHORT loc_4_2E9D
        mov    edi, dword ptr [edi + eax]
        xchg   edi, edx
        mov    ebx, dword ptr [edx + 0ch]
        mov    dword ptr [esi], ebx
        mov    eax, dword ptr [ebx + 4]
        inc    word ptr [eax + 32h]
        mov    ax, word ptr [ebx + 76h]
        mov    word ptr [edi + 2ah], ax
        mov    esi, dword ptr [edi + 14h]
        push   0
        push   eax
        push   esi
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 100000h
        jb     SHORT loc_4_2E90
        sub    eax, dword ptr [esi + 4]
loc_4_2E90:
        shr    eax, 4
        mov    word ptr [ebx + 76h], ax
        lea    eax, [edi + 2ah]
        mov    dword ptr [ebx + 40h], eax
loc_4_2E9D:
        clc
        ret
loc_4_2E9F:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        cmp    dword ptr [edi + 2ch], 2ah
        jne    SHORT loc_4_2EDA
        db  08Bh, 035h, 0ECh, 010h, 000h, 000h
        mov    esi, dword ptr [edi + esi]
        mov    eax, 2
        bt     dword ptr [esi + 2ch], 5
        jae    SHORT loc_4_2EC5
        mov    eax, 4
loc_4_2EC5:
        db  0CDh, 020h, 004h, 000h, 011h, 000h
        mov    eax, dword ptr [esi + 2ch]
        shr    eax, 6
        and    eax, 1
        db  0CDh, 020h, 008h, 000h, 011h, 000h
loc_4_2EDA:
        clc
        ret
loc_4_2EDC:
        cmp    dword ptr [edi + 2ch], 2ah
        jne    SHORT loc_4_2F2A
        db  03Bh, 03Dh, 010h, 011h, 000h, 000h
        jne    SHORT loc_4_2EF4
        db  0C7h, 005h, 010h, 011h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_2EF4:
        db  03Bh, 03Dh, 03Ch, 003h, 000h, 000h
        jne    SHORT loc_4_2F06
        db  0C7h, 005h, 03Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_2F06:
        db  08Bh, 01Dh, 0ECh, 010h, 000h, 000h
        mov    ebx, dword ptr [edi + ebx]
        mov    eax, dword ptr [ebx + 4]
        dec    word ptr [eax + 32h]
        jne    SHORT loc_4_2F2A
        or     dword ptr [eax + 20h], 800000h
        push   0
        db  0CDh, 020h, 024h, 001h, 001h, 000h
        add    esp, 4
loc_4_2F2A:
        clc
        ret
loc_4_2F2C:
        cmp    dword ptr [edi + 2ch], 2ah
        jne    SHORT loc_4_2F64
        db  08Bh, 015h, 0ECh, 010h, 000h, 000h
        mov    esi, dword ptr [edi + edx]
        test   byte ptr [esi + 2eh], 2
        jne    SHORT loc_4_2F64
        mov    dword ptr [edi + edx], 0
        push   esi
        call   sub_4_25E4
        mov    ebx, dword ptr [esi + 0ch]
        or     ebx, ebx
        je     SHORT loc_4_2F64
        push   -1
        push   ebx
        push   dword ptr [ebx + 8]
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
loc_4_2F64:
        clc
        ret
sub_4_2F66:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_4_2F74:
        mov    eax, 5
        mov    ecx, 10h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  089h, 035h, 0C8h, 011h, 000h, 000h
        ret
sub_4_2F8B:
        push   ebx
        push   esi
        push   edi
        db  0BEh, 089h, 00Ch, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        db  0A3h, 0C4h, 011h, 000h, 000h
        call   sub_4_2F74
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h
loc_4_2FA8:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   _VWIN32_Get_Thread_Context
        mov    ecx, dword ptr [esp + 0ch]
        add    esp, 8
        mov    dword ptr [ecx + 1ch], eax
        ret    10h
_VWIN32_Get_Thread_Context:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_4_2FD9
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp + 8], edi
        pop    edi
loc_4_2FD9:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2ch], 2ah
        je     SHORT loc_4_2FE9
        xor    eax, eax
        jmp    NEAR PTR loc_4_31EE
loc_4_2FE9:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        test   byte ptr [edx], 10h
        mov    ecx, dword ptr [eax + 10h]
        je     SHORT loc_4_301E
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dr0
        mov    dword ptr [ebx + 4], eax
        mov    eax, dr1
        mov    dword ptr [ebx + 8], eax
        mov    eax, dr2
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dr3
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dr6
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dr7
        mov    dword ptr [ebx + 18h], eax
loc_4_301E:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax], 8
        je     SHORT loc_4_3040
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        lea    esi, [esi + 1ch]
        db  0CDh, 020h, 005h, 000h, 011h, 000h
        xor    eax, eax
        db  0CDh, 020h, 003h, 000h, 011h, 000h
        mov    dword ptr [esi + 6ch], eax
loc_4_3040:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        je     SHORT loc_4_3081
        test   al, 4
        je     SHORT loc_4_30B5
        test   byte ptr [ecx + 2eh], 2
        je     SHORT loc_4_3081
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 68h]
        mov    dword ptr [edx + 8ch], eax
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 64h]
        mov    dword ptr [edx + 90h], eax
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 5ch]
        mov    dword ptr [edx + 94h], eax
        movzx  eax, word ptr [ecx + 60h]
        jmp    SHORT loc_4_30AC
loc_4_3081:
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 44h]
        mov    dword ptr [edx + 8ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [ecx + 40h]
        mov    dword ptr [eax + 90h], edx
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 38h]
        mov    dword ptr [edx + 94h], eax
        movzx  eax, word ptr [ecx + 3ch]
loc_4_30AC:
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 98h], eax
loc_4_30B5:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax], 2
        je     SHORT loc_4_3104
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 9ch], eax
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0a0h], eax
        mov    eax, dword ptr [ecx + 14h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0a8h], eax
        mov    eax, dword ptr [ecx + 18h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0ach], eax
        mov    eax, dword ptr [ecx + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0a4h], eax
        mov    eax, dword ptr [ecx + 1ch]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0b0h], eax
loc_4_3104:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        jne    SHORT loc_4_3145
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 1001fh
loc_4_3117:
        mov    eax, dword ptr [ecx + 24h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0b8h], eax
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 28h]
        mov    dword ptr [edx + 0bch], eax
        mov    eax, dword ptr [ecx + 30h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0c4h], eax
        movzx  eax, word ptr [ecx + 34h]
        jmp    NEAR PTR loc_4_31C8
loc_4_3145:
        test   al, 1
        je     NEAR PTR loc_4_31E9
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_4_3193
        test   byte ptr [ecx + 2eh], 2
        je     SHORT loc_4_3117
        mov    eax, dword ptr [ecx + 48h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0b8h], eax
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 4ch]
        mov    dword ptr [edx + 0bch], eax
        mov    eax, dword ptr [ecx + 54h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0c4h], eax
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 58h]
        mov    dword ptr [edx + 0c8h], eax
        mov    eax, dword ptr [ecx + 50h]
        jmp    SHORT loc_4_31D4
loc_4_3193:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [edx + 0b8h], eax
        mov    eax, dword ptr [ebp + 8]
        movzx  edx, word ptr [eax + 28h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0bch], edx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 1ch]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0c4h], eax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 1ah]
loc_4_31C8:
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0c8h], eax
        mov    eax, dword ptr [ecx + 2ch]
loc_4_31D4:
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0c0h], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 0b4h], eax
loc_4_31E9:
        mov    eax, 1
loc_4_31EE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_31F3:
        push   esi
        push   dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   _VWIN32_Set_Thread_Context
        add    esp, 8
        test   esi, esi
        je     SHORT loc_4_3215
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [esi + 10h], ecx
        je     SHORT loc_4_3215
        mov    dword ptr [ecx + 1ch], eax
loc_4_3215:
        pop    esi
        ret    10h
_VWIN32_Set_Thread_Context:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_4_3230
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp + 8], edi
        pop    edi
loc_4_3230:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2ch], 2ah
        je     SHORT loc_4_3240
        xor    eax, eax
        jmp    NEAR PTR loc_4_3469
loc_4_3240:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        test   byte ptr [edx], 10h
        mov    ecx, dword ptr [eax + 10h]
        je     SHORT loc_4_3275
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 4]
        mov    dr0, eax
        mov    eax, dword ptr [ebx + 8]
        mov    dr1, eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dr2, eax
        mov    eax, dword ptr [ebx + 10h]
        mov    dr3, eax
        mov    eax, dword ptr [ebx + 14h]
        mov    dr6, eax
        mov    eax, dword ptr [ebx + 18h]
        mov    dr7, eax
loc_4_3275:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax], 8
        je     SHORT loc_4_328C
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        lea    esi, [esi + 1ch]
        db  0CDh, 020h, 006h, 000h, 011h, 000h
loc_4_328C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        je     SHORT loc_4_32D6
        test   al, 4
        je     SHORT loc_4_330A
        test   byte ptr [ecx + 2eh], 2
        je     SHORT loc_4_32D6
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 8ch]
        mov    word ptr [ecx + 68h], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 90h]
        mov    word ptr [ecx + 64h], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 94h]
        mov    word ptr [ecx + 5ch], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 98h]
        mov    word ptr [ecx + 60h], dx
        jmp    SHORT loc_4_330A
loc_4_32D6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 8ch]
        mov    word ptr [ecx + 44h], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 90h]
        mov    word ptr [ecx + 40h], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 94h]
        mov    word ptr [ecx + 38h], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 98h]
        mov    word ptr [ecx + 3ch], dx
loc_4_330A:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax], 2
        je     SHORT loc_4_3356
        mov    edx, dword ptr [eax + 9ch]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0a0h]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0a8h]
        mov    dword ptr [ecx + 14h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0ach]
        mov    dword ptr [ecx + 18h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0a4h]
        mov    dword ptr [ecx + 10h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0b0h]
        mov    dword ptr [ecx + 1ch], edx
loc_4_3356:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        je     NEAR PTR loc_4_340E
        test   al, 1
        je     NEAR PTR loc_4_3464
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_4_33CE
        test   byte ptr [ecx + 2eh], 2
        je     NEAR PTR loc_4_340E
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0b8h]
        mov    dword ptr [ecx + 48h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0bch]
        mov    word ptr [ecx + 4ch], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0c4h]
        mov    dword ptr [ecx + 54h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0c8h]
        mov    word ptr [ecx + 58h], dx
        mov    edx, dword ptr [ecx + 50h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0c0h]
        xor    eax, edx
        and    eax, 0dd5h
        xor    eax, edx
        mov    dword ptr [ecx + 50h], eax
        jmp    NEAR PTR loc_4_3458
loc_4_33CE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        mov    dword ptr [edx + 20h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0bch]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 28h], dx
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 0c4h]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 1ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [edx + 1ah], ax
        jmp    SHORT loc_4_3440
loc_4_340E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0b8h]
        mov    dword ptr [ecx + 24h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0bch]
        mov    word ptr [ecx + 28h], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0c4h]
        mov    dword ptr [ecx + 30h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0c8h]
        mov    word ptr [ecx + 34h], dx
loc_4_3440:
        mov    edx, dword ptr [ecx + 2ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0c0h]
        xor    eax, edx
        and    eax, 0dd5h
        xor    eax, edx
        mov    dword ptr [ecx + 2ch], eax
loc_4_3458:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0b4h]
        mov    dword ptr [ecx + 8], edx
loc_4_3464:
        mov    eax, 1
loc_4_3469:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_346E:
        sub    esp, 4
        mov    dword ptr [esp], 0ffffffffh
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    edi, ebx
        and    edi, 0fffh
        mov    ebp, dword ptr [esp + 1ch]
        add    edi, ebp
        mov    esi, ebx
        shr    esi, 0ch
        add    edi, 0fffh
        shr    edi, 0ch
        push   20000000h
        push   -1
        push   edi
        push   esi
        call   sub_4_0ED7
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_4_34ED
        test   eax, 20000h
        jne    SHORT loc_4_34D7
        test   byte ptr [esp + 26h], 2
        je     SHORT loc_4_34D7
        lea    eax, [ebx + ebp]
        cmp    eax, 80000000h
        jae    SHORT loc_4_34ED
        push   ebp
        push   ebx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_34ED
loc_4_34D7:
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   edi
        push   esi
        call   sub_4_0ED7
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
loc_4_34ED:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    10h
loc_4_34FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp - 14h], edi
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 10h], eax
        jne    SHORT loc_4_3530
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_4_346E
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        jmp    NEAR PTR loc_4_35DD
loc_4_3530:
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 14h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_4_356C
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], 0ffffffffh
        jmp    SHORT loc_4_35DD
loc_4_356C:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0ffffffffh
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ecx + 0ch], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, 10h
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, 102h
        mov    edx, dword ptr [ebp - 4]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_35BE:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], -2
        jne    SHORT loc_4_35D2
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], 0ffffffffh
        jmp    SHORT loc_4_35DD
loc_4_35D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx + 1ch], edx
loc_4_35DD:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_35E5:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp - 1ch], edi
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [ebp + 10h], eax
        jne    SHORT loc_4_361A
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_4_0EEF
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        jmp    NEAR PTR loc_4_36F8
loc_4_361A:
        mov    esi, dword ptr [ebp + 1ch]
        cmp    esi, 1ch
        lea    eax, [esi + 4]
        jae    SHORT loc_4_362A
        mov    eax, 4
loc_4_362A:
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 14h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_4_3665
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], 0
        jmp    NEAR PTR loc_4_36F8
loc_4_3665:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0ffffffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], esi
        mov    eax, 10h
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, 102h
        mov    edx, dword ptr [ebp - 4]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_36A2:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax]
        push   dword ptr [ebp + 18h]
        add    eax, 4
        push   eax
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_4_36C9
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx + 1ch], edx
        jmp    SHORT loc_4_36D3
loc_4_36C9:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], 0
loc_4_36D3:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 20h], eax
loc_4_36F8:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_4_3700:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        mov    eax, dword ptr [ebp + 1ch]
        push   esi
        add    eax, 0ch
        push   edi
        mov    dword ptr [ebp - 14h], eax
        xor    edi, edi
        mov    dword ptr [ebp - 18h], edi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, edi
        jne    SHORT loc_4_3743
        mov    edi, 8
        jmp    NEAR PTR loc_4_37F6
loc_4_3743:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 8], eax
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], 0ffffffffh
        mov    eax, 10h
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, 102h
        mov    edx, dword ptr [ebp - 4]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3783:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax], ecx
        je     SHORT loc_4_3792
        mov    edi, 3e6h
loc_4_3792:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax]
        push   dword ptr [ebp + 18h]
        add    eax, 0ch
        push   eax
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_37B1
        mov    edi, 57h
loc_4_37B1:
        mov    eax, dword ptr [ebp + 20h]
        test   eax, eax
        je     SHORT loc_4_37D1
        push   0
        push   4
        push   eax
        push   dword ptr [ebp - 4]
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_37D1
        mov    edi, 57h
loc_4_37D1:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
loc_4_37F6:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_3805:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 8]
        call   _VWIN32_CopyMem
        add    esp, 10h
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret
loc_4_3835:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        mov    eax, dword ptr [ebp + 1ch]
        push   esi
        add    eax, 0ch
        push   edi
        mov    dword ptr [ebp - 14h], eax
        xor    edi, edi
        mov    dword ptr [ebp - 18h], edi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, edi
        jne    SHORT loc_4_3878
        mov    edi, 8
        jmp    NEAR PTR loc_4_3935
loc_4_3878:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 4]
        push   0
        push   ecx
        mov    dword ptr [edx + 8], eax
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        push   dword ptr [ebp + 18h]
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_38BC
        mov    edi, 57h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_38F0
loc_4_38BC:
        mov    eax, 10h
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, 102h
        mov    edx, dword ptr [ebp - 4]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_38E1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax], ecx
        je     SHORT loc_4_38F0
        mov    edi, 3e6h
loc_4_38F0:
        mov    eax, dword ptr [ebp + 20h]
        test   eax, eax
        je     SHORT loc_4_3910
        push   0
        push   4
        push   eax
        push   dword ptr [ebp - 4]
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_3910
        mov    edi, 57h
loc_4_3910:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
loc_4_3935:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_3944:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   20020000h
        mov    eax, dword ptr [ebp - 4]
        push   -1
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 8]
        call   sub_4_346E
        cmp    eax, -1
        je     SHORT loc_4_3981
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 8]
        add    eax, 0ch
        push   eax
        call   _VWIN32_CopyMem
        add    esp, 10h
        jmp    SHORT loc_4_398A
loc_4_3981:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
loc_4_398A:
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret
loc_4_3996:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 10h]
        push   dword ptr [eax + 0ch]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 8]
        call   sub_4_346E
        cmp    eax, -1
        jne    SHORT loc_4_39C3
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0fffffffeh
        jmp    SHORT loc_4_39C8
loc_4_39C3:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
loc_4_39C8:
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret
loc_4_39D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 10h]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 8]
        call   sub_4_0EEF
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h
sub_4_3A08:
        push   edi
        db  08Bh, 03Dh, 0E0h, 010h, 000h, 000h
loc_4_3A0F:
        mov    eax, dword ptr [edi + 34h]
        or     eax, eax
        je     SHORT loc_4_3A2B
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edi + 34h], ecx
        mov    edx, dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        db  0E8h, 0B3h, 0D0h, 0FBh, 0FFh
        pop    eax
        call   edx
        jmp    SHORT loc_4_3A0F
loc_4_3A2B:
        pop    edi
        ret
sub_4_3A2D:
        db  08Bh, 00Dh, 0E0h, 010h, 000h, 000h
        jecxz  loc_4_3A38
        inc    dword ptr [ecx + 30h]
loc_4_3A38:
        push   edx
        db  08Bh, 015h, 0ACh, 002h, 000h, 000h
        call   sub_4_2BAF
        pop    edx
        ret
sub_4_3A46:
        push   edx
        db  08Bh, 015h, 0ACh, 002h, 000h, 000h
        call   sub_4_2C1E
        pop    edx
        db  08Bh, 00Dh, 0E0h, 010h, 000h, 000h
        jecxz  loc_4_3A6B
        dec    dword ptr [ecx + 30h]
        jne    SHORT loc_4_3A6B
        cmp    dword ptr [ecx + 34h], 0
        je     SHORT loc_4_3A6B
        call   sub_4_3A08
loc_4_3A6B:
        ret
sub_4_3A6C:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        pop    ebx
        jne    SHORT loc_4_3A94
        db  0A1h, 0E0h, 010h, 000h, 000h
        cmp    dword ptr [eax + 30h], 0
        jne    SHORT loc_4_3A98
        call   sub_4_3A2D
        xor    eax, eax
        inc    eax
        ret
loc_4_3A94:
        xor    eax, eax
        stc
        ret
loc_4_3A98:
        push   edi
        push   edx
        push   ecx
        lea    edi, [eax + 34h]
        db  0E8h, 018h, 0D0h, 0FBh, 0FFh
        jae    SHORT loc_4_3AAA
        pop    ecx
        pop    edx
        pop    edi
        jmp    SHORT loc_4_3A94
loc_4_3AAA:
        pop    dword ptr [eax + 8]
        pop    dword ptr [eax + 4]
        mov    edx, dword ptr [edi]
loc_4_3AB2:
        or     edx, edx
        je     SHORT loc_4_3ABC
        lea    edi, [edx]
        mov    edx, dword ptr [edi]
        jmp    SHORT loc_4_3AB2
loc_4_3ABC:
        mov    dword ptr [eax], edx
        mov    dword ptr [edi], eax
        pop    edi
        mov    eax, 1
        stc
        ret
sub_4_3AC8:
        cmp    dword ptr [ebx], 3
        jne    SHORT loc_4_3AD5
        cmp    dword ptr [ebx + 18h], edi
        jne    SHORT loc_4_3AD5
        test   edi, edi
        ret
loc_4_3AD5:
        cmp    dword ptr [ebx + 10h], 0
        ret
sub_4_3ADA:
        cmp    dword ptr [ebx], 3
        je     SHORT loc_4_3AEE
        cmp    dword ptr [ebx], 2
        jne    SHORT loc_4_3AF1
        test   dword ptr [ebx + 18h], 1
        je     SHORT loc_4_3AF1
        ret
loc_4_3AEE:
        mov    dword ptr [ebx + 18h], edi
loc_4_3AF1:
        dec    dword ptr [ebx + 10h]
        ret
sub_4_3AF5:
        push   esi
        mov    esi, eax
        db  039h, 005h, 0B0h, 004h, 000h, 000h
        jae    SHORT loc_4_3B48
        xor    ecx, ecx
loc_4_3B02:
        add    ecx, 40h
        db  08Bh, 015h, 0B0h, 004h, 000h, 000h
        add    edx, ecx
        cmp    edx, eax
        jb     SHORT loc_4_3B02
        mov    eax, 10h
        mul    ecx
        push   ecx
        push   201h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    ecx
        or     eax, eax
        je     SHORT loc_4_3B6E
        db  08Bh, 015h, 0ACh, 004h, 000h, 000h
        db  001h, 00Dh, 0B0h, 004h, 000h, 000h
loc_4_3B39:
        mov    dword ptr [eax], edx
        mov    edx, eax
        add    eax, 10h
        loop   loc_4_3B39
        db  089h, 015h, 0ACh, 004h, 000h, 000h
loc_4_3B48:
        mov    ecx, esi
        db  0A1h, 0ACh, 004h, 000h, 000h
loc_4_3B4F:
        mov    edx, eax
        or     eax, eax
        je     SHORT loc_4_3B6E
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [eax]
        loop   loc_4_3B4F
        db  087h, 005h, 0ACh, 004h, 000h, 000h
        mov    dword ptr [edx], eax
        db  029h, 035h, 0B0h, 004h, 000h, 000h
loc_4_3B6E:
        pop    esi
        ret
sub_4_3B70:
        mov    edx, dword ptr [eax]
        db  087h, 015h, 0ACh, 004h, 000h, 000h
        mov    dword ptr [eax], edx
        db  001h, 00Dh, 0B0h, 004h, 000h, 000h
        ret
sub_4_3B81:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        mov    dword ptr [edi + 54h], 0
        mov    edi, dword ptr [edi + 5ch]
        mov    esi, eax
        mov    ebx, eax
        xor    ecx, ecx
loc_4_3B97:
        inc    ecx
        mov    eax, dword ptr [ebx + 0ch]
        add    eax, 0ch
loc_4_3B9E:
        mov    edx, dword ptr [eax]
        cmp    edx, ebx
        je     SHORT loc_4_3BAD
        or     edx, edx
        je     SHORT loc_4_3BAD
        lea    eax, [edx + 4]
        jmp    SHORT loc_4_3B9E
loc_4_3BAD:
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax], edx
        mov    edx, dword ptr [ebx + 0ch]
        cmp    dword ptr [edx], 3
        jne    SHORT loc_4_3BEF
        sub    eax, 0ch
        cmp    edx, eax
        jne    SHORT loc_4_3BEF
        cmp    dword ptr [edx + 18h], 0
        je     SHORT loc_4_3BEF
        push   edi
        push   ebx
        push   ecx
        db  0CDh, 020h, 06Ah, 001h, 001h, 000h
        mov    ecx, dword ptr [ebx + 4]
        jecxz  loc_4_3BEC
        mov    edi, dword ptr [edx + 18h]
        mov    ebx, dword ptr [ecx + 8]
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 5ch]
        mov    edi, dword ptr [edi + 5ch]
        db  0CDh, 020h, 063h, 001h, 001h, 000h
loc_4_3BEC:
        pop    ecx
        pop    ebx
        pop    edi
loc_4_3BEF:
        mov    ebx, dword ptr [ebx]
        cmp    esi, ebx
        jne    SHORT loc_4_3B97
        mov    eax, ebx
        call   sub_4_3B70
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  0E8h, 028h, 0FEh, 0FFh, 0FFh, 08Bh, 040h, 054h, 00Bh, 0C0h, 074h, 005h, 0E8h, 070h, 0FFh, 0FFh
        db  0FFh, 0E8h, 030h, 0FEh, 0FFh, 0FFh, 0C3h
sub_4_3C17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_4_3C1D:
        xor    eax, eax
        inc    eax
        call   sub_4_3AF5
        or     eax, eax
        jne    SHORT loc_4_3C3E
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        call   sub_4_3A2D
        jmp    SHORT loc_4_3C1D
loc_4_3C3E:
        mov    esi, eax
        db  08Bh, 01Dh, 0D8h, 010h, 000h, 000h
        mov    dword ptr [esi + 8], ebx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ch], edx
        mov    dword ptr [ebx + 54h], esi
        mov    dword ptr [ebx + 58h], 0
        lea    edi, [edx + 0ch]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
loc_4_3C63:
        jecxz  loc_4_3C78
        mov    ecx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 34h]
        cmp    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        jb     SHORT loc_4_3C78
        lea    edi, [ecx + 4]
        mov    ecx, dword ptr [edi]
        jmp    SHORT loc_4_3C63
loc_4_3C78:
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 4], ecx
        cmp    dword ptr [edx], 3
        jne    SHORT loc_4_3CAD
        lea    eax, [edx + 0ch]
        cmp    edi, eax
        jne    SHORT loc_4_3CAD
        jecxz  loc_4_3C99
        mov    edi, dword ptr [ecx + 8]
        mov    edi, dword ptr [edi + 5ch]
        mov    eax, edx
        db  0CDh, 020h, 06Ah, 001h, 001h, 000h
loc_4_3C99:
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 5ch]
        mov    edi, dword ptr [edx + 18h]
        mov    edi, dword ptr [edi + 5ch]
        db  0CDh, 020h, 063h, 001h, 001h, 000h
loc_4_3CAD:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  0E8h, 038h, 0D0h, 0FBh, 0FFh
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_3CC0:
        push   ebp
        mov    ebp, esp
        add    esp, -4
        push   ebx
        push   esi
        push   edi
loc_4_3CC9:
        mov    eax, dword ptr [ebp + 8]
        call   sub_4_3AF5
        or     eax, eax
        jne    SHORT loc_4_3CEA
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        call   sub_4_3A2D
        jmp    SHORT loc_4_3CC9
loc_4_3CEA:
        mov    esi, eax
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 01Dh, 0D8h, 010h, 000h, 000h
        mov    dword ptr [ebx + 54h], esi
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebx + 58h], eax
loc_4_3D02:
        mov    dword ptr [esi + 8], ebx
        mov    edx, dword ptr [ebp - 4]
        shl    edx, 2
        add    edx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx], 4
        jbe    SHORT loc_4_3D18
        mov    edx, dword ptr [edx + 0ch]
loc_4_3D18:
        mov    dword ptr [esi + 0ch], edx
        lea    edi, [edx + 0ch]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
loc_4_3D25:
        jecxz  loc_4_3D3A
        mov    ecx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 34h]
        cmp    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        jb     SHORT loc_4_3D3A
        lea    edi, [ecx + 4]
        mov    ecx, dword ptr [edi]
        jmp    SHORT loc_4_3D25
loc_4_3D3A:
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 4], ecx
        cmp    dword ptr [edx], 3
        jne    SHORT loc_4_3D75
        lea    eax, [edx + 0ch]
        cmp    edi, eax
        jne    SHORT loc_4_3D75
        cmp    dword ptr [edx + 18h], 0
        je     SHORT loc_4_3D75
        jecxz  loc_4_3D61
        mov    edi, dword ptr [ecx + 8]
        mov    edi, dword ptr [edi + 5ch]
        mov    eax, edx
        db  0CDh, 020h, 06Ah, 001h, 001h, 000h
loc_4_3D61:
        mov    eax, dword ptr [ebx + 34h]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 5ch]
        mov    edi, dword ptr [edx + 18h]
        mov    edi, dword ptr [edi + 5ch]
        db  0CDh, 020h, 063h, 001h, 001h, 000h
loc_4_3D75:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], eax
        db  08Bh, 01Dh, 0D8h, 010h, 000h, 000h
        ja     NEAR PTR loc_4_3D02
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        db  0E8h, 058h, 0CFh, 0FBh, 0FFh
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_3DA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_4_3DA6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0ch]
loc_4_3DAC:
        or     eax, eax
        je     SHORT loc_4_3E19
        mov    edi, dword ptr [eax + 8]
        call   sub_4_3AC8
        je     SHORT loc_4_3E19
        mov    ecx, dword ptr [edi + 60h]
        cmp    dword ptr [ecx + 20h], 0
        je     SHORT loc_4_3DDA
        call   sub_4_3B81
        push   40000001h
        push   dword ptr [edi + 5ch]
        db  0E8h, 061h, 0CAh, 0FBh, 0FFh
        add    esp, 8
        jmp    SHORT loc_4_3DA6
loc_4_3DDA:
        test   byte ptr [edi + 58h], 1
        jne    SHORT loc_4_3E1E
        xor    esi, esi
        dec    esi
        mov    ecx, dword ptr [edi + 54h]
loc_4_3DE6:
        inc    esi
        cmp    ecx, eax
        mov    ecx, dword ptr [ecx]
        jne    SHORT loc_4_3DE6
        call   sub_4_3ADA
        mov    ebx, dword ptr [eax + 4]
        call   sub_4_3B81
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_4_3E06
        add    esi, 80h
loc_4_3E06:
        push   esi
        push   dword ptr [edi + 5ch]
        db  0E8h, 027h, 0CAh, 0FBh, 0FFh
        add    esp, 8
        mov    eax, ebx
        mov    ebx, dword ptr [ebp + 8]
        jmp    SHORT loc_4_3DAC
loc_4_3E19:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_3E1E:
        mov    esi, dword ptr [eax]
loc_4_3E20:
        cmp    esi, eax
        je     SHORT loc_4_3E3B
        mov    ebx, dword ptr [esi + 0ch]
        call   sub_4_3AC8
        mov    esi, dword ptr [esi]
        jne    SHORT loc_4_3E20
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        jmp    NEAR PTR loc_4_3DAC
loc_4_3E3B:
        mov    esi, eax
loc_4_3E3D:
        mov    ebx, dword ptr [esi + 0ch]
        call   sub_4_3ADA
        mov    esi, dword ptr [esi]
        cmp    esi, eax
        jne    SHORT loc_4_3E3D
        mov    ebx, dword ptr [eax + 4]
        call   sub_4_3B81
        xor    esi, esi
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_4_3E06
        mov    esi, 80h
        jmp    SHORT loc_4_3E06
_VWIN32_SetWin32Event:
        mov    edx, eax
        cmp    dword ptr [edx], 2
        je     SHORT loc_4_3E6C
        xor    eax, eax
        stc
        ret
loc_4_3E6C:
        db  0B9h, 061h, 03Eh, 000h, 000h
        call   sub_4_3A6C
        jb     SHORT loc_4_3E92
        mov    dword ptr [edx + 10h], 1
        push   0
        push   edx
        call   sub_4_3DA0
        add    esp, 8
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
loc_4_3E92:
        ret
_VWIN32_PulseWin32Event:
        mov    edx, eax
        cmp    dword ptr [edx], 2
        je     SHORT loc_4_3E9E
        xor    eax, eax
        stc
        ret
loc_4_3E9E:
        db  0B9h, 093h, 03Eh, 000h, 000h
        call   sub_4_3A6C
        jb     SHORT loc_4_3ECB
        mov    dword ptr [edx + 10h], 1
        push   0
        push   edx
        call   sub_4_3DA0
        add    esp, 8
        mov    dword ptr [edx + 10h], 0
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
loc_4_3ECB:
        ret
_VWIN32_ResetWin32Event:
        mov    edx, eax
        cmp    dword ptr [edx], 2
        je     SHORT loc_4_3ED7
        xor    eax, eax
        stc
        ret
loc_4_3ED7:
        db  0B9h, 0CCh, 03Eh, 000h, 000h
        call   sub_4_3A6C
        jb     SHORT loc_4_3EF2
        mov    dword ptr [edx + 10h], 0
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
loc_4_3EF2:
        ret
_VWIN32_WaitMultipleObjects:
        push   esi
        push   edi
        push   ebx
loc_4_3EF6:
        mov    ebx, 0ffffffffh
        jecxz  loc_4_3F6F
        test   al, al
        je     SHORT loc_4_3F17
        db  08Bh, 01Dh, 0E0h, 010h, 000h, 000h
        cmp    dword ptr [ebx + 38h], 0
        je     SHORT loc_4_3F17
        mov    ebx, 0c0h
        jmp    NEAR PTR loc_4_3FE8
loc_4_3F17:
        db  08Bh, 03Dh, 0D8h, 010h, 000h, 000h
        push   eax
        mov    bh, ah
        call   sub_4_3A2D
        xor    eax, eax
        test   bh, bh
        jne    NEAR PTR loc_4_3FB6
loc_4_3F2F:
        mov    ebx, dword ptr [esi + eax*4]
        call   sub_4_3AC8
        jne    SHORT loc_4_3FAD
        inc    eax
        cmp    eax, ecx
        jb     SHORT loc_4_3F2F
        xor    ebx, ebx
loc_4_3F40:
        or     edx, edx
        mov    ebx, 102h
        je     NEAR PTR loc_4_3FE2
        mov    eax, dword ptr [esp]
        push   edx
        push   ecx
        movzx  eax, al
        push   eax
        push   ebx
        push   edx
        push   esi
        push   ecx
        call   sub_4_3CC0
        add    esp, 14h
        mov    ebx, eax
        pop    ecx
        pop    edx
        pop    eax
        cmp    ebx, 40000001h
        je     SHORT loc_4_3F71
loc_4_3F6F:
        jmp    SHORT loc_4_3FE8
loc_4_3F71:
        db  08Bh, 01Dh, 0E0h, 010h, 000h, 000h
        test   byte ptr [ebx + 2dh], 1
        jne    SHORT loc_4_3F91
        cmp    dword ptr [ebx + 20h], 0
        je     NEAR PTR loc_4_3EF6
        cmp    dword ptr [ebx + 28h], 0
        jne    NEAR PTR loc_4_3EF6
loc_4_3F91:
        pushad
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0E8h, 0DCh, 0C7h, 0FBh, 0FFh
        or     byte ptr [ebx + 2ch], 4
        push   eax
        db  0E8h, 0FEh, 0C6h, 0FBh, 0FFh
        popad
        jmp    NEAR PTR loc_4_3EF6
loc_4_3FAD:
        call   sub_4_3ADA
        mov    ebx, eax
        jmp    SHORT loc_4_3FE2
loc_4_3FB6:
        mov    ebx, dword ptr [esi + eax*4]
        call   sub_4_3AC8
        je     SHORT loc_4_3FD8
        inc    eax
        cmp    eax, ecx
        jb     SHORT loc_4_3FB6
        xor    eax, eax
loc_4_3FC7:
        mov    ebx, dword ptr [esi + eax*4]
        call   sub_4_3ADA
        inc    eax
        cmp    eax, ecx
        jb     SHORT loc_4_3FC7
        xor    ebx, ebx
        jmp    SHORT loc_4_3FE2
loc_4_3FD8:
        mov    ebx, 1
        jmp    NEAR PTR loc_4_3F40
loc_4_3FE2:
        pop    eax
        call   sub_4_3A46
loc_4_3FE8:
        mov    eax, ebx
        pop    ebx
        pop    edi
        pop    esi
        ret
_VWIN32_WaitSingleObject:
        push   edi
        push   esi
        push   ebx
loc_4_3FF1:
        test   eax, eax
        je     SHORT loc_4_4008
        db  08Bh, 035h, 0E0h, 010h, 000h, 000h
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_4_4008
        mov    esi, 0c0h
        jmp    SHORT loc_4_407A
loc_4_4008:
        db  08Bh, 03Dh, 0D8h, 010h, 000h, 000h
        mov    esi, eax
        call   sub_4_3A2D
        call   sub_4_3AC8
        jne    SHORT loc_4_406E
        or     edx, edx
        jne    SHORT loc_4_4027
        mov    esi, 102h
        jmp    SHORT loc_4_4075
loc_4_4027:
        push   edx
        push   esi
        push   edx
        push   ebx
        call   sub_4_3C17
        add    esp, 0ch
        pop    edx
        xchg   esi, eax
        cmp    esi, 40000001h
        jne    SHORT loc_4_407A
        db  08Bh, 035h, 0E0h, 010h, 000h, 000h
        test   byte ptr [esi + 2dh], 1
        jne    SHORT loc_4_4055
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_4_3FF1
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_4_3FF1
loc_4_4055:
        pushad
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0E8h, 018h, 0C7h, 0FBh, 0FFh
        or     byte ptr [esi + 2ch], 4
        push   eax
        db  0E8h, 03Ah, 0C6h, 0FBh, 0FFh
        popad
        jmp    SHORT loc_4_3FF1
loc_4_406E:
        call   sub_4_3ADA
        mov    esi, eax
loc_4_4075:
        call   sub_4_3A46
loc_4_407A:
        mov    eax, esi
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_4_4080:
        sub    esp, 18h
        mov    dword ptr [esp + 10h], 0c0000094h
        db  0C7h, 044h, 024h, 014h, 000h, 000h, 000h, 000h
        mov    dword ptr [esp + 8], 0ffffffffh
        mov    ecx, dword ptr [esp + 1ch]
        push   ebx
        push   esi
        mov    bl, 1
        push   edi
        push   ebp
        mov    edi, 4
        mov    ebp, dword ptr [ecx + 24h]
loc_4_40AD:
        lea    eax, [esp + 13h]
        push   0
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_4_4306
        movzx  eax, byte ptr [esp + 13h]
        inc    ebp
        cmp    eax, 2eh
        jg     SHORT loc_4_40DB
        je     SHORT loc_4_4127
        cmp    eax, 26h
        je     SHORT loc_4_4127
        jmp    SHORT loc_4_4116
loc_4_40DB:
        cmp    eax, 3eh
        jg     SHORT loc_4_40E9
        je     SHORT loc_4_4127
        cmp    eax, 36h
        je     SHORT loc_4_4127
        jmp    SHORT loc_4_4116
loc_4_40E9:
        cmp    eax, 64h
        jl     SHORT loc_4_4116
        cmp    eax, 65h
        jle    SHORT loc_4_4127
        cmp    eax, 66h
        je     SHORT loc_4_411A
        cmp    eax, 67h
        je     NEAR PTR loc_4_4306
        cmp    eax, 0f0h
        je     SHORT loc_4_4127
        cmp    eax, 0f2h
        jl     SHORT loc_4_4116
        cmp    eax, 0f3h
        jle    SHORT loc_4_4127
loc_4_4116:
        xor    bl, bl
        jmp    SHORT loc_4_4127
loc_4_411A:
        mov    dword ptr [esp + 18h], 0ffffh
        mov    edi, 2
loc_4_4127:
        test   bl, bl
        jne    SHORT loc_4_40AD
        cmp    byte ptr [esp + 13h], 0f7h
        je     SHORT loc_4_413D
        cmp    byte ptr [esp + 13h], 0f6h
        jne    NEAR PTR loc_4_4306
loc_4_413D:
        cmp    byte ptr [esp + 13h], 0f6h
        jne    SHORT loc_4_4151
        mov    dword ptr [esp + 18h], 0ffh
        mov    edi, 1
loc_4_4151:
        lea    eax, [esp + 13h]
        push   0
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_4_4306
        inc    ebp
        mov    al, byte ptr [esp + 13h]
        shr    al, 4
        mov    bl, byte ptr [esp + 13h]
        movzx  eax, al
        and    al, 0fch
        and    bl, 7
        cmp    edi, 1
        db  08Dh, 0B0h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_419C
        mov    al, byte ptr [esp + 13h]
        and    al, 0c0h
        cmp    al, 0c0h
        jne    SHORT loc_4_419C
        db  0C7h, 044h, 024h, 024h, 000h, 000h, 000h, 000h
loc_4_419C:
        mov    dword ptr [esp + 14h], 0
        cmp    byte ptr [esi], bl
        je     NEAR PTR loc_4_4233
        cmp    byte ptr [esi + 1], bl
        jne    SHORT loc_4_421D
        lea    eax, [esp + 13h]
        push   0
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_4_4306
        inc    ebp
        mov    al, byte ptr [esp + 13h]
        and    al, 38h
        shr    al, 3
        movzx  ecx, al
        mov    dword ptr [esp + 1ch], ecx
        cmp    ecx, 4
        je     SHORT loc_4_41FE
        db  00Fh, 0B6h, 091h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [edx + ecx]
        mov    cl, byte ptr [esp + 13h]
        shr    cl, 6
        mov    dword ptr [esp + 14h], eax
        shl    dword ptr [esp + 14h], cl
loc_4_41FE:
        mov    al, byte ptr [esp + 13h]
        mov    ecx, dword ptr [esp + 2ch]
        and    eax, 7
        db  00Fh, 0B6h, 090h, 000h, 000h, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [edx + ecx]
        add    dword ptr [esp + 14h], eax
        jmp    SHORT loc_4_4233
loc_4_421D:
        mov    eax, dword ptr [esp + 24h]
        movzx  ecx, bl
        movzx  edx, byte ptr [ecx + eax]
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [edx + ecx]
        mov    dword ptr [esp + 14h], eax
loc_4_4233:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, byte ptr [esi + 2]
        test   edx, eax
        je     SHORT loc_4_428E
        cmp    byte ptr [esi + 3], 4
        push   0
        jne    SHORT loc_4_4265
        lea    eax, [esp + 20h]
        push   4
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_4286
        jmp    NEAR PTR loc_4_4306
loc_4_4265:
        lea    eax, [esp + 17h]
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_4_4306
        movsx  eax, byte ptr [esp + 13h]
        mov    dword ptr [esp + 1ch], eax
loc_4_4286:
        mov    eax, dword ptr [esp + 1ch]
        add    dword ptr [esp + 14h], eax
loc_4_428E:
        cmp    byte ptr [esi + 3], 0
        je     SHORT loc_4_42F4
        cmp    edi, 1
        je     SHORT loc_4_42A5
        cmp    edi, 2
        je     SHORT loc_4_42C0
        cmp    edi, 4
        je     SHORT loc_4_42DB
        jmp    SHORT loc_4_42F4
loc_4_42A5:
        lea    eax, [esp + 14h]
        push   0
        push   1
        push   eax
        push   dword ptr [esp + 20h]
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_42F4
        jmp    SHORT loc_4_4306
loc_4_42C0:
        lea    eax, [esp + 14h]
        push   0
        push   2
        push   eax
        push   dword ptr [esp + 20h]
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_42F4
        jmp    SHORT loc_4_4306
loc_4_42DB:
        lea    eax, [esp + 14h]
        push   0
        push   4
        push   eax
        push   dword ptr [esp + 20h]
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_4_4306
loc_4_42F4:
        mov    eax, dword ptr [esp + 18h]
        test   dword ptr [esp + 14h], eax
        je     SHORT loc_4_4306
        mov    dword ptr [esp + 20h], 0c0000095h
loc_4_4306:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_4_4312:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 28h]
        ret    4
sub_4_431D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [eax]
        push   edi
        mov    ecx, dword ptr [ebx + 5ch]
        mov    dword ptr [eax + 14h], 0
        mov    edi, dword ptr [ecx + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_433B:
        xor    esi, esi
        mov    eax, dword ptr [ebp + 8]
        xchg   dword ptr [eax + 10h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 60h]
        or     byte ptr [eax + 2dh], 8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ecx]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_4370
loc_4_435D:
        push   ebx
        push   dword ptr [esi + 4]
        call   sub_4_449B
        add    esp, 8
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_435D
loc_4_4370:
        mov    eax, dword ptr [ebx + 60h]
        xor    ecx, ecx
        mov    word ptr [ebx + 2eh], 0ffffh
        mov    dword ptr [eax + 30h], ecx
        mov    dword ptr [eax + 34h], ecx
        mov    edx, dword ptr [ebx + 40h]
        mov    dword ptr [eax + 1ch], ecx
        and    byte ptr [eax + 2ch], 0fch
        mov    dword ptr [ebx + 10h], 0ffffffffh
        test   byte ptr [edx + 20h], 8
        mov    dword ptr [eax + 28h], ecx
        je     SHORT loc_4_43A1
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_43A1:
        mov    eax, dword ptr [ebx + 40h]
        mov    esi, dword ptr [eax + 20h]
        test   esi, 80h
        je     SHORT loc_4_43BA
        call   sub_4_18BF
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 14h], eax
loc_4_43BA:
        test   esi, 8
        je     NEAR PTR loc_4_4447
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_4_45A9
        inc    word ptr [ebx + 2eh]
        db  0A1h, 000h, 000h, 000h, 000h
        movzx  ecx, ax
        shr    eax, 10h
        mov    dword ptr [edi + 24h], ecx
        mov    word ptr [edi + 28h], ax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        movzx  edx, word ptr [ecx + 8]
        mov    ax, word ptr [ecx + 0ah]
        mov    dword ptr [edi + 30h], edx
        db  066h, 08Bh, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 4], ebx
        mov    word ptr [edi + 34h], ax
        xor    eax, eax
        mov    word ptr [edi + 3ch], ax
        sub    edx, 16ch
        mov    word ptr [edi + 38h], ax
        mov    dword ptr [edi + 30h], edx
        mov    word ptr [edi + 40h], si
        mov    eax, dword ptr [ebx + 40h]
        mov    esi, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi + 8], edx
        mov    dword ptr [ebx + 18h], esi
        mov    dword ptr [ebx + 14h], eax
        db  066h, 08Bh, 00Dh, 002h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        jmp    SHORT loc_4_447A
loc_4_4447:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [edi + 28h], cx
        mov    edx, dword ptr [ebx + 68h]
        mov    cx, word ptr [ebx + 4ch]
        mov    dword ptr [edi + 24h], eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 30h], edx
        mov    word ptr [edi + 34h], ax
        mov    word ptr [edi + 3ch], ax
        mov    word ptr [edi + 38h], ax
        mov    word ptr [edi + 40h], cx
loc_4_447A:
        mov    word ptr [edi + 44h], 0
        mov    eax, dword ptr [edi + 2ch]
        and    eax, 0fffff22ah
        or     ah, 2
        mov    dword ptr [edi + 2ch], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edi + 10h], ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_4_449B:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        push   35h
        push   eax
        call   sub_4_0EF5
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_4_44D9
        mov    esi, dword ptr [eax + 4]
        test   esi, esi
        lea    edi, [esi - 1]
        jle    SHORT loc_4_44D9
loc_4_44C7:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_4_2C24
        mov    eax, edi
        dec    edi
        test   eax, eax
        jg     SHORT loc_4_44C7
loc_4_44D9:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_4_0EFB
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_44ED:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        test   esi, esi
        je     SHORT loc_4_4511
        mov    eax, esi
        dec    esi
        test   eax, eax
        je     SHORT loc_4_4511
loc_4_44FD:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_4_2BB5
        mov    eax, esi
        dec    esi
        test   eax, eax
        jne    SHORT loc_4_44FD
loc_4_4511:
        pop    esi
        ret
sub_4_4513:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        push   esi
        push   35h
        push   eax
        call   sub_4_0EF5
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_4_4538
        sub    esi, dword ptr [eax + 4]
loc_4_4538:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_4_0EFB
        add    esp, 4
        test   esi, esi
        je     SHORT loc_4_457E
        mov    eax, esi
        jle    SHORT loc_4_4567
        dec    esi
        test   eax, eax
        jle    SHORT loc_4_457E
loc_4_4553:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_4_2BB5
        mov    eax, esi
        dec    esi
        test   eax, eax
        jg     SHORT loc_4_4553
        jmp    SHORT loc_4_457E
loc_4_4567:
        inc    esi
        test   eax, eax
        jge    SHORT loc_4_457E
loc_4_456C:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_4_2C24
        mov    eax, esi
        inc    esi
        test   eax, eax
        jl     SHORT loc_4_456C
loc_4_457E:
        pop    esi
        pop    ebp
        ret
sub_4_4581:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    ecx, dword ptr [eax + 50h]
        mov    esi, dword ptr [ecx]
        test   esi, esi
        je     SHORT loc_4_45A5
loc_4_458F:
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 5ch]
        test   eax, eax
        je     SHORT loc_4_459F
        push   eax
        call   sub_4_1C1B
loc_4_459F:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_4_458F
loc_4_45A5:
        pop    esi
        ret    4
sub_4_45A9:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 60h]
        inc    dword ptr [ecx + 28h]
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        call   sub_4_2BB5
        ret    8
sub_4_45C3:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        call   sub_4_2C24
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 60h]
        dec    dword ptr [ecx + 28h]
        ret    8
loc_4_45DD:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_4_0F01
        mov    ecx, dword ptr [esp + 0ch]
        add    esp, 8
        mov    dword ptr [ecx + 14h], eax
        ret    10h
sub_4_45F7:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        cmp    word ptr [edx + 28h], ax
        jne    SHORT loc_4_4632
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    word ptr [edx + 34h], ax
        jne    SHORT loc_4_4632
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_4_4632
        push   dword ptr [esp + 0ch]
        push   edx
        push   dword ptr [esp + 0ch]
        call   ecx
        add    esp, 0ch
        mov    eax, 1
        jmp    SHORT loc_4_4634
loc_4_4632:
        xor    eax, eax
loc_4_4634:
        ret    0ch
loc_4_4637:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 64h]
        shr    ebx, 0ch
        add    eax, 10000h
        shr    eax, 0ch
        cmp    eax, ebx
        jae    NEAR PTR loc_4_46EF
        push   20000000h
        lea    eax, [ebx - 3]
        push   3
        push   eax
        call   sub_4_0ECB
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_4696
        lea    edi, [ebx - 1]
        push   20000000h
        push   0
        push   1
        push   1
        push   edi
        call   sub_4_0EC5
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_4_4696
        shl    edi, 0ch
        mov    dword ptr [esi + 18h], edi
        jmp    SHORT loc_4_46EB
loc_4_4696:
        xor    edi, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    edi, dword ptr [eax + 10h]
        movzx  ecx, word ptr [edi + 34h]
        push   ecx
        push   dword ptr [eax + 14h]
        call   sub_4_0EDD
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_46EF
        test   eax, eax
        jne    SHORT loc_4_46C7
        mov    eax, dword ptr [edi + 30h]
        jmp    SHORT loc_4_46CD
loc_4_46C7:
        movzx  ecx, word ptr [edi + 30h]
        add    eax, ecx
loc_4_46CD:
        shr    eax, 0ch
        cmp    eax, ebx
        jbe    SHORT loc_4_46EF
        push   20000000h
        push   0
        push   1
        push   ebx
        call   sub_4_0ED7
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_4_46EF
loc_4_46EB:
        and    byte ptr [esi + 44h], 7fh
loc_4_46EF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_46F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp - 0ch], edi
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 2ch], 2ah
        jne    NEAR PTR loc_4_4910
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 10h], 40h
        jne    NEAR PTR loc_4_4910
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    edi, eax
        mov    dword ptr [ebp - 8], eax
        shr    edi, 0ch
        mov    eax, dword ptr [ebx + 64h]
        add    eax, 10000h
        mov    ecx, dword ptr [ebx + 14h]
        shr    eax, 0ch
        shr    ecx, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 18h]
        shr    eax, 0ch
        test   byte ptr [ebx + 44h], 8
        je     NEAR PTR loc_4_4903
        sub    eax, 8
        cmp    eax, edi
        ja     NEAR PTR loc_4_4903
        cmp    ecx, edi
        jbe    NEAR PTR loc_4_4903
        cmp    dword ptr [ebp - 4], edi
        ja     NEAR PTR loc_4_486E
        mov    eax, dword ptr [esi + 8]
        and    al, 5
        cmp    al, 1
        jne    NEAR PTR loc_4_486E
        push   20000000h
        lea    eax, [edi - 3]
        push   3
        push   eax
        call   sub_4_0ECB
        add    esp, 0ch
        push   20060000h
        push   0
        push   1
        push   edi
        call   sub_4_0ED7
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_4_47C0
        push   esi
        push   dword ptr [ebp + 8]
        push   10eh
        call   sub_4_45F7
        jmp    NEAR PTR loc_4_4912
loc_4_47C0:
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_4_480E
        push   20000000h
        lea    eax, [edi - 1]
        push   0
        push   1
        push   1
        push   eax
        call   sub_4_0EC5
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_4_480E
        lea    eax, [edi - 1]
        shl    eax, 0ch
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, edi
        cmp    eax, -1
        jne    SHORT loc_4_480A
        push   20060000h
        lea    eax, [edi - 1]
        push   0
        push   1
        push   eax
        call   sub_4_0ED7
        add    esp, 10h
        jmp    SHORT loc_4_480E
loc_4_480A:
        xor    eax, eax
        jmp    SHORT loc_4_4813
loc_4_480E:
        mov    eax, 1
loc_4_4813:
        test   eax, eax
        je     SHORT loc_4_4864
        db  0A1h, 008h, 000h, 000h, 000h
        cmp    dword ptr [eax + 28h], 0
        jbe    SHORT loc_4_4851
        test   byte ptr [ebx + 44h], 80h
        jne    SHORT loc_4_4851
        mov    eax, dword ptr [ebx + 44h]
        push   0
        or     al, 80h
        mov    dword ptr [ebx + 44h], eax
        push   dword ptr [ebp - 0ch]
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4840:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_4_4851
        mov    eax, 1
        jmp    NEAR PTR loc_4_4912
loc_4_4851:
        push   esi
        push   dword ptr [ebp + 8]
        push   10eh
        call   sub_4_45F7
        jmp    NEAR PTR loc_4_4912
loc_4_4864:
        mov    eax, 1
        jmp    NEAR PTR loc_4_4912
loc_4_486E:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 8
        cmp    eax, edi
        ja     NEAR PTR loc_4_4903
        push   0
        mov    eax, dword ptr [ebp + 8]
        movzx  ecx, word ptr [eax + 34h]
        push   ecx
        mov    edx, dword ptr [ebp - 0ch]
        push   dword ptr [edx + 14h]
        call   sub_4_0EDD
        add    esp, 0ch
        cmp    eax, -1
        jne    SHORT loc_4_48A6
        push   esi
        push   dword ptr [ebp + 8]
        push   0eh
        call   sub_4_45F7
        jmp    SHORT loc_4_4912
loc_4_48A6:
        test   eax, eax
        je     SHORT loc_4_48BB
        mov    ecx, eax
        xor    ecx, dword ptr [ebp - 8]
        test   ecx, 0fffff000h
        je     SHORT loc_4_48C8
        test   eax, eax
        jne    SHORT loc_4_4903
loc_4_48BB:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0fffh
        cmp    eax, 0ah
        jne    SHORT loc_4_4903
loc_4_48C8:
        push   1
        db  0A1h, 010h, 000h, 000h, 000h
        push   edi
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_0ED1
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_48F3
        push   esi
        push   dword ptr [ebp + 8]
        push   10eh
        call   sub_4_45F7
        jmp    SHORT loc_4_4912
loc_4_48F3:
        shl    edi, 0ch
        mov    eax, 1
        mov    word ptr [edi + 0ah], 10h
        jmp    SHORT loc_4_4912
loc_4_4903:
        push   esi
        push   dword ptr [ebp + 8]
        push   0eh
        call   sub_4_45F7
        jmp    SHORT loc_4_4912
loc_4_4910:
        xor    eax, eax
loc_4_4912:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
_VWIN32_Emulate_Npx:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [esi + 34h]
        push   eax
        push   dword ptr [ecx + 14h]
        call   sub_4_0EDD
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_49A8
        lea    ebx, [esi + 30h]
        test   eax, eax
        jne    SHORT loc_4_494C
        mov    ecx, dword ptr [ebx]
        jmp    SHORT loc_4_4951
loc_4_494C:
        movzx  ecx, word ptr [ebx]
        add    ecx, eax
loc_4_4951:
        mov    edi, dword ptr [esi + 2ch]
        mov    edx, dword ptr [esi + 24h]
        push   0
        sub    ecx, 0ch
        push   0ch
        mov    dword ptr [ebp - 4], edi
        push   ecx
        mov    dword ptr [ebp - 0ch], edx
        movzx  eax, word ptr [esi + 28h]
        lea    edx, [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        push   edx
        call   _VWIN32_CopyMem
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_4989
        push   7
        push   esi
        call   _VWIN32_FaultPopup
        add    esp, 8
        jmp    SHORT loc_4_49A8
loc_4_4989:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    edi, 0fffffeffh
        mov    word ptr [esi + 28h], ax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    dword ptr [ebx], 0ch
        mov    dword ptr [esi + 2ch], edi
        mov    dword ptr [esi + 24h], ecx
loc_4_49A8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end
