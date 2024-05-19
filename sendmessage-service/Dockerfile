FROM archlinux:latest
COPY src /src
COPY docker-dep /docker-dep
WORKDIR /
RUN pacman -Syu --noconfirm \
    && pacman -Sy --noconfirm clang base-devel \
    && pacman -U --noconfirm /docker-dep/*.pkg*
RUN cd src/sendmessage-service \
    && if [ ! -d build ]; then mkdir build; fi && cd build \
    && qmake .. && make \
    && mv sendmessage-service /
CMD ./sendmessage-service
