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
    return nullptr;
}

IntersectionType::IntersectionType(const IntersectionType &t):CompositeType(t)
{
    for(Type* t:this->allType)
        this->allType.push_back(copyType(t));
}

UnionType::UnionType(const UnionType &t):CompositeType(t)
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

MapType::MapType(const MapType &t):CompositeType(t)
{
    this->image=copyType(t.image);
    this->inverseImage=copyType(t.inverseImage);
}

ParametrisedType::ParametrisedType(CompositeType *rootT)
{
    this->rootT=dynamic_cast<CompositeType*>(copyType(rootT));
    this->parnum=rootT->getParNum();
}

ParametrisedType::ParametrisedType(const ParametrisedType &t)
{
    this->rootT=dynamic_cast<CompositeType*>(copyType(t.rootT));
    this->parnum=t.parnum;
}
