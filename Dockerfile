FROM ubuntu:18.04
ADD csd-identifier /usr/bin/csd-identifier

COPY shared_library/libboost_thread.so.1.65.1 /usr/lib/x86_64-linux-gnu/
COPY shared_library/libpthread.so.0 /lib/x86_64-linux-gnu/
COPY shared_library/libboost_system.so.1.65.1 /usr/lib/x86_64-linux-gnu/
COPY shared_library/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/
COPY shared_library/libgcc_s.so.1 /lib/x86_64-linux-gnu/
COPY shared_library/libc.so.6 /lib/x86_64-linux-gnu/
COPY shared_library/libm.so.6 /lib/x86_64-linux-gnu/
COPY shared_library/librt.so.1 /lib/x86_64-linux-gnu/

ENTRYPOINT ["/usr/bin/csd-identifier"]