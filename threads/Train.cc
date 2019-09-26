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

//add station to train route. this should be done in order.
void Train::addStation(int ID, int depTime, int fareCoach, int fareBusiness)
{
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
	bool reachedDep = false;
	for (; !stationIter.IsDone(); stationIter.Next()) {
		if (stationIter.Item()->getId() == req->getDepStation()->getId() || reachedDep) {
			reachedDep = true;
			if (req->isBusiness())
				stationIter.Item()->decNumBusiness(req->getPassengerCount());
			else
				stationIter.Item()->decNumCoach(req->getPassengerCount());
		}
		if (stationIter.Item()->getId() == req->getDesStation()->getId())
			break;
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

//go through every station for every train and see if there is room for current request at each station for each train
bool Train::hasRoom(Request *req)
{
	ListIterator<Station*> stationIter(&stations);
	bool reachedDep = false;
	for (; !stationIter.IsDone(); stationIter.Next()) {
		if (stationIter.Item()->getId() == req->getDepStation()->getId() || reachedDep) {
			reachedDep = true;
			if (req->isBusiness()) {
				if (stationIter.Item()->getNumBusiness() < req->getPassengerCount())
					return false;
			} else {
				if (stationIter.Item()->getNumCoach() < req->getPassengerCount())
					return false;
			}
		}
		if (stationIter.Item()->getId() == req->getDesStation()->getId())
			break;
	}
	return true;
}

bool Train::processRequestArrive(Request* req)
{
	req->setOnTrain();
	return false;
}

void Train::processRequestDepart(Request* req)
{
	req->setFinished();


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
	if (depdesCount == 2)
		return true;
	return false;
}

int Train::calculateFare(Request* req)
{
	ListIterator<Station*> stationIter(&stations);
	bool reachedDep = false;
	int fare = 0;
	for (; !stationIter.IsDone(); stationIter.Next()) {
		if (stationIter.Item()->getId() == req->getDepStation()->getId() || reachedDep) {
			reachedDep = true;
			if (req->isBusiness())
				fare += stationIter.Item()->getBusinessFare() * req->getPassengerCount();
			else
				fare += stationIter.Item()->getCoachFare() * req->getPassengerCount();
		}
		if (stationIter.Item()->getId() == req->getDesStation()->getId())
			break;
	}
	return fare;
}

int Train::getNumOfRequests()
{
	return requests.NumInList();
}

int Train::getNumOfPassengers()
{
	int num=0;
	ListIterator<Request*> reqIter(&requests);
	for (; !reqIter.IsDone(); reqIter.Next()){
		num+=reqIter.Item()->getPassengerCount();
	}
	return num;
}
