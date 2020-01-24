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