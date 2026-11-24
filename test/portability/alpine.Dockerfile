FROM alpine:3.24
RUN apk add --no-cache cmake clang lld ninja build-base
WORKDIR /workdir
