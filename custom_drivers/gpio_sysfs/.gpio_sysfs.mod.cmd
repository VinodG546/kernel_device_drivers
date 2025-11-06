savedcmd_gpio_sysfs.mod := printf '%s\n'   gpio_sysfs.o | awk '!x[$$0]++ { print("./"$$0) }' > gpio_sysfs.mod
