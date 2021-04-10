#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<string>
using namespace std;

class base{
    protected:
        int c;
    public:
        virtual void foo(){cout<<"base";};
        bool operator== (int zxc)const{
            return (c==zxc);
        }
        int getc(){return c;}
};



class a:public base{
    public:
    a(int d){
        c=d;
    }
    virtual void foo(){
        cout<<"a";
    }
};

class b:public base{
    public:
    b(int d){
        c=d;
    }
    virtual void foo(){
        cout<<"b";
    }
};

int main(){
    vector<base*> vec;
    a aa(22);
    b bb(33);
    vec.push_back(&aa);
    vec.push_back(&bb);
    vec[0]->foo();
    vec[1]->foo();
    vector<base*>::iterator it=find_if(vec.begin(),vec.end(),[](base* item){return (*item)==22;});
    (*it)->foo();

    fstream open("symbol.txt");
    int num=0;
    string s="";
    char ch;
    while (!open.eof())
    {
        open>>ch;
        cout<<ch<<endl;
        if (ch<='9'&&ch>='0')
        {
            num = num*10 + (ch-48);
        }else if (ch == '\n')
        {
            cout<<num<<" "<<s<<endl;
            num = 0;s="";
        }
        else if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
        {
            s = s + ch;
        }
    }
    
}