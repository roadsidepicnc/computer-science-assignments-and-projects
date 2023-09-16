def partition(arr, l, r):
    p = arr[l]
    s = l
    for i in range(l + 1, r + 1):
        if(arr[i] < p):
            s += 1
            temp = arr[i]
            arr[i] = arr[s]
            arr[s] = temp
    
    temp = arr[s]
    arr[s] = arr[l]
    arr[l] = temp

    return s

def meaningful(arr, l, r, k):
    pivot = partition(arr, l, r)
 
    if (pivot - l == k - 1):
        return arr[pivot]
 
    if (pivot - l > k - 1):
        return meaningful(arr, l, pivot - 1, k)
    else:   
        return meaningful(arr, pivot + 1, r, k - pivot + l - 1)
                             
    
arr = [11, 2, 11, 8, 7, 32, 9]
k = 4
print("Answer is", meaningful(arr, 0, len(arr) - 1, k))