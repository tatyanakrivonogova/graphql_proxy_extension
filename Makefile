# contrib/graphql_proxy/Makefile

MODULE_big = graphql_proxy
OBJS = \
  $(WIN32RES) \
  hashmap/map.o \
  http/http_parser.o \
  http/input_parsing.o \
  io_uring/event_handling.o \
  io_uring/multiple_user_access.o  \
  postgres_connect/postgres_connect.o \
  json_graphql/config/config.o \
  schema/schema.o \
  handlers/utils.o \
  handlers/handle_operation.o \
  schema/schema_converting.o \
  schema/operation_converting.o \
  json_graphql/cJSON.o \
  json_graphql/resolvers/resolverLoader.o \
  libgraphqlparser/schema_to_json.o \
  query_parser/query_parser.o \
  response_creator/response_creator.o \
  graphql_proxy.o

EXTENSION = graphql_proxy
DATA = graphql_proxy--1.0.sql

SHLIB_LINK += -lpq -luring -fPIC -lgraphqlparser

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/graphql_proxy
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
