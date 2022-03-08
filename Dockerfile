FROM ubuntu:focal as builder

WORKDIR /build
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y gcc make pkg-config libev-dev libx11-dev libxi-dev

COPY Makefile xmousepasteblock.c ./

RUN make && make install

FROM scratch as export
COPY --from=builder /usr/bin/xmousepasteblock .

ENTRYPOINT []
