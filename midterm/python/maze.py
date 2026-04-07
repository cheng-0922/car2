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
        moveDir=dict()
        # use Node as key to store in visitd,parent 
        if node not in self.nodes:
            return False

        queue = deque([node])
        visited.append(node)
        # nodes[start_node]['visited'] = True

        while queue:
            u = queue.popleft()
            for i in u.get_successors:
                v=self.node_dict[i[0]]
                if i[0] > 0 and v in self.nodes and v[0] not in visited: #and not nodes[v]['visited']
                    visited.append(v)
                    parent[v]=u
                    moveDir[v]=i[1]
                    queue.append(v)

        return None

    def BFS_2(self, node_from: Node, node_to: Node):
        # TODO : similar to BFS but with fixed start point and end point
        # Tips : return a sequence of nodes of the shortest path
        visited=[]
        parent=dict()
        moveDir=dict()
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
                    moveDir[v]=i[1]
                    queue.append(v)

        path=[]
        v=node_to

        while(v in self.nodes):
            path.append(v)
            if v==node_from: break
            v=parent[v]
        path = path[::-1]

        final_path = ""
        last_dir=-1
        for i in range(1, len(path)):
            target_node = path[i]
            prev_node = path[i-1]

            curr_dir = moveDir[target_node]
            if i == 1:
                final_path += "f"
            else:
                last_dir=moveDir[prev_node]
                if last_dir == curr_dir:
                    final_path += "f"
                elif (last_dir == 1 and curr_dir == 4) or (last_dir == 2 and curr_dir == 3) or \
                    (last_dir == 3 and curr_dir == 1) or (last_dir == 4 and curr_dir == 2):
                    final_path += "r"
                elif (last_dir == 1 and curr_dir == 3) or (last_dir == 2 and curr_dir == 4) or \
                    (last_dir == 3 and curr_dir == 2) or (last_dir == 4 and curr_dir == 1):
                    final_path += "l"
                else:
                    final_path += "b"
                
        return final_path

        return None
    def testBFS(self, node_from:int, node_to):
        return self.BFS_2(self.node_dict[node_from],self.node_dict[node_to])
    


    def getAction(self, car_dir, node_from: Node, node_to: Node):
        # TODO : get the car action
        # Tips : return an action and the next direction of the car if the node_to is the Successor of node_to
        # If not, print error message and return 0
        return None

    def getActions(self, nodes: List[Node]):
        # TODO : given a sequence of nodes, return the corresponding action sequence
        # Tips : iterate through the nodes and use getAction() in each iteration
        return None

    def actions_to_str(self, actions):
        # cmds should be a string sequence like "fbrl....", use it as the input of BFS checklist #1
        cmd = "fbrls"
        cmds = ""
        for action in actions:
            cmds += cmd[action - 1]
        log.info(cmds)
        return cmds

    def strategy(self, node: Node):
        return self.BFS(node)

    def strategy_2(self, node_from: Node, node_to: Node):
        return self.BFS_2(node_from, node_to)

m = Maze('maze (1).csv')
print(m.testBFS(1,10))