//defining magic number to device driver
#define MAGIC_NO 'k'

//defining configurations to be performed on device
#define ALL_DEFINE_CONFIGS _IOWR(MAGIC_NO,0,int)
#define SET_BAUD_RATE _IOW(MAGIC_NO,1,int)
#define GET_BAUD_RATE _IOR(MAGIC_NO,2,int)
#define ODD_PARITY _IO(MAGIC_NO,3)
#define EVEN_PARITY _IO(MAGIC_NO,4)
#define SET_BIT _IOW(MAGIC_NO,5,int)
#define RESET_BIT _IOW(MAGIC_NO,6,int)