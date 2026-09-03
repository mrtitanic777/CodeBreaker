; vcdfsd_pcode_06.asm -- VCDFSD object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_0000:
        push   eax
        db  066h, 081h, 03Dh, 0C4h, 000h, 000h, 000h, 010h, 001h
        jg     SHORT loc_6_0050
        push   0
        push   65h
        db  00Fh, 0B7h, 005h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0CDh, 020h, 026h, 000h, 040h, 000h
        lea    esp, [esp + 0ch]
        push   0
        push   67h
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0CDh, 020h, 026h, 000h, 040h, 000h
        lea    esp, [esp + 0ch]
loc_6_0050:
        clc
        pop    eax
        ret
VCDFSD_Service_02:
        db  03Ah, 00Dh, 0CEh, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 6], 2
        jb     SHORT loc_6_0094
        cmp    word ptr [eax + 4], 20h
        jb     SHORT loc_6_0094
        push   eax
        push   0
        call   dword ptr [eax + 8]
        lea    esp, [esp + 8]
        or     eax, eax
        jne    SHORT loc_6_0094
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 0CCh, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 0CCh, 000h, 000h, 000h
loc_6_008A:
        xor    eax, eax
        ret
loc_6_008D:
        cmp    dword ptr [esp + 4], 2
        jbe    SHORT loc_6_008A
loc_6_0094:
        mov    eax, 5
        ret
VCDFSD_Service_03:
        db  03Ah, 00Dh, 0CEh, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        db  08Dh, 015h, 0CCh, 000h, 000h, 000h
        jmp    SHORT loc_6_00AE
loc_6_00AC:
        mov    edx, dword ptr [edx]
loc_6_00AE:
        cmp    dword ptr [edx], edx
        je     SHORT loc_6_0094
        cmp    dword ptr [edx], eax
        jne    SHORT loc_6_00AC
        push   eax
        push   edx
        push   eax
        push   1
        call   dword ptr [eax + 8]
        lea    esp, [esp + 8]
        or     eax, eax
        pop    edx
        pop    eax
        jne    SHORT loc_6_0094
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        xor    eax, eax
loc_6_00CE:
        ret
loc_6_00CF:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        movzx  ecx, byte ptr [eax + 2]
        cmp    cl, 80h
        je     SHORT loc_6_014D
        ja     SHORT loc_6_00FC
        cmp    cl, 3
        je     SHORT loc_6_010D
        cmp    cl, 7
        je     SHORT loc_6_014D
        cmp    cl, 0ch
        je     SHORT loc_6_010D
        jb     SHORT loc_6_0155
        cmp    cl, 0eh
        jbe    SHORT loc_6_014D
        jmp    SHORT loc_6_0155
loc_6_00FC:
        cmp    cl, 81h
        je     SHORT loc_6_0155
        cmp    cl, 85h
        jbe    SHORT loc_6_014D
        cmp    cl, 88h
        je     SHORT loc_6_014D
        jmp    SHORT loc_6_0155
loc_6_010D:
        push   ecx
        movzx  edx, word ptr [eax + 10h]
        movzx  ecx, word ptr [eax + 0eh]
        shl    edx, 4
        add    edx, ecx
        add    edx, dword ptr [ebx + 4]
        pop    ecx
        cmp    cl, 3
        movzx  ecx, byte ptr [edx]
        je     SHORT loc_6_012E
        cmp    cl, 5
        jbe    SHORT loc_6_014D
        jmp    SHORT loc_6_0155
loc_6_012E:
        jecxz  loc_6_013C
        cmp    cl, 2
        je     SHORT loc_6_0155
        cmp    cl, 0fh
        jbe    SHORT loc_6_014D
        jmp    SHORT loc_6_0155
loc_6_013C:
        db  08Bh, 00Dh, 0C8h, 000h, 000h, 000h
        mov    dword ptr [edx + 1], ecx
        mov    word ptr [eax + 3], 100h
        jmp    SHORT loc_6_015D
loc_6_014D:
        mov    word ptr [eax + 3], 8101h
        jmp    SHORT loc_6_015D
loc_6_0155:
        mov    word ptr [eax + 3], 8103h
        jmp    SHORT loc_6_015D
loc_6_015D:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
_PTEXT ends
        end
