#ifndef __PARSE__
#define __PARSE__

#define DELIMITER '\n'

bool parse(char* data, char output[][MAX_STR_LEN]) {

  uint16_t end = strlen(data);
  
  uint16_t current = 0;
  uint16_t wordBegin = current;
  uint8_t i = 0;
  
  while(current <= end) {
    if(data[current] == DELIMITER || current == end) {
      memcpy(output[i], data + wordBegin, current - wordBegin);
      wordBegin = current + 1;
      i++;
    }
    current++;
  }
  
  return true;
    
}

#endif;
