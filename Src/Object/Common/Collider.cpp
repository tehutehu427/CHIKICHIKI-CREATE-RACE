#include "Collider.h"

Collider::Collider(ObjectBase& _owner,COL_TAG _tag,COLLISION_TYPE _type, int _modelId):
	owner_(_owner), tag_(_tag), type_(_type), modelId_(_modelId)
{

}

Collider::~Collider(void)
{
}
