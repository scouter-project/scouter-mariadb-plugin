/*
 * log.cpp
 *
 *  Created on: 2015. 4. 1.
 *      Author: Kim kyungnam
 */
#include "util/log.h"
#include <iostream>
#include <time.h>
namespace spotter {

log* log::instance = NULL;

log* log::get_instance() {
	if(log::instance == NULL)
	{
		log::instance = new log();
	}
	return log::instance;
}

log::log() {
	pthread_mutex_init(&mutex,NULL);
	const char log_file_name[200] = "/home/monm/log/monitor2.log";
	log_stream.open(log_file_name,std::ofstream::out | std::ofstream::app);


}

log::~log() {
	log_stream.close();
	pthread_mutex_destroy(&mutex);
}


/*
void log::info(char* msg)
{
	char str_time[30];
	get_date(str_time);
	pthread_mutex_lock(&mutex);
	log_stream<<str_time<<":"<<msg<<std::endl;
	pthread_mutex_unlock(&mutex);

}
*/

void log::info(const char* msg)
{
	char str_time[30];
	get_date(str_time);
	pthread_mutex_lock(&mutex);
	log_stream<<str_time<<":"<<msg<<std::endl;
	pthread_mutex_unlock(&mutex);

}
/*
void log::info(int value) {
	char str_time[30];
	get_date(str_time);
	pthread_mutex_lock(&mutex);
	log_stream<<str_time<<":"<<value<<std::endl;
	pthread_mutex_unlock(&mutex);
}
*/

void log::get_date(char* dest){
	time_t curr;
	tm* local;
	time(&curr);
	local=localtime(&curr);
	strftime(dest,20,"%Y-%m-%d %H:%M:%S",local);
}
}
