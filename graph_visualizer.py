from typing import Mapping
import networkx as nx
import matplotlib.pyplot as plt
import pydot 

G = nx.DiGraph() 
file = open("./basic_block.txt", 'r')

n = int(file.readline())

func = []

for i in range(n):
    func.append(file.readline())
    length = len(func[i])
    func[i] = func[i][0:length-1]

mapping = dict()

for i in range(n):
    mapping[i] = func[i]


# print(mapping)
adj = []

for i in range(n):
    str = file.readline()
    row = str.split(' ')
    introw = []
    for j in range(n):
        if row[j] == '1':
            G.add_edge(i,j)

file.close()
H = nx.relabel_nodes(G,mapping)
nx.draw(H, with_labels=True, node_color="none")
plt.show() 

