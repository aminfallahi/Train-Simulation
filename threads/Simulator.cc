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
	//Adding request and trying to find a train for it
	//find a train that goes from depStation and desStation and has room for passengersCount
	bool trainFound = false;
	ListIterator<Train*> trainIter(&trains);
	for (; !trainIter.IsDone(); trainIter.Next()) {
		if (trainIter.Item()->findTrain(req)) {
			if (!trainIter.Item()->hasRoom(req)) {
				printf("\t\tTrain %d matches request %d path but there is no available seat\n", trainIter.Item()->getId(), req->getId(), trainIter.Item()->getId());
				continue;
			}
			trainIter.Item()->addRequest(req);
			trainFound = true;
			requests.Append(req);
			printf("\t\tTrain %d found for request %d\n", trainIter.Item()->getId(), req->getId());
			printf("\t\tFare for request %d is %d\n", req->getId(), trainIter.Item()->calculateFare(req));
			break;
		}
	}
	if (!trainFound)
		printf("\t\tTrain not found for request %d\n", req->getId());
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

List<Train*> Simulator::getTrains()
{
	return trains;
}

void Simulator::addResThread(Thread* t)
{
	resThreads.Append(t);
}

void Simulator::addTrainThread(Thread* t)
{
	trainThreads.Append(t);
}

Thread* Simulator::getResThreadById(int rtid)
{
	ListIterator<Thread*> threadIter(&resThreads);
	int i = 0;
	for (; !threadIter.IsDone(); threadIter.Next()) {
		if (i == rtid)
			return threadIter.Item();
		i++;
	}
}

Thread* Simulator::getTrainThreadById(int rtid)
{
	ListIterator<Thread*> threadIter(&trainThreads);
	int i = 0;
	for (; !threadIter.IsDone(); threadIter.Next()) {
		if (i == rtid)
			return threadIter.Item();
		i++;
	}
}