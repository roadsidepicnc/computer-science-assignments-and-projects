def cluster(low, high, arr):
    if (low == high):
        return arr[low]
    med = (low + high) // 2
    return max(cluster(low, med, arr), cluster(med + 1, high, arr), maxSum(low, med, high, arr))
    
###############################################    

def maxSum(low, med, high, arr):
    temp = 0
    leftMaxSum = 0
    rightMaxSum = 0
    for i in range(low - 1, med):
        temp = temp + arr[med - i - 1]
        if (temp > leftMaxSum):
            leftMaxSum = temp
    temp = 0
    for i in range(med + 1, high + 1):
        temp = temp + arr[i]
        if (temp > rightMaxSum):
            rightMaxSum = temp
 
    return max(leftMaxSum, rightMaxSum, leftMaxSum + rightMaxSum)
    
    
arr = [3, -5, 2, 11, -8, 9, -5]
print("Maximum profit is: ", cluster(0, len(arr) - 1, arr))