import sys

print(sys.argv[1])

def compare(fn1, fn2):
    # file 1
    list1 = []
    f1 = open(fn1)
    line = f1.readline()
    while line:
    	if "Simplify Success" in line:
    		list1.append(line.strip())
    	line = f1.readline()
    f1.close()
    # file 2
    list2 = []
    f2 = open(fn2)
    line = f2.readline()
    while line:
    	if "Simplify Success" in line:
    		list2.append(line.strip())
    	line = f2.readline()
    f2.close()
    
    # 
    intersect1 = 0
    count1 = 0
    for l in list1:
        if l not in list2:
            print(l)
            count1 = count1 + 1
        else:
        	intersect1 = intersect1 + 1

    intersect2 = 0
    count2 = 0
    for l in list2:
        if l not in list1:
            print(l)
            count2 = count2 + 1
        else:
        	intersect2 = intersect2 + 1

    print("list1={}".format(len(list1)))
    print("list2={}".format(len(list2)))
    print("count1={}".format(count1))
    print("intersect1={}".format(intersect1))
    print("count2={}".format(count2))
    print("intersect2={}".format(intersect2))

compare(sys.argv[1], sys.argv[2])

