/* 
 * File:   Request.cc
 * Author: amin
 * 
 * Created on September 23, 2019, 2:59 PM
 */

#include "Request.h"

int Request::lastId;

Request::Request()
{
	id=lastId;
	lastId++;
	onTrain=false;
	finished=false;
}

Request::~Request()
{
}

void Request::setThread(Thread* rt)
{
	reqThread=rt;
}

Station* Request::getDepStation()
{
	return depStation;
}

Station* Request::getDesStation()
{
	return desStation;
}

int Request::getPassengerCount()
{
	return passengersCount;
}

Thread* Request::getThread()
{
	return reqThread;
}


bool Request::isBusiness()
{
	return seatType;
}

void Request::setDepStation(Station* ds)
{
	depStation=ds;
}

void Request::setDepTime(int dt)
{
	depTime=dt;
}

void Request::setDesStation(Station* ds)
{
	desStation=ds;
}

void Request::setFinished()
{
	finished=true;
}

void Request::setOnTrain()
{
	onTrain=true;
}

void Request::setPassengerCount(int pc)
{
	passengersCount=pc;
}

void Request::setSeatType(bool st)
{
	seatType=st;
}

bool Request::isFinished()
{
	return finished;
}

int Request::getId()
{
	return id;
}
