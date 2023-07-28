  .section _start,"ax"
  lui sp, %hi(_estack)
  addi sp, sp, %lo(_estack)
  call main
  ebreak
