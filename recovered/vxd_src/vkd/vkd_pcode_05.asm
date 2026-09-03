; vkd_pcode_05.asm -- VKD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VKD_SYS_VM_TERMINATE:
        movzx  eax, word ptr [417h]
        db  0A3h, 02Dh, 012h, 000h, 000h
        clc
        ret
VKD_SYSTEM_EXIT:
        db  0A1h, 018h, 012h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_001D
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_5_001D:
        db  0A1h, 02Dh, 012h, 000h, 000h
        mov    word ptr [417h], ax
        db  0E8h, 0D3h, 0FFh, 0FAh, 0FFh
        call   sub_5_005C
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        db  0E8h, 056h, 006h, 0FBh, 0FFh
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        db  0E9h, 031h, 000h, 0FBh, 0FFh
VKD_SYS_CRITICAL_EXIT:
        mov    al, 0aeh
        db  0E8h, 0E4h, 00Eh, 0FBh, 0FFh
        clc
        ret
        db  000h
sub_5_005C:
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        jne    NEAR PTR loc_5_0125
        db  0C6h, 005h, 014h, 015h, 000h, 000h, 0FFh
        db  066h, 0C7h, 005h, 0FCh, 014h, 000h, 000h, 000h, 000h
        db  0A1h, 0FEh, 014h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0C6h, 005h, 013h, 015h, 000h, 000h, 0FFh
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0c201h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        db  0A1h, 00Ch, 015h, 000h, 000h
        add    eax, ebx
        cmp    byte ptr [eax + 4], 0ffh
        je     SHORT loc_5_0109
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_5_0109
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 1ch], 0c207h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        mov    word ptr [ebp + 1ch], 0c200h
        mov    word ptr [ebp + 10h], 100h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
loc_5_0109:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0C6h, 005h, 013h, 015h, 000h, 000h, 000h
loc_5_0125:
        ret
_PTEXT ends
        end
