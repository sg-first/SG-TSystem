#include "typesystem.h"

bool Type::baseEqu(Type *t) const
{
    return (this->getTypeType()==t->getTypeType())&&(this->getLabel()==t->getLabel());
}

bool BasicType::isLegal(Type *t)
{
    if(this->baseEqu(t))
    {
        BasicType* pt=dynamic_cast<BasicType*>(t);
        if(this->getBasicType()==pt->getBasicType())
            return true;
    }
    return false;
}

bool IntersectionType::isLegal(Type* t)
{
    if(this->baseEqu(t))
    {
        IntersectionType* pt=dynamic_cast<IntersectionType*>(t);
        if(this->allType.size()==pt->allType.size())
        {
            for(unsigned int i=0;i<this->allType.size();i++)
            {
                if(!this->allType.at(i)->isEqu(pt->allType.at(i)))
                    return false;
            }
            return true;
        }
    }
    return false;
}

bool UnionType::isLegal(Type *t)
{
    if(this->isEqu(t)) //相等为合法的一种情形
        return true;
    else
    {
        for(Type* ti:this->allType)
        {
            if(ti->isEqu(t))
                return true;
        }
    }
    return false;
}

bool UnionType::isEqu(Type *t)
{
    if(this->baseEqu(t))
    {
        UnionType* pt=dynamic_cast<UnionType*>(t);
        for(unsigned int i=0;i<this->allType.size();i++)
        {
            if(!this->allType.at(i)->isEqu(pt->allType.at(i)))
                return false;
        }
        return true;
    }
    return false;
}


bool MapType::isLegal(Type *t)
{
    if(this->baseEqu(t))
    {
        MapType* pt=dynamic_cast<MapType*>(t);
        if(this->inverseImage->isEqu(pt->inverseImage)&&this->image->isEqu(pt->image))
            return true;
    }
    return false;
}
