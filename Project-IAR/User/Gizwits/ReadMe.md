
# Gizwits.c #

this file is the main code to communicate with Gizwits<Gokit3>,It is based on USART protocol which was made by myself.

# The function explain: #

**GizwitsInit**

to init the USART which Gizwits used..

**GizwitsExecDirective**

execute the instruction from Gizwits,It call function<GizwitsRecv>,**Notificationg:**if nothing received,It will blocked here,

**GizwitsSync**

upload information to the Gizwits,





