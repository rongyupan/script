#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 1. 输入点的值，格式：先 x 后 y
#  points = ([0.4, 0.5, 0.6, 0.7, 0.8],
#  [-0.916291, -0.693147, -0.510826, -0.356675, -0.223144])
points = ([0, 0.4, 0.8, 1.2, 1.6, 2.0],
          [1.9, -0.14456959999999963, -0.6438207999999994, 4.520876799999998, 31.694713600000007, 135.5])

max_order = len(points[0])
print(f"下面将要计算{max_order - 1}阶差商\n")

# 2. 计算差商值，保存到 res 中
res = list()
res.append(points[0])
res.append(points[1])

for order in range(1, max_order):
    p_cal = order
    tmp = list()
    for _ in range(order):
        tmp.append(99999)
    while p_cal < max_order:
        molecular = res[order][p_cal] - res[order][p_cal - 1]
        denominator = res[0][p_cal] - res[0][p_cal - order]
        tmp.append(molecular/denominator)
        p_cal += 1
    res.append(tmp)

for i in range(len(res)):
    print(res[i], "\n")
