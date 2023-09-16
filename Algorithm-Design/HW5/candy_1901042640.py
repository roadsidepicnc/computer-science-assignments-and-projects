import math
def candy(price_arr, length):
    temp_arr = []
    for i in range(length + 1):
        temp_arr.append(0)
    
    for i in range(1, length + 1):
        max_price = -math.inf
        for j in range(i):
            if(price_arr[j] + temp_arr[i - j - 1] > max_price):
                max_price = price_arr[j] + temp_arr[i - j - 1]
        temp_arr[i] = max_price

    return temp_arr[length]

arr = [1, 5, 8, 9, 10, 17, 17, 20]
length = len(arr)
print("Maximum obtainable value = ", candy(arr, length))