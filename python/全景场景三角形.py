floor = 2
points = 32

# 每一层的原点
origin_list = list()

# 每一层的点序号，points_list有floor个元素，每个元素为list
points_list = list()

# 构成三角形的序号，每个元素都为list
res_list = list()

# 生成原点和每层点位的序号；
for i in range(floor):
    origin_list.append(floor*points + i)
    tmp_list = list()
    for j in range(points):
        tmp_list.append(j*floor + i)
    points_list.append(tmp_list)

# 构成底面三角形
for i in range(points):
    # 最后一个点要和第一个点构成三角形
    if i < points-1:
        res_list.append([origin_list[0], points_list[0][i], points_list[0][i+1]])
    else:
        res_list.append([origin_list[0], points_list[0][i], points_list[0][0]])

# 构成顶面三角形
for i in range(points):
    # 最后一个点要和第一个点构成三角形
    if i < points-1:
        res_list.append([origin_list[floor-1], points_list[floor-1][i+1], \
                points_list[floor-1][i]])
    else:
        res_list.append([origin_list[floor-1], points_list[floor-1][0], \
                points_list[floor-1][i]])

# 构成斜面三角形：floor层对应(floor-1)层斜面
for i in range(floor-1):
    for j in range(points):
        if j < points-1:
            res_list.append([points_list[i][j], points_list[i+1][j], points_list[i+1][j+1]])
            res_list.append([points_list[i][j], points_list[i+1][j+1], points_list[i][j+1]])
        else:
            res_list.append([points_list[i][j], points_list[i+1][j], points_list[i+1][0]])
            res_list.append([points_list[i][j], points_list[i+1][0], points_list[i][0]])

# 输出结果
f = open("./triangle.txt", 'w')
for i in res_list:
    tmp = str(i[0]) + ", " + str(i[1]) + ", " + str(i[2]) + ",\n"
    f.write(tmp)
