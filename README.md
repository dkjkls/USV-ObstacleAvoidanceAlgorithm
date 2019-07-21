# USV-ObstacleAvoidanceAlgorithm

Source code of Mater Dissertation Research Paper *Local Risk Obstacle Avoidance Algorithm of USV*.

## Highlights

- Requires C++ 
- Requires MFC
- algorithm -> Source code of Mater Dissertation Research Paper *Local Risk Obstacle Avoidance Algorithm of USV*
- applications -> Executable application generated during the paper process
- data -> The data and results used in the process of the paper,  the experimental scenarios in the paper  can be reproduced by these data

## Application interface



## Links to paper

[中英文摘要](https://blog.csdn.net/dkjkls/article/details/53439185)

[第1章 绪论](https://blog.csdn.net/dkjkls/article/details/53442472)

[第2章 无人艇局部危险避障算法研究](https://blog.csdn.net/dkjkls/article/details/53468684)

[第3章 基于PSO的已知静态路径规划方法](https://blog.csdn.net/dkjkls/article/details/53495566)

[第4章 基于PSO并融合海事规则的已知动态路径规划方法](https://blog.csdn.net/dkjkls/article/details/53544551)

[第5章 动态未知环境下的避障策略](https://blog.csdn.net/dkjkls/article/details/53586494)

[第6章 USV局部危险避障系统及综合仿真实验分析](https://blog.csdn.net/dkjkls/article/details/53614823)

[结论](https://blog.csdn.net/dkjkls/article/details/53645457)

[参考文献](https://blog.csdn.net/dkjkls/article/details/53452924)

[水面无人艇局部危险避障算法研究-答辩PPT（内附原版PPT）](https://blog.csdn.net/dkjkls/article/details/61418014)

# Paper: Local Risk Obstacle Avoidance Algorithm of USV

## Abstract

Unmanned Surface Vehicle (short as USV) is an autonomous marine vehicle, and its autonomy requires interaction with the environment, while the interaction with the outside world must have the ability to adopt global path planning and avoid obstacles of local hazardous situations. During the voyage, the USV is in an environment of unpredictable and highly dynamic marine situation, the obstacles are likely to appear on the already planned global path. Therefor the USV must react to these unpredictable events in a certain way to make it still can reach the goals and accomplish the tasks, while a reliable algorithm can just improve the intelligence of USV. So based on the global path planning, the real-time local hazard avoidance act as a supplementary to the successful task implementation.

This paper divides the USV local risk obstacle avoidance into three layers: known static path planning method based on Particle Swarm Optimization (PSO); known dynamic path planning method integrated with the marine rules based on PSO; obstacle avoidance method under unknown environment based on the rolling windows. The first two belong to known local obstacles avoidance based on the knowledge of the environment, and the last is unknown local obstacles avoidance based on the knowledge of the sensor.

In the first layer of known static path planning algorithm, this thesis introduce the heuristic knowledge (Initialization of normal distribution) to initialize the path of the particle swarm path planning algorithm, to ensure the decrease of buffeting amplitude on this route, and guarantee the particle swarm distribute randomly in a reasonable search space. At the same time, this paper make some smooth optimization on the final path, which shorten the calculation time, reduced the turning points, got a better path, and improved the feasibility and effectiveness of the algorithm. In order to get more effective path interval, this paper put forward a more realistic simulation (outside circle and Oriented Bounding Box) of the obstacles. In the second layer of algorithm, this paper establish a model of the known dynamic obstacles, conclude the condition to complete the avoidance of this kind, and discuss the constraints that Marine Rules have on the USV avoidance behavior. In the last layer of algorithm, this paper solve the problem of real-time obstacle avoidance under dynamic unknown environment by using model predictive control and rolling optimization principle, combined with rolling windows method.

This paper set the USV local risk obstacle avoidance as the research object, create a virtual environment that include three kinds of obstacle on the simulation experiment platform developed by myself, and analyze the simulation results of the three layers algorithm, have verified the feasibility and effectiveness of the algorithm that this paper proposed.

## Key words 

Unmanned Surface Vehicle (USV), Path Planning, Dynamic Collision Avoidance, Particle Swarm Optimization (PSO), Marine Rules

# 论文：水面无人艇局部危险避障算法研究

## 中文摘要

　　水面无人艇（USV，Unmanned Surface Vehicle）是一种自主式海洋运载器，其具有自主能力就要求无人艇能与外界环境进行交互，而与外界环境交互就需要无人艇能够进行全局路径规划，以及进行局部危险状况下的避障。无人艇在航行过程中，处在不可预见和高度动态的海洋环境中，障碍物很可能出现在已经规划好的全局路径上。无人艇必须对这些不可预测的事件以某种方式做出反应，使之仍然能顺利到达目标、完成任务，而可靠的避障算法正是提高无人艇智能化的主要障碍。因此，在全局路径规划的基础上，还需要进行实时的局部危险避障，才能满足无人艇顺利执行任务的需求。

　　本文将水面无人艇局部危险避障分为三层进行考虑：基于PSO的已知静态路径规划方法，基于PSO并融合海事规则的已知动态路径规划方法和基于滚动窗口的未知环境下的避障方法。前两层算法是基于环境先验知识的已知局部危险避障，第三层算法是基于传感器知识的未知局部危险避障。

　　在第一层基于PSO的已知静态路径规划算法中，引入了启发式知识（即正态分布初始化）对粒子群路径规划算法的路径进行初始化，使得初始化生成的路径上下颠簸幅度减少，同时可保证粒子群随机分布在较为合理的搜索空间中；并对最终产生的路径进行路径平滑优化处理，有效缩短了解算最优解的耗时，减少了航行转向点数，得到更优的路径解，提高了算法的可行性和有效性。为了得到更多的有效路径区间，提出了将障碍物进行更贴合实际形状的模拟（外包圆和有向包围盒）。在第二层基于PSO并融合海事规则的已知动态路径规划算法中，对动态已知障碍物建立避障模型，得出完成对动态已知障碍物规避的条件，并论述了国际海上避碰规则公约对无人艇避障行为的约束。在第三层基于滚动窗口的未知环境下的避障算法中，利用模型预测控制及滚动优化原理，结合滚动窗口方法，解决动态未知环境下的实时避障问题。

　　本文以无人艇局部危险避障为研究对象，通过自行开发的水面无人艇局部危险避障仿真实验平台建立包含三种障碍物的真实环境，并对三层避障算法进行综合仿真实验分析，验证了本文所提出的三层避障算法的可行性和有效性。

## 关键词

水面无人艇，路径规划，动态避障，粒子群优化算法，海事规则

