/*
   mod_mptcp 0.2
   Copyright (C) 2017 Markus Jungbluth

   Author: Markus Jungbluth

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include "httpd.h"
#include "http_config.h"
#include "http_connection.h"
#include "http_log.h"
#include "ap_mpm.h"
#include "apr_strings.h"
#include <netinet/tcp.h>


#define MODULE_NAME "mod_mptcp"
#define MODULE_VERSION "0.1"
#define MPTCP_ENABLED 42

module AP_MODULE_DECLARE_DATA mptcp_module;

static int server_limit, thread_limit;



static int pre_connection(conn_rec *c, void* csd)
{

    int optval;
    int optlen;

     struct tcp_info info;
     int infoLen = sizeof(info);

    apr_os_sock_t *p_os_fd = apr_palloc( c->pool, sizeof(apr_os_sock_t) );

    /* Retrieve the new connection's socket number */
    apr_os_sock_get(p_os_fd, (apr_socket_t *)csd);
    //getsockopt(*p_os_fd, SOL_TCP, TCP_INFO, (void *)&info, (socklen_t *)&infoLen);
    getsockopt(*p_os_fd, SOL_TCP, MPTCP_ENABLED, &optval, &optlen);
    ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "MPTCP Status: %u", optval); 

    /*ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "MPTCP %u %u %u %u %u %u %u %u %u %u %u\n", info.tcpi_snd_cwnd, info.tcpi_snd_ssthresh,
   info.tcpi_unacked, info.tcpi_lost, info.tcpi_retrans, info.tcpi_total_retrans,
   info.tcpi_rtt, info.tcpi_rttvar, info.tcpi_rcv_rtt, info.tcpi_rto, info.tcpi_options);*/

   return OK;
}


static void child_init (apr_pool_t *p, server_rec *s)
{
    ap_add_version_component(p, MODULE_NAME "/" MODULE_VERSION);
}


static void register_hooks(apr_pool_t *p)
{
    ap_hook_pre_connection(pre_connection, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(child_init, NULL, NULL, APR_HOOK_MIDDLE);    
}

module AP_MODULE_DECLARE_DATA mptcp_module = {
    STANDARD20_MODULE_STUFF,
    NULL,			/* creadte per-dir config structures */
    NULL,			/* merge  per-dir    config structures */
    NULL,		/* create per-server config structures */
    NULL,			/* merge  per-server config structures */
    NULL,		/* table of config file commands       */
    register_hooks
};
