//
//  Heros.h
//  Clp
//
//  Created by Sam Wo on 2023/5/25.
//
#include"Basic.h"
#include<stdlib.h>
#include<time.h>
using namespace std;

#ifndef Heros_h
#define Heros_h


class Priest: public Hero //牧师
{
public:
    Priest(){HP=3;type=2;name="牧师";attackLimit=1;maxHP=5;}
    virtual void ability()
    {   RecordHistory(4.01);
        int j;
        cout<<" 奶1 盾2 ";
        cin>>j;
        switch (j) {
            case 1:
                if(EN>=4){ int i,k,p;
                    p=this->order;
                    cout<<" target? ";
                    cin>>i;
                    Hlist[i]->HP+=1;
                    for(k=0;k<N;k++){def[k][i]+=4;def[k][p]+=4;}//牧师的奶自带小盾效果
                    EN-=4;}
                else cout<<"invalid ability\n";
                 break;
            case 2:
                if(EN>=1){ int i,k,p;
                    p=this->order;
                    cout<<" target? ";
                    cin>>i;
                    for(k=0;k<N;k++){def[k][i]+=5;def[k][p]+=5;}//自身和目标都加一级全局防御
                  
                    EN-=1;}
                else cout<<"invalid ability\n";
                break;
               
                   default:
                cout<<"Wrong Input!\n";
                     break;
        }}
        
        
       
};
class Assassin:public Hero //刺客
{
public:
    Assassin(){HP=1;type=3;eff=4;name="刺客";maxHP=3;}
};
class Vampire:public Hero //吸血鬼
{
public:
    Vampire(){HP=2;type=4;eff=1;name="吸血鬼";maxHP=4;}
    virtual void passive()
    {
        int k;
        int p=this->order;
        for(k=0;k<N;k++)
        {if(att[p][k]>def[p][k])this->HP+=damage;}
    }
};
class Tank:public Hero //坦克
{
public:
    Tank(){HP=4;type=5;eff=1;name="坦克";maxHP=6;}
    int round[N]={0};//用于仇恨机制的倒计时
    void causeHatred(int k)//制造仇恨
    {
        for (int m=0;m<N;m++){if (m!=this->order) Hlist[k]->visionLimit[m]+=1;}
      
        cout<<"下回合有仇恨\n";
    }
   virtual void passive()
    {
      
    
        int p=this->order;
       
        vector<int>dmg;
        dmg.push_back(0);
        for(int k=0;k<=N;k++)
        {
            if (def[k][p]<att[k][p])
                dmg.push_back(Hlist[k]->damage);
        }
        int maxValue=*max_element(dmg.begin(), dmg.end());//受伤加Ji
        this->EN+=maxValue;
        for(int k=0;k<N;k++)
        {
            if(att[p][k]>def[p][k])//坦克对k造成了伤害
            {
                round[k]=2;//刷新对k的仇恨计时
            }
        }
       for(int k=0;k<N;k++)
       {
           if (round[k]>0)//确认下回合是否有仇恨，若有
           {
               causeHatred(k);
               round[k]-=1;//仇恨倒计时-1
           }
       }
    }
};
class theVainglorious: public Hero //虚荣
{public:
    theVainglorious(){HP=2;eff=1;name="虚荣勇士";EN+=2;maxHP=4;type=6;}
};
class DongZhuo: public Hero//董卓
{public:
    DongZhuo(){HP=6;name="董卓";maxHP=8;type=7;}
    virtual void passive()
    {
        int k=0;
        int i=this->order;
        vector<int>dmg;
        dmg.push_back(0);
        for(k=0;k<=N;k++)
        {
            if (def[k][i]<att[k][i])
                dmg.push_back(Hlist[k]->damage);
        }
        int maxValue=*max_element(dmg.begin(), dmg.end());
        int sumValue=accumulate(dmg.begin(),dmg.end(),-maxValue);
        if (maxValue==0){//cout<<Hlist[i]->name<<" Not Hurt\n";
            
        }
       
        Hlist[i]->HP-=sumValue;
     
    }
};
class IceWizard:public Hero //冰法
{
public:
    IceWizard(){HP=2;maxHP=4;name="寒冰法师";type=8;}
    int fragment=1;
    int ifUse=0;
    int ifDamage=0;
    vector <int> rc={0,0};//roundcount
    virtual void PrePassive()
    {
        for(int k=0;k<N;k++)
        {if(att[this->order][k]==1) {att[this->order][k]=3; cout<<"IceWizard's lv1 Attack is Strengthened!\n";}
            if(att[k][this->order]==3){att[k][this->order]=1;def[this->order][k]=1;cout<<"lv3 Attack Towards IceWizard is Weakened\n";}
        }
    }
    virtual void ability()
    {
        RecordHistory(4);
        //cout<<"Cd is:"<<*(rc.end()-1)<<*(rc.end()-2);
    
        if (*(rc.end()-1)==1&&*(rc.end()-2)==1){cout<<"Ji刀正在冷却，无法使用 "<<endl;}
        else
        {int i;
            cout<<" target?"<<endl;
            cin>>i;
            attack(this->order,i,0);
            ifUse=1;}
    }
    virtual void passive()
    {
        bool a=Explode();
        AddDamage();
        rc.push_back((ifUse==1)?1:0);
        if(ifUse==1)
        {
            int k;
            int OnTarget=0;
            int p=this->order;
            for(k=0;k<N;k++)
            {if(att[p][k]>def[p][k])OnTarget+=1;}
            fragment+=(OnTarget==0)?1:2;
        }
        if (a==true &&ifDamage>0)
        { this->fragment-=(fragment>7)?7:fragment;}
        cout<<"fragment="<<fragment<<endl;
        ifUse=0;
       
        ifDamage=0;
    }
    bool Explode()
    {
        if (fragment>=5){damage=2;
            return true;}
        else return false;
    }
    void AddDamage()
    {   //判定本回合是否造成伤害（既可以是ji刀的也可以是普通攻击的）
        
        int p=this->order;
        for(int k=0;k<N;k++)
        {if(att[p][k]>def[p][k])//对k造成了伤害
            
            ifDamage+=1;}
        if (Explode()==true &&ifDamage>0)//确认发生暴击，附加额外伤害，并且回血，并且扣除碎片
            {
                for(int k=0;k<N;k++)
                {if(att[p][k]>def[p][k])//对k造成了伤害
                    Hlist[k]->HP-=1;
                }
                this->HP+=1;
            }
        
    }
    
};

class Groot:public Hero //"I am Groot"
{
public:Groot(){HP=3;name="树人";type=9;attackLimit=1;}
int round[N]={0};//用于荆棘保护的倒计时

int p= this->order;
void causeProtection(int k)//对k队友制造保护
{
    for (int m=0;m<N;m++)
    {
    
        if (Hlist[m]->team!=this->team)   //若m不在树人阵营
        {if (
             (round[this->order]==3&&(*(Hlist[m]->History.end()-1)<31))||
            (round[this->order]==2&&(*(Hlist[m]->History.end()-1))<31&&(*(Hlist[m]->History.end()-2))<31)||
             (round[this->order]==1&&(*(Hlist[m]->History.end()-1))<31&&(*(Hlist[m]->History.end()-2))<31&&(*(Hlist[m]->History.end()-3))<31)
             )
             
            {
                Hlist[m]->visionLimit[k]+=1;
               
            }
            
        }
        
    }//m不在树人阵营，且他没有出超防解除控制，那么他不能攻击k
      
   
}
virtual void ability()//荆棘丛生
    
    { RecordHistory(4.09);
        cout<<" 荆棘for Team "<<this->team<<endl;
    if(EN>=3)
    {
        EN+=1;
        defe(1);
        for(int k=0;k<N;k++)
        { if (Hlist[k]->team == this->team ) round[k]=3;}//刷新对k保护的倒计时
        EN-=3;
        
    }
    else cout<<"invalid ability\n";
}
    void Groot_Cure()  //树人治疗
    {
        int i;
        cout<<" Groot:Choose a Player and Cure him/her\n";
        cin>>i;
        Hlist[i]->HP+=1;
        cout<<" Player "<<i<<" is cured by Groot.\n";
    }
virtual void passive()
    {
    
    for(int k=0;k<N;k++)
    {
        
        if (Hlist[k]->team ==this->team && round[k]>0)//若为己方目标
        {
            causeProtection(k);
        }
        
        round[k]-=1;//倒计时-1
    }//完成荆棘的结算
    int j=0;
    for(int k=0;k<N;k++)
    {
        if(att[this->order][k]>def[this->order][k])//树人对k造成了伤害
            j++;//统计本回合树人造成的伤害
    }
   if (j>1)//若伤害大于等于2，触发治疗被动
       Groot_Cure();
    
}
};


/*
class Bot: public Hero
{
public:
    
    Bot(){HP=5;name="Bott";maxHP=7;type=0;}
    bool enemyDanger()
    {
        int j=0;
        for(int i=0;i<N;i++)
        {
            if (i==this->order){}
            else
                j+=Hlist[i]->EN;
        }
    
        return(j=0)? false:true;
    }
    void noEn()
    {
        srand(time_t(nullptr));
        int action = rand()% 3;
        if (enemyDanger()==true)
        {
            if(action==0)
            {Hlist[this->order]->energize();cout<<"Bot Ji"<<endl;}
            else  {Hlist[this->order]->defe(0);cout<<"Bot 防"<<endl;}
        }}
   virtual void random()
    {
        
        if (Hlist[this->order]->EN==0)
        {
            noEn();
        }
        else
        {
            srand(time(nullptr));
            int action = rand()% 3;
            cout<<"random="<<action<<endl;
            if (action==0)noEn();
            else
            {
                srand(time(nullptr));
                int action = rand()% N;
               
                    while(action==this->order)
                    { action = rand()% N;}

                if (Hlist[this->order]->EN<3)
                attack(this->order, action, 1);
                else if (Hlist[this->order]->EN<5)
                    attack(this->order, action, 2);
                else if(action==0)
                    attack(this->order, action, 5);
                cout<< "Bot 刀"<<endl;
            }
        }
    }
};
 */
#endif /* Heros_h */
