#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kvm.h>

int main(int argc, char *argv[]){
    int kvm_fd = open("/dev/kvm", O_RDWR);
    if(kvm_fd < 0){
        printf("open kvm failed\n");
        return 1;
    }

    int ret = ioctl(kvm_fd, KVM_GET_API_VERSION, NULL);

    printf("KVM API Version=%d\n", ret);

    close(kvm_fd);

}