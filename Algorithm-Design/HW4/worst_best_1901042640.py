def worst_best_rec (arr, low, high, best, worst):	# recursive function
    
    mid = low + (high - low) // 2

    if(arr[mid] > best):
        best = arr[mid]
        
    if(arr[mid] < worst): 
        worst = arr[mid]
    
    if(low >= high):
        return (best, worst)
        
    left = worst_best_rec(arr, low, mid -1, best, worst)
    right = worst_best_rec(arr, mid + 1, high, best, worst)
    
    a = 0
    b = 0
    
    if(left[0] > right[0]):
        a = left[0]
    else:
        a = right[0]
    if(left[1] < right[1]):
        b = left[1]
    else:
        b = right[1] 
        
    pair = (a,b)    
    return pair

def worst_best(arr):			# main function
    best = arr[len(arr) // 2]
    worst = arr[len(arr) // 2]
    worst_best_rec(arr, 0, len(arr) -1, best, worst)
    pair = (worst_best_rec (arr, 0, len(arr) -1, best, worst))
    return pair

test_results = [-45, 1, 3, 4, 5, 31, 61, -2]
res = worst_best(test_results)
print("Worst =", res[1])
print("Best =", res[0])