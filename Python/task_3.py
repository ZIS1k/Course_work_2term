import math
t = float(input("Введите t от 0 до 20:"))
x = 16 / ( t + 2 )
y = ( 40 - 60 * t ) / ( ( t + 2 ) * ( t + 2 ) * ( t + 2 ) )
print('(', x, ',', y, ')' )
print("Рука по отношению к предмету находится:" )
if x > 0 :
    print(" правее")
else :
    print(" левее")
if y > 0 :
    print(" выше")
else :
    print(" ниже")
