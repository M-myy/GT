#ifndef _eeprom_h_
#define _eeprom_h_

void eeprom_read_dat(unsigned char *buf,unsigned char addr,unsigned char len);
void eeprom_write_dat(unsigned char *buf,unsigned char addr,unsigned char len);

#endif
