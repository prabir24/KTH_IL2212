import re
"""
fo1 = open("flag_tiny_03_1_op.h", "w")
fo1.write("unsigned char test_ppm_1[] = {")

# Open a file
file1 = open("flag_tiny_03_1.ppm", "r+")
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

"""
with open('flag_tiny_03_1.ppm', 'r') as file:
    data = file.read().replace('\n', ',')
    data1 = data[0:-1].replace(' ', ',')
    data2 = data1.replace(',,', ',')
    data3 = data2.replace('P3,#,Created,by,IrfanView,', '')

print(data)
print(data1)
print(data2)
print(data3)


with open('flag_tiny_03_1_op.h', 'w') as file:
    file.write('unsigned char test_ppm_1[] = {' + data3 + '};')
