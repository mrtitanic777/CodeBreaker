; S3MINI -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2668 instructions recovered by recursive descent

Declare_Virtual_Device S3MINI, 3, 1, S3MINI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x144c  R-X
S3MINI_Control:
        cmp    eax, 0x1c
        jae    loc_1_000C
        jmp    dword ptr [eax*4 + 0xe]       ; -> obj1:0x000E
loc_1_000C:
        clc    
        ret    
        ; ---- dispatch table (28 entries) ----
        dd  loc_2_0000
        dd  loc_2_001B
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_2_001B
loc_1_007E:
        cmp    ebx, dword ptr [0x1340]       ; -> obj1:0x1340
        je     loc_1_00DD
        cmp    edx, 0x9ae8
        je     loc_1_0096
        cmp    edx, 0x9ae9
        jne    loc_1_00DD
loc_1_0096:
        cmp    ecx, 0
        je     loc_1_00BE
        cmp    ecx, 8
        jne    loc_1_00DD
        mov    ecx, 0xffff
loc_1_00A5:
        in     ax, dx
        cmp    ax, -1
        je     loc_1_00B9
        test   ah, 2
        je     loc_1_00DA
        call   sub_4_0800
        loop   loc_1_00A5
loc_1_00B9:
        and    ah, 0xfd
        jmp    loc_1_00DA
loc_1_00BE:
        cmp    dl, 0xe9
        jne    loc_1_00DD
        mov    ecx, 0xffff
loc_1_00C8:
        in     al, dx
        cmp    al, 0xff
        je     loc_1_00D8
        test   al, 2
        je     loc_1_00DA
        call   sub_4_0800
        loop   loc_1_00C8
loc_1_00D8:
        and    al, 0xfd
loc_1_00DA:
        stc    
        jmp    loc_1_00DE
loc_1_00DD:
        clc    
loc_1_00DE:
        ret    
loc_1_00DF:
        cmp    dx, 0x4f00
        je     loc_1_00FC
        cmp    dx, 0x4f02
        jne    loc_1_0121
        cmp    ebx, dword ptr [0x1340]       ; -> obj1:0x1340
        je     loc_1_0121
        call   sub_4_0376
        jmp    loc_1_0121
loc_1_00FC:
        cmp    ebx, dword ptr [0x1340]       ; -> obj1:0x1340
        jne    loc_1_0121
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_1_0121:
        ret    
        db  000h, 000h
sub_1_0124:
        push   eax
        push   ebx
        push   ecx
        push   edx
        test   word ptr [eax + 2], 0x200
        jne    loc_1_0168
        movzx  ecx, word ptr [eax + 0x24]
        movzx  edx, word ptr [eax + 0x26]
        mov    ebx, 0x2f0                    ; -> loc_1_02F0
loc_1_013D:
        mov    al, byte ptr [ebx]
        cmp    dl, al
        jb     loc_1_0155
        push   ebx
        mov    eax, edi
        shr    eax, 0x10
        and    eax, 0xf
        add    ebx, eax
        mov    al, byte ptr [ebx]
        cmp    al, 0xff
        pop    ebx
        jne    loc_1_015C
loc_1_0155:
        add    ebx, 7
        or     al, al
        jne    loc_1_013D
loc_1_015C:
        xchg   bx, si
        cmp    bl, al
        jbe    loc_1_0165
        xchg   al, bl
loc_1_0165:
        mov    si, bx
loc_1_0168:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_1_016D:
        add    byte ptr [edx], 1
        add    byte ptr [eax], ah
        add    eax, dword ptr [edx]
        add    byte ptr [eax], al
        add    al, 3
        add    byte ptr [eax + 0x404], al
        add    eax, 0x6400005
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], al
        add    byte ptr [edx], dl            ; -> loc_1_0712
        pop    es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    al, 0
        retf   4
        db  000h, 040h, 000h, 000h, 000h, 000h, 002h, 002h, 000h
        dd  loc_1_05FC               ; fixup
        db  040h, 000h, 000h, 000h, 000h, 002h, 004h, 000h
        dd  loc_1_0398               ; fixup
        db  040h, 000h, 000h, 080h, 000h, 040h, 002h, 000h
        dd  loc_1_0712               ; fixup
        db  040h, 000h, 000h, 080h, 000h, 040h, 004h, 000h
        dd  loc_1_04CA               ; fixup
        db  040h, 000h, 000h, 080h, 000h, 002h, 002h, 000h
        dd  loc_1_05FC               ; fixup
        db  040h, 000h, 000h, 080h, 000h, 002h, 004h, 000h
        dd  loc_1_0398               ; fixup
        db  040h, 000h, 000h, 000h, 080h, 000h, 002h, 000h
        dd  loc_1_0712               ; fixup
        db  040h, 000h, 000h, 000h, 080h, 000h, 004h, 000h
        dd  loc_1_04CA               ; fixup
        db  040h, 000h, 000h, 000h, 000h, 010h, 002h, 000h
        dd  loc_1_05FC               ; fixup
        db  040h, 000h, 000h, 000h, 000h, 010h, 004h, 000h
        dd  loc_1_0398               ; fixup
        db  040h, 000h, 000h, 080h, 080h, 000h, 002h, 000h
        dd  loc_1_0712               ; fixup
        db  040h, 000h, 000h, 080h, 080h, 000h, 004h, 000h
        dd  loc_1_04CA               ; fixup
        db  040h, 000h, 000h, 080h, 000h, 010h, 002h, 000h
        dd  loc_1_05FC               ; fixup
        db  040h, 000h, 000h, 080h, 000h, 010h, 004h, 000h
        dd  loc_1_0398               ; fixup
        db  040h, 000h, 000h, 000h, 000h, 001h, 002h, 000h
        dd  loc_1_0828               ; fixup
        db  040h, 000h, 000h, 000h, 000h, 001h, 004h, 000h
        dd  loc_1_083E               ; fixup
        db  002h, 000h, 000h, 000h, 000h, 001h, 002h, 000h
        dd  loc_1_0955               ; fixup
        db  002h, 000h, 000h, 000h, 000h, 001h, 004h, 000h
        dd  obj1:0x0854              ; fixup
        db  008h, 000h, 000h, 000h, 000h, 004h, 002h, 000h
        dd  loc_1_0DCC               ; fixup
        db  008h, 000h, 000h, 000h, 000h, 004h, 001h, 000h
        dd  loc_1_0ED4               ; fixup
        db  001h, 000h, 000h, 000h, 000h, 004h, 002h, 000h
        dd  loc_1_0A56               ; fixup
        db  001h, 000h, 000h, 000h, 000h, 004h, 001h, 000h
        dd  loc_1_0B5E               ; fixup
        db  001h, 000h, 000h, 040h, 000h, 004h, 002h, 000h
        dd  loc_1_0C11               ; fixup
        db  001h, 000h, 000h, 040h, 000h, 004h, 001h, 000h
        dd  loc_1_0D19               ; fixup
        db  020h, 000h, 000h, 000h, 000h, 004h, 002h, 000h
        dd  loc_1_0F87               ; fixup
        db  020h, 000h, 000h, 000h, 000h, 004h, 001h, 000h
        dd  loc_1_107A               ; fixup
        db  004h, 000h, 000h, 000h, 000h, 004h, 002h, 000h
        dd  loc_1_1126               ; fixup
        db  004h, 000h, 000h, 000h, 000h, 004h, 001h, 000h
        dd  loc_1_11E1               ; fixup
        db  000h
loc_1_02F0:
        pushal 
        db  0FFh, 0FFh, 078h, 0FFh, 0FFh, 04Ch, 05Fh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 04Bh, 059h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 048h, 052h, 0FFh, 0FFh, 0FFh, 05Ah, 0FFh, 042h, 051h, 0FFh, 0FFh, 064h, 0FFh
        db  0FFh, 0FFh, 04Fh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 0FFh, 04Eh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ah, 0FFh
        db  04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 0FFh, 04Bh, 0FFh, 078h, 0FFh, 0FFh, 0FFh, 0FFh, 04Ah, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 047h, 0FFh, 0FFh, 0FFh, 04Bh, 0FFh, 0FFh, 040h, 078h, 064h, 050h
        db  046h, 03Ch, 0FFh, 03Ch, 0FFh, 0FFh, 04Bh, 0FFh, 0FFh, 0FFh, 03Ah, 0FFh, 0FFh, 048h, 0FFh, 0FFh
        db  0FFh, 038h, 064h, 05Ah, 046h, 03Ch, 0FFh, 0FFh, 031h, 0FFh, 0FFh, 03Ch, 0FFh, 02Bh, 0FFh, 030h
        db  05Ah, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 02Eh, 0FFh, 04Bh, 0FFh, 0FFh, 0FFh, 0FFh, 026h, 048h, 03Ch
        db  0FFh, 0FFh, 0FFh, 0FFh, 025h, 04Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 024h, 0FFh, 038h, 02Bh, 0FFh
        db  0FFh, 0FFh, 023h, 0FFh, 038h, 0FFh, 0FFh, 0FFh, 0FFh, 01Fh, 03Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 03Ch, 03Ch, 02Bh, 03Ch, 02Bh, 03Ch
loc_1_0398:
        pop    ebx
        loopne loc_1_03AE
        add    eax, dword ptr [eax]
        add    byte ptr [esi + 0x3000003], ch ; -> loc_1_03AE
        add    al, 0
        add    byte ptr [eax + 4], bl        ; -> loc_1_0458
        add    byte ptr [eax], al
        jne    loc_1_03B0
        add    byte ptr [eax], al
loc_1_03AE:
        js     loc_1_03BE
loc_1_03B0:
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_03BE:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0403:
        js     loc_1_0413
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0413:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0458:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_0475:
        js     loc_1_0485
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0485:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_04CA:
        pop    ebx
        loopne loc_1_04E0
        add    al, 0
        add    al, ah                        ; -> loc_1_04E0
        add    al, 0
        add    byte ptr [0x8a000005], dh     ; -> loc_1_0535
        add    eax, 0x5a70000                ; -> loc_1_05A7
        add    byte ptr [eax], al
loc_1_04E0:
        js     loc_1_04F0
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_04F0:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0535:
        js     loc_1_0545
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0545:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_058A:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_05A7:
        js     loc_1_05B7
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_05B7:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_05FC:
        pop    ebx
        loopne loc_1_0612
        push   es
        add    byte ptr [eax], al
        adc    al, byte ptr [esi]
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        mov    esp, 0xbd000006               ; -> loc_1_06BD
        push   es
        add    byte ptr [eax], al
loc_1_0612:
        js     loc_1_0622
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0622:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0667:
        js     loc_1_0677
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0677:
        add    al, 0xff
        add    al, 0xff
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_06BD:
        js     loc_1_06CD
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_06CD:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0712:
        pop    ebx
        loopne loc_1_0728
        pop    es
        add    byte ptr [eax], al
        sub    byte ptr [edi], al
        add    byte ptr [eax], al
        jge    0x725
        add    byte ptr [eax], al
        rol    byte ptr [edi], cl
        add    byte ptr [eax], al
        rol    dword ptr [edi], cl
        add    byte ptr [eax], al
loc_1_0728:
        js     loc_1_0738
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0738:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_077D:
        js     loc_1_078D
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_078D:
        add    al, 0xff
        add    al, 0xff
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_07D3:
        js     loc_1_07E3
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_07E3:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0828:
        pop    ebx
        loopne loc_1_096B
        or     dword ptr [eax], eax
        add    byte ptr [ebx + 9], ch        ; -> loc_1_096B
        add    byte ptr [eax], al
        mov    ecx, 0x7000009                ; -> obj1:0x0A07
        or     al, byte ptr [eax]
        add    byte ptr [eax], cl            ; -> loc_1_0A08
        or     al, byte ptr [eax]
        db  000h
loc_1_083E:
        pop    ebx
        loopne 0x8ab                         ; -> obj1:0x086A
        or     byte ptr [eax], al
        add    byte ptr [edx + 8], ch        ; -> obj1:0x086A
        add    byte ptr [eax], al
        mov    eax, 0x6000008                ; -> obj1:0x0906
        or     dword ptr [eax], eax
        add    byte ptr [edi], al            ; -> loc_1_0907
        or     dword ptr [eax], eax
        add    byte ptr [ebx - 0x20], bl
        push   8
        add    byte ptr [eax], al
        push   8
        add    byte ptr [eax], al
        mov    eax, 0x6000008                ; -> obj1:0x0906
        or     dword ptr [eax], eax
        add    byte ptr [edi], al            ; -> loc_1_0907
        or     dword ptr [eax], eax
        add    byte ptr [eax + 0xe], bh
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 004h, 0FFh, 0FFh, 0FFh, 0FFh
        db  050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h
        db  002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 00Ah
        db  00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh
        db  0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_08B8:
        js     loc_1_08C8
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_08C8:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h, 000h
loc_1_0907:
        js     loc_1_0917
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0917:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0955:
        pop    ebx
        loopne loc_1_096B
        or     dword ptr [eax], eax
        add    byte ptr [ebx + 9], ch        ; -> loc_1_096B
        add    byte ptr [eax], al
        mov    ecx, 0x7000009                ; -> obj1:0x0A07
        or     al, byte ptr [eax]
        add    byte ptr [eax], cl            ; -> loc_1_0A08
        or     al, byte ptr [eax]
        db  000h
loc_1_096B:
        js     loc_1_097B
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_097B:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_09B9:
        js     loc_1_09C9
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_09C9:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0A08:
        js     loc_1_0A18
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A18:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_0A56:
        pop    ebx
        loopne loc_1_0A6C
        or     al, byte ptr [eax]
        add    byte ptr [edx + ecx], ch      ; -> loc_1_0A6C
        add    byte ptr [edx + 0x800000a], bh ; -> loc_1_0ABA
        or     eax, dword ptr [eax]
        add    byte ptr [ecx], cl            ; -> loc_1_0B09
        or     eax, dword ptr [eax]
        db  000h
loc_1_0A6C:
        js     loc_1_0A7C
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A7C:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0ABA:
        js     loc_1_0ACA
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0ACA:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0B09:
        js     0xb0a
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0B5E:
        pop    ebx
        loopne loc_1_0B74
        or     eax, dword ptr [eax]
        add    byte ptr [ebx + ecx], dh      ; -> loc_1_0B74
        add    dl, al                        ; -> loc_1_0BC2
        or     eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> obj1:0x0C10
        or     al, 0
        add    byte ptr [eax], dl            ; -> obj1:0x0C10
        or     al, 0
        db  000h
loc_1_0B74:
        js     loc_1_0B84
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0B84:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0BC2:
        js     loc_1_0BD2
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0BD2:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0C11:
        pop    ebx
        loopne loc_1_0C27
        or     al, 0
        add    byte ptr [edi], ah            ; -> loc_1_0C27
        or     al, 0
        add    byte ptr [ebp + 0xc], dh      ; -> loc_1_0C75
        add    byte ptr [eax], al
        ret    
        db  00Ch, 000h, 000h
        dd  obj1:0x0CC3              ; fixup
loc_1_0C27:
        js     loc_1_0C37
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0C37:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0C75:
        js     loc_1_0C85
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0C85:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 078h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 000h
loc_1_0D19:
        pop    ebx
        loopne loc_1_0D2F
        or     eax, 0xd2f0000                ; -> loc_1_0D2F
        add    byte ptr [eax], al
        jge    0xd32
        add    byte ptr [eax], al
        retf   
        db  00Dh, 000h, 000h
        dd  obj1:0x0DCB              ; fixup
loc_1_0D2F:
        js     loc_1_0D3F
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0D3F:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0D7D:
        js     0xd8d
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0DCC:
        imul   esp, eax, -0x1e               ; -> loc_1_0DE2
        or     eax, 0xde20000                ; -> loc_1_0DE2
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
        jle    loc_1_0DEA
        add    byte ptr [eax], al
        jle    0xdee
        add    byte ptr [eax], al
loc_1_0DE2:
        js     loc_1_0DF2
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h
loc_1_0DEA:
        or     eax, 0xffff0e0d
        call   ptr [edx + 4]
loc_1_0DF2:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0E30:
        js     loc_1_0E40
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0E40:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 078h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 000h
loc_1_0ED4:
        imul   esp, eax, -0x16               ; -> loc_1_0EEA
        push   cs
        add    byte ptr [eax], al
        ljmp   0xf:0x3800000e                ; -> loc_1_0F38
        add    byte ptr [esi - 0x79fffff1], al ; -> obj1:0x0F86
        sldt   word ptr [eax]
loc_1_0EEA:
        js     loc_1_0EFA
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0EFA:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0F38:
        js     loc_1_0F48
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0F48:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0F87:
        pop    ebx
        loopne loc_1_0F9D
        sldt   word ptr [eax]
        popfd  
        sldt   word ptr [eax]
        in     al, 0xf
        add    byte ptr [eax], al
        sub    edx, dword ptr [eax]
        add    byte ptr [eax], al
        xor    edx, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_0F9D:
        js     loc_1_0FAD
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0FAD:
        add    al, 0xff
        add    al, 0xff
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4bffffff]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    bh, bh
        dec    eax
        add    byte ptr [esi], al
        add    al, 0xff
        add    al, 0xff
        inc    esi
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0FE4:
        js     loc_1_0FF4
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0FF4:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_102B:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1033:
        js     0x1043
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_107A:
        pop    ebx
        loopne loc_1_1090
        adc    byte ptr [eax], al
        add    byte ptr [eax - 0x28fffff0], dl ; -> loc_1_1090
        adc    byte ptr [eax], al
        add    byte ptr [esi], bl            ; -> loc_1_111E
        adc    dword ptr [eax], eax
        add    byte ptr [esi], bl            ; -> loc_1_111E
        adc    dword ptr [eax], eax
        db  000h
loc_1_1090:
        js     loc_1_10A0
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_10A0:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_10D7:
        js     loc_1_10E7
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_10E7:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_111E:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1126:
        imul   esp, eax, 0x3c                ; -> loc_1_113C
        adc    dword ptr [eax], eax
        add    byte ptr [ecx + edx], bh      ; -> loc_1_113C
        add    byte ptr [eax], al
        mov    dl, byte ptr [ecx]
        add    byte ptr [eax], al
        fcom   dword ptr [ecx]
        add    byte ptr [eax], al
        fst    dword ptr [ecx]
        add    byte ptr [eax], al
loc_1_113C:
        js     loc_1_114C
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_114C:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_118A:
        js     0x119a
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_11D9:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_11E1:
        imul   esp, eax, -9                  ; -> loc_1_11F7
        adc    dword ptr [eax], eax
        add    bh, dh                        ; -> loc_1_11F7
        adc    dword ptr [eax], eax
        add    byte ptr [ebp + 0x12], al     ; -> loc_1_1245
        add    byte ptr [eax], al
        xchg   ebx, eax
        adc    al, byte ptr [eax]
        db  000h
        dd  obj1:0x129B              ; fixup
loc_1_11F7:
        js     loc_1_1207
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1207:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_1245:
        js     loc_1_1255
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1255:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1293:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_129C:
        cmp    ebx, dword ptr [0x13ec]       ; -> loc_1_13EC
        jne    loc_1_12B5
        cmp    word ptr [0x1448], 0x40       ; -> obj1:0x1448
        jne    loc_1_12B5
        cmp    dx, 0x4f15
        je     loc_1_12C4
loc_1_12B5:
        cmp    dword ptr [0x143b], 0         ; -> obj1:0x143B
        je     loc_1_12EB
        jmp    dword ptr [0x143b]            ; -> obj1:0x143B
loc_1_12C4:
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 0x10
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x55
        out    dx, al
        in     ax, dx
        and    ah, 0xf8
        out    dx, ax
        rol    eax, 0x10
        out    dx, al
        pop    edx
        pop    eax
loc_1_12EB:
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'S3MINI  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  S3MINI_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1364:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1368:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1388:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13D2:
        add    byte ptr [eax], al
loc_1_13D4:
        add    byte ptr [eax], al
loc_1_13D6:
        add    byte ptr [eax], al
loc_1_13D8:
        add    byte ptr [eax], al
loc_1_13DA:
        add    byte ptr [eax], al
loc_1_13DC:
        add    bh, bh
loc_1_13DE:
        add    byte ptr [eax], al
loc_1_13E0:
        add    bh, bh
loc_1_13E2:
        add    byte ptr [eax], al
loc_1_13E4:
        add    byte ptr [eax], al
loc_1_13E6:
        add    byte ptr [eax], al
loc_1_13E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_143A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, ah                        ; -> loc_1_11E1
        adc    dword ptr [eax], eax
        add    byte ptr [ebx], bl
loc_1_1445:
        cmp    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x6e4  R-X
loc_2_0000:
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        rol    eax, 8
        out    dx, al
        clc    
        ret    
loc_2_001B:
        push   ebx
        mov    dword ptr [0x1340], ebx       ; -> obj1:0x1340
        mov    byte ptr [0x13d9], 0          ; -> obj1:0x13D9
        pushal 
        mov    eax, 0x20
        mov    esi, 0x6d0                    ; -> loc_2_06D0
        mov    edi, 0x6d8                    ; -> loc_2_06D8
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        cmp    eax, 0x20
        je     loc_2_004E
        cmp    eax, 0x40
        je     loc_2_004E
        mov    eax, 0x20
loc_2_004E:
        shl    eax, 0xa
        mov    dword ptr [0x133c], eax       ; -> obj1:0x133C
        popal  
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_0402
        mov    dword ptr [0x1350], eax       ; -> obj1:0x1350
        mov    edx, 0x3d4
        mov    ax, 0x4838
        out    dx, ax
        mov    al, 0x30
        out    dx, al
        inc    dx
        in     al, dx
        cmp    al, 0xe1
        je     loc_2_00C2
        mov    ch, al
        and    ch, 0xf0
        cmp    ch, 0x80
        je     loc_2_00F4
        cmp    ch, 0x90
        je     loc_2_00F4
        cmp    ch, 0xb0
        je     loc_2_00F4
        cmp    ch, 0xa0
        je     loc_2_00F4
        mov    byte ptr [0x13d8], 0xff       ; -> loc_1_13D8
        cmp    ch, 0xc0
        je     loc_2_00F4
        cmp    ch, 0xd0
        je     loc_2_00F4
        cmp    ch, 0xe0
        jne    loc_2_0402
loc_2_00C2:
        mov    byte ptr [0x13d8], 0xff       ; -> loc_1_13D8
        dec    dl
        mov    al, 0x2e
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xf0
        or     al, 1
        mov    ch, al
        cmp    ch, 0x11
        je     loc_2_00F4
        cmp    ch, 0x81
        je     loc_2_00F4
        cmp    ch, 0x91
        je     loc_2_00F4
        cmp    ch, 0xb1
        je     loc_2_00F4
        cmp    ch, 0xf1
        jne    loc_2_0402
loc_2_00F4:
        dec    dl
        mov    al, 0x30
        out    dx, al
        inc    dl
        in     al, dx
        inc    al
        mov    ah, al
        out    dx, al
        in     al, dx
        cmp    al, ah
        je     loc_2_0402
        dec    dl
        mov    al, 0x36
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        mov    cl, al
        cmp    ch, 0x80
        je     loc_2_0160
        shr    cl, 5
        mov    eax, 0x400000
        or     cl, cl
        je     loc_2_016F
        mov    eax, 0x300000
        cmp    cl, 2
        je     loc_2_016F
        mov    eax, 0x800000
        cmp    cl, 3
        je     loc_2_016F
        mov    eax, 0x200000
        cmp    cl, 4
        je     loc_2_016F
        mov    eax, 0x600000
        cmp    cl, 5
        je     loc_2_016F
        mov    eax, 0x100000
        cmp    cl, 6
        je     loc_2_016F
        mov    eax, 0x80000
        jmp    loc_2_016F
loc_2_0160:
        mov    eax, 0x100000
        test   cl, 0x20
        je     loc_2_016F
        mov    eax, 0x80000
loc_2_016F:
        mov    dword ptr [0x134c], eax       ; -> obj1:0x134C
        mov    byte ptr [0x13d7], ch         ; -> obj1:0x13D7
        push   0
        push   0x10
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x1354], eax       ; -> obj1:0x1354
        or     eax, eax
        je     loc_2_0402
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_0402
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0        ; -> loc_4_0000
        mov    dword ptr [edi + 8], 0xcb     ; -> loc_4_00CB
        mov    dword ptr [edi + 0xc], 0x152  ; -> loc_4_0152
        mov    dword ptr [edi + 0x58], 0x201 ; -> loc_4_0201
        mov    dword ptr [edi + 0x5c], 0x27f ; -> loc_4_027F
        mov    dword ptr [edi + 0x10], 0x2d7 ; -> loc_4_02D7
        mov    dword ptr [edi + 0x14], 0x33b ; -> loc_4_033B
        mov    dword ptr [edi + 0x18], 0x366 ; -> loc_4_0366
        mov    dword ptr [edi + 0x1c], 0x36e ; -> loc_4_036E
        mov    dword ptr [edi + 0x20], 0x376 ; -> sub_4_0376
        mov    dword ptr [edi + 0x24], 0x3d3 ; -> loc_4_03D3
        mov    dword ptr [edi + 0x2c], 0x51f ; -> loc_4_051F
        mov    dword ptr [edi + 0x30], 0x560 ; -> loc_4_0560
        mov    dword ptr [edi + 0x34], 0x5c1 ; -> loc_4_05C1
        mov    dword ptr [edi + 0x38], 0x6a0 ; -> sub_4_06A0
        mov    dword ptr [edi + 0x3c], 0x78a ; -> loc_4_078A
        mov    dword ptr [edi + 0x40], 0x80c ; -> loc_4_080C
        mov    dword ptr [edi + 0x44], 0x84d ; -> loc_4_084D
        mov    dword ptr [edi + 0xb8], 0x36e ; -> loc_3_036E
        mov    dword ptr [edi + 0xb4], 0x341 ; -> loc_3_0341
        mov    dword ptr [edi + 0x68], 0x31e ; -> loc_3_031E
        mov    dword ptr [edi + 0x70], 0x5f  ; -> loc_3_005F
        mov    dword ptr [edi + 0x74], 0x2f2 ; -> loc_3_02F2
        mov    dword ptr [edi + 0x90], 0x8d6 ; -> loc_4_08D6
        mov    dword ptr [edi + 0x80], 0x8de ; -> loc_4_08DE
        mov    dword ptr [edi + 0x84], 0x8de ; -> loc_4_08DE
        mov    dword ptr [edi + 0x94], 0x92b ; -> loc_4_092B
        mov    dword ptr [edi + 0x88], 0xa61 ; -> loc_4_0A61
        mov    dword ptr [edi + 0x9c], 0x934 ; -> loc_4_0934
        mov    dword ptr [edi + 0xa0], 0x9e8 ; -> loc_4_09E8
        mov    dword ptr [edi + 0xa8], 0x405 ; -> loc_2_0405
        mov    dword ptr [edi + 0xb0], 0x7e  ; -> loc_1_007E
        mov    dword ptr [edi + 0xbc], 0xdf  ; -> loc_1_00DF
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        jne    loc_2_02C7
        mov    dword ptr [edi + 0x98], 0xac1 ; -> loc_4_0AC1
loc_2_02C7:
        mov    edx, 0x3c2
        mov    ecx, 0x8be                    ; -> loc_4_08BE
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    dword ptr [0x1364], ecx       ; -> loc_1_1364
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0x1344], esi       ; -> obj1:0x1344
        mov    dword ptr [0x1348], edi       ; -> obj1:0x1348
        mov    edx, 0x9ae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xe2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xbee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x96e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x86e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x82e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x8ee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x8ae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xb6e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xbae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xaae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xaee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa6e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x92e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xb2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    esi, 0x769                    ; -> loc_4_0769
        mov    edx, 0x4ae8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0402
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0402
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    ebx, dword ptr [0x1340]       ; -> obj1:0x1340
        call   sub_2_0410
        mov    al, byte ptr [0x13d7]         ; -> obj1:0x13D7
        clc    
        jmp    loc_2_0403
loc_2_0402:
        stc    
loc_2_0403:
        pop    ebx
        ret    
loc_2_0405:
        movzx  eax, byte ptr [0x13d7]        ; -> obj1:0x13D7
        ret    
        db  000h, 000h, 000h
sub_2_0410:
        push   ebx
        mov    dword ptr [0x13ec], ebx       ; -> loc_1_13EC
        mov    esi, 0x548                    ; -> loc_2_0548
        mov    ebx, 8
        call   sub_2_051A
        jb     loc_2_0517
        mov    esi, 0x550                    ; -> loc_2_0550
        mov    ebx, 0xb
        call   sub_2_051A
        jae    loc_2_0474
        mov    esi, 0x55b                    ; -> loc_2_055B
        mov    ebx, 0xa
        call   sub_2_051A
        jae    loc_2_0474
        mov    esi, 0x565                    ; -> loc_2_0565
        mov    ebx, 0xb
        call   sub_2_051A
        jae    loc_2_0474
        mov    esi, 0x570                    ; -> loc_2_0570
        mov    ebx, 0xe
        call   sub_2_051A
        jb     loc_2_0517
loc_2_0474:
        call   sub_2_0582
        mov    word ptr [0x1446], ax         ; -> obj1:0x1446
        mov    word ptr [0x1448], bx         ; -> obj1:0x1448
        and    ax, 0x7fff
        mov    esi, 0x187                    ; -> obj1:0x0187
loc_2_048F:
        test   word ptr [esi], bx
        je     loc_2_04AF
        push   bx
        and    bx, 0xc000
        cmp    bx, word ptr [esi + 2]
        pop    bx
        jne    loc_2_04AF
        test   word ptr [esi + 4], ax
        je     loc_2_04AF
        cmp    cx, word ptr [esi + 6]
        je     loc_2_04BA
loc_2_04AF:
        add    esi, 0xc
        cmp    word ptr [esi], 0
        je     loc_2_0517
        jmp    loc_2_048F
loc_2_04BA:
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [0x143f], esi       ; -> obj1:0x143F
        mov    ebx, dword ptr [0x13ec]       ; -> loc_1_13EC
        mov    ebp, dword ptr [ebx + 8]
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_0517
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_2_0517
        call   dword ptr [edi + 0xa8]
        mov    byte ptr [0x143a], al         ; -> loc_1_143A
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x70], 0x55a ; -> loc_3_055A
        mov    eax, dword ptr [edi + 0x74]
        mov    dword ptr [edi + 0x74], 0x384 ; -> loc_3_0384
        mov    eax, dword ptr [edi + 0xbc]
        mov    dword ptr [edi + 0xbc], 0x129c ; -> loc_1_129C
        mov    dword ptr [0x143b], eax       ; -> obj1:0x143B
        clc    
        jmp    loc_2_0518
loc_2_0517:
        stc    
loc_2_0518:
        pop    ebx
        ret    
sub_2_051A:
        push   eax
        push   edi
        push   ecx
        mov    edi, 0xc0000
        mov    ecx, 0x100
loc_2_0527:
        mov    al, byte ptr [esi]
        repne scasb al, byte ptr es:[edi]
        or     ecx, ecx
        je     loc_2_0540
        push   esi
        push   edi
        push   ecx
        dec    edi
        mov    ecx, ebx
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        or     ecx, ecx
        pop    ecx
        pop    edi
        pop    esi
        je     loc_2_0543
        loop   loc_2_0527
loc_2_0540:
        stc    
        jmp    loc_2_0544
loc_2_0543:
        clc    
loc_2_0544:
        pop    ecx
        pop    edi
        pop    eax
        ret    
loc_2_0548:
        inc    esp
        imul   esp, dword ptr [ecx + 0x6d], 0x646e6f
loc_2_0550:
        push   ebx
        je     0x5b8
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_2_05BF
        and    byte ptr [esi], dh
        xor    al, 0
loc_2_055B:
        push   ebx
        je     0x5c3
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_2_05CA
        xor    al, 0
loc_2_0565:
        push   ebx
        je     0x5cd
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_2_05D4
        and    byte ptr [ebx + 0x45], dl
        db  000h
loc_2_0570:
        push   ebx
        je     loc_2_05D8
        popal  
        insb   byte ptr es:[edi], dx
        je     0x5df
        and    byte ptr [esi + 0x69], dl
        outsd  dx, dword ptr gs:[esi]
        db  000h
loc_2_057E:
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], eax
sub_2_0582:
        push   dx
        mov    dx, 0x3d4
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa039
        out    dx, ax
        mov    al, 0x36
        out    dx, al
        jmp    loc_2_0599
loc_2_0599:
        in     ax, dx
        jmp    loc_2_059D
loc_2_059D:
        and    ah, 0xc0
        mov    cx, 4
        or     ah, ah
        je     loc_2_05B5
        mov    cx, 2
        cmp    ah, 0x80
        je     loc_2_05B5
        mov    cx, 1
loc_2_05B5:
        push   cx
        mov    al, 0x30
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
loc_2_05BF:
        mov    ah, al
        and    ah, 0xf
        and    al, 0xf0
        cmp    al, 0xd0
        je     loc_2_063E
loc_2_05CA:
        cmp    al, 0xc0
        je     loc_2_05FA
        cmp    al, 0xe0
        jne    loc_2_05FA
        mov    al, 0x2e
loc_2_05D4:
        out    dx, al
        inc    dx
        in     al, dx
loc_2_05D8:
        dec    dx
        cmp    al, 0x80
        je     loc_2_065C
        cmp    al, 0x90
        je     loc_2_0666
        cmp    al, 0xb0
        je     loc_2_0670
        cmp    al, 0xf0
        je     loc_2_0670
        cmp    al, 0x10
        je     loc_2_0648
        cmp    al, 0x11
        je     loc_2_0652
loc_2_05FA:
        mov    bx, 1
        push   esi
        push   ebx
        mov    esi, 0x57e                    ; -> loc_2_057E
        mov    ebx, 4
        call   sub_2_051A
        pop    ebx
        pop    esi
        jae    loc_2_0618
        or     bx, 0x4000
loc_2_0618:
        mov    cx, 0x400
        mov    dx, 0x3c8
        in     al, dx
        mov    dx, 0x3c6
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        and    al, 0xf0
        cmp    al, 0xb0
        jne    loc_2_0634
        or     cx, 0x800
loc_2_0634:
        mov    dx, 0x3c8
        in     al, dx
        jmp    loc_2_06C7
loc_2_063E:
        mov    bx, 2
        mov    cx, 0x100
        jmp    loc_2_06C7
loc_2_0648:
        mov    bx, 4
        mov    cx, 0x400
        jmp    loc_2_06C7
loc_2_0652:
        mov    bx, 8
        mov    cx, 0x400
        jmp    loc_2_06C7
loc_2_065C:
        mov    bx, 0x10
        mov    cx, 0x400
        jmp    loc_2_06C7
loc_2_0666:
        mov    bx, 0x20
        mov    cx, 0x400
        jmp    loc_2_06C7
loc_2_0670:
        mov    bx, 0x40
        push   ax
        push   bx
        push   si
        push   di
        mov    ax, 0x1daa
        mov    bx, 0xfdec
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        and    ax, 0xf00
        pop    di
        pop    si
        pop    bx
        xor    cx, cx
        cmp    ax, 0xc00
        jne    loc_2_06A4
        or     cx, 0x200
loc_2_06A4:
        cmp    ax, 0xd00
        jne    loc_2_06AF
        or     cx, 0x4200
loc_2_06AF:
        cmp    ax, 0xf00
        jne    loc_2_06BA
        or     cx, 0x1000
loc_2_06BA:
        cmp    ax, 0
        jne    loc_2_06C5
        or     cx, 0x1080
loc_2_06C5:
        pop    ax
loc_2_06C7:
        xchg   cx, ax
        pop    cx
        pop    dx
        ret    
        db  000h, 000h
loc_2_06D0:
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x79616c
loc_2_06D8:
        push   esi
        inc    esp
        inc    esp
        push   edx
        jae    0x744
        jb     0x757
        add    byte ptr gs:[eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x664  R-X
loc_3_0000:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x1358], eax       ; -> obj1:0x1358
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x135c], eax       ; -> obj1:0x135C
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x1360], eax       ; -> obj1:0x1360
        mov    ecx, dword ptr [ebp + 0x18]
        mov    al, 0x5a
        mov    ah, cl
        mov    word ptr [0x13d2], ax         ; -> loc_1_13D2
        mov    al, 0x59
        mov    ah, ch
        mov    word ptr [0x13d4], ax         ; -> loc_1_13D4
        shr    ecx, 0x10
        mov    byte ptr [0x13e6], cl         ; -> loc_1_13E6
        mov    byte ptr [0x13d6], ch         ; -> loc_1_13D6
        mov    edx, 0x3cc
        in     al, dx
        mov    byte ptr [0x13e7], al         ; -> obj1:0x13E7
        ret    
loc_3_005F:
        mov    eax, 0x1388                   ; -> loc_1_1388
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_02F1
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_02F1
        movzx  esi, word ptr [eax + 0x18]
        or     esi, esi
        je     loc_3_02F1
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    eax, 0xa539
        out    dx, ax
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_3_0142
        cmp    byte ptr [0x13d7], 0xa0       ; -> obj1:0x13D7
        je     loc_3_019B
        cmp    byte ptr [0x13d7], 0xa0       ; -> obj1:0x13D7
        je     loc_3_019B
        cmp    byte ptr [0x13d7], 0x90       ; -> obj1:0x13D7
        je     loc_3_019B
        cmp    byte ptr [0x13d7], 0xb0       ; -> obj1:0x13D7
        je     loc_3_019B
        cmp    byte ptr [0x13d7], 0xc0       ; -> obj1:0x13D7
        je     loc_3_0211
        cmp    byte ptr [0x13d7], 0xd0       ; -> obj1:0x13D7
        je     loc_3_0211
        cmp    byte ptr [0x13d7], 0xe0       ; -> obj1:0x13D7
        je     loc_3_0211
        cmp    byte ptr [0x13d7], 0x11       ; -> obj1:0x13D7
        je     loc_3_0211
        cmp    byte ptr [0x13d7], 0x81       ; -> obj1:0x13D7
        je     loc_3_0211
        cmp    byte ptr [0x13d7], 0x91       ; -> obj1:0x13D7
        je     loc_3_0211
        cmp    byte ptr [0x13d7], 0xb1       ; -> obj1:0x13D7
        je     loc_3_0211
loc_3_0142:
        mov    al, 0x41
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xf8
        cmp    esi, 0x280
        je     loc_3_018A
        cmp    esi, 0x320
        je     loc_3_0178
        cmp    esi, 0x400
        je     loc_3_0166
        mov    ah, 7
        jmp    loc_3_0193
loc_3_0166:
        mov    ah, 7
        cmp    edi, 0x46
        jae    loc_3_0193
        mov    ah, 6
        cmp    edi, 0x3c
        jae    loc_3_0193
        mov    ah, 5
        jmp    loc_3_0193
loc_3_0178:
        mov    ah, 3
        cmp    edi, 0x48
        jae    loc_3_0193
        mov    ah, 2
        cmp    edi, 0x3c
        jae    loc_3_0193
        mov    ah, 4
        jmp    loc_3_0193
loc_3_018A:
        mov    ah, 1
        cmp    edi, 0x48
        jae    loc_3_0193
        xor    ah, ah
loc_3_0193:
        or     al, ah
        out    dx, al
        jmp    loc_3_02EE
loc_3_019B:
        mov    ch, 3
        cmp    esi, 0x280
        je     loc_3_01D1
        mov    ch, 0xc
        cmp    esi, 0x320
        je     loc_3_01D1
        mov    ch, 0x30
        cmp    esi, 0x400
        je     loc_3_01D1
        mov    ch, 0x30
        cmp    esi, 0x480
        je     loc_3_01D1
        mov    ch, 0xc0
        cmp    esi, 0x500
        jne    loc_3_02EE
loc_3_01D1:
        mov    cl, ch
        not    cl
        mov    ah, 0xff
        cmp    edi, 0x48
        jae    loc_3_01EC
        mov    ah, 0xaa
        cmp    edi, 0x3c
        jae    loc_3_01EC
        mov    ah, 0x55
        cmp    edi, 0x38
        jae    loc_3_01EC
        xor    ah, ah
loc_3_01EC:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        cmp    byte ptr [0x13d7], 0xa0       ; -> obj1:0x13D7
        jne    loc_3_0206
        mov    byte ptr [0x13e8], 0x52       ; -> loc_1_13E8
        mov    byte ptr [0x13e9], al         ; -> obj1:0x13E9
loc_3_0206:
        and    ax, cx
        or     al, ah
        out    dx, al
        jmp    loc_3_02EE
loc_3_0211:
        mov    ah, 0x42
        cmp    byte ptr [0x13d7], 0xe0       ; -> obj1:0x13D7
        je     loc_3_0227
        cmp    byte ptr [0x13d7], 0x11       ; -> obj1:0x13D7
        je     loc_3_0227
        mov    ah, 0x5b
loc_3_0227:
        cmp    esi, 0x280
        jne    loc_3_0254
        mov    cx, 0x8f20
        cmp    edi, 0x4b
        jae    loc_3_0245
        mov    cx, 0x8f10
        cmp    edi, 0x48
        jae    loc_3_0245
        mov    cx, 0x8f00
loc_3_0245:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    loc_3_02EE
loc_3_0254:
        cmp    esi, 0x320
        jne    loc_3_0292
        mov    cx, 0x180
        cmp    edi, 0x4b
        jae    loc_3_027B
        mov    cx, 0x100
        cmp    edi, 0x48
        jae    loc_3_027B
        mov    cx, 0x80
        cmp    edi, 0x3c
        jae    loc_3_027B
        mov    cx, 0
loc_3_027B:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x7f
        or     al, cl
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfc
        or     al, cl
        out    dx, al
        jmp    loc_3_02EE
loc_3_0292:
        cmp    esi, 0x400
        je     loc_3_02A2
        cmp    esi, 0x480
        jne    loc_3_02C5
loc_3_02A2:
        mov    cl, 0x10
        cmp    edi, 0x4b
        jae    loc_3_02B9
        mov    cl, 0xc
        cmp    edi, 0x46
        jae    loc_3_02B9
        mov    cl, 8
        cmp    edi, 0x3c
        jae    loc_3_02B9
        mov    cl, 0
loc_3_02B9:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xe3
        or     al, cl
        out    dx, al
        jmp    loc_3_02EE
loc_3_02C5:
        cmp    esi, 0x500
        jne    loc_3_02EE
        mov    cl, 0xc0
        cmp    edi, 0x4b
        jae    loc_3_02E4
        mov    cl, 0xa0
        cmp    edi, 0x48
        jae    loc_3_02E4
        mov    cl, 0x80
        cmp    edi, 0x3c
        jae    loc_3_02E4
        mov    cl, 0
loc_3_02E4:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x1f
        or     al, cl
        out    dx, al
loc_3_02EE:
        pop    eax
        dec    edx
        out    dx, al
loc_3_02F1:
        ret    
loc_3_02F2:
        cmp    word ptr [0x13e8], 0          ; -> loc_1_13E8
        je     loc_3_031D
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    eax, 0xa539
        out    dx, ax
        mov    ax, word ptr [0x13e8]         ; -> loc_1_13E8
        out    dx, ax
        mov    word ptr [0x13e8], 0          ; -> loc_1_13E8
        pop    eax
        out    dx, al
loc_3_031D:
        ret    
loc_3_031E:
        mov    byte ptr [0x13da], 0          ; -> loc_1_13DA
        call   sub_4_06A0
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        ret    
loc_3_0341:
        push   eax
        push   edi
        mov    edi, 0x1368                   ; -> loc_1_1368
        mov    al, 0x67
        cmp    byte ptr [0x449], 3
        jbe    loc_3_0355
        mov    al, 0x63
loc_3_0355:
        mov    byte ptr [edi + 0xc], al
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        mov    byte ptr [0x13dc], 0xff       ; -> loc_1_13DC
        pop    edi
        pop    eax
        ret    
loc_3_036E:
        cld    
        push   edi
        mov    edi, 0x1378                   ; -> loc_1_1378
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        pop    edi
        ret    
        db  000h
loc_3_0384:
        push   ebx
        mov    eax, 0x13f0                   ; -> loc_1_13F0
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_0558
        mov    dh, byte ptr [eax + 0x1f]
        or     dh, dh
        je     loc_3_0558
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_0558
        movzx  edi, word ptr [eax + 0x18]
        or     edi, edi
        je     loc_3_0558
        test   word ptr [eax + 2], 0x280
        jne    loc_3_0404
        xor    dl, dl
        cmp    byte ptr [eax + 0x30], 0x2b
        je     loc_3_03E1
        cmp    byte ptr [eax + 0x30], 0x2d
        jne    loc_3_0404
        mov    dl, 0x40
loc_3_03E1:
        cmp    byte ptr [eax + 0x31], 0x2b
        je     loc_3_03F0
        cmp    byte ptr [eax + 0x31], 0x2d
        jne    loc_3_0404
        or     dl, 0x80
loc_3_03F0:
        push   ax
        mov    ah, dl
        mov    dx, 0x3cc
        in     al, dx
        and    al, 0x3f
        or     al, ah
        mov    dx, 0x3c2
        out    dx, al
        pop    ax
loc_3_0404:
        push   ax
        mov    dx, 0x3d4
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa039
        out    dx, ax
        pop    ax
        mov    dh, byte ptr [eax + 0x1f]
        or     dh, dh
        je     loc_3_04E4
        cmp    dh, 0x10
        jne    loc_3_04E4
        movzx  edi, word ptr [eax + 0x18]
        or     edi, edi
        je     loc_3_04E4
        cmp    edi, 0x640
        jne    loc_3_04E4
        mov    dx, word ptr [0x1446]         ; -> obj1:0x1446
        and    dx, 0x4200
        je     loc_3_048C
        mov    dx, 0x3d4
        in     al, dx
        push   ax
        mov    al, 0x55
        out    dx, al
        in     ax, dx
        push   ax
        mov    al, 0x55
        out    dx, ax
        mov    dl, 0xc8
        in     al, dx
        push   ax
        mov    al, 0x18
        out    dx, al
        mov    al, 2
        mov    dl, 0xd5
        out    dx, al
        mov    dl, 0xc6
        in     al, dx
        or     al, 1
        out    dx, al
        xor    al, al
        mov    dl, 0xd5
        out    dx, al
        pop    ax
        mov    dl, 0xc8
        out    dx, al
        pop    ax
        mov    dl, 0xd4
        out    dx, ax
        pop    ax
        out    dx, al
        jmp    loc_3_04E4
loc_3_048C:
        mov    dx, word ptr [0x1446]         ; -> obj1:0x1446
        and    dx, 0x1080
        je     loc_3_04E4
        mov    dx, 0x3d4
        in     al, dx
        push   ax
        mov    al, 0x55
        out    dx, al
        in     ax, dx
        push   ax
        mov    ax, 0x355
        out    dx, ax
        mov    dl, 0xc7
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        mov    dl, 0xc9
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        dec    dx
        in     al, dx
        push   ax
        mov    al, 0xc
        out    dx, al
        mov    dl, 0xc6
        in     al, dx
        or     al, 2
        out    dx, al
        mov    dl, 0xc8
        pop    ax
        out    dx, al
        mov    dl, 0xc9
        pop    ax
        out    dx, al
        mov    dl, 0xc7
        pop    ax
        out    dx, al
        mov    dl, 0xd4
        pop    ax
        out    dx, ax
        pop    ax
        out    dx, al
loc_3_04E4:
        mov    dx, word ptr [0x1446]         ; -> obj1:0x1446
        and    dx, 0x1080
        je     loc_3_0546
        mov    dx, 0x3d4
        in     al, dx
        push   ax
        mov    al, 0x42
        out    dx, al
        in     ax, dx
        and    ah, 0x20
        mov    cl, ah
        mov    al, 0x55
        out    dx, al
        in     ax, dx
        push   ax
        mov    ax, 0x355
        out    dx, ax
        mov    dl, 0xc7
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        mov    dl, 0xc9
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        dec    dx
        in     al, dx
        push   ax
        mov    al, 0x71
        out    dx, al
        mov    dl, 0xc6
        in     al, dx
        or     al, cl
        out    dx, al
        mov    dl, 0xc8
        pop    ax
        out    dx, al
        mov    dl, 0xc9
        pop    ax
        out    dx, al
        mov    dl, 0xc7
        pop    ax
        out    dx, al
        mov    dl, 0xd4
        pop    ax
        out    dx, ax
        pop    ax
        out    dx, al
loc_3_0546:
        mov    dx, 0x3d4
        mov    ax, word ptr [0x1443]         ; -> obj1:0x1443
        out    dx, ax
        mov    al, byte ptr [0x1445]         ; -> loc_1_1445
        out    dx, al
loc_3_0558:
        pop    ebx
        ret    
loc_3_055A:
        push   ebx
        mov    eax, 0x13f0                   ; -> loc_1_13F0
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_0662
        mov    dh, byte ptr [eax + 0x1f]
        or     dh, dh
        je     loc_3_0662
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_0662
        movzx  edi, word ptr [eax + 0x18]
        or     edi, edi
        je     loc_3_0662
        mov    dl, 1
        mov    bx, di
        mov    edi, 0x16d                    ; -> loc_1_016D
loc_3_05A9:
        cmp    word ptr [edi], 0
        je     loc_3_0662
        cmp    word ptr [edi], bx
        je     loc_3_05BF
        add    edi, 4
        inc    dl
        jmp    loc_3_05A9
loc_3_05BF:
        movzx  edi, dl
        shl    edi, 0x10
        xor    dl, dl
        movzx  esi, word ptr [eax + 0x20]
        or     esi, esi
        je     loc_3_0662
        or     si, dx
        call   sub_1_0124
        mov    di, si
        xchg   si, dx
        mov    byte ptr [0x144a], dl         ; -> obj1:0x144A
        mov    edx, 0x3d4
        in     al, dx
        mov    byte ptr [0x1445], al         ; -> loc_1_1445
        push   eax
        mov    eax, 0xa539
        out    dx, ax
        mov    esi, dword ptr [0x143f]       ; -> obj1:0x143F
        mov    al, byte ptr [esi + 1]
        mov    ah, al
        mov    al, byte ptr [esi]
        out    dx, al
        mov    bl, al
        inc    dl
        in     al, dx
        mov    bh, al
        mov    word ptr [0x1443], bx         ; -> obj1:0x1443
        and    ah, al
        add    esi, 2
        xor    ebx, ebx
        mov    bx, di
        shr    ebx, 0xb
        shl    ebx, 2
        add    esi, ebx
        mov    esi, dword ptr [esi]
        mov    bx, di
        xor    bh, bh
loc_3_062F:
        mov    al, byte ptr [esi]
        or     al, al
        je     loc_3_065C
        cmp    bl, al
        jb     loc_3_0655
        push   ebx
        push   esi
        mov    ebx, esi
        push   ax
        mov    eax, edi
        shr    eax, 0x10
        and    eax, 0xf
        add    ebx, eax
        mov    al, byte ptr [ebx]
        pop    bx
        mov    ah, bh
        cmp    al, 0xff
        pop    esi
        pop    ebx
        jne    loc_3_065C
loc_3_0655:
        add    esi, 7
        or     al, al
        jne    loc_3_062F
loc_3_065C:
        or     al, ah
        out    dx, al
        pop    eax
        dec    edx
        out    dx, al
loc_3_0662:
        pop    ebx
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xad4  R-X
loc_4_0000:
        mov    byte ptr [0x13da], 0xff       ; -> loc_1_13DA
        mov    byte ptr [0x13db], 0xff       ; -> obj1:0x13DB
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_4_0036
        mov    edx, dword ptr [0x134c]       ; -> obj1:0x134C
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_4_00C6
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_4_0036:
        mov    edx, dword ptr [0x134c]       ; -> obj1:0x134C
        sub    edx, ecx
        cmp    edx, dword ptr [0x133c]       ; -> obj1:0x133C
        mov    edx, dword ptr [0x133c]       ; -> obj1:0x133C
        ja     loc_4_0051
        mov    edx, 0x8000
loc_4_0051:
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_0081
        mov    byte ptr [0x13de], al         ; -> loc_1_13DE
        mov    byte ptr [0x13dd], 0x3f       ; -> obj1:0x13DD
        cmp    byte ptr [0x13d7], 0xe0       ; -> obj1:0x13D7
        je     loc_4_00C8
        cmp    byte ptr [0x13d7], 0x11       ; -> obj1:0x13D7
        je     loc_4_00C8
        mov    byte ptr [0x13dd], 0xff       ; -> obj1:0x13DD
        jmp    loc_4_00C8
loc_4_0081:
        push   edx
        push   ebx
        mov    bl, al
        mov    bh, al
        and    bl, 0xf
        shr    bh, 4
        mov    edx, 0x3d4
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf0
        or     al, bl
        mov    byte ptr [0x13de], al         ; -> loc_1_13DE
        mov    byte ptr [0x13df], 0          ; -> obj1:0x13DF
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_00C2
        dec    edx
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        shl    bh, 2
        or     al, bh
        mov    byte ptr [0x13df], al         ; -> obj1:0x13DF
loc_4_00C2:
        pop    ebx
        pop    edx
        jmp    loc_4_00C8
loc_4_00C6:
        mov    edx, ecx
loc_4_00C8:
        xor    ah, ah
        ret    
loc_4_00CB:
        cmp    byte ptr [0x13e1], 0xff       ; -> obj1:0x13E1
        jne    loc_4_0151
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_0151
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x13e0], al         ; -> loc_1_13E0
        or     al, 1
        out    dx, al
        dec    edx
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_0120
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x13e1], al         ; -> obj1:0x13E1
        mov    al, byte ptr [0x13de]         ; -> loc_1_13DE
        out    dx, al
        dec    dl
        jmp    loc_4_014B
loc_4_0120:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x13e1], al         ; -> obj1:0x13E1
        mov    al, byte ptr [0x13de]         ; -> loc_1_13DE
        out    dx, al
        dec    edx
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_014B
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x13e2], al         ; -> loc_1_13E2
        mov    al, byte ptr [0x13df]         ; -> obj1:0x13DF
        out    dx, al
        dec    edx
loc_4_014B:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0151:
        ret    
loc_4_0152:
        cmp    byte ptr [0x13e1], 0xff       ; -> obj1:0x13E1
        je     loc_4_0200
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_019B
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        and    al, byte ptr [0x13dd]         ; -> obj1:0x13DD
        cmp    al, byte ptr [0x13de]         ; -> loc_1_13DE
        jne    loc_4_01EE
        mov    al, byte ptr [0x13e1]         ; -> obj1:0x13E1
        out    dx, al
        jmp    loc_4_01EE
loc_4_019B:
        xor    ah, ah
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_01AE
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
loc_4_01AE:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    dl, byte ptr [0x13de]         ; -> loc_1_13DE
        mov    dh, byte ptr [0x13df]         ; -> obj1:0x13DF
        cmp    ax, dx
        jne    loc_4_01EE
        mov    edx, 0x3d5
        mov    al, byte ptr [0x13e1]         ; -> obj1:0x13E1
        out    dx, al
        dec    edx
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_01E4
        mov    al, 0x51
        out    dx, al
        inc    edx
        mov    al, byte ptr [0x13e2]         ; -> loc_1_13E2
        out    dx, al
        dec    edx
loc_4_01E4:
        mov    al, 0x31
        mov    ah, byte ptr [0x13e0]         ; -> loc_1_13E0
        out    dx, ax
loc_4_01EE:
        mov    byte ptr [0x13e1], 0xff       ; -> obj1:0x13E1
        mov    edx, 0x3d4
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0200:
        ret    
loc_4_0201:
        push   eax
        push   edx
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_027C
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x13e3], al         ; -> obj1:0x13E3
        or     al, 1
        out    dx, al
        dec    edx
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_024D
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x13e4], al         ; -> loc_1_13E4
        mov    al, byte ptr [0x13de]         ; -> loc_1_13DE
        out    dx, al
        dec    dl
        jmp    loc_4_0278
loc_4_024D:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x13e4], al         ; -> loc_1_13E4
        mov    al, byte ptr [0x13de]         ; -> loc_1_13DE
        out    dx, al
        dec    edx
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_0278
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x13e5], al         ; -> obj1:0x13E5
        mov    al, byte ptr [0x13df]         ; -> obj1:0x13DF
        out    dx, al
        dec    edx
loc_4_0278:
        rol    eax, 8
        out    dx, al
loc_4_027C:
        pop    edx
        pop    eax
        ret    
loc_4_027F:
        push   eax
        push   edx
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_02D4
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    al, 0x31
        mov    ah, byte ptr [0x13e3]         ; -> obj1:0x13E3
        out    dx, ax
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_02B2
        mov    al, 0x6a
        mov    ah, byte ptr [0x13e4]         ; -> loc_1_13E4
        out    dx, ax
        jmp    loc_4_02D0
loc_4_02B2:
        mov    al, 0x35
        mov    ah, byte ptr [0x13e4]         ; -> loc_1_13E4
        out    dx, ax
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_02D0
        mov    al, 0x51
        out    dx, al
        inc    edx
        mov    al, byte ptr [0x13e5]         ; -> obj1:0x13E5
        out    dx, al
        dec    edx
loc_4_02D0:
        rol    eax, 8
        out    dx, al
loc_4_02D4:
        pop    edx
        pop    eax
        ret    
loc_4_02D7:
        or     byte ptr [0x13d9], 2          ; -> obj1:0x13D9
        mov    eax, dword ptr [0x135c]       ; -> obj1:0x135C
        or     eax, eax
        je     loc_4_0324
        mov    word ptr [eax], 0
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    dl
        in     al, dx
        or     al, 1
        and    al, 0xf7
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
loc_4_030C:
        mov    edx, 0x9ae9
        in     al, dx
        test   al, 2
        je     loc_4_0324
        cmp    al, 0xff
        je     loc_4_0323
        mov    edx, 0xe2e8
        out    dx, ax
        jmp    loc_4_030C
loc_4_0323:
        int3   
loc_4_0324:
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        ret    
loc_4_033B:
        mov    edx, 0x4ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    byte ptr [0x13d9], 0          ; -> obj1:0x13D9
        cmp    ebx, dword ptr [0x1348]       ; -> obj1:0x1348
        je     loc_4_0365
        call   sub_4_0376
loc_4_0365:
        ret    
loc_4_0366:
        or     byte ptr [0x13d9], 1          ; -> obj1:0x13D9
        ret    
loc_4_036E:
        mov    byte ptr [0x13d9], 0          ; -> obj1:0x13D9
        ret    
sub_4_0376:
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_03D2
        push   edx
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        mov    al, 0x42
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 3], al
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_03C4
        mov    al, 0x53
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 5], al
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 6], al
loc_4_03C4:
        mov    dl, 0xd4
        rol    eax, 8
        out    dx, al
        mov    dl, 0xcc
        in     al, dx
        mov    byte ptr [edi + 0xc], al
        pop    edi
        pop    edx
loc_4_03D2:
        ret    
loc_4_03D3:
        push   edx
        cmp    byte ptr [0x13da], 0          ; -> loc_1_13DA
        je     loc_4_051D
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_051D
        cmp    ecx, dword ptr [0x1348]       ; -> obj1:0x1348
        jne    loc_4_0405
        mov    ecx, 0x1368                   ; -> loc_1_1368
        sub    ecx, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    esi, ecx
        jmp    loc_4_0418
loc_4_0405:
        cmp    esi, dword ptr [0x1344]       ; -> obj1:0x1344
        jne    loc_4_0418
        mov    esi, 0x1378                   ; -> loc_1_1378
        sub    esi, dword ptr [0x1354]       ; -> obj1:0x1354
loc_4_0418:
        cmp    ecx, dword ptr [0x1340]       ; -> obj1:0x1340
        jne    loc_4_0431
        mov    al, byte ptr [0x13e7]         ; -> obj1:0x13E7
        or     al, al
        je     loc_4_045D
        mov    edx, 0x3c2
        out    dx, al
        jmp    loc_4_045D
loc_4_0431:
        add    ecx, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    edx, 0x3c2
        mov    al, byte ptr [ecx + 0xc]
        out    dx, al
        and    al, 0xc
        cmp    al, 0xc
        jne    loc_4_0457
        mov    dl, 0xd4
        in     al, dx
        ror    eax, 8
        mov    al, 0x42
        mov    ah, byte ptr [ecx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_0457:
        sub    ecx, dword ptr [0x1354]       ; -> obj1:0x1354
loc_4_045D:
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_051D
        cmp    esi, dword ptr [0x1340]       ; -> obj1:0x1340
        jne    loc_4_04D4
        test   byte ptr [0x13d9], 3          ; -> obj1:0x13D9
        jne    loc_4_04D4
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        mov    edi, dword ptr [0x135c]       ; -> obj1:0x135C
        mov    ah, byte ptr [edi]
        and    ax, 0x1fe
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x53
        out    dx, al
        inc    edx
        in     al, dx
        mov    edi, dword ptr [0x1360]       ; -> obj1:0x1360
        mov    ah, byte ptr [edi]
        and    ax, 0x10ef
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    edi, dword ptr [0x135c]       ; -> obj1:0x135C
        mov    ah, byte ptr [edi]
        not    ah
        and    ax, 0x1ef
        shl    ah, 4
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        jmp    loc_4_051D
loc_4_04D4:
        add    esi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 2]
        and    ax, 0x9f6
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x53
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 5]
        and    ax, 0x10ef
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 6]
        and    ax, 0x10ef
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_051D:
        pop    edx
        ret    
loc_4_051F:
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_055F
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_055F
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    ax, 0xa5a
        out    dx, ax
        mov    ax, 0x59
        out    dx, ax
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfc
        out    dx, al
        dec    edx
        mov    ax, 0x39
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_055F:
        ret    
loc_4_0560:
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_05C0
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_05C0
        cmp    byte ptr [0x13d6], 0          ; -> loc_1_13D6
        jne    loc_4_05C0
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfe
        out    dx, al
        dec    edx
        mov    ax, word ptr [0x13d2]         ; -> loc_1_13D2
        out    dx, ax
        mov    ax, word ptr [0x13d4]         ; -> loc_1_13D4
        out    dx, ax
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [0x13e6]         ; -> loc_1_13E6
        and    ax, 0x3fc
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_05C0:
        ret    
loc_4_05C1:
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_0689
        mov    edx, 0x9ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x9ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x96e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x86e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x82e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xb6e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa6e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x92e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xb2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_4_0689:
        mov    edx, 0x4ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        ret    
sub_4_06A0:
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_0768
        mov    edx, 0x9ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x9ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x96e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x86e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x82e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xb6e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa6e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x92e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xb2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_4_0768:
        ret    
loc_4_0769:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0x1340]       ; -> obj1:0x1340
        jne    loc_4_077D
        cmp    ebx, edi
        jne    loc_4_0787
loc_4_077D:
        cmp    ecx, 4
        je     loc_4_0786
        out    dx, ax
        jmp    loc_4_0787
loc_4_0786:
        out    dx, al
loc_4_0787:
        pop    esi
        pop    edi
        ret    
loc_4_078A:
        push   edx
        cmp    eax, dword ptr [0x1340]       ; -> obj1:0x1340
        jne    loc_4_07FE
        cmp    byte ptr [0x13db], 0          ; -> obj1:0x13DB
        je     loc_4_07FE
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 1
        je     loc_4_07FE
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_07DB
        test   ah, 8
        jne    loc_4_07FE
        ror    eax, 8
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 0x10
        jne    loc_4_07FE
loc_4_07DB:
        mov    eax, dword ptr [0x1358]       ; -> obj1:0x1358
        cmp    byte ptr [eax], 0
        jne    loc_4_07FE
        mov    ecx, 0x1000
loc_4_07EA:
        mov    edx, 0x9ae8
        in     ax, dx
        test   ah, 2
        je     loc_4_07FE
        call   sub_4_0800
        dec    ecx
        jne    loc_4_07EA
loc_4_07FE:
        pop    edx
        ret    
sub_4_0800:
        push   ecx
        mov    ecx, 0x3e8
loc_4_0806:
        push   ecx
        pop    ecx
        loop   loc_4_0806
        pop    ecx
        ret    
loc_4_080C:
        cmp    cl, 0x30
        jae    loc_4_0814
        clc    
        jmp    loc_4_084C
loc_4_0814:
        dec    dl
        in     al, dx
        mov    ch, al
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        cmp    cl, 0x40
        jne    loc_4_0832
        mov    al, 0x40
        out    dx, al
        inc    dl
        in     al, dx
        jmp    loc_4_083D
loc_4_0832:
        cmp    cl, 0x30
        jne    loc_4_0842
        mov    al, 0x30
        out    dx, al
        inc    dl
        in     al, dx
loc_4_083D:
        dec    dl
        stc    
        jmp    loc_4_0843
loc_4_0842:
        clc    
loc_4_0843:
        xchg   ch, al
        out    dx, al
        mov    dl, 0xd5
        mov    al, ch
        mov    ch, 0
loc_4_084C:
        ret    
loc_4_084D:
        cmp    cl, 0x30
        cmc    
        jae    loc_4_08BD
        cmp    byte ptr [0x13dc], 0xff       ; -> loc_1_13DC
        jb     loc_4_08BD
        dec    dl
        push   eax
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    eax
        inc    dl
        cmp    cl, 0x40
        jne    loc_4_0888
        mov    edi, ebx
        add    edi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    byte ptr [edi + 2], al
        jmp    loc_4_08BC
loc_4_0888:
        cmp    cl, 0x42
        jne    loc_4_089A
        mov    edi, ebx
        add    edi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    byte ptr [edi + 3], al
        jmp    loc_4_08BC
loc_4_089A:
        cmp    cl, 0x53
        jne    loc_4_08AC
        mov    edi, ebx
        add    edi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    byte ptr [edi + 5], al
        jmp    loc_4_08BC
loc_4_08AC:
        cmp    cl, 0x58
        jne    loc_4_08BC
        mov    edi, ebx
        add    edi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    byte ptr [edi + 6], al
loc_4_08BC:
        clc    
loc_4_08BD:
        ret    
loc_4_08BE:
        pushfd 
        test   cl, 4
        je     loc_4_08CF
        push   ebx
        add    ebx, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    byte ptr [ebx + 0xc], al
        pop    ebx
loc_4_08CF:
        popfd  
        jmp    loc_1_1364
loc_4_08D6:
        mov    ecx, dword ptr [0x134c]       ; -> obj1:0x134C
        stc    
        ret    
loc_4_08DE:
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_08FC
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        or     al, al
        jne    loc_4_0920
loc_4_08FC:
        xor    ah, ah
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_0911
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
loc_4_0911:
        mov    al, 0x35
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0xc0f
        shl    ah, 2
        or     al, ah
loc_4_0920:
        mov    dl, 0xd4
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_4_092B:
        mov    edx, 0x10000
        xor    eax, eax
        stc    
        ret    
loc_4_0934:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi], al
        or     al, 1
        out    dx, al
        dec    dl
        mov    al, 0x35
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 1], al
        dec    dl
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_09DF
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 4], al
        out    dx, al
        dec    dl
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_0995
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 9], al
        dec    dl
loc_4_0995:
        mov    al, 0x40
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 2], al
        and    al, 0xfe
        or     al, 8
        out    dx, al
        dec    dl
        mov    al, 0x53
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 5], al
        and    al, 0xef
        out    dx, al
        dec    dl
        mov    al, 0x59
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 7], al
        xor    al, al
        out    dx, al
        dec    dl
        mov    al, 0x5a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 8], al
        mov    al, 0xa
        out    dx, al
        dec    dl
        mov    al, 0x58
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 6], al
        and    al, 0xfc
        or     al, 0x10
        out    dx, al
        dec    dl
loc_4_09DF:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_4_09E8:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x1354]       ; -> obj1:0x1354
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 0x35
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_0A58
        mov    al, 0x51
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_0A35
        mov    al, 0x6a
        mov    ah, byte ptr [esi + 9]
        out    dx, ax
loc_4_0A35:
        mov    al, 0x40
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        mov    al, 0x53
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        mov    al, 0x59
        mov    ah, byte ptr [esi + 7]
        out    dx, ax
        mov    al, 0x5a
        mov    ah, byte ptr [esi + 8]
        out    dx, ax
        mov    al, 0x58
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
loc_4_0A58:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_4_0A61:
        push   edx
        push   eax
        push   ecx
        mov    cl, dl
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x35
        mov    ah, cl
        and    ah, 0xf
        out    dx, ax
        cmp    byte ptr [0x13d7], 0x80       ; -> obj1:0x13D7
        je     loc_4_0AB8
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xf3
        mov    ah, cl
        and    ah, 0x30
        shr    ah, 2
        or     al, ah
        out    dx, al
        dec    dl
        cmp    byte ptr [0x13d8], 0          ; -> loc_1_13D8
        je     loc_4_0AB8
        mov    al, 0x6a
        mov    ah, cl
        test   cl, 0xc0
        jne    loc_4_0AB6
        xor    ah, ah
loc_4_0AB6:
        out    dx, ax
loc_4_0AB8:
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_0AC1:
        push   eax
        mov    eax, 0x86
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    eax
        clc    
        ret    
        db  020h, 053h
VxD__LTEXT ENDS
