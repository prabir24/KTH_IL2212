import re

fo1 = open("outputArray2.h", "w")
fo1.write("unsigned char test_ppm_1[] = {")

# Open a file
file1 = open("image.ppm", "r+")
Lines = file1.readlines()
count = 0
# Strips the newline character
length = len(Lines)
for line in Lines:
    print(count)
    if count == 1:

        fo1.write(re.sub("\s+", ",", line.strip()))
    elif count >= 2:
        fo1.write(',' + line.strip())
    count += 1

fo1.write("};")

file1.close()
fo1.close()
