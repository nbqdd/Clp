//
//  Basic.h
//  Clp
//
//  Created by Sam Wo on 2023/5/25.
//

#ifndef Basic_h
#define Basic_h

#define N 4
using namespace std;

int att[N][N];//att[b][a]是b对a的攻击，同时会产生def[a][b]
int def[N][N];//def[b][a]是a面向b的防御。def[b][a]<att[b][a]时，a受到来自b的damage
//int vision[N][N];//vision[a][b]=0代表a对b的攻击是自由的，=1代表a不能攻击b

class Hero
{
    friend class Game;
public:
    Hero(){memset(visionLimit,0,sizeof(visionLimit));}
    int HP;//血
#ifdef TEST
    int EN=30;//Ji,测试版初始30个
#else
    int EN=0;
#endif
    int order;
    int damage=1;//默认伤害为1
    int eff=1;//出Ji效率，一抵几，默认1
    int maxHP;//最大生命值，一般为基础生命值+2
    int attackLimit=0;//辅助的攻击受限制
    int visionLimit[N];//视野限制，为0表示能选中，大于0表示不能选中
    int team;//阵营
    vector <double> History;//历史记录
    void attack(int Actioner, int Target, int level)
    {if (level<=EN){att[Actioner][Target]=level;cout<<"Player"<<Actioner<<" attacked "<<"Player"<<Target<<" with level"<<level<<endl;EN-=level;def[Target][Actioner]=level;}else cout<<"invalid attack";}
    void AskForAttack();
    void energize(){EN+=eff;RecordHistory(1);}//出Ji
    void defend();//防
    void defe(int i);//防
    int type;
    string name;
    int roundCount=0;//计回合器
    virtual void count(int i){};
    
public:
    virtual void ability(){RecordHistory(-1); cout<<"No ability";}//有待重载
    virtual void passive(){}//被动
    virtual void PrePassive(){}
    virtual void random(){};
    void RecordHistory(double k){History.push_back(k);}//记录
    
    
    
    
};

class Attack  //暂未使用
{
public:
    Hero Target;
    Hero Actioner;
    int level;
    
};
vector<shared_ptr<Hero>>Hlist;//“Hero List”，用来存放Hero的子类，即各个具体英雄。
void Hero::AskForAttack()
{
    
    int i=99,k;
    int j=this->order;
    
    
    RecordHistory(0);
    cout<<" 请选择攻击目标，输入-1来结束。"<<endl;
    for(;i>-1;)
    {   cin>>i;
        if(i>=0&&this->visionLimit[i]==0)//是否能选中攻击目标
        {
            cout<<" Attack Level?"<<endl;
            cin>>k;
            
            if(this->attackLimit==0)attack(j,i,k);
            if(this->attackLimit==1){if (k==1||k==5||k==10)attack(j,i,k);else cout<<"Your Attack is Limited!Only use 1,5,10\n";}//辅助攻击只能用1（小刀）、5（鬼刀）、10（加特林）
            
        }
        else {cout<<"你的回合结束\n";i=-1;}
    }
    
}
void Hero::defe(int i)
{
    
    int k=0;
    int j=this->order;
    switch (i) {
        case 0:
            for(k=0;k<N;k++)
            {def[k][j]=4;}
            break;
        case 1:
            for(k=0;k<N;k++)
            {def[k][j]=9;}this->EN-=1;break;
        case 2:
            for(k=0;k<N;k++)
            {def[k][j]=10;}this->EN-=2;break;
        default:
            break;
    }}

void Hero::defend()
{
    int i=0;
    int k=0;
    int j=this->order;
    cout<<" level of df:";
    cin>>i;
    switch (i) {
        case 0:
            for(k=0;k<N;k++)
            {def[k][j]=4;}RecordHistory(30);
            break;      //普通防0费，可以抵挡小于等于4级（火刀）的攻击
        case 1:
            for(k=0;k<N;k++)
            {def[k][j]=9;}
            this->EN-=1;
            RecordHistory(31);
            break;   //超防1费，可以抵挡小于等于9级（九阴真经）的攻击
        case 2:
            for(k=0;k<N;k++)
            {def[k][j]=10;}
            this->EN-=2;RecordHistory(32);  //无敌防2费，可以抵挡小于等于10级（加特林）的攻击
            break;
        default:
            break;
    }
}

void showHistory()
{ cout<<"历史记录：\n";
    for(int k=0;k<N;k++)
    {
        string nome=Hlist[k]->name;
        cout<<nome<<"的历史记录为：";
        
        for (size_t i = 0; i < Hlist[k]->History.size(); ++i)
        {
           
            cout << Hlist[k]->History[i] << "; ";
        }cout<<endl;
    }cout<<endl;
}
class Game
{
    friend class Hero;
public:
    Game(){};
    int count=1;
    friend class Hero;
public:
    
    void reSet()//回合开始前重置def[][]和att[][]
    {
        memset(def, -1, sizeof(def));
        memset(att, -1, sizeof(att));
    }
    void reSetVision()
    {
        for(int k=0;k<N;k++)
        {
            for (int m=0;m<N;m++){Hlist[k]->visionLimit[m]=0;}
        }

    }
    void ifHurt(int i)//第i位英雄收到的伤害判定
    {
        int k=0;
        vector<int>dmg;
        dmg.push_back(0);
        for(k=0;k<=N;k++)
        {
            if (def[k][i]<att[k][i])
                dmg.push_back(Hlist[k]->damage);
        }
        int maxValue=*max_element(dmg.begin(), dmg.end());
        if (maxValue==0){//cout<<Hlist[i]->name<<" Not Hurt\n";
            
        }
        else cout<<Hlist[i]->name<<" HP decreased\n";
        Hlist[i]->HP-=maxValue;
        //return maxValue;
    }
    int ifDead(int i)
    {
        if (Hlist[i]->HP==0){return 0;}
        else return 1;
    }
    void showDeath()//用于展示死亡信息,并设置死者不可被选中
    {
        for(int i=0;i<N;i++)
        {
            if(Hlist[i]->HP<=0)
            {
                cout<<Hlist[i]->name<<" is dead\n";
                Hlist[i]->EN=-100;
                for(int j=0;j<N;j++)
                { Hlist[j]->visionLimit[i]+=1;}
            }
        }
    }
    bool ifEndSoloDown() //单人阵亡全队失败模式
    {   int j=0;
        int k=1;
        for(j=0;j<N;j++)
        {k*=ifDead(j);}
        if(k==1)return false; //k==1表示所有人都活着，游戏继续
        else return true;
    }
    bool ifEndTeamDown()
    {
        for(int j=0;j<N;j++)
        {
            int p=0;
            for(int m=0;m<N;m++)
            {
                if(Hlist[m]->team==Hlist[j]->team)
                    p+=ifDead(m);
            } if(p==0)return true;
        }
        return false;
    }//全队阵亡模式
    void showState(int i)
    {
        cout<<Hlist[i]->name<<Hlist[i]->order<<" 血量为"<<Hlist[i]->HP<<",Ji数为"<<Hlist[i]->EN<<endl;
    }
    void roundRun()
    {
     
       
        do{cout<<"Round "<<count<<" Start!\n";
            count++;for(int i=0;i<N;i++){Hlist[i]->roundCount++;}//回合计数
            reSet(); //初始化
       
            int i,j;
            for (j=0;j<N;j++) //行动
            {
              
                if (Hlist[j]->type!=0)
                {
                cout<<Hlist[j]->name<<"("<<Hlist[j]->order<<")"<<":集1 防2 刀3 技4 ";
                cin>>i;
                switch (i)
                {
                    case 1:
                        Hlist[j]->energize();break;
                    case 2:
                        Hlist[j]->defend();break;
                    case 3:
                        Hlist[j]->AskForAttack();break;
                    case 4:
                        Hlist[j]->ability();break;
                    default:
                        Hlist[j]->RecordHistory(-1);
                        break;
                }
                }
                else {Hlist[j]->random();sleep(1);}
            }
            
            //第一次被动结算
            for (j=0;j<N;j++)
            {
                Hlist[j]->PrePassive();
                
            }
          
                reSetVision();
                //伤害结算。然后第二次被动结算，血量上限结算
                for (j=0;j<N;j++)
                {
                    ifHurt(j);
                    Hlist[j]->passive();
                    
                }
        
                //显示当前数据
            cout<<"\n第"<<count-1<<"回合结算后数据：\n";
                for (j=0;j<N;j++)
                {showState(j);}
            cout<<endl;
            cout<<endl;
    #ifdef TEST
            showHistory();
    #endif
            showDeath();
             }while(ifEndTeamDown()==false);
     
        cout<<"Game Over!"<<endl;
    }
    
    
};
#endif /* Basic_h */
