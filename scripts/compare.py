import sys

# print(sys.argv[1])

def compare(fn1, fn2):
    # file 1
    set1 = set()
    f1 = open(fn1)
    line = f1.readline()
    while line:
        if "Simplify Success" in line:
            set1.add(line.strip())
        line = f1.readline()
    f1.close()
    # file 2
    set2 = set()
    f2 = open(fn2)
    line = f2.readline()
    while line:
        if "Simplify Success" in line:
            set2.add(line.strip())
        line = f2.readline()
    f2.close()
    # 
    intersect1 = 0
    count1 = 0
    for l in set1:
        if l not in set2:
            print(l)
            count1 = count1 + 1
        else:
            intersect1 = intersect1 + 1
    print("----------")
    intersect2 = 0
    count2 = 0
    for l in set2:
        if l not in set1:
            print(l)
            count2 = count2 + 1
        else:
          intersect2 = intersect2 + 1
    print("----------")
    print("set1={}".format(len(set1)))
    print("set2={}".format(len(set2)))
    print("count1={}".format(count1))
    print("intersect1={}".format(intersect1))
    print("count2={}".format(count2))
    print("intersect2={}".format(intersect2))

compare(sys.argv[1], sys.argv[2])

