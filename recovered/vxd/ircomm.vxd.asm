; IRCOMM -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 7912 instructions recovered by recursive descent

Declare_Virtual_Device IRCOMM, 1, 0, IRCOMM_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 02FFFFFFBh   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x6154  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'IRCOMM  '                  ; DDB_Name
        dd  02FFFFFFBh                  ; DDB_Init_Order
        dd  IRCOMM_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  sub_1_54CE               ; fixup
        dd  sub_2_0349               ; fixup
        dd  sub_1_5633               ; fixup
        dd  loc_1_57B3               ; fixup
        dd  loc_2_036E               ; fixup
        dd  loc_1_54B2               ; fixup
        dd  loc_1_5818               ; fixup
        dd  loc_2_03F3               ; fixup
        dd  loc_2_0412               ; fixup
        dd  loc_1_583D               ; fixup
        dd  loc_1_544C               ; fixup
        dd  loc_2_0452               ; fixup
        dd  loc_1_58D6               ; fixup
        dd  loc_1_58FF               ; fixup
        dd  loc_1_59B1               ; fixup
        dd  loc_2_0474               ; fixup
        dd  loc_2_04C5               ; fixup
        dd  loc_2_0528               ; fixup
        dd  loc_2_057C               ; fixup
        dd  loc_2_0594               ; fixup
        dd  loc_2_05F1               ; fixup
        dd  loc_2_0639               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00B8:
        add    byte ptr [eax + eax], 0
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
loc_1_00E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00F8:
        add    byte ptr [ecx], al
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    byte ptr [eax], al
loc_1_0100:
        add    byte ptr [ecx], al
        push   es
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [esi], eax
        add    dword ptr [ecx], eax
        add    dword ptr [edx + 0x53415203], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0118:
        and    byte ptr [eax], al
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
loc_1_0138:
        sbb    al, 0
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
loc_1_0154:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0158:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_015C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0160:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0164:
        dec    ecx
        jb     loc_1_01AB
        inc    ecx
        cmp    cl, byte ptr [ecx + 0x72]
        inc    ebx
        dec    edi
        dec    ebp
        dec    ebp
        add    byte ptr [eax + 0x61], dl
        jb     loc_1_01D5
        insd   dword ptr es:[edi], dx
        je     loc_1_01DD
        jb     0x1ed
        add    byte ptr [eax], al
loc_1_017C:
        dec    ecx
        jb     0x1cb
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x72], cl
        inc    esp
        inc    ecx
        cmp    cl, byte ptr [ecx + 0x72]
        dec    esp
        dec    ebp
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 0x61]
        jo     loc_1_01E7
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_0198:
        dec    ecx
        jb     loc_1_01DF
        inc    ecx
        cmp    dl, byte ptr [ecx + ebp*2 + 0x6e]
        jns    0x1f6
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 0x61]
        jo     0x1fc
        insb   byte ptr es:[edi], dx
loc_1_01AB:
        add    byte ptr [ecx + 0x72], cl
        inc    esp
        inc    ecx
        cmp    cl, byte ptr [ecx + 0x72]
        dec    esp
        dec    ebp
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 0x61]
        jo     0x20f
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_01C0:
        dec    ecx
        jb     0x207
        inc    ecx
        cmp    cl, byte ptr [ecx + 0x72]
        dec    esp
        dec    ebp
        push   eax
        cmp    cl, byte ptr [ebx + edx*2 + 0x41]
        push   eax
        push   ebx
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_01D4:
        dec    ecx
loc_1_01D5:
        jb     loc_1_021A
        dec    edi
        dec    ebp
        dec    ebp
        add    byte ptr [eax], al
loc_1_01DC:
        dec    ecx
loc_1_01DD:
        jb     0x223
loc_1_01DF:
        inc    ecx
        cmp    cl, byte ptr [ecx + 0x72]
        inc    ebx
        dec    edi
        dec    ebp
        dec    ebp
loc_1_01E7:
        add    byte ptr [ecx + 0x72], cl
        inc    esp
        inc    ecx
        cmp    dl, byte ptr [ecx + ebp*2 + 0x6e]
        jns    loc_1_0246
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 0x61]
        jo     loc_1_024C
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax + 0x61], dl
        jb     loc_1_0261
        insd   dword ptr es:[edi], dx
        je     loc_1_0269
        jb     loc_1_0279
        add    byte ptr [eax], al
loc_1_0208:
        dec    esp
        push   eax
        push   esp
        db  000h
loc_1_020C:
        push   edx
        inc    ecx
        push   ebx
        db  000h
sub_1_0210:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_021A:
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
loc_1_0246:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_024C:
        pop    ss
        add    al, 0x80
        add    eax, eax
        add    byte ptr [eax], ah
        xor    byte ptr [eax], al
        sbb    byte ptr [eax], al
        or     al, 0
        or     byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0261:
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0269:
        add    byte ptr [edx], al
        add    ah, cl
        int3   
        int3   
        int3   
IRCOMM_Control:
        cmp    eax, 0x1b
        je     loc_4_0000
loc_1_0279:
        cmp    eax, 0x1c
        je     loc_1_0284
        clc    
        ret    
loc_1_0284:
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [0]            ; -> obj1:0x00B4
        VMMCall  VMM.List_Destroy                ; cd209c000100
        call   sub_1_1B78
        pop    ebx
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh
sub_1_02A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_02B7
loc_1_02B4:
        dec    dword ptr [ebp - 4]
loc_1_02B7:
        cmp    dword ptr [ebp - 4], 0
        jbe    loc_1_02D6
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 0xc]
        inc    dword ptr [ebp + 8]
        jmp    loc_1_02B4
loc_1_02D6:
        mov    eax, dword ptr [ebp + 0x10]
        jmp    loc_1_02DE
loc_1_02DE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_02E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        push   eax
        call   sub_1_5F4C
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0330
        call   sub_1_5F60
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        push   eax
        call   sub_1_5F4C
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0330
        xor    eax, eax
        jmp    loc_1_0346
loc_1_0330:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        jmp    loc_1_0346
loc_1_0346:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_034B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_5F52
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_036E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_0385:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_03B6
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x110], 2
        je     loc_1_03B6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0385
loc_1_03B6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_03C5
        jmp    loc_1_0427
loc_1_03C5:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_042C
        add    esp, 8
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_03F8
        push   0x800
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        jmp    loc_1_0427
loc_1_03F8:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x418], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x110], 3
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1a5], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_0427:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_042C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_0529
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_0466
loc_1_0462:
        inc    word ptr [ebp - 4]
loc_1_0466:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_051A
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x303]
        cmp    dword ptr [eax + ecx + 8], edx
        je     loc_1_04AD
        jmp    loc_1_0462
loc_1_04AD:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 0x1a3]
        and    edx, 0xffff
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 0x1b7]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1a3]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_0515
        jmp    loc_1_051A
loc_1_0515:
        jmp    loc_1_0462
loc_1_051A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        jmp    loc_1_0650
loc_1_0529:
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_0538
loc_1_0534:
        inc    word ptr [ebp - 4]
loc_1_0538:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_05E6
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        cmp    eax, 8
        jl     loc_1_0571
        jmp    loc_1_05E6
loc_1_0571:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx + 6]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_05E1
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 0x1a3]
        and    edx, 0xffff
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 0x1b7]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1a3]
loc_1_05E1:
        jmp    loc_1_0534
loc_1_05E6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 2
        je     loc_1_0621
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0655
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_074C
        add    esp, 8
        jmp    loc_1_0641
loc_1_0621:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_074C
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0655
        add    esp, 8
loc_1_0641:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        jmp    loc_1_0650
loc_1_0650:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0655:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_066D
loc_1_066A:
        inc    dword ptr [ebp - 8]
loc_1_066D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_0747
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        cmp    eax, 8
        jl     loc_1_069E
        jmp    loc_1_0747
loc_1_069E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx + 6]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_06C3
        jmp    loc_1_066A
loc_1_06C3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 0xc]
        add    eax, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x20]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0742
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 8
        je     loc_1_0742
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 0x1a3]
        and    edx, 0xffff
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 0x1b7]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1a3]
loc_1_0742:
        jmp    loc_1_066A
loc_1_0747:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_074C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_0764
loc_1_0761:
        inc    dword ptr [ebp - 8]
loc_1_0764:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_0863
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        cmp    eax, 8
        jl     loc_1_0795
        jmp    loc_1_0863
loc_1_0795:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx + 6]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_07BA
        jmp    loc_1_0761
loc_1_07BA:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 0xc]
        add    eax, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x20]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_085E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 8
        jne    loc_1_085E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_085E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        test   al, 4
        je     loc_1_085E
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 0x1a3]
        and    edx, 0xffff
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 0x1b7]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1a3]
loc_1_085E:
        jmp    loc_1_0761
loc_1_0863:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0868:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_0871:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1a5]
        and    ecx, 0xffff
        cmp    eax, ecx
        jg     loc_1_08A9
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1439
        add    esp, 4
        jmp    loc_1_099E
loc_1_08A9:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0x9f
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a5]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x1b7
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x348], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x354], 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x34c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x350], 0xbb8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x355], 0    ; -> loc_1_00E8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x361
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x35d], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x35b], 0x45
        mov    dword ptr [1], 0              ; -> obj1:0x00E9
        mov    byte ptr [0], 0xb             ; -> loc_1_00E8
        mov    dword ptr [6], 0              ; -> loc_1_00EE
        mov    byte ptr [5], 0xa             ; -> obj1:0x00ED
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x344], 0    ; -> loc_1_09A3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x328
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_099E
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_099E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        jmp    loc_1_0871
loc_1_099E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_09A3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, 0x328
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0xf
        jne    loc_1_09DE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    loc_1_0EE1
loc_1_09DE:
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_09FB
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x31]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0B20
loc_1_09FB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 3
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x20]
        mov    al, byte ptr [eax + 0x28]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0A7D
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x20]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   al, 8
        je     loc_1_0A7D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x123]
        and    eax, 0xff
        test   al, 1
        je     loc_1_0A7D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x80
        jne    loc_1_0A7D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     loc_1_0A9A
loc_1_0A7D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    loc_1_0EE1
loc_1_0A9A:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x1a5]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 0x1af], 1
        mov    dword ptr [1], 0              ; -> obj1:0x00E9
        mov    byte ptr [0], 5               ; -> loc_1_00E8
        mov    dword ptr [6], 0              ; -> loc_1_00EE
        mov    byte ptr [5], 0x12            ; -> obj1:0x00ED
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_1_104C
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_0B1B
        cmp    dword ptr [ebp + 8], 2
        je     loc_1_0B1B
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_0B1B:
        jmp    loc_1_0EE1
loc_1_0B20:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x20
        jne    loc_1_0DEE
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x32]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x367]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x36a
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x28], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x368]
        mov    word ptr [ebp - 0x14], ax
        mov    word ptr [ebp - 0x24], 0
        mov    byte ptr [ebp - 0x20], 3
        mov    byte ptr [ebp - 0x10], 0
        mov    byte ptr [ebp - 0x1c], 0
        mov    byte ptr [ebp - 0x18], 0
loc_1_0B96:
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x14]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_1_0D00
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x2c], eax
        jmp    loc_1_0CD5
loc_1_0BCA:
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        add    eax, 2
        mov    word ptr [ebp - 0x24], ax
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 0x20], al
        inc    word ptr [ebp - 0x24]
        jmp    loc_1_0CFB
loc_1_0BF3:
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        add    eax, 2
        mov    word ptr [ebp - 0x24], ax
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 0x10], al
        inc    word ptr [ebp - 0x24]
        jmp    loc_1_0CFB
loc_1_0C1C:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x80
        jne    loc_1_0C4B
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0x80
        or     al, byte ptr [ebp - 0x18]
        mov    byte ptr [ebp - 0x18], al
loc_1_0C4B:
        inc    word ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 0x1c], al
        inc    word ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        add    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x28], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x24]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 0x24], ax
        jmp    loc_1_0CFB
loc_1_0C8B:
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0x80
        or     al, byte ptr [ebp - 0x18]
        mov    byte ptr [ebp - 0x18], al
        inc    word ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x24]
        and    ecx, 0xffff
        add    eax, ecx
        inc    eax
        mov    word ptr [ebp - 0x24], ax
        jmp    loc_1_0CFB
        db  0E9h, 026h, 000h, 000h, 000h
loc_1_0CD5:
        cmp    dword ptr [ebp - 0x2c], 0
        je     loc_1_0BF3
        cmp    dword ptr [ebp - 0x2c], 1
        je     loc_1_0BCA
        cmp    dword ptr [ebp - 0x2c], 0x82
        je     loc_1_0C1C
        jmp    loc_1_0C8B
loc_1_0CFB:
        jmp    loc_1_0B96
loc_1_0D00:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x123]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x10]
        and    eax, ecx
        mov    byte ptr [ebp - 0x10], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x20]
        and    eax, ecx
        mov    byte ptr [ebp - 0x20], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   eax, eax
        jne    loc_1_0DEE
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        test   eax, eax
        je     loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x20]
        cmp    eax, ecx
        jne    loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_0D9F
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf4]
        push   eax
        call   sub_1_4AC0
        add    esp, 0xc
        test   eax, eax
        je     loc_1_0DEE
loc_1_0D9F:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 0x20
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x365]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x11c], ax
        mov    al, byte ptr [ebp - 0x10]
        and    eax, 0xfe
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x1a5]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 0x1af], al
loc_1_0DEE:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x361]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x363]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_0EE1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x20
        je     loc_1_0EC4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x1a5]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 0x1af]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0E67
        mov    dword ptr [6], 0              ; -> loc_1_00EE
        mov    byte ptr [5], 0x13            ; -> obj1:0x00ED
        jmp    loc_1_0E92
loc_1_0E67:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x1a5]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 0x1af], 1
        mov    dword ptr [6], 0              ; -> loc_1_00EE
        mov    byte ptr [5], 0x12            ; -> obj1:0x00ED
loc_1_0E92:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_1_0EE6
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_0EBF
        cmp    dword ptr [ebp + 8], 2
        jne    loc_1_0EC4
loc_1_0EBF:
        jmp    loc_1_0EE1
loc_1_0EC4:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    loc_1_0EE1
loc_1_0EE1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0EE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, 0x328
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_0F17
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x31]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0F34
loc_1_0F17:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    loc_1_1047
loc_1_0F34:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_0FD7
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x32]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0FD7
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x367]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_0FD7
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x11c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x365]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_0FD7
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 0x40
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x368]
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x1a5]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 0x1a7], al
loc_1_0FD7:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x361]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x363]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_1047
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_102F
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 0x1a5]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1a3], 1
        jmp    loc_1_103B
loc_1_102F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
loc_1_103B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_1047:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_104C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, 0x328
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0xf
        jne    loc_1_1087
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    loc_1_1221
loc_1_1087:
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_10A4
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x31]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_10F8
loc_1_10A4:
        mov    dword ptr [6], 0              ; -> loc_1_00EE
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_1_1226
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_10F3
        cmp    dword ptr [ebp + 8], 2
        je     loc_1_10F3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_10F3:
        jmp    loc_1_1221
loc_1_10F8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_11B1
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x32]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_11B1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x367]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_11B1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x20
        je     loc_1_1175
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x11c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x365]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_11B1
loc_1_1175:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 0x40
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x368]
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x1a5]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 0x1a7], al
loc_1_11B1:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x361]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x363]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_1221
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_1209
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 0x1a5]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1a3], 1
        jmp    loc_1_1215
loc_1_1209:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
loc_1_1215:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_1221:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1226:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, 0x328
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_1257
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x31]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_1274
loc_1_1257:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    loc_1_139D
loc_1_1274:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_132D
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x32]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_132D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x367]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_132D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x20
        je     loc_1_12F1
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x11c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x365]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_132D
loc_1_12F1:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 0x40
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x368]
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x1a5]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 0x1a7], al
loc_1_132D:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x361]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x363]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_139D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_1385
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 0x1a5]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1a3], 1
        jmp    loc_1_1391
loc_1_1385:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
loc_1_1391:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_139D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_13A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x1a3]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a5]
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_13CC
loc_1_13C8:
        inc    word ptr [ebp - 4]
loc_1_13CC:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_142F
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        inc    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0xffff
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 0x1b7]
        lea    esi, [eax + ecx + 0x1b7]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_13C8
loc_1_142F:
        jmp    loc_1_1434
loc_1_1434:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1439:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_146F
        push   0x800
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        jmp    loc_1_1568
loc_1_146F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a3]
        and    eax, 0xffff
        cmp    eax, 1
        jle    loc_1_1498
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_156D
        add    esp, 4
        jmp    loc_1_14A4
loc_1_1498:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a5], 0
loc_1_14A4:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a5]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x1b7
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x410], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0x80
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x121], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a5]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x1a7]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x419], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a5]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x1af]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x450], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1584
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_1568
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1568
        push   0x400
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
loc_1_1568:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_156D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a5], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1584:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_0154
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x116], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x124], 0xffff
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x126], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x128], 0xffff
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x12a], 0xffff
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_1_1661
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x116]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x152]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x152]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x156], 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x116], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x11a], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x156]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14f
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_42D9
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x156], ax
        jmp    loc_1_1691
loc_1_1661:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x156], 0
        mov    eax, dword ptr [0]            ; -> loc_1_0154
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x116], ax
        mov    eax, dword ptr [0]            ; -> loc_1_0154
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11a], ax
loc_1_1691:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x414], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14f
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x124
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3df
        push   eax
        call   sub_1_5F0D
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_16D5
loc_1_16D5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_16DA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x410], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x418], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a5], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        or     dword ptr [ecx + 0x34], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        jmp    loc_1_1736
loc_1_1736:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_173B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_1762
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_1_1767
loc_1_1762:
        jmp    loc_1_1817
loc_1_1767:
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_1775:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_17BC
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x110], 0
        jne    loc_1_17A5
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 1
        je     loc_1_17BC
loc_1_17A5:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1775
loc_1_17BC:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_17CB
        jmp    loc_1_1817
loc_1_17CB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_181C
        add    esp, 8
        test   eax, eax
        jne    loc_1_17E8
        jmp    loc_1_1817
loc_1_17E8:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x418], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x110], 3
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1a5], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_1817:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_181C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_1871
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x303]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_186C
        xor    eax, eax
        jmp    loc_1_18F3
loc_1_186C:
        jmp    loc_1_18C9
loc_1_1871:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x20]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_188B
        xor    eax, eax
        jmp    loc_1_18F3
loc_1_188B:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 8
        jne    loc_1_18C9
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_18C2
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        test   al, 4
        jne    loc_1_18C9
loc_1_18C2:
        xor    eax, eax
        jmp    loc_1_18F3
loc_1_18C9:
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x1b7
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a3], 1
        mov    eax, 1
        jmp    loc_1_18F3
loc_1_18F3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_18F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_1A9C
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_1919:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1A97
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x410], 0
        je     loc_1_1A80
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x410]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_1A80
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1A25
loc_1_1956:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x121], al
        jmp    loc_1_1A80
loc_1_1974:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        or     al, 2
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x121], al
        jmp    loc_1_1A80
loc_1_1992:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        or     al, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x121], al
        jmp    loc_1_1A80
loc_1_19B0:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xfe
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x121], al
        jmp    loc_1_1A80
loc_1_19CC:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xf9
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x121], al
        jmp    loc_1_1A80
loc_1_19E8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xf8
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x121], al
        jmp    loc_1_1A80
loc_1_1A04:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xf8
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x121], al
loc_1_1A1B:
        jmp    loc_1_1A80
        db  0E9h, 05Bh, 000h, 000h, 000h
loc_1_1A25:
        sub    dword ptr [ebp - 8], 0x19
        cmp    dword ptr [ebp - 8], 0x17
        ja     loc_1_1A80
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]            ; -> obj1:0x1A68
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x1A48
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_19B0
        dd  loc_1_19E8
        dd  loc_1_1956
        dd  loc_1_1A04
        dd  loc_1_1974
        dd  loc_1_1992
        dd  loc_1_19CC
        dd  loc_1_1A1B
        db  000h, 001h, 002h, 007h, 003h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 004h, 005h, 006h
loc_1_1A80:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1919
loc_1_1A97:
        jmp    loc_1_1B05
loc_1_1A9C:
        cmp    dword ptr [ebp + 0xc], 0x18
        je     loc_1_1AAB
        jmp    loc_1_1B05
loc_1_1AAB:
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_1AB9:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1AEA
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x110], 1
        je     loc_1_1AEA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1AB9
loc_1_1AEA:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1AF9
        jmp    loc_1_1B05
loc_1_1AF9:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1B0A
        add    esp, 4
loc_1_1B05:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1B0A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 2
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3df
        push   eax
        call   sub_1_5F07
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0x17
        jne    loc_1_1B52
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 1
        jmp    loc_1_1B73
loc_1_1B52:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1B73
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_1B73
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 0
loc_1_1B73:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1B78:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_1B81:
        mov    eax, dword ptr [0]            ; -> loc_1_0160
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1BAE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [0], eax            ; -> loc_1_0160
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    loc_1_1B81
loc_1_1BAE:
        mov    eax, dword ptr [0]            ; -> loc_1_015C
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1BDB
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [0], eax            ; -> loc_1_015C
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    loc_1_1BAE
loc_1_1BDB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        call   sub_4_005A
loc_1_1BF9:
        add    esp, 4
        cmp    dword ptr [ebp - 4], 2
        jne    loc_1_1C9C
        mov    dword ptr [0], 1              ; -> loc_1_0158
        mov    dword ptr [0], 0xa            ; -> loc_1_0154
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_1C29
loc_1_1C26:
        inc    dword ptr [ebp - 8]
loc_1_1C29:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [0], eax            ; -> loc_1_0154
        jbe    loc_1_1C9C
        push   0x5c
        call   sub_1_02E3
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1C54
        jmp    loc_1_1CA6
loc_1_1C54:
        mov    eax, dword ptr [0]            ; -> loc_1_015C
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [0], eax            ; -> loc_1_015C
        push   0x800
        call   sub_1_02E3
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1C85
        jmp    loc_1_1CA6
loc_1_1C85:
        mov    eax, dword ptr [0]            ; -> loc_1_0160
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_0160
        jmp    loc_1_1C26
loc_1_1C9C:
        mov    eax, 1
        jmp    loc_1_1CB2
loc_1_1CA6:
        call   sub_1_1B78
        xor    eax, eax
        jmp    loc_1_1CB2
loc_1_1CB2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1CB7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_0158
        jne    loc_1_1CE1
        push   0x5c
        call   sub_1_02E3
        add    esp, 4
        jmp    loc_1_1D05
        db  0E9h, 024h, 000h, 000h, 000h
loc_1_1CE1:
        mov    eax, dword ptr [0]            ; -> loc_1_015C
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1CFD
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [0], eax            ; -> loc_1_015C
loc_1_1CFD:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1D05
loc_1_1D05:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1D0A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_0158
        jne    loc_1_1D2E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    loc_1_1D4A
loc_1_1D2E:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_1D4A
        mov    eax, dword ptr [0]            ; -> loc_1_015C
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_015C
loc_1_1D4A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1D4F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_0158
        jne    loc_1_1D8D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    eax, ecx
        ja     loc_1_1D7A
        mov    eax, ecx
loc_1_1D7A:
        push   eax
        call   sub_1_02E3
        add    esp, 4
        jmp    loc_1_1DB1
        db  0E9h, 024h, 000h, 000h, 000h
loc_1_1D8D:
        mov    eax, dword ptr [0]            ; -> loc_1_0160
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1DA9
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [0], eax            ; -> loc_1_0160
loc_1_1DA9:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1DB1
loc_1_1DB1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1DB6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_0158
        jne    loc_1_1DDA
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    loc_1_1DEC
loc_1_1DDA:
        mov    eax, dword ptr [0]            ; -> loc_1_0160
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_0160
loc_1_1DEC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1DF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_1E09
loc_1_1E06:
        inc    dword ptr [ebp - 8]
loc_1_1E09:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [0], eax            ; -> loc_1_0154
        jbe    loc_1_1EA5
        call   sub_1_1CB7
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1E9B
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_1D4F
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xb], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xb], 0
        je     loc_1_1E85
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], 0x50
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xa], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x58], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0xf], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x12c
        push   eax
        call   sub_1_5F70
        add    esp, 8
        jmp    loc_1_1E96
loc_1_1E85:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_1EBE
loc_1_1E96:
        jmp    loc_1_1EA0
loc_1_1E9B:
        jmp    loc_1_1EBE
loc_1_1EA0:
        jmp    loc_1_1E06
loc_1_1EA5:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x114], ax
        mov    eax, 1
        jmp    loc_1_1F03
loc_1_1EBE:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x12c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1EFC
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_1EBE
loc_1_1EFC:
        xor    eax, eax
        jmp    loc_1_1F03
loc_1_1F03:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1F08:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x410
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1DF1
        add    esp, 8
        test   eax, eax
        jne    loc_1_1F4B
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2026
        add    esp, 0xc
        jmp    loc_1_2021
loc_1_1F4B:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xffff
        or     eax, 0x13e
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x11e], ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xf7
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x121], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x110], 5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4F72
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_1_2013
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_41AF
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1FEE
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2026
        add    esp, 0xc
        jmp    loc_1_2021
loc_1_1FEE:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax], ecx
        jbe    loc_1_2013
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_46E9
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
loc_1_2013:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_2021:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2026:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0xb]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0xf], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_5F19
        add    esp, 8
        cmp    eax, 2
        je     loc_1_2082
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0xc
loc_1_2082:
        push   0x400
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4FDF
        add    esp, 4
        jmp    loc_1_20A4
loc_1_20A4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_20A9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x410
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 1
        je     loc_1_20EA
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0xc
        jmp    loc_1_226B
loc_1_20EA:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1DF1
        add    esp, 8
        test   eax, eax
        jne    loc_1_2119
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0xc
        jmp    loc_1_226B
loc_1_2119:
        mov    eax, dword ptr [0]            ; -> loc_1_0154
        shr    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x116], ax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_41AF
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_215F
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0xc
        jmp    loc_1_226B
loc_1_215F:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xffff
        or     eax, 0x13e
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x11e], ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x123]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x450], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xf7
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x121], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x110], 5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4F72
        add    esp, 4
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax], ecx
        jbe    loc_1_21EE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_46E9
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
loc_1_21EE:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x116]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x152]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x116], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x156], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14f
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5F13
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_1_225D
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_225D
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0xc
        jmp    loc_1_226B
loc_1_225D:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_226B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x152]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x156], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14f
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_5F19
        add    esp, 8
        cmp    eax, 2
        je     loc_1_22B8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x418], 0
loc_1_22B8:
        jmp    loc_1_22BD
loc_1_22BD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_22C2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x410
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 2
        je     loc_1_2306
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 3
        je     loc_1_2306
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 4
        jne    loc_1_231C
loc_1_2306:
        push   0x800
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        jmp    loc_1_2589
loc_1_231C:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 7
        jne    loc_1_239E
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x110], 9
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F43
        add    esp, 4
loc_1_234A:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        je     loc_1_238D
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x134
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0xc
        jmp    loc_1_234A
loc_1_238D:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6144
        add    esp, 4
        jmp    loc_1_2589
loc_1_239E:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 5
        jne    loc_1_2589
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F43
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        je     loc_1_241A
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        or     al, 0x80
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x121], al
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 8]
        add    edi, 0x2ff
        mov    esi, dword ptr [eax + 0x410]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x110], 0
        jmp    loc_1_2431
loc_1_241A:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x121], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x110], 6
loc_1_2431:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_4FDF
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x104]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_2460
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_2476
loc_1_2460:
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [eax + 0x34], 0x400
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
loc_1_2476:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x410], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x418], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x1a5], 0
loc_1_24A5:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x13c
        cmp    dword ptr [eax + 0x13c], ecx
        je     loc_1_24F1
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_24A5
loc_1_24F1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        je     loc_1_253D
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x134
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_24F1
loc_1_253D:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x12c
        cmp    dword ptr [eax + 0x12c], ecx
        je     loc_1_2589
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x12c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_253D
loc_1_2589:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_258E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x410
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_1_278D
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 0x11a]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x11a], ax
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 1
        jbe    loc_1_2719
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_46E9
        add    esp, 0xc
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax], ecx
        jbe    loc_1_270F
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        je     loc_1_264F
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5B35
        add    esp, 0x10
        jmp    loc_1_2672
loc_1_264F:
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5A92
        add    esp, 0x10
loc_1_2672:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0xc]
        cmp    eax, dword ptr [ebp - 0x10]
        jbe    loc_1_2700
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_26FB
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0xc]
        sub    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 0x10]
        add    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_02A0
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0xc]
        sub    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xf], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x58], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_5F70
        add    esp, 8
        jmp    loc_1_26FB
loc_1_26FB:
        jmp    loc_1_270A
loc_1_2700:
        mov    eax, dword ptr [ebp - 8]
        inc    word ptr [eax + 0x116]
loc_1_270A:
        jmp    loc_1_2719
loc_1_270F:
        mov    eax, dword ptr [ebp - 8]
        inc    word ptr [eax + 0x116]
loc_1_2719:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 5
        je     loc_1_2739
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 7
        jne    loc_1_2788
loc_1_2739:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x11a]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2763
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
        jmp    loc_1_2788
loc_1_2763:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x116]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2788
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_2788:
        jmp    loc_1_288C
loc_1_278D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        je     loc_1_27C5
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5B35
        add    esp, 0x10
        jmp    loc_1_27E2
loc_1_27C5:
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5A92
        add    esp, 0x10
loc_1_27E2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax], ecx
        jbe    loc_1_288C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x12c
        cmp    dword ptr [eax + 0x12c], ecx
        je     loc_1_2877
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2872
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_02A0
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xf], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x58], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_5F70
        add    esp, 8
loc_1_2872:
        jmp    loc_1_288C
loc_1_2877:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5F2B
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [eax + 0x34], 1
loc_1_288C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_2891:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, 0x410
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x14f
        cmp    eax, dword ptr [ebp + 0x10]
        je     loc_1_28EA
        mov    eax, dword ptr [ebp - 8]
        dec    byte ptr [eax + 0x14e]
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x144
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0xc
        jmp    loc_1_2906
loc_1_28EA:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 8
        jne    loc_1_2906
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6144
        add    esp, 4
loc_1_2906:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 5
        je     loc_1_2926
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 7
        jne    loc_1_293E
loc_1_2926:
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_293E
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_293E:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x110], 7
        jne    loc_1_29AE
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x144
        cmp    dword ptr [eax + 0x144], ecx
        jne    loc_1_29AE
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        jne    loc_1_29AE
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x110], 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x410], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x121], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6144
        add    esp, 4
loc_1_29AE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_29CA:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2A11
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x110], 0
        jne    loc_1_29FA
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 8
        jne    loc_1_2A11
loc_1_29FA:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_29CA
loc_1_2A11:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2A20
        jmp    loc_1_2A71
loc_1_2A20:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_042C
        add    esp, 8
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_2A42
        jmp    loc_1_2A71
loc_1_2A42:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x418], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x110], 3
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1a5], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_2A71:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_2A76:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0x2d
        je     loc_1_2A8E
        jmp    loc_1_2B11
loc_1_2A8E:
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_2A9C:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2B11
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x110], 9
        je     loc_1_2AFA
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 0x34], 0x400
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x410], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x121], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x110], 9
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x418], 0
loc_1_2AFA:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00B4
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_2A9C
loc_1_2B11:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_2B16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x3a6
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xef
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_6144
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2B52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_5ED8
        test   eax, eax
        jne    loc_1_2B72
        mov    eax, 1
        jmp    loc_1_2EED
loc_1_2B72:
        call   sub_1_5EE3
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2B98
        cmp    dword ptr [ebp - 4], 4
        je     loc_1_2B98
        mov    eax, 1
        jmp    loc_1_2EED
loc_1_2B98:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x451], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x410], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1a5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x121], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x11e], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_49BF
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x114], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x14d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x14c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x14e], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3df
        push   eax
        call   sub_1_5EE9
        add    esp, 4
        test   eax, eax
        je     loc_1_2C36
        mov    eax, 1
        jmp    loc_1_2EED
loc_1_2C36:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x418], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3df
        push   eax
        call   sub_1_5EEF
        add    esp, 8
        test   eax, eax
        je     loc_1_2C6C
        mov    eax, 1
        jmp    loc_1_2EED
loc_1_2C6C:
        push   0                             ; -> loc_1_01D4
        push   0                             ; -> loc_1_0138
        push   0                             ; -> loc_1_0118
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3df
        push   eax
        push   0xffff
        call   sub_1_5EFB
        add    esp, 0x14
        test   eax, eax
        je     loc_1_2CA3
        mov    eax, 1
        jmp    loc_1_2EED
loc_1_2CA3:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x3bd], 0    ; -> loc_1_01DC
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3bc], 0xb
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x3c1], 0    ; -> loc_1_00B8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3c5], 2
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x3c8], 0    ; -> obj1:0x01E8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3c7], 0x13
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3cc], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x3df]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x3cd], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x3d4], 0    ; -> obj1:0x01FC
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3d3], 0xa
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3d8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_2D60
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3d9], 0xb
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x3db], 0    ; -> obj1:0x0108
        jmp    loc_1_2DAD
loc_1_2D60:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 2
        je     loc_1_2D94
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3d9], 6
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x3db], 0    ; -> loc_1_00F8
        jmp    loc_1_2DAD
loc_1_2D94:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3d9], 6
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x3db], 0    ; -> loc_1_0100
loc_1_2DAD:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x140], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x140]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x13c], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x134
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x138], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x138]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x134], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x144
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x148], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x148]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x144], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x12c
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x130], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x130]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x12c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x14f], 0x50
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x151], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x156], 0x40
        push   0x40
        call   sub_1_02E3
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x152], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x152], 0
        jne    loc_1_2E88
        mov    eax, 1
        jmp    loc_1_2EED
loc_1_2E88:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3a6
        push   eax
        call   sub_1_5F31
        add    esp, 4
        test   eax, eax
        jne    loc_1_2EBF
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x451], al
        jmp    loc_1_2EDA
loc_1_2EBF:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3df
        push   eax
        call   sub_1_5F01
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_2EED
loc_1_2EDA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B0A
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_2EED
loc_1_2EED:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2EF2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 0x17
        jne    loc_1_2F38
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 0
loc_1_2F38:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 2
        je     loc_1_2F68
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 3
        je     loc_1_2F68
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 4
        jne    loc_1_2F72
loc_1_2F68:
        call   sub_1_4C75
        jmp    loc_1_2F38
loc_1_2F72:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        test   al, 7
        je     loc_1_2FBF
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14f
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F19
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F43
        add    esp, 4
        jmp    loc_1_30B7
loc_1_2FBF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 5
        jne    loc_1_30B7
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 7
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x144
        cmp    dword ptr [eax + 0x144], ecx
        jne    loc_1_300C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        je     loc_1_301A
loc_1_300C:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_614C
        add    esp, 8
loc_1_301A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4FDF
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x152]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x156], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14f
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F19
        add    esp, 8
        cmp    eax, 2
        jne    loc_1_30B7
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 8
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_614C
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 9
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        test   al, 7
        je     loc_1_30B7
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F43
        add    esp, 4
loc_1_30B7:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x410], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x121], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 9
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x10
        je     loc_1_3127
        push   0                             ; -> loc_1_2B16
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3a6
        push   eax
        call   sub_1_5F37
        add    esp, 8
        cmp    eax, 2
        je     loc_1_3127
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xef
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x451], al
loc_1_3127:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 0x10
        je     loc_1_314B
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_614C
        add    esp, 8
loc_1_314B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3df
        push   eax
        call   sub_1_5F01
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x152]
        push   eax
        call   sub_1_034B
        add    esp, 4
loc_1_316E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x13c
        cmp    dword ptr [eax + 0x13c], ecx
        je     loc_1_31BA
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_316E
loc_1_31BA:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        je     loc_1_3206
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x134
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_31BA
loc_1_3206:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x12c
        cmp    dword ptr [eax + 0x12c], ecx
        je     loc_1_3252
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x12c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    loc_1_3206
loc_1_3252:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_325A
loc_1_325A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_325F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x6e0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x451], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 0x17
        jne    loc_1_32A5
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x110], 0
loc_1_32A5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 2
        je     loc_1_32D5
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 3
        je     loc_1_32D5
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 4
        jne    loc_1_32E1
loc_1_32D5:
        mov    dword ptr [ebp - 4], 0x6e1
        jmp    loc_1_335F
loc_1_32E1:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 5
        jne    loc_1_335F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x104]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_3314
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_335F
loc_1_3314:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        test   al, 1
        je     loc_1_3336
        mov    dword ptr [ebp - 4], 0x6e2
        jmp    loc_1_335F
loc_1_3336:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x121]
        and    eax, 0xff
        test   al, 6
        je     loc_1_3358
        mov    dword ptr [ebp - 4], 0x6e3
        jmp    loc_1_335F
loc_1_3358:
        mov    dword ptr [ebp - 4], 0x6e1
loc_1_335F:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_3367
loc_1_3367:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_336C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_3375:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x14d]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_33C0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x116]
        jmp    loc_1_3375
loc_1_33C0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x116]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_33E3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_33E8
        add    esp, 4
loc_1_33E3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_33E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_1_3418
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
        jmp    loc_1_342B
loc_1_3418:
        push   0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F2B
        add    esp, 8
loc_1_342B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3430:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_343B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_3444:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x13c
        cmp    dword ptr [eax + 0x13c], ecx
        jne    loc_1_3461
        jmp    loc_1_358D
loc_1_3461:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        je     loc_1_34CF
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x58]
        and    ecx, 0xffff
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x58]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0xb]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5B35
        add    esp, 0x10
        jmp    loc_1_350E
loc_1_34CF:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x58]
        and    ecx, 0xffff
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x58]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0xb]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A92
        add    esp, 0x10
loc_1_350E:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x58]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x58], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x58]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0xf]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_356E
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x116]
        jmp    loc_1_3583
loc_1_356E:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        push   eax
        call   sub_1_4B5D
        add    esp, 8
loc_1_3583:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3444
loc_1_358D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x116]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_35B0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_33E8
        add    esp, 4
loc_1_35B0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_35B5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 0
        jne    loc_1_35ED
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        test   al, 8
        je     loc_1_35ED
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B0A
        add    esp, 4
loc_1_35ED:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 6
        je     loc_1_360D
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 9
        jne    loc_1_3628
loc_1_360D:
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 0x34], 0x400
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        jmp    loc_1_3716
loc_1_3628:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 5
        je     loc_1_364D
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 7
        je     loc_1_364D
        jmp    loc_1_3716
loc_1_364D:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x108]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_36AB
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        jne    loc_1_368F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_36AB
loc_1_368F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xffff
        or     eax, 0x40
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11e], ax
loc_1_36AB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        je     loc_1_3701
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_36EC
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_39EE
        add    esp, 8
        jmp    loc_1_36FC
loc_1_36EC:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_394F
        add    esp, 8
loc_1_36FC:
        jmp    loc_1_3711
loc_1_3701:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_394F
        add    esp, 8
loc_1_3711:
        jmp    loc_1_3716
loc_1_3716:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_371B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_373E
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14c
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_3753
loc_1_373E:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_1_3753
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14d
        mov    dword ptr [ebp - 4], eax
loc_1_3753:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> loc_1_0154
        shr    ecx, 1
        cmp    eax, ecx
        jae    loc_1_37E6
        mov    eax, dword ptr [ebp - 4]
        inc    byte ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x12c
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x5a], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x58], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xb]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xb]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_37D0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xf], 2
        jmp    loc_1_37D9
loc_1_37D0:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xf], 0
loc_1_37D9:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_37ED
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_37E6:
        xor    eax, eax
        jmp    loc_1_37ED
loc_1_37ED:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_37F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_3815
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x134
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_383A
loc_1_3815:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_1_382F
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_383A
loc_1_382F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x144
        mov    dword ptr [ebp - 4], eax
loc_1_383A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        je     loc_1_3873
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3886
loc_1_3873:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
loc_1_3886:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_388E
loc_1_388E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3893:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_38B6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x134
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_38DB
loc_1_38B6:
        cmp    dword ptr [ebp + 0x10], 1
        jne    loc_1_38D0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x13c
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_38DB
loc_1_38D0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x144
        mov    dword ptr [ebp - 4], eax
loc_1_38DB:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_38F5
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_5F70
        add    esp, 8
loc_1_38F5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_38FA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_3918
        mov    eax, dword ptr [ebp + 8]
        dec    byte ptr [eax + 0x14c]
        jmp    loc_1_392B
loc_1_3918:
        cmp    dword ptr [ebp + 0x10], 1
        jne    loc_1_392B
        mov    eax, dword ptr [ebp + 8]
        dec    byte ptr [eax + 0x14d]
loc_1_392B:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_394A
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x12c
        push   eax
        call   sub_1_5F70
        add    esp, 8
loc_1_394A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_394F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x104]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_3998
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x58], 0
        jne    loc_1_3998
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        jne    loc_1_3998
        jmp    loc_1_39E9
loc_1_3998:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_39B8
        jmp    loc_1_39E9
loc_1_39B8:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E00
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
loc_1_39E9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_39EE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x104]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x58], 0
        jne    loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x116]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        jne    loc_1_3A65
        jmp    loc_1_3BE0
loc_1_3A65:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3BB8
loc_1_3A70:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BE5
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    loc_1_3BE0
loc_1_3A8D:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3AAD
        jmp    loc_1_3BE0
loc_1_3AAD:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E00
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    loc_1_3BE0
loc_1_3AE3:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3B03
        jmp    loc_1_3BE0
loc_1_3B03:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CBE
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    loc_1_3BE0
loc_1_3B39:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BE5
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3B65
        jmp    loc_1_3BE0
loc_1_3B65:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CBE
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E00
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    loc_1_3BE0
        db  0E9h, 02Dh, 000h, 000h, 000h, 0E9h, 028h, 000h, 000h, 000h
loc_1_3BB8:
        cmp    dword ptr [ebp - 8], 4
        ja     loc_1_3BE0
        mov    eax, dword ptr [ebp - 8]
        jmp    dword ptr [eax*4]             ; -> obj1:0x3BCC
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_3B39
        dd  loc_1_3A70
        dd  loc_1_3B39
        dd  loc_1_3A8D
        dd  loc_1_3AE3
loc_1_3BE0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        je     loc_1_3C4F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x134]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x116]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0xb]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x116], 0
        jmp    loc_1_3CB9
        db  0E9h, 06Ah, 000h, 000h, 000h
loc_1_3C4F:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3C6F
        jmp    loc_1_3CB9
loc_1_3C6F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xb]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x116]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0xb]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x116], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        jmp    loc_1_3CB9
loc_1_3CB9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3CBE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3DF3
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x5a]
        and    eax, 0xffff
        test   al, 2
        jne    loc_1_3D6A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x114]
        and    eax, 0xffff
        sub    eax, 0x40
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0xf]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_3D6A
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_42D9
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0xf], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x11e], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x5a]
        and    eax, 1
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x5a], ax
        jmp    loc_1_3DF3
loc_1_3D6A:
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp + 0xc], eax
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_3DA4
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_1_3DFB
        db  0E9h, 04Fh, 000h, 000h, 000h
loc_1_3DA4:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_42D9
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0xf], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x11e], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x5a]
        and    eax, 1
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x5a], ax
loc_1_3DF3:
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_1_3DFB
loc_1_3DFB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_3E1B
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_1_404B
loc_1_3E1B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x104]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_3EEB
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x114]
        and    eax, 0xffff
        dec    eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0xf]
        and    ecx, 0xffff
        cmp    eax, ecx
        jg     loc_1_3E8B
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp + 0xc], eax
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_3E8B
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_1_404B
loc_1_3E8B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x106]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0xf]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [edx + 0xb]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        inc    eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0xf], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x5a]
        and    eax, 0xffff
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x5a], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x104]
        and    eax, 0xbf
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x104], al
loc_1_3EEB:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_4043
        mov    dword ptr [ebp - 4], 0
loc_1_3EFF:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x5a]
        and    eax, 0xffff
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x5a], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        je     loc_1_3F82
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x114]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0xf]
        and    ecx, 0xffff
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        add    eax, dword ptr [ecx + 0xb]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5D7A
        add    esp, 0x10
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5E54
        add    esp, 8
        jmp    loc_1_3FD4
loc_1_3F82:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x114]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0xf]
        and    ecx, 0xffff
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        add    eax, dword ptr [ecx + 0xb]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5D20
        add    esp, 0x10
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5E0B
        add    esp, 8
loc_1_3FD4:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0xf], ax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_4039
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0xc
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp + 0xc], eax
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_4034
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_1_404B
loc_1_4034:
        jmp    loc_1_403E
loc_1_4039:
        jmp    loc_1_4043
loc_1_403E:
        jmp    loc_1_3EFF
loc_1_4043:
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_1_404B
loc_1_404B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4050:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_4059:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x14e]
        and    eax, 0xff
        cmp    eax, 2
        jge    loc_1_41AA
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x134
        cmp    dword ptr [eax + 0x134], ecx
        je     loc_1_41AA
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x134
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        cmp    eax, 2
        jne    loc_1_40BD
        mov    eax, dword ptr [ebp - 4]
        dec    word ptr [eax + 0xf]
        jmp    loc_1_40EE
loc_1_40BD:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11a]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_40EE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x134
        push   eax
        call   sub_1_4B5D
        add    esp, 8
        jmp    loc_1_41AA
loc_1_40EE:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x410
        push   eax
        call   sub_1_5F1F
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_4152
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_4152
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x134
        push   eax
        call   sub_1_4B5D
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 0x34], 0x400
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        jmp    loc_1_41AA
        db  0E9h, 053h, 000h, 000h, 000h
loc_1_4152:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xf]
        and    eax, 0xffff
        cmp    eax, 1
        jle    loc_1_4187
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_4187
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x11a]
loc_1_4187:
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 0x14e]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x144
        push   eax
        call   sub_1_5F70
        add    esp, 8
        jmp    loc_1_4059
loc_1_41AA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_41AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0x7f
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11a], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        test   al, 0x80
        jne    loc_1_41FE
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_42D4
loc_1_41FE:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        inc    eax
        add    dword ptr [ebp - 0xc], eax
        inc    dword ptr [ebp - 8]
loc_1_4216:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_42CC
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_1_42B8
loc_1_4244:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
loc_1_4256:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     loc_1_4294
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x18], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_428C
        xor    eax, eax
        jmp    loc_1_42D4
loc_1_428C:
        dec    byte ptr [ebp - 4]
        jmp    loc_1_4256
loc_1_4294:
        jmp    loc_1_42C7
loc_1_4299:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        inc    eax
        add    dword ptr [ebp - 8], eax
        jmp    loc_1_42C7
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_42B8:
        cmp    dword ptr [ebp - 0x1c], 1
        je     loc_1_4244
        jmp    loc_1_4299
loc_1_42C7:
        jmp    loc_1_4216
loc_1_42CC:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_42D4
loc_1_42D4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_42D9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 3]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        jne    loc_1_4336
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        je     loc_1_432C
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x3f
        mov    word ptr [ebp + 0xc], ax
        jmp    loc_1_4336
loc_1_432C:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 1
        mov    word ptr [ebp + 0xc], ax
loc_1_4336:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   al, 1
        je     loc_1_4375
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
loc_1_4375:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   al, 2
        je     loc_1_43EB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4B90
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x10
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 4
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp - 0xc]
        shr    eax, 0x18
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp - 0xc]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 1], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 2], ah
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 3], al
        add    dword ptr [ebp + 0x14], 4
loc_1_43EB:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   al, 4
        je     loc_1_442D
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x11
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0x3f
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
loc_1_442D:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   al, 8
        je     loc_1_4520
        mov    byte ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9d], 2
        je     loc_1_445B
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 1
        mov    byte ptr [ebp - 0x10], al
loc_1_445B:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9d], 1
        je     loc_1_4475
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 2
        mov    byte ptr [ebp - 0x10], al
loc_1_4475:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9d], 0x40
        je     loc_1_448F
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 4
        mov    byte ptr [ebp - 0x10], al
loc_1_448F:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9c], 8
        je     loc_1_44A9
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 8
        mov    byte ptr [ebp - 0x10], al
loc_1_44A9:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9d], 0x20
        je     loc_1_44C3
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 0x10
        mov    byte ptr [ebp - 0x10], al
loc_1_44C3:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9c], 0x10
        je     loc_1_44DD
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 0x20
        mov    byte ptr [ebp - 0x10], al
loc_1_44DD:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9c], 0x60
        je     loc_1_44F7
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 0x40
        mov    byte ptr [ebp - 0x10], al
loc_1_44F7:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x12
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 0x14]
        mov    al, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
loc_1_4520:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   al, 0x10
        je     loc_1_4574
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x13
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 2
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xad]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xae]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
loc_1_4574:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   al, 0x20
        je     loc_1_45C8
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x14
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 2
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xae]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xad]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
loc_1_45C8:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   al, 0x40
        je     loc_1_463B
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x16
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x108]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_4617
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        jmp    loc_1_4621
loc_1_4617:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0
loc_1_4621:
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x108]
        and    eax, 0xbf
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x108], al
loc_1_463B:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   ah, 1
        je     loc_1_4692
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x20
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x452]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x452]
        and    eax, 0xfc
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x452], al
loc_1_4692:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        test   ah, 4
        je     loc_1_46BD
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0x22
        inc    dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0
        inc    dword ptr [ebp + 0x14]
loc_1_46BD:
        mov    eax, dword ptr [ebp + 0x14]
        sub    eax, dword ptr [ebp - 4]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx + 1]
        and    ecx, 0xff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 0x14]
        jmp    loc_1_46E4
loc_1_46E4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_46E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        inc    eax
        add    dword ptr [ebp + 0x10], eax
        inc    dword ptr [ebp - 8]
loc_1_4710:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_49B2
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_494F
loc_1_473E:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x123]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        and    eax, ecx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     loc_1_4784
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x450], al
loc_1_4784:
        jmp    loc_1_49AD
loc_1_4789:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        jne    loc_1_47CA
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_47DA
loc_1_47CA:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_51DA
        add    esp, 8
loc_1_47DA:
        jmp    loc_1_49AD
loc_1_47DF:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x120]
        and    eax, 0xff
        test   al, 1
        jne    loc_1_4820
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_4830
loc_1_4820:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5186
        add    esp, 8
loc_1_4830:
        jmp    loc_1_49AD
loc_1_4835:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_4896
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4A4A
        add    esp, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_501C
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xfbff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11e], ax
loc_1_4896:
        jmp    loc_1_49AD
loc_1_489B:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_48EF
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_501C
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xfbff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11e], ax
loc_1_48EF:
        jmp    loc_1_49AD
loc_1_48F4:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x450]
        and    eax, 0xff
        test   al, 4
        je     loc_1_492B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11e]
        and    eax, 0xffff
        or     eax, 0x100
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11e], ax
loc_1_492B:
        jmp    loc_1_49AD
loc_1_4930:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        inc    eax
        add    dword ptr [ebp - 8], eax
        jmp    loc_1_49AD
        db  0E9h, 05Eh, 000h, 000h, 000h
loc_1_494F:
        cmp    dword ptr [ebp - 0x10], 0x22
        ja     loc_1_4930
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]            ; -> obj1:0x498A
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x496E
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_473E
        dd  loc_1_4789
        dd  loc_1_47DF
        dd  loc_1_4835
        dd  loc_1_489B
        dd  loc_1_48F4
        dd  loc_1_4930
        db  000h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 001h, 002h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  003h, 004h, 005h
loc_1_49AD:
        jmp    loc_1_4710
loc_1_49B2:
        mov    eax, dword ptr [ebp + 0x10]
        jmp    loc_1_49BA
loc_1_49BA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_49BF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   3
        push   0                             ; -> loc_1_0208
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf4]
        push   eax
        call   sub_1_4AC0
        add    esp, 0xc
        test   eax, eax
        je     loc_1_49FF
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x120], 2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x123], 3
        jmp    loc_1_4A45
loc_1_49FF:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x120], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x123], 7
        push   0                             ; -> loc_1_020C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0150
loc_1_4A21:
        add    esp, 8
        test   eax, eax
        je     loc_1_4A45
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x451]
        and    eax, 0xff
        or     al, 0x80
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x451], al
loc_1_4A45:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4A4A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   al, 8
        je     loc_1_4A6E
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0x10
        mov    byte ptr [ebp - 4], al
loc_1_4A6E:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   al, 2
        je     loc_1_4A85
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 1
        mov    byte ptr [ebp - 4], al
loc_1_4A85:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   al, 4
        je     loc_1_4A9C
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0xa0
        mov    byte ptr [ebp - 4], al
loc_1_4A9C:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   al, 1
        je     loc_1_4AB3
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0xa
        mov    byte ptr [ebp - 4], al
loc_1_4AB3:
        mov    al, byte ptr [ebp - 4]
        jmp    loc_1_4ABB
loc_1_4ABB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_4AC9:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_4B0A
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_4B0A
        dec    dword ptr [ebp + 0x10]
        jmp    loc_1_4AC9
loc_1_4B0A:
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_4B1E
        mov    eax, 1
        jmp    loc_1_4B20
loc_1_4B1E:
        xor    eax, eax
loc_1_4B20:
        jmp    loc_1_4B25
loc_1_4B25:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4B2A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_4B58
loc_1_4B58:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4B5D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_4B90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x98]
        cmp    eax, 0x1c200
        je     loc_1_4BC6
        cmp    eax, 0xff10
        jb     loc_1_4BC6
        mov    ebx, eax
        sub    ebx, 0xff10
        shl    ebx, 1
        movzx  ecx, word ptr [ebx]           ; -> loc_1_024C
        xor    edx, edx
        mov    eax, 0x1c200
        div    ecx
loc_1_4BC6:
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        pop    esi
        leave  
        ret    
sub_1_4BED:
        pushal 
        or     byte ptr [esi + 0xcd], al
        mov    ebx, dword ptr [esi + 0xf0]
        push   dword ptr [ebx]
        push   eax
        cmp    eax, 4
        jne    loc_1_4C11
        push   dword ptr [esi + 0xc8]
        push   esi
        call   dword ptr [esi + 0xc4]
        jmp    loc_1_4C37
loc_1_4C11:
        cmp    eax, 1
        jne    loc_1_4C25
        push   dword ptr [esi + 0xd4]
        push   esi
        call   dword ptr [esi + 0xd0]
        jmp    loc_1_4C37
loc_1_4C25:
        cmp    eax, 2
        jne    loc_1_4C37
        push   dword ptr [esi + 0xdc]
        push   esi
        call   dword ptr [esi + 0xd8]
loc_1_4C37:
        add    esp, 0x10
        popal  
        ret    
sub_1_4C3C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0xf0]
        bts    dword ptr [ebx], 9
        jb     loc_1_4C70
        test   byte ptr [esi + 0xcd], 0x40
        je     loc_1_4C70
        test   dword ptr [esi + 0xec], 0x200
        je     loc_1_4C70
        mov    eax, 4
        call   sub_1_4BED
loc_1_4C70:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_4C75:
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
sub_1_4CA8:
        cld    
        and    dword ptr [esi + 0x34], 0xfffffeff
        mov    eax, dword ptr [esi + 0x48]
        sub    eax, dword ptr [esi + 0x58]
        jle    loc_1_4D1D
        cmp    ecx, eax
        jbe    loc_1_4CC5
        mov    ecx, eax
        or     dword ptr [esi + 0x34], 0x100
loc_1_4CC5:
        push   ecx
        mov    eax, dword ptr [esi + 0x60]
        xor    edx, edx
        add    eax, ecx
        sub    eax, dword ptr [esi + 0x48]
        jbe    loc_1_4CD6
        mov    edx, eax
        sub    ecx, edx
loc_1_4CD6:
        push   esi
        mov    eax, dword ptr [esi + 0x44]
        mov    esi, dword ptr [esi + 0x60]
        add    esi, eax
        xchg   edi, esi
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        jecxz  loc_1_4CEC
        mov    edi, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_4CEC:
        pop    esi
        sub    edi, eax
        cmp    edi, dword ptr [esi + 0x48]
        jb     loc_1_4CF6
        xor    edi, edi
loc_1_4CF6:
        mov    dword ptr [esi + 0x60], edi
        pop    eax
        add    dword ptr [esi + 0x58], eax
        mov    ebx, dword ptr [esi + 0xe4]
        cmp    dword ptr [esi + 0x58], ebx
        jb     loc_1_4D0F
        and    byte ptr [esi + 0xcd], 0xfd
loc_1_4D0F:
        push   eax
        push   3
        push   esi
        call   sub_1_35B5
        add    esp, 8
        pop    eax
        ret    
loc_1_4D1D:
        or     eax, eax
        je     loc_1_4D2A
        or     dword ptr [esi + 0x34], 0x100
        xor    eax, eax
loc_1_4D2A:
        jmp    loc_1_4D0F
sub_1_4D2C:
        or     ebx, ebx
        je     loc_1_4D77
        xor    eax, eax
        test   byte ptr [esi + 0x120], 2
        jne    loc_1_4D69
        mov    al, byte ptr [esi + 0x10d]
        and    al, byte ptr [esi + 0x10a]
        xor    al, byte ptr [esi + 0x10a]
        shr    al, 4
        mov    ah, byte ptr [esi + 0x108]
        and    ah, 0x18
        or     al, ah
        mov    ah, byte ptr [esi + 0x104]
        and    ah, 0x60
        or     al, ah
        movzx  eax, al
loc_1_4D69:
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esi + 0x4c]
        mov    dword ptr [ebx + 4], ecx
        mov    ecx, dword ptr [esi + 0x58]
        mov    dword ptr [ebx + 8], ecx
loc_1_4D77:
        ret    
sub_1_4D78:
        test   byte ptr [esi + 0x108], 0x12
        je     loc_1_4DC7
        cmp    ecx, dword ptr [esi + 0xa0]
        ja     loc_1_4DC7
        test   byte ptr [esi + 0x108], 2
        je     loc_1_4DB0
        mov    ah, byte ptr [esi + 0x109]
        or     ah, ah
        je     loc_1_4DB0
        mov    al, byte ptr [esi + 0x10e]
        or     al, ah
        and    byte ptr [esi + 0x108], 0xfd
        call   sub_1_5036
loc_1_4DB0:
        test   byte ptr [esi + 0x108], 0x10
        je     loc_1_4DC7
        and    byte ptr [esi + 0x108], 0xef
        or     byte ptr [esi + 0x108], 4
loc_1_4DC7:
        ret    
sub_1_4DC8:
        mov    dword ptr [esi + 0x94], 0x30
        test   cl, 1
        je     loc_1_4DE8
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0x98], eax
        or     word ptr [esi + 0x11e], 2
loc_1_4DE8:
        test   cl, 0x70
        je     loc_1_4E10
        mov    al, byte ptr [ebx + 0x16]
        mov    byte ptr [esi + 0xaa], al
        mov    al, byte ptr [ebx + 0x17]
        mov    byte ptr [esi + 0xab], al
        mov    al, byte ptr [ebx + 0x18]
        mov    byte ptr [esi + 0xac], al
        or     word ptr [esi + 0x11e], 4
loc_1_4E10:
        test   cl, 0x80
        je     loc_1_4E26
        mov    al, byte ptr [ebx + 0x19]
        mov    byte ptr [esi + 0xad], al
        or     word ptr [esi + 0x11e], 0x30
loc_1_4E26:
        test   ch, 1
        je     loc_1_4E3C
        mov    al, byte ptr [ebx + 0x1a]
        mov    byte ptr [esi + 0xae], al
        or     word ptr [esi + 0x11e], 0x30
loc_1_4E3C:
        test   cl, 2
        je     loc_1_4E5C
        mov    eax, dword ptr [ebx + 8]
        xchg   dword ptr [esi + 0x9c], eax
        xor    eax, dword ptr [ebx + 8]
        test   eax, 0x6378
        je     loc_1_4E5C
        or     word ptr [esi + 0x11e], 8
loc_1_4E5C:
        push   esi
        lea    esi, [esi + 0x94]
        test   cl, 4
        je     loc_1_4E6E
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0xc], eax
loc_1_4E6E:
        test   cl, 8
        je     loc_1_4E79
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_4E79:
        test   ch, 2
        je     loc_1_4E84
        mov    al, byte ptr [ebx + 0x1b]
        mov    byte ptr [esi + 0x1b], al
loc_1_4E84:
        test   ch, 4
        je     loc_1_4E8F
        mov    al, byte ptr [ebx + 0x1c]
        mov    byte ptr [esi + 0x1c], al
loc_1_4E8F:
        test   ch, 8
        je     loc_1_4E9A
        mov    al, byte ptr [ebx + 0x1d]
        mov    byte ptr [esi + 0x1d], al
loc_1_4E9A:
        test   ch, 0x10
        je     loc_1_4EA5
        mov    al, byte ptr [ebx + 0x1e]
        mov    byte ptr [esi + 0x1e], al
loc_1_4EA5:
        test   ch, 0x20
        je     loc_1_4EB0
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [esi + 0x20], eax
loc_1_4EB0:
        test   ch, 0x80
        je     loc_1_4EBB
        mov    eax, dword ptr [ebx + 0x28]
        mov    dword ptr [esi + 0x28], eax
loc_1_4EBB:
        test   ch, 0x40
        je     loc_1_4EC6
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x24], eax
loc_1_4EC6:
        test   ecx, 0x10000
        je     loc_1_4ED4
        mov    eax, dword ptr [ebx + 0x2c]
        mov    dword ptr [esi + 0x2c], eax
loc_1_4ED4:
        mov    ax, word ptr [ebx + 0x14]
        mov    word ptr [esi + 0x14], ax
        pop    esi
        ret    
sub_1_4EDE:
        push   edx
        mov    ecx, dword ptr [ebx + 4]
        xor    eax, eax
        cmp    ecx, 0x1c200
        je     loc_1_4EFB
        cmp    ecx, 0xff10
        jae    loc_1_4F02
        cmp    ecx, 2
        jae    loc_1_4EFB
loc_1_4EF9:
        mov    ecx, eax
loc_1_4EFB:
        mov    eax, 0xfffffff4
        pop    edx
        ret    
loc_1_4F02:
        cmp    ecx, 0xff1f
        ja     loc_1_4EF9
        push   ebx
        mov    ebx, ecx
        sub    ebx, 0xff10
        shl    ebx, 1
        movzx  eax, word ptr [ebx]           ; -> loc_1_024C
        pop    ebx
        jmp    loc_1_4EF9
sub_1_4F1F:
        movzx  eax, word ptr [ebx + 0x16]
        cmp    ah, 4
        ja     loc_1_4F6B
        mov    edi, 0xff1e
        or     ah, ah
        jne    loc_1_4F37
        xor    edi, 0x404
loc_1_4F37:
        cmp    al, 8
        ja     loc_1_4F64
        sub    al, 5
        jb     loc_1_4F64
        add    ah, ah
        je     loc_1_4F45
        dec    ah
loc_1_4F45:
        shl    ah, 3
        or     al, ah
        or     al, 4
        mov    ah, byte ptr [ebx + 0x18]
        or     ah, ah
        js     loc_1_4F6B
        je     loc_1_4F60
        sub    ah, 2
        je     loc_1_4F62
        jns    loc_1_4F6B
        test   al, 3
        jne    loc_1_4F6B
loc_1_4F60:
        and    al, 0xfb
loc_1_4F62:
        clc    
        ret    
loc_1_4F64:
        mov    eax, 0xfffffff5
        stc    
        ret    
loc_1_4F6B:
        mov    eax, 0xfffffffb
        stc    
        ret    
sub_1_4F72:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x120], 1
        je     loc_1_4FDB
        and    byte ptr [esi + 0x108], 0xf7
        test   byte ptr [esi + 0x450], 4
        je     loc_1_4FBA
        mov    ah, 3
        mov    al, byte ptr [esi + 0x10e]
        test   al, 2
        je     loc_1_4FA2
        or     ah, 8
loc_1_4FA2:
        test   al, 1
        je     loc_1_4FA9
        or     ah, 4
loc_1_4FA9:
        mov    byte ptr [esi + 0x452], ah
        or     word ptr [esi + 0x11e], 0x100
        jmp    loc_1_4FDB
loc_1_4FBA:
        xor    ah, ah
        mov    al, byte ptr [esi + 0x10e]
        test   al, 2
        je     loc_1_4FC9
        or     ah, 0x11
loc_1_4FC9:
        test   al, 1
        je     loc_1_4FD0
        or     ah, 0xaa
loc_1_4FD0:
        mov    byte ptr [esi + 0x10d], ah
        call   sub_1_50F0
loc_1_4FDB:
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_4FDF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x120], 1
        je     loc_1_5018
        or     byte ptr [esi + 0x108], 8
        xor    ah, ah
        mov    al, byte ptr [esi + 0x10e]
        test   al, 2
        je     loc_1_5006
        or     ah, 1
loc_1_5006:
        test   al, 1
        je     loc_1_500D
        or     ah, 0xa
loc_1_500D:
        mov    byte ptr [esi + 0x10d], ah
        call   sub_1_50F0
loc_1_5018:
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_501C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x10d], al
        call   sub_1_50F0
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_5036:
        mov    ah, al
        xor    ah, byte ptr [esi + 0x10e]
        and    ah, 3
        mov    byte ptr [esi + 0x10e], al
        and    al, 3
        shl    al, 2
        or     al, ah
        mov    ah, byte ptr [esi + 0x109]
        or     ah, ah
        je     loc_1_5076
        push   eax
        mov    al, byte ptr [esi + 0x10e]
        and    al, ah
        cmp    al, ah
        je     loc_1_506E
        or     byte ptr [esi + 0x108], 2
        jmp    loc_1_5075
loc_1_506E:
        and    byte ptr [esi + 0x108], 0xfd
loc_1_5075:
        pop    eax
loc_1_5076:
        xor    ah, ah
        cmp    dword ptr [esi + 0x110], 5
        je     loc_1_50A4
        push   eax
        call   sub_1_50E0
        pop    eax
        and    al, 5
        cmp    al, 5
        jne    loc_1_50A2
        cmp    dword ptr [esi + 0x110], 0
        jne    loc_1_50A2
        pushal 
        push   esi
        call   sub_1_1B0A
        add    esp, 4
        popal  
loc_1_50A2:
        jmp    loc_1_50EF
loc_1_50A4:
        test   byte ptr [esi + 0x450], 4
        je     loc_1_50BE
        mov    byte ptr [esi + 0x452], al
        or     word ptr [esi + 0x11e], 0x100
        jmp    loc_1_50EF
loc_1_50BE:
        test   al, 8
        je     loc_1_50C5
        or     ah, 0x10
loc_1_50C5:
        test   al, 2
        je     loc_1_50CC
        or     ah, 1
loc_1_50CC:
        test   al, 4
        je     loc_1_50D6
        or     ah, 0x20
        or     ah, 0x80
loc_1_50D6:
        test   al, 1
        je     sub_1_50E0
        or     ah, 2
        or     ah, 8
sub_1_50E0:
        push   ebx
        push   ecx
        mov    byte ptr [esi + 0x10d], ah
        call   sub_1_50F0
        pop    ecx
        pop    ebx
loc_1_50EF:
        ret    
sub_1_50F0:
        mov    al, byte ptr [esi + 0x10d]
        mov    ebx, dword ptr [esi + 0xfc]
        mov    byte ptr [ebx], al
        mov    ebx, dword ptr [esi + 0xf0]
        mov    ecx, dword ptr [ebx]
        push   ecx
        mov    ch, al
        movzx  eax, al
        mov    ah, al
        shr    eax, 2
        shr    ah, 1
        shr    eax, 3
        and    eax, 0x138
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 2
        and    eax, 0xc00
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 3
        and    eax, 0x1000
        or     dword ptr [ebx], eax
        mov    ah, ch
        shl    ah, 3
        and    eax, 0x2000
        or     dword ptr [ebx], eax
        mov    al, byte ptr [esi + 0x10a]
        or     al, al
        je     loc_1_5161
        and    ch, al
        cmp    ch, al
        je     loc_1_515A
        or     byte ptr [esi + 0x108], 0x20
        jmp    loc_1_5161
loc_1_515A:
        and    byte ptr [esi + 0x108], 0x5f
loc_1_5161:
        mov    eax, dword ptr [esi + 0xec]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0xcd], 0x40
        je     loc_1_5185
        not    ebx
        and    eax, ebx
        je     loc_1_5185
        mov    eax, 4
        call   sub_1_4BED
loc_1_5185:
        ret    
sub_1_5186:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        and    ah, 1
        je     loc_1_51D6
        mov    ah, 0x10
        and    ah, byte ptr [esi + 0x107]
        je     loc_1_51D6
        or     byte ptr [esi + 0x34], ah
        mov    ebx, dword ptr [esi + 0xf0]
        mov    ecx, dword ptr [ebx]
        push   ecx
        or     byte ptr [ebx], 0x80
        or     byte ptr [ebx], 0x40
        mov    eax, dword ptr [esi + 0xec]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0xcd], 0x40
        je     loc_1_51D6
        not    ebx
        and    eax, ebx
        je     loc_1_51D6
        mov    eax, 4
        call   sub_1_4BED
loc_1_51D6:
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_51DA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        and    ah, 0xe
        and    ah, byte ptr [esi + 0x107]
        je     loc_1_5223
        or     byte ptr [esi + 0x34], ah
        mov    ebx, dword ptr [esi + 0xf0]
        mov    ecx, dword ptr [ebx]
        push   ecx
        or     byte ptr [ebx], 0x80
        mov    eax, dword ptr [esi + 0xec]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0xcd], 0x40
        je     loc_1_5223
        not    ebx
        and    eax, ebx
        je     loc_1_5223
        mov    eax, 4
        call   sub_1_4BED
loc_1_5223:
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        test   byte ptr [esi + 0x108], 0x60
        jne    loc_1_525B
        test   byte ptr [esi + 0x9c], 0x60
        jne    loc_1_5252
        test   byte ptr [esi + 0x108], 4
        je     loc_1_5252
        and    byte ptr [esi + 0x108], 0xeb
loc_1_5252:
        test   byte ptr [esi + 0x108], 8
        je     loc_1_525C
loc_1_525B:
        inc    eax
loc_1_525C:
        pop    esi
        leave  
        ret    
sub_1_525F:
        mov    esi, dword ptr [0]            ; -> obj1:0x00B4
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_5287
loc_1_526D:
        test   byte ptr [eax + 0x105], 0x20
        jne    loc_1_527F
        cmp    dword ptr [eax + 0xe0], -1
        jne    loc_1_5297
loc_1_527F:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_526D
loc_1_5287:
        xor    esi, esi
        xchg   dword ptr [0], esi            ; -> obj1:0x00B0
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        jmp    loc_1_52B8
loc_1_5297:
        cmp    dword ptr [0], 0              ; -> obj1:0x00B0
        jne    loc_1_52B8
        mov    edx, esi
        mov    eax, 0x64
        mov    esi, 0                        ; -> loc_1_52B9
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0], esi            ; -> obj1:0x00B0
loc_1_52B8:
        ret    
loc_1_52B9:
        mov    dword ptr [0], 0              ; -> obj1:0x00B0
        mov    esi, edx
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_530F
loc_1_52CD:
        test   byte ptr [eax + 0x105], 0x20
        jne    loc_1_5307
        cmp    dword ptr [eax + 0xe0], -1
        je     loc_1_5307
        cmp    dword ptr [eax + 0x4c], 0
        je     loc_1_5307
        test   byte ptr [eax + 0xcd], 1
        jne    loc_1_5307
        xor    byte ptr [eax + 0xcd], 0x80
        js     loc_1_5307
        push   eax
        push   esi
        mov    esi, eax
        mov    eax, 1
        call   sub_1_4BED
        pop    esi
        pop    eax
loc_1_5307:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_52CD
loc_1_530F:
        mov    edx, esi
        mov    eax, 0x64
        mov    esi, 0                        ; -> loc_1_52B9
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0], esi            ; -> obj1:0x00B0
        ret    
sub_1_5328:
        shl    ebx, 2
        call   sub_1_0210
        sti    
        jb     loc_1_5337
        mov    eax, dword ptr [esi + 0x34]
loc_1_5337:
        ret    
loc_1_5338:
        or     byte ptr [esi + 0x108], 8
loc_1_533F:
        clc    
        ret    
loc_1_5341:
        and    byte ptr [esi + 0x108], 0xf7
        clc    
        ret    
loc_1_534A:
        mov    ax, word ptr [esi + 0x11e]
        push   eax
        mov    al, byte ptr [esi + 0x10e]
        or     al, 2
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 0x11e]
        je     loc_1_5376
        pushal 
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popal  
loc_1_5376:
        clc    
        ret    
loc_1_5378:
        mov    ax, word ptr [esi + 0x11e]
        push   eax
        mov    al, byte ptr [esi + 0x10e]
        and    al, 0xfd
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 0x11e]
        je     loc_1_53A4
        pushal 
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popal  
loc_1_53A4:
        clc    
        ret    
loc_1_53A6:
        mov    ax, word ptr [esi + 0x11e]
        push   eax
        mov    al, byte ptr [esi + 0x10e]
        or     al, 1
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 0x11e]
        je     loc_1_53D2
        pushal 
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popal  
loc_1_53D2:
        clc    
        ret    
loc_1_53D4:
        mov    ax, word ptr [esi + 0x11e]
        push   eax
        mov    al, byte ptr [esi + 0x10e]
        and    al, 0xfe
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 0x11e]
        je     loc_1_5400
        pushal 
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popal  
loc_1_5400:
        clc    
        ret    
loc_1_5402:
        push   esi
        call   sub_1_3430
        add    esp, 4
        clc    
        ret    
loc_1_540D:
        mov    ecx, 0xff40
        jmp    loc_1_5419
loc_1_5414:
        mov    ecx, 0xffffbf00
loc_1_5419:
        and    byte ptr [esi + 0x108], ch
        or     byte ptr [esi + 0x108], cl
        mov    al, byte ptr [esi + 0x10f]
        and    al, ch
        or     al, cl
        mov    byte ptr [esi + 0x10f], al
        or     word ptr [esi + 0x11e], 0x40
        pushal 
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popal  
        clc    
        ret    
loc_1_544C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, 0xc
        lea    edi, [esi + 0x4c]
        or     bl, bl
        jne    loc_1_5469
        add    edi, ecx
        mov    edx, dword ptr [esi + 0x58]
loc_1_5469:
        shr    ecx, 2
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        or     bl, bl
        je     loc_1_5488
        mov    ecx, eax
        call   sub_1_4D78
        sti    
        push   esi
        call   sub_1_336C
        add    esp, 4
        jmp    loc_1_54A7
loc_1_5488:
        and    byte ptr [esi + 0x104], 0xbf
        sti    
        or     edx, edx
        je     loc_1_54A7
        cmp    dword ptr [esi + 0xe4], 0
        je     loc_1_54A7
        mov    eax, 2
        call   sub_1_4BED
loc_1_54A7:
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_54B2:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_1_4D2C
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        dec    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_54CE:
        enter  4, 0
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x120], 1
        jne    loc_1_5503
        cld    
        push   esi
        lea    edi, [esi + 0x94]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 0xc
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    dword ptr [esi + 0x94], 0x30
        jmp    loc_1_5620
loc_1_5503:
        mov    bx, word ptr [esi + 0x11e]
        mov    word ptr [ebp - 4], bx
        mov    ebx, dword ptr [ebp + 0xc]
        test   byte ptr [ebp + 0x10], 1
        je     loc_1_551E
        call   sub_1_4EDE
        jecxz  loc_1_552B
loc_1_551E:
        test   byte ptr [ebp + 0x10], 0x70
        je     loc_1_5530
        call   sub_1_4F1F
        jae    loc_1_5530
loc_1_552B:
        jmp    loc_1_5622
loc_1_5530:
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_1_4DC8
        lea    ebx, [esi + 0x94]
        test   byte ptr [ebp + 0x10], 0x70
        je     loc_1_555D
        call   sub_1_4F1F
        mov    byte ptr [esi + 0x10f], al
        mov    eax, edi
        and    byte ptr [esi + 0x9c], ah
        mov    byte ptr [esi + 0x107], al
loc_1_555D:
        mov    al, 0xb
        test   byte ptr [ebp + 0x10], 2
        je     loc_1_55A3
        mov    al, byte ptr [esi + 0x9c]
        and    al, 0x82
        rol    al, 1
        shr    al, 1
        adc    al, 0
        mov    ah, al
        xor    al, 0xb
        mov    ecx, eax
        mov    al, byte ptr [esi + 0x9d]
        rol    al, 1
        rol    al, 1
        rol    al, 1
        and    al, 3
        not    ah
        and    al, ah
        mov    byte ptr [esi + 0x109], al
        mov    al, byte ptr [esi + 0x9c]
        shl    al, 1
        and    al, 0x30
        mov    byte ptr [esi + 0x10a], al
        mov    eax, ecx
loc_1_55A3:
        call   sub_1_5036
        test   byte ptr [esi + 0x9d], 0x80
        jne    loc_1_55E7
        mov    al, byte ptr [esi + 0x10d]
        mov    ah, byte ptr [esi + 0x10a]
        and    al, ah
        cmp    al, ah
        je     loc_1_55E7
        cmp    ah, 0x30
        jne    loc_1_55E0
        test   byte ptr [esi + 0x104], 1
        jne    loc_1_55E0
        cmp    al, 0x10
        jne    loc_1_55E0
        and    ah, 0xdf
        mov    byte ptr [esi + 0x10a], ah
        jmp    loc_1_55E7
loc_1_55E0:
        or     byte ptr [esi + 0x108], 0xa0
loc_1_55E7:
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, dword ptr [esi + 0xa4]
        mov    dword ptr [esi + 0xe8], eax
        cmp    dword ptr [esi + 0x110], 5
        je     loc_1_5606
        or     byte ptr [esi + 0x108], 8
loc_1_5606:
        mov    bx, word ptr [esi + 0x11e]
        cmp    bx, word ptr [ebp - 4]
        je     loc_1_5620
        pushal 
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popal  
loc_1_5620:
        xor    eax, eax
loc_1_5622:
        mov    dword ptr [esi + 0xc], eax
        or     eax, eax
        jne    loc_1_562C
        dec    eax
        jmp    loc_1_562E
loc_1_562C:
        xor    eax, eax
loc_1_562E:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_5633:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        lea    edi, [edx + 0x4c]
        mov    ecx, 6
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        cli    
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    edi, edx
        and    byte ptr [edi + 0x105], 0x7f
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        jne    loc_1_5683
        test   byte ptr [edi + 0x105], 8
        je     loc_1_56F0
loc_1_566A:
        and    byte ptr [edi + 0x105], 0xf7
        push   0
        push   dword ptr [edi + 0x3c]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_56F0
loc_1_5683:
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        jne    loc_1_56C9
        test   byte ptr [edi + 0x105], 8
        je     loc_1_56AA
        push   1
        push   dword ptr [ebp + 0x10]
        push   dword ptr [edi + 0x3c]
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_566A
        jne    loc_1_56E9
loc_1_56AA:
        push   1
        push   dword ptr [ebp + 0x10]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_57A2
        or     byte ptr [edi + 0x105], 8
        jmp    loc_1_56E9
loc_1_56C9:
        test   byte ptr [edi + 0x105], 8
        je     loc_1_56E9
        push   eax
        push   0
        push   dword ptr [edi + 0x3c]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    eax
        and    byte ptr [edi + 0x105], 0xf7
loc_1_56E9:
        or     byte ptr [edi + 0x105], 0x80
loc_1_56F0:
        mov    dword ptr [edi + 0x3c], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edi + 0x40], eax
        sub    eax, dword ptr [edi + 0xa4]
        mov    dword ptr [edi + 0xe8], eax
        and    byte ptr [edi + 0x105], 0xef
        mov    eax, dword ptr [ebp + 0x18]
        or     eax, eax
        jne    loc_1_5739
        test   byte ptr [edi + 0x105], 4
        je     loc_1_57A2
loc_1_5720:
        and    byte ptr [edi + 0x105], 0xfb
        push   0
        push   dword ptr [edi + 0x44]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_57A2
loc_1_5739:
        mov    eax, dword ptr [ebp + 0x14]
        or     eax, eax
        jne    loc_1_577B
        test   byte ptr [edi + 0x105], 4
        je     loc_1_5760
        push   1
        push   dword ptr [ebp + 0x18]
        push   dword ptr [edi + 0x44]
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_5720
        jne    loc_1_579B
loc_1_5760:
        push   1
        push   dword ptr [ebp + 0x18]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_57A2
        or     byte ptr [edi + 0x105], 4
        jmp    loc_1_579B
loc_1_577B:
        test   byte ptr [edi + 0x105], 4
        je     loc_1_579B
        push   eax
        push   0
        push   dword ptr [edi + 0x44]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    eax
        and    byte ptr [edi + 0x105], 0xfb
loc_1_579B:
        or     byte ptr [edi + 0x105], 0x10
loc_1_57A2:
        mov    dword ptr [edi + 0x44], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edi + 0x48], eax
        or     al, 1
        sti    
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_57B3:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0xffffffe2
        cmp    dword ptr [esi + 0x110], 5
        je     loc_1_57D1
        or     byte ptr [esi + 0x451], 8
        jmp    loc_1_5807
loc_1_57D1:
        test   byte ptr [esi + 0x121], 7
        je     loc_1_57E3
        or     byte ptr [esi + 0x121], 8
        jmp    loc_1_5807
loc_1_57E3:
        and    byte ptr [esi + 0x121], 0xf7
        test   byte ptr [esi + 0x104], 0x40
        jne    loc_1_5807
        mov    ah, byte ptr [ebp + 0xc]
        cli    
        or     byte ptr [esi + 0x104], 0x40
        mov    byte ptr [esi + 0x106], ah
        sti    
        xor    eax, eax
loc_1_5807:
        mov    dword ptr [esi + 0xc], eax
        or     eax, eax
        mov    eax, 0xffffffff
        je     loc_1_5815
        xor    eax, eax
loc_1_5815:
        pop    esi
        leave  
        ret    
loc_1_5818:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_1_4D2C
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        xchg   dword ptr [esi + 0x34], eax
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_583D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 0xc], 0xffffffec
        cmp    bl, 0xc8
        jne    loc_1_586E
        mov    dword ptr [esi + 0xc], eax
        cmp    dword ptr [esi + 0x4c], eax
        je     loc_1_58D1
        mov    eax, dword ptr [esi + 0x3c]
        add    eax, dword ptr [esi + 0x50]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [eax], cl
        jmp    loc_1_58CF
loc_1_586E:
        cmp    bl, 0x15
        jne    loc_1_587C
        and    byte ptr [esi + 0x105], 0xdf
        jmp    loc_1_58CF
loc_1_587C:
        cmp    bl, 0x14
        jne    loc_1_588A
        or     byte ptr [esi + 0x105], 0x40
        jmp    loc_1_58CF
loc_1_588A:
        cmp    bl, 0x10
        jne    loc_1_5898
        or     byte ptr [esi + 0x105], 0x20
        jmp    loc_1_58CF
loc_1_5898:
        cmp    bl, 0x13
        jne    loc_1_58A8
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x100], eax
        jmp    loc_1_58CF
loc_1_58A8:
        cmp    bl, 0x6e
        jne    loc_1_58BB
        mov    dword ptr [esi + 0xc], eax
        push   esi
        call   sub_1_325F
        add    esp, 4
        jmp    loc_1_58C8
loc_1_58BB:
        cmp    bl, 0xd
        ja     loc_1_58D1
        mov    dword ptr [esi + 0xc], eax
        call   sub_1_5328
loc_1_58C8:
        mov    ecx, dword ptr [ebp + 0x14]
        jecxz  loc_1_58CF
        mov    dword ptr [ecx], eax
loc_1_58CF:
        or     al, 1
loc_1_58D1:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_58D6:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0xc], 0
        mov    edx, dword ptr [edx + 0xf0]
        mov    eax, dword ptr [ebp + 0xc]
        not    eax
        cli    
        mov    ecx, dword ptr [edx]
        and    eax, ecx
        mov    dword ptr [edx], eax
        sti    
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], ecx
        or     al, 1
        leave  
        ret    
loc_1_58FF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x110], 5
        je     loc_1_5947
        or     byte ptr [esi + 0x451], 8
        or     dword ptr [esi + 0x34], 0x100
        xor    ecx, ecx
        cmp    dword ptr [esi + 0xd8], 0
        je     loc_1_598D
        mov    eax, dword ptr [esi + 0x58]
        cmp    eax, dword ptr [esi + 0xe4]
        jae    loc_1_5945
        mov    eax, 2
        call   sub_1_4BED
loc_1_5945:
        jmp    loc_1_598D
loc_1_5947:
        and    byte ptr [esi + 0x121], 0xf7
        test   byte ptr [esi + 0x105], 0x10
        jne    loc_1_599C
        xor    eax, eax
        cmp    dword ptr [esi + 0x58], eax
        je     loc_1_5968
        int3   
        or     dword ptr [esi + 0x34], 0x100
        jmp    loc_1_59A1
loc_1_5968:
        mov    dword ptr [esi + 0x44], edi
        mov    dword ptr [esi + 0x48], ecx
        mov    dword ptr [esi + 0x58], ecx
        mov    dword ptr [esi + 0x60], eax
        mov    dword ptr [esi + 0x5c], eax
        and    dword ptr [esi + 0x34], 0xfffffeff
        cmp    ecx, dword ptr [esi + 0xe4]
        jb     loc_1_598D
        and    byte ptr [esi + 0xcd], 0xfd
loc_1_598D:
        push   ecx
        push   3
        push   esi
        call   sub_1_35B5
        add    esp, 8
        pop    eax
        jmp    loc_1_59A1
loc_1_599C:
        call   sub_1_4CA8
loc_1_59A1:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_59B1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        test   byte ptr [esi + 0x105], 0x40
        jne    loc_1_59D3
        or     eax, dword ptr [esi + 0x34]
        je     loc_1_59D3
        mov    ecx, eax
        xor    eax, eax
        jmp    loc_1_5A7C
loc_1_59D3:
        mov    ecx, dword ptr [ebp + 0x10]
        or     ecx, ecx
        je     loc_1_5A7A
        test   byte ptr [esi + 0x105], 0x80
        je     loc_1_5A7A
        mov    eax, dword ptr [esi + 0x4c]
        cmp    ecx, eax
        jbe    loc_1_59F4
        mov    ecx, eax
loc_1_59F4:
        mov    eax, dword ptr [esi + 0x4c]
        or     eax, eax
        je     loc_1_5A5C
        cmp    ecx, eax
        jbe    loc_1_5A01
        mov    ecx, eax
loc_1_5A01:
        push   ecx
        mov    edx, dword ptr [esi + 0x40]
        mov    eax, dword ptr [esi + 0x50]
        sub    edx, eax
        cmp    edx, ecx
        jbe    loc_1_5A10
        mov    edx, ecx
loc_1_5A10:
        xchg   edx, ecx
        sub    edx, ecx
        mov    edi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [esi + 0x3c]
        mov    esi, ebx
        add    esi, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        jecxz  loc_1_5A2A
        mov    esi, ebx
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_5A2A:
        sub    esi, ebx
        mov    ebx, esi
        pop    esi
        pop    eax
        cli    
        mov    dword ptr [esi + 0x50], ebx
        sub    dword ptr [esi + 0x4c], eax
        sti    
        mov    ecx, dword ptr [esi + 0x4c]
        cmp    ecx, dword ptr [esi + 0xe0]
        jae    loc_1_5A4A
        and    byte ptr [esi + 0xcd], 0xfe
loc_1_5A4A:
        push   eax
        call   sub_1_4D78
        push   esi
        call   sub_1_343B
        add    esp, 4
        pop    eax
        jmp    loc_1_5A7A
loc_1_5A5C:
        test   byte ptr [esi + 0x9c], 1
        jne    loc_1_5A7A
        test   byte ptr [esi + 0x104], 0x20
        je     loc_1_5A7A
        mov    cl, byte ptr [esi + 0xb0]
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [edi], cl
        inc    eax
loc_1_5A7A:
        xor    ecx, ecx
loc_1_5A7C:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        mov    dword ptr [esi + 0xc], ecx
        mov    eax, 0xffffffff
        jecxz  loc_1_5A8D
        xor    eax, eax
loc_1_5A8D:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_5A92:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x105], 0x80
        jne    loc_1_5AA8
loc_1_5AA4:
        xor    eax, eax
        jmp    loc_1_5B25
loc_1_5AA8:
        mov    ecx, dword ptr [ebp + 0x10]
        or     ecx, ecx
        je     loc_1_5AA4
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, dword ptr [esi + 0x4c]
        jle    loc_1_5AA4
        test   byte ptr [esi + 0x105], 1
        je     loc_1_5AC4
        mov    eax, ecx
        jmp    loc_1_5B25
loc_1_5AC4:
        cmp    ecx, eax
        jbe    loc_1_5ACA
        mov    ecx, eax
loc_1_5ACA:
        push   ecx
        mov    eax, dword ptr [esi + 0x54]
        xor    edx, edx
        add    eax, ecx
        sub    eax, dword ptr [esi + 0x40]
        jbe    loc_1_5ADB
        mov    edx, eax
        sub    ecx, edx
loc_1_5ADB:
        mov    edi, dword ptr [ebp + 0xc]
        push   esi
        mov    eax, dword ptr [esi + 0x3c]
        mov    esi, dword ptr [esi + 0x54]
        add    esi, eax
        xchg   edi, esi
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        jecxz  loc_1_5AF4
        mov    edi, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_5AF4:
        pop    esi
        sub    edi, eax
        cmp    edi, dword ptr [esi + 0x40]
        jb     loc_1_5AFE
        xor    edi, edi
loc_1_5AFE:
        mov    dword ptr [esi + 0x54], edi
        pop    ecx
        add    dword ptr [esi + 0x4c], ecx
        test   byte ptr [esi + 0xcd], 1
        jne    loc_1_5B23
        mov    eax, dword ptr [esi + 0x4c]
        cmp    eax, dword ptr [esi + 0xe0]
        jb     loc_1_5B23
        mov    eax, 1
        call   sub_1_4BED
loc_1_5B23:
        mov    eax, ecx
loc_1_5B25:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_5B35:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x105], 0x80
        jne    loc_1_5B53
loc_1_5B47:
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        jmp    loc_1_5D15
loc_1_5B53:
        mov    ecx, dword ptr [ebp + 0x10]
        or     ecx, ecx
        je     loc_1_5B47
        test   byte ptr [esi + 0x108], 0x12
        jne    loc_1_5B47
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, dword ptr [esi + 0x4c]
        jle    loc_1_5B47
        cmp    ecx, eax
        jbe    loc_1_5B71
        mov    ecx, eax
loc_1_5B71:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ebx, dword ptr [esi + 0x100]
        mov    dword ptr [ebx], eax
        and    byte ptr [esi + 0xcd], 0x7f
        mov    ebx, dword ptr [esi + 0xf0]
        mov    eax, dword ptr [ebx]
        push   eax
        or     byte ptr [ebx], 1
        push   ecx
        mov    edx, dword ptr [ebp + 0xc]
loc_1_5B96:
        or     ecx, ecx
        je     loc_1_5CC5
        test   byte ptr [esi + 0x108], 0x12
        jne    loc_1_5CC5
        mov    al, byte ptr [edx]
        inc    edx
        dec    ecx
        test   byte ptr [esi + 0x104], 0x20
        je     loc_1_5BBE
        or     dword ptr [esi + 0x34], 1
        jmp    loc_1_5B96
loc_1_5BBE:
        or     al, al
        jne    loc_1_5BCB
        test   byte ptr [esi + 0x9d], 8
        jne    loc_1_5B96
loc_1_5BCB:
        cmp    al, byte ptr [esi + 0xb0]
        jne    loc_1_5BE5
        test   byte ptr [esi + 0x9c], 1
        jne    loc_1_5BE5
        or     byte ptr [esi + 0x104], 0x20
        jmp    loc_1_5B96
loc_1_5BE5:
        push   edx
        push   ecx
        mov    ecx, dword ptr [esi + 0x4c]
        test   byte ptr [esi + 0x9d], 1
        je     loc_1_5C49
        cmp    al, byte ptr [esi + 0xae]
        jne    loc_1_5C28
        or     byte ptr [esi + 0x108], 8
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_1_5C21
        cmp    ecx, dword ptr [esi + 0xa0]
        ja     loc_1_5C21
        and    byte ptr [esi + 0x108], 0xf7
        and    byte ptr [esi + 0x108], 0xeb
loc_1_5C21:
        pop    ecx
        pop    edx
        jmp    loc_1_5B96
loc_1_5C28:
        cmp    al, byte ptr [esi + 0xad]
        jne    loc_1_5C49
        and    byte ptr [esi + 0x108], 0xf7
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_1_5C47
        and    byte ptr [esi + 0x108], 0xf7
loc_1_5C47:
        jmp    loc_1_5C21
loc_1_5C49:
        cmp    al, byte ptr [esi + 0xb1]
        jne    loc_1_5C5A
        mov    ebx, dword ptr [esi + 0xf0]
        or     byte ptr [ebx], 2
loc_1_5C5A:
        test   byte ptr [esi + 0x105], 1
        jne    loc_1_5C21
        mov    edi, dword ptr [esi + 0x3c]
        mov    ebx, dword ptr [esi + 0x54]
        mov    byte ptr [edi + ebx], al
        inc    ebx
        cmp    ebx, dword ptr [esi + 0x40]
        jb     loc_1_5C74
        xor    ebx, ebx
loc_1_5C74:
        mov    dword ptr [esi + 0x54], ebx
        inc    ecx
        mov    dword ptr [esi + 0x4c], ecx
        cmp    ecx, dword ptr [esi + 0xe8]
        jb     loc_1_5C21
        test   byte ptr [esi + 0x108], 0x12
        jne    loc_1_5C21
        mov    ah, byte ptr [esi + 0x109]
        or     ah, ah
        je     loc_1_5CAC
        mov    al, byte ptr [esi + 0x10e]
        not    ah
        and    al, ah
        or     byte ptr [esi + 0x108], 2
        call   sub_1_5036
loc_1_5CAC:
        test   byte ptr [esi + 0x9d], 2
        je     loc_1_5C21
        or     byte ptr [esi + 0x108], 0x10
        jmp    loc_1_5C21
loc_1_5CC5:
        pop    eax
        sub    eax, ecx
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        test   byte ptr [esi + 0xcd], 1
        jne    loc_1_5CEB
        mov    eax, dword ptr [esi + 0x4c]
        cmp    eax, dword ptr [esi + 0xe0]
        jb     loc_1_5CEB
        mov    eax, 1
        call   sub_1_4BED
loc_1_5CEB:
        mov    ebx, dword ptr [esi + 0xf0]
        mov    eax, dword ptr [esi + 0xec]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0xcd], 0x40
        je     loc_1_5D15
        not    ebx
        and    eax, ebx
        je     loc_1_5D15
        mov    eax, 4
        call   sub_1_4BED
loc_1_5D15:
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_5D20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        xor    eax, eax
        or     ecx, ecx
        je     loc_1_5D6A
        mov    eax, dword ptr [esi + 0x58]
        or     eax, eax
        je     loc_1_5D6A
        cmp    ecx, eax
        jbe    loc_1_5D3F
        mov    ecx, eax
loc_1_5D3F:
        push   ecx
        mov    edx, dword ptr [esi + 0x48]
        mov    eax, dword ptr [esi + 0x5c]
        sub    edx, eax
        cmp    edx, ecx
        jbe    loc_1_5D4E
        mov    edx, ecx
loc_1_5D4E:
        xchg   edx, ecx
        sub    edx, ecx
        push   esi
        mov    ebx, dword ptr [esi + 0x44]
        mov    esi, ebx
        add    esi, eax
        mov    edi, dword ptr [ebp + 0xc]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        jecxz  loc_1_5D68
        mov    esi, ebx
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_5D68:
        pop    esi
        pop    eax
loc_1_5D6A:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_5D7A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        xor    eax, eax
        or     ecx, ecx
        je     loc_1_5DFB
        mov    eax, dword ptr [esi + 0x58]
        or     eax, eax
        jne    loc_1_5D9E
        mov    ebx, dword ptr [esi + 0xf0]
        or     byte ptr [ebx], 4
        jmp    loc_1_5DFB
loc_1_5D9E:
        cmp    ecx, eax
        jbe    loc_1_5DA4
        mov    ecx, eax
loc_1_5DA4:
        push   ecx
        test   byte ptr [esi + 0x9c], 0x20
        je     loc_1_5DD1
        mov    eax, dword ptr [esi + 0xf8]
        or     eax, eax
        jne    loc_1_5DBF
        or     byte ptr [esi + 0x108], 8
loc_1_5DBF:
        add    eax, ecx
        cmp    eax, dword ptr [esi + 0xa0]
        jb     loc_1_5DCB
        xor    eax, eax
loc_1_5DCB:
        mov    dword ptr [esi + 0xf8], eax
loc_1_5DD1:
        mov    edx, dword ptr [esi + 0x48]
        mov    eax, dword ptr [esi + 0x5c]
        sub    edx, eax
        cmp    edx, ecx
        jbe    loc_1_5DDF
        mov    edx, ecx
loc_1_5DDF:
        xchg   edx, ecx
        sub    edx, ecx
        push   esi
        mov    ebx, dword ptr [esi + 0x44]
        mov    esi, ebx
        add    esi, eax
        mov    edi, dword ptr [ebp + 0xc]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        jecxz  loc_1_5DF9
        mov    esi, ebx
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_5DF9:
        pop    esi
        pop    eax
loc_1_5DFB:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_5E0B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        or     ecx, ecx
        je     loc_1_5E4F
        cli    
        mov    ebx, dword ptr [esi + 0x5c]
        add    ebx, ecx
        mov    edx, dword ptr [esi + 0x48]
        cmp    ebx, edx
        jb     loc_1_5E2A
        sub    ebx, edx
loc_1_5E2A:
        mov    dword ptr [esi + 0x5c], ebx
        sub    dword ptr [esi + 0x58], ecx
        sti    
        test   byte ptr [esi + 0xcd], 2
        jne    loc_1_5E4F
        mov    eax, dword ptr [esi + 0x58]
        cmp    eax, dword ptr [esi + 0xe4]
        jae    loc_1_5E4F
        mov    eax, 2
        call   sub_1_4BED
loc_1_5E4F:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_5E54:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        or     ecx, ecx
        je     loc_1_5ED2
        cli    
        mov    ebx, dword ptr [esi + 0x5c]
        add    ebx, ecx
        mov    edx, dword ptr [esi + 0x48]
        cmp    ebx, edx
        jb     loc_1_5E73
        sub    ebx, edx
loc_1_5E73:
        mov    dword ptr [esi + 0x5c], ebx
        sub    dword ptr [esi + 0x58], ecx
        sti    
        test   byte ptr [esi + 0x9c], 0x40
        je     loc_1_5EA8
        add    ecx, dword ptr [esi + 0xf8]
        mov    dword ptr [esi + 0xf8], ecx
        cmp    ecx, dword ptr [esi + 0xa0]
        jbe    loc_1_5EA8
        mov    dword ptr [esi + 0xf8], 0
        or     byte ptr [esi + 0x108], 8
loc_1_5EA8:
        mov    ebx, dword ptr [esi + 0xf0]
        or     dword ptr [ebx], 0x4000
        test   byte ptr [esi + 0xcd], 2
        jne    loc_1_5ED2
        mov    eax, dword ptr [esi + 0x58]
        cmp    eax, dword ptr [esi + 0xe4]
        jae    loc_1_5ED2
        mov    eax, 2
        call   sub_1_4BED
loc_1_5ED2:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
        db  0CCh
sub_1_5ED8:
        VxDCall  Dev_04A2.Service_0000h          ; cd200000a204
        jae    loc_1_5EE2
        xor    eax, eax
loc_1_5EE2:
        ret    
sub_1_5EE3:
        VxDCall  Dev_04A2.Service_8002h          ; cd200280a204
sub_1_5EE9:
        VxDCall  Dev_04A2.Service_801Ch          ; cd201c80a204
sub_1_5EEF:
        VxDCall  Dev_04A2.Service_801Dh          ; cd201d80a204
        VxDCall  Dev_04A2.Service_801Eh          ; cd201e80a204
sub_1_5EFB:
        VxDCall  Dev_04A2.Service_8004h          ; cd200480a204
sub_1_5F01:
        VxDCall  Dev_04A2.Service_8005h          ; cd200580a204
sub_1_5F07:
        VxDCall  Dev_04A2.Service_8006h          ; cd200680a204
sub_1_5F0D:
        VxDCall  Dev_04A2.Service_8007h          ; cd200780a204
sub_1_5F13:
        VxDCall  Dev_04A2.Service_8008h          ; cd200880a204
sub_1_5F19:
        VxDCall  Dev_04A2.Service_8009h          ; cd200980a204
sub_1_5F1F:
        VxDCall  Dev_04A2.Service_800Ah          ; cd200a80a204
        VxDCall  Dev_04A2.Service_800Ch          ; cd200c80a204
sub_1_5F2B:
        VxDCall  Dev_04A2.Service_800Eh          ; cd200e80a204
sub_1_5F31:
        VxDCall  Dev_04A2.Service_8019h          ; cd201980a204
sub_1_5F37:
        VxDCall  Dev_04A2.Service_801Ah          ; cd201a80a204
sub_1_5F3D:
        VxDCall  Dev_04A2.Service_8018h          ; cd201880a204
sub_1_5F43:
        VxDCall  Dev_04A2.Service_8022h          ; cd202280a204
        int3   
        int3   
        int3   
sub_1_5F4C:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_5F52:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_5F5F
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
loc_1_5F5F:
        ret    
sub_1_5F60:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
sub_1_5F70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_5FD1
loc_1_5FD1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_6053:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_6082
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jne    loc_1_6075
        mov    eax, 1
        jmp    loc_1_6089
loc_1_6075:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_6053
loc_1_6082:
        xor    eax, eax
        jmp    loc_1_6089
loc_1_6089:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    loc_1_60BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_60C6
loc_1_60BF:
        xor    eax, eax
        jmp    loc_1_611F
loc_1_60C6:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_60DB
loc_1_60D3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_60DB:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_6115
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jne    loc_1_6109
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_6110
loc_1_6109:
        xor    eax, eax
        jmp    loc_1_611F
loc_1_6110:
        jmp    loc_1_60D3
loc_1_6115:
        mov    eax, 1
        jmp    loc_1_611F
loc_1_611F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6124:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_1_6134:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_1_6144:
        VMMCall  VMM._SignalID                   ; cd2027810100
        int3   
        int3   
sub_1_614C:
        VMMCall  VMM._BlockOnID                  ; cd20fa800100
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x66b  R-X
loc_2_0000:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_2_000A
        int3   
        ret    
loc_2_000A:
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 0x24]
        or     edi, edi
        je     loc_2_0118
        mov    esi, dword ptr [0]            ; -> obj1:0x00B4
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_2_003A
loc_2_0027:
        call   sub_2_016E
        je     loc_2_0118
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_2_0027
loc_2_003A:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_2_0118
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    ebx, eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 0x115
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [ebx], 0x94
        mov    word ptr [ebx + 2], 0x10a
        mov    dword ptr [ebx + 4], 0        ; -> obj1:0x0050
        mov    dword ptr [ebx + 8], 0x17
        mov    byte ptr [ebx + 0x104], 0x96
        lea    esi, [0]                      ; -> obj3:0x0009
        mov    dword ptr [esi], 0
        mov    dword ptr [0], 4              ; -> obj3:0x000D
        push   0
        push   0                             ; -> obj3:0x000D
        push   esi
        push   3
        push   0                             ; -> obj3:0x0000
        push   0
        push   dword ptr [esp + 0x2c]
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_00BF
        cmp    byte ptr [esi + 2], 0
        je     loc_2_00BF
        or     byte ptr [ebx + 0x104], 1
loc_2_00BF:
        mov    byte ptr [ebx + 0x105], 0x20
        mov    dword ptr [ebx + 0x110], 9
        mov    edi, dword ptr [esp + 0x24]
        mov    esi, edi
        xor    eax, eax
        xor    ecx, ecx
        dec    ecx
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        dec    ecx
        push   ecx
        push   1
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    edi, eax
        pop    ecx
        mov    dword ptr [ebx + 0xf4], eax
        or     eax, eax
        je     loc_2_00FE
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_2_0105
loc_2_00FE:
        call   sub_2_011C
        jmp    loc_2_0118
loc_2_0105:
        push   eax
        push   0                             ; -> loc_2_01A8
        push   dword ptr [esp + 0x20]
        VxDCall  BIOS.Service_0019h              ; cd2019002b00
        add    esp, 0xc
loc_2_0118:
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_2_011C:
        push   esi
        cmp    dword ptr [ebx + 0xf4], 0
        je     loc_2_0137
        push   0
        push   dword ptr [ebx + 0xf4]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_2_0137:
        mov    esi, dword ptr [0]            ; -> obj1:0x00B4
        mov    eax, ebx
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        jb     loc_2_014D
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_2_014D:
        pop    esi
        ret    
        db  0CCh
sub_2_0150:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        or     edi, edi
        je     loc_2_0168
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        call   sub_2_016E
        je     loc_2_016A
loc_2_0168:
        xor    eax, eax
loc_2_016A:
        pop    edi
        pop    esi
        leave  
        ret    
sub_2_016E:
        push   edi
        mov    edx, dword ptr [eax + 0xf4]
        or     edx, edx
        je     loc_2_0195
        xor    ecx, ecx
loc_2_017B:
        mov    cl, byte ptr [edx]
        mov    ch, byte ptr [edi]
        or     ecx, ecx
        je     loc_2_0197
        call   sub_2_0199
        xchg   ch, cl
        call   sub_2_0199
        inc    edx
        inc    edi
        cmp    cl, ch
        je     loc_2_017B
loc_2_0195:
        or     esi, esi
loc_2_0197:
        pop    edi
        ret    
sub_2_0199:
        cmp    cl, 0x41
        jb     loc_2_01A6
        cmp    cl, 0x5a
        ja     loc_2_01A6
        or     cl, 0x20
loc_2_01A6:
        ret    
        db  0CCh
loc_2_01A8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00B4
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_2_01BD:
        je     loc_2_01CE
        call   sub_2_016E
        je     loc_2_01D8
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jmp    loc_2_01BD
loc_2_01CE:
        mov    eax, 0xfffffff6
        jmp    loc_2_0332
loc_2_01D8:
        mov    esi, eax
        mov    eax, 0xfffffffe
        cmp    dword ptr [esi + 0x64], 0x534d5446
        je     loc_2_0332
        cmp    dword ptr [esi + 0x110], 9
        jne    loc_2_0332
        xor    ebx, ebx
        mov    dword ptr [esi + 0x3c], ebx
        mov    dword ptr [esi + 0x40], ebx
        mov    dword ptr [esi + 0x44], ebx
        mov    dword ptr [esi + 0x48], ebx
        mov    dword ptr [esi + 0x4c], ebx
        mov    dword ptr [esi + 0x50], ebx
        mov    dword ptr [esi + 0x54], ebx
        mov    dword ptr [esi + 0x58], ebx
        mov    dword ptr [esi + 0x5c], ebx
        mov    dword ptr [esi + 0x60], ebx
        mov    byte ptr [esi + 0x109], bl
        mov    byte ptr [esi + 0x10a], bl
        mov    byte ptr [esi + 0x105], 0x20
        mov    dword ptr [esi + 0xe4], ebx
        mov    dword ptr [esi + 0xcc], ebx
        mov    dword ptr [esi + 0xec], ebx
        mov    dword ptr [esi + 0xd8], ebx
        dec    ebx
        mov    dword ptr [esi + 0xe0], ebx
        mov    dword ptr [esi + 0xa0], ebx
        mov    dword ptr [esi + 0xa4], ebx
        mov    dword ptr [esi + 0xe8], 1
        mov    dword ptr [esi + 0x9c], 1
        lea    edi, [esi + 0x30]
        mov    dword ptr [esi + 0xf0], edi
        lea    edi, [esi + 0x38]
        mov    dword ptr [esi + 0xfc], edi
        mov    dword ptr [esi + 0x94], 0x30
        lea    edi, [esi + 0x88]
        mov    dword ptr [esi + 0x100], edi
        mov    word ptr [esi + 0xad], 0x1311
        xor    ah, ah
        mov    byte ptr [esi + 0x10e], ah
        mov    byte ptr [esi + 0x452], 0
        mov    byte ptr [esi + 0x10d], ah
        push   esi
        mov    esi, dword ptr [esi + 0xfc]
        mov    byte ptr [esi], ah
        pop    esi
        mov    byte ptr [esi + 0x108], 8
        mov    dword ptr [esi + 0x98], 0x2580
        mov    byte ptr [esi + 0xaa], 8
        mov    byte ptr [esi + 0xab], 0
        mov    byte ptr [esi + 0xac], 0
        mov    byte ptr [esi + 0x10f], 3
        or     word ptr [esi + 0x11e], 6
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        cmp    eax, 0x400
        jae    loc_2_0314
        push   0x7d0
        push   0
        push   0x7d0
        push   0
        push   esi
        call   sub_1_5633
loc_2_0311:
        add    esp, 0x14
loc_2_0314:
        mov    dword ptr [esi + 0x34], 0
        push   esi
        call   sub_1_2B52
loc_2_0321:
        add    esp, 4
        cmp    eax, 0
        je     loc_2_0330
        mov    eax, 0xfffffffb
        jmp    loc_2_0332
loc_2_0330:
        xor    eax, eax
loc_2_0332:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], eax
        or     eax, eax
        jne    loc_2_0342
        mov    dword ptr [esi + 0xc], eax
        mov    eax, esi
        jmp    loc_2_0344
loc_2_0342:
        xor    eax, eax
loc_2_0344:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_2_0349:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        xor    eax, eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xc], eax
        lea    esi, [esi + 0x94]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0xc
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_2_036E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 0x105], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x34], ecx
        mov    dword ptr [esi + 0x4c], ecx
        call   sub_1_4D78
loc_2_038B:
        push   esi
        call   sub_1_336C
loc_2_0391:
        add    esp, 4
        push   esi
        call   sub_1_2EF2
loc_2_039A:
        add    esp, 4
        cmp    eax, 0
        je     loc_2_03A7
        mov    eax, 0xfffffffe
loc_2_03A7:
        mov    dword ptr [esi + 0xc], eax
        test   byte ptr [esi + 0x105], 8
        je     loc_2_03C1
        push   0
        push   dword ptr [esi + 0x3c]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_2_03C1:
        test   byte ptr [esi + 0x105], 4
        je     loc_2_03D8
        push   0
        push   dword ptr [esi + 0x44]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_2_03D8:
        mov    byte ptr [esi + 0x105], 0x20
        mov    ebx, esi
        call   sub_1_525F
loc_2_03E6:
        call   sub_2_011C
        xor    eax, eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_2_03F3:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_2_040B
        mov    dword ptr [edx + 0xfc], ecx
        mov    al, byte ptr [edx + 0x10d]
        mov    byte ptr [ecx], al
loc_2_040B:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        ret    
loc_2_0412:
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [esp + 0x10]
        test   byte ptr [edx + 0x120], 2
        jne    loc_2_0429
        mov    esi, 0                        ; -> obj3:0x0044
        jmp    loc_2_042E
loc_2_0429:
        mov    esi, 0                        ; -> obj3:0x0084
loc_2_042E:
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, edi
        mov    ecx, 0x40
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [edx + 0x40]
        mov    dword ptr [eax + 0x30], ecx
        mov    ecx, dword ptr [edx + 0x48]
        mov    dword ptr [eax + 0x2c], ecx
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_2_0452:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xec], eax
        mov    ecx, dword ptr [ebp + 0x10]
        jecxz  loc_2_046C
        mov    dword ptr [edx + 0xf0], ecx
loc_2_046C:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        leave  
        ret    
loc_2_0474:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 0xc4], ecx
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [edx + 0xc8], eax
        and    byte ptr [edx + 0xcd], 0xbf
        jecxz  loc_2_04BE
        cli    
        or     byte ptr [edx + 0xcd], 0x40
        mov    ecx, dword ptr [edx + 0xf0]
        mov    eax, dword ptr [edx + 0xec]
        and    eax, dword ptr [ecx]
        sti    
        or     eax, eax
        je     loc_2_04BE
        push   esi
        mov    esi, edx
        mov    eax, 4
        call   sub_1_4BED
loc_2_04BD:
        pop    esi
loc_2_04BE:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        ret    
loc_2_04C5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0xd4], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, ecx
        inc    eax
        je     loc_2_04EF
        test   byte ptr [esi + 0x105], 0x80
        je     loc_2_04EF
        cmp    ecx, dword ptr [esi + 0x40]
        jbe    loc_2_04EF
        mov    ecx, dword ptr [esi + 0x40]
loc_2_04EF:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xd0], ebx
        or     ebx, ebx
        jne    loc_2_04FF
        or     ecx, 0xffffffff
loc_2_04FF:
        mov    dword ptr [esi + 0xe0], ecx
        cmp    dword ptr [esi + 0x4c], 0
        jne    loc_2_0517
        mov    ebx, dword ptr [esi + 0x100]
        mov    dword ptr [ebx], 0
loc_2_0517:
        mov    ebx, esi
        call   sub_1_525F
loc_2_051E:
        xor    eax, eax
        mov    dword ptr [ebx + 0xc], eax
        inc    eax
        pop    ebx
        pop    esi
        leave  
        ret    
loc_2_0528:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0xdc], ebx
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        je     loc_2_0551
        dec    edx
        test   byte ptr [esi + 0x105], 0x10
        je     loc_2_0551
        cmp    edx, dword ptr [esi + 0x48]
        jbe    loc_2_0551
        mov    edx, dword ptr [esi + 0x48]
loc_2_0551:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0xd8], ebx
        or     ebx, ebx
        jne    loc_2_0560
        xor    edx, edx
loc_2_0560:
        mov    dword ptr [esi + 0xe4], edx
        cmp    dword ptr [esi + 0x58], edx
        jae    loc_2_0572
        or     byte ptr [esi + 0xcd], 2
loc_2_0572:
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    esi
        leave  
        ret    
loc_2_057C:
        mov    edx, dword ptr [esp + 4]
        mov    al, byte ptr [edx + 0x10d]
        and    eax, 0xf0
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [edx], eax
        or     al, 1
        ret    
loc_2_0594:
        push   esi
        push   edi
        push   ebx
        xor    eax, eax
        mov    edx, dword ptr [esp + 0x18]
        push   0x34
        pop    ecx
        cmp    dword ptr [edx], ecx
        mov    dword ptr [edx], ecx
        jb     loc_2_05E8
        mov    edx, dword ptr [esp + 0x14]
        or     edx, edx
        je     loc_2_05E8
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [edx + 0x2c], eax
        mov    byte ptr [edx + 0x30], al
        mov    al, 0x34
        mov    dword ptr [edx], eax
        mov    esi, dword ptr [esp + 0x10]
        test   byte ptr [esi + 0x120], 2
        jne    loc_2_05CC
        mov    al, 1
        jmp    loc_2_05CE
loc_2_05CC:
        mov    al, 2
loc_2_05CE:
        mov    dword ptr [edx + 0x24], eax
        mov    word ptr [edx + 4], 0x100
        lea    eax, [esi + 0x94]
        lea    edx, [edx + 8]
        VxDCall  BIOS.Service_0020h              ; cd2020002b00
        xor    eax, eax
loc_2_05E8:
        mov    dword ptr [esi + 0xc], eax
        or     al, 1
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_2_05F1:
        sub    esp, 0x30
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 0x38]
        mov    dword ptr [esi + 0xc], 0xffffffeb
        cmp    dword ptr [esp + 0x40], 0x34
        jb     loc_2_0634
        lea    edx, [esp + 4]
        push   edx
        push   esi
        call   sub_2_0349
        add    esp, 8
        mov    eax, dword ptr [esp + 0x3c]
        lea    eax, [eax + 8]
        lea    edx, [esp + 4]
        VxDCall  BIOS.Service_001Fh              ; cd201f002b00
        push   -1
        push   edx
        push   esi
        call   sub_1_54CE
loc_2_0631:
        add    esp, 0xc
loc_2_0634:
        pop    esi
        add    esp, 0x30
        ret    
loc_2_0639:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        test   byte ptr [ecx + 0x120], 1
        jne    loc_2_065F
        test   byte ptr [ecx + 0x35], 8
        jne    loc_2_065F
        test   byte ptr [ecx + 0x35], 0x10
        je     loc_2_0667
        mov    dword ptr [eax], 0x1c
        jmp    loc_2_0667
loc_2_065F:
        mov    dword ptr [eax], 0x15
        jmp    loc_2_0667
loc_2_0667:
        or     al, 1
        pop    esi
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0xc4  RW-
        db  053h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h
        db  074h, 020h, 048h, 065h, 077h, 06Ch, 065h, 074h, 074h, 02Dh, 050h, 061h, 063h, 06Bh, 061h, 072h
        db  064h, 020h, 043h, 06Fh, 06Dh, 070h, 061h, 06Eh, 079h, 020h, 031h, 039h, 039h, 034h, 02Dh, 031h
        db  039h, 039h, 035h, 000h, 040h, 000h, 002h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 001h, 000h, 000h, 000h
        db  0FFh, 000h, 000h, 000h, 07Fh, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 010h, 00Fh, 000h, 007h, 01Fh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 040h, 000h, 001h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x6e  R-X
loc_4_0000:
        cmp    dword ptr [0], 0              ; -> obj1:0x00AC
        jne    loc_4_004C
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [0], ebx            ; -> obj1:0x00AC
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        jb     loc_4_004E
        mov    eax, 4
        mov    ecx, 0x454
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_4_004E
        mov    dword ptr [0], esi            ; -> obj1:0x00B4
        push   0                             ; -> loc_2_0000
        VxDCall  BIOS.Service_0001h              ; cd2001002b00
        add    esp, 4
        call   sub_1_1BE0
loc_4_0048:
        or     eax, eax
        je     loc_4_004E
loc_4_004C:
        clc    
        ret    
loc_4_004E:
        mov    dword ptr [0], 0              ; -> obj1:0x00AC
        stc    
        ret    
sub_4_005A:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    ebx, ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], ebx
        pop    ebx
        pop    ebp
        ret    
VxD__LTEXT ENDS
