from unittest import result
from gurobipy import*
import pandas as pd
import numpy as np



def each_run(labels,cluster_i,A,S_H,S_L,L,lambda_value,van_cost,oil_cost):
    file_name = "./Kmeans_clustered_dist_matrix/Clustered_dist_matrix_labels"+str(labels)+".xlsx"
    with pd.ExcelFile(file_name, engine='openpyxl') as reader:
        cluster0 = pd.read_excel(reader, sheet_name='Cluster '+str(cluster_i),header=None)
    
    # 記錄站點名稱
    name_list = [0]
    for i in range(1,cluster0[0].size):
        name_list.append(int(cluster0[0][i]))
    name_list.append(0)

    # 準備矩陣（丟掉第一欄、第一列）
    cluster = cluster0.drop(columns=cluster0.columns[[0]], axis=1)
    cluster = cluster.drop(index=[0], axis=0)

    # 製作加入虛擬點的成本矩陣
    c = []
    I = cluster.columns.size
    print(I)
    c0 = [0] * (I+2)
    c.append(c0)
    for i in range(I):
        c.append([0])
        for j in range(I):
            c[i+1].append(cluster[i+1][j+1]) 
            # print(cluster[i+1][j+1])
        c[i+1].append(0)
    c.append(c0)

    # 記錄站點容量與已停數量
    data = pd.read_excel('spaceInfo_6.1_17.xlsx')
    data.drop(['Unnamed: 0'], axis = 1)

    # initialization
    station_id = [0]
    Parked = [0]
    Capacity = [0]
    # print("name_list",name_list)

    for i in range(1,len(name_list)-1):
        id = name_list[i]
        # print(id)
        station_id.append(id)
        Capacity.append(int(data["space_total"][id]))
        Parked.append(int(data["space_occupied"][id]))

    station_id.append(10000)
    Parked.append(0)
    Capacity.append(10000)

    # print("station_id",station_id)
    # print("Parked",Parked)
    # print("Capacity",Capacity)

    model = Model('OR Final Project')

    # I = 5

    # A = 16               # 一台調度車容量上限
    # S_H = 0.7            # 停車率的安全範圍上限比率
    # S_L = 0.3            # 停車率的安全範圍下限比率
    # L = 1               # 調度車在單位時間內移動的距離（或時間）上限
    # lambda_value = 500   #有改過 比較合理


    # 為站點 i 的車柱總數
    # Capacity = [0,200,100,150,20,100,10000]

    # 站點 i 的停車數量
    # Parked = [Capacity[i]*S_H if i!= 0 and i!=I+1 else 0 for i in range(I+2)] # 測試剛好最高
    # Parked = [Capacity[i]*S_L if i!= 0 and i!=I+1 else 0 for i in range(I+2)] # 測試剛好最低
    # Parked = [0,55,80,40,10,50,0]  # 自己key



    """ set cost """
    # 調度車輛從站點 i 到站點 j 的成本 (c_ij 要等於 c_ji)
    # 
    real_cost = 1
    virtual_cost = 0
    # oil_cost = 3.136
    # van_cost = 250
    # c = [[real_cost if i!=I+1 and i!=0 and i!=j else virtual_cost for i in range(I+2)] if j!=I+1 and j!=0 else [virtual_cost for k in range(I+2)]for j in range(I+2)]

    # 不用做事（如全部的車站都符合標準）的時候，讓虛擬起點到虛擬終點的成本是負的，迫使solver讓調度車不經過真實站點
    virtual_start_and_end = -(van_cost/oil_cost)
    c[0][0]     = 0
    c[0][I+1]   = virtual_start_and_end
    c[I+1][0]   = 0
    c[I+1][I+1] = 0

    # print("Cost:",*c, sep='\n') # print cost to check

    """ declare variables """
    a=[]    # 調度車從i站拿走的量
    b=[]    # 調度車放入i站的量
    p=[]    # 進i站時調度車上的腳踏車量
    q=[]    # 出i站時調度車上的腳踏車量
    x=[]    # =1代表有經過i站
    y=[]    # =1代表從i走到j
    w_L=[]  # 各點的懲罰比率
    w_H=[]  # 各點的懲罰比率
    u=[]    # 車站的經過「順序」
    v=[]    # =1代表u[i]>u[j]

    a =   model.addVars(I+2,      lb = 0, vtype = GRB.INTEGER, name = 'a')
    b =   model.addVars(I+2,      lb = 0, vtype = GRB.INTEGER, name = 'b')
    p =   model.addVars(I+2,      lb = 0, vtype = GRB.INTEGER, name = 'p')
    q =   model.addVars(I+2,      lb = 0, vtype = GRB.INTEGER, name = 'q')

    x =   model.addVars(I+2,      lb = 0, vtype = GRB.BINARY,  name = 'x')
    y =   model.addVars(I+2, I+2, lb = 0, vtype = GRB.BINARY,  name = 'y') 
    w_L = model.addVars(I+2,      lb = 0, vtype = GRB.CONTINUOUS, name = 'w_L')
    w_H = model.addVars(I+2,      lb = 0, vtype = GRB.CONTINUOUS, name = 'w_H')

    u =   model.addVars(I+2,      lb = 0, vtype = GRB.INTEGER, name = 'u')
    v =   model.addVars(I+2, I+2, lb = 0, vtype = GRB.BINARY , name = 'v') # 決定每個u的關聯

    """ set objective and constraints """
    # 虛擬點為0是起點,n+1為終點
    # 真正的點為1~n
    model.update()
    model.setObjective( oil_cost * quicksum(y[i,j]*c[i][j] for i in range(I+2) for j in range(I+2)) + lambda_value * (100 * quicksum(w_L[i]+w_H[i] for i in range(1,I+1))) ** 2 + van_cost, GRB.MINIMIZE)
        
        
    M=sum(Parked)
    """路線限制!!!"""

    # 兩個點都走到才有機會連在一起->有y必有x
    model.addConstrs((x[i] + x[j]  >= 2*(y[i, j]+y[j, i]) for i in range(I+2) for j in range(I+2)), "C1")

    # 對於起終點以外，進的點=出的點(也就是都只有一個點)
    model.addConstrs(   (quicksum(y[i, k] for i in range(0,I+1))==quicksum(y[k, j] for j in range(1,I+2)) for k in range(1,I+1)   ),"C2")

    # 不能到自己
    model.addConstrs(   (y[i, i]==0 for i in range(I+2))   ,"C3") # 這邊的寫法跟pdf上不一樣有小改ij to ii

    # 不能來回走
    model.addConstrs(   (y[i, j]+y[j, i] <= 1 for i in range(I+2) for j in range(I+2))   ,"C4")

    # 不能跑超過一小時能跑的距離(L)
    model.addConstr(    (quicksum(y[i, j]*c[i][j] for i in range(I+2) for j in range(I+2)) <= L)   ,"C5")

    # 一站只能進或出一次
    model.addConstrs(   (quicksum(y[i, j] for i in range(I+2))<=1 for j in range(I+2))   ,"C6-1")
    model.addConstrs(   (quicksum(y[i, j] for j in range(I+2))<=1 for i in range(I+2))   ,"C6-2")

    # x不為0時y要有東西
    # 這邊的寫法跟pdf上不一樣有小改範圍
    model.addConstrs(   ((quicksum(y[i, j] for j in range(I+2))+quicksum(y[j, i] for j in range(I+2))) >= x[i] for i in range(I+2))   ,"C7")



    """ 處理subtour """
    model.addConstr(  (u[0] == 1), "")
    model.addConstrs( (u[i] <= (I+2) for i in range(1,I+2)) , "")
    model.addConstrs( (u[i] >= 2 for i in range(1,I+2)) , "")
    model.addConstrs( (u[i] - u[j] + 1 <= (M)*(1-y[i, j]) for i in range(I+2) for j in range(I+2) if i != j) , "")

    # v_ij=1 if ui > uj else 0
    e = 0.01
    model.addConstrs( (u[i]-u[j] <= M*v[i, j]-e     for i in range(I+2) for j in range(I+2) if i != j),"每個u不一樣")
    model.addConstrs( (u[i]-u[j] >= e-M*(1-v[i, j]) for i in range(I+2) for j in range(I+2) if i != j),"每個u不一樣")



    """站點bike限制!!!"""
    # 只能拿或放，不能同時
    model.addConstrs(   (a[i]*b[i] == 0 for i in range(I+2)) , "C8")

    # 有經過才拿或放
    model.addConstrs( (M*x[i] >= a[i] for i in range(I+2)), "C9-1")
    model.addConstrs( (M*x[i] >= b[i] for i in range(I+2)), "C9-2")

    # 懲罰
    model.addConstrs((w_L[i] >= S_L - (Parked[i] - a[i] * x[i] + b[i] * x[i])/Capacity[i]    for i in range(1,I+1)), "C10-1-1")
    model.addConstrs((w_L[i] >= 0 for i in range(1,I)), "")

    model.addConstrs((w_H[i] >= (Parked[i] - a[i] * x[i] + b[i] * x[i])/Capacity[i] - S_H    for i in range(1,I+1)), "C10-2-1")
    model.addConstrs((w_H[i] >= 0 for i in range(1,I)), "")

    # 所有從各站拿走總合等於放回 卡車最後沒東西都放回虛擬點
    model.addConstr(  (quicksum(a[i]*x[i] for i in range(I+2)) == quicksum(b[i]*x[i] for i in range(I+2))), "C11")

    # 合理的放回車站
    model.addConstrs( (b[i] +  Parked[i] <= Capacity[i] for i in range(1,I+2)), "C12-1")
    # 合理地拿出車站
    model.addConstrs( (a[i] <= Parked[i]                for i in range(1,I+1)), "C12-2")


    """ trucks constraints """
    # 進+拿-放=出
    model.addConstrs( (p[i] + a[i] - b[i] == q[i] for i in range(I+2)), "C13")
    # 從此站出的腳踏車量等於到下站時的腳踏車量
    model.addConstrs( (q[i]*y[i,j] == p[j]*y[i,j] for i in range(I+2) for j in range(I+2) if i != j), "C14")
    # 車上總車數不能超過容量
    model.addConstrs( (p[i] <= A for i in range(I+2)), "C15-1")
    model.addConstrs( (q[i] <= A for i in range(I+2)), "C15-2")
    # 不能放下超過貨車上進場時的腳踏車量
    model.addConstrs( (b[i] <= p[i] for i in range(I+2)), "")


    """ Virtual points constraints """
    # 起點的a,b為0
    model.addConstr(  (a[0]   == 0), "")
    model.addConstr(  (b[0]   == 0), "")
    # 終點的a為0,b都可
    model.addConstr(  (a[I+1] == 0), "")
    model.addConstr(  (b[I+1] >= 0), "終點可以放車（真實的站點最後一站沒補完）")
    # 起終點必經
    model.addConstr(  (x[0]   == 1), "")
    model.addConstr(  (x[I+1] == 1), "")
    # 起點不進，終點不出
    model.addConstrs( (y[i,   0] == 0 for i in range(1, I+2))  , "不能進入虛擬起點")# 範圍小改
    model.addConstrs( (y[I+1, i] == 0 for i in range(I+1))     , "虛擬終點不能出去")   # 範圍小改
    # 在起點時沒車，終點時即便原本有車也要放到沒
    model.addConstr(  (p[0]   == 0), "")
    model.addConstr(  (q[0]   == 0), "")
    model.addConstr(  (p[I+1] >= 0), "進入終點時可以有車（真實的站點最後一站沒補完）")
    model.addConstr(  (q[I+1] == 0), "")
    # 起終點不懲罰
    model.addConstr(  (w_L[0]   == 0), "")
    model.addConstr(  (w_H[0]   == 0), "")
    model.addConstr(  (w_L[I+1] == 0), "")
    model.addConstr(  (w_H[I+1] == 0), "")

    model.Params.timeLimit = 20

    model.optimize()

    r_a = []
    r_b = []
    r_p = []
    r_q = []
    r_x = []
    r_w_L = []
    r_w_H = []
    r_u = []

    r_y = []
    r_v = []
    for var in model.getVars():
        if var.varName[0] == "a": r_a.append(var.x)
        if var.varName[0] == "b": r_b.append(var.x)
        if var.varName[0] == "p": r_p.append(var.x)
        if var.varName[0] == "q": r_q.append(var.x)
        if var.varName[0] == "x": r_x.append(var.x)
        if var.varName[0] == "u": r_u.append(var.x)
        if var.varName[2] == "L": r_w_L.append(var.x)
        if var.varName[2] == "H": r_w_H.append(var.x)
        if var.varName[0] == "y": r_y.append(var.x)
        step = I+2
        new_r_y = [r_y[i:i+step] for i in range(0, len(r_y), step)]
        # for i in range(I+2):

    # print(r_a, r_b, r_p, r_q, r_x, r_w_H, r_w_L, r_u)
        print(var.varName, '=', var.x)
    print("objective value =", model.objVal)

    only_station_ID = name_list[1:-1]
    return model.objVal, r_a, r_b, r_p, r_q, r_x, r_w_H, r_w_L, r_u, new_r_y, only_station_ID, Parked, Capacity




def return_detail(labels, A, S_H, S_L, L, lambda_value,van_cost,oil_cost):
    detail = {}
    
    for i in range(labels):
        print("=========" + str(i) + "===============")
        objVal, r_a, r_b, r_p, r_q, r_x, r_w_H, r_w_L, r_u, r_y, name_list, Parked, Capacity = each_run(labels,i,A,S_H,S_L,L,lambda_value,van_cost,oil_cost)
        detail[i] = {}
        detail[i]["ID"] = i
        detail[i]["name_list"] = name_list
        detail[i]["r_a"] = r_a
        detail[i]["r_b"] = r_b
        detail[i]["r_p"] = r_p
        detail[i]["r_q"] = r_q
        detail[i]["r_x"] = r_x
        detail[i]["r_w_H"] = r_w_H
        detail[i]["r_w_L"] = r_w_L
        detail[i]["r_u"] = r_u
        detail[i]["r_y"] = r_y
        detail[i]["objVal"] = objVal
        detail[i]["Parked"] = Parked
        detail[i]["Capacity"] = Capacity
    return detail



if __name__ == '__main__':
# test
    # labels = K
    K = 1
    A = 16
    S_H = 0.8
    S_L = 0.2
    L = 10
    lambda_value = 0.5
    van_cost = 250
    oil_cost = 3.136

    results = return_detail(K,A,S_H,S_L,L,lambda_value,van_cost,oil_cost)

    for key, value in results.items():
        print(key, ': ', value)
