# car2
```bash
car2/
 ├── README.md                  # 專案說明文件
 ├── motorPID.h                 # 舊版馬達控制函式
 ├── motorPID.ino               # 舊版馬達控制函式、循跡
 ├── motor/                     # 舊版馬達控制函式、循跡
 │   └── motor.ino    
 ├── init_hm10/                 # 循跡車 Mega3650 之程式
 │   ├── init_hm10.ino          # 循跡車 Mega3650 之主程式：通訊、讀取UID、存取指令
 │   └── motorPID.h             # 馬達控制函式、循跡判斷
 ├── midterm/                   # 放置主要程式碼
 │   ├── .gitignore             
 │   ├── LICENSE
 │   ├── README.md
 │   ├── arduino/midterm_project # sample code 未採用   
 │   │   ├── RFID.h              # sample code 未採用   
 │   │   ├── bluetooth.h         # sample code 未採用   
 │   │   ├── midterm_project.ino # sample code 未採用   
 │   │   ├── node.h              # sample code 未採用   
 │   │   └── track.h             # sample code 未採用   
 │   └── my_project/     # 
 │       ├── __pycache__
 │       ├── data               # 迷宮csv檔案
 │       │   ├── big_maze_114.csv
 │       │   ├── fakeUID.csv
 │       │   ├── maze (1).csv
 │       │   ├── medium_maze.csv
 │       │   └── small_maze.csv
 │       ├── hm10_esp32         # hm10_esp32通訊函式，取自第三周課程提供版本
 │       │   ├── README.md
 │       │   ├── __init__.py
 │       │   └── hm10_esp32_bridge.py
 │       ├── requirements.txt
 │       ├── main.py            # 電腦指令發送主程式
 │       ├── maze.py            # 載入迷宮與演算法
 │       ├── node.py            # 定義地圖node class
 │       └── score.py           # 計分程式
 └── CarCarImproveWireless      # 藍芽通訊，第三周課程提供版本
```
## midterm
### python/

main.py: OK

maze.py: OK

node.py: OK

score.py: no nead to change


### arduino/

all are not complete, use init_hm10.ino now.

## init_hm10
this is newest version for arduino conntrol

## git commands
```bash
git push origin main // download from branch "main"
git add . // automatically add files 
git commit -m "" // save changes as a version
git push origin main // upload updates to branch "main"
```
