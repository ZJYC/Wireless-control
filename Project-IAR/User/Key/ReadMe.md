# ADC-Key #

## summary ##

the key driver based on ADC value.

# how to use it? #


1. **d_ADC_Key.d_open()** to init the GPIO and related register
2. timing call **d_ADC_Key.d_TimingProcess()**;
3. call func **d_ADC_Key.d_Gets()** to get the Key,default is 'N'
4. call func **d_ADC_Key.d_close()** to deinit.

note:

there are about 0.5s delay between User's pressing key and **d_ADCKey.dGets()** get the Keynum,because the we need to filter the sample.

