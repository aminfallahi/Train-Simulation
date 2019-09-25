/* 
 * File:   Seat.h
 * Author: amin
 *
 * Created on September 23, 2019, 2:46 PM
 */

#ifndef SEAT_H
#define	SEAT_H

class Seat {
public:
    Seat();
    virtual ~Seat();
private:
    int id;
    int type; //business or coach
};

#endif	/* SEAT_H */

