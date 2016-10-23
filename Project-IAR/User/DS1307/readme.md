
# DS1307.c #

## Summary ##

DS1307 is a RTC with IIC interface,this is a Driver for it.

# How to use this driver? #


1. call DS1307.d_open() to init IIC and DS1307

2. call DS1307.DS1307_set() to set the time,for example DS1307.DS1307_set(ADDR_MON,12):set 12 as the month.


3. call DS1307.d_puts() to get curtime,for example DS1307.d_puts(0,0,0) It get curtime and storage it to structure Ds1307Time








