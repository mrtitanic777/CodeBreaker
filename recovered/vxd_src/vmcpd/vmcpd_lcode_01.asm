; vmcpd_lcode_01.asm -- VMCPD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VMCPD_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 01Ah, 000h, 0FFh, 0FFh
        cmp    eax, 3
        je     NEAR PTR loc_1_03B1
        cmp    eax, 7
        je     NEAR PTR loc_1_03B1
        cmp    eax, 1dh
        je     NEAR PTR loc_1_03B1
        cmp    eax, 1eh
        je     NEAR PTR loc_1_03BD
        cmp    eax, 21h
        je     NEAR PTR loc_1_03DA
        cmp    eax, 0ch
        je     NEAR PTR loc_1_03D4
        cmp    eax, 5
        je     NEAR PTR loc_1_0420
        clc
        ret
VMCPD_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    ax, 1
        je     SHORT loc_1_006E
        cmp    ax, 0
        jne    SHORT loc_1_00A3
        db  00Fh, 0B7h, 005h, 01Ch, 008h, 000h, 000h
        xchg   al, ah
        jmp    SHORT loc_1_009A
loc_1_006E:
        db  08Bh, 00Dh, 040h, 008h, 000h, 000h
        mov    eax, 1
        test   ecx, 1
        je     SHORT loc_1_009A
        or     eax, 2
        test   ecx, 2
        je     SHORT loc_1_008F
        or     eax, 4
loc_1_008F:
        test   ecx, 8
        je     SHORT loc_1_009A
        or     eax, 8
loc_1_009A:
        mov    word ptr [ebp + 18h], ax
        and    dword ptr [ebp + 2ch], 0fffffffeh
loc_1_00A2:
        ret
loc_1_00A3:
        or     dword ptr [ebp + 2ch], 1
        jmp    SHORT loc_1_00A2
VMCPD_Get_Version:
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 010h, 000h, 000h, 000h
        jne    SHORT loc_1_00BA
        db  0E8h, 045h, 000h, 0FFh, 0FFh
loc_1_00BA:
        db  00Fh, 0B7h, 005h, 01Ch, 008h, 000h, 000h
        xchg   al, ah
        sub    ecx, ecx
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_00DE
        push   eax
        mov    cl, 3
        mov    eax, cr0
        test   al, 10h
        jne    SHORT loc_1_00DD
        dec    cl
loc_1_00DD:
        pop    eax
loc_1_00DE:
        clc
        ret
VMCPD_Get_Virt_State:
        push   edi
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 011h, 000h
        shr    eax, 1
        and    eax, 3
        pop    edi
        ret
VMCPD_Set_Virt_State:
        push   eax
        push   edx
        push   edi
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        and    eax, 3
        shl    eax, 1
        db  0CDh, 020h, 004h, 000h, 011h, 000h
        pop    edi
        pop    edx
        pop    eax
        ret
VMCPD_Get_CR0_State:
        push   edi
        mov    eax, edi
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_0122
        mov    edi, eax
        call   VMCPD_Service_09
loc_1_0122:
        mov    eax, dword ptr [edi]
        and    eax, 6
        db  08Bh, 03Dh, 064h, 008h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_0139
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        je     SHORT loc_1_013C
loc_1_0139:
        or     eax, 8
loc_1_013C:
        pop    edi
        ret
VMCPD_Set_CR0_State:
        mov    edx, edi
        db  003h, 015h, 070h, 008h, 000h, 000h
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_0157
        push   edi
        push   eax
        call   VMCPD_Service_09
        mov    edx, edi
        pop    eax
        pop    edi
loc_1_0157:
        and    eax, 6
        and    dword ptr [edx], 0fffffff9h
        or     dword ptr [edx], eax
        test   eax, 4
        je     SHORT loc_1_0187
        and    dword ptr [edx], 0ffffff77h
        db  03Bh, 03Dh, 064h, 008h, 000h, 000h
        jne    SHORT loc_1_0187
        db  0C7h, 005h, 064h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_0187:
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        jne    SHORT loc_1_0196
        push   edi
        call   sub_1_0280
        pop    edi
loc_1_0196:
        ret
VMCPD_Get_Thread_State:
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_01F4
        push   ecx
        push   esi
        push   edi
        mov    ecx, edi
        db  003h, 00Dh, 070h, 008h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_01BD
        push   edi
        call   VMCPD_Service_09
        mov    ecx, edi
        pop    edi
loc_1_01BD:
        test   dword ptr [ecx], 80h
        jne    SHORT loc_1_01CF
        call   sub_1_030B
        call   sub_1_02A5
loc_1_01CF:
        db  03Bh, 03Dh, 064h, 008h, 000h, 000h
        jne    SHORT loc_1_01E5
        call   sub_1_030B
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_01E5:
        mov    edi, esi
        lea    esi, [ecx + 4]
        mov    ecx, 1bh
        rep movsd
        pop    edi
        pop    esi
        pop    ecx
loc_1_01F4:
        ret
VMCPD_Set_Thread_State:
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_024C
        push   ecx
        push   esi
        push   edi
        mov    ecx, edi
        db  003h, 00Dh, 070h, 008h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_021B
        push   edi
        call   VMCPD_Service_09
        mov    ecx, edi
        pop    edi
loc_1_021B:
        and    dword ptr [ecx], 0fffffff7h
        or     dword ptr [ecx], 80h
        db  03Bh, 03Dh, 064h, 008h, 000h, 000h
        jne    SHORT loc_1_023F
        db  0C7h, 005h, 064h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_023F:
        lea    edi, [ecx + 4]
        mov    ecx, 1bh
        rep movsd
        pop    edi
        pop    esi
        pop    ecx
loc_1_024C:
        ret
VMCPD_Set_Thread_Precision:
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_027F
        push   ecx
        mov    ecx, edi
        db  003h, 00Dh, 070h, 008h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_0273
        push   edi
        push   eax
        call   VMCPD_Service_09
        pop    eax
        mov    ecx, edi
        pop    edi
loc_1_0273:
        and    eax, 1
        shl    eax, 4
        and    dword ptr [ecx], 0ffffffefh
        or     dword ptr [ecx], eax
        pop    ecx
loc_1_027F:
        ret
sub_1_0280:
        mov    edx, edi
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_0295
        mov    edi, edx
        call   VMCPD_Service_09
loc_1_0295:
        mov    edx, dword ptr [edi]
        and    dl, 0eh
        mov    eax, cr0
        and    al, 0f9h
        or     al, dl
        mov    cr0, eax
        ret
sub_1_02A5:
        db  089h, 03Dh, 064h, 008h, 000h, 000h
        push   edi
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_02C0
        mov    edi, dword ptr [esp]
        call   VMCPD_Service_09
loc_1_02C0:
        clts
        bts    dword ptr [edi], 7
        jae    SHORT loc_1_02D5
        frstor dword ptr [edi + 4]
        test   dword ptr [edi], 8
        jne    SHORT loc_1_0300
loc_1_02D3:
        pop    edi
        ret
loc_1_02D5:
        or     dword ptr [edi], 100h
        fninit
        test   dword ptr [edi], 10h
        je     SHORT loc_1_02F7
        fnstcw word ptr [edi + 4]
        and    word ptr [edi + 4], 0fcffh
        or     word ptr [edi + 4], 200h
        fldcw  word ptr [edi + 4]
loc_1_02F7:
        wait
        and    dword ptr [edi], 0fffffeffh
        jmp    SHORT loc_1_02D3
loc_1_0300:
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
        jmp    SHORT loc_1_02D3
sub_1_030B:
        push   edi
        db  08Bh, 03Dh, 064h, 008h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_0345
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_032A
        mov    edi, dword ptr [esp]
        call   VMCPD_Service_09
loc_1_032A:
        or     dword ptr [edi], 100h
        clts
        fnsave dword ptr [edi + 4]
        db  0C7h, 005h, 064h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        and    dword ptr [edi], 0fffffeffh
loc_1_0345:
        pop    edi
        ret
loc_1_0347:
        mov    edx, dword ptr [edi]
        and    edx, 10000000h
        je     SHORT loc_1_038D
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_03AB
        push   edi
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_1_03A1
        inc    dword ptr [edi - 4]
        mov    edx, dword ptr [edi - 4]
        pop    edi
        db  039h, 03Dh, 064h, 008h, 000h, 000h
        je     SHORT loc_1_039D
        cmp    edx, 100h
        jg     SHORT loc_1_03A2
        call   sub_1_030B
        call   sub_1_02A5
        jmp    SHORT loc_1_03AB
loc_1_038D:
        db  03Bh, 005h, 064h, 008h, 000h, 000h
        je     SHORT loc_1_03A2
        db  03Bh, 03Dh, 064h, 008h, 000h, 000h
        jne    SHORT loc_1_03AB
loc_1_039D:
        clts
        jmp    SHORT loc_1_03AB
loc_1_03A1:
        pop    edi
loc_1_03A2:
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_03AB:
        call   sub_1_0280
        ret
loc_1_03B1:
        xor    eax, eax
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    dword ptr [edi], eax
        clc
        ret
loc_1_03BD:
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    eax, dword ptr [edi]
        or     eax, eax
        jne    SHORT loc_1_03D2
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_03D2:
        clc
        ret
loc_1_03D4:
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
loc_1_03DA:
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_0401
        db  03Bh, 03Dh, 064h, 008h, 000h, 000h
        jne    SHORT loc_1_0401
        db  0C7h, 005h, 064h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_0401:
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_041E
        sub    eax, 4
        push   esi
        db  08Bh, 035h, 074h, 008h, 000h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        pop    esi
loc_1_041E:
        clc
        ret
loc_1_0420:
        mov    eax, cr0
        and    eax, 0ffffffe1h
        db  00Bh, 005h, 068h, 008h, 000h, 000h
        mov    cr0, eax
        db  066h, 0A1h, 010h, 000h, 000h, 000h
        and    al, 2
        or     byte ptr [410h], al
        clc
        ret
sub_1_043F:
        push   esi
        db  08Bh, 035h, 070h, 008h, 000h, 000h
        mov    esi, dword ptr [esi + edi]
        push   ebp
        db  0CDh, 020h, 008h, 000h, 02Ah, 000h
        add    esp, 4
        cmp    eax, 1
        je     SHORT loc_1_0468
        clts
        and    dword ptr [esi], 0fffffff7h
        cmp    eax, 2
        je     SHORT loc_1_0468
        db  0CDh, 020h, 006h, 000h, 02Ah, 000h
loc_1_0468:
        clc
        pop    esi
        ret
sub_1_046B:
        stc
loc_1_046C:
        lodsb
loc_1_046D:
        clc
loc_1_046E:
        ret
        db  02Eh, 036h, 03Eh, 026h, 064h, 065h, 066h, 067h
_VMCPD_Get_FP_Instruction_Size:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [ebp + 0ch]
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_04A3
        shl    eax, 4
        add    esi, eax
        add    esi, dword ptr [ebx + 4]
        mov    ebx, esi
        mov    edx, 1
        jmp    SHORT loc_1_04DF
loc_1_04A3:
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_1_057E
        add    esi, eax
        mov    ebx, esi
        lar    eax, word ptr [ebp + 0ch]
        jne    NEAR PTR loc_1_057E
        and    ax, 1800h
        cmp    ax, 1800h
        jne    NEAR PTR loc_1_057E
        sub    edx, edx
        test   eax, 400000h
        jne    SHORT loc_1_04DF
        inc    edx
loc_1_04DF:
        call   sub_1_046B
        jb     NEAR PTR loc_1_057E
        cmp    al, 9bh
        je     NEAR PTR loc_1_0575
        db  0BFh, 06Fh, 004h, 000h, 000h
        mov    ecx, 8
        cld
        repne scasb
        jne    SHORT loc_1_050A
        cmp    al, 67h
        jne    SHORT loc_1_04DF
        xor    edx, 1
        jmp    SHORT loc_1_04DF
loc_1_050A:
        mov    ah, al
        and    al, 0f8h
        cmp    al, 0d8h
        jne    SHORT loc_1_057E
        call   sub_1_046B
        jb     SHORT loc_1_057E
        cmp    ax, 0dfe0h
        jne    SHORT loc_1_052E
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    edi, dword ptr [edi + 10h]
        mov    word ptr [edi + 1ch], 0ffffh
loc_1_052E:
        cmp    al, 0c0h
        jae    SHORT loc_1_0575
        mov    cl, al
        mov    ch, al
        shr    cl, 6
        and    ch, 7
        or     dl, dl
        je     SHORT loc_1_054B
        cmp    cx, 600h
        jne    SHORT loc_1_0570
        mov    cl, 2
        jmp    SHORT loc_1_0570
loc_1_054B:
        cmp    cx, 500h
        je     SHORT loc_1_056E
        cmp    ch, 4
        jne    SHORT loc_1_0569
        call   sub_1_046B
        jb     SHORT loc_1_057E
        and    al, 7
        mov    ch, al
        cmp    cx, 500h
        je     SHORT loc_1_056E
loc_1_0569:
        cmp    cl, 2
        jne    SHORT loc_1_0570
loc_1_056E:
        mov    cl, 4
loc_1_0570:
        movzx  ecx, cl
        add    esi, ecx
loc_1_0575:
        sub    esi, ebx
        mov    eax, esi
loc_1_0579:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_057E:
        xor    eax, eax
        jmp    SHORT loc_1_0579
        db  0EBh, 006h, 0FFh, 025h, 078h, 008h, 000h, 000h
loc_1_058A:
        call   sub_1_05E5
        jb     SHORT loc_1_0592
        ret
loc_1_0592:
        db  0FFh, 025h, 078h, 008h, 000h, 000h
        db  0EBh, 006h, 0FFh, 025h, 080h, 008h, 000h, 000h
loc_1_05A0:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, cr0
        push   eax
        and    eax, 0fffffffbh
        mov    cr0, eax
        or     ecx, 0ffffffffh
        call   sub_1_05E5
        pop    edx
        pop    edi
        jb     SHORT loc_1_05C9
        and    edx, 4
        mov    eax, cr0
        or     eax, edx
        mov    cr0, eax
        ret
loc_1_05C9:
        db  0FFh, 025h, 080h, 008h, 000h, 000h
        db  0EBh, 006h, 0FFh, 025h, 07Ch, 008h, 000h, 000h
loc_1_05D7:
        call   sub_1_05E5
        jb     SHORT loc_1_05DF
        ret
loc_1_05DF:
        db  0FFh, 025h, 07Ch, 008h, 000h, 000h
sub_1_05E5:
        mov    eax, cr0
        bt     eax, 2
        jb     SHORT loc_1_0667
        db  00Fh, 0BAh, 025h, 040h, 008h, 000h, 000h, 000h
        cmc
        jb     SHORT loc_1_0643
        sti
        db  00Fh, 0BAh, 025h, 040h, 008h, 000h, 000h, 001h
        jae    SHORT loc_1_064D
        db  03Bh, 03Dh, 064h, 008h, 000h, 000h
        je     SHORT loc_1_061D
        push   ecx
        call   sub_1_0684
        call   sub_1_030B
        pop    ecx
        call   sub_1_02A5
loc_1_061D:
        db  0A1h, 070h, 008h, 000h, 000h
        mov    eax, dword ptr [edi + eax]
        test   dword ptr [eax], 8
        jne    SHORT loc_1_0644
loc_1_062D:
        push   edi
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_1_0641
        mov    dword ptr [edi - 4], 0
loc_1_0641:
        pop    edi
        clc
loc_1_0643:
        ret
loc_1_0644:
        push   edi
        call   sub_1_043F
        pop    edi
        jmp    SHORT loc_1_062D
loc_1_064D:
        call   sub_1_0684
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        db  0CDh, 020h, 007h, 000h, 011h, 000h
        add    esp, 8
        add    dword ptr [ebp + 24h], eax
        clc
        jmp    SHORT loc_1_0643
loc_1_0667:
        call   sub_1_0684
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        stc
        jne    SHORT loc_1_0643
        sti
        push   edi
        push   ebp
        db  0CDh, 020h, 007h, 000h, 02Ah, 000h
        add    esp, 8
        clc
        jmp    SHORT loc_1_0643
sub_1_0684:
        push   edi
        push   eax
        cmp    ecx, -1
        je     SHORT loc_1_0693
        test   dword ptr [edi], 10000000h
        je     SHORT loc_1_06CD
loc_1_0693:
        mov    eax, cr0
        bt     eax, 2
        jb     SHORT loc_1_06C5
        db  00Fh, 0BAh, 025h, 040h, 008h, 000h, 000h, 000h
        jae    SHORT loc_1_06C7
        db  00Fh, 0BAh, 025h, 040h, 008h, 000h, 000h, 001h
        jae    SHORT loc_1_06C7
        mov    eax, edi
        db  003h, 005h, 070h, 008h, 000h, 000h
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_06C3
        call   VMCPD_Service_09
loc_1_06C3:
        jmp    SHORT loc_1_06CD
loc_1_06C5:
        jmp    SHORT loc_1_06C7
loc_1_06C7:
        db  0CDh, 020h, 018h, 000h, 02Ah, 000h
loc_1_06CD:
        pop    eax
        pop    edi
        ret
loc_1_06D0:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, eax
        sub    al, al
        out    0f0h, al
        mov    eax, ebx
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  03Bh, 03Dh, 064h, 008h, 000h, 000h
        jne    SHORT loc_1_0735
        db  08Bh, 035h, 070h, 008h, 000h, 000h
        mov    esi, dword ptr [edi + esi]
        test   dword ptr [esi], 148h
        jne    SHORT loc_1_0735
        btr    dword ptr [esi], 5
        jb     SHORT loc_1_0735
        db  0CDh, 020h, 01Bh, 000h, 02Ah, 000h
        or     eax, eax
        je     SHORT loc_1_071C
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
        or     dword ptr [esi], 28h
        jmp    SHORT loc_1_0735
loc_1_071C:
        or     dword ptr [esi], 40h
        mov    edx, ebx
        mov    ebx, edi
        sub    eax, eax
        mov    ecx, 148h
        db  0BEh, 038h, 007h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_0735:
        pop    edi
        clc
        ret
loc_1_0738:
        mov    ecx, 1
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    eax, edx
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        ret
loc_1_074C:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        ret
loc_1_0753:
        push   edi
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        and    dword ptr [edi], 0ffffffbfh
        pop    edi
        ret
VMCPD_Service_09:
        push   ecx
        push   edi
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_1_0781
        mov    edi, dword ptr [esp]
        push   ecx
        jmp    SHORT loc_1_07D7
loc_1_0781:
        push   esi
        db  08Bh, 035h, 074h, 008h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        pop    esi
        jae    SHORT loc_1_0795
        xor    edi, edi
        jmp    SHORT loc_1_07E0
loc_1_0795:
        xchg   edi, eax
        mov    ecx, 1dh
        push   eax
        push   edi
        xor    eax, eax
        rep stosd
        pop    edi
        pop    eax
        xchg   edi, eax
        add    eax, 4
        mov    dword ptr [edi], eax
        mov    edi, eax
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_07E0
        db  083h, 03Dh, 064h, 008h, 000h, 000h, 000h
        jne    SHORT loc_1_07DF
        push   edi
        mov    edi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, -1
        jne    SHORT loc_1_07D7
        mov    eax, cr0
        and    eax, 0fffffffbh
        mov    cr0, eax
loc_1_07D7:
        clts
        call   sub_1_02A5
        pop    ecx
loc_1_07DF:
        clc
loc_1_07E0:
        pop    ecx
        pop    ecx
        ret
        db  000h, 0A9h, 000h, 000h, 000h, 0E0h, 000h, 000h, 000h, 0F4h, 000h, 000h, 000h, 00Ch, 001h, 000h
        db  000h, 03Eh, 001h, 000h, 000h, 097h, 001h, 000h, 000h, 0F5h, 001h, 000h, 000h, 077h, 004h, 000h
        db  000h, 04Dh, 002h, 000h, 000h, 06Dh, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 08Ah, 000h, 000h
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 011h, 000h, 004h, 002h, 000h, 000h, 056h, 04Dh, 043h, 050h
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 048h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E4h, 007h, 000h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_0878:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_087C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0880:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
