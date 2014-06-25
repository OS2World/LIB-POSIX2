/* sample DLL for testdl */

#include <math.h>

int my2(int argument);
float mysin(float argument);

int my2(int argument)
{
 return (2*argument);
}


float mysin(float argument)
{
 return (sin((double)argument));
}
