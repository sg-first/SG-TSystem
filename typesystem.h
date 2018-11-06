#pragma once
#include <string>
#include <vector>
using namespace std;

enum bType{Num,String,Null,Void};
enum tType{_BasicType,_IntersectionType,_UnionType,_MapType,_Placeholder,_ParametrisedType};

class Type
{
protected:
    string label;
public:
    Type(){}
    Type(const Type& t):label(t.getLabel()){}
    virtual ~Type(){}
    bool baseEqu(Type* t) const;
    //相等为类型严格相等
    //合法（的值）为互赋值能否接受
    virtual bool isLegal(Type*)=0;
    virtual bool isEqu(Type*)=0;
    virtual int getTypeType() const =0;
    virtual bool isCompositeType() {return false;}

    void setLabel(string label) {this->label=label;}
    string getLabel() const {return this->label;}
};


class Placeholder : public Type //只有复合类型才有空插占位符，别的不行
{
public:
    Placeholder(const Placeholder& t):Type(t){}
    virtual bool isLegal(Type*) {return true;}
    virtual bool isEqu(Type*) {return true;}
    virtual int getTypeType() const {return _Placeholder;}
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
    virtual int getTypeType() const {return _BasicType;}

    int getBasicType() const {return this->basicType;}
};


class StructuredType : public Type
{
public:
    virtual bool isCompositeType() {return true;}
    virtual bool isEquIgP(Type *t)=0; //感觉完全是为了给参数类型信息开的洞
    virtual unsigned int getParNum()=0;
};


class IntersectionType : public StructuredType
{
protected:
    vector<Type*>allType;
public:
    IntersectionType(){}
    IntersectionType(const IntersectionType& t);
    virtual ~IntersectionType();
    virtual bool isLegal(Type *t);
    virtual bool isEqu(Type* t);  //对于交叉类型，合法与相等逻辑相同
    virtual bool isEquIgP(Type *t);
    virtual unsigned int getParNum();
    virtual int getTypeType() const {return _IntersectionType;}

    void addType(Type* t); //拷贝之后再添加，持有所有权
    //为参数类型开的洞
    void reSetElm(unsigned int sub,Type* t);
    bool isParametrise(unsigned int sub) {return this->allType[sub]->getTypeType()==_Placeholder;}
    unsigned int getSize() {return this->allType.size();}
};


class UnionType : public StructuredType
{
protected:
    vector<Type*>allType;
public:
    UnionType(){}
    UnionType(const UnionType& t);
    virtual ~UnionType();
    virtual bool isLegal(Type* t);
    virtual bool isEqu(Type *t); //对于联合类型，相等为合法的一种情形
    virtual bool isEquIgP(Type *t);
    virtual unsigned int getParNum();
    virtual getTypeType() const {return _UnionType;}

    void addType(Type* t); //拷贝之后再添加，持有所有权
    //为参数类型开的洞
    void reSetElm(unsigned int sub,Type* t);
    bool isParametrise(unsigned int sub) {return this->allType[sub]->getTypeType()==_Placeholder;}
    unsigned int getSize() {return this->allType.size();}
};


class MapType : public StructuredType
{
protected:
    Type* inverseImage;
    Type* image;
public:
    MapType(Type* inverseImage,Type* image); //凡是用指针传递到成员的，全部复制，然后持有复制后对象的所有权
    MapType(const MapType& t);
    virtual ~MapType();
    virtual bool isLegal(Type* t);
    virtual bool isEqu(Type* t);
    virtual bool isEquIgP(Type *t);
    virtual unsigned int getParNum();
    virtual int getTypeType() const {return _MapType;}

    Type* calu(Type* t);

    //为参数类型开的洞
    void reSetImage(Type* t);
    void reSetInverseImage(Type* t);
    bool isImageParametrise() {return this->image->getTypeType()==_Placeholder;}
    bool isInverseImageParametrise() {return this->inverseImage->getTypeType()==_Placeholder;}
};


class ParametrisedType : public StructuredType //所有含有占位符的类型都应该被本类包装
{
protected:
    StructuredType* rootT;
    unsigned int parnum;
public:
    ParametrisedType(StructuredType *rootT);
    ParametrisedType(const ParametrisedType& t);
    virtual ~ParametrisedType() {delete this->rootT;}
    virtual bool isLegal(Type* t);
    //这个isLegal的思想——特化产生的类型看作ParametrisedType的“值”。因此与相等及不充分也不必要
    virtual bool isEqu(Type* t); //相等即严格相等
    virtual int getTypeType() const {return _ParametrisedType;}
    virtual unsigned int getParNum() {return (rootT->getTypeType()==_Placeholder)?1:0;}
    virtual bool isEquIgP(Type *t);

    StructuredType* specialization(vector<Type*>&parlist);
    //parlist中不应再包含占位符（因为有遍历开销，这里就先不检查了）。如果所有参数都有，返回特化后的rootT，否则仍然返回ParametrisedType

    //为参数类型开的洞
    void reSetRootT(StructuredType* t);
    bool isRootTParametrise() {return this->rootT->getTypeType()==_Placeholder;}
};
