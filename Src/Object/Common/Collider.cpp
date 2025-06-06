#include "Collider.h"

Collider::Collider(COL_TAG type, int modelId)
{
	type_ = type;
	modelId_ = modelId;
}

Collider::~Collider(void)
{
}
