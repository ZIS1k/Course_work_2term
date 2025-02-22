import math
N = float(input("Введите N:"))
CtoF = math.pi * N / 8
flag = int(input("Введите номер флага (от 0 до 7):"))
S = N
if flag <= 4 :
    S += CtoF * flag
else :
    S += CtoF * (flag - 4)
print("Ваш робот прошёл расстояние равное", S)