# car2
```bash
car2/
 ├── README.md                   # 專案說明文件
 ├── legacy/  
 │   ├── motorPID.h              # 舊版馬達控制標頭檔 (3/28)
 │   ├── motorPID.ino            # 舊版馬達控制函式 (3/28)
 │   ├── motor/                  # 舊版馬達控制函式、循跡(3/25)
 │   │   └── motor.ino    
 │   └── midterm_project/        # 基於 sample code 的草稿   
 │       ├── RFID.h              
 │       ├── bluetooth.h         
 │       ├── midterm_project.ino 
 │       ├── node.h              
 │       └── track.h             
 └── midterm/                    # 放置主要程式碼
     ├── .gitignore             
     ├── LICENSE
     ├── README.md
     ├── arduino/                # 車體控制 (原 init_hm10/)
     │   ├── init_hm10.ino       # 循跡車 Mega3650 之主程式：通訊、讀取UID、存取指令
     │   └── motorPID.h          # 馬達控制函式、PD循跡判斷、node 偵測與方向控制
     └── python/                 # 邊緣運算端 / 演算法主控 
         ├── requirements.txt
         ├── main.py             # 電腦指令發送主程式
         ├── maze.py             # 載入迷宮與演算法
         ├── node.py             # 定義地圖node class
         ├── score.py            # 計分程式
         ├── data/               # 迷宮csv檔案
         │   ├── big_maze_114.csv
         │   ├── fakeUID.csv
         │   ├── maze (1).csv
         │   ├── medium_maze.csv
         │   └── small_maze.csv
         └── hm10_esp32          # 藍牙通訊模組，取自第三周課程提供版本
             ├── README.md
             ├── __init__.py
             └── hm10_esp32_bridge.py

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
this is newest version for arduino control

