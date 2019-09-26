/* 
 * File:   Station.h
 * Author: amin
 *
 * Created on September 23, 2019, 2:47 PM
 */

#ifndef STATION_H
#define	STATION_H

class Station {
public:
    Station();
    virtual ~Station();
    int getId();
    void setDepTime(int);
    void setFares(int, int);
    void setId(int);
    int getDepTime();
    friend bool operator==(const Station&, const Station&);
    void decNumCoach(int);
    void decNumBusiness(int);
    void incNumCoach(int);
    void incNumBusiness(int);
    int getNumCoach();
    int getNumBusiness();
    int getBusinessFare();
    int getCoachFare();

private:
    int id;
    int depTime;
    int fareCoach;
    int fareBusiness;
    int numCouch;
    int numBusiness;
};

#endif	/* STATION_H */

