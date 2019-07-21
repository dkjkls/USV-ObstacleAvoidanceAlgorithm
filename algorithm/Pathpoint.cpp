#include "stdafx.h"
#include "Pathpoint.h"


Pathpoint::Pathpoint()
{
	GeneralP.pa = 0.0f;
	GeneralP.pm = 0.0f;
}

Pathpoint::Pathpoint(float pm, float pa)
{
	this->GeneralP.pm = pm;
	this->GeneralP.pa = pa;
}

Pathpoint::~Pathpoint()
{
}