FROM alpine:latest
RUN apk add --no-cache cmake clang lld ninja build-base
WORKDIR /workdir
