//
//  main.cpp
//  Clp
//
//  Created by Sam Wo on 2023/5/24.
//
#define TEST  //测试模式，将JI初始化为30个，并展示历史记录
#include <iostream>
#include<vector>
#include<string>
#include<memory>
#include<numeric>
#include<unistd.h>

#include"Heros.h"
int main()
{
   
 
        Hlist.push_back(make_shared<Assassin>());
        Hlist.push_back(make_shared<Groot>());
        Hlist.push_back(make_shared<Vampire>());
    Hlist.push_back(make_shared<Tank>());
        Hlist[0]->order=0;
        Hlist[0]->team=1;
        Hlist[1]->order=1;
        Hlist[1]->team=1;
    Hlist[2]->order=2;
    Hlist[2]->team=2;
    Hlist[3]->order=3;
    Hlist[3]->team=2;
   
        Game* A = new Game();
        A->roundRun();

  
}

