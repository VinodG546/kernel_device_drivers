savedcmd_device_create.mod := printf '%s\n'   device_create.o | awk '!x[$$0]++ { print("./"$$0) }' > device_create.mod
