; vkd_pcode_07.asm -- VKD object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_7_0000:
        mov    dword ptr [esi + 43h], 5
        mov    dword ptr [esi + 47h], 19h
        mov    dword ptr [esi + 4bh], 3
        mov    dword ptr [esi + 4fh], 0c8h
        mov    dword ptr [esi + 53h], 3e8h
        mov    byte ptr [esi + 57h], 2
        mov    byte ptr [esi + 58h], 0ah
        ret
VKD_VM_CRITICAL_INIT:
        call   sub_7_00F0
        db  003h, 01Dh, 010h, 012h, 000h, 000h
        cmp    byte ptr [ebx + 59h], 0
        jne    SHORT loc_7_0044
        mov    esi, ebx
        call   0
loc_7_0044:
        or     dword ptr [ebx], 100000h
        db  0A0h, 059h, 012h, 000h, 000h
        mov    byte ptr [ebx + 3ch], al
        push   ebx
        db  02Bh, 01Dh, 010h, 012h, 000h, 000h
        db  0E8h, 0C4h, 008h, 0F9h, 0FFh
        pop    ebx
        mov    word ptr [ebx + 36h], ax
        mov    dl, al
        and    dl, 70h
        mov    byte ptr [ebx + 41h], dl
        mov    esi, ebx
        mov    edx, ebx
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        db  0E8h, 0DBh, 005h, 0F9h, 0FFh
        je     SHORT loc_7_0081
        db  0E8h, 02Fh, 001h, 0F9h, 0FFh
loc_7_0081:
        clc
        ret
loc_7_0083:
        pushad
        mov    eax, 0c06h
        mov    dl, 0ffh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  0E8h, 072h, 003h, 0F9h, 0FFh
        popad
        clc
        ret
loc_7_009B:
        db  0A1h, 010h, 012h, 000h, 000h
        add    eax, ebx
        movzx  ecx, word ptr [edx + 4]
        mov    dword ptr [eax + 43h], ecx
        movzx  ecx, word ptr [edx + 6]
        mov    dword ptr [eax + 47h], ecx
        movzx  ecx, word ptr [edx + 8]
        mov    dword ptr [eax + 4bh], ecx
        movzx  ecx, word ptr [edx + 0ah]
        mov    dword ptr [eax + 4fh], ecx
        movzx  ecx, word ptr [edx + 0ch]
        mov    dword ptr [eax + 53h], ecx
        movzx  ecx, byte ptr [edx + 0eh]
        cmp    ecx, 1
        adc    ecx, 0
        mov    byte ptr [eax + 57h], cl
        mov    cl, byte ptr [edx + 10h]
        cmp    ecx, 1
        adc    ecx, 0
        mov    byte ptr [eax + 58h], cl
        mov    byte ptr [eax + 59h], 1
        test   byte ptr [edx], 1
        sete   al
        db  0CDh, 020h, 009h, 000h, 00Dh, 000h
        ret
        db  000h
sub_7_00F0:
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_7_010B
        push   ebx
        db  003h, 01Dh, 00Ch, 015h, 000h, 000h
        mov    dword ptr [ebx], 0
        mov    byte ptr [ebx + 5], 0
        pop    ebx
loc_7_010B:
        clc
        ret
_PTEXT ends
        end
