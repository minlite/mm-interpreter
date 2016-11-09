program() 
declare sum count number
sum := 0
count := 0
input "Value to sum: " number
while number > 0
sum := sum + number
count := count + 1
input "Another value to sum: " number
endwhile
println "Entered " count " numbers"
print "Sum is " sum

