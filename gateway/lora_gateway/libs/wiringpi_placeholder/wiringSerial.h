#pragma once

int serialDataAvail(int fd){
	return 0;
}

uint8_t serialGetchar(int fd){
	return 0;
}

void serialPutchar(int fd, uint8_t data){}

void serialClose(int fd){}

int serialOpen(const char *device, int baudRate){
	return 0;
}
