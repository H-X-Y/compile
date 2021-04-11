#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<stdio.h>
#include<map>
#include"macro.h"
using namespace std;


symToEng("+",ad)
symToEng("=",eq)
//symToEng("-",min)
const char mi[] = "-";
symToEng("*",mu)
symToEng("/",di)
symToEng("<",le)
symToEng(">",gr)
symToEng("!",no)
symToEng("%",mo)
symToEng("&",an)
symToEng("|",oR)

enum Type{integer,floting,character};
enum Cat{sig,arr};

struct symTabElem{
    Type type;
    Cat cat;
    int addr;
};

class symbol{
protected:
    int snum;
    string sstr;

protected:
    static vector<symbol*> svec;
    static string character;
    static ofstream fout;
    static FILE *fp;
    static char ch;
    static streampos pos;
    static map<string,symTabElem> symTab;
public:
    symbol(const int n,const string s):snum(n),sstr(s){}
    symbol(){}

    int getInt(){return snum;}

    string& getSstr(){return sstr;}

    void lexical(char* cname){
        loadC(cname);
        loadSymbol();
        traverse();
        close();
        freeVector();
    }
    
    void getC(char* cname){}

    virtual void processor(){
        result();
    }

    virtual bool operator==(string s){
        return sstr == s;
    }

    virtual void result(){
        cout<<snum<<endl;
        fout<<snum<<endl;
    }

    void freeVector(){
        for (auto i : svec){
            delete(i);
        }
        for(auto i : symTab){
            cout<<i.first<<" "<<i.second.type;
        }
    }

    void close(){
        fout.close();
        fclose(fp);
    }

private:
    

    void loadSymbol();

    void loadC(char* cname){
        fp=fopen(cname,"rt");
    }

    void traverse(){
        fout.open("out.txt");
        ch=fgetc(fp);
        while (ch != EOF)
        {
            while ((ch==' '||ch=='\n'||ch=='\t')&&ch != EOF)
            {
                ch=fgetc(fp);
            }
            auto it = find_if(svec.begin(),svec.end(),[](symbol* sy){
                if (eqd(ch))
                {
                    return sy->getInt()==1;
                }else if (eql(ch))
                {
                    return sy->getInt()==2;
                }else{
                    return sy->getSstr()==string(1,ch);
                }
            });
            if (it != svec.end())
            {
                (*it)->processor();
            }
    
            ch=fgetc(fp);
        }
        
    }
};

vector<symbol*> symbol::svec;
string symbol::character="";
FILE *symbol::fp;
char symbol::ch=' ';
streampos symbol::pos=0;
ofstream symbol::fout;
map<string,symTabElem> symbol::symTab;

class number:public symbol{
private:
    int tnum = 0;
public:
    number(int n,string s):symbol(n,s){}

    virtual void processor(){
        while (eqd(ch))
        { 
            tnum = tnum*10 + (ch-48);
            ch=fgetc(fp);
        }
        fseek(fp,-1,SEEK_CUR);
        result();
    }

    virtual void result(){
        cout<<snum<<' '<<tnum<<endl;
        fout<<snum<<' '<<tnum<<endl;
        tnum=0;
    }
};

class letter:public symbol{
private:
    string l="";
public:
    letter(int n,string s):symbol(n,s){}

    virtual void processor(){
        l="";
        while(eql(ch)||eqd(ch))
        {
            l = l + ch;
            ch=fgetc(fp);
        } 
        fseek(fp,-1,SEEK_CUR);
        auto it = find_if(svec.begin(),svec.end(),[this](symbol* s){return (*s)==l;});
        if (it != svec.end())
        {
            (*it)-> result();
        }else{
            result();
        }
    }

    virtual void result(){
        cout<<snum<<' '<<l<<endl;
        fout<<snum<<' '<<l<<endl;
        symTab.emplace(l,symTabElem());
    }
};

template <const char* op1,const char* op2> 
class operators:public symbol{
public:
    operators(int n,string s):symbol(n,s){}

    virtual void processor(){
        ch=fgetc(fp);
        if (ch == *op2)
        {
            auto it = find_if(svec.begin(),svec.end(),[this](symbol* s){return (*s)==(string(1,*op1)+op2);});
            (*it)-> result();
        }else{
            fseek(fp,-1,SEEK_CUR);
            result();
        }
    }
};



void symbol::loadSymbol()
{
    number* vecn = new number(1, "`");
    svec.emplace_back(vecn);
    letter* vecl = new letter(2, "`");
    svec.emplace_back(vecl);
    fstream openFile("symbol.txt");
    char tch;
    int num = 0;
    string s = "";
    while (!openFile.eof())
    {
        openFile.get(tch);
        if (tch <= '9' && tch >= '0')
        {
            num = num * 10 + (tch - 48);
        }
        else if (tch == '\n')
        {
            if_opera_else(ad,eq,"=",num)
            if_opera_else(mi,eq,"-",num)
            if_opera_else(mu,eq,"*",num)
            if_opera_else(di,eq,"/",num)
            if_opera_else(le,eq,"<",num)
            if_opera_else(gr,eq,"<",num)
            if_opera_else(eq,eq,"=",num)
            if_opera_else(no,eq,"!",num)
            if_opera_else(mo,eq,"%",num)
            if_opera_else(an,an,"&",num)
            if_opera_else(oR,oR,"|",num)
            {
            symbol* vecs = new symbol(num,s);
            svec.emplace_back(vecs);
            }
            num = 0;
            s = "";
        }
        else if (tch == ' ')
        {
        }
        else
        {
            s = s + tch;
        }
    }
    cout<<"加载符号\n";
    for_each(svec.begin(),svec.end(),[](symbol* sy){cout<<sy->getInt()<<' '<<sy->getSstr();return 1;});
    cout<<endl;
    openFile.close();
}
