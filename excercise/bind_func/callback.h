typedef void (*typecallback) (void);


#ifdef __cplusspluss
extern "C" { 
#endif
	int cInit(typecallback cb); 
#ifdef __cplusspluss
};
#endif



