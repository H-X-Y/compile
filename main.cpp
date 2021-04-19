#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<stdio.h>
#include<map>
#include<stack>
#include<assert.h>
#include"macro.h"

using namespace std;

enum Type{integer,floting,character};
enum Cat{sig,arr};

class syntax;
struct symTabElem{
    Type type;
    Cat cat;
    int addr;
};

class lexResultElem{
public:
    int num;
    string sym;
    
    lexResultElem(int n,string s=" "):num(n),sym(s){}
};

map<string,symTabElem> symTab;
vector<lexResultElem> lvec;

//词法分析
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





class symbol{
protected:
    int snum;
    string sstr;
    friend class syntax;

protected:
    static vector<symbol*> svec;
    static string character;
    static ofstream fout;
    static FILE *fp;
    static char ch;
    static streampos pos;
    // static map<string,symTabElem> symTab;
    // static vector<lexResultElem> lvec;
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
        fout<<snum<<endl;
        lvec.emplace_back(lexResultElem(snum));
    }

    void freeVector(){
        for (auto i : svec){
            delete(i);
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
//map<string,symTabElem> symbol::symTab;
//vector<lexResultElem> symbol::lvec;

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
        fout<<snum<<' '<<tnum<<endl;
        lvec.emplace_back(lexResultElem(1));
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
        fout<<snum<<' '<<l<<endl;
        symTab.emplace(l,symTabElem());
        lvec.emplace_back(2,l);
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
            if_opera_else(gr,eq,">",num)
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
    openFile.close();
}


class syntax{
private:
    vector<lexResultElem>::iterator sym;
    int tem;
    vector<lexResultElem>::iterator symNext(){
        return sym++;
    }

    void symBack(){
        sym--;
    }

    //program ::= type id ‘(‘ param_decl_list ‘)’ compound_stmt
    void program(){
        type();
        if (sym->num == 2) //id
        {
            symNext();
            if (sym->num == 11) //(
            {
                symNext();
                param_decl_list();
                if (sym->num == 12) //)
                {
                    symNext();
                    compound_stmt();
                }else{
                    cout<<"program缺少)\n";
                }
            }else{
                cout<<"缺少(\n";
            }
        }else{
            cout<<"缺少函数名称\n";
        }
    }

    //type ::= int | char | void
    void type(){
        if (sym->num == 201) //void
        {
            symNext();
        }else if(sym->num == 202) //int
        {
            symNext();
        }else if (sym->num == 204) //char
        {
            symNext();
        }else
        {
            cout<<"类型错误\n";
        }
    }

    //param_decl_list ::= param_decl (‘,’ param_decl )*| empty
    void param_decl_list(){
        if (sym->num != 12)
        {
            param_decl();
            while (sym->num == 19) //,
            {
                symNext();
                param_decl();
            }
        }
    }

    //param_decl ::= type id 
    void param_decl(){
        type();
        if (sym->num == 2) //id
        {
            symNext();
        }else
        {
            cout<<"未输入变量名\n";
        }
    }

    //compound_stmt ::= ‘{‘ (var_decl* stmt*)? ‘}’
    void compound_stmt(){
        if (sym->num == 15)
        {
            symNext();
            while (true)
            {
                if (sym->num == 201 || sym->num == 202 || sym->num == 204)
                {
                    var_decl();
                }else if (sym==lvec.end())
                {
                    cout<<"缺少}\n";
                    break;
                }else if (sym->num == 16)
                {
                    break;
                }else
                {
                    stmt();
                }
            }
            if (sym->num == 16) //{
            {
                symNext();
            }else
            {
                cout<<"缺少}\n";
            }
        }
        
    }

    //var_decl ::= type var_decl_list ‘;’
    void var_decl(){
        type();
        var_decl_list();
        if (sym->num == 24) //;
        {
            symNext();
        }else
        {
            cout<<"缺少;\n";
        }
    }

    //var_decl_list ::= variable_id ( ‘,’ variable_id)*
    void var_decl_list(){
        variable_id();
        while (sym->num == 19) //,
        {
            symNext();
            variable_id();
        }
    }

    //variable_id ::= id ( ‘=’ expr )?
    void variable_id(){
        if (sym->num == 2)
        {
            symNext();
            if (sym->num == 9) //=
            {
                symNext();
                expr();
            }
        }else
        {
            cout<<"变量定义缺少变量名\n";
        }
    }

    //stmt ::= expr ';'|compound_stmt | cond_stmt | while_stmt | break ‘;’ | continue ‘;’ | return expr ‘;’ 
    void stmt(){
        if (sym->num == 15) //{
        {
            compound_stmt();
        }else if (sym->num == 205) //if
        {
            cond_stmt();
        }else if (sym->num == 207) //while
        {
            while_stmt();
        }else if (sym->num == 210) //break
        {
            symNext();
            if (sym->num == 24) //;
            {
                symNext();
            }else
            {
                cout<<"缺少;\n";
            }
        }else if (sym->num == 211) //continue
        {
            symNext();
            if (sym->num == 24) //;
            {
                symNext();
            }else
            {
                cout<<"缺少;";
            }
        }else if (sym->num == 212) //return
        {
            symNext();
            if (sym->num == 24) //;
            {
                symNext();
            }else
            {
                cout<<"缺少;";
            }
        }else
        {
            expr();
            if (sym->num == 24)
            {
                symNext();
            }else
            {
                cout<<"表达式缺少;\n";
            }
            
            
        }
    }

    //cond_stmt ::= if ‘(‘ expr ‘)’ stmt (else stmt)?
    void cond_stmt(){
        if (sym->num == 205) //if
        {
            symNext();
            if (sym->num == 11) //(
            {
                symNext();
                expr();
                if (sym->num == 12) //)
                {
                    symNext();
                    stmt();
                    if (sym->num == 206) //else
                    {
                        symNext();
                        stmt();
                    }
                }else
                {
                    cout<<"if缺少)\n";
                }
            }else
            {
                cout<<"缺少(\n";
            }
        }
        
    }

    //while_stmt ::= while ‘(‘ expr ‘)’ stmt
    void while_stmt(){
        if (sym->num == 207)
        {
            symNext();
            if (sym->num == 11)
            {
                symNext();
                if (sym->num == 12)
                {
                    symNext();
                    stmt();
                }else
                {
                    cout<<"while缺少)\n";
                }
            }else
            {
                cout<<"缺少(\n";
            }
        }
        
    }

    

    // enum X{exprop ,sim_expr,cmp,add_expr,addop,term,mulop,factor,call,args,args_list}x;
    // enum A{lpar=100,rpar,id,num,lt,gr,eq,ge,le,ne,add,sub,mul,div,sem}a;

    // int M[11][15] = {
    //    //(  )  id NUM <  > == >= <= !=  +  -  *  /  ;
    //     {13,99,13,13,99,99,99,99,99,99,99,99,99,99,99},
    //     {14,99,14,14,99,99,99,99,99,99,99,99,99,99,99},
    //     {99,99,99,99,15,15,15,15,15,15,99,99,99,99,99},
    //     {16,99,16,16,99,99,99,99,99,99,99,99,99,99,99},
    //     {99,99,99,99,99,99,99,99,99,99,17,17,99,99,99},
    //     {18,99,18,18,99,99,99,99,99,99,99,99,99,99,99},
    //     {99,99,99,99,99,99,99,99,99,99,99,99,19,19,99},
    //     {20,99,20,20,99,99,99,99,99,99,99,99,99,99,99},
    //     {99,99,21,99,99,99,99,99,99,99,99,99,99,99,99},
    //     {22,0,22,22,99,99,99,99,99,99,99,99,99,99,0},
    //     {23,99,23,23,99,99,99,99,99,99,99,99,99,99,99}
    // };

    // stack<int> S;

    //expr ::= id = expr | sim_expr
    void expr(){
        if (sym->num == 2) //id
        {
            symNext();
            if (sym->num == 9) //=
            {
                symNext();
                expr();
            }else
            {
                symBack();
                sim_expr();
            }
        }else
        {
            sim_expr();
        }
        
        
        // while (sym->num != 24 && sym->num != 12)
        // {
        //     symNext();
        // }
    }

    //sim_expr ::= add_expr [ cmp add_expr ]
    void sim_expr(){
        add_expr();
        if (sym->num == 7 || sym->num == 8 || sym->num == 101 || sym->num == 102 || sym->num == 103 || sym->num == 104)
        {
            cmp();
            add_expr();
        }
        
    }

    //cmp ::= < | > | == | >= | <= | != 
    void cmp(){
        if (sym->num == 7)
        {
            symNext();
        }else if (sym->num == 8)
        {
            symNext();
        }else if (sym->num == 101)
        {
            symNext();
        }else if (sym->num == 102)
        {
            symNext();
        }else if (sym->num == 103)
        {
            symNext();
        }else if (sym->num == 104)
        {
            symNext();
        }
    }


    //add_expr ::= term { addop term }
    void add_expr(){
        term();
        while (sym->num == 3 || sym->num == 4)
        {
            addop();
            term();
        }
        
    }

    //addop ::= + | - 
    void addop(){
        if (sym->num == 3)
        {
            symNext();
        }else if (sym->num == 4)
        {
            symNext();
        }
        
        
    }

    //term ::= factor { mulop factor } 
    void term(){
        factor();
        while (sym->num == 5 || sym->num == 6)
        {
            mulop();
            factor();
        }
    }

    //mulop ::= * | /
    void mulop(){
        if (sym->num == 5)
        {
            symNext();
        }else if (sym->num == 6)
        {
            symNext();
        }
    }

    //factor ::= ( expr ) | id | call | NUM
    void factor(){
        if (sym->num == 11)
        {
            symNext();
            expr();
            if (sym->num == 12)
            {
                symNext();
            }else
            {
                cout<<"表达式缺少)\n";
            }
        }else if (sym->num == 2)
        {
            symNext();
            if (sym->num == 11)
            {
                symBack();
                call();
            }
        }else if (sym->num == 1)
        {
            symNext();
        }else
        {
            symNext();
            cout<<"应输入一个语句\n";
            //assert(0);
        }
    }

    //call ::= id ( args )
    void call(){
        if (sym->num == 2)
        {
            symNext();
            if (sym->num == 11)
            {
                symNext();
                args();
                if (sym->num == 12)
                {
                    symNext();
                }else
                {
                    cout<<"函数调用缺少)\n";
                }
            }else
            {
                cout<<"函数调用缺少(\n";
            }
        }else
        {
            cout<<"需用函数名进行调用\n";
        }
        
    }

    //args ::= arg_list | empty
    void args(){
        if (sym->num == 12)
        {
            return;
        }else
        {
            arg_list();
        }
    }

    //arg_list ::= expr { ,expr }
    void arg_list(){
        expr();
        while (true)
        {
            if (sym->num == 19)
            {
                symNext();
                expr();
            }else if (sym->num == 12)
            {
                break;
            }else
            {
                cout<<"函数调用时参数列表错误\n";
            }
        }
        
    }
    // #define retTag(a,b) if(i->num == a){return b;}else

    // int toAryTag(vector<lexResultElem>::iterator i){
    //     retTag(3,10) //+
    //     retTag(4,11) //-
    //     retTag(5,12) //*
    //     retTag(6,13) ///
    //     retTag(7,4) //<
    //     retTag(8,5) //>
    //     retTag(101,8) //<=
    //     retTag(102,7) //>=
    //     retTag(103,9) //!=
    //     retTag(104,6) //==
    //     retTag(2,2) //id
    //     retTag(1,3) //num
    //     retTag(11,0) //*
    //     retTag(12,1) //*
    //     retTag(24,14) //*
    //     retTag(9,15) //=
    //     {
    //         return -1;
    //     }
    // }

public:
    syntax(){
        tem = 0;
        sym = lvec.begin();
    }

    void start(){
        while (sym != lvec.end())
        {
            program();
        }
    }
};









int main(int argc, char * argv []){
    symbol lex;
    lex.lexical(argv[1]);

    syntax syn;
    syn.start();
}