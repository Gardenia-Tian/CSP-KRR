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

#f g是两个子句，查找两个子句中可以用于消解的谓词下标
def findPredicateToResolution(f, g):
    #对f中的每一个formula
    for i in range(len(f)):
        #在j中查找判断
        for j in range(len(g)):
            if (f[i][0] == g[j][0][1:] and g[j][0][0] == '¬') or (f[i][0][1:] == g[j][0] and f[i][0][0]=='¬'):
                return i,j
    return -1,-1

def findDisagreement(f, g):
    #从零开始比就可以，因为传进来就已经把为此的部分截掉了
    for i in range(len(f)):
        if f[i]!=g[i]:
            return i
    #
    return -1

#b里面包含a
def variableInterm(a, b):
    ret = []
    index1 = b.find('(')
    index2 = b.find(')')
    if(index1!=-1 and index2!=-1):
        #res现在是括号里的所有的
        res = b[index1+1:index2]
        it = re.finditer('[(, ,]'+a+'[), ,]',b)
        #注意这里得到的下标是带两边的符号的，记得-1和+1
        for match in it:
            ret.append(match.span()[0]+1)

    #没匹配上就返回空列表,匹配上了就返回起始下标
    return ret

#这里只是一个字符串的替换函数
def multi_sub(oristring, substring, ibegin, iend):
    new = list(substring)
    ori = list(oristring)
    ori[ibegin:iend] = iter(new)
    return ''.join(ori)

#有两个formula:f ，还有一个替换sigma,现在要把sigma作用在f上
def substitude(f ,sigma):
    #不要让sigma做外层循环，让每一个公式的项做外层循环，这样实现的才是同时替换
    #因为替换后下一次就不会在考虑他了
    for i in range(1,len(f)):
        for sub in sigma:
            #如果匹配可以替换
            if f[i]==sub[0]:
                f[i] = sub[1]
            else:
                #否则找里面的是不是能有可以替换的
                index = variableInterm(sub[0],f[i])
                for ind in index:
                    f[i] = multi_sub(f[i],sub[1], ind, ind + len(sub[0]))





#做一个简单的trick，和prolog保持一致，如果小写字母开头就是常量，如果大写字母开头就是变量
#f和g表示两个公式
def MGU(f, g):
    sigma = []
    # 如果不这样的话，在循环里面操作每次都要创建匿名变量我觉得效率低开销大
    f_list = f[1:]
    g_list = g[1:]
    k = 0
    if len(f) != len(g) or (not isSamePredicate(f[0],g[0])):
        #表示两个公式不能合一
        #sigma.append(-1)
        return None
    #只要两个公式还不相等
    while f_list!=g_list:
        #找到第一个不等的位置，接下来就要考虑做替换
        i = findDisagreement(f_list,g_list)
        #如果不一样的这一项的首字母都是小写，那么就是说这两个都是项，无论如何都不能做合一
        if f_list[i][0].islower() and g_list[i][0].islower():
            return None
        #如果f的这个是变量，但是g里面的项包含了它
        elif f_list[i][0].isupper() and variableInterm(f_list[i],g_list[i])!=[]:
            return None
        # 如果g的这个是变量，但是f里面的项包含了它
        elif g_list[i][0].isupper() and variableInterm(g_list[i],f_list[i]) != []:
            return None
        #至少有一个是变量而且不相互包含，可以替换合一
        else:
            k = k+1
            tempf = f_list[i]
            tempg = g_list[i]
            #优先考虑f的是变量
            if f_list[i][0].isupper():
                #在找合一的时候就已经替换过了
                for sub in sigma:
                    if sub[1]== tempf:
                        sub[1] = tempg
                    else:
                        index = variableInterm(tempf,sub[1])
                        for ind in index:
                            sub[1] = multi_sub(sub[1],tempg,ind,ind + len(tempf))
                sigma.append([f_list[i], g_list[i]])
                for sub in sigma:
                    if sub[0]==sub[1]:
                        sigma.remove(sub)


                for j in range(len(f_list)):
                    if f_list[j] == tempf:
                        f_list[j] = tempg
                    else:
                        index = variableInterm(tempf, f_list[j])
                        for ind in index:
                            f_list[j] = multi_sub(f_list[j],tempg,ind,ind + len(tempf))
                for j in range(len(g_list)):
                    if g_list[j] == tempf:
                        g_list[j] = tempg
                    else:
                        index = variableInterm(tempf, g_list[j])
                        for ind in index:
                            g_list[j] = multi_sub(g_list[j], tempg, ind, ind + len(tempf))
            else:
                for sub in sigma:
                    if sub[1]== tempg:
                        sub[1] = tempf
                    else:
                        index = variableInterm(tempg,sub[1])
                        for ind in index:
                            sub[1] = multi_sub(sub[1], tempf, ind, ind + len(tempg))
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
                        for ind in index:
                            g_list[j] = multi_sub(g_list[j], tempf, ind, ind + len(tempg))

                for j in range(len(f_list)):
                    if f_list[j] == tempg:
                        f_list[j] = tempf
                    else:
                        index = variableInterm(tempg, f_list[j])
                        for ind in index:
                            f_list[j] = multi_sub(f_list[j], tempf, ind, ind + len(tempg))

    #在找MGU的时候不要进行替换
    #f[1:] = f_list
    #g[1:] = g_list
    #把替换后的也返回，这样如果要用就很方便
    return sigma




#消解两个子句,这里只是消解两个子句，后面再写一个函数消解所有的
#如果能消解，就把消解过程写进clauses,返回true；否则什么都不写，返回false
def resolution_two(f, g, clauses, ope):
    # Y.append(chr(97 + index))
    inf = clauses.index(f)
    ing = clauses.index(g)
    while(1):
        # 首先判断能不能消解，消解只有一种形式，同一谓词有非和无非
        indexf,indexg = findPredicateToResolution(f,g)
        if indexf==-1 and indexg==-1:
            break
        sigma = MGU(f[indexf],f[indexg])
        #说明存在合一
        if sigma != None:
            del(f[indexf])
            del(g[indexg])
            substitude(f,g,sigma)
            #还要把这次替换的结果写到clauses中
            newclause = list(set(f + g))
            clauses.append(newclause)
            ope.append([inf,chr(97+indexf),ing,chr(97+indexg),sigma,newclause])



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

test1 = ['P','f(X)','Z']
test2 = ['P','Y','X']
sigma = MGU(test1,test2)
print(sigma)







