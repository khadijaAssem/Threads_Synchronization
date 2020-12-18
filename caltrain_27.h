#include <pthread.h>

struct station {
	// FILL ME IN
	int waiting_passengers;
	int free_seats;
	int tB_seated;
	pthread_mutex_t tS_mutex;
	pthread_cond_t cond_trainFull, cond_trainArrived;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);