import pandas as pd
import matplotlib.pyplot as plt
fig, axes = plt.subplots(nrows=1, ncols=3)
df = pd.read_csv("metrics.csv")

df_sp = df.loc[df["algorithm"] == " spacePartition"]
df_pol = df.loc[df["algorithm"] == " polar"]
df_nv = df.loc[df["algorithm"] == " naivePoly"]

#space partition
axes[0].plot(df_sp['num_polygons'], df_sp['size'], color="blue", label = "Space Partition")
axes[1].plot(df_sp['num_polygons'], df_sp['time'], color="blue", label = "Space Partition")
axes[2].plot(df_sp['num_polygons'], df_sp['time'], color="blue", label = "Space Partition")
#polar
axes[0].plot(df_pol['num_polygons'], df_pol['size'], color="red", label = "Polar")
axes[1].plot(df_pol['num_polygons'], df_pol['time'], color="red", label = "Polar")
axes[2].plot(df_pol['num_polygons'], df_pol['time'], color="red", label = "Polar")
#naive polygon
axes[0].plot(df_nv['num_polygons'], df_nv['size'], color="green", label = "Naive Polygon")
axes[1].plot(df_nv['num_polygons'], df_nv['time'], color="green", label = "Naive Polygon")

axes[0].legend(loc='upper left')
axes[1].legend(loc='upper left')
axes[2].legend(loc='upper left')

axes[0].set_xlabel('Number of polygons') 
axes[0].set_ylabel('Size (in bytes)')

axes[1].set_xlabel('Number of polygons') 
axes[1].set_ylabel('Time (in seconds)')

axes[2].set_xlabel('Number of polygons') 
axes[2].set_ylabel('Time (in seconds)')

plt.suptitle("Metrics analysis")
plt.show()