; vmm_lcode_08_02.asm -- VMM object 8 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_8_5288:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 20h]
        push   ecx
        mov    eax, dword ptr [ecx + 12h]
        or     eax, 1
        mov    dword ptr [ecx + 12h], eax
        and    eax, 0fffffffbh
        mov    dword ptr [ecx + 12h], eax
        call   sub_8_4DB7
        add    esp, 4
        mov    eax, dword ptr [esi + 10h]
        or     eax, 1
        mov    dword ptr [esi + 10h], eax
        test   al, 18h
        jne    SHORT loc_8_52BA
        call   sub_8_7AE7
loc_8_52BA:
        pop    esi
        ret
sub_8_52BC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 24h]
        push   ecx
        mov    eax, dword ptr [ecx + 10h]
        or     eax, 1
        mov    dword ptr [ecx + 10h], eax
        and    eax, 0fffffffbh
        mov    dword ptr [ecx + 10h], eax
        call   sub_8_4E23
        add    esp, 4
        mov    eax, dword ptr [esi + 10h]
        or     eax, 1
        mov    dword ptr [esi + 10h], eax
        test   al, 18h
        jne    SHORT loc_8_52EE
        call   sub_8_7AE7
loc_8_52EE:
        pop    esi
        ret
sub_8_52F0:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    ecx, dword ptr [eax + 0ch]
        or     ecx, 1
        mov    dword ptr [eax + 0ch], ecx
        and    ecx, 0fffffffbh
        mov    dword ptr [eax + 0ch], ecx
        call   sub_8_4E84
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, 1
        mov    dword ptr [eax + 10h], ecx
        test   cl, 18h
        jne    SHORT loc_8_5323
        call   sub_8_7AE7
loc_8_5323:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_8_5330:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   0
        mov    esi, dword ptr [esp + 30h]
        push   4
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5359
        mov    eax, 57h
        jmp    NEAR PTR loc_8_541C
loc_8_5359:
        push   1
        push   0
        push   0
        mov    edi, dword ptr [esp + 2ch]
        push   1
        push   edi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_537C
        mov    eax, 57h
        jmp    NEAR PTR loc_8_541C
loc_8_537C:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_538F
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_541C
loc_8_538F:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_5415
        mov    eax, dword ptr [esp + 10h]
        push   1
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp + 18h]
        push   eax
        push   edi
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_8_1FF0
        add    esp, 14h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_5415
        push   dword ptr [esp + 0ch]
        call   sub_8_747A
        add    esp, 4
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 10h]
        call   sub_8_7157
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_5404
        push   dword ptr [esp + 0ch]
        call   sub_8_7482
        add    esp, 4
        mov    ebx, 0eh
        jmp    SHORT loc_8_5415
loc_8_5404:
        mov    dword ptr [esi], eax
        inc    dword ptr [eax + 14h]
        mov    ecx, dword ptr [esp + 10h]
        sub    ebx, ebx
        mov    ecx, dword ptr [ecx + 1ch]
        mov    dword ptr [eax + 1ch], ecx
loc_8_5415:
        call   sub_8_84FA
        mov    eax, ebx
loc_8_541C:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_8_5423:
        sub    esp, 4
        push   esi
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_5437
        mov    eax, 0a7h
        jmp    SHORT loc_8_54AC
loc_8_5437:
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_8_717D
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_8_5456
        cmp    esi, 3fah
        jne    SHORT loc_8_54A5
loc_8_5456:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0ch]
        call   sub_8_7482
        add    esp, 4
        cmp    esi, 3fah
        je     SHORT loc_8_5483
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 1ch], 0
        jne    SHORT loc_8_5483
        push   eax
        call   sub_8_3D28
        add    esp, 4
loc_8_5483:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 19h], 40h
        jne    SHORT loc_8_54A3
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 14h], 0
        jne    SHORT loc_8_54A3
        push   eax
        call   sub_8_7375
        add    esp, 4
loc_8_54A3:
        sub    esi, esi
loc_8_54A5:
        call   sub_8_84FA
        mov    eax, esi
loc_8_54AC:
        pop    esi
        add    esp, 4
        ret
sub_8_54B1:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 48h]
        or     esi, esi
        je     NEAR PTR loc_8_598F
        cmp    dword ptr [esp + 4ch], 0
        je     NEAR PTR loc_8_598F
        cmp    dword ptr [esp + 54h], 0
        je     NEAR PTR loc_8_598F
        mov    ecx, dword ptr [esp + 54h]
        test   byte ptr [ecx + 3], 80h
        jne    NEAR PTR loc_8_598F
        push   0
        push   0
        push   0
        push   10h
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5502
        jmp    NEAR PTR loc_8_598F
loc_8_5502:
        push   0
        push   0
        push   0
        push   4
        push   dword ptr [esp + 64h]
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_551F
        jmp    NEAR PTR loc_8_598F
loc_8_551F:
        mov    ecx, dword ptr [esp + 54h]
        push   0
        mov    ebx, dword ptr [ecx]
        push   0
        push   0
        mov    edi, dword ptr [esp + 5ch]
        push   ebx
        push   edi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5542
        jmp    NEAR PTR loc_8_598F
loc_8_5542:
        mov    dword ptr [esp + 24h], ebx
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 30h], 0
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_5569
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_5994
loc_8_5569:
        lea    eax, [esp + 34h]
        push   eax
        push   dword ptr [esp + 48h]
        call   sub_8_717D
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_8_595A
        mov    eax, dword ptr [esp + 34h]
        push   1
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 3ch], eax
        lea    eax, [esp + 40h]
        push   eax
        push   0
        lea    eax, [esp + 44h]
        push   dword ptr [esp + 40h]
        push   eax
        call   sub_8_1FF0
        mov    dword ptr [esp + 24h], eax
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_8_595A
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        call   sub_8_22A8
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_8_595A
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        call   sub_8_70F0
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, -1
        jne    SHORT loc_8_55FC
        mov    dword ptr [esp + 10h], 3f3h
        jmp    NEAR PTR loc_8_595A
loc_8_55FC:
        mov    eax, dword ptr [esp + 34h]
        cmp    dword ptr [eax + 1ch], 80000006h
        je     NEAR PTR loc_8_56F0
        sub    ebx, ebx
        cmp    dword ptr [esp + 4ch], ebx
        je     SHORT loc_8_5660
        lea    ebp, [esi + 4]
loc_8_5618:
        push   dword ptr [ebp - 4]
        push   dword ptr [esp + 30h]
        call   sub_8_2366
        add    esp, 8
        cmp    eax, -1
        je     SHORT loc_8_567B
        mov    dword ptr [ebp + 4], eax
        movzx  ecx, word ptr [eax + 0ah]
        mov    dword ptr [ebp], ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 8], eax
        add    dword ptr [esp + 18h], ecx
        cmp    eax, 1
        jne    SHORT loc_8_564B
        inc    dword ptr [esp + 18h]
        inc    dword ptr [ebp]
loc_8_564B:
        or     edi, edi
        jne    SHORT loc_8_5656
        mov    dword ptr [ebp + 4], 0
loc_8_5656:
        add    ebp, 10h
        inc    ebx
        cmp    ebx, dword ptr [esp + 4ch]
        jb     SHORT loc_8_5618
loc_8_5660:
        mov    ecx, dword ptr [esp + 54h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        or     edi, edi
        jne    SHORT loc_8_5688
        mov    dword ptr [esp + 10h], 0
        jmp    NEAR PTR loc_8_595A
loc_8_567B:
        mov    dword ptr [esp + 10h], 0ffff0001h
        jmp    NEAR PTR loc_8_595A
loc_8_5688:
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, dword ptr [esp + 24h]
        jbe    SHORT loc_8_569F
        mov    dword ptr [esp + 10h], 0eah
        jmp    NEAR PTR loc_8_595A
loc_8_569F:
        cmp    dword ptr [esp + 4ch], 0
        je     SHORT loc_8_56E3
        add    esi, 8
        mov    ebx, dword ptr [esp + 4ch]
loc_8_56AD:
        mov    ebp, dword ptr [esi]
        or     ebp, ebp
        je     SHORT loc_8_56DD
        mov    dword ptr [esi], edi
        movzx  eax, word ptr [ebp + 0ah]
        push   eax
        movzx  eax, word ptr [ebp + 8]
        lea    eax, [ebp + eax + 0ch]
        push   eax
        push   edi
        call   sub_8_366A
        add    esp, 0ch
        cmp    dword ptr [ebp], 1
        jne    SHORT loc_8_56DA
        movzx  eax, word ptr [ebp + 0ah]
        mov    byte ptr [edi + eax], 0
loc_8_56DA:
        add    edi, dword ptr [esi - 4]
loc_8_56DD:
        add    esi, 10h
        dec    ebx
        jne    SHORT loc_8_56AD
loc_8_56E3:
        mov    dword ptr [esp + 10h], 0
        jmp    NEAR PTR loc_8_595A
loc_8_56F0:
        or     edi, edi
        je     SHORT loc_8_571F
        mov    eax, dword ptr [esp + 4ch]
        push   201h
        imul   eax, eax, 0ch
        push   eax
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        mov    dword ptr [esp + 38h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_571F
        mov    dword ptr [esp + 10h], 0eh
        jmp    NEAR PTR loc_8_595A
loc_8_571F:
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 20h], 0
        cmp    dword ptr [esp + 4ch], 0
        je     NEAR PTR loc_8_5804
        lea    ebp, [esi + 4]
        mov    ebx, dword ptr [esp + 30h]
loc_8_5747:
        mov    eax, dword ptr [esp + 34h]
        cmp    dword ptr [eax + 1ch], 80000006h
        jne    NEAR PTR loc_8_57EC
        push   dword ptr [ebp - 4]
        push   dword ptr [esp + 30h]
        call   sub_8_2366
        add    esp, 8
        cmp    eax, -1
        je     NEAR PTR loc_8_581F
        movzx  ecx, word ptr [eax + 8]
        lea    ecx, [eax + ecx + 0ch]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp], eax
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 10h], 1
        je     SHORT loc_8_57D5
        mov    dword ptr [esp + 1ch], 0
        mov    eax, dword ptr [eax]
        or     eax, eax
        push   0
        je     SHORT loc_8_57B9
        lea    ecx, [esp + 20h]
        push   ecx
        push   0
        push   1
        push   ebx
        push   dword ptr [esp + 3ch]
        call   eax
        jmp    SHORT loc_8_57CE
loc_8_57B9:
        lea    eax, [esp + 20h]
        push   eax
        push   0
        push   1
        push   ebx
        mov    eax, dword ptr [esp + 28h]
        push   dword ptr [esp + 3ch]
        call   dword ptr [eax + 4]
loc_8_57CE:
        mov    dword ptr [esp + 28h], eax
        add    esp, 18h
loc_8_57D5:
        mov    eax, dword ptr [esp + 18h]
        add    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 18h], eax
        or     edi, edi
        jne    SHORT loc_8_57EC
        mov    dword ptr [ebp + 4], 0
loc_8_57EC:
        add    ebp, 10h
        add    ebx, 0ch
        inc    dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 4ch]
        cmp    dword ptr [esp + 20h], eax
        jb     NEAR PTR loc_8_5747
loc_8_5804:
        or     edi, edi
        mov    ecx, dword ptr [esp + 54h]
        mov    eax, dword ptr [esp + 18h]
        jne    SHORT loc_8_582C
        mov    dword ptr [ecx], eax
        mov    dword ptr [esp + 10h], 0
        jmp    NEAR PTR loc_8_595A
loc_8_581F:
        mov    dword ptr [esp + 10h], 0ffff0001h
        jmp    NEAR PTR loc_8_595A
loc_8_582C:
        cmp    dword ptr [ecx], eax
        jae    SHORT loc_8_583F
        mov    dword ptr [ecx], eax
        mov    dword ptr [esp + 10h], 0eah
        jmp    NEAR PTR loc_8_595A
loc_8_583F:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_8_58C0
        push   0
        push   dword ptr [esp + 58h]
        push   edi
        push   dword ptr [esp + 58h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 3ch]
        call   eax
        mov    dword ptr [esp + 28h], eax
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_8_5881
        mov    ecx, dword ptr [esp + 54h]
        mov    dword ptr [esp + 10h], 3f4h
        mov    dword ptr [ecx], 0
        jmp    NEAR PTR loc_8_595A
loc_8_5881:
        cmp    dword ptr [esp + 4ch], 0
        je     NEAR PTR loc_8_595A
        mov    edi, dword ptr [esp + 30h]
        add    edi, 8
        mov    eax, dword ptr [esp + 14h]
        add    esi, 8
        mov    ecx, dword ptr [eax + 10h]
        and    ecx, 1
        mov    edx, dword ptr [esp + 4ch]
loc_8_58A4:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        or     ecx, ecx
        je     SHORT loc_8_58B2
        mov    eax, dword ptr [edi - 8]
        mov    dword ptr [esi - 4], eax
loc_8_58B2:
        add    edi, 0ch
        add    esi, 10h
        dec    edx
        jne    SHORT loc_8_58A4
        jmp    NEAR PTR loc_8_595A
loc_8_58C0:
        sub    ebx, ebx
        mov    dword ptr [esp + 20h], ebx
        cmp    dword ptr [esp + 4ch], ebx
        je     NEAR PTR loc_8_5954
        add    esi, 8
        mov    ebp, dword ptr [esp + 30h]
loc_8_58D7:
        mov    ecx, dword ptr [esp + 54h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_8_5906
        lea    eax, [esp + 24h]
        push   0
        push   eax
        lea    eax, [edi + ebx]
        push   eax
        push   1
        push   ebp
        push   dword ptr [esp + 3ch]
        call   ecx
        mov    dword ptr [esp + 28h], eax
        add    esp, 18h
loc_8_5906:
        mov    ecx, dword ptr [esp + 54h]
        mov    eax, dword ptr [esp + 24h]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_8_5925
        or     eax, eax
        jne    SHORT loc_8_5925
        lea    eax, [edi + ebx]
        mov    dword ptr [esi], eax
loc_8_5925:
        mov    eax, dword ptr [esp + 14h]
loc_8_5929:
        add    ebx, dword ptr [esp + 24h]
        test   byte ptr [eax + 10h], 1
        je     SHORT loc_8_5939
        mov    eax, dword ptr [ebp]
loc_8_5936:
        mov    dword ptr [esi - 4], eax
loc_8_5939:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_8_597B
        add    esi, 10h
        add    ebp, 0ch
        inc    dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 4ch]
        cmp    dword ptr [esp + 20h], eax
        jb     SHORT loc_8_58D7
loc_8_5954:
        mov    ecx, dword ptr [esp + 54h]
        mov    dword ptr [ecx], ebx
loc_8_595A:
        cmp    dword ptr [esp + 30h], 0
loc_8_595F:
        je     SHORT loc_8_5970
        push   0
        push   dword ptr [esp + 34h]
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_8_5970:
        call   sub_8_84FA
        mov    eax, dword ptr [esp + 10h]
        jmp    SHORT loc_8_5994
loc_8_597B:
        mov    ecx, dword ptr [esp + 54h]
        mov    dword ptr [esp + 10h], 3f4h
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_8_595A
loc_8_598F:
        mov    eax, 57h
loc_8_5994:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
sub_8_599C:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [esp + 58h]
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_59CD
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5C59
loc_8_59CD:
        push   1
        push   0
        push   0
        mov    esi, dword ptr [esp + 4ch]
        push   4
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_59F0
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5C59
loc_8_59F0:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [esp + 48h]
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5A12
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5C59
loc_8_5A12:
        push   1
        push   0
        push   0
        mov    edi, dword ptr [esp + 48h]
        push   1
        push   edi
        call   sub_8_1598
loc_8_5A24:
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5A35
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5C59
loc_8_5A35:
        cmp    dword ptr [esp + 48h], 0
        mov    ebp, 0
        je     SHORT loc_8_5A47
        mov    ecx, dword ptr [esp + 48h]
        mov    ebp, dword ptr [ecx]
loc_8_5A47:
        or     esi, esi
loc_8_5A49:
        je     SHORT loc_8_5A51
        mov    dword ptr [esi], 0
loc_8_5A51:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_5A64
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_5C59
loc_8_5A64:
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 38h]
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_5C35
        mov    eax, dword ptr [esp + 18h]
        push   1
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 40h]
        lea    eax, [esp + 20h]
        push   dword ptr [esp + 24h]
        push   eax
        call   sub_8_1FF0
        add    esp, 14h
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_5C35
        lea    eax, [esp + 20h]
        push   eax
loc_8_5AB5:
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 1ch]
        call   sub_8_22A8
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_5C35
        push   edi
        push   dword ptr [esp + 24h]
        call   sub_8_2366
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, -1
        jne    SHORT loc_8_5B1C
        or     edi, edi
        je     SHORT loc_8_5AF6
        cmp    byte ptr [edi], 0
        je     SHORT loc_8_5AF6
        mov    ebx, 0ffff0001h
        jmp    NEAR PTR loc_8_5C35
loc_8_5AF6:
        mov    edi, dword ptr [esp + 44h]
        or     edi, edi
        je     SHORT loc_8_5B05
        or     ebp, ebp
        je     SHORT loc_8_5B05
        mov    byte ptr [edi], 0
loc_8_5B05:
        inc    dword ptr [esp + 10h]
        or     esi, esi
        je     NEAR PTR loc_8_5C33
        mov    dword ptr [esi], 1
        jmp    NEAR PTR loc_8_5C33
loc_8_5B1C:
        or     esi, esi
        je     SHORT loc_8_5B24
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
loc_8_5B24:
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [eax + 1ch], 80000006h
        jne    NEAR PTR loc_8_5BC9
        mov    dword ptr [esp + 10h], ebp
        movzx  eax, word ptr [ebx + 8]
        push   dword ptr [esp + 1ch]
        lea    ebx, [ebx + eax + 0ch]
        push   dword ptr [esp + 18h]
        call   sub_8_70F0
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_5B60
        mov    ebx, 3f3h
        jmp    NEAR PTR loc_8_5C35
loc_8_5B60:
        mov    ecx, dword ptr [eax + 20h]
        mov    edx, dword ptr [ecx + 14h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_8_5B75
        mov    ebx, 3f3h
        jmp    NEAR PTR loc_8_5C35
loc_8_5B75:
        or     esi, esi
        je     SHORT loc_8_5B7E
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi], eax
loc_8_5B7E:
        test   byte ptr [ecx + 10h], 1
        mov    edi, dword ptr [esp + 44h]
        jne    SHORT loc_8_5BA5
        or     edi, edi
        jne    SHORT loc_8_5B97
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 10h], eax
        jmp    NEAR PTR loc_8_5C33
loc_8_5B97:
        cmp    dword ptr [ebx], ebp
        jbe    SHORT loc_8_5BA5
        mov    ebx, 0eah
        jmp    NEAR PTR loc_8_5C35
loc_8_5BA5:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esp + 28h], eax
        lea    eax, [esp + 10h]
        push   0
        push   eax
        push   edi
        lea    eax, [esp + 30h]
        push   1
        push   eax
        push   edx
        call   dword ptr [ecx]
        add    esp, 18h
        mov    ebx, eax
        or     edi, edi
        jne    SHORT loc_8_5C35
        jmp    SHORT loc_8_5C33
loc_8_5BC9:
        cmp    dword ptr [ebx], 1
        jne    SHORT loc_8_5BD2
        inc    dword ptr [esp + 10h]
loc_8_5BD2:
        movzx  eax, word ptr [ebx + 0ah]
        add    dword ptr [esp + 10h], eax
        mov    edi, dword ptr [esp + 44h]
        or     edi, edi
loc_8_5BE0:
        jne    SHORT loc_8_5BE4
        jmp    SHORT loc_8_5C33
loc_8_5BE4:
        cmp    dword ptr [esp + 10h], ebp
        jbe    SHORT loc_8_5BF1
loc_8_5BEA:
        mov    ebx, 0eah
        jmp    SHORT loc_8_5C35
loc_8_5BF1:
        push   1
        push   0
        push   0
        push   dword ptr [esp + 1ch]
        push   edi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5C0F
        mov    ebx, 57h
        jmp    SHORT loc_8_5C35
loc_8_5C0F:
        movzx  eax, word ptr [ebx + 0ah]
        push   eax
        movzx  eax, word ptr [ebx + 8]
        lea    eax, [ebx + eax + 0ch]
loc_8_5C1C:
        push   eax
        push   edi
        call   sub_8_366A
        add    esp, 0ch
        cmp    dword ptr [ebx], 1
        jne    SHORT loc_8_5C33
        movzx  eax, word ptr [ebx + 0ah]
        mov    byte ptr [edi + eax], 0
loc_8_5C33:
        sub    ebx, ebx
loc_8_5C35:
        cmp    dword ptr [esp + 48h], 0
        je     SHORT loc_8_5C52
        or     ebx, ebx
        je     SHORT loc_8_5C48
        cmp    ebx, 0eah
        jne    SHORT loc_8_5C52
loc_8_5C48:
        mov    ecx, dword ptr [esp + 48h]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
loc_8_5C52:
        call   sub_8_84FA
        mov    eax, ebx
loc_8_5C59:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_8_5C61:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        push   0
        push   0
        mov    esi, dword ptr [esp + 3ch]
        push   14h
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5C8B
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5E3F
loc_8_5C8B:
        push   0
        push   0
        mov    eax, dword ptr [esp + 40h]
        push   0
        shl    eax, 2
        mov    ebp, dword ptr [esp + 40h]
        push   eax
        push   ebp
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
loc_8_5CA8:
        jne    SHORT loc_8_5CB4
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5E3F
loc_8_5CB4:
        push   0
        push   0
        push   0
        mov    edi, dword ptr [esp + 48h]
        push   4
        push   edi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
loc_8_5CCB:
        jne    SHORT loc_8_5CD7
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5E3F
loc_8_5CD7:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_5CEA
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_5E3F
loc_8_5CEA:
        lea    eax, [esp + 20h]
        push   eax
loc_8_5CEF:
        push   80000006h
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_5E38
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 10h], eax
        test   byte ptr [eax + 11h], 80h
        je     SHORT loc_8_5D21
        mov    ebx, 5
        jmp    NEAR PTR loc_8_5E38
loc_8_5D21:
        push   201h
        push   18h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_5D43
loc_8_5D39:
        mov    ebx, 0eh
        jmp    NEAR PTR loc_8_5E38
loc_8_5D43:
        lea    eax, [esp + 14h]
        push   2
        push   eax
        push   dword ptr [esp + 30h]
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 2ch]
        push   eax
        call   sub_8_1FF0
        add    esp, 14h
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_8_5D79
        push   0
        push   dword ptr [esp + 1ch]
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_8_5E38
loc_8_5D79:
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_8_7157
        mov    dword ptr [esp + 24h], eax
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_5DAB
        push   0
        mov    ebx, 0eh
        push   dword ptr [esp + 1ch]
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_8_5E38
loc_8_5DAB:
        push   dword ptr [esp + 10h]
        call   sub_8_747A
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 4
        inc    dword ptr [ecx + 14h]
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    dword ptr [edi], ecx
        mov    ecx, 5
        mov    edi, dword ptr [esp + 18h]
        rep movsd
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 14h], ecx
        sub    esi, esi
        mov    ecx, dword ptr [esp + 1ch]
        cmp    dword ptr [esp + 38h], esi
        mov    dword ptr [ecx + 20h], eax
        je     SHORT loc_8_5E38
loc_8_5DEF:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp]
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_8_5E33
        push   0ch
        push   3
        lea    eax, [ebp + 4]
        push   eax
        push   dword ptr [ebp]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        call   sub_8_28AE
        add    esp, 18h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_5E38
        add    ebp, 10h
        inc    esi
        cmp    esi, dword ptr [esp + 38h]
        jb     SHORT loc_8_5DEF
        jmp    SHORT loc_8_5E38
loc_8_5E33:
        mov    ebx, 57h
loc_8_5E38:
        call   sub_8_84FA
        mov    eax, ebx
loc_8_5E3F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_8_5E47:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   0
        mov    esi, dword ptr [esp + 30h]
        push   4
        push   esi
        call   sub_8_1598
loc_8_5E5F:
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5E70
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5F4B
loc_8_5E70:
        push   1
        push   0
        push   0
        mov    edi, dword ptr [esp + 2ch]
        push   1
        push   edi
        call   sub_8_1598
loc_8_5E82:
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5E93
        mov    eax, 57h
        jmp    NEAR PTR loc_8_5F4B
loc_8_5E93:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_5EA6
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_5F4B
loc_8_5EA6:
        lea    eax, [esp + 0ch]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_5F44
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [eax + 1ch], 80000006h
        jne    SHORT loc_8_5ED5
        mov    ebx, 5
        jmp    SHORT loc_8_5F44
loc_8_5ED5:
        mov    eax, dword ptr [esp + 0ch]
        push   2
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esp + 18h]
        push   eax
        push   edi
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 18h]
        push   eax
loc_8_5EF1:
        call   sub_8_1FF0
        add    esp, 14h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_5F44
        push   dword ptr [esp + 10h]
        call   sub_8_747A
        add    esp, 4
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_8_7157
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_5F33
        push   dword ptr [esp + 10h]
        call   sub_8_7482
        add    esp, 4
        mov    ebx, 0eh
        jmp    SHORT loc_8_5F44
loc_8_5F33:
        mov    dword ptr [esi], eax
        inc    dword ptr [eax + 14h]
        mov    ecx, dword ptr [esp + 0ch]
        sub    ebx, ebx
        mov    ecx, dword ptr [ecx + 1ch]
        mov    dword ptr [eax + 1ch], ecx
loc_8_5F44:
        call   sub_8_84FA
loc_8_5F49:
        mov    eax, ebx
loc_8_5F4B:
        pop    edi
        pop    esi
loc_8_5F4D:
        pop    ebx
        add    esp, 0ch
        ret
sub_8_5F52:
        sub    esp, 10h
        push   esi
        push   edi
        push   1
        push   0
        push   0
        mov    esi, dword ptr [esp + 2ch]
        push   1
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_5F7A
        mov    eax, 57h
        jmp    NEAR PTR loc_8_608F
loc_8_5F7A:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_5F8D
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_608F
loc_8_5F8D:
        lea    eax, [esp + 8]
loc_8_5F91:
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_8_717D
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_8_6088
        or     esi, esi
        jne    SHORT loc_8_5FB6
        mov    edi, 57h
        jmp    NEAR PTR loc_8_6088
loc_8_5FB6:
        cmp    byte ptr [esi], 0
        jne    SHORT loc_8_5FD9
        db  081h, 07Ch, 024h, 008h, 048h, 006h, 000h, 000h
loc_8_5FC3:
        je     SHORT loc_8_5FCF
        db  081h, 07Ch, 024h, 008h, 020h, 006h, 000h, 000h
        jne    SHORT loc_8_5FD9
loc_8_5FCF:
        mov    edi, 5
        jmp    NEAR PTR loc_8_6088
loc_8_5FD9:
        mov    eax, dword ptr [esp + 8]
        push   1
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp + 14h]
        push   eax
        push   esi
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 14h]
loc_8_5FF4:
        push   eax
        call   sub_8_1FF0
        add    esp, 14h
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_8_6088
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        movzx  eax, word ptr [eax + 1ch]
        imul   eax, eax, 28h
        db  005h, 0D0h, 005h, 000h, 000h
        push   eax
        call   sub_8_8618
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_6031
        mov    edi, 5
        jmp    SHORT loc_8_6088
loc_8_6031:
        mov    eax, dword ptr [esp + 0ch]
        test   byte ptr [eax + 11h], 80h
        je     SHORT loc_8_6042
        mov    edi, 5
        jmp    SHORT loc_8_6088
loc_8_6042:
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   esi
loc_8_604C:
        movzx  eax, word ptr [eax + 1ch]
        imul   eax, eax, 28h
        db  005h, 0D0h, 005h, 000h, 000h
        push   eax
        call   sub_8_8618
        add    esp, 0ch
        or     eax, eax
        mov    edi, 5
        je     SHORT loc_8_6088
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_8_249C
        add    esp, 8
        mov    edi, eax
        push   dword ptr [esp + 0ch]
        call   sub_8_52BC
        add    esp, 4
loc_8_6088:
        call   sub_8_84FA
        mov    eax, edi
loc_8_608F:
        pop    edi
        pop    esi
        add    esp, 10h
        ret
sub_8_6095:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   1
        push   0
        push   0
        mov    esi, dword ptr [esp + 28h]
        push   1
        push   esi
loc_8_60A8:
        call   sub_8_1598
        add    esp, 14h
loc_8_60B0:
        or     eax, eax
        jne    SHORT loc_8_60BE
        mov    eax, 57h
        jmp    NEAR PTR loc_8_6144
loc_8_60BE:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_60CE
        mov    eax, 0a7h
        jmp    SHORT loc_8_6144
loc_8_60CE:
        lea    eax, [esp + 0ch]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_613D
        mov    eax, dword ptr [esp + 0ch]
        mov    edi, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [edi + 24h]
        add    ecx, dword ptr [eax + 10h]
        db  08Dh, 044h
loc_8_60F4:
        and    al, 10h
        push   eax
        push   ecx
        push   edi
        call   sub_8_22A8
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
loc_8_6105:
        jne    SHORT loc_8_613D
        test   byte ptr [edi + 11h], 80h
        jne    SHORT loc_8_6138
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [eax + 1ch], 80000006h
        je     SHORT loc_8_6138
        push   esi
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_8_240A
        add    esp, 0ch
        cmp    eax, -1
        jne    SHORT loc_8_6134
        mov    ebx, 2
        jmp    SHORT loc_8_613D
loc_8_6134:
        sub    ebx, ebx
        jmp    SHORT loc_8_613D
loc_8_6138:
        mov    ebx, 5
loc_8_613D:
        call   sub_8_84FA
        mov    eax, ebx
loc_8_6144:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_8_614B:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 34h]
        or     edi, edi
        push   0
loc_8_615A:
        push   0
        push   0
        jne    SHORT loc_8_617D
        mov    esi, dword ptr [esp + 3ch]
        push   1
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_6199
        mov    eax, 57h
        jmp    NEAR PTR loc_8_6285
loc_8_617D:
        mov    esi, dword ptr [esp + 3ch]
        push   edi
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
loc_8_618D:
        jne    SHORT loc_8_6199
        mov    eax, 57h
        jmp    NEAR PTR loc_8_6285
loc_8_6199:
        push   1
        push   0
        push   0
        mov    ebx, dword ptr [esp + 30h]
        push   1
        push   ebx
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_61BC
        mov    eax, 57h
        jmp    NEAR PTR loc_8_6285
loc_8_61BC:
        push   1
        push   0
loc_8_61C0:
        push   0
        push   1
        push   dword ptr [esp + 38h]
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_61DE
        mov    eax, 57h
        jmp    NEAR PTR loc_8_6285
loc_8_61DE:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_61F1
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_6285
loc_8_61F1:
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_8_717D
        add    esp, 8
        mov    ebp, eax
loc_8_6204:
        or     ebp, ebp
        jne    SHORT loc_8_627E
        mov    eax, dword ptr [esp + 14h]
        push   2
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 14h], eax
loc_8_6215:
        lea    eax, [esp + 1ch]
loc_8_6219:
        push   eax
        push   ebx
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 20h]
        push   eax
        call   sub_8_1FF0
        add    esp, 14h
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_8_627E
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax + 11h], 80h
        jne    SHORT loc_8_6279
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 1ch], 80000006h
        je     SHORT loc_8_6279
        mov    ebx, dword ptr [esp + 2ch]
        cmp    ebx, 1
        jne    SHORT loc_8_625E
        push   esi
        call   sub_8_3651
        add    esp, 4
        movzx  edi, ax
loc_8_625E:
        push   edi
        push   ebx
        push   esi
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        call   sub_8_28AE
        add    esp, 18h
        mov    ebp, eax
        jmp    SHORT loc_8_627E
loc_8_6279:
        mov    ebp, 5
loc_8_627E:
        call   sub_8_84FA
        mov    eax, ebp
loc_8_6285:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_8_628D:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    edi, dword ptr [esp + 2ch]
        push   0
        mov    esi, dword ptr [esp + 2ch]
        push   edi
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_62B6
        mov    eax, 57h
        jmp    SHORT loc_8_6300
loc_8_62B6:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_62C6
        mov    eax, 0a7h
        jmp    SHORT loc_8_6300
loc_8_62C6:
        lea    eax, [esp + 0ch]
        push   eax
loc_8_62CB:
        push   dword ptr [esp + 1ch]
loc_8_62CF:
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_62F9
        mov    dword ptr [esp + 10h], edi
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 18h]
        call   sub_8_2B72
        add    esp, 10h
        mov    ebx, eax
loc_8_62F9:
        call   sub_8_84FA
        mov    eax, ebx
loc_8_6300:
        pop    edi
        pop    esi
        pop    ebx
loc_8_6303:
        add    esp, 8
        ret
sub_8_6307:
        sub    esp, 18h
        push   ebx
loc_8_630B:
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        push   1
        push   0
        push   0
        mov    esi, dword ptr [esp + 4ch]
        push   4
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_6339
        mov    eax, 57h
        jmp    NEAR PTR loc_8_655B
loc_8_6339:
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [esp + 48h]
        call   sub_8_1598
        add    esp, 14h
loc_8_634D:
        or     eax, eax
        jne    SHORT loc_8_635B
        mov    eax, 57h
        jmp    NEAR PTR loc_8_655B
loc_8_635B:
        push   0
        push   0
        mov    eax, dword ptr [esp + 40h]
        push   0
        push   dword ptr [eax]
        push   dword ptr [esp + 44h]
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_6381
        mov    eax, 57h
        jmp    NEAR PTR loc_8_655B
loc_8_6381:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_6394
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_655B
loc_8_6394:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 30h]
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_6554
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    ebx, dword ptr [ecx + 24h]
        add    ebx, dword ptr [eax + 10h]
        lea    eax, [esp + 18h]
        push   eax
        push   ebx
        push   ecx
        call   sub_8_22A8
        add    esp, 0ch
loc_8_63CB:
        or     eax, eax
        je     SHORT loc_8_63D9
        mov    ebx, 103h
        jmp    NEAR PTR loc_8_6554
loc_8_63D9:
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 1ch]
        call   sub_8_2CE9
        add    esp, 8
        mov    edi, eax
        cmp    edi, -1
        jne    SHORT loc_8_63FA
        mov    ebx, 103h
        jmp    NEAR PTR loc_8_6554
loc_8_63FA:
        or     esi, esi
        je     SHORT loc_8_6402
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
loc_8_6402:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 1ch], 80000006h
        jne    NEAR PTR loc_8_64AD
        movzx  eax, word ptr [edi + 8]
        lea    ebp, [edi + eax + 0ch]
        mov    eax, dword ptr [esp + 10h]
        push   ebx
        push   dword ptr [eax + 0ch]
        call   sub_8_70F0
        add    esp, 8
        cmp    eax, -1
loc_8_642E:
        jne    SHORT loc_8_643A
        mov    ebx, 3f3h
        jmp    NEAR PTR loc_8_6554
loc_8_643A:
        mov    eax, dword ptr [eax + 20h]
        mov    edx, dword ptr [eax + 14h]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_8_644F
        mov    ebx, 3f3h
        jmp    NEAR PTR loc_8_6554
loc_8_644F:
        or     esi, esi
        je     SHORT loc_8_6458
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi], ecx
loc_8_6458:
        test   byte ptr [eax + 10h], 1
        mov    esi, dword ptr [esp + 44h]
        jne    SHORT loc_8_6489
        or     esi, esi
        jne    SHORT loc_8_6474
        mov    ecx, dword ptr [esp + 48h]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
loc_8_646F:
        jmp    NEAR PTR loc_8_650F
loc_8_6474:
        mov    ecx, dword ptr [esp + 48h]
        mov    ebx, dword ptr [ecx]
        cmp    dword ptr [ebp], ebx
        jbe    SHORT loc_8_648D
        mov    ebx, 0eah
        jmp    NEAR PTR loc_8_6554
loc_8_6489:
        mov    ecx, dword ptr [esp + 48h]
loc_8_648D:
        mov    ebx, dword ptr [ebp + 4]
        mov    dword ptr [esp + 20h], ebx
        push   0
        push   ecx
loc_8_6497:
        push   esi
        lea    ecx, [esp + 28h]
        push   1
        push   ecx
        push   edx
        call   dword ptr [eax]
        add    esp, 18h
        mov    ebx, eax
        or     esi, esi
        jne    SHORT loc_8_6511
        jmp    SHORT loc_8_650F
loc_8_64AD:
        mov    ecx, dword ptr [esp + 48h]
        or     ecx, ecx
        je     SHORT loc_8_64C9
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esp + 14h], eax
        movzx  eax, word ptr [edi + 0ah]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [edi], 1
        jne    SHORT loc_8_64C9
        inc    eax
        mov    dword ptr [ecx], eax
loc_8_64C9:
        mov    esi, dword ptr [esp + 44h]
        or     esi, esi
        je     SHORT loc_8_650F
        or     ecx, ecx
        jne    SHORT loc_8_64DC
        mov    ebx, 57h
        jmp    SHORT loc_8_6554
loc_8_64DC:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [ecx], eax
        jbe    SHORT loc_8_64EB
        mov    ebx, 0eah
        jmp    SHORT loc_8_6554
loc_8_64EB:
        movzx  eax, word ptr [edi + 0ah]
        push   eax
        movzx  eax, word ptr [edi + 8]
        lea    eax, [edi + eax + 0ch]
        push   eax
        push   esi
        call   sub_8_366A
        add    esp, 0ch
        cmp    dword ptr [edi], 1
        jne    SHORT loc_8_650F
        movzx  eax, word ptr [edi + 0ah]
        mov    byte ptr [esi + eax], 0
loc_8_650F:
        sub    ebx, ebx
loc_8_6511:
        or     ebx, ebx
        jne    SHORT loc_8_6554
        mov    eax, dword ptr [esp + 38h]
        movzx  esi, word ptr [edi + 8]
        mov    eax, dword ptr [eax]
        lea    ecx, [esi + 1]
        cmp    ecx, eax
loc_8_6524:
        jbe    SHORT loc_8_6532
        mov    ebx, 0eah
        or     eax, eax
        je     SHORT loc_8_6554
        lea    esi, [eax - 1]
loc_8_6532:
        push   esi
        add    edi, 0ch
        push   edi
        push   dword ptr [esp + 3ch]
        call   sub_8_366A
        add    dword ptr [esp + 40h], esi
        mov    eax, dword ptr [esp + 40h]
        add    esp, 0ch
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [eax], esi
loc_8_6554:
        call   sub_8_84FA
        mov    eax, ebx
loc_8_655B:
        pop    ebp
loc_8_655C:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  083h, 0ECh, 00Ch, 053h, 056h, 057h, 055h, 06Ah, 001h, 06Ah, 000h, 06Ah, 000h, 06Ah, 004h, 0FFh
        db  074h, 024h, 034h, 0E8h, 01Dh, 0B0h, 0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 00Ah, 0B8h
        db  057h, 000h, 000h, 000h, 0E9h, 0A0h, 001h, 000h, 000h, 06Ah, 001h, 06Ah, 000h
loc_8_6590:
        push   0
        db  08Bh, 074h
loc_8_6594:
        and    al, 38h
        push   4
loc_8_6598:
        push   esi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_65AF
        mov    eax, 57h
        jmp    NEAR PTR loc_8_672C
loc_8_65AF:
        push   1
        push   0
        push   0
        mov    edi, dword ptr [esp + 3ch]
        push   4
        push   edi
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_65D2
        mov    eax, 57h
        jmp    NEAR PTR loc_8_672C
loc_8_65D2:
        push   1
        push   0
loc_8_65D6:
        push   0
        push   4
        push   dword ptr [esp + 44h]
        call   sub_8_1598
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_65F4
        mov    eax, 57h
        jmp    NEAR PTR loc_8_672C
loc_8_65F4:
        call   sub_8_84CD
        or     eax, eax
        jne    SHORT loc_8_6607
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_672C
loc_8_6607:
        lea    eax, [esp + 14h]
loc_8_660B:
        push   eax
        push   dword ptr [esp + 24h]
loc_8_6610:
        call   sub_8_717D
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_6725
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 24h]
        add    edx, dword ptr [eax + 10h]
        lea    eax, [esp + 18h]
        push   eax
        push   edx
        push   ecx
        call   sub_8_22A8
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_6725
        mov    eax, dword ptr [esp + 18h]
        or     esi, esi
        db  00Fh, 0B7h
loc_8_6650:
        pop    eax
        push   cs
        je     SHORT loc_8_6656
        mov    dword ptr [esi], ebx
loc_8_6656:
        or     edi, edi
        jne    SHORT loc_8_6661
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_8_66B6
loc_8_6661:
        sub    esi, esi
        cmp    ebx, esi
        je     SHORT loc_8_66A3
        sub    ebx, ebx
        sub    bp, bp
loc_8_666C:
        movzx  eax, bp
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_8_2CE9
        add    esp, 8
        cmp    eax, -1
        je     SHORT loc_8_66A3
        movzx  ecx, word ptr [eax + 8]
        cmp    esi, ecx
        jae    SHORT loc_8_668B
        mov    esi, ecx
loc_8_668B:
        mov    cx, word ptr [eax + 0ah]
        cmp    dword ptr [eax], 1
        jne    SHORT loc_8_6696
        inc    cx
loc_8_6696:
        movzx  eax, cx
        cmp    ebx, eax
        jae    SHORT loc_8_669F
        mov    ebx, eax
loc_8_669F:
        inc    bp
        jmp    SHORT loc_8_666C
loc_8_66A3:
        or     edi, edi
        je     SHORT loc_8_66A9
        mov    dword ptr [edi], esi
loc_8_66A9:
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_8_66B6
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [eax], ebx
loc_8_66B6:
        cmp    dword ptr [esp + 24h], 0
        mov    esi, dword ptr [esp + 28h]
        jne    SHORT loc_8_66C5
        or     esi, esi
        je     SHORT loc_8_6723
loc_8_66C5:
        sub    edi, edi
        sub    bp, bp
loc_8_66CA:
        mov    dword ptr [esp + 10h], 0
        lea    eax, [esp + 10h]
        push   eax
        push   0
        movzx  eax, bp
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_8_2B72
        add    esp, 10h
        mov    ebx, eax
        cmp    ebx, 103h
        je     SHORT loc_8_670D
        or     ebx, ebx
        je     SHORT loc_8_66FF
        cmp    ebx, 0eah
        jne    SHORT loc_8_6725
loc_8_66FF:
        cmp    dword ptr [esp + 10h], edi
        jbe    SHORT loc_8_6709
        mov    edi, dword ptr [esp + 10h]
loc_8_6709:
        inc    bp
        jmp    SHORT loc_8_66CA
loc_8_670D:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_8_671D
        mov    eax, dword ptr [esp + 24h]
        movzx  ecx, bp
        mov    dword ptr [eax], ecx
loc_8_671D:
        or     esi, esi
        je     SHORT loc_8_6723
        mov    dword ptr [esi], edi
loc_8_6723:
        sub    ebx, ebx
loc_8_6725:
        call   sub_8_84FA
        mov    eax, ebx
loc_8_672C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  083h, 0ECh, 004h, 056h, 08Bh, 074h, 024h, 00Ch, 081h, 0FEh, 04Dh, 057h, 04Ah, 048h, 075h, 00Dh
        db  066h, 0C7h, 005h, 0C8h, 003h, 000h, 000h, 001h, 000h, 02Bh, 0C0h, 0EBh, 04Eh, 081h, 0FEh, 04Dh
        db  057h, 050h, 053h, 075h, 00Ah, 0E8h, 03Fh, 016h, 000h, 000h, 0BEh, 002h, 000h, 000h, 080h, 0E8h
        db  065h, 01Dh, 000h, 000h, 00Bh, 0C0h, 075h, 007h, 0B8h, 0A7h, 000h, 000h, 000h, 0EBh, 02Ch, 08Dh
        db  044h, 024h, 004h, 050h, 056h, 0E8h, 0FFh, 009h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0F0h, 00Bh
        db  0F6h, 075h, 011h, 08Bh, 044h, 024h, 004h, 0FFh, 070h, 00Ch, 0E8h, 095h, 0D5h, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 08Bh, 0F0h, 0E8h, 05Dh, 01Dh, 000h, 000h, 08Bh, 0C6h, 05Eh, 083h, 0C4h, 004h, 0C3h
        db  081h, 0ECh, 030h, 001h, 000h, 000h, 053h, 056h, 057h, 055h, 0C7h, 044h, 024h, 024h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 020h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 018h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0C7h, 044h, 024h, 014h, 0FFh, 0FFh, 0FFh, 0FFh, 0C7h, 044h, 024h, 010h, 0FFh, 0FFh
        db  0FFh, 0FFh, 06Ah, 001h, 06Ah, 000h, 06Ah, 000h, 08Bh, 0BCh, 024h, 054h, 001h, 000h, 000h, 06Ah
        db  001h, 057h, 0E8h, 0ADh, 0ADh, 0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 00Ah, 0B8h, 057h
        db  000h, 000h, 000h, 0E9h, 08Fh, 003h, 000h, 000h, 06Ah, 001h, 06Ah, 000h, 06Ah, 000h, 06Ah, 001h
        db  0FFh, 0B4h, 024h, 05Ch, 001h, 000h, 000h, 0E8h, 088h, 0ADh, 0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh
        db  0C0h, 075h, 00Ah, 0B8h, 057h, 000h, 000h, 000h, 0E9h, 06Ah, 003h, 000h, 000h, 06Ah, 001h, 06Ah
        db  000h, 06Ah, 000h, 08Bh, 09Ch, 024h, 05Ch, 001h, 000h, 000h, 06Ah, 001h, 053h, 0E8h, 062h, 0ADh
        db  0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 00Ah, 0B8h, 057h, 000h, 000h, 000h, 0E9h, 044h
        db  003h, 000h, 000h, 0E8h, 081h, 01Ch, 000h, 000h, 00Bh, 0C0h, 075h, 00Ah, 0B8h, 0A7h, 000h, 000h
        db  000h, 0E9h, 031h, 003h, 000h, 000h, 08Dh, 044h, 024h, 038h, 050h, 0BEh, 001h, 000h, 000h, 000h
        db  0FFh, 0B4h, 024h, 048h, 001h, 000h, 000h, 0E8h, 00Dh, 009h, 000h, 000h, 083h, 0C4h, 008h, 08Bh
        db  0E8h, 00Bh, 0EDh, 00Fh, 085h, 051h, 002h, 000h, 000h, 08Bh, 044h, 024h, 038h, 056h, 08Bh, 040h
        db  00Ch, 089h, 044h, 024h, 020h, 08Dh, 044h, 024h, 040h, 050h, 057h, 08Dh, 044h, 024h, 028h, 0FFh
        db  074h, 024h, 044h, 050h, 0E8h, 053h, 0B7h, 0FFh, 0FFh, 083h, 0C4h, 014h, 08Bh, 0E8h, 00Bh, 0EDh
        db  00Fh, 085h, 024h, 002h, 000h, 000h, 068h, 001h, 002h, 000h, 000h, 068h, 000h, 001h, 000h, 000h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h, 089h, 044h, 024h, 034h, 083h, 0C4h, 008h, 083h, 0F8h, 0FFh
        db  00Fh, 084h, 0FFh, 001h, 000h, 000h, 068h, 001h, 002h, 000h, 000h, 068h, 000h, 040h, 000h, 000h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h, 089h, 044h, 024h, 038h, 083h, 0C4h, 008h, 083h, 0F8h, 0FFh
        db  00Fh, 084h, 0DFh, 001h, 000h, 000h, 08Bh, 044h, 024h, 03Ch, 08Bh, 040h, 00Ch, 083h, 0F8h, 0FFh
        db  00Fh, 084h, 0C8h, 001h, 000h, 000h, 039h, 005h, 058h, 006h, 000h, 000h, 074h, 00Ch, 039h, 005h
        db  030h, 006h, 000h, 000h, 00Fh, 085h, 0B4h, 001h, 000h, 000h, 00Bh, 0DBh, 074h, 056h, 0FFh, 074h
        db  024h, 01Ch, 0E8h, 00Dh, 0D4h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E8h, 0D7h, 01Bh, 000h, 000h, 08Dh
        db  044h, 024h, 034h, 050h, 057h, 02Bh, 0F6h, 0FFh, 0B4h, 024h, 04Ch, 001h, 000h, 000h, 0E8h, 0F9h
        db  0E9h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 08Bh, 0E8h, 03Bh, 0EEh, 00Fh, 085h, 08Ah, 001h, 000h, 000h
        db  056h, 053h, 0FFh, 074h, 024h, 03Ch, 0E8h, 0EEh, 005h, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 0E8h
        db  0FFh, 074h, 024h, 034h, 0E8h, 0C6h, 0EAh, 0FFh, 0FFh, 083h, 0C4h, 004h, 03Bh, 0EEh, 00Fh, 085h
        db  066h, 001h, 000h, 000h, 00Bh, 0F6h, 074h, 005h, 0E8h, 089h, 01Bh, 000h, 000h, 02Bh, 0F6h, 0FFh
        db  0B4h, 024h, 04Ch, 001h, 000h, 000h, 068h, 0E4h, 003h, 000h, 000h, 068h, 003h, 000h, 000h, 080h
        db  0E8h, 047h, 003h, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 0E8h, 03Bh, 0EEh, 00Fh, 085h, 038h, 001h
        db  000h, 000h, 0C7h, 044h, 024h, 024h, 001h, 000h, 000h, 000h, 08Dh, 044h, 024h, 018h, 050h, 068h
        db  0E4h, 003h, 000h, 000h, 068h, 003h, 000h, 000h, 080h, 0E8h, 07Eh, 0E9h, 0FFh, 0FFh, 083h, 0C4h
        db  00Ch, 08Bh, 0E8h, 03Bh, 0EEh, 00Fh, 085h, 00Fh, 001h, 000h, 000h, 08Dh, 044h, 024h, 040h, 06Ah
        db  072h, 050h, 08Bh, 044h, 024h, 024h, 0FFh, 070h, 014h, 0E8h, 0F7h, 0E2h, 0FFh, 0FFh, 083h, 0C4h
        db  00Ch, 08Dh, 044h, 024h, 040h, 056h, 050h, 0E8h, 096h, 0B5h, 0FFh, 0FFh, 089h, 044h, 024h, 030h
        db  083h, 0C4h, 008h, 03Bh, 0C6h, 075h, 022h, 08Dh, 044h, 024h, 040h, 050h, 08Bh, 044h, 024h, 020h
        db  0FFh, 070h, 014h, 0E8h, 0E9h, 0E4h, 0FFh, 0FFh, 083h, 0C4h, 008h, 03Bh, 0C6h, 074h, 00Ah, 0BDh
        db  0F5h, 003h, 000h, 000h, 0E9h, 0C1h, 000h, 000h, 000h, 08Dh, 044h, 024h, 040h, 050h, 068h, 0D8h
        db  003h, 000h, 000h, 068h, 003h, 000h, 000h, 080h, 0E8h, 0AFh, 002h, 000h, 000h, 083h, 0C4h, 00Ch
        db  08Bh, 0E8h, 00Bh, 0EDh, 00Fh, 085h, 0A0h, 000h, 000h, 000h, 0C7h, 044h, 024h, 020h, 001h, 000h
        db  000h, 000h, 08Dh, 044h, 024h, 014h, 050h, 068h, 0D8h, 003h, 000h, 000h, 068h, 003h, 000h, 000h
        db  080h, 0E8h, 0E6h, 0E8h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 08Bh, 0E8h, 00Bh, 0EDh, 075h, 07Bh, 08Dh
        db  044h, 024h, 010h, 050h, 057h, 0FFh, 074h, 024h, 01Ch, 0E8h, 0CEh, 0E8h, 0FFh, 0FFh, 083h, 0C4h
        db  00Ch, 00Bh, 0C0h, 075h, 019h, 0FFh, 074h, 024h, 010h, 0E8h, 0B1h, 0E9h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 057h, 0FFh, 074h, 024h, 018h, 0E8h, 0D3h, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 008h, 08Dh, 044h
        db  024h, 010h, 050h, 057h, 0FFh, 074h, 024h, 01Ch, 0E8h, 0B6h, 0F3h, 0FFh, 0FFh, 083h, 0C4h, 00Ch
        db  08Bh, 0E8h, 00Bh, 0EDh, 075h, 034h, 0FFh, 074h, 024h, 030h, 0FFh, 074h, 024h, 030h, 0FFh, 074h
        db  024h, 018h, 0FFh, 074h, 024h, 024h, 0E8h, 0BAh, 01Eh, 000h, 000h, 083h, 0C4h, 010h, 0FFh, 074h
        db  024h, 010h, 0E8h, 079h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 0E8h, 0EBh, 00Ch, 0BDh, 057h
        db  000h, 000h, 000h, 0EBh, 005h, 0BDh, 00Eh, 000h, 000h, 000h, 00Bh, 0EDh, 074h, 043h, 083h, 07Ch
        db  024h, 028h, 000h, 075h, 022h, 08Dh, 044h, 024h, 040h, 06Ah, 000h, 050h, 0E8h, 091h, 0B4h, 0FFh
        db  0FFh, 083h, 0C4h, 008h, 00Bh, 0C0h, 074h, 00Fh, 08Dh, 044h, 024h, 040h, 06Ah, 000h, 050h, 0E8h
        db  0ABh, 0B3h, 0FFh, 0FFh, 083h, 0C4h, 008h, 06Ah, 000h, 053h, 0E8h, 073h, 0B4h, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 00Bh, 0C0h, 074h, 00Bh, 06Ah, 000h, 053h, 0E8h, 091h, 0B3h, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 00Bh, 0F6h, 074h, 005h, 0E8h, 0DCh, 019h, 000h, 000h, 083h, 07Ch, 024h, 018h, 0FFh, 074h
        db  00Ch, 0FFh, 074h, 024h, 018h, 0E8h, 0F5h, 0E8h, 0FFh, 0FFh, 083h, 0C4h, 004h, 083h, 07Ch, 024h
        db  010h, 0FFh, 074h, 00Ch, 0FFh, 074h, 024h, 010h, 0E8h, 0E2h, 0E8h, 0FFh, 0FFh, 083h, 0C4h, 004h
        db  083h, 07Ch, 024h, 014h, 0FFh, 074h, 00Ch, 0FFh, 074h, 024h, 014h, 0E8h, 0CFh, 0E8h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 083h, 07Ch, 024h, 024h, 000h, 074h, 012h, 068h, 0E4h, 003h, 000h, 000h, 068h
        db  003h, 000h, 000h, 080h, 0E8h, 0B4h, 002h, 000h, 000h, 083h, 0C4h, 008h, 083h, 07Ch, 024h, 020h
        db  000h, 074h, 012h, 068h, 0D8h, 003h, 000h, 000h, 068h, 003h, 000h, 000h, 080h, 0E8h, 09Bh, 002h
        db  000h, 000h, 083h, 0C4h, 008h, 08Bh, 0C5h, 05Dh, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 030h, 001h, 000h
        db  000h, 0C3h, 083h, 0ECh, 004h, 056h, 0E8h, 02Eh, 019h, 000h, 000h, 00Bh, 0C0h, 075h, 00Ah, 0B8h
        db  0A7h, 000h, 000h, 000h, 0E9h, 01Eh, 001h, 000h, 000h, 08Dh, 044h, 024h, 004h, 050h, 0FFh, 074h
        db  024h, 010h, 0E8h, 0C2h, 005h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0F0h, 00Bh, 0F6h, 00Fh, 085h
        db  0FCh, 000h, 000h, 000h, 08Bh, 044h, 024h, 010h, 03Dh, 001h, 000h, 000h, 080h, 075h, 04Fh, 08Bh
        db  044h, 024h, 004h, 081h, 078h, 01Ch, 003h, 000h, 000h, 080h, 00Fh, 085h, 0E0h, 000h, 000h, 000h
        db  068h, 001h, 000h, 000h, 080h, 0E8h, 05Eh, 009h, 000h, 000h, 08Bh, 044h
loc_8_6BF0:
        and    al, 8
        add    esp, 4
        mov    eax, dword ptr [eax + 10h]
        db  0A3h, 008h, 006h, 000h, 000h
        db  0C7h, 005h, 00Ch, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 005h, 010h, 006h, 000h, 000h, 000h, 040h, 000h, 000h
        mov    eax, dword ptr [eax + 0ch]
        db  0A3h, 004h, 006h, 000h, 000h
        jmp    NEAR PTR loc_8_6CC2
        db  03Dh, 005h, 000h, 000h, 080h, 075h, 04Ch, 08Bh, 044h, 024h, 004h, 081h, 078h, 01Ch, 002h, 000h
        db  000h, 080h, 00Fh, 085h, 08Ah, 000h, 000h, 000h, 068h, 005h, 000h, 000h, 080h, 0E8h, 008h, 009h
        db  000h, 000h, 08Bh, 044h, 024h, 008h, 083h, 0C4h, 004h, 08Bh, 040h, 010h, 0A3h, 0A8h, 006h, 000h
        db  000h, 08Bh, 044h, 024h, 004h, 0C7h, 005h, 0ACh, 006h, 000h, 000h, 001h, 000h, 000h, 000h, 08Bh
        db  040h, 00Ch, 0A3h, 0A4h, 006h, 000h, 000h, 0C7h, 005h, 0B0h, 006h, 000h, 000h, 000h, 040h, 000h
        db  000h, 0EBh, 04Dh, 03Dh, 006h, 000h, 000h, 080h, 075h, 048h, 08Bh, 044h, 024h, 004h, 081h, 078h
        db  01Ch, 006h, 000h, 000h, 080h, 075h, 03Bh, 068h, 006h, 000h, 000h, 080h, 0E8h, 0B9h, 008h, 000h
        db  000h, 08Bh, 044h, 024h, 008h, 083h, 0C4h, 004h, 08Bh, 040h, 010h, 0A3h, 0D0h, 006h, 000h, 000h
        db  08Bh, 044h, 024h, 004h, 0C7h, 005h, 0D4h, 006h, 000h, 000h, 001h, 000h, 000h, 000h, 08Bh, 040h
        db  00Ch, 0A3h, 0CCh, 006h, 000h, 000h, 0C7h, 005h, 0D8h, 006h, 000h, 000h, 000h, 040h, 000h, 000h
loc_8_6CC2:
        sub    esi, esi
        call   sub_8_84FA
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret
        db  083h, 0ECh, 008h, 053h, 056h, 057h, 055h, 06Ah, 001h, 06Ah, 000h, 06Ah, 000h, 08Bh, 07Ch, 024h
        db  02Ch, 06Ah, 001h, 057h, 0E8h, 0AFh, 0A8h, 0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 00Ah
        db  0B8h, 057h, 000h, 000h, 000h, 0E9h, 01Fh, 001h, 000h, 000h, 06Ah, 001h, 06Ah, 000h, 06Ah, 000h
        db  06Ah, 001h, 0FFh, 074h, 024h, 034h, 0E8h, 08Dh, 0A8h, 0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh, 0C0h
        db  075h, 00Ah, 0B8h, 057h, 000h, 000h, 000h, 0E9h, 0FDh, 000h, 000h, 000h, 08Bh, 074h, 024h, 01Ch
        db  081h, 0FEh, 002h, 000h, 000h, 080h, 074h, 01Ah, 081h, 0FEh, 003h, 000h, 000h, 080h, 074h, 012h
        db  081h, 0FEh, 006h, 000h, 000h, 080h, 074h, 012h, 0B8h, 0F2h, 003h, 000h, 000h, 0E9h, 0D7h, 000h
        db  000h, 000h, 081h, 0FEh, 006h, 000h, 000h, 080h, 075h, 007h, 0BBh, 0C0h, 006h, 000h, 000h, 0EBh
        db  017h, 08Dh, 086h, 0FEh, 0FFh, 0FFh, 07Fh, 083h, 0F8h, 001h, 01Bh, 0C0h, 083h, 0C0h, 003h, 06Bh
        db  0C0h, 028h, 08Dh, 098h, 0D0h, 005h, 000h, 000h, 0F6h, 043h, 019h, 020h, 075h, 00Ah, 0B8h, 0F2h
        db  003h, 000h, 000h, 0E9h, 0A1h, 000h, 000h, 000h, 0E8h, 050h, 017h, 000h, 000h, 00Bh, 0C0h, 075h
        db  00Ah, 0B8h, 0A7h, 000h, 000h, 000h, 0E9h, 08Eh, 000h, 000h, 000h, 08Bh, 043h, 00Ch, 089h, 044h
        db  024h, 010h, 08Dh, 044h, 024h, 014h, 06Ah, 001h, 050h, 057h, 08Dh, 044h, 024h, 01Ch, 053h, 050h
        db  0E8h, 04Bh, 0B2h, 0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 007h, 0BEh, 0F2h, 003h, 000h
        db  000h, 0EBh, 05Fh, 0E8h, 0DCh, 007h, 000h, 000h, 08Bh, 0E8h, 0E8h, 0CBh, 006h, 000h, 000h, 089h
        db  044h, 024h, 010h, 081h, 0FEh, 006h, 000h, 000h, 080h, 075h, 007h, 081h, 048h, 010h, 000h, 001h
        db  000h, 000h, 0FFh, 074h, 024h, 010h, 0FFh, 074h, 024h, 028h, 0E8h, 041h, 0C9h, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 08Bh, 0F0h, 00Bh, 0F6h, 075h, 015h, 0FFh, 074h, 024h, 010h, 055h, 057h, 053h, 0E8h
        db  0B9h, 018h, 000h, 000h, 083h, 0C4h, 010h, 08Bh, 0F0h, 00Bh, 0F6h, 074h, 015h, 0FFh, 074h, 024h
        db  010h, 0E8h, 009h, 007h, 000h, 000h, 083h, 0C4h, 004h, 055h, 0E8h, 0D8h, 007h, 000h, 000h, 083h
        db  0C4h, 004h, 0E8h, 0E3h, 016h, 000h, 000h, 08Bh, 0C6h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 008h
        db  0C3h, 083h, 0ECh, 004h, 053h, 056h, 057h, 02Bh, 0F6h, 08Bh, 044h, 024h, 014h, 03Dh, 002h, 000h
        db  000h, 080h, 074h, 011h, 03Dh, 003h, 000h, 000h, 080h, 074h, 00Ah, 0B8h, 0F2h, 003h, 000h, 000h
        db  0E9h, 0F1h, 000h, 000h, 000h, 005h, 0FEh, 0FFh, 0FFh, 07Fh, 083h, 0F8h, 001h, 01Bh, 0C0h, 083h
        db  0C0h, 003h, 06Bh, 0C0h, 028h, 08Dh, 0B8h, 0D0h, 005h, 000h, 000h, 0F6h, 047h, 019h, 020h, 075h
        db  00Ah, 0B8h, 0F2h, 003h, 000h, 000h, 0E9h, 0CBh, 000h, 000h, 000h, 06Ah, 001h, 06Ah, 000h, 06Ah
        db  000h, 08Bh, 05Ch, 024h, 024h, 06Ah, 001h, 053h, 0E8h, 01Bh, 0A7h, 0FFh, 0FFh, 083h, 0C4h, 014h
        db  00Bh, 0C0h, 075h, 00Ah, 0B8h, 057h, 000h, 000h, 000h, 0E9h, 0A8h, 000h, 000h, 000h, 0E8h, 03Ah
        db  016h, 000h, 000h, 00Bh, 0C0h, 075h, 00Ah, 0B8h, 0A7h, 000h, 000h, 000h, 0E9h, 095h, 000h, 000h
        db  000h, 08Dh, 044h, 024h, 00Ch, 050h, 053h, 057h, 0E8h, 06Bh, 017h, 000h, 000h, 083h, 0C4h, 00Ch
        db  00Bh, 0C0h, 074h, 007h, 0BEh, 0F2h, 003h, 000h, 000h, 0EBh, 074h, 08Bh, 044h, 024h, 00Ch, 0FFh
        db  070h, 00Ch, 0E8h, 0A9h, 006h, 000h, 000h, 08Bh, 044h, 024h, 010h, 083h, 0C4h, 004h, 08Bh, 00Dh
        db  004h, 006h, 000h, 000h, 039h, 048h, 00Ch, 075h, 00Ah, 06Ah, 0FFh, 0E8h, 00Fh, 011h, 000h, 000h
        db  083h, 0C4h, 004h, 0FFh, 074h, 024h, 00Ch, 057h, 0E8h, 04Eh, 018h, 000h, 000h, 08Bh, 044h, 024h
        db  014h, 083h, 0C4h, 008h, 0FFh, 070h, 00Ch, 0E8h, 0C8h, 011h, 000h, 000h, 08Bh, 044h, 024h, 010h
        db  083h, 0C4h, 004h, 0FFh, 070h, 00Ch, 0E8h, 004h, 006h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 044h
        db  024h, 00Ch, 06Ah, 000h, 0FFh, 070h, 00Ch, 0FFh, 015h, 06Ch, 0D4h, 000h, 000h, 083h, 0C4h, 008h
        db  06Ah, 000h, 0FFh, 074h, 024h, 010h, 0FFh, 015h, 06Ch, 0D4h, 000h, 000h, 083h, 0C4h, 008h, 0E8h
        db  0C6h, 015h, 000h, 000h, 08Bh, 0C6h, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h, 083h, 0ECh, 014h
        db  053h, 056h, 057h, 055h, 0BBh, 0FFh, 0FFh, 0FFh, 0FFh, 0BEh, 0F0h, 003h, 000h, 000h, 08Dh, 07Ch
        db  024h, 018h, 0A5h, 0A5h, 06Ah, 001h, 08Bh, 0EBh, 06Ah, 000h, 06Ah, 000h, 06Ah, 001h, 066h, 0A5h
        db  0FFh, 074h, 024h, 03Ch, 0E8h, 02Fh, 0A6h, 0FFh, 0FFh, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 00Ah
        db  0B8h, 057h, 000h, 000h, 000h, 0E9h, 04Ch, 001h, 000h, 000h, 08Dh, 044h, 024h, 014h, 050h, 0FFh
        db  074h, 024h, 02Ch, 0E8h, 0F5h, 001h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0F8h, 00Bh, 0FFh, 00Fh
        db  085h, 00Dh, 001h, 000h, 000h, 0FFh, 074h, 024h, 02Ch, 0FFh, 074h, 024h, 02Ch, 0E8h, 0C5h, 017h
        db  000h, 000h, 083h, 0C4h, 008h, 08Bh, 0F8h, 00Bh, 0FFh, 00Fh, 085h, 0F3h, 000h, 000h, 000h, 068h
        db  001h, 002h, 000h, 000h, 068h, 000h, 001h, 000h, 000h, 0FFh, 015h, 064h, 0D4h, 000h, 000h, 083h
        db  0C4h, 008h, 08Bh, 0D8h, 083h, 0FBh, 0FFh, 00Fh, 084h, 0C2h, 000h, 000h, 000h, 068h, 001h, 002h
        db  000h, 000h, 068h, 000h, 040h, 000h, 000h, 0FFh, 015h, 064h, 0D4h, 000h, 000h, 083h, 0C4h, 008h
        db  08Bh, 0E8h, 083h, 0FDh, 0FFh, 00Fh, 084h, 0A4h, 000h, 000h, 000h, 0A0h, 0D4h, 003h, 000h, 000h
        db  088h, 044h, 024h, 020h, 0FEh, 0C0h, 0A2h, 0D4h, 003h, 000h, 000h, 03Ch, 039h, 076h, 007h, 0C6h
        db  005h, 0D4h, 003h, 000h, 000h, 030h, 08Bh, 044h, 024h, 014h, 08Bh, 040h, 01Ch, 03Dh, 003h, 000h
        db  000h, 080h, 074h, 00Ch, 03Dh, 001h, 000h, 000h, 080h, 0BEh, 003h, 000h, 000h, 080h, 075h, 005h
        db  0BEh, 002h, 000h, 000h, 080h, 08Dh, 044h, 024h, 018h, 0FFh, 074h, 024h, 02Ch, 050h, 056h, 0E8h
        db  09Ch, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 08Bh, 0F8h, 00Bh, 0FFh, 075h, 057h, 08Dh, 044h, 024h
        db  010h, 050h, 08Dh, 044h, 024h, 01Ch, 050h, 056h, 0E8h, 0E3h, 0E2h, 0FFh, 0FFh, 083h, 0C4h, 00Ch
        db  08Bh, 0F8h, 00Bh, 0FFh, 075h, 03Eh, 055h, 053h, 0FFh, 074h, 024h, 018h, 0FFh, 074h, 024h, 034h
        db  0E8h, 004h, 019h, 000h, 000h, 083h, 0C4h, 010h, 0FFh, 074h, 024h, 010h, 0E8h, 0C3h, 0F6h, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 08Bh, 0F8h, 0FFh, 074h, 024h, 010h, 0E8h, 0A4h, 0E3h, 0FFh, 0FFh, 08Dh
        db  044h, 024h, 01Ch, 083h, 0C4h, 004h, 050h, 056h, 0E8h, 094h, 0FDh, 0FFh, 0FFh, 0EBh, 010h, 0BFh
        db  00Eh, 000h, 000h, 000h, 06Ah, 000h, 0FFh, 074h, 024h, 030h, 0E8h, 004h, 0AEh, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 083h, 0FBh, 0FFh, 074h, 00Ch, 06Ah, 000h, 053h, 0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        db  083h, 0C4h, 008h, 083h, 0FDh, 0FFh, 074h, 00Ch, 06Ah, 000h, 055h, 0FFh, 015h, 06Ch, 0D4h, 000h
        db  000h, 083h, 0C4h, 008h, 08Bh, 0C7h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 014h, 0C3h, 08Bh, 044h
        db  024h, 008h, 0FFh, 074h, 024h, 004h, 0FFh, 070h, 008h, 0E8h, 09Eh, 0E0h, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_8_70F0:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        sub    cx, cx
        sub    esi, dword ptr [edx + 24h]
loc_8_70FF:
        movzx  eax, cx
        imul   eax, eax, 28h
        db  039h, 0B0h, 0E0h, 005h, 000h, 000h
        jne    SHORT loc_8_7115
        db  039h, 090h, 0DCh, 005h, 000h, 000h
        je     SHORT loc_8_714A
loc_8_7115:
        inc    cx
        cmp    cx, 7
        jb     SHORT loc_8_70FF
        db  0A1h, 0E8h, 006h, 000h, 000h
        cmp    eax, -1
        je     SHORT loc_8_7143
        mov    ecx, 101000h
loc_8_712C:
        cmp    dword ptr [eax + 10h], esi
        jne    SHORT loc_8_713B
        cmp    dword ptr [eax + 0ch], edx
        jne    SHORT loc_8_713B
        test   dword ptr [eax + 18h], ecx
        je     SHORT loc_8_7148
loc_8_713B:
        mov    eax, dword ptr [eax + 8]
        cmp    eax, -1
        jne    SHORT loc_8_712C
loc_8_7143:
        mov    eax, 0ffffffffh
loc_8_7148:
        pop    esi
        ret
loc_8_714A:
        movzx  eax, cx
        imul   eax, eax, 28h
        db  005h, 0D0h, 005h, 000h, 000h
        jmp    SHORT loc_8_7148
sub_8_7157:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_8_70F0
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_717A
loc_8_7170:
        push   edi
        push   esi
        call   sub_8_7315
        add    esp, 8
loc_8_717A:
        pop    edi
        pop    esi
loc_8_717C:
        ret
sub_8_717D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0ffffffffh
        cmp    dword ptr [ebp + 8], 80000000h
        jb     NEAR PTR loc_8_71BD
        cmp    dword ptr [ebp + 8], 80000007h
        jae    NEAR PTR loc_8_71BD
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 80000000h
        movzx  eax, ax
        imul   eax, eax, 28h
        db  005h, 0D0h, 005h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
loc_8_71BD:
        cmp    dword ptr [ebp - 8], -1
        jne    NEAR PTR loc_8_7204
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_8_71D6
loc_8_71D3:
        inc    dword ptr [ebp - 0ch]
loc_8_71D6:
        cmp    dword ptr [ebp - 0ch], 7
        jae    NEAR PTR loc_8_7204
loc_8_71E0:
        mov    eax, dword ptr [ebp - 0ch]
        imul   eax, eax, 28h
        db  005h, 0D0h, 005h, 000h, 000h
        cmp    eax, dword ptr [ebp + 8]
        jne    NEAR PTR loc_8_71FF
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_8_7204
loc_8_71FF:
        jmp    NEAR PTR loc_8_71D3
loc_8_7204:
        cmp    dword ptr [ebp - 8], -1
        jne    NEAR PTR loc_8_724A
        db  0A1h, 0E8h, 006h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
loc_8_7216:
        cmp    dword ptr [ebp - 4], -1
        je     NEAR PTR loc_8_7245
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_8_7237
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_8_7245
loc_8_7237:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
loc_8_7240:
        jmp    NEAR PTR loc_8_7216
loc_8_7245:
        jmp    NEAR PTR loc_8_72C9
loc_8_724A:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [eax + 18h], 101000h
        je     NEAR PTR loc_8_72C9
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1ch], 80000001h
        jne    NEAR PTR loc_8_7293
        push   -1
        call   sub_8_7FEF
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_8_728E
        mov    eax, 3fah
        jmp    NEAR PTR loc_8_7310
loc_8_728E:
        jmp    NEAR PTR loc_8_72C9
loc_8_7293:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1ch], 80000000h
        jne    NEAR PTR loc_8_72BF
        call   sub_8_7F65
loc_8_72A8:
        or     eax, eax
        jne    NEAR PTR loc_8_72BA
loc_8_72B0:
        mov    eax, 3fah
        jmp    NEAR PTR loc_8_7310
loc_8_72BA:
        jmp    NEAR PTR loc_8_72C9
loc_8_72BF:
        mov    eax, 3fah
        jmp    NEAR PTR loc_8_7310
loc_8_72C9:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 8], -1
        je     NEAR PTR loc_8_7306
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [eax + 18h], 101000h
        je     NEAR PTR loc_8_72FA
        mov    eax, 3fah
        jmp    NEAR PTR loc_8_7310
        db  0E9h, 007h, 000h, 000h, 000h
loc_8_72FA:
        sub    eax, eax
loc_8_72FC:
        jmp    NEAR PTR loc_8_7310
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_8_7306:
        mov    eax, 3f2h
        jmp    NEAR PTR loc_8_7310
loc_8_7310:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_7315:
        push   esi
        push   201h
        push   28h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_7331
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_7373
loc_8_7331:
        mov    dword ptr [eax], 4e4b4752h
        mov    dword ptr [eax + 4], 0ffffffffh
        db  08Bh, 015h, 0E8h, 006h, 000h, 000h
        mov    dword ptr [eax + 8], edx
        mov    ecx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        sub    esi, dword ptr [ecx + 24h]
        cmp    edx, -1
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 18h], 0
        mov    dword ptr [eax + 14h], 0
        db  0A3h, 0E8h, 006h, 000h, 000h
        je     SHORT loc_8_7373
        mov    dword ptr [edx + 4], eax
loc_8_7373:
        pop    esi
loc_8_7374:
        ret
sub_8_7375:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        cmp    eax, -1
        je     SHORT loc_8_7388
        mov    dword ptr [eax + 8], ecx
loc_8_7388:
        cmp    ecx, -1
        je     SHORT loc_8_7390
        mov    dword ptr [ecx + 4], eax
loc_8_7390:
        db  039h, 035h, 0E8h, 006h, 000h, 000h
        jne    SHORT loc_8_739E
        db  089h, 00Dh, 0E8h, 006h, 000h, 000h
loc_8_739E:
        cmp    dword ptr [esi + 1ch], 80000006h
        jne    SHORT loc_8_73B5
        push   0
        push   dword ptr [esi + 20h]
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_8_73B5:
        push   0
        push   esi
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
        pop    esi
        ret
sub_8_73C3:
        db  08Bh, 015h, 0E8h, 006h, 000h, 000h
        cmp    edx, -1
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_8_73D4:
        je     SHORT loc_8_73EF
loc_8_73D6:
        cmp    dword ptr [edx + 10h], eax
        jne    SHORT loc_8_73E7
        cmp    dword ptr [edx + 0ch], ecx
        jne    SHORT loc_8_73E7
loc_8_73E0:
        or     dword ptr [edx + 18h], 1000h
loc_8_73E7:
        mov    edx, dword ptr [edx + 8]
        cmp    edx, -1
        jne    SHORT loc_8_73D6
loc_8_73EF:
        db  039h, 005h, 008h, 006h, 000h, 000h
        jne    SHORT loc_8_741D
        db  039h, 00Dh, 004h, 006h, 000h, 000h
        jne    SHORT loc_8_741D
        db  081h, 00Dh, 010h, 006h, 000h, 000h, 000h, 010h, 000h, 000h
        db  0C7h, 005h, 008h, 006h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 00Ch, 006h, 000h, 000h, 000h, 000h, 000h, 000h
loc_8_741D:
        db  039h, 005h, 0E0h, 005h, 000h, 000h
        jne    SHORT loc_8_744B
        db  039h, 00Dh, 0DCh, 005h, 000h, 000h
        jne    SHORT loc_8_744B
        db  081h, 00Dh, 0E8h, 005h, 000h, 000h, 000h, 010h, 000h, 000h
        db  0C7h, 005h, 0E0h, 005h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 0E4h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_8_744B:
        db  039h, 005h, 0A8h, 006h, 000h, 000h
        jne    SHORT loc_8_7479
        db  039h, 00Dh, 0A4h, 006h, 000h, 000h
        jne    SHORT loc_8_7479
        db  081h, 00Dh, 0B0h, 006h, 000h, 000h, 000h, 010h, 000h, 000h
        db  0C7h, 005h, 0A8h, 006h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 0ACh, 006h, 000h, 000h, 000h, 000h, 000h, 000h
loc_8_7479:
        ret
sub_8_747A:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 1ch]
        ret
sub_8_7482:
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 1ch]
        ret
sub_8_748A:
        push   201h
        push   40h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_74A5
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_750E
loc_8_74A5:
        mov    dword ptr [eax], 53464752h
        mov    word ptr [eax + 0ch], 1
        mov    word ptr [eax + 0eh], 0
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [eax + 1ch], 0
        mov    dword ptr [eax + 20h], 0ffffffffh
        mov    dword ptr [eax + 24h], 0ffffffffh
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    dword ptr [eax + 2ch], 0
        db  08Bh, 00Dh, 0FCh, 003h, 000h, 000h
        cmp    ecx, -1
        je     SHORT loc_8_74FB
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax + 8], ecx
        jmp    SHORT loc_8_7502
loc_8_74FB:
        mov    dword ptr [eax + 8], 0ffffffffh
loc_8_7502:
        mov    dword ptr [eax + 4], 0ffffffffh
        db  0A3h, 0FCh, 003h, 000h, 000h
loc_8_750E:
        ret
        db  08Bh, 04Ch, 024h, 004h, 081h, 039h, 052h, 047h, 046h, 053h, 074h, 007h, 0B8h
loc_8_751C:
        push   edi
        add    byte ptr [eax], al
        add    bl, ch
        sub    dword ptr [ebx - 57cf7afh], ecx
        push   dword ptr [esi + eax - 75h]
        inc    ecx
        add    al, 89h
        inc    edx
        add    al, 8bh
        inc    ecx
        add    al, 83h
        clc
        push   dword ptr [ebx + eax - 77h]
        push   eax
        or     byte ptr [ecx], bh
        db  00Dh, 0FCh, 003h, 000h, 000h
        jne    SHORT loc_8_7549
        db  089h, 015h, 0FCh, 003h, 000h, 000h
loc_8_7549:
        sub    eax, eax
        ret
        db  08Bh, 00Dh, 0E8h, 006h, 000h, 000h, 083h, 0F9h, 0FFh, 074h, 018h, 08Bh, 044h, 024h, 004h, 039h
        db  041h, 01Ch, 075h, 007h, 081h, 049h, 018h, 000h, 000h, 010h, 000h, 08Bh, 049h, 008h, 083h, 0F9h
loc_8_756C:
        push   dword ptr [ebp - 14h]
        ret
sub_8_7570:
        db  08Bh, 00Dh, 0E8h, 006h, 000h, 000h
        cmp    ecx, -1
        je     SHORT loc_8_7593
        mov    eax, dword ptr [esp + 4]
loc_8_757F:
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_8_758B
        or     dword ptr [ecx + 18h], 100000h
loc_8_758B:
        mov    ecx, dword ptr [ecx + 8]
        cmp    ecx, -1
        jne    SHORT loc_8_757F
loc_8_7593:
        ret
        db  068h, 001h, 002h, 000h, 000h, 06Ah, 020h, 0FFh, 015h, 064h, 0D4h, 000h, 000h, 083h, 0C4h, 008h
loc_8_75A4:
        or     eax, eax
        jne    SHORT loc_8_75AF
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_75E6
loc_8_75AF:
        mov    dword ptr [eax], 56484752h
        mov    dword ptr [eax + 8], 0ffffffffh
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    dword ptr [eax + 0ch], 0ffffffffh
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    dword ptr [eax + 14h], 0ffffffffh
        mov    dword ptr [eax + 1ch], 0
        mov    dword ptr [eax + 18h], 0
loc_8_75E6:
        ret
        db  08Bh, 044h, 024h, 004h, 081h
loc_8_75EC:
        cmp    byte ptr [edx + 47h], dl
        dec    eax
        push   esi
        je     SHORT loc_8_75FA
        mov    eax, 57h
loc_8_75F8:
        jmp    SHORT loc_8_7615
loc_8_75FA:
        mov    edx, dword ptr [eax + 8]
        cmp    edx, -1
        je     SHORT loc_8_7608
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
loc_8_7608:
        mov    eax, dword ptr [eax + 4]
        cmp    eax, -1
        je     SHORT loc_8_7613
loc_8_7610:
        mov    dword ptr [eax + 8], edx
loc_8_7613:
        sub    eax, eax
loc_8_7615:
        ret
        db  000h, 000h
loc_8_7618:
        call   sub_8_8293
        clc
        ret
        db  056h, 00Fh, 0B7h, 045h, 01Ch, 066h, 025h, 0FFh, 000h, 03Dh, 010h, 000h, 000h, 000h, 077h, 06Ah
        db  096h, 066h, 0B8h, 008h, 034h, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 096h, 083h, 0C6h, 006h, 0C1h
        db  0E0h, 003h, 08Bh, 090h, 02Ch
loc_8_7644:
        add    al, 0
        db  000h, 08Dh
loc_8_7648:
        db  0B8h, 028h, 004h, 000h, 000h
        push   ebp
        mov    ebp, esp
        jmp    dword ptr [edx]
        db  083h, 03Eh, 000h, 074h, 062h
loc_8_7657:
        mov    ax, word ptr [esi + 2]
        push   ebp
loc_8_765C:
        mov    ebp, dword ptr [ebx + 8]
        db  066h
loc_8_7660:
        mov    dword ptr [ebp + 1ch], eax
        pop    ebp
        mov    ax, 1cffh
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_8_76B9
        db  00Fh
loc_8_7674:
        mov    bh, 0eh
        add    eax, ecx
loc_8_7678:
        add    esi, 4
        push   eax
        add    edx, 4
        jmp    dword ptr [edx]
        db  083h, 0C6h, 004h, 083h, 0C2h, 004h, 0FFh
loc_8_7688:
        and    dh, byte ptr [ebx]
        sar    byte ptr [ecx], 6
        je     SHORT loc_8_7678
        jmp    SHORT loc_8_7657
        db  08Bh, 006h, 0EBh, 0E3h, 033h, 0C0h, 0EBh, 0E2h
loc_8_7699:
        or     dword ptr [ebp + 2ch], 1
        cmp    eax, 0ffff0000h
        jne    SHORT loc_8_76AB
        mov    eax, 3f3h
        jmp    SHORT loc_8_76B7
loc_8_76AB:
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_76B7
        mov    eax, 3f4h
loc_8_76B7:
        jmp    SHORT loc_8_76CE
loc_8_76B9:
        mov    eax, 57h
        jmp    SHORT loc_8_76C5
        db  08Bh, 006h, 050h, 0FFh, 017h
loc_8_76C5:
        leave
        or     eax, eax
        jne    SHORT loc_8_7699
        and    dword ptr [ebp + 2ch], 0fffffffeh
loc_8_76CE:
        push   eax
        pop    ax
        pop    dx
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 14h], dx
        pop    esi
        ret
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 010h, 0FFh, 075h, 00Ch, 0FFh, 075h, 008h, 0E8h, 058h, 0E7h
        db  0FFh, 0FFh, 083h
loc_8_76F0:
        les    ecx, ptr [ebx + ecx]
        sal    byte ptr [ebp + 2], 0c9h
        ret
        db  03Dh, 000h, 000h, 0FFh, 0FFh, 074h, 007h, 03Dh, 001h, 000h, 0FFh, 0FFh
loc_8_7704:
        db  075h, 0F0h
        mov    eax, 2
        db  0EBh, 0E9h
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 01Ch, 0FFh, 075h, 018h, 0FFh
loc_8_7718:
        db  075h, 014h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_8_5C61
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_8_7731
loc_8_772F:
        leave
loc_8_7730:
        ret
loc_8_7731:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_773F
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_772F
loc_8_773F:
        mov    eax, 2
        jmp    SHORT loc_8_772F
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h
loc_8_774C:
        sbb    bh, bh
        db  075h, 014h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_8_54B1
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_7767
loc_8_7765:
        leave
        ret
loc_8_7767:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_7775
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_7765
loc_8_7775:
        mov    eax, 2
        jmp    SHORT loc_8_7765
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 010h, 0FFh, 075h, 00Ch, 0FFh, 075h, 008h, 0E8h, 0A2h, 0DBh
        db  0FFh, 0FFh, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 002h
loc_8_7795:
        leave
        ret
        db  03Dh, 000h, 000h, 0FFh, 0FFh
loc_8_779C:
        je     SHORT loc_8_77A5
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_7795
loc_8_77A5:
        mov    eax, 2
        jmp    SHORT loc_8_7795
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 008h, 0E8h, 06Bh, 0DCh, 0FFh, 0FFh
loc_8_77B8:
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_8_77C1
loc_8_77BF:
        leave
        ret
loc_8_77C1:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_77CF
loc_8_77C8:
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_77BF
loc_8_77CF:
        mov    eax, 2
loc_8_77D4:
        jmp    SHORT loc_8_77BF
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h
loc_8_77DC:
        or     al, 0ffh
        jne    SHORT loc_8_77E8
        call   sub_8_5F52
        add    esp, 8
loc_8_77E8:
        or     eax, eax
        jne    SHORT loc_8_77EE
loc_8_77EC:
        leave
        ret
loc_8_77EE:
        cmp    eax, 0ffff0000h
        db  074h
loc_8_77F4:
        pop    es
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_77EC
        mov    eax, 2
        jmp    SHORT loc_8_77EC
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 014h, 0FFh, 075h
loc_8_780C:
        adc    bh, bh
        db  075h, 00Ch
loc_8_7810:
        push   dword ptr [ebp + 8]
        call   sub_8_628D
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_8_7821
loc_8_781F:
        leave
        ret
loc_8_7821:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_782F
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_781F
loc_8_782F:
        mov    eax, 2
        jmp    SHORT loc_8_781F
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 00Ch, 0FFh, 075h, 008h
loc_8_7840:
        call   sub_8_6095
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_784E
loc_8_784C:
        leave
        ret
loc_8_784E:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_785C
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_784C
loc_8_785C:
        mov    eax, 2
        jmp    SHORT loc_8_784C
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 024h, 0FFh, 075h, 020h, 0FFh, 075h, 01Ch, 0FFh, 075h, 018h
        db  0FFh, 075h, 014h, 0FFh, 075h
loc_8_7878:
        adc    bh, bh
        db  075h, 00Ch
        push   dword ptr [ebp + 8]
        call   sub_8_6307
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_8_788D
loc_8_788B:
        leave
        ret
loc_8_788D:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_789B
loc_8_7894:
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_788B
loc_8_789B:
        mov    eax, 2
        jmp    SHORT loc_8_788B
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 014h, 0FFh, 075h, 010h, 033h, 0C0h, 050h, 050h
loc_8_78B0:
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_8_599C
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_8_78C4
loc_8_78C2:
        leave
        ret
loc_8_78C4:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_78D2
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_78C2
loc_8_78D2:
        mov    eax, 2
        jmp    SHORT loc_8_78C2
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        xchg   dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 10h], eax
        pop    ebp
        enter  0, 0
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_8_599C
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_8_7909
loc_8_7907:
        leave
        ret
loc_8_7909:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_7917
loc_8_7910:
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_7907
loc_8_7917:
        mov    eax, 2
loc_8_791C:
        jmp    SHORT loc_8_7907
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 018h, 0FFh, 075h, 014h, 0FFh, 075h, 010h, 033h, 0C0h, 050h
        db  0FFh, 075h, 00Ch, 0FFh, 075h, 008h
loc_8_7934:
        call   sub_8_614B
        add    esp, 18h
loc_8_793C:
        or     eax, eax
        jne    SHORT loc_8_7942
loc_8_7940:
        leave
        ret
loc_8_7942:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_7950
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_7940
loc_8_7950:
        mov    eax, 2
        jmp    SHORT loc_8_7940
        push   ebp
        mov    ebp, esp
        xor    eax, eax
loc_8_795C:
        xchg   dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 10h], eax
        pop    ebp
        enter  0, 0
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_8_614B
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_8_7987
loc_8_7985:
        leave
        ret
loc_8_7987:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_7995
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_7985
loc_8_7995:
        mov    eax, 2
        jmp    SHORT loc_8_7985
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 008h, 0E8h, 08Ch, 0EDh, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Bh
        db  0C0h, 075h, 002h
loc_8_79AF:
        leave
        ret
        db  03Dh, 000h, 000h, 0FFh, 0FFh, 074h, 007h, 03Dh, 001h, 000h, 0FFh, 0FFh, 075h, 0F0h, 0B8h, 002h
        db  000h, 000h, 000h
loc_8_79C4:
        jmp    SHORT loc_8_79AF
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 02Ch, 0FFh, 075h, 028h, 0FFh, 075h, 024h, 0FFh, 075h, 01Ch
        db  0FFh, 075h, 018h, 0FFh, 075h, 008h, 0E8h, 082h, 0EBh, 0FFh, 0FFh, 083h, 0C4h, 018h, 00Bh, 0C0h
        db  075h, 002h
loc_8_79E8:
        leave
        ret
        db  03Dh, 000h, 000h, 0FFh, 0FFh, 074h, 007h, 03Dh, 001h, 000h, 0FFh, 0FFh, 075h, 0F0h, 0B8h, 002h
        db  000h, 000h, 000h, 0EBh, 0E9h
        push   ebp
        mov    ebp, esp
        push   ebx
        cmp    dword ptr [ebp + 8], 3
        jae    SHORT loc_8_7A75
loc_8_7A0C:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_8_7A75
        test   dword ptr [ebp + 10h], 0fffffffeh
        jne    SHORT loc_8_7A75
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 01Ch, 09Dh, 06Ch, 005h, 000h, 000h
        db  068h, 0CBh, 005h, 000h, 000h
        push   ebx
        push   80000002h
loc_8_7A30:
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_7A72
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        db  0FFh, 035h, 0CBh, 005h, 000h, 000h
        test   dword ptr [ebp + 10h], 1
        jne    SHORT loc_8_7A5A
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        jmp    SHORT loc_8_7A60
loc_8_7A5A:
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
loc_8_7A60:
        add    esp, 0ch
        push   eax
        db  0FFh, 035h, 0CBh, 005h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
loc_8_7A70:
        pop    ecx
        pop    eax
loc_8_7A72:
        pop    ebx
        leave
        ret
loc_8_7A75:
        mov    eax, 57h
        jmp    SHORT loc_8_7A72
        db  0C8h, 000h, 000h, 000h, 0FFh, 075h, 00Ch, 0FFh, 075h, 008h, 0E8h, 00Bh, 0F1h, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 00Bh, 0C0h, 075h, 002h
loc_8_7A92:
        leave
        ret
loc_8_7A94:
        cmp    eax, 0ffff0000h
        je     SHORT loc_8_7AA2
        cmp    eax, 0ffff0001h
        jne    SHORT loc_8_7A92
loc_8_7AA2:
        mov    eax, 2
        db  0EBh
loc_8_7AA8:
        db  0E9h, 033h, 0C0h, 0A3h, 000h
        db  004h, 000h, 000h, 0EBh, 000h
sub_8_7AB2:
        db  039h, 005h, 004h, 004h, 000h, 000h
        jne    SHORT loc_8_7AD0
        push   1
        push   eax
        db  068h, 0D1h, 06Ah, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        db  0A3h, 004h, 004h, 000h, 000h
loc_8_7AD0:
        ret
        db  033h, 0C0h, 0A3h
loc_8_7AD4:
        add    al, 4
        add    byte ptr [eax], al
loc_8_7AD8:
        call   sub_8_8293
        test   eax, eax
        jne    SHORT loc_8_7AE6
        call   sub_8_8B25
loc_8_7AE6:
        ret
sub_8_7AE7:
        pushad
        db  083h, 03Dh, 008h, 004h, 000h, 000h, 0FFh
        jne    SHORT loc_8_7B02
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cmp    eax, -1
        jne    SHORT loc_8_7AFD
        dec    eax
loc_8_7AFD:
        db  0A3h, 008h, 004h, 000h, 000h
loc_8_7B02:
        db  083h, 03Dh, 000h, 004h, 000h, 000h, 000h
        jne    SHORT loc_8_7B21
        mov    eax, 0ea60h
        db  0BEh, 0A9h, 06Ah, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 000h, 004h, 000h, 000h
loc_8_7B21:
        popad
        ret
        db  083h, 03Dh, 008h, 004h, 000h, 000h, 0FFh, 074h, 022h, 0CDh, 020h, 0CFh, 000h, 001h, 000h, 02Dh
        db  010h, 027h, 000h, 000h, 039h
loc_8_7B38:
        db  005h, 008h, 004h, 000h, 000h
        jae    SHORT loc_8_7B4E
        sbb    eax, eax
        db  0A3h, 008h, 004h, 000h, 000h
        xor    eax, eax
        call   sub_8_7AB2
        stc
loc_8_7B4E:
        cmc
        ret
sub_8_7B50:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
loc_8_7B5C:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_8_7B70:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        db  0E8h, 0D5h, 002h, 0FAh, 0FFh
        mov    edi, dword ptr [esp + 18h]
        cmp    edi, esi
        je     NEAR PTR loc_8_7C51
        db  083h, 03Dh, 054h, 006h, 000h, 000h, 0FFh
        jne    SHORT loc_8_7B9E
        call   sub_8_748A
        db  0A3h, 054h, 006h, 000h, 000h
        mov    ebp, eax
        jmp    SHORT loc_8_7BA4
loc_8_7B9E:
        db  08Bh, 02Dh, 054h, 006h, 000h, 000h
loc_8_7BA4:
        push   edi
        call   sub_8_4D04
        add    esp, 4
        or     eax, eax
        je     SHORT loc_8_7BB8
        or     dword ptr [ebp + 10h], 200h
loc_8_7BB8:
        push   ebp
        push   edi
        call   sub_8_3720
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_8_7C16
        test   byte ptr [esp + 1ch], 5
        je     SHORT loc_8_7C16
        cmp    esi, 0eh
        je     SHORT loc_8_7C16
        mov    ebx, dword ptr [ebp + 10h]
        and    ebx, 228h
        push   0
        push   edi
loc_8_7BE0:
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_7BF7
        push   0
        push   edi
        call   sub_8_1EA3
        add    esp, 8
loc_8_7BF7:
        push   ebp
        push   edi
        call   sub_8_816C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_8_7C13
        push   ebp
loc_8_7C08:
        push   edi
        call   sub_8_3720
        add    esp, 8
        mov    esi, eax
loc_8_7C13:
        or     dword ptr [ebp + 10h], ebx
loc_8_7C16:
        and    dword ptr [ebp + 10h], 0ffff7fffh
        or     esi, esi
        jne    SHORT loc_8_7C63
        mov    eax, dword ptr [ebp + 24h]
        cmp    eax, -1
        je     SHORT loc_8_7C63
        mov    eax, dword ptr [eax + 8]
loc_8_7C2C:
        db  0A3h, 058h, 006h, 000h, 000h
        db  0C7h, 005h, 05Ch, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        db  081h, 025h, 060h, 006h, 000h, 000h, 0FFh, 0EFh, 0EFh, 0FFh
        push   -1
        call   sub_8_7FEF
loc_8_7C4C:
        add    esp, 4
        jmp    SHORT loc_8_7C63
loc_8_7C51:
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        jne    SHORT loc_8_7C6F
        mov    eax, 57h
        jmp    NEAR PTR loc_8_7D37
loc_8_7C63:
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        je     NEAR PTR loc_8_7D35
loc_8_7C6F:
        or     esi, esi
        jne    NEAR PTR loc_8_7D35
        db  083h, 03Dh, 02Ch, 006h, 000h, 000h, 0FFh
        jne    SHORT loc_8_7C8E
        call   sub_8_748A
        db  0A3h, 02Ch, 006h, 000h, 000h
        mov    ebp, eax
        jmp    SHORT loc_8_7C94
loc_8_7C8E:
        db  08Bh, 02Dh, 02Ch, 006h, 000h, 000h
loc_8_7C94:
        push   edi
        call   sub_8_4D04
        add    esp, 4
        or     eax, eax
        je     SHORT loc_8_7CA8
        or     dword ptr [ebp + 10h], 200h
loc_8_7CA8:
        push   ebp
        push   edi
        call   sub_8_3720
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_8_7D01
        test   byte ptr [esp + 1ch], 5
        je     SHORT loc_8_7D01
        mov    ebx, dword ptr [ebp + 10h]
        and    ebx, 228h
        push   0
        push   edi
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_7CE2
        push   0
        push   edi
        call   sub_8_1EA3
        add    esp, 8
loc_8_7CE2:
        push   ebp
        push   edi
        call   sub_8_816C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_8_7CFE
        push   ebp
        push   edi
loc_8_7CF4:
        call   sub_8_3720
        add    esp, 8
        mov    esi, eax
loc_8_7CFE:
        or     dword ptr [ebp + 10h], ebx
loc_8_7D01:
        and    dword ptr [ebp + 10h], 0ffff7fffh
loc_8_7D08:
        or     esi, esi
        jne    SHORT loc_8_7D35
        mov    eax, dword ptr [ebp + 24h]
        cmp    eax, -1
        je     SHORT loc_8_7D35
        mov    eax, dword ptr [eax + 8]
        db  0A3h, 030h, 006h, 000h, 000h
        db  0C7h, 005h, 034h, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        db  081h, 025h, 038h, 006h, 000h, 000h, 0FFh, 0EFh, 0EFh, 0FFh
        call   sub_8_7F65
loc_8_7D35:
        mov    eax, esi
loc_8_7D37:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  083h, 0ECh, 004h, 056h, 0E8h, 00Bh, 001h, 0FAh, 0FFh, 08Bh, 035h, 02Ch, 006h, 000h, 000h, 083h
        db  0FEh, 0FFh, 074h, 046h, 08Bh, 0C6h, 083h, 078h, 02Ch, 000h, 074h, 03Eh, 0C6h, 044h, 024h, 007h
        db  000h, 08Dh, 044h, 024h, 007h, 056h, 050h, 0E8h, 0B8h, 0B9h, 0FFh, 0FFh, 083h, 0C4h, 008h, 081h
        db  066h, 010h, 0FFh, 07Fh, 0FFh, 0FFh, 08Bh, 046h, 024h, 083h, 0F8h, 0FFh, 074h, 01Ch, 08Bh, 040h
        db  008h, 0A3h, 030h, 006h, 000h, 000h, 0C7h, 005h, 034h, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        db  081h, 025h, 038h, 006h, 000h, 000h, 0FFh, 0EFh, 0EFh, 0FFh, 02Bh, 0C0h, 05Eh, 083h, 0C4h, 004h
        db  0C3h, 081h, 0ECh, 000h, 001h, 000h, 000h, 053h, 056h, 057h, 083h, 03Dh, 0ECh, 006h, 000h, 000h
        db  000h, 00Fh, 085h, 0A8h, 001h, 000h, 000h, 0C7h, 005h, 0ECh, 006h, 000h, 000h, 001h, 000h, 000h
        db  000h, 08Bh, 035h, 02Ch, 006h, 000h, 000h, 083h, 0FEh, 0FFh, 00Fh, 084h, 04Ch, 001h, 000h, 000h
        db  08Bh, 0C6h, 083h, 0C0h, 02Ch, 083h, 038h, 000h
loc_8_7DD4:
        je     NEAR PTR loc_8_7F18
        mov    dword ptr [eax], 0
        db  0E8h, 09Fh, 000h, 0FAh, 0FFh
        db  08Dh, 044h, 024h
loc_8_7DE8:
        or     al, 50h
        db  0E8h, 052h, 0FFh, 0F9h, 0FFh
        lea    eax, [esp + 10h]
        add    esp, 4
        push   eax
        call   sub_8_3651
        inc    ax
        add    esp, 4
        movzx  edi, ax
        push   201h
        push   edi
        db  0E8h, 0D9h, 02Ch, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        db  074h, 012h
        lea    eax, [esp + 0ch]
loc_8_7E1C:
        push   edi
        push   eax
        push   ebx
        call   sub_8_366A
        add    esp, 0ch
        mov    dword ptr [esi + 14h], ebx
        db  08Dh, 044h
loc_8_7E2C:
        and    al, 0ch
        push   eax
        db  0E8h, 0FBh, 0FEh, 0F9h, 0FFh
        add    esp, 4
        call   sub_8_748A
        mov    esi, eax
        db  089h, 035h, 054h, 006h, 000h, 000h
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_8_4D04
        add    esp, 4
        or     eax, eax
        je     SHORT loc_8_7E5C
        or     dword ptr [esi + 10h], 200h
loc_8_7E5C:
        lea    eax, [esp + 0ch]
        push   esi
        push   eax
        call   sub_8_3720
        add    esp, 8
        and    dword ptr [esi + 10h], 0ffff7fffh
        call   sub_8_7F65
        cmp    esi, -1
        je     SHORT loc_8_7EA1
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, -1
        je     SHORT loc_8_7EA1
        mov    eax, dword ptr [eax + 8]
        db  0A3h, 058h, 006h, 000h, 000h
        db  0C7h, 005h, 05Ch, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        db  081h, 025h, 060h, 006h, 000h, 000h, 0FFh, 0EFh, 0EFh, 0FFh
        jmp    SHORT loc_8_7EBF
loc_8_7EA1:
        db  0C7h, 005h, 058h, 006h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 05Ch, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  081h, 00Dh, 060h
loc_8_7EB8:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        adc    byte ptr [eax], al
loc_8_7EBF:
        push   -1
        call   sub_8_7FEF
        add    esp, 4
        call   sub_8_748A
        mov    esi, eax
        db  089h, 035h, 0CCh, 006h, 000h, 000h
        or     dword ptr [esi + 10h], 100h
        push   esi
        push   0
        call   sub_8_3720
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_7F11
loc_8_7EEC:
        db  0C7h, 005h, 0D4h, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 006h, 000h, 000h, 000h, 040h, 000h, 000h
        db  089h, 035h, 0CCh, 006h, 000h, 000h
        mov    eax, dword ptr [esi + 24h]
        mov    eax, dword ptr [eax + 8]
loc_8_7F0C:
        db  0A3h, 0D0h, 006h, 000h, 000h
loc_8_7F11:
        call   sub_8_84B0
        jmp    SHORT loc_8_7F59
loc_8_7F18:
        lea    eax, [esp + 0ch]
        push   eax
        db  0E8h, 01Fh, 0FEh, 0F9h, 0FFh
        add    esp, 4
        push   0
        lea    eax, [esp + 10h]
        push   0
        push   eax
        call   sub_8_7B70
        lea    eax, [esp + 18h]
        add    esp, 0ch
        push   eax
        db  0E8h, 0EFh, 0FDh, 0F9h, 0FFh
        add    esp, 4
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        push   0
        call   sub_8_7B70
        add    esp, 0ch
        db  0E8h, 02Bh, 0FFh, 0F9h, 0FFh
loc_8_7F59:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 100h
loc_8_7F64:
        ret
sub_8_7F65:
        sub    esp, 4
        db  0A1h, 02Ch, 006h, 000h, 000h
        cmp    dword ptr [eax + 24h], -1
        je     SHORT loc_8_7FC8
        lea    eax, [esp]
        push   0
        push   eax
        db  068h, 0F0h, 006h, 000h, 000h
        db  0FFh, 035h, 030h, 006h, 000h, 000h
        db  0FFh, 035h, 02Ch, 006h, 000h, 000h
        call   sub_8_2531
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_7FC8
        db  08Bh, 00Dh, 02Ch, 006h, 000h, 000h
        mov    eax, dword ptr [esp]
        sub    eax, dword ptr [ecx + 24h]
        db  0A3h, 0E0h, 005h, 000h, 000h
        mov    eax, ecx
        db  0C7h, 005h, 0E4h, 005h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 005h, 000h, 000h, 000h, 040h, 000h, 000h
        db  0A3h, 0DCh, 005h, 000h, 000h
        sub    eax, eax
        jmp    SHORT loc_8_7FEB
loc_8_7FC8:
        db  0C7h, 005h, 0E0h, 005h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 0E4h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  081h, 00Dh, 0E8h, 005h, 000h, 000h, 000h, 010h, 010h, 000h
        mov    eax, 3f3h
loc_8_7FEB:
        add    esp, 4
        ret
sub_8_7FEF:
        sub    esp, 0ch
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, -1
        je     SHORT loc_8_8032
        lea    ecx, [esp + 8]
        push   ecx
        push   eax
        call   sub_8_717D
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_8012
        jmp    NEAR PTR loc_8_80BB
loc_8_8012:
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp], eax
        cmp    eax, -1
        je     SHORT loc_8_805D
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 24h]
        add    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esp + 4], eax
        jmp    SHORT loc_8_8065
loc_8_8032:
        db  0A1h, 054h, 006h, 000h, 000h
        mov    dword ptr [esp], eax
        lea    eax, [esp + 4]
        push   2
        push   eax
        db  068h, 004h, 007h, 000h, 000h
        lea    eax, [esp + 0ch]
        db  068h, 048h, 006h, 000h, 000h
loc_8_8050:
        push   eax
        call   sub_8_1FF0
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_8_8065
loc_8_805D:
        mov    dword ptr [esp + 4], 0ffffffffh
loc_8_8065:
        cmp    dword ptr [esp + 4], -1
        je     SHORT loc_8_809D
        mov    ecx, dword ptr [esp]
        mov    eax, dword ptr [esp + 4]
        sub    eax, dword ptr [ecx + 24h]
        db  0A3h, 008h, 006h, 000h, 000h
loc_8_807C:
        db  0C7h, 005h, 00Ch, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 010h, 006h, 000h, 000h, 000h, 040h, 000h, 000h
        mov    eax, dword ptr [esp]
        db  0A3h, 004h, 006h, 000h, 000h
        sub    eax, eax
        jmp    SHORT loc_8_80C0
loc_8_809D:
        db  0C7h, 005h, 008h, 006h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 00Ch, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  081h, 00Dh, 010h, 006h, 000h, 000h, 000h, 010h, 010h, 000h
loc_8_80BB:
        mov    eax, 3f3h
loc_8_80C0:
        add    esp, 0ch
        ret
sub_8_80C4:
        push   ebx
        push   esi
        push   edi
        push   ebp
loc_8_80C8:
        mov    edi, dword ptr [esp + 14h]
        cmp    edi, -1
        je     NEAR PTR loc_8_8167
        cmp    dword ptr [edi], 53464752h
        jne    NEAR PTR loc_8_8167
        mov    eax, dword ptr [edi + 20h]
        cmp    eax, -1
        je     SHORT loc_8_80FB
        push   0
        push   eax
        db  0E8h, 0FFh, 029h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 20h], 0ffffffffh
loc_8_80FB:
        mov    eax, dword ptr [edi + 24h]
        cmp    eax, -1
        je     SHORT loc_8_8115
        push   0
        push   eax
        db  0E8h, 0E5h, 029h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 24h], 0ffffffffh
loc_8_8115:
        mov    ebx, dword ptr [edi + 28h]
        cmp    ebx, -1
        je     SHORT loc_8_8167
        mov    ebp, ebx
        movzx  esi, word ptr [edi + 0eh]
        or     esi, esi
        je     SHORT loc_8_8155
loc_8_8127:
        mov    eax, dword ptr [ebp]
        cmp    eax, -1
        je     SHORT loc_8_814F
        mov    eax, dword ptr [eax + 18h]
        cmp    eax, -1
        je     SHORT loc_8_8142
        push   0
        push   eax
        db  0E8h, 0B1h, 029h, 0FAh, 0FFh
        add    esp, 8
loc_8_8142:
        push   0
        push   dword ptr [ebp]
        db  0E8h, 0A4h, 029h, 0FAh, 0FFh
        add    esp, 8
loc_8_814F:
        add    ebp, 0ch
        dec    esi
        jne    SHORT loc_8_8127
loc_8_8155:
        push   0
        push   ebx
        db  0E8h, 093h, 029h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 28h], 0ffffffffh
loc_8_8167:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_816C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        push   201h
        db  06Ah
loc_8_8178:
        pushad
        db  0E8h, 06Ah, 029h, 0FAh, 0FFh
        add    esp, 8
        mov    edi, eax
        cmp    edi, esi
        jne    SHORT loc_8_8191
        mov    eax, 0eh
        jmp    NEAR PTR loc_8_828E
loc_8_8191:
        mov    ebx, dword ptr [esp + 14h]
        push   0
        push   ebx
        call   sub_8_1E66
        add    esp, 8
loc_8_81A0:
        mov    ebp, eax
        cmp    ebp, -1
        jne    SHORT loc_8_81B1
        mov    eax, 3f8h
        jmp    NEAR PTR loc_8_828E
loc_8_81B1:
        push   1
        push   edi
        call   sub_8_8510
        add    esp, 8
        push   0
        push   20h
        push   edi
        push   ebp
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_81D4
        mov    esi, 3f8h
loc_8_81D4:
        push   edi
        call   sub_8_855B
        add    esp, 4
        push   0
        push   20h
        push   edi
        push   ebp
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_81F5
        mov    esi, 3f8h
loc_8_81F5:
        push   edi
        call   sub_8_8582
        add    esp, 4
        push   0
        push   38h
        push   edi
        push   ebp
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_8216
        mov    esi, 3f8h
loc_8_8216:
        push   0
        push   0
        push   1000h
        push   ebp
        call   sub_8_1E29
        add    esp, 10h
        push   edi
        call   sub_8_85CE
        add    esp, 4
        push   0
        push   2ch
        push   edi
        push   ebp
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_8249
        mov    esi, 3f8h
loc_8_8249:
        push   0
        push   0
        push   2000h
        push   ebp
        call   sub_8_1E29
        add    esp, 10h
        push   0
        push   0
        push   edi
        push   ebp
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_8273
        mov    esi, 3f8h
loc_8_8273:
        push   1
        push   0
        push   ebp
loc_8_8278:
        push   ebx
        call   sub_8_1CFB
        add    esp, 10h
        push   0
        push   edi
        db  0E8h, 067h, 028h, 0FAh, 0FFh
        add    esp, 8
        mov    eax, esi
loc_8_828E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_8293:
        push   esi
        push   edi
        db  0A1h, 000h, 004h, 000h, 000h
        or     eax, eax
        db  074h, 010h
        push   eax
        db  0E8h, 054h, 028h, 0FAh, 0FFh
        db  0C7h, 005h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 004h
loc_8_82B0:
        add    al, 0
        add    byte ptr [ebx], cl
        sal    byte ptr [ebx + edx + 50h], 0e8h
        and    ebp, dword ptr [eax]
        cli
        inc    dword ptr [ebx + 5c704c4h]
        add    al, 4
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0C7h, 005h, 008h, 004h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_8_82D4:
        call   sub_8_84CD
        or     eax, eax
        je     SHORT loc_8_8359
        db  0FFh, 035h, 02Ch, 006h, 000h, 000h
        call   sub_8_3D28
        add    esp, 4
        mov    edi, eax
        db  0FFh, 035h, 054h, 006h, 000h, 000h
        call   sub_8_3D28
        add    esp, 4
        db  00Bh
loc_8_82FC:
        push   dword ptr [ebp + 2]
        mov    edi, eax
        db  08Bh, 035h, 06Ch, 006h, 000h, 000h
        cmp    esi, -1
        je     SHORT loc_8_8325
loc_8_830C:
        push   dword ptr [esi + 0ch]
        call   sub_8_3D28
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_8_831D
        mov    edi, eax
loc_8_831D:
        mov    esi, dword ptr [esi + 8]
        cmp    esi, -1
        jne    SHORT loc_8_830C
loc_8_8325:
        db  08Bh, 035h, 044h, 006h, 000h, 000h
        cmp    esi, -1
        je     SHORT loc_8_8349
loc_8_8330:
        push   dword ptr [esi + 0ch]
        call   sub_8_3D28
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_8_8341
        mov    edi, eax
loc_8_8341:
        mov    esi, dword ptr [esi + 8]
        cmp    esi, -1
        jne    SHORT loc_8_8330
loc_8_8349:
        call   sub_8_84FA
        or     edi, edi
        jne    SHORT loc_8_8359
        mov    eax, 1
        jmp    SHORT loc_8_835B
loc_8_8359:
        sub    eax, eax
loc_8_835B:
        pop    edi
        pop    esi
        ret
        db  000h, 000h
loc_8_8360:
        push   esi
        push   edi
        mov    edi, 0ffffffffh
        mov    esi, dword ptr [esp + 0ch]
        cmp    esi, 80000000h
        jb     SHORT loc_8_8387
        cmp    esi, 80000003h
        ja     SHORT loc_8_8387
        movzx  eax, si
        imul   eax, eax, 28h
        db  08Dh, 0B8h, 0D0h, 005h, 000h, 000h
loc_8_8387:
        cmp    edi, -1
        je     SHORT loc_8_83CB
loc_8_838C:
        mov    ecx, dword ptr [edi + 0ch]
        cmp    ecx, -1
        jne    SHORT loc_8_839E
loc_8_8394:
        call   sub_8_748A
        mov    dword ptr [edi + 0ch], eax
loc_8_839C:
        mov    ecx, eax
loc_8_839E:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 34h], eax
        add    esi, 7ffffffeh
        mov    dword ptr [ecx + 38h], 0
        cmp    esi, 1
        sbb    eax, eax
        add    eax, 3
        mov    dword ptr [ecx + 30h], eax
        mov    dword ptr [ecx + 2ch], 1
loc_8_83CB:
        pop    edi
        pop    esi
        ret
sub_8_83CE:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 7
        jb     SHORT loc_8_83DE
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_83FB
loc_8_83DE:
        movzx  eax, ax
        imul   eax, eax, 28h
loc_8_83E4:
        db  08Bh, 088h, 0DCh, 005h, 000h, 000h
        cmp    ecx, -1
        mov    eax, 0ffffffffh
        je     SHORT loc_8_83FB
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 38h], eax
loc_8_83FB:
        ret
sub_8_83FC:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 7
loc_8_8404:
        jb     SHORT loc_8_840D
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_8464
loc_8_840D:
        movzx  eax, ax
        imul   eax, eax, 28h
        db  08Bh, 0B0h, 0DCh, 005h, 000h, 000h
        cmp    esi, -1
loc_8_841C:
        jne    SHORT loc_8_8425
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_8464
loc_8_8425:
        mov    ecx, dword ptr [esi + 34h]
        mov    edx, dword ptr [esi + 38h]
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        je     SHORT loc_8_8457
        cmp    eax, 1
        je     SHORT loc_8_8441
        cmp    eax, 2
        je     SHORT loc_8_8449
        mov    eax, edx
        jmp    SHORT loc_8_8461
loc_8_8441:
        mov    eax, dword ptr [esp + 0ch]
        add    eax, edx
        jmp    SHORT loc_8_845B
loc_8_8449:
        sub    ecx, dword ptr [esp + 0ch]
        mov    eax, ecx
        or     eax, eax
        jge    SHORT loc_8_8461
        sub    eax, eax
        jmp    SHORT loc_8_8461
loc_8_8457:
        mov    eax, dword ptr [esp + 0ch]
loc_8_845B:
        cmp    ecx, eax
        jge    SHORT loc_8_8461
        mov    eax, ecx
loc_8_8461:
        mov    dword ptr [esi + 38h], eax
loc_8_8464:
        pop    esi
        ret
sub_8_8466:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, 7
        jb     SHORT loc_8_8475
        sub    eax, eax
        jmp    SHORT loc_8_84A7
loc_8_8475:
        movzx  eax, ax
        imul   eax, eax, 28h
        db  08Bh, 0B8h, 0DCh, 005h, 000h, 000h
        cmp    edi, -1
        mov    eax, 0
        je     SHORT loc_8_84A7
        mov    esi, dword ptr [esp + 14h]
        push   esi
        push   dword ptr [esp + 14h]
loc_8_8494:
        push   dword ptr [edi + 38h]
        push   dword ptr [edi + 3ch]
        call   sub_8_1FA8
        add    esp, 10h
        add    dword ptr [edi + 38h], esi
        mov    eax, esi
loc_8_84A7:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_8_84B0:
        push   0
        push   0
        db  0FFh, 015h, 02Ch, 0D7h, 000h, 000h
        add    esp, 8
        db  0A3h, 010h, 007h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_8_84CC
        db  0FFh, 015h, 024h, 0D6h, 000h, 000h
loc_8_84CC:
        ret
sub_8_84CD:
        db  08Bh, 00Dh, 010h, 007h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_8_84D9
        jmp    SHORT loc_8_84F4
loc_8_84D9:
        db  0A1h, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [ecx + 10h], eax
        mov    eax, 0
        je     SHORT loc_8_84F9
        push   1
        push   ecx
        db  0FFh, 015h, 0E0h, 0D7h, 000h, 000h
        add    esp, 8
loc_8_84F4:
        mov    eax, 1
loc_8_84F9:
        ret
sub_8_84FA:
        db  0A1h, 010h, 007h, 000h, 000h
        or     eax, eax
        je     SHORT loc_8_850D
        push   eax
        db  0FFh, 015h, 0E4h, 0D7h, 000h, 000h
        add    esp, 4
loc_8_850D:
        ret
        db  000h, 000h
sub_8_8510:
        mov    eax, dword ptr [esp + 4]
loc_8_8514:
        mov    cx, word ptr [esp + 8]
        mov    dword ptr [eax], 47455243h
        mov    dword ptr [eax + 4], 10000h
        mov    dword ptr [eax + 8], 1000h
        mov    dword ptr [eax + 0ch], 0
loc_8_8534:
        mov    word ptr [eax + 10h], 1
        mov    dword ptr [eax + 12h], 0
        mov    word ptr [eax + 16h], cx
        mov    byte ptr [eax + 1ah], 0
        mov    byte ptr [eax + 19h], 0
        mov    byte ptr [eax + 18h], 0
        push   eax
        call   sub_8_4DB7
        add    esp, 4
        ret
sub_8_855B:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 4e4b4752h
        mov    dword ptr [eax + 4], 0fe0h
        mov    dword ptr [eax + 8], 20h
        mov    dword ptr [eax + 0ch], 3ch
        mov    dword ptr [eax + 10h], 8
        ret
sub_8_8582:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
loc_8_858C:
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0ffffffffh
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    dword ptr [eax + 14h], 0ffffffffh
        mov    dword ptr [eax + 18h], 0ffffffffh
        add    eax, 1ch
        mov    dword ptr [eax], 0ffffffffh
        mov    dword ptr [eax + 4], 0fa4h
        mov    dword ptr [eax + 8], 0ffffffffh
        ret
sub_8_85CE:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 42444752h
        mov    dword ptr [eax + 4], 1000h
        mov    dword ptr [eax + 8], 0fe0h
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [eax + 10h], 20h
        mov    word ptr [eax + 14h], 0
        mov    word ptr [eax + 16h], 0
        add    eax, 20h
        mov    dword ptr [eax], 0fe0h
        mov    dword ptr [eax + 4], 0ffffffffh
loc_8_8610:
        mov    dword ptr [eax + 8], 0ffffffffh
        ret
sub_8_8618:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   byte ptr [edi + 19h], 20h
        jne    SHORT loc_8_8627
        jmp    SHORT loc_8_869A
loc_8_8627:
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_8_3580
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_8644
        mov    eax, 3f2h
        jmp    SHORT loc_8_869F
loc_8_8644:
        cmp    byte ptr [eax], 0
        je     SHORT loc_8_8650
        mov    eax, 3f2h
        jmp    SHORT loc_8_869F
loc_8_8650:
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_36EB
        add    esp, 4
        mov    ebx, eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_3651
        add    esp, 4
        mov    si, ax
        mov    edi, dword ptr [edi + 24h]
        cmp    edi, -1
        je     SHORT loc_8_869A
loc_8_8677:
        cmp    dword ptr [edi + 18h], ebx
        jne    SHORT loc_8_8692
        push   esi
        push   dword ptr [edi + 14h]
loc_8_8680:
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_35F6
        add    esp, 0ch
        or     ax, ax
loc_8_8690:
        je     SHORT loc_8_86A3
loc_8_8692:
        mov    edi, dword ptr [edi + 8]
        cmp    edi, -1
        jne    SHORT loc_8_8677
loc_8_869A:
        mov    eax, 3f3h
loc_8_869F:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_8_86A3:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        jmp    SHORT loc_8_869F
        db  053h, 056h, 057h, 08Bh, 074h, 024h, 018h, 08Bh, 044h, 024h, 01Ch, 089h, 046h, 00Ch, 08Bh, 040h
        db  024h, 08Bh, 07Ch, 024h, 014h, 08Bh, 040h, 008h, 089h, 046h, 010h
loc_8_86C8:
        push   edi
        call   sub_8_3651
        add    esp, 4
        movzx  ebx, ax
        inc    ebx
        push   201h
        push   ebx
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
loc_8_86E4:
        or     eax, eax
        jne    SHORT loc_8_86EF
loc_8_86E8:
        mov    eax, 0eh
        jmp    SHORT loc_8_8737
loc_8_86EF:
        mov    dword ptr [esi + 14h], eax
        push   ebx
        push   edi
        push   eax
        call   sub_8_366A
        add    esp, 0ch
        push   edi
        call   sub_8_36EB
        add    esp, 4
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 4], 0ffffffffh
        mov    ecx, dword ptr [eax + 24h]
        cmp    ecx, -1
        mov    dword ptr [esi + 8], ecx
        je     SHORT loc_8_8722
        mov    dword ptr [ecx + 4], esi
loc_8_8722:
        mov    dword ptr [eax + 24h], esi
        cmp    dword ptr [eax + 1ch], 80000006h
        jne    SHORT loc_8_8735
        and    dword ptr [eax + 18h], 0ffefefffh
loc_8_8735:
        sub    eax, eax
loc_8_8737:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 04Ch, 024h, 008h, 08Bh, 051h, 008h, 083h, 0FAh, 0FFh, 074h, 006h, 08Bh, 041h, 004h, 089h
        db  042h, 004h, 08Bh, 041h, 004h, 083h, 0F8h, 0FFh, 074h, 005h, 089h, 050h, 008h, 0EBh, 00Ch, 08Bh
        db  044h, 024h, 004h, 039h, 048h, 024h, 075h, 003h, 089h, 050h, 024h, 0C3h, 083h, 0ECh, 008h, 053h
        db  056h, 057h, 055h, 0BEh, 0FFh, 0FFh, 0FFh, 0FFh, 066h, 083h, 03Dh, 0C8h, 003h, 000h, 000h, 000h
        db  074h, 00Ah, 0B8h, 0F8h, 003h, 000h, 000h, 0E9h, 0DAh, 001h, 000h, 000h, 08Dh, 044h, 024h, 014h
        db  050h, 0FFh, 074h, 024h, 020h, 0E8h, 0E8h, 0E9h, 0FFh, 0FFh, 083h, 0C4h, 008h, 00Bh, 0C0h, 00Fh
        db  085h, 0C1h, 001h, 000h, 000h, 068h, 001h, 002h, 000h, 000h, 068h, 000h, 010h, 000h, 000h, 0FFh
        db  015h, 064h, 0D4h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0F8h, 083h, 0FFh, 0FFh, 075h, 00Ah, 0B8h
        db  00Eh, 000h, 000h, 000h, 0E9h, 09Dh, 001h, 000h, 000h
loc_8_87C4:
        mov    eax, dword ptr [esp + 14h]
        mov    edx, dword ptr [eax + 0ch]
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [edx + 24h]
        mov    ecx, dword ptr [esp + 18h]
        add    eax, dword ptr [ecx + 10h]
        push   eax
        push   edx
        call   sub_8_22A8
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_8_892F
        mov    eax, dword ptr [esp + 10h]
        add    eax, 14h
        push   eax
        call   sub_8_36EB
        add    esp, 4
        push   0
        mov    ebp, eax
        push   dword ptr [esp + 24h]
        call   sub_8_1E66
        add    esp, 8
        mov    esi, eax
loc_8_8810:
        cmp    esi, -1
        db  075h, 00Ah
        mov    ebx, 3f8h
        jmp    NEAR PTR loc_8_892F
        db  068h
loc_8_8820:
        add    byte ptr [eax], 0
        add    byte ptr [edi - 18h], dl
        out    0fch, al
        db  0FFh, 0FFh, 083h, 0C4h, 008h, 06Ah, 000h, 06Ah, 020h, 057h, 056h, 0E8h
loc_8_8834:
        mov    eax, 83ffff95h
        les    edx, ptr [eax]
        cmp    eax, -1
        jne    SHORT loc_8_884A
        mov    ebx, 3f8h
        jmp    NEAR PTR loc_8_892F
loc_8_884A:
        push   edi
        call   sub_8_855B
        add    esp, 4
        lea    eax, [edi + 20h]
        push   eax
        call   sub_8_8582
loc_8_885C:
        add    esp, 4
        mov    dword ptr [edi + 38h], 0
        mov    dword ptr [edi + 24h], ebp
        push   0
        push   58h
        push   edi
        push   esi
        call   sub_8_1DF0
        add    esp, 10h
        push   0
        push   0
        push   1000h
        push   esi
        call   sub_8_1E29
        add    esp, 10h
        push   edi
        call   sub_8_85CE
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        add    eax, 14h
        push   eax
        call   sub_8_3651
        add    esp, 4
        mov    dword ptr [edi + 24h], 0
        mov    word ptr [edi + 2ch], ax
        mov    word ptr [edi + 2eh], 0
        mov    word ptr [edi + 30h], 0
        movzx  ebp, ax
        mov    word ptr [edi + 32h], 0
        mov    eax, dword ptr [esp + 10h]
        push   ebp
        add    eax, 14h
        push   eax
        lea    eax, [edi + 34h]
        push   eax
        call   sub_8_366A
        add    esp, 0ch
        add    ebp, 14h
        mov    dword ptr [edi + 28h], ebp
        mov    dword ptr [edi + 20h], ebp
        mov    word ptr [edi + 16h], 1
        lea    eax, [ebp + 20h]
        mov    dword ptr [edi + 10h], eax
        add    eax, edi
        mov    ecx, 0fe0h
        sub    ecx, ebp
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    dword ptr [eax + 8], 0ffffffffh
        mov    dword ptr [edi + 8], ecx
        push   0
        push   1000h
        push   edi
        push   esi
        call   sub_8_1DF0
        add    esp, 10h
        push   1
        push   0
        push   esi
        mov    esi, 0ffffffffh
        push   dword ptr [esp + 2ch]
        call   sub_8_1CFB
        add    esp, 10h
loc_8_892F:
        cmp    esi, -1
        je     SHORT loc_8_8953
        push   1
        push   0
        push   esi
        push   dword ptr [esp + 2ch]
        call   sub_8_1CFB
        add    esp, 10h
        push   0
        push   dword ptr [esp + 24h]
        call   sub_8_1EA3
        add    esp, 8
loc_8_8953:
        push   0
        push   edi
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_8_8969:
        sub    esp, 14h
loc_8_896C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_8_7B50
        sub    ebp, ebp
        mov    esi, dword ptr [esp + 28h]
        db  08Bh
loc_8_897C:
        jl     SHORT loc_8_89A2
        xor    byte ptr [ebx - 38cbdba4h], cl
        inc    esp
        and    al, 20h
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        mov    dword ptr [esp + 18h], 4000h
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   ebx
        push   eax
        lea    eax, [esp + 2ch]
loc_8_89A2:
        push   0
        push   eax
        push   edi
        push   ebp
        push   esi
        call   sub_8_6307
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_8_89CF
loc_8_89B4:
        push   dword ptr [esp + 18h]
        push   ebx
        inc    ebp
        push   dword ptr [esp + 24h]
        push   edi
        push   0
        push   dword ptr [esp + 40h]
        call   sub_8_614B
        add    esp, 18h
        db  0EBh, 0B4h
loc_8_89CF:
        sub    ebp, ebp
        push   100h
        push   edi
        push   ebp
        push   esi
        call   sub_8_628D
        add    esp, 10h
        cmp    eax, ebp
        jne    SHORT loc_8_8A45
loc_8_89E5:
        lea    eax, [esp + 14h]
        push   eax
        push   edi
        push   esi
        call   sub_8_5330
        lea    eax, [esp + 1ch]
        add    esp, 0ch
loc_8_89F8:
        push   eax
        push   edi
        push   dword ptr [esp + 34h]
        call   sub_8_5E47
        add    esp, 0ch
        push   ebx
        push   edi
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 20h]
        call   sub_8_8969
        add    esp, 10h
        inc    ebp
        push   dword ptr [esp + 14h]
        call   sub_8_5423
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_8_5423
        add    esp, 4
        push   100h
        push   edi
        push   ebp
        push   esi
        call   sub_8_628D
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_8_89E5
loc_8_8A45:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  000h, 000h, 000h
loc_8_8A50:
        cmp    dword ptr [esp + 4], 219h
        jne    SHORT loc_8_8ABA
        cmp    dword ptr [esp + 8], 20h
        jne    SHORT loc_8_8ABA
        db  0FFh, 035h, 00Ch, 004h, 000h, 000h
        db  0E8h, 06Ch, 020h, 0FAh, 0FFh
loc_8_8A6C:
        add    esp, 4
        db  066h, 0C7h, 005h, 0CCh, 003h, 000h, 000h, 001h, 000h
        call   sub_8_84CD
        or     eax, eax
        je     SHORT loc_8_8ABA
        push   1
        call   sub_8_8AC0
        add    esp, 4
        db  083h, 03Dh, 054h, 006h, 000h, 000h, 0FFh
        je     SHORT loc_8_8AAB
        db  0A1h, 004h, 006h, 000h, 000h
        db  039h, 005h, 054h, 006h, 000h, 000h
        je     SHORT loc_8_8AAB
        push   3
        call   sub_8_8AC0
        add    esp, 4
loc_8_8AAB:
        push   2
        call   sub_8_8AC0
        add    esp, 4
        call   sub_8_84FA
loc_8_8ABA:
        mov    eax, 1
        ret
sub_8_8AC0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        imul   edi, edi, 28h
        db  08Bh, 0B7h, 0DCh, 005h, 000h, 000h
        cmp    esi, -1
        je     SHORT loc_8_8B22
        push   esi
        call   sub_8_3C2C
        add    esp, 4
        mov    eax, dword ptr [esi + 10h]
        test   al, 28h
        db  074h, 007h
        push   3f4h
        jmp    SHORT loc_8_8B13
        db  0F6h
loc_8_8AEC:
        les    eax, ptr [esp + esi*2]
        add    ch, bl
        pop    ss
        push   0
        push   esi
        call   sub_8_5012
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_8B22
        mov    eax, dword ptr [esi + 10h]
        test   ah, 10h
        je     SHORT loc_8_8B22
        or     eax, 1000h
        mov    dword ptr [esi + 10h], eax
        push   0
loc_8_8B13:
        db  08Dh, 087h, 0D0h, 005h, 000h, 000h
        push   eax
        call   sub_8_8BAC
        add    esp, 8
loc_8_8B22:
        pop    edi
        pop    esi
        ret
sub_8_8B25:
        push   1
        call   sub_8_8B51
        add    esp, 4
        db  0A1h
loc_8_8B30:
        add    al, 6
        add    byte ptr [eax], al
        db  039h, 005h, 054h, 006h, 000h, 000h
        je     SHORT loc_8_8B46
        push   3
        call   sub_8_8B51
        add    esp, 4
loc_8_8B46:
        push   2
        call   sub_8_8B51
        add    esp, 4
        ret
sub_8_8B51:
        mov    edx, dword ptr [esp + 4]
        imul   edx, edx, 28h
loc_8_8B58:
        db  08Bh, 082h, 0DCh, 005h, 000h, 000h
        cmp    eax, -1
        je     SHORT loc_8_8BAB
        db  066h, 083h, 03Dh, 0CCh, 003h, 000h, 000h, 000h
        je     SHORT loc_8_8BAB
        mov    eax, dword ptr [eax + 10h]
        test   al, 28h
        je     SHORT loc_8_8B7B
        mov    eax, 3f4h
        jmp    SHORT loc_8_8B9B
loc_8_8B7B:
        test   al, 10h
        je     SHORT loc_8_8B86
        mov    eax, 3f7h
        jmp    SHORT loc_8_8B9B
loc_8_8B86:
        test   al, 40h
        je     SHORT loc_8_8B91
        mov    eax, 3f5h
        jmp    SHORT loc_8_8B9B
loc_8_8B91:
        test   ah, 8
loc_8_8B94:
        je     SHORT loc_8_8BAB
        mov    eax, 0eh
loc_8_8B9B:
        push   eax
        db  08Dh, 082h, 0D0h, 005h, 000h, 000h
        push   eax
        call   sub_8_8BAC
        add    esp, 8
loc_8_8BAB:
        ret
sub_8_8BAC:
        sub    esp, 20ch
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 218h]
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [eax + 14h]
        call   sub_8_3651
        add    esp, 4
        movzx  eax, ax
        cmp    eax, 0feh
        jle    SHORT loc_8_8BD8
loc_8_8BD3:
        mov    eax, 0feh
loc_8_8BD8:
        inc    eax
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [eax + 14h]
        lea    eax, [esp + 11ch]
        push   eax
        call   sub_8_366A
        db  0C6h, 084h, 024h, 01Fh
loc_8_8BF1:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    esp, 0ch
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    edi, dword ptr [eax + 24h]
        add    edi, dword ptr [esi + 10h]
        test   byte ptr [esi + 19h], 40h
        db  075h, 029h
        mov    ax, word ptr [edi + 0ch]
        or     ax, ax
        db  074h, 020h
        movzx  eax, ax
        push   eax
        lea    eax, [edi + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_8_366A
        add    esp, 0ch
        movzx  eax, word ptr [edi + 0ch]
        db  0C6h
loc_8_8C31:
        inc    esp
        add    al, 14h
        add    bl, ch
        add    eax, 142444c6h
        db  000h, 08Bh, 046h
loc_8_8C3E:
        or     al, 89h
        inc    esp
        and    al, 10h
        mov    eax, dword ptr [esp + 21ch]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 8]
        push   eax
        push   20ch
        db  068h, 020h, 007h, 000h, 000h
        db  068h, 014h, 007h, 000h, 000h
        db  0E8h, 069h, 01Eh, 0FAh, 0FFh
        add    esp, 10h
        pop    edi
        pop    esi
        add    esp, 20ch
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_8_8FAC:
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
loc_8_906C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9078:
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
loc_8_9096:
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
loc_8_90B0:
        and    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    esp
        add    al, byte ptr [eax]
        db  000h, 010h
        add    al, 0
        db  000h, 0ECh
        pop    es
        add    byte ptr [eax], al
        dec    ebx
        or     al, 0
        db  000h, 0C7h
        or     al, 0
        db  000h, 093h, 00Fh, 000h, 000h, 04Dh
        adc    dword ptr [eax], eax
loc_8_90CF:
        db  000h, 0B5h, 004h, 000h, 000h, 0F8h
        adc    dword ptr [eax], eax
        db  000h, 03Dh, 005h, 000h, 000h, 07Fh
        db  005h, 000h, 000h, 018h, 000h
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    ebx
        adc    al, byte ptr [eax]
        db  000h, 030h
loc_8_90ED:
        add    byte ptr [eax], al
        db  000h, 063h, 000h
        add    byte ptr [eax], al
        scasd
        add    byte ptr [eax], al
        db  000h, 0D0h
        add    byte ptr [eax], al
        db  000h, 0DBh
        add    byte ptr [eax], al
        db  000h, 000h
        add    dword ptr [eax], eax
loc_8_9103:
        db  000h, 052h, 001h
        add    byte ptr [eax], al
        db  09Ah, 001h, 000h, 000h, 0F3h, 001h, 000h
        db  000h, 07Bh, 002h
        add    byte ptr [eax], al
        jecxz  loc_8_9118
        add    byte ptr [eax], al
loc_8_9118:
        db  025h, 003h, 000h, 000h, 060h
        add    eax, dword ptr [eax]
        db  000h, 0B5h
loc_8_9121:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9135:
        pop    ebx
        and    eax, 5d583830h
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
loc_8_917D:
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
loc_8_9195:
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
loc_8_9231:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9235:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9239:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_923D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9241:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9245:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_929F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + 49h], dl
        dec    esi
        inc    esp
        dec    ecx
        push   edx
        add    byte ptr [eax + 41h], dl
        push   esp
        dec    eax
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
loc_8_92DB:
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
loc_8_92F9:
        add    byte ptr [eax], al
        db  000h, 07Fh, 005h
        add    byte ptr [eax], al
        sub    al, 9
        add    byte ptr [eax], al
        db  0D9h, 00Bh, 000h, 000h, 009h, 00Ch, 000h, 000h, 08Ah, 00Ch, 000h, 000h, 0EFh
loc_8_9311:
        or     al, 0
        db  000h, 0E1h
        or     dword ptr [eax], eax
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        db  073h
loc_8_9323:
        db  074h, 065h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_8_9391
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_8_9395
        push   ebx
loc_8_933A:
        jb     NEAR PTR loc_8_93B3
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        db  078h, 000h
        db  000h
loc_8_9354:
        sub    eax, 2b000000h
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
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
loc_8_9375:
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
loc_8_9391:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9395:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 063h, 00Eh
        add    byte ptr [eax], al
        hlt
        sldt   word ptr [eax]
        db  0BCh, 012h, 000h, 000h, 0E8h
        adc    al, byte ptr [eax]
loc_8_93B3:
        db  000h, 028h
        adc    eax, dword ptr [eax]
        db  000h, 05Ah, 013h
        add    byte ptr [eax], al
        movsb
        adc    dword ptr [eax], eax
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [esp], ah
        and    al, 53h
loc_8_93DC:
        pop    ecx
        push   ebx
        dec    esp
        dec    edi
        inc    ecx
        inc    esp
        add    byte ptr [eax], al
        and    al, 24h
        and    al, 4eh
        inc    ebp
        push   edi
        dec    esp
        dec    edi
loc_8_93EC:
        inc    ecx
        inc    esp
        add    byte ptr [eax], al
        and    al, 53h
        inc    ecx
        push   esi
        inc    ebp
        dec    ebx
        inc    ebp
        pop    ecx
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 055h, 053h, 045h, 052h, 02Eh, 044h, 041h, 054h
        db  000h, 053h, 059h, 053h, 054h, 045h, 04Dh, 02Eh, 044h, 041h, 054h, 000h, 030h, 043h, 000h, 000h
        db  0BCh, 004h, 000h, 000h, 047h, 04Eh, 000h, 000h, 0BCh, 004h, 000h, 000h, 023h, 044h, 000h, 000h
        db  0C4h, 004h, 000h, 000h, 052h, 04Fh, 000h, 000h, 0CCh, 004h, 000h, 000h, 04Bh, 051h, 000h, 000h
        db  0E4h, 004h, 000h, 000h, 09Ch, 049h, 000h, 000h, 0FCh, 004h, 000h, 000h
loc_8_9458:
        lea    edx, [edx]
        db  000h, 0D4h
        add    al, 0
        db  000h, 095h, 050h, 000h, 000h, 0F4h
        add    al, 0
        db  000h, 007h
        push   ebx
        add    byte ptr [eax], al
        adc    al, 5
        add    byte ptr [eax], al
        pushfd
        dec    ecx
        add    byte ptr [eax], al
        xor    al, 5
        add    byte ptr [eax], al
        dec    ebx
        push   ecx
        add    byte ptr [eax], al
        push   eax
        db  005h, 000h, 000h, 034h, 057h
        add    byte ptr [eax], al
        les    eax, ptr [eax + eax]
        db  000h, 0D0h
        pop    esp
        add    byte ptr [eax], al
        enter  4, 0
        and    dword ptr [esi], ebx
        db  000h, 0CCh
        add    al, 0
        db  000h, 03Dh, 05Fh, 000h, 000h, 0B0h
        add    al, 0
        db  000h, 0CEh
        pushad
        add    byte ptr [eax], al
        int3
        add    al, 0
        db  000h, 096h, 05Bh, 000h, 000h
loc_8_94AC:
        fadd   qword ptr [eax + eax]
        db  000h
loc_8_94B0:
        xchg   ecx, eax
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        shl    byte ptr [esi], 0
        push   edx
        add    byte ptr [eax], al
        mov    dword ptr [esi], esp
        db  000h, 0C0h
        add    byte ptr [eax], al
loc_8_94C8:
        push   edx
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        shl    byte ptr [esi], 0
        xchg   ecx, eax
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        xchg   ecx, eax
        add    byte ptr [eax], al
loc_8_94E0:
        shl    byte ptr [esi], 0
        xchg   ecx, eax
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        xchg   ecx, eax
        add    byte ptr [eax], al
        xchg   ebp, eax
        add    byte ptr [eax], al
        mov    dword ptr [esi], esp
        db  000h, 0C0h
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
loc_8_9500:
        push   edx
        add    byte ptr [eax], al
        xchg   ebp, eax
        add    byte ptr [eax], al
        xchg   ebp, eax
        add    byte ptr [eax], al
        mov    dword ptr [esi], esp
        db  000h, 0C0h
        add    byte ptr [eax], al
        mov    dword ptr [esi], esp
        db  000h, 089h, 066h, 000h, 000h, 089h
        add    byte ptr [eax], al
        mov    dword ptr [esi], esp
        db  000h, 052h, 066h
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        xchg   ecx, eax
        add    byte ptr [eax], al
        shl    byte ptr [esi], 0
        mov    dword ptr [esi], esp
        db  000h, 089h, 066h, 000h, 000h, 089h
        add    byte ptr [eax], al
        db  081h, 066h, 000h, 000h, 052h, 066h, 000h
        db  000h, 095h, 066h, 000h, 000h, 0C0h
        add    byte ptr [eax], al
        xchg   ecx, eax
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        xchg   ecx, eax
        add    byte ptr [eax], al
        db  081h, 066h, 000h, 000h, 089h, 066h, 000h
        db  000h, 095h, 066h, 000h, 000h, 0C0h
        add    byte ptr [eax], al
        db  078h, 005h
        add    byte ptr [eax], al
        db  07Dh, 005h
        add    byte ptr [eax], al
        movsd
        add    eax, 6e450000h
        jne    SHORT loc_8_95E9
        add    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_8_95EE
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        jb     NEAR PTR loc_8_9610
        imul   esp, dword ptr [ebx + 65h], 6c435c73h
loc_8_95A1:
        popad
        db  073h, 073h
        add    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_8_9621
        jb     SHORT loc_8_9616
        outsb
        je     SHORT loc_8_95F7
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        jb     NEAR PTR loc_8_9638
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edx
        inc    edi
        dec    ebx
        dec    esi
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h
loc_8_95E9:
        inc    eax
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_95EE:
        add    byte ptr [eax - 1], al
        db  0FFh, 0FFh, 0FFh
loc_8_95F7:
        call   dword ptr [edx + 47h]
        dec    ebx
        dec    esi
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h
loc_8_9610:
        add    byte ptr [eax + 10h], al
        add    byte ptr [ecx], al
        db  000h
loc_8_9616:
        add    byte ptr [eax - 1], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 052h
loc_8_9621:
        inc    edi
        dec    ebx
        dec    esi
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h
loc_8_9638:
        add    byte ptr [eax + 10h], ah
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        cmp    bh, 0ffh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 052h, 047h, 04Bh, 04Eh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 060h
        db  010h, 000h, 003h, 000h, 000h, 080h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 052h
loc_8_9671:
        inc    edi
        dec    ebx
        dec    esi
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 040h, 010h, 000h, 004h, 000h, 000h, 080h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 052h, 047h, 04Bh, 04Eh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 040h, 010h, 000h
        db  005h, 000h, 000h, 080h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 052h, 047h, 04Bh, 04Eh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 060h, 010h, 000h, 006h, 000h, 000h, 080h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 053h, 04Fh, 046h, 054h
        db  057h, 041h, 052h, 045h, 05Ch
loc_8_96F9:
        inc    ebx
        insb
        popad
        db  073h, 073h
        jae    NEAR PTR loc_8_9701
loc_8_9701:
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        inc    esp
        popaw
        db  075h, 06Ch
        je     SHORT loc_8_970D
loc_8_970D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dh
        jae    SHORT loc_8_977C
        insd
        arpl   word ptr cs:[eax + 6ch], si
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 52h], al
        imul   esi, dword ptr gs:[bp + di + 74h], 72457972h
        db  072h, 06Fh
        jb     SHORT loc_8_9730
loc_8_9730:
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
loc_8_977C:
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
loc_8_97E6:
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
loc_8_9800:
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
loc_8_9928:
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
loc_8_9B44:
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
loc_8_9B74:
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
loc_8_9B9E:
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
loc_8_9BC8:
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
loc_8_9BE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_9BEC:
        add    byte ptr [eax], al
loc_8_9BEE:
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
loc_8_9C02:
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
loc_8_9C34:
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
loc_8_9C58:
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
_LTEXT ends
        end
