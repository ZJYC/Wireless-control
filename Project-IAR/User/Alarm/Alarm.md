# Alarm.c #

----------

## summary ##

this is just a simple GPIO driver for LED&&Ring..etc
## API ##

**Alarm_open**

this function init the GPIO register needed,I use STM32_HAL Function to implement that

**Alarm_close**

deinit the GPIO register

**Alarm_detect**

not used now

**Alarm_command**

not used now

**Alarm_set**

set GPIO the new state

**Alarm_puts**

input the instruction to the structure "caution",Here we use MUTEX to sync the structure.

**Alarm_gets**

not used now

**Alarm_timing_process**

the most operation was implemented by timing call this API,So we need to run this code at a timer interrupt or timing loop and pass the timer interval to it


----------


# How to use this driver? #

----------

1. step 1:call Alarm.d_open() to init the GPIO
2. step 2:timingly call Alarm.d_timing_proceee(interval),for example  Alarm.d_timing_proceee(20)indicate we call it every 20ms

3. step 3:Alarm.d_puts() to do want you want,for example:Alarm.d_puts(LED2,"10001000",3) indicate we flick LED2 like "10001000" style for 3 times
