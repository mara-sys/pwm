#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>

#define dbmsg(fmt, args ...) printf("%s[%d]: "fmt"\n", __FUNCTION__, __LINE__,##args)	//__FILE__,


int fd_period = 0,fd_duty = 0,fd_enable = 0,duty_m = 0;

int pwm_setup(char *period,char *duty_ratio)
{
    int fd,ret;

    fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    if(fd < 0)
    {
        dbmsg("open export error\n");
        return -1;
    }
    if((access("/sys/class/pwm/pwmchip0/pwm0/",F_OK))==-1)
    {
        ret = write(fd, "0", strlen("0"));
        if(ret < 0)
        {
            dbmsg("creat pwm0 error\n");
            return -1;
        }else
            dbmsg("export pwm0 ok\n");
    }


    fd_period = open("/sys/class/pwm/pwmchip0/pwm0/period", O_RDWR);
    fd_duty = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_RDWR);
    fd_enable = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_RDWR);

    if((fd_period < 0)||(fd_duty < 0)||(fd_enable < 0))
    {
        dbmsg("open error\n");
        return -1;
    }

    ret = write(fd_period, period,strlen(period));
    if(ret < 0)
    {
        dbmsg("change period error\n");
        return -1;
    }else
    dbmsg("change period ok\n");

    ret = write(fd_duty, duty_ratio, strlen(duty_ratio));
    if(ret < 0)
    {
        dbmsg("change duty_cycle error\n");
        return -1;
    }else
    dbmsg("change duty_cycle ok\n");

    ret = write(fd_enable, "1", strlen("1"));
    if(ret < 0)
    {
        dbmsg("enable pwm0 error\n");
        return -1;
    }else
    dbmsg("enable pwm0 ok\n");

    return 0;
}

int main ( int argc, char *argv[] )
{
  if(argc != 3)
  {
    printf("请输入周期和正采样，单位为ns\n");
    return -1;
  }
  printf("%s---%s\n",argv[1],argv[2]);
  pwm_setup(argv[1],argv[2]);
  return 0;
}
