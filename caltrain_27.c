#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	// FILL ME IN
	pthread_mutex_init(&(station->tS_mutex), NULL);
	pthread_cond_init(&(station->cond_trainFull), NULL);
	pthread_cond_init(&(station->cond_trainArrived), NULL);
	station->waiting_passengers = 0;
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
	station->free_seats = count;
	station->tB_seated = 0;

	pthread_cond_broadcast(&(station->cond_trainArrived));//YA GAMA3A L ATR GAH

	pthread_mutex_lock(&(station->tS_mutex));//BLOCK ALL CONDITION VARIABLE /*Msh 3arfa lazem a7otaha wala la2*/
	while (station->tB_seated != 0 || (station->free_seats != 0 && station->waiting_passengers != 0))//All passengers are seated?
		pthread_cond_wait(&(station->cond_trainFull),&(station->tS_mutex));//Wait till all passengers are on board
	pthread_mutex_unlock(&(station->tS_mutex));/*Msh 3arfa lazem a7otaha wala la2*/
	
	station->free_seats = 0;//Maba2ash feeh train 5alas
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
	pthread_mutex_lock(&(station->tS_mutex));
	station->waiting_passengers++;//PASSENGER GDEED GAH MSTANY
	while (station->free_seats == 0)//MAFESH FREE SEATS -ASL MAFESH TRAIN
		pthread_cond_wait(&(station->cond_trainArrived),&(station->tS_mutex));//HASTANA L7AD MA L TRAIN YEGY
	station->waiting_passengers--;//L TRAIN GAH FA HA&GEZ BA2A LELLY WA2EF
	station->free_seats--;
	station->tB_seated++;//LMAFROOOF L ROBOT LESSA HAYA%DOH W Y2A3ADOH
	pthread_mutex_unlock(&(station->tS_mutex));
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	pthread_mutex_lock(&(station->tS_mutex));
	station->tB_seated--;
	pthread_mutex_unlock(&(station->tS_mutex));

	if (station->tB_seated == 0){ //KOLO 2A3AD? BARA L CRITICAL SECTOION 3alashan : kda kda msh had5ol hna ella lama a decrement l tB_Seated
		pthread_cond_signal(&(station->cond_trainFull));
	}
}