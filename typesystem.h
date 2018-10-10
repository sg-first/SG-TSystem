#pragma once
#include <string>
#include <vector>
using namespace std;

enum bType{Num,String};
enum tType{_BasicType,_IntersectionType,_UnionType};

class Type
{
protected:
    int typeType;
    string label;
    bool baseEqu(Type &t);
public:
    Type(int typeType):typeType(typeType){}
    //相等为类型严格相等
    //合法（的值）为互赋值能否接受
    virtual bool isLegal(const Type&) {return true;}

    int getTypeType() {return this->typeType;}
    void setLabel(string label) {this->label=label;}
    string getLabel() {return this->label;}
};


class BasicType : public Type
{
protected:
    int basicType;
public:
    BasicType(int basicType):Type(_BasicType),basicType(basicType){}
    virtual bool isLegal(Type &t); //对于基本类型，合法即相等

    int getBasicType() {return this->basicType;}
};


class IntersectionType : public Type
{
protected:
    vector<Type>allType;
public:
    IntersectionType():Type(_IntersectionType){}
    virtual bool isLegal(Type &t); //对于交叉类型，合法即相等

    void addType(Type t) {this->allType.push_back(t);}
};


class UnionType : public Type
{
protected:
    vector<Type>allType;
public:
    UnionType():Type(_UnionType){}
    virtual bool isLegal(Type &t); //对于联合类型，相等为合法的一种情形

    void addType(Type t) {this->allType.push_back(t);}
};
