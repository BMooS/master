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