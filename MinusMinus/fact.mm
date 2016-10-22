main()
  declare n
  input "Number to get a factorial of: " n
  declare x
  x := fact(n)
  println "Fact of " n " should be " x
  return 0
fact(number)
  declare x
  x := number - 1
  if number > 2
    x := fact(x)
  endif
  x := x * number
  return x
