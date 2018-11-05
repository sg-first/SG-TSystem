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

//开洞开始
//注意，这种等价this必须是有占位符的一边，不能反过来
bool UnionType::isEquIgP(Type *t)
{
    if(this->baseEqu(t))
    {
        UnionType* pt=dynamic_cast<UnionType*>(t);
        if(pt->allType.size()==this->allType.size())
        {
            for(unsigned int i=0;i<pt->allType.size();i++)
            {
                if(this->allType.at(i)->getTypeType()!=_Placeholder) //对于this.rootT中是占位符的位置，可以不相等，所以就不检查
                {
                    Type* &t1=this->allType.at(i);
                    Type* &t2=pt->allType.at(i);
                    if(!t1->isEqu(t2)) //有一个不相等就不行
                        return false;
                }
            }
            return true;
        }
        else
            return false;
    }
    return false;
}

bool IntersectionType::isEquIgP(Type *t)
{
    if(this->baseEqu(t))
    {
        IntersectionType* pt=dynamic_cast<IntersectionType*>(t);
        if(pt->allType.size()==this->allType.size())
        {
            for(unsigned int i=0;i<pt->allType.size();i++)
            {
                if(this->allType.at(i)->getTypeType()!=_Placeholder) //对于this.rootT中是占位符的位置，可以不相等，所以就不检查
                {
                    Type* &t1=this->allType.at(i);
                    Type* &t2=pt->allType.at(i);
                    if(!t1->isEqu(t2)) //有一个不相等就不行
                        return false;
                }
            }
            return true;
        }
        else
            return false;
    }
    return false;
}

bool MapType::isEquIgP(Type *t)
{
    if(this->baseEqu(t))
    {
        MapType* pt=dynamic_cast<MapType*>(t);
        if(this->image->getTypeType()!=_Placeholder)
        {
            if(!this->image->isEqu(pt->image))
                return false;
        }
        if(this->inverseImage->getTypeType()!=_Placeholder)
        {
            if(!this->inverseImage->isEqu(pt->inverseImage))
                return false;
        }
        return true;
    }
    return false;
}

bool ParametrisedType::isEqu(Type *t)
{
    if(this->baseEqu(t))
    {
        ParametrisedType* pt=dynamic_cast<ParametrisedType*>(t);
        return this->rootT->isEqu(pt->rootT); //类型相等暗含占位符个数相等
    }
    return false;
}

bool ParametrisedType::isEquIgP(Type *t)
{
    if(this->baseEqu(t))
    {
        if(this->rootT->getTypeType()==_Placeholder)
            return true;
        else
            return this->isEqu(t);
    }
    return false;
}

unsigned int UnionType::getParNum()
{
    unsigned int result=0;
    for(Type* t:this->allType)
    {
        if(t->getTypeType()==_Placeholder)
            result++;
    }
    return result;
}

unsigned int IntersectionType::getParNum()
{
    unsigned int result=0;
    for(Type* t:this->allType)
    {
        if(t->getTypeType()==_Placeholder)
            result++;
    }
    return result;
}

unsigned int MapType::getParNum()
{
    unsigned int result=0;
    if(this->image->getTypeType()==_Placeholder)
        result++;
    if(this->inverseImage->getTypeType()==_Placeholder)
        result++;
    return result;
}
