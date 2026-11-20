# frsqrte (first used by Up): the reciprocal square root estimate, which CodeBreaker computes exactly. The ISA only
# bounds the estimate's error (1/32), so these inputs have exact answers: 1/sqrt(4) = 0.5, 1/sqrt(0.25) = 2.

test_frsqrte_4:
  #_ REGISTER_IN f1 4.0
  frsqrte f2, f1
  blr
  #_ REGISTER_OUT f1 4.0
  #_ REGISTER_OUT f2 0.5

test_frsqrte_quarter:
  #_ REGISTER_IN f1 0.25
  frsqrte f2, f1
  blr
  #_ REGISTER_OUT f1 0.25
  #_ REGISTER_OUT f2 2.0
