/*
 * log.h
 *
 *  Created on: 2015. 4. 1.
 *      Author: Kyungnam
 */

#ifndef PLUGIN_SPOTTER_LOG_H_
#define PLUGIN_SPOTTER_LOG_H_


#include <pthread.h>
#include <iostream>
#include <fstream>
namespace spotter {
class log {
	private:
		static log* instance;

		void init_log();
		std::ofstream log_stream;
		pthread_mutex_t mutex;
		log();
		void get_date(char* time);


	public:
		/*void info(char* str);
		void info(const char* str);
		void info(int value);*/
		template <typename T>
		void info(T value){
			char str_time[30];
			get_date(str_time);
			pthread_mutex_lock(&mutex);
			log_stream<<str_time<<":"<<value<<std::endl;
			pthread_mutex_unlock(&mutex);

		}
		void info(const char* str);
		static log* get_instance();
		virtual ~log();



};


} /* namespace spotter */
#endif /* PLUGIN_SPOTTER_LOG_H_ */
