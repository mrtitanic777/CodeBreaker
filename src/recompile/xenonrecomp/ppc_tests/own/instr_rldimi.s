# rldimi's mask is the value of a subtable of the description (a constant computed from MB and SH): a translator that read it from
# guest memory inserted nothing (Minecraft, rldimi r24,r9,32,0 putting a word into the high half of a doubleword).

test_rldimi_high_word:
  #_ REGISTER_IN r3 0x1111111122222222
  #_ REGISTER_IN r4 0x00000000AABBCCDD
  rldimi r3, r4, 32, 0
  blr
  #_ REGISTER_OUT r3 0xAABBCCDD22222222
  #_ REGISTER_OUT r4 0x00000000AABBCCDD

test_rldimi_middle_bits:
  #_ REGISTER_IN r3 0xFFFFFFFFFFFFFFFF
  #_ REGISTER_IN r4 0
  rldimi r3, r4, 8, 48
  blr
  #_ REGISTER_OUT r3 0xFFFFFFFFFFFF00FF
  #_ REGISTER_OUT r4 0
