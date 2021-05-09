import pandas as pd
import matplotlib.pyplot as plt
df = pd.read_csv("test.csv")
#for every algo
#polygons vs time
#polygons vs size

df.plot(x='num', y='time')
plt.show()