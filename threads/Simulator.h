/* 
 * File:   SuperMarket.h
 * Author: amin
 *
 * Created on September 6, 2019, 4:32 PM
 */

#ifndef SIMULATOR_H
#define	SIMULATOR_H

#include "list.h"
#include "Train.h"
#include "Request.h"
#include "Station.h"

class Simulator {
public:
    Simulator();
    Station* getRandomStation(int);
    bool addRequest(Request*);
    bool addStation(int,int,int,int);
    Station* getStationById(int);
    void addTrain(Train*);
    List<Train*> getTrains();
    void addTrainThread(Thread*);
    void addResThread(Thread*);
    Thread* getTrainThreadById(int);
    Thread* getResThreadById(int);
private:
    List<Station*> stations;
    List<Request*> requests;
    List<Train*> trains;
    List<Thread*> resThreads;
    List<Thread*> trainThreads;
};

#endif	/* SIMULATOR_H */

