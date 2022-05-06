from cfgexplorer import cfg_explore
import networkx as nx
from networkx.drawing.nx_agraph import read_dot

cfg_explore(binary=r'sample', output='./cfg_output.svg')

# G = read_dot('./cfg_output.dot')

# print(G.nodes.data(True))

# print(G.edges)



