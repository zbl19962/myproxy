
#ifndef __MP_CONFIG_H__
#define __MP_CONFIG_H__

#include "json_parser.h"
#include "ctnr_impl.h"



/* 
 * proxy config structures 
 */
class AUTH_BLOCK  {
public:
  std::string usr;
  std::string pwd;
} ;

/* io type of data nodes */
enum ioTypes {
  it_read,
  it_write,
  it_both
} ;

auto iot2str = [&](auto &iot) {
  return iot==it_read?"ioType 'read'" : 
    iot==it_write?"ioType 'write'" : 
    iot==it_both?"ioType 'both'" :
    "ioType error";
} ;

class MAPPING_INFO  {
public:
  std::string dataNode; /* target data node name */
  uint8_t io_type ; /* 0 read, 1 write, 2 both */
} ;

/* distribution rules */
enum tRules {
  t_dummy, /* dummy rule */
  t_modN, /* get data node by MOD(data-node-count) */
  t_rangeMap, /* column value range and data node mapping */
  t_maxRules,
} ;

class TABLE_INFO {
public:
  std::string name ; /* target table name */
  std::vector<MAPPING_INFO*> map_list ;
#if 0
  std::vector<char*> priKeys ; /* primary key list */
  int num_priKeys;
  uint8_t rule ; /* the distribution rule id */
#endif
} ;
/*
 * schema block definition
 */
class SCHEMA_BLOCK {
public:
  std::string name; /* the schema name */
  std::vector<AUTH_BLOCK*> auth_list ;
  std::vector<TABLE_INFO*> table_list ;
} ;
/* 
 * data node definition 
 */
class DATA_NODE {
public:
  std::string name;
  uint32_t address;
  uint16_t port ;
  std::string schema;
  AUTH_BLOCK auth ;
} ;
/*
 * global settings
 */
class GLOBAL_SETTINGS {
public:
  size_t szCachePool ;
  size_t numDnGrp ;
  size_t szThreadPool ;
  std::string bndAddr ;
  int listenPort ;
  int idleSeconds ;
} ;

class mp_cfg : public SimpleJsonParser 
{
public:
  mp_cfg(void);
  ~mp_cfg(void);

protected:
  std::string m_confFile ;

public:
  /*
   * global settings item
   */
  GLOBAL_SETTINGS m_globSettings ;
  /* 
   * the data node list 
   */
  std::vector<DATA_NODE*> m_dataNodes ;
  /* 
   * the schema list 
   */
  std::vector<SCHEMA_BLOCK*> m_schemas;
  /*
   * the sharding column list
   */
  safeShardingColumnList m_shds ;

  /* the global id list */
  safeGlobalIdColumnList m_gidConf ;

public:
  int read_conf(const char*);
  int reload(void);

  void dump(void);

  char* get_pwd(char*,char*);

  bool is_db_exists(char*);

  DATA_NODE* get_dataNode(uint16_t);
  int get_dataNode(char*);

  SCHEMA_BLOCK* get_schema(uint16_t);

  size_t get_num_dataNodes(void);

  size_t get_num_schemas(void);

  SCHEMA_BLOCK* get_schema(const char*);

  TABLE_INFO* get_table(SCHEMA_BLOCK*,
    uint16_t);
    
  TABLE_INFO* get_table(SCHEMA_BLOCK *sch,
    const char *tbl);

  size_t get_num_tables(SCHEMA_BLOCK*);

  size_t get_cache_pool_size(void) const ;

  size_t get_dn_group_count(void) const ;

  size_t get_thread_pool_size(void) const ;

  char* get_bind_address(void) const;

  int get_listen_port(void) const;

  int get_idle_seconds(void) const;

protected:

  int parse(std::string&);

  int parse_global_settings(void);

  int parse_data_nodes(void);

  int parse_schemas(void);

  int parse_mapping_list(jsonKV_t*, TABLE_INFO*);

  int parse_shardingkey_list(jsonKV_t*, char*, char*);

  int parse_globalid_list(jsonKV_t*, char*, char*);

  uint32_t parse_ipv4(std::string&);

  void reset(void);

  int check_duplicate(const char*,std::vector<struct tJsonParserKeyVal*>&,size_t);

  int save_range_maps(jsonKV_t*,SHARDING_EXTRA&);
} ;

#endif /* __MP_CONFIG_H__*/


