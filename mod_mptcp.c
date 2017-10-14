/*
   mod_mptcp 0.1
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
#include "http_protocol.h"
#include "http_log.h"
#include "ap_mpm.h"
#include "ap_config.h"
#include "apr_strings.h"
#include <netinet/tcp.h>
#include <stdlib.h>
#include "ap_config.h"


#define MODULE_NAME "mod_mptcp"
#define MODULE_VERSION "0.1"
#define MPTCP_ENABLED 42

module AP_MODULE_DECLARE_DATA mptcp_module;



char* itoa(int, char* , int);

static int pre_connection(conn_rec *c, void* csd)
{
    int optval;
    int optlen;

    apr_os_sock_t *p_os_fd = apr_palloc( c->pool, sizeof(apr_os_sock_t) );
    apr_os_sock_get(p_os_fd, (apr_socket_t *)csd);

    char buffer[5];   

    if (getsockopt(*p_os_fd, SOL_TCP, MPTCP_ENABLED, &optval, &optlen) != -1) {
    	sprintf(buffer, "%d", optval);
    } else {
	ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "mod_mptcp: You are using this plugin without a MPTCP Linux Kernel.");
	buffer[0] = '0';
	buffer[1] = '\0';
    }

    apr_table_set(c->notes, "MPTCP_ENABLED", buffer);
    
    //ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "MPTCP Status: %s", buffer); 

   return OK;
}

static int mptcp_env_handler(request_rec *r)
{


    const char* mptcp_status = apr_table_get(r->connection->notes, "MPTCP_ENABLED");
    //ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "MPTCP Status2: %s", mptcp_status); 
    apr_table_set(r->subprocess_env, "MPTCP_ENABLED", mptcp_status);


    return OK;
}

static void child_init (apr_pool_t *p, server_rec *s)
{
    ap_add_version_component(p, MODULE_NAME "/" MODULE_VERSION);
}


static void register_hooks(apr_pool_t *p)
{
    ap_hook_pre_connection(pre_connection, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_post_read_request(mptcp_env_handler, NULL, NULL, APR_HOOK_REALLY_FIRST);
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
