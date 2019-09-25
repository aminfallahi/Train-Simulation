/* 
 * File:   SuperMarket.cc
 * Author: amin
 * 
 * Created on September 6, 2019, 4:32 PM
 */

#include "Simulator.h"
#include "Train.h"
#include "Station.h"
#include "Request.h"

Simulator::Simulator()
{

}

bool Simulator::addRequest(Request* req)
{
	printf("Adding request %d and trying to find a train for it\n", req->getId());
	//printf("This request dep station is %d and des station is %d\n",req->getDepStation()->getId(),req->getDesStation()->getId());
	//find a train that goes from depStation and desStation and has room for passengersCount
	bool trainFound = false;
	ListIterator<Train*> trainIter(&trains);
	for (; !trainIter.IsDone(); trainIter.Next()) {
		//printf("iterating train %d\n", trainIter.Item()->getId());
		if (!trainIter.Item()->hasRoom(req->getPassengerCount(), req->isBusiness()))
			continue;

		if (trainIter.Item()->findTrain(req)) {
			trainIter.Item()->addRequest(req);
			trainFound = true;
			requests.Append(req);
			printf("Train %d found for request %d\n", trainIter.Item()->getId(), req->getId());
			break;
		}
	}
	if (!trainFound)
		printf("Train not found for request %d\n", req->getId());
	return trainFound;
}

bool Simulator::addStation(int id, int depTime, int fareCoach, int fareBusiness)
{
	Station* station = new Station();
	station->setId(id);
	ListIterator<Station*> stationIter(&stations);
	for (; !stationIter.IsDone(); stationIter.Next()) {
		if (stationIter.Item()->getId() == id)
			return false;
	}
	printf("creating station with id %d\n", id);
	stations.Append(station);
	return true;
}

Station* Simulator::getRandomStation(int pivot)
{
	int numStations = stations.NumInList();
	printf("numstations is %d\n", numStations);
	int r = rand() % (numStations - pivot) + pivot;
	ListIterator<Station*> stationIter(&stations);
	int i;
	for (i = 0; i < r; i++, stationIter.Next());
	return stationIter.Item();
}

Station* Simulator::getStationById(int sid)
{
	ListIterator<Station*> stationIter(&stations);
	for (; !stationIter.IsDone(); stationIter.Next())
		if (stationIter.Item()->getId() == sid)
			return stationIter.Item();
	printf("Station not found\n");
}

void Simulator::addTrain(Train* tr)
{
	trains.Append(tr);
}
