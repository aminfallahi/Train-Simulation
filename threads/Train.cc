/* 
 * File:   Train.cc
 * Author: amin
 * 
 * Created on September 6, 2019, 4:32 PM
 */


#include "Train.h"
#include "Request.h"

int Train::lastId;

Train::Train()
{
	id = lastId;
	lastId++;
}

Train::~Train()
{

}

void Train::addStation(int ID, int depTime, int fareCoach, int fareBusiness)
{
	printf("Addint station to train\n");
	Station* station = new Station();
	station->setId(ID);
	station->setDepTime(depTime);
	station->setFares(fareCoach, fareBusiness);
	stations.Append(station);
}

void Train::addRequest(Request* req)
{
	requests.Append(req);
	ListIterator<Station*> stationIter(&stations);
	bool reachedDep=false;
	for (; !stationIter.IsDone(); stationIter.Next()){
		if (stationIter.Item()->getId()==req->getDepStation()->getId() || reachedDep){
			reachedDep=true;
			if (req->isBusiness())
				stationIter.Item()->decNumBusiness(req->getPassengerCount());
			else
				stationIter.Item()->decNumCoach(req->getPassengerCount());
		}
	}
}

List<Request*> Train::getRequests()
{
	return requests;
}

List<Station*> Train::getStations()
{
	return stations;
}

bool Train::hasRoom(int num, bool isBusiness, Station* station)
{
	if (isBusiness) {
		if (seatCountBusiness - numBusiness >= num)
			return true;
		return false;
	}
	if (seatCountCoach - numCoach >= num)
		return true;
	return false;
}

bool Train::processRequestArrive(Request* req)
{
	/*if (req->isBusiness()) {
		if (this->hasRoom(req->getPassengerCount(), true)) {
			numBusiness += req->getPassengerCount();
			req->setOnTrain();
		}
		return true;
	}
	if (this->hasRoom(req->getPassengerCount(), false)) {
		numCoach += req->getPassengerCount();
		req->setOnTrain();
		return true;
	}*/
	req->setOnTrain();
	return false;
}

void Train::processRequestDepart(Request* req)
{
	req->setFinished();
	/*if (req->isBusiness())
		numBusiness -= req->getPassengerCount();
	else
		numCoach -= req->getPassengerCount();*/
	

}

int Train::getId()
{
	return id;
}

bool operator==(const Train &t1, const Train &t2)
{
	return t1.id == t2.id;
}

bool Train::findTrain(Request* req)
{
	int depdesCount = 0;
	ListIterator<Station*> stationIter(&stations);
	for (; !stationIter.IsDone(); stationIter.Next()) {
		if (depdesCount == 0) {
			if (stationIter.Item()->getId() == req->getDepStation()->getId()) {
				depdesCount++;
			}
		} else {
			if (stationIter.Item()->getId() == req->getDesStation()->getId()) {
				depdesCount++;
			}
		}
	}
	if (depdesCount==2)
		return true;
	return false;
}
