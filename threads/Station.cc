/* 
 * File:   Station.cc
 * Author: amin
 * 
 * Created on September 23, 2019, 2:47 PM
 */

#include "Station.h"

Station::Station()
{
	id=0;
	numCouch=40;
	numBusiness=20;
}


Station::~Station()
{
}

int Station::getDepTime()
{
	return depTime;
}

int Station::getId()
{
	return id;
}

void Station::setDepTime(int dt)
{
	depTime=dt;
}

void Station::setFares(int coach, int business)
{
	fareCoach=coach;
	fareBusiness=business;
}

void Station::setId(int ID)
{
	id=ID;
}
bool operator==(const Station &s1, const Station &s2)
{
	return s1.id == s2.id;
}

void Station::decNumBusiness(int n)
{
	numBusiness-=n;
}

void Station::decNumCoach(int n)
{
	numCouch-=n;
}

void Station::incNumBusiness(int n)
{
	numBusiness+=n;
}

void Station::incNumCoach(int n)
{
	numCouch+=n;
}

int Station::getNumBusiness()
{
	return numBusiness;
}

int Station::getNumCoach()
{
	return numCouch;
}

int Station::getBusinessFare()
{
	return fareBusiness;
}

int Station::getCoachFare()
{
	return fareCoach;
}
