#include "postgres.h"

#include "parser/scanner.h"
#include "nodes/pg_list.h"
#include "nodes/parsenodes.h"
#include "nodes/value.h"
#include "mgr_gram.h"

#define PG_KEYWORD(a,b,c) {a,b,c},

const ScanKeyword ManagerKeywords[] = {
PG_KEYWORD("acl", ACL, UNRESERVED_KEYWORD)
PG_KEYWORD("add", ADD_P, UNRESERVED_KEYWORD)
PG_KEYWORD("agent", AGENT, UNRESERVED_KEYWORD)
PG_KEYWORD("all", ALL, RESERVED_KEYWORD)
PG_KEYWORD("alter", ALTER, UNRESERVED_KEYWORD)
PG_KEYWORD("append", APPEND, UNRESERVED_KEYWORD)
PG_KEYWORD("check_password", CHECK_PASSWORD, UNRESERVED_KEYWORD)
PG_KEYWORD("check_user", CHECK_USER, UNRESERVED_KEYWORD)
PG_KEYWORD("clean", CLEAN, UNRESERVED_KEYWORD)
PG_KEYWORD("cluster", CLUSTER, UNRESERVED_KEYWORD)
/* PG_KEYWORD("config", CONFIG, UNRESERVED_KEYWORD) */
PG_KEYWORD("coordinator", COORDINATOR, RESERVED_KEYWORD)
PG_KEYWORD("create", CREATE, UNRESERVED_KEYWORD)
PG_KEYWORD("datanode", DATANODE, RESERVED_KEYWORD)
PG_KEYWORD("deploy", DEPLOY, UNRESERVED_KEYWORD)
PG_KEYWORD("drop", DROP, UNRESERVED_KEYWORD)
PG_KEYWORD("exists", EXISTS, UNRESERVED_KEYWORD)
PG_KEYWORD("extension", EXTENSION, UNRESERVED_KEYWORD)
PG_KEYWORD("extra", EXTRA, UNRESERVED_KEYWORD)
PG_KEYWORD("f", F, UNRESERVED_KEYWORD)
PG_KEYWORD("failover", FAILOVER, UNRESERVED_KEYWORD)
PG_KEYWORD("false", FALSE_P, RESERVED_KEYWORD)
PG_KEYWORD("fast", FAST, UNRESERVED_KEYWORD)
PG_KEYWORD("flush", FLUSH, UNRESERVED_KEYWORD)
PG_KEYWORD("force", FORCE, UNRESERVED_KEYWORD)
PG_KEYWORD("from", FROM, UNRESERVED_KEYWORD)
PG_KEYWORD("get_agtm_node_topology", GET_AGTM_NODE_TOPOLOGY, UNRESERVED_KEYWORD)
PG_KEYWORD("get_alarm_info_asc", GET_ALARM_INFO_ASC, UNRESERVED_KEYWORD)
PG_KEYWORD("get_alarm_info_count", GET_ALARM_INFO_COUNT, UNRESERVED_KEYWORD)
PG_KEYWORD("get_alarm_info_desc", GET_ALARM_INFO_DESC, UNRESERVED_KEYWORD)
PG_KEYWORD("get_all_nodename_in_spec_host", GET_ALL_NODENAME_IN_SPEC_HOST, UNRESERVED_KEYWORD)
PG_KEYWORD("get_cluster_fouritem", GET_CLUSTER_FOURITEM, UNRESERVED_KEYWORD)
PG_KEYWORD("get_cluster_headpage_line", GET_CLUSTER_HEADPAGE_LINE, UNRESERVED_KEYWORD)
PG_KEYWORD("get_cluster_summary", GET_CLUSTER_SUMMARY, UNRESERVED_KEYWORD)
PG_KEYWORD("get_coordinator_node_topology", GET_COORDINATOR_NODE_TOPOLOGY, UNRESERVED_KEYWORD)
PG_KEYWORD("get_database_summary", GET_DATABASE_SUMMARY, UNRESERVED_KEYWORD)
PG_KEYWORD("get_database_tps_qps", GET_DATABASE_TPS_QPS, UNRESERVED_KEYWORD)
PG_KEYWORD("get_database_tps_qps_interval_time", GET_DATABASE_TPS_QPS_INTERVAL_TIME, UNRESERVED_KEYWORD)
PG_KEYWORD("get_datanode_node_topology", GET_DATANODE_NODE_TOPOLOGY, UNRESERVED_KEYWORD)
PG_KEYWORD("get_db_threshold_all_type", GET_DB_THRESHOLD_ALL_TYPE, UNRESERVED_KEYWORD)
PG_KEYWORD("get_host_history_usage", GET_HOST_HISTORY_USAGE, UNRESERVED_KEYWORD)
PG_KEYWORD("get_host_history_usage_by_time_period", GET_HOST_HISTORY_USAGE_BY_TIME_PERIOD, UNRESERVED_KEYWORD)
PG_KEYWORD("get_host_list_all", GET_HOST_LIST_ALL, UNRESERVED_KEYWORD)
PG_KEYWORD("get_host_list_spec", GET_HOST_LIST_SPEC, UNRESERVED_KEYWORD)
PG_KEYWORD("get_slowlog", GET_SLOWLOG, UNRESERVED_KEYWORD)
PG_KEYWORD("get_slowlog_count", GET_SLOWLOG_COUNT, UNRESERVED_KEYWORD)
PG_KEYWORD("get_threshold_all_type", GET_THRESHOLD_ALL_TYPE, UNRESERVED_KEYWORD)
PG_KEYWORD("get_threshold_type", GET_THRESHOLD_TYPE, UNRESERVED_KEYWORD)
PG_KEYWORD("get_user_info", GET_USER_INFO, UNRESERVED_KEYWORD)
PG_KEYWORD("grant", GRANT, UNRESERVED_KEYWORD)
PG_KEYWORD("gtm", GTM, UNRESERVED_KEYWORD)
PG_KEYWORD("ha", HA, UNRESERVED_KEYWORD)
PG_KEYWORD("hba", HBA, UNRESERVED_KEYWORD)
PG_KEYWORD("host", HOST, UNRESERVED_KEYWORD)
PG_KEYWORD("i", I, UNRESERVED_KEYWORD)
PG_KEYWORD("if", IF_P, UNRESERVED_KEYWORD)
PG_KEYWORD("immediate", IMMEDIATE, UNRESERVED_KEYWORD)
PG_KEYWORD("init", INIT, UNRESERVED_KEYWORD)
PG_KEYWORD("item", ITEM, UNRESERVED_KEYWORD)
PG_KEYWORD("job", JOB, UNRESERVED_KEYWORD)
PG_KEYWORD("list", LIST, UNRESERVED_KEYWORD)
PG_KEYWORD("master", MASTER, UNRESERVED_KEYWORD)
PG_KEYWORD("mode", MODE, UNRESERVED_KEYWORD)
PG_KEYWORD("monitor", MONITOR, UNRESERVED_KEYWORD)
PG_KEYWORD("monitor_databasetps_func_by_time_period", MONITOR_DATABASETPS_FUNC_BY_TIME_PERIOD, UNRESERVED_KEYWORD)
PG_KEYWORD("node", NODE, UNRESERVED_KEYWORD)
PG_KEYWORD("not", NOT, RESERVED_KEYWORD)
PG_KEYWORD("off", OFF, UNRESERVED_KEYWORD)
PG_KEYWORD("on", ON, RESERVED_KEYWORD)
PG_KEYWORD("param", PARAM, UNRESERVED_KEYWORD)
PG_KEYWORD("password", PASSWORD, UNRESERVED_KEYWORD)
PG_KEYWORD("remove", REMOVE, UNRESERVED_KEYWORD)
PG_KEYWORD("reset", RESET, UNRESERVED_KEYWORD)
PG_KEYWORD("resolve_alarm", RESOLVE_ALARM, UNRESERVED_KEYWORD)
PG_KEYWORD("revoke", REVOKE, UNRESERVED_KEYWORD)
PG_KEYWORD("s", S, UNRESERVED_KEYWORD)
PG_KEYWORD("set", SET, UNRESERVED_KEYWORD)
PG_KEYWORD("show", SHOW, UNRESERVED_KEYWORD)
PG_KEYWORD("slave", SLAVE, UNRESERVED_KEYWORD)
PG_KEYWORD("smart", SMART, UNRESERVED_KEYWORD)
PG_KEYWORD("start", START, UNRESERVED_KEYWORD)
PG_KEYWORD("stop", STOP, UNRESERVED_KEYWORD)
PG_KEYWORD("to", TO, UNRESERVED_KEYWORD)
PG_KEYWORD("true", TRUE_P, RESERVED_KEYWORD)
PG_KEYWORD("update_password", UPDATE_PASSWORD, UNRESERVED_KEYWORD)
PG_KEYWORD("update_threshold_value", UPDATE_THRESHOLD_VALUE, UNRESERVED_KEYWORD)
PG_KEYWORD("update_user", UPDATE_USER, UNRESERVED_KEYWORD)
PG_KEYWORD("user", USER, UNRESERVED_KEYWORD)
};

const int NumManagerKeywords = lengthof(ManagerKeywords);
