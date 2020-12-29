#ifndef __QAX_FC_TG_C__
#define __QAX_FC_TG_C__

typedef struct my_data{
	char buf[128];
	int flags;
}qax_msg_t;

#define FC_TGC_IOC_MAGIC 'x'

#if 0
#define FC_TGC_IOC_R	_IOR(FC_TGC_IOC_MAGIC, 1, qax_msg_t)
#define FC_TGC_IOC_W	_IOW(FC_TGC_IOC_MAGIC, 1, qax_msg_t)
#define FC_TGC_IOC_RW	_IOWR(FC_TGC_IOC_MAGIC, 1, qax_msg_t)
#define FC_TGC_IOC_CW	_IOW(FC_TGC_IOC_MAGIC, 1, qax_msg_t)
#endif

#define FC_TGC_IOC_R	_IOR(FC_TGC_IOC_MAGIC, 1, int)
#define FC_TGC_IOC_W	_IOW(FC_TGC_IOC_MAGIC, 1, int)
#define FC_TGC_IOC_RW	_IOWR(FC_TGC_IOC_MAGIC, 1, int)
#define FC_TGC_IOC_CW	_IOW(FC_TGC_IOC_MAGIC, 1, char)

#endif
