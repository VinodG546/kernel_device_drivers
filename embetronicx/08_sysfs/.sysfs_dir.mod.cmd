savedcmd_sysfs_dir.mod := printf '%s\n'   sysfs_dir.o | awk '!x[$$0]++ { print("./"$$0) }' > sysfs_dir.mod
