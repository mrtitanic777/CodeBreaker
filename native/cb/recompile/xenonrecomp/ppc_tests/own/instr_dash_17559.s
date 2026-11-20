# Instructions first used by dash.xex 2.0.17559 (XenonRecomp patch 0009). Expected values follow the Power ISA
# definitions (no hardware capture): vaddsws saturates each signed word; vnor/vnor128 = ~(a | b); lwaux/lhaux load
# sign-extended and write the effective address back to rA; rlwimi rotates the doubled low word, so a wrapping mask
# (MB > ME) fills the high word too; rlwimi. sets CR0 from the low word (read back with mfcr).

test_vaddsws_saturate:
  #_ REGISTER_IN v3 [7FFFFFFF, 80000000, 00000001, FFFFFFFF]
  #_ REGISTER_IN v4 [00000001, FFFFFFFF, 7FFFFFFF, 80000000]
  vaddsws v5, v3, v4
  blr
  #_ REGISTER_OUT v3 [7FFFFFFF, 80000000, 00000001, FFFFFFFF]
  #_ REGISTER_OUT v4 [00000001, FFFFFFFF, 7FFFFFFF, 80000000]
  #_ REGISTER_OUT v5 [7FFFFFFF, 80000000, 7FFFFFFF, 80000000]

test_vaddsws_plain:
  #_ REGISTER_IN v3 [00000005, FFFFFFF0, 12345678, 40000000]
  #_ REGISTER_IN v4 [00000003, 00000020, 11111111, 3FFFFFFF]
  vaddsws v3, v3, v4
  blr
  #_ REGISTER_OUT v3 [00000008, 00000010, 23456789, 7FFFFFFF]
  #_ REGISTER_OUT v4 [00000003, 00000020, 11111111, 3FFFFFFF]

test_vnor:
  #_ REGISTER_IN v3 [F0F0F0F0, 00000000, FFFFFFFF, 12345678]
  #_ REGISTER_IN v4 [0F0F0000, 00000000, 00000000, 87654321]
  vnor v5, v3, v4
  blr
  #_ REGISTER_OUT v3 [F0F0F0F0, 00000000, FFFFFFFF, 12345678]
  #_ REGISTER_OUT v4 [0F0F0000, 00000000, 00000000, 87654321]
  #_ REGISTER_OUT v5 [00000F0F, FFFFFFFF, 00000000, 688AA886]

test_vnor128:
  #_ REGISTER_IN v3 [F0F0F0F0, 00000000, FFFFFFFF, 12345678]
  #_ REGISTER_IN v4 [0F0F0000, 00000000, 00000000, 87654321]
  vnor128 v5, v3, v4
  blr
  #_ REGISTER_OUT v3 [F0F0F0F0, 00000000, FFFFFFFF, 12345678]
  #_ REGISTER_OUT v4 [0F0F0000, 00000000, 00000000, 87654321]
  #_ REGISTER_OUT v5 [00000F0F, FFFFFFFF, 00000000, 688AA886]

test_vnor128_not:
  #_ REGISTER_IN v3 [F0F0F0F0, 00000000, FFFFFFFF, 12345678]
  vnor128 v3, v3, v3
  blr
  #_ REGISTER_OUT v3 [0F0F0F0F, FFFFFFFF, 00000000, EDCBA987]

test_lwaux_negative:
  #_ MEMORY_IN 10001000 80 00 00 01 7f ff ff fe
  #_ REGISTER_IN r4 0x10000FF8
  #_ REGISTER_IN r5 0x8
  lwaux r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0xFFFFFFFF80000001
  #_ REGISTER_OUT r4 0x10001000
  #_ REGISTER_OUT r5 0x8

test_lwaux_positive:
  #_ MEMORY_IN 10001000 80 00 00 01 7f ff ff fe
  #_ REGISTER_IN r4 0x10001000
  #_ REGISTER_IN r5 0x4
  lwaux r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0x7FFFFFFE
  #_ REGISTER_OUT r4 0x10001004
  #_ REGISTER_OUT r5 0x4

test_lhaux_negative:
  #_ MEMORY_IN 10001000 80 01 7f fe
  #_ REGISTER_IN r4 0x10000FFE
  #_ REGISTER_IN r5 0x2
  lhaux r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0xFFFFFFFFFFFF8001
  #_ REGISTER_OUT r4 0x10001000
  #_ REGISTER_OUT r5 0x2

test_lhaux_positive:
  #_ MEMORY_IN 10001000 80 01 7f fe
  #_ REGISTER_IN r4 0x10001000
  #_ REGISTER_IN r5 0x2
  lhaux r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0x7FFE
  #_ REGISTER_OUT r4 0x10001002
  #_ REGISTER_OUT r5 0x2

test_rlwimi_wrapping_mask:
  #_ REGISTER_IN r4 0x0000000012345678
  #_ REGISTER_IN r6 0xAAAAAAAAAAAAAAAA
  rlwimi r6, r4, 4, 28, 3
  blr
  #_ REGISTER_OUT r4 0x0000000012345678
  #_ REGISTER_OUT r6 0x234567812AAAAAA1

test_rlwimi_record_lt:
  #_ REGISTER_IN r4 0x0000000080000000
  #_ REGISTER_IN r6 0x0000000000000000
  rlwimi. r6, r4, 0, 0, 0
  mfcr r7
  blr
  #_ REGISTER_OUT r6 0x0000000080000000
  #_ REGISTER_OUT r7 0x0000000080000000

test_rlwimi_record_gt:
  #_ REGISTER_IN r4 0x0000000000000001
  #_ REGISTER_IN r6 0xFFFFFFFF00000000
  rlwimi. r6, r4, 0, 31, 31
  mfcr r7
  blr
  #_ REGISTER_OUT r6 0xFFFFFFFF00000001
  #_ REGISTER_OUT r7 0x0000000040000000

test_rlwimi_record_eq:
  #_ REGISTER_IN r4 0x0000000000000000
  #_ REGISTER_IN r6 0x00000000FFFF0000
  rlwimi. r6, r4, 0, 0, 15
  mfcr r7
  blr
  #_ REGISTER_OUT r6 0x0000000000000000
  #_ REGISTER_OUT r7 0x0000000020000000

test_subfc_same_register:
  # subfc rD,rA,rA always carries (rA >= rA); addze reads CA back
  #_ REGISTER_IN r27 0x80000005
  #_ REGISTER_IN r9 0x0
  subfc r7, r27, r27
  addze r8, r9
  blr
  #_ REGISTER_OUT r7 0x0
  #_ REGISTER_OUT r8 0x1
  #_ REGISTER_OUT r27 0x80000005

test_subfc_borrow:
  #_ REGISTER_IN r3 0x5
  #_ REGISTER_IN r4 0x3
  #_ REGISTER_IN r9 0x0
  subfc r7, r3, r4
  addze r8, r9
  blr
  #_ REGISTER_OUT r7 0xFFFFFFFFFFFFFFFE
  #_ REGISTER_OUT r8 0x0
