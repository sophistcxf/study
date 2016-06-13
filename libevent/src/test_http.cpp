/*************************************************************************
  > File Name: test_http.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 05:27:02 PM CST
 ************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
using namespace std;

#include "event2/http.h"
#include "event2/http_struct.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/keyvalq_struct.h"

extern int errno;

void http_request_handler(struct evhttp_request *req, void *arg) { 
  cout << "remote host: " << req->remote_host << endl;
  cout << "uri: " << req->uri << endl;
  cout << "host_cache: " << req->host_cache << endl;
  // traverse input header key-value
  struct evkeyvalq* input_headers = req->input_headers;
  for (struct evkeyval* header = input_headers->tqh_first; header; header = header->next.tqe_next)
    cout << "key: " << header->key << ", value: " << header->value << endl;
  struct evbuffer *databuf = evbuffer_new(); 
  evbuffer_add_printf(databuf, "hello world");
  evhttp_send_reply_start(req,200,"OK");
  evhttp_send_reply_chunk(req,databuf);
  evhttp_send_reply_end(req);
	evbuffer_free(databuf); 
}

int bind_server_socket(int port, int backlog)
{
	int flags = 0;

	int nfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (nfd < 0) {
		return -1; 
	}   

	const int one = 1;
	int r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));
	if (r < 0) {
		goto err;
	}   

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
	if (r < 0) {
		goto err;
	}   

	r = listen(nfd, backlog);
	if (r < 0) {
		goto err;
	}   

	if ((flags = fcntl(nfd, F_GETFL, 0)) < 0 
			|| fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0) {
		goto err;
	}   

	return nfd;        

err:
	close(nfd);
	return -1; 
}

void* thread_cb1(void* param)
{
  struct event_base *base = event_base_new(); 
  struct evhttp *httpd = evhttp_new(base); 
  evhttp_bind_socket(httpd,"0.0.0.0",9077); 
  evhttp_set_gencb(httpd, http_request_handler, NULL); 
  event_base_dispatch(base); 
}

int main() { 
	pthread_t pt[1];
	pthread_create(&pt[0], NULL, thread_cb1, NULL);
	for (size_t i = 0; i < sizeof(pt) / sizeof(pthread_t); ++i)
		pthread_join(pt[i], NULL);	
	return 0;
}
