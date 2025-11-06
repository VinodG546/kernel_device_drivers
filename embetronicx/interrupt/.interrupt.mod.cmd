savedcmd_interrupt.mod := printf '%s\n'   interrupt.o | awk '!x[$$0]++ { print("./"$$0) }' > interrupt.mod
