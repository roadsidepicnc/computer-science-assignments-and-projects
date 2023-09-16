import math
def cluster(arr, length):
    max_now = 0
    max_overall = -math.inf
    
    for i in range(0, length):
        max_now = max_now + arr[i]
        if (max_overall < max_now):
            max_overall = max_now

        if (max_now < 0):
            max_now = 0   
    return max_overall
    
arr = [3, -5, 2, 11, -8, 9, -5]
print("The most profit is = ", cluster(arr, len(arr)))