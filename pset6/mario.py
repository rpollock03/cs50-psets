import cs50

# declare variable height
height = 0
# check user inputted height is between 1 and 8
while height <= 0 or height > 8:
    height = cs50.get_int("Pyramid Height 1-8: ")
# build pyramid
for i in range(1, height+1):
    print(" "*(height-i), end="")
    print("#"*i)