; dsound_lcode_01.asm -- DSOUND object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 046h, 0D9h, 0D4h, 047h, 0E8h, 062h, 0CFh, 011h
        db  093h, 0BCh, 044h, 045h, 053h, 054h, 000h, 000h, 083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h
        db  0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h
        db  0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 0C0h, 033h, 042h, 08Ch, 0CCh, 0B4h, 0CEh, 011h
        db  092h, 094h, 044h, 045h, 053h, 054h, 000h, 000h, 0C1h, 033h, 042h, 08Ch, 0CCh, 0B4h, 0CEh, 011h
        db  092h, 094h, 044h, 045h, 053h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 07Eh, 035h, 004h, 002h, 000h, 000h, 044h, 053h, 04Fh, 055h
        db  04Eh, 044h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
DSOUND_Control:
        cmp    eax, 1bh
        je     SHORT loc_1_0147
        cmp    eax, 1ch
        je     SHORT loc_1_0150
        cmp    eax, 23h
        je     SHORT loc_1_0160
        clc
        ret
DSOUND_PM_API:
        clc
        ret
loc_1_0147:
        call   sub_1_01E0
        sub    eax, 1
        ret
loc_1_0150:
        call   sub_1_01E5
        sub    eax, 1
        ret
DSOUND_Service_00:
        mov    eax, 402h
        clc
        ret
loc_1_0160:
        cmp    ecx, 1eh
        jb     SHORT loc_1_0178
        xor    eax, eax
        cmp    ecx, -1
        jne    SHORT loc_1_0170
        ret
loc_1_0170:
        mov    eax, ecx
        mov    eax, 0ffffffffh
        ret
loc_1_0178:
        push   edi
        push   esi
        push   ebx
        db  089h, 035h, 000h, 000h, 000h, 000h
        push   esi
        db  0FFh, 014h, 08Dh, 000h, 000h, 000h, 000h
        add    esp, 4
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_0190:
        xor    eax, eax
        mov    ecx, 402h
        ret
loc_1_0198:
        mov    eax, edx
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        ret
loc_1_01A1:
        push   ebx
        push   esi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        pop    esi
        pop    ebx
        ret
sub_1_01B7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        jne    SHORT loc_1_01CD
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        jmp    SHORT loc_1_01DD
loc_1_01CD:
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        mov    eax, esi
loc_1_01DD:
        pop    esi
        leave
        ret
sub_1_01E0:
        db  0E9h, 000h, 000h, 000h, 000h
sub_1_01E5:
        db  0E9h, 000h, 000h, 000h, 000h
        db  0CCh, 0CCh
_LTEXT ends
        end
