# The divides as CodeBreaker translates them (found by Up's shader compiler, whose register bitsets are 64-bit): a word
# divide writes the whole register, its quotient zero-extended, so a stale high word never survives into the 64-bit
# shifts and compares that follow; a zero divisor (and INT_MIN / -1) gives 0 instead of faulting the host. The
# console leaves those results undefined; 0 and the zero-extension are what the reference recompilers and Xenia use.

test_divwu_clears_high_word:
  #_ REGISTER_IN r3 0xFFFFFFFF00000000
  #_ REGISTER_IN r4 7
  #_ REGISTER_IN r5 2
  divwu r3, r4, r5
  blr
  #_ REGISTER_OUT r3 3
  #_ REGISTER_OUT r4 7
  #_ REGISTER_OUT r5 2

test_divwu_zero_divisor:
  #_ REGISTER_IN r3 0x1234
  #_ REGISTER_IN r4 1
  #_ REGISTER_IN r5 0
  divwu r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0
  #_ REGISTER_OUT r4 1
  #_ REGISTER_OUT r5 0

test_divw_negative_zero_extended:
  #_ REGISTER_IN r3 0xFFFFFFFF00000000
  #_ REGISTER_IN r4 -7
  #_ REGISTER_IN r5 2
  divw r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0x00000000FFFFFFFD
  #_ REGISTER_OUT r4 -7
  #_ REGISTER_OUT r5 2

test_divw_zero_divisor:
  #_ REGISTER_IN r4 5
  #_ REGISTER_IN r5 0
  divw r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0
  #_ REGISTER_OUT r4 5
  #_ REGISTER_OUT r5 0

test_divw_min_over_minus_one:
  #_ REGISTER_IN r4 0x80000000
  #_ REGISTER_IN r5 -1
  divw r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0
  #_ REGISTER_OUT r4 0x80000000
  #_ REGISTER_OUT r5 -1

test_divdu_zero_divisor:
  #_ REGISTER_IN r4 9
  #_ REGISTER_IN r5 0
  divdu r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0
  #_ REGISTER_OUT r4 9
  #_ REGISTER_OUT r5 0

test_divd_zero_divisor:
  #_ REGISTER_IN r4 9
  #_ REGISTER_IN r5 0
  divd r3, r4, r5
  blr
  #_ REGISTER_OUT r3 0
  #_ REGISTER_OUT r4 9
  #_ REGISTER_OUT r5 0

test_divd_record:
  #_ REGISTER_IN r4 -0x100000000
  #_ REGISTER_IN r5 1
  divd. r3, r4, r5
  mfcr r6
  blr
  #_ REGISTER_OUT r3 -0x100000000
  #_ REGISTER_OUT r6 0x80000000
