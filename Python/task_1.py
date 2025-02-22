import math
eps = 3.35
d = 1.5
D = float(input("Введите D:"))
Z = 276 / math.sqrt(eps) * math.log( D / d + math.sqrt(1 + D * D / (d * d)))
print("Волновое сопротивление двухпроводной линии равно =", Z)