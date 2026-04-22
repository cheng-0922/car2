import csv
import logging
import math
from enum import IntEnum
from typing import List

import numpy as np
import pandas

from node import Direction, Node
from collections import deque #queue
log = logging.getLogger(__name__)


class Action(IntEnum):
    ADVANCE = 1
    U_TURN = 2
    TURN_RIGHT = 3
    TURN_LEFT = 4
    HALT = 5


class Maze:
    def __init__(self, filepath: str):
        # TODO : read file and implement a data structure you like
        # For example, when parsing raw_data, you may create several Node objects.
        # Then you can store these objects into self.nodes.
        # Finally, add to nd_dict by {key(index): value(corresponding node)}
        self.raw_data = pandas.read_csv(filepath).values
        self.nodes = []
        self.node_dict = dict()  # key: index, value: the correspond node
        for node_i in self.raw_data: #construct Nodes
            new_n = Node(node_i[0])
            self.nodes.append(new_n)
            self.node_dict[node_i[0]] = new_n

        for node_i in self.raw_data: #update adjacency of nodes
            n=self.node_dict[node_i[0]]
            for i in [1,2,3,4]:
                if not pandas.isna(node_i[i]):
                    n.set_successor(self.node_dict[node_i[i]],i,node_i[i+4])


    def get_start_point(self):
        if len(self.node_dict) < 2:
            log.error("Error: the start point is not included.")
            return 0
        return self.node_dict[1]

    def get_node_dict(self):
        return self.node_dict

    def BFS(self, node: Node):
        # TODO : design your data structure here for your algorithm
        # Tips : return a sequence of nodes from the node to the nearest unexplored deadend
        visited=[]
        parent=dict()
        # use Node as key to store in visitd,parent 
        if node not in self.nodes:
            return None

        queue = deque([node])
        visited.append(node)
        # nodes[start_node]['visited'] = True
        node_to=Node()
        while queue:
            u = queue.popleft()
            if len(u.get_successors())==1 and u != node:
                    node_to=u
                    break
            for i in u.get_successors():
                
                v = i[0]
                if v in self.nodes and v not in visited: #and not nodes[v]['visited']
                    visited.append(v)
                    parent[v]=u
                    queue.append(v)

        path=[]
        

        while(v in self.nodes):
            path.append(v)
            if v==node: break
            v=parent[v]
        path = path[::-1]

        return path
    
    def testBFS(self, node_from:int):
        return self.BFS(self.node_dict[node_from])

    def BFS_2(self, node_from: Node, node_to: Node):
        # TODO : similar to BFS but with fixed start point and end point
        # Tips : return a sequence of nodes of the shortest path
        visited=[]
        parent=dict()
        # use Node as key to store in visitd,parent 
        if node_from not in self.nodes:
            return None

        queue = deque([node_from])
        visited.append(node_from)
        # nodes[start_node]['visited'] = True

        while queue:
            u = queue.popleft()
            for i in u.get_successors():
                v = i[0]
                if v in self.nodes and v not in visited: #and not nodes[v]['visited']
                    visited.append(v)
                    parent[v]=u
                    queue.append(v)

        path=[]
        v=node_to

        while(v in self.nodes):
            path.append(v)
            if v==node_from: break
            v=parent[v]
        path = path[::-1]

        return path

        return None
    def testBFS2(self, node_from:int, node_to):
        return self.BFS_2(self.node_dict[node_from],self.node_dict[node_to])
    


    def getAction(self, car_dir, node_from: Node, node_to: Node):
        # TODO : get the car action
        # Tips : return an action and the next direction of the car if the node_to is the Successor of node_to
        # If not, print error message and return 0
        future_dir= node_from.get_direction(node_to)
        if future_dir==0:
            return None
        
        
        if ( car_dir==1 and future_dir==2 ) or (car_dir==3 and future_dir ==4) or \
            ( car_dir==2 and future_dir==1) or ( car_dir==4 and future_dir==3 ):
            return Action(2)
        elif ( car_dir==1 and future_dir==3) or ( car_dir==2 and future_dir==4 ) or \
            ( car_dir==3 and future_dir==2 ) or ( car_dir==4 and future_dir==1 ):
            return Action(4)
        elif ( car_dir==1 and future_dir==4 ) or ( car_dir==2 and future_dir==3 ) or\
            ( car_dir==3 and future_dir==1 ) or ( car_dir==4 and future_dir==2 ):
            return Action(3)
        elif car_dir==future_dir:
            return Action(1)
        else:
            return 0
        

    def getActions(self, nodes: List[Node]):
        # TODO : given a sequence of nodes, return the corresponding action sequence
        # Tips : iterate through the nodes and use getAction() in each iteration
        acts=[]
        curr_dir=0
        
        for i in range(1,len(nodes)):
            target_node = nodes[i]
            prev_node = nodes[i-1]

            if i==1:
                acts.append(Action(1))
            else:
                acts.append(self.getAction(curr_dir,prev_node,target_node))

            
            curr_dir = prev_node.get_direction(target_node)

        return acts

    def actions_to_str(self, actions):
        # cmds should be a string sequence like "fbrl....", use it as the input of BFS checklist #1
        # cmd = "fbrls"
        cmd = "wsdax"
        cmds = ""
        for action in actions:
            cmds += cmd[action - 1]
        log.info(cmds)
        return cmds

    def strategy(self, node: Node):
        result = [node]
        start = node
        # result.extend(self.BFS_2(start,self.node_dict[7])[1:])
        # start=self.node_dict[7]
        for dist in self.nodes[::-1]:
            if (not dist in result )and len(dist.get_successors())==1:
                result.extend(self.BFS_2(start,dist)[1:])
                # new_list = self.BFS_2(start,dist)
                # if new_list: result.append(self.BFS_2(start,dist))
                start = dist
        return result

    def strategy_2(self, node_from: Node, node_to: Node):
        return self.BFS_2(node_from, node_to)
    
    def point(self, start: Node, node: Node):
        nodelist = self.BFS_2(start, node)
        ns = 0
        ew = 0
        node_prev = nodelist[0]
        for node in nodelist:
            i = node.get_direction(node_prev)
            match(i):
                case 1 : ns+=1
                case 2 : ns-=1
                case 3 : ew+=1
                case 4 : ew-=1
                case _ : pass
            node_prev = node
        return abs(ns)+abs(ew)
        





# m = Maze("data/big_maze_114.csv")
# # nodelist = m.testBFS2(1,12)
# nodelist=m.strategy(m.nodes[24])
# acts = m.getActions(nodelist)

# # print(f'route:{m.actions_to_str(acts)}')
# # for node in nodelist:
# #     print(node.get_index())

# nodelist=m.strategy(m.nodes[25])

# acts = m.getActions(nodelist)
# point_dict = dict()
# route_dict = dict()
# avg_dict = dict()
# for dist in m.nodes:
#     if dist.get_index() == 25: continue
#     if len(dist.get_successors())==1 :
#         nl = m.BFS_2(m.nodes[24], dist)
#         route_dict[int(dist.get_index())]= len(nl)-1
#         point_dict[int(dist.get_index())]= m.point(m.nodes[24],dist)
#         avg_dict [int(dist.get_index())]=  (m.point(m.nodes[24],dist)) /(len(nl)-1) 


# print(point_dict)
# print(route_dict)
# print(avg_dict)
# for node in nodelist[::1]:
#     print(int(node.get_index()))
#     nodelist.pop()

    
# print(f'route:{m.actions_to_str(acts)}')
# for node in nodelist:
#     print(node.get_index())
# for nodelist in m.strategy(m.nodes[0]):
#     acts = m.getActions(nodelist)
#     print(f'route:{m.actions_to_str(acts)}')
# nodelist = m.testBFS(1)
# acts = m.getActions(nodelist)
# print(m.actions_to_str(acts))