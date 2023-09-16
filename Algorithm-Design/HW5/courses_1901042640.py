class Course:
    def __init__(self, start_time, finish_time):
        self.start_time = start_time
        self.finish_time = finish_time
        
def sort_finish_time(e):
    return e.finish_time
    
def courses(arr):
    arr.sort(key = sort_finish_time)
    last_time = arr[0].finish_time
    result = 1
    
    for i in range(1, len(arr)):
        if(arr[i].start_time >= last_time):
            result = result + 1
            last_time = arr[i].finish_time
            
    return result  
   
arr = []

c1 = Course(1,2)
c2 = Course(3,4)
c3 = Course(0,6)
c4 = Course(5,7)
c5 = Course(8,9)
c6 = Course(5,9)

arr.append(c1)
arr.append(c2)
arr.append(c3)
arr.append(c4)
arr.append(c5)
arr.append(c6)

print("Maximum number of courses a student can attend =", courses(arr))