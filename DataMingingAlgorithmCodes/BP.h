//
//  BP.h
//  DataMingingAlgorithmCodes
//
//  Created by yuxiaoMac on 4/11/17.
//  Copyright © 2017 yuxiaoMac. All rights reserved.
//

#ifndef BP_h
#define BP_h

#include<iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;

#define RANNDOM rand()/32767.0 // 0~1 随机数生成函数

const int Layer_Max=5;//
const double PI = 3.1415927; // 圆周率
const int Layer_number[Layer_Max]= {2,4,4,2,1};
const int Neural_Max=4;//神经网络各层最大神经元个数
const int InMax=21;//样本输入的个数


// 定义类 BP
class BP
{
public:
    BP();
    void BP_Print();// 打印权系数
    double F(double x); // 神经元的激发函数
    double Y(double x1, double x2); // 要逼近的函数
    double NetWorkOut(int x1, int x2); // 网络输出，他的输入为第input个样本
    void AllLayer_D(int x1, int x2); // 求所有神经元的输出误差微分
    
    void Change_W(); // 改变权系数
    
    void Train(); // 训练函数
    void After_Train_Out(); // 经过训练后，21个样本的神经网络输出
    double Cost(double out, double Exp); // 代价函数
    
private:
    double W[Layer_Max][Neural_Max][Neural_Max];
    //保存权系数
    //规定W[i][j][k]表示网络第i层的第j个神经元连接到
    //第i－1层第k个神经元的权系数
    
    double Input_Net[2][InMax];
    //21个样本输入,约定Input_Net[0][i]
    //表示第i个样本的输入x1
    //而 Input_Net[1][i]表示第i个样本的输入x2
    
    double Out_Exp[InMax][InMax]; // 期望输出
    
    double Layer_Node[Layer_Max][Neural_Max];
    //保存各神经元的输出
    //规定Layer_Node[i][j]表示第i层的第j个神经元的输出
    double D[Layer_Max][Neural_Max];
    //保存各神经元的误差微分
    //规定D[i][j]表示第i层第j个神经元的误差微分
    
    double Study_Speed;//学习速度
    
    double e; //误差
    
};


#endif /* BP_h */
