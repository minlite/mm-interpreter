convert()
  declare temp x
  input "Farenheit emperature? " temp
  x := celsius(temp)
  println "Celsius is " x
  return 0
celsius(temp)
  declare newtemp
  newtemp := (temp - 32) * 5 / 9
  return newtemp
