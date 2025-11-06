savedcmd_dev_ioctl.mod := printf '%s\n'   dev_ioctl.o | awk '!x[$$0]++ { print("./"$$0) }' > dev_ioctl.mod
