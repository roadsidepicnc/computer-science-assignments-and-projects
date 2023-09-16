def find_rop(arr):	# main function
    
    temp_arr = []
    temp_arr.extend(arr)

    counter = find_rop_rec(temp_arr, 0, len(arr) - 1)
    return counter
    
def find_rop_rec(arr, low, high):	# recursive function

    mid = low + (high - low) // 2
     
    if(low > high):
        return 0
    temp = reverse_ordered_pairs(arr, mid)
    x = find_rop_rec(arr, low, mid - 1)
    y = find_rop_rec(arr, mid + 1, high)
    return x + y + temp

def reverse_ordered_pairs(arr, i):	# support function
    counter = 0
    for j in range(i, len(arr)):
        if(arr[i] > arr[j]):
            counter += 1
    return counter
    
x = [8,5,4,1,2]

print("number of reverse-ordered pairs =",find_rop(x))