; VMCPD -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 854 instructions recovered by recursive descent

Declare_Virtual_Device VMCPD, 4, 2, VMCPD_Control, 00011h, Undefined_Init_Order
;   DDK version 0400h   init_order 048000000h   flags 00000h
;   service table: obj1:0x7e4, 12 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x884  R-X
VMCPD_Control:
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 1
        je     loc_2_002C
        cmp    eax, 3
        je     loc_1_03B1
        cmp    eax, 7
        je     loc_1_03B1
        cmp    eax, 0x1d
        je     loc_1_03B1
        cmp    eax, 0x1e
        je     loc_1_03BD
        cmp    eax, 0x21
        je     loc_1_03DA
        cmp    eax, 0xc
        je     loc_1_03D4
        cmp    eax, 5
        je     loc_1_0420
        clc    
        ret    
VMCPD_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        je     loc_1_006E
        cmp    ax, 0
        jne    loc_1_00A3
        movzx  eax, word ptr [0x81c]         ; -> obj1:0x081C
        xchg   al, ah
        jmp    loc_1_009A
loc_1_006E:
        mov    ecx, dword ptr [0x840]        ; -> obj1:0x0840
        mov    eax, 1
        test   ecx, 1
        je     loc_1_009A
        or     eax, 2
        test   ecx, 2
        je     loc_1_008F
        or     eax, 4
loc_1_008F:
        test   ecx, 8
        je     loc_1_009A
        or     eax, 8
loc_1_009A:
        mov    word ptr [ebp + 0x18], ax
        and    dword ptr [ebp + 0x2c], 0xfffffffe
loc_1_00A2:
        ret    
loc_1_00A3:
        or     dword ptr [ebp + 0x2c], 1
        jmp    loc_1_00A2
VMCPD_Get_Version:
        test   dword ptr [0x840], 0x10       ; -> obj1:0x0840
        jne    loc_1_00BA
        call   sub_2_00FF
loc_1_00BA:
        movzx  eax, word ptr [0x81c]         ; -> obj1:0x081C
        xchg   al, ah
        sub    ecx, ecx
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        je     loc_1_00DE
        push   eax
        mov    cl, 3
        mov    eax, cr0
        test   al, 0x10
        jne    loc_1_00DD
        dec    cl
loc_1_00DD:
        pop    eax
loc_1_00DE:
        clc    
        ret    
VMCPD_Get_Virt_State:
        push   edi
        VMMCall  VMM.Get_Initial_Thread_Handle   ; cd200d010100
        VxDCall  VMCPD.VMCPD_Get_CR0_State       ; cd2003001100
        shr    eax, 1
        and    eax, 3
        pop    edi
        ret    
VMCPD_Set_Virt_State:
        push   eax
        push   edx
        push   edi
        VMMCall  VMM.Get_Initial_Thread_Handle   ; cd200d010100
        and    eax, 3
        shl    eax, 1
        VxDCall  VMCPD.VMCPD_Set_CR0_State       ; cd2004001100
        pop    edi
        pop    edx
        pop    eax
        ret    
VMCPD_Get_CR0_State:
        push   edi
        mov    eax, edi
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_0122
        mov    edi, eax
        call   VMCPD_Service_09
loc_1_0122:
        mov    eax, dword ptr [edi]
        and    eax, 6
        mov    edi, dword ptr [0x864]        ; -> obj1:0x0864
        or     edi, edi
        je     loc_1_0139
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        je     loc_1_013C
loc_1_0139:
        or     eax, 8
loc_1_013C:
        pop    edi
        ret    
VMCPD_Set_CR0_State:
        mov    edx, edi
        add    edx, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_0157
        push   edi
        push   eax
        call   VMCPD_Service_09
        mov    edx, edi
        pop    eax
        pop    edi
loc_1_0157:
        and    eax, 6
        and    dword ptr [edx], 0xfffffff9
        or     dword ptr [edx], eax
        test   eax, 4
        je     loc_1_0187
        and    dword ptr [edx], 0xffffff77
        cmp    edi, dword ptr [0x864]        ; -> obj1:0x0864
        jne    loc_1_0187
        mov    dword ptr [0x864], 0          ; -> obj1:0x0864
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_0187:
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        jne    loc_1_0196
        push   edi
        call   sub_1_0280
        pop    edi
loc_1_0196:
        ret    
VMCPD_Get_Thread_State:
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        je     loc_1_01F4
        push   ecx
        push   esi
        push   edi
        mov    ecx, edi
        add    ecx, dword ptr [0x870]        ; -> obj1:0x0870
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_01BD
        push   edi
        call   VMCPD_Service_09
        mov    ecx, edi
        pop    edi
loc_1_01BD:
        test   dword ptr [ecx], 0x80
        jne    loc_1_01CF
        call   sub_1_030B
        call   sub_1_02A5
loc_1_01CF:
        cmp    edi, dword ptr [0x864]        ; -> obj1:0x0864
        jne    loc_1_01E5
        call   sub_1_030B
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_01E5:
        mov    edi, esi
        lea    esi, [ecx + 4]
        mov    ecx, 0x1b
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ecx
loc_1_01F4:
        ret    
VMCPD_Set_Thread_State:
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        je     loc_1_024C
        push   ecx
        push   esi
        push   edi
        mov    ecx, edi
        add    ecx, dword ptr [0x870]        ; -> obj1:0x0870
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_021B
        push   edi
        call   VMCPD_Service_09
        mov    ecx, edi
        pop    edi
loc_1_021B:
        and    dword ptr [ecx], 0xfffffff7
        or     dword ptr [ecx], 0x80
        cmp    edi, dword ptr [0x864]        ; -> obj1:0x0864
        jne    loc_1_023F
        mov    dword ptr [0x864], 0          ; -> obj1:0x0864
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_023F:
        lea    edi, [ecx + 4]
        mov    ecx, 0x1b
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ecx
loc_1_024C:
        ret    
VMCPD_Set_Thread_Precision:
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        je     loc_1_027F
        push   ecx
        mov    ecx, edi
        add    ecx, dword ptr [0x870]        ; -> obj1:0x0870
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_0273
        push   edi
        push   eax
        call   VMCPD_Service_09
        pop    eax
        mov    ecx, edi
        pop    edi
loc_1_0273:
        and    eax, 1
        shl    eax, 4
        and    dword ptr [ecx], 0xffffffef
        or     dword ptr [ecx], eax
        pop    ecx
loc_1_027F:
        ret    
sub_1_0280:
        mov    edx, edi
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_0295
        mov    edi, edx
        call   VMCPD_Service_09
loc_1_0295:
        mov    edx, dword ptr [edi]
        and    dl, 0xe
        mov    eax, cr0
        and    al, 0xf9
        or     al, dl
        mov    cr0, eax
        ret    
sub_1_02A5:
        mov    dword ptr [0x864], edi        ; -> obj1:0x0864
        push   edi
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_02C0
        mov    edi, dword ptr [esp]
        call   VMCPD_Service_09
loc_1_02C0:
        clts   
        bts    dword ptr [edi], 7
        jae    loc_1_02D5
        frstor dword ptr [edi + 4]
        test   dword ptr [edi], 8
        jne    loc_1_0300
loc_1_02D3:
        pop    edi
        ret    
loc_1_02D5:
        or     dword ptr [edi], 0x100
        fninit 
        test   dword ptr [edi], 0x10
        je     loc_1_02F7
        fnstcw word ptr [edi + 4]
        and    word ptr [edi + 4], 0xfcff
        or     word ptr [edi + 4], 0x200
        fldcw  word ptr [edi + 4]
loc_1_02F7:
        wait   
        and    dword ptr [edi], 0xfffffeff
        jmp    loc_1_02D3
loc_1_0300:
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
        jmp    loc_1_02D3
sub_1_030B:
        push   edi
        mov    edi, dword ptr [0x864]        ; -> obj1:0x0864
        test   edi, edi
        je     loc_1_0345
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_032A
        mov    edi, dword ptr [esp]
        call   VMCPD_Service_09
loc_1_032A:
        or     dword ptr [edi], 0x100
        clts   
        fnsave dword ptr [edi + 4]
        mov    dword ptr [0x864], 0          ; -> obj1:0x0864
        and    dword ptr [edi], 0xfffffeff
loc_1_0345:
        pop    edi
        ret    
loc_1_0347:
        mov    edx, dword ptr [edi]
        and    edx, 0x10000000
        je     loc_1_038D
        test   dword ptr [0x840], 1          ; -> obj1:0x0840
        je     loc_1_03AB
        push   edi
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_1_03A1
        inc    dword ptr [edi - 4]
        mov    edx, dword ptr [edi - 4]
        pop    edi
        cmp    dword ptr [0x864], edi        ; -> obj1:0x0864
        je     loc_1_039D
        cmp    edx, 0x100
        jg     loc_1_03A2
        call   sub_1_030B
        call   sub_1_02A5
        jmp    loc_1_03AB
loc_1_038D:
        cmp    eax, dword ptr [0x864]        ; -> obj1:0x0864
        je     loc_1_03A2
        cmp    edi, dword ptr [0x864]        ; -> obj1:0x0864
        jne    loc_1_03AB
loc_1_039D:
        clts   
        jmp    loc_1_03AB
loc_1_03A1:
        pop    edi
loc_1_03A2:
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_03AB:
        call   sub_1_0280
        ret    
loc_1_03B1:
        xor    eax, eax
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    dword ptr [edi], eax
        clc    
        ret    
loc_1_03BD:
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    eax, dword ptr [edi]
        or     eax, eax
        jne    loc_1_03D2
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_03D2:
        clc    
        ret    
loc_1_03D4:
        VMMCall  VMM.Get_Initial_Thread_Handle   ; cd200d010100
loc_1_03DA:
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        je     loc_1_0401
        cmp    edi, dword ptr [0x864]        ; -> obj1:0x0864
        jne    loc_1_0401
        mov    dword ptr [0x864], 0          ; -> obj1:0x0864
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
loc_1_0401:
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     loc_1_041E
        sub    eax, 4
        push   esi
        mov    esi, dword ptr [0x874]        ; -> obj1:0x0874
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        pop    esi
loc_1_041E:
        clc    
        ret    
loc_1_0420:
        mov    eax, cr0
        and    eax, 0xffffffe1
        or     eax, dword ptr [0x868]        ; -> obj1:0x0868
        mov    cr0, eax
        mov    ax, word ptr [0x10]           ; -> obj4:0x0010
        and    al, 2
        or     byte ptr [0x410], al
        clc    
        ret    
sub_1_043F:
        push   esi
        mov    esi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    esi, dword ptr [esi + edi]
        push   ebp
        VxDCall  NDIS.Service_0008h              ; cd2008002a00
        add    esp, 4
        cmp    eax, 1
        je     loc_1_0468
        clts   
        and    dword ptr [esi], 0xfffffff7
        cmp    eax, 2
        je     loc_1_0468
        VxDCall  NDIS.Service_0006h              ; cd2006002a00
loc_1_0468:
        clc    
        pop    esi
        ret    
sub_1_046B:
        stc    
loc_1_046C:
        lodsb  al, byte ptr [esi]
loc_1_046D:
        clc    
loc_1_046E:
        ret    
        db  02Eh, 036h, 03Eh, 026h, 064h, 065h, 066h, 067h
_VMCPD_Get_FP_Instruction_Size:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [ebp + 0xc]
        test   dword ptr [ebx], 0x20
        jne    loc_1_04A3
        shl    eax, 4
        add    esi, eax
        add    esi, dword ptr [ebx + 4]
        mov    ebx, esi
        mov    edx, 1
        jmp    loc_1_04DF
loc_1_04A3:
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_057E
        add    esi, eax
        mov    ebx, esi
        lar    eax, word ptr [ebp + 0xc]
        jne    loc_1_057E
        and    ax, 0x1800
        cmp    ax, 0x1800
        jne    loc_1_057E
        sub    edx, edx
        test   eax, 0x400000
        jne    loc_1_04DF
        inc    edx
loc_1_04DF:
        call   sub_1_046B
        jb     loc_1_057E
        cmp    al, 0x9b
        je     loc_1_0575
        mov    edi, 0x46f                    ; -> obj1:0x046F
        mov    ecx, 8
        cld    
        repne scasb al, byte ptr es:[edi]
        jne    loc_1_050A
        cmp    al, 0x67
        jne    loc_1_04DF
        xor    edx, 1
        jmp    loc_1_04DF
loc_1_050A:
        mov    ah, al
        and    al, 0xf8
        cmp    al, 0xd8
        jne    loc_1_057E
        call   sub_1_046B
        jb     loc_1_057E
        cmp    ax, 0xdfe0
        jne    loc_1_052E
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    edi, dword ptr [edi + 0x10]
        mov    word ptr [edi + 0x1c], 0xffff
loc_1_052E:
        cmp    al, 0xc0
        jae    loc_1_0575
        mov    cl, al
        mov    ch, al
        shr    cl, 6
        and    ch, 7
        or     dl, dl
        je     loc_1_054B
        cmp    cx, 0x600
        jne    loc_1_0570
        mov    cl, 2
        jmp    loc_1_0570
loc_1_054B:
        cmp    cx, 0x500
        je     loc_1_056E
        cmp    ch, 4
        jne    loc_1_0569
        call   sub_1_046B
        jb     loc_1_057E
        and    al, 7
        mov    ch, al
        cmp    cx, 0x500
        je     loc_1_056E
loc_1_0569:
        cmp    cl, 2
        jne    loc_1_0570
loc_1_056E:
        mov    cl, 4
loc_1_0570:
        movzx  ecx, cl
        add    esi, ecx
loc_1_0575:
        sub    esi, ebx
        mov    eax, esi
loc_1_0579:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_057E:
        xor    eax, eax
        jmp    loc_1_0579
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_0878               ; fixup
loc_1_058A:
        call   sub_1_05E5
        jb     loc_1_0592
        ret    
loc_1_0592:
        jmp    loc_1_0878
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_0880               ; fixup
loc_1_05A0:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, cr0
        push   eax
        and    eax, 0xfffffffb
        mov    cr0, eax
        or     ecx, 0xffffffff
        call   sub_1_05E5
        pop    edx
        pop    edi
        jb     loc_1_05C9
        and    edx, 4
        mov    eax, cr0
        or     eax, edx
        mov    cr0, eax
        ret    
loc_1_05C9:
        jmp    loc_1_0880
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_087C               ; fixup
loc_1_05D7:
        call   sub_1_05E5
        jb     loc_1_05DF
        ret    
loc_1_05DF:
        jmp    loc_1_087C
sub_1_05E5:
        mov    eax, cr0
        bt     eax, 2
        jb     loc_1_0667
        bt     dword ptr [0x840], 0          ; -> obj1:0x0840
        cmc    
        jb     loc_1_0643
        sti    
        bt     dword ptr [0x840], 1          ; -> obj1:0x0840
        jae    loc_1_064D
        cmp    edi, dword ptr [0x864]        ; -> obj1:0x0864
        je     loc_1_061D
        push   ecx
        call   sub_1_0684
        call   sub_1_030B
        pop    ecx
        call   sub_1_02A5
loc_1_061D:
        mov    eax, dword ptr [0x870]        ; -> obj1:0x0870
        mov    eax, dword ptr [edi + eax]
        test   dword ptr [eax], 8
        jne    loc_1_0644
loc_1_062D:
        push   edi
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_1_0641
        mov    dword ptr [edi - 4], 0
loc_1_0641:
        pop    edi
        clc    
loc_1_0643:
        ret    
loc_1_0644:
        push   edi
        call   sub_1_043F
        pop    edi
        jmp    loc_1_062D
loc_1_064D:
        call   sub_1_0684
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        VxDCall  VMCPD._VMCPD_Get_FP_Instruction_Size ; cd2007001100
        add    esp, 8
        add    dword ptr [ebp + 0x24], eax
        clc    
        jmp    loc_1_0643
loc_1_0667:
        call   sub_1_0684
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        stc    
        jne    loc_1_0643
        sti    
        push   edi
        push   ebp
        VxDCall  NDIS.Service_0007h              ; cd2007002a00
        add    esp, 8
        clc    
        jmp    loc_1_0643
sub_1_0684:
        push   edi
        push   eax
        cmp    ecx, -1
        je     loc_1_0693
        test   dword ptr [edi], 0x10000000
        je     loc_1_06CD
loc_1_0693:
        mov    eax, cr0
        bt     eax, 2
        jb     loc_1_06C5
        bt     dword ptr [0x840], 0          ; -> obj1:0x0840
        jae    loc_1_06C7
        bt     dword ptr [0x840], 1          ; -> obj1:0x0840
        jae    loc_1_06C7
        mov    eax, edi
        add    eax, dword ptr [0x870]        ; -> obj1:0x0870
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_06C3
        call   VMCPD_Service_09
loc_1_06C3:
        jmp    loc_1_06CD
loc_1_06C5:
        jmp    loc_1_06C7
loc_1_06C7:
        VxDCall  NDIS.Service_0018h              ; cd2018002a00
loc_1_06CD:
        pop    eax
        pop    edi
        ret    
loc_1_06D0:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, eax
        sub    al, al
        out    0xf0, al
        mov    eax, ebx
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        cmp    edi, dword ptr [0x864]        ; -> obj1:0x0864
        jne    loc_1_0735
        mov    esi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    esi, dword ptr [edi + esi]
        test   dword ptr [esi], 0x148
        jne    loc_1_0735
        btr    dword ptr [esi], 5
        jb     loc_1_0735
        VxDCall  NDIS.Service_001Bh              ; cd201b002a00
        or     eax, eax
        je     loc_1_071C
        mov    eax, cr0
        or     eax, 8
        mov    cr0, eax
        or     dword ptr [esi], 0x28
        jmp    loc_1_0735
loc_1_071C:
        or     dword ptr [esi], 0x40
        mov    edx, ebx
        mov    ebx, edi
        sub    eax, eax
        mov    ecx, 0x148
        mov    esi, 0x738                    ; -> loc_1_0738
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_1_0735:
        pop    edi
        clc    
        ret    
loc_1_0738:
        mov    ecx, 1
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    eax, edx
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        ret    
loc_1_074C:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        ret    
loc_1_0753:
        push   edi
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        and    dword ptr [edi], 0xffffffbf
        pop    edi
        ret    
VMCPD_Service_09:
        push   ecx
        push   edi
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_1_0781
        mov    edi, dword ptr [esp]
        push   ecx
        jmp    loc_1_07D7
loc_1_0781:
        push   esi
        mov    esi, dword ptr [0x874]        ; -> obj1:0x0874
        VMMCall  VMM.List_Allocate               ; cd209d000100
        pop    esi
        jae    loc_1_0795
        xor    edi, edi
        jmp    loc_1_07E0
loc_1_0795:
        xchg   edi, eax
        mov    ecx, 0x1d
        push   eax
        push   edi
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        pop    edi
        pop    eax
        xchg   edi, eax
        add    eax, 4
        mov    dword ptr [edi], eax
        mov    edi, eax
        test   dword ptr [0x840], 1          ; -> obj1:0x0840
        je     loc_1_07E0
        cmp    dword ptr [0x864], 0          ; -> obj1:0x0864
        jne    loc_1_07DF
        push   edi
        mov    edi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, -1
        jne    loc_1_07D7
        mov    eax, cr0
        and    eax, 0xfffffffb
        mov    cr0, eax
loc_1_07D7:
        clts   
        call   sub_1_02A5
        pop    ecx
loc_1_07DF:
        clc    
loc_1_07E0:
        pop    ecx
        pop    ecx
        ret    
        db  000h
        dd  VMCPD_Get_Version        ; fixup
        dd  VMCPD_Get_Virt_State     ; fixup
        dd  VMCPD_Set_Virt_State     ; fixup
        dd  VMCPD_Get_CR0_State      ; fixup
        dd  VMCPD_Set_CR0_State      ; fixup
        dd  VMCPD_Get_Thread_State   ; fixup
        dd  VMCPD_Set_Thread_State   ; fixup
        dd  _VMCPD_Get_FP_Instruction_Size ; fixup
        dd  VMCPD_Set_Thread_Precision ; fixup
        dd  VMCPD_Service_09         ; fixup
        dd  VMCPD_Service_10         ; fixup
        dd  VMCPD_Service_11         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00011h                      ; DDB_Req_Device_Number
        db  4, 2                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VMCPD   '                  ; DDB_Name
        dd  048000000h                  ; DDB_Init_Order
        dd  VMCPD_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VMCPD_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x07E4              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0878:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_087C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0880:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x18d  R-X
loc_2_0000:
        VMMCall  VMM._AllocateThreadDataSlot     ; cd20ff000100
        test   eax, eax
        jne    loc_2_0010
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_2_0010:
        mov    dword ptr [0x870], eax        ; -> obj1:0x0870
        mov    ecx, 0x74
        mov    eax, 1
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    dword ptr [0x874], esi        ; -> obj1:0x0874
        ret    
loc_2_002C:
        mov    eax, cr0
        and    eax, 0x1e
        mov    ecx, dword ptr [0x840]        ; -> obj1:0x0840
        and    ecx, 4
        or     eax, ecx
        mov    dword ptr [0x868], eax        ; -> obj1:0x0868
        mov    eax, cr0
        and    eax, 0xfffffff1
        mov    cr0, eax
        mov    ax, word ptr [0x410]
        mov    word ptr [0x10], ax           ; -> obj4:0x0010
        test   dword ptr [0x840], 1          ; -> obj1:0x0840
        je     loc_2_00A2
        test   dword ptr [0x840], 0x10       ; -> obj1:0x0840
        jne    loc_2_0074
        call   sub_2_00FF
loc_2_0074:
        mov    ecx, 0xa
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        jne    loc_2_0088
        and    ecx, 0xfffffffd
loc_2_0088:
        mov    eax, cr0
        or     eax, ecx
        mov    cr0, eax
        mov    edi, 0x154                    ; -> loc_2_0154
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_2_00F6
        mov    dword ptr [0x86c], eax        ; -> obj1:0x086C
loc_2_00A2:
        mov    esi, 0                        ; -> obj4:0x0000
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
        mov    dword ptr [0x864], 0          ; -> obj1:0x0864
        mov    eax, 7
        mov    esi, 0x58a                    ; -> loc_1_058A
        VMMCall  VMM.Hook_PM_Fault               ; cd2080000100
        mov    esi, 0x5d7                    ; -> loc_1_05D7
        VMMCall  VMM.Hook_V86_Fault              ; cd207f000100
        mov    eax, 7
        mov    esi, 0x5a0                    ; -> loc_1_05A0
        VMMCall  VMM.Hook_VMM_Fault              ; cd2081000100
        mov    esi, 0x347                    ; -> loc_1_0347
        VMMCall  VMM.Call_When_Thread_Switched   ; cd2004010100
        or     dword ptr [0x840], 0x10       ; -> obj1:0x0840
        clc    
        ret    
loc_2_00F6:
        pushal 
        xor    esi, esi
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
sub_2_00FF:
        push   esi
        push   edi
        mov    ax, word ptr [0x410]
        mov    word ptr [0x10], ax           ; -> obj4:0x0010
        sub    eax, eax
        mov    esi, 0x174                    ; -> loc_2_0174
        mov    edi, 0x184                    ; -> loc_2_0184
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        or     eax, eax
        je     loc_2_0147
        cmp    eax, 2
        je     loc_2_0139
        cmp    eax, 1
        jne    loc_2_0147
        test   dword ptr [0x840], 8          ; -> obj1:0x0840
        je     loc_2_0147
loc_2_0139:
        and    dword ptr [0x840], 0xfffffffd ; -> obj1:0x0840
        and    byte ptr [0x410], 0xfd
loc_2_0147:
        or     dword ptr [0x840], 0x10       ; -> obj1:0x0840
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h
loc_2_0154:
        or     eax, 0xd0000000               ; -> loc_1_06D0
        push   es
        add    byte ptr [eax], al
        dec    esp
        pop    es
        add    byte ptr [eax], al
        push   ebx
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_0174:
        dec    ebp
        popal  
        je     0x1e0
        inc    ebx
        outsd  dx, dword ptr [esi]
        jo     0x1ee
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebp + 0x73], sp
        jae    0x1f1
        jb     loc_2_0184
loc_2_0184:
        inc    esi
        push   eax
        push   ebp
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x18d
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xf1  R-X
VMCPD_Service_10:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        je     loc_3_0081
        push   edi
        cmp    dword ptr [0x864], edi        ; -> obj1:0x0864
        je     loc_3_004E
        mov    eax, edi
        add    eax, dword ptr [0x870]        ; -> obj1:0x0870
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_3_0044
        cmp    dword ptr [0x864], 0          ; -> obj1:0x0864
        pushfd 
        or     ecx, 0xffffffff
        call   VMCPD_Service_09
        popfd  
        je     loc_3_004E
        mov    edi, dword ptr [esp]
loc_3_0044:
        call   sub_1_030B
        call   sub_1_02A5
loc_3_004E:
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    esi, dword ptr [edi]
        fnsave dword ptr [esi + 4]
        add    esi, 4
        mov    edi, dword ptr [ebp + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    esi, 4
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        mov    eax, 1
        call   VMCPD_Set_Thread_Precision
        wait   
        fninit 
        wait   
        mov    eax, 0
        jmp    loc_3_0086
loc_3_0081:
        mov    eax, 0xc000014a
loc_3_0086:
        pop    esi
        pop    edi
        leave  
        ret    
VMCPD_Service_11:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        test   dword ptr [0x840], 2          ; -> obj1:0x0840
        je     loc_3_00E1
        mov    eax, cr0
        cmp    dword ptr [0x864], edi        ; -> obj1:0x0864
        je     loc_3_00B1
        call   sub_1_030B
loc_3_00B1:
        push   edi
        add    edi, dword ptr [0x870]        ; -> obj1:0x0870
        mov    edi, dword ptr [edi]
        add    edi, 4
        mov    eax, edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    edi, 4
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [edx + 0x18], 0xffffffff
        mov    dword ptr [eax + 0xc], 0xffff
        pop    edi
        jmp    loc_3_00E8
loc_3_00E1:
        mov    eax, 0xc0000001
        jmp    loc_3_00ED
loc_3_00E8:
        mov    eax, 0
loc_3_00ED:
        pop    esi
        pop    edi
        leave  
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x12  RW-
        db  000h, 000h, 000h, 000h
        dd  loc_1_046C               ; fixup
        dd  loc_1_046D               ; fixup
        dd  loc_1_046E               ; fixup
        db  000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x73  R-X
        db  033h, 0DBh, 066h, 033h, 0D2h, 00Fh, 001h, 0E0h, 08Ah, 0D0h, 080h, 0E2h, 004h, 0A8h, 004h, 074h
        db  005h, 024h, 0FBh, 00Fh, 001h, 0F0h, 0E8h, 01Ah, 000h, 00Ah, 0D0h, 0B8h, 000h, 000h, 08Bh, 0F3h
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 0C0h, 07Eh, 001h, 050h, 041h, 000h, 000h, 000h, 080h, 0FFh
        db  0FFh, 047h, 041h, 0DBh, 0E3h, 0BEh, 021h, 000h, 0C7h, 004h, 05Ah, 05Ah, 0DDh, 03Ch, 080h, 03Ch
        db  000h, 075h, 02Ch, 0D9h, 03Ch, 08Bh, 004h, 025h, 03Fh, 010h, 083h, 0F8h, 03Fh, 075h, 020h, 0DDh
        db  006h, 023h, 000h, 0DCh, 036h, 02Bh, 000h, 0DCh, 00Eh, 02Bh, 000h, 0DCh, 02Eh, 023h, 000h, 0D9h
        db  0E8h, 0DEh, 0D9h, 0DFh, 0E0h, 0DBh, 0E3h, 09Eh, 0B0h, 003h, 073h, 002h, 00Ch, 008h, 0C3h, 02Bh
        db  0C0h, 0EBh, 0FBh
VxD__LTEXT ENDS
