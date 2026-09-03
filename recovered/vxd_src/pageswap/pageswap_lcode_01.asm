; pageswap_lcode_01.asm -- PAGESWAP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

PAGESWAP_Control:
        cmp    eax, 5
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
PageSwap_Get_Version:
        db  08Ah, 01Dh, 0D4h, 000h, 000h, 000h
        mov    eax, 400h
        db  08Bh, 00Dh, 0D0h, 000h, 000h, 000h
        clc
        ret
PageSwap_Test_IO_Valid:
        db  080h, 03Dh, 0D4h, 000h, 000h, 000h, 001h
        jbe    SHORT loc_1_002D
        db  0CDh, 020h, 006h, 080h, 021h, 000h
loc_1_002D:
        clc
        ret
PageSwap_Read_Or_Write:
        enter  10h, 0
        push   ebx
        lea    ebx, [ebp - 10h]
        mov    byte ptr [ebx + 6], 0ffh
        mov    byte ptr [ebx + 4], ch
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 0ch], edi
        mov    byte ptr [ebx + 7], al
        db  0CDh, 020h, 004h, 000h, 021h, 000h
        movzx  eax, byte ptr [ebx + 6]
        dec    eax
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 00Bh, 000h, 000h, 000h, 011h, 000h, 000h, 000h, 011h, 000h, 000h, 000h, 011h
        db  000h, 000h, 000h, 011h, 000h, 000h, 000h, 011h, 000h, 000h, 000h, 01Eh, 000h, 000h, 000h, 02Fh
        db  000h, 000h, 000h, 013h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 007h, 000h, 002h, 00Ah, 000h, 000h, 050h, 041h, 047h, 045h
        db  053h, 057h, 041h, 050h, 000h, 000h, 000h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  058h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
