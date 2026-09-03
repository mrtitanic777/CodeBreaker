; v86mmgr_pcode_05.asm -- V86MMGR object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

V86MMGR_Get_Xlat_Buff_State:
        db  0A1h, 0FCh, 011h, 000h, 000h
        add    eax, ebx
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, dword ptr [eax + 18h]
        movzx  eax, word ptr [eax + 1eh]
        ret
        db  000h, 000h
_PTEXT ends
        end
