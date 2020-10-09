import matplotlib.pyplot as plt
import numpy as np
A=[0,0.001,0.009,0.03,0.079,0.131,0.223,0.511,0.75,1.227]
B=[0,0.001,0.009,0.052,0.082,0.174,0.262,0.488,0.72,0.884]
plt.plot([i*45+2 for i in range(10)],A)
plt.xlabel('tamaño de max')
plt.ylabel('tiempo')
plt.legend(["Mult clasica"])
plt.grid(True)
plt.savefig('G2'+'.png')
plt.show()