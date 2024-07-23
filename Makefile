# contrib/graphql_proxy/Makefile

MODULE_big = graphql_proxy
OBJS = \
  $(WIN32RES) \
  http/http_parser.o \
  http/input_parsing.o \
  io_uring/event_handling.o \
  multiple_user_access.o  \
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
