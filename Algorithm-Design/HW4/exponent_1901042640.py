def exponent_brute_force(a, n):		# brute force algorithm
    result = 1
    for i in range(0, n):
        result *= a
    return result
    
    
def exponent_divide_and_conquer(a, n):		# divide and conquer algorithm
    
    if(n == 0):
        return 1

    if(n % 2 == 0): 
        return exponent_divide_and_conquer(a * a, n // 2)
    else:
        return a * exponent_divide_and_conquer(a * a, n // 2)

print("2^3 (Using Brute Force) =",exponent_brute_force(2,3))            
print("3^5 (Using Divide and Conquer) =",exponent_divide_and_conquer(3,5))