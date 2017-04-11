//
//  main.cpp
//  DataMingingAlgorithmCodes
//
//  Created by yuxiaoMac on 4/11/17.
//  Copyright © 2017 yuxiaoMac. All rights reserved.
//

#include "BP.h"
#include <stdio.h>

int main(){
    BP B; //生成一个BP 类对象B
    B.Train(); // 开始训练
    B.BP_Print(); // 把结果打印出来
    B.After_Train_Out(); // 把结果保存到文件中
}