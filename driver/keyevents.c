#include <linux/uinput.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int fd = 0;

void emit(int fd, int type, int code, int val)
{
	struct input_event ie;
	ie.type = type;
	ie.code = code;
	ie.value = val;
	/* timestamp values below are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;
	write(fd, &ie, sizeof(ie));
}
void send_key (int fd, int key_code)
{
	/* Key press, report the event, send key release, and report again */
	emit(fd, EV_KEY, key_code, 1);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	emit(fd, EV_KEY, key_code, 0);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	usleep(100);
}
int signal_handler(int sigNum)
{
	printf("\n !! SIGNAL COUGHT !! \n");
	if(ioctl(fd, UI_DEV_DESTROY) < 0)
		printf("ERROR: uinput device not closed\n");
	else
		printf("uinput device closed \n");
	exit(0);
}

int main(void)
{
	//struct uinput_setup usetup;
	struct uinput_user_dev uidev;
	struct input_event ev;
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	int i = 0;
	int x = 0, y = 0;
	if(signal(SIGINT,signal_handler)==SIG_ERR)
	{
		printf("error registering signal handler\n");
	}
	/*
	 * The ioctls below will enable the device that is about to be
	 * created, to pass key events, in this case the space key.
	 */
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
	//ioctl(fd, UI_SET_KEYBIT, BTN_MOUSE);
	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(fd, UI_SET_KEYBIT, BTN_FORWARD);
	ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
	//memset(&usetup, 0, sizeof(usetup));
	//usetup.id.bustype = BUS_USB;
	//usetup.id.vendor = 0x1234; /* sample vendor */
	//usetup.id.product = 0x5678; /* sample product */
	//strcpy(usetup.name, "Example device");
	//ioctl(fd, UI_DEV_SETUP, &usetup);

	if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
		printf("error: ioctl %d\n",__LINE__);
	if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
		printf("error: ioctl %d\n",__LINE__);
	if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
		printf("error: ioctl %d\n",__LINE__);

	//if(ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)       
//		printf("error: ioctl %d\n",__LINE__);
//	if(ioctl(fd, UI_SET_ABSBIT,ABS_X) < 0)
//		printf("error: ioctl %d\n",__LINE__);
//	if(ioctl(fd, UI_SET_ABSBIT, ABS_Y) < 0)
//		printf("error: ioctl %d\n",__LINE__);

	memset(&uidev, 0, sizeof(uidev));
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 0x1234; /* sample vendor */
	uidev.id.product = 0x5678; /* sample product */
	strcpy(uidev.name, "virtual keyboard device");

	uidev.absmin[ABS_X]=0;
	uidev.absmax[ABS_X]=1023;
	uidev.absfuzz[ABS_X]=0;
	uidev.absflat[ABS_X ]=0;
	uidev.absmin[ABS_Y]=0;
	uidev.absmax[ABS_Y]=767;
	uidev.absfuzz[ABS_Y]=0;
	uidev.absflat[ABS_Y ]=0;	

	write(fd, &uidev, sizeof(uidev));
	//ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);
	//40 Chapter 1. Linux Input Subsystem/devices/virtual/input/input17 userspace API
	//The Linux input driver subsystem, Release 4.13.0-rc4+
	/*
	 * On UI_DEV_CREATE the kernel will create the device node for this
	 * device. We are inserting a pause here so that userspace has time
	 * to detect, initialize the new device, and can start listening to
	 * the event, otherwise it will not notice the event we are about
	 * to send. This pause is only needed in our example code!
	 */
	//while(i < 10)
	{
		//send_key(fd, KEY_SPACE);
		//send_key(fd, BTN_LEFT);
		//i++;
	}

	while(1)
	{

		printf("\nEnter the absoulte x(0-1023) and y(0-767) co-ordinates:");
		scanf("%d %d",&x,&y);       
		memset(&ev, 0, sizeof(struct input_event));
		gettimeofday(&ev.time,NULL);
		ev.type = EV_REL;
		ev.code = REL_X;
		ev.value = x;
		if(write(fd, &ev, sizeof(struct input_event)) < 0)
		printf("error: ioctl %d\n",__LINE__);

		memset(&ev, 0, sizeof(struct input_event));
		ev.type = EV_SYN;
		if(write(fd, &ev, sizeof(struct input_event)) < 0)
		printf("error: ioctl %d\n",__LINE__);

		memset(&ev, 0, sizeof(struct input_event));
		ev.type = EV_REL;
		ev.code = REL_Y;
		ev.value = y;
		if(write(fd, &ev, sizeof(struct input_event)) < 0)
		printf("error: ioctl %d\n",__LINE__);
		memset(&ev, 0, sizeof(struct input_event));
		ev.type = EV_SYN;
		if(write(fd, &ev, sizeof(struct input_event)) < 0)
		printf("error: ioctl %d\n",__LINE__);
		usleep(15000);
		printf("\nWritten x:%d y:%d to uinput.Press CTRL-C to quit:",x,y);


	}
	/*
	 * Give userspace some time to read the events before we destroy the
	 * device with UI_DEV_DESTOY.
	 */
	sleep(1);
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	return 0;
}
