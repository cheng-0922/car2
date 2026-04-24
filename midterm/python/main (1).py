import argparse
import logging
import os
import sys
import time

import numpy as np
import pandas
# from BTinterface import BTInterface
from maze import Action, Maze
from score import ScoreboardServer, ScoreboardFake

from hm10_esp32 import HM10ESP32Bridge
import time
import sys
import threading

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

log = logging.getLogger(__name__)

# TODO : Fill in the following information
TEAM_NAME = "TEAM2"
SERVER_URL = "http://140.112.175.18"
MAZE_FILE = "data/medium_maze.csv"
BT_PORT = ""


PORT = 'COM9'
EXPECTED_NAME = 'HM10_Car2'

def background_listener(bridge,point,maze):
    # fin = maze.get_node_dict()[12]
    # now_pos= maze.get_start_point()
    # nodelist= maze.strategy_2(now_pos, fin)
    # next_pos = nodelist[1]
    # car_dir = now_pos.get_direction(next_pos)
    # cmds = "wsdax"
    # cmd = ''+cmds[maze.getAction(car_dir, now_pos, next_pos)- 1]
    bridge.send('x') #stop initially
    time.sleep(2)
    print('wait 1 second, please put it to start')
    now_pos = maze.get_start_point()
    nodelist = maze.strategy(now_pos)[::-1]
    nodelist.pop()
    next_pos = nodelist.pop()
    car_dir = now_pos.get_direction(next_pos)
    bridge.send('b')
    time.sleep(0.5)
    cmds = "wsdax"

    # the command when entering second
    now_pos= next_pos
    next_pos= nodelist.pop()
    cmd = ''+cmds[maze.getAction(car_dir, now_pos, next_pos)- 1]
    bridge.send(cmd)
    endend = False
    while True:
        car_msg = bridge.listen()
        if car_msg:
            print(f"\r[HM10]: {car_msg}\n", end="")
            stop = False
            # if (car_msg=='b') :
            #     now_pos=next_pos
            #     if len(nodelist)>0: next_pos= nodelist.pop()
            #     else : bridge.send('w')
            #     cmd = ''+cmds[maze.getAction(car_dir, now_pos, next_pos)- 1]

                # bridge.send(cmd)
            if(car_msg=='n'):
                now_pos=next_pos
                if len(nodelist)>0: next_pos= nodelist.pop()
                else : 
                    if(stop) :bridge.send('x')
                    stop = True
                    
                if(maze.getAction(car_dir, now_pos, next_pos)):
                    cmd = ''+cmds[maze.getAction(car_dir, now_pos, next_pos)- 1]
                else : 
                    if(endend): cmd = 'x'
                    else :
                        cmd = 's'
                        endend = True
                

                bridge.send(cmd)
            elif(car_msg=='d'):
                match car_dir:
                    case 1:
                        car_dir = 4
                    case 2:
                        car_dir = 3
                    case 3:
                        car_dir = 1
                    case 4:
                        car_dir = 2
                    case _:
                        pass
            elif(car_msg=='a'):
                match car_dir:
                    case 1:
                        car_dir = 3
                    case 2:
                        car_dir = 4
                    case 3:
                        car_dir = 2
                    case 4:
                        car_dir = 1
                    case _:
                        pass
            elif(car_msg=='s'):
                match car_dir:
                    case 1:
                        car_dir = 2
                    case 2:
                        car_dir = 1
                    case 3:
                        car_dir = 4
                    case 4:
                        car_dir = 3
                    case _:
                        pass
            if(len(car_msg)==8):
                point.add_UID(car_msg)
        time.sleep(0.1)
class Bluetooth:     
    def __init__(self):
        self.bridge = HM10ESP32Bridge(port=PORT)
        
        # 1. Configuration Check
        current_name = self.bridge.get_hm10_name()
        if current_name != EXPECTED_NAME:
            print(f"Target mismatch. Current: {current_name}, Expected: {EXPECTED_NAME}")
            print(f"Updating target name to {EXPECTED_NAME}...")
            
            while not self.bridge.set_hm10_name(EXPECTED_NAME):
                print("❌ Failed to set name. Exiting.")
            

            if self.bridge.set_hm10_name(EXPECTED_NAME):
                print("✅ Name updated successfully. Resetting ESP32...")
                self.bridge.reset()
                # Re-init after reset
                self.bridge = HM10ESP32Bridge(port=PORT)
            else:
                print("❌ Failed to set name. Exiting.")
                
        # 2. Connection Check
        status = self.bridge.get_status()
        if status != "CONNECTED":
            print(f"⚠️ ESP32 is {status}. Please ensure HM-10 is advertising. Exiting.")
            sys.exit(0)

        print(f"✨ Ready! Connected to {EXPECTED_NAME}")
        

        
    
    print("\nChat closed.")



def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("mode", help="0: treasure-hunting, 1: self-testing", type=str)
    parser.add_argument("--maze-file", default=MAZE_FILE, help="Maze file", type=str)
    parser.add_argument("--bt-port", default=BT_PORT, help="Bluetooth port", type=str)
    parser.add_argument(
        "--team-name", default=TEAM_NAME, help="Your team name", type=str
    )
    parser.add_argument("--server-url", default=SERVER_URL, help="Server URL", type=str)
    return parser.parse_args()


def main(mode: int, bt_port: str, team_name: str, server_url: str, maze_file: str):
    
    # point = ScoreboardFake("your team name", "data/fakeUID.csv") # for local testing

    ### Bluetooth connection haven't been implemented yet, we will update ASAP ###
    # interface = BTInterface(port=bt_port)
    # TODO : Initialize necessary variables
    
    if mode == "0":
        log.info("Mode 0: For treasure-hunting")
        # TODO : for treasure-hunting, which encourages you to hunt as many scores as possible
        bl = Bluetooth()
        

    elif mode == 1:
        log.info("Mode 1: Self-testing mode.")
        # TODO: You can write your code to test specific function.
        bl = Bluetooth()
        maze = Maze(maze_file)
        point = ScoreboardServer(team_name, server_url)
        
        threading.Thread(target=background_listener, args=(bl.bridge,point,maze), daemon=True).start()
        try:
            while True:
                user_msg = input("You: ")
                if user_msg.lower() in ['exit', 'quit']: break
                if user_msg: bl.bridge.send(user_msg)
        except (KeyboardInterrupt, EOFError):
            pass
    elif mode == 2:
        log.info("Mode 2: Self-testing mode.")
        # Text Debug, Use keyboard send car_msg
        maze = Maze(maze_file)
        point = ScoreboardServer(team_name, server_url)
        # fin = maze.get_node_dict()[12]
        # now_pos= maze.get_start_point()
        # nodelist= maze.strategy_2(now_pos, fin)
        # next_pos = nodelist[1]
        # car_dir = now_pos.get_direction(next_pos)
        # cmds = "wsdax"
        # cmd = ''+cmds[maze.getAction(car_dir, now_pos, next_pos)- 1]
        print('x')
        time.sleep(1)
        print('wait 1 second, please put it to start')
        now_pos = maze.get_start_point()
        nodelist = maze.strategy(now_pos)[::-1]
        nodelist.pop()
        next_pos = nodelist.pop()
        car_dir = now_pos.get_direction(next_pos)
        # the command when entering second
        now_pos= next_pos
        next_pos= nodelist.pop()
        for i in nodelist:
            print(i.get_index())
        print(f'{now_pos.get_index()}|{next_pos.get_index()}|{maze.getAction(car_dir, now_pos, next_pos)}')
        cmds = "wsdax"
        cmd = ''+cmds[maze.getAction(car_dir, now_pos, next_pos)- 1]
        print(cmd)
        try:
            while True:
                car_msg = input('car_msg?')
                if car_msg:
                    print(f"\r[HM10]: {car_msg}\n", end="")
                    if(len(car_msg)>2):
                        point.add_UID(car_msg)
                    elif(car_msg=='n'):
                        now_pos=next_pos
                        print(f'{now_pos.get_index()}|{next_pos.get_index}|{maze.getAction(car_dir, now_pos, next_pos)}')
                        if len(nodelist)>0: next_pos= nodelist.pop()
                        else : print("end")
                        cmd = ''+cmds[maze.getAction(car_dir, now_pos, next_pos)- 1]

                        print(cmd)
                    elif(car_msg=='d'):
                        match car_dir:
                            case 1:
                                car_dir = 4
                            case 2:
                                car_dir = 3
                            case 3:
                                car_dir = 1
                            case 4:
                                car_dir = 2
                            case _:
                                pass
                    elif(car_msg=='a'):
                        match car_dir:
                            case 1:
                                car_dir = 3
                            case 2:
                                car_dir = 4
                            case 3:
                                car_dir = 2
                            case 4:
                                car_dir = 1
                            case _:
                                pass
                    elif(car_msg=='s'):
                        match car_dir:
                            case 1:
                                car_dir = 2
                            case 2:
                                car_dir = 1
                            case 3:
                                car_dir = 4
                            case 4:
                                car_dir = 3
                            case _:
                                pass
                else:   
                    user_msg = input("You: ")
                    if user_msg.lower() in ['exit', 'quit']: break
                    if user_msg: bl.bridge.send(user_msg)
        except (KeyboardInterrupt, EOFError):
            pass

    else:
        log.error("Invalid mode")
        sys.exit(1)


# if __name__ == "__main__":
#     args = parse_args()
#     main(**vars(args))

main(1,'COM6', 'WED2', SERVER_URL,MAZE_FILE)