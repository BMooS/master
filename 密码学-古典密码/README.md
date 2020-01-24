# 古典密码

(1) 仿射密码  

参数选取：模数n=26+10=36 (26个字母+10个数字)，k2 = 学号后3位 mod n；k1 = 学号后4位 mod n, 若k1与n不互素，则更新k1 = k1+7 或 k1 = k1-7。

**代码如下：**

~~~python
import itertools

n = 36

def gcd(a,b):
    if a%b==0:
        return b
    else:
        return gcd(b,a%b)

def find(x,m = n): 
    for y in itertools.count(1): 
        #itertools.count(start,step)函数的意思是创建一个从start开始每次的步长是step的无穷序列
        if (x*y)%m==1:
            return y
 
k1 = int(input("输入学号后四位：")) % n

while gcd(n, k1) != 1:
    k1 += 7
    k1 %= n
    print("k1与n不互素，更新k1:",k1)
k2 = int(input("输入学号后三位：")) % n

def enc(x):
    return (k1*x+k2)%n
def dec(x):
    return (find(k1)*(x-k2))%n

U = {'0':0,'1':1,'2':2,'3':3,'4':4,'5':5,'6':6,'7':7,'8':8,'9':9,'a':10,'b':11,'c':12,'d':13,'e':14,'f':15,'g':16,'h':17,'i':18,'j':19,'k':20,'l':21,'m':22,'n':23,'o':24,'p':25,'q':26,'r':27,'s':28,'t':29,'u':30,'v':31,'w':32,'x':33,'y':34,'z':35}
V = {0:'0',1:'1',2:'2',3:'3',4:'4',5:'5',6:'6',7:'7',8:'8',9:'9',10:'a',11:'b',12:'c',13:'d',14:'e',15:'f',16:'g',17:'h',18:'i',19:'j',20:'k',21:'l',22:'m',23:'n',24:'o',25:'p',26:'q',27:'r',28:'s',29:'t',30:'u',31:'v',32:'w',33:'x',34:'y',35:'z'}

def encryption():
    m = input("输入要加密字符串：\n")
    c_list = []
    c = ''
    for i in m:
        c_list.append(enc(U[i]))
    for i in c_list:
        c += V[i]
    return c

def decrypt():
    c = input("输入要解密的字符串：\n")
    m_list = []
    m = ''
    for i in c:
        m_list.append(dec(U[i]))
    for i in m_list:
        m += V[i]
    return m

while(1):
    change = input("1.加密 2.解密:")
    if change == '1':
        c = encryption()
        print("密文为:",c)
    elif change == '2':
        m = decrypt()
        print("明文为:",m)
~~~

**解析：**

​	将26个字母和10位数字映射到0-35上

​	加密为 (k1*x+k2)%n

​	解密为 ((k1^-1)*(x-k2))%n

(2) 置换密码

参数选取：分组长度为7；置换关系随机选取；

长度不足时后面全补填充长度。

**代码如下：**

~~~python
def Group(m1,model):
    m1_list = []
    m1_list_new = []
    for i in m1:
        m1_list.append(i)
    if model == '1':
        rules = [4,1,0,6,3,5,2]
    elif model == '2':
        rules = [2,1,6,4,0,5,3]
    for i in rules:
        m1_list_new.append(m1_list[i])
    return ''.join(m1_list_new)

def enc_or_dec():
    model = input("1,加密 2,解密：")
    if model == '1':
        m = input("输入明文字符串：")
    elif model == '2':
        m = input("输入密文字符串：")
    patch = 7 - len(m)%7
    if len(m)%7 != 0:
        for i in range(7 - len(m)%7):
            m += str(patch)
    m_list= []
    for i in m:
        m_list.append(i)
    c = ''
    m1 = ''
    for i in range(int(len(m)/7)):
        for j in range(7):
            m1 += m_list[7*i+j]
        c1 = Group(m1,model)
        m1 = ''
        c += c1
    return c
while(1):    
    k = enc_or_dec()
    print(k)
~~~

**解析：**

正置换：[4,1,0,6,3,5,2]

逆置换：[2,1,6,4,0,5,3]

(3) Hill密码 

参数选取：密钥矩阵和明文/密文的元素均取自 Z26

密钥矩阵为：![UTOOLS1576823947931.png](https://i.loli.net/2019/12/20/4cajiulRt3BH8eF.png)

加解密：若明文为7,8,11,11, 计算密文；若密文为9,8,8,24，计算明文。

~~~python
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
        K_int[-1].append(int(round(j))) #round用来四舍五入
A_list_inverse = mod26(K_int)
#矩阵化
A = np.array(mod26(A_list))
A_inverse = np.array(A_list_inverse)

while(1): #python矩阵模块的使用
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
~~~

**解析：**

对矩阵取模函数：

~~~python
def mod26(A):
    B = []
    for i in A:
        B.append([])
        for j in i:
            j %= 26
            B[-1].append(j)#加入到B新增的元素中，该元素为1*n矩阵
    return B
~~~

A为n*n矩阵，for i in A 则遍历n次，每次i为1\*n矩阵，用for j in i 遍历i，对j取模26，在加入B中。