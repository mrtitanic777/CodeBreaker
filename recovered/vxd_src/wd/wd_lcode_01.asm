; wd_lcode_01.asm -- WD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

WD_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_007D:
        mov    edi, ebx
        db  003h, 03Dh, 050h, 001h, 000h, 000h
        test   byte ptr [edi + 1], 2
        je     SHORT loc_1_00A5
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4806h
        out    dx, ax
        mov    al, 27h
        mov    ah, byte ptr [edi + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_1_00A5:
        test   byte ptr [edi + 1], 1
        je     SHORT loc_1_00FB
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    byte ptr [edi + 2], 0a6h
        mov    al, 32h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        inc    al
        out    dx, al
        jmp    SHORT loc_1_00C5
loc_1_00C5:
        in     al, dx
        dec    al
        out    dx, al
        dec    dl
        cmp    al, ah
        je     SHORT loc_1_00D9
        mov    byte ptr [edi + 2], 0
        mov    ax, 0a034h
        out    dx, ax
loc_1_00D9:
        mov    al, 32h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, byte ptr [edi]
        and    ax, 0cf3h
        or     al, ah
        out    dx, al
        dec    dl
        cmp    byte ptr [edi + 2], 0
        jne    SHORT loc_1_00F7
        mov    al, 34h
        mov    ah, byte ptr [edi + 2]
        out    dx, ax
loc_1_00F7:
        rol    eax, 8
        out    dx, al
loc_1_00FB:
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 057h, 044h, 020h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0159:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
