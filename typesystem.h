#pragma once
#include <string>
#include <vector>
using namespace std;

enum bType{Num,String};
enum tType{_BasicType,_IntersectionType,_UnionType,_MapType};

class Type
{
protected:
    string label;
    bool baseEqu(Type* t) const;
public:
    Type(){}
    Type(const Type& t):label(t.getLabel()){}
    virtual ~Type(){}
    //相等为类型严格相等
    //合法（的值）为互赋值能否接受
    virtual bool isLegal(Type*)=0;
    virtual bool isEqu(Type* t)=0;
    virtual int getTypeType() const =0;

    void setLabel(string label) {this->label=label;}
    string getLabel() const {return this->label;}
};


class BasicType : public Type
{
protected:
    int basicType;
public:
    BasicType(int basicType):basicType(basicType){}
    BasicType(const BasicType& t):Type(t),basicType(t.getBasicType()){}
    virtual bool isLegal(Type* t);
    virtual bool isEqu(Type* t) {return this->isLegal(t);}  //对于基本类型，合法即相等
    virtual getTypeType() const {return _BasicType;}

    int getBasicType() const {return this->basicType;}
};


class IntersectionType : public Type
{
protected:
    vector<Type*>allType;
public:
    IntersectionType(){}
    IntersectionType(const IntersectionType& t);
    virtual ~IntersectionType();
    virtual bool isLegal(Type *t);
    virtual bool isEqu(Type* t) {return this->isLegal(t);}  //对于交叉类型，合法即相等
    virtual getTypeType() const {return _IntersectionType;}

    void addType(Type* t); //拷贝之后再添加，持有所有权
};


class UnionType : public Type
{
protected:
    vector<Type*>allType;
public:
    UnionType(){}
    UnionType(const UnionType& t);
    virtual ~UnionType();
    virtual bool isLegal(Type* t);
    virtual bool isEqu(Type *t); //对于联合类型，相等为合法的一种情形
    virtual getTypeType() const {return _UnionType;}

    void addType(Type* t); //拷贝之后再添加，持有所有权
};

class MapType : public Type
{
protected:
    Type* inverseImage;
    Type* image;
public:
    MapType(Type* inverseImage,Type* image); //凡是用指针传递到成员的，全部复制，然后持有复制后对象的所有权
    MapType(const MapType& t);
    virtual ~MapType();
    virtual bool isLegal(Type* t);
    virtual bool isEqu(Type* t) {return this->isLegal(t);} //对于映射类型，合法即相等
    virtual getTypeType() const {return _MapType;}
};
