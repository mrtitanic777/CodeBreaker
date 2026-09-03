; S3MINI -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 3450 instructions recovered by recursive descent

Declare_Virtual_Device S3MINI, 3, 1, S3MINI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x15d0  R-X
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
        dd  loc_1_007E
        dd  loc_1_000C
        dd  loc_1_00D5
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
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_1_00D4
        pushal 
        mov    byte ptr [0x1538], 1          ; -> loc_1_1538
        mov    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4022
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
loc_1_00D4:
        ret    
loc_1_00D5:
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_1_00F6
        pushal 
        mov    eax, 0xfa
        mov    esi, 0xf7                     ; -> loc_1_00F7
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x1549], esi       ; -> obj1:0x1549
        popal  
loc_1_00F6:
        ret    
loc_1_00F7:
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_1_012E
        pushal 
        mov    esi, dword ptr [0x1549]       ; -> obj1:0x1549
        cmp    esi, 0
        je     loc_1_011C
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    dword ptr [0x1549], 0         ; -> obj1:0x1549
loc_1_011C:
        mov    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        mov    esi, 0x12f                    ; -> loc_1_012F
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        popal  
loc_1_012E:
        ret    
loc_1_012F:
        pushal 
        mov    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4023
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        popal  
        ret    
loc_1_0178:
        pushal 
        mov    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4021
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        popal  
        ret    
loc_1_01C1:
        cmp    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        je     loc_1_0220
        cmp    edx, 0x9ae8
        je     loc_1_01D9
        cmp    edx, 0x9ae9
        jne    loc_1_0220
loc_1_01D9:
        cmp    ecx, 0
        je     loc_1_0201
        cmp    ecx, 8
        jne    loc_1_0220
        mov    ecx, 0xffff
loc_1_01E8:
        in     ax, dx
        cmp    ax, -1
        je     loc_1_01FC
        test   ah, 2
        je     loc_1_021D
        call   sub_4_0BA7
        loop   loc_1_01E8
loc_1_01FC:
        and    ah, 0xfd
        jmp    loc_1_021D
loc_1_0201:
        cmp    dl, 0xe9
        jne    loc_1_0220
        mov    ecx, 0xffff
loc_1_020B:
        in     al, dx
        cmp    al, 0xff
        je     loc_1_021B
        test   al, 2
        je     loc_1_021D
        call   sub_4_0BA7
        loop   loc_1_020B
loc_1_021B:
        and    al, 0xfd
loc_1_021D:
        stc    
        jmp    loc_1_0221
loc_1_0220:
        clc    
loc_1_0221:
        ret    
loc_1_0222:
        cmp    dx, 0x4f00
        je     loc_1_023F
        cmp    dx, 0x4f02
        jne    loc_1_0264
        cmp    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        je     loc_1_0264
        call   sub_4_0549
        jmp    loc_1_0264
loc_1_023F:
        cmp    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        jne    loc_1_0264
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
loc_1_0264:
        ret    
        db  000h, 000h, 000h
sub_1_0268:
        push   eax
        push   ebx
        push   ecx
        push   edx
        test   word ptr [eax + 2], 0x200
        jne    loc_1_02AC
        movzx  ecx, word ptr [eax + 0x24]
        movzx  edx, word ptr [eax + 0x26]
        mov    ebx, 0x434                    ; -> loc_1_0434
loc_1_0281:
        mov    al, byte ptr [ebx]
        cmp    dl, al
        jb     loc_1_0299
        push   ebx
        mov    eax, edi
        shr    eax, 0x10
        and    eax, 0xf
        add    ebx, eax
        mov    al, byte ptr [ebx]
        cmp    al, 0xff
        pop    ebx
        jne    loc_1_02A0
loc_1_0299:
        add    ebx, 7
        or     al, al
        jne    loc_1_0281
loc_1_02A0:
        xchg   bx, si
        cmp    bl, al
        jbe    loc_1_02A9
        xchg   al, bl
loc_1_02A9:
        mov    si, bx
loc_1_02AC:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_1_02B1:
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
        add    byte ptr [esi + 8], dl        ; -> loc_1_0856
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    al, 0
        push   cs
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [edx]
        add    byte ptr [eax + 7], al        ; -> loc_1_0740
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax + eax]
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x24000], al
        push   esi
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x44000], al
        push   cs
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 2
        add    al, byte ptr [eax]
        inc    eax
        pop    es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 2
        add    al, 0
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 2
        add    byte ptr [esi + 8], dl        ; -> loc_1_0856
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax + 0xe000400], al ; -> loc_1_060E
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [edx], al
        add    byte ptr [eax + 7], al        ; -> loc_1_0740
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], al
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x20080], al
        push   esi
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x40080], al
        push   cs
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 0x10
        add    al, byte ptr [eax]
        inc    eax
        pop    es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 0x10
        add    al, 0
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, byte ptr [eax]
        insb   byte ptr es:[edi], dx
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, 0
        or     byte ptr [ecx], 0
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + 0x200000a], bl ; -> loc_1_0A99
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax + eax], eax
        cwde   
        or     dword ptr [eax], eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 2
        add    byte ptr [eax], dl            ; -> loc_1_0F10
        sldt   word ptr [eax]
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], al
        add    byte ptr [eax], bl            ; -> loc_1_1018
        adc    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 2
        add    byte ptr [edx + 0x100000b], bl ; -> loc_1_0B9A
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 1
        add    byte ptr [edx + 0x100000c], ah ; -> loc_1_0CA2
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [edx + eax], al
        add    byte ptr [ebp + 0xd], dl      ; -> loc_1_0D55
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 1
        add    byte ptr [ebp + 0xe], bl      ; -> loc_1_0E5D
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + eax], al
        add    bl, cl                        ; -> loc_1_10CB
        adc    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 1
        add    byte ptr [esi + 0x4000011], bh ; -> loc_1_11BE
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 2
        add    byte ptr [edx + 0x12], ch     ; -> loc_1_126A
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], al
        add    byte ptr [0x13], ah           ; -> loc_1_1325
loc_1_0434:
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
loc_1_04DC:
        pop    ebx
        loopne loc_1_04F2
        add    al, 0
        add    dl, dh                        ; -> loc_1_04F2
        add    al, 0
        add    byte ptr [edi + 5], al        ; -> loc_1_0547
        add    byte ptr [eax], al
        pushfd 
        add    eax, 0x5b90000                ; -> loc_1_05B9
        add    byte ptr [eax], al
loc_1_04F2:
        js     loc_1_0502
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0502:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0547:
        js     loc_1_0557
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0557:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_059C:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_05B9:
        js     loc_1_05C9
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_05C9:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_060E:
        pop    ebx
        loopne loc_1_0624
        push   es
        add    byte ptr [eax], al
        and    al, 6
        add    byte ptr [eax], al
        jns    loc_1_0620
        add    byte ptr [eax], al
        into   
        push   es
        add    byte ptr [eax], al
loc_1_0620:
        jmp    0x628
        db  000h, 000h
loc_1_0624:
        js     loc_1_0634
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0634:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0679:
        js     loc_1_0689
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0689:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_06CE:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_06EB:
        js     loc_1_06FB
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_06FB:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0740:
        pop    ebx
        loopne loc_1_0756
        pop    es
        add    byte ptr [eax], al
        push   esi
        pop    es
        add    byte ptr [eax], al
        stosd  dword ptr es:[edi], eax
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    dword ptr [eax], ecx
        add    byte ptr [eax], al
loc_1_0756:
        js     loc_1_0766
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0766:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_07AB:
        js     loc_1_07BB
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_07BB:
        add    al, 0xff
        add    al, 0xff
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0801:
        js     loc_1_0811
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0811:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0856:
        pop    ebx
        loopne loc_1_086C
        or     byte ptr [eax], al
        add    byte ptr [eax + ecx], ch      ; -> loc_1_086C
        add    cl, al                        ; -> loc_1_08C1
        or     byte ptr [eax], al
        add    byte ptr [esi], dl            ; -> obj1:0x0916
        or     dword ptr [eax], eax
        add    byte ptr [edi], dl            ; -> loc_1_0917
        or     dword ptr [eax], eax
        db  000h
loc_1_086C:
        js     loc_1_087C
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_087C:
        add    al, 0xff
        add    al, 0xf
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_08C1:
        js     loc_1_08D1
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_08D1:
        add    al, 0xff
        add    al, 0xff
        call   dword ptr [eax - 1]
        dec    dword ptr [0x4cffffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0917:
        js     loc_1_0927
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0927:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_096C:
        pop    ebx
        loopne loc_1_0AAF
        or     al, byte ptr [eax]
        add    byte ptr [edi - 0x2fffff6], ch ; -> loc_1_0AAF
        or     al, byte ptr [eax]
        add    byte ptr [ebx + 0xb], cl      ; -> obj1:0x0B4B
        add    byte ptr [eax], al
        dec    esp
        or     eax, dword ptr [eax]
        add    byte ptr [ebx - 0x20], bl
        scasb  al, byte ptr es:[edi]
        or     dword ptr [eax], eax
        add    byte ptr [esi - 0x3fffff7], ch ; -> loc_1_09AE
        or     dword ptr [eax], eax
        add    byte ptr [edx + 0xa], cl      ; -> obj1:0x0A4A
        add    byte ptr [eax], al
        dec    ebx
        or     al, byte ptr [eax]
        add    byte ptr [ebx - 0x20], bl
        scasb  al, byte ptr es:[edi]
        or     dword ptr [eax], eax
        add    byte ptr [esi - 0x3fffff7], ch ; -> loc_1_09AE
        or     dword ptr [eax], eax
        add    byte ptr [edx + 0xa], cl      ; -> obj1:0x0A4A
        add    byte ptr [eax], al
        dec    ebx
        or     al, byte ptr [eax]
        db  000h
loc_1_09AE:
        js     loc_1_09BE
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_09BE:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_09FC:
        js     loc_1_0A0C
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A0C:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h, 000h
loc_1_0A4B:
        js     loc_1_0A5B
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A5B:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0A99:
        pop    ebx
        loopne loc_1_0AAF
        or     al, byte ptr [eax]
        add    byte ptr [edi - 0x2fffff6], ch ; -> loc_1_0AAF
        or     al, byte ptr [eax]
        add    byte ptr [ebx + 0xb], cl      ; -> obj1:0x0B4B
        add    byte ptr [eax], al
        dec    esp
        or     eax, dword ptr [eax]
        db  000h
loc_1_0AAF:
        js     loc_1_0ABF
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0ABF:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_0AFD:
        js     loc_1_0B0D
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0B0D:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0B4C:
        js     loc_1_0B5C
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0B5C:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_0B9A:
        pop    ebx
        loopne loc_1_0BB0
        or     eax, dword ptr [eax]
        add    byte ptr [eax - 0x1fffff5], dh ; -> loc_1_0BB0
        or     eax, dword ptr [eax]
        add    byte ptr [esp + ecx], cl      ; -> obj1:0x0C4C
        add    byte ptr [ebp + 0xc], cl      ; -> loc_1_0C4D
        add    byte ptr [eax], al
loc_1_0BB0:
        js     loc_1_0BC0
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0BC0:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0BFE:
        js     loc_1_0C0E
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0C0E:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0C4D:
        js     0xc4e
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0CA2:
        pop    ebx
        loopne loc_1_0CB8
        or     al, 0
        add    byte ptr [eax + 0x600000c], bh ; -> loc_1_0CB8
        or     eax, 0xd540000                ; -> obj1:0x0D54
        add    byte ptr [eax], al
        push   esp
        db  00Dh, 000h, 000h
loc_1_0CB8:
        js     loc_1_0CC8
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0CC8:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0D06:
        js     loc_1_0D16
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0D16:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0D55:
        pop    ebx
        loopne loc_1_0D6B
        or     eax, 0xd6b0000                ; -> loc_1_0D6B
        add    byte ptr [eax], al
        mov    ecx, 0x700000d                ; -> obj1:0x0E07
        push   cs
        add    byte ptr [eax], al
        pop    es
        push   cs
        add    byte ptr [eax], al
loc_1_0D6B:
        js     loc_1_0D7B
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0D7B:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0DB9:
        js     loc_1_0DC9
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0DC9:
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
loc_1_0E5D:
        pop    ebx
        loopne loc_1_0E73
        push   cs
        add    byte ptr [eax], al
        jae    loc_1_0E73
        add    byte ptr [eax], al
        ror    dword ptr [esi], 0
        add    byte ptr [edi], cl            ; -> obj1:0x0F0F
        sldt   word ptr [eax]
        dd  obj1:0x0F0F              ; fixup
loc_1_0E73:
        js     loc_1_0E83
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0E83:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0EC1:
        js     0xed1
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0F10:
        imul   esp, eax, 0x26                ; -> loc_1_0F26
        sldt   word ptr [eax]
        sldt   word ptr es:[eax]
        je     0xf2b
        add    byte ptr [eax], al
        ret    0xf
        db  000h
        dd  obj1:0x0FC2              ; fixup
loc_1_0F26:
        js     loc_1_0F36
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0F36:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0F74:
        js     loc_1_0F84
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0F84:
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
loc_1_1018:
        imul   esp, eax, 0x2e                ; -> loc_1_102E
        adc    byte ptr [eax], al
        add    byte ptr [esi], ch            ; -> loc_1_102E
        adc    byte ptr [eax], al
        add    byte ptr [eax + edx], bh      ; -> loc_1_107C
        add    dl, cl                        ; -> obj1:0x10CA
        adc    byte ptr [eax], al
        add    dl, cl                        ; -> obj1:0x10CA
        adc    byte ptr [eax], al
        db  000h
loc_1_102E:
        js     loc_1_103E
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_103E:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_107C:
        js     loc_1_108C
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_108C:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_10CB:
        pop    ebx
        loopne loc_1_10E1
        adc    byte ptr [eax], al
        add    cl, ah                        ; -> loc_1_10E1
        adc    byte ptr [eax], al
        add    byte ptr [eax], ch            ; -> loc_1_1128
        adc    dword ptr [eax], eax
        add    byte ptr [edi + 0x11], ch     ; -> loc_1_116F
        add    byte ptr [eax], al
        ja     0x10f0
        add    byte ptr [eax], al
loc_1_10E1:
        js     loc_1_10F1
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_10F1:
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
loc_1_1128:
        js     loc_1_1138
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1138:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_116F:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1177:
        js     0x1187
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_11BE:
        pop    ebx
        loopne loc_1_11D4
        adc    dword ptr [eax], eax
        add    ah, dl                        ; -> loc_1_11D4
        adc    dword ptr [eax], eax
        add    byte ptr [ebx], bl            ; -> loc_1_121B
        adc    al, byte ptr [eax]
        add    byte ptr [edx + 0x12], ah     ; -> loc_1_1262
        add    byte ptr [eax], al
        bound  edx, qword ptr [edx]
        add    byte ptr [eax], al
loc_1_11D4:
        js     loc_1_11E4
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_11E4:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_121B:
        js     loc_1_122B
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_122B:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1262:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_126A:
        imul   esp, eax, -0x80               ; -> loc_1_1280
        adc    al, byte ptr [eax]
        add    byte ptr [eax - 0x31ffffee], al ; -> loc_1_1280
        adc    al, byte ptr [eax]
        add    byte ptr [ebx + edx], bl      ; -> obj1:0x131C
        add    byte ptr [eax], al
        dd  loc_1_131D               ; fixup
loc_1_1280:
        js     loc_1_1290
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1290:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_12CE:
        js     0x12de
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_131D:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1325:
        imul   esp, eax, 0x3b                ; -> loc_1_133B
        adc    eax, dword ptr [eax]
        add    byte ptr [ebx], bh            ; -> loc_1_133B
        adc    eax, dword ptr [eax]
        add    byte ptr [ecx - 0x28ffffed], cl ; -> loc_1_1389
        adc    eax, dword ptr [eax]
        add    bh, bl                        ; -> obj1:0x13DF
        adc    eax, dword ptr [eax]
        db  000h
loc_1_133B:
        js     loc_1_134B
        push   cs
        ud0    
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_134B:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_1389:
        js     loc_1_1399
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1399:
        add    al, 0xff
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_13D7:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_13E0:
        cmp    ebx, dword ptr [0x1570]       ; -> obj1:0x1570
        jne    loc_1_13F9
        cmp    word ptr [0x15cc], 0x40       ; -> loc_1_15CC
        jne    loc_1_13F9
        cmp    dx, 0x4f15
        je     loc_1_1408
loc_1_13F9:
        cmp    dword ptr [0x15bf], 0         ; -> loc_1_15BF
        je     loc_1_142F
        jmp    loc_1_15BF
loc_1_1408:
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
loc_1_142F:
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_14B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14E4:
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
loc_1_1526:
        add    byte ptr [eax], al
loc_1_1528:
        add    byte ptr [eax], al
loc_1_152A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1530:
        add    byte ptr [eax], al
loc_1_1532:
        add    byte ptr [eax], al
loc_1_1534:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1538:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_153C:
        add    byte ptr [eax], al
loc_1_153E:
        inc    dword ptr [eax]
loc_1_1540:
        add    byte ptr [eax], al
loc_1_1542:
        inc    dword ptr [eax]
loc_1_1544:
        add    byte ptr [eax], al
loc_1_1546:
        add    byte ptr [eax], al
loc_1_1548:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x4e], al
        push   ebp
        dec    ebp
        pop    esp
        push   eax
        inc    ebx
        dec    ecx
        pop    esp
        push   esi
        inc    ebp
        dec    esi
        pop    edi
        cmp    byte ptr [eax], dh
        cmp    byte ptr [esi], dh
        inc    esp
        inc    ebp
        push   esi
        pop    edi
        xor    byte ptr [eax + edi], dh
        add    byte ptr ss:[eax], al
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15BF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15C3:
        and    eax, 0x5b000013
        add    byte ptr [ecx], bh
loc_1_15CA:
        add    byte ptr [eax], al
loc_1_15CC:
        add    byte ptr [eax], al
loc_1_15CE:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x868  R-X
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
        mov    dword ptr [0x1484], ebx       ; -> obj1:0x1484
        mov    byte ptr [0x1532], 0          ; -> loc_1_1532
        pushal 
        mov    eax, 0x20
        mov    esi, 0x854                    ; -> loc_2_0854
        mov    edi, 0x85c                    ; -> loc_2_085C
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        cmp    eax, 0x20
        je     loc_2_004E
        cmp    eax, 0x40
        je     loc_2_004E
        mov    eax, 0x20
loc_2_004E:
        shl    eax, 0xa
        mov    dword ptr [0x1480], eax       ; -> obj1:0x1480
        popal  
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_058B
        mov    dword ptr [0x1494], eax       ; -> obj1:0x1494
        mov    dword ptr [0x14b0], 0         ; -> obj1:0x14B0
        mov    dword ptr [0x14b4], 0x9ae8    ; -> obj1:0x14B4
        xor    eax, eax
        mov    edx, 0x3d4
        mov    ax, 0x4838
        out    dx, ax
        mov    al, 0x36
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 3
        mov    byte ptr [0x1531], al         ; -> obj1:0x1531
        dec    edx
        mov    al, 0x30
        out    dx, al
        inc    dx
        in     al, dx
        cmp    al, 0xe1
        je     loc_2_0106
        mov    ch, al
        and    ch, 0xf0
        mov    byte ptr [0x152b], ch         ; -> obj1:0x152B
        and    al, 0xf
        mov    byte ptr [0x152f], al         ; -> obj1:0x152F
        cmp    ch, 0x80
        je     loc_2_01CC
        cmp    ch, 0x90
        je     loc_2_01CC
        cmp    ch, 0xb0
        je     loc_2_01CC
        cmp    ch, 0xa0
        je     loc_2_01CC
        mov    byte ptr [0x1530], 0xff       ; -> loc_1_1530
        cmp    ch, 0xc0
        je     loc_2_01CC
        cmp    ch, 0xd0
        je     loc_2_01CC
        jmp    loc_2_058B
loc_2_0106:
        mov    byte ptr [0x1530], 0xff       ; -> loc_1_1530
        or     dword ptr [0x14b0], 1         ; -> obj1:0x14B0
        xor    eax, eax
        dec    dl
        mov    al, 0x2d
        out    dx, al
        inc    dl
        in     al, dx
        shl    eax, 8
        dec    dl
        mov    al, 0x2e
        out    dx, al
        inc    dl
        in     al, dx
        shl    eax, 8
        dec    dl
        mov    al, 0x2f
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x152f], al         ; -> obj1:0x152F
        and    al, 0x40
        mov    dword ptr [0x152b], eax       ; -> obj1:0x152B
        cmp    eax, 0x881100
        je     loc_2_018E
        cmp    eax, 0x881000
        je     loc_2_018E
        cmp    eax, 0x888000
        je     loc_2_01BB
        cmp    eax, 0x889000
        je     loc_2_01BB
        cmp    eax, 0x88b000
        je     loc_2_01BB
        cmp    eax, 0x88f000
        je     loc_2_01BB
        cmp    eax, 0x881140
        je     loc_2_017C
        cmp    eax, 0x881200
        jne    loc_2_058B
loc_2_017C:
        or     byte ptr [0x1533], 1          ; -> obj1:0x1533
        cmp    byte ptr [0x1531], 1          ; -> obj1:0x1531
        jne    loc_2_01BB
        jmp    loc_2_01C5
loc_2_018E:
        cmp    byte ptr [0x152f], 0x20       ; -> obj1:0x152F
        je     loc_2_01C5
        cmp    byte ptr [0x152f], 0x30       ; -> obj1:0x152F
        je     loc_2_01C5
        cmp    byte ptr [0x152f], 3          ; -> obj1:0x152F
        je     loc_2_01C5
        cmp    byte ptr [0x152f], 0x12       ; -> obj1:0x152F
        je     loc_2_01C5
        cmp    byte ptr [0x152f], 0x22       ; -> obj1:0x152F
        jne    loc_2_01CC
loc_2_01BB:
        mov    dword ptr [0x14b4], 0x8118    ; -> obj1:0x14B4
loc_2_01C5:
        or     dword ptr [0x14b0], 2         ; -> obj1:0x14B0
loc_2_01CC:
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
        je     loc_2_058B
        dec    dl
        mov    eax, 0x14dc                   ; -> loc_1_14DC
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        mov    eax, dword ptr [0x14e4]       ; -> loc_1_14E4
        or     eax, 0x20202020
        cmp    eax, 0x642e3373
        je     loc_2_0248
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_2_058B
        cmp    dword ptr [0x152b], 0x90      ; -> obj1:0x152B
        je     loc_2_058B
        cmp    dword ptr [0x152b], 0xb0      ; -> obj1:0x152B
        je     loc_2_058B
        cmp    dword ptr [0x152b], 0xa0      ; -> obj1:0x152B
        je     loc_2_058B
loc_2_0248:
        mov    byte ptr [0x156c], 0          ; -> obj1:0x156C
        mov    eax, 0x154d                   ; -> obj1:0x154D
        mov    edx, 0x1568                   ; -> obj1:0x1568
        push   edx
        push   eax
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_2_0283
        inc    byte ptr [0x156c]             ; -> obj1:0x156C
        push   dword ptr [0x1568]            ; -> obj1:0x1568
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_0283:
        mov    dx, 0x3d4
        mov    al, 0x36
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        mov    cl, al
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_2_02E2
        shr    cl, 5
        mov    eax, 0x400000
        or     cl, cl
        je     loc_2_02F1
        mov    eax, 0x300000
        cmp    cl, 2
        je     loc_2_02F1
        mov    eax, 0x800000
        cmp    cl, 3
        je     loc_2_02F1
        mov    eax, 0x200000
        cmp    cl, 4
        je     loc_2_02F1
        mov    eax, 0x600000
        cmp    cl, 5
        je     loc_2_02F1
        mov    eax, 0x100000
        cmp    cl, 6
        je     loc_2_02F1
        mov    eax, 0x80000
        jmp    loc_2_02F1
loc_2_02E2:
        mov    eax, 0x100000
        test   cl, 0x20
        je     loc_2_02F1
        mov    eax, 0x80000
loc_2_02F1:
        mov    dword ptr [0x1490], eax       ; -> obj1:0x1490
        push   0
        push   0x10
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x1498], eax       ; -> obj1:0x1498
        or     eax, eax
        je     loc_2_058B
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_058B
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0        ; -> loc_4_0000
        mov    dword ptr [edi + 8], 0x12b    ; -> loc_4_012B
        mov    dword ptr [edi + 0xc], 0x1b9  ; -> loc_4_01B9
        mov    dword ptr [edi + 0x58], 0x26e ; -> loc_4_026E
        mov    dword ptr [edi + 0x5c], 0x2ef ; -> loc_4_02EF
        mov    dword ptr [edi + 0x10], 0x34a ; -> loc_4_034A
        mov    dword ptr [edi + 0x14], 0x4ce ; -> loc_4_04CE
        mov    dword ptr [edi + 0x18], 0x4f9 ; -> loc_4_04F9
        mov    dword ptr [edi + 0x1c], 0x501 ; -> loc_4_0501
        mov    dword ptr [edi + 0x20], 0x549 ; -> sub_4_0549
        mov    dword ptr [edi + 0x24], 0x5c7 ; -> loc_4_05C7
        mov    dword ptr [edi + 0x2c], 0x7f5 ; -> loc_4_07F5
        mov    dword ptr [edi + 0x30], 0x845 ; -> loc_4_0845
        mov    dword ptr [edi + 0x34], 0x8b7 ; -> loc_4_08B7
        mov    dword ptr [edi + 0x38], 0x996 ; -> sub_4_0996
        mov    dword ptr [edi + 0x3c], 0xa80 ; -> loc_4_0A80
        mov    dword ptr [edi + 0x40], 0xbb3 ; -> loc_4_0BB3
        mov    dword ptr [edi + 0x44], 0xc08 ; -> loc_4_0C08
        mov    dword ptr [edi + 0xb8], 0x595 ; -> loc_3_0595
        mov    dword ptr [edi + 0xb4], 0x55b ; -> loc_3_055B
        mov    dword ptr [edi + 0x68], 0x4c4 ; -> loc_3_04C4
        mov    dword ptr [edi + 0x70], 0xc9  ; -> loc_3_00C9
        mov    dword ptr [edi + 0x74], 0x49e ; -> loc_3_049E
        mov    dword ptr [edi + 0x90], 0xcc8 ; -> loc_4_0CC8
        mov    dword ptr [edi + 0x80], 0xcd0 ; -> loc_4_0CD0
        mov    dword ptr [edi + 0x84], 0xcd0 ; -> loc_4_0CD0
        mov    dword ptr [edi + 0x94], 0xd20 ; -> loc_4_0D20
        mov    dword ptr [edi + 0x88], 0xe67 ; -> loc_4_0E67
        mov    dword ptr [edi + 0x9c], 0xd29 ; -> loc_4_0D29
        mov    dword ptr [edi + 0xa0], 0xdeb ; -> loc_4_0DEB
        mov    dword ptr [edi + 0xa8], 0x58e ; -> loc_2_058E
        mov    dword ptr [edi + 0xb0], 0x1c1 ; -> loc_1_01C1
        mov    dword ptr [edi + 0xa4], 0xedb ; -> loc_4_0EDB
        mov    dword ptr [edi + 0xbc], 0x222 ; -> loc_1_0222
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        jne    loc_2_0450
        mov    dword ptr [edi + 0x98], 0xeca ; -> loc_4_0ECA
loc_2_0450:
        mov    edx, 0x3c2
        mov    ecx, 0xcb0                    ; -> loc_4_0CB0
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    dword ptr [0x14b8], ecx       ; -> loc_1_14B8
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0x1488], esi       ; -> obj1:0x1488
        mov    dword ptr [0x148c], edi       ; -> obj1:0x148C
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
        mov    esi, 0xa5f                    ; -> loc_4_0A5F
        mov    edx, 0x4ae8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_058B
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_058B
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        call   sub_2_0594
        mov    eax, dword ptr [0x152b]       ; -> obj1:0x152B
        clc    
        jmp    loc_2_058C
loc_2_058B:
        stc    
loc_2_058C:
        pop    ebx
        ret    
loc_2_058E:
        mov    eax, dword ptr [0x152b]       ; -> obj1:0x152B
        ret    
sub_2_0594:
        push   ebx
        mov    dword ptr [0x1570], ebx       ; -> obj1:0x1570
        mov    esi, 0x6cc                    ; -> loc_2_06CC
        mov    ebx, 8
        call   sub_2_069E
        jb     loc_2_069B
        mov    esi, 0x6d4                    ; -> loc_2_06D4
        mov    ebx, 0xb
        call   sub_2_069E
        jae    loc_2_05F8
        mov    esi, 0x6df                    ; -> loc_2_06DF
        mov    ebx, 0xa
        call   sub_2_069E
        jae    loc_2_05F8
        mov    esi, 0x6e9                    ; -> loc_2_06E9
        mov    ebx, 0xb
        call   sub_2_069E
        jae    loc_2_05F8
        mov    esi, 0x6f4                    ; -> loc_2_06F4
        mov    ebx, 0xe
        call   sub_2_069E
        jb     loc_2_069B
loc_2_05F8:
        call   sub_2_0706
        mov    word ptr [0x15ca], ax         ; -> loc_1_15CA
        mov    word ptr [0x15cc], bx         ; -> loc_1_15CC
        and    ax, 0x7fff
        mov    esi, 0x2cb                    ; -> obj1:0x02CB
loc_2_0613:
        test   word ptr [esi], bx
        je     loc_2_0633
        push   bx
        and    bx, 0xc000
        cmp    bx, word ptr [esi + 2]
        pop    bx
        jne    loc_2_0633
        test   word ptr [esi + 4], ax
        je     loc_2_0633
        cmp    cx, word ptr [esi + 6]
        je     loc_2_063E
loc_2_0633:
        add    esi, 0xc
        cmp    word ptr [esi], 0
        je     loc_2_069B
        jmp    loc_2_0613
loc_2_063E:
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [0x15c3], esi       ; -> loc_1_15C3
        mov    ebx, dword ptr [0x1570]       ; -> obj1:0x1570
        mov    ebp, dword ptr [ebx + 8]
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_069B
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_2_069B
        call   dword ptr [edi + 0xa8]
        mov    byte ptr [0x15be], al         ; -> obj1:0x15BE
        mov    eax, dword ptr [edi + 0x70]
        mov    dword ptr [edi + 0x70], 0x78e ; -> loc_3_078E
        mov    eax, dword ptr [edi + 0x74]
        mov    dword ptr [edi + 0x74], 0x5b8 ; -> loc_3_05B8
        mov    eax, dword ptr [edi + 0xbc]
        mov    dword ptr [edi + 0xbc], 0x13e0 ; -> loc_1_13E0
        mov    dword ptr [0x15bf], eax       ; -> loc_1_15BF
        clc    
        jmp    loc_2_069C
loc_2_069B:
        stc    
loc_2_069C:
        pop    ebx
        ret    
sub_2_069E:
        push   eax
        push   edi
        push   ecx
        mov    edi, 0xc0000
        mov    ecx, 0x100
loc_2_06AB:
        mov    al, byte ptr [esi]
        repne scasb al, byte ptr es:[edi]
        or     ecx, ecx
        je     loc_2_06C4
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
        je     loc_2_06C7
        loop   loc_2_06AB
loc_2_06C4:
        stc    
        jmp    loc_2_06C8
loc_2_06C7:
        clc    
loc_2_06C8:
        pop    ecx
        pop    edi
        pop    eax
        ret    
loc_2_06CC:
        inc    esp
        imul   esp, dword ptr [ecx + 0x6d], 0x646e6f
loc_2_06D4:
        push   ebx
        je     0x73c
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_2_0743
        and    byte ptr [esi], dh
        xor    al, 0
loc_2_06DF:
        push   ebx
        je     0x747
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_2_074E
        xor    al, 0
loc_2_06E9:
        push   ebx
        je     0x751
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_2_0758
        and    byte ptr [ebx + 0x45], dl
        db  000h
loc_2_06F4:
        push   ebx
        je     loc_2_075C
        popal  
        insb   byte ptr es:[edi], dx
        je     0x763
        and    byte ptr [esi + 0x69], dl
        outsd  dx, dword ptr gs:[esi]
        add    byte ptr [ecx], dh
        xor    dword ptr cs:[eax], eax
sub_2_0706:
        push   dx
        mov    dx, 0x3d4
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa039
        out    dx, ax
        mov    al, 0x36
        out    dx, al
        jmp    loc_2_071D
loc_2_071D:
        in     ax, dx
        jmp    loc_2_0721
loc_2_0721:
        and    ah, 0xc0
        mov    cx, 4
        or     ah, ah
        je     loc_2_0739
        mov    cx, 2
        cmp    ah, 0x80
        je     loc_2_0739
        mov    cx, 1
loc_2_0739:
        push   cx
        mov    al, 0x30
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
loc_2_0743:
        mov    ah, al
        and    ah, 0xf
        and    al, 0xf0
        cmp    al, 0xd0
        je     loc_2_07C2
loc_2_074E:
        cmp    al, 0xc0
        je     loc_2_077E
        cmp    al, 0xe0
        jne    loc_2_077E
        mov    al, 0x2e
loc_2_0758:
        out    dx, al
        inc    dx
        in     al, dx
loc_2_075C:
        dec    dx
        cmp    al, 0x80
        je     loc_2_07E0
        cmp    al, 0x90
        je     loc_2_07EA
        cmp    al, 0xb0
        je     loc_2_07F4
        cmp    al, 0xf0
        je     loc_2_07F4
        cmp    al, 0x10
        je     loc_2_07CC
        cmp    al, 0x11
        je     loc_2_07D6
loc_2_077E:
        mov    bx, 1
        push   esi
        push   ebx
        mov    esi, 0x702                    ; -> obj2:0x0702
        mov    ebx, 4
        call   sub_2_069E
        pop    ebx
        pop    esi
        jae    loc_2_079C
        or     bx, 0x4000
loc_2_079C:
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
        jne    loc_2_07B8
        or     cx, 0x800
loc_2_07B8:
        mov    dx, 0x3c8
        in     al, dx
        jmp    loc_2_084B
loc_2_07C2:
        mov    bx, 2
        mov    cx, 0x100
        jmp    loc_2_084B
loc_2_07CC:
        mov    bx, 4
        mov    cx, 0x400
        jmp    loc_2_084B
loc_2_07D6:
        mov    bx, 8
        mov    cx, 0x400
        jmp    loc_2_084B
loc_2_07E0:
        mov    bx, 0x10
        mov    cx, 0x400
        jmp    loc_2_084B
loc_2_07EA:
        mov    bx, 0x20
        mov    cx, 0x400
        jmp    loc_2_084B
loc_2_07F4:
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
        jne    loc_2_0828
        or     cx, 0x200
loc_2_0828:
        cmp    ax, 0xd00
        jne    loc_2_0833
        or     cx, 0x4200
loc_2_0833:
        cmp    ax, 0xf00
        jne    loc_2_083E
        or     cx, 0x1000
loc_2_083E:
        cmp    ax, 0
        jne    loc_2_0849
        or     cx, 0x1080
loc_2_0849:
        pop    ax
loc_2_084B:
        xchg   cx, ax
        pop    cx
        pop    dx
        ret    
        db  000h, 000h
loc_2_0854:
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x79616c
loc_2_085C:
        push   esi
        inc    esp
        inc    esp
        push   edx
        jae    0x8c8
        jb     0x8db
        add    byte ptr gs:[eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x8bd  R-X
loc_3_0000:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x149c], eax       ; -> obj1:0x149C
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x14a0], eax       ; -> obj1:0x14A0
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x14a4], eax       ; -> obj1:0x14A4
        mov    ecx, dword ptr [ebp + 0x18]
        mov    al, 0x5a
        mov    ah, cl
        mov    word ptr [0x1526], ax         ; -> loc_1_1526
        mov    al, 0x59
        mov    ah, ch
        mov    word ptr [0x1528], ax         ; -> loc_1_1528
        shr    ecx, 0x10
        mov    byte ptr [0x1547], cl         ; -> obj1:0x1547
        mov    byte ptr [0x152a], ch         ; -> loc_1_152A
        mov    eax, dword ptr [ebp + 4]
        shr    eax, 0x10
        or     ax, ax
        je     loc_3_009D
        cmp    ax, word ptr [0x14aa]         ; -> obj1:0x14AA
        je     loc_3_009D
        mov    word ptr [0x14aa], ax         ; -> obj1:0x14AA
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        test   dword ptr [0x14b0], 2         ; -> obj1:0x14B0
        je     loc_3_009D
        movzx  eax, word ptr [0x14aa]        ; -> obj1:0x14AA
        push   0
        push   eax
        push   dword ptr [0x1484]            ; -> obj1:0x1484
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        mov    dword ptr [0x14ac], eax       ; -> obj1:0x14AC
loc_3_009D:
        mov    eax, dword ptr [ebp]
        shr    eax, 0x10
        or     ax, ax
        je     loc_3_00BD
        cmp    ax, word ptr [0x14a8]         ; -> obj1:0x14A8
        je     loc_3_00BD
        mov    word ptr [0x14a8], ax         ; -> obj1:0x14A8
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
loc_3_00BD:
        mov    edx, 0x3cc
        in     al, dx
        mov    byte ptr [0x1548], al         ; -> loc_1_1548
        ret    
loc_3_00C9:
        mov    word ptr [0x1539], 0          ; -> obj1:0x1539
        mov    eax, 0x14dc                   ; -> loc_1_14DC
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_047F
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_047F
        movzx  esi, word ptr [eax + 0x18]
        or     esi, esi
        je     loc_3_047F
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    eax, 0xa539
        out    dx, ax
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_3_0209
        cmp    dword ptr [0x152b], 0xa0      ; -> obj1:0x152B
        je     loc_3_0262
        cmp    dword ptr [0x152b], 0xa0      ; -> obj1:0x152B
        je     loc_3_0262
        cmp    dword ptr [0x152b], 0x90      ; -> obj1:0x152B
        je     loc_3_0262
        cmp    dword ptr [0x152b], 0xb0      ; -> obj1:0x152B
        je     loc_3_0262
        cmp    dword ptr [0x152b], 0xc0      ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0xd0      ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x881100  ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x881140  ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x881000  ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x888000  ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x889000  ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x88b000  ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x88f000  ; -> obj1:0x152B
        je     loc_3_02C3
        cmp    dword ptr [0x152b], 0x881200  ; -> obj1:0x152B
        je     loc_3_02C3
loc_3_0209:
        mov    al, 0x41
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xf8
        cmp    esi, 0x280
        je     loc_3_0251
        cmp    esi, 0x320
        je     loc_3_023F
        cmp    esi, 0x400
        je     loc_3_022D
        mov    ah, 7
        jmp    loc_3_025A
loc_3_022D:
        mov    ah, 7
        cmp    edi, 0x46
        jae    loc_3_025A
        mov    ah, 6
        cmp    edi, 0x3c
        jae    loc_3_025A
        mov    ah, 5
        jmp    loc_3_025A
loc_3_023F:
        mov    ah, 3
        cmp    edi, 0x48
        jae    loc_3_025A
        mov    ah, 2
        cmp    edi, 0x3c
        jae    loc_3_025A
        mov    ah, 4
        jmp    loc_3_025A
loc_3_0251:
        mov    ah, 1
        cmp    edi, 0x48
        jae    loc_3_025A
        xor    ah, ah
loc_3_025A:
        or     al, ah
        out    dx, al
        jmp    loc_3_047C
loc_3_0262:
        mov    ch, 3
        cmp    esi, 0x280
        je     loc_3_0298
        mov    ch, 0xc
        cmp    esi, 0x320
        je     loc_3_0298
        mov    ch, 0x30
        cmp    esi, 0x400
        je     loc_3_0298
        mov    ch, 0x30
        cmp    esi, 0x480
        je     loc_3_0298
        mov    ch, 0xc0
        cmp    esi, 0x500
        jne    loc_3_047C
loc_3_0298:
        mov    cl, ch
        not    cl
        mov    ah, 0xff
        cmp    edi, 0x48
        jae    loc_3_02B3
        mov    ah, 0xaa
        cmp    edi, 0x3c
        jae    loc_3_02B3
        mov    ah, 0x55
        cmp    edi, 0x38
        jae    loc_3_02B3
        xor    ah, ah
loc_3_02B3:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    ax, cx
        or     al, ah
        out    dx, al
        jmp    loc_3_047C
loc_3_02C3:
        mov    ah, 0x41
        cmp    dword ptr [0x152b], 0x881100  ; -> obj1:0x152B
        je     loc_3_02F7
        cmp    dword ptr [0x152b], 0x881000  ; -> obj1:0x152B
        je     loc_3_02F7
        cmp    dword ptr [0x152b], 0x881140  ; -> obj1:0x152B
        je     loc_3_02F7
        cmp    dword ptr [0x152b], 0x881200  ; -> obj1:0x152B
        je     loc_3_02F7
        mov    ah, 0x5b
loc_3_02F7:
        cmp    esi, 0x280
        jne    loc_3_0351
        mov    cx, 0x8f30
        mov    word ptr [0x1539], 0x55       ; -> obj1:0x1539
        cmp    edi, 0x55
        jae    loc_3_0342
        mov    cx, 0x8f20
        mov    word ptr [0x1539], 0x4b       ; -> obj1:0x1539
        cmp    edi, 0x4b
        jae    loc_3_0342
        mov    cx, 0x8f10
        mov    word ptr [0x1539], 0x48       ; -> obj1:0x1539
        cmp    edi, 0x48
        jae    loc_3_0342
        mov    cx, 0x8f00
        mov    word ptr [0x1539], 0x3c       ; -> obj1:0x1539
loc_3_0342:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    loc_3_047C
loc_3_0351:
        cmp    esi, 0x320
        jne    loc_3_03C8
        mov    cx, 0x200
        mov    word ptr [0x1539], 0x55       ; -> obj1:0x1539
        cmp    edi, 0x55
        jae    loc_3_03AE
        mov    cx, 0x180
        mov    word ptr [0x1539], 0x4b       ; -> obj1:0x1539
        cmp    edi, 0x4b
        jae    loc_3_03AE
        mov    cx, 0x100
        mov    word ptr [0x1539], 0x48       ; -> obj1:0x1539
        cmp    edi, 0x48
        jae    loc_3_03AE
        mov    cx, 0x80
        mov    word ptr [0x1539], 0x3c       ; -> obj1:0x1539
        cmp    edi, 0x3c
        jae    loc_3_03AE
        mov    cx, 0
        mov    word ptr [0x1539], 0x38       ; -> obj1:0x1539
loc_3_03AE:
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
        or     al, ch
        out    dx, al
        jmp    loc_3_047C
loc_3_03C8:
        cmp    esi, 0x400
        je     loc_3_03D8
        cmp    esi, 0x480
        jne    loc_3_042F
loc_3_03D8:
        mov    cl, 0x14
        mov    word ptr [0x1539], 0x55       ; -> obj1:0x1539
        cmp    edi, 0x55
        jae    loc_3_0423
        mov    cl, 0x10
        mov    word ptr [0x1539], 0x4b       ; -> obj1:0x1539
        cmp    edi, 0x4b
        jae    loc_3_0423
        mov    cl, 0xc
        mov    word ptr [0x1539], 0x46       ; -> obj1:0x1539
        cmp    edi, 0x46
        jae    loc_3_0423
        mov    cl, 8
        mov    word ptr [0x1539], 0x3c       ; -> obj1:0x1539
        cmp    edi, 0x3c
        jae    loc_3_0423
        mov    cl, 0
        mov    word ptr [0x1539], 0x2b       ; -> obj1:0x1539
loc_3_0423:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xe3
        or     al, cl
        out    dx, al
        jmp    loc_3_047C
loc_3_042F:
        cmp    esi, 0x500
        jne    loc_3_047C
        mov    cl, 0xc0
        mov    word ptr [0x1539], 0x4b       ; -> obj1:0x1539
        cmp    edi, 0x4b
        jae    loc_3_0472
        mov    cl, 0xa0
        mov    word ptr [0x1539], 0x48       ; -> obj1:0x1539
        cmp    edi, 0x48
        jae    loc_3_0472
        mov    cl, 0x80
        mov    word ptr [0x1539], 0x3c       ; -> obj1:0x1539
        cmp    edi, 0x3c
        jae    loc_3_0472
        mov    cl, 0
        mov    word ptr [0x1539], 0x2d       ; -> obj1:0x1539
loc_3_0472:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x1f
        or     al, cl
        out    dx, al
loc_3_047C:
        pop    eax
        dec    edx
        out    dx, al
loc_3_047F:
        cmp    word ptr [0x1539], 0          ; -> obj1:0x1539
        je     loc_3_049D
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        mov    cx, word ptr [0x1539]         ; -> obj1:0x1539
        mov    word ptr [ebx + 0x18], cx
loc_3_049D:
        ret    
loc_3_049E:
        test   byte ptr [0x156c], 1          ; -> obj1:0x156C
        je     loc_3_04C3
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x66
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf7
        out    dx, al
        pop    eax
        mov    edx, 0x3d4
        out    dx, al
loc_3_04C3:
        ret    
loc_3_04C4:
        mov    byte ptr [0x153b], 0          ; -> obj1:0x153B
        call   sub_4_0996
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_3_055A
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    al, 0x67
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0xc
        cmp    al, 0xc
        jne    loc_3_0557
        or     byte ptr [0x1533], 2          ; -> obj1:0x1533
        mov    eax, dword ptr [0x14ac]       ; -> obj1:0x14AC
        mov    edx, dword ptr [eax + 0x81ec]
        mov    dword ptr [0x1534], edx       ; -> loc_1_1534
        and    edx, 0xfff78000
        or     edx, 0x3000
        cmp    byte ptr [0x1531], 1          ; -> obj1:0x1531
        jne    loc_3_0535
        or     edx, 0x80000
loc_3_0535:
        mov    dword ptr [eax + 0x81ec], edx
        mov    edx, 0x3da
loc_3_0540:
        in     al, dx
        test   al, 8
        jne    loc_3_0540
loc_3_0545:
        in     al, dx
        test   al, 8
        je     loc_3_0545
        mov    dx, 0x3d4
        mov    al, 0x67
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0xf3
        out    dx, al
loc_3_0557:
        dec    edx
        pop    eax
        out    dx, al
loc_3_055A:
        ret    
loc_3_055B:
        push   eax
        push   edi
        mov    edi, 0x14bc                   ; -> loc_1_14BC
        mov    al, 0x67
        cmp    byte ptr [0x449], 3
        jbe    loc_3_056F
        mov    al, 0x63
loc_3_056F:
        mov    byte ptr [edi + 0xd], al
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_3_058B
        mov    byte ptr [edi + 9], 0
loc_3_058B:
        mov    byte ptr [0x153d], 0xff       ; -> obj1:0x153D
        pop    edi
        pop    eax
        ret    
loc_3_0595:
        cld    
        push   edi
        mov    edi, 0x14cc                   ; -> loc_1_14CC
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_3_05B5
        mov    byte ptr [edi + 9], 0
loc_3_05B5:
        pop    edi
        ret    
        db  000h
loc_3_05B8:
        push   ebx
        mov    eax, 0x1574                   ; -> obj1:0x1574
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_078C
        mov    dh, byte ptr [eax + 0x1f]
        or     dh, dh
        je     loc_3_078C
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_078C
        movzx  edi, word ptr [eax + 0x18]
        or     edi, edi
        je     loc_3_078C
        test   word ptr [eax + 2], 0x280
        jne    loc_3_0638
        xor    dl, dl
        cmp    byte ptr [eax + 0x30], 0x2b
        je     loc_3_0615
        cmp    byte ptr [eax + 0x30], 0x2d
        jne    loc_3_0638
        mov    dl, 0x40
loc_3_0615:
        cmp    byte ptr [eax + 0x31], 0x2b
        je     loc_3_0624
        cmp    byte ptr [eax + 0x31], 0x2d
        jne    loc_3_0638
        or     dl, 0x80
loc_3_0624:
        push   ax
        mov    ah, dl
        mov    dx, 0x3cc
        in     al, dx
        and    al, 0x3f
        or     al, ah
        mov    dx, 0x3c2
        out    dx, al
        pop    ax
loc_3_0638:
        push   ax
        mov    dx, 0x3d4
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa039
        out    dx, ax
        pop    ax
        mov    dh, byte ptr [eax + 0x1f]
        or     dh, dh
        je     loc_3_0718
        cmp    dh, 0x10
        jne    loc_3_0718
        movzx  edi, word ptr [eax + 0x18]
        or     edi, edi
        je     loc_3_0718
        cmp    edi, 0x640
        jne    loc_3_0718
        mov    dx, word ptr [0x15ca]         ; -> loc_1_15CA
        and    dx, 0x4200
        je     loc_3_06C0
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
        jmp    loc_3_0718
loc_3_06C0:
        mov    dx, word ptr [0x15ca]         ; -> loc_1_15CA
        and    dx, 0x1080
        je     loc_3_0718
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
loc_3_0718:
        mov    dx, word ptr [0x15ca]         ; -> loc_1_15CA
        and    dx, 0x1080
        je     loc_3_077A
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
loc_3_077A:
        mov    dx, 0x3d4
        mov    ax, word ptr [0x15c7]         ; -> obj1:0x15C7
        out    dx, ax
        mov    al, byte ptr [0x15c9]         ; -> obj1:0x15C9
        out    dx, al
loc_3_078C:
        pop    ebx
        ret    
loc_3_078E:
        push   ebx
        mov    byte ptr [0x15ce], 0          ; -> loc_1_15CE
        mov    eax, 0x1574                   ; -> obj1:0x1574
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_089D
        mov    dh, byte ptr [eax + 0x1f]
        or     dh, dh
        je     loc_3_089D
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_089D
        movzx  edi, word ptr [eax + 0x18]
        or     edi, edi
        je     loc_3_089D
        mov    dl, 1
        mov    bx, di
        mov    edi, 0x2b1                    ; -> loc_1_02B1
loc_3_07E4:
        cmp    word ptr [edi], 0
        je     loc_3_089D
        cmp    word ptr [edi], bx
        je     loc_3_07FA
        add    edi, 4
        inc    dl
        jmp    loc_3_07E4
loc_3_07FA:
        movzx  edi, dl
        shl    edi, 0x10
        xor    dl, dl
        movzx  esi, word ptr [eax + 0x20]
        or     esi, esi
        je     loc_3_089D
        or     si, dx
        call   sub_1_0268
        mov    di, si
        xchg   si, dx
        mov    byte ptr [0x15ce], dl         ; -> loc_1_15CE
        mov    edx, 0x3d4
        in     al, dx
        mov    byte ptr [0x15c9], al         ; -> obj1:0x15C9
        push   eax
        mov    eax, 0xa539
        out    dx, ax
        mov    esi, dword ptr [0x15c3]       ; -> loc_1_15C3
        mov    al, byte ptr [esi + 1]
        mov    ah, al
        mov    al, byte ptr [esi]
        out    dx, al
        mov    bl, al
        inc    dl
        in     al, dx
        mov    bh, al
        mov    word ptr [0x15c7], bx         ; -> obj1:0x15C7
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
loc_3_086A:
        mov    al, byte ptr [esi]
        or     al, al
        je     loc_3_0897
        cmp    bl, al
        jb     loc_3_0890
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
        jne    loc_3_0897
loc_3_0890:
        add    esi, 7
        or     al, al
        jne    loc_3_086A
loc_3_0897:
        or     al, ah
        out    dx, al
        pop    eax
        dec    edx
        out    dx, al
loc_3_089D:
        cmp    byte ptr [0x15ce], 0          ; -> loc_1_15CE
        je     loc_3_08BB
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        movzx  cx, byte ptr [0x15ce]         ; -> loc_1_15CE
        mov    word ptr [ebx + 0x18], cx
loc_3_08BB:
        pop    ebx
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xf00  R-X
loc_4_0000:
        test   byte ptr [0x1533], 2          ; -> obj1:0x1533
        je     loc_4_003F
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    ax, 0xa039
        out    dx, ax
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        or     al, 0xc
        out    dx, al
        mov    eax, dword ptr [0x14ac]       ; -> obj1:0x14AC
        mov    edx, dword ptr [0x1534]       ; -> loc_1_1534
        mov    dword ptr [eax + 0x81ec], edx
        and    byte ptr [0x1533], 0xfd       ; -> obj1:0x1533
        pop    eax
        mov    edx, 0x3d4
        out    dx, al
loc_4_003F:
        mov    byte ptr [0x153b], 0xff       ; -> obj1:0x153B
        mov    byte ptr [0x153c], 0xff       ; -> loc_1_153C
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_4_0075
        mov    edx, dword ptr [0x1490]       ; -> obj1:0x1490
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_4_0126
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_4_0075:
        mov    edx, dword ptr [0x1490]       ; -> obj1:0x1490
        sub    edx, ecx
        cmp    edx, dword ptr [0x1480]       ; -> obj1:0x1480
        mov    edx, dword ptr [0x1480]       ; -> obj1:0x1480
        ja     loc_4_0090
        mov    edx, 0x8000
loc_4_0090:
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_00DE
        mov    byte ptr [0x153f], al         ; -> obj1:0x153F
        mov    byte ptr [0x153e], 0x3f       ; -> loc_1_153E
        cmp    dword ptr [0x152b], 0x881200  ; -> obj1:0x152B
        je     loc_4_0128
        cmp    dword ptr [0x152b], 0x881140  ; -> obj1:0x152B
        je     loc_4_0128
        cmp    dword ptr [0x152b], 0x881100  ; -> obj1:0x152B
        je     loc_4_0128
        cmp    dword ptr [0x152b], 0x881000  ; -> obj1:0x152B
        je     loc_4_0128
        mov    byte ptr [0x153e], 0xff       ; -> loc_1_153E
        jmp    loc_4_0128
loc_4_00DE:
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
        mov    byte ptr [0x153f], al         ; -> obj1:0x153F
        mov    byte ptr [0x1540], 0          ; -> loc_1_1540
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0122
        dec    edx
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        shl    bh, 2
        or     al, bh
        mov    byte ptr [0x1540], al         ; -> loc_1_1540
loc_4_0122:
        pop    ebx
        pop    edx
        jmp    loc_4_0128
loc_4_0126:
        mov    edx, ecx
loc_4_0128:
        xor    ah, ah
        ret    
loc_4_012B:
        cmp    byte ptr [0x1542], 0xff       ; -> loc_1_1542
        jne    loc_4_01B8
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_01B8
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
        mov    byte ptr [0x1541], al         ; -> obj1:0x1541
        or     al, 1
        out    dx, al
        dec    edx
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_0184
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x1542], al         ; -> loc_1_1542
        mov    al, byte ptr [0x153f]         ; -> obj1:0x153F
        out    dx, al
        dec    dl
        jmp    loc_4_01B2
loc_4_0184:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x1542], al         ; -> loc_1_1542
        mov    al, byte ptr [0x153f]         ; -> obj1:0x153F
        out    dx, al
        dec    edx
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_01B2
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x1543], al         ; -> obj1:0x1543
        mov    al, byte ptr [0x1540]         ; -> loc_1_1540
        out    dx, al
        dec    edx
loc_4_01B2:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01B8:
        ret    
loc_4_01B9:
        cmp    byte ptr [0x1542], 0xff       ; -> loc_1_1542
        je     loc_4_026D
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_0202
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        and    al, byte ptr [0x153e]         ; -> loc_1_153E
        cmp    al, byte ptr [0x153f]         ; -> obj1:0x153F
        jne    loc_4_025B
        mov    al, byte ptr [0x1542]         ; -> loc_1_1542
        out    dx, al
        jmp    loc_4_025B
loc_4_0202:
        xor    ah, ah
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0218
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
loc_4_0218:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    dl, byte ptr [0x153f]         ; -> obj1:0x153F
        mov    dh, byte ptr [0x1540]         ; -> loc_1_1540
        cmp    ax, dx
        jne    loc_4_025B
        mov    edx, 0x3d5
        mov    al, byte ptr [0x1542]         ; -> loc_1_1542
        out    dx, al
        dec    edx
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0251
        mov    al, 0x51
        out    dx, al
        inc    edx
        mov    al, byte ptr [0x1543]         ; -> obj1:0x1543
        out    dx, al
        dec    edx
loc_4_0251:
        mov    al, 0x31
        mov    ah, byte ptr [0x1541]         ; -> obj1:0x1541
        out    dx, ax
loc_4_025B:
        mov    byte ptr [0x1542], 0xff       ; -> loc_1_1542
        mov    edx, 0x3d4
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_026D:
        ret    
loc_4_026E:
        push   eax
        push   edx
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_02EC
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
        mov    byte ptr [0x1544], al         ; -> loc_1_1544
        or     al, 1
        out    dx, al
        dec    edx
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_02BA
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x1545], al         ; -> obj1:0x1545
        mov    al, byte ptr [0x153f]         ; -> obj1:0x153F
        out    dx, al
        dec    dl
        jmp    loc_4_02E8
loc_4_02BA:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x1545], al         ; -> obj1:0x1545
        mov    al, byte ptr [0x153f]         ; -> obj1:0x153F
        out    dx, al
        dec    edx
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_02E8
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x1546], al         ; -> loc_1_1546
        mov    al, byte ptr [0x1540]         ; -> loc_1_1540
        out    dx, al
        dec    edx
loc_4_02E8:
        rol    eax, 8
        out    dx, al
loc_4_02EC:
        pop    edx
        pop    eax
        ret    
loc_4_02EF:
        push   eax
        push   edx
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_0347
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    al, 0x31
        mov    ah, byte ptr [0x1544]         ; -> loc_1_1544
        out    dx, ax
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_0322
        mov    al, 0x6a
        mov    ah, byte ptr [0x1545]         ; -> obj1:0x1545
        out    dx, ax
        jmp    loc_4_0343
loc_4_0322:
        mov    al, 0x35
        mov    ah, byte ptr [0x1545]         ; -> obj1:0x1545
        out    dx, ax
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0343
        mov    al, 0x51
        out    dx, al
        inc    edx
        mov    al, byte ptr [0x1546]         ; -> loc_1_1546
        out    dx, al
        dec    edx
loc_4_0343:
        rol    eax, 8
        out    dx, al
loc_4_0347:
        pop    edx
        pop    eax
        ret    
loc_4_034A:
        or     byte ptr [0x1532], 2          ; -> loc_1_1532
        mov    eax, dword ptr [0x14a0]       ; -> obj1:0x14A0
        or     eax, eax
        je     loc_4_03CC
        cmp    word ptr [eax], 2
        je     loc_4_0365
        mov    word ptr [eax], 0
loc_4_0365:
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
        test   dword ptr [0x14b0], 2         ; -> obj1:0x14B0
        je     loc_4_03B5
        mov    edx, dword ptr [0x14ac]       ; -> obj1:0x14AC
        add    edx, dword ptr [0x14b4]       ; -> obj1:0x14B4
        mov    ecx, 0x3e8
loc_4_03A2:
        mov    ax, word ptr [edx]
        test   ax, 0x200
        je     loc_4_03CC
        call   sub_4_0BA7
        dec    ecx
        jne    loc_4_03A2
        jmp    loc_4_03CC
loc_4_03B5:
        mov    edx, 0x9ae9
        in     al, dx
        test   al, 2
        je     loc_4_03CC
        cmp    al, 0xff
        je     loc_4_03CC
        mov    edx, 0xe2e8
        out    dx, ax
        jmp    loc_4_03B5
loc_4_03CC:
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_4_04CD
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    al, 0x67
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0xc
        cmp    al, 0xc
        jne    loc_4_04CA
        or     byte ptr [0x1533], 2          ; -> obj1:0x1533
        cmp    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        je     loc_4_0434
        pushal 
        mov    eax, 0x400000
        mov    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        mov    ecx, 8
        mov    esi, 0x178                    ; -> loc_1_0178
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        popal  
        jmp    loc_4_047C
loc_4_0434:
        push   ebx
        mov    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4021
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        pop    ebx
loc_4_047C:
        mov    eax, dword ptr [0x14ac]       ; -> obj1:0x14AC
        mov    edx, dword ptr [eax + 0x81ec]
        mov    dword ptr [0x1534], edx       ; -> loc_1_1534
        and    edx, 0xfff78000
        or     edx, 0x3000
        cmp    byte ptr [0x1531], 1          ; -> obj1:0x1531
        jne    loc_4_04A8
        or     edx, 0x80000
loc_4_04A8:
        mov    dword ptr [eax + 0x81ec], edx
        mov    edx, 0x3da
loc_4_04B3:
        in     al, dx
        test   al, 8
        jne    loc_4_04B3
loc_4_04B8:
        in     al, dx
        test   al, 8
        je     loc_4_04B8
        mov    dx, 0x3d4
        mov    al, 0x67
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0xf3
        out    dx, al
loc_4_04CA:
        dec    edx
        pop    eax
        out    dx, al
loc_4_04CD:
        ret    
loc_4_04CE:
        mov    edx, 0x4ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    byte ptr [0x1532], 0          ; -> loc_1_1532
        cmp    ebx, dword ptr [0x148c]       ; -> obj1:0x148C
        je     loc_4_04F8
        call   sub_4_0549
loc_4_04F8:
        ret    
loc_4_04F9:
        or     byte ptr [0x1532], 1          ; -> loc_1_1532
        ret    
loc_4_0501:
        mov    byte ptr [0x1532], 0          ; -> loc_1_1532
        test   byte ptr [0x1533], 2          ; -> obj1:0x1533
        je     loc_4_0548
        push   ebx
        mov    edx, 0x3d4
        mov    ax, 0xa039
        out    dx, ax
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        or     al, 0xc
        add    ebx, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    byte ptr [ebx + 9], al
        mov    eax, dword ptr [0x14ac]       ; -> obj1:0x14AC
        mov    edx, dword ptr [0x1534]       ; -> loc_1_1534
        mov    dword ptr [eax + 0x81ec], edx
        and    byte ptr [0x1533], 0xfd       ; -> obj1:0x1533
        pop    ebx
loc_4_0548:
        ret    
sub_4_0549:
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_05C6
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_05C6
        push   edx
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0x1498]       ; -> obj1:0x1498
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
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_05B8
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
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_4_05B8
        dec    edx
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 9], al
loc_4_05B8:
        mov    dl, 0xd4
        rol    eax, 8
        out    dx, al
        mov    dl, 0xcc
        in     al, dx
        mov    byte ptr [edi + 0xd], al
        pop    edi
        pop    edx
loc_4_05C6:
        ret    
loc_4_05C7:
        push   edx
        cmp    byte ptr [0x153b], 0          ; -> obj1:0x153B
        je     loc_4_07F3
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_07F3
        cmp    ecx, dword ptr [0x148c]       ; -> obj1:0x148C
        jne    loc_4_05F9
        mov    ecx, 0x14bc                   ; -> loc_1_14BC
        sub    ecx, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    esi, ecx
        jmp    loc_4_060C
loc_4_05F9:
        cmp    esi, dword ptr [0x1488]       ; -> obj1:0x1488
        jne    loc_4_060C
        mov    esi, 0x14cc                   ; -> loc_1_14CC
        sub    esi, dword ptr [0x1498]       ; -> obj1:0x1498
loc_4_060C:
        cmp    ecx, dword ptr [0x1484]       ; -> obj1:0x1484
        jne    loc_4_0625
        mov    al, byte ptr [0x1548]         ; -> loc_1_1548
        or     al, al
        je     loc_4_0651
        mov    edx, 0x3c2
        out    dx, al
        jmp    loc_4_0651
loc_4_0625:
        add    ecx, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    edx, 0x3c2
        mov    al, byte ptr [ecx + 0xd]
        out    dx, al
        and    al, 0xc
        cmp    al, 0xc
        jne    loc_4_064B
        mov    dl, 0xd4
        in     al, dx
        ror    eax, 8
        mov    al, 0x42
        mov    ah, byte ptr [ecx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_064B:
        sub    ecx, dword ptr [0x1498]       ; -> obj1:0x1498
loc_4_0651:
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_07F3
        cmp    esi, dword ptr [0x1484]       ; -> obj1:0x1484
        jne    loc_4_0761
        test   byte ptr [0x1532], 3          ; -> loc_1_1532
        jne    loc_4_0761
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        push   ecx
        mov    ax, 0xa539
        out    dx, ax
        mov    ax, 0x4838
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        mov    edi, dword ptr [0x14a0]       ; -> obj1:0x14A0
        mov    ah, byte ptr [edi]
        and    ax, 0x3fe
        cmp    ah, 2
        jne    loc_4_06A8
        shr    ah, 1
loc_4_06A8:
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x53
        out    dx, al
        inc    edx
        in     al, dx
        mov    edi, dword ptr [0x14a4]       ; -> obj1:0x14A4
        mov    ah, byte ptr [edi]
        mov    cl, byte ptr [edi + 1]
        mov    ch, cl
        not    cl
        and    ax, cx
        or     al, ah
        out    dx, al
        dec    edx
        cmp    byte ptr [0x1547], 0          ; -> obj1:0x1547
        je     loc_4_06D9
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfe
        out    dx, al
        dec    edx
loc_4_06D9:
        mov    ax, word ptr [0x1526]         ; -> loc_1_1526
        out    dx, ax
        mov    ax, word ptr [0x1528]         ; -> loc_1_1528
        out    dx, ax
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [0x1547]         ; -> obj1:0x1547
        and    ax, 0x3fc
        or     al, ah
        mov    edi, dword ptr [0x14a0]       ; -> obj1:0x14A0
        mov    ah, byte ptr [edi]
        and    ah, 1
        not    ah
        and    ax, 0x1ef
        shl    ah, 4
        or     al, ah
        out    dx, al
        dec    edx
        pop    ecx
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_4_0753
        mov    edi, ecx
        add    edi, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [edi + 9]
        cmp    al, ah
        je     loc_4_0753
        cmp    al, 0xc
        je     loc_4_0753
        and    ax, 0xcf3
        or     al, ah
        mov    ecx, eax
        mov    edx, 0x3da
loc_4_0741:
        in     al, dx
        test   al, 8
        jne    loc_4_0741
loc_4_0746:
        in     al, dx
        test   al, 8
        je     loc_4_0746
        mov    edx, 0x3d5
        mov    eax, ecx
        out    dx, al
loc_4_0753:
        mov    edx, 0x3d4
        rol    eax, 8
        out    dx, al
        jmp    loc_4_07F3
loc_4_0761:
        add    esi, dword ptr [0x1498]       ; -> obj1:0x1498
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
        mov    edi, dword ptr [0x14a4]       ; -> obj1:0x14A4
        mov    cl, byte ptr [edi + 1]
        mov    ch, cl
        not    cl
        and    ax, cx
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
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_4_07EF
        cmp    byte ptr [0x1538], 0          ; -> loc_1_1538
        je     loc_4_07EF
        mov    byte ptr [0x1538], 0          ; -> loc_1_1538
        mov    edx, 0x3da
loc_4_07D0:
        in     al, dx
        test   al, 8
        jne    loc_4_07D0
loc_4_07D5:
        in     al, dx
        test   al, 8
        je     loc_4_07D5
        mov    edx, 0x3d4
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 9]
        and    ax, 0xcf3
        or     al, ah
        out    dx, al
        dec    edx
loc_4_07EF:
        rol    eax, 8
        out    dx, al
loc_4_07F3:
        pop    edx
        ret    
loc_4_07F5:
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0844
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_0844
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        cmp    dword ptr [0x152b], 0xd0      ; -> obj1:0x152B
        ja     loc_4_0831
        mov    ax, 0xa5a
        out    dx, ax
        mov    ax, 0x59
        out    dx, ax
loc_4_0831:
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xec
        out    dx, al
        dec    edx
        mov    ax, 0x39
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_0844:
        ret    
loc_4_0845:
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_08B6
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_08B6
        cmp    byte ptr [0x152a], 0          ; -> loc_1_152A
        jne    loc_4_08B6
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
        cmp    dword ptr [0x152b], 0xd0      ; -> obj1:0x152B
        ja     loc_4_089D
        mov    ax, word ptr [0x1526]         ; -> loc_1_1526
        out    dx, ax
        mov    ax, word ptr [0x1528]         ; -> loc_1_1528
        out    dx, ax
loc_4_089D:
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [0x1547]         ; -> obj1:0x1547
        and    ax, 0x3fc
        or     al, ah
        or     al, 0x10
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_08B6:
        ret    
loc_4_08B7:
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_097F
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
loc_4_097F:
        mov    edx, 0x4ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        ret    
sub_4_0996:
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_0A5E
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
loc_4_0A5E:
        ret    
loc_4_0A5F:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0x1484]       ; -> obj1:0x1484
        jne    loc_4_0A73
        cmp    ebx, edi
        jne    loc_4_0A7D
loc_4_0A73:
        cmp    ecx, 4
        je     loc_4_0A7C
        out    dx, ax
        jmp    loc_4_0A7D
loc_4_0A7C:
        out    dx, al
loc_4_0A7D:
        pop    esi
        pop    edi
        ret    
loc_4_0A80:
        push   edx
        cmp    eax, dword ptr [0x1484]       ; -> obj1:0x1484
        jne    loc_4_0BA5
        cmp    byte ptr [0x153c], 0          ; -> loc_1_153C
        je     loc_4_0BA5
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
        je     loc_4_0BA5
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0AE8
        test   ah, 8
        jne    loc_4_0BA5
        ror    eax, 8
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 0x10
        jne    loc_4_0BA5
loc_4_0AE8:
        mov    eax, dword ptr [0x149c]       ; -> obj1:0x149C
        cmp    byte ptr [eax], 0
        jne    loc_4_0BA5
        mov    ecx, 0x1000
        test   dword ptr [0x14b0], 2         ; -> obj1:0x14B0
        je     loc_4_0B91
        mov    eax, dword ptr [0x14a4]       ; -> obj1:0x14A4
        cmp    byte ptr [eax], 0
        je     loc_4_0BA5
        xor    eax, eax
        cmp    word ptr [0x1528], 0xa59      ; -> loc_1_1528
        jne    loc_4_0B30
        cmp    word ptr [0x1526], 0x5a       ; -> loc_1_1526
        je     loc_4_0B5E
loc_4_0B30:
        dec    eax
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    al, 0x59
        mov    ah, al
        out    dx, al
        inc    edx
        in     al, dx
        xchg   al, ah
        dec    edx
        push   eax
        mov    al, 0x5a
        mov    ah, al
        out    dx, al
        inc    edx
        in     al, dx
        xchg   al, ah
        push   eax
        dec    edx
        mov    ax, word ptr [0x1526]         ; -> loc_1_1526
        out    dx, ax
        mov    ax, word ptr [0x1528]         ; -> loc_1_1528
        out    dx, ax
loc_4_0B5E:
        mov    edx, dword ptr [0x14ac]       ; -> obj1:0x14AC
        add    edx, dword ptr [0x14b4]       ; -> obj1:0x14B4
loc_4_0B6A:
        mov    ax, word ptr [edx]
        test   ax, 0x200
        je     loc_4_0B7B
        call   sub_4_0BA7
        dec    ecx
        jne    loc_4_0B6A
loc_4_0B7B:
        test   eax, 0xffff0000
        je     loc_4_0BA5
        mov    edx, 0x3d4
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, al
        jmp    loc_4_0BA5
loc_4_0B91:
        mov    edx, 0x9ae8
        in     ax, dx
        test   ah, 2
        je     loc_4_0BA5
        call   sub_4_0BA7
        dec    ecx
        jne    loc_4_0B91
loc_4_0BA5:
        pop    edx
        ret    
sub_4_0BA7:
        push   ecx
        mov    ecx, 0x3e8
loc_4_0BAD:
        push   ecx
        pop    ecx
        loop   loc_4_0BAD
        pop    ecx
        ret    
loc_4_0BB3:
        cmp    cl, 0x30
        jae    loc_4_0BBB
        clc    
        jmp    loc_4_0C07
loc_4_0BBB:
        test   byte ptr [0x1532], 2          ; -> loc_1_1532
        je     loc_4_0BCF
        dec    dl
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        stc    
        jmp    loc_4_0C07
loc_4_0BCF:
        dec    dl
        in     al, dx
        mov    ch, al
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        cmp    cl, 0x40
        jne    loc_4_0BED
        mov    al, 0x40
        out    dx, al
        inc    dl
        in     al, dx
        jmp    loc_4_0BF8
loc_4_0BED:
        cmp    cl, 0x30
        jne    loc_4_0BFD
        mov    al, 0x30
        out    dx, al
        inc    dl
        in     al, dx
loc_4_0BF8:
        dec    dl
        stc    
        jmp    loc_4_0BFE
loc_4_0BFD:
        clc    
loc_4_0BFE:
        xchg   ch, al
        out    dx, al
        mov    dl, 0xd5
        mov    al, ch
        mov    ch, 0
loc_4_0C07:
        ret    
loc_4_0C08:
        cmp    cl, 0x30
        cmc    
        jae    loc_4_0CAF
        cmp    byte ptr [0x153d], 0xff       ; -> obj1:0x153D
        jb     loc_4_0CAF
        test   byte ptr [0x1532], 2          ; -> loc_1_1532
        je     loc_4_0C33
        dec    dl
        mov    ah, al
        mov    al, cl
        out    dx, ax
        stc    
        jmp    loc_4_0CAF
loc_4_0C33:
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
        jne    loc_4_0C5F
        mov    edi, ebx
        add    edi, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    byte ptr [edi + 2], al
        jmp    loc_4_0CAE
loc_4_0C5F:
        cmp    cl, 0x42
        jne    loc_4_0C71
        mov    edi, ebx
        add    edi, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    byte ptr [edi + 3], al
        jmp    loc_4_0CAE
loc_4_0C71:
        cmp    cl, 0x53
        jne    loc_4_0C83
        mov    edi, ebx
        add    edi, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    byte ptr [edi + 5], al
        jmp    loc_4_0CAE
loc_4_0C83:
        cmp    cl, 0x58
        jne    loc_4_0C95
        mov    edi, ebx
        add    edi, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    byte ptr [edi + 6], al
        jmp    loc_4_0CAE
loc_4_0C95:
        cmp    cl, 0x67
        jne    loc_4_0CAE
        test   byte ptr [0x1533], 1          ; -> obj1:0x1533
        je     loc_4_0CAE
        mov    edi, ebx
        add    edi, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    byte ptr [edi + 9], al
loc_4_0CAE:
        clc    
loc_4_0CAF:
        ret    
loc_4_0CB0:
        pushfd 
        test   cl, 4
        je     loc_4_0CC1
        push   ebx
        add    ebx, dword ptr [0x1498]       ; -> obj1:0x1498
        mov    byte ptr [ebx + 0xd], al
        pop    ebx
loc_4_0CC1:
        popfd  
        jmp    loc_1_14B8
loc_4_0CC8:
        mov    ecx, dword ptr [0x1490]       ; -> obj1:0x1490
        stc    
        ret    
loc_4_0CD0:
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_0CEE
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        or     al, al
        jne    loc_4_0D15
loc_4_0CEE:
        xor    ah, ah
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0D06
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
loc_4_0D06:
        mov    al, 0x35
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0xc0f
        shl    ah, 2
        or     al, ah
loc_4_0D15:
        mov    dl, 0xd4
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_4_0D20:
        mov    edx, 0x10000
        xor    eax, eax
        stc    
        ret    
loc_4_0D29:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x1498]       ; -> obj1:0x1498
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
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0DE2
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 4], al
        out    dx, al
        dec    dl
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_0D8D
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 0xa], al
        dec    dl
loc_4_0D8D:
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
        mov    edi, dword ptr [0x14a4]       ; -> obj1:0x14A4
        mov    cl, byte ptr [edi + 1]
        not    cl
        and    al, cl
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
loc_4_0DE2:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_4_0DEB:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x1498]       ; -> obj1:0x1498
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
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0E5E
        mov    al, 0x51
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_0E3B
        mov    al, 0x6a
        mov    ah, byte ptr [esi + 0xa]
        out    dx, ax
loc_4_0E3B:
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
loc_4_0E5E:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_4_0E67:
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
        cmp    dword ptr [0x152b], 0x80      ; -> obj1:0x152B
        je     loc_4_0EC1
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
        cmp    byte ptr [0x1530], 0          ; -> loc_1_1530
        je     loc_4_0EC1
        mov    al, 0x6a
        mov    ah, cl
        test   cl, 0xc0
        jne    loc_4_0EBF
        xor    ah, ah
loc_4_0EBF:
        out    dx, ax
loc_4_0EC1:
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_0ECA:
        push   eax
        mov    eax, 0x86
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    eax
        clc    
        ret    
loc_4_0EDB:
        cmp    ebx, dword ptr [0x1484]       ; -> obj1:0x1484
        je     loc_4_0EFE
        cmp    ax, 0x4f02
        jne    loc_4_0EFE
        test   byte ptr [ebp + 0x11], 0x80
        jne    loc_4_0EFE
        push   eax
        mov    eax, 0x13
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    eax
loc_4_0EFE:
        clc    
        ret    
VxD__LTEXT ENDS
