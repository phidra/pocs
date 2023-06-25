FROM ubuntu:20.04


# if we do not do this, some installation require human-interaction to set timezone :
ENV TZ=Europe/Paris
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt update
RUN apt install -y build-essential
RUN apt install -y python3 python3-venv python3-pip pipx
RUN apt install -y cmake
RUN apt install -y iputils-ping

# --force is necessary as we are root inside container :
RUN pipx install --force conan
RUN echo "export PATH=/root/.local/bin:$PATH" >> ~/.bashrc

