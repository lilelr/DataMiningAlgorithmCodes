//
//  main.cpp
//  DataMingingAlgorithmCodes
//
//  Created by yuxiaoMac on 4/4/17.
//  Copyright © 2017 yuxiaoMac. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Apriori
{
public:
    Apriori(size_t is=0,unsigned int mv=0){
        item_size = is;
        min_value = mv;
    
    }
    
    void getItem();
    map<vector<string>, unsigned int> find_freitem(); // 求事务的频繁项
    // 连接两个 K-1 级频繁集，得到第K级频繁集
    map<vector<string>, unsigned int> apri_gen(unsigned int K, map<vector<string>, unsigned int> K_item);
    
    // 展示频繁集
    void showApriorItem(unsigned int K, map<vector<string>, unsigned int> showmap);
    
private:
    map<int, vector<string>> item;   //存储所有最开始的事务及其项
    map<vector<string>, unsigned int> K_item; // 存储频繁项集
    size_t item_size; // 事务数目
    unsigned int min_value; // 最小阈值
};

void Apriori::getItem()
{
    int ci = item_size;
    for (int i=0; i<ci; i++) {
        string str;
        vector<string> temp;
        cout<<"请输入第  "<<i+1<<"个事务的项集（123 end);";
        while (cin>> str && str !="123") {
            temp.push_back(str);
        }
        sort(temp.begin(),temp.end());
        pair<map<int, vector<string>>::iterator, bool> ret = item.insert(make_pair(i+1, temp));
        if (!ret.second) {
            --i;
            cout<<"输入的元素已经存在，请重新输入"<<endl;
        }
    }
    cout<<"运行结果如下-----------"<<endl;
}

map<vector<string>, unsigned int> Apriori::find_freitem()
{
    unsigned int i =1;
    bool isEmpty = false;
    map<int, vector<string>>::iterator mit;
    for (mit = item.begin(); mit!=item.end(); mit++) {
        vector<string> vec = mit->second;
        if (vec.size()!=0) {
            break;
        }
    }
    
    if (mit==item.end()) { //事务集为空
        isEmpty = true;
        cout<<"事务集为空！程序无法进行。。。"<<endl;
        map<vector<string>, unsigned int> empty;
        return empty;
    }
    
    while (1) {
        map<vector<string>, unsigned int> K_itemTemp = K_item;
        K_item = apri_gen(i++, K_item);
        
        if (K_itemTemp == K_item) {
            i=UINT_MAX;
            break;
        }
        
        // 判断是否需要进行下一次寻找
        map<vector<string>, unsigned int> pre_K_item = K_item;
        size_t Kitemsize = K_item.size();
        // 存储应该删除的第K级频繁项集，不能和其他K级频繁项集构成第K+1级项集的集合
        if (Kitemsize!=1 && i==1) {
            vector<map<vector<string>, unsigned int>::iterator> eraseVecMit;
            map<vector<string>, unsigned int>::iterator pre_K_item_it1 = pre_K_item.begin(), pre_K_item_it2;
            while (pre_K_item_it1!=pre_K_item.end()) {
                map<vector<string>, unsigned int>::iterator mit = pre_K_item_it1;
                bool isExist = true;
                vector<string> vec1;
                vec1 = pre_K_item_it1->first;
                vector<string> vec11(vec1.begin(),vec1.end()-1);
                while (mit!=pre_K_item.end()) {
                    vector<string> vec2;
                    vec2 = mit->first;
                    vector<string> vec22(vec2.begin(),vec2.end()-1);
                    if (vec11 == vec22) {
                        break;
                    }
                    mit++;
                }
                
                if (mit == pre_K_item.end()) {
                    isExist = false;
                }
                
                if (!isExist && pre_K_item_it1 != pre_K_item.end()) {
                    eraseVecMit.push_back(pre_K_item_it1); // 该第K级频繁集项应该删除
                }
                
                ++pre_K_item_it1;
            
            }
            
            size_t eraseSetSize = eraseVecMit.size();
            if (eraseSetSize == Kitemsize) {
                break;
            }else{
                vector<map<vector<string>, unsigned int>::iterator>::iterator currentErs = eraseVecMit.begin();
                while (currentErs!=eraseVecMit.end()) {// 删除所有应该删除的第K级频繁集
                    map<vector<string>, unsigned int>::iterator eraseMit = *currentErs;
                    K_item.erase(eraseMit);
                    ++currentErs;
                }
            }
        }else{
            if (Kitemsize==1) {
                break;
            }
        }
    }
    
    cout<<endl;
    return K_item;
    
    
}

map<vector<string>, unsigned int> Apriori::apri_gen(unsigned int K, map<vector<string>, unsigned int> K_item)
{
    
    if (1==K) { // 求候选集 C1
        size_t c1 = item_size;
        map<int, vector<string>>::iterator mapit = item.begin();
        vector<string> vec;
        map<string, unsigned int> c1_itemtemp;
        while (mapit!=item.end()) { // 将原事务中所有的单项统计出来
            vector<string> temp = mapit->second;
            vector<string>::iterator vecit = temp.begin();
            while (vecit!=temp.end()) {
                pair<map<string, unsigned int>::iterator, bool> ret = c1_itemtemp.insert(make_pair(*vecit++,1));
                if (!ret.second) { // 该item已经存在，+1
                    ++ ret.first->second;
                }
            }
            mapit++;
        }
        
        // 构造第一级频繁项集
        map<string, unsigned int>::iterator item_it = c1_itemtemp.begin();
        map<vector<string>, unsigned int> c1_item;
        while (item_it!=c1_itemtemp.end()) {
            vector<string> temp;
            if (item_it->second >= min_value) {
                temp.push_back(item_it->first);
                c1_item.insert(make_pair(temp, item_it->second));
            }
            ++item_it;
        }
        return c1_item;
    }else{
        cout<<endl;
        map<vector<string>, unsigned int>::iterator ck_item_it1 = K_item.begin();
        map<vector<string>, unsigned int>::iterator ck_item_it2;
        map<vector<string>, unsigned int> ck_item;
        while (ck_item_it1 != K_item.end()) {
            ck_item_it2 = ck_item_it1;
            ++ck_item_it2;
            map<vector<string>, unsigned int>::iterator mit = ck_item_it2;
            while (mit!=K_item.end()) {
                vector<string> vec, vec1, vec2;
                vec1 = ck_item_it1->first;
                vec2  = mit->first;
                vector<string>::iterator vit1,vit2;
                
                vit1 = vec1.begin();
                vit2 = vec2.begin();
                while (vit1< vec1.end() && vit2<vec2.end()) {
                    string str1 = *vit1;
                    string str2 = *vit2;
                    ++vit1;
                    ++vit2;
                    if (K==2 || str1==str2) {
                        if (vit1!=vec1.end() && vit2!=vec2.end()) {
                            vec.push_back(str1);
                        }
                    }else{
                        break;
                    }
                }
                
                if (vit1==vec1.end() && vit2 == vec2.end()) {
                    --vit1;
                    --vit2;
                    string str1 = *vit1;
                    string str2 = *vit2;
                    if (str1>str2) {
                        vec.push_back(str2);
                        vec.push_back(str1);
                    }else{
                        vec.push_back(str1);
                        vec.push_back(str2);
                    }
                    
                    map<int, vector<string>>::iterator base_item = item.begin();
                    unsigned int Account = 0;
                    while (base_item!=item.end()) {
                        unsigned int count=0, mincount = UINT_MAX;
                        vector<string> vv = base_item->second;
                        vector<string>::iterator vecit,bvit;
                        for (vecit=vec.begin(); vecit<vec.end(); vecit++) {
                            string t = * vecit;
                            count = 0;
                            for (bvit=vv.begin(); bvit<vv.end(); bvit++) {
                                if (t == *bvit) {
                                    count++;
                                }
                            }
                            mincount=(count<mincount? count: mincount);
                        }
                        if (mincount>=1 && mincount!=UINT_MAX) {
                            Account+=mincount;
                        }
                        ++base_item;
                    }
                    
                    if (Account >= min_value && Account!=0) {
                        sort(vec.begin(), vec.end());
                        // 该第K+1 级候选集为频繁项，插入频繁项集
                        pair<map<vector<string>, unsigned int>::iterator, bool> ret = ck_item.insert(make_pair(vec, Account));
                        if (!ret.second) {
                            ret.first->second += Account;
                        }
                    }
                }
                ++mit;
            }
            ++ck_item_it1;
        }
        if (ck_item.empty()) { // 该第K+1 级频繁项集为空，说明调用结束，把上一级频繁项集返回
            return K_item;
        }else{
            return ck_item;
        }
    }
}

unsigned int parseNumber(const char* str){
    if (str==NULL) {
        return 0;
    }else{
        unsigned int num = 0;
        size_t len = strlen(str);
        for (size_t i=0; i<len; i++) {
            num *=10;
            if (str[i]>'0' && str[i]<'9') {
                num += str[i]-'0';
            }else{
                return 0;
            }
        }
        return num;
    }
}

//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}
