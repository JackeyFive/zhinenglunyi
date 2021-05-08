#include "sys.h"   
#include "delay.h"


void sendmessage(void);
void sendtoserver(void);
void sendtogprs(u8 *s);
void tostring(float f,char *buf);

extern u8 s; 
extern float wendu;
extern float shidu;
extern float rate;
extern float spo2;
extern float jingdu;
extern float weidu;
extern u8 *message;

