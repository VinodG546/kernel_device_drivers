savedcmd_work_isr.mod := printf '%s\n'   work_isr.o | awk '!x[$$0]++ { print("./"$$0) }' > work_isr.mod
