import math
t = 3.8
k = float(input("Введите k:"))
y = (math.sin(k) - math.cos(k * k)) + math.pow(math.e , t - 2)
if y > 7 :
    a = y
else :
    a = y * y
if a < 20 :
    b = math.fabs( t + a )
else :
    b = math.fabs( t * t - a )
Z = a * a * math.sin( t ) / ( ( b + 1 ) * ( b + 1 ) )
print("Искомый Z =", Z)