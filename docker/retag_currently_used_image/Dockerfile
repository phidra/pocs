FROM busybox:latest

RUN echo "while true ; do date  ; sleep 1 ; done" > /loop.sh && chmod +x /loop.sh

CMD ["/bin/ash", "/loop.sh"]
