class Cheese:
    def __init__(self, price, weight):
        self.price = price
        self.weight = weight
        self.ratio = 0
  
def sort_ratio(e):
    return e.ratio
        
def cheese(arr, Weight):
    result = 0
    ratios = []
    
    for i in range(len(arr)):
        arr[i].ratio = (arr[i].price / arr[i].weight)
        ratios.append(0)
    
    arr.sort(reverse = True, key = sort_ratio)
    weight = 0 
    
    for i in range(len(arr)):
        if(weight + arr[i].weight < Weight):
            weight = weight + arr[i].weight
            ratios[i] = 1
        else:
            ratios[i] = (Weight - weight) / arr[i].weight
            break
        
    for i in range(len(arr)):
        result = result + arr[i].price * ratios[i]
        
    return result
   
arr = []
c1 = Cheese(280, 40) 
c2 = Cheese(100, 10) 
c3 = Cheese(120, 20) 
c4 = Cheese(120, 24) 

arr.append(c1)
arr.append(c2)
arr.append(c3)
arr.append(c4)

print(cheese(arr, 60))




