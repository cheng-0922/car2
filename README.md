# car2

contributor： 何承憲、林侑葶、李旻諺

### 資料夾架構

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

### 執行方式
進入`car2\midterm\python\`，於terninal執行 
```bash
python main.py [-h] [--maze-file MAZE_FILE] [--bt-port BT_PORT] [--team-name TEAM_NAME]
               [--server-url SERVER_URL]
               mode
```

範例

(比賽時使用)：

`python main.py 0 --maze-file data/big_maze_114.csv --bt-port COM4 `

(測試模式)：

`python main.py 1 --maze-file data/medium_maze.csv --bt-port COM4 --server-url http://140.112.175.18`


### branch 說明
```bash
PID
  └── control
        └── alt          ← 第一個可運作完整版本
              └── new    ← 節點偵測邏輯改版
                    └── newmotor     ← 換馬達後調參
                          └── newnewmotor  ← 最終調參
                                └── main  ← 整合最終版

BFS  （獨立開發，最終整合進 alt）
```
