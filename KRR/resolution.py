# the code for P02 part 2

###### input for AIpine Club ######
# A(tony)
# A(mike)
# A(john)
# L(tony, rain)
# L(tony, snow)
# (¬A(X), S(X), C(X))
# (¬C(Y), ¬L(Y, rain))
# (L(Z, snow), ¬S(Z))
# (¬L(tony, U), ¬L(mike, U))
# (L(tony, V), L(mike, V))
# (¬A(W), ¬C(W), S(W))
###################################


#### input for hardworker(sue) ####
# GradStudent(sue)
# (¬GradStudent(X), Student(X))
# (¬Student(X), HardWorker(X))
# ¬HardWorker(sue)
###################################

####### input for 3' blocks #######
# On(aa,bb)
# On(bb,cc)
# Green(aa)
# ¬Green(cc)
# (¬On(X,Y), ¬Green(X), Green(Y))
###################################
import re

#就是匹配一下这两个字符串
def isSamePredicate(a, b):
    if a==b :
        return True
    if a==b[1:] or a[1:]==b:
        return True
    return False

def findDisagreement(f, g):
    #从零开始比就可以，因为传进来就已经把为此的部分截掉了
    for i in range(len(f)):
        if f[i]!=g[i]:
            return i
    #
    return -1

#b里面包含a
def variableInterm(a, b):
    index1 = b.find('(')
    index2 = b.find(')')
    if(index1!=-1 and index2!=-1):
        res = b[index1+1:index2]
        #如果匹配上了就返回匹配起始下标
        if a == res:
            return index1 + 1
        else:
            return -1
    #没匹配上就返回-1
    return -1

def multi_sub(oristring, substring, ibegin, iend):
    new = list(substring)
    ori = list(oristring)
    ori[ibegin:iend] = iter(new)
    return ''.join(ori)


#做一个简单的trick，和prolog保持一致，如果小写字母开头就是常量，如果大写字母开头就是变量
def MGU(f, g):
    sigma = []
    if len(f) != len(g) or (not isSamePredicate(f[0],g[0])):
        #表示两个公式不能合一
        sigma.append(-1)
        return sigma
    #如果不这样的话，在循环里面操作每次都要创建匿名变量我觉得效率低开销大
    f_list = f[1:]
    g_list = g[1:]
    k = 0
    #只要两个公式还不相等
    while f_list!=g_list:
        #找到第一个不等的位置，接下来就要考虑做替换
        i = findDisagreement(f_list,g_list)
        #如果不一样的这一项的首字母都是小写，那么就是说这两个都是项，无论如何都不能做合一
        if f_list[i][0].islower() and g_list[i][0].islower():
            #清空已经找到的替换
            sigma.clear()
            #加入-1表示不可能合一
            sigma.append(-1)
            return sigma
        #如果f的这个是变量，但是g里面的项包含了它
        elif f_list[i][0].isupper() and variableInterm(f_list[i],g_list[i])!=-1:
            # 清空已经找到的替换
            sigma.clear()
            # 加入-1表示不可能合一
            sigma.append(-1)
            return sigma
        # 如果g的这个是变量，但是f里面的项包含了它
        elif g_list[i][0].isupper() and variableInterm(g_list[i],f_list[i]) != -1:
            # 清空已经找到的替换
            sigma.clear()
            # 加入-1表示不可能合一
            sigma.append(-1)
            return sigma
        #至少有一个是变量而且不相互包含，可以替换合一
        else:
            k = k+1
            tempf = f_list[i]
            tempg = g_list[i]
            #优先考虑f的是变量
            if f_list[i][0].isupper():
                for sub in sigma:
                    if sub[1]== tempf:
                        sub[1] = tempg
                    else:
                        index = variableInterm(tempf,sub[1])
                        if index != -1:
                            sub[1] = multi_sub(sub[1],tempg,index,index + len(tempf))

                for sub in sigma:
                    if sub[0]==sub[1]:
                        sigma.remove(sub)

                sigma.append([f_list[i],g_list[i]])

                for j in range(len(f_list)):
                    if f_list[j] == tempf:
                        f_list[j] = tempg
                    else:
                        index = variableInterm(tempf, f_list[j])
                        if index != -1:
                            #f_list[j][index:index + len(tempf)] = tempg
                            f_list[j] = multi_sub(f_list[j],tempg,index,index + len(tempf))
                for j in range(len(g_list)):
                    if g_list[j] == tempf:
                        g_list[j] = tempg
                    else:
                        index = variableInterm(tempf, g_list[j])
                        if index != -1:
                            #g_list[j][index:index + len(tempf)] = tempg
                            g_list[j] = multi_sub(g_list[j],tempg,index,index + len(tempf))


            else:
                for sub in sigma:
                    if sub[1]== tempg:
                        sub[1] = tempf
                    else:
                        index = variableInterm(tempg,sub[1])
                        if index != -1:
                            sub[1] = multi_sub(sub[1],tempf,index,index + len(tempg))
                for sub in sigma:
                    if sub[0]==sub[1]:
                        sigma.remove(sub)
                #含义就是用后面的替换前面的，就相当于省略了等号
                sigma.append([g_list[i],f_list[i]])
                tempf = f_list[i]
                tempg = g_list[i]
                for j in range(len(g_list)):
                    if g_list[j] == tempg:
                        g_list[j] = tempf
                    else:
                        index = variableInterm(tempg,g_list[j])
                        if index!=-1:
                            #g_list[j][index:index+len(tempg)] = tempf
                            g_list[j] = multi_sub(g_list[j],tempf,index,index + len(tempg))
                for j in range(len(f_list)):
                    if f_list[j] == tempg:
                        f_list[j] = tempf
                    else:
                        index = variableInterm(tempg, f_list[j])
                        if index != -1:
                            #f_list[j][index:index + len(tempg)] = tempf
                            f_list[j] = multi_sub(f_list[j],tempf,index,index+len(tempg))


    #在找MGU的时候不要进行替换
    #f[1:] = f_list
    #g[1:] = g_list
    return sigma

#消解两个子句
#def resolution(f, g):


# num = 0
# clauses = []
# num=int(input())
# for i in range(0, num):
#     clause = []
#     for item in re.findall(r'¬*[a-zA-Z]+\([a-zA-Z,\s]*\)', input()):
#         items = re.findall(r'[¬a-zA-Z]+', item)
#         clause.append(items)
#
#     clauses.append(clause)
# print(clauses)

test1 = ['P','X','X']
test2 = ['¬P','Y','f(Y)']
sigma = MGU(test1,test2)
print(sigma)

