#include <stdio.h>
#include "JavaTest.h"
#include <jni.h>

int addOne (int x) {
    
    
    x+=1;
    return x;     
}

JNIEXPORT jint JNICALL Java_JavaTest_addOne(JNIEnv * env, jobject o, jint i)
{
  printf("in c");
	return i+=3;
  
}


