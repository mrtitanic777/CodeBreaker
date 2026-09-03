; vpd_pcode_06.asm -- VPD object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_6_0000:
        mov    edx, dword ptr [esp + 0ch]
        inc    edx
        in     al, dx
        mov    ch, al
        inc    edx
        in     al, dx
        mov    cl, al
        xor    eax, eax
        cmp    cl, 0ffh
        je     SHORT loc_6_0043
        cmp    cl, ch
        je     SHORT loc_6_0043
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 0ch]
loc_6_001D:
        movzx  edx, word ptr [ecx*2 + 408h]
        cmp    eax, edx
        je     SHORT loc_6_0043
        test   edx, edx
        je     SHORT loc_6_003B
        test   dh, 0ffh
        je     SHORT loc_6_0043
        inc    cl
        cmp    cl, 2
        jbe    SHORT loc_6_001D
        jmp    SHORT loc_6_0043
loc_6_003B:
        mov    word ptr [ecx*2 + 408h], ax
loc_6_0043:
        ret
_PTEXT ends
        end
