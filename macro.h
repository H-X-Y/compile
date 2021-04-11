#define eqd(n) (n>='0'&&n<='9')

#define eql(c) ((c>='a'&&c<='z')||(c>='A'&&c<='Z'))

#define back cFile.seekg(-3,ios::cur)

#define if_opera_else(eop1,eop2,op1,snum) if (s == op1)\
{\
    operators<eop1,eop2>* veco = new operators<eop1, eop2>(snum, op1);\
    svec.emplace_back(veco);\
}else


#define symToEng(sym,eng) const char eng[] = sym;