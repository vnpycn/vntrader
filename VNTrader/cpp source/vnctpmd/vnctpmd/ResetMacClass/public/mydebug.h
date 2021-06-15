#ifndef MYDEBUG_H_H_
#define MYDEBUG_H_H_
 
#ifdef _DEBUG
/////////����
#define ASSERT(f)\
	if(!(f)) _asm{int 3}
////////////����ڴ�й¶
#define ISLEAKED \
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);\
	flag |= _CRTDBG_LEAK_CHECK_DF;\
	_CrtSetDbgFlag(flag);
#define new new(_NORMAL_BLOCK,   __FILE__,   __LINE__)

#else

//#define ASSERT(f)
#define ISLEAKED

#endif


#endif