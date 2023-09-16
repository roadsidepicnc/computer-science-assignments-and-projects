def count_str(str, a, b):	# str is input string, a, first letter, b is last letter
    counter = 0
    for i in range(len(str) - 1):
        if str[i] == a:
            for j in range(i + 1, len(str)):
                if str[j] == b:
                    counter += 1
    return counter
    
str = "TXZXXJZWX"
a = 'X'
b = 'Z'
print("There are ", count_str(str, a, b), "substrings in " , str)