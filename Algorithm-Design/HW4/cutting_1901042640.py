def cutting(wire):
    if(wire <= 1):
        return 0
    else:
        return cutting((wire + 1) // 2) + 1
 
   
print("minimum number of cuts needed =",cutting(100))