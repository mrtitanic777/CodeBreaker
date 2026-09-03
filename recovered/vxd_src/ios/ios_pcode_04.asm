; ios_pcode_04.asm -- IOS object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
IOS_BD_Register_Device:
        pushad
        db  089h, 03Dh, 094h, 01Fh, 000h, 000h
        lea    edi, [edi + 34h]
        push   0ch
        pop    ecx
        xor    eax, eax
        cld
        rep stosd
        db  08Bh, 03Dh, 094h, 01Fh, 000h, 000h
        db  0C7h, 005h, 0FCh, 038h, 000h, 000h, 045h, 001h, 000h, 000h
        db  0C7h, 005h, 000h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0F0h, 038h, 000h, 000h
        call   IOS_Register
        pop    eax
        db  066h, 083h, 03Dh, 01Ch, 039h, 000h, 000h, 001h
        je     SHORT loc_4_0077
        jmp    SHORT loc_4_00A6
loc_4_0077:
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_4_0084
        db  0E8h, 094h, 000h, 0FCh, 0FFh
        jb     SHORT loc_4_00A6
loc_4_0084:
        test   dword ptr [edi + 8], 1
        je     SHORT loc_4_0099
        db  0E8h, 030h, 005h, 0FCh, 0FFh
        push   edi
        db  0E8h, 021h, 034h, 0FCh, 0FFh
        pop    edi
loc_4_0099:
        clc
loc_4_009A:
        db  0C7h, 005h, 094h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h
        popad
        ret
loc_4_00A6:
        stc
        jmp    SHORT loc_4_009A
IOS_Get_Device_List:
        db  08Bh, 03Dh, 003h, 022h, 000h, 000h
        ret
IOS_Find_Int13_Drive:
        db  08Dh, 03Dh, 003h, 022h, 000h, 000h
loc_4_00B6:
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_4_00C2
        cmp    byte ptr [edi + 7], al
        jne    SHORT loc_4_00B6
        ret
loc_4_00C2:
        stc
        ret
sub_4_00C4:
        db  07Ch, 021h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        db  000h, 061h, 000h
        add    byte ptr [eax], al
IOS_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 4
        ja     SHORT loc_4_00F7
        db  0FFh, 014h, 085h, 0C4h, 000h, 000h, 000h
        jb     SHORT loc_4_00F7
        mov    word ptr [ebp + 1ch], 0
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_4_00F7:
        mov    word ptr [ebp + 1ch], 0ffffh
        or     byte ptr [ebp + 2ch], 1
        ret
        db  000h, 000h
IOS_Register:
        cld
        pushad
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 24h]
        lea    esi, [ebx + 14h]
        call   sub_4_2198
        jb     NEAR PTR loc_4_0260
loc_4_0119:
        mov    edi, dword ptr [ebx + 10h]
        db  0C7h, 047h, 010h, 0E8h, 014h, 000h, 000h
        db  0C7h, 047h, 014h, 0DAh, 014h, 000h, 000h
        db  0C7h, 007h, 010h, 013h, 000h, 000h
        db  0C7h, 047h, 004h, 05Fh, 002h, 000h, 000h
        db  0C7h, 047h, 008h, 098h, 014h, 000h, 000h
        db  0C7h, 047h, 00Ch, 0A8h, 014h, 000h, 000h
        db  0C7h, 047h, 020h, 089h, 019h, 000h, 000h
        db  0C7h, 047h, 024h, 0D0h, 018h, 000h, 000h
        mov    byte ptr [edi + 32h], 0
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [edi + 1ch], eax
        push   0
        push   58h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_0A3F
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [eax + 10h], 0
        db  08Ah, 01Dh, 02Dh, 003h, 000h, 000h
        db  000h, 01Dh, 02Ch, 003h, 000h, 000h
        db  0C6h, 005h, 02Dh, 003h, 000h, 000h, 000h
        mov    dword ptr [edi + 18h], eax
        push   eax
        mov    edi, eax
        mov    ebx, dword ptr [ebp + 24h]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 8], eax
        mov    word ptr [edi + 6], 0
        mov    byte ptr [edi + 4fh], 0
        mov    byte ptr [edi + 14h], 0
        mov    eax, edi
        db  08Bh, 035h, 06Ch, 003h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_01D2
        mov    dword ptr [esi + 18h], edi
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edi + 45h], ecx
        add    edi, 14h
        lea    esi, [esi + 0bh]
        call   sub_4_21E3
loc_4_01D2:
        lea    edi, [eax + 38h]
        lea    esi, [ebx + 25h]
        mov    ecx, 4
        rep movsb
        lea    esi, [ebx + 29h]
        mov    ecx, 2
        rep movsb
        lea    esi, [ebx + 2bh]
        movsb
        lea    esi, [ebx + 2eh]
        mov    ecx, 4
        rep movsb
        pop    ebx
        db  089h, 01Dh, 045h, 001h, 000h, 000h
        mov    edi, ebx
        mov    ebx, dword ptr [ebp + 24h]
        mov    word ptr [ebx + 2ch], 1
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        je     SHORT loc_4_0255
        db  00Fh, 0B6h, 00Dh, 053h, 001h, 000h, 000h
loc_4_0217:
        bsr    eax, ebx
        btr    ebx, eax
        loop   loc_4_0217
        db  08Dh, 035h, 0A2h, 01Fh, 000h, 000h
loc_4_0225:
        mov    ecx, esi
        mov    esi, dword ptr [esi + 2]
        or     esi, esi
        je     SHORT loc_4_0233
        cmp    byte ptr [esi + 44h], al
        ja     SHORT loc_4_0225
loc_4_0233:
        mov    dword ptr [ecx + 2], edi
        mov    dword ptr [edi + 2], esi
        db  0A2h, 052h, 001h, 000h, 000h
        xor    ebx, ebx
        bts    ebx, eax
        db  089h, 01Dh, 04Eh, 001h, 000h, 000h
        mov    byte ptr [edi + 44h], al
        cmp    eax, 1fh
        jbe    NEAR PTR loc_4_0309
loc_4_0255:
        mov    ebx, dword ptr [ebp + 24h]
        mov    word ptr [ebx + 2ch], 4
        popad
loc_4_025F:
        ret
loc_4_0260:
        cmp    dword ptr [ebx + 8], 100000h
        jne    NEAR PTR loc_4_0119
        mov    word ptr [ebx + 2ch], 1
        db  089h, 03Dh, 045h, 001h, 000h, 000h
        mov    eax, dword ptr [ebx + 2eh]
        mov    dword ptr [edi + 3fh], eax
        call   sub_4_0383
        jmp    NEAR PTR loc_4_0322
sub_4_0289:
        add    byte ptr [ebx], 0
        db  000h, 024h, 003h
        add    byte ptr [eax], al
        add    byte ptr [ebx], 0
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 02Eh
        add    eax, dword ptr [eax]
        db  000h, 083h, 003h, 000h, 000h, 083h
        add    eax, dword ptr [eax]
        db  000h, 082h, 003h, 000h, 000h, 083h
        add    eax, dword ptr [eax]
        db  000h, 083h, 003h, 000h, 000h, 024h
        add    eax, dword ptr [eax]
        db  000h, 083h, 003h, 000h, 000h, 083h
        add    eax, dword ptr [eax]
        db  000h, 083h, 003h, 000h, 000h, 024h
        add    eax, dword ptr [eax]
        db  000h, 083h, 003h, 000h, 000h, 083h
        add    eax, dword ptr [eax]
        db  000h, 024h, 003h
        add    byte ptr [eax], al
loc_4_0309:
        db  0FFh, 014h, 085h, 089h, 002h, 000h, 000h
        db  083h, 03Dh, 045h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_0322
        mov    ebx, dword ptr [ebp + 24h]
        mov    word ptr [ebx + 2ch], 3
loc_4_0322:
        popad
        ret
loc_4_0324:
        mov    ebx, dword ptr [ebp + 24h]
        mov    word ptr [ebx + 2ch], 4
        ret
loc_4_032E:
        sub    esp, 20h
        mov    esi, esp
        mov    word ptr [esi], 0
        call   sub_4_0982
        add    esp, 20h
        db  08Bh, 03Dh, 045h, 001h, 000h, 000h
        test   byte ptr [edi + 3ah], 2
        jne    SHORT loc_4_0381
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        mov    edi, dword ptr [edi + 30h]
loc_4_0355:
        or     edi, edi
        je     SHORT loc_4_0381
        sub    esp, 10h
        mov    esi, esp
        mov    word ptr [esi], 3
        mov    dword ptr [esi + 0ch], edi
        push   edi
        db  08Bh, 03Dh, 045h, 001h, 000h, 000h
        mov    ebx, dword ptr [edi + 10h]
        mov    dword ptr [esi + 4], ebx
        pop    edi
        call   sub_4_0982
        add    esp, 10h
        mov    edi, dword ptr [edi + 0ch]
        jmp    SHORT loc_4_0355
loc_4_0381:
        ret
loc_4_0382:
        ret
sub_4_0383:
        push   1
        push   12bh
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_0A3F
        mov    word ptr [eax + 3eh], 4342h
        db  0A3h, 03Eh, 003h, 000h, 000h
        db  0C6h, 005h, 04Dh, 001h, 000h, 000h, 000h
loc_4_03AD:
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        db  08Bh, 03Dh, 045h, 001h, 000h, 000h
        mov    dword ptr [edi + 10h], 0
        sub    esp, 20h
        mov    esi, esp
        mov    word ptr [esi], 0
        db  08Bh, 03Dh, 044h, 003h, 000h, 000h
        mov    dword ptr [esi + 17h], edi
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        mov    dword ptr [esi + 13h], edi
        db  08Dh, 005h, 007h, 022h, 000h, 000h
        mov    dword ptr [edi + 8], eax
        db  08Bh, 01Dh, 045h, 001h, 000h, 000h
        mov    byte ptr [esi + 10h], 0
        mov    eax, dword ptr [ebx + 3fh]
        mov    dword ptr [esi + 0ch], eax
        call   sub_4_0982
        movzx  ecx, word ptr [esi + 2]
        mov    dl, byte ptr [esi + 10h]
        db  088h, 015h, 04Dh, 001h, 000h, 000h
        test   dl, 1
        je     SHORT loc_4_0410
        or     byte ptr [ebx + 38h], 4
loc_4_0410:
        mov    dx, word ptr [esi + 11h]
        add    esp, 20h
        or     ecx, ecx
        jne    NEAR PTR loc_4_04F8
        inc    byte ptr [ebx + 4fh]
        mov    word ptr [ebx + 49h], dx
        mov    edi, ebx
        mov    ecx, dword ptr [edi + 10h]
        db  089h, 00Dh, 049h, 001h, 000h, 000h
        or     ecx, ecx
        je     NEAR PTR loc_4_04F8
        db  083h, 03Dh, 044h, 003h, 000h, 000h, 0FFh
        je     SHORT loc_4_045C
        db  0A1h, 044h, 003h, 000h, 000h
        mov    dword ptr [ecx + 18h], eax
        mov    ebx, edx
        push   0
        push   ecx
        push   eax
        push   eax
        db  0CDh, 020h, 00Ah, 000h, 033h, 000h
        add    esp, 10h
        mov    edx, ebx
loc_4_045C:
        mov    ebx, edi
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        call   sub_4_09CB
        test   dword ptr [ebx + 38h], 40h
        je     SHORT loc_4_0488
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        db  08Dh, 005h, 007h, 022h, 000h, 000h
        mov    dword ptr [edi + 8], eax
        mov    esi, edi
        call   sub_4_08A9
loc_4_0488:
        mov    edi, ebx
        db  08Bh, 00Dh, 045h, 001h, 000h, 000h
        cmp    byte ptr [ecx + 3eh], 1
        jne    SHORT loc_4_04AF
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        movzx  ecx, byte ptr [ecx + 10h]
loc_4_04A0:
        push   ecx
        call   sub_4_0538
        pop    ecx
        inc    ch
        dec    cl
        jne    SHORT loc_4_04A0
        jmp    SHORT loc_4_04B4
loc_4_04AF:
        call   sub_4_068F
loc_4_04B4:
        db  08Bh, 03Dh, 045h, 001h, 000h, 000h
        db  08Bh, 035h, 049h, 001h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_04F8
        mov    ecx, dword ptr [esi + 8]
        db  089h, 00Dh, 049h, 001h, 000h, 000h
        test   byte ptr [esi + 11h], 4
        je     SHORT loc_4_04F0
        mov    ecx, esi
        call   sub_4_0E2E
        db  08Bh, 035h, 045h, 001h, 000h, 000h
        dec    byte ptr [esi + 4fh]
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        db  080h, 025h, 04Dh, 001h, 000h, 000h, 0FDh
loc_4_04F0:
        or     ecx, ecx
        jne    NEAR PTR loc_4_045C
loc_4_04F8:
        db  0F6h, 005h, 04Dh, 001h, 000h, 000h, 002h
        jne    NEAR PTR loc_4_03AD
        db  08Bh, 035h, 045h, 001h, 000h, 000h
        cmp    byte ptr [esi + 44h], 14h
        jne    SHORT loc_4_0526
        cmp    byte ptr [esi + 4fh], 0
        jne    SHORT loc_4_0526
        db  0C7h, 005h, 045h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        call   IOS_Service_17
loc_4_0526:
        push   0
        db  0FFh, 035h, 03Eh, 003h, 000h, 000h
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        ret
sub_4_0538:
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        sub    esp, 10h
        mov    esi, esp
        mov    word ptr [esi], 6
        call   sub_4_09CB
        mov    byte ptr [edi + 75h], ch
        mov    byte ptr [edi + 7dh], 0
        mov    byte ptr [edi + 7ch], 0
        db  0C6h, 005h, 03Eh, 001h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], edi
        db  0F6h, 005h, 04Dh, 001h, 000h, 000h, 001h
        je     SHORT loc_4_056E
        mov    byte ptr [edi + 7ch], dl
loc_4_056E:
        db  08Bh, 01Dh, 045h, 001h, 000h, 000h
        mov    cl, byte ptr [ebx + 3eh]
        mov    byte ptr [edi + 74h], cl
        mov    word ptr [esi + 2], 1
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        mov    dword ptr [esi + 4], ecx
        call   sub_4_0982
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        push   edx
        mov    dx, word ptr [edi + 7ch]
        mov    eax, dword ptr [ecx + 0ch]
loc_4_059C:
        or     eax, eax
        je     SHORT loc_4_05B3
        cmp    word ptr [eax + 7ch], dx
        jne    SHORT loc_4_05AE
        mov    cl, byte ptr [edi + 75h]
        cmp    byte ptr [eax + 75h], cl
        je     SHORT loc_4_05B3
loc_4_05AE:
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_059C
loc_4_05B3:
        pop    edx
        cmp    word ptr [esi + 2], 0
        jne    NEAR PTR loc_4_0677
        test   byte ptr [edi + 82h], 0e0h
        jne    NEAR PTR loc_4_0677
        or     eax, eax
        je     SHORT loc_4_05F4
        push   esi
        push   edi
        lea    esi, [eax + 82h]
        lea    edi, [edi + 82h]
        mov    ecx, 9
        repe cmpsd
        pop    edi
        pop    esi
        je     SHORT loc_4_0624
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        call   sub_4_0F01
loc_4_05F4:
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        db  0A1h, 045h, 001h, 000h, 000h
        mov    al, byte ptr [eax + 4fh]
        dec    al
        mov    byte ptr [edi + 7eh], al
        db  0A0h, 03Eh, 001h, 000h, 000h
        db  0FEh, 005h, 03Eh, 001h, 000h, 000h
        mov    byte ptr [edi + 0bah], al
        push   esi
        call   sub_4_0772
        call   sub_4_08A9
        pop    esi
loc_4_0624:
        mov    ch, byte ptr [edi + 7dh]
        cmp    ch, dh
        je     SHORT loc_4_0646
        mov    cl, byte ptr [edi + 7ch]
        mov    bl, byte ptr [edi + 75h]
        call   sub_4_09CB
        mov    byte ptr [edi + 75h], bl
        mov    byte ptr [edi + 7ch], cl
        inc    ch
        mov    byte ptr [edi + 7dh], ch
        jmp    NEAR PTR loc_4_056E
loc_4_0646:
        mov    cl, byte ptr [edi + 7ch]
        db  0F6h, 005h, 04Dh, 001h, 000h, 000h, 001h
        je     SHORT loc_4_065A
        or     cl, cl
        je     SHORT loc_4_068B
        dec    cl
        jmp    SHORT loc_4_0660
loc_4_065A:
        cmp    cl, dl
        je     SHORT loc_4_068B
        inc    cl
loc_4_0660:
        mov    bl, byte ptr [edi + 75h]
        call   sub_4_09CB
        mov    byte ptr [edi + 75h], bl
        mov    byte ptr [edi + 7ch], cl
        mov    byte ptr [edi + 7dh], 0
        jmp    NEAR PTR loc_4_056E
loc_4_0677:
        cmp    word ptr [esi + 2], 2
        je     SHORT loc_4_068B
        or     eax, eax
        je     SHORT loc_4_0646
        push   edi
        call   sub_4_0F01
        pop    edi
        jmp    SHORT loc_4_0646
loc_4_068B:
        add    esp, 10h
        ret
sub_4_068F:
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        sub    esp, 10h
        mov    esi, esp
        mov    word ptr [esi], 6
        mov    dword ptr [esi + 0ch], edi
        mov    byte ptr [edi + 0bah], 0
loc_4_06A9:
        db  08Bh, 01Dh, 045h, 001h, 000h, 000h
        mov    cl, byte ptr [ebx + 3eh]
        mov    byte ptr [edi + 74h], cl
        mov    word ptr [esi + 2], 1
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        mov    dword ptr [esi + 4], ecx
        call   sub_4_0982
        cmp    word ptr [esi + 2], 2
        je     NEAR PTR loc_4_0764
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        mov    dl, byte ptr [edi + 0bah]
        mov    eax, dword ptr [ecx + 0ch]
loc_4_06E3:
        or     eax, eax
        je     SHORT loc_4_06F4
        cmp    byte ptr [eax + 0bah], dl
        je     SHORT loc_4_06F4
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_06E3
loc_4_06F4:
        cmp    word ptr [esi + 2], 0
        jne    SHORT loc_4_0759
        or     eax, eax
        je     SHORT loc_4_071D
        push   esi
        push   edi
        lea    esi, [eax + 82h]
        lea    edi, [edi + 82h]
        mov    ecx, 9
        repe cmpsd
        pop    edi
        pop    esi
        je     SHORT loc_4_073C
        call   sub_4_0F01
loc_4_071D:
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        db  0A1h, 045h, 001h, 000h, 000h
        mov    al, byte ptr [eax + 4fh]
        dec    al
        mov    byte ptr [edi + 7eh], al
        push   esi
        call   sub_4_0772
        call   sub_4_08A9
        pop    esi
loc_4_073C:
        mov    dl, byte ptr [edi + 0bah]
        cmp    dl, 7fh
        je     SHORT loc_4_076E
        inc    dl
        call   sub_4_09CB
        mov    byte ptr [edi + 0bah], dl
        jmp    NEAR PTR loc_4_06A9
loc_4_0759:
        or     eax, eax
        je     SHORT loc_4_073C
        call   sub_4_0F01
        jmp    SHORT loc_4_073C
loc_4_0764:
        db  0C7h, 005h, 049h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_076E:
        add    esp, 10h
        ret
sub_4_0772:
        enter  4, 0
        pushad
        db  0A1h, 044h, 003h, 000h, 000h
        cmp    eax, -1
        je     SHORT loc_4_0784
        mov    dword ptr [edi + 70h], eax
loc_4_0784:
        mov    al, byte ptr [edi + 82h]
        and    al, 1fh
        db  0BBh, 064h, 004h, 000h, 000h
        mov    ecx, 0ch
loc_4_0796:
        cmp    al, byte ptr [ebx]
        je     SHORT loc_4_07A4
        add    ebx, 0ch
        loop   loc_4_0796
        db  0BBh, 0E8h, 004h, 000h, 000h
loc_4_07A4:
        sub    esp, 0ch
        mov    esi, esp
        mov    ax, word ptr [ebx + 2]
        mov    word ptr [esi + 4], ax
        mov    word ptr [esi], 1
        push   esi
        db  0E8h, 054h, 00Bh, 0FCh, 0FFh
        mov    esi, dword ptr [esi + 6]
        add    esp, 10h
        push   esi
        push   edi
        xchg   esi, edi
        mov    ecx, 12bh
        rep movsb
        pop    edi
        pop    esi
        or     byte ptr [esi + 21h], 4
        db  08Dh, 005h, 007h, 022h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], esi
        test   byte ptr [esi + 21h], 40h
        je     SHORT loc_4_0819
        mov    al, byte ptr [edi + 40h]
        mov    byte ptr [esi + 40h], al
        mov    al, byte ptr [esi + 0bah]
        mov    byte ptr [esi + 15h], al
        mov    byte ptr [esi + 0cah], al
        sub    esp, 0ch
        mov    ebp, esp
        mov    word ptr [ebp], 6
        mov    byte ptr [ebp + 8], al
        mov    byte ptr [ebp + 9], 0
        mov    dword ptr [ebp + 4], esi
        push   ebp
        db  0E8h, 0FCh, 00Ah, 0FCh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_081F
loc_4_0819:
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [esi + 40h], al
loc_4_081F:
        mov    eax, dword ptr [ebx + 4]
        inc    byte ptr [eax]
        movzx  eax, word ptr [ebx + 8]
        mov    dword ptr [esi + 0dbh], eax
        mov    al, byte ptr [ebx + 0ah]
        mov    byte ptr [esi + 3ch], al
        mov    al, byte ptr [ebx + 0bh]
        mov    byte ptr [esi + 0b9h], al
        db  08Bh, 01Dh, 049h, 001h, 000h, 000h
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 6ch], eax
        mov    dword ptr [ebx + 0ch], esi
        test   byte ptr [esi + 21h], 20h
        jne    SHORT loc_4_08A2
        call   sub_4_2F05
        mov    word ptr [esi + 48h], 0ffffh
        lea    ecx, [ebp - 4]
        sub    esp, 18h
        mov    eax, esp
        mov    word ptr [eax], 11h
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [eax + 0ch], 1
        mov    dword ptr [eax + 10h], 3
        mov    dword ptr [eax + 14h], ecx
        db  0C7h, 040h, 008h, 098h, 000h, 000h, 000h
        push   eax
        db  0E8h, 085h, 00Ah, 0FCh, 0FFh
        movzx  eax, word ptr [eax + 2]
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_4_08A2
        mov    ax, word ptr [ebp - 4]
        sub    ax, 4141h
        mov    word ptr [esi + 48h], ax
loc_4_08A2:
        mov    dword ptr [esp + 4], esi
        popad
        leave
        ret
sub_4_08A9:
        pushad
        sub    esp, 10h
        mov    edi, esp
        mov    word ptr [edi], 3
        mov    dword ptr [edi + 0ch], esi
        xchg   esi, edi
        db  08Bh, 00Dh, 0A4h, 01Fh, 000h, 000h
loc_4_08BF:
        or     ecx, ecx
        je     SHORT loc_4_08EE
        test   dword ptr [ecx + 38h], 60000h
        je     SHORT loc_4_08E9
        mov    edx, dword ptr [ecx + 10h]
        mov    dword ptr [esi + 4], edx
        db  0FFh, 035h, 045h, 001h, 000h, 000h
        db  089h, 00Dh, 045h, 001h, 000h, 000h
        call   sub_4_0982
        db  08Fh, 005h, 045h, 001h, 000h, 000h
loc_4_08E9:
        mov    ecx, dword ptr [ecx + 2]
        jmp    SHORT loc_4_08BF
loc_4_08EE:
        db  08Bh, 01Dh, 049h, 001h, 000h, 000h
        mov    dword ptr [esi + 4], ebx
        call   sub_4_0982
        add    esp, 10h
        test   byte ptr [edi + 20h], 10h
        je     SHORT loc_4_0939
        sub    esp, 18h
        mov    ecx, esp
        mov    word ptr [ecx], 5
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ecx + 12h], eax
        mov    dword ptr [ecx + 4], edi
        db  0C7h, 041h, 008h, 0A8h, 000h, 000h, 000h
        mov    byte ptr [ecx + 16h], 12h
        mov    dword ptr [ecx + 0ch], 0
        mov    word ptr [ecx + 10h], 0
        push   ecx
        db  0E8h, 0DAh, 009h, 0FCh, 0FFh
        add    esp, 1ch
loc_4_0939:
        sub    esp, 18h
        mov    ecx, esp
        mov    word ptr [ecx], 11h
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [ecx + 0ch], 3
        mov    dword ptr [ecx + 10h], 1
        db  0C7h, 041h, 014h, 043h, 001h, 000h, 000h
        db  0C7h, 041h, 008h, 060h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 0A8h, 009h, 0FCh, 0FFh
        movzx  ecx, word ptr [ecx + 2]
        add    esp, 1ch
        or     ecx, ecx
        jne    SHORT loc_4_0980
        db  080h, 03Dh, 043h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0980
        or     byte ptr [edi + 20h], 4
loc_4_0980:
        popad
        ret
sub_4_0982:
        pushad
        db  08Bh, 01Dh, 045h, 001h, 000h, 000h
        mov    cl, byte ptr [ebx + 44h]
        mov    byte ptr [esi + 8], cl
        cmp    dword ptr [ebx + 8], 0
        je     SHORT loc_4_099C
        push   esi
        call   dword ptr [ebx + 8]
        add    esp, 4
loc_4_099C:
        popad
        ret
        db  060h, 08Dh, 00Dh, 0A4h, 01Fh, 000h, 000h, 039h, 001h, 074h, 00Bh, 08Bh, 009h, 00Bh, 0C9h, 074h
        db  019h, 08Dh, 049h, 002h, 0EBh, 0F1h, 08Bh, 050h, 002h, 089h, 011h, 06Ah, 000h, 050h, 0CDh, 020h
        db  051h, 000h, 001h, 000h, 083h, 0C4h, 008h, 061h, 0F8h, 0C3h, 061h, 0F9h, 0C3h
sub_4_09CB:
        push   ebx
        push   ecx
        mov    al, byte ptr [edi + 77h]
        push   eax
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 1ch]
        push   dword ptr [edi + 50h]
        push   dword ptr [edi + 4]
        push   edi
        xor    al, al
        mov    ecx, 12bh
        rep stosb
        pop    edi
        mov    word ptr [edi + 3eh], 4342h
        pop    dword ptr [edi + 4]
        pop    dword ptr [edi + 50h]
        pop    dword ptr [edi + 1ch]
        pop    dword ptr [edi + 8]
        pop    eax
        mov    byte ptr [edi + 77h], al
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 20h], 8000h
        pop    ecx
        pop    ebx
        ret
IOS_Service_17:
        pushad
loc_4_0A0B:
        mov    ecx, dword ptr [esi + 0ch]
        or     ecx, ecx
        je     SHORT loc_4_0A1B
        push   esi
        call   sub_4_0E2E
        pop    esi
        jmp    SHORT loc_4_0A0B
loc_4_0A1B:
        db  08Dh, 00Dh, 0A4h, 01Fh, 000h, 000h
loc_4_0A21:
        cmp    dword ptr [ecx], esi
        je     SHORT loc_4_0A2C
        mov    ecx, dword ptr [ecx]
        lea    ecx, [ecx + 2]
        jmp    SHORT loc_4_0A21
loc_4_0A2C:
        mov    edi, dword ptr [esi + 2]
        mov    dword ptr [ecx], edi
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        popad
        ret
loc_4_0A3F:
        pushad
        db  0BEh, 0F4h, 004h, 000h, 000h
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_0A50:
        ror    byte ptr [edx], 1
        add    byte ptr [eax], al
        ror    byte ptr [edx], 1
        add    byte ptr [eax], al
        aam    0ah
        add    byte ptr [eax], al
        aam    0ah
        add    byte ptr [eax], al
        int3
        or     al, byte ptr [eax]
        db  000h, 0CCh
        or     al, byte ptr [eax]
        db  000h, 0D4h
        or     al, byte ptr [eax]
        db  000h, 0D4h
        or     al, byte ptr [eax]
        db  000h, 0D6h
        or     al, byte ptr [eax]
        db  000h, 0D4h
        or     al, byte ptr [eax]
        db  000h, 0D4h
        or     al, byte ptr [eax]
        db  000h, 0D4h
        or     al, byte ptr [eax]
        db  000h, 0D4h
        or     al, byte ptr [eax]
        db  000h, 05Fh, 00Bh
        add    byte ptr [eax], al
        add    cl, byte ptr [ebx]
        add    byte ptr [eax], al
        and    cl, byte ptr [ebx]
        add    byte ptr [eax], al
        aam    0ah
        add    byte ptr [eax], al
        cmpsd
        or     al, 0
        add    bh, al
        xchg   byte ptr [eax + 1000000h], ah
        add    byte ptr [eax], al
        add    byte ptr [eax - 75h], dl
        xchg   byte ptr [edx - 80000000h], dl
        cld
        inc    esp
        jne    SHORT loc_4_0AC5
        and    eax, 0ffh
        cmp    eax, 11h
        ja     SHORT loc_4_0AC5
        db  08Bh, 004h, 085h, 050h, 00Ah, 000h, 000h
loc_4_0AC1:
        call   eax
        pop    eax
        ret
loc_4_0AC5:
        db  0B8h, 0D0h, 00Ah, 000h, 000h
        jmp    SHORT loc_4_0AC1
loc_4_0ACC:
        or     byte ptr [esi + 6dh], 40h
loc_4_0AD0:
        or     byte ptr [esi + 6eh], 1
loc_4_0AD4:
        stc
        ret
loc_4_0AD6:
        xor    eax, eax
        or     dword ptr [esi + 6ch], 14008h
        test   dword ptr [ebx + 20h], 2000h
        stc
        jne    SHORT loc_4_0B01
        test   dword ptr [ebx + 20h], 4
        jne    SHORT loc_4_0AF4
        inc    al
loc_4_0AF4:
        mov    dword ptr [esi + 0a0h], eax
        mov    word ptr [esi + 6ah], 0
        clc
loc_4_0B01:
        ret
loc_4_0B02:
        xor    eax, eax
        test   byte ptr [ebx + 1dh], 20h
        je     SHORT loc_4_0B14
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0aeh]
        inc    al
loc_4_0B14:
        mov    dword ptr [esi + 0a0h], eax
        mov    word ptr [esi + 6ah], 0
        clc
        ret
loc_4_0B22:
        mov    eax, dword ptr [ebx]
        xor    ecx, ecx
        test   byte ptr [eax + 1dh], 20h
        je     SHORT loc_4_0B3D
        movzx  ecx, byte ptr [esi + 90h]
        dec    cl
        mov    byte ptr [eax + 0aeh], cl
        inc    cl
loc_4_0B3D:
        mov    dword ptr [esi + 0a0h], ecx
        mov    word ptr [esi + 6ah], 0
        clc
        ret
loc_4_0B4B:
        inc    dword ptr [eax]
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh
loc_4_0B55:
        inc    dword ptr [eax]
        add    bh, bh
        db  0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh
loc_4_0B5F:
        movzx  eax, byte ptr [esi + 94h]
        cmp    al, 67h
        je     SHORT loc_4_0BB8
        cmp    al, 47h
        je     SHORT loc_4_0BD7
        cmp    al, 6fh
        je     SHORT loc_4_0BE8
        cmp    al, 40h
        jb     SHORT loc_4_0B9E
loc_4_0B76:
        cmp    al, 60h
        je     SHORT loc_4_0BA0
        db  08Dh, 00Dh, 055h, 00Bh, 000h, 000h
        ja     SHORT loc_4_0B88
        db  08Dh, 00Dh, 04Bh, 00Bh, 000h, 000h
loc_4_0B88:
        and    al, 9fh
        cmp    al, 0ah
        ja     NEAR PTR loc_4_0ACC
        movsx  eax, byte ptr [ecx + eax]
        and    eax, 14000h
        or     dword ptr [esi + 6ch], eax
loc_4_0B9E:
        stc
        ret
loc_4_0BA0:
        mov    eax, dword ptr [esi + 98h]
        test   byte ptr [eax], 1
        jne    SHORT loc_4_0B9E
        or     byte ptr [esi + 6eh], 1
        or     dword ptr [esi + 6ch], 4000h
        stc
        ret
loc_4_0BB8:
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_0BD7
        mov    al, 1
        mov    ecx, dword ptr [esi + 98h]
        cmp    byte ptr [ebx + 40h], 0ah
        je     SHORT loc_4_0BD4
        test   byte ptr [ebx + 16h], 1
        je     SHORT loc_4_0BD4
        xor    al, al
loc_4_0BD4:
        mov    byte ptr [ecx + 1], al
loc_4_0BD7:
        or     byte ptr [esi + 6eh], 1
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_0B76
        xor    eax, eax
        jmp    NEAR PTR loc_4_0AF4
loc_4_0BE8:
        mov    ecx, dword ptr [esi + 98h]
        mov    ebx, dword ptr [esi + 8]
        movzx  eax, byte ptr [ecx]
        mov    ah, 10h
        cmp    ah, al
        ja     NEAR PTR loc_4_0C95
        ror    eax, 10h
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_0C64
        mov    edi, dword ptr [esi + 4]
        mov    edx, dword ptr [ebx + 28h]
        test   byte ptr [ebx + 1eh], 2
        je     SHORT loc_4_0C15
        or     al, 8
loc_4_0C15:
        or     al, 3
        mov    ah, 0ffh
        test   byte ptr [edi + 20h], 1
        je     SHORT loc_4_0C22
        mov    ah, byte ptr [edi + 15h]
loc_4_0C22:
        ror    eax, 10h
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 0ch], 0
        movzx  eax, byte ptr [esi + 0b1h]
        sub    esp, 8
        mov    byte ptr [esp], 6
        mov    byte ptr [esp + 2], al
        push   esi
        lea    esi, [esp + 4]
        call   sub_4_193B
        mov    eax, dword ptr [esi + 3]
        pop    esi
        mov    ecx, dword ptr [esi + 98h]
        mov    dword ptr [ecx + 4], eax
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_4_0AF4
loc_4_0C64:
        push   esi
        push   ecx
        push   eax
        movzx  eax, byte ptr [esi + 0b1h]
        call   sub_4_1BC0
        mov    edx, esi
        pop    eax
        pop    ecx
        pop    esi
        mov    ah, byte ptr [edx + 1]
        push   eax
        push   ebx
        movzx  eax, ah
        or     eax, 40000000h
        db  0E8h, 003h, 006h, 0FCh, 0FFh
        pop    ebx
        pop    eax
        jb     SHORT loc_4_0C90
        or     al, 2
loc_4_0C90:
        mov    edx, dword ptr [edx + 6]
        jmp    SHORT loc_4_0C22
loc_4_0C95:
        mov    dword ptr [esi + 0a0h], 16h
        mov    word ptr [esi + 6ah], 10h
        clc
        ret
loc_4_0CA7:
        mov    eax, dword ptr [esi + 94h]
        cmp    ah, 8
        jne    SHORT loc_4_0CBE
        cmp    al, 67h
        je     SHORT loc_4_0CD0
        cmp    al, 47h
        je     SHORT loc_4_0CD0
        cmp    al, 6fh
        je     SHORT loc_4_0CDA
loc_4_0CBE:
        or     word ptr [esi + 6dh], 140h
        mov    dword ptr [esi + 0a0h], 16h
        stc
        ret
loc_4_0CD0:
        or     byte ptr [esi + 6eh], 1
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_0CBE
loc_4_0CDA:
        xor    eax, eax
        jmp    NEAR PTR loc_4_0AF4
        db  000h, 000h, 000h
loc_4_0CE4:
        test   byte ptr [edi + 0ah], 1
        jne    SHORT loc_4_0D54
        movzx  eax, word ptr [edi + 4]
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_0D54
        mov    ebx, eax
loc_4_0D00:
        movzx  ecx, word ptr [edi + 4]
        mov    dword ptr [edi + 6], ebx
        sub    ecx, 4
        mov    edi, ebx
        add    edi, 4
        xor    ax, ax
        rep stosb
loc_4_0D14:
        mov    word ptr [ebx + 12h], 4442h
        db  08Bh, 03Dh, 045h, 001h, 000h, 000h
        mov    dword ptr [ebx + 14h], edi
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_4_0D38
loc_4_0D2A:
        mov    esi, eax
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        jne    SHORT loc_4_0D2A
        mov    dword ptr [esi + 4], ebx
        jmp    SHORT loc_4_0D3B
loc_4_0D38:
        mov    dword ptr [edi + 0ch], ebx
loc_4_0D3B:
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_4_0D50
loc_4_0D42:
        mov    esi, eax
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        jne    SHORT loc_4_0D42
        mov    dword ptr [esi + 8], ebx
        jmp    SHORT loc_4_0D53
loc_4_0D50:
        mov    dword ptr [edi + 10h], ebx
loc_4_0D53:
        ret
loc_4_0D54:
        movzx  esi, word ptr [edi + 4]
        cmp    esi, 7d0h
        ja     SHORT loc_4_0D6E
loc_4_0D60:
        mov    ecx, 5
        xor    eax, eax
        db  0E8h, 0BFh, 0F9h, 0FBh, 0FFh
        jmp    SHORT loc_4_0D00
loc_4_0D6E:
        shr    esi, 0ch
        inc    esi
        push   0fh
        lea    eax, [edi + 6]
        push   eax
        push   0fffh
        push   0
        push   0
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        movzx  esi, word ptr [edi + 4]
        or     eax, edx
        je     SHORT loc_4_0D60
        or     byte ptr [edx + 11h], 2
        mov    ebx, edx
        xchg   dword ptr [edi + 6], edx
        mov    dword ptr [ebx], edx
        jmp    NEAR PTR loc_4_0D14
loc_4_0DA7:
        movzx  eax, word ptr [edi + 4]
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        mov    ebx, eax
        jne    SHORT loc_4_0DD7
        movzx  esi, word ptr [edi + 4]
        mov    ecx, 4
        xor    eax, eax
        db  0E8h, 05Eh, 0F9h, 0FBh, 0FFh
        movzx  ecx, word ptr [edi + 4]
        mov    edi, ebx
        xor    eax, eax
        rep stosb
loc_4_0DD7:
        mov    dword ptr [edi + 6], ebx
        mov    word ptr [ebx + 3eh], 4342h
        or     byte ptr [ebx + 21h], 4
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        mov    edi, dword ptr [edi + 30h]
        or     edi, edi
        je     SHORT loc_4_0DFC
loc_4_0DF1:
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_4_0E07
        mov    edi, eax
        jmp    SHORT loc_4_0DF1
loc_4_0DFC:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [ecx + 30h], ebx
        jmp    SHORT loc_4_0E0A
loc_4_0E07:
        mov    dword ptr [edi + 0ch], ebx
loc_4_0E0A:
        mov    dword ptr [ebx + 0ch], 0
        or     edi, edi
        je     SHORT loc_4_0E1E
loc_4_0E15:
        mov    edi, dword ptr [edi + 0ch]
        or     edi, edi
        je     SHORT loc_4_0E1E
        jmp    SHORT loc_4_0E15
loc_4_0E1E:
        db  08Bh, 03Dh, 045h, 001h, 000h, 000h
        inc    word ptr [edi + 6]
        ret
loc_4_0E29:
        mov    ecx, dword ptr [edi + 4]
        jmp    SHORT loc_4_0E30
sub_4_0E2E:
        xor    edi, edi
loc_4_0E30:
        db  08Bh, 035h, 0A4h, 01Fh, 000h, 000h
loc_4_0E36:
        lea    edx, [esi + 0ch]
loc_4_0E39:
        cmp    dword ptr [edx], ecx
        je     SHORT loc_4_0E4D
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     SHORT loc_4_0E48
        lea    edx, [edx + 4]
        jmp    SHORT loc_4_0E39
loc_4_0E48:
        mov    esi, dword ptr [esi + 2]
        jmp    SHORT loc_4_0E36
loc_4_0E4D:
        db  089h, 035h, 045h, 001h, 000h, 000h
        push   edx
        mov    eax, dword ptr [ecx + 0ch]
loc_4_0E57:
        or     eax, eax
        je     SHORT loc_4_0E64
        or     byte ptr [eax + 23h], 4
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_0E57
loc_4_0E64:
        mov    eax, dword ptr [ecx + 0ch]
loc_4_0E67:
        or     eax, eax
        je     SHORT loc_4_0E7E
        mov    dword ptr [eax + 70h], 0
        mov    edx, dword ptr [eax + 6ch]
        call   sub_4_0F01
        mov    eax, edx
        jmp    SHORT loc_4_0E67
loc_4_0E7E:
        pop    edx
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx], edi
        sub    esp, 0ch
        mov    esi, esp
        mov    word ptr [esi], 0fh
        mov    dword ptr [esi + 4], ecx
        call   sub_4_0982
        add    esp, 0ch
        mov    eax, dword ptr [ecx + 18h]
        or     eax, eax
        je     SHORT loc_4_0EB1
        push   ecx
        push   0
        push   0
        push   eax
        push   eax
        db  0CDh, 020h, 00Ah, 000h, 033h, 000h
        add    esp, 10h
        pop    ecx
loc_4_0EB1:
        test   byte ptr [ecx + 11h], 2
        jne    SHORT loc_4_0EE2
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        cmp    ecx, eax
        jb     SHORT loc_4_0ED5
        add    eax, dword ptr [eax + 60h]
        add    eax, 6eh
        cmp    ecx, eax
        ja     SHORT loc_4_0ED5
        push   edi
        mov    edi, ecx
        db  0E8h, 04Ch, 0FBh, 0FBh, 0FFh
        pop    edi
        jmp    SHORT loc_4_0EE1
loc_4_0ED5:
        push   0
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0EE1:
        ret
loc_4_0EE2:
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_4_0EE1
        db  00Bh, 0FFh, 074h, 0EDh, 066h, 0C7h, 047h, 002h, 0FFh, 0FFh, 0EBh, 0E5h
loc_4_0EFC:
        mov    eax, dword ptr [edi + 4]
        jmp    SHORT loc_4_0F03
sub_4_0F01:
        xor    edi, edi
loc_4_0F03:
        push   ecx
        push   edx
        push   eax
        push   ebp
        mov    ecx, edi
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [edi], 15h
        db  0E8h, 03Fh, 008h, 0FCh, 0FFh
        mov    eax, dword ptr [edi + 0ch]
        add    esp, 10h
        mov    edi, ecx
        db  0E8h, 0B0h, 008h, 0FCh, 0FFh
        sub    esp, 10h
        mov    ebp, esp
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp], 4
        push   esi
        test   byte ptr [eax + 21h], 40h
        jne    SHORT loc_4_0F54
        mov    esi, dword ptr [eax + 10h]
        mov    dword ptr [eax + 10h], 0
loc_4_0F47:
        or     esi, esi
        je     SHORT loc_4_0F6A
        push   eax
        call   sub_4_0F99
        pop    eax
        jmp    SHORT loc_4_0F47
loc_4_0F54:
        mov    ebx, dword ptr [eax]
loc_4_0F56:
        or     ebx, ebx
        je     SHORT loc_4_0F6A
        cmp    dword ptr [ebx + 10h], eax
        je     SHORT loc_4_0F64
        mov    ebx, dword ptr [ebx + 10h]
        jmp    SHORT loc_4_0F56
loc_4_0F64:
        mov    esi, dword ptr [eax + 10h]
        mov    dword ptr [ebx + 10h], esi
loc_4_0F6A:
        mov    esi, eax
        mov    eax, dword ptr [esi + 70h]
        or     eax, eax
        je     SHORT loc_4_0F7F
        push   0
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 033h, 000h
        add    esp, 8
loc_4_0F7F:
        call   sub_4_0F99
        pop    esi
        add    esp, 10h
        pop    ebp
        pop    eax
        pop    edx
        pop    ecx
        ret
        db  00Bh, 0FFh, 074h, 0F7h, 066h, 0C7h, 047h, 002h, 0FFh, 0FFh, 0EBh, 0EFh
sub_4_0F99:
        or     byte ptr [esi + 23h], 4
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [edx + 28h], 1
        je     SHORT loc_4_0FFA
        test   byte ptr [esi + 21h], 80h
        je     SHORT loc_4_0FCF
        test   byte ptr [esi + 20h], 1
        je     SHORT loc_4_0FCF
        mov    eax, dword ptr [edx + 38h]
loc_4_0FB8:
        mov    dl, byte ptr [eax]
        cmp    dl, 0ffh
        je     SHORT loc_4_0FCF
        cmp    byte ptr [esi + 15h], dl
        jne    SHORT loc_4_0FCA
        and    byte ptr [eax + 9], 0fdh
        jmp    SHORT loc_4_0FCF
loc_4_0FCA:
        add    eax, 0bh
        jmp    SHORT loc_4_0FB8
loc_4_0FCF:
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_0FFA
        movzx  eax, byte ptr [esi + 14h]
        push   esi
        call   sub_4_1BC0
        mov    edi, esi
        pop    esi
        jb     SHORT loc_4_0FF0
        btr    word ptr [edi + 10h], 0
loc_4_0FF0:
        push   eax
        db  0CDh, 020h, 04Eh, 000h, 040h, 000h
        add    esp, 4
loc_4_0FFA:
        xor    edi, edi
        call   sub_4_293A
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        lea    ebx, [edx + 40h]
        sub    ebx, 24h
        mov    edx, dword ptr [edx + 40h]
loc_4_1010:
        or     edx, edx
        je     SHORT loc_4_1036
        cmp    dword ptr [edx + 28h], esi
        je     SHORT loc_4_1020
        mov    ebx, edx
        mov    edx, dword ptr [edx + 24h]
        jmp    SHORT loc_4_1010
loc_4_1020:
        mov    edi, dword ptr [edx + 24h]
        mov    dword ptr [ebx + 24h], edi
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    edx, edi
        jmp    SHORT loc_4_1010
loc_4_1036:
        mov    dword ptr [ebp + 0ch], esi
        mov    edi, ebp
        db  0E8h, 01Ah, 007h, 0FCh, 0FFh
        push   dword ptr [esi + 10h]
        call   sub_4_105B
        call   sub_4_1082
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    esi
        ret
sub_4_105B:
        mov    edi, dword ptr [esi + 8]
loc_4_105E:
        or     edi, edi
        je     SHORT loc_4_1081
        mov    edx, dword ptr [edi + 0ch]
        db  081h, 0FFh, 007h, 022h, 000h, 000h
        je     SHORT loc_4_107D
        xchg   edx, edi
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    edx, edi
loc_4_107D:
        mov    edi, edx
        jmp    SHORT loc_4_105E
loc_4_1081:
        ret
sub_4_1082:
        push   ebp
        db  08Bh, 02Dh, 0A4h, 01Fh, 000h, 000h
loc_4_1089:
        or     ebp, ebp
        je     SHORT loc_4_10BC
        mov    ecx, dword ptr [ebp + 0ch]
loc_4_1090:
        or     ecx, ecx
        je     SHORT loc_4_10B1
        lea    ebx, [ecx + 0ch]
        sub    ebx, 6ch
        mov    edx, dword ptr [ebx + 6ch]
loc_4_109D:
        or     edx, edx
        je     SHORT loc_4_10AC
        cmp    edx, esi
        je     SHORT loc_4_10B6
        mov    ebx, edx
        mov    edx, dword ptr [edx + 6ch]
        jmp    SHORT loc_4_109D
loc_4_10AC:
        mov    ecx, dword ptr [ecx + 4]
        jmp    SHORT loc_4_1090
loc_4_10B1:
        mov    ebp, dword ptr [ebp + 2]
        jmp    SHORT loc_4_1089
loc_4_10B6:
        mov    edi, dword ptr [edx + 6ch]
        mov    dword ptr [ebx + 6ch], edi
loc_4_10BC:
        pop    ebp
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        add    ebx, 24h
        mov    edx, dword ptr [ebx + 0ch]
loc_4_10C9:
        or     edx, edx
        je     SHORT loc_4_112C
        cmp    edx, esi
        je     SHORT loc_4_10D8
        mov    ebx, edx
        mov    edx, dword ptr [edx + 0ch]
        jmp    SHORT loc_4_10C9
loc_4_10D8:
        mov    edi, dword ptr [edx + 0ch]
        mov    dword ptr [ebx + 0ch], edi
        test   byte ptr [esi + 20h], 1
        je     SHORT loc_4_10F8
        movzx  edx, byte ptr [esi + 15h]
        sub    dl, 80h
        jb     SHORT loc_4_10F8
        db  0C7h, 004h, 095h, 014h, 021h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_10F8:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_111A
        dec    byte ptr [esi + 46h]
        movzx  edx, byte ptr [esi + 14h]
        db  0C7h, 004h, 095h, 0ACh, 020h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        btr    dword ptr [ebx + 44h], edx
loc_4_111A:
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        cmp    dword ptr [edx + 3ch], esi
        jne    SHORT loc_4_112C
        mov    dword ptr [edx + 3ch], 0
loc_4_112C:
        ret
loc_4_112D:
        push   ebp
        push   ebx
        push   esi
        mov    eax, dword ptr [edi + 4]
        mov    word ptr [edi + 2], 0ffffh
        test   byte ptr [eax + 21h], 40h
        jne    SHORT loc_4_116C
        sub    esp, 14h
        mov    ebp, esp
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 10h], 0
        mov    word ptr [ebp], 0ch
        push   edi
        mov    edi, ebp
        db  0E8h, 0FEh, 005h, 0FCh, 0FFh
        pop    edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edi + 8], eax
        mov    word ptr [edi + 2], 0
        add    esp, 14h
loc_4_116C:
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_1170:
        push   1
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_11ED
        mov    ebx, dword ptr [edi + 8]
        mov    dword ptr [eax], ebx
        mov    ebx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [edi + 12h]
        mov    dword ptr [eax + 4], ebx
        mov    ecx, dword ptr [edi + 4]
        mov    ebx, dword ptr [ecx + 4]
        add    bx, word ptr [edi + 10h]
        add    ebx, 3
        and    bl, 0fch
        mov    dword ptr [ecx + 4], ebx
        neg    ebx
        mov    word ptr [eax + 10h], bx
        mov    bl, byte ptr [edi + 16h]
        mov    byte ptr [eax + 13h], bl
        lea    edx, [ecx + 8]
        mov    esi, dword ptr [edx]
loc_4_11B6:
        or     esi, esi
        je     SHORT loc_4_11DC
        test   byte ptr [esi + 6], 80h
        jne    SHORT loc_4_11CC
loc_4_11C0:
        cmp    byte ptr [esi + 13h], bl
        jae    SHORT loc_4_11DC
        lea    edx, [esi + 0ch]
        mov    esi, dword ptr [edx]
        jmp    SHORT loc_4_11B6
loc_4_11CC:
        push   ebx
        mov    ebx, dword ptr [eax + 4]
        or     ebx, 800000h
        mov    dword ptr [esi + 4], ebx
        pop    ebx
        jmp    SHORT loc_4_11C0
loc_4_11DC:
        mov    esi, dword ptr [edx]
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [edx], eax
        mov    ebx, dword ptr [ecx + 8]
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 1ch], ebx
        ret
loc_4_11ED:
        mov    word ptr [edi + 2], 1
        ret
loc_4_11F4:
        movzx  ebx, byte ptr [edi + 8]
        mov    eax, dword ptr [edi + 4]
        test   byte ptr [eax + 21h], 20h
        jne    SHORT loc_4_127C
        cmp    byte ptr [eax + 40h], 0ah
        je     NEAR PTR loc_4_1292
loc_4_120B:
        cmp    bl, 0ffh
        je     NEAR PTR loc_4_12A5
        test   bl, 80h
        je     SHORT loc_4_122D
        sub    bl, 80h
        cmp    bl, 18h
        ja     NEAR PTR loc_4_12A6
        db  089h, 004h, 09Dh, 014h, 021h, 000h, 000h
        ret
loc_4_122D:
        cmp    bl, 1ah
        jae    SHORT loc_4_12A6
        cmp    byte ptr [eax + 46h], 0
        jne    SHORT loc_4_123B
        mov    byte ptr [eax + 14h], bl
loc_4_123B:
        inc    byte ptr [eax + 46h]
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        bts    dword ptr [ecx + 44h], ebx
        db  087h, 004h, 09Dh, 0ACh, 020h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_1256
        dec    byte ptr [eax + 46h]
loc_4_1256:
        test   byte ptr [ecx + 29h], 4
        jne    SHORT loc_4_127B
        push   ebx
        db  0CDh, 020h, 04Dh, 000h, 040h, 000h
        pop    ebx
        test   byte ptr [edi + 9], 1
        jne    SHORT loc_4_127B
        push   0
        push   ebx
        db  068h, 055h, 013h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
loc_4_127B:
        ret
loc_4_127C:
        test   byte ptr [eax + 21h], 80h
        je     SHORT loc_4_120B
        test   byte ptr [eax + 21h], 40h
        jne    SHORT loc_4_120B
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [ebx + 3ch], eax
        ret
loc_4_1292:
        test   byte ptr [eax + 21h], 40h
        jne    SHORT loc_4_122D
        cmp    bl, 8
        ja     SHORT loc_4_12A6
        db  089h, 004h, 09Dh, 074h, 021h, 000h, 000h
        ret
loc_4_12A5:
        ret
loc_4_12A6:
        mov    word ptr [edi + 2], 1
        ret
loc_4_12AD:
        mov    eax, dword ptr [edi + 4]
        call   sub_4_2819
        jae    SHORT loc_4_12BD
        mov    word ptr [edi + 2], 0ffffh
loc_4_12BD:
        ret
loc_4_12BE:
        mov    edx, dword ptr [edi + 4]
        cmp    edx, 1ah
        jae    SHORT loc_4_130C
        xor    eax, eax
        db  087h, 004h, 095h, 0ACh, 020h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_130B
        dec    byte ptr [eax + 46h]
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        btr    dword ptr [eax + 44h], edx
        mov    eax, dword ptr [eax + 28h]
        xor    al, 1
        test   eax, 4001h
        jne    SHORT loc_4_130B
        push   ecx
        push   edx
        db  0CDh, 020h, 04Eh, 000h, 040h, 000h
        add    esp, 4
        mov    edx, dword ptr [edi + 4]
        push   0
        push   edx
        db  068h, 07Eh, 013h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        pop    ecx
loc_4_130B:
        ret
loc_4_130C:
        mov    word ptr [edi + 2], 1
        ret
loc_4_1313:
        mov    esi, dword ptr [edi + 4]
        mov    edi, dword ptr [edi + 8]
        and    edi, 1
loc_4_131C:
        call   sub_4_293A
        mov    esi, dword ptr [esi + 10h]
        or     esi, esi
        jne    SHORT loc_4_131C
        ret
loc_4_1329:
        mov    esi, dword ptr [edi + 4]
loc_4_132C:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_134D
        push   edi
        mov    edi, dword ptr [edi + 8]
        and    edi, 1
        mov    edx, 0
        mov    eax, 8000h
        movzx  ebx, byte ptr [esi + 14h]
        call   sub_4_28F9
        pop    edi
loc_4_134D:
        mov    esi, dword ptr [esi + 10h]
        or     esi, esi
        jne    SHORT loc_4_132C
        ret
loc_4_1355:
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 10h]
        db  0E8h, 02Ch, 0FFh, 0FBh, 0FFh
        jb     SHORT loc_4_137A
        mov    esi, ebx
        xor    edi, edi
        mov    edx, 0
        mov    eax, 8000h
        mov    ebx, dword ptr [esp + 10h]
        call   sub_4_28F9
loc_4_137A:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_4_137E:
        push   esi
        push   edi
        push   ebx
        xor    edi, edi
        mov    edx, edi
        mov    eax, 8004h
        mov    ebx, dword ptr [esp + 10h]
        call   sub_4_28F9
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_4_1397:
        sub    esp, 4
        test   byte ptr [edi + 0ah], 10h
        jne    NEAR PTR loc_4_1475
        mov    ebx, dword ptr [edi + 4]
        test   byte ptr [edi + 0ah], 8
        jne    SHORT loc_4_1403
        mov    esi, dword ptr [ebx + 70h]
        or     esi, esi
        je     SHORT loc_4_1403
        mov    ax, word ptr [ebx + 48h]
        cmp    ax, -1
        je     SHORT loc_4_1403
loc_4_13BE:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        push   ebx
        movzx  ebx, al
        bts    dword ptr [ecx + 44h], ebx
        pop    ebx
        jae    SHORT loc_4_144E
        test   byte ptr [edi + 0ah], 4
        je     SHORT loc_4_13E4
        push   ebx
        movzx  ebx, al
        bt     dword ptr [ecx + 48h], ebx
        pop    ebx
        jb     NEAR PTR loc_4_146E
loc_4_13E4:
        test   byte ptr [edi + 0ah], 2
        je     SHORT loc_4_13FB
        push   esi
        call   sub_4_1BC0
        mov    ecx, esi
        pop    esi
        jb     SHORT loc_4_13FB
        test   byte ptr [ecx + 10h], 2
        jne    SHORT loc_4_146E
loc_4_13FB:
        inc    al
        je     SHORT loc_4_1403
        cmp    al, ah
        jbe    SHORT loc_4_13BE
loc_4_1403:
        test   byte ptr [edi + 0ah], 1
        jne    SHORT loc_4_1446
        xor    eax, eax
        db  00Fh, 0B6h, 01Dh, 042h, 003h, 000h, 000h
        test   byte ptr [edi + 0ah], 8
        je     SHORT loc_4_1428
        movzx  eax, byte ptr [edi + 8]
        dec    eax
        cmp    byte ptr [edi + 9], bl
        jae    SHORT loc_4_1441
        movzx  ebx, byte ptr [edi + 9]
        jmp    SHORT loc_4_1441
loc_4_1428:
        db  08Bh, 00Ch, 085h, 0BEh, 000h, 000h, 000h
        test   byte ptr [ecx + 44h], 40h
        jne    SHORT loc_4_1441
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        bts    dword ptr [edx + 44h], eax
        jae    SHORT loc_4_146E
loc_4_1441:
        inc    eax
        cmp    eax, ebx
        jbe    SHORT loc_4_1428
loc_4_1446:
        mov    word ptr [edi + 2], 0ffffh
        jmp    SHORT loc_4_1471
loc_4_144E:
        movzx  eax, al
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        btr    dword ptr [ecx + 44h], eax
        push   eax
        db  08Bh, 004h, 085h, 0BEh, 000h, 000h, 000h
        test   byte ptr [eax + 44h], 40h
        pop    eax
        jne    SHORT loc_4_1403
        bts    dword ptr [ecx + 44h], eax
loc_4_146E:
        mov    byte ptr [edi + 8], al
loc_4_1471:
        add    esp, 4
        ret
loc_4_1475:
        movzx  edx, byte ptr [edi + 8]
        db  038h, 015h, 042h, 003h, 000h, 000h
        jb     SHORT loc_4_1446
        db  083h, 03Ch, 095h, 0ACh, 020h, 000h, 000h, 000h
        jne    SHORT loc_4_1446
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        btr    dword ptr [eax + 44h], edx
        jae    SHORT loc_4_1446
        jmp    SHORT loc_4_1471
loc_4_1498:
        mov    eax, dword ptr [edi + 4]
        cmp    eax, 1ah
        cmc
        jb     SHORT loc_4_14A6
        call   sub_4_1BC0
loc_4_14A6:
        sbb    eax, eax
        mov    dword ptr [edi + 8], esi
        mov    word ptr [edi + 2], ax
        ret
loc_4_14B0:
        mov    eax, dword ptr [edi + 4]
        cmp    eax, 1ah
        cmc
        jb     SHORT loc_4_14DC
        call   sub_4_1BC0
        jb     SHORT loc_4_14DC
        mov    eax, edi
        mov    edi, esi
loc_4_14C4:
        add    esi, 12h
        cmp    byte ptr [esi], 0ffh
        jne    SHORT loc_4_14C4
        mov    ecx, esi
        sub    ecx, edi
        sub    ecx, 11h
        lea    esi, [edi + 12h]
        cld
        rep movsb
        mov    edi, eax
        clc
loc_4_14DC:
        sbb    eax, eax
        mov    word ptr [edi + 2], ax
        ret
loc_4_14E3:
        mov    edi, dword ptr [edi + 4]
        db  0E8h, 06Fh, 002h, 0FCh, 0FFh
        ret
sub_4_14EC:
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  08Bh, 035h, 0A4h, 01Fh, 000h, 000h
        sub    esp, 0ch
        mov    edi, esp
        mov    word ptr [edi], 2
        mov    dword ptr [edi + 4], 0
loc_4_150D:
        or     esi, esi
        je     SHORT loc_4_1553
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_4_154E
        pushad
        push   edi
        call   dword ptr [esi + 8]
        add    esp, 4
        popad
        cmp    word ptr [edi + 2], 0
        je     SHORT loc_4_154E
        mov    dword ptr [esi + 8], 0
        mov    edx, dword ptr [esi + 45h]
        or     edx, edx
        je     SHORT loc_4_1542
        mov    bx, 0ffffh
        mov    edx, dword ptr [edx + 0bh]
        db  0CDh, 020h, 002h, 000h, 027h, 000h
loc_4_1542:
        mov    ebx, dword ptr [esi + 2]
        call   IOS_Service_17
        mov    esi, ebx
        jmp    SHORT loc_4_150D
loc_4_154E:
        mov    esi, dword ptr [esi + 2]
        jmp    SHORT loc_4_150D
loc_4_1553:
        add    esp, 0ch
        pop    ebp
        ret
IOS_Requestor_Service:
        pushad
        mov    esi, dword ptr [esp + 24h]
        xor    ebx, ebx
        mov    byte ptr [esi + 1], bl
        mov    bl, byte ptr [esi]
        cmp    bl, 0ah
        ja     SHORT loc_4_1572
        db  0FFh, 014h, 09Dh, 077h, 015h, 000h, 000h
loc_4_1570:
        popad
        ret
loc_4_1572:
        inc    byte ptr [esi + 1]
        jmp    SHORT loc_4_1570
sub_4_1577:
        sbb    edx, dword ptr [esi]
        add    byte ptr [eax], al
        pop    eax
        pop    ss
        add    byte ptr [eax], al
        db  0A3h, 015h, 000h, 000h, 01Dh
        sbb    byte ptr [eax], al
        db  000h, 0DFh
        sbb    byte ptr [eax], al
        db  000h, 005h, 019h, 000h, 000h, 03Bh
        sbb    dword ptr [eax], eax
        db  000h, 00Ah
        sbb    al, byte ptr [eax]
        db  000h, 0CDh
        sbb    dword ptr [eax], eax
        db  000h, 08Fh, 01Ah, 000h, 000h, 0ECh
        sbb    al, byte ptr [eax]
        add    byte ptr [ebx + 57h], dl
        movzx  ebx, byte ptr [esi + 2]
        test   bl, 80h
        jne    SHORT loc_4_15D1
        cmp    bl, 41h
        jb     SHORT loc_4_1614
        cmp    bl, 7ah
        ja     SHORT loc_4_1614
        and    bl, 0dfh
        cmp    bl, 5ah
        ja     SHORT loc_4_1614
        sub    bl, 41h
        cmp    bl, 1ah
        jae    SHORT loc_4_1614
        db  08Bh, 03Ch, 09Dh, 0ACh, 020h, 000h, 000h
        jmp    SHORT loc_4_15E0
loc_4_15D1:
        sub    bl, 80h
        cmp    bl, 18h
        ja     SHORT loc_4_1614
        db  08Bh, 03Ch, 09Dh, 014h, 021h, 000h, 000h
loc_4_15E0:
        or     edi, edi
        je     SHORT loc_4_1614
        mov    ebx, dword ptr [edi + 20h]
        mov    dword ptr [esi + 3], ebx
        test   byte ptr [edi + 1dh], 20h
        je     SHORT loc_4_15F4
        or     byte ptr [esi + 5], 80h
loc_4_15F4:
        test   byte ptr [edi + 1eh], 40h
        je     SHORT loc_4_15FE
        or     byte ptr [esi + 5], 40h
loc_4_15FE:
        test   ebx, 2000h
        jne    SHORT loc_4_1611
        mov    edi, dword ptr [edi]
        mov    ebx, dword ptr [edi + 20h]
        and    ebx, 1
        or     dword ptr [esi + 3], ebx
loc_4_1611:
        pop    ebx
        pop    edi
        ret
loc_4_1614:
        pop    ebx
        pop    edi
        mov    byte ptr [esi + 1], 1
        ret
loc_4_161B:
        mov    edi, esi
        mov    eax, dword ptr [esi + 3]
        test   byte ptr [esi + 2], 1
        jne    NEAR PTR loc_4_16F9
        db  0E8h, 05Eh, 0FCh, 0FBh, 0FFh
        jb     NEAR PTR loc_4_16F3
loc_4_1635:
        push   0
        push   30h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_16F3
        mov    ecx, dword ptr [ebx + 1ch]
        and    ecx, 653e10h
        test   dword ptr [ebx + 20h], 40000h
        je     SHORT loc_4_1662
        or     ecx, 800000h
loc_4_1662:
        test   dword ptr [ebx + 20h], 4
        je     SHORT loc_4_1671
        or     ecx, 4000h
loc_4_1671:
        mov    edx, ebx
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_167B
        mov    edx, dword ptr [ebx]
loc_4_167B:
        mov    dword ptr [eax], ecx
        xor    ecx, ecx
        cmp    dword ptr [edx + 41h], 0
        je     SHORT loc_4_168B
        or     ecx, 8000h
loc_4_168B:
        mov    dword ptr [eax + 4], ecx
        test   byte ptr [ebx + 21h], 20h
        je     SHORT loc_4_1698
        or     byte ptr [eax + 4], 80h
loc_4_1698:
        test   byte ptr [ebx + 20h], 2
        jne    SHORT loc_4_16A2
        or     byte ptr [eax + 4], 40h
loc_4_16A2:
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 0bch
        mov    word ptr [eax + 0ah], cx
        movzx  ecx, byte ptr [edx + 77h]
        mov    word ptr [eax + 8], cx
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 64h
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [edx + 0dbh]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [ebx + 28h]
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [eax + 28h], ebx
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_16DD
        mov    ebx, dword ptr [ebx]
loc_4_16DD:
        add    ebx, 0c3h
        mov    dword ptr [eax + 1ch], ebx
        db  0E8h, 0B0h, 000h, 0FCh, 0FFh
        mov    byte ptr [edi + 1], 0
        mov    dword ptr [edi + 0bh], eax
loc_4_16F2:
        ret
loc_4_16F3:
        mov    byte ptr [edi + 1], 0a0h
        jmp    SHORT loc_4_16F2
loc_4_16F9:
        push   edi
        mov    esi, eax
        call   sub_4_21EF
        mov    edi, esi
        sub    esp, 10h
        mov    edx, esp
        mov    word ptr [edx], 8
        mov    byte ptr [edx + 0ch], 0ffh
        mov    dword ptr [edx + 4], 0
        push   edx
loc_4_1719:
        db  0E8h, 0F2h, 0FBh, 0FBh, 0FFh
        movzx  ecx, word ptr [edx + 2]
        or     ecx, ecx
        jne    SHORT loc_4_174A
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [edx + 4], ebx
        test   byte ptr [ebx + 20h], 80h
        je     SHORT loc_4_1719
        test   byte ptr [ebx + 21h], 80h
        je     SHORT loc_4_1719
        mov    esi, dword ptr [ebx + 0cfh]
        call   sub_4_21EF
        call   sub_4_21D0
        jne    SHORT loc_4_1719
loc_4_174A:
        add    esp, 14h
        pop    edi
        or     ecx, ecx
        je     NEAR PTR loc_4_1635
        jmp    SHORT loc_4_16F3
loc_4_1758:
        pushfd
        movzx  eax, byte ptr [esi + 2]
        test   byte ptr [esi + 3], 1
        je     SHORT loc_4_176B
        mov    edi, dword ptr [esi + 4]
        mov    ebx, dword ptr [edi + 1ch]
        jmp    SHORT loc_4_177D
loc_4_176B:
        db  0E8h, 01Dh, 0FBh, 0FBh, 0FFh
        jb     NEAR PTR loc_4_1817
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        je     SHORT loc_4_17D2
loc_4_177D:
        db  0E8h, 02Ch, 000h, 0FCh, 0FFh
        jb     SHORT loc_4_17F3
        mov    ecx, dword ptr [esi + 8]
        or     ecx, ecx
        jne    SHORT loc_4_17BD
        cmp    dword ptr [ebx + 18h], edi
        jne    SHORT loc_4_1793
        mov    dword ptr [ebx + 18h], ecx
loc_4_1793:
        test   byte ptr [esi + 3], 1
        jne    SHORT loc_4_17C4
        sub    esp, 14h
        mov    word ptr [esp], 13h
        mov    dword ptr [esp + 0ch], edi
        mov    edi, esp
        movzx  ecx, byte ptr [esi + 2]
        mov    dword ptr [edi + 10h], ecx
        db  0E8h, 0A6h, 0FFh, 0FBh, 0FFh
        mov    edi, dword ptr [esp + 0ch]
        add    esp, 14h
        jmp    SHORT loc_4_17C4
loc_4_17BD:
        mov    dword ptr [ebx + 18h], ecx
        and    dword ptr [ecx + 4], 0ffffffdeh
loc_4_17C4:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_4_17F3
loc_4_17D2:
        mov    ecx, dword ptr [esi + 8]
        jecxz  loc_4_17F3
        sub    esp, 14h
        mov    edi, esp
        mov    word ptr [edi], 12h
        mov    dword ptr [edi + 0ch], ecx
        movzx  ecx, byte ptr [esi + 2]
        mov    dword ptr [edi + 10h], ecx
        db  0E8h, 06Ah, 0FFh, 0FBh, 0FFh
        add    esp, 14h
loc_4_17F3:
        test   byte ptr [esi + 3], 2
        jne    SHORT loc_4_1815
        cli
        mov    ecx, dword ptr [ebx + 18h]
        or     ecx, ecx
        je     SHORT loc_4_1815
        and    dword ptr [ecx + 4], 0fffffffch
        and    dword ptr [ebx + 20h], 0ffefefffh
        mov    eax, dword ptr [ebx]
        mov    byte ptr [eax + 0b7h], 0
loc_4_1815:
        popfd
        ret
loc_4_1817:
        mov    byte ptr [esi + 1], 1
        jmp    SHORT loc_4_1815
loc_4_181D:
        mov    edx, dword ptr [esi + 3]
        or     edx, edx
        jne    SHORT loc_4_1831
        movzx  eax, byte ptr [esi + 2]
        db  0E8h, 060h, 0FAh, 0FBh, 0FFh
        mov    ebp, ebx
        jmp    SHORT loc_4_183A
loc_4_1831:
        mov    ebp, dword ptr [edx + 1ch]
        sub    ebp, 0c3h
loc_4_183A:
        mov    al, byte ptr [esi + 2]
        mov    edi, esi
        mov    esi, dword ptr [ebp + 4]
        add    esi, 64h
        sub    esp, 10h
        mov    ebx, esp
        mov    dword ptr [ebx + 6], esi
        add    esi, 58h
        mov    word ptr [ebx + 4], si
        mov    byte ptr [ebx + 0eh], 2
        mov    word ptr [ebx], 2
        push   ebx
        db  0E8h, 0ADh, 0FAh, 0FBh, 0FFh
        mov    ebx, dword ptr [ebx + 0ah]
        add    esp, 14h
        add    ebx, dword ptr [ebp + 4]
        mov    esi, edi
        mov    word ptr [ebx + 68h], 6
        mov    byte ptr [ebx + 0b1h], al
        xor    eax, eax
        mov    dword ptr [ebx + 64h], eax
        mov    dword ptr [ebx + 0a8h], eax
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [ebx + 80h], eax
        mov    dword ptr [ebx + 6ch], 500h
        mov    edi, ebx
        add    ebx, 64h
        xchg   esi, ebx
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        mov    ax, word ptr [edi + 6ah]
        mov    edi, dword ptr [edi + 88h]
        db  0E8h, 06Dh, 0F1h, 0FBh, 0FFh
        mov    byte ptr [ebx + 1], 0a2h
        cmp    ax, 10h
        jae    SHORT loc_4_18D1
        mov    byte ptr [ebx + 1], 0
        mov    edx, dword ptr [ebx + 3]
        or     edx, edx
        je     SHORT loc_4_18CA
        and    dword ptr [edx + 4], 0fffffffdh
loc_4_18CA:
        and    dword ptr [ebp + 20h], 0ffffefffh
loc_4_18D1:
        ret
        db  090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
loc_4_18DF:
        mov    byte ptr [esi + 1], 1
        db  08Bh, 03Dh, 0ACh, 020h, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_1904
        test   byte ptr [edi + 1dh], 20h
        je     SHORT loc_4_1904
        mov    al, byte ptr [esi + 2]
        cmp    al, 1
        ja     SHORT loc_4_1904
        mov    byte ptr [edi + 0aeh], al
        mov    byte ptr [esi + 1], 0
loc_4_1904:
        ret
loc_4_1905:
        db  0C6h, 005h, 063h, 005h, 000h, 000h, 001h
        push   0
        db  0FFh, 035h, 058h, 03Ah, 000h, 000h
        db  0CDh, 020h, 032h, 000h, 033h, 000h
        add    esp, 8
        db  0C7h, 005h, 06Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    esp, 0ch
        mov    word ptr [esp], 0dh
        mov    edi, esp
        db  0E8h, 023h, 0FEh, 0FBh, 0FFh
        add    esp, 0ch
        ret
sub_4_193B:
        mov    dword ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 0ffh
        movzx  eax, byte ptr [esi + 2]
        db  0E8h, 03Eh, 0F9h, 0FBh, 0FFh
        jb     SHORT loc_4_19B5
        test   byte ptr [ebx + 21h], 40h
        je     SHORT loc_4_19B5
        sub    esp, 18h
        mov    word ptr [esp], 10h
        mov    dword ptr [esp + 0ch], ebx
        mov    dword ptr [esp + 10h], 0
        mov    byte ptr [esp + 14h], al
        mov    edi, esp
        db  0E8h, 0E3h, 0FDh, 0FBh, 0FFh
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 3], eax
        mov    byte ptr [esi + 1], 0
        add    esp, 18h
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        cmp    dword ptr [ecx + 3ch], 0
        je     SHORT loc_4_19B5
        call   sub_4_2257
        movzx  edx, byte ptr [esi + 2]
        db  08Dh, 03Dh, 0CFh, 021h, 000h, 000h
        call   sub_4_19B6
        or     dword ptr [esi + 3], eax
        db  08Dh, 03Dh, 0E9h, 021h, 000h, 000h
        call   sub_4_19B6
        or     dword ptr [esi + 3], eax
loc_4_19B5:
        ret
sub_4_19B6:
        xor    ecx, ecx
        xor    eax, eax
loc_4_19BA:
        cmp    byte ptr [ecx + edi], dl
        jne    SHORT loc_4_19C6
        cmp    ecx, edx
        je     SHORT loc_4_19C6
        bts    eax, ecx
loc_4_19C6:
        inc    ecx
        cmp    ecx, 1ah
        jb     SHORT loc_4_19BA
        ret
loc_4_19CD:
        sub    esp, 20h
        mov    edi, esp
        mov    word ptr [edi], 11h
        movzx  eax, word ptr [esi + 2]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 0ch], eax
        and    dword ptr [edi + 10h], 0
        db  0E8h, 06Dh, 0FDh, 0FBh, 0FFh
        mov    eax, dword ptr [edi + 14h]
        mov    word ptr [esi + 2], ax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 10h], eax
        add    esp, 20h
        ret
loc_4_1A0A:
        mov    dword ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 0ffh
        movzx  eax, byte ptr [esi + 2]
        mov    edx, eax
        or     eax, 40000000h
        db  0E8h, 068h, 0F8h, 0FBh, 0FFh
        jb     SHORT loc_4_1A50
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_1A33
        test   byte ptr [ebx + 21h], 80h
        je     SHORT loc_4_1A8E
loc_4_1A33:
        xor    eax, eax
        mov    ecx, ebx
        cmp    byte ptr [ebx + 40h], 0ah
        jne    SHORT loc_4_1A42
        bts    eax, edx
        jmp    SHORT loc_4_1A87
loc_4_1A42:
        mov    ecx, dword ptr [ecx + 10h]
        jecxz  loc_4_1A52
        movzx  ebx, byte ptr [ecx + 14h]
        bts    eax, ebx
        jmp    SHORT loc_4_1A42
loc_4_1A50:
        xor    eax, eax
loc_4_1A52:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    ecx, dword ptr [ecx + 3ch]
        jecxz  loc_4_1A87
loc_4_1A5D:
        mov    ecx, dword ptr [ecx + 10h]
        jecxz  loc_4_1A87
        movzx  ebx, byte ptr [ecx + 14h]
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        mov    edi, dword ptr [edi + 34h]
loc_4_1A6F:
        cmp    byte ptr [edi], 0ffh
        je     SHORT loc_4_1A5D
        cmp    byte ptr [edi], bl
        je     SHORT loc_4_1A7D
        add    edi, 12h
        jmp    SHORT loc_4_1A6F
loc_4_1A7D:
        cmp    dl, byte ptr [edi + 1]
        jne    SHORT loc_4_1A5D
        bts    eax, ebx
        jmp    SHORT loc_4_1A5D
loc_4_1A87:
        mov    dword ptr [esi + 3], eax
        mov    byte ptr [esi + 1], 0
loc_4_1A8E:
        ret
loc_4_1A8F:
        mov    eax, dword ptr [esi + 2]
        mov    byte ptr [esi + 1], 0ffh
        sub    esp, 10h
        mov    edx, esp
        mov    word ptr [edx], 8
        mov    byte ptr [edx + 0ch], 0ffh
        mov    dword ptr [edx + 4], eax
        push   edx
loc_4_1AA8:
        db  0E8h, 063h, 0F8h, 0FBh, 0FFh
        cmp    word ptr [edx + 2], 0
        jne    SHORT loc_4_1AE8
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [edx + 4], ebx
        test   byte ptr [ebx + 20h], 80h
        je     SHORT loc_4_1AA8
        test   byte ptr [ebx + 21h], 80h
        je     SHORT loc_4_1AA8
        mov    edx, esi
        mov    edi, dword ptr [esi + 6]
        mov    esi, dword ptr [ebx + 0cfh]
        mov    ecx, 0bh
loc_4_1AD6:
        lodsb
        or     al, al
        je     SHORT loc_4_1ADE
        stosb
        loop   loc_4_1AD6
loc_4_1ADE:
        mov    byte ptr [edi], 0
        mov    byte ptr [edx + 1], 0
        mov    dword ptr [edx + 2], ebx
loc_4_1AE8:
        add    esp, 14h
        ret
loc_4_1AEC:
        mov    byte ptr [esi + 1], 0ffh
        mov    byte ptr [esi + 3], 0
        movzx  eax, byte ptr [esi + 2]
        db  0E8h, 090h, 0F7h, 0FBh, 0FFh
        jb     SHORT loc_4_1B38
        test   byte ptr [ebx + 21h], 40h
        je     SHORT loc_4_1B38
        call   sub_4_2257
        movzx  eax, byte ptr [esi + 2]
        db  00Fh, 0B6h, 088h, 0CFh, 021h, 000h, 000h
        cmp    ecx, 0ffh
        je     SHORT loc_4_1B39
        cmp    ecx, eax
        je     SHORT loc_4_1B39
loc_4_1B21:
        mov    dl, 1
        db  00Fh, 0A3h, 005h, 0CBh, 021h, 000h, 000h
        jae    SHORT loc_4_1B2E
        mov    dl, 2
loc_4_1B2E:
        mov    byte ptr [esi + 2], cl
        mov    byte ptr [esi + 3], dl
loc_4_1B34:
        mov    byte ptr [esi + 1], 0
loc_4_1B38:
        ret
loc_4_1B39:
        db  00Fh, 0B6h, 088h, 0E9h, 021h, 000h, 000h
        cmp    ecx, 0ffh
        je     SHORT loc_4_1B4E
        cmp    ecx, eax
        je     SHORT loc_4_1B4E
        jmp    SHORT loc_4_1B21
loc_4_1B4E:
        test   byte ptr [ebx + 21h], 2
        je     SHORT loc_4_1B34
        mov    cl, byte ptr [ebx + 47h]
        jmp    SHORT loc_4_1B21
        db  000h, 000h, 000h
sub_4_1B5C:
        mov    ds, word ptr [ebx]
        add    byte ptr [eax], al
        popad
        add    byte ptr [eax], al
        db  000h
IOS_V86_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 1
        ja     SHORT loc_4_1B83
        db  0FFh, 014h, 085h, 05Ch, 01Bh, 000h, 000h
        jb     SHORT loc_4_1B83
        mov    word ptr [ebp + 1ch], 0
        and    dword ptr [ebp + 2ch], 0fffffffeh
        ret
loc_4_1B83:
        mov    word ptr [ebp + 1ch], 0ffffh
        or     dword ptr [ebp + 2ch], 1
        ret
loc_4_1B8E:
        movzx  eax, word ptr [ebp + 10h]
        cmp    word ptr [ebp + 18h], 1
        jne    SHORT loc_4_1B9E
        or     eax, 40000000h
loc_4_1B9E:
        db  0E8h, 0EAh, 0F6h, 0FBh, 0FFh
        jb     SHORT loc_4_1BBD
        mov    ax, word ptr [ebx + 16h]
        and    ax, 40h
        mov    ecx, dword ptr [ebx + 20h]
        and    ecx, 2000h
        or     ax, cx
        mov    word ptr [ebp + 14h], ax
loc_4_1BBD:
        ret
        db  000h, 000h
sub_4_1BC0:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    esi, dword ptr [ecx + 34h]
loc_4_1BC9:
        cmp    byte ptr [esi], 0ffh
        je     SHORT loc_4_1BD7
        cmp    byte ptr [esi], al
        je     SHORT loc_4_1BD8
        add    esi, 12h
        jmp    SHORT loc_4_1BC9
loc_4_1BD7:
        stc
loc_4_1BD8:
        ret
loc_4_1BD9:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    word ptr [eax + 3], 800ch
        db  0CDh, 020h, 04Ah, 080h, 001h, 000h
        db  000h
sub_4_1BF0:
        call   sub_4_2145
        db  083h, 03Dh, 020h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_1C40
        db  08Bh, 015h, 028h, 003h, 000h, 000h
        db  08Bh, 00Dh, 024h, 003h, 000h, 000h
        db  080h, 03Dh, 01Bh, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_1C40
        or     bx, 0ffffh
        mov    edx, dword ptr [edx + 0bh]
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        xor    eax, eax
        db  087h, 005h, 020h, 003h, 000h, 000h
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     SHORT loc_4_1C34
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_1C34:
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_1C40:
        ret
sub_4_1C41:
        db  08Bh, 01Dh, 01Ch, 003h, 000h, 000h
        mov    edx, 13h
loc_4_1C4C:
        dec    edx
        js     SHORT loc_4_1C80
        mov    edi, dword ptr [ebx + edx*4]
loc_4_1C52:
        or     edi, edi
        je     SHORT loc_4_1C4C
        cmp    byte ptr [edi + 0ah], 1
        je     SHORT loc_4_1C7B
        push   edx
        mov    edx, dword ptr [edi + 4]
        db  0CDh, 020h, 004h, 000h, 027h, 000h
        pop    edx
        mov    byte ptr [edi + 0ah], 1
        mov    esi, dword ptr [edi + 20h]
loc_4_1C6E:
        or     esi, esi
        je     SHORT loc_4_1C7B
        mov    byte ptr [edi + 0ah], 1
        mov    esi, dword ptr [esi + 20h]
        jmp    SHORT loc_4_1C6E
loc_4_1C7B:
        mov    edi, dword ptr [edi + 1ch]
        jmp    SHORT loc_4_1C52
loc_4_1C80:
        ret
loc_4_1C81:
        mov    eax, 21h
        db  080h, 03Dh, 063h, 005h, 000h, 000h, 000h
        je     SHORT loc_4_1CB3
        push   1
        db  068h, 060h, 029h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Fh, 000h, 033h, 000h
        add    esp, 0ch
        push   3
        push   0
        db  068h, 088h, 023h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
loc_4_1CB3:
        stc
        ret
sub_4_1CB5:
        push   1
        push   80h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2206
        db  0A3h, 01Ch, 003h, 000h, 000h
        db  08Bh, 03Dh, 0F9h, 001h, 000h, 000h
        db  0BEh, 006h, 003h, 000h, 000h
        call   sub_4_21E3
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 004h
        jne    SHORT loc_4_1D1B
        db  0BAh, 0FDh, 001h, 000h, 000h
        mov    esi, edx
        add    esi, 1eh
        mov    ah, 1ah
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_2221
        db  0BAh, 079h, 001h, 000h, 000h
        xor    cx, cx
        mov    ah, 4eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_1D84
        jmp    SHORT loc_4_1D51
loc_4_1D1B:
        db  0BEh, 079h, 001h, 000h, 000h
        xor    ecx, ecx
        db  0BAh, 068h, 005h, 000h, 000h
        mov    eax, 4e00h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        jb     SHORT loc_4_1D84
        db  0A3h, 064h, 005h, 000h, 000h
loc_4_1D39:
        db  0BAh, 068h, 005h, 000h, 000h
        lea    esi, [edx + 130h]
        cmp    byte ptr [esi], 0
        jne    SHORT loc_4_1D51
        lea    esi, [edx + 2ch]
        cmp    byte ptr [esi], 0
        je     SHORT loc_4_1D84
loc_4_1D51:
        push   esi
        call   sub_4_21EF
        call   sub_4_2198
        jb     SHORT loc_4_1D6E
        db  08Bh, 03Dh, 0F9h, 001h, 000h, 000h
        call   sub_4_21E3
        call   sub_4_1DA7
loc_4_1D6E:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 004h
        jne    SHORT loc_4_1D8C
        mov    ah, 4fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    esi
        jae    SHORT loc_4_1D51
loc_4_1D84:
        db  0C6h, 005h, 053h, 001h, 000h, 000h, 001h
        ret
loc_4_1D8C:
        pop    esi
        db  08Bh, 01Dh, 064h, 005h, 000h, 000h
        db  0BAh, 068h, 005h, 000h, 000h
        mov    eax, 4f00h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        jb     SHORT loc_4_1D84
        jmp    SHORT loc_4_1D39
sub_4_1DA7:
        push   ebp
        push   0
        push   0
        db  0BAh, 079h, 001h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     NEAR PTR loc_4_1E5D
        or     edx, edx
        je     NEAR PTR loc_4_1E5D
        db  08Bh, 01Dh, 01Ch, 003h, 000h, 000h
        db  0C6h, 005h, 053h, 001h, 000h, 000h, 000h
        mov    esi, dword ptr [eax + 2ch]
        mov    ebp, dword ptr [esi + 8]
        mov    edi, ebp
        mov    ecx, 20h
loc_4_1DE1:
        rcr    edi, 1
        jae    SHORT loc_4_1DE9
        inc    dword ptr [esp + 4]
loc_4_1DE9:
        loop   loc_4_1DE1
        xor    edi, edi
loc_4_1DED:
        bsr    esi, ebp
        je     SHORT loc_4_1E55
        btr    ebp, esi
        db  0FEh, 005h, 053h, 001h, 000h, 000h
        push   eax
        push   edx
        push   1
        push   24h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2206
        mov    dword ptr [eax + 20h], edi
        mov    edi, eax
        pop    edx
        pop    eax
        cmp    esi, 13h
        jb     SHORT loc_4_1E21
        mov    dword ptr [esp], edi
loc_4_1E21:
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi], eax
        db  08Ah, 00Dh, 053h, 001h, 000h, 000h
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [esp + 4]
        mov    byte ptr [edi + 9], cl
        push   esi
        push   edi
        lea    edi, [edi + 0bh]
        db  08Bh, 035h, 0F9h, 001h, 000h, 000h
        call   sub_4_21E3
        pop    edi
        pop    esi
        mov    ecx, dword ptr [ebx + esi*4]
        jecxz  loc_4_1E63
        mov    dword ptr [ebx + esi*4], edi
        mov    dword ptr [edi + 1ch], ecx
        jmp    SHORT loc_4_1DED
loc_4_1E55:
        mov    edi, dword ptr [esp]
        add    esp, 8
loc_4_1E5B:
        pop    ebp
        ret
loc_4_1E5D:
        add    esp, 8
        stc
        jmp    SHORT loc_4_1E5B
loc_4_1E63:
        mov    dword ptr [ebx + esi*4], edi
        jmp    SHORT loc_4_1DED
sub_4_1E68:
        push   ebp
        db  08Bh, 01Dh, 01Ch, 003h, 000h, 000h
        mov    edx, 13h
loc_4_1E74:
        dec    edx
        js     NEAR PTR loc_4_1F0B
        db  08Bh, 03Dh, 0A4h, 01Fh, 000h, 000h
loc_4_1E81:
        cmp    byte ptr [edi + 44h], dl
        jne    SHORT loc_4_1E91
        test   byte ptr [edi + 3ah], 2
        jne    SHORT loc_4_1E91
        call   sub_4_2103
loc_4_1E91:
        mov    edi, dword ptr [edi + 2]
        or     edi, edi
        jne    SHORT loc_4_1E81
        mov    edi, dword ptr [ebx + edx*4]
loc_4_1E9B:
        or     edi, edi
        je     SHORT loc_4_1E74
        mov    ecx, dword ptr [edi + 18h]
        or     ecx, ecx
        jne    SHORT loc_4_1F06
        cmp    byte ptr [edi + 0ah], 0
        jne    SHORT loc_4_1F06
        db  089h, 03Dh, 06Ch, 003h, 000h, 000h
        push   edx
        push   edi
        push   ebx
        mov    ebp, dword ptr [edi]
        mov    al, byte ptr [edi + 8]
        db  0A2h, 053h, 001h, 000h, 000h
        mov    eax, 1bh
        call   dword ptr [ebp + 18h]
        pop    ebx
        pop    edi
        db  0C7h, 005h, 06Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edi + 4]
        rcl    byte ptr [edi + 0ah]
        mov    cl, byte ptr [edi + 9]
        cmp    cl, byte ptr [edi + 8]
        jne    SHORT loc_4_1F05
        mov    esi, edi
        movzx  ecx, byte ptr [edi + 8]
loc_4_1EE7:
        cmp    byte ptr [esi + 0ah], 0
        je     SHORT loc_4_1EFA
        mov    esi, dword ptr [esi + 20h]
        loop   loc_4_1EE7
        db  0CDh, 020h, 004h, 000h, 027h, 000h
        jmp    SHORT loc_4_1F05
loc_4_1EFA:
        or     word ptr [ebp + 0ah], 2
        db  0CDh, 020h, 003h, 000h, 027h, 000h
loc_4_1F05:
        pop    edx
loc_4_1F06:
        mov    edi, dword ptr [edi + 1ch]
        jmp    SHORT loc_4_1E9B
loc_4_1F0B:
        pop    ebp
        db  0C6h, 005h, 053h, 001h, 000h, 000h, 001h
        ret
        db  0F6h, 041h, 03Ah, 002h, 075h, 0ECh, 087h, 0CFh, 0E8h, 0E2h, 001h, 000h, 000h, 08Bh, 0F9h, 0EBh
        db  0E1h
sub_4_1F25:
        call   sub_4_20EE
        mov    ecx, 9
        mov    edi, esi
        mov    al, 2eh
        repne scasb
        cmp    dword ptr [edi - 1], 64706d2eh
        je     NEAR PTR loc_4_1FDB
        call   sub_4_1DA7
        jb     NEAR PTR loc_4_2099
        or     edi, edi
        je     NEAR PTR loc_4_2099
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    esi, dword ptr [ecx + 2ch]
        mov    ebx, dword ptr [esi + 8]
        bt     ebx, 1eh
        jb     SHORT loc_4_1F8F
        call   sub_4_209E
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        jb     SHORT loc_4_1F8F
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        db  0F6h, 005h, 044h, 022h, 000h, 000h, 004h
        jne    SHORT loc_4_1F86
        bt     ebx, 1bh
        jb     SHORT loc_4_1F8F
loc_4_1F86:
        test   dword ptr [eax + 28h], 2
        jne    SHORT loc_4_1FD0
loc_4_1F8F:
        db  089h, 03Dh, 06Ch, 003h, 000h, 000h
        push   edx
        push   ecx
        mov    eax, 1bh
        call   dword ptr [ecx + 18h]
        pop    ecx
        pop    edx
        jb     SHORT loc_4_1FD0
        mov    ebx, dword ptr [ecx + 2ch]
        mov    ebx, dword ptr [ebx + 8]
        bt     ebx, 15h
        jae    SHORT loc_4_1FC0
        db  0A3h, 020h, 003h, 000h, 000h
        db  089h, 015h, 028h, 003h, 000h, 000h
        db  089h, 00Dh, 024h, 003h, 000h, 000h
loc_4_1FC0:
        or     word ptr [ecx + 0ah], 2
        db  0CDh, 020h, 003h, 000h, 027h, 000h
        jmp    NEAR PTR loc_4_207B
loc_4_1FD0:
        db  0CDh, 020h, 004h, 000h, 027h, 000h
        jmp    NEAR PTR loc_4_2099
loc_4_1FDB:
        db  083h, 03Dh, 020h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_4_2099
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   dword ptr [eax + 28h], 2
        je     SHORT loc_4_2001
        call   sub_4_209E
        jae    NEAR PTR loc_4_2099
loc_4_2001:
        push   esi
        push   edi
        db  0BFh, 048h, 003h, 000h, 000h
        db  089h, 03Dh, 06Ch, 003h, 000h, 000h
        lea    edi, [edi + 0bh]
        call   sub_4_21E3
        sub    esp, 4
        mov    edx, esp
        db  08Dh, 00Dh, 079h, 001h, 000h, 000h
        db  0A1h, 020h, 003h, 000h, 000h
        lea    eax, [eax]
        push   1
        push   eax
        push   ecx
        push   edx
        db  0CDh, 020h, 011h, 000h, 027h, 000h
        add    esp, 10h
        db  08Fh, 005h, 04Ch, 003h, 000h, 000h
        pop    edi
        pop    esi
        or     eax, eax
        jne    SHORT loc_4_2099
        mov    byte ptr [edi - 1], 0
        db  08Bh, 03Dh, 04Ch, 003h, 000h, 000h
        push   edi
        db  0CDh, 020h, 00Ah, 000h, 027h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_2087
        push   eax
        push   esi
        db  0A1h, 020h, 003h, 000h, 000h
        call   dword ptr [eax + 4]
        add    esp, 4
        pop    ebx
        or     eax, eax
        jne    SHORT loc_4_2087
        db  08Bh, 00Dh, 045h, 001h, 000h, 000h
        mov    dword ptr [ecx + 4bh], ebx
        db  0FEh, 005h, 01Bh, 003h, 000h, 000h
loc_4_207B:
        clc
loc_4_207C:
        db  0C7h, 005h, 06Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
loc_4_2087:
        db  0A1h, 020h, 003h, 000h, 000h
        lea    eax, [eax]
        push   eax
        push   edi
        db  0CDh, 020h, 00Eh, 000h, 027h, 000h
        add    esp, 8
loc_4_2099:
        xor    eax, eax
        stc
        jmp    SHORT loc_4_207C
sub_4_209E:
        db  0A1h, 044h, 003h, 000h, 000h
        db  0C7h, 005h, 03Fh, 001h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C6h, 005h, 044h, 001h, 000h, 000h, 000h
        push   1
        db  068h, 03Fh, 001h, 000h, 000h
        db  068h, 043h, 001h, 000h, 000h
        push   3
        db  068h, 0B2h, 000h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_4_20E2
        db  080h, 03Dh, 043h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_20E2
loc_4_20E0:
        stc
        ret
loc_4_20E2:
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 2ah], 8
        jne    SHORT loc_4_20E0
        ret
sub_4_20EE:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    ecx, dword ptr [ecx + 30h]
loc_4_20F7:
        jecxz  loc_4_2102
        and    byte ptr [ecx + 21h], 0fbh
        mov    ecx, dword ptr [ecx + 0ch]
        jmp    SHORT loc_4_20F7
loc_4_2102:
        ret
sub_4_2103:
        pushad
        sub    esp, 10h
        mov    esi, esp
        mov    cl, byte ptr [edi + 44h]
        mov    byte ptr [esi + 8], cl
        mov    word ptr [esi], 3
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 4], eax
        mov    ebx, edi
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        mov    edi, dword ptr [edi + 30h]
loc_4_2125:
        or     edi, edi
        je     SHORT loc_4_2140
        test   byte ptr [edi + 21h], 4
        je     SHORT loc_4_213B
        mov    dword ptr [esi + 0ch], edi
        push   ebx
        push   edi
        push   esi
        call   dword ptr [ebx + 8]
        pop    esi
        pop    edi
        pop    ebx
loc_4_213B:
        mov    edi, dword ptr [edi + 0ch]
        jmp    SHORT loc_4_2125
loc_4_2140:
        add    esp, 10h
        popad
        ret
sub_4_2145:
        db  08Bh, 01Dh, 01Ch, 003h, 000h, 000h
        mov    edi, 20h
loc_4_2150:
        dec    edi
        js     SHORT loc_4_216D
        mov    ecx, dword ptr [ebx + edi*4]
loc_4_2156:
        jecxz  loc_4_2150
        mov    esi, dword ptr [ecx + 1ch]
        push   0
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        mov    ecx, esi
        jmp    SHORT loc_4_2156
loc_4_216D:
        push   0
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        ret
loc_4_217C:
        call   sub_4_1CB5
        call   sub_4_20EE
        call   sub_4_1E68
        call   sub_4_1BF0
        call   sub_4_14EC
        clc
        ret
        db  000h
sub_4_2198:
        push   ebx
        push   edx
        mov    ebx, esi
        db  08Bh, 03Dh, 0A4h, 01Fh, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_21CB
        mov    edx, edi
        lea    edi, [edi + 14h]
        cld
loc_4_21AC:
        call   sub_4_21D0
        je     SHORT loc_4_21C5
        mov    edi, edx
        mov    edi, dword ptr [edi + 2]
        or     edi, edi
        je     SHORT loc_4_21CA
        mov    edx, edi
        lea    edi, [edi + 14h]
        mov    esi, ebx
        jmp    SHORT loc_4_21AC
loc_4_21C5:
        stc
        mov    edi, edx
        jmp    SHORT loc_4_21CB
loc_4_21CA:
        clc
loc_4_21CB:
        mov    esi, ebx
        pop    edx
        pop    ebx
        ret
sub_4_21D0:
        push   esi
        push   edi
        push   eax
loc_4_21D3:
        lodsb
        or     al, al
        je     SHORT loc_4_21DF
        cmp    al, byte ptr [edi]
        jne    SHORT loc_4_21DF
        inc    edi
        jmp    SHORT loc_4_21D3
loc_4_21DF:
        pop    eax
        pop    edi
        pop    esi
        ret
sub_4_21E3:
        cld
        push   esi
        push   eax
loc_4_21E6:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_4_21E6
        pop    eax
        pop    esi
        ret
sub_4_21EF:
        push   esi
loc_4_21F0:
        lodsb
        or     al, al
        je     SHORT loc_4_2204
        cmp    al, 41h
        jb     SHORT loc_4_21F0
        cmp    al, 5ah
        ja     SHORT loc_4_21F0
        add    al, 20h
        mov    byte ptr [esi - 1], al
        jmp    SHORT loc_4_21F0
loc_4_2204:
        pop    esi
        ret
loc_4_2206:
        db  00Fh, 0B7h, 035h, 010h, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        pushad
        mov    esi, esi
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_2221:
        db  00Fh, 0B7h, 035h, 00Ch, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        pushad
        mov    esi, esi
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_223C:
        db  00Fh, 0B7h, 035h, 00Eh, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        pushad
        mov    esi, esi
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
sub_4_2257:
        pushad
        db  0BFh, 0CFh, 021h, 000h, 000h
        mov    al, 0ffh
        mov    ecx, 1ah
        cld
        rep stosb
        xor    edi, edi
        xor    esi, esi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    NEAR PTR loc_4_235A
        cmp    word ptr [ebp + 10h], 444dh
        jne    NEAR PTR loc_4_235A
        movzx  ecx, byte ptr [ebp + 18h]
        sub    cl, 41h
        add    cl, byte ptr [ebp + 19h]
        xor    ebx, ebx
loc_4_22C3:
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 1
        mov    byte ptr [ebp + 14h], bl
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_4_2350
        mov    al, byte ptr [ebp + 10h]
        test   al, 80h
        je     SHORT loc_4_2350
        and    al, 7fh
        db  088h, 083h, 0CFh, 021h, 000h, 000h
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 1
        mov    byte ptr [ebp + 14h], al
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_4_2350
        mov    al, byte ptr [ebp + 10h]
        and    al, 7fh
        cmp    al, bl
        je     SHORT loc_4_2323
        db  088h, 083h, 0CFh, 021h, 000h, 000h
loc_4_2323:
        bts    edi, ebx
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 0fh
        mov    byte ptr [ebp + 14h], bl
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_4_2350
        test   byte ptr [ebp + 10h], 80h
        je     SHORT loc_4_2350
        bts    esi, ebx
loc_4_2350:
        inc    ebx
        jecxz  loc_4_235A
        dec    ecx
        jne    NEAR PTR loc_4_22C3
loc_4_235A:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  089h, 03Dh, 0C7h, 021h, 000h, 000h
        db  089h, 035h, 0CBh, 021h, 000h, 000h
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [edx + 64h], edi
        popad
        ret
        db  000h, 000h
loc_4_2388:
        sub    esp, 10h
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [esp + 28h]
        db  089h, 01Dh, 044h, 003h, 000h, 000h
        mov    edi, dword ptr [esp + 20h]
        cmp    edi, 1
        jne    NEAR PTR loc_4_266D
        lea    esi, [esp + 14h]
        lea    ecx, [esp + 10h]
        push   0
        push   ebx
        push   ecx
        push   esi
        db  0CDh, 020h, 036h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        mov    eax, 13h
        jne    NEAR PTR loc_4_26B1
        test   byte ptr [esi + 2], 2
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        jne    SHORT loc_4_2411
        lea    eax, [esp + 14h]
        push   4
        push   ebx
        push   eax
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        cmp    eax, 0
        je     SHORT loc_4_2411
        lea    eax, [esp + 14h]
        push   3
        push   ebx
        push   eax
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [ecx + 2ah], 8
        cmp    eax, 0
        jne    SHORT loc_4_2438
loc_4_2411:
        mov    eax, 13h
        and    byte ptr [ecx + 2ah], 0f3h
        and    byte ptr [ecx + 2ah], 0fdh
        test   byte ptr [ecx + 28h], 4
        jne    NEAR PTR loc_4_26B1
        mov    eax, 22h
        call   sub_4_27FC
        jb     NEAR PTR loc_4_26B1
loc_4_2438:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 004h
        je     SHORT loc_4_2448
        pushad
        call   sub_4_1CB5
        popad
loc_4_2448:
        lea    edx, [esp + 0ch]
        mov    dword ptr [edx], 0dh
        db  08Dh, 035h, 0C8h, 006h, 000h, 000h
        push   1
        push   edx
        push   esi
        push   1
        db  068h, 0B5h, 006h, 000h, 000h
        push   0
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        or     eax, eax
        mov    eax, 13h
        jne    NEAR PTR loc_4_26B1
        call   sub_4_21EF
        db  08Bh, 03Dh, 0F9h, 001h, 000h, 000h
        call   sub_4_21E3
        push   esi
        db  08Dh, 035h, 054h, 001h, 000h, 000h
        call   sub_4_2FF4
        pop    esi
        call   sub_4_2198
        jb     SHORT loc_4_251D
        mov    ecx, 9
        mov    edi, esi
        mov    al, 2eh
        repne scasb
        cmp    dword ptr [edi - 1], 64706d2eh
        jne    SHORT loc_4_2510
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [ecx + 2ah], 80h
        je     SHORT loc_4_24DF
        cmp    dword ptr [edi - 8], 72617073h
        jne    SHORT loc_4_24DF
        cmp    dword ptr [edi - 4], 2e776f72h
        jne    SHORT loc_4_24DF
        mov    dword ptr [esp + 18h], 0
        jmp    NEAR PTR loc_4_259C
loc_4_24DF:
        db  083h, 03Dh, 020h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_2510
        push   esi
        db  0BEh, 0A8h, 006h, 000h, 000h
        db  08Bh, 03Dh, 0F9h, 001h, 000h, 000h
        call   sub_4_21E3
        call   sub_4_1F25
        pop    esi
        jb     NEAR PTR loc_4_264C
        db  08Bh, 03Dh, 0F9h, 001h, 000h, 000h
        call   sub_4_21E3
loc_4_2510:
        call   sub_4_1F25
        jb     NEAR PTR loc_4_264C
        jmp    SHORT loc_4_2558
loc_4_251D:
        call   sub_4_20EE
        db  089h, 03Dh, 045h, 001h, 000h, 000h
        xchg   edi, esi
        lea    edi, [esi + 14h]
        mov    ecx, 9
        mov    al, 2eh
        repne scasb
        cmp    dword ptr [edi - 1], 64706d2eh
        jne    SHORT loc_4_2553
        push   dword ptr [esi + 4bh]
        lea    eax, [esi + 14h]
        push   eax
        db  0A1h, 020h, 003h, 000h, 000h
        call   dword ptr [eax + 4]
        add    esp, 8
        jmp    SHORT loc_4_2558
loc_4_2553:
        call   sub_4_0383
loc_4_2558:
        call   sub_4_1E68
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 004h
        jne    NEAR PTR loc_4_265E
        db  0E8h, 04Ah, 00Fh, 0FCh, 0FFh
loc_4_256F:
        lea    eax, [esp + 18h]
        mov    ebx, dword ptr [esp + 28h]
        push   0
        push   ebx
        push   ebx
        push   eax
        db  0CDh, 020h, 009h, 000h, 033h, 000h
        add    esp, 10h
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_4_259C
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        btr    dword ptr [eax + 28h], 11h
        jae    NEAR PTR loc_4_2629
loc_4_259C:
        db  08Dh, 035h, 05Dh, 001h, 000h, 000h
        call   sub_4_2FF4
        mov    eax, 0
        mov    edi, dword ptr [esp + 18h]
        or     edi, edi
        je     NEAR PTR loc_4_26B1
        call   sub_4_2C57
        lea    edx, [esp + 0ch]
        mov    dword ptr [edx], 4
        db  08Dh, 035h, 0C8h, 006h, 000h, 000h
        push   1
        push   edx
        push   esi
        push   1
        db  068h, 0C0h, 006h, 000h, 000h
        push   0
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        or     eax, eax
        mov    eax, 0
        jne    NEAR PTR loc_4_26B1
        call   sub_4_21EF
        cmp    word ptr [esi], 6579h
        jne    SHORT loc_4_2606
        cmp    byte ptr [esi], 73h
        je     NEAR PTR loc_4_26B1
loc_4_2606:
        cmp    word ptr [esi], 6f6eh
        jne    NEAR PTR loc_4_26B1
        mov    esi, dword ptr [esp + 18h]
        mov    esi, dword ptr [esi + 0ch]
loc_4_2618:
        or     esi, esi
        je     NEAR PTR loc_4_26B1
        or     byte ptr [esi + 23h], 8
        mov    esi, dword ptr [esi + 6ch]
        jmp    SHORT loc_4_2618
loc_4_2629:
        db  08Dh, 035h, 06Bh, 001h, 000h, 000h
        call   sub_4_2FF4
        mov    eax, 13h
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [ecx + 2ah], 4
        je     SHORT loc_4_26B1
        mov    eax, 2bh
        jmp    SHORT loc_4_26B1
loc_4_264C:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 004h
        je     NEAR PTR loc_4_256F
        call   sub_4_1C41
loc_4_265E:
        call   sub_4_1BF0
        call   sub_4_14EC
        jmp    NEAR PTR loc_4_256F
loc_4_266D:
        lea    eax, [esp + 18h]
        push   0
        push   ebx
        push   ebx
        push   eax
        db  0CDh, 020h, 009h, 000h, 033h, 000h
        add    esp, 10h
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        je     SHORT loc_4_26AC
        cmp    edi, 2
        je     NEAR PTR loc_4_275F
        cmp    edi, 4
        je     NEAR PTR loc_4_275F
        cmp    edi, 9
        je     NEAR PTR loc_4_27DA
        cmp    edi, 0ah
        je     SHORT loc_4_26B8
        cmp    edi, 3
        je     SHORT loc_4_26D2
loc_4_26AC:
        mov    eax, 1
loc_4_26B1:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 10h
        ret
loc_4_26B8:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, 8003h
loc_4_26C0:
        or     eax, eax
        je     NEAR PTR loc_4_274A
        call   sub_4_288C
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_26C0
loc_4_26D2:
        mov    eax, dword ptr [esi + 0ch]
loc_4_26D5:
        or     eax, eax
        je     SHORT loc_4_2721
        cmp    byte ptr [eax + 40h], 0ah
        je     SHORT loc_4_2754
        push   eax
        push   esi
        db  0CDh, 020h, 007h, 000h, 021h, 000h
        movzx  ecx, byte ptr [esi]
        pop    esi
        pop    eax
        jecxz  loc_4_26FD
        push   eax
        mov    eax, ecx
        sub    al, 41h
        db  0E8h, 095h, 0EBh, 0FBh, 0FFh
        pop    eax
        cmp    eax, dword ptr [ebx]
        je     SHORT loc_4_2754
loc_4_26FD:
        push   eax
        db  00Fh, 0B6h, 005h, 079h, 001h, 000h, 000h
        or     al, 20h
        sub    al, 61h
        db  0E8h, 07Fh, 0EBh, 0FBh, 0FFh
        pop    eax
        jb     SHORT loc_4_2715
        cmp    eax, dword ptr [ebx]
        je     SHORT loc_4_2754
loc_4_2715:
        call   sub_4_2819
        jb     SHORT loc_4_2754
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_26D5
loc_4_2721:
        cli
        mov    eax, dword ptr [esi + 0ch]
loc_4_2725:
        or     eax, eax
        je     SHORT loc_4_2735
        db  0E8h, 02Eh, 0F7h, 0FBh, 0FFh
        jb     SHORT loc_4_2754
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_2725
loc_4_2735:
        mov    eax, dword ptr [esi + 0ch]
loc_4_2738:
        or     eax, eax
        je     SHORT loc_4_2745
        or     byte ptr [eax + 23h], 4
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_2738
loc_4_2745:
        or     byte ptr [esi + 11h], 1
        sti
loc_4_274A:
        mov    eax, 0
        jmp    NEAR PTR loc_4_26B1
loc_4_2754:
        sti
        mov    eax, 17h
        jmp    NEAR PTR loc_4_26B1
loc_4_275F:
        push   dword ptr [esi + 14h]
        mov    ecx, esi
        call   sub_4_0E2E
        pop    esi
        dec    byte ptr [esi + 4fh]
        jne    SHORT loc_4_27D0
        cmp    byte ptr [esi + 44h], 14h
        jne    SHORT loc_4_27D0
        mov    edi, dword ptr [esi + 45h]
        call   IOS_Service_17
        db  0A1h, 020h, 003h, 000h, 000h
        lea    eax, [eax]
        push   eax
        push   edi
        db  0CDh, 020h, 00Eh, 000h, 027h, 000h
        add    esp, 8
        db  08Bh, 015h, 028h, 003h, 000h, 000h
        db  08Bh, 00Dh, 024h, 003h, 000h, 000h
        db  0FEh, 00Dh, 01Bh, 003h, 000h, 000h
        jne    SHORT loc_4_27D0
        or     bx, 0ffffh
        mov    edx, dword ptr [edx + 0bh]
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        xor    eax, eax
        db  087h, 005h, 020h, 003h, 000h, 000h
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     SHORT loc_4_27C4
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_27C4:
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_27D0:
        call   sub_4_14EC
        jmp    NEAR PTR loc_4_274A
loc_4_27DA:
        and    byte ptr [esi + 11h], 0feh
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, 8002h
loc_4_27E6:
        or     eax, eax
        je     NEAR PTR loc_4_274A
        and    byte ptr [eax + 23h], 0fbh
        call   sub_4_288C
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_4_27E6
sub_4_27FC:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 004h
        je     SHORT loc_4_2818
        push   eax
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        mov    eax, dword ptr [eax + 3ch]
        or     eax, eax
        je     SHORT loc_4_2817
        call   sub_4_2819
loc_4_2817:
        pop    eax
loc_4_2818:
        ret
sub_4_2819:
        pushad
        mov    esi, eax
        mov    edi, eax
        xor    ebp, ebp
loc_4_2820:
        xor    edx, edx
        or     esi, esi
        je     SHORT loc_4_286B
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_2866
loc_4_282C:
        mov    eax, edx
        call   sub_4_28C1
        jb     SHORT loc_4_2866
        push   edi
        push   edx
        xor    edi, edi
        mov    edx, 1
        mov    eax, 8001h
        call   sub_4_28F9
        pop    edx
        pop    edi
        bts    ebp, ebx
        cmp    eax, 2ah
        je     SHORT loc_4_286E
        push   edx
        mov    eax, ebx
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 040h, 000h
        add    esp, 4
        pop    edx
        or     eax, eax
        jne    SHORT loc_4_286E
        jmp    SHORT loc_4_282C
loc_4_2866:
        mov    esi, dword ptr [esi + 10h]
        jmp    SHORT loc_4_2820
loc_4_286B:
        clc
loc_4_286C:
        popad
        ret
loc_4_286E:
        bsf    ebx, ebp
        je     SHORT loc_4_2889
        btr    ebp, ebx
        xor    edi, edi
        mov    edx, 0
        mov    eax, 8002h
        call   sub_4_28F9
        jmp    SHORT loc_4_286E
loc_4_2889:
        stc
        jmp    SHORT loc_4_286C
sub_4_288C:
        pushad
        mov    esi, eax
        mov    edi, eax
loc_4_2891:
        xor    edx, edx
        or     esi, esi
        je     SHORT loc_4_28BF
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_28BA
loc_4_289D:
        mov    eax, edx
        call   sub_4_28C1
        jb     SHORT loc_4_28BA
        push   edi
        push   edx
        xor    edi, edi
        mov    edx, 0
        mov    eax, ecx
        call   sub_4_28F9
        pop    edx
        pop    edi
        jmp    SHORT loc_4_289D
loc_4_28BA:
        mov    esi, dword ptr [esi + 10h]
        jmp    SHORT loc_4_2891
loc_4_28BF:
        popad
        ret
sub_4_28C1:
        push   edi
        push   ecx
        db  0BFh, 0ACh, 020h, 000h, 000h
        mov    ecx, 1ah
        or     eax, eax
        je     SHORT loc_4_28DC
        mov    eax, edx
        sub    eax, edi
        shr    eax, 2
        sub    ecx, eax
        mov    edi, edx
loc_4_28DC:
        jecxz  loc_4_28F6
        mov    eax, esi
        repne scasd
        jne    SHORT loc_4_28F6
        mov    edx, edi
        db  081h, 0EFh, 0ACh, 020h, 000h, 000h
        shr    edi, 2
        dec    edi
        mov    ebx, edi
        clc
loc_4_28F3:
        pop    ecx
        pop    edi
        ret
loc_4_28F6:
        stc
        jmp    SHORT loc_4_28F3
sub_4_28F9:
        sub    esp, 18h
        push   edx
        push   ecx
        lea    ecx, [esp + 8]
        mov    dword ptr [ecx], 12h
        mov    dword ptr [ecx + 4], 2
        mov    dword ptr [ecx + 0ch], 0
        bts    dword ptr [ecx + 0ch], ebx
        mov    word ptr [ecx + 10h], 0
        or     edi, edi
        je     SHORT loc_4_2928
        or     byte ptr [ecx + 10h], 1
loc_4_2928:
        push   edx
        push   ecx
        push   eax
        db  0CDh, 020h, 058h, 000h, 033h, 000h
        add    esp, 0ch
        pop    ecx
        pop    edx
        add    esp, 18h
        ret
sub_4_293A:
        pushad
        db  0CDh, 020h, 00Dh, 000h, 017h, 000h
        or     eax, eax
        je     SHORT loc_4_295E
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_295E
        mov    edx, 0
        mov    eax, 8004h
        movzx  ebx, byte ptr [esi + 14h]
        call   sub_4_28F9
loc_4_295E:
        popad
        ret
loc_4_2960:
        sub    esp, 1d8h
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [esp + 1e8h], 5
        mov    eax, 1
        jne    NEAR PTR loc_4_2AA5
        mov    eax, dword ptr [esp + 1f0h]
        db  0A3h, 044h, 003h, 000h, 000h
        lea    eax, [esp + 1e0h]
        push   0
        push   dword ptr [esp + 1f4h]
        push   dword ptr [esp + 1f8h]
        push   eax
        db  0CDh, 020h, 009h, 000h, 033h, 000h
        add    esp, 10h
        mov    esi, dword ptr [esp + 1e0h]
        or     esi, esi
        je     NEAR PTR loc_4_2A9B
        db  089h, 035h, 049h, 001h, 000h, 000h
        bts    word ptr [esi + 11h], 2
        jae    NEAR PTR loc_4_2A9B
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 28h], 1
        je     NEAR PTR loc_4_2A9B
        mov    ebx, dword ptr [esi + 14h]
        cmp    byte ptr [ebx + 3eh], 1
        jne    NEAR PTR loc_4_2A9B
        call   sub_4_1CB5
        call   sub_4_20EE
        push   1
        push   12bh
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        mov    word ptr [eax + 3eh], 4342h
        db  0A3h, 03Eh, 003h, 000h, 000h
        db  08Bh, 035h, 049h, 001h, 000h, 000h
        mov    ebx, dword ptr [esi + 14h]
        db  089h, 01Dh, 045h, 001h, 000h, 000h
        db  08Bh, 03Dh, 03Eh, 003h, 000h, 000h
        call   sub_4_09CB
        test   byte ptr [ebx + 38h], 40h
        je     SHORT loc_4_2A3D
        db  08Dh, 005h, 007h, 022h, 000h, 000h
        mov    dword ptr [edi + 8], eax
        mov    esi, edi
        call   sub_4_08A9
loc_4_2A3D:
        mov    edi, ebx
        db  08Bh, 00Dh, 049h, 001h, 000h, 000h
        movzx  ecx, byte ptr [ecx + 10h]
loc_4_2A49:
        push   ecx
        mov    dx, word ptr [ebx + 49h]
        db  0C6h, 005h, 04Dh, 001h, 000h, 000h, 000h
        test   byte ptr [ebx + 38h], 4
        je     SHORT loc_4_2A62
        db  0C6h, 005h, 04Dh, 001h, 000h, 000h, 001h
loc_4_2A62:
        push   ebx
        call   sub_4_0538
        pop    ebx
        pop    ecx
        inc    ch
        dec    cl
        jne    SHORT loc_4_2A49
        db  0A1h, 03Eh, 003h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        call   sub_4_1E68
        call   sub_4_1BF0
        call   sub_4_14EC
        db  08Bh, 03Dh, 049h, 001h, 000h, 000h
        call   sub_4_2C57
loc_4_2A9B:
        mov    eax, 0
        jmp    NEAR PTR loc_4_2C4D
loc_4_2AA5:
        cmp    dword ptr [esp + 1e8h], 6
        je     NEAR PTR loc_4_2B43
        cmp    dword ptr [esp + 1e8h], 0fh
        jne    NEAR PTR loc_4_2C4D
        lea    eax, [esp + 1e0h]
        push   0
        push   dword ptr [esp + 1f4h]
        push   dword ptr [esp + 1f8h]
        push   eax
        db  0CDh, 020h, 009h, 000h, 033h, 000h
        add    esp, 10h
        mov    esi, dword ptr [esp + 1e0h]
        or     esi, esi
        je     SHORT loc_4_2A9B
        mov    esi, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esp + 1f4h]
loc_4_2AF7:
        or     esi, esi
        je     SHORT loc_4_2A9B
        test   byte ptr [esi + 21h], 80h
        je     SHORT loc_4_2B06
        cmp    dword ptr [esi + 70h], ecx
        je     SHORT loc_4_2B0B
loc_4_2B06:
        mov    esi, dword ptr [esi + 6ch]
        jmp    SHORT loc_4_2AF7
loc_4_2B0B:
        lea    edi, [esp + 0e0h]
        call   sub_4_2E82
        db  08Dh, 005h, 055h, 000h, 000h, 000h
        lea    ecx, [esp + 0e0h]
        add    edx, 13h
        push   0
        push   edx
        push   ecx
        push   1
        push   eax
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        or     eax, eax
        jmp    NEAR PTR loc_4_2A9B
loc_4_2B43:
        mov    esi, dword ptr [esp + 1f4h]
        lea    ebx, [esp + 0dch]
        mov    byte ptr [esp + 0ddh], 0
        lea    ecx, [esp + 0d8h]
        mov    dword ptr [ecx], 1
        push   0
        push   ecx
        push   ebx
        push   3
        db  068h, 04Ah, 000h, 000h, 000h
        push   0
        push   esi
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        or     eax, eax
        jne    NEAR PTR loc_4_2C4D
        xor    ebx, ebx
        mov    bl, byte ptr [esp + 0dch]
        lea    eax, [esp + 0ch]
        push   0
        push   esi
        push   eax
        db  0CDh, 020h, 006h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_2C4D
        lea    edi, [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        push   0
        push   eax
        push   edi
        push   esi
        db  0CDh, 020h, 007h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_2C4D
        mov    al, 5ch
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, 1
loc_4_2BD0:
        repne scasb
        dec    edx
        js     SHORT loc_4_2BD9
        mov    esi, edi
        jmp    SHORT loc_4_2BD0
loc_4_2BD9:
        mov    byte ptr [edi - 1], 2ch
        mov    ecx, esi
        db  08Dh, 035h, 0FBh, 006h, 000h, 000h
        mov    esi, dword ptr [esi + ebx*4]
        call   sub_4_21E3
        sub    edi, ecx
        mov    esi, dword ptr [esp + 1f4h]
        push   0
        push   edi
        push   ecx
        push   1
        db  068h, 0EAh, 006h, 000h, 000h
        push   0
        push   esi
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        db  0BFh, 03Dh, 007h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_2C2A
        cmp    ebx, 0ah
        je     SHORT loc_4_2C2A
        db  0BFh, 047h, 007h, 000h, 000h
        cmp    ebx, 5
        je     SHORT loc_4_2C2A
        db  0BFh, 035h, 007h, 000h, 000h
loc_4_2C2A:
        mov    ebx, edi
        xor    ecx, ecx
        dec    ecx
        xor    al, al
        repne scasb
        neg    ecx
        dec    ecx
        push   0
        push   ecx
        push   ebx
        push   1
        db  068h, 0F5h, 006h, 000h, 000h
        push   0
        push   esi
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
loc_4_2C4D:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 1d8h
        ret
sub_4_2C57:
        sub    esp, 114h
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [edi + 0ch]
loc_4_2C63:
        or     esi, esi
        je     NEAR PTR loc_4_2E78
        cmp    dword ptr [esi + 70h], 0
        je     NEAR PTR loc_4_2E70
        mov    byte ptr [esp + 1dh], 0
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_4_2CD8
        db  08Dh, 015h, 010h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 7ch]
        call   sub_4_2EFA
        mov    byte ptr [esp + 1ch], al
        lea    ecx, [esp + 1ch]
        push   0
        push   2
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        db  08Dh, 015h, 01Dh, 000h, 000h, 000h
        mov    al, byte ptr [esi + 7dh]
        call   sub_4_2EFA
        mov    byte ptr [esp + 1ch], al
        lea    ecx, [esp + 1ch]
        push   0
        push   2
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
loc_4_2CD8:
        push   esi
        lea    esi, [esi + 8ah]
        lea    edi, [esp + 24h]
        mov    ecx, 1ch
        rep movsb
        pop    esi
        db  08Dh, 015h, 025h, 000h, 000h, 000h
        lea    ecx, [esp + 20h]
        add    ecx, 18h
        mov    byte ptr [ecx + 4], 0
        push   0
        push   5
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        db  08Dh, 015h, 033h, 000h, 000h, 000h
        lea    ecx, [esp + 20h]
        add    ecx, 8
        mov    byte ptr [ecx + 10h], 0
        push   0
        push   11h
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        db  08Dh, 015h, 03Dh, 000h, 000h, 000h
        lea    ecx, [esp + 20h]
        mov    byte ptr [ecx + 8], 0
        push   0
        push   9
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        db  08Dh, 015h, 04Ah, 000h, 000h, 000h
        mov    al, byte ptr [esi + 40h]
        mov    byte ptr [esp + 1ch], al
        lea    ecx, [esp + 1ch]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        mov    byte ptr [esp + 1ch], 0
        cmp    byte ptr [esi + 40h], 0ah
        jne    SHORT loc_4_2D94
        mov    byte ptr [esp + 1ch], 2
loc_4_2D94:
        test   byte ptr [esi + 20h], 1
        je     SHORT loc_4_2D9F
        mov    byte ptr [esp + 1ch], 1
loc_4_2D9F:
        cmp    byte ptr [esp + 1ch], 0
        je     SHORT loc_4_2DC6
        db  08Dh, 015h, 087h, 000h, 000h, 000h
        lea    ecx, [esp + 1ch]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
loc_4_2DC6:
        test   byte ptr [esi + 16h], 40h
        je     SHORT loc_4_2DF1
        mov    byte ptr [esp + 1ch], 1
        db  08Dh, 015h, 08Dh, 000h, 000h, 000h
        lea    ecx, [esp + 1ch]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
loc_4_2DF1:
        test   byte ptr [esi + 20h], 4
        je     SHORT loc_4_2E1C
        db  08Dh, 015h, 060h, 000h, 000h, 000h
        mov    byte ptr [esp + 1ch], 1
        lea    ecx, [esp + 1ch]
        push   0
        push   1
        push   ecx
        push   3
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
loc_4_2E1C:
        mov    dword ptr [esp + 18h], 0
        mov    ecx, esi
loc_4_2E26:
        or     ecx, ecx
        je     SHORT loc_4_2E46
        cmp    byte ptr [ecx + 46h], 0
        je     SHORT loc_4_2E41
        mov    al, byte ptr [ecx + 14h]
        add    al, 41h
        mov    edx, dword ptr [esp + 18h]
        mov    byte ptr [esp + edx + 20h], al
        inc    dword ptr [esp + 18h]
loc_4_2E41:
        mov    ecx, dword ptr [ecx + 10h]
        jmp    SHORT loc_4_2E26
loc_4_2E46:
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [esp + eax + 20h], 0
        db  08Dh, 015h, 06Ah, 000h, 000h, 000h
        lea    ecx, [esp + 20h]
        push   0
        push   eax
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        jmp    SHORT loc_4_2E70
loc_4_2E70:
        mov    esi, dword ptr [esi + 6ch]
        jmp    NEAR PTR loc_4_2C63
loc_4_2E78:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 114h
        ret
sub_4_2E82:
        push   esi
        lea    esi, [esi + 8ah]
        mov    ecx, 8
        rep movsb
        pop    esi
        dec    edi
        cmp    byte ptr [edi], 20h
        jne    SHORT loc_4_2EA7
        std
        mov    al, 20h
        mov    ecx, 8
        repe scasb
        cld
        add    edi, 3
        jmp    SHORT loc_4_2EB2
loc_4_2EA7:
        mov    ecx, 9
        inc    edi
        inc    edi
        mov    byte ptr [edi - 1], 20h
loc_4_2EB2:
        push   esi
        lea    esi, [esi + 92h]
        mov    edx, ecx
        mov    ecx, 10h
        rep movsb
        pop    esi
        mov    byte ptr [edi], 0
        ret
loc_4_2EC7:
        mov    esi, dword ptr [edi + 4]
        mov    edx, dword ptr [esi + 70h]
        or     edx, edx
        je     SHORT loc_4_2EF2
        lea    eax, [edi + 10h]
        push   0
        push   eax
        push   dword ptr [edi + 14h]
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 8]
        push   0
        push   edx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        cmp    eax, 0
        jne    SHORT loc_4_2EF2
loc_4_2EF1:
        ret
loc_4_2EF2:
        mov    word ptr [edi + 2], 0ffffh
        jmp    SHORT loc_4_2EF1
sub_4_2EFA:
        mov    cl, 30h
        cmp    al, 0ah
        jb     SHORT loc_4_2F02
        mov    cl, 37h
loc_4_2F02:
        add    al, cl
        ret
sub_4_2F05:
        sub    esp, 8
        push   esi
        lea    eax, [esp + 8]
        push   0
        push   dword ptr [esi + 70h]
        push   eax
        db  0CDh, 020h, 006h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_2FEC
        mov    eax, dword ptr [esp + 8]
        add    eax, 22h
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2206
        push   eax
        mov    ebx, esi
        db  08Dh, 035h, 0D8h, 006h, 000h, 000h
        cmp    byte ptr [ebx + 74h], 0
        jne    SHORT loc_4_2F56
        db  08Dh, 035h, 0DEh, 006h, 000h, 000h
        jmp    SHORT loc_4_2F62
loc_4_2F56:
        cmp    byte ptr [ebx + 74h], 2
        jne    SHORT loc_4_2F62
        db  08Dh, 035h, 0E4h, 006h, 000h, 000h
loc_4_2F62:
        mov    ecx, 5
        mov    edi, eax
        rep movsb
        lea    esi, [ebx + 8ah]
        mov    ecx, 19h
        rep movsb
        mov    al, 5ch
        stosb
        mov    eax, dword ptr [esp + 0ch]
        push   0
        push   eax
        push   edi
        push   dword ptr [ebx + 70h]
        db  0CDh, 020h, 007h, 000h, 033h, 000h
        add    esp, 10h
        pop    esi
        xor    ecx, ecx
        or     eax, eax
        jne    SHORT loc_4_2FEC
        lea    edi, [esi + 1fh]
        mov    al, 5ch
        mov    ecx, dword ptr [esp + 8]
        mov    edx, ecx
loc_4_2FA1:
        repne scasb
        jecxz  loc_4_2FAB
        mov    byte ptr [edi - 1], 26h
        jmp    SHORT loc_4_2FA1
loc_4_2FAB:
        lea    eax, [esi + 1fh]
        add    eax, edx
        movzx  ecx, byte ptr [ebx + 0bah]
        cmp    byte ptr [ebx + 74h], 1
        jne    SHORT loc_4_2FC1
        mov    cx, word ptr [ebx + 7ch]
loc_4_2FC1:
        add    cx, 3030h
        mov    word ptr [eax], cx
        lea    edi, [esp + 4]
        push   0
        push   dword ptr [ebx + 70h]
        push   esi
        push   edi
        db  0CDh, 020h, 00Bh, 000h, 033h, 000h
        add    esp, 10h
        xor    ecx, ecx
        or     eax, eax
        je     SHORT loc_4_2FE8
        cmp    eax, 10h
        jne    SHORT loc_4_2FEC
loc_4_2FE8:
        mov    ecx, dword ptr [esp + 4]
loc_4_2FEC:
        pop    esi
        mov    dword ptr [esi + 70h], ecx
        add    esp, 8
        ret
sub_4_2FF4:
        mov    eax, 1
        db  0CDh, 020h, 08Bh, 001h, 001h, 000h
        db  0CDh, 020h, 088h, 001h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_4_3068
        pushad
        db  08Dh, 03Dh, 0FDh, 001h, 000h, 000h
        mov    edx, edi
        call   sub_4_21E3
        dec    edi
        db  08Bh, 035h, 0F9h, 001h, 000h, 000h
        call   sub_4_21E3
        mov    dword ptr [edi - 1], 0a0dh
        inc    edi
        mov    ecx, edi
        sub    ecx, edx
        db  0CDh, 020h, 089h, 001h, 001h, 000h
        db  0CDh, 020h, 08Ah, 001h, 001h, 000h
        db  08Bh, 035h, 025h, 022h, 000h, 000h
        movzx  eax, byte ptr [esi + 69h]
        dec    eax
        db  0E8h, 042h, 0E2h, 0FBh, 0FFh
        jb     SHORT loc_4_3067
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_4_3067
        push   0
        push   eax
        db  0CDh, 020h, 04Ch, 000h, 040h, 000h
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_223C
loc_4_3067:
        popad
loc_4_3068:
        xor    eax, eax
        db  0CDh, 020h, 08Bh, 001h, 001h, 000h
        ret
        db  000h, 000h, 000h
loc_4_3074:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 7
        jne    SHORT loc_4_3082
        call   sub_4_308D
loc_4_3082:
        xor    eax, eax
        leave
        ret
sub_4_3086:
        db  0C6h, 005h, 008h, 000h, 000h, 000h, 0EEh
sub_4_308D:
        push   ebx
        mov    bl, 1
        db  086h, 01Dh, 008h, 000h, 000h, 000h
        db  080h, 03Dh, 00Eh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_30B3
        db  068h, 008h, 000h, 000h, 000h
        push   1
        db  0CDh, 020h, 007h, 000h, 026h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_3101
loc_4_30B3:
        db  0A0h, 008h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    al, 0
        je     SHORT loc_4_30CC
        cmp    al, 2
        je     SHORT loc_4_30CC
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
loc_4_30CC:
        cmp    al, bl
        je     SHORT loc_4_3101
        shr    ecx, 2
        db  089h, 00Dh, 020h, 022h, 000h, 000h
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        mov    edx, dword ptr [edx + 30h]
loc_4_30E2:
        or     edx, edx
        je     SHORT loc_4_3101
        test   byte ptr [edx + 21h], 80h
        je     SHORT loc_4_30FC
        test   byte ptr [edx + 23h], 80h
        je     SHORT loc_4_30FC
        mov    dword ptr [edx + 0bfh], 0
loc_4_30FC:
        mov    edx, dword ptr [edx + 0ch]
        jmp    SHORT loc_4_30E2
loc_4_3101:
        pop    ebx
        ret
loc_4_3103:
        jecxz  loc_4_3127
        dec    ecx
        cmp    ecx, 1
        mov    eax, 1
        ja     SHORT loc_4_311F
        db  0FFh, 014h, 08Dh, 050h, 007h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_311F
        ret
loc_4_311F:
        mov    dword ptr [esi + 20h], 0
        ret
loc_4_3127:
        mov    ecx, 30ah
        xor    eax, eax
        ret
loc_4_312F:
        cmp    dword ptr [esi + 14h], 4
        mov    eax, 0dh
        jb     SHORT loc_4_3172
        mov    eax, dword ptr [esi + 10h]
        cmp    dword ptr [eax], 0
        mov    eax, 0fh
        jne    SHORT loc_4_3172
        cmp    dword ptr [esi + 1ch], 0ch
        mov    eax, 7ah
        jb     SHORT loc_4_3172
        push   edx
        mov    edx, dword ptr [esi + 18h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 004h, 000h, 000h, 000h
        mov    dword ptr [edx + 8], eax
        xor    eax, eax
        mov    dword ptr [edx], eax
        mov    dword ptr [esi + 20h], 0ch
        pop    edx
loc_4_3172:
        ret
loc_4_3173:
        cmp    dword ptr [esi + 14h], 0ch
        mov    eax, 7ah
        jb     SHORT loc_4_31AC
        mov    eax, dword ptr [esi + 10h]
        cmp    dword ptr [eax], 0
        mov    eax, 0fh
        jne    SHORT loc_4_31AC
        push   edx
        push   ecx
        mov    edx, dword ptr [esi + 10h]
        mov    eax, dword ptr [edx + 4]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 8]
        db  0A3h, 004h, 000h, 000h, 000h
        call   sub_4_3086
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        pop    ecx
        pop    edx
loc_4_31AC:
        ret
_PTEXT ends
        end
