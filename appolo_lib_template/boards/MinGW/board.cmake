SET (TARGET_ARCH       "MinGW")
INCLUDE (${TOPDIR}/arch/${TARGET_ARCH}/arch.cmake)
SET (TARGET_C_FLAGS "${TARGET_C_FLAGS} -DAPP_COPY_A")