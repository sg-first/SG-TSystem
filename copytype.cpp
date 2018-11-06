#include "typesystem.h"

Type* copyType(Type* t)
{
    if(t->getTypeType()==_BasicType)
        return new BasicType(*dynamic_cast<BasicType*>(t));
    if(t->getTypeType()==_IntersectionType)
        return new IntersectionType(*dynamic_cast<IntersectionType*>(t));
    if(t->getTypeType()==_UnionType)
        return new UnionType(*dynamic_cast<UnionType*>(t));
    if(t->getTypeType()==_MapType)
        return new MapType(*dynamic_cast<MapType*>(t));
    if(t->getTypeType()==_ParametrisedType)
        return new ParametrisedType(*dynamic_cast<ParametrisedType*>(t));
    if(t->getTypeType()==_Placeholder)
        return new Placeholder(*dynamic_cast<Placeholder*>(t));
    return nullptr;
}

IntersectionType::IntersectionType(const IntersectionType &t):StructuredType(t)
{
    for(Type* t:this->allType)
        this->allType.push_back(copyType(t));
}

UnionType::UnionType(const UnionType &t):StructuredType(t)
{
    for(Type* t:this->allType)
        this->allType.push_back(copyType(t));
}

IntersectionType::~IntersectionType()
{
    for(Type* t:this->allType)
        delete t;
}

UnionType::~UnionType()
{
    for(Type* t:this->allType)
        delete t;
}

MapType::~MapType()
{
    delete this->image;
    delete this->inverseImage;
}

MapType::MapType(Type *inverseImage, Type *image)
{
    this->image=copyType(inverseImage);
    this->inverseImage=copyType(image);
}

MapType::MapType(const MapType &t):StructuredType(t)
{
    this->image=copyType(t.image);
    this->inverseImage=copyType(t.inverseImage);
}

ParametrisedType::ParametrisedType(StructuredType *rootT)
{
    this->rootT=dynamic_cast<StructuredType*>(copyType(rootT));
    this->parnum=rootT->getParNum();
}

ParametrisedType::ParametrisedType(const ParametrisedType &t)
{
    this->rootT=dynamic_cast<StructuredType*>(copyType(t.rootT));
    this->parnum=t.parnum;
}

void UnionType::reSetElm(unsigned int sub, Type *t)
{
    delete this->allType[sub];
    this->allType[sub]=copyType(t);
}

void IntersectionType::reSetElm(unsigned int sub, Type *t)
{
    delete this->allType[sub];
    this->allType[sub]=copyType(t);
}

void MapType::reSetImage(Type* t)
{
    delete this->image;
    this->image=copyType(t);
}

void MapType::reSetInverseImage(Type *t)
{
    delete this->inverseImage;
    this->inverseImage=copyType(t);
}

void ParametrisedType::reSetRootT(StructuredType* t)
{
    delete this->rootT;
    this->rootT=dynamic_cast<StructuredType*>(copyType(t));
}

StructuredType* ParametrisedType::specialization(vector<Type*>&parlist)
{
    if(parlist.size()>this->parnum)
        throw string("Too many parameters");

    StructuredType* resultType=dynamic_cast<StructuredType*>(copyType(this->rootT)); //返回复制的
    unsigned int nowsub=0; //最后结果是特化数目

    //warn:如果添加新的构造类型这里要进行添加
    if(this->rootT->getTypeType()==_UnionType)
    {
        UnionType* pt=dynamic_cast<UnionType*>(resultType);
        for(unsigned int i=0;i<pt->getSize();i++)
        {
            if(pt->isParametrise(i))
            {
                pt->reSetElm(i,parlist.at(nowsub));
                nowsub++;
            }
        }
    }
    if(this->rootT->getTypeType()==_IntersectionType)
    {
        IntersectionType* pt=dynamic_cast<IntersectionType*>(resultType);
        for(unsigned int i=0;i<pt->getSize();i++)
        {
            if(pt->isParametrise(i))
            {
                pt->reSetElm(i,parlist.at(nowsub));
                nowsub++;
            }
        }
    }
    if(this->rootT->getTypeType()==_MapType)
    {
        MapType* pt=dynamic_cast<MapType*>(resultType);
        if(pt->isImageParametrise())
        {
            pt->reSetImage(parlist.at(nowsub));
            nowsub++;
        }
        if(pt->isInverseImageParametrise())
        {
            pt->reSetImage(parlist.at(nowsub));
            nowsub++;
        }
    }
    if(this->rootT->getTypeType()==_ParametrisedType)
    {
        ParametrisedType* pt=dynamic_cast<ParametrisedType*>(resultType);
        if(pt->isRootTParametrise())
        {
            pt->reSetRootT(dynamic_cast<StructuredType*>(parlist.at(nowsub))); //此处开发者有责任进行保障
            nowsub++;
        }
    }

    if(this->parnum==nowsub) //完全特化完成，直接返回
        return resultType;
    else
    {
        ParametrisedType* relresultType=new ParametrisedType(resultType);
        delete resultType;
        return relresultType;
    }
}

Type* MapType::calu(Type* t)
{
    if(!this->inverseImage->isEqu(t))
        throw string("Mismatch with inverseImage");
    return copyType(this->image);
}

void IntersectionType::addType(Type *t)
{
    this->allType.push_back(copyType(t));
}

void UnionType::addType(Type *t)
{
    this->allType.push_back(copyType(t));
}
