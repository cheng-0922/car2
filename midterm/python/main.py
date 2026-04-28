import argparse
import logging
import os
import sys
import time

# import numpy as np
# import pandas
# from BTinterface import BTInterface
from maze import Action, Maze
from score import ScoreboardServer, ScoreboardFake

from hm10_esp32 import HM10ESP32Bridge
import time
import sys
import threading

class Timer:
    def __enter__(self):
        self.start = time.time()

    def __exit__(self, exc_type, exc_val, exc_tb):
        print(time.time() - self.start)

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

log = logging.getLogger(__name__)

# TODO : Fill in the following information
TEAM_NAME = "TEAM2"
SERVER_URL = "http://140.112.175.18"
MAZE_FILE = "data/medium_maze.csv"
# MAZE_FILE = "data/big_maze_114.csv"

BT_PORT = 'COM4'


PORT = 'COM4'
EXPECTED_NAME = 'HM10_Car2'

def background_listener(bridge,point,maze, nodelist):
    now_pos = maze.get_start_point()
    
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
    stop=False
    while True:
        byte = bridge.ser.read(1)

        if not byte:
            continue

        # 🔹 UID 封包
        if byte == b'\xAA':
            data = bridge.ser.read(4)

            if len(data) == 4:
                hex_str = data.hex().upper()
                print(f"[UID] {hex_str}")
                point.add_UID(hex_str)

        # 🔹 控制訊號（字元）
        else:
            car_msg = byte.decode(errors='ignore')

            # print(f"[CMD] {car_msg}")

            if car_msg == 'n':
                now_pos = next_pos
                if len(nodelist) > 0:
                    next_pos = nodelist.pop()
                    cmd = cmds[maze.getAction(car_dir, now_pos, next_pos) - 1]
                else:
                    if stop:
                        bridge.send('x')
                    stop = True

                if maze.getAction(car_dir, now_pos, next_pos):
                    cmd = cmds[maze.getAction(car_dir, now_pos, next_pos) - 1]
                else:
                    if endend:
                        time.sleep(1)
                        cmd = 'x'
                    else:
                        cmd = 's'
                        endend = True

                bridge.send(cmd)
                print(car_msg)

            elif car_msg == 'd':
                match car_dir:
                    case 1: car_dir = 4
                    case 2: car_dir = 3
                    case 3: car_dir = 1
                    case 4: car_dir = 2
                print(car_msg)
                

            elif car_msg == 'a':
                match car_dir:
                    case 1: car_dir = 3
                    case 2: car_dir = 4
                    case 3: car_dir = 2
                    case 4: car_dir = 1
                print(car_msg)
                

            elif car_msg == 's':
                match car_dir:
                    case 1: car_dir = 2
                    case 2: car_dir = 1
                    case 3: car_dir = 4
                    case 4: car_dir = 3
                print(car_msg)

            elif car_msg == 'w':
                print(car_msg)
        # time.sleep(0.0001)

            
                
                
class Bluetooth:     
    def __init__(self, port):
        self.bridge = HM10ESP32Bridge(port)
        
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
        bl = Bluetooth(bt_port)
        maze = Maze(maze_file)
        
        bl.bridge.send('x') #stop initially
        time.sleep(2)
        print('wait 1 second, please put it to start')
        start = 'q'
        while start=='q':
            start = input("key to start")

        
        point = ScoreboardServer(team_name, server_url)
        
        now_pos = maze.get_start_point()
        nodelist = maze.strategy(now_pos)[::1]

        nodelist.reverse()
        print(len(nodelist))
        threading.Thread(target=background_listener, args=(bl.bridge,point,maze,nodelist), daemon=True).start()
        try:
            while True:
                user_msg = input("You: ")
                if user_msg.lower() in ['exit', 'quit']: break
                if user_msg: bl.bridge.send(user_msg)
        except (KeyboardInterrupt, EOFError):
            pass

    elif mode == "1":
        log.info("Mode 1: Self-testing mode.")
        # TODO: You can write your code to test specific function.
        bl = Bluetooth(bt_port)
        maze = Maze(maze_file)

        bl.bridge.send('x') #stop initially
        time.sleep(2)
        print('wait 1 second, please put it to start')
        start = 'q'
        while start=='q':
            start = input("key to start")


        point = ScoreboardFake("your team name", "data/fakeUID.csv") # for local testing
        
        now_pos = maze.get_start_point()
        nodelist = maze.strategy(now_pos)        
        # nodelist.extend(maze.strategy(nodelist[-1])[1::1])
        # nodelist.extend(maze.strategy(nodelist[-1])[1::1])
        # nodelist.extend(maze.strategy(nodelist[-1])[1::1])
        # nodelist.extend(maze.strategy(nodelist[-1])[1::1])

        nodelist.reverse()
        print(len(nodelist))
        threading.Thread(target=background_listener, args=(bl.bridge,point,maze,nodelist), daemon=True).start()
        try:
            while True:
                user_msg = input("You: ")
                if user_msg.lower() in ['exit', 'quit']: break
                if user_msg: bl.bridge.send(user_msg)
        except (KeyboardInterrupt, EOFError):
            pass
    elif mode == "2":
        log.info("Mode 2: Self-testing mode.")
        # Text Debug, Use keyboard send car_msg
        maze = Maze(maze_file)
        point = ScoreboardServer(team_name, server_url)
        

if __name__ == "__main__":
    args = parse_args()
    main(**vars(args))

# main("1",'COM4', 'WED2', SERVER_URL,MAZE_FILE)
