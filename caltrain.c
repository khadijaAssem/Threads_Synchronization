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

	pthread_mutex_lock(&(station->tS_mutex));//BLOCK ALL CONDITION VARIABLE
	while (station->tB_seated != 0 || (station->free_seats != 0 && station->waiting_passengers != 0))//All passengers are seated?
		pthread_cond_wait(&(station->cond_trainFull),&(station->tS_mutex));//Wait till all passengers are on board
	station->free_seats = 0;//Maba2ash feeh train 5alas
	pthread_mutex_unlock(&(station->tS_mutex));
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
	pthread_mutex_lock(&(station->tS_mutex));
	station->waiting_passengers++;
	while (station->free_seats == 0)
		pthread_cond_wait(&(station->cond_trainArrived),&(station->tS_mutex));
	station->waiting_passengers--;
	station->free_seats--;
	station->tB_seated++;
	pthread_mutex_unlock(&(station->tS_mutex));
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	pthread_mutex_lock(&(station->tS_mutex));
	station->tB_seated--;
	if (station->tB_seated == 0){
		pthread_cond_signal(&(station->cond_trainFull));
	}
	pthread_mutex_unlock(&(station->tS_mutex));
}
