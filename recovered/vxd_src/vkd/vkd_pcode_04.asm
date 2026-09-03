; vkd_pcode_04.asm -- VKD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VKD_Disable_Keyboard:
        push   ebx
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  080h, 025h, 051h, 012h, 000h, 000h, 0EFh
        mov    edx, 64h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 60h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        call   sub_4_06A2
        pop    ebx
        ret
VKD_Enable_Keyboard:
        mov    edx, 64h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 60h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  080h, 00Dh, 051h, 012h, 000h, 000h, 010h
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        call   sub_4_0698
        ret
loc_4_006A:
        db  08Bh, 015h, 010h, 012h, 000h, 000h
        add    edx, ebx
        mov    ax, word ptr [ebp + 1ch]
        cmp    ah, 4fh
        jne    SHORT loc_4_0085
        cmp    al, 0e1h
        je     SHORT loc_4_00ED
        test   byte ptr [edx + 3], 1
        jne    SHORT loc_4_00CD
loc_4_0085:
        cmp    ax, 9002h
        jne    SHORT loc_4_00CB
        cmp    byte ptr [ebp + 29h], 0f0h
        jb     SHORT loc_4_00CB
        mov    ax, word ptr [41ah]
        cmp    ax, word ptr [41ch]
        jne    SHORT loc_4_00BF
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        sub    word ptr [ebp + 24h], 2
        db  0CDh, 020h, 0F9h, 000h, 001h, 000h
        jne    SHORT loc_4_00BD
        or     byte ptr [edx + 2], 40h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_4_00BD:
        clc
        ret
loc_4_00BF:
        btr    dword ptr [edx], 16h
        jae    SHORT loc_4_00CB
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
loc_4_00CB:
        stc
        ret
loc_4_00CD:
        cmp    al, 0c5h
        jne    SHORT loc_4_00F1
        xor    eax, eax
        mov    ecx, 6bh
        db  0BEh, 002h, 003h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        and    byte ptr [edx + 3], 0feh
        or     byte ptr [edx + 2], 80h
        jmp    SHORT loc_4_00F1
loc_4_00ED:
        or     byte ptr [edx + 3], 1
loc_4_00F1:
        and    byte ptr [ebp + 2ch], 0feh
        clc
        ret
VKD_SET_DEVICE_FOCUS:
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_4_0105
        db  0E8h, 0EFh, 0FFh, 0FBh, 0FFh
loc_4_0105:
        test   edx, edx
        je     SHORT loc_4_010E
        cmp    edx, 0dh
        jne    SHORT loc_4_0156
loc_4_010E:
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        je     SHORT loc_4_0156
        call   sub_4_0698
        push   ebx
        db  08Bh, 01Dh, 014h, 012h, 000h, 000h
        db  0E8h, 0CFh, 007h, 0FCh, 0FFh
        pop    ebx
        db  089h, 01Dh, 014h, 012h, 000h, 000h
        db  08Bh, 015h, 010h, 012h, 000h, 000h
        add    edx, ebx
        test   dword ptr [edx], 100000h
        je     SHORT loc_4_0156
        mov    esi, edx
        movzx  eax, word ptr [edx + 36h]
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        db  0E8h, 006h, 005h, 0FCh, 0FFh
        je     SHORT loc_4_0158
loc_4_0151:
        db  0E8h, 05Ah, 000h, 0FCh, 0FFh
loc_4_0156:
        clc
        ret
loc_4_0158:
        push   eax
        mov    al, byte ptr [edx + 34h]
        cmp    al, byte ptr [edx + 35h]
        pop    eax
        je     SHORT loc_4_0156
        jmp    SHORT loc_4_0151
VKD_Get_Version:
        mov    eax, 20ah
        clc
        ret
VKD_Define_Hot_Key:
        push   ecx
        push   edi
        xor    edi, edi
        test   cl, 80h
        je     SHORT loc_4_018E
        and    cl, 7fh
loc_4_0177:
        db  00Fh, 0BCh, 03Dh, 04Dh, 012h, 000h, 000h
        je     SHORT loc_4_018B
        db  00Fh, 0B3h, 03Dh, 04Dh, 012h, 000h, 000h
        jae    SHORT loc_4_0177
        jmp    SHORT loc_4_018E
loc_4_018B:
        stc
        jmp    SHORT loc_4_01E7
loc_4_018E:
        push   edi
        push   esi
        mov    edi, eax
        db  08Bh, 035h, 008h, 013h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        xchg   edi, eax
        mov    word ptr [edi + 6], bx
        ror    ebx, 10h
        mov    word ptr [edi + 4], bx
        ror    ebx, 10h
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], 0
        or     ah, ah
        je     SHORT loc_4_01C2
        cmp    ah, 0ffh
        je     SHORT loc_4_01C2
        mov    ah, 80h
        or     byte ptr [edi + 1], 80h
loc_4_01C2:
        mov    byte ptr [edi + 2], ah
        test   cl, 0fh
        jne    SHORT loc_4_01CD
        or     cl, 0fh
loc_4_01CD:
        mov    byte ptr [edi + 3], cl
        pop    dword ptr [edi + 0ch]
        mov    dword ptr [edi + 14h], edx
        pop    dword ptr [edi + 8]
        pop    eax
        push   eax
        mov    dword ptr [edi + 10h], eax
        mov    eax, edi
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        clc
loc_4_01E7:
        pop    edi
        pop    ecx
        ret
VKD_Remove_Hot_Key:
        pushad
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_4_021A
        mov    ecx, dword ptr [eax + 8]
        db  00Fh, 0ABh, 00Dh, 04Dh, 012h, 000h, 000h
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_4_0207:
        btr    dword ptr [esi + ebx + 0ch], ecx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_4_0207
loc_4_021A:
        db  08Bh, 035h, 008h, 013h, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        popad
        ret
VKD_Local_Enable_Hot_Key:
        push   ebx
        push   ecx
        mov    ecx, dword ptr [eax + 8]
        cmp    cl, 0
        je     SHORT loc_4_0242
        db  003h, 01Dh, 010h, 012h, 000h, 000h
        btr    dword ptr [ebx + 0ch], ecx
loc_4_0242:
        pop    ecx
        pop    ebx
        ret
VKD_Local_Disable_Hot_Key:
        push   ebx
        push   ecx
        mov    ecx, dword ptr [eax + 8]
        cmp    cl, 0
        je     SHORT loc_4_0259
        db  003h, 01Dh, 010h, 012h, 000h, 000h
        bts    dword ptr [ebx + 0ch], ecx
loc_4_0259:
        pop    ecx
        pop    ebx
        ret
VKD_Reflect_Hot_Key:
        pushad
        mov    esi, ebx
        db  003h, 035h, 010h, 012h, 000h, 000h
        mov    edx, ebx
        mov    edi, eax
        movzx  eax, word ptr [esi + 36h]
        mov    ebx, ecx
        db  0E8h, 0E1h, 003h, 0FCh, 0FFh
        db  0E8h, 02Bh, 001h, 0FCh, 0FFh
        movzx  eax, word ptr [edi]
        db  0E8h, 0FDh, 002h, 0FCh, 0FFh
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_4_0290
        and    byte ptr [edi + 3], 7fh
        db  0A3h, 039h, 012h, 000h, 000h
loc_4_0290:
        or     al, 80h
        db  0E8h, 0E7h, 002h, 0FCh, 0FFh
        call   sub_4_05E7
        popad
        ret
VKD_Cancel_Hot_Key_State:
        pushfd
        pushad
        db  08Bh, 035h, 014h, 012h, 000h, 000h
        db  003h, 035h, 010h, 012h, 000h, 000h
        call   sub_4_05E7
        popad
        popfd
        ret
sub_4_02B4:
        test   byte ptr [esi + 1], 20h
        jne    SHORT loc_4_032B
        mov    al, byte ptr [esi + 35h]
        cmp    al, byte ptr [esi + 34h]
        jne    SHORT loc_4_032B
        mov    edi, 41ah
        add    edi, dword ptr [edx + 4]
        movzx  eax, word ptr [edi]
        movzx  ebx, word ptr [edi + 2]
        cmp    ebx, eax
        je     SHORT loc_4_030B
        ja     SHORT loc_4_02E6
        mov    edi, 480h
        add    edi, dword ptr [edx + 4]
        sub    bx, word ptr [edi]
        add    bx, word ptr [edi + 2]
loc_4_02E6:
        sub    ebx, eax
        shr    ebx, 1
        cmp    ebx, 8
        jbe    SHORT loc_4_030B
        push   edx
        push   esi
        mov    eax, dword ptr [esi + 4fh]
        mov    ebx, edx
        mov    edx, esi
        db  0BEh, 0C1h, 003h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        pop    esi
        pop    edx
        or     byte ptr [esi + 1], 20h
        jmp    SHORT loc_4_032B
loc_4_030B:
        mov    edi, dword ptr [esi + 14h]
        cmp    edi, dword ptr [esi + 18h]
        jb     SHORT loc_4_032C
        movzx  eax, word ptr [esi + 36h]
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        db  0E8h, 033h, 003h, 0FCh, 0FFh
        xor    eax, eax
        mov    ebx, edx
        db  0E8h, 012h, 0FDh, 0FBh, 0FFh
loc_4_032B:
        ret
loc_4_032C:
        mov    eax, edi
        add    eax, 4
        mov    dword ptr [esi + 14h], eax
        movzx  eax, word ptr [esi + 36h]
        movzx  ebx, word ptr [edi + 2]
        cmp    ebx, 0ffffh
        jne    SHORT loc_4_0346
        xor    ebx, ebx
loc_4_0346:
        pushfd
        and    ebx, 6
        db  0E8h, 006h, 003h, 0FCh, 0FFh
        popfd
        je     SHORT loc_4_0363
        mov    al, byte ptr [edi + 1]
        db  0E8h, 012h, 001h, 0FCh, 0FFh
        or     al, 80h
        db  0E8h, 00Bh, 001h, 0FCh, 0FFh
        jmp    SHORT loc_4_032B
loc_4_0363:
        mov    al, 38h
        db  0E8h, 002h, 001h, 0FCh, 0FFh
        mov    al, byte ptr [edi]
        aam    0ah
        mov    cl, al
        mov    al, ah
        aam    0ah
        mov    ch, al
        movzx  ebx, ah
        db  08Ah, 083h, 0FEh, 012h, 000h, 000h
        db  0E8h, 0E8h, 000h, 0FCh, 0FFh
        or     al, 80h
        db  0E8h, 0E1h, 000h, 0FCh, 0FFh
        movzx  ebx, ch
        db  08Ah, 083h, 0FEh, 012h, 000h, 000h
        db  0E8h, 0D3h, 000h, 0FCh, 0FFh
        or     al, 80h
        db  0E8h, 0CCh, 000h, 0FCh, 0FFh
        movzx  ebx, cl
        db  08Ah, 083h, 0FEh, 012h, 000h, 000h
        db  0E8h, 0BEh, 000h, 0FCh, 0FFh
        or     al, 80h
        db  0E8h, 0B7h, 000h, 0FCh, 0FFh
        mov    al, 0b8h
        db  0E8h, 0B0h, 000h, 0FCh, 0FFh
        jmp    NEAR PTR loc_4_032B
loc_4_03C1:
        and    byte ptr [edx + 1], 0dfh
        db  0E9h, 0E6h, 0FDh, 0FBh, 0FFh
loc_4_03CA:
        and    byte ptr [edx + 2], 0feh
        db  0E9h, 0DDh, 0FDh, 0FBh, 0FFh
        db  090h
loc_4_03D4:
        mov    edi, ebx
        db  003h, 03Dh, 010h, 012h, 000h, 000h
        inc    dword ptr [ebp + 24h]
        test   byte ptr [edi + 1], 4
        jne    SHORT loc_4_040F
        movzx  edx, word ptr [ebp + 34h]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 30h]
        add    edx, dword ptr [ebx + 4]
        movzx  edx, word ptr [ecx + edx]
loc_4_03F7:
        db  0CDh, 020h, 000h, 000h, 018h, 000h
loc_4_03FD:
        db  08Bh, 015h, 01Ch, 012h, 000h, 000h
        add    edx, dword ptr [ebx + 4]
        mov    word ptr [edx], ax
        ret
sub_4_040A:
        xor    eax, eax
        inc    eax
        jmp    SHORT loc_4_03FD
loc_4_040F:
        test   byte ptr [edi], 80h
        jne    SHORT sub_4_040A
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 10h
        jne    SHORT sub_4_040A
        xor    esi, esi
        xchg   dword ptr [edi + 24h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        test   byte ptr [edi + 1], 2
        je     SHORT loc_4_044A
        mov    esi, 41ah
        add    esi, dword ptr [ebx + 4]
        mov    ax, word ptr [esi + 2]
        cmp    ax, word ptr [esi]
        jne    SHORT sub_4_040A
        and    byte ptr [edi + 1], 0fdh
loc_4_044A:
        movzx  edx, word ptr [ebp + 34h]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 30h]
        add    edx, dword ptr [ebx + 4]
        test   byte ptr [ecx + edx], 40h
        je     SHORT sub_4_040A
        dec    byte ptr [edi + 38h]
        jne    SHORT sub_4_040A
        mov    esi, dword ptr [edi + 14h]
        cmp    esi, dword ptr [edi + 18h]
        jb     SHORT loc_4_0474
        xor    eax, eax
        db  0E8h, 0CBh, 0FBh, 0FBh, 0FFh
        jmp    SHORT sub_4_040A
loc_4_0474:
        cld
        lodsd
        mov    dword ptr [edi + 14h], esi
        cmp    ah, 0ffh
        jne    SHORT loc_4_0480
        xor    ah, ah
loc_4_0480:
        mov    cl, byte ptr [edi + 58h]
        cmp    al, 0dh
        je     SHORT loc_4_048A
        mov    cl, byte ptr [edi + 57h]
loc_4_048A:
        mov    byte ptr [edi + 38h], cl
        call   sub_4_04A8
        or     al, al
        je     SHORT loc_4_04A3
        sub    dword ptr [edi + 14h], 4
        cmp    al, 0ffh
        jne    SHORT loc_4_04A3
        db  0E8h, 01Eh, 0FCh, 0FBh, 0FFh
loc_4_04A3:
        jmp    NEAR PTR sub_4_040A
sub_4_04A8:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        or     byte ptr [edi], 80h
        mov    word ptr [ebp + 18h], ax
        mov    word ptr [ebp + 1ch], 5ffh
        xor    eax, eax
        mov    al, 16h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    al, byte ptr [ebp + 1ch]
        and    byte ptr [edi], 7fh
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        ret
loc_4_04EC:
        xchg   ebx, eax
        mov    al, byte ptr [esp + 1ch]
        mov    ah, 40h
        btr    eax, 7
        jae    SHORT loc_4_04FC
        add    ah, 1
loc_4_04FC:
        call   sub_4_057E
        xchg   ebx, eax
        db  0E9h, 07Eh, 005h, 0FCh, 0FFh
sub_4_0507:
        push   eax
        push   ecx
        mov    ecx, ebx
        db  087h, 00Dh, 010h, 013h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_4_0533
        cmp    ecx, ebx
        je     SHORT loc_4_0533
        test   byte ptr [ecx + 3], 8
        je     SHORT loc_4_0533
        push   ebx
        mov    ebx, ecx
        mov    al, byte ptr [ebx]
        mov    ah, 3
        call   sub_4_057E
        db  080h, 025h, 051h, 012h, 000h, 000h, 0FEh
        pop    ebx
loc_4_0533:
        pop    ecx
        db  080h, 00Dh, 051h, 012h, 000h, 000h, 001h
        xor    ah, ah
        test   al, 80h
        je     SHORT loc_4_054F
        and    byte ptr [ebx + 3], 7fh
        test   byte ptr [ebx + 3], 2
        je     SHORT loc_4_056B
        inc    ah
        jmp    SHORT loc_4_0572
loc_4_054F:
        or     byte ptr [ebx + 3], 80h
        db  03Ah, 005h, 014h, 013h, 000h, 000h
        jne    SHORT loc_4_0565
        test   byte ptr [ebx + 3], 4
        je     SHORT loc_4_056B
        mov    ah, 2
        jmp    SHORT loc_4_0572
loc_4_0565:
        test   byte ptr [ebx + 3], 1
        jne    SHORT loc_4_0572
loc_4_056B:
        db  0A2h, 014h, 013h, 000h, 000h
        jmp    SHORT loc_4_057C
loc_4_0572:
        db  0A2h, 014h, 013h, 000h, 000h
        call   sub_4_057E
loc_4_057C:
        pop    eax
        ret
sub_4_057E:
        pushad
        xor    edi, edi
        db  08Bh, 00Dh, 02Dh, 012h, 000h, 000h
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_4_05DF
        push   eax
        db  08Bh, 035h, 00Ch, 013h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        mov    edx, eax
        mov    dword ptr [edx], ebx
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [edx + 4], eax
        pop    eax
        mov    word ptr [edx + 8], ax
        mov    word ptr [edx + 0ah], cx
        mov    eax, edx
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        db  00Fh, 0BAh, 02Dh, 051h, 012h, 000h, 000h, 003h
        jb     SHORT loc_4_05E5
        mov    eax, 10h
        mov    ecx, 3
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 046h, 006h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        jmp    SHORT loc_4_05E5
loc_4_05DF:
        mov    edx, dword ptr [ebx + 14h]
        call   dword ptr [ebx + 0ch]
loc_4_05E5:
        popad
        ret
sub_4_05E7:
        xor    ebx, ebx
        db  087h, 01Dh, 010h, 013h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_0645
        db  0A0h, 014h, 013h, 000h, 000h
        test   al, 80h
        jne    SHORT loc_4_0611
        and    byte ptr [ebx + 3], 7fh
        or     al, 80h
        call   sub_4_0507
        db  0C7h, 005h, 010h, 013h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0611:
        db  080h, 025h, 051h, 012h, 000h, 000h, 0FEh
        test   byte ptr [ebx + 3], 8
        je     SHORT loc_4_0627
        mov    al, byte ptr [ebx]
        mov    ah, 3
        call   sub_4_057E
loc_4_0627:
        push   edx
        mov    edx, esi
        db  02Bh, 015h, 010h, 012h, 000h, 000h
        movzx  eax, word ptr [esi + 36h]
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        db  0E8h, 016h, 000h, 0FCh, 0FFh
        db  0E8h, 060h, 0FDh, 0FBh, 0FFh
        pop    edx
loc_4_0645:
        ret
loc_4_0646:
        db  080h, 025h, 051h, 012h, 000h, 000h, 0F7h
loc_4_064D:
        db  08Bh, 035h, 00Ch, 013h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_0696
        mov    edx, eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    ebx, dword ptr [edx]
        mov    edi, dword ptr [edx + 4]
        movzx  ecx, word ptr [edx + 8]
        movzx  edx, word ptr [edx + 0ah]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        xchg   edi, eax
        sub    edi, eax
        mov    eax, ecx
        mov    ecx, edx
        mov    edx, dword ptr [ebx + 10h]
        or     edx, edx
        je     SHORT loc_4_068E
        cmp    edi, edx
        ja     SHORT loc_4_064D
loc_4_068E:
        mov    edx, dword ptr [ebx + 14h]
        call   dword ptr [ebx + 0ch]
        jmp    SHORT loc_4_064D
loc_4_0696:
        ret
        db  000h
sub_4_0698:
        push   eax
        mov    al, 0aeh
        db  0E8h, 09Dh, 008h, 0FCh, 0FFh
        pop    eax
        ret
sub_4_06A2:
        push   eax
        mov    al, 0adh
        db  0E8h, 093h, 008h, 0FCh, 0FFh
        pop    eax
        ret
_PTEXT ends
        end
