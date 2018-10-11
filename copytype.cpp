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

IntersectionType::IntersectionType(const IntersectionType &t):Type(t)
{
    for(Type* t:this->allType)
        this->allType.push_back(copyType(t));
}

UnionType::UnionType(const UnionType &t):Type(t)
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

MapType::MapType(const MapType &t):Type(t)
{
    this->image=copyType(t.image);
    this->inverseImage=copyType(t.inverseImage);
}
