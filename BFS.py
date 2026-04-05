import csv
from collections import deque

nodes = {}

def read_maze_csv(filename):
    global nodes
    nodes = {} 
    try:
        # 使用 utf-8-sig 處理 Excel 存檔可能產生的隱藏字元 (BOM)
        with open(filename, mode='r', encoding='utf-8-sig') as f:
            reader = csv.DictReader(f)
            for row in reader:
                if not row['index']: continue
                u = int(row['index'])
                
                neighbors = [0, 0, 0, 0]
                directions = ['North', 'South', 'West', 'East']
                for i, d_name in enumerate(directions):
                    if d_name in row and row[d_name] and row[d_name].strip():
                        neighbors[i] = int(row[d_name])
                
                nodes[u] = {
                    'to': neighbors,
                    'parent': 0,
                    'moveDir': -1,
                    'visited': False
                }
        print(f"成功讀取了 {len(nodes)} 個節點。")
    except FileNotFoundError:
        print(f"錯誤：找不到檔案 '{filename}'")

def do_bfs(start_node):
    if start_node not in nodes:
        return False

    queue = deque([start_node])
    nodes[start_node]['visited'] = True
    
    while queue:
        u = queue.popleft()
        for i in range(4):
            v = nodes[u]['to'][i]
            if v > 0 and v in nodes and not nodes[v]['visited']:
                nodes[v]['visited'] = True
                nodes[v]['parent'] = u
                nodes[v]['moveDir'] = i 
                queue.append(v)
    return True

def get_path(start_node, end_node):
    if end_node not in nodes or not nodes[end_node]['visited']:
        return "No path found"

    path = []
    curr = end_node
    while curr != 0:
        path.append(curr)
        if curr == start_node: break
        curr = nodes[curr]['parent']
    path.reverse()
    
    final_path = ""
    # 我們從路徑的第二個點開始看，因為第一個點是起點，還沒有移動
    for i in range(1, len(path)):
        target_node = path[i]
        prev_node = path[i-1]
        
        # 這次移動的方向
        curr_dir = nodes[target_node]['moveDir']
        
        # 如果是第一步移動，我們通常預設它是 'f' (前進)
        if i == 1:
            final_path += "f"
        else:
            # 取得「上一步」移動的方向來做比較
            last_dir = nodes[prev_node]['moveDir']
            
            if last_dir == curr_dir:
                final_path += "f"
            elif (last_dir == 0 and curr_dir == 3) or (last_dir == 1 and curr_dir == 2) or \
                 (last_dir == 2 and curr_dir == 0) or (last_dir == 3 and curr_dir == 1):
                final_path += "r"
            elif (last_dir == 0 and curr_dir == 2) or (last_dir == 1 and curr_dir == 3) or \
                 (last_dir == 2 and curr_dir == 1) or (last_dir == 3 and curr_dir == 0):
                final_path += "l"
            else:
                final_path += "b"
                
    return final_path

if __name__ == "__main__":
    # 確認檔名與你上傳的一致
    read_maze_csv('maze (8).csv')
    
    if nodes:
        try:
            s_input = input("請輸入起點: ")
            e_input = input("請輸入終點: ")
            if s_input and e_input:
                start, end = int(s_input), int(e_input)
                if do_bfs(start):
                    print("結果指令序列:", get_path(start, end))
                else:
                    print("搜尋失敗，可能路徑不通。")
        except ValueError:
            print("請輸入數字。")