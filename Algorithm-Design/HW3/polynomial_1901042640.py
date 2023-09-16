def polynomial (x, size, arr):  # x: x0, size: length of arr, arr contains constans of polyomial(a0, a1, .. an) 
    result = 0
    for i in range(0, size):
        temp = 1
        for j in range(0, size - i - 1):
            temp = temp * x
        result = arr[i] * temp + result
    return result
    
a = [1, 2, 3, 4];   
x0 = 3;

print("1*3^3 + 2*3^2 + 3*3^1 + 4*3^0 = ", polynomial(x0, len(a), a))