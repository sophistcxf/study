/*************************************************************************
  > File Name: test_http.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 05:27:02 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "event2/http.h"
#include "event2/http_struct.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/keyvalq_struct.h"

void http_request_handler(struct evhttp_request *req, void *arg) { 
  cout << "remote host: " << req->remote_host << endl;
  cout << "uri: " << req->uri << endl;
  cout << "host_cache: " << req->host_cache << endl;
  struct evkeyvalq* input_headers = req->input_headers;
  for (struct evkeyval* header = input_headers->tqh_first; header; header = header->next.tqe_next) {
    cout << "key: " << header->key << ", value: " << header->value << endl;
  }
  struct evbuffer *databuf = evbuffer_new(); 
  evbuffer_add_printf(databuf, "hello world");
  evhttp_send_reply_start(req,200,"OK");
  evhttp_send_reply_chunk(req,databuf);
  evhttp_send_reply_end(req);
  evbuffer_free(databuf); 
}

int main() { 
  struct event_base *base = event_base_new(); 
  struct evhttp *httpd = evhttp_new(base); 
  evhttp_bind_socket(httpd,"0.0.0.0",9077); 
  evhttp_set_gencb(httpd, http_request_handler, NULL); 
  event_base_dispatch(base); 
}
