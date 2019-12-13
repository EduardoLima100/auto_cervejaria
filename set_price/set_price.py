import numpy as np
from scipy.linalg import solve
import matplotlib.pyplot as plt

custo_insumos = 218.63
rendimento = 46
custo_L = round(custo_insumos / rendimento,2)
print("Custo/Litro (sem tampinha): ", custo_L)
custo_tampa = 0.14
fator_lucro = 3


X = [355,500,600,740,1000]
Y = [5.5,7.5,8.5,10,12.5]
lucro = []

print("\nCusto de cada garrafa:")
for i in range(len(X)):
    Y[i] = ((custo_L/1000)*X[i]+custo_tampa)*fator_lucro
    lucro.append(Y[i]-(custo_L/1000)*X[i]+custo_tampa)
    print(X[i], round((custo_L/1000)*X[i]+custo_tampa,2))
      
print("\nPreço/Litro de cada garrafa:")  
preco_L = []  
for i in range(len(X)):
    preco_L.append(round(Y[i]/(X[i]/1000),2))
    print(X[i],preco_L[-1])

X = np.array(X)
Y = np.array(Y)

def linear_regression(X,Y):
    # Calcula os elementos das marizes
    a11 = np.sum(X**2)
    a12 = np.sum(X)
    a22 = len(X)
    b1 = np.sum(X * Y)
    b2 = np.sum(Y)

    # Monta e resolve o sistema
    A = np.array([[a11, a12], [a12, a22]])

    B = np.array([b1,b2])

    a = solve(A, B)

    # define a funcao g(x) para plotar 
    return lambda x: a[0]*x+a[1]

def second_degree_regression(X,Y):
    # Calcula os elementos das marizes
    A11 = np.sum(X**4)
    A12 = np.sum(X**3)
    A13 = np.sum(X**2)
    A23 = np.sum(X)
    A33 = len(X)
    B1 = np.sum(X**2*Y) 
    B2 = np.sum(X*Y)
    B3 = np.sum(Y)
    
    # Monta e resolve o sistema
    A = np.array([[A11,A12, A13],[A12, A13, A23], [A13, A23, A33]])
    B = np.array([B1,B2,B3])
    a = solve(A, B)
    
    # define a funcao g(x) para plotar 
    print("\n",round(a[0],2),"x²+",round(a[1],2),"x+",round(a[2],2))
    return lambda x: a[0]*x*x+a[1]*x+a[2]
    
def plot_XY(X,Y):
    plt.plot(X,Y,'.')
    #plt.axis('square')
    plt.grid()
    plt.show()

def plot_func(X,Y,f):
    Xr = np.arange(X[0]-1, X[-1]+1, 0.1)
        
    # Plota os pontos e a reta
    plt.plot(X, Y, ".", Xr, f(Xr), "-") 
    #plt.axis('square')
    plt.grid()
    plt.show()

def projections(X,Y,n):
    f = linear_regression(X,Y)
    g = second_degree_regression(X,Y)
    x = len(X)+n
    
    a = int((f(x)+g(x))/2)
    b = int((a + Y.sum()/len(Y))/2)
    c = int(g(x))
    
    R = [a,b,c]
    R.sort()
    return R

def arredondar(x,val=0.5):
    a = 0
    while a<x:
        a = a + val
    return a


#plot_func(X,Y,linear_regression(X,Y))
f = second_degree_regression(X,Y)
plot_func(X,Y,f)
print(Y)
print("\nPreços:")
for i in range(len(X)):
    print("{}ml - R${}".format(X[i], arredondar(Y[i])))



