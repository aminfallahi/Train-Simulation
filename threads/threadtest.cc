#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "list.h"
#include "Simulator.h"
#include <stdlib.h>
#include <time.h>
#include "Request.h"
#include "Station.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "Train.h"

using namespace std;

std::vector<std::string> split(std::string str, std::string sep)
{
	char* cstr = const_cast<char*> (str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}


Simulator sim;
int hour, minute;
Thread* resT[1000];
int numPassStation[1000];
int numOfStations = 0;
void trainThread(Train* train);
void reservationThread(Request* req);
int reqGrantedPerTime = 0, reqRefusedPerTime = 0;
int reqTot = 0, reqTotGranted = 0;
int passTot = 0;

void adminThread(int x)
{
	kernel->interrupt->SetLevel(IntOff);

	srand(time(NULL));
	Train::lastId = 0;
	Request::lastId = 0;
	//read from files
	ifstream in;
	string line;
	in.open("../threads/in.txt");
	while (getline(in, line)) {
		Train* train = new Train();
		Thread* newTrainThread;
		newTrainThread = new Thread("train thread");
		sim.addTrainThread(newTrainThread);
		vector<string> stations;
		stations = split(line, "-");
		int i;
		//add stations to train route
		for (i = 0; i < stations.size(); i++) {
			vector<string> stationInfo;
			stationInfo = split(stations[i], "*");
			int depTime = stoi(split(stationInfo[1], ":")[0])*60 + stoi(split(stationInfo[1], ":")[1]);
			train->addStation(stoi(stationInfo[0]), depTime, stoi(stationInfo[2]), stoi(stationInfo[3]));
			if (sim.addStation(stoi(stationInfo[0]), depTime, stoi(stationInfo[2]), stoi(stationInfo[3])))
				numOfStations++;
		}
		sim.addTrain(train);

		sim.getTrainThreadById(train->getId())->Fork((VoidFunctionPtr) trainThread, (void*) train);
	}

	for (hour = 6; hour < 23; hour++) {

		for (minute = 0; minute < 60; minute += 10) {
			printf("-----------\nTIME %d:%d\n", hour, minute);
			int reqCount;
			reqGrantedPerTime = 0;
			reqRefusedPerTime = 0;
			for (reqCount = 0; reqCount < 5; reqCount++) {
				//create new request
				Request* req = new Request();
				resT[req->getId()] = new Thread("forked thread");
				resT[req->getId()]->Fork((VoidFunctionPtr) reservationThread, (void *) req);
				reqTot++;
			}
			kernel->currentThread->Yield();
			printf("\n\tGranted/Refused requests in this time unit: %d %d\n\n", reqGrantedPerTime, reqRefusedPerTime);
		}

	}

	printf("--------\nPrinting totals:\n");
	int i;
	List<Train*> trains = sim.getTrains();
	ListIterator<Train*> trainIter(&trains);
	for (; !trainIter.IsDone(); trainIter.Next()) {
		printf("\tNum of itineraries served for train %d: %d\n", trainIter.Item()->getId(), trainIter.Item()->getNumOfRequests());
		printf("\tNum of passengers for train %d: %d\n", trainIter.Item()->getId(), trainIter.Item()->getNumOfPassengers());
		passTot += trainIter.Item()->getNumOfPassengers();
	}
	int max = 0, maxId = 0;
	i = 0;
	for (i = 0; i < numOfStations; i++) {
		if (numPassStation[i] > max) {
			max = numPassStation[i];
			maxId = i;
		}
	}
	printf("\n\tThe busiest section is between station %d and station %d\n", maxId, maxId + 1);
	printf("\tTotal number of passengers served: %d\n", passTot);
	printf("\tTotal number of requests: %d\n", reqTot);
	printf("\tTotal number of granted requests: %d\n", reqTotGranted);
}

void trainThread(Train* train)
{
	kernel->interrupt->SetLevel(IntOff);

	while (true) { //or hour<23?
		//check train arrival-departure hours and see if it's arrival time
		List<Station*> stations = train->getStations();
		ListIterator<Station*> stationsIter(&stations);
		for (; !stationsIter.IsDone(); stationsIter.Next()) {
			int t = stationsIter.Item()->getDepTime();

			if ((int) (t - 10) / 60 == hour && (t - 10) % 60 == minute) { //arrival
				List<Request*> requests = train->getRequests();
				ListIterator<Request*> reqIter(&requests);

				//check to see if any reservations are scheduled for this train
				//get new passengers on the train
				int numToBoard = 0;
				int numItinerary = 0;
				for (; !reqIter.IsDone(); reqIter.Next()) {
					if (reqIter.Item()->getDepStation()->getId() == stationsIter.Item()->getId()) {
						train->processRequestArrive(reqIter.Item());
						printf("\tTrain %d arrived for request %d\n", train->getId(), reqIter.Item()->getId());

						//make reservation thread ready to run
						kernel->scheduler->ReadyToRun(resT[reqIter.Item()->getId()]);
						numToBoard += reqIter.Item()->getPassengerCount();
						numItinerary++;
						//itineraryTot++;
						//passTot += reqIter.Item()->getPassengerCount();
						numPassStation[stationsIter.Item()->getId()] += reqIter.Item()->getPassengerCount();
					}
				}
				printf("\tNumber of passengers boarding train %d at station %d: %d\n", train->getId(), stationsIter.Item()->getId(), numToBoard);
				printf("\tNumber of itineraries for boarding train %d at station %d: %d\n", train->getId(), stationsIter.Item()->getId(), numItinerary);

			}
			if ((int) t / 60 == hour && t % 60 == minute) { //departure
				List<Request*> requests = train->getRequests();

				ListIterator<Request*> reqIter(&requests);

				int numToDepart = 0;
				int numItinerary = 0;

				//check reservations and remove passengers with current destination
				for (; !reqIter.IsDone(); reqIter.Next()) {
					if (reqIter.Item()->getDesStation()->getId() == stationsIter.Item()->getId()) {
						train->processRequestDepart(reqIter.Item());
						printf("\tTrain %d reached destination for request %d\n", train->getId(), reqIter.Item()->getId());

						//make reservation thread ready to run
						kernel->scheduler->ReadyToRun(resT[reqIter.Item()->getId()]);
						numToDepart += reqIter.Item()->getPassengerCount();
						numItinerary++;

					}
				}
				printf("\tNumber of passengers departing train %d at station %d: %d\n", train->getId(), stationsIter.Item()->getId(), numToDepart);
				printf("\tNumber of itineraries for departing train %d at station %d: %d\n", train->getId(), stationsIter.Item()->getId(), numItinerary);

			}

		}
		kernel->currentThread->Yield();
	}

}

void reservationThread(Request* req)
{
	kernel->interrupt->SetLevel(IntOff);
	
	//generate random departure/destination station
	int depStationId = rand() % numOfStations + 1;
	int desStationId = rand() % numOfStations + 1;
	while (depStationId == desStationId)
		desStationId = rand() % numOfStations + 1;
	if (desStationId < depStationId) {
		int temp = desStationId;
		desStationId = depStationId;
		depStationId = temp;
	}
	bool seatType = (rand() % 2) == 1 ? true : false;
	printf("\n\tCreating new request %d with departure station %d and destination station %d and seat type %s\n", req->getId(), depStationId, desStationId, seatType ? "Business" : "Coach");
	Station* depStation = new Station();
	depStation = sim.getStationById(depStationId);
	Station* desStation = new Station();
	desStation = sim.getStationById(desStationId);
	//set request info
	req->setSeatType(seatType);
	req->setPassengerCount(rand() % 8 + 1);
	req->setDepStation(depStation);
	req->setDesStation(desStation);
	//check if there is any train with appropriate path and seats to handle this req
	if (sim.addRequest(req)) {
		reqGrantedPerTime++;
		reqTotGranted++;
		//request granted. Sleep until train arrives.
		kernel->currentThread->Sleep(false);
	} else {
		reqRefusedPerTime++;
		//request refused. Finish thread
		kernel->currentThread->Finish();
	}
	//train arrived for this reservation, if I am at this point, train thread should have made me ready to run.
	//I will sleep again until my train reaches destination
	kernel->currentThread->Sleep(false);
	//train arrived at destination. That is why train thread has made me ready to run
	//let's finish this reservation. nothing more to do.
	kernel->currentThread->Finish();
}

void
ThreadTest()
{

	kernel->interrupt->SetLevel(IntOff);
	Thread *adminT = new Thread("admin thread");
	adminT->Fork((VoidFunctionPtr) adminThread, (void*) 1);
}
