#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>

struct FileBuffer {
	uint8_t delta;
	uint16_t* start_time_buffer; // military time
	uint16_t* end_time_buffer;
	char* info_tag_buffer[32]; 
	char** info_buffer; // related times to info strings
};

void Schedule_read(char* file_name, uint32_t length);
void Schedule_write(char* file_name, uint32_t length);

#endif
