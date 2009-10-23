#include <stdio.h>

int addOne (int x) {
    
    
    x+=1;
    return x;     
}
main ()
{
    int z = 2;
    z = addOne(z);
    printf ("The value of k is %d\n", z); 
}
