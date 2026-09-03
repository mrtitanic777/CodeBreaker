; vcomm_pcode_06.asm -- VCOMM object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_0000:
        mov    eax, 100h
        ret
sub_6_0006:
        push   edi
        push   dword ptr [esp + 8]
        db  0CDh, 020h, 004h, 000h, 02Bh, 000h
        add    esp, 8
        cmp    eax, -0ah
        jae    SHORT loc_6_003D
        push   eax
        push   0
        push   0
        push   10h
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        pop    eax
        push   eax
        push   0
        push   0
        push   14h
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        pop    eax
loc_6_003D:
        ret
sub_6_003E:
        push   dword ptr [esp + 0ch]
        push   0
        push   dword ptr [esp + 10h]
        push   0
        push   dword ptr [esp + 14h]
        db  0CDh, 020h, 007h, 000h, 02Bh, 000h
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_6_006E
        mov    ecx, dword ptr [esp + 4]
        and    byte ptr [ecx + 39h], 0f7h
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_6_006E
        or     byte ptr [ecx + 39h], 8
loc_6_006E:
        ret
sub_6_006F:
        db  068h, 0B0h, 000h, 000h, 000h
        db  068h, 0B0h, 000h, 000h, 000h
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        ret
sub_6_008B:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [edx], eax
        mov    eax, esp
        ret
sub_6_009B:
        db  0CDh, 020h, 00Dh, 080h, 02Bh, 000h
VCOMM_Map_Ring0DCB_To_Win32:
        mov    dword ptr [edx], 1ch
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        xor    ebx, ebx
        movzx  ebx, cx
        shr    bl, 2
        shl    bl, 1
        and    bl, 0eh
        test   cl, 1
        je     SHORT loc_6_00C6
        or     bl, 1
loc_6_00C6:
        test   ecx, 10000h
        je     SHORT loc_6_00D1
        or     bl, 40h
loc_6_00D1:
        test   ecx, 20000h
        je     SHORT loc_6_00DC
        or     bl, 80h
loc_6_00DC:
        and    bh, 0fh
        test   ecx, 80000h
        je     SHORT loc_6_00EA
        or     bh, 40h
loc_6_00EA:
        test   cl, 80h
        jne    SHORT loc_6_00FC
        test   ch, 20h
        je     SHORT loc_6_00F9
        or     bl, 20h
        jmp    SHORT loc_6_00FC
loc_6_00F9:
        or     bl, 10h
loc_6_00FC:
        test   cl, 2
        jne    SHORT loc_6_0119
        test   ecx, 100000h
        je     SHORT loc_6_010E
        or     bh, 10h
        jmp    SHORT loc_6_0119
loc_6_010E:
        or     bh, 20h
        test   ch, 40h
        jne    SHORT loc_6_0119
        or     bh, 10h
loc_6_0119:
        mov    ecx, ebx
        pop    ebx
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [eax + 0ch]
        mov    word ptr [edx + 0eh], cx
        mov    ecx, dword ptr [eax + 10h]
        mov    word ptr [edx + 10h], cx
        mov    cl, byte ptr [eax + 16h]
        mov    byte ptr [edx + 12h], cl
        mov    cl, byte ptr [eax + 17h]
        mov    byte ptr [edx + 13h], cl
        mov    cl, byte ptr [eax + 18h]
        mov    byte ptr [edx + 14h], cl
        mov    cl, byte ptr [eax + 19h]
        mov    byte ptr [edx + 15h], cl
        mov    cl, byte ptr [eax + 1ah]
        mov    byte ptr [edx + 16h], cl
        mov    cl, byte ptr [eax + 1bh]
        mov    byte ptr [edx + 17h], cl
        mov    cl, byte ptr [eax + 1ch]
        mov    byte ptr [edx + 18h], cl
        mov    cl, byte ptr [eax + 1dh]
        mov    byte ptr [edx + 19h], cl
        mov    cx, word ptr [eax + 14h]
        mov    word ptr [edx + 0ch], cx
        ret
sub_6_0166:
        enter  30h, 0
        lea    eax, [ebp - 30h]
        push   eax
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 006h, 000h, 02Bh, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_6_018D
        lea    eax, [ebp - 30h]
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 020h, 000h, 02Bh, 000h
        xor    eax, eax
        inc    eax
loc_6_018D:
        leave
        ret
sub_6_018F:
        push   0
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        db  0CDh, 020h, 01Ah, 000h, 02Bh, 000h
        add    esp, 0ch
        ret
sub_6_01A3:
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        test   cl, 3
        je     SHORT loc_6_01E2
        test   cl, 2
        je     SHORT loc_6_01C5
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 80h]
        or     esi, esi
        je     SHORT loc_6_01C5
        db  0E8h, 00Fh, 008h, 0FAh, 0FFh
loc_6_01C5:
        test   dword ptr [esp + 0ch], 1
        je     SHORT loc_6_01E2
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 84h]
        or     esi, esi
        je     SHORT loc_6_01E2
        db  0E8h, 0F2h, 007h, 0FAh, 0FFh
loc_6_01E2:
        test   dword ptr [esp + 0ch], 4
        je     SHORT loc_6_01FB
        push   0
        push   dword ptr [esp + 0ch]
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
loc_6_01FB:
        test   dword ptr [esp + 0ch], 8
        je     SHORT loc_6_0214
        push   1
        push   dword ptr [esp + 0ch]
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
loc_6_0214:
        pop    esi
        ret
sub_6_0216:
        mov    eax, dword ptr [esp + 4]
        lea    eax, [eax + 30h]
        push   eax
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        db  0CDh, 020h, 010h, 000h, 02Bh, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_0247
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [eax + 10h], edx
        push   eax
        db  0E8h, 0B7h, 0FFh, 0F9h, 0FFh
        pop    ecx
        or     al, 1
loc_6_0247:
        ret
VCOMM_Map_Win32DCB_To_Ring0:
        mov    dword ptr [edx], 30h
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        xor    ebx, ebx
        mov    bl, cl
        and    bl, 0eh
        shl    bl, 1
        or     bl, 1
        mov    bh, ch
        and    bh, 0fh
        test   cl, 40h
        je     SHORT loc_6_0274
        or     ebx, 10000h
loc_6_0274:
        test   cl, 80h
        je     SHORT loc_6_027F
        or     ebx, 20000h
loc_6_027F:
        test   ch, 40h
        je     SHORT loc_6_028A
        or     ebx, 80000h
loc_6_028A:
        push   ecx
        and    ecx, 30h
        shr    cl, 4
        db  08Bh, 00Ch, 08Dh, 0A4h, 000h, 000h, 000h
        or     ebx, ecx
        pop    ecx
        and    ecx, 3000h
        shr    ecx, 0ch
        db  08Bh, 00Ch, 08Dh, 094h, 000h, 000h, 000h
        or     ebx, ecx
        mov    ecx, ebx
        pop    ebx
        mov    dword ptr [edx + 8], ecx
        movzx  ecx, word ptr [eax + 0eh]
        mov    dword ptr [edx + 0ch], ecx
        movzx  ecx, word ptr [eax + 10h]
        mov    dword ptr [edx + 10h], ecx
        mov    cl, byte ptr [eax + 12h]
        mov    byte ptr [edx + 16h], cl
        mov    cl, byte ptr [eax + 13h]
        mov    byte ptr [edx + 17h], cl
        mov    cl, byte ptr [eax + 14h]
        mov    byte ptr [edx + 18h], cl
        mov    cl, byte ptr [eax + 15h]
        mov    byte ptr [edx + 19h], cl
        mov    cl, byte ptr [eax + 16h]
        mov    byte ptr [edx + 1ah], cl
        mov    cl, byte ptr [eax + 17h]
        mov    byte ptr [edx + 1bh], cl
        mov    cl, byte ptr [eax + 18h]
        mov    byte ptr [edx + 1ch], cl
        mov    cl, byte ptr [eax + 19h]
        mov    byte ptr [edx + 1dh], cl
        mov    cx, word ptr [eax + 0ch]
        mov    word ptr [edx + 14h], cx
        ret
sub_6_02FA:
        enter  30h, 0
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 30h]
        db  0CDh, 020h, 01Fh, 000h, 02Bh, 000h
        push   0fffh
        push   edx
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 005h, 000h, 02Bh, 000h
        add    esp, 0ch
        leave
        ret
sub_6_031E:
        push   1
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        db  0CDh, 020h, 01Ah, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    eax, 1
        ret
sub_6_0337:
        db  0CDh, 020h, 008h, 080h, 02Bh, 000h
sub_6_033D:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    eax, dword ptr [edi + 10h]
        and    eax, dword ptr [edi + 30h]
        je     SHORT loc_6_035D
        push   dword ptr [esp + 0ch]
        push   dword ptr [edi + 10h]
        push   edi
        db  0CDh, 020h, 011h, 000h, 02Bh, 000h
        add    esp, 0ch
        jmp    SHORT loc_6_0391
loc_6_035D:
        or     byte ptr [edi + 39h], 1
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [edi + 28h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [edi + 20h], ecx
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [edi + 24h], ecx
        mov    ecx, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ecx + 2ch], edi
        push   ecx
        db  068h, 0FBh, 001h, 000h, 000h
        push   ecx
        db  0CDh, 020h, 014h, 000h, 02Bh, 000h
        add    esp, 0ch
loc_6_0391:
        pop    edi
        ret
sub_6_0393:
        db  0CDh, 020h, 00Ch, 080h, 02Bh, 000h
sub_6_0399:
        enter  34h, 0
        lea    edx, [ebp - 34h]
        db  0B8h, 0E4h, 001h, 000h, 000h
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 4], eax
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 24h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edx + 10h], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [edx + 2ch], eax
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [edx + 28h], edi
        pop    edi
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 39h], 8
        jne    SHORT loc_6_03E6
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_6_03FB
loc_6_03E6:
        lea    eax, [ebp - 34h]
        push   dword ptr [ebp + 14h]
        push   eax
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 01Bh, 000h, 02Bh, 000h
        add    esp, 0ch
        leave
        ret
sub_6_03FB:
        bts    dword ptr [edx + 24h], 0
        jb     SHORT loc_6_0432
        push   edx
        push   eax
        mov    dword ptr [edx + 4], ecx
        add    ecx, eax
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   40000000h
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        pop    ecx
        pop    edx
        je     SHORT loc_6_0432
        and    ecx, 0fffh
        add    eax, ecx
        mov    dword ptr [edx], eax
loc_6_0432:
        ret
sub_6_0433:
        enter  34h, 0
        lea    edx, [ebp - 34h]
        db  0B8h, 0E4h, 001h, 000h, 000h
        mov    dword ptr [edx + 8], eax
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 24h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edx + 10h], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [edx + 2ch], eax
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [edx + 28h], edi
        pop    edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 4], eax
        push   dword ptr [ebp + 14h]
        push   edx
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 01Ch, 000h, 02Bh, 000h
        add    esp, 0ch
        leave
        ret
sub_6_047E:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
sub_6_0484:
        push   0
        push   0
        push   6
        push   dword ptr [esp + 10h]
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        db  0CDh, 020h, 009h, 080h, 02Bh, 000h
        ret
sub_6_049E:
        db  0CDh, 020h, 015h, 080h, 02Bh, 000h
sub_6_04A4:
        ret
sub_6_04A5:
        push   0
        push   dword ptr [esp + 8]
        db  0E8h, 0E2h, 00Dh, 0FAh, 0FFh
        pop    ecx
        pop    ecx
        ret
sub_6_04B3:
        mov    cl, 13h
        db  0E9h, 036h, 0FCh, 0F9h, 0FFh
        db  0C3h
sub_6_04BB:
        mov    cl, 14h
        db  0E9h, 02Eh, 0FCh, 0F9h, 0FFh
        db  0C3h
sub_6_04C3:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        xor    edi, edi
        mov    dword ptr [esi], edi
        mov    cl, 15h
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0E8h, 014h, 0FCh, 0F9h, 0FFh
        pop    ecx
        pop    ecx
        cmp    dword ptr [esi], edi
        jne    SHORT loc_6_04F4
        mov    edi, dword ptr [esp + 0ch]
        btr    word ptr [edi + 39h], 1
        jae    SHORT loc_6_04F4
        mov    dword ptr [esi], 461h
loc_6_04F4:
        pop    edi
        pop    esi
        ret
sub_6_04F7:
        push   esi
        push   edi
        xor    eax, eax
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [ecx + 64h], 534d5446h
        jne    SHORT loc_6_052F
        mov    esi, dword ptr [ecx + 84h]
        or     esi, esi
        je     SHORT loc_6_0530
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_6_0530
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ecx + 90h], edi
        or     byte ptr [ecx + 39h], 4
        xor    eax, eax
        inc    eax
        jmp    SHORT loc_6_0530
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
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_6_05A9
        mov    esi, eax
        mov    ecx, dword ptr [esp + 24h]
        xor    eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esi], eax
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 8], ecx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [esi + 0ch], edi
        mov    cl, 16h
        push   esi
        db  068h, 0ACh, 005h, 000h, 000h
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        db  0E8h, 061h, 0FBh, 0F9h, 0FFh
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_6_059B
        cmp    eax, -1
        jne    SHORT loc_6_05A9
loc_6_059B:
        push   eax
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    eax
loc_6_05A9:
        pop    edi
        pop    esi
        ret
loc_6_05AC:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    edi, dword ptr [edx + 0ch]
        db  0BEh, 0CCh, 005h, 000h, 000h
        db  0CDh, 020h, 0FBh, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 8]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        pop    edi
        pop    esi
        ret
loc_6_05CC:
        mov    eax, dword ptr [edx + 10h]
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        ret
        db  000h, 000h
_PTEXT ends
        end
