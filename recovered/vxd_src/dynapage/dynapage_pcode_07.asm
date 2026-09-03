; dynapage_pcode_07.asm -- DYNAPAGE object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_7_0000:
        db  080h, 03Dh, 0F5h, 000h, 000h, 000h, 000h
        db  00Fh, 085h, 0F3h, 0FFh, 0F8h, 0FFh
        db  08Bh, 01Dh, 0F0h, 000h, 000h, 000h
        xor    ecx, ecx
        mov    eax, 0d601h
        push   edx
        shl    edx, 0ch
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        pop    edx
        jb     SHORT loc_7_002D
        db  089h, 015h, 088h, 000h, 000h, 000h
loc_7_002D:
        ret
PageFile_Grow_File:
        sub    esp, 4
        push   edx
        push   ecx
        db  08Bh, 015h, 088h, 000h, 000h, 000h
        add    edx, ecx
        js     NEAR PTR loc_7_00DC
        db  03Bh, 015h, 084h, 000h, 000h, 000h
        ja     SHORT loc_7_005B
        db  08Bh, 015h, 084h, 000h, 000h, 000h
        db  03Bh, 015h, 088h, 000h, 000h, 000h
        je     NEAR PTR loc_7_00DC
loc_7_005B:
        db  03Bh, 015h, 080h, 000h, 000h, 000h
        ja     NEAR PTR loc_7_00ED
        or     ecx, ecx
        jns    SHORT loc_7_0090
        db  08Bh, 00Dh, 08Ch, 000h, 000h, 000h
        dec    ecx
        lea    eax, [esp + 8]
        push   edx
        push   0
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 06Fh, 000h, 040h, 000h
        add    esp, 10h
        pop    edx
        cmp    dword ptr [esp + 8], -1
        jne    SHORT loc_7_00DC
        jmp    SHORT loc_7_00D7
loc_7_0090:
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, ecx
        mov    ax, 3601h
        db  08Bh, 015h, 08Ch, 000h, 000h, 000h
        db  080h, 03Dh, 0F5h, 000h, 000h, 000h, 000h
        je     SHORT loc_7_00B3
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jmp    SHORT loc_7_00B9
loc_7_00B3:
        db  0CDh, 020h, 032h, 000h, 040h, 000h
loc_7_00B9:
        mov    edx, dword ptr [esp]
        jb     SHORT loc_7_00D1
        imul   ecx, eax
        imul   ecx, ebx
        shr    ecx, 0ch
        sub    ecx, edx
        db  03Bh, 00Dh, 090h, 000h, 000h, 000h
        jl     SHORT loc_7_00ED
loc_7_00D1:
        db  003h, 015h, 088h, 000h, 000h, 000h
loc_7_00D7:
        call   0
loc_7_00DC:
        pop    ecx
        pop    edx
        db  0A1h, 088h, 000h, 000h, 000h
        db  08Bh, 01Dh, 080h, 000h, 000h, 000h
        add    esp, 4
        ret
loc_7_00ED:
        stc
        jmp    SHORT loc_7_00DC
PageFile_Get_Size_Info:
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, eax
        mov    ah, 36h
        db  08Bh, 015h, 08Ch, 000h, 000h, 000h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        imul   ecx, eax
        imul   ecx, ebx
        shr    ecx, 0ch
        db  0BEh, 000h, 000h, 000h, 000h
        db  0A1h, 088h, 000h, 000h, 000h
        db  08Bh, 01Dh, 080h, 000h, 000h, 000h
        add    ecx, eax
        db  08Bh, 015h, 084h, 000h, 000h, 000h
        cmp    ecx, ebx
        jb     SHORT loc_7_012D
        mov    ecx, ebx
loc_7_012D:
        ret
        db  025h, 050h
_PTEXT ends
        end
