; vpowerd_pcode_05.asm -- VPOWERD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        cmp    byte ptr [ebp + 1dh], 54h
        je     8
        stc
        ret
        db  057h, 08Bh, 03Dh, 030h, 006h, 000h, 000h, 00Fh, 0BAh, 067h, 003h, 007h, 05Fh, 073h, 0EFh, 080h
        db  04Dh, 02Ch, 001h, 0F8h, 0C3h
loc_5_001D:
        mov    ecx, dword ptr [esi + 0ch]
        jecxz  loc_5_009F
        cmp    ecx, -1
        je     SHORT loc_5_009F
        mov    eax, 1
        dec    ecx
        cmp    ecx, 19h
        jae    SHORT loc_5_00A1
        mov    ebx, ecx
        db  00Fh, 0B6h, 00Ch, 05Dh, 000h, 000h, 000h, 000h
        cmp    cl, 0ffh
        je     SHORT loc_5_00A1
        mov    eax, 57h
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_5_0058
        mov    edi, dword ptr [esi + 1ch]
        or     edi, edi
        je     SHORT loc_5_0058
        cmp    edi, 4
        jne    SHORT loc_5_00A1
loc_5_0058:
        mov    edx, dword ptr [esi + 10h]
        or     edx, edx
        jne    SHORT loc_5_0066
        mov    dword ptr [esi + 14h], 0
loc_5_0066:
        lea    edi, [ecx*4]
        cmp    edi, dword ptr [esi + 14h]
        jne    SHORT loc_5_00A1
        jecxz  loc_5_007E
        add    edx, edi
loc_5_0076:
        sub    edx, 4
        mov    eax, dword ptr [edx]
        push   eax
        loop   loc_5_0076
loc_5_007E:
        db  0FFh, 014h, 09Dh, 070h, 005h, 000h, 000h
        add    esp, edi
        mov    ecx, dword ptr [esi + 18h]
        jecxz  loc_5_009F
        cmp    dword ptr [esi + 1ch], 4
        jne    SHORT loc_5_009F
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi + 20h]
        jecxz  loc_5_009F
        mov    dword ptr [ecx], 4
loc_5_009F:
        xor    eax, eax
loc_5_00A1:
        ret
loc_5_00A2:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 219h
        jne    SHORT loc_5_00DA
        cmp    dword ptr [ebp + 0ch], 8100h
        jne    SHORT loc_5_00DA
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        cmp    word ptr [esi + 4], 0dh
        jne    SHORT loc_5_00DA
        cmp    dword ptr [esi], 0eh
        jne    SHORT loc_5_00DA
        mov    eax, dword ptr [esi + 0ah]
        push   eax
        push   1
        db  0CDh, 020h, 00Dh, 000h, 026h, 000h
        add    esp, 8
        mov    dword ptr [esi + 6], eax
        pop    esi
loc_5_00DA:
        mov    eax, 1
        leave
        ret
        db  000h, 000h, 000h
sub_5_00E4:
        push   ebp
        push   ebx
        push   esi
        push   edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 0dh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ecx, 20h
loc_5_011A:
        cmp    ecx, 3
        jb     SHORT loc_5_015C
        mov    byte ptr [ebp + 10h], cl
        mov    word ptr [ebp + 1ch], 4409h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_5_015A
        test   byte ptr [ebp + 15h], 90h
        jne    SHORT loc_5_015A
        mov    word ptr [ebp + 14h], cx
        mov    word ptr [ebp + 18h], 1
        mov    word ptr [ebp + 1ch], 710dh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_5_015A:
        loop   loc_5_011A
loc_5_015C:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_5_0176:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   di, 10h
        jne    SHORT loc_5_018E
        db  080h, 025h, 040h, 006h, 000h, 000h, 0FBh
        or     edi, 1
loc_5_018E:
        call   sub_5_00E4
        db  0E8h, 003h, 001h, 0FBh, 0FFh
        push   0
        push   edi
        push   4
        call   sub_5_0298
        add    esp, 0ch
        push   0
        push   edi
        push   5
        call   sub_5_0298
        add    esp, 0ch
        pop    edi
        leave
        ret
loc_5_01B5:
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, ebx
        xor    edx, edx
        cmp    ecx, 2
        ja     SHORT loc_5_01C5
        inc    edx
loc_5_01C5:
        or     edx, 80000000h
        and    ebx, 7
        cmp    ebx, 1
        je     SHORT loc_5_01DE
        xor    eax, eax
        db  0F6h, 005h, 040h, 006h, 000h, 000h, 006h
        jne    SHORT loc_5_01F0
loc_5_01DE:
        push   ebx
        push   edx
        push   ebx
        db  0CDh, 020h, 005h, 000h, 026h, 000h
        add    esp, 8
        pop    ebx
        cmp    ebx, 1
        je     SHORT loc_5_020B
loc_5_01F0:
        db  080h, 025h, 040h, 006h, 000h, 000h, 0FEh
        cmp    eax, 60h
        jne    SHORT loc_5_020B
        db  0FEh, 00Dh, 080h, 006h, 000h, 000h
        jne    SHORT loc_5_0212
        db  080h, 00Dh, 040h, 006h, 000h, 000h, 004h
loc_5_020B:
        db  0C6h, 005h, 080h, 006h, 000h, 000h, 003h
loc_5_0212:
        leave
        ret
sub_5_0214:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    eax, 100h
        push   eax
        mov    eax, 1
        push   eax
        mov    eax, 0
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0CDh, 020h, 070h, 000h, 040h, 000h
        add    esp, 14h
        mov    eax, dword ptr [ebp + 0ch]
        pop    edx
        pop    ecx
        leave
        ret
loc_5_0243:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        shr    ecx, 2
        db  00Fh, 0B6h, 089h, 084h, 006h, 000h, 000h
        test   ecx, 10h
        jne    SHORT loc_5_026A
        db  083h, 03Dh, 044h, 006h, 000h, 000h, 000h
        jne    SHORT loc_5_026A
        or     ecx, 80h
loc_5_026A:
        push   ecx
        call   sub_5_0176
        pop    ecx
        leave
        ret
loc_5_0273:
        enter  8, 0
        push   esi
        lea    esi, [ebp - 8]
        push   esi
        push   1
        db  0CDh, 020h, 007h, 000h, 026h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_0295
        db  0C7h, 005h, 03Ch, 006h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_5_0295:
        pop    esi
        leave
        ret
sub_5_0298:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  08Dh, 01Dh, 088h, 006h, 000h, 000h
        mov    edi, dword ptr [ebp + 0ch]
        test   di, 4
        je     SHORT loc_5_02B3
        mov    ebx, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebx]
loc_5_02B3:
        xor    esi, esi
        test   di, 8
        je     SHORT loc_5_02C1
        mov    esi, 4
loc_5_02C1:
        mov    ebx, dword ptr [esi + ebx]
        xor    eax, eax
        db  081h, 0FBh, 088h, 006h, 000h, 000h
        je     SHORT loc_5_02EE
        push   edi
        push   dword ptr [ebp + 8]
        call   dword ptr [ebx + 8]
        pop    ecx
        pop    ecx
        test   di, 2
        jne    SHORT loc_5_02C1
        cmp    eax, 80000000h
        je     SHORT loc_5_02C1
        or     eax, eax
        je     SHORT loc_5_02C1
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], ebx
loc_5_02EE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
_VPOWERD_Get_Power_Status:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 030h, 006h, 000h, 000h
        test   byte ptr [eax + 3], 8
        jne    NEAR PTR loc_5_0393
        mov    ax, 530ah
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, 0ffffh
        mov    cx, 0ffffh
        push   ebp
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_5_038B
        mov    di, si
        mov    eax, 80000003h
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        je     SHORT loc_5_038E
        db  0C7h, 005h, 03Ch, 006h, 000h, 000h, 042h, 000h, 000h, 000h
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        mov    byte ptr [esi + 3], cl
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        jne    SHORT loc_5_0354
        mov    ch, 0ffh
        mov    dx, 0ffffh
loc_5_0354:
        mov    byte ptr [esi + 2], ch
        mov    word ptr [esi + 4], dx
        cmp    dword ptr [ebp + 8], 1
        jne    SHORT loc_5_0389
        mov    eax, dword ptr [esi]
        db  03Bh, 005h, 049h, 000h, 000h, 000h
        je     SHORT loc_5_0389
        db  087h, 005h, 049h, 000h, 000h, 000h
        cmp    eax, -1
        je     SHORT loc_5_0389
        db  0B8h, 02Ah, 002h, 000h, 000h
        push   0
        push   0
        push   eax
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
loc_5_0389:
        xor    eax, eax
loc_5_038B:
        movzx  eax, ah
loc_5_038E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_5_0393:
        db  0C7h, 005h, 049h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        mov    eax, 9
        jmp    SHORT loc_5_038E
_VPOWERD_W32_Get_System_Power_Status:
        push   ebp
        mov    ebp, esp
        push   edi
        push   dword ptr [ebp + 8]
        push   1
        db  0CDh, 020h, 016h, 000h, 026h, 000h
        add    esp, 8
        mov    edi, dword ptr [ebp + 8]
        pop    edi
        leave
        ret
VPOWERD_Service_22:
        enter  0ch, 0
        push   esi
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, dword ptr [ebp + 0ch]
        push   esi
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 007h, 000h, 026h, 000h
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        jne    NEAR PTR loc_5_048E
        mov    al, byte ptr [esi]
        cmp    al, 2
        jb     SHORT loc_5_03F0
        ja     SHORT loc_5_03EE
        mov    al, 0
        jmp    SHORT loc_5_03F0
loc_5_03EE:
        mov    al, 0ffh
loc_5_03F0:
        mov    byte ptr [edi], al
        xor    ecx, ecx
        db  03Ah, 005h, 048h, 000h, 000h, 000h
        je     SHORT loc_5_0406
        db  0A2h, 048h, 000h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_5_0406
        inc    ecx
loc_5_0406:
        mov    dword ptr [ebp - 0ch], ecx
        mov    al, byte ptr [esi + 2]
        cmp    al, 0ffh
        jne    SHORT loc_5_0421
        mov    cl, byte ptr [esi + 1]
        cmp    cl, 0ffh
        je     SHORT loc_5_0421
        cmp    cl, 3
        ja     SHORT loc_5_0421
        mov    al, 1
        shl    al, cl
loc_5_0421:
        cmp    al, 0
        jne    SHORT loc_5_0427
        mov    al, 0ffh
loc_5_0427:
        mov    byte ptr [edi + 1], al
        mov    al, byte ptr [esi + 3]
        cmp    al, 64h
        jbe    SHORT loc_5_0433
        mov    al, 0ffh
loc_5_0433:
        mov    byte ptr [edi + 2], al
        movzx  eax, word ptr [esi + 4]
        cmp    ax, -1
        jne    SHORT loc_5_0443
        cwde
        jmp    SHORT loc_5_047A
loc_5_0443:
        btr    eax, 0fh
        jae    SHORT loc_5_044C
        imul   eax, eax, 3ch
loc_5_044C:
        movzx  ecx, byte ptr [edi + 2]
        cmp    cl, 0ffh
        je     SHORT loc_5_047A
        db  083h, 03Dh, 044h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_5_0464
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_047A
loc_5_0464:
        push   eax
        imul   eax, eax, 64h
        cdq
        jecxz  loc_5_046F
        idiv   ecx
        jmp    SHORT loc_5_0474
loc_5_046F:
        mov    eax, 0ffffffffh
loc_5_0474:
        db  0A3h, 044h, 000h, 000h, 000h
        pop    eax
loc_5_047A:
        mov    dword ptr [edi + 4], eax
        db  0A1h, 044h, 000h, 000h, 000h
        mov    dword ptr [edi + 8], eax
        mov    byte ptr [edi + 3], 0
        mov    eax, 1
loc_5_048E:
        pop    edi
        pop    esi
        leave
        ret
_VPOWERD_W32_Set_System_Power_State:
        enter  4, 0
        lea    eax, [ebp - 4]
        push   0
        push   eax
        db  0CDh, 020h, 057h, 000h, 033h, 000h
        add    esp, 8
        mov    eax, 80000002h
        test   byte ptr [ebp - 4], 2
        jne    SHORT loc_5_04C6
        xor    eax, eax
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_5_04BA
        mov    al, 2
loc_5_04BA:
        push   eax
        push   2
        db  0CDh, 020h, 005h, 000h, 026h, 000h
        add    esp, 8
loc_5_04C6:
        leave
        ret
VPOWERD_Service_17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ch
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 001h
        jne    NEAR PTR loc_5_0565
        mov    eax, 80000003h
        mov    ebx, dword ptr [ebp + 8]
        or     ebx, ebx
        je     SHORT loc_5_0565
        movzx  eax, word ptr [ebx]
        xor    edx, edx
        mov    ecx, 64h
        div    ecx
        db  0E8h, 074h, 0FEh, 0FAh, 0FFh
        shl    ax, 8
        mov    di, ax
        mov    ax, dx
        db  0E8h, 065h, 0FEh, 0FAh, 0FFh
        or     di, ax
        mov    ax, word ptr [ebx + 8]
        xor    edx, edx
        db  0E8h, 057h, 0FEh, 0FAh, 0FFh
        mov    dh, al
        mov    ax, word ptr [ebx + 0ah]
        db  0E8h, 04Ch, 0FEh, 0FAh, 0FFh
        or     dx, ax
        mov    ax, word ptr [ebx + 0ch]
        db  0E8h, 040h, 0FEh, 0FAh, 0FFh
        mov    ch, al
        mov    ax, word ptr [ebx + 2]
        db  0E8h, 035h, 0FEh, 0FAh, 0FFh
        mov    si, ax
        shl    si, 8
        mov    ax, word ptr [ebx + 6]
        db  0E8h, 025h, 0FEh, 0FAh, 0FFh
        or     si, ax
        mov    ax, 5311h
        xor    bx, bx
        mov    cl, 2
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     SHORT loc_5_0562
        xor    eax, eax
loc_5_0562:
        movzx  eax, ah
loc_5_0565:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
VPOWERD_Service_18:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ch
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 001h
        jne    NEAR PTR loc_5_0686
        mov    ax, 5311h
        xor    ebx, ebx
        mov    ecx, 1
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     NEAR PTR loc_5_0683
        mov    eax, 80000003h
        mov    ebx, dword ptr [ebp + 8]
        or     ebx, ebx
        je     NEAR PTR loc_5_0686
        push   edx
        and    dx, 0ff00h
        shr    dx, 8
        mov    al, dl
        db  0E8h, 0ACh, 0FDh, 0FAh, 0FFh
        mov    word ptr [ebx + 8], ax
        pop    edx
        and    dx, 0ffh
        mov    al, dl
        db  0E8h, 09Bh, 0FDh, 0FAh, 0FFh
        mov    word ptr [ebx + 0ah], ax
        shr    cx, 8
        xor    ch, ch
        mov    al, cl
        db  0E8h, 08Ah, 0FDh, 0FAh, 0FFh
        mov    word ptr [ebx + 0ch], ax
        push   esi
        and    si, 0ff00h
        shr    si, 8
        mov    ax, si
        db  0E8h, 074h, 0FDh, 0FAh, 0FFh
        mov    word ptr [ebx + 2], ax
        pop    esi
        and    si, 0ffh
        mov    ax, si
        db  0E8h, 062h, 0FDh, 0FAh, 0FFh
        mov    word ptr [ebx + 6], ax
        mov    ax, di
        movzx  eax, ah
        db  0E8h, 053h, 0FDh, 0FAh, 0FFh
        mov    ecx, 64h
        mul    ecx
        push   eax
        mov    ax, di
        movzx  eax, al
        db  0E8h, 040h, 0FDh, 0FAh, 0FFh
        pop    ecx
        add    ax, cx
        mov    word ptr [ebx], ax
        mov    word ptr [ebx + 0eh], 0
        db  08Dh, 035h, 081h, 000h, 000h, 000h
        movzx  edi, ax
        test   edi, 3
        jne    SHORT loc_5_064A
        db  08Dh, 035h, 04Dh, 000h, 000h, 000h
loc_5_064A:
        movzx  eax, word ptr [ebx + 2]
        lea    esi, [esi + eax*4]
        mov    eax, dword ptr [esi]
        movzx  ecx, word ptr [ebx + 6]
        dec    ecx
        add    eax, ecx
        push   eax
        sub    edi, 7c5h
        push   edi
        mov    eax, edi
        mov    ecx, 16dh
        mul    ecx
        pop    edx
        shr    edx, 2
        pop    ecx
        add    ecx, edx
        add    eax, ecx
        xor    edx, edx
        mov    ecx, 7
        div    ecx
        mov    word ptr [ebx + 4], dx
        xor    eax, eax
loc_5_0683:
        movzx  eax, ah
loc_5_0686:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
loc_5_068C:
        enter  4, 0
        push   ebx
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 80000000h
        mov    ecx, 7
        cmp    eax, 9
        ja     NEAR PTR loc_5_07A5
        db  0FFh, 024h, 085h, 0B8h, 000h, 000h, 000h
loc_5_06B8:
        db  0C6h, 005h, 0F6h, 000h, 000h, 000h, 000h
        test   dl, 2
        jne    NEAR PTR loc_5_07B3
        mov    eax, 1
        test   dl, 10h
        jne    SHORT loc_5_06DC
        mov    eax, 0
        mov    ecx, 0fh
loc_5_06DC:
        and    edx, 1
        push   edx
        push   eax
        push   ecx
        push   1
        push   dword ptr [ebp + 0ch]
        call   sub_5_07B8
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_5_07B3
        mov    ebx, 80000002h
        mov    edx, dword ptr [ebp + 0ch]
loc_5_06FF:
        mov    eax, 3
        test   dl, 10h
        jne    NEAR PTR loc_5_0791
        mov    eax, 2
        jmp    SHORT loc_5_078C
loc_5_0714:
        db  0C6h, 005h, 0F6h, 000h, 000h, 000h, 001h
        mov    eax, 5
        test   dl, 10h
        jne    SHORT loc_5_0791
        mov    eax, 4
        jmp    SHORT loc_5_078C
loc_5_072C:
        test   dl, 4
        je     SHORT loc_5_0741
        db  080h, 03Dh, 0F6h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_06FF
        mov    dword ptr [ebp - 4], 1
loc_5_0741:
        mov    eax, 6
        test   dl, 20h
        jne    SHORT loc_5_0791
        mov    eax, 8
        test   dl, 10h
        jne    SHORT loc_5_0791
        mov    eax, 7
        jmp    SHORT loc_5_078C
loc_5_075C:
        db  0A1h, 049h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0ah
        jmp    SHORT loc_5_078C
loc_5_076B:
        db  0A1h, 030h, 006h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, 10h
        jmp    SHORT loc_5_078C
loc_5_077D:
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0bh
        jmp    SHORT loc_5_078C
loc_5_0787:
        mov    eax, 9
loc_5_078C:
        mov    ecx, 0fh
loc_5_0791:
        push   dword ptr [ebp - 4]
        push   eax
        push   ecx
        push   0
        push   dword ptr [ebp + 0ch]
        call   sub_5_07B8
        add    esp, 14h
        jmp    SHORT loc_5_07B3
loc_5_07A5:
        cmp    eax, 200h
        jb     SHORT loc_5_07B3
        cmp    eax, 2ffh
        jbe    SHORT loc_5_077D
loc_5_07B3:
        mov    eax, ebx
        pop    ebx
        leave
        ret
sub_5_07B8:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        lea    ebx, [ebp - 4]
        mov    edi, dword ptr [ebp + 14h]
        db  0BEh, 0E0h, 000h, 000h, 000h
        test   byte ptr [ebp + 8], 8
        je     SHORT loc_5_07D5
        db  0BEh, 0E6h, 000h, 000h, 000h
loc_5_07D5:
        movzx  ecx, byte ptr [esi]
        jecxz  loc_5_080C
        inc    esi
        test   cl, 80h
        jne    SHORT loc_5_0811
        test   cl, 40h
        jne    SHORT loc_5_082F
        and    ecx, dword ptr [ebp + 10h]
        je     SHORT loc_5_07D5
        mov    dword ptr [ebx], ecx
        push   dword ptr [ebp + 18h]
        push   edi
        push   218h
        push   ebx
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 010h, 000h, 017h, 000h
        add    esp, 14h
        test   byte ptr [ebp + 0ch], 1
        je     SHORT loc_5_07D5
        or     eax, eax
        jne    SHORT loc_5_07D5
loc_5_080C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_5_0811:
        cmp    edi, 4
        jb     SHORT loc_5_07D5
        cmp    edi, 7
        ja     SHORT loc_5_07D5
        db  00Fh, 0B6h, 087h, 0E8h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_07D5
        push   eax
        call   sub_5_084D
        pop    ecx
        jmp    SHORT loc_5_07D5
loc_5_082F:
        cmp    edi, 4
        jb     SHORT loc_5_07D5
        cmp    edi, 9
        ja     SHORT loc_5_07D5
        db  00Fh, 0B6h, 087h, 0ECh, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_07D5
        push   eax
        call   sub_5_087D
        pop    ecx
        jmp    SHORT loc_5_07D5
sub_5_084D:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 4]
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 1ah
        xor    ebx, ebx
        xor    edx, edx
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        xor    eax, eax
        cmp    dword ptr [ebp - 4], -1
        je     SHORT loc_5_0878
        inc    eax
loc_5_0878:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_5_087D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 530bh
        mov    ebx, dword ptr [ebp + 8]
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        xor    eax, eax
        cmp    bh, 80h
        je     SHORT loc_5_089B
        inc    eax
loc_5_089B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_PTEXT ends
        end
