import numpy as np

def mod26(A):
    B = []
    for i in A:
        B.append([])
        for j in i:
            j %= 26
            B[-1].append(j)
    return B

A_list = [[8,6,9,5],
          [6,9,5,10],
          [5,8,4,9],
          [10,6,11,4]]
#求逆
K = np.linalg.inv(A_list)
K_int = []
for i in K:
    K_int.append([])
    for j in i:
        K_int[-1].append(int(round(j)))
A_list_inverse = mod26(K_int)
#矩阵化
A = np.array(mod26(A_list))
A_inverse = np.array(A_list_inverse)

while(1):
    select = input("1,加密 2,解密：")
    s = input("输入(逗号相隔)：")
    S_list = s.split(',')
    S_list_int = []
    for i in S_list:
        S_list_int.append(int(i))
    S = np.array(S_list_int)
    if select == '1':
        OUT = np.dot(S,A)
        OUT_list = []
        for i in OUT:
            OUT_list.append(i%26)
        print(OUT_list)
    elif select == '2':
        OUT = np.dot(S,A_inverse)
        OUT_list = []
        for i in OUT:
            OUT_list.append(i%26)
        print(OUT_list)