FROM ubuntu:22.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn/g" /etc/apt/sources.list && \
    apt-get update && apt-get -y dist-upgrade && \
    apt-get update && apt-get install -y lib32z1 xinetd build-essential && \
    apt-get install -y openssl && \
    apt-get install -y sqlite3 && \
    useradd -m ctf
WORKDIR /home/ctf


COPY ./ctf.xinetd /etc/xinetd.d/ctf
COPY ./client /home/ctf
COPY ./safebox /home/ctf
COPY ./serve /home/ctf
COPY ./Nep_CTF_FLAG_FOUR /home/ctf
COPY ./Nep_CTF_FLAG_ONE /home/ctf
COPY ./Nep_CTF_FLAG_TWO /home/ctf
COPY ./Nep_CTF_FLAG_THREE /home/ctf
COPY ./start.sh /home/ctf

RUN chmod +x /home/ctf/* && \
    chown -R ctf /home/ctf && \
    chmod -R 750 /home/ctf && \
    touch /home/ctf/* && \
    chmod -R 777 /home/ctf/Nep_CTF_FLAG_FOUR && \
    chmod -R 777 /home/ctf/Nep_CTF_FLAG_ONE && \
    chmod -R 777 /home/ctf/Nep_CTF_FLAG_TWO && \
    chmod -R 777 /home/ctf/Nep_CTF_FLAG_THREE && \
    echo "Blocked by ctf_xinetd" > /etc/banner_fail && \
    echo 'ctf - nproc 1500' >>/etc/security/limits.conf

CMD exec /bin/bash -c "/etc/init.d/xinetd start; trap : TERM INT; sleep infinity & wait"
