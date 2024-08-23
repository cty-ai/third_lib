#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <event2/http.h>
#include <evhttp.h>

void socket_read_cb(bufferevent *bev, void *arg)  
{  
    char msg[4096];  
    size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 );  
    msg[len] = '\0';  
    printf("server read the data %s\n", msg);  
    char reply[] = "I has read your data";  
    bufferevent_write(bev, reply, strlen(reply) );  
}  

void socket_event_cb(bufferevent *bev, short events, void *arg)  
{  
    if (events & BEV_EVENT_EOF)  
        printf("connection closed\n");  
    else if (events & BEV_EVENT_ERROR)  
        printf("some other error\n");  
    //这将自动close套接字和free读写缓冲区  
    bufferevent_free(bev);  
}  
    
//一个新客户端连接上服务器了  
//当此函数被调用时，libevent已经帮我们accept了这个客户端。该客户端的
//文件描述符为fd
void listener_cb(evconnlistener *listener, evutil_socket_t fd,  
                 struct sockaddr *sock, int socklen, void *arg)  
{  
    printf("accept a client %d\n", fd);  
    event_base *base = (event_base*)arg;  
    //为这个客户端分配一个bufferevent  
    bufferevent *bev =  bufferevent_socket_new(base, fd,  
                                               BEV_OPT_CLOSE_ON_FREE);  
    bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);  
    bufferevent_enable(bev, EV_READ | EV_PERSIST);  
}  


void EvTcpServerInit(int port)
{
    struct sockaddr_in sin;  
    memset(&sin, 0, sizeof(struct sockaddr_in));  
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(port);  
    event_base *base = event_base_new();  
    evconnlistener *listener  
            = evconnlistener_new_bind(base, listener_cb, base,  
                                      LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,  
                                      10, (struct sockaddr*)&sin,  
                                      sizeof(struct sockaddr_in));  
    event_base_dispatch(base);  
    evconnlistener_free(listener);  
    event_base_free(base);  
}



void httpd_handler(evhttp_request *req, void *arg)
{
    printf("HTTP request received.\n");
    //读取HTTP请求的URI
    const char *uri = evhttp_request_uri(req);
    printf("URI: %s\n", uri);
 
    //读取HTTP请求的头部信息
    // struct evkeyvalq *headers = evhttp_request_get_input_headers(req);
    // struct evkeyval *header;
    // while ((header = evhttp_header_find(headers, "Host")) != NULL) {
    //     printf("Host: %s\n", header->value);
    // }

     //判断HTTP请求的类型
     //if (evhttp_request_get_command (req) == EVHTTP_REQ_GET)

    //读取HTTP请求的正文
    struct evbuffer *inbuf = evhttp_request_get_input_buffer(req);
    size_t len = evbuffer_get_length(inbuf);
    char *data = (char*) malloc(len+1);
    evbuffer_copyout(inbuf, data, len);
    data[len] = '\0';
    printf("HTTP request body: %s\n", data);
 
    //构造HTTP响应
    evbuffer *outbuf = evbuffer_new();
    // evbuffer_add_printf(outbuf, "HTTP/1.1 200 OK\r\n");
    // evbuffer_add_printf(outbuf, "Content-Type: text/plain\r\n");
    // evbuffer_add_printf(outbuf, "Content-Length: %ld\r\n", len);
    // evbuffer_add_printf(outbuf, "\r\n");
    evbuffer_add(outbuf, data, len);
 
    //发送HTTP响应
    evhttp_send_reply(req, HTTP_OK, "OK", outbuf);

    //释放资源
    evbuffer_free(outbuf);
    free(data);
}

void login_handler(evhttp_request *req, void *arg)
{


}

int EvHttpServerInit(int port){
    /* 使用libevent创建HTTP Server */
 
    //初始化event API
    event_init();
 
    //创建一个http server
    struct evhttp *httpd;
    
    httpd = evhttp_start("0.0.0.0", port);
 
    //evhttp_set_gencb(httpd, httpd_handler, NULL);
    evhttp_set_timeout(httpd, 60);
 
    //也可以为特定的URI指定callback
    evhttp_set_cb(httpd, "/hi", httpd_handler, NULL);
    evhttp_set_cb(httpd, "/login", login_handler, NULL);
 
    //循环处理events
    event_dispatch();
 
    evhttp_free(httpd);
 
    return 0;
}

#include <openssl/ssl.h>
#include <openssl/err.h>

static void server_setup_certs (SSL_CTX *ctx,
        const char *certificate_chain,
        const char *private_key)
{ 
    printf ("Loading certificate chain from '%s'\n"
            "and private key from '%s'\n",
            certificate_chain, private_key);

    if (1 != SSL_CTX_use_certificate_chain_file (ctx, certificate_chain))
            fprintf (stderr, "Error loading certificate chain file\n");
    if (1 != SSL_CTX_use_PrivateKey_file (ctx, private_key, SSL_FILETYPE_PEM))
            fprintf (stderr, "Error loading private key file\n");
    if (1 != SSL_CTX_check_private_key (ctx))
            fprintf (stderr, "Private key does not match the certificate public key\n");
}

static struct bufferevent* bevcb (struct event_base *base, void *arg)
{ 
    struct bufferevent* r;
    SSL_CTX *ctx = (SSL_CTX *) arg;

    r = bufferevent_openssl_socket_new (base,
            -1,
            SSL_new (ctx),
            BUFFEREVENT_SSL_ACCEPTING,
            BEV_OPT_CLOSE_ON_FREE);
    return r;
}

int EvHttpsServerInit(int port){
    struct event_base *base;
    struct evhttp *http;
    struct evhttp_bound_socket *handle;


    /* 初始化SSL库 */
    SSL_load_error_strings ();
    SSL_library_init ();
    
    base = event_base_new ();
    if (! base)
    { 
        fprintf (stderr, "Couldn't create an event_base: exiting\n");
        return 1;
    }

    /* 创建一个 evhttp 句柄，去处理用户端的requests请求 */
    http = evhttp_new (base);
    if (! http)
    { fprintf (stderr, "couldn't create evhttp. Exiting.\n");
        return 1;
    }

    /* 创建SSL上下文环境 ，可以理解为 SSL句柄 */
    SSL_CTX *ctx = SSL_CTX_new (SSLv23_server_method ());
    SSL_CTX_set_options (ctx,
            SSL_OP_SINGLE_DH_USE |
            SSL_OP_SINGLE_ECDH_USE |
            SSL_OP_NO_SSLv2);

    /* 选择服务器证书 和 服务器私钥. */
    const char *certificate_chain = "server.pem";
    const char *private_key = "server.key";
    /* 设置服务器证书 和 服务器私钥 到 
     OPENSSL ctx上下文句柄中 */
    server_setup_certs (ctx, certificate_chain, private_key);

    /* 
        使我们创建好的evhttp句柄 支持 SSL加密
        实际上，加密的动作和解密的动作都已经帮
        我们自动完成，我们拿到的数据就已经解密之后的
    */
    evhttp_set_bevcb (http, bevcb, ctx);

    /* 设置http回调函数 */
    //默认回调
    //专属uri路径回调
    evhttp_set_cb(http, "/hi", httpd_handler, NULL);

    /* 设置监听IP和端口 */
    handle = evhttp_bind_socket_with_handle (http, "0.0.0.0", port);
    if (! handle)
    { 
        fprintf (stderr, "couldn't bind to port %d. Exiting.\n",(int) port);
        return 1;
    }

    
    event_base_dispatch (base);

 
    return 0;
}

